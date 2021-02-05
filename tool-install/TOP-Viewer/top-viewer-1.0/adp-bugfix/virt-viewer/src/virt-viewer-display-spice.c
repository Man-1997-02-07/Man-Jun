/*
 * Virt Viewer: A virtual machine console viewer
 *
 * Copyright (C) 2007-2009 Red Hat, Inc.
 * Copyright (C) 2009-2012 Daniel P. Berrange
 * Copyright (C) 2010 Marc-André Lureau
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Daniel P. Berrange <berrange@redhat.com>
 */

#include <config.h>

#include <math.h>
#include <spice-client-gtk.h>

#include <glib/gi18n.h>

#include "virt-viewer-util.h"
#include "virt-viewer-display-spice.h"
#include "virt-viewer-auth.h"

typedef enum {
    AUTO_RESIZE_ALWAYS,
    AUTO_RESIZE_FULLSCREEN,
    AUTO_RESIZE_NEVER,
} AutoResizeState;

struct _VirtViewerDisplaySpicePrivate {
    SpiceChannel *channel; /* weak reference */
    SpiceDisplay *display;
    AutoResizeState auto_resize;
    guint x;
    guint y;
};

G_DEFINE_TYPE_WITH_PRIVATE (VirtViewerDisplaySpice, virt_viewer_display_spice, VIRT_VIEWER_TYPE_DISPLAY)

static void virt_viewer_display_spice_send_keys(VirtViewerDisplay *display,
                                                const guint *keyvals,
                                                int nkeyvals);
static GdkPixbuf *virt_viewer_display_spice_get_pixbuf(VirtViewerDisplay *display);
static void virt_viewer_display_spice_release_cursor(VirtViewerDisplay *display);
static gboolean virt_viewer_display_spice_selectable(VirtViewerDisplay *display);
static void virt_viewer_display_spice_enable(VirtViewerDisplay *display);
static void virt_viewer_display_spice_disable(VirtViewerDisplay *display);

static void
virt_viewer_display_spice_class_init(VirtViewerDisplaySpiceClass *klass)
{
    VirtViewerDisplayClass *dclass = VIRT_VIEWER_DISPLAY_CLASS(klass);

    dclass->send_keys = virt_viewer_display_spice_send_keys;
    dclass->get_pixbuf = virt_viewer_display_spice_get_pixbuf;
    dclass->release_cursor = virt_viewer_display_spice_release_cursor;
    dclass->selectable = virt_viewer_display_spice_selectable;
    dclass->enable = virt_viewer_display_spice_enable;
    dclass->disable = virt_viewer_display_spice_disable;
}


static SpiceMainChannel*
get_main(VirtViewerDisplay *self)
{
    VirtViewerSessionSpice *session;

    session = VIRT_VIEWER_SESSION_SPICE(virt_viewer_display_get_session(self));

    return virt_viewer_session_spice_get_main_channel(session);
}


void virt_viewer_display_spice_change_resolution(VirtViewerDisplay *self, gint width, gint height)
{
    SpiceMainChannel *main_channel = get_main(self);
    guint nth;

    if (!main_channel)
        return;

    g_object_get(self, "nth-display", &nth, NULL);

    g_message("nth = %u\n", nth);

    spice_main_channel_update_display_enabled(main_channel, nth, TRUE, FALSE);
    spice_main_channel_update_display(main_channel, nth, 0, 0, width, height, TRUE);

    spice_main_channel_send_monitor_config(main_channel);
}


static void
virt_viewer_display_spice_monitor_geometry_changed(VirtViewerDisplaySpice *self)
{
    g_signal_emit_by_name(self, "monitor-geometry-changed", NULL);
}

static void update_enabled(VirtViewerDisplay *self, gboolean enabled, gboolean send)
{
    SpiceMainChannel *main_channel = get_main(self);
    guint nth;

    /* this may happen when finalizing */
    if (!main_channel)
        return;

    g_object_get(self, "nth-display", &nth, NULL);
    spice_main_channel_update_display_enabled(main_channel, nth, enabled, send);
}

static void
show_hint_changed(VirtViewerDisplay *self)
{
    /* just keep spice-gtk state up-to-date, but don't send change anything */
    update_enabled(self, virt_viewer_display_get_enabled(self), FALSE);
}

static void virt_viewer_display_spice_enable(VirtViewerDisplay *self)
{
    virt_viewer_display_set_enabled(self, TRUE);
    update_enabled(self, TRUE, TRUE);
}

static void virt_viewer_display_spice_disable(VirtViewerDisplay *self)
{
    virt_viewer_display_set_enabled(self, FALSE);
    update_enabled(self, FALSE, TRUE);
}

static void
virt_viewer_display_spice_init(VirtViewerDisplaySpice *self G_GNUC_UNUSED)
{
    self->priv = virt_viewer_display_spice_get_instance_private(self);
    self->priv->auto_resize = AUTO_RESIZE_ALWAYS;

    g_signal_connect(self, "notify::show-hint", G_CALLBACK(show_hint_changed), NULL);
}

static void
virt_viewer_display_spice_send_keys(VirtViewerDisplay *display,
                                    const guint *keyvals,
                                    int nkeyvals)
{
    VirtViewerDisplaySpice *self = VIRT_VIEWER_DISPLAY_SPICE(display);

    g_return_if_fail(self != NULL);
    g_return_if_fail(self->priv->display != NULL);

    spice_display_send_keys(self->priv->display, keyvals, nkeyvals, SPICE_DISPLAY_KEY_EVENT_CLICK);
}

static GdkPixbuf *
virt_viewer_display_spice_get_pixbuf(VirtViewerDisplay *display)
{
    VirtViewerDisplaySpice *self = VIRT_VIEWER_DISPLAY_SPICE(display);

    g_return_val_if_fail(self != NULL, NULL);
    g_return_val_if_fail(self->priv->display != NULL, NULL);

    return spice_display_get_pixbuf(self->priv->display);
}

static void
update_display_ready(VirtViewerDisplaySpice *self)
{
    gboolean ready;

    g_object_get(self->priv->display, "ready", &ready, NULL);

    virt_viewer_display_set_show_hint(VIRT_VIEWER_DISPLAY(self),
                                      VIRT_VIEWER_DISPLAY_SHOW_HINT_READY, ready);
}

static void
virt_viewer_display_spice_keyboard_grab(SpiceDisplay *display G_GNUC_UNUSED,
                                        int grabbed,
                                        VirtViewerDisplaySpice *self)
{
    if (grabbed)
        g_signal_emit_by_name(self, "display-keyboard-grab");
    else
        g_signal_emit_by_name(self, "display-keyboard-ungrab");
}


static void
virt_viewer_display_spice_mouse_grab(SpiceDisplay *display G_GNUC_UNUSED,
                                     int grabbed,
                                     VirtViewerDisplaySpice *self)
{
    if (grabbed)
        g_signal_emit_by_name(self, "display-pointer-grab");
    else
        g_signal_emit_by_name(self, "display-pointer-ungrab");
}


static void
virt_viewer_display_spice_size_allocate(VirtViewerDisplaySpice *self,
                                        GtkAllocation *allocation,
                                        gpointer data G_GNUC_UNUSED)
{
    GtkRequisition preferred;

    if (!virt_viewer_display_get_enabled(VIRT_VIEWER_DISPLAY(self)))
        return;

    /* ignore all allocations before the widget gets mapped to screen since we
     * only want to trigger guest resizing due to user actions
     */
    if (!gtk_widget_get_mapped(GTK_WIDGET(self)))
        return;

    /* when the window gets resized due to a change in zoom level, we don't want
     * to re-size the guest display.  So if we get an allocation event that
     * resizes the window to the size it already wants to be (based on desktop
     * size and zoom level), just return early
     */
    gtk_widget_get_preferred_size(GTK_WIDGET(self), NULL, &preferred);
    if (preferred.width == allocation->width
        && preferred.height == allocation->height) {
        return;
    }

    if (self->priv->auto_resize != AUTO_RESIZE_NEVER)
        virt_viewer_display_spice_monitor_geometry_changed(self);

    if (self->priv->auto_resize == AUTO_RESIZE_FULLSCREEN)
        self->priv->auto_resize = AUTO_RESIZE_NEVER;
}

static void
zoom_level_changed(VirtViewerDisplaySpice *self,
                   GParamSpec *pspec G_GNUC_UNUSED,
                   VirtViewerApp *app G_GNUC_UNUSED)
{
    if (self->priv->auto_resize != AUTO_RESIZE_NEVER)
        return;

    virt_viewer_display_spice_monitor_geometry_changed(self);
}

static void
enable_accel_changed(VirtViewerApp *app,
                     GParamSpec *pspec G_GNUC_UNUSED,
                     VirtViewerDisplaySpice *self)
{
    GtkAccelKey key = {0, 0, 0};
    if (virt_viewer_app_get_enable_accel(app))
        gtk_accel_map_lookup_entry("<virt-viewer>/view/release-cursor", &key);

    if (key.accel_key || key.accel_mods) {
        SpiceGrabSequence *seq = spice_grab_sequence_new(0, NULL);
        /* disable default grab sequence */
        spice_display_set_grab_keys(self->priv->display, seq);
        spice_grab_sequence_free(seq);
    } else {
        spice_display_set_grab_keys(self->priv->display, NULL);
    }
}

static void
disable_inputs_changed(VirtViewerApp *app,
                     GParamSpec *pspec G_GNUC_UNUSED,
                     VirtViewerDisplaySpice *self)
{
    gboolean disable_inputs = virt_viewer_app_get_disable_inputs(app);
    g_object_set(self->priv->display,"disable-inputs",disable_inputs,NULL);
}

static void
resize_guest_changed(VirtViewerApp *app,
                       GParamSpec *pspec G_GNUC_UNUSED,
                       VirtViewerDisplaySpice *self)
{
    gboolean resize_guest = virt_viewer_app_get_resize_guest(app);
    g_debug("resize_guest = %d\n", resize_guest);
    g_object_set(self->priv->display,"resize-guest",resize_guest,NULL);
}

static void
scaling_changed(VirtViewerApp *app,
                     GParamSpec *pspec G_GNUC_UNUSED,
                     VirtViewerDisplaySpice *self)
{
    gboolean scaling = virt_viewer_app_get_scaling(app);
    g_object_set(self->priv->display,"scaling",scaling,NULL);
}


static void
imageCompression_changed(VirtViewerApp *app,
                         GParamSpec *pspec G_GNUC_UNUSED,
                         VirtViewerDisplaySpice *self)
{
    gchar * image_compression = virt_viewer_app_get_preferred_compression(app);
    
    if (!g_strcmp0(image_compression, "auto-glz")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_AUTO_GLZ);
    } else if (!g_strcmp0(image_compression, "auto-lz")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_AUTO_LZ);
    } else if (!g_strcmp0(image_compression, "quic")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_QUIC);
    } else if (!g_strcmp0(image_compression, "glz")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_GLZ);
    } else if (!g_strcmp0(image_compression, "lz")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_LZ);
    } else if (!g_strcmp0(image_compression, "lz4")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_LZ4);
    } else if (!g_strcmp0(image_compression, "off")) {
        spice_display_channel_change_preferred_compression(self->priv->channel, SPICE_IMAGE_COMPRESSION_OFF);
    }
}

void
video_codec_changed(VirtViewerApp *app,
                GParamSpec *pspec G_GNUC_UNUSED,
                VirtViewerDisplaySpice *self)
{
    static GArray *preferred_codecs = NULL;
    static gint selected_codec;
    guint i;
    GError *err = NULL;

    gchar * video_codec = virt_viewer_app_get_video_codec(app);

    if (!g_strcmp0(video_codec, "mjpeg")) {
        selected_codec = SPICE_VIDEO_CODEC_TYPE_MJPEG;
    } else if (!g_strcmp0(video_codec, "vp8")) {
        selected_codec = SPICE_VIDEO_CODEC_TYPE_VP8;
    } else if (!g_strcmp0(video_codec, "vp9")) {
        selected_codec = SPICE_VIDEO_CODEC_TYPE_VP9;
    } else if (!g_strcmp0(video_codec, "h264")) {
        selected_codec = SPICE_VIDEO_CODEC_TYPE_H264;
    } 

    if (!preferred_codecs) {
        preferred_codecs = g_array_sized_new(FALSE, FALSE, sizeof(gint), 4);
        for (i = SPICE_VIDEO_CODEC_TYPE_MJPEG; i < SPICE_VIDEO_CODEC_TYPE_H265; i++) {
            g_array_append_val(preferred_codecs, i);
        }
    }

    for (i = 0; i < preferred_codecs->len && g_array_index(preferred_codecs, gint, i) != selected_codec; i++);
    g_assert(i < preferred_codecs->len);
    g_array_remove_index(preferred_codecs, i);
    g_array_prepend_val(preferred_codecs, selected_codec);


    if (!spice_channel_test_capability(SPICE_CHANNEL(self->priv->channel), SPICE_DISPLAY_CAP_PREF_VIDEO_CODEC_TYPE))
    {
        g_warning("SpiceChannel kind capability is unavailable");
        return;
    }

    if (!spice_display_channel_change_preferred_video_codec_types(SPICE_CHANNEL(self->priv->channel),
                                                                  (gint *) preferred_codecs->data,
                                                                  preferred_codecs->len, &err)) {
        g_warning("setting preferred video codecs failed: %s", err->message);
        g_error_free(err);
    } 

    
}

static void
fullscreen_changed(VirtViewerDisplaySpice *self,
                   GParamSpec *pspec G_GNUC_UNUSED,
                   VirtViewerApp *app)
{
    if (virt_viewer_display_get_fullscreen(VIRT_VIEWER_DISPLAY(self))) {
        gboolean auto_conf;
        g_object_get(app, "fullscreen", &auto_conf, NULL);
        if (auto_conf)
            self->priv->auto_resize = AUTO_RESIZE_NEVER;
        else
            self->priv->auto_resize = AUTO_RESIZE_FULLSCREEN;
    } else
        self->priv->auto_resize = AUTO_RESIZE_ALWAYS;
}

GtkWidget *
virt_viewer_display_spice_new(VirtViewerSessionSpice *session,
                              SpiceChannel *channel,
                              gint monitorid)
{
    VirtViewerDisplaySpice *self;
    VirtViewerApp *app;
    gint channelid;
    SpiceSession *s;

    g_return_val_if_fail(SPICE_IS_DISPLAY_CHANNEL(channel), NULL);

    g_object_get(channel, "channel-id", &channelid, NULL);
    if (channelid != 0 && monitorid != 0) {
        g_warning("Unsupported graphics configuration:\n"
                  "spice-gtk only supports multiple graphics channels if they are single-head");
        return NULL;
    }

    self = g_object_new(VIRT_VIEWER_TYPE_DISPLAY_SPICE,
                        "session", session,
                        // either monitorid is always 0 or channelid
                        // is, we can't have display (0, 2) and (2, 0)
                        // for example
                        "nth-display", channelid + monitorid,
                        NULL);
    self->priv->channel = channel;

    g_object_get(session, "spice-session", &s, NULL);
    self->priv->display = spice_display_new_with_monitor(s, channelid, monitorid);
    g_object_unref(s);

    virt_viewer_signal_connect_object(self->priv->display, "notify::ready",
                                      G_CALLBACK(update_display_ready), self,
                                      G_CONNECT_SWAPPED);
    update_display_ready(self);

    gtk_container_add(GTK_CONTAINER(self), GTK_WIDGET(self->priv->display));
    gtk_widget_show(GTK_WIDGET(self->priv->display));
    g_object_set(self->priv->display,
                 "grab-keyboard", TRUE,
                 "grab-mouse", TRUE,
                 NULL);

    virt_viewer_signal_connect_object(self->priv->display, "keyboard-grab",
                                      G_CALLBACK(virt_viewer_display_spice_keyboard_grab), self, 0);
    virt_viewer_signal_connect_object(self->priv->display, "mouse-grab",
                                      G_CALLBACK(virt_viewer_display_spice_mouse_grab), self, 0);
    virt_viewer_signal_connect_object(self, "size-allocate",
                                      G_CALLBACK(virt_viewer_display_spice_size_allocate), self, 0);


    app = virt_viewer_session_get_app(VIRT_VIEWER_SESSION(session));
    virt_viewer_signal_connect_object(app, "notify::enable-accel",
                                      G_CALLBACK(enable_accel_changed), self, 0);
    virt_viewer_signal_connect_object(app, "notify::disable-inputs",
                                      G_CALLBACK(disable_inputs_changed), self, 0);
    virt_viewer_signal_connect_object(app, "notify::resize-guest",
                                      G_CALLBACK(resize_guest_changed), self, 0);
    virt_viewer_signal_connect_object(app, "notify::scaling",
                                      G_CALLBACK(scaling_changed), self, 0);
    
    virt_viewer_signal_connect_object(app, "notify::preferred-compression",
                                      G_CALLBACK(imageCompression_changed), self, 0);
    virt_viewer_signal_connect_object(app, "notify::video-codec",
                                      G_CALLBACK(video_codec_changed), self, 0);
    virt_viewer_signal_connect_object(self, "notify::fullscreen",
                                      G_CALLBACK(fullscreen_changed), app, 0);
    virt_viewer_signal_connect_object(self, "notify::zoom-level",
                                      G_CALLBACK(zoom_level_changed), app, 0);
    fullscreen_changed(self, NULL, app);
    enable_accel_changed(app, NULL, self);
    disable_inputs_changed(app,NULL,self);
    resize_guest_changed(app,NULL,self);
    scaling_changed(app,NULL,self);
    imageCompression_changed(app, NULL, self);
    //video_codec_changed(app,NULL,self);

    return GTK_WIDGET(self);
}

static void
virt_viewer_display_spice_release_cursor(VirtViewerDisplay *display)
{
    VirtViewerDisplaySpice *self = VIRT_VIEWER_DISPLAY_SPICE(display);

    spice_display_mouse_ungrab(self->priv->display);
}

static gboolean
virt_viewer_display_spice_selectable(VirtViewerDisplay *self)
{
    gboolean agent_connected;
    SpiceMainChannel *mainc;

    mainc = get_main(self);
    g_object_get(mainc,
                 "agent-connected", &agent_connected,
                 NULL);

    return agent_connected;
}

void
virt_viewer_display_spice_set_desktop(VirtViewerDisplay *display,
                                      guint x, guint y,
                                      guint width, guint height)
{
    VirtViewerDisplaySpicePrivate *priv;
    //VirtViewerSession* session;
    guint desktopWidth, desktopHeight;

   // session = virt_viewer_display_get_session(display);

    g_return_if_fail(VIRT_VIEWER_IS_DISPLAY_SPICE(display));

    virt_viewer_display_get_desktop_size(display, &desktopWidth, &desktopHeight);

    priv = VIRT_VIEWER_DISPLAY_SPICE(display)->priv;

    if (desktopWidth == width && desktopHeight == height && priv->x == x && priv->y == y)
        return;

    g_object_set(G_OBJECT(display), "desktop-width", width, "desktop-height", height, NULL);
    priv->x = x;
    priv->y = y;

    virt_viewer_display_queue_resize(display);

    g_signal_emit_by_name(display, "display-desktop-resize");

  // g_object_set(virt_viewer_session_get_app(VIRT_VIEWER_SESSION(session)), "resize-guest",FALSE, NULL);

}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 * End:
 */

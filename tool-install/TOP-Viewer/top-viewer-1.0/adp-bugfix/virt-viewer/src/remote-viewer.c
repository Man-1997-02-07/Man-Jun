/*
 * Virt Viewer: A virtual machine console viewer
 *
 * Copyright (C) 2007-2012 Red Hat, Inc.
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
 * Author: Marc-André Lureau <marcandre.lureau@redhat.com>
 */

#include <config.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>
#include <libxml/uri.h>
#include <string.h>

#ifdef HAVE_OVIRT
#include <govirt/govirt.h>
#include "ovirt-foreign-menu.h"
#include "virt-viewer-vm-connection.h"
#endif

#ifdef HAVE_SPICE_GTK
#include "virt-viewer-session-spice.h"
#endif

#include "virt-viewer-app.h"
#include "virt-viewer-auth.h"
#include "virt-viewer-file.h"
#include "virt-viewer-session.h"
#include "virt-viewer-util.h"
#include "remote-viewer.h"
#include "remote-viewer-connect.h"

struct _RemoteViewerPrivate {
#ifdef HAVE_OVIRT
    OvirtForeignMenu *ovirt_foreign_menu;
#endif
    gboolean open_recent_dialog;
};

G_DEFINE_TYPE_WITH_PRIVATE (RemoteViewer, remote_viewer, VIRT_VIEWER_TYPE_APP)

enum RemoteViewerProperties {
    PROP_0,
#ifdef HAVE_OVIRT
    PROP_OVIRT_FOREIGN_MENU,
#endif
};

#ifdef HAVE_OVIRT
static OvirtVm * choose_vm(GtkWindow *main_window,
                           char **vm_name,
                           OvirtCollection *vms,
                           GError **error);
#endif

static gboolean remote_viewer_start(VirtViewerApp *self, GError **error);


static void
remote_viewer_dispose (GObject *object)
{
#if defined(HAVE_OVIRT)
    RemoteViewer *self = REMOTE_VIEWER(object);
    RemoteViewerPrivate *priv = self->priv;
#endif

#ifdef HAVE_OVIRT
    if (priv->ovirt_foreign_menu) {
        g_object_unref(priv->ovirt_foreign_menu);
        priv->ovirt_foreign_menu = NULL;
    }
#endif

    G_OBJECT_CLASS(remote_viewer_parent_class)->dispose (object);
}

static void
remote_viewer_deactivated(VirtViewerApp *app, gboolean connect_error)
{
    RemoteViewer *self = REMOTE_VIEWER(app);
    RemoteViewerPrivate *priv = self->priv;

    if (connect_error && priv->open_recent_dialog) {
        if (virt_viewer_app_start(app, NULL)) {
            return;
        }
    }

    VIRT_VIEWER_APP_CLASS(remote_viewer_parent_class)->deactivated(app, connect_error);
}

static gchar **opt_args = NULL;
static char  *opt_title = NULL;
static gchar *opt_username = NULL;
static gchar *opt_password = NULL;




static void
remote_viewer_add_option_entries(VirtViewerApp *self, GOptionContext *context, GOptionGroup *group)
{
    static const GOptionEntry options[] = {
        { "title", 't', 0, G_OPTION_ARG_STRING, &opt_title,
          N_("Set window title"), NULL },
        { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_STRING_ARRAY, &opt_args,
          NULL, "URI|VV-FILE" },
        { NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
    };

    VIRT_VIEWER_APP_CLASS(remote_viewer_parent_class)->add_option_entries(self, context, group);
    g_option_context_set_summary(context, _("Top viewer client"));
    g_option_group_add_entries(group, options);

#ifdef HAVE_OVIRT
    g_option_context_add_group (context, ovirt_get_option_group ());
#endif
}

static gboolean
remote_viewer_local_command_line (GApplication   *gapp,
                                  gchar        ***args,
                                  int            *status)
{
    gboolean ret = FALSE;
    VirtViewerApp *app = VIRT_VIEWER_APP(gapp);
    RemoteViewer *self = REMOTE_VIEWER(app);
	gchar *guri = NULL;
    gchar *zoom = NULL;
	gchar **params;
	gchar **uri_elements;
	gchar **user_pass;
	guint element_count;

    ret = G_APPLICATION_CLASS(remote_viewer_parent_class)->local_command_line(gapp, args, status);
    if (ret)
        goto end;

    if (!opt_args) {
        self->priv->open_recent_dialog = TRUE;
    } else {
        if (g_strv_length(opt_args) > 2) {
            g_printerr(_("\nError: can't handle multiple URIs\n\n"));
            ret = TRUE;
            *status = 1;
            goto end;
        }

//2020.01.16 xc save source file
		/*****************************************************************************************************************************************************/
        //ADD URI支持格式:
        //  spice://ip:port?xxx(title),xxx(password),xxx(username)[#xxx(watermark)]
        //  spice://ip:port?xxx(title),xxx(password)[#xxx(watermark)]
        //  spice://ip:port?xxx(title)[#xxx(watermark)]
       /* guri = opt_args[0];
		if (NULL != g_strrstr(guri, "?")) {
            uri_elements = g_strsplit(guri, "?", 2);
            element_count = g_strv_length(uri_elements);
            if (element_count == 2) {
                guri = g_strdup(uri_elements[0]);

                params = g_strsplit(uri_elements[1], "#", 2);
			    if (g_strv_length(params) > 0) {
                    user_pass = g_strsplit(params[0], ",", 3);
                    element_count = g_strv_length(user_pass);
                    if (element_count == 1) {
                        opt_title = g_strdup(user_pass[0]);
                    } else if (element_count == 2) {
                        opt_title = g_strdup(user_pass[0]);
                        opt_password = g_strdup(user_pass[1]);
                        g_object_set(app, "password", opt_password, NULL);
                    } else if (element_count == 3) {
                        opt_title = g_strdup(user_pass[0]);
                        opt_password = g_strdup(user_pass[1]);
                        opt_username = g_strdup(user_pass[2]);
					
                        g_object_set(app, "username", opt_username, NULL);
                        g_object_set(app, "password", opt_password, NULL);
                    }
				            urldecode(opt_title);
                    g_strfreev(uri_elements);
                    g_strfreev(user_pass);
			    }
				if (g_strv_length(params) == 2) {
				    g_object_set(app, "watermark", g_strdup(params[1]), NULL);
                }
            }
        }

        if (g_strv_length(opt_args) == 2) {
            zoom = opt_args[1];
        }*/
/*****************************************************************************************************************************************************/
//2020.01.16 xc change source file save new file
		//ADD URI支持格式:
        //  spice://ip:port?xxx(title),xxx(password),xxx(username)[#xxx(watermark)]
        //  spice://ip:port?xxx(title),xxx(password)[#xxx(watermark)]
        //  spice://ip:port?xxx(title)[#xxx(watermark)]
		//输入参数 大于0的情况如下
		if((g_strv_length(opt_args) <2)&&(g_strv_length(opt_args) >0))
		{
			guri = opt_args[0];
			if (NULL != g_strrstr(guri, "?")) {//带标题等其他字段标识
				uri_elements = g_strsplit(guri, "?", 2);
				element_count = g_strv_length(uri_elements);
				if (element_count == 2) {
					guri = g_strdup(uri_elements[0]);
					params = g_strsplit(uri_elements[1], "#", 2);
					if (g_strv_length(params) > 0) {
						user_pass = g_strsplit(params[0], ",", 3);
						element_count = g_strv_length(user_pass);
						if (element_count == 1) {
							opt_title = g_strdup(user_pass[0]);//一个参数仅为水印字段
							printf("##################opt_title: %s##################\n", opt_title);//2020.09.21 xc add
						} else if (element_count == 2) {
							opt_title = g_strdup(user_pass[0]);//一个参数为水印字段
							opt_password = g_strdup(user_pass[1]);//另一个参数为密码字段
							printf("##################opt_title: %s ,opt_password: %s##################\n", opt_title, opt_password);//2020.09.21 xc add
							g_object_set(app, "password", opt_password, NULL);
						} else if (element_count == 3) {
							opt_title = g_strdup(user_pass[0]);
							opt_password = g_strdup(user_pass[1]);
							opt_username = g_strdup(user_pass[2]);
							printf("##################opt_title: %s ,opt_password: %s, opt_username: %s##################\n", opt_title, opt_password, opt_username);//2020.09.21 xc add
							g_object_set(app, "username", opt_username, NULL);
							g_object_set(app, "password", opt_password, NULL);
						}
						
							urldecode(opt_title);//窗口标题文本串
															
						g_strfreev(uri_elements);
						g_strfreev(user_pass);
					}
					if (g_strv_length(params) == 2) {
						printf("##################(g_strv_length(params) == 2)##################\n");//2020.09.23 xc add 
						g_object_set(app, "watermark", g_strdup(params[1]), NULL);//设置水印字段
						//char *UTL = params[1];//2020.09.22 xc add
						//urldecode(UTL);//水印字段文本串
						//g_object_set(app, "watermark", g_strdup(UTL), NULL);//设置水印字段
					}
				}
				//设置用户名称
				g_object_set(app, "guri", guri, NULL);
			}
			else{//不带标题等其他字段标识				
				gchar *Xguri = NULL;
				gchar **Xparams;
				gchar **Xuri_elements;
				if (NULL != g_strrstr(guri, ":"))
				{
						Xguri = guri;
						Xuri_elements = g_strsplit(Xguri, ":", 2);
						Xguri = Xuri_elements[1];//10.30.10.21:5935
						printf("####################################start：Xguri =%s####################################\n", Xguri);
						if(Xguri != NULL)
						{
							Xuri_elements = g_strsplit(Xguri, ":", 2);
							Xguri = Xuri_elements[1];
							printf("####################################[Xguri != NULL]Xguri =%s####################################\n", Xguri);
							if(Xguri != NULL)//输入ip/port,弹出登录的界面的入口
							{
								printf("####################################[Xguri != NULL][输入ip/port,弹出登录的界面的入口]####################################\n");
							}
							else
							{//只输入ip
								printf("####################################[Xguri == NULL]####################################\n");
								guri = "NULL";
							}
						}
						else
						{
							guri = "NULL";
						}
				}
			}
		}        
        else if (g_strv_length(opt_args) == 2) {//输入参数等于2的情况如下
			printf("##################((g_strv_length(opt_args) == 2))##################\n");//2020.09.23 xc add		
            zoom = opt_args[1];
        }

/*****************************************************************************************************************************************************/
		g_debug("uri:%s, username:%s, password:%s title:%s", guri, opt_username, opt_password, opt_title);

        g_object_set(app, "zoom", zoom, NULL);
        g_object_set(app, "guri", guri, NULL);
    }

    if (opt_title)
        g_object_set(app, "title", opt_title, NULL);

end:
    if (ret && *status)
        g_printerr(_("Run '%s --help' to see a full list of available command line options\n"), g_get_prgname());

    g_strfreev(opt_args);
    return ret;
}

static void
remote_viewer_get_property(GObject *object, guint property_id,
                           GValue *value G_GNUC_UNUSED,
                           GParamSpec *pspec)
{
#ifdef HAVE_OVIRT
    RemoteViewer *self = REMOTE_VIEWER(object);
    RemoteViewerPrivate *priv = self->priv;
#endif

    switch (property_id) {
#ifdef HAVE_OVIRT
    case PROP_OVIRT_FOREIGN_MENU:
        g_value_set_object(value, priv->ovirt_foreign_menu);
        break;
#endif

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
remote_viewer_class_init (RemoteViewerClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    VirtViewerAppClass *app_class = VIRT_VIEWER_APP_CLASS (klass);
    GApplicationClass *g_app_class = G_APPLICATION_CLASS(klass);

    object_class->get_property = remote_viewer_get_property;
    object_class->dispose = remote_viewer_dispose;

    g_app_class->local_command_line = remote_viewer_local_command_line;

    app_class->start = remote_viewer_start;
    app_class->deactivated = remote_viewer_deactivated;
    app_class->add_option_entries = remote_viewer_add_option_entries;

#ifdef HAVE_OVIRT
    g_object_class_install_property(object_class,
                                    PROP_OVIRT_FOREIGN_MENU,
                                    g_param_spec_object("ovirt-foreign-menu",
                                                        "oVirt Foreign Menu",
                                                        "Object which is used as interface to oVirt",
                                                        OVIRT_TYPE_FOREIGN_MENU,
                                                        G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));
#endif
}

static void
remote_viewer_init(RemoteViewer *self)
{
    self->priv = remote_viewer_get_instance_private(self);
}

RemoteViewer *
remote_viewer_new(void)
{
    return g_object_new(REMOTE_VIEWER_TYPE,
                        "application-id", "www.topsec.com.cn.top-viewer",
                        "flags", G_APPLICATION_NON_UNIQUE,
                        NULL);
}

#if defined(HAVE_SPICE_GTK)
static SpiceSession *
remote_viewer_get_spice_session(RemoteViewer *self)
{
    VirtViewerSession *vsession = NULL;
    SpiceSession *session = NULL;

    g_object_get(self, "session", &vsession, NULL);
    g_return_val_if_fail(vsession != NULL, NULL);

    g_object_get(vsession, "spice-session", &session, NULL);

    g_object_unref(vsession);

    return session;
}
#endif

#ifdef HAVE_OVIRT
static gboolean
parse_ovirt_uri(const gchar *uri_str, char **rest_uri, char **name, char **username)
{
    char *vm_name = NULL;
    char *rel_path;
    xmlURIPtr uri;
    gchar **path_elements;
    guint element_count;

    g_return_val_if_fail(uri_str != NULL, FALSE);
    g_return_val_if_fail(rest_uri != NULL, FALSE);
    g_return_val_if_fail(name != NULL, FALSE);

    uri = xmlParseURI(uri_str);
    g_return_val_if_fail(uri != NULL, FALSE);

    if (g_strcmp0(uri->scheme, "ovirt") != 0) {
        xmlFreeURI(uri);
        return FALSE;
    }

    if (username && uri->user)
        *username = g_strdup(uri->user);

    if (uri->path == NULL) {
        *name = NULL;
        *rest_uri = g_strdup(uri->server);
        xmlFreeURI(uri);
        return TRUE;
    }

    if (*uri->path != '/') {
        xmlFreeURI(uri);
        return FALSE;
    }

    /* extract VM name from path */
    path_elements = g_strsplit(uri->path, "/", -1);

    element_count = g_strv_length(path_elements);
    if (element_count == 0) {
        g_strfreev(path_elements);
        return FALSE;
    }
    vm_name = path_elements[element_count-1];
    path_elements[element_count-1] = NULL;

    /* build final URI */
    rel_path = g_strjoinv("/", path_elements);
    *rest_uri = g_strdup_printf("%s%s", uri->server, rel_path);
    *name = vm_name;
    g_free(rel_path);
    g_strfreev(path_elements);
    xmlFreeURI(uri);

    g_debug("oVirt base URI: %s", *rest_uri);
    g_debug("oVirt VM name: %s", *name);

    return TRUE;
}

static gboolean
authenticate_cb(RestProxy *proxy, G_GNUC_UNUSED RestProxyAuth *auth,
                G_GNUC_UNUSED gboolean retrying, gpointer user_data)
{
    gchar *username = NULL;
    gchar *password = NULL;
    VirtViewerWindow *window;
    gboolean success = FALSE;
    gboolean kiosk = FALSE;

    g_object_get(proxy,
                 "username", &username,
                 NULL);

    g_object_get(G_OBJECT(user_data), "kiosk", &kiosk, NULL);

    if (username == NULL || *username == '\0')
        username = g_strdup(g_get_user_name());

    window = virt_viewer_app_get_main_window(VIRT_VIEWER_APP(user_data));
    do {
        success = virt_viewer_auth_collect_credentials(virt_viewer_window_get_window(window),
                                                       "oVirt",
                                                       NULL,
                                                       &username, &password);
    } while (kiosk && !success);

    if (success) {
        g_object_set(G_OBJECT(proxy),
                     "username", username,
                     "password", password,
                     NULL);
    } else {
        rest_proxy_auth_cancel(auth);
    }

    g_free(username);
    g_free(password);
    return success;
}

static void
ovirt_foreign_menu_update(GtkApplication *gtkapp, GtkWindow *gtkwin, G_GNUC_UNUSED gpointer data)
{
    RemoteViewer *self = REMOTE_VIEWER(gtkapp);
    VirtViewerWindow *win = g_object_get_data(G_OBJECT(gtkwin), "virt-viewer-window");
    GtkBuilder *builder = virt_viewer_window_get_builder(win);
    GtkWidget *menu = GTK_WIDGET(gtk_builder_get_object(builder, "menu-change-cd"));
    gtk_widget_set_visible(menu, self->priv->ovirt_foreign_menu != NULL);
}

static void
ovirt_foreign_menu_update_each(gpointer value,
                               gpointer user_data)
{
    ovirt_foreign_menu_update(GTK_APPLICATION(user_data),
                              virt_viewer_window_get_window(VIRT_VIEWER_WINDOW(value)),
                              NULL);
}

static void
ovirt_foreign_menu_updated(RemoteViewer *self)
{
    GList *windows = virt_viewer_app_get_windows(VIRT_VIEWER_APP(self));

    g_debug("Spice foreign menu updated");

    g_list_foreach(windows, ovirt_foreign_menu_update_each, self);
}

static void
virt_viewer_app_set_ovirt_foreign_menu(VirtViewerApp *app,
                                       OvirtForeignMenu *foreign_menu)
{
    RemoteViewer *self;
    g_return_if_fail(REMOTE_VIEWER_IS(app));
    g_return_if_fail(OVIRT_IS_FOREIGN_MENU(foreign_menu));

    self = REMOTE_VIEWER(app);
    g_clear_object(&self->priv->ovirt_foreign_menu);
    self->priv->ovirt_foreign_menu = foreign_menu;
    g_signal_connect(G_OBJECT(app), "window-added",
                     (GCallback)ovirt_foreign_menu_update, NULL);
    ovirt_foreign_menu_updated(self);
}

static gboolean
create_ovirt_session(VirtViewerApp *app, const char *uri, GError **err)
{
    OvirtProxy *proxy = NULL;
    OvirtApi *api = NULL;
    OvirtCollection *vms;
    OvirtVm *vm = NULL;
    OvirtVmDisplay *display = NULL;
    OvirtVmState state;
    GError *error = NULL;
    char *rest_uri = NULL;
    char *vm_name = NULL;
    char *username = NULL;
    gboolean success = FALSE;
    guint port;
    guint secure_port;
    char *proxy_url = NULL;
    OvirtVmDisplayType type;
    const char *session_type;

    gchar *gport = NULL;
    gchar *gtlsport = NULL;
    gchar *ghost = NULL;
    gchar *ticket = NULL;
    gchar *host_subject = NULL;
    gchar *guid = NULL;

    g_return_val_if_fail(VIRT_VIEWER_IS_APP(app), FALSE);

    if (!parse_ovirt_uri(uri, &rest_uri, &vm_name, &username)) {
        g_set_error_literal(&error, VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                            _("failed to parse ovirt uri"));
        goto error;
    }

    proxy = ovirt_proxy_new(rest_uri);
    g_object_set(proxy,
                 "username", username,
                 NULL);
    ovirt_set_proxy_options(proxy);
    g_signal_connect(G_OBJECT(proxy), "authenticate",
                     G_CALLBACK(authenticate_cb), app);

    api = ovirt_proxy_fetch_api(proxy, &error);
    if (error != NULL) {
        g_debug("failed to get oVirt 'api' collection: %s", error->message);
        if (g_error_matches(error, OVIRT_REST_CALL_ERROR, OVIRT_REST_CALL_ERROR_CANCELLED)) {
            g_clear_error(&error);
            g_set_error_literal(&error,
                                VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_CANCELLED,
                                _("Authentication was cancelled"));
        }
        goto error;
    }
    vms = ovirt_api_get_vms(api);
    ovirt_collection_fetch(vms, proxy, &error);
    if (error != NULL) {
        g_debug("failed to fetch oVirt 'vms' collection: %s", error->message);
        goto error;
    }
    if (vm_name == NULL ||
        (vm = OVIRT_VM(ovirt_collection_lookup_resource(vms, vm_name))) == NULL) {
        VirtViewerWindow *main_window = virt_viewer_app_get_main_window(app);
        vm = choose_vm(virt_viewer_window_get_window(main_window),
                       &vm_name,
                       vms,
                       &error);
        if (vm == NULL) {
            goto error;
        }
    }
    g_object_get(G_OBJECT(vm), "state", &state, NULL);
    if (state != OVIRT_VM_STATE_UP) {
        g_set_error(&error, VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                    _("oVirt VM %s is not running"), vm_name);
        g_debug("%s", error->message);
        goto error;
    }
    g_object_set(app, "guest-name", vm_name, NULL);

    if (!ovirt_vm_get_ticket(vm, proxy, &error)) {
        g_debug("failed to get ticket for %s: %s", vm_name, error->message);
        goto error;
    }

    g_object_get(G_OBJECT(vm), "display", &display, "guid", &guid, NULL);
    if (display == NULL) {
        g_set_error(&error, VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                    _("oVirt VM %s has no display"), vm_name);
        goto error;
    }

    if (guid != NULL) {
        g_object_set(app, "uuid", guid, NULL);
    }

    g_object_get(G_OBJECT(display),
                 "type", &type,
                 "address", &ghost,
                 "port", &port,
                 "secure-port", &secure_port,
                 "ticket", &ticket,
                 "host-subject", &host_subject,
                 "proxy-url", &proxy_url,
                 NULL);
    if (port != 0) {
        gport = g_strdup_printf("%u", port);
    }
    if (secure_port != 0) {
        gtlsport = g_strdup_printf("%u", secure_port);
    }

    if (ghost == NULL) {
        g_set_error(&error, VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                    _("oVirt VM %s has no host information"), vm_name);
        g_debug("%s", error->message);
        goto error;
    }

    if (type == OVIRT_VM_DISPLAY_SPICE) {
        session_type = "spice";
    } else if (type == OVIRT_VM_DISPLAY_VNC) {
        session_type = "vnc";
    } else {
        g_set_error(&error, VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                    _("oVirt VM %s has unknown display type: %u"), vm_name, type);
        g_debug("%s", error->message);
        goto error;
    }

    {
        OvirtForeignMenu *ovirt_menu = ovirt_foreign_menu_new(proxy);
        g_object_set(G_OBJECT(ovirt_menu), "api", api, "vm", vm, NULL);
        virt_viewer_app_set_ovirt_foreign_menu(app, ovirt_menu);
    }

    virt_viewer_app_set_connect_info(app, NULL, ghost, gport, gtlsport,
                                     session_type, NULL, NULL, 0, NULL);

    if (!virt_viewer_app_create_session(app, session_type, &error))
        goto error;

#ifdef HAVE_SPICE_GTK
    if (type == OVIRT_VM_DISPLAY_SPICE) {
        SpiceSession *session;
        GByteArray *ca_cert;

        session = remote_viewer_get_spice_session(REMOTE_VIEWER(app));
        g_object_set(G_OBJECT(session),
                     "password", ticket,
                     "cert-subject", host_subject,
                     "proxy", proxy_url,
                     NULL);
        g_object_get(G_OBJECT(proxy), "ca-cert", &ca_cert, NULL);
        if (ca_cert != NULL) {
            g_object_set(G_OBJECT(session),
                    "ca", ca_cert,
                    NULL);
            g_byte_array_unref(ca_cert);
        }
    }
#endif

    success = TRUE;

error:
    g_free(username);
    g_free(rest_uri);
    g_free(vm_name);
    g_free(ticket);
    g_free(gport);
    g_free(gtlsport);
    g_free(ghost);
    g_free(host_subject);
    g_free(guid);
    g_free(proxy_url);

    if (error != NULL)
        g_propagate_error(err, error);
    if (display != NULL)
        g_object_unref(display);
    if (vm != NULL)
        g_object_unref(vm);
    if (proxy != NULL)
        g_object_unref(proxy);

    return success;
}

static OvirtVm *
choose_vm(GtkWindow *main_window,
          char **vm_name,
          OvirtCollection *vms_collection,
          GError **error)
{
    GtkListStore *model;
    GtkTreeIter iter;
    GHashTable *vms;
    GHashTableIter vms_iter;
    OvirtVmState state;
    OvirtVm *vm;

    g_return_val_if_fail(vm_name != NULL, NULL);
    free(*vm_name);

    model = gtk_list_store_new(1, G_TYPE_STRING);

    vms = ovirt_collection_get_resources(vms_collection);
    g_hash_table_iter_init(&vms_iter, vms);
    while (g_hash_table_iter_next(&vms_iter, (gpointer *) vm_name, (gpointer *) &vm)) {
        g_object_get(G_OBJECT(vm), "state", &state, NULL);
        if (state == OVIRT_VM_STATE_UP) {
            gtk_list_store_append(model, &iter);
            gtk_list_store_set(model, &iter, 0, *vm_name, -1);
       }
    }

    *vm_name = virt_viewer_vm_connection_choose_name_dialog(main_window,
                                                            GTK_TREE_MODEL(model),
                                                            error);
    g_object_unref(model);
    if (*vm_name == NULL)
        return NULL;

    vm = OVIRT_VM(ovirt_collection_lookup_resource(vms_collection, *vm_name));

    return vm;
}
#endif /* HAVE_OVIRT */

static void
remote_viewer_recent_add(gchar *uri, const gchar *mime_type)
{
    GtkRecentManager *recent;
    GtkRecentData meta = {
        .app_name     = (char*)"top-viewer",
        .app_exec     = (char*)"top-viewer %u",
        .mime_type    = (char*)mime_type,
    };

    g_return_if_fail(uri != NULL);

    recent = gtk_recent_manager_get_default();
    meta.display_name = uri;
    if (!gtk_recent_manager_add_full(recent, uri, &meta))
        g_warning("Recent item couldn't be added");
}

static void
remote_viewer_session_connected(VirtViewerSession *session,
                                gchar *guri)
{
    gchar *uri = virt_viewer_session_get_uri(session);
    const gchar *mime = virt_viewer_session_mime_type(session);

    if (uri == NULL)
        uri = g_strdup(guri);

    remote_viewer_recent_add(uri, mime);
    g_free(uri);
    g_free(guri);
}

static gchar *
read_all_stdin(gsize *len, GError **err)
{
    GIOChannel *ioc = g_io_channel_unix_new(fileno(stdin));
    gchar *content = NULL;
    GIOStatus status;

    status = g_io_channel_read_to_end(ioc, &content, len, err);
    g_assert(status != G_IO_STATUS_AGAIN);

    g_io_channel_unref(ioc);
    g_assert((content && !*err) || (!content && *err));

    return content;
}

static gboolean
remote_viewer_initial_connect(RemoteViewer *self, const gchar *type, const gchar *guri,
                              VirtViewerFile *vvfile, GError **error)
{
    VirtViewerApp *app = VIRT_VIEWER_APP(self);
	SpiceSession *session = NULL;

#ifdef HAVE_OVIRT
    if (g_strcmp0(type, "ovirt") == 0) {
        if (!create_ovirt_session(app, guri, error)) {
            g_prefix_error(error, _("Couldn't open oVirt session: "));
            return FALSE;
        }
    } else
#endif
    {
        if (!virt_viewer_app_create_session(app, type, error))
            return FALSE;
    }

    
	g_warning("initial connect vvfile: %p username: %s, password: %s", vvfile, opt_username, opt_password);
	if (NULL != vvfile) {
	    g_warning("000");
		if (NULL != opt_username) {
		    virt_viewer_file_set_username(vvfile, opt_username);
		}
		if (NULL != opt_password) {
			virt_viewer_file_set_password(vvfile, opt_password);
		}
	} else {
		session = remote_viewer_get_spice_session(self);
		g_warning("000 RemoteViewer: %p, SpiceSession: %p", self, session);
		if (NULL != opt_username) {
			g_object_set(G_OBJECT(session), "username", opt_username, NULL);
		}
		if (NULL != opt_password) {
			g_object_set(G_OBJECT(session), "password", opt_password, NULL);
		}
	}

    g_signal_connect(virt_viewer_app_get_session(app), "session-connected",
                     G_CALLBACK(remote_viewer_session_connected), g_strdup(guri));

    virt_viewer_session_set_file(virt_viewer_app_get_session(app), vvfile);
#ifdef HAVE_OVIRT
    if (vvfile != NULL) {
        OvirtForeignMenu *ovirt_menu;
        ovirt_menu = ovirt_foreign_menu_new_from_file(vvfile);
        if (ovirt_menu != NULL) {
            virt_viewer_app_set_ovirt_foreign_menu(app, ovirt_menu);
        }
    }
#endif

    if (!virt_viewer_app_initial_connect(app, error)) {
        if (*error == NULL) {
            g_set_error_literal(error,
                                VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                                _("Failed to initiate connection"));
        }
        return FALSE;
    }

    return TRUE;
}

static gboolean
remote_viewer_start(VirtViewerApp *app, GError **err)
{
    g_return_val_if_fail(REMOTE_VIEWER_IS(app), FALSE);

    RemoteViewer *self = REMOTE_VIEWER(app);
    RemoteViewerPrivate *priv = self->priv;
    GFile *file = NULL;
    VirtViewerFile *vvfile = NULL;
    gboolean ret = FALSE;
    gchar *guri = NULL;
    gchar *type = NULL;
    GError *error = NULL;
/********************************************************************************************************************************************************************************/
//2020.01.16 xc save source file
/*retry_dialog:
    {
        if (priv->open_recent_dialog) {
            VirtViewerWindow *main_window = virt_viewer_app_get_main_window(app);
            if (!remote_viewer_connect_dialog(virt_viewer_window_get_window(main_window), &guri)) {
                g_set_error_literal(&error,
                            VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_CANCELLED,
                            _("No connection was chosen"));
                g_propagate_error(err, error);
                return FALSE;
            }
            g_object_set(app, "guri", guri, NULL);
        } else
            g_object_get(app, "guri", &guri, NULL);

        g_return_val_if_fail(guri != NULL, FALSE);

        g_debug("Opening display to %s", guri);

        if (!g_strcmp0(guri, "-")) {
            gsize len = 0;
            gchar *buf = read_all_stdin(&len, &error);

            if (error) {
                g_prefix_error(&error, _("Failed to read stdin: "));
                g_warning("%s", error->message);
                goto cleanup;
            }

            vvfile = virt_viewer_file_new_from_buffer(buf, len, &error);
            g_free(buf);
        } else {
            file = g_file_new_for_commandline_arg(guri);
            if (g_file_query_exists(file, NULL)) {
                gchar *path = g_file_get_path(file);
                vvfile = virt_viewer_file_new(path, &error);
                g_free(path);
            }
        }

        if (error) {
            g_prefix_error(&error, _("Invalid file %s: "), guri);
            g_warning("%s", error->message);
            goto cleanup;
        }

        if (vvfile) {
            g_object_get(G_OBJECT(vvfile), "type", &type, NULL);
        } else if (virt_viewer_util_extract_host(guri, &type, NULL, NULL, NULL, NULL) < 0 || type == NULL) {
            g_set_error_literal(&error,
                                VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                                _("Cannot determine the connection type from URI"));
            goto cleanup;
        }
        if (!remote_viewer_initial_connect(self, type, guri, vvfile, &error))
            goto cleanup;
    }

    ret = VIRT_VIEWER_APP_CLASS(remote_viewer_parent_class)->start(app, &error);*/
/********************************************************************************************************************************************************************************/
//2020.01.16 xc change source file save new file
	retry_dialog:
    {
        if (priv->open_recent_dialog) {
            VirtViewerWindow *main_window = virt_viewer_app_get_main_window(app);
            if (!remote_viewer_connect_dialog(virt_viewer_window_get_window(main_window), &guri)) {
                g_set_error_literal(&error,
                            VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_CANCELLED,
                            _("No connection was chosen"));
                g_propagate_error(err, error);
                return FALSE;
            }
            g_object_set(app, "guri", guri, NULL);
        } else
            g_object_get(app, "guri", &guri, NULL);

        g_return_val_if_fail(guri != NULL, FALSE);

        g_debug("Opening display to %s", guri);
		char *debugStr = (char *)malloc(sizeof(char)*128);//2020.09.18 xc add
		char destDebugStr[128];			
		debugStr = "Enter individual characters: 'Error', Please check the input marked Ip/Port, Retry: ";
        if (!g_strcmp0(guri, "-")) {
            gsize len = 0;
			g_debug("statu：进入阻塞模式，加一个提示弹窗（已经在输入标注ip/port，这个提示窗口待考虑实际情况：）");//2020.09.18 xc add
			virt_viewer_app_simple_message_dialog(app, debugStr, guri);//创建提示窗口
			g_debug("virt_viewer_app_simple_message_dialog: %s", guri);//2020.09.18 xc add
			goto cleanup;//2020.09.18 xc add
			
            gchar *buf = read_all_stdin(&len, &error);

            if (error) {
                g_prefix_error(&error, _("Failed to read stdin: "));
                g_warning("%s", error->message);
                goto cleanup;
            }

            vvfile = virt_viewer_file_new_from_buffer(buf, len, &error);
            g_free(buf);
        } else {						
				printf("##################(不带标题等其他字段标识)##################\n");//2020.09.23 xc add				
				//处理错误情况输入
				gchar *Xguri = NULL;
				gchar **Xparams;
				gchar **Xuri_elements;
				if (NULL != g_strrstr(guri, ":"))
				{
						Xguri = guri;
						Xuri_elements = g_strsplit(Xguri, ":", 2);
						Xguri = Xuri_elements[1];//10.30.10.21:5935
						printf("####################################start：Xguri =%s####################################\n", Xguri);
						if(Xguri != NULL)
						{
							Xuri_elements = g_strsplit(Xguri, ":", 2);
							Xguri = Xuri_elements[1];
							printf("####################################[Xguri != NULL]Xguri =%s####################################\n", Xguri);
							if(Xguri != NULL)//输入ip/port,跳出登录的界面
							{
								debugStr = "Enter individual characters: 'Ip/Port', Please check the input marked Ip/Port, Retry: ";
							}		
							else
							{//只输入ip
								virt_viewer_app_simple_message_dialog(app, debugStr, guri);//创建提示窗口
								printf("virt_viewer_app_simple_message_dialog: %s", guri);//2020.09.18 xc add
								goto cleanup;//2020.09.18 xc add
							}
						}
						else
						{
							virt_viewer_app_simple_message_dialog(app, debugStr, guri);//创建提示窗口
							printf("virt_viewer_app_simple_message_dialog: %s", guri);//2020.09.18 xc add
							goto cleanup;//2020.09.18 xc add
						}						
				}				
			printf("匹配前2：%s", guri);//2020.09.18 xc add
            file = g_file_new_for_commandline_arg(guri);
			gchar *path_Pre = g_file_get_path(file);
			printf("g_file_get_path_Pre：%s", path_Pre);//2020.09.18 xc add
            if (g_file_query_exists(file, NULL)) {//用于检查特定文件是否存在
				printf("g_file_query_exists：ture");//2020.09.18 xc add
                gchar *path = g_file_get_path(file);
                vvfile = virt_viewer_file_new(path, &error);
                g_free(path);
            }
        }

        if (error) {
            g_prefix_error(&error, _("Invalid file %s: "), guri);
            g_warning("%s", error->message);
            goto cleanup;
        }

        if (vvfile) 
		{
            g_object_get(G_OBJECT(vvfile), "type", &type, NULL);
			printf("typePre： %s", type);//2020.09.21 xc add
        } 
		else if (virt_viewer_util_extract_host(guri, &type, NULL, NULL, NULL, NULL) < 0 || type == NULL) 
		{
            g_set_error_literal(&error,
                                VIRT_VIEWER_ERROR, VIRT_VIEWER_ERROR_FAILED,
                                _("Cannot determine the connection type from URI"));
            goto cleanup;
        }
        if (!remote_viewer_initial_connect(self, type, guri, vvfile, &error))//Guest (null) has a spice display
		{
			printf("typeEnd： %s", type);//2020.09.21 xc add
			printf("remote_viewer_initial_connect：TRUE");//2020.09.21 xc add
			goto cleanup;
		}
    }

    ret = VIRT_VIEWER_APP_CLASS(remote_viewer_parent_class)->start(app, &error);
/********************************************************************************************************************************************************************************/

cleanup:
    g_clear_object(&file);
    g_clear_object(&vvfile);
    g_free(guri);
    guri = NULL;
    g_free(type);
    type = NULL;

    if (!ret && priv->open_recent_dialog) {
        if (error != NULL) {
            virt_viewer_app_simple_message_dialog(app, _("Unable to connect: %s"), error->message);
        }
        g_clear_error(&error);
        goto retry_dialog;
    }
    if (error != NULL)
        g_propagate_error(err, error);

    return ret;
}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 *  indent-tabs-mode: nil
 * End:
 */

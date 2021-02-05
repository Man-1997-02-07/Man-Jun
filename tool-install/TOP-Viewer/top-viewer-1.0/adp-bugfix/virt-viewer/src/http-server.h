#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include "virt-viewer-app.h"
#include "virt-viewer-window.h"



typedef struct _Httpdata Httpdata;
typedef enum _ImageCom ImageCom;
typedef enum _VideoCode VideoCode;
typedef enum _WaterMark Watermark;

struct _Httpdata
{
    VirtViewerApp* app; 
    guint listen_port;
    gpointer user_data;
};

enum _ImageCom
{
    AUTOGLZ,
    AUTOLZ,
    QUIC,
    GLZ,
    LZ,
    LZ4,
    OFF,
    BadOption,
};

enum _VideoCode
{
    MJPEG,
    VP8,
    VP9,
    H264,
};

enum _WaterMark
{
    CONTENT,
    INTERVAL,
    SLANT,
    FONT_SIZE,
    WEIGHT,
    FONT_COLOR,
    STYLE,
    UNDERLINE,
    BACK_COLOR,
    BACK_COLOR_ALPHA,
};



static const struct{
    const char *name;
    ImageCom imgcode;
} imagewords[] = {
    {"auto-glz", AUTOGLZ},
    {"auto-lz", AUTOLZ},
    {"quic", QUIC},
    {"glz", GLZ},
    {"lz", LZ},
    {"lz4", LZ4},
    {"off", OFF},
    {NULL, BadOption}
};

static const struct{
    const char *name;
    VideoCode videocode;
} videowords[] = {
    {"mjpeg", MJPEG},
    {"vp8", VP8},
    {"vp9", VP9},
    {"h264", H264},
    {NULL, BadOption}
};

static struct{
    gchar* value;
    Watermark watermark;
} watermarkwords[] = {
    {"top", CONTENT},
    {"4", INTERVAL},
    {"normal", SLANT},
    {"9", FONT_SIZE},
    {"20", WEIGHT},
    {"black", FONT_COLOR},
    {"normal", STYLE},
    {"none", UNDERLINE},
    {"white", BACK_COLOR},
    {"65535", BACK_COLOR_ALPHA},
};



gchar* parse_request_body(const gchar* json_data, const gchar* path);

guint parse_image(const char* img);
guint parse_video(const char* video);
gchar* parse_watermark(const gchar* label, guint* watermark_dynamic_status);

void http_server_callback(SoupServer *server, SoupMessage *msg, 
                          const char *path, GHashTable *query,
                          SoupClientContext *client, gpointer user_data);

void http_server_init(gpointer data);

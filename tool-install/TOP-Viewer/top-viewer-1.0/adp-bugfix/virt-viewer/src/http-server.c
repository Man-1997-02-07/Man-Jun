#include "http-server.h"

gchar*
parse_request_body(const char* json_data, const gchar* path)
{
    gchar* result = NULL;
    JsonParser *parser = json_parser_new();
    JsonNode *node = json_node_new(JSON_NODE_OBJECT);
    if ( !json_parser_load_from_data (parser, json_data, strlen(json_data), NULL) ) {
        g_message("json_parser_load_from_data Error!\n");
    }
    node = json_parser_get_root(parser);
    JsonObject *obj = json_object_new();  
    obj = json_node_get_object(node);
    if ( json_object_has_member(obj, "resolution") || json_object_has_member(obj, "disinput") || 
         json_object_has_member(obj, "compression") || json_object_has_member(obj, "videocode") ||
         json_object_has_member(obj, "watermark") || json_object_has_member(obj, "content") ) {

        if (g_str_equal (path, "/top-viewer/resize")) {
            result = json_object_get_string_member(obj, "resolution");
        }
        if (g_str_equal (path, "/top-viewer/disinput")) {
            result = json_object_get_string_member(obj, "disinput");
        }
        if (g_str_equal (path, "/top-viewer/change_image_compression")) {
            result = json_object_get_string_member(obj, "compression");
        }
        if (g_str_equal (path, "/top-viewer/change_video_code")) {
            result = json_object_get_string_member(obj, "videocode");
        }
        if (g_str_equal (path, "/top-viewer/change_watermark")) {
            result = json_object_get_string_member(obj, "watermark");
        }
        if (g_str_equal (path, "/top-viewer/notify") ){
            result = json_object_get_string_member(obj, "content");
        }
        return g_strdup(result);
    }else{
        g_message("argument error!\n");
    }
}

guint parse_image(const char* img)
{
     for (guint i = 0; imagewords[i].name; i++){
         if (g_str_equal(img, imagewords[i].name))
             return imagewords[i].imgcode;
     }
}

guint parse_video(const char* video)
{
    for (guint i = 0; videowords[i].name; i++){
        if (g_str_equal(video, videowords[i].name))
            return videowords[i].videocode;
    }
}

gchar* parse_watermark(const gchar* label, guint* watermark_dynamic_status)
{
    gchar **infos = g_strsplit(label, ",", 12);
    guint infos_cnt = g_strv_length(infos);
    gdouble alpha_percent = 1;
    guint alpha = 65535;
    gfloat watermark_x = 0.0;//水印自定义位置-横坐标
    gfloat watermark_y = 0.0;//水印自定义位置-纵坐标
    for (guint i = 0; i < infos_cnt; ++i) {
        if (watermarkwords[i].watermark == BACK_COLOR_ALPHA) {
            alpha_percent = g_ascii_strtod(infos[i], NULL);
            if (1.0 == alpha_percent){
                alpha = 1;
            }else{
                alpha *= (1.0-alpha_percent);
            }
        }
        watermarkwords[i].value = g_strdup(infos[i]);
    }

    watermark_x = g_ascii_strtod(infos[10], NULL);//x
    if (watermark_x > 1.0)
	    watermark_x = g_random_double_range(0.0, 1.0);

    watermark_y = g_ascii_strtod(infos[11], NULL);//y
    if (watermark_y > 1.0)
	    watermark_y = g_random_double_range(0.0, 1.0);

    if ( 0 == g_ascii_strtoll(watermarkwords[1].value, NULL, 10)) {//interval参数为0,水印改为静止。
	    virt_viewer_window_change_watermark_to_static(1, watermark_x, watermark_y);//x,y
	    }else{
		    *watermark_dynamic_status = g_ascii_strtoll(watermarkwords[1].value, NULL, 10);
	    }

    gchar *result = g_markup_printf_escaped("<span font_family='%s' style='%s' foreground='%s' weight='%s' font='%s' background='%s' background_alpha='%u' underline='%s'>%s</span>",
                                            watermarkwords[6].value, watermarkwords[2].value, watermarkwords[5].value, watermarkwords[4].value,
                                            watermarkwords[3].value, watermarkwords[8].value, alpha, watermarkwords[7].value, watermarkwords[0].value);

    return g_strdup(result);
}


void
http_server_callback(SoupServer *server, SoupMessage *msg, 
                     const char *path, GHashTable *query,
                     SoupClientContext *client, gpointer user_data)
{
    g_message("path = %s\n",path);
    g_message("client Request method is %s\n", msg->method);//请求方法

    if (0 == strlen(msg->request_body->data)) {//请求体为空直接返回
            g_message("request_body is empty \n");
        soup_message_set_status (msg, SOUP_STATUS_OK);
        return;
    }

    if (g_str_equal (path, "/top-viewer/resize")) {//通过path区分请求，参数都在请求体中
        gchar *result;
        const gchar *value;
        guint resolution_width;
        guint resolution_height;

        result = parse_request_body(msg->request_body->data, path);//解析json数据，有待完善
        value = g_hash_table_lookup (query, "resolution");//解析path后的KV参数

        if (parse_resolution_from_string_to_guint(result, &resolution_width, &resolution_height)) {
            virt_viewer_app_change_guest_desktop_resolution((VirtViewerApp *)(((Httpdata*)user_data)->app), resolution_width, resolution_height);
        }else{
            virt_viewer_app_change_guest_desktop_resolution((VirtViewerApp *)(((Httpdata*)user_data)->app), 1920, 1080);
        }

        if (NULL != value )
            soup_message_set_response (msg, "top/code", SOUP_MEMORY_COPY,
                                       value, strlen(value));
        soup_message_set_status (msg, SOUP_STATUS_OK);
    }else if(g_str_equal (path, "/top-viewer/disinput")) {
        gchar *result;
        const gchar *value;

        result = parse_request_body(msg->request_body->data, path);//YES：禁止输入；NO：允许输入
                value = g_hash_table_lookup (query, "disinput");
       
        if (g_str_equal(result, "true")) {
            g_message("result is true\n");
            virt_viewer_app_set_disable_inputs((VirtViewerApp *)(((Httpdata *)user_data)->app), TRUE);
        }

        if (g_str_equal(result, "false")) {
            g_message("result is false\n");
            virt_viewer_app_set_disable_inputs((VirtViewerApp *)(((Httpdata *)user_data)->app), FALSE);
        }
        
        if (NULL != value)
            soup_message_set_response (msg, "top/code", SOUP_MEMORY_COPY,
                                       value, strlen(value));
        soup_message_set_status(msg, SOUP_STATUS_OK);
    }else if (g_str_equal (path, "/top-viewer/change_image_compression")){
        gchar *result;
        guint code;
        const gchar *value;

        value = g_hash_table_lookup (query, "compression");
        result = parse_request_body(msg->request_body->data, path);
        code = parse_image(result);
        switch (code) {
        case AUTOGLZ:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[AUTOGLZ].name);
            break;
        case AUTOLZ:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[AUTOLZ].name);
            break;
        case QUIC:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[QUIC].name);
            break;
        case GLZ:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[GLZ].name);
            break;
        case LZ:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[LZ].name);
            break;
        case LZ4:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[LZ4].name);
            break;
        case OFF:
            virt_viewer_app_set_preferred_compression((VirtViewerApp *)(((Httpdata *)user_data)->app), imagewords[OFF].name);
            break;
        }

        if (NULL != value)
            soup_message_set_response (msg, "top/code", SOUP_MEMORY_COPY,
                                       value, strlen(value));
        soup_message_set_status(msg, SOUP_STATUS_OK);
    } else if (g_str_equal (path, "/top-viewer/change_video_code")) {
        gchar *result;
        guint code;
        const gchar *value;

        value = g_hash_table_lookup (query, "video");
        result = parse_request_body(msg->request_body->data, path);
        code = parse_video(result);
        switch (code) {
        case MJPEG:
            virt_viewer_app_set_video_codec((VirtViewerApp *)(((Httpdata *)user_data)->app), videowords[MJPEG].name);
            break;
        case VP8:
            virt_viewer_app_set_video_codec((VirtViewerApp *)(((Httpdata *)user_data)->app), videowords[VP8].name);
            break;
        case VP9:
            virt_viewer_app_set_video_codec((VirtViewerApp *)(((Httpdata *)user_data)->app), videowords[VP9].name);
            break;
        case H264:
            virt_viewer_app_set_video_codec((VirtViewerApp *)(((Httpdata *)user_data)->app), videowords[H264].name);
            break;
        }
        if (NULL != value)
            soup_message_set_response (msg, "top/code", SOUP_MEMORY_COPY,
                                       value, strlen(value));
        soup_message_set_status(msg, SOUP_STATUS_OK);
    } else if(g_str_equal (path, "/top-viewer/change_watermark")){
        gchar *result, *watermark_info;
        const gchar *value;
        guint watermark_dynamic_status = 0;

        value = g_hash_table_lookup (query, "watermark");
        result = parse_request_body(msg->request_body->data, path);
        g_message("%s\n", result);
        watermark_info = parse_watermark(result, &watermark_dynamic_status);
        g_message("watermark_info = %s\n", watermark_info);
        
        if (watermark_dynamic_status) {
            virt_viewer_app_set_window_watermark_to_dynamic((VirtViewerApp *)(((Httpdata *)user_data)->app), watermark_dynamic_status);
        }


        vitr_viewer_app_set_window_watermark((VirtViewerApp *)(((Httpdata *)user_data)->app), watermark_info);

        if (NULL != value)
            soup_message_set_response (msg, "top/code", SOUP_MEMORY_COPY,
                                       value, strlen(value));
        soup_message_set_status(msg, SOUP_STATUS_OK);
    } else if(g_str_equal (path, "/top-viewer/notify")){
        gchar *result;
        result = parse_request_body(msg->request_body->data, path);
        urldecode(result);
        g_debug("notify window's content: %s\n", result);
        virt_viewer_app_show_report(result);
        soup_message_set_status(msg, SOUP_STATUS_OK);
    } else {
        g_message ("path is NULL\n");
        soup_message_set_status (msg, SOUP_STATUS_OK);
    }
}

void
http_server_init(gpointer data)
{
    SoupServer *server;
    GError error;
    char *http_aliases[] = { "dav", NULL };

    server = soup_server_new (SOUP_SERVER_HTTP_ALIASES, http_aliases,NULL);

    if (!soup_server_listen_all(server, (0 == ((Httpdata *)data)->listen_port)?0:((Httpdata *)data)->listen_port, 0, &error)) //暂时设置监听ipv4和ipv6
    {
        g_message("listen_all  error \n");
    }else{
        GSList *uris;
        SoupURI *uri;
        uris = soup_server_get_uris (server);
        guint number = g_slist_length (uris);
        g_message ("the number of uris = %u\n", number);
        for(guint i = 0; i < number; ++i){
            uri = uris->data;
            g_message ("HTTP_SERVRR_PORT = %u\n", uri->port);
            uris = uris->next;
        }
    }
    soup_server_add_handler(server, NULL, http_server_callback, data, NULL);
}


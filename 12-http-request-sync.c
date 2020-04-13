/* 12-http-request-sync.c --- HTTP 请求（同步） */
#include <gtk/gtk.h>
#include <libsoup/soup.h>
#include <stdio.h>

static void entry_callback(GtkWidget* widget,
                           gpointer user_data) {
  gchar const* url = gtk_entry_get_text(GTK_ENTRY(widget));
  g_print("url: %s\n", url);
  SoupSession* session = soup_session_new();
  SoupMessage* message = soup_message_new("GET", url);
  guint status = soup_session_send_message(session, message);
  g_print("status: %u\n", status);

  GBytes* body = 0;
  g_object_get(G_OBJECT(message), "response-body-data", &body, (void*)0);
  g_assert(body);
  gsize size = 0;
  gconstpointer data = g_bytes_get_data(body, &size);
  if (data && size) {
    GtkWidget* view = user_data;
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, data, size);
  }

  /* XXX 这是什么？必须要吗？ 理解 https://stackoverflow.com/a/2849391/2999892 */
  g_object_unref(message);
  g_object_unref(session);
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "发送 HTTP 请求（同步）");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);

  /* 布局 */
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  /* 元素 */
  GtkWidget* entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(entry), "http://example.com/");
  GtkWidget* view = gtk_text_view_new();
  g_signal_connect(entry, "activate", G_CALLBACK(entry_callback), view);
  // g_object_set(G_OBJECT(view), "editable", FALSE, (void*)0);

  gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), view, TRUE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(window), box);
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

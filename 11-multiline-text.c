/* 11-multiline-text.c --- 跨行文本框 */
#include <gtk/gtk.h>

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "跨行文本框");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

  /* 添加点边距 */
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);

  /* 元素 */
  GtkWidget* view = gtk_text_view_new();
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  gtk_text_buffer_set_text(buffer, "你好，跨行文本框\n\n  怎么设置字体大小呢", -1);

  gtk_container_add(GTK_CONTAINER(window), view);
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

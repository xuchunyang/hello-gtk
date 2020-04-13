/* 01-hello.c --- 显示窗口 */
#include <gtk/gtk.h>

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  /* 显示窗口 */
  gtk_widget_show(window);

  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

/* 02-label.c --- 标签 */
#include <gtk/gtk.h>

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "标签测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  
  /* 创建标签 */
  GtkWidget* label = gtk_label_new("你好，标签！");

  /* 添加标签 */
  gtk_container_add(GTK_CONTAINER(window), label);

  /* 显示标签 */
  gtk_widget_show(label);

  /* 显示窗口 */
  gtk_widget_show(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

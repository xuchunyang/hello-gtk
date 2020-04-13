/* 09-separator.c --- 间隔线 */
#include <gtk/gtk.h>

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "间隔线测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);

  /* 容器 */
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  
  /* 元素 */
  GtkWidget* label1 = gtk_label_new("第一个标签");
  GtkWidget* separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  GtkWidget* label2 = gtk_label_new("第二个标签");

  /* 布局 */
  gtk_box_pack_start(GTK_BOX(box), label1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), separator, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), label2, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(window), box);
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

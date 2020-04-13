/* 03-button.c --- 按钮 */
#include <gtk/gtk.h>

static void print_hello(GtkWidget* widget __attribute__((unused)),
                        gpointer data __attribute__((unused))) {
  g_print("不要再点我了！\n");
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "按钮测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  
  /* 创建标签 */
  GtkWidget* button = gtk_button_new_with_label("按钮测试");

  /* 点击按钮时，打印信息 */
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), 0);
  
  /* 添加标签 */
  gtk_container_add(GTK_CONTAINER(window), button);

  /* 显示窗口和它上面附着的所有 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

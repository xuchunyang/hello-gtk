/* 05-input.c --- 用户输入 */
#include <gtk/gtk.h>

static void echo(GtkWidget* widget,
                 gpointer data __attribute__((unused))) {
  g_print("%s\n", gtk_entry_get_text(GTK_ENTRY(widget)));
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "输入测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);
  
  /* 创建、添加输入框 */
  GtkWidget* text = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(window), text);

  /* 处理 Enter */
  g_signal_connect(text, "activate", G_CALLBACK(echo), 0);
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

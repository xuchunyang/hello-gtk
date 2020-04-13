/* 08-progress-bar.c --- 进度条 */
#include <gtk/gtk.h>

static gboolean fill(gpointer user_data) {
  GtkWidget* progress_bar = user_data;

  /* 获得当前进度 */
  gdouble fraction =
    gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progress_bar));

  /* + 10% */
  fraction += 0.1;
  g_print("%g\n", fraction*100);

  /* 更新进度 */
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR (progress_bar), fraction);

  /* 如果达到 100% 返回 false 结束计时器；否则返回 true 继续计时器 */
  return fraction < 1.0;
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "进度条测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 20);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);

  /* 创建元素 */
  GtkWidget* progress_bar = gtk_progress_bar_new();
  
  /* 添加元素 */
  gtk_container_add(GTK_CONTAINER(window), progress_bar);

  /* 每 500 毫米更新一次 */
  g_timeout_add(500, fill, GTK_PROGRESS_BAR(progress_bar));
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

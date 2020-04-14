/* 14-events.c --- 各种事件（窗口移动、点击、拖拽）*/
#include <gtk/gtk.h>

static void clicked_cb(GtkWidget* widget __attribute__((unused)),
                       gpointer data __attribute__((unused))) {
  g_print("不要再点我了！\n");
}

static void button_enter_cb(GtkButton* button,
                            gpointer data __attribute__((unused))) {
  gtk_button_set_label(button, "不要点我！");
}

static void button_leave_cb(GtkButton* button,
                            gpointer data __attribute__((unused))) {
  gtk_button_set_label(button, "点我！");
}

static void window_button_press_cb(GtkWindow* window,
                                   GdkEvent *event,
                                   gpointer data __attribute__((unused))) {
  if (event->type == GDK_BUTTON_PRESS) {
    /* 查看 GdkEventButton 结构
       https://developer.gnome.org/gtk-tutorial/stable/a2767.html */
    GdkEventButton* be = (GdkEventButton*) event;
    g_print("button id: %u\n", be->button);
    g_assert(be->button == 1);
    gtk_window_begin_move_drag
      (window, be->button, be->x_root, be->y_root, be->time);
  }
}

static void configure_cb(GtkWindow* window,
                         GdkEvent *event,
                         gpointer user_data __attribute__((unused))){
  /* https://developer.gnome.org/gtk-tutorial/stable/a2767.html */
  /* https://developer.gnome.org/gdk3/stable/gdk3-Events.html */
  // g_print("Event: %d\n", gdk_event_get_event_type(event));

  /* Generated when a window size or position has changed. */
  if (gdk_event_get_event_type(event) == GDK_CONFIGURE) {
    GdkEventConfigure* e = (GdkEventConfigure*) event;
    g_assert(e->type == GDK_CONFIGURE);
    g_print("%d, %d | %d, %d\n", e->x, e->y, e->width, e->height);

    /* 设置标题 */
    GString *buf = g_string_new(0);
    g_string_printf(buf, "%d, %d\t%d, %d\n", e->x, e->y, e->width, e->height);
    gtk_window_set_title(window, buf->str);
    g_string_free(buf, TRUE);
  }
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "事件测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget* button = gtk_button_new_with_label("Button");

  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

  /* 按钮点击事件 */
  g_signal_connect(button, "clicked", G_CALLBACK(clicked_cb), 0);

  /* 按钮进入事件 */
  /* XXX 这两个信号已经被淘汰了，该用 enter-notify-event，例子 https://gist.github.com/bert/260034 */
  g_signal_connect(button, "enter", G_CALLBACK(button_enter_cb), 0);
  g_signal_connect(button, "leave", G_CALLBACK(button_leave_cb), 0);
  
  /* 窗口移动、缩放事件 */
  g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(configure_cb), 0);

  g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(window_button_press_cb), 0);
  
  /* 显示窗口和它上面附着的所有 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

/* 10-tooltip.c --- 提示符 */
#include <gtk/gtk.h>

static gboolean hover(GtkWidget* widget __attribute__((unused)),
                      gint x,
                      gint y,
                      gboolean keyboard_mode,
                      GtkTooltip* tooltip,
                      gpointer user_data) {
  static unsigned count = 0;
  g_print("hover！ count = %u, x = %d, y = %d, keyboard = %d\n", count, x, y, keyboard_mode);
  /* 图标没用（不显示） 好像需要安装 GTK 主题 https://stackoverflow.com/a/7541503/2999892 */
  /* gtk_tooltip_set_icon_from_icon_name(tooltip, "battery", GTK_ICON_SIZE_BUTTON); */
  GtkWidget* image = user_data;
  gtk_tooltip_set_custom(tooltip, image);
  count++;
  return TRUE;
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "提示符测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);

  /* 容器 */
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  
  /* 元素 */
  GtkWidget* label1 = gtk_label_new("鼠标放到我上面试试 (纯文本 tooltip)");
  GtkWidget* label2 = gtk_label_new("还有我 (markup tooltip)");
  GtkWidget* label3 = gtk_label_new("还有我 (自定义 tooltip)");

  /* 设置 tooltip */
  gtk_widget_set_tooltip_text(label1, "Hello, Tooltip!");
  gtk_widget_set_tooltip_markup
    (label2,
     /* https://developer.gnome.org/pygtk/stable/pango-markup-language.html */
     "<s><i><span size=\"large\" foreground=\"red\">Strikethrough</span></i></s>");

  /* 设置 label3 的 “has-tooltip” 为 TRUE */
  GValue a = {0};
  g_value_init(&a, G_TYPE_BOOLEAN);
  g_value_set_boolean(&a, TRUE);
  g_object_set_property(G_OBJECT(label3), "has-tooltip", &a);
  g_value_unset(&a);

  GtkWidget* image = gtk_image_new_from_file("GTK_logo.svg");

  /* 不加这句，后面 gtk_tooltip_set_custom 会 SIGSEGV */
  g_object_ref(image);

  g_signal_connect(label3, "query-tooltip", G_CALLBACK(hover), image);
  
  /* 布局 */
  gtk_box_pack_start(GTK_BOX(box), label1, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), label2, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), label3, TRUE, TRUE, 0);
  gtk_container_add(GTK_CONTAINER(window), box);
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

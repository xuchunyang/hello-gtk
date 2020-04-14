/* 15-dialog.c --- 对话框 */
#include <gtk/gtk.h>

static void button1_clicked_cb(GtkButton* button __attribute__((unused)),
                               gpointer user_data) {
  GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  GtkWindow* parent = user_data;
  GtkWidget* dialog = gtk_dialog_new_with_buttons
    ("Message", parent, flags, "_OK", GTK_RESPONSE_NONE, (void*)0);
  GtkWidget* label = gtk_label_new("Hello, world!");
  g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);
  GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_container_add(GTK_CONTAINER(content_area), label);
  gtk_widget_show_all(dialog);
}

static void checkout(GtkDialog *dialog,
                     gint response_id,
                     gpointer user_data __attribute__((unused))) {
  static gboolean shown = FALSE;
  if (!shown) {
    shown = TRUE;
    /* DEBUG 各种响应 https://developer.gnome.org/gtk3/stable/GtkDialog.html#GtkResponseType */
    g_print("GTK_RESPONSE_NONE: %d\n", GTK_RESPONSE_NONE);
    g_print("GTK_RESPONSE_REJECT: %d\n", GTK_RESPONSE_REJECT);
    g_print("GTK_RESPONSE_ACCEPT: %d\n", GTK_RESPONSE_ACCEPT);
    g_print("GTK_RESPONSE_DELETE_EVENT: %d\n", GTK_RESPONSE_DELETE_EVENT);
    g_print("GTK_RESPONSE_OK: %d\n", GTK_RESPONSE_OK);
    g_print("GTK_RESPONSE_CANCEL: %d\n", GTK_RESPONSE_CANCEL);
    g_print("GTK_RESPONSE_CLOSE: %d\n", GTK_RESPONSE_CLOSE);
    g_print("GTK_RESPONSE_YES: %d\n", GTK_RESPONSE_YES);
    g_print("GTK_RESPONSE_NO: %d\n", GTK_RESPONSE_NO);
    g_print("GTK_RESPONSE_APPLY: %d\n", GTK_RESPONSE_APPLY);
    g_print("GTK_RESPONSE_HELP: %d\n", GTK_RESPONSE_HELP);
  }

  switch (response_id) {
  case GTK_RESPONSE_OK:
    g_print("已付款\n");
    break;
  case GTK_RESPONSE_CANCEL:
  case GTK_RESPONSE_DELETE_EVENT:
    g_print("未付款\n");
    break;
  default:
    g_print("ERROR response_id %d\n", response_id);
    break;
  }
  /* 关闭对话框 */
  gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void button2_clicked_cb(GtkWindow* parent_window) {
  GtkWidget* dialog = gtk_dialog_new_with_buttons ("对话框",
                                                   parent_window,
                                                   /* XXX 看看还有什么其它的 */
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   "OK",
                                                   GTK_RESPONSE_OK,
                                                   "Cancel",
                                                   GTK_RESPONSE_CANCEL,
                                                   (void*)0);
  /* 添加个说明 */
  GtkWidget* label = gtk_label_new("确认付款吗？");
  GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  gtk_container_add(GTK_CONTAINER(content_area), label);

  g_signal_connect(dialog, "response", G_CALLBACK(checkout), 0);

  gtk_widget_show_all(dialog);
}

static void button3_clicked_cb(GtkWindow* parent_window) {
  GtkWidget* dialog = gtk_message_dialog_new(parent_window,
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_QUESTION,
                                             GTK_BUTTONS_YES_NO,
                                             "Are you ready?");
  gint result = gtk_dialog_run(GTK_DIALOG(dialog));
  /* https://developer.gnome.org/gtk3/stable/GtkDialog.html#GtkResponseType */
  switch (result) {
  case GTK_RESPONSE_YES:
    g_print("Yes\n");
    break;
  case GTK_RESPONSE_NO:
    g_print("No\n");
    break;
  case GTK_RESPONSE_DELETE_EVENT:
    g_print("未选择，关闭窗口\n");
    break;
  default:
    g_print("ERROR: response_id %d\n", result);
    break;
  }
  gtk_widget_destroy(dialog);
}

static char const*const authors[] = {"Xu Chunyang"};
static char const*const artists[] = {"Blah Blah", "Blah"};

static void button4_clicked_cb(GtkWindow* parent_window) {
  GError* error = 0;
  GdkPixbuf* logo = gdk_pixbuf_new_from_file("GTK_logo.svg", &error);
  if (!logo) {
    g_print("gdk_pixbuf_new_from_file failed: %s\n", error->message);
    g_error_free(error);
  } else {
    gtk_show_about_dialog(parent_window,
                          "program-name", "15-dialog",
                          "logo", logo,
                          "comments", "演示各种对话框",
                          "authors", authors,
                          "artists", artists,
                          "license-type", GTK_LICENSE_GPL_3_0,
                          "website", "https://github.com/xuchunyang/hello-gtk",
                          "version", "1.1",
                          (void*)0);
    g_object_unref(logo);
  }
}

static void button5_clicked_cb(GtkWindow* parent_window) {
  GtkWidget* dialog = gtk_color_chooser_dialog_new("选颜色", parent_window);
  gint result = gtk_dialog_run(GTK_DIALOG(dialog));
  if (result == GTK_RESPONSE_OK) {
    GtkColorChooser* chooser = GTK_COLOR_CHOOSER(dialog);
    GdkRGBA color = {0};
    gtk_color_chooser_get_rgba(chooser, &color);
    g_print("RGBA: %g %g %g %g\n",
            color.red, color.green, color.blue, color.alpha);
  }
  gtk_widget_destroy(dialog);
}

static void button6_clicked_cb(GtkWindow* parent_window) {
  GtkWidget* dialog = gtk_font_chooser_dialog_new("选字体", parent_window);
  gint result = gtk_dialog_run(GTK_DIALOG(dialog));
  if (result == GTK_RESPONSE_OK) {
    GtkFontChooser* chooser = GTK_FONT_CHOOSER(dialog);
    gchar const* font = gtk_font_chooser_get_font(chooser);
    g_print("Font: %s\n", font);
  } else {
    g_print("你没有选字体, %d\n", result);
  }
  gtk_widget_destroy(dialog);
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "对话框测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  GtkWidget* button1 = gtk_button_new_with_label("对话框 1 自制");
  GtkWidget* button2 = gtk_button_new_with_label("对话框 2 返回值");
  GtkWidget* button3 = gtk_button_new_with_label("对话框 3 GtkMessageDialog");
  GtkWidget* button4 = gtk_button_new_with_label("对话框 4 GtkAboutDialog");
  GtkWidget* button5 = gtk_button_new_with_label("对话框 5 GtkColorChooserDialog");
  GtkWidget* button6 = gtk_button_new_with_label("对话框 5 GtkFontChooserDialog");

  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), button3, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), button4, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), button5, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), button6, FALSE, FALSE, 0);

  g_signal_connect(button1, "clicked", G_CALLBACK(button1_clicked_cb), window);
  g_signal_connect_swapped(button2, "clicked", G_CALLBACK(button2_clicked_cb), window);
  g_signal_connect_swapped(button3, "clicked", G_CALLBACK(button3_clicked_cb), window);
  g_signal_connect_swapped(button4, "clicked", G_CALLBACK(button4_clicked_cb), window);
  g_signal_connect_swapped(button5, "clicked", G_CALLBACK(button5_clicked_cb), window);
  g_signal_connect_swapped(button6, "clicked", G_CALLBACK(button6_clicked_cb), window);
  
  /* 显示窗口和它上面附着的所有 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

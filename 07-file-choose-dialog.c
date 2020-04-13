/* 07-file-choose-dialog.c --- 文件选择器 */
#include <gtk/gtk.h>

static void on_button1_clicked(GtkWidget* parent_window) {
  GtkWidget* dialog =
    gtk_file_chooser_dialog_new ("Open File",
                                 GTK_WINDOW(parent_window),
                                 GTK_FILE_CHOOSER_ACTION_OPEN,
                                 "_Cancel",
                                 GTK_RESPONSE_CANCEL,
                                 "_Open",
                                 GTK_RESPONSE_ACCEPT,
                                 (void*)0);
  gint res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    gchar *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    g_print("[button1] You selected: %s\n", filename);
    g_free (filename);    
  }
  gtk_widget_destroy(dialog);
}

static void on_button2_clicked(GtkWidget* parent_window) {
  GtkFileChooserNative* native =
    gtk_file_chooser_native_new("Open Image",
                                GTK_WINDOW(parent_window),
                                GTK_FILE_CHOOSER_ACTION_OPEN,
                                "_Open",
                                "_Cancel");
  gint res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(native));
  if (res == GTK_RESPONSE_ACCEPT) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
    gchar* filename = gtk_file_chooser_get_filename(chooser);
    g_print("[button2] You selected: %s\n", filename);
    g_free(filename);
  }
  g_object_unref(native);
}

int main(void) {
  /* 初始化，一般传命令行参数，我这用不上 */
  gtk_init(0, 0);

  /* 创建窗口 */
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* 窗口居中 */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* 窗口标题 */
  gtk_window_set_title(GTK_WINDOW(window), "选择文件测试");
  
  /* 设置窗口大小 */
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
  
  /* 用户关闭窗口时，退出 GTK 循环 */
  g_signal_connect(window, "destroy", gtk_main_quit, 0);

  /* 单列容器 */
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  
  /* 创建元素 */
  GtkWidget* button1 = gtk_button_new_with_label("选择文件 - GtkFileChooserDialog - GTK样式");
  GtkWidget* button2 = gtk_button_new_with_label("选择文件 - GtkFileChooserNative - 系统原生");

  /* 按钮点击 */
  g_signal_connect_swapped(button1, "clicked", G_CALLBACK(on_button1_clicked), window);
  g_signal_connect_swapped(button2, "clicked", G_CALLBACK(on_button2_clicked), window);

  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 0);
  
  /* 添加容器 */
  gtk_container_add(GTK_CONTAINER(window), box);
  
  /* 显示窗口 */
  gtk_widget_show_all(window);
  
  /* 进入 GTK 循环，堵塞直到循环结束 */
  gtk_main();
}

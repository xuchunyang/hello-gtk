/* 30-headerbar.c --- 标题栏 */
#include <gtk/gtk.h>

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Headerbar 例子");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* hb = gtk_header_bar_new();
  g_object_set(hb, "title", "Header Example", (void*)0);
  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(hb), TRUE);
  gtk_window_set_titlebar(GTK_WINDOW(window), hb);

  GtkWidget* button = gtk_button_new_from_icon_name
     /* brew ls adwaita-icon-theme | grep mail-send */
    ("mail-send-receive-symbolic", GTK_ICON_SIZE_BUTTON);
  gtk_header_bar_pack_end(GTK_HEADER_BAR(hb), button);

  GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  GtkWidget* button2 = gtk_button_new_from_icon_name
    ("go-next-symbolic", GTK_ICON_SIZE_BUTTON);
  gtk_box_pack_start(GTK_BOX(hbox), button2, FALSE, FALSE, 0);
  GtkWidget* button3 = gtk_button_new_from_icon_name
    ("go-previous-symbolic", GTK_ICON_SIZE_BUTTON);
  gtk_box_pack_start(GTK_BOX(hbox), button3, FALSE, FALSE, 0);
  gtk_header_bar_pack_start(GTK_HEADER_BAR(hb), hbox);

  gtk_widget_show_all(window);
  gtk_main();
}

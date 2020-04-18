/* 36-gtk-css.c --- GTK CSS */
#include "gtk/gtkcssprovider.h"
#include <gtk/gtk.h>

/* https://stackoverflow.com/a/41850551/2999892 */
int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GTK CSS Demo");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget* button1 = gtk_button_new_with_label("Red");
  GtkWidget* button2 = gtk_button_new_with_label("Yellow");
  GtkWidget* button3 = gtk_button_new_with_label("Green");
  GtkWidget* button4 = gtk_button_new_with_label("Blue");

  /* 自动伸缩 */
  gtk_widget_set_hexpand(button1, TRUE);
  gtk_widget_set_vexpand(button1, TRUE);
  gtk_widget_set_hexpand(button4, TRUE);
  gtk_widget_set_vexpand(button4, TRUE);

  /* CSS ID */
  gtk_widget_set_name(button1, "myButton_red");
  gtk_widget_set_name(button2, "myButton_yellow");
  gtk_widget_set_name(button3, "myButton_green");
  gtk_widget_set_name(button4, "myButton_blue");

  gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button4, 1, 1, 1, 1);

  GtkCssProvider* provider = gtk_css_provider_new();
  GdkDisplay *display = gdk_display_get_default();
  GdkScreen* screen = gdk_display_get_default_screen(display);
  gtk_css_provider_load_from_path(provider, "36-gtk-css.css", 0);
  gtk_style_context_add_provider_for_screen
    (screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_widget_show_all(window);
  gtk_main();
}

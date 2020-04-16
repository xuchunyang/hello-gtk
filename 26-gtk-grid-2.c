/* 26-gtk-grid-2.c --- 复杂一点点的网格布局 */
/* 复制效果
   https://python-gtk-3-tutorial.readthedocs.io/en/latest/layout.html#grid */
#include <gtk/gtk.h>

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Grid Example");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget* button1 = gtk_button_new_with_label("Button 1");
  GtkWidget* button2 = gtk_button_new_with_label("Button 2");
  GtkWidget* button3 = gtk_button_new_with_label("Button 3");
  GtkWidget* button4 = gtk_button_new_with_label("Button 4");
  GtkWidget* button5 = gtk_button_new_with_label("Button 5");
  GtkWidget* button6 = gtk_button_new_with_label("Button 6");

  gtk_grid_attach(GTK_GRID(grid), button1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button2, 1, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), button3, 0, 1, 1, 2);
  gtk_grid_attach(GTK_GRID(grid), button4, 1, 1, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), button5, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), button6, 2, 2, 1, 1);

  gtk_widget_show_all(window);
  gtk_main();
}

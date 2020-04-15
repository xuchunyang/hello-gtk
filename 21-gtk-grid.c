/* 21-gtk-grid.c --- 网格布局 */
#include <gtk/gtk.h>

char const*const values[][4] = {
  {"7", "8", "9", "/"},
  {"4", "5", "6", "*"},
  {"1", "2", "3", "-"},
  {"0", ".", "=", "+"},
};

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkGrid 演示");
  gtk_window_set_default_size(GTK_WINDOW(window), 250, 180);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(window), grid);

  g_assert((sizeof values)/(sizeof *values) == 4);
  g_assert((sizeof values[0])/(sizeof *values[0]) == 4);
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      g_print("%s, ", values[i][j]);
      GtkWidget* button = gtk_button_new_with_label(values[i][j]);
      gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
    }
    g_print("\n");
  }

  g_message("default   row spacing: %d", gtk_grid_get_row_spacing(GTK_GRID(grid)));
  g_message("default colum spacing: %d", gtk_grid_get_column_spacing(GTK_GRID(grid)));
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

  gtk_widget_show_all(window);
  gtk_main();
}

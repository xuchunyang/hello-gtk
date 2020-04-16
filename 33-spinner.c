/* 33-spinner.c --- Spinner */
#include <gtk/gtk.h>

static void on_toggled(GtkWidget* button, gpointer data){
  GtkWidget* spinner = data;
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
    gtk_spinner_start(GTK_SPINNER(spinner));
  else
    gtk_spinner_stop(GTK_SPINNER(spinner));
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Spinner Demo");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 3);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* button = gtk_toggle_button_new_with_label("Start Spinning");
  gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

  GtkWidget* spinner = gtk_spinner_new();
  gtk_box_pack_start(GTK_BOX(box), spinner, TRUE, TRUE, 20);

  g_signal_connect(button, "toggled", G_CALLBACK(on_toggled), spinner);

  gtk_widget_show_all(window);
  gtk_main();
}

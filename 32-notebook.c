/* 32-notebook.c --- 标签栏 */
#include <gtk/gtk.h>

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "GtkNotebook 例子");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* notebook = gtk_notebook_new();
  gtk_container_add(GTK_CONTAINER(window), notebook);

  GtkWidget* page1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_set_border_width(GTK_CONTAINER(page1), 10);
  gtk_container_add(GTK_CONTAINER(page1), gtk_label_new("Default Page!"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page1, gtk_label_new("Plain Title"));

  GtkWidget* page2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_set_border_width(GTK_CONTAINER(page1), 10);
  gtk_container_add(GTK_CONTAINER(page2),
                    gtk_label_new("A page with an image for a Title."));
  gtk_notebook_append_page
    (GTK_NOTEBOOK(notebook), page2,
     /* brew ls adwaita-icon-theme | grep help-about */
     gtk_image_new_from_icon_name("help-about", GTK_ICON_SIZE_MENU));

  /* 默认是上面 GTK_POS_TOP */
  // g_object_set(notebook, "tab-pos", GTK_POS_LEFT, (void*)0);

  gtk_widget_show_all(window);
  gtk_main();
}

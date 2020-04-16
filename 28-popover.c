/* 28-popover.c --- 弹出窗口 */
#include <gtk/gtk.h>

static void on_clicked(GtkWidget* popover) {
  gtk_widget_show_all(popover);
  gtk_popover_popup(GTK_POPOVER(popover));
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Popover 测试");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  /* XXX 怎么限制 Button 的宽度？ */
  GtkWidget* button = gtk_button_new_with_label("Popover 测试");
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);
  gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

  GtkWidget* popover = gtk_popover_new(button);
  GtkWidget* box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(box2), gtk_label_new("Hi, I'm the Popover!"));
  GtkWidget* mb = gtk_model_button_new();
  g_object_set(mb, "text", "I'm a GtkModelButton!", (void*)0);
  gtk_container_add(GTK_CONTAINER(box2), mb);
  gtk_container_add(GTK_CONTAINER(box2), gtk_label_new("Hi, I'm the Popover!"));
  gtk_container_add(GTK_CONTAINER(popover), box2);
  gtk_popover_set_position(GTK_POPOVER(popover), GTK_POS_BOTTOM);

  g_signal_connect_swapped(button, "clicked", G_CALLBACK(on_clicked), popover);

  gtk_widget_show_all(window);
  gtk_main();
}

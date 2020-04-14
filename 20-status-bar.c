/* 20-status-bar.c --- 状态栏 */
#include <gtk/gtk.h>
#include <time.h>

static void update(GtkStatusbar* status) {
  time_t t = time(0);
  struct tm* tm = gmtime(&t);
  char* s = asctime(tm);
  gtk_statusbar_push (GTK_STATUSBAR(status),
                      gtk_statusbar_get_context_id(GTK_STATUSBAR(status), s),
                      s);
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "状态栏演示");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 320);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);
  
  GtkWidget* statusbar = gtk_statusbar_new();
  // g_object_set(statusbar, "vexpand", FALSE, (void*)0);
  /* g_object_set(statusbar, "valign", GTK_ALIGN_CENTER, (void*)0); */
  /* g_object_set(statusbar, "margin-top", 400, (void*)0); */
  gtk_box_pack_end(GTK_BOX(box), statusbar, FALSE, FALSE, 0);

  guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "context description here");
  guint id2 = gtk_statusbar_push(GTK_STATUSBAR(statusbar), id, "Hello, Statusbar!");

  g_message("context id: %u, %u", id, id2);
  update(GTK_STATUSBAR(statusbar));

  GtkWidget* button = gtk_button_new_with_label("更新状态栏");
  gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(update), statusbar);
  
  gtk_widget_show_all(window);
  gtk_main();
}

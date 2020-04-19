/* 42-async-countdown.c --- 倒计时，Async 编程之基本使用 */
#include <gtk/gtk.h>

static void button_clicked_cb(GtkWidget* button __attribute__((unused)),
                              gpointer   user_data) {
  g_message("button_clicked_cb");
  GCancellable* cancellable = user_data;
  g_cancellable_cancel(cancellable);
}

static void countdown_finish_cb(GObject* obj,
                                GAsyncResult* result,
                                gpointer user_data) {
  g_message("countdown_finish_cb");
  GError* error = 0;
  GSubprocess* proc = G_SUBPROCESS(obj);
  g_subprocess_wait_finish(proc, result, &error);
  if (error) {
    g_message("proc failed: %s", error->message);
    if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))
      g_message("canceled");
  } else {
    g_message("proc successed");
  }
  GCancellable* cancellable = user_data;
  g_clear_object(&cancellable);
  g_assert(!cancellable);
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "倒计时 5 秒");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* button = gtk_button_new_with_label("停止");
  gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

  GtkWidget* spinner = gtk_spinner_new();
  gtk_box_pack_start(GTK_BOX(box), spinner, TRUE, TRUE, 20);
  gtk_spinner_start(GTK_SPINNER(spinner));

  /* 开始计时 */
  GError* error = 0;
  GSubprocess* proc = g_subprocess_new(G_SUBPROCESS_FLAGS_NONE, &error, "sleep", "5", (void*)0);
  if (!proc) g_error("%s", error->message);

  GCancellable* cancellable = g_cancellable_new();
  g_subprocess_wait_async(proc, cancellable, countdown_finish_cb, cancellable);

  g_signal_connect(button, "clicked", G_CALLBACK(button_clicked_cb), cancellable);

  gtk_widget_show_all(window);
  gtk_main();
}

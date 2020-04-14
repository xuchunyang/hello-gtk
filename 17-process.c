/* 17-process.c --- 同步、异步 subprocess */
#include <gtk/gtk.h>
#include <unistd.h>

static void sync_test(gpointer data __attribute__((unused))) {
  g_print("sync_test: run command...\n");
  gchar* _stdout = 0;
  gchar* _stderr = 0;
  gint exit_status = 1;
  GError* error = 0;
  if (!g_spawn_command_line_sync("/bin/date", &_stdout, &_stderr, &exit_status, &error)) {
    g_print("error: %s\n", error->message);
    g_error_free(error);
    return;
  }
  g_print("stdout: %s\n", _stdout);
  g_print("stderr: %s\n", _stderr);
}

static void async_test(gpointer data __attribute__((unused))) {
  g_assert(g_spawn_command_line_async("/bin/sleep 3", 0));
}

void done_cb(GPid pid, gint status, gpointer data) {
  g_message("PID %d, exit with %d, 程序 %s\n", pid, status, (gchar *)data);
  g_spawn_close_pid(pid);
}

static void async_test_2(gpointer data __attribute__((unused))) {
  gchar* argv[] = {"/bin/sleep", "3", 0};
  GPid child_pid = 0;
  g_message("开始执行: %s %s", argv[0], argv[1]);
  g_assert(g_spawn_async(0, argv, 0, G_SPAWN_DO_NOT_REAP_CHILD, 0, 0, &child_pid, 0));
  g_print("pid: %d\n", child_pid);
  g_child_watch_add(child_pid, done_cb, argv[0]);
}

static void async_test_3(gpointer data __attribute__((unused))) {
  gchar* argv[] = {"tr", "a-z", "A-A", 0};
  GPid pid;
  g_message("开始执行: %s", argv[0]);
  gint _stdin;
  gint _stdout;
  gint _stderr;
  GError* error = 0;
  if (!g_spawn_async_with_pipes
      (0, argv, 0,
       G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH,
       0, 0, &pid, &_stdin, &_stdout, &_stderr, &error)) {
    g_error("%s", error->message);
    return;
  }
  g_print("pid: %d\n", pid);
  write(_stdin, "abcde", 5);
  /* XXX 不解决问题*/
  /* sleep(1); */
  close(_stdin);
  gchar buffer[5] = {0};
  g_assert(read(_stdout, buffer, sizeof buffer) == sizeof buffer);
  /* XXX 结果应该是 ABCDE， 实际却是 AAAAA*/
  g_message("结果：%s\n", buffer);
  g_child_watch_add(pid, done_cb, argv[0]);
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "子进程测试");
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 320);
  g_signal_connect_swapped(window, "destroy", gtk_main_quit, 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* button1 = gtk_button_new_with_label("同步测试 g_spawn_command_line_sync");
  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 0);
  g_signal_connect_swapped(button1, "clicked", G_CALLBACK(sync_test), 0);

  GtkWidget* button2 = gtk_button_new_with_label("异步测试 g_spawn_command_line_async");
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 0);
  g_signal_connect_swapped(button2, "clicked", G_CALLBACK(async_test), 0);

  GtkWidget* button3 = gtk_button_new_with_label("异步测试 g_spawn_async");
  gtk_box_pack_start(GTK_BOX(box), button3, FALSE, FALSE, 0);
  g_signal_connect_swapped(button3, "clicked", G_CALLBACK(async_test_2), 0);

  GtkWidget* button4 = gtk_button_new_with_label("异步测试 g_spawn_async_with_pipes");
  gtk_box_pack_start(GTK_BOX(box), button4, FALSE, FALSE, 0);
  g_signal_connect_swapped(button4, "clicked", G_CALLBACK(async_test_3), 0);
  
  gtk_widget_show_all(window);
  gtk_main();
}

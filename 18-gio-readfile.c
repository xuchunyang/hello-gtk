/* 18-gio-readfile.c --- 用 gio 读文件内容 */
#include <gtk/gtk.h>
#include <unistd.h>

static void readfile_internal(gchar const*const filename) {
  g_message("读取文件: %s", filename);
  GFile* file = g_file_new_for_path(filename);
  GError* error = 0;
  GFileInfo* info = g_file_query_info(file, G_FILE_ATTRIBUTE_ACCESS_CAN_READ, G_FILE_QUERY_INFO_NONE, 0, &error);
  if (!info) {
    g_message("%s", error->message);
    g_error_free(error);
    goto CLEANUP;
  }
  g_object_unref(info);
  error = 0;
  GFileInputStream* input_stream = g_file_read(file, 0, &error);
  if (!input_stream) {
    g_message("%s", error->message);
    g_error_free(error);
  }
  gchar const* buffer[1024];
  for (gint status;;) {
    error = 0;
    status = g_input_stream_read(G_INPUT_STREAM(input_stream), buffer, sizeof buffer, 0, &error);
    if (status == -1) {
      g_message("g_input_stream_read: %s", error->message);
      g_error_free(error);
      break;
    }
    write(STDOUT_FILENO, buffer, status);
    if (status == 0) {
      /* 成功结束 */
      break;
    }
  }
  g_object_unref(input_stream);
 CLEANUP:
  g_object_unref(file);
}

static void readfile(GtkWindow* parent) {
  GtkFileChooserNative* native = gtk_file_chooser_native_new
    ("Open file", parent, GTK_FILE_CHOOSER_ACTION_OPEN, "_Open", "_Cancel");
  if (gtk_native_dialog_run(GTK_NATIVE_DIALOG(native)) == GTK_RESPONSE_ACCEPT) {
    gchar* filename =
      gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(native));
    g_message("选择了文件：%s", filename);
    readfile_internal(filename);
    g_free(filename);
  }
  g_object_unref(native);
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "gio 读文件测试");
  gtk_window_set_default_size(GTK_WINDOW(win), 640, 320);
  gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
  g_signal_connect_swapped(win, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* button = gtk_button_new_with_label("选择文件...");
  gtk_container_add(GTK_CONTAINER(win), button);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(readfile), win);
  
  gtk_widget_show_all(win);
  gtk_main();
}

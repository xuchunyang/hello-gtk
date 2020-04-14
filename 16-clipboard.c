/* 16-clipboard.c --- 剪切板 */
#include <gtk/gtk.h>

static void paste_cb(GtkButton* button __attribute__((unused)),
                    gpointer* data) {
  GtkClipboard* board = (GtkClipboard*)data;
  gchar* s = gtk_clipboard_wait_for_text(board);
  g_print("读取到：'%s'\n", s);
}

static void copy_cb(GPtrArray* array) {
  GtkClipboard* board = g_ptr_array_index(array, 0);
  GtkWidget* input = g_ptr_array_index(array, 1);
  char const*const text = gtk_entry_get_text(GTK_ENTRY(input));
  g_print("已复制: %s\n", text);
  gtk_clipboard_set_text(board, text, -1);
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "剪切板测试");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
  
  GtkWidget* input = gtk_entry_new();
  GtkWidget* copy = gtk_button_new_with_label("复制");
  GtkWidget* paste = gtk_button_new_with_label("粘贴");

  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_box_pack_start(GTK_BOX(box), input, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), copy, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), paste, FALSE, FALSE, 0);
  
  GtkClipboard* board = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

  g_signal_connect(paste, "clicked", G_CALLBACK(paste_cb), board);

  GPtrArray* array = g_ptr_array_new();
  g_ptr_array_add(array, board);
  g_ptr_array_add(array, input);
  g_signal_connect_swapped(copy, "clicked", G_CALLBACK(copy_cb), array);
  
  gtk_widget_show_all(window);
  gtk_main();
  g_ptr_array_free(array, TRUE);
}

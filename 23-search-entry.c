/* 23-search-entry.c --- 搜索控件 */
#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

GArray *files = 0;

enum {
  FILENAME_COLUMN,
  N_COLUMNS,
};

static void search(GtkListStore* list_store, gchar const*const pattern) {
  gtk_list_store_clear(list_store);
  GtkTreeIter iter = {0};
  gchar const* filename = 0;
  for (size_t i = 0; (filename = g_array_index(files, gchar const*, i)); ++i) {
    // g_print("%zu. '%s'\n", i, filename);
    if (!pattern || strstr(filename, pattern)) {
      gtk_list_store_append(list_store, &iter);
      gtk_list_store_set(list_store, &iter, FILENAME_COLUMN, filename, -1);
    }
  }
}

static void on_search_changed(GtkSearchEntry* entry,
                              gpointer data) {
  gchar const*const input = gtk_entry_get_text(GTK_ENTRY(entry));
  g_message("on_search_changed: search '%s'...", input);
  GtkListStore* list_store = data;
  search(list_store, input);
}

static void on_stop_search(GtkSearchEntry* entry __attribute__((unused)),
                           gpointer data __attribute__((unused))) {
  /* XXX 按下 ESC 没效果，需要绑定还是怎么的？ */
  g_message("on_stop_search: 停止搜索，用户按下了 ESC 键");
}

static void on_next_match(GtkSearchEntry* entry __attribute__((unused)),
                          gpointer data __attribute__((unused))) {
  g_message("on_next_match: 下一个匹配，用户按下了 C-g 键");
}

static void on_prev_match(GtkSearchEntry* entry __attribute__((unused)),
                          gpointer data __attribute__((unused))) {
  g_message("on_prev_match: 前一个匹配，用户按下了 S-C-g 键");
}

static GArray* dir_files() {
  GArray* array = g_array_new(true, true, sizeof(gchar *));
  GError* error = 0;
  GDir* dir = g_dir_open(".", 0, &error);
  if (!dir) {
    g_error("g_dir_open failed: %s", error->message);
    /* XXX g_error 之后的还会运行吗？ */
    g_error_free(error);
    return 0;
  }
  for (gchar const* name = 0; (name = g_dir_read_name(dir));) {
    // g_print("'%s'\n", name);
    if (!g_file_test(name, G_FILE_TEST_IS_DIR)) {
      /* 需要复制一份，不然会有很多空白，应该是 g_dir_read_name 清空的 */
      gchar* copy = g_strdup(name);
      g_array_append_val(array, copy);
    }
  }
  return array;
}

int main(void) {
  files = dir_files();
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "搜索文件");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* entry = gtk_search_entry_new();
  gtk_box_pack_start(GTK_BOX(box), entry, false, false, 0);
  g_signal_connect(entry, "stop-search",    G_CALLBACK(on_stop_search),    0);
  g_signal_connect(entry, "next-match",     G_CALLBACK(on_next_match),     0);
  g_signal_connect(entry, "previous-match", G_CALLBACK(on_prev_match),     0);

  GtkListStore* list_store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
  search(list_store, 0);

  g_signal_connect(entry, "search-changed", G_CALLBACK(on_search_changed), list_store);

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes
    ("Filename", renderer, "text", FILENAME_COLUMN, (void*)0);
  GtkWidget* tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  /* XXX 高度、滚动怎么调整？ */
  GtkWidget* sw = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start(GTK_BOX(box), sw, true, true, 0);
  gtk_container_add(GTK_CONTAINER(sw), tree);

  gtk_widget_show_all(window);
  gtk_main();
  g_array_free(files, true);
}

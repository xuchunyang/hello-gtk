/* 24-tree-view.c --- 带层级的列表控件 */
#include <gtk/gtk.h>

enum {
  LANGUAGE_COLUMN,
  N_COLNUMS,
};

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkTreeWidget 测试");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkTreeStore* store = gtk_tree_store_new(N_COLNUMS, G_TYPE_STRING);

  GtkTreeIter iter1 = {0};      /* parent */
  GtkTreeIter iter2 = {0};      /* child */

  gtk_tree_store_append(store, &iter1, 0);
  gtk_tree_store_set(store, &iter1, LANGUAGE_COLUMN, "Scripting languages", -1);
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, LANGUAGE_COLUMN, "Ruby", -1);
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, LANGUAGE_COLUMN, "Python", -1);
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, LANGUAGE_COLUMN, "JavaScript", -1);

  gtk_tree_store_append(store, &iter1, 0);
  gtk_tree_store_set(store, &iter1, LANGUAGE_COLUMN, "Compiling languages", -1);
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, LANGUAGE_COLUMN, "C", -1);
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, LANGUAGE_COLUMN, "Rust", -1);
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, LANGUAGE_COLUMN, "Go", -1);

  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes("Programming languages",
                                                                       renderer,
                                                                       "text", LANGUAGE_COLUMN,
                                                                       (void*)0);
  GtkWidget* tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  /* XXX 前面没考虑？ */
  /* The view now holds a reference.  We can get rid of our own
   * reference */
  /* 用 gtk_tree_view_get_model 么 */
  g_object_unref(store);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

  /* 全部展开 */
  gtk_tree_view_expand_all(GTK_TREE_VIEW(tree_view));

  gtk_container_add(GTK_CONTAINER(window), tree_view);
  gtk_widget_show_all(window);
  gtk_main();
}

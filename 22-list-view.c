/* 22-list-view.c --- 列表控件 */
#include <gtk/gtk.h>

static void toggle_cb (GtkCellRendererToggle* cell_renderer __attribute__((unused)),
                       gchar* path_str,
                       gpointer user_data) {
  g_message("toggle_cb: path_str: %s", path_str);
  GtkTreePath* path = gtk_tree_path_new_from_string(path_str);
  GtkListStore* list_store = user_data;
  GtkTreeIter iter = {0};
  gtk_tree_model_get_iter(GTK_TREE_MODEL(list_store), &iter, path);
  gtk_tree_path_free(path);
  /* Toggle */
  GValue a = G_VALUE_INIT;
  gtk_tree_model_get_value(GTK_TREE_MODEL(list_store), &iter, 1, &a);
  gboolean checked = g_value_get_boolean(&a);
  g_message("Already checked? %u\n", checked);
  g_value_unset(&a);
  gtk_list_store_set(list_store, &iter, 1, !checked, -1);
}

static void tree_selection_change_cb(GtkTreeSelection* treeselection,
                                     gpointer user_data __attribute__((unused))) {
  GtkTreeIter iter = {0};
  GtkTreeModel* model = 0;
  if (gtk_tree_selection_get_selected(treeselection, &model, &iter)) {
    gchar* lang = 0;
    gtk_tree_model_get(model, &iter, 0, &lang, -1);
    g_print ("You selected an item by %s\n", lang);
    g_free(lang);
  }
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "列表控件");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  /* 创建 list store */
  GtkListStore* store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_BOOLEAN);

  /* 创建 iterator */
  GtkTreeIter iter = {0};

  /* 添加数据 */
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "C",          1, TRUE, -1);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "Python",     1, FALSE, -1);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "Emacs Lisp", 1, FALSE, -1);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "JavaScript", 1, FALSE, -1);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "Go",         1, TRUE, -1);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 0, "Ruby",       1, FALSE, -1);

  /* 创建视图部件 */
  GtkWidget* tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  /* 添加列 */
  GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
  // g_object_set(renderer, "size", 24, "foreground", "red", (void*)0);
  g_object_set(renderer,
               "size-points", 16.0,
               "foreground", "red",
               (void*)0);
  GtkTreeViewColumn* column = gtk_tree_view_column_new_with_attributes("Language", renderer, "text", 0, (void*)0);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  GtkCellRenderer* r2 = gtk_cell_renderer_toggle_new();
  /* XXX 处理 toggle 事件 */
  // g_object_set(r2, "activatable", TRUE, (void*)0);
  g_signal_connect(r2, "toggled", G_CALLBACK(toggle_cb), store);
  /* g_object_set(r2, "radio", TRUE, (void*)0); */
  GtkTreeViewColumn* column2 = gtk_tree_view_column_new_with_attributes("Compiled?", r2, "active", 1, (void*)0);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column2);

  /* 处理选中事件 */
  GtkTreeSelection* select = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
  g_signal_connect(G_OBJECT(select), "changed", G_CALLBACK(tree_selection_change_cb), 0);

  gtk_container_add(GTK_CONTAINER(window), tree);
  gtk_widget_show_all(window);
  gtk_main();
}

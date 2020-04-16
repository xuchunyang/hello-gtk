/* 27-list-box.c --- GtkListbox */
#include <gtk/gtk.h>
#include <stdbool.h>

void destroy_func(gpointer data) {
  /* XXX destroy_func 干嘛用的 */
  g_assert(!data);
  g_message("destroy_func called with %p", data);
}

bool filter_func(GtkListBoxRow *row,
                 gpointer data __attribute__((unused))) {
  GString const* gs = g_object_get_data(G_OBJECT(row), "data");
  g_message("data = %s", gs->str);
  return !!g_strcmp0(gs->str, "Fail");
}

int sort_func(GtkListBoxRow *row1,
              GtkListBoxRow *row2,
              gpointer data __attribute__((unused))) {
  GString * gs1 = g_object_get_data(G_OBJECT(row1), "data");
  GString * gs2 = g_object_get_data(G_OBJECT(row2), "data");
  /* NOTE 就地转化成小写 */
  g_string_ascii_down(gs1);
  g_string_ascii_down(gs2);
  return g_strcmp0(gs1->str, gs2->str);
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkListBox");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* box = gtk_list_box_new();
  gtk_list_box_set_selection_mode(GTK_LIST_BOX(box), GTK_SELECTION_NONE);
  gtk_container_add(GTK_CONTAINER(window), box);

  gchar** strings = g_strsplit("This is a sorted ListBox Fail", " ", -1);
  for (gchar **p = strings; *p; ++p) {
    g_message("%s", *p);
    GtkWidget* row = gtk_list_box_row_new();
    gtk_container_add(GTK_CONTAINER(box), row);
    /* 保存标签 GString* 到 data */
    g_object_set_data(G_OBJECT(row), "data", g_string_new(*p));

    GtkWidget* label = gtk_label_new(*p);
    gtk_container_add(GTK_CONTAINER(row), label);
  }
  /* 过滤掉 "Fail" */
  gtk_list_box_set_filter_func
    (GTK_LIST_BOX(box), (GtkListBoxFilterFunc)filter_func, 0, destroy_func);

  /* 排序 */
  gtk_list_box_set_sort_func(GTK_LIST_BOX(box), (GtkListBoxSortFunc)sort_func, 0, destroy_func);

  gtk_widget_show_all(window);
  gtk_main();

  for (size_t i = 0; strings[i]; ++i)
    free(strings[i]);
  free(strings);
}

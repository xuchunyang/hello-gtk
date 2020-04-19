/* 39-gtksourceview.c --- GtkSourceView 编辑器库 */
#include "gtk/gtkcssprovider.h"
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

static GString* readfile(char const* path) {
  GFile* file = g_file_new_for_path(path);
  GError* error = 0;
  GFileInputStream* input = g_file_read(file, 0, &error);
  if (!input) {
    g_error("g_file_read: %s", error->message);
  }
  GString* gs = g_string_new(0);
  while (TRUE) {
    char buffer[1024];
    ssize_t size = g_input_stream_read(G_INPUT_STREAM(input), buffer, sizeof buffer, 0, &error);
    if (size == -1)
      g_error("g_input_stream_read: %s", error->message);
    if (size == 0)
      break;
    g_string_append_len(gs, buffer, size);
  }
  return gs;
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkSourceView demo");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* initialize */
  gtk_source_init();

  /* lang */
  GtkSourceLanguageManager* lm = gtk_source_language_manager_new();
  GtkSourceLanguage* lang = gtk_source_language_manager_guess_language(lm, "main.c", 0);
  g_message("%s", gtk_source_language_get_id(lang));

  /* buffer */
  GtkSourceBuffer* buffer = gtk_source_buffer_new(0);
  gtk_source_buffer_set_language(buffer, lang);
  gtk_text_buffer_set_text
    (GTK_TEXT_BUFFER(buffer), "int main(void) { return 0; }", -1);
  GString* gs = readfile("39-gtksourceview.c");
  gtk_text_buffer_set_text
    (GTK_TEXT_BUFFER(buffer), gs->str, gs->len);
  g_string_free(gs, TRUE);

  /* view */
  GtkWidget* sw = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy
    (GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_box_pack_start(GTK_BOX(box), sw, FALSE, FALSE, 0);
  GtkWidget* view = gtk_source_view_new_with_buffer(buffer);
  gtk_container_add(GTK_CONTAINER(sw), view);
  /* XXX 自适应宽度 */
  gtk_widget_set_size_request(sw, 550, 550);
  /* gtk_widget_set_vexpand(view, TRUE); */
  /* gtk_widget_set_hexpand(box, TRUE); */
  /* gtk_widget_set_hexpand(view, TRUE); */

  /* 调大点字体 */
  GtkCssProvider* provider = gtk_css_provider_new();
  gtk_css_provider_load_from_data(provider, "textview {font-size: 18px;}", -1, 0);
  gtk_style_context_add_provider(gtk_widget_get_style_context(view),
                                 GTK_STYLE_PROVIDER(provider),
                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_object_unref(provider);

  /* 行号 */
  gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(view), TRUE);

  /* 高亮当前行 */
  gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(view), TRUE);

  /* minimap */
  GtkWidget* map = gtk_source_map_new();
  gtk_source_map_set_view(GTK_SOURCE_MAP(map), GTK_SOURCE_VIEW(view));
  gtk_box_pack_start(GTK_BOX(box), map, FALSE, FALSE, 0);

  gtk_widget_show_all(window);
  gtk_main();

  /* finalize */
  gtk_source_finalize();
}

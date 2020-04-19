/* 40-gspell.c --- Spell check */
#include <gtk/gtk.h>
#include <gspell/gspell.h>

int main(void) {
  gtk_init(0, 0);

  g_message("default lang: %s",
            gspell_language_get_name(gspell_language_get_default()));

  /* GtkEntry */

  GtkWidget* gtk_entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(gtk_entry), "helloo worldd!");

  GspellEntry* gspell_entry = gspell_entry_get_from_gtk_entry(GTK_ENTRY(gtk_entry));
  gspell_entry_basic_setup(gspell_entry);

  /* GtkTextView */

  GtkWidget* gtk_view = gtk_text_view_new();
  GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_view));
  gtk_text_buffer_set_text(buffer, "some typoo hera", -1);

  GspellTextView* gspell_view = gspell_text_view_get_from_gtk_text_view(GTK_TEXT_VIEW(gtk_view));
  gspell_text_view_basic_setup(gspell_view);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_box_pack_start(GTK_BOX(box), gtk_entry, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), gtk_view, TRUE, TRUE, 0);

  gtk_widget_show_all(window);
  gtk_main();
}

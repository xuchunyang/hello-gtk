/* 41-terminal.c --- terminal using VTE */
#include <gtk/gtk.h>
#include <stdio.h>
#include <vte/vte.h>

static void termainl_exit_callback(VteTerminal* termainl,
                                   gint status,
                                   gpointer data) {
  GString* gs = g_string_new(0);
  g_string_printf(gs, "shell exits with status (%d)", status);
  vte_terminal_feed(termainl, gs->str, gs->len);
  g_string_free(gs, TRUE);
}

static void my_vte_terminal_spawn_async_callback(VteTerminal* terminal,
                                                 GPid pid,
                                                 GError* error,
                                                 gpointer data) {
  g_message("my_vte_terminal_spawn_async_callback is called");
  if (pid == -1)
    g_error("vte_terminal_spawn_async failed: %s", error->message);

  vte_terminal_watch_child(terminal, pid);
  g_signal_connect(terminal, "child-exited", G_CALLBACK(termainl_exit_callback), 0);

  /* XXX 启动的时候需要按一次键才能出现 prompt，不知道怎么解决 */
  // vte_terminal_reset(terminal, TRUE, TRUE);
  // vte_terminal_feed(terminal, " ", 1);
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "终端测试");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* terminal = vte_terminal_new();
  GtkWidget* scrollbar = gtk_scrollbar_new
    (GTK_ORIENTATION_VERTICAL, gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(terminal)));

  gtk_box_pack_start(GTK_BOX(box), terminal, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), scrollbar, FALSE, FALSE, 0);

  /* 设置大小，怎么知道默认大小呢？ */
  // vte_terminal_set_size(VTE_TERMINAL(terminal), 80, 25);

  /* 字号调大些 */
  PangoFontDescription* desc = pango_font_description_from_string("Monaco 14");
  vte_terminal_set_font(VTE_TERMINAL(terminal), desc);
  pango_font_description_free(desc);

  char* shell = vte_get_user_shell();
  g_message("user shell: %s", shell);
  // free(shell);

  g_message("a list of features vte was compiled with: %s", vte_get_features());

  VtePty* pty = vte_terminal_get_pty(VTE_TERMINAL(terminal));
  g_assert(!pty);

  /* zsh -i */
  char* pty_argv[] = {shell, "-i", 0};
  vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT, 0, pty_argv, 0, G_SPAWN_DO_NOT_REAP_CHILD, 0, 0, 0, -1, 0, my_vte_terminal_spawn_async_callback, 0);

  gtk_widget_show_all(window);
  gtk_main();
}

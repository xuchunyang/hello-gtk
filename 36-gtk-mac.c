/* 36-gtk-mac.c --- gtk-mac-integration */
#include <gtk/gtk.h>
#include <gtkosxapplication.h>

/* https://jralls.github.io/gtk-mac-integration/GtkosxApplication.html */
/* https://github.com/GNOME/gtk-mac-integration/blob/master/src/test-integration.c */

static void app_will_quit_cb(GtkosxApplication* app __attribute__((unused)),
                             gpointer data __attribute__((unused))) {
  g_print("quitting now...");
}

int main(void) {
  gtk_init(0, 0);

  /* https://github.com/haskell/ThreadScope/commit/9de1d06d565529ac2b3e6ba2c2089c76d44b2cb0 */
  GtkosxApplication *theApp = g_object_new(GTKOSX_TYPE_APPLICATION, 0);
  /* https://github.com/nim-lang/Aporia/issues/111 */
  GtkWidget* menubar = gtk_menu_bar_new();
  gtkosx_application_set_menu_bar(theApp, GTK_MENU_SHELL(menubar));
  g_signal_connect(theApp, "NSApplicationWillTerminate", G_CALLBACK(app_will_quit_cb), 0);
  gtkosx_application_ready(theApp);
  /* https://cpp.hotexamples.com/examples/-/-/gtkosx_application_set_menu_bar/cpp-gtkosx_application_set_menu_bar-function-examples.html */

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_widget_show_all(window);
  gtk_main();
  g_object_unref(theApp);
}

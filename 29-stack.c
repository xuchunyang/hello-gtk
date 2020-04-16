/* 29-stack.c --- GtkStack */
/* https://python-gtk-3-tutorial.readthedocs.io/en/latest/layout.html#stack-and-stackswitcher */
#include <gtk/gtk.h>

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  /* NOTE 不要设置大小，使用自动分配的大小，避免按钮等拉伸 */
  /* gtk_window_set_default_size(GTK_WINDOW(window), 400, 400); */
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "GtkStack Demo");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
  gtk_stack_set_transition_duration(GTK_STACK(stack), 1000);

  GtkWidget* checkbutton = gtk_check_button_new_with_label("Click me!");
  gtk_stack_add_titled(GTK_STACK(stack), checkbutton, "check", "Check Button");

  GtkWidget* label = gtk_label_new("A plain label");
  gtk_stack_add_titled(GTK_STACK(stack), label, "label", "A label");

  GtkWidget* sw = gtk_stack_switcher_new();
  gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(sw), GTK_STACK(stack));

  gtk_box_pack_start(GTK_BOX(box), sw, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(box), stack, TRUE, TRUE, 0);

  gtk_widget_show_all(window);
  gtk_main();
}

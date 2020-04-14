/* 19-frame.c --- 装饰性箱子 */
#include <gtk/gtk.h>

GtkShadowType types[] = {
  GTK_SHADOW_NONE,
  GTK_SHADOW_IN,
  GTK_SHADOW_OUT,
  GTK_SHADOW_ETCHED_IN,
  GTK_SHADOW_ETCHED_OUT,
};

unsigned current_type = 0;

gchar const* shadow_name(GtkShadowType type) {
  switch (type) {
  case GTK_SHADOW_NONE:
    return "GTK_SHADOW_NONE";
  case GTK_SHADOW_IN:
    return "GTK_SHADOW_IN";
  case GTK_SHADOW_OUT:
    return "GTK_SHADOW_OUT";
  case GTK_SHADOW_ETCHED_IN:
    return "GTK_SHADOW_OUT";
  case GTK_SHADOW_ETCHED_OUT:
    return "GTK_SHADOW_ETCHED_OUT";
  }
}

static void cb(GtkFrame* frame) {
  size_t size = (sizeof types)/(sizeof *types);
  g_assert(size == 5);
  current_type = (current_type+1) % size;
  gtk_frame_set_shadow_type(frame, types[current_type]);
  gtk_frame_set_label(frame, shadow_name(types[current_type]));
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 320);
  gtk_window_set_title(GTK_WINDOW(window), "GtkFrame 测试");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);
  gtk_container_add(GTK_CONTAINER(window), box);

  GtkWidget* frame1 = gtk_frame_new("Frame 1");
  gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_IN);
  gtk_box_pack_start(GTK_BOX(box), frame1, TRUE, TRUE, 0);

  GtkWidget* frame2 = gtk_frame_new("Frame 2");
  /* NOTE 所有 shadow 都看不出效果来，可能受到 theme 影响，macOS 下有问题
     https://developer.gnome.org/gtk3/stable/gtk3-Standard-Enumerations.html#GtkShadowType
  */
  gtk_frame_set_shadow_type(GTK_FRAME(frame2), types[current_type]);
  gtk_box_pack_start(GTK_BOX(box), frame2, TRUE, TRUE, 0);

  GtkWidget* button = gtk_button_new_with_label("改变 GtkFrame 阴影效果");
  gtk_container_add(GTK_CONTAINER(frame2), button);
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(cb), frame2);
  
  gtk_widget_show_all(window);
  gtk_main();
}

/* 25-cairo.c --- cairo 测试 */
#include <cairo.h>
#include <gtk/gtk.h>

static gboolean on_draw_event(GtkWidget* widget __attribute__((unused)),
                              cairo_t* cr,
                              gpointer data __attribute__((unused))) {
  g_message("on_draw_event: 画图...");
  /* 绿色 */
  cairo_set_source_rgb(cr, 0, 1, 0);
  /* 0,0 位置 100,100 矩形 */
  cairo_rectangle(cr, 0, 0, 100, 100);
  /* 画 */
  cairo_fill(cr);

  /* 黑色 */
  cairo_set_source_rgb(cr, 0, 0, 0);
  /* 字形 */
  cairo_select_font_face(cr, "PingFang SC", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_NORMAL);
  /* 字体大小 */
  cairo_set_font_size(cr, 40);
  /* 移动 */
  cairo_move_to(cr, 100, 100);
  /* 写 */
  cairo_show_text(cr, "Hello, 世界");

  return FALSE;
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Cairo 画图测试");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);

  GtkWidget* draw_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), draw_area);
  g_signal_connect(draw_area, "draw", G_CALLBACK(on_draw_event), 0);

  gtk_widget_show_all(window);
  gtk_main();
}

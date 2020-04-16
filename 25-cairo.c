/* 25-cairo.c --- cairo 测试 */
#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

static gboolean on_draw_event(GtkWidget* widget,
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

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, 200, 200);
  /* 描线 */
  cairo_stroke(cr);

  cairo_set_line_width(cr, 9);
  cairo_set_source_rgb(cr, 0.69, 0.19, 0);
  GtkWidget* window = gtk_widget_get_toplevel(widget);
  gint width;
  gint height;
  gtk_window_get_size(GTK_WINDOW(window), &width, &height);
  /* 原点居中 */
  cairo_translate(cr, width/2, height/2);
  cairo_arc(cr, 0, 0, 50, 0, 2 * M_PI);
  /* 描线不清空形状 */
  cairo_stroke_preserve(cr);
  cairo_set_source_rgb(cr, 0.3, 0.4, 0.6);
  cairo_fill(cr);

  /* XXX 搞不清楚 cairo_translate 的效果如何取消？ */
  cairo_translate(cr, 0, 0);
  double x;
  double y;
  cairo_move_to(cr, 0, 0);
  cairo_get_current_point(cr, &x, &y);
  g_debug("x: %g, y: %g\n", x, y);
  cairo_rel_move_to(cr, -x, 0);
  cairo_move_to(cr, 0, 200);
  for (size_t i = 1; i <= 10; ++i) {
    cairo_set_source_rgba(cr, 0, 0, 1, i*0.1);
    cairo_rectangle(cr, 50*i, 20, 40, 40);
    cairo_fill(cr);
  }

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

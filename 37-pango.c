/* 37-pango.c --- 用 Pango 显示文字 */
#include <gtk/gtk.h>

static int pango_puts(GtkWidget* widget,
                       cairo_t* cr,
                       char const* s,
                       char const* font,
                       int height) {
  cairo_move_to(cr, 0, height);

  PangoLayout* layout = gtk_widget_create_pango_layout(widget, 0);
  pango_layout_set_text(layout, s, -1);
  PangoFontDescription* desc = pango_font_description_from_string(font);
  pango_layout_set_font_description(layout, desc);
  pango_font_description_free(desc);
  pango_cairo_show_layout(cr, layout);

  int width;
  int height2;
  pango_layout_get_pixel_size(layout, &width, &height2);
  int const line_space = 10;
  return height + height2 + line_space;
}

static gboolean draw(GtkWidget* widget,
                     cairo_t* cr,
                     gpointer data __attribute__((unused))) {
  int height = 0;
  // cairo_set_source_rgb(cr, 1, 0, 0);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，华康翩翩。", "HanziPen SC 40", height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，苹方。",    "PingFang SC 40", height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，楷体。",    "Kaiti SC 40",    height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，魏碑。",    "Weibei SC 40",   height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，娃娃。",    "Wawati SC 40",   height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，雅痞。",    "Yuppy SC 40",    height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，圆体。",    "Yuanti SC 40",   height);
  height = pango_puts(widget, cr, "汉体书写信息技术标准相容，华文报隶。", "Baoli SC 40",    height);

  /* XXX 竖排，想要得到的效果是： */
  /* $ pango-view 登黄鹤楼.txt --font "Kaiti SC 40" --margin 60 --gravity east --rotate -90 -o x.png */
  /* 以下能达到效果，但感觉是应该用 pango_matrix_rotate，但不知道怎么用 */
  /* cairo_move_to(cr, 30, height); */
  /* cairo_rotate(cr, G_PI_2); */
  /* PangoLayout* layout = gtk_widget_create_pango_layout(widget, "ABCDEFG"); */
  /* PangoContext* context = pango_layout_get_context(layout); */

  /* pango_context_set_base_gravity(context, PANGO_GRAVITY_EAST); */
  /* pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT); */
  /* pango_cairo_show_layout(cr, layout); */

  /* TRUE to stop other handlers from being invoked for the event. FALSE to
     propagate the event further. */
  return TRUE;
}

int main(void) {
  gtk_init(0, 0);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* draw_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), draw_area);

  g_signal_connect(draw_area, "draw", G_CALLBACK(draw), 0);

  gtk_widget_show_all(window);
  gtk_main();
}

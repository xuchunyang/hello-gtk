/* 31-flowbox.c --- GtkFlowbox*/
#include "cairo.h"
#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION == 3 && GTK_MINOR_VERSION < 12
# error "GtkFlowbox requires GTK 3.12"
#endif

#if !GTK_CHECK_VERSION(3, 12, 0)
# error "GtkFlowbox requires GTK 3.12"
#endif

static gboolean on_draw(GtkWidget* widget,
                        cairo_t* cr,
                        gpointer data) {
  // g_message("on_draw calling...");
  GdkRGBA* rgba = data;
  g_debug("%s", gdk_rgba_to_string(rgba));
  GtkStyleContext* context = gtk_widget_get_style_context(widget);
  int width = gtk_widget_get_allocated_width(widget);
  int height = gtk_widget_get_allocated_height(widget);
  gtk_render_background(context, cr, 0, 0, width, height);
  cairo_set_source_rgba(cr, rgba->red, rgba->green, rgba->blue, rgba->alpha);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);
  return FALSE;
}

int main(void) {
  gtk_init(0, 0);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "31-flowbox.c");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), 0);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  GtkWidget* sw = gtk_scrolled_window_new(0, 0);
  gtk_scrolled_window_set_policy
    (GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(window), sw);

  GtkWidget* flowbox = gtk_flow_box_new();
  /* NOTE valign GTK_ALIGN_START 垂直方向有多余空间时不拉伸 */
  /* https://developer.gnome.org/gtk3/unstable/GtkWidget.html#GtkAlign */
  g_object_set(flowbox, "valign", GTK_ALIGN_START, (void*)0);
  gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox), 30);
  gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox), GTK_SELECTION_NONE);
  gtk_container_add(GTK_CONTAINER(sw), flowbox);

  char const* colors[] = {
    "AliceBlue",
    "AntiqueWhite",
    "AntiqueWhite1",
    "AntiqueWhite2",
    "AntiqueWhite3",
    "AntiqueWhite4",
    "aqua",
    "aquamarine",
    "aquamarine1",
    "aquamarine2",
    "aquamarine3",
    "aquamarine4",
    "azure",
    "azure1",
    "azure2",
    "azure3",
    "azure4",
    "beige",
    "bisque",
    "bisque1",
    "bisque2",
    "bisque3",
    "bisque4",
    "black",
    "BlanchedAlmond",
    "blue",
    "blue1",
    "blue2",
    "blue3",
    "blue4",
    "BlueViolet",
    "brown",
    "brown1",
    "brown2",
    "brown3",
    "brown4",
    "burlywood",
    "burlywood1",
    "burlywood2",
    "burlywood3",
    "burlywood4",
    "CadetBlue",
    "CadetBlue1",
    "CadetBlue2",
    "CadetBlue3",
    "CadetBlue4",
    "chartreuse",
    "chartreuse1",
    "chartreuse2",
    "chartreuse3",
    "chartreuse4",
    "chocolate",
    "chocolate1",
    "chocolate2",
    "chocolate3",
    "chocolate4",
    "coral",
    "coral1",
    "coral2",
    "coral3",
    "coral4",
  };

  for (size_t i = 0; i < (sizeof colors)/(sizeof *colors); ++i) {
    /* TODO free memory, or use glib array/list */
    GdkRGBA* rgba = g_malloc0(sizeof *rgba);
    /* GdkRGBA rgba = {0};         /\* WRONG *\/ */
    gdk_rgba_parse(rgba, colors[i]);
    g_debug("%s", gdk_rgba_to_string(rgba));

    GtkWidget* button = gtk_button_new();
    GtkWidget* area = gtk_drawing_area_new();
    gtk_widget_set_size_request(area, 24, 24);
    g_signal_connect(area, "draw", G_CALLBACK(on_draw), rgba);
    gtk_container_add(GTK_CONTAINER(button), area);
    gtk_container_add(GTK_CONTAINER(flowbox), button);
  }

  gtk_widget_show_all(window);
  gtk_main();
}

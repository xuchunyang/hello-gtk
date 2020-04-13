/* 00-version.c --- 打印 GTK+ 版本号 */
#include <gtk/gtk.h>

int main(void) {
  printf("GTK version: %d.%d.%d\n",
         gtk_major_version, gtk_minor_version, gtk_micro_version);
}

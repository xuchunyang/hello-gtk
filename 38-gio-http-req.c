/* 38-gio-http-req.c --- curl example.com with gio */
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char const*const host = "example.com";
  unsigned const   port = 80;

  GSocketClient* client = g_socket_client_new();
  GError* error = 0;
  GSocketConnection* connection = g_socket_client_connect_to_host(client, host, port, 0, &error);
  g_object_unref(client);
  if (!connection) {
    g_warning("g_socket_client_connect_to_host: %s", error->message);
    g_error_free(error);
  }

  GOutputStream* output = g_io_stream_get_output_stream(G_IO_STREAM(connection));
  GString* gs = g_string_new(0);
  g_string_printf(gs, "GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  if(!g_output_stream_write_all(output, gs->str, gs->len, 0, 0, &error)) {
    g_warning("g_socket_client_connect_to_host: %s", error->message);
    g_error_free(error);
    /* XXX free resource? */
    return EXIT_FAILURE;
  }
  g_string_free(gs, TRUE);

  if(!g_output_stream_close(output, 0, &error)) {
    g_warning("g_output_stream_close: %s", error->message);
    g_error_free(error);
  }

  GInputStream* input = g_io_stream_get_input_stream(G_IO_STREAM(connection));
  char const* buffer[100];
  while(TRUE) {
    g_message("loop... %d", g_input_stream_is_closed(input));
    /* XXX 不知道结束了没有 */
    ssize_t size = g_input_stream_read(input, buffer, sizeof buffer, 0, &error);
    g_message("loop... size = %zd", size);
    if (size == -1) {           /* error */
      g_warning("g_input_stream_read: %s", error->message);
      g_error_free(error);
      break;
    }
    if (size == 0) {            /* end-of-file */
      break;
    }
    fwrite(buffer, size, 1, stdout);
  }
}

/* 13-parse-json.c --- 解析 JSON */
#include <gtk/gtk.h>
#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

static char const*const json_data = "{\"data\":[{\"msg\": \"hello\"}, {\"msg\": \"你好\"}]}";

static void read_with_json_path() {
  JsonParser* parser = json_parser_new();
  json_parser_load_from_data(parser, json_data, -1, 0);
  JsonPath* path = json_path_new();
  json_path_compile(path, "$.data..msg", 0);
  JsonNode* result = json_path_match(path, json_parser_get_root(parser));
  JsonArray* array = json_node_get_array(result);
  g_assert(2 == json_array_get_length(array));
  JsonNode* e1 = json_array_get_element(array, 0);
  JsonNode* e2 = json_array_get_element(array, 1);
  g_print("%s\n", json_node_get_string(e1));
  /* XXX 中文有问题 */
  g_print("%s\n", json_node_get_string(e2));
  json_node_unref(result);
  g_object_unref(parser);
}

static void read_with_json_reader() {
  JsonParser* parser = json_parser_new();
  json_parser_load_from_data(parser, "[42, 3, \"你好\"]", -1, 0);
  JsonReader* reader = json_reader_new(json_parser_get_root(parser));

  json_reader_read_element(reader, 0);
  g_print("%lld\n", json_reader_get_int_value(reader));
  json_reader_end_element (reader);

  json_reader_read_element(reader, 1);
  g_print("%lld\n", json_reader_get_int_value(reader));
  json_reader_end_element (reader);

  json_reader_read_element(reader, 2);
  /* XXX 中文问题 */
  g_print("%s\n", json_reader_get_string_value(reader));
  json_reader_end_element (reader);
  
  g_object_unref(reader);
  g_object_unref(parser);
}

int main(void) {
  read_with_json_path();
  read_with_json_reader();
}

/* 35-glib-the-data-types.c --- GLib 数据类型 */

#include <glib.h>
#include <string.h>

/* https://developer.gnome.org/glib/2.64/glib-Strings.html */
static void strings_fun() {
  char const*const s = "Hello";

  GString* gs = g_string_new(s);
  g_assert(gs->len == 5);
  g_assert(!strcmp(gs->str, s));
  g_assert(gs->allocated_len >= gs->len);
  // g_message("%lu", gs->allocated_len);

  g_string_append(gs, ", ");
  g_string_append_printf(gs, "%s (%s)",
                         g_get_user_name(),
                         g_get_real_name());
  g_string_append_c(gs, '!');
  g_string_prepend(gs, "==> ");

  g_string_insert(gs, 4, "<INSERT>");

  g_print("%s\n", gs->str);

  g_string_overwrite(gs, gs->len - 1, "?");
  g_print("%s\n", gs->str);

  g_string_erase(gs, 0, 4);
  g_print("%s\n", gs->str);

  g_string_truncate(gs, 15);
  g_string_set_size(gs, 16);

  g_print("%X\n", g_string_hash(gs));

  g_string_free(gs, TRUE);
}

void on_each_hash_table_kv(gpointer key,
                           gpointer value,
                           gpointer user_data __attribute__((unused))) {
  char const*const k = key;
  int const v = GPOINTER_TO_INT(value);
  g_print("ht: key = %s, value = %d\n", k, v);
}

void on_hash_table_destroy_key(gpointer data) {
  char* s = data;
  g_message("on_hash_table_destroy_key: freeing %s", s);
  free(s);
}

void on_hash_table_destroy_value(gpointer data) {
  char* s = data;
  g_message("on_hash_table_destroy_value: freeing %s", s);
  free(s);
}


/* https://developer.gnome.org/glib/2.64/glib-Hash-Tables.html */
static void hash_table_fun() {
  GHashTable* ht = g_hash_table_new(g_str_hash, g_str_equal);
  g_hash_table_insert(ht, "one", GINT_TO_POINTER(1));
  g_hash_table_insert(ht, "two", GINT_TO_POINTER(2));
  g_hash_table_insert(ht, "three", GINT_TO_POINTER(3));
  g_hash_table_insert(ht, "forty-two", GINT_TO_POINTER(42));

  g_assert(GPOINTER_TO_INT(g_hash_table_lookup(ht, "one")) == 1);
  g_assert(GPOINTER_TO_INT(g_hash_table_lookup(ht, "forty-two")) == 42);

  g_assert(g_hash_table_lookup_extended(ht, "three", 0, 0));
  g_assert(!g_hash_table_lookup_extended(ht, "non-exist", 0, 0));

  g_hash_table_foreach(ht, on_each_hash_table_kv, 0);

  g_assert(g_hash_table_size(ht) == 4);
  g_hash_table_remove(ht, "two");
  g_assert(g_hash_table_size(ht) == 3);

  g_hash_table_destroy(ht);

  {
    GHashTable* ht = g_hash_table_new_full(g_str_hash,
                                           g_str_equal,
                                           on_hash_table_destroy_key,
                                           on_hash_table_destroy_value);
    g_hash_table_insert(ht, g_strdup("one"), g_strdup("1111111111"));
    g_hash_table_insert(ht, g_strdup("two"), g_strdup("2222222222"));
    g_hash_table_insert(ht, g_strdup("three"), g_strdup("33333"));
    g_hash_table_remove(ht, "one");
    g_print("-----\n");
    g_hash_table_destroy(ht);
  }
}

int int_cmp(gconstpointer a,
            gconstpointer b) {
  int const* p = a;
  int const* q = b;
  /* 降序 */
  if (*p > *q)
    return -1;
  if (*p < *q)
    return 1;
  return 0;
}

/* https://developer.gnome.org/glib/2.64/glib-Arrays.html */
static void array_fun() {
  GArray* ga = g_array_new(TRUE, TRUE, sizeof(int));
  for (size_t i = 0; i < 10; ++i) {
    int val = i*i;
    g_array_append_val(ga, val);
  }
  for (size_t i = 0; i < 10; ++i)
    g_print("ga[%zu] = %d\n", i, g_array_index(ga, int, i));

  g_array_set_size(ga, 5);
  g_array_set_size(ga, 10);
  for (size_t i = 0; i < 10; ++i)
    g_print("ga[%zu] = %d\n", i, g_array_index(ga, int, i));
  g_array_set_size(ga, 5);
  /* val: 0 1 4 9 16 */
  /* idx: 0 1 2 3 4 */
  g_array_remove_range(ga, 1, 2);
  for (size_t i = 0; i < ga->len; ++i)
    g_print("ga[%zu] = %d\n", i, g_array_index(ga, int, i));

  g_array_sort(ga, int_cmp);
  for (size_t i = 0; i < ga->len; ++i)
    g_print("ga[%zu] = %d\n", i, g_array_index(ga, int, i));

  g_array_free(ga, TRUE);
}

static int str_cmp(void const* a, void const* b) {
  char const* s1 = a;
  char const* s2 = b;
  return g_strcmp0(s1, s2);
}

/* https://developer.gnome.org/glib/2.64/glib-Doubly-Linked-Lists.html */
static void list_fun() {
  GList* l = 0;
  l = g_list_append(l, "a");
  l = g_list_append(l, "b");
  l = g_list_append(l, "d");
  l = g_list_append(l, "u");
  l = g_list_append(l, "z");
  l = g_list_prepend(l, "start");
  l = g_list_insert(l, "middle", 2);
  l = g_list_insert_sorted(l, "t", str_cmp);

  /* Print a list */
  for (GList* p = l; p; p = p->next)
    g_print("=> %s\n", p->data);

  /* remove some elements */
  for (GList* p = l; p;) {
    /* 提前保存 next 不然可能会清空 */
    GList* next = p->next;
    if (strlen(p->data) > 1) {
      g_print("removing '%s'\n", p->data);
      l = g_list_delete_link(l, p);
    }
    p = next;
  }

  for (GList* p = l; p; p = p->next)
    g_print("=> %s\n", p->data);

  l = g_list_remove(l, "a");
  l = g_list_remove(l, "z");
  for (GList* p = l; p; p = p->next)
    g_print("%s, ", p->data);
  g_print("\n");

  g_message("%d", g_list_length(l));

  g_list_free(l);
}

static void queue_fun() {
  GQueue* q = g_queue_new();
  g_assert(g_queue_is_empty(q));
  g_queue_push_head(q, "a");
  g_queue_push_head(q, "b");
  g_queue_push_head(q, "c");
  g_queue_push_head(q, "d");
  g_assert(4 == g_queue_get_length(q));
  g_assert(2 == g_queue_index(q, "b"));
  g_print("%s\n", g_queue_pop_head(q));
  g_print("%s\n", g_queue_pop_head(q));
  g_print("%s\n", g_queue_pop_head(q));
  g_print("%s\n", g_queue_peek_head(q));
  g_assert(1 == g_queue_get_length(q));
  g_queue_free(q);
}

/* https://developer.gnome.org/glib/2.64/glib-Balanced-Binary-Trees.html */
static void tree_fun() {
  GTree* tree = g_tree_new(str_cmp);
  g_tree_insert(tree, "one", "1");
  g_tree_insert(tree, "two", "2");
  g_tree_insert(tree, "three", "3");
  g_tree_insert(tree, "four", "4");
  g_print("'%s'\n", g_tree_lookup(tree, "two"));
  g_tree_destroy(tree);
}

int main(void) {
  strings_fun();
  hash_table_fun();
  array_fun();
  list_fun();
  queue_fun();
  tree_fun();
}

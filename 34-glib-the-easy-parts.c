/* 34-glib-the-easy-parts.c --- glib 简单使用（排除thread async memory) */
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

/* https://developer.gnome.org/glib/stable/glib-Version-Information.html */
static void version() {
  g_message("Glib v%d.%d.%d",
            GLIB_MAJOR_VERSION,
            GLIB_MINOR_VERSION,
            GLIB_MICRO_VERSION);
  g_message("%s", glib_check_version(2, 0, 0));
  g_message("%s", glib_check_version(3, 0, 0));
  g_message("Glib v%d.%d.%d",
            glib_major_version,
            glib_minor_version,
            glib_micro_version);
  g_message("binary age: %u", glib_binary_age);
  g_message("interface age: %u", glib_interface_age);

  g_message("%d", G_ENCODE_VERSION(2, 26));
}

/* https://developer.gnome.org/glib/stable/glib-Basic-Types.html */
static void basic_types() {
  g_message("%lld", G_GINT64_CONSTANT(0x1d636b02300a7aa7));
  g_message("%lld", G_GINT64_CONSTANT(0x1d636b02300a7aa7U));
  g_message("%lld", G_GINT64_CONSTANT(0x1d636b02300a7aa7));
}

static void standard_macros() {
  g_assert(G_IS_DIR_SEPARATOR('/'));
  g_assert(!G_IS_DIR_SEPARATOR('\\'));
  g_assert(1 == MIN(1, 2));
  g_assert(2 == MAX(1, 2));
  g_assert(3 == ABS(-3));
  g_assert(CLAMP(5, 10, 15) == 10);
  g_assert(CLAMP(15, 5, 10) == 10);
  g_assert(CLAMP(20, 15, 25) == 20);
  g_assert(G_APPROX_VALUE(5, 6, 2));
  g_message("%g", FLT_EPSILON);
  g_message("%lu", G_SIZEOF_MEMBER(GString, str));

  g_message("%ld", G_STRUCT_OFFSET(GString, str));
  g_message("%ld", G_STRUCT_OFFSET(GString, len));
  g_message("%ld", G_STRUCT_OFFSET(GString, allocated_len));

  g_message("%lu", G_ALIGNOF(int));
  g_message("%lu", G_ALIGNOF(GString));
  double arr[] = {1.2, 3.4, 5.6};
  g_assert(G_N_ELEMENTS(arr) == 3);

#ifdef G_OS_WIN32
  g_message("Windows");
#endif

#ifdef G_OS_UNIX
  g_message("UNIX");
#endif
}

/* https://developer.gnome.org/glib/stable/glib-Type-Conversion-Macros.html */
static void type_conv_macros() {
  /* 用指针保存数字，方便传递数字参数到 callback */
  gpointer p = GINT_TO_POINTER(1);
  g_message("%p", p);
  g_assert(GPOINTER_TO_INT(p) == 1);

  g_message("%p", GSIZE_TO_POINTER(1));
}

static void byte_order_macro() {
  switch (G_BYTE_ORDER) {
  case G_LITTLE_ENDIAN:
    g_message("litter endian");
    break;
  case G_BIG_ENDIAN:
    g_message("big endian");
    break;
  default:
    g_error("impossible: %d", G_BYTE_ORDER);
  }

  g_message("%x", g_htonl(-1));
  g_message("%x", g_ntohl(1));
}

static void num_defs() {
  g_message("%d", G_IEEE754_FLOAT_BIAS);
  g_message("%d", G_IEEE754_DOUBLE_BIAS);
  g_message("%g", G_E);
  g_message("%g", G_PI);
}

GPrintFunc old_print_func = 0;
static void chunyang_print_func(gchar const* string) {
  g_message("chunyang_print_func is running...");
  fprintf(stdout, "===> %s <===\n", string);
}

static void warn_and_assert() {
  g_warn_if_fail(FALSE);
  g_warn_if_reached();

  old_print_func = g_set_print_handler(chunyang_print_func);
  g_print("hello, 测试 chunyang_print_func");
  g_set_print_handler(old_print_func);

  g_return_if_reached();
}

static void message_and_debug() {
  g_message("message_and_debug...");
  // https://developer.gnome.org/glib/2.64/glib-running.html
  // env G_MESSAGES_DEBUG=all ./34-glib-the-easy-parts
  g_log(0, G_LOG_LEVEL_INFO, "g_log DEBUG...");
  g_log(0, G_LOG_LEVEL_INFO, "g_log INFO...");
  g_log("x", G_LOG_LEVEL_INFO, "g_log MESSAGE...");
  g_log("34-glib-the-easy-parts.c", G_LOG_LEVEL_INFO, "g_log...");
}

static void string_utility() {
  g_print("%s\n", g_strjoin(", ", "a", "b", "c", "d", (void*)0)); /* XXX free */

  char const*const arr[] = {"one", "two", "three", 0};
  char* res = g_strjoinv("|", (char **)arr);
  g_message("%s", res);
  free(res);

  char* s = g_strconcat("hello", "world", "!", (void*)0);
  g_message("%s", s);
  free(s);

  {
    char* s = g_strdup("ABCDE");
    g_assert(g_strreverse(s) == s);
    g_message("%s", s);

    char s2[] = "ABCDE";
    s2[0] = 'a';
    g_strreverse(s2);
    g_message("%s", s2);
  }

  g_assert(!g_ascii_strncasecmp("abc", "abC", 3));

  {
    gchar** arr = g_strsplit("a,bb,ccc,dddd", ",", 3);
    for (size_t i = 0; arr[i]; ++i)
      g_print("%s\n", arr[i]);
    g_strfreev(arr);
  }

  {
    gchar** arr = g_strsplit_set("a.bb,ccc.dddd|eee", ".,|", 4);
    for (size_t i = 0; arr[i]; ++i)
      g_print("%s\n", arr[i]);
    g_strfreev(arr);
  }

}

static void charset_conv() {
  // gchar buffer[10];
  // Char: 好 (22909, #o54575, #x597d, file ...) point=5044 of 5428 (93%) column=32
  char* s = g_convert("好", -1, "GBK", "UTF-8", 0, 0, 0);
  g_assert(strlen(s) == 2);
  /* (vconcat (encode-coding-string "好" 'gbk)) */
  /* ;; => [186 195] */
  g_message("=> %u, %u",
            /* https://stackoverflow.com/a/35978592/2999892 */
            (unsigned char)s[0],
            (unsigned char)s[1]);
  free(s);

  {
    /* XXX 为什么 Glib 不能输出中文？ Unicode */

    /* 没效 https://stackoverflow.com/questions/3092354/printing-utf8-in-glib */
    setlocale(LC_ALL, "en_US.utf8");
    g_print("你好\n");
    /* 也不行 */
    g_print("Presentación.sxi\n");
  }
}

static void unicode_fun() {
  gunichar* s = g_utf8_to_ucs4("好①", -1, 0, 0, 0);
  g_assert(s);
  g_print("'%s'\n", G_GINT32_FORMAT);
  g_print("Read U+%04"G_GINT32_FORMAT"X\n", s[0]);
  // g_message(s);

  // g_assert(g_unichar_isdigit(s[1]));
  g_print("==> %d\n", g_unichar_digit_value(g_utf8_get_char("五")));
  g_print("==> %d\n", g_unichar_digit_value(g_utf8_get_char("①")));
  g_print("==> %d\n", g_unichar_digit_value(g_utf8_get_char("5")));
  // g_assert(5 == g_unichar_digit_value(g_utf8_get_char("五")));

  g_assert(g_utf8_strlen("你好，世界！", -1) == 6);
  g_message("%s", g_utf8_substring("你好，world！", 3, 8));
}

static void base64_fun() {
  /* (base64-encode-string "hello") */
  /* ;; => "aGVsbG8=" */
  // unsigned char* data = "hello";
  /* cast is bad? */
  // unsigned char* data = (unsigned char*)"hello";
  /* happy now? */
  unsigned char data[] = {'h', 'e', 'l', 'l', 'o', 0};
  gchar* s = g_base64_encode(data, 5);
  g_assert(s);
  g_assert(!strcmp(s, "aGVsbG8="));

  g_base64_decode("aGVsbG8=", 0);
}

static void checksum_fun() {
  /* (md5 "hello") */
  /* ;; => "5d41402abc4b2a76b9719d911017c592" */
  g_message("%s", g_compute_checksum_for_string(G_CHECKSUM_MD5, "hello", -1));
}

static void date_and_time() {
  g_message("microseconds since unix epoch: %lld", g_get_real_time());
}

static void timezone_fun() {
  GTimeZone* tz =  g_time_zone_new_local();
  g_message("timezone: %s", g_time_zone_get_identifier(tz));
  /* LMT https://en.wikipedia.org/wiki/Local_mean_time */
  g_message("timezone abbr: %s", g_time_zone_get_abbreviation(tz, 0));
  g_message("timezone abbr: %s", g_time_zone_get_abbreviation(tz, 12));
  g_message("timezone abbr: %s", g_time_zone_get_abbreviation(tz, 6));
  g_time_zone_unref(tz);
}

static void gdatetime_fun() {
  GDateTime* now = g_date_time_new_now_local();
  g_message("%04d-%02d-%02d",
            g_date_time_get_year(now),
            g_date_time_get_month(now),
            g_date_time_get_day_of_month(now));
  char* s = g_date_time_format_iso8601(now);
  g_message("%s", s);
  free(s);
  g_date_time_unref(now);
}

static void random_fun() {
  GRand *r = g_rand_new();
  /* [0, 9] */
  g_message("random within [0, 9] %d", g_rand_int_range(r, 0, 10));
  g_rand_free(r);
}

static void misc_fun() {
  g_assert(!g_get_application_name());
  g_message("%s", g_get_prgname());
  g_message("%s", g_get_user_name());
  g_message("%s", g_get_real_name());
  char* s = g_get_os_info(G_OS_INFO_KEY_NAME);
  g_message("%s", s);
  free(s);
}

static void timer_fun() {
  GTimer* timer = g_timer_new();
  usleep(100000);               /* 0.1s */
  g_message("%g", g_timer_elapsed(timer, 0));
  g_assert(g_timer_is_active(timer));
  g_timer_destroy(timer);
}

static void glob_fun() {
  g_assert(g_pattern_match_simple("*.c", "main.c"));
  g_assert(!g_pattern_match_simple("*.h", "main.c"));
  GPatternSpec* spec = g_pattern_spec_new("*.c");
  GDir* dir = g_dir_open(".", 0, 0);
  g_assert(dir);
  char const* filename;
  while ((filename = g_dir_read_name(dir))) {
    if (g_pattern_match(spec, strlen(filename), filename, 0))
      g_print("matching '*.c' => %s\n", filename);
  }
  g_dir_close(dir);
  g_pattern_spec_free(spec);
}

static void guuid_fun() {
  g_message("uuid: %s", g_uuid_string_random());
}

int main(void) {
  version();
  basic_types();
  standard_macros();
  type_conv_macros();
  byte_order_macro();
  num_defs();
  warn_and_assert();
  message_and_debug();
  string_utility();
  charset_conv();
  unicode_fun();
  base64_fun();
  checksum_fun();
  date_and_time();
  timezone_fun();
  gdatetime_fun();
  random_fun();
  misc_fun();
  timer_fun();
  glob_fun();
  guuid_fun();

  g_print("\n--------\nOver!\n");
}

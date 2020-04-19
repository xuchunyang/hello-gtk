CFLAGS = -std=c11 -Wall -Wextra -Wpedantic $(shell pkg-config --cflags gtk+-3.0 libsoup-2.4 json-glib-1.0 gtksourceview-4 gspell-1)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0 libsoup-2.4 json-glib-1.0 gtksourceview-4 gspell-1)

SOURCES = $(wildcard *.c)
EXECUTABLES = $(patsubst %.c,%,$(SOURCES))

.PHONY: all
all: $(EXECUTABLES)

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(EXECUTABLES)

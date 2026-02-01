CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

TARGET = aprsfmt
SRCS = main.c sum.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(TARGET)
	install -m 755 $(TARGET) /usr/bin/

uninstall:
	rm -f /usr/bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

.PHONY: all clean install uninstall rebuild

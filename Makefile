CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

SOURCE = order_logic.c chaos_logic.c main_logic.c
OBJECT = order_logic.h chaos_logic.h main_logic.h
TARGET = order_vs_chaos

.PHONY game clean rebuild order_vs_chaos

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCE) $(OBJECT)

clean:
	rm - rf $(TARGET)
	rm -rf *.o
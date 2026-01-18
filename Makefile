CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

SOURCE = order_logic.c chaos_logic.c main_logic.c
HEADERS = order_logic.h chaos_logic.h main_logic.h
OBJECT = $(SOURCE:.c=.o)
TARGET = order_vs_chaos

.PHONY: all clean rebuild order_vs_chaos game

all: $(TARGET)

# можно и так
# %.o: %.c $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

# Компиляция каждого файла в объектный
order_logic.o: order_logic.c $(HEADERS)
	$(CC) $(CFLAGS) -c order_logic.c -o order_logic.o

chaos_logic.o: chaos_logic.c $(HEADERS)
	$(CC) $(CFLAGS) -c chaos_logic.c -o chaos_logic.o

main_logic.o: main_logic.c $(HEADERS)
	$(CC) $(CFLAGS) -c main_logic.c -o main_logic.o

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) -o $(TARGET)

order_vs_chaos: $(TARGET)

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJECT)

rebuild: clean all
#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // для usleep
#include <ncurses.h> // для ncurses

typedef struct
{
    int pixel_x;
    int pixel_y;
    int move_on_x;
    int move_on_y;
    char type;
    int score;
    int is_active;
} astra;

// Настройки
#define HI 30
#define LE 90
#define QUANTITY_STARS 50
#define SPEED 100      // Задержка в миллисекундах

// Существа
#define ORDER 'o'
#define ALFA_ORDER 'O'
#define CHAOS 'x'
#define ALFA_CHAOS 'X'

// Цветовые пары для ncurses
#define ORDER_COLOR_PAIR 1
#define ALFA_ORDER_COLOR_PAIR 2
#define CHAOS_COLOR_PAIR 3
#define ALFA_CHAOS_COLOR_PAIR 4
#define BORDER_COLOR_PAIR 5

// Прототипы функций
void init_stars(astra stars[], int count);
void move_stars(astra stars[], int count);
void draw_pool_ncurses(int pool[HI][LE]);

#endif // MAIN_LOGIC_H
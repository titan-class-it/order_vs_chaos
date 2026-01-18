#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>



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

// Увеличиваем количество звезд для размножения
#define HI 30
#define LE 90
#define QUANTITY_STARS 100  // Увеличили для размножения
#define SPEED 100

#define ORDER 'o'
#define ALFA_ORDER 'O'
#define CHAOS 'x'
#define ALFA_CHAOS 'X'

#define ORDER_COLOR_PAIR 1
#define ALFA_ORDER_COLOR_PAIR 2
#define CHAOS_COLOR_PAIR 3
#define ALFA_CHAOS_COLOR_PAIR 4
#define BORDER_COLOR_PAIR 5

// Прототипы
void init_stars(astra stars[], int count);
void move_stars(astra stars[], int count);
void draw_pool_ncurses(int pool[HI][LE]);

#endif
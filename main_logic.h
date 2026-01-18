#ifndef MAIN_LOGIC_H
#define MAIN_LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Условные include для разных платформ
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>  // для usleep в Linux/Mac
#endif

typedef struct
{
    int pixel_x;
    int pixel_y;
    int move_on_x;
    int move_on_y;
    char type;
    int color;
    int score;         // Счетчик съеденных ORDER для CHAOS
    int is_active;     // Активен ли объект (1) или уничтожен (0)
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

// Цвета (Windows)
#define ORDER_COLOR 10         // Ярко-зеленый
#define ALFA_ORDER_COLOR 10    // Ярко-зеленый
#define CHAOS_COLOR 12         // Ярко-красный
#define ALFA_CHAOS_COLOR 13    // Ярко-пурпурный
#define BORDER_COLOR 8         // Темно-серый для границ

// Прототипы функций
void set_border(int pool[HI][LE]);
void set_star_position(int pool[HI][LE], int x, int y, int value);
void clear_position(int pool[HI][LE], int x, int y);
void draw_pool_new(int pool[HI][LE]);
void draw_pixel(int pool[HI][LE], int x, int y);
void clear_pixel(int pool[HI][LE], int x, int y);
void clear_pool();
void init_stars(astra stars[], int count);
void set_text_color(int color_code);
void reset_text_color();
void move_stars(astra stars[], int count);

#endif // MAIN_LOGIC_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <windows.h>
#include <time.h>

typedef struct
{
	int pixel_x;
	int pixel_y;
	int move_on_x;
	int move_on_y;
	char type;
	int color;
	int score;	   // Счетчик съеденных ORDER для CHAOS
	int is_active; // Активен ли объект (1) или уничтожен (0)
} astra;

// Настройки
#define HI 30
#define LE 90
#define QUANTITY_STARS 50
#define SPEED 100

// Коды цветов для Windows Console
#define COLOR_RED 12   // Ярко-красный
#define COLOR_GREEN 10 // Ярко-зеленый
#define COLOR_BLUE 9   // Ярко-синий

// Существа
#define ORDER 'O'
#define CHAOS 'X'
#define STONE '#'
#define ALFA_CHAOS '@'

// Цвета (Windows)
#define ORDER_COLOR 10		// Ярко-зеленый
#define CHAOS_COLOR 12		// Ярко-красный
#define ALFA_CHAOS_COLOR 13 // Ярко-пурпурный
#define STONE_COLOR 7		// Белый/серый (стандартный)
#define BORDER_COLOR 8		// Темно-серый для границ

void draw_pool(int pool[HI][LE], astra stars[], int count);
void draw_pixel(int pool[HI][LE], int x, int y);
void clear_pixel(int pool[HI][LE], int x, int y);
void clear_pool();
void init_stars(astra stars[], int count); // Новая функция для инициализации звезд
// Цвета
void set_text_color(int color_code);
void reset_text_color();
// Пожиратели
void check_collisions(astra stars[], int count);
void handle_collision(astra *star1, astra *star2);
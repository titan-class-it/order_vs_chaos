#include "main_logic.h"
#include "order_logic.h"
#include "chaos_logic.h"

#include "main_logic.h"
#include "order_logic.h"
#include "chaos_logic.h"

astra stars[QUANTITY_STARS]; // Объявление массива звезд

int main() {
    int pool[HI][LE];
    
    // Инициализация случайных чисел
    srand(time(NULL));
    
    // Инициализируем границы
    set_border(pool);
    
    // Инициализируем звезды
    init_stars(stars, QUANTITY_STARS);
    
    while (1) {
        // Очищаем старые позиции звезд
        for (int i = 0; i < QUANTITY_STARS; i++) {
            if (stars[i].is_active) {
                clear_position(pool, stars[i].pixel_x, stars[i].pixel_y);
            }
        }
        
        // Двигаем звезды
        move_stars(stars, QUANTITY_STARS);
        
        check_collisions(stars, QUANTITY_STARS);
        
        // Ставим новые позиции
        for (int i = 0; i < QUANTITY_STARS; i++) {
            if (stars[i].is_active) {
                int value;
                switch(stars[i].type) {
                    case 'o': value = 2; break;  // ORDER
                    case 'O': value = 3; break;  // ALFA_ORDER
                    case 'x': value = 4; break;  // CHAOS
                    case 'X': value = 5; break;  // ALFA_CHAOS
                    default:  value = 0;
                }
                set_star_position(pool, stars[i].pixel_x, stars[i].pixel_y, value);
            }
        }
        
        clear_pool();
        draw_pool_new(pool);
        
        // Задержка для анимации (исправленная версия)
#ifdef _WIN32
        Sleep(SPEED); // Windows
#else
        sleep(SPEED * 1000); // Linux/Mac (переводим миллисекунды в микросекунды)
#endif
    }
    
    return 0;
}

// Остальные функции остаются без изменений...

// Функция инициализации звезд
void init_stars(astra stars[], int count) {
  for (int i = 0; i < count; i++) {
    // Случайные координаты внутри поля (1..LE-2 и 1..HI-2)
    stars[i].pixel_x = (rand() % (LE - 2)) + 1;
    stars[i].pixel_y = (rand() % (HI - 2)) + 1;

    // Случайное направление: -1, 0 или 1
    stars[i].move_on_x = (rand() % 3) - 1;
    stars[i].move_on_y = (rand() % 3) - 1;

    // Гарантируем, что звездочка не стоит на месте
    if (stars[i].move_on_x == 0 && stars[i].move_on_y == 0) {
      stars[i].move_on_x = 1; // задаем движение по умолчанию
    }

    if (rand() % 2 == 0) {
      stars[i].type = ORDER;
      stars[i].color = ORDER_COLOR;
    } else {
      stars[i].type = CHAOS;
      stars[i].color = CHAOS_COLOR;
    }

    stars[i].score = 0;     // Начальный счет
    stars[i].is_active = 1; // Все активны
  }
}

void draw_pool(int pool[HI][LE], astra stars[], int count) {

  // Очищаем массив pool
  for (int i = 0; i < HI; i++) {
    for (int j = 0; j < LE; j++) {
      pool[i][j] = -1; // -1 означает пусто
    }
  }

  // Заполняем массив pool индексами звезд
  for (int k = 0; k < count; k++) {
    if (stars[k].pixel_x >= 0 && stars[k].pixel_x < LE &&
        stars[k].pixel_y >= 0 && stars[k].pixel_y < HI) {
      pool[stars[k].pixel_y][stars[k].pixel_x] = k;
    }
  }

  for (int i = 0; i < HI; i++) {
    for (int j = 0; j < LE; j++) {
      // Границы
      if ((i == 0) || (i == (HI - 1)) || (j == 0 || j == (LE - 1))) {
        set_text_color(BORDER_COLOR);
        printf("*");
        reset_text_color();
      }
      // Объекты
      else if (pool[i][j] != -1) {
        int star_index = pool[i][j];
        set_text_color(stars[star_index].color);
        printf("%c", stars[star_index].type); // ORDER или CHAOS
        reset_text_color();
      }
      // Пустота
      else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void draw_pixel(int pool[HI][LE], int x, int y) {
  if (x >= 0 && x < LE && y >= 0 && y < HI) {
    pool[y][x] = 2;
  }
}

void clear_pixel(int pool[HI][LE], int x, int y) {
  if (x >= 0 && x < LE && y >= 0 && y < HI) {
    pool[y][x] = 0;
  }
}

void clear_pool() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// Функция установки цвета текста
// Функция установки цвета текста (кросс-платформенная)
void set_text_color(int color_code) {
#ifdef _WIN32
  // Windows версия
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color_code);
#else
  // Linux/Mac версия (ANSI escape codes)
  switch (color_code) {
  case ORDER_COLOR:
    printf("\033[32m");
    break; // зеленый
  case CHAOS_COLOR:
    printf("\033[31m");
    break; // красный
  case ALFA_CHAOS_COLOR:
    printf("\033[35m");
    break; // пурпурный
  case BORDER_COLOR:
    printf("\033[90m");
    break; // серый
  default:
    printf("\033[0m");
    break; // сброс
  }
#endif
}

// Функция сброса цвета
void reset_text_color() {
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, 7);
#else
  printf("\033[0m"); // сброс цвета в Linux/Mac
#endif
}

// // Подсчет количества существ каждого типа
// void count_stars(astra stars[], int count, int *order_count, int
// *chaos_count)
// {
// 	*order_count = 0;
// 	*chaos_count = 0;

// 	for (int i = 0; i < count; i++)
// 	{
// 		if (stars[i].type == ORDER)
// 			(*order_count)++;
// 		else if (stars[i].type == CHAOS)
// 			(*chaos_count)++;
// 	}
// }

// // Изменение типа существа (для будущей механики)
// void change_star_type(astra *star, char new_type)
// {
// 	star->type = new_type;

// 	switch (new_type)
// 	{
// 	case ORDER:
// 		star->color = COLOR_ORDER;
// 		star->move_on_x = 0; // Останавливаем движение
// 		star->move_on_y = 0;
// 		break;
// 	case CHAOS:
// 		star->color = COLOR_CHAOS;
// 		// Можно задать случайное движение
// 		star->move_on_x = (rand() % 3) - 1;
// 		star->move_on_y = (rand() % 3) - 1;
// 		break;
// 	case STONE:
// 		star->color = COLOR_STONE;
// 		star->move_on_x = 0;
// 		star->move_on_y = 0;
// 		break;
// 	}
// }



// Установка границ
void set_border(int pool[HI][LE]) {
    for (int i = 0; i < HI; i++) {
        for (int j = 0; j < LE; j++) {
            if (i == 0 || i == HI - 1 || j == 0 || j == LE - 1) {
                pool[i][j] = 1;  // граница = 1
            } else {
                pool[i][j] = 0;  // пусто = 0
            }
        }
    }
}

// Установка позиции звезды с определенным значением
void set_star_position(int pool[HI][LE], int x, int y, int value) {
    if (x >= 1 && x < LE - 1 && y >= 1 && y < HI - 1) {
        pool[y][x] = value;  // значение зависит от типа существа
    }
}

// Очистка позиции
void clear_position(int pool[HI][LE], int x, int y) {
    if (x >= 1 && x < LE - 1 && y >= 1 && y < HI - 1) {
        pool[y][x] = 0;  // пусто
    }
}

// Новая функция отрисовки
void draw_pool_new(int pool[HI][LE]) {
    for (int i = 0; i < HI; i++) {
        for (int j = 0; j < LE; j++) {
            int value = pool[i][j];
            
            if (value == 1) {  // граница
                set_text_color(BORDER_COLOR);
                printf("*");
                reset_text_color();
            }
            else if (value == 2) {  // ORDER 'o'
                set_text_color(ORDER_COLOR);
                printf("o");
                reset_text_color();
            }
            else if (value == 3) {  // ALFA_ORDER 'O'
                set_text_color(ALFA_ORDER_COLOR);
                printf("O");
                reset_text_color();
            }
            else if (value == 4) {  // CHAOS 'x'
                set_text_color(CHAOS_COLOR);
                printf("x");
                reset_text_color();
            }
            else if (value == 5) {  // ALFA_CHAOS 'X'
                set_text_color(ALFA_CHAOS_COLOR);
                printf("X");
                reset_text_color();
            }
            else {
                printf(" ");  // пусто (0)
            }
        }
        printf("\n");
    }
}

// Функция движения звезд (добавь после init_stars)
void move_stars(astra stars[], int count) {
    for (int i = 0; i < count; i++) {
        if (!stars[i].is_active)
            continue;
        
        // Сохраняем старые координаты
        int old_x = stars[i].pixel_x;
        int old_y = stars[i].pixel_y;
        
        // Вычисляем новые координаты
        int new_x = old_x + stars[i].move_on_x;
        int new_y = old_y + stars[i].move_on_y;
        
        // Проверяем столкновение с границами
        if (new_x <= 0 || new_x >= LE - 1) {
            stars[i].move_on_x = -stars[i].move_on_x; // Отскок по X
            new_x = old_x + stars[i].move_on_x; // Пересчитываем с новым направлением
        }
        
        if (new_y <= 0 || new_y >= HI - 1) {
            stars[i].move_on_y = -stars[i].move_on_y; // Отскок по Y
            new_y = old_y + stars[i].move_on_y; // Пересчитываем с новым направлением
        }
        
        // Устанавливаем новые координаты
        stars[i].pixel_x = new_x;
        stars[i].pixel_y = new_y;
    }
}
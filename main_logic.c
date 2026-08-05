#include "main_logic.h"
#include "collisions.h"
#include "chaos_logic.h"
#include "order_logic.h"

astra stars[QUANTITY_STARS];

// Функция инициализации звезд
void init_stars(astra stars[], int count) {
    for (int i = 0; i < count; i++) {
        stars[i].pixel_x = (rand() % (LE - 2)) + 1;
        stars[i].pixel_y = (rand() % (HI - 2)) + 1;
        
        stars[i].move_on_x = (rand() % 3) - 1;
        stars[i].move_on_y = (rand() % 3) - 1;
        
        if (stars[i].move_on_x == 0 && stars[i].move_on_y == 0) {
            stars[i].move_on_x = 1;
        }
        
        if (rand() % 2 == 0) {
            stars[i].type = ORDER;
        } else {
            stars[i].type = CHAOS;
        }
        
        stars[i].score = 0;
        stars[i].is_active = 1;
    }
}

// Функция движения звезд (исправленная)
void move_stars(astra stars[], int count) {
    for (int i = 0; i < count; i++) {
        if (!stars[i].is_active)
            continue;
        
        // Вычисляем новые координаты
        int new_x = stars[i].pixel_x + stars[i].move_on_x;
        int new_y = stars[i].pixel_y + stars[i].move_on_y;
        
        // Проверяем столкновение с границами
        if (new_x <= 0 || new_x >= LE - 1) {
            stars[i].move_on_x = -stars[i].move_on_x;
            continue; // Отскакиваем без движения в этом кадре
        }
        
        if (new_y <= 0 || new_y >= HI - 1) {
            stars[i].move_on_y = -stars[i].move_on_y;
            continue; // Отскакиваем без движения в этом кадре
        }
        
        // Устанавливаем новые координаты
        stars[i].pixel_x = new_x;
        stars[i].pixel_y = new_y;
    }
}

// Отрисовка с использованием ncurses
void draw_pool_ncurses(int pool[HI][LE]) {
    for (int i = 0; i < HI; i++) {
        for (int j = 0; j < LE; j++) {
            int value = pool[i][j];
            
            move(i, j); // Устанавливаем позицию курсора
            
            if (i == 0 || i == HI - 1 || j == 0 || j == LE - 1) {
                // Границы - всегда отрисовываем
                attron(COLOR_PAIR(BORDER_COLOR_PAIR));
                addch('*');
                attroff(COLOR_PAIR(BORDER_COLOR_PAIR));
            }
            else if (value == 2) { // ORDER 'o' - зеленый
                attron(COLOR_PAIR(ORDER_COLOR_PAIR));
                addch('o');
                attroff(COLOR_PAIR(ORDER_COLOR_PAIR));
            }
            else if (value == 3) { // ALFA_ORDER 'O' - ярко-зеленый
                attron(COLOR_PAIR(ALFA_ORDER_COLOR_PAIR) | A_BOLD);
                addch('O');
                attroff(COLOR_PAIR(ALFA_ORDER_COLOR_PAIR) | A_BOLD);
            }
            else if (value == 4) { // CHAOS 'x' - красный
                attron(COLOR_PAIR(CHAOS_COLOR_PAIR));
                addch('x');
                attroff(COLOR_PAIR(CHAOS_COLOR_PAIR));
            }
            else if (value == 5) { // ALFA_CHAOS 'X' - ярко-красный
                attron(COLOR_PAIR(ALFA_CHAOS_COLOR_PAIR) | A_BOLD);
                addch('X');
                attroff(COLOR_PAIR(ALFA_CHAOS_COLOR_PAIR) | A_BOLD);
            }
            else if (value == 6) { // MEGA_CHAOS 'M' - ярко-пурпурный
                attron(COLOR_PAIR(MEGA_CHAOS_COLOR_PAIR) | A_BOLD);
                addch('M');
                attroff(COLOR_PAIR(MEGA_CHAOS_COLOR_PAIR) | A_BOLD);
            }
            else {
                addch(' ');
            }
        }
    }
}

int main() {
    int pool[HI][LE];
    // int active_star_count = QUANTITY_STARS;
    
    // Инициализация ncurses
    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    
    // Инициализация цветов ncurses
    if (has_colors()) {
        start_color();
        
        // ORDER - зеленый
        init_pair(ORDER_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
        
        // ALFA_ORDER - ярко-зеленый (зеленый с жирным)
        init_pair(ALFA_ORDER_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
        
        // CHAOS - красный
        init_pair(CHAOS_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
        
        // ALFA_CHAOS - ярко-красный (красный с жирным)
        init_pair(ALFA_CHAOS_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
        
        // MEGA_CHAOS - ярко-пурпурный (пурпурный с жирным)
        init_pair(MEGA_CHAOS_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
        
        // Границы - белый
        init_pair(BORDER_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
    }
    
    // Инициализация случайных чисел
    srand(time(NULL));
    
    // Инициализируем массив pool
    for (int i = 0; i < HI; i++) {
        for (int j = 0; j < LE; j++) {
            pool[i][j] = 0;
        }
    }
    
    // Инициализируем звезды
    init_stars(stars, QUANTITY_STARS);
    
    // Основной игровой цикл
    while (1) {
        clear();
        
        // Очищаем старые позиции в pool (кроме границ)
        for (int i = 1; i < HI - 1; i++) {
            for (int j = 1; j < LE - 1; j++) {
                pool[i][j] = 0;
            }
        }
        
        // Двигаем звезды
        move_stars(stars, QUANTITY_STARS);
        
        // Проверяем столкновения
        check_collisions(stars, QUANTITY_STARS);
        
        // Обновляем pool
        for (int i = 0; i < QUANTITY_STARS; i++) {
            if (stars[i].is_active) {
                int value;
                switch(stars[i].type) {
                    case ORDER: value = 2; break;        // 'o' - зеленый
                    case ALFA_ORDER: value = 3; break;   // 'O' - ярко-зеленый
                    case CHAOS: value = 4; break;        // 'x' - красный
                    case ALFA_CHAOS: value = 5; break;   // 'X' - ярко-красный
                    case MEGA_CHAOS: value = 6; break;   // 'M' - ярко-пурпурный
                    default:  value = 0;
                }
                
                // Проверяем, что позиция внутри поля
                if (stars[i].pixel_x > 0 && stars[i].pixel_x < LE - 1 &&
                    stars[i].pixel_y > 0 && stars[i].pixel_y < HI - 1) {
                    pool[stars[i].pixel_y][stars[i].pixel_x] = value;
                }
            }
        }
        
        // Отрисовываем
        draw_pool_ncurses(pool);
        
        // Обновляем экран
        refresh();
        
        // Задержка
        napms(SPEED);
        
        // Проверка нажатия клавиши для выхода (q или ESC)
        int ch = getch();
        if (ch == 'q' || ch == 'Q' || ch == 27) {
            break;
        }
    }
    
    // Завершение ncurses
    endwin();
    return 0;
}
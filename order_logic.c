#include "order_logic.h"
#include <stdlib.h>

// Функция проверки, занята ли клетка
int is_cell_occupied(astra stars[], int max_count, int x, int y) {
    for (int i = 0; i < max_count; i++) {
        if (stars[i].is_active && 
            stars[i].pixel_x == x && 
            stars[i].pixel_y == y) {
            return 1;
        }
    }
    return 0;
}

void order_reproduce(astra *star1, astra *star2, astra stars[], int *star_count, int max_count) {
    // Используем параметры, чтобы убрать предупреждения
    (void)star2; // Пока не используется, но оставляем для будущего
    
    // Только если есть свободное место
    if (*star_count >= max_count) {
        return;
    }
    
    // Ищем свободное место рядом
    int attempts = 0;
    int new_x, new_y;
    
    do {
        // Случайное смещение в пределах 1 клетки
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;
        
        new_x = star1->pixel_x + dx;
        new_y = star1->pixel_y + dy;
        
        attempts++;
        
        // Проверяем границы
        if (new_x <= 0 || new_x >= LE - 1 || 
            new_y <= 0 || new_y >= HI - 1) {
            continue;
        }
        
    } while (attempts < 100 && 
             is_cell_occupied(stars, max_count, new_x, new_y));
    
    if (attempts < 100) {
        // Создаем новую звезду ORDER
        for (int i = 0; i < max_count; i++) {
            if (!stars[i].is_active) {
                stars[i].pixel_x = new_x;
                stars[i].pixel_y = new_y;
                stars[i].move_on_x = (rand() % 3) - 1;
                stars[i].move_on_y = (rand() % 3) - 1;
                stars[i].type = ORDER;
                stars[i].score = 0;
                stars[i].is_active = 1;
                (*star_count)++;
                break;
            }
        }
    }
}
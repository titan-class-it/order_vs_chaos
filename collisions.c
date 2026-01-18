// #include "collisions.h"
#include "main_logic.h"
// #include <stdlib.h>

// Функция проверки столкновений между звездами
void check_collisions(astra stars[], int *active_star_count, int max_count) {
    for (int i = 0; i < max_count; i++) {
        if (!stars[i].is_active) continue;
        
        for (int j = i + 1; j < max_count; j++) {
            if (!stars[j].is_active) continue;
            
            // Проверяем, находятся ли звезды на одной клетке
            if (stars[i].pixel_x == stars[j].pixel_x && 
                stars[i].pixel_y == stars[j].pixel_y) {
                
                // Разные типы звезд - уничтожают друг друга
                if (stars[i].type != stars[j].type) {
                    stars[i].is_active = 0;
                    stars[j].is_active = 0;
                    *active_star_count -= 2;
                } 
                // Одинаковые типы - остаются активными
                // Можно добавить дополнительную логику здесь
            }
        }
    }
}
// #include "order_logic.h"
// #include <stdlib.h>

// // Функция проверки, занята ли клетка
// int is_cell_occupied(astra stars[], int max_count, int x, int y) {
//     for (int i = 0; i < max_count; i++) {
//         if (stars[i].is_active && 
//             stars[i].pixel_x == x && 
//             stars[i].pixel_y == y) {
//             return 1;
//         }
//     }
//     return 0;
// }

// // Вспомогательная функция для поиска неактивной звезды
// int find_inactive_star_idx(astra stars[], int max_count) {
//     for (int i = 0; i < max_count; i++) {
//         if (!stars[i].is_active) {
//             return i;
//         }
//     }
//     return -1;
// }

// void order_reproduce(astra *star1, astra *star2, astra stars[], int max_count) {
//     // Используем параметры, чтобы убрать предупреждения
//     (void)star2; // Пока не используется, но оставляем для будущего
    
//     // Ищем свободное место рядом
//     int attempts = 0;
//     int new_x, new_y;
    
//     do {
//         // Случайное смещение в пределах 1 клетки
//         int dx = (rand() % 3) - 1;
//         int dy = (rand() % 3) - 1;
        
//         new_x = star1->pixel_x + dx;
//         new_y = star1->pixel_y + dy;
        
//         attempts++;
        
//         // Проверяем границы
//         if (new_x <= 0 || new_x >= LE - 1 || 
//             new_y <= 0 || new_y >= HI - 1) {
//             continue;
//         }
        
//     } while (attempts < 100 && 
//              is_cell_occupied(stars, max_count, new_x, new_y));
    
//     if (attempts < 100) {
//         // Создаем новую звезду ORDER
//         int idx = find_inactive_star_idx(stars, max_count);
//         if (idx != -1) {
//             stars[idx].pixel_x = new_x;
//             stars[idx].pixel_y = new_y;
//             stars[idx].move_on_x = (rand() % 3) - 1;
//             if (stars[idx].move_on_x == 0) stars[idx].move_on_x = 1;
//             stars[idx].move_on_y = (rand() % 3) - 1;
//             stars[idx].type = ORDER;
//             stars[idx].score = 0;
//             stars[idx].is_active = 1;
//         }
//         // Если нет свободных мест, ничего не делаем
//     }
// }

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

// Вспомогательная функция для поиска неактивной звезды
int find_inactive_star_idx(astra stars[], int max_count) {
    for (int i = 0; i < max_count; i++) {
        if (!stars[i].is_active) {
            return i;
        }
    }
    return -1;
}

// Функция поиска свободной клетки вокруг точки
int find_free_cell_around(astra stars[], int max_count, int base_x, int base_y, int *result_x, int *result_y) {
    int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    // Пробуем все направления в случайном порядке
    int indices[8] = {0,1,2,3,4,5,6,7};
    for (int i = 0; i < 8; i++) {
        int j = rand() % 8;
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    
    for (int attempt = 0; attempt < 8; attempt++) {
        int idx = indices[attempt];
        int new_x = base_x + directions[idx][0];
        int new_y = base_y + directions[idx][1];
        
        if (new_x > 0 && new_x < LE - 1 && 
            new_y > 0 && new_y < HI - 1 &&
            !is_cell_occupied(stars, max_count, new_x, new_y)) {
            *result_x = new_x;
            *result_y = new_y;
            return 1;
        }
    }
    return 0;
}

void order_reproduce(astra *star1, astra *star2, astra stars[], int max_count) {
    int created_count = 0;
    
    // Пытаемся создать 2 новых ORDER (по одному рядом с каждым родителем)
    astra *parents[2] = {star1, star2};
    
    for (int p = 0; p < 2; p++) {
        int new_x, new_y;
        
        // Ищем свободную клетку рядом с родителем
        if (find_free_cell_around(stars, max_count, 
                                   parents[p]->pixel_x, 
                                   parents[p]->pixel_y, 
                                   &new_x, &new_y)) {
            int idx = find_inactive_star_idx(stars, max_count);
            if (idx != -1) {
                // Создаем нового ORDER
                stars[idx].pixel_x = new_x;
                stars[idx].pixel_y = new_y;
                stars[idx].move_on_x = (rand() % 3) - 1;
                if (stars[idx].move_on_x == 0) stars[idx].move_on_x = 1;
                stars[idx].move_on_y = (rand() % 3) - 1;
                if (stars[idx].move_on_y == 0) stars[idx].move_on_y = 1;
                stars[idx].type = ORDER;  // Создаем обычный ORDER
                stars[idx].score = 0;
                stars[idx].is_active = 1;
                created_count++;
            }
        }
    }
    
    // Увеличиваем счетчик размножений у родителей только если создали потомков
    if (created_count > 0) {
        star1->score++;
        star2->score++;
    }
    
    // Если ALFA_ORDER набрали 10 размножений, превращаются в ORDER? 
    // Или может в супер-ALFA_ORDER?
    if (star1->score >= 10 && star1->type == ALFA_ORDER) {
        // Можно превратить в что-то новое или оставить как есть
        // Например, сбросить счетчик и создать дополнительного ORDER
        star1->score = 0;
        // Или превратить в супер-ALFA_ORDER (если добавить новый тип)
    }
    if (star2->score >= 10 && star2->type == ALFA_ORDER) {
        star2->score = 0;
    }
}
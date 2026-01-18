#include "chaos_logic.h"
#include "order_logic.h"

extern void order_reproduce(astra *star1, astra *star2, astra stars[], int max_count);
extern int is_cell_occupied(astra stars[], int count, int x, int y);

// Вспомогательная функция для поиска неактивной звезды
int find_inactive_star(astra stars[], int max_count) {
    for (int i = 0; i < max_count; i++) {
        if (!stars[i].is_active) {
            return i;
        }
    }
    return -1; // Все звезды активны
}

// Вспомогательная функция для поиска свободной клетки
int find_free_cell(astra stars[], int max_count, int base_x, int base_y, int *result_x, int *result_y) {
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

void handle_collision(astra *star1, astra *star2, astra stars[], int max_count)
{
    // ORDER + ORDER = создают 2 новых ORDER
    if (star1->type == ORDER && star2->type == ORDER)
    {
        // Увеличиваем счетчик размножений
        star1->score++;
        star2->score++;
        
        // Создаем 2 новых ORDER, если есть место
        for (int i = 0; i < 2; i++) {
            int new_x, new_y;
            if (find_free_cell(stars, max_count, star1->pixel_x, star1->pixel_y, &new_x, &new_y)) {
                int index = find_inactive_star(stars, max_count);
                if (index != -1) { // Нашли неактивную звезду
                    stars[index].pixel_x = new_x;
                    stars[index].pixel_y = new_y;
                    stars[index].move_on_x = (rand() % 3) - 1;
                    if (stars[index].move_on_x == 0) stars[index].move_on_x = 1;
                    stars[index].move_on_y = (rand() % 3) - 1;
                    stars[index].type = ORDER;
                    stars[index].score = 0;
                    stars[index].is_active = 1;
                }
                // Если нет свободных мест, просто игнорируем создание новой звезды
            }
        }
        
        // Проверяем превращение в ALFA_ORDER после 5 размножений
        if (star1->score >= 5 && star1->type == ORDER) {
            star1->type = ALFA_ORDER;
            star1->score = 0;
        }
        if (star2->score >= 5 && star2->type == ORDER) {
            star2->type = ALFA_ORDER;
            star2->score = 0;
        }
        
        // Меняем направления для разбегания
        int temp_x = star1->move_on_x;
        int temp_y = star1->move_on_y;
        star1->move_on_x = star2->move_on_x;
        star1->move_on_y = star2->move_on_y;
        star2->move_on_x = temp_x;
        star2->move_on_y = temp_y;
    }
    // CHAOS + ORDER = CHAOS ест ORDER
    else if ((star1->type == CHAOS && star2->type == ORDER) ||
             (star2->type == CHAOS && star1->type == ORDER))
    {
        astra *chaos_star = (star1->type == CHAOS) ? star1 : star2;
        astra *order_star = (star1->type == ORDER) ? star1 : star2;
        
        chaos_star->score++;
        order_star->is_active = 0; // ORDER становится неактивным
        
        // Если CHAOS съел 10 ORDER, превращаем в ALFA_CHAOS
        if (chaos_star->score >= 10 && chaos_star->type == CHAOS) {
            chaos_star->type = ALFA_CHAOS;
            chaos_star->score = 0;
        }
    }
    // ALFA_ORDER + ALFA_ORDER = размножаются
    else if (star1->type == ALFA_ORDER && star2->type == ALFA_ORDER)
    {
        order_reproduce(star1, star2, stars, max_count);
        
        int temp_x = star1->move_on_x;
        int temp_y = star1->move_on_y;
        star1->move_on_x = star2->move_on_x;
        star1->move_on_y = star2->move_on_y;
        star2->move_on_x = temp_x;
        star2->move_on_y = temp_y;
    }
    // ALFA_CHAOS ест CHAOS и ORDER
    else if ((star1->type == ALFA_CHAOS && (star2->type == CHAOS || star2->type == ORDER)) ||
             (star2->type == ALFA_CHAOS && (star1->type == CHAOS || star1->type == ORDER)))
    {
        astra *alfa_chaos = (star1->type == ALFA_CHAOS) ? star1 : star2;
        astra *other = (star1->type == ALFA_CHAOS) ? star2 : star1;
        
        alfa_chaos->score++;
        other->is_active = 0;
        
        // ALFA_CHAOS после 50 съеденных становится MEGA_CHAOS
        if (alfa_chaos->score >= 50 && alfa_chaos->type == ALFA_CHAOS) {
            alfa_chaos->type = MEGA_CHAOS;
            alfa_chaos->score = 0;
        }
    }
    // MEGA_CHAOS ест всех кроме ALFA_ORDER
    else if ((star1->type == MEGA_CHAOS && star2->type != ALFA_ORDER) ||
             (star2->type == MEGA_CHAOS && star1->type != ALFA_ORDER))
    {
        astra *mega_chaos = (star1->type == MEGA_CHAOS) ? star1 : star2;
        astra *other = (star1->type == MEGA_CHAOS) ? star2 : star1;
        
        if (other->type != ALFA_ORDER) {
            mega_chaos->score++;
            other->is_active = 0;
        }
    }
    // CHAOS и CHAOS - отскок
    else if (star1->type == CHAOS && star2->type == CHAOS)
    {
        int temp_x = star1->move_on_x;
        int temp_y = star1->move_on_y;
        star1->move_on_x = star2->move_on_x;
        star1->move_on_y = star2->move_on_y;
        star2->move_on_x = temp_x;
        star2->move_on_y = temp_y;
    }
    // Взаимное уничтожение разных типов (кроме вышеперечисленных случаев)
    else if (star1->type != star2->type)
    {
        star1->is_active = 0;
        star2->is_active = 0;
    }
}
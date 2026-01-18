#include "chaos_logic.h"
#include "order_logic.h"

// Добавь этот прототип вверху файла, если функция не объявлена в order_logic.h
extern void order_reproduce(astra *star1, astra *star2, astra stars[], int *star_count, int max_count);
extern int is_cell_occupied(astra stars[], int count, int x, int y);

void handle_collision(astra *star1, astra *star2, astra stars[], int *star_count, int max_count)
{
    // ORDER и ORDER - размножаются
    if (star1->type == ORDER && star2->type == ORDER)
    {
        // Вызываем функцию размножения
        order_reproduce(star1, star2, stars, star_count, max_count);
        
        // После размножения родители продолжают движение
        // Меняем направления для естественного разбегания
        int temp_x = star1->move_on_x;
        int temp_y = star1->move_on_y;
        star1->move_on_x = star2->move_on_x;
        star1->move_on_y = star2->move_on_y;
        star2->move_on_x = temp_x;
        star2->move_on_y = temp_y;
    }
    // CHAOS ест ORDER (остается без изменений)
    else if (star1->type == CHAOS && star2->type == ORDER)
    {
        star1->score++;         // Увеличиваем счетчик
        star2->is_active = 0;   // ORDER уничтожен

        // Если CHAOS съел 10 ORDER, превращаем в ALFA_CHAOS
        if (star1->score >= 10 && star1->type == CHAOS)
        {
            star1->type = ALFA_CHAOS;
            star1->move_on_x *= 2; // Увеличиваем скорость
            star1->move_on_y *= 2;
        }
    }
    else if (star2->type == CHAOS && star1->type == ORDER)
    {
        star2->score++;
        star1->is_active = 0;

        if (star2->score >= 10 && star2->type == CHAOS)
        {
            star2->type = ALFA_CHAOS;
            star2->move_on_x *= 2;
            star2->move_on_y *= 2;
        }
    }
    // ALFA_CHAOS ест всех (остается без изменений)
    else if (star1->type == ALFA_CHAOS &&
             (star2->type == ORDER || star2->type == CHAOS))
    {
        star2->is_active = 0;
        star1->score++;
    }
    else if (star2->type == ALFA_CHAOS &&
             (star1->type == ORDER || star1->type == CHAOS))
    {
        star1->is_active = 0;
        star2->score++;
    }
    // CHAOS и CHAOS - отскок (как и раньше)
    else if (star1->type == CHAOS && star2->type == CHAOS)
    {
        // Простой отскок - меняем направления
        int temp_x = star1->move_on_x;
        int temp_y = star1->move_on_y;
        star1->move_on_x = star2->move_on_x;
        star1->move_on_y = star2->move_on_y;
        star2->move_on_x = temp_x;
        star2->move_on_y = temp_y;
    }
}
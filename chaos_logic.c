#include "chaos_logic.h"

void check_collisions(astra stars[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (!stars[i].is_active)
			continue; // Пропускаем неактивных

		for (int j = 0; j < count; j++)
		{
			if (i == j || !stars[j].is_active)
				continue; // Не проверяем с собой и неактивными

			// Проверяем столкновение (на одной клетке)
			if (stars[i].pixel_x == stars[j].pixel_x &&
				stars[i].pixel_y == stars[j].pixel_y)
			{
				handle_collision(&stars[i], &stars[j]);
				break; // Выходим из внутреннего цикла после обработки столкновения
			}
		}
	}
}

void handle_collision(astra *star1, astra *star2)
{
	// CHAOS ест ORDER
	if (star1->type == CHAOS && star2->type == ORDER)
	{
		star1->score++;		  // Увеличиваем счетчик
		star2->is_active = 0; // ORDER уничтожен

		// Если CHAOS съел 10 ORDER, превращаем в ALFA_CHAOS
		if (star1->score >= 10 && star1->type == CHAOS)
		{
			star1->type = ALFA_CHAOS;
			star1->color = ALFA_CHAOS_COLOR;
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
			star2->color = ALFA_CHAOS_COLOR;
			star2->move_on_x *= 2;
			star2->move_on_y *= 2;
		}
	}
	// ALFA_CHAOS ест всех
	else if (star1->type == ALFA_CHAOS &&
			 (star2->type == ORDER || star2->type == CHAOS))
	{
		star2->is_active = 0;
		star1->score++; // Можно вести общий счет
	}
	else if (star2->type == ALFA_CHAOS &&
			 (star1->type == ORDER || star1->type == CHAOS))
	{
		star1->is_active = 0;
		star2->score++;
	}
	// ORDER и ORDER или CHAOS и CHAOS - просто меняем направление (отскок)
	else if (star1->type == star2->type)
	{
		// Простой отскок - меняем направления
		int temp_x = star1->move_on_x;
		int temp_y = star1->move_on_y;
		star1->move_on_x = star2->move_on_x;
		star1->move_on_y = star2->move_on_y;
		star2->move_on_x = temp_x;
		star2->move_on_y = temp_y;

		// Сдвигаем, чтобы не застряли вместе
		star1->pixel_x += star1->move_on_x;
		star1->pixel_y += star1->move_on_y;
		star2->pixel_x += star2->move_on_x;
		star2->pixel_y += star2->move_on_y;
	}
}
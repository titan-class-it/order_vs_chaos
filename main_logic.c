#include "main_logic.h"

int main()
{
	int pool[HI][LE] = {0};
	astra stars[QUANTITY_STARS];

	// Инициализируем генератор случайных чисел
	srand(time(NULL));

	// Инициализируем все звезды
	init_stars(stars, QUANTITY_STARS);

	while (1)
	{
		// Очищаем предыдущие позиции ВСЕХ звездочек
		for (int i = 0; i < QUANTITY_STARS; i++)
		{
			clear_pixel(pool, stars[i].pixel_x, stars[i].pixel_y);
		}

		// Обновляем координаты ВСЕХ звездочек
		for (int i = 0; i < QUANTITY_STARS; i++)
		{
			stars[i].pixel_x += stars[i].move_on_x;
			stars[i].pixel_y += stars[i].move_on_y;

			// Проверка границ для каждой звезды
			if (stars[i].pixel_x <= 1 || stars[i].pixel_x >= LE - 2)
			{
				stars[i].move_on_x = -stars[i].move_on_x;
			}
			if (stars[i].pixel_y <= 1 || stars[i].pixel_y >= HI - 2)
			{
				stars[i].move_on_y = -stars[i].move_on_y;
			}
		}

		// Рисуем активные звезды
		for (int i = 0; i < QUANTITY_STARS; i++)
		{
			if (stars[i].is_active)
			{
				draw_pixel(pool, stars[i].pixel_x, stars[i].pixel_y);
			}
		}

		clear_pool();
		draw_pool(pool, stars, QUANTITY_STARS);

		// Задержка для анимации
#ifdef _WIN32
		Sleep(SPEED); // Windows
#else
		usleep(100000); // Linux/Mac
#endif
	}

	return 0;
}

// Функция инициализации звезд
void init_stars(astra stars[], int count)
{
	for (int i = 0; i < count; i++)
	{
		// Случайные координаты внутри поля (1..LE-2 и 1..HI-2)
		stars[i].pixel_x = (rand() % (LE - 2)) + 1; // от 1 до 28
		stars[i].pixel_y = (rand() % (HI - 2)) + 1; // от 1 до 8

		// Случайное направление: -1, 0 или 1
		stars[i].move_on_x = (rand() % 3) - 1;
		stars[i].move_on_y = (rand() % 3) - 1;

		// Гарантируем, что звездочка не стоит на месте
		if (stars[i].move_on_x == 0 && stars[i].move_on_y == 0)
		{
			stars[i].move_on_x = 1; // задаем движение по умолчанию
		}

		if (rand() % 2 == 0)
		{
			stars[i].type = ORDER;
			stars[i].color = ORDER_COLOR;
		}
		else
		{
			stars[i].type = CHAOS;
			stars[i].color = CHAOS_COLOR;
		}

		stars[i].score = 0;		// Начальный счет
		stars[i].is_active = 1; // Все активны
	}
}

void draw_pool(int pool[HI][LE], astra stars[], int count)
{

	// Очищаем массив pool
	for (int i = 0; i < HI; i++)
	{
		for (int j = 0; j < LE; j++)
		{
			pool[i][j] = -1; // -1 означает пусто
		}
	}

	// Заполняем массив pool индексами звезд
	for (int k = 0; k < count; k++)
	{
		if (stars[k].pixel_x >= 0 && stars[k].pixel_x < LE &&
			stars[k].pixel_y >= 0 && stars[k].pixel_y < HI)
		{
			pool[stars[k].pixel_y][stars[k].pixel_x] = k;
		}
	}

	for (int i = 0; i < HI; i++)
	{
		for (int j = 0; j < LE; j++)
		{
			// Границы
			if ((i == 0) || (i == (HI - 1)) || (j == 0 || j == (LE - 1)))
			{
				set_text_color(BORDER_COLOR);
				printf("*");
				reset_text_color();
			}
			// Объекты
			else if (pool[i][j] != -1)
			{
				int star_index = pool[i][j];
				set_text_color(stars[star_index].color);
				printf("%c", stars[star_index].type); // ORDER или CHAOS
				reset_text_color();
			}
			// Пустота
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void draw_pixel(int pool[HI][LE], int x, int y)
{
	if (x >= 1 && x < LE - 1 && y >= 1 && y < HI - 1)
	{
		pool[y][x] = 2;
	}
}

void clear_pixel(int pool[HI][LE], int x, int y)
{
	if (x >= 1 && x < LE - 1 && y >= 1 && y < HI - 1)
	{
		pool[y][x] = 0;
	}
}

void clear_pool()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

// Функция установки цвета текста
void set_text_color(int color_code)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color_code);
}

// Функция сброса цвета к стандартному (белый/серый)
void reset_text_color()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7); // стандартный серый
}

// // Подсчет количества существ каждого типа
// void count_stars(astra stars[], int count, int *order_count, int *chaos_count)
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
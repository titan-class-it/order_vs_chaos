#ifndef ORDER_LOGIC_H
#define ORDER_LOGIC_H

#include "main_logic.h"

// Размножение
void order_reproduce(astra *star1, astra *star2, astra stars[], int max_count);
int is_cell_occupied(astra stars[], int max_count, int x, int y);

#endif
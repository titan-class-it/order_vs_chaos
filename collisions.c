#include "collisions.h"
#include "chaos_logic.h"
#include "order_logic.h"
#include <stdlib.h>

void check_collisions(astra stars[], int max_count) {
    for (int i = 0; i < max_count; i++) {
        if (!stars[i].is_active) continue;
        
        for (int j = i + 1; j < max_count; j++) {
            if (!stars[j].is_active) continue;
            
            if (stars[i].pixel_x == stars[j].pixel_x && 
                stars[i].pixel_y == stars[j].pixel_y) {
                
                // Вызываем handle_collision с правильными аргументами
                handle_collision(&stars[i], &stars[j], stars, max_count);
            }
        }
    }
}
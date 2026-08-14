#include "snake.h"
#include <stdlib.h>

SnakeSegment *seg_alloc(void){
    return malloc(sizeof(SnakeSegment));
}

void seg_free(SnakeSegment *seg) {
    free(seg);
}

void snake_init(Snake *snake, Point start_pos){
    //TODO
}

void snake_move(Snake *snake){
    //TODO
}

void snake_grow(Snake *snake, Point new_head_pos) {
    //TODO
}

bool snake_check_self_collision(const Snake *snake) {
    //TODO
}

void snake_destroy(Snake *snake) {
    //TODO
}
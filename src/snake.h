#ifndef SNAKE_H
#define SNAKE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t x, y;
} Point;

typedef enum {
    DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT
} Direction;

typedef struct SnakeSegment {
    Point pos;
    struct SnakeSegment *next;
    struct SnakeSegment *prev; //using a doubly linked list instead to make the tail removal cleaner and constant.
} SnakeSegment;

typedef struct {
    SnakeSegment *head;
    SnakeSegment *tail;
    size_t length;
    Direction dir;
} Snake;

/* Memory allocation thin wrapper. Phase 1 makes use of malloc/free while phase 2 swaps implementation without touching callers */
SnakeSegment *seg_alloc(void);
void seg_free(SnakeSegment *seg);

/* Core game logic to implement later*/
void snake_init(Snake *snake, Point start_pos, Direction start_dir);
void snake_move(Snake *snake);
void snake_grow(Snake *snake, Point new_head_pos);
bool snake_check_self_collision(const Snake *snake);
void snake_destroy(Snake *snake);

#endif
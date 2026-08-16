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
Point snake_next_head_pos(const Snake *snake);
bool points_equal(Point a, Point b);
void snake_init(Snake *snake, Point start_pos, Direction start_dir);
void snake_move(Snake *snake);
bool snake_out_of_bounds(Point pos, int max_y, int max_x);
void snake_grow(Snake *snake);
bool snake_check_self_collision(Point curr_pos, const Snake *snake, bool will_grow);
void snake_destroy(Snake *snake);
Point snake_spawn_food(const Snake *snake, int max_y, int max_x);

#endif
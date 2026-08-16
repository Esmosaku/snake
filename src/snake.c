#include "snake.h"
#include <stdlib.h>

SnakeSegment *seg_alloc(void){
    return malloc(sizeof(SnakeSegment));
}

void seg_free(SnakeSegment *seg) {
    free(seg);
}

Point snake_next_head_pos(const Snake *snake){
    
}

/*
* Allocate the head segment via seg_alloc()
* Set the snake head, tail, length, and direction accordingly.
* @param *snake the snake pointer of the Snake structure
* @param start_pos the starting position of the snake.
* @param start_dir the starting direction of the snake.
*/
void snake_init(Snake *snake, Point start_pos, Direction start_dir){
    SnakeSegment *head_segment = seg_alloc();
    if (head_segment == NULL){
        //TODO
    }
    head_segment -> pos = start_pos;

    snake -> head = head_segment;
    snake -> tail = head_segment;

    head_segment -> next = NULL;
    head_segment -> prev = NULL;

    snake -> length = 1;
    snake -> dir = start_dir;
}

/* 
* Computes the new head position based on the snake's current direction
* Allocates a new head segnment from that direction and frees the old tail segment
* @param *snake the snake pointer of the Snake structure.
*/
void snake_move(Snake *snake){

    Direction curr_dir = snake -> dir;
    SnakeSegment *curr_head = snake -> head;
    Point new_pos = curr_head -> pos;

    switch (curr_dir){
        //DIR_UP is a subtraction because for the origin (0,0), the row 0 is the top of the screen and the row number increases as you go down.
        case DIR_UP: new_pos.y -= 1; break; 
        case DIR_DOWN: new_pos.y += 1; break;
        case DIR_LEFT: new_pos.x -= 1; break;
        case DIR_RIGHT: new_pos.x += 1; break;
    }
    
    SnakeSegment *new_head = seg_alloc();
    if (new_head == NULL){
        //TODO
    }
    new_head -> pos = new_pos;

    snake -> head = new_head;
    new_head -> prev = NULL;
    new_head -> next = curr_head;
    curr_head -> prev = new_head;
    
    SnakeSegment *temp = snake -> tail -> prev;
    seg_free(snake -> tail);
    snake -> tail = temp;
    temp -> next = NULL;
}

void snake_grow(Snake *snake, Point new_head_pos) {
    //TODO: like snake_move but don't feee the tail - the snake gets one segment longer
}

bool snake_check_self_collision(const Snake *snake) {
    //TODO: walk the list, check if head position matches other segment's position
    //Check against terminal boundaries

    return false;
}

void snake_destroy(Snake *snake) {
    //TODO: walk the list and seg_free() every node - this matters for Phase 2
    //the allocator needs every alloc paired with a free to prove it is not leaking
}
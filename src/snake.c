#include "snake.h"
#include <stdlib.h>

SnakeSegment *seg_alloc(void){
    return malloc(sizeof(SnakeSegment));
}

void seg_free(SnakeSegment *seg) {
    free(seg);
}

/*
* Helper function to get the next position of the Snake before going into the snake_move function
* @param *snake the snake pointer of the Snake struct.
*/
Point snake_next_head_pos(const Snake *snake){
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

    return new_pos;
}

/*
* Helper function that checks if the Point struct are equal by comparing each field.
* @param a the first Point struct to be compared.
* @param b the second Point struct to be compared.
*/
bool points_equal(Point a, Point b){
    return a.x == b.x && a.y == b.y;
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

    SnakeSegment *curr_head = snake -> head;
    Point next_pos = snake_next_head_pos(snake);
    
    SnakeSegment *new_head = seg_alloc();
    if (new_head == NULL){
        //TODO
    }
    new_head -> pos = next_pos;

    snake -> head = new_head;
    new_head -> prev = NULL;
    new_head -> next = curr_head;
    curr_head -> prev = new_head;
    
    SnakeSegment *temp = snake -> tail -> prev;
    seg_free(snake -> tail);
    snake -> tail = temp;
    temp -> next = NULL;
}

/*
* Checks if the snake is out of bounds with the current terminal.
* @param pos the current position of the snake
* @param max_y the maximum y axis of the terminal boundary.
* @param max_x the maximum x axis of the terminal boundary.
*/
bool snake_out_of_bounds(Point pos, int max_y, int max_x){
    if (pos.y < 0 || pos.y > (max_y-1) || pos.x < 0 || pos.x > (max_x-1)){
        return true;
    }
    return false;  
}

void snake_grow(Snake *snake, Point new_head_pos) {
    //TODO: like snake_move but don't feee the tail - the snake gets one segment longer
}

/*
* Walk through the list and checks if the head position matches other segment's position.
* Then, checks this against terminal boundaries.
* If the snake will grow, iclude the tail in the check because the tail is not freed.
* If the snake is just moving and not growing, exclude the tail in the check because the tail will be freed.
* @param curr_pos the current position of the snake.
* @param *snake the snake pointer
*/
bool snake_check_self_collision(Point next_pos, const Snake *snake, bool will_grow) {
    SnakeSegment *curr_head  = snake -> head;

    if (will_grow){
        while (curr_head){
            if (points_equal(curr_head -> pos, next_pos)){
                return true;
            }
            curr_head = curr_head -> next;
        }
    }
    else{
        while (curr_head -> next){
            if (points_equal(curr_head -> pos, next_pos)){
                return true;
            }
            curr_head = curr_head -> next;
        } 
    }
    
    return false;
}

void snake_destroy(Snake *snake) {
    //TODO: walk the list and seg_free() every node - this matters for Phase 2
    //the allocator needs every alloc paired with a free to prove it is not leaking
}
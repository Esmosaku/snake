/*
* The free list mechanism tracks which slots in the array are sitting unused and available to be handed out next time seg_alloc() is called.
* For a slab allocator, the space inside the unused slot can be used to store a pointer to the next unused slot temporarily.
* 
*/
#include "snake.h"
#include "allocator.h"

#define POOL_CAPACITY 3000 //this is from max_x * max_y where max_x = 100 and max_y = 30

#ifdef USE_MALLOC

void allocator_init(void){
    /* nothing to do here since it needs no free list head*/
}

#else

static SnakeSegment pool[POOL_CAPACITY];
static SnakeSegment *free_list_head;

void allocator_init(void){
    for(int i = 0; i < POOL_CAPACITY-1; i++){
        pool[i].next = &(pool[i+1]);
    }
    pool[POOL_CAPACITY-1].next = NULL;

    free_list_head = &(pool[0]);
}

SnakeSegment *seg_alloc(void){
    SnakeSegment *hand_out; 

    if (free_list_head == NULL){
        return NULL;
    }
    else{
        hand_out = free_list_head;
        free_list_head = free_list_head -> next;
    }

    return hand_out;
}

void seg_free(SnakeSegment *seg){
    seg -> next = free_list_head;
    free_list_head = seg;
}

#endif

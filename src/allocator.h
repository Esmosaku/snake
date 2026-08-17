#ifndef ALLOCATOR_H
#define ALLOCATOR_H

void allocator_init(void);
SnakeSegment *seg_alloc(void);
void seg_free(SnakeSegment *seg);

#endif
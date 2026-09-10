#ifndef NET_H
#define NET_H

#include <stddef.h>
#include <stdint.h>
#include "snake.h"

#define WINNER_NONE 0
#define WINNER_P0 1
#define WINNER_P1 2
#define WINNER_DRAW 3

#define BOARD_WIDTH 100
#define BOARD_HEIGHT 30

#define PORT 3000

typedef struct{
    uint32_t length; //Want a length with explicit fixed width
    uint32_t score;
    Point segments[3000];
}SnakeWire;


typedef struct{
    SnakeWire players[2];
    Point food_pos;
    uint8_t game_over; //the smallest fixed-width integer type using just 1 byte, plenty of room for 0/1
    uint8_t winner;
} GameStateWire;

typedef struct{
    Direction client_to_server;
}InputWire;


#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "net.h"
#include "snake.h"
#include "leaderboard.h"
#include "allocator.h"

int main(void){
    int winner;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1){
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(socketfd, (struct sockaddr *)&addr, sizeof(addr));

    listen(socketfd, 2);

    int client1_fd = accept(socketfd, NULL, NULL);
    if (client1_fd == -1){
        return 0;
    }
    printf("Player 1 connected. Waiting for player 2...\n");

    int client2_fd = accept(socketfd, NULL, NULL);
    if(client2_fd == -1){
        return 0;
    }

    allocator_init();
    bool running = true;

    Point start_a = {.x = BOARD_WIDTH / 4, .y = BOARD_HEIGHT / 2};
    Point start_b = {.x = (BOARD_WIDTH *3) / 4, .y = BOARD_HEIGHT / 2};

    //Snake instances
    Snake snake_1;
    snake_init(&snake_1, start_a, DIR_RIGHT);

    Snake snake_2;
    snake_init(&snake_2, start_b, DIR_LEFT);

    Point food_pos = snake_spawn_food(&snake_1, &snake_2, BOARD_HEIGHT, BOARD_WIDTH);

    while(running){
        //printf("Player 1 score: %u\n", snake_1.score);
        //printf("Player 2 score: %u\n", snake_2.score);

        snake_1.dir = DIR_RIGHT;
        snake_2.dir = DIR_LEFT;

        Point next_pos_a = snake_next_head_pos(&snake_1);
        Point next_pos_b = snake_next_head_pos(&snake_2);

        bool will_grow_a = points_equal(next_pos_a, food_pos);
        bool will_grow_b = points_equal(next_pos_b, food_pos);

        bool a_dies = snake_out_of_bounds(next_pos_a, BOARD_HEIGHT, BOARD_WIDTH) || snake_check_self_collision(next_pos_a, &snake_1, will_grow_a) ||  snake_check_self_collision(next_pos_a, &snake_2, will_grow_b);
        bool b_dies = snake_out_of_bounds(next_pos_b, BOARD_HEIGHT, BOARD_WIDTH) || snake_check_self_collision(next_pos_b, &snake_2, will_grow_b) ||  snake_check_self_collision(next_pos_b, &snake_1, will_grow_a);
        

        if (a_dies || b_dies){
            running = false;
            if (snake_1.score > snake_2.score){
                winner = WINNER_P0;
            }
            else if (snake_2.score > snake_1.score){
                winner = WINNER_P1;
            }
            else{
                winner = WINNER_DRAW;
            }
        }
        else if (will_grow_a || will_grow_b){

            if (will_grow_a && !will_grow_b){
                snake_grow(&snake_1);
                food_pos = snake_spawn_food(&snake_1, NULL, BOARD_HEIGHT, BOARD_WIDTH);
            }
            else if (!will_grow_a && will_grow_b){
                snake_grow(&snake_2);
                food_pos = snake_spawn_food(NULL,&snake_2, BOARD_HEIGHT, BOARD_WIDTH);
            }
            else{
                snake_grow(&snake_1);
                snake_grow(&snake_2);
                food_pos = snake_spawn_food(&snake_1,&snake_2, BOARD_HEIGHT, BOARD_WIDTH);
            }
        }
        else{
            snake_move(&snake_1);
            snake_move(&snake_2);
        }

        usleep(150000); //sleeps ~150ms: 150,000 microseconds = 150ms
    }

    printf("Game Over! Winner: %d\n", winner);

    //providing raw send/recv actually works between them using simple messages before moving on to the real game data
    //char *msg = "Hello from server";
    //send(client1_fd, msg, strlen(msg), 0);

    return 0;
}

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
    GameStateWire state;
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

        state.game_over = 0;
        state.food_pos = food_pos;
        InputWire input;

        int bytes = recv(client1_fd, &input, sizeof(InputWire), MSG_DONTWAIT);
        if(bytes == (int)sizeof(InputWire)){
            snake_1.dir = input.client_to_server;
        }

        int bytes_2 = recv(client2_fd, &input, sizeof(InputWire), MSG_DONTWAIT);
        if(bytes_2 == (int)sizeof(InputWire)){
            snake_2.dir = input.client_to_server;
        }

        Point next_pos_a = snake_next_head_pos(&snake_1);
        Point next_pos_b = snake_next_head_pos(&snake_2);

        bool will_grow_a = points_equal(next_pos_a, food_pos);
        bool will_grow_b = points_equal(next_pos_b, food_pos);

        bool a_dies = snake_out_of_bounds(next_pos_a, BOARD_HEIGHT, BOARD_WIDTH) || snake_check_self_collision(next_pos_a, &snake_1, will_grow_a) ||  snake_check_self_collision(next_pos_a, &snake_2, will_grow_b);
        bool b_dies = snake_out_of_bounds(next_pos_b, BOARD_HEIGHT, BOARD_WIDTH) || snake_check_self_collision(next_pos_b, &snake_2, will_grow_b) ||  snake_check_self_collision(next_pos_b, &snake_1, will_grow_a);

        printf("A: (%d,%d) B:(%d,%d) a_dies=%d b_dies=%d\n", next_pos_a.x, next_pos_a.y, next_pos_b.x, next_pos_b.y, a_dies, b_dies);
        

        if (a_dies || b_dies){
            running = false;
            state.game_over = 1;
            if (snake_1.score > snake_2.score){
                winner = WINNER_P0;
            }
            else if (snake_2.score > snake_1.score){
                winner = WINNER_P1;
            }
            else{
                winner = WINNER_DRAW;
            }
            state.winner = winner;
        }
        else if (will_grow_a || will_grow_b){

            if (will_grow_a && !will_grow_b){
                snake_grow(&snake_1);
                food_pos = snake_spawn_food(&snake_1, &snake_2, BOARD_HEIGHT, BOARD_WIDTH);
            }
            else if (!will_grow_a && will_grow_b){
                snake_grow(&snake_2);
                food_pos = snake_spawn_food(&snake_1 ,&snake_2, BOARD_HEIGHT, BOARD_WIDTH);
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

        SnakeSegment *curr = snake_1.head;
        for(size_t i = 0; i < snake_1.length; i++){
            state.players[0].segments[i] = curr -> pos;
            curr = curr -> next;
        }
        state.players[0].length = snake_1.length;
        state.players[0].score = snake_1.score;

        SnakeSegment *curr2 = snake_2.head;
        for(size_t i = 0; i < snake_2.length; i++){
            state.players[1].segments[i] = curr2 -> pos;
            curr2 = curr2 -> next;
        }
        state.players[1].length = snake_2.length;
        state.players[1].score = snake_2.score;

        send(client1_fd, &state, sizeof(GameStateWire), 0);
        send(client2_fd, &state, sizeof(GameStateWire), 0);

        usleep(150000); //sleeps ~150ms: 150,000 microseconds = 150ms
    }

    if (state.winner == 1){
        printf("Game Over! Player 1 wins!");
    }
    else if(state.winner == 2){
        printf("Game Over! Player 2 wins!");
    }
    else if(state.winner == 3){
        printf("Game Over! The game is a draw!");
    }
    else{
        printf("Both players died at the same time");
    }

    //providing raw send/recv actually works between them using simple messages before moving on to the real game data
    //char *msg = "Hello from server";
    //send(client1_fd, msg, strlen(msg), 0);

    return 0;
}

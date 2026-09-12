#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include "net.h"
#include "leaderboard.h"
#include "snake.h"

Direction direction_from_key(int ch, Direction current) {
    switch (ch) {
        case KEY_UP:    return DIR_UP;
        case KEY_DOWN:  return DIR_DOWN;
        case KEY_LEFT:  return DIR_LEFT;
        case KEY_RIGHT: return DIR_RIGHT;
        default:        return current; //this means that if there is no key pressed, it would remain in the same direction
    }
}

int main(void){
    initscr();
    cbreak(); //disables line buffering so that getch() sees keys immediately instead of waiting for Enter
    noecho(); //stop typed keys from printing onto the screen immediately
    curs_set(0);
    srand(time(NULL));
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    if (BOARD_WIDTH < getmaxx(stdscr) && BOARD_HEIGHT < getmaxy(stdscr)){
        printf("Window is too small");
    }
    
    if (BOARD_WIDTH > getmaxx(stdscr) && BOARD_HEIGHT > getmaxy(stdscr)){
        printf("Window is too large");
    }

    int socketfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd1 == -1){
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(PORT);

    int result = connect(socketfd1, (struct sockaddr *)&addr, sizeof(addr));
    if (result == -1){
        return 0;
    }

    Direction pending_dir = DIR_RIGHT;

    GameStateWire state;
    do{
        clear();

        int bytes = recv(socketfd1, &state, sizeof(GameStateWire), 0);
        if (bytes <= 0){
            endwin();
            return 0; 
        }
        for(uint32_t i = 0; i < state.players[0].length; i++){
            mvaddch(state.players[0].segments[i].y, state.players[0].segments[i].x, '@');
        }
        for(u_int32_t i = 0; i < state.players[1].length; i++){
            mvaddch(state.players[1].segments[i].y, state.players[1].segments[i].x, '#');
        }

        mvaddch(state.food_pos.y, state.food_pos.x, '*');

        refresh();

        int ch = getch();
        if(ch != ERR){
            pending_dir = direction_from_key(ch, pending_dir);
            InputWire input;
            input.client_to_server = pending_dir;
            send(socketfd1, &input, sizeof(InputWire), 0);
        }
    }while (!state.game_over);

    endwin();
    printf("The winner is:%d\n", state.winner);
    close(socketfd1);
    return 0;

}

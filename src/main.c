#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "snake.h"
#include "allocator.h"
#include "leaderboard.h"


Direction direction_from_key(int ch, Direction current) {
    switch (ch) {
        case KEY_UP:    return DIR_UP;
        case KEY_DOWN:  return DIR_DOWN;
        case KEY_LEFT:  return DIR_LEFT;
        case KEY_RIGHT: return DIR_RIGHT;
        default:        return current; //this means that if there is no key pressed, it would remain in the same direction
    }
}

ScoreEntry board[LEADERBOARD_MAX];
int board_count = 0;

int main (void){
    initscr();
    cbreak(); //disables line buffering so that getch() sees keys immediately instead of waiting for Enter
    noecho(); //stop typed keys from printing onto the screen immediately
    curs_set(0);
    srand(time(NULL));
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);

    Snake snake;
    Point test_point = {.x = 10, .y = 10};

    Direction pending_dir = DIR_RIGHT;
    
    allocator_init();
    board_count = leaderboard_load(board, LEADERBOARD_MAX);

    snake_init(&snake, test_point, pending_dir);
    Point food_pos = snake_spawn_food(&snake, NULL, getmaxy(stdscr), getmaxx(stdscr));

    bool running = true;
    mvprintw(1, 0, "Score so far");
    for(int i = 0; i < board_count; i++){
        mvprintw(2+i, 0, "%i. %u", (i+1), board[i].score);
        refresh();
    }

    getch();
    nodelay(stdscr, TRUE);

    while (running){
        clear();
        mvprintw(0,0, "Score: %u", snake.score);
        SnakeSegment *seg = snake.head;
        while (seg != NULL){
            mvaddch(seg-> pos.y, seg -> pos.x, '@');
            seg = seg -> next;
        }

        mvaddch(food_pos.y, food_pos.x, '#');

        //mvprintw(0, 0, "dir: %d", pending_dir);

        snake.dir = pending_dir;

        Point next_pos = snake_next_head_pos(&snake);

        bool will_grow = points_equal(next_pos, food_pos);

        

        if (snake_out_of_bounds(next_pos, getmaxy(stdscr), getmaxx(stdscr)) || snake_check_self_collision(next_pos, &snake, will_grow)){
            running = false;
        }
        else if (will_grow){
            //mvprintw(1, 0, "next: (%d,%d) food: (%d,%d) grow: %d", next_pos.x, next_pos.y, food_pos.x, food_pos.y, will_grow);
            snake_grow(&snake);
            food_pos = snake_spawn_food(&snake, NULL, getmaxy(stdscr), getmaxx(stdscr));
        }
        else{
            snake_move(&snake);
        }
        refresh();

        int ch = getch();
        if (ch == 'q') {
            running = false;
        } else {
            pending_dir = direction_from_key(ch, pending_dir);
        }

        napms(150); //sleeps ~100ms
    }

    snake_destroy(&snake);
    ScoreEntry new_entry;
    strcpy(new_entry.name, "Player");
    new_entry.score = snake.score;
    new_entry.timestamp = time(NULL);
    insert_score(board, new_entry, &board_count);
    leaderboard_save(board, board_count);

    endwin();
    return 0;
}
#include <ncurses.h>
#include <stdbool.h>
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

int main (void){
    initscr();
    cbreak(); //disables line buffering so that getch() sees keys immediately instead of waiting for Enter
    noecho(); //stop typed keys from printing onto the screen immediately
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Snake snake;
    Point test_point = {.x = 10, .y = 10};

    Direction pending_dir = DIR_RIGHT;
    snake_init(&snake, test_point, pending_dir);

    bool running = true;

    while (running){
        clear();
        mvaddch(snake.head -> pos.y, snake.head -> pos.x, '@');
        mvprintw(0, 0, "dir: %d", pending_dir);
        snake.dir = pending_dir;
        snake_move(&snake);
        refresh();

        int ch = getch();
        if (ch == 'q') {
            running = false;
        } else {
            pending_dir = direction_from_key(ch, pending_dir);
        }

        napms(100); //sleeps ~100ms
    }

    endwin();
    return 0;
}
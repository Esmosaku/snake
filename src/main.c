#include <ncurses.h>
#include <stdbool.h>
#include "snake.h"


int main (void){
    initscr();
    cbreak(); //disables line buffering so that getch() sees keys immediately instead of waiting for Enter
    noecho(); //stop typed keys from printing onto the screen immediately
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    Point test_point = {.x = 10, .y = 10};

    bool running = true;
    while (running){
        clear();
        mvaddch(test_point.y, test_point.x, '@');
        refresh();

        int ch = getch();
        if (ch == 'q') running = false;

        napms(100); //sleeps ~100ms
    }

    endwin();
    return 0;
}
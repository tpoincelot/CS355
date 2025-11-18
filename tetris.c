#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

/* Thomas Poincelot
 * Initialize ncurses and configure the terminal for the game. */
void init_game(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
}

int main(void) {
    init_game();
    endwin();
    return 0;
}
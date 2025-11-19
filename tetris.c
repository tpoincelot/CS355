#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// TP Dimensions of the playing field (10 columns × 25 rows rendering grid).
#define WIDTH 10
#define HEIGHT 25

// TP Field grid storing occupied cells (0 = empty, 1 = filled).
static int field[HEIGHT][WIDTH];

// TP Reset every cell in the playfield to empty.
void init_field(void) {
    memset(field, 0, sizeof(field));
}

// TP Render the current playfield grid onto the terminal screen as a clean board.
void draw_field(void) {
    clear();
    for (int y = 0; y < HEIGHT; y++) {
        move(y, 0);
        for (int x = 0; x < WIDTH; x++) {
            addch('.');
        }
        clrtoeol();
    }
}

// TP Initialize ncurses and configure the terminal for the game.
void init_game(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    init_field();
}

int main(void) {
    init_game();

    // TP Display the empty board, use Ctrl+C to exit(Just a test).
    draw_field();
    refresh();
    while (1) {
        usleep(100000);
    }
    endwin();
    return 0;
}
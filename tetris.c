#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// TP Dimensions of the playing field (10 columns × 25 rows rendering grid).
#define WIDTH 10
#define HEIGHT 25

#define NUM_COLORS 7

// TP Field grid storing occupied cells (0 = empty, 1 = filled).
static int field[HEIGHT][WIDTH];

static const int piece[][4][4][4] = {
    { // Cube
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }
    },
    { // Line
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        },
        {
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        },
        {
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { // L shape
        {
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
    { // Z shape
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}
        }
    }
};

// TP Sample 2×2 piece definition.
// static const int cube[] = {
//     1, 1,
//     1, 1
// };

// TP Function that adds shape into the top of the field. Adjusted to skip empty rows at the top.
//JT - add random color to each new piece
void add_piece(int origin_x, const int shape[4][4], int shape_width, int shape_height) {
    int color_id = (rand() % NUM_COLORS) + 1; //JT - pick random color

    int start_row = 0;
    for (int r = 0; r < shape_height; r++) {
        int is_empty = 1;
        for (int c = 0; c < shape_width; c++) {
            if (shape[r][c]) {
                is_empty = 0;
                break;
            }
        }
        if (!is_empty) {
            start_row = r;
            break;
        }
    }

    // Place the piece on the field
    for (int r = start_row; r < shape_height; r++) {
        for (int c = 0; c < shape_width; c++) {
            if (!shape[r][c]) {
                continue;
            }
            int fx = origin_x + c;
            int fy = r - start_row;
            if (fx >= 0 && fx < WIDTH && fy >= 0 && fy < HEIGHT) {
                field[fy][fx] = color_id; //JT - store color id
            }
        }
    }
}

// TP Reset every cell in the playfield to empty.
void init_field(void) {
    memset(field, 0, sizeof(field));
}

// TP Render the current playfield grid onto the terminal screen.
void draw_field(void) {
    clear();
    for (int y = 0; y < HEIGHT; y++) {
        move(y, 0);
        for (int x = 0; x < WIDTH; x++) {
            if (field[y][x]){
                attron(COLOR_PAIR(field[y][x]));
                addch('#');
                attroff(COLOR_PAIR(field[y][x]));
            } else {
                addch('.');
            }
        }
    }
}

// TP Initialize ncurses and configure the terminal for the game.
//JT - add colors
void init_game(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_field();
}

int main(void) {
    srand((unsigned)time(NULL)); // If you don't use this the piece will be added in the same spot every run
    init_game();

    // TP Place the cube near the top center so the board can show a piece.
    int piece_width = 4;
    int piece_height = 4;

    while(1){
        //JT - pick random piece type and rotation
        int piece_type = rand() % 4;
        int rotation = rand() % 4;
        int origin_x = rand() % (WIDTH - piece_width + 1); // Ensure the piece fits horizontally

        add_piece(origin_x, piece[piece_type][rotation], piece_width, piece_height);
    
        draw_field();
        refresh();
        usleep(500000);
    }

    endwin();
    return 0;
}
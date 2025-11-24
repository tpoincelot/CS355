#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// TP Dimensions of the playing field (10 columns × 25 rows rendering grid).
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 25

#define NUM_COLORS 7

static int g_score = 0;

// TP Field grid storing occupied cells (0 = empty, 1 = filled).
static int g_field[FIELD_HEIGHT][FIELD_WIDTH];

static const int g_pieces[][4][4][4] = {
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

//JT - track an active piece
typedef struct {
    int type;
    int rotation;
    int x;
    int y;
    int color;
} active_piece;

static active_piece g_current_piece;

// TP Reset every cell in the playfield to empty.
void initialize_field(void) {
    memset(g_field, 0, sizeof(g_field));
}

// JT Clear full rows and update the score
void clear_completed_rows(void) {
    int rows_cleared = 0;

    for (int y = 0; y < FIELD_HEIGHT; y++) {
        int is_full = 1;
        for (int x = 0; x < FIELD_WIDTH; x++) {
            if (!g_field[y][x]) {
                is_full = 0;
                break;
            }
        }

        if (is_full) {
            rows_cleared++;
            for (int r = y; r > 0; r--) {
                for (int c = 0; c < FIELD_WIDTH; c++) {
                    g_field[r][c] = g_field[r - 1][c];
                }
            }

            for (int c = 0; c < FIELD_WIDTH; c++) {
                g_field[0][c] = 0;
            }
        }
    }

    if (rows_cleared > 0) {
        g_score += rows_cleared * 100; // 100 points per row
    }
}

// TP Render the current playfield grid onto the terminal screen.
// JT also draw the active falling piece
void render_field(void) {
    clear();

    //center game
    int cell_width = 2;
    int board_width = FIELD_WIDTH * cell_width;
    int board_height = FIELD_HEIGHT;
    int x_offset = (COLS - board_width) / 2;
    int y_offset = (LINES - board_height) / 2;

    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            move(y_offset + y, x_offset + x * cell_width);
            if (g_field[y][x]){
                attron(COLOR_PAIR(g_field[y][x]));
                addch('#');
                addch(' ');
                attroff(COLOR_PAIR(g_field[y][x]));
            } else {
                addch('.');
                addch(' ');
            }
        }
    }

    const int (*shape)[4] = g_pieces[g_current_piece.type][g_current_piece.rotation];
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (shape[r][c]) {
                int fx = g_current_piece.x + c;
                int fy = g_current_piece.y + r;
                if (fx >= 0 && fx < FIELD_WIDTH && fy >= 0 && fy < FIELD_HEIGHT) {
                    move(y_offset + fy, x_offset + fx * cell_width);
                    attron(COLOR_PAIR(g_current_piece.color));
                    addch('#');
                    addch(' ');
                    attroff(COLOR_PAIR(g_current_piece.color));
                }
            }
        }
    }

    move(FIELD_HEIGHT, 0);
    printw("Score: %d", g_score);
}

// JT check if a piece can be placed at a given position

int can_place(int x, int y, const int shape [4][4], int shape_width, int shape_height){
    for (int r = 0; r < shape_height; r++){
        for (int c = 0; c < shape_width; c++){
            if (shape[r][c]){
                int fx = x + c;
                int fy = y + r;

                if (fx < 0 || fx >= FIELD_WIDTH || fy < 0 || fy >= FIELD_HEIGHT || g_field[fy][fx]){
                    return 0;
                }
            }
        }
    }
    return 1;
}

// TP Initialize ncurses and configure the terminal for the game.
// JT add colors
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
    initialize_field();
}

void start_screen(void) {
    clear();
    char msg1[] = "Welcome to TP and JT Tetris Game";
    char msg2[] = "Press ENTER to Start Game";
    mvprintw(LINES / 2 - 1, (COLS - strlen(msg1)) / 2, "%s", msg1);
    mvprintw(LINES / 2, (COLS - strlen(msg2)) / 2, "%s", msg2);
    refresh();
    nodelay(stdscr, FALSE);
    while (1) {
        int ch = getch();
        if (ch == '\n' || ch == KEY_ENTER) break;
    }
    nodelay(stdscr, TRUE);
}

int game_overdose(void) {
    clear();
    char msg1[] = "GAME OVER!!";
    char msg2[] = "Play again? (y/n)";
    mvprintw(LINES / 2 - 1, (COLS - strlen(msg1)) / 2, "%s", msg1);
    mvprintw(LINES / 2, (COLS - strlen(msg2)) / 2, "%s", msg2);
    refresh();

    nodelay(stdscr, FALSE);
    while (1) {
        int ch = getch();
        if (ch == 'y' || ch == 'Y') {
            nodelay(stdscr, TRUE);
            return 1; // restart
        } else if (ch == 'n' || ch == 'N') {
            return 0; // quit
        }
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    init_game();
    start_screen();

    int piece_width = 4;
    int piece_height = 4;
    const int refresh_rate = 50000; // 50ms
    const int fall_frames = 10;     // piece falls every 10 frames
    int frame_counter = 0;

    while (1) {
        int has_piece = 0;
        initialize_field();

        while (1) {
            if (!has_piece) {
                g_current_piece.type = rand() % 4;
                g_current_piece.rotation = rand() % 4;
                g_current_piece.x = FIELD_WIDTH / 2 - piece_width / 2;
                g_current_piece.y = 0;
                g_current_piece.color = (rand() % NUM_COLORS) + 1;
                has_piece = 1;
            }

            int ch = getch();
            if (ch != ERR) {
                const int (*shape)[4] = g_pieces[g_current_piece.type][g_current_piece.rotation];
                switch (ch) {
                    case KEY_LEFT:
                        if (can_place(g_current_piece.x - 1, g_current_piece.y, shape, piece_width, piece_height)) {
                            g_current_piece.x--;
                        }
                        break;
                    case KEY_RIGHT:
                        if (can_place(g_current_piece.x + 1, g_current_piece.y, shape, piece_width, piece_height)) {
                            g_current_piece.x++;
                        }
                        break;
                    case KEY_DOWN:
                        if (can_place(g_current_piece.x, g_current_piece.y + 1, shape, piece_width, piece_height)) {
                            g_current_piece.y++;
                        }
                        break;
                    case ' ':
                        {
                            int new_rotation = (g_current_piece.rotation + 1) % 4;
                            const int (*new_shape)[4] = g_pieces[g_current_piece.type][new_rotation];
                            if (can_place(g_current_piece.x, g_current_piece.y, new_shape, piece_width, piece_height)) {
                                g_current_piece.rotation = new_rotation;
                            }
                        }
                        break;
                }
            }

            if (frame_counter >= fall_frames) {
                int can_move_down = 1;
                const int (*shape)[4] = g_pieces[g_current_piece.type][g_current_piece.rotation];
                for (int r = 0; r < piece_height; r++) {
                    for (int c = 0; c < piece_width; c++) {
                        if (shape[r][c]) {
                            int fy = g_current_piece.y + r + 1;
                            if (fy >= FIELD_HEIGHT || (g_field[fy][g_current_piece.x + c])) {
                                can_move_down = 0;
                            }
                        }
                    }
                }

                if (can_move_down) {
                    g_current_piece.y++;
                } else {
                    // lock piece into field
                    for (int r = 0; r < piece_height; r++) {
                        for (int c = 0; c < piece_width; c++) {
                            if (shape[r][c]) {
                                int fx = g_current_piece.x + c;
                                int fy = g_current_piece.y + r;
                                if (fx >= 0 && fx < FIELD_WIDTH && fy >= 0 && fy < FIELD_HEIGHT) {
                                    g_field[fy][fx] = g_current_piece.color;
                                }
                            }
                        }
                    }

                    // check for game over (piece at line 0)
                    int game_over_flag = 0;
                    for (int r = 0; r < piece_height; r++) {
                        for (int c = 0; c < piece_width; c++) {
                            if (shape[r][c]) {
                                int fy = g_current_piece.y + r;
                                if (fy == 0) {
                                    game_over_flag = 1;
                                }
                            }
                        }
                    }

                    if (game_over_flag) {
                        if (game_overdose()) {
                            initialize_field();
                            has_piece = 0;
                            continue;   // restart loop
                        } else {
                            endwin();
                            return 0;   // exit program
                        }
                    }
                    clear_completed_rows(); //JT - clear rows after locking the piece
                    has_piece = 0; // spawn next piece
                }

                frame_counter = 0; // reset frame counter
            }

            render_field();
            refresh();
            usleep(refresh_rate);
            frame_counter++;
        }
    }

    endwin();
    return 0;
}
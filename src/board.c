#include "../include/board.h"

#include <stdio.h>
#include <stdlib.h>

char board[BOARD_ROWS * BOARD_COLS];

void set_board(int x, int y, char c) {
    if (x < BOARD_ROWS && y < BOARD_COLS)
        board[y * BOARD_ROWS + x] = c;
}

void fill_board() {
    for (int x = 0; x < BOARD_ROWS; x++) {
        for (int y = 0; y < BOARD_COLS;
             y++) {
            if (x == 0 || x == BOARD_ROWS - 1 || y == 0 || y == BOARD_COLS - 1) {
                set_board(x, y, '#');
            } else {
                set_board(x, y, ' ');
            }
        }
    }
}

void draw_board() {
    system("clear");
    for (int y = 0; y < BOARD_COLS; y++) {
        for (int x = 0; x < BOARD_ROWS; x++) {
            printf("%c", board[y * BOARD_ROWS + x]);
        }
        printf("\n");
    }
}
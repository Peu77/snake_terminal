#include "../include/snake.h"
#include "../include/board.h"
#include "../include/game.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

Snake snake;

char getInput() {
    char buf = 0;
    struct termios old = {0}, new;
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    new = old;
    new.c_lflag &= ~ICANON;
    new.c_lflag &= ~ECHO;
    new.c_cc[VMIN] = 0;
    new.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &new) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    if (tcsetattr(0, TCSANOW, &old) < 0)  // reset terminal settings after read
        perror("tcsetattr ~ICANON");
    return buf;
}

void player_input() {
    char input = getInput();

    switch (input) {
        case 'w':
            if (snake.direction != DOWN) {
                snake.direction = UP;
            }
            break;
        case 's':
            if (snake.direction != UP) {
                snake.direction = DOWN;
            }
            break;
        case 'a':
            if (snake.direction != RIGHT) {
                snake.direction = LEFT;
            }
            break;
        case 'd':
            if (snake.direction != LEFT) {
                snake.direction = RIGHT;
            }
            break;
        case 'q':
            game_over = true;
    }
}

void init_snake() {
    SnakeSegment head = {BOARD_ROWS / 2, BOARD_COLS / 2};
    snake.segments[0] = head;
    snake.length = 50;
    snake.delta_x = 0;
    snake.delta_y = 0;
}

void update_snake() {
    switch (snake.direction) {
        case UP:
            snake.delta_x = 0;
            snake.delta_y = -1;
            break;
        case DOWN:
            snake.delta_x = 0;
            snake.delta_y = 1;
            break;
        case LEFT:
            snake.delta_x = -1;
            snake.delta_y = 0;
            break;
        case RIGHT:
            snake.delta_x = 1;
            snake.delta_y = 0;
            break;
    }

    for (int i = snake.length; i > 0; i--) {
        snake.segments[i] = snake.segments[i - 1];
    }

    SnakeSegment *head = &snake.segments[0];

    head->x += snake.delta_x;
    head->y += snake.delta_y / 2;

    // check if the snake moved out of the board
    if (head->x < 0 || head->x >= BOARD_ROWS || head->y < 0 || head->y >= BOARD_COLS) {
        game_over = true;
    }

    // check if the snake hit itself
    for (int i = 1; i < snake.length; i++) {
        if (head->x == snake.segments[i].x && head->y == snake.segments[i].y) {
            game_over = true;
        }
    }

    if (game_over)
        draw_board();
}

void draw_snake() {
    for (int i = 0; i < snake.length; i++) {
        int *x = &snake.segments[i].x;
        double *y = &snake.segments[i].y;

        // uninitialized segments are at 0,0 so we skip them
        if (*x == 0 && *y == 0)
            continue;

        set_board(*x, *y, i == 0 ? '@' : 'o');
    }
}
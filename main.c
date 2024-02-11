#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

char getch() {
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

#define BORD_SIZE 100
#define BOARD_ROWS BORD_SIZE
// cols have to be half of the rows to make the board look like a square, because the terminal add space between the lines
// This is also the reason why, we divide the delta-y by 2 so the snake moves in the same speed in both directions
#define BOARD_COLS (BORD_SIZE / 2)

#define FPS 60
#define FRAME_TARGET_TIME (1000000 / FPS)

bool game_over = false;
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
    for (int y = 0; y < BOARD_COLS;
         y++) {
        for (int x = 0; x < BOARD_ROWS; x++) {
            printf("%c", board[y * BOARD_ROWS + x]);
        }
        printf("\n");
    }
}


typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    int x;
    double y;
} SnakeSegment;

#define SNAKE_MAX_LENGTH 100
struct {
    SnakeSegment segments[SNAKE_MAX_LENGTH];
    int length;
    int delta_x;
    double delta_y;

    Direction direction;
} snake;


void player_input() {
    char input = getch();

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

int main() {
    init_snake();

    int tick = 0;
    while (!game_over) {
        clock_t startTime = clock();
        tick++;
        fill_board();

        if (tick == 2) {
            player_input();
            update_snake();
            tick = 0;
        }
        draw_snake();
        draw_board();

        clock_t endTime = clock();
        int frameTime =
                (endTime - startTime) * 1000000 / CLOCKS_PER_SEC;  // Time taken for current frame in microseconds

        if (frameTime < FRAME_TARGET_TIME) {
            usleep(FRAME_TARGET_TIME - frameTime);  // Pause for the remaining time to achieve the desired frame rate
        }
    }
    return 0;
}

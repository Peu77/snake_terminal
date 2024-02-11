#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../include/game.h"
#include "../include/board.h"
#include "../include/snake.h"

bool game_over = false;

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

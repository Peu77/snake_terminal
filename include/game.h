#ifndef game_h
#define game_h

#define BORD_SIZE 100
#define BOARD_ROWS BORD_SIZE
// cols have to be half of the rows to make the board look like a square, because the terminal add space between the lines
// This is also the reason why, we divide the delta-y by 2 so the snake moves in the same speed in both directions
#define BOARD_COLS (BORD_SIZE / 2)

#define FPS 60
#define FRAME_TARGET_TIME (1000000 / FPS)

extern bool game_over;

#endif
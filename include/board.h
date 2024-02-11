#ifndef board_h
#define board_h

#include "game.h"

extern char board[];

void set_board(int x, int y, char c);

void fill_board();

void draw_board();

#endif
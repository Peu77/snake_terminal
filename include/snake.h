#ifndef snake_h
#define snake_h

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
typedef struct {
    SnakeSegment segments[SNAKE_MAX_LENGTH];
    int length;
    int delta_x;
    double delta_y;

    Direction direction;
} Snake;

extern Snake snake;

void init_snake();
void player_input();
void update_snake();
void draw_snake();


#endif
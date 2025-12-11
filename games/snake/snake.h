#ifndef SNAKE_H
#define SNAKE_H

#include "../../include/keyboard.h"

#define INIT_LENGTH 3

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

typedef struct{
    int x, y;
} Node;

void init_snake();
void game_start();

#endif

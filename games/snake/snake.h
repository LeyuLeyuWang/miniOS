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

typedef struct {
    int x;
    int y;
} Node;

void init_snake(void);
void game_start(void);

// keyboard constant
// 如果 keyboard.h 里已经有 UP/DOWN/LEFT/RIGHT，可以把下面这段删掉；
// 如果没有，就保留它们。
#ifndef UP
#define UP          0x25   /* Up       */
#define DOWN        0x26   /* Down     */
#define LEFT        0x27   /* Left     */
#define RIGHT       0x28   /* Right    */
#endif

#endif

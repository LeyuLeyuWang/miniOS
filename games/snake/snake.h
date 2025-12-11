#ifndef SNAKE_H
#define SNAKE_H

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

// keyboard constant
#define UP          0x25   /* Up       */
#define DOWN        0x26   /* Down     */
#define LEFT        0x27   /* Left     */
#define RIGHT       0x28   /* Right    */

#endif

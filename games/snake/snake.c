#include "snake.h"
#include "../../lib/lib.h"

#define BOARD_WIDTH  30
#define BOARD_HEIGHT 15
#define MAX_SNAKE_LENGTH (BOARD_WIDTH * BOARD_HEIGHT)

static Node snake_body[MAX_SNAKE_LENGTH];
static int  snake_length = 0;
static Node food = {-1, -1};
static int  direction = Right;
static int  game_over = 0;

static void wait_for_key_press()
{
    while (keyboard_input() == 0) {}
}

static int is_snake_cell(int x, int y)
{
    for (int i = 0; i < snake_length; i++) {
        if (snake_body[i].x == x && snake_body[i].y == y) {
            return 1;
        }
    }
    return 0;
}

void init_snake()
{
    direction = Right;
    snake_length = INIT_LENGTH;
    int start_x = BOARD_WIDTH / 2 - 1;
    int start_y = BOARD_HEIGHT / 2;
    for (int i = 0; i < snake_length; i++) {
        snake_body[i].x = start_x - (snake_length - 1 - i);
        snake_body[i].y = start_y;
    }
}

static void place_food()
{
    do {
        food.x = random(BOARD_WIDTH);
        food.y = random(BOARD_HEIGHT);
    } while (is_snake_cell(food.x, food.y));
}

static void draw_board()
{
    char border[BOARD_WIDTH + 3];
    for (int i = 0; i < BOARD_WIDTH + 2; i++) {
        border[i] = '#';
    }
    border[BOARD_WIDTH + 2] = '\0';

    printf("============== Snake ==============\n");
    printf("Controls: Arrow Keys or WASD\n");
    printf("Food: @  Snake: O\n\n");

    printf("%s\n", border);
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        char line[BOARD_WIDTH + 3];
        line[0] = '#';
        for (int x = 0; x < BOARD_WIDTH; x++) {
            line[x + 1] = ' ';
        }
        line[BOARD_WIDTH + 1] = '#';
        line[BOARD_WIDTH + 2] = '\0';

        for (int i = 0; i < snake_length; i++) {
            if (snake_body[i].y == y) {
                int pos = snake_body[i].x + 1;
                if (pos >= 1 && pos <= BOARD_WIDTH) {
                    line[pos] = (i == snake_length - 1) ? 'O' : 'o';
                }
            }
        }

        if (food.y == y && food.x >= 0 && food.x < BOARD_WIDTH) {
            line[food.x + 1] = '@';
        }

        printf("%s\n", line);
    }
    printf("%s\n", border);
}

static int move_snake()
{
    Node head = snake_body[snake_length - 1];
    Node next = head;

    if (direction == Left) {
        next.x -= 1;
    } else if (direction == Right) {
        next.x += 1;
    } else if (direction == Up) {
        next.y -= 1;
    } else {
        next.y += 1;
    }

    if (next.x < 0 || next.x >= BOARD_WIDTH || next.y < 0 || next.y >= BOARD_HEIGHT) {
        return 0;
    }
    if (is_snake_cell(next.x, next.y)) {
        return 0;
    }

    if (next.x == food.x && next.y == food.y) {
        if (snake_length < MAX_SNAKE_LENGTH) {
            snake_body[snake_length] = next;
            snake_length++;
            place_food();
        }
    } else {
        for (int i = 0; i < snake_length - 1; i++) {
            snake_body[i] = snake_body[i + 1];
        }
        snake_body[snake_length - 1] = next;
    }

    return 1;
}

static void handle_input()
{
    char key = keyboard_input();
    if (key == 0) {
        return;
    }

    if ((key == 'w' || key == 'W' || key == UP) && direction != Down) {
        direction = Up;
    } else if ((key == 's' || key == 'S' || key == DOWN) && direction != Up) {
        direction = Down;
    } else if ((key == 'a' || key == 'A' || key == LEFT) && direction != Right) {
        direction = Left;
    } else if ((key == 'd' || key == 'D' || key == RIGHT) && direction != Left) {
        direction = Right;
    }
}

void game_start()
{
    game_over = 0;
    init_snake();
    place_food();

    while (!game_over) {
        handle_input();
        if (!move_snake()) {
            game_over = 1;
            break;
        }

        clear();
        draw_board();
        sleep(150);
    }

    printf("\nGame Over! Press any key to return to the menu.\n");
    wait_for_key_press();
}

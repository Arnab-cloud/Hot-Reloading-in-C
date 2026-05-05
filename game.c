#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

#define WIDTH 800.0
#define HEIGHT 600.0

#define RADIUS 20
#define DIST 150

#define SPEED 5

typedef struct state {
    float centerX;
    float centerY;
    float distance;
    float rad;
    float angle;
} State;

// State s = {0};

void *init_game() {
    State *s = (State *)malloc(sizeof(State));
    *s = (State){WIDTH / 2, HEIGHT / 2, DIST, RADIUS, 0};
    printf("INFO: GAME: init called\n");
    return (void *)s;
}

void update_game(void *state) {
    State *cur_state = (State *)state;
    int x = cur_state->distance * cosf(cur_state->angle);
    int y = cur_state->distance * sinf(cur_state->angle);
    cur_state->angle += SPEED * GetFrameTime();
    ClearBackground(BLACK);
    DrawCircle(WIDTH / 2 + x, HEIGHT / 2 - y, cur_state->rad, RED);
}

void clear_game(void *state) { printf("INFO: GAME: clear called\n"); }

int add_game(int a, int b) { return a * b; }

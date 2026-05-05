#include "raylib.h"
#include <math.h>
#include <stdio.h>

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

State s = {0};

void init_game() {
    s = (State){WIDTH / 2, HEIGHT / 2, DIST, RADIUS, 0};
    // InitWindow(WIDTH, HEIGHT, "Window");
    printf("INFO: GAME: init called\n");
}

void update_game() {
    int x = s.distance * cosf(s.angle);
    int y = s.distance * sinf(s.angle);
    s.angle += SPEED * GetFrameTime();
    ClearBackground(BLACK);
    DrawCircle(WIDTH / 2 + x, HEIGHT / 2 - y, s.rad, BLUE);
}

int add_game(int a, int b) { return a * b; }

void clear_game() {
    // CloseWindow();
    printf("INFO: GAME: clear called\n");
}

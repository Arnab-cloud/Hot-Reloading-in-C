#ifndef GAME_H_
#define GAME_H_

void init_game();

void update_game();

void clear_game();

int add_game(int a, int b);

typedef typeof(init_game) *init_game_t;
typedef typeof(update_game) *update_game_t;
typedef typeof(clear_game) *clear_game_t;
typedef typeof(add_game) *add_game_t;

#endif

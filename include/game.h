#ifndef GAME_H
#define GAME_H

#include "types.h"
#include "config.h"

typedef struct {
    Point player_pos;
    Size  player_size;

    Point falling_objects[MAX_FALLING_OBJECTS];
    Size  falling_object_size;
    float bonus_speed[MAX_FALLING_OBJECTS];
    int   falling_count;
    float fall_speed;

    int   lives;
    int   tick;
    int   seconds;
    float progression;

    Point powerups[MAX_POWERUPS];
    int   powerup_type[MAX_POWERUPS];
    int   powerup_active[MAX_POWERUPS];
    int   powerup_count;
    int   speed_boost_timer;
    int   invuln_timer;

    int   is_left_pressed;
    int   is_right_pressed;
    int   r_press_count;
    int   is_paused;
    int   hit_flash_timer;
} GameState;

void game_init(GameState *state);
void game_reset(GameState *state);
void game_soft_reset(GameState *state);
void game_update(GameState *state);
int  game_is_over(const GameState *state);

#endif

#include "game.h"
#include "physics.h"
#include <stdlib.h>
#include <time.h>

static int rdm_range(int min, int range) {
    return min + rand() % range;
}

static void randomize_falling_objects(GameState *state) {
    for (int i = 0; i < MAX_FALLING_OBJECTS; i++) {
        state->falling_objects[i].x = (float)rdm_range(SPAWN_X_MIN, SPAWN_X_RANGE);
        state->falling_objects[i].y = (float)rdm_range(SPAWN_Y_MIN, SPAWN_Y_RANGE);
        state->bonus_speed[i] = ((float)rdm_range(0, BONUS_RANGE)) / 100.0f;
    }
}

void game_init(GameState *state) {
    srand((unsigned)time(NULL));
    game_reset(state);
}

void game_reset(GameState *state) {
    state->player_pos.x = PLAYER_START_X;
    state->player_pos.y = PLAYER_START_Y;
    state->player_size.width  = PLAYER_WIDTH;
    state->player_size.height = PLAYER_HEIGHT;

    state->falling_object_size.width  = FALLING_OBJ_WIDTH;
    state->falling_object_size.height = FALLING_OBJ_HEIGHT;
    state->fall_speed = INITIAL_FALL_SPEED;
    state->falling_count = 1;

    state->is_left_pressed  = 0;
    state->is_right_pressed = 0;
    state->r_press_count    = 0;
    state->is_paused        = 0;
    state->hit_flash_timer  = 0;
    state->speed_boost_timer = 0;
    state->invuln_timer      = 0;
    state->powerup_count     = 0;
    for (int i = 0; i < MAX_POWERUPS; i++) {
        state->powerup_active[i] = 0;
        state->powerup_type[i]   = POWERUP_NONE;
    }

    state->lives       = INITIAL_LIVES;
    state->tick         = 0;
    state->seconds      = 0;
    state->progression  = PROGRESS_START;

    randomize_falling_objects(state);
}

void game_soft_reset(GameState *state) {
    state->player_pos.x = PLAYER_START_X;
    state->player_pos.y = PLAYER_START_Y;
    state->is_left_pressed  = 0;
    state->is_right_pressed = 0;
    state->fall_speed = INITIAL_FALL_SPEED;
    state->seconds = 0;

    randomize_falling_objects(state);
}

void game_update(GameState *state) {
    if (state->is_paused) return;

    state->tick++;
    if (state->tick % TICKS_PER_SECOND == 0) {
        state->seconds++;
    }
    if (state->hit_flash_timer > 0) {
        state->hit_flash_timer--;
    }
    if (state->speed_boost_timer > 0) {
        state->speed_boost_timer--;
    }
    if (state->invuln_timer > 0) {
        state->invuln_timer--;
    }

    if (!game_is_over(state)) {
        physics_update_falling(state);
        physics_move_player(state);
    }
    state->progression = (state->tick * PROGRESS_RATE) + PROGRESS_START;
}

int game_is_over(const GameState *state) {
    return state->lives < 0 || state->progression >= PROGRESS_WIN;
}

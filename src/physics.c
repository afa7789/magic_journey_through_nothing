#include "physics.h"
#include <stdlib.h>

static int rdm_range(int min, int range) {
    return min + rand() % range;
}

void physics_move_player(GameState *state) {
    float half_screen = WINDOW_SIZE / 2.0f;
    float half_player = state->player_size.width / 2.0f;
    float boundary = half_screen - half_player;
    float speed_mult = state->speed_boost_timer > 0 ? SPEED_BOOST_MULT : 1.0f;

    if (state->is_left_pressed && state->player_pos.x - PLAYER_SPEED_LEFT * speed_mult > -boundary) {
        state->player_pos.x -= PLAYER_SPEED_LEFT * speed_mult;
    }
    if (state->is_right_pressed && state->player_pos.x + PLAYER_SPEED_RIGHT * speed_mult < boundary) {
        state->player_pos.x += PLAYER_SPEED_RIGHT * speed_mult;
    }
}

int physics_check_collision(const GameState *state, int i) {
    float px = state->player_pos.x;
    float py = state->player_pos.y;
    float hw = PLAYER_HITBOX_W / 2.0f;
    float hh = PLAYER_HITBOX_H / 2.0f;

    float ox = state->falling_objects[i].x;
    float oy = state->falling_objects[i].y;

    return (px - hw < ox && ox < px + hw) &&
           (py - hh < oy && oy < py + hh);
}

void physics_respawn_object(GameState *state, int index) {
    state->falling_objects[index].y = (float)rdm_range(260, 40);
    state->falling_objects[index].x = (float)rdm_range(SPAWN_X_MIN, SPAWN_X_RANGE);
}

static void accelerate(GameState *state, int i) {
    float y = state->falling_objects[i].y;
    if (y < 50.0f)  state->fall_speed += FALL_ACCELERATION;
    if (y < 0.0f)   state->fall_speed += FALL_ACCELERATION;
    if (y < -210.0f) state->fall_speed += FALL_ACCELERATION;
}

void physics_update_falling(GameState *state) {
    for (int i = 0; i < state->falling_count; i++) {
        state->falling_objects[i].y -= (state->fall_speed + state->bonus_speed[i]);
        accelerate(state, i);

        if (physics_check_collision(state, i)) {
            if (state->invuln_timer <= 0) {
                state->lives--;
                state->hit_flash_timer = HIT_FLASH_DURATION;
            }
            physics_respawn_object(state, i);
        }

        if (state->falling_objects[i].y < (float)rdm_range(-290, 80)) {
            physics_respawn_object(state, i);
        }
    }

    if (state->tick % TICKS_PER_SECOND == 0 &&
        state->seconds % SPAWN_INTERVAL_SEC == 0 &&
        state->falling_count < MAX_FALLING_OBJECTS) {
        state->falling_count++;
    }

    /* Spawn a power-up every POWERUP_SPAWN_SEC seconds */
    if (state->tick % TICKS_PER_SECOND == 0 &&
        state->seconds % POWERUP_SPAWN_SEC == 0 &&
        state->seconds > 0) {
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!state->powerup_active[i]) {
                state->powerup_active[i] = 1;
                state->powerup_type[i] = (rand() % 2 == 0) ? POWERUP_SPEED : POWERUP_INVULN;
                state->powerups[i].x = (float)rdm_range(SPAWN_X_MIN, SPAWN_X_RANGE);
                state->powerups[i].y = 260.0f;
                break;
            }
        }
    }

    /* Update power-ups */
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!state->powerup_active[i]) continue;

        state->powerups[i].y -= POWERUP_FALL_SPEED;

        /* Check collision with player */
        float px = state->player_pos.x;
        float py = state->player_pos.y;
        float hw = PLAYER_HITBOX_W / 2.0f;
        float hh = PLAYER_HITBOX_H / 2.0f;
        float ox = state->powerups[i].x;
        float oy = state->powerups[i].y;

        if ((px - hw < ox && ox < px + hw) &&
            (py - hh < oy && oy < py + hh)) {
            if (state->powerup_type[i] == POWERUP_SPEED) {
                state->speed_boost_timer = SPEED_BOOST_DURATION;
            } else {
                state->invuln_timer = INVULN_DURATION;
            }
            state->powerup_active[i] = 0;
        }

        /* Off-screen */
        if (state->powerups[i].y < -260.0f) {
            state->powerup_active[i] = 0;
        }
    }
}

#include "input.h"
#include <stdlib.h>

void input_key_down(GameState *state, unsigned char key) {
    switch (key) {
    case 27: /* ESC */
        exit(0);
        break;
    case 'a': case 'A':
        if (!state->is_right_pressed)
            state->is_left_pressed = 1;
        break;
    case 'd': case 'D':
        if (!state->is_left_pressed)
            state->is_right_pressed = 1;
        break;
    case 'r':
        state->r_press_count++;
        if (state->r_press_count >= 2) {
            state->r_press_count = 0;
            game_reset(state);
        }
        break;
    case 'p': case 'P':
        state->is_paused = !state->is_paused;
        break;
    case 's': case 'S':
        if (state->falling_count < MAX_FALLING_OBJECTS)
            state->falling_count++;
        break;
    default:
        break;
    }
}

void input_key_up(GameState *state, unsigned char key) {
    switch (key) {
    case 'a': case 'A':
        state->is_left_pressed = 0;
        break;
    case 'd': case 'D':
        state->is_right_pressed = 0;
        break;
    default:
        break;
    }
}

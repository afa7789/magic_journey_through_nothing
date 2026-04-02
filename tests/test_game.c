#include "../vendor/unity/unity.h"
#include "../include/game.h"
#include <stdlib.h>

static GameState state;

void setUp(void) {
    srand(42);
    game_reset(&state);
}

void tearDown(void) {}

void test_init_sets_lives(void) {
    TEST_ASSERT_EQUAL_INT(INITIAL_LIVES, state.lives);
}

void test_init_sets_player_position(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PLAYER_START_X, state.player_pos.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PLAYER_START_Y, state.player_pos.y);
}

void test_init_sets_player_size(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PLAYER_WIDTH,  state.player_size.width);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PLAYER_HEIGHT, state.player_size.height);
}

void test_init_falling_count(void) {
    TEST_ASSERT_EQUAL_INT(1, state.falling_count);
}

void test_init_fall_speed(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, INITIAL_FALL_SPEED, state.fall_speed);
}

void test_init_progression(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.01f, PROGRESS_START, state.progression);
}

void test_init_not_paused(void) {
    TEST_ASSERT_EQUAL_INT(0, state.is_paused);
}

void test_game_not_over_at_start(void) {
    TEST_ASSERT_FALSE(game_is_over(&state));
}

void test_game_over_no_lives(void) {
    state.lives = -1;
    TEST_ASSERT_TRUE(game_is_over(&state));
}

void test_game_over_progression_complete(void) {
    state.progression = PROGRESS_WIN;
    TEST_ASSERT_TRUE(game_is_over(&state));
}

void test_update_increments_tick(void) {
    game_update(&state);
    TEST_ASSERT_EQUAL_INT(1, state.tick);
}

void test_update_skipped_when_paused(void) {
    state.is_paused = 1;
    game_update(&state);
    TEST_ASSERT_EQUAL_INT(0, state.tick);
}

void test_update_ticks_during_game_over(void) {
    state.lives = -1;
    game_update(&state);
    TEST_ASSERT_EQUAL_INT(1, state.tick);
}

void test_progression_advances(void) {
    float old = state.progression;
    game_update(&state);
    TEST_ASSERT_TRUE(state.progression > old);
}

void test_reset_restores_lives(void) {
    state.lives = 0;
    game_reset(&state);
    TEST_ASSERT_EQUAL_INT(INITIAL_LIVES, state.lives);
}

void test_seconds_increment_at_tick_rate(void) {
    for (int i = 0; i < TICKS_PER_SECOND; i++) {
        game_update(&state);
    }
    TEST_ASSERT_EQUAL_INT(1, state.seconds);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_sets_lives);
    RUN_TEST(test_init_sets_player_position);
    RUN_TEST(test_init_sets_player_size);
    RUN_TEST(test_init_falling_count);
    RUN_TEST(test_init_fall_speed);
    RUN_TEST(test_init_progression);
    RUN_TEST(test_init_not_paused);
    RUN_TEST(test_game_not_over_at_start);
    RUN_TEST(test_game_over_no_lives);
    RUN_TEST(test_game_over_progression_complete);
    RUN_TEST(test_update_increments_tick);
    RUN_TEST(test_update_skipped_when_paused);
    RUN_TEST(test_update_ticks_during_game_over);
    RUN_TEST(test_progression_advances);
    RUN_TEST(test_reset_restores_lives);
    RUN_TEST(test_seconds_increment_at_tick_rate);
    return UNITY_END();
}

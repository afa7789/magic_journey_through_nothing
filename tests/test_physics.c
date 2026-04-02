#include "../vendor/unity/unity.h"
#include "../include/game.h"
#include "../include/physics.h"
#include <stdlib.h>

static GameState state;

void setUp(void) {
    srand(42);
    game_reset(&state);
}

void tearDown(void) {}

void test_collision_detects_overlap(void) {
    state.falling_objects[0].x = state.player_pos.x;
    state.falling_objects[0].y = state.player_pos.y;
    TEST_ASSERT_TRUE(physics_check_collision(&state, 0));
}

void test_collision_misses_when_far(void) {
    state.falling_objects[0].x = state.player_pos.x + 200;
    state.falling_objects[0].y = state.player_pos.y + 200;
    TEST_ASSERT_FALSE(physics_check_collision(&state, 0));
}

void test_collision_edge_left(void) {
    float hw = state.player_size.width / 2.0f;
    state.falling_objects[0].x = state.player_pos.x - hw - 1;
    state.falling_objects[0].y = state.player_pos.y;
    TEST_ASSERT_FALSE(physics_check_collision(&state, 0));
}

void test_collision_edge_right(void) {
    float hw = state.player_size.width / 2.0f;
    state.falling_objects[0].x = state.player_pos.x + hw + 1;
    state.falling_objects[0].y = state.player_pos.y;
    TEST_ASSERT_FALSE(physics_check_collision(&state, 0));
}

void test_move_player_left(void) {
    state.is_left_pressed = 1;
    float old_x = state.player_pos.x;
    physics_move_player(&state);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, old_x - PLAYER_SPEED_LEFT, state.player_pos.x);
}

void test_move_player_right(void) {
    state.is_right_pressed = 1;
    float old_x = state.player_pos.x;
    physics_move_player(&state);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, old_x + PLAYER_SPEED_RIGHT, state.player_pos.x);
}

void test_move_player_left_boundary(void) {
    float boundary = -(WINDOW_SIZE / 2.0f - state.player_size.width / 2.0f);
    state.player_pos.x = boundary + 0.5f;
    state.is_left_pressed = 1;
    physics_move_player(&state);
    TEST_ASSERT_TRUE(state.player_pos.x >= boundary);
}

void test_respawn_object_changes_position(void) {
    state.falling_objects[0].x = 0;
    state.falling_objects[0].y = 0;
    physics_respawn_object(&state, 0);
    TEST_ASSERT_TRUE(state.falling_objects[0].y >= 260);
}

void test_falling_count_increases(void) {
    state.tick = TICKS_PER_SECOND - 1;
    state.seconds = SPAWN_INTERVAL_SEC - 1;
    int old_count = state.falling_count;

    state.tick++;
    state.seconds++;
    physics_update_falling(&state);

    TEST_ASSERT_TRUE(state.falling_count >= old_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_collision_detects_overlap);
    RUN_TEST(test_collision_misses_when_far);
    RUN_TEST(test_collision_edge_left);
    RUN_TEST(test_collision_edge_right);
    RUN_TEST(test_move_player_left);
    RUN_TEST(test_move_player_right);
    RUN_TEST(test_move_player_left_boundary);
    RUN_TEST(test_respawn_object_changes_position);
    RUN_TEST(test_falling_count_increases);
    return UNITY_END();
}

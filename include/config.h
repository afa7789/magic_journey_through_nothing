#ifndef CONFIG_H
#define CONFIG_H

#define WINDOW_SIZE        500
#define WINDOW_TITLE       "Magic Journey Through Nothing"

#define PLAYER_WIDTH       42.0f
#define PLAYER_HEIGHT      44.0f
#define PLAYER_HITBOX_W    22.0f
#define PLAYER_HITBOX_H    38.0f
#define PLAYER_START_X     0.0f
#define PLAYER_START_Y     -192.0f
#define PLAYER_SPEED_LEFT  1.65f
#define PLAYER_SPEED_RIGHT 1.65f

#define FALLING_OBJ_WIDTH  8.0f
#define FALLING_OBJ_HEIGHT 8.0f
#define MAX_FALLING_OBJECTS 1000
#define INITIAL_FALL_SPEED 1.0f
#define FALL_ACCELERATION  0.000001f

#define INITIAL_LIVES      3
#define HEART_SIZE         15.0f
#define HEART_SPACING      20.0f

#define TICKS_PER_SECOND   47
#define SPAWN_INTERVAL_SEC 5
#define FRAME_DELAY_MS     16
#define HIT_FLASH_DURATION 20

#define GROUND_Y_TOP       -210.0f
#define GROUND_Y_BOTTOM    -250.0f

#define PROGRESS_RATE      0.017f
#define PROGRESS_START     -240.0f
#define PROGRESS_WIN       240.0f

#define SPAWN_X_MIN        -246
#define SPAWN_X_RANGE      496
#define SPAWN_Y_MIN        250
#define SPAWN_Y_RANGE      200
#define BONUS_RANGE         50

#define MAX_POWERUPS         3
#define POWERUP_SIZE         10.0f
#define POWERUP_FALL_SPEED   1.5f
#define POWERUP_SPAWN_SEC    8
#define SPEED_BOOST_DURATION 150
#define SPEED_BOOST_MULT     2.0f
#define INVULN_DURATION      150

#define POWERUP_NONE         0
#define POWERUP_SPEED        1
#define POWERUP_INVULN       2

#define ASSET_PATH_CREATURE  "assets/creature1.png"
#define ASSET_PATH_GROUND    "assets/bar1.png"
#define ASSET_PATH_WHITE     "assets/branco.png"
#define ASSET_PATH_GAMEOVER  "assets/gameover.png"
#define ASSET_PATH_BACKGROUND "assets/canvas1.png"

#endif

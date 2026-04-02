#ifndef PHYSICS_H
#define PHYSICS_H

#include "game.h"

void physics_move_player(GameState *state);
void physics_update_falling(GameState *state);
int  physics_check_collision(const GameState *state, int object_index);
void physics_respawn_object(GameState *state, int index);

#endif

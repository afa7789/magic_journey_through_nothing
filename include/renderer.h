#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"

typedef struct {
    unsigned int tex_creature;
    unsigned int tex_ground;
    unsigned int tex_white;
    unsigned int tex_gameover;
    unsigned int tex_background;
} Textures;

void renderer_init(Textures *textures);
void renderer_reshape(int w, int h);
void renderer_draw_scene(const GameState *state, const Textures *textures);

#endif

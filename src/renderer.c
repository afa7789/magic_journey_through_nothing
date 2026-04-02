#include "platform.h"
#include "renderer.h"
#include <SOIL/SOIL.h>

void renderer_init(Textures *tex) {
    tex->tex_creature = SOIL_load_OGL_texture(ASSET_PATH_CREATURE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    tex->tex_ground   = SOIL_load_OGL_texture(ASSET_PATH_GROUND,   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    tex->tex_white    = SOIL_load_OGL_texture(ASSET_PATH_WHITE,    SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    tex->tex_gameover   = SOIL_load_OGL_texture(ASSET_PATH_GAMEOVER,   SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    tex->tex_background = SOIL_load_OGL_texture(ASSET_PATH_BACKGROUND, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    /* Pixel art textures: nearest-neighbor filtering for crisp rendering */
    glBindTexture(GL_TEXTURE_2D, tex->tex_creature);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, tex->tex_white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    /* Background texture: clamp so it doesn't tile, linear for smooth scroll */
    glBindTexture(GL_TEXTURE_2D, tex->tex_background);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderer_reshape(int w, int h) {
    (void)w; (void)h;
    int half = WINDOW_SIZE / 2;
    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-half, half, -half, half, -3, 3);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void draw_textured_quad(GLuint tex, float x, float y, float hw, float hh, float z) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, tex);
    glTranslatef(x, y, 0);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(-hw, -hh, z);
        glTexCoord2f(1, 0); glVertex3f( hw, -hh, z);
        glTexCoord2f(1, 1); glVertex3f( hw,  hh, z);
        glTexCoord2f(0, 1); glVertex3f(-hw,  hh, z);
    glEnd();
    glPopMatrix();
}

static void draw_single_heart(float x, float y) {
    float s = HEART_SIZE;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(-s,     0,       1);
        glVertex3f( 0,    -s,       1);
        glVertex3f( s,     0,       1);
        glVertex3f( s/2,   s,       1);
        glVertex3f( 0,     s/2,     1);
        glVertex3f(-s/2,   s,       1);
    glEnd();
    glPopMatrix();
}

static void draw_hearts(int lives, float x, float y) {
    glColor3f(1, 1, 1);
    for (int i = 0; i < lives && i < INITIAL_LIVES; i++) {
        draw_single_heart(x - i * 2 * HEART_SPACING, y);
    }
}

void renderer_draw_scene(const GameState *state, const Textures *tex) {
    int half = WINDOW_SIZE / 2;

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    /* Scrolling background — UV scroll only, no geometry movement.
       The image is 500x10000 (ratio 1:20), so we show a 1:1 window
       (v_size = 500/10000 = 0.05) that slides from bottom to top. */
    {
        float scroll_progress = (state->progression - PROGRESS_START)
                              / (PROGRESS_WIN - PROGRESS_START);
        if (scroll_progress < 0.0f) scroll_progress = 0.0f;
        if (scroll_progress > 1.0f) scroll_progress = 1.0f;

        float v_size = 0.05f; /* visible fraction of the tall image */
        float v_bottom = scroll_progress * (1.0f - v_size);
        float v_top    = v_bottom + v_size;

        glBindTexture(GL_TEXTURE_2D, tex->tex_background);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, v_bottom); glVertex3f(-half, -half, -2);
            glTexCoord2f(1, v_bottom); glVertex3f( half, -half, -2);
            glTexCoord2f(1, v_top);    glVertex3f( half,  half, -2);
            glTexCoord2f(0, v_top);    glVertex3f(-half,  half, -2);
        glEnd();
    }

    /* Player — tint for effects */
    if (state->hit_flash_timer > 0) {
        float t = (float)state->hit_flash_timer / HIT_FLASH_DURATION;
        glColor3f(1.0f, 1.0f - t, 1.0f - t);
    } else if (state->invuln_timer > 0) {
        glColor3f(1.0f, 0.4f, 0.7f);
    } else if (state->speed_boost_timer > 0) {
        glColor3f(1.0f, 1.0f, 0.0f);
    }
    draw_textured_quad(tex->tex_creature,
        state->player_pos.x, state->player_pos.y,
        state->player_size.width / 2, state->player_size.height / 2, 2);
    glColor3f(1, 1, 1);

    /* Falling objects */
    float fhw = state->falling_object_size.width / 2;
    float fhh = state->falling_object_size.height / 2;
    for (int i = 0; i < state->falling_count; i++) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, tex->tex_white);
        glTranslatef(state->falling_objects[i].x, state->falling_objects[i].y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0); glVertex3f(-fhw,  0,    1);
            glTexCoord2f(1, 0); glVertex3f( 0,   -fhh,  1);
            glTexCoord2f(1, 1); glVertex3f( fhw,  0,    1);
            glTexCoord2f(0, 1); glVertex3f( 0,    fhh,  1);
        glEnd();
        glPopMatrix();
    }

    /* Power-ups — draw without texture so glColor takes effect */
    glDisable(GL_TEXTURE_2D);
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!state->powerup_active[i]) continue;
        float ps = POWERUP_SIZE / 2;
        if (state->powerup_type[i] == POWERUP_SPEED) {
            glColor3f(1.0f, 1.0f, 0.0f); /* Yellow */
        } else {
            glColor3f(1.0f, 0.4f, 0.7f); /* Pink */
        }
        glPushMatrix();
        glTranslatef(state->powerups[i].x, state->powerups[i].y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glVertex3f(-ps,  0,   1);
            glVertex3f( 0,  -ps,  1);
            glVertex3f( ps,  0,   1);
            glVertex3f( 0,   ps,  1);
        glEnd();
        glPopMatrix();
    }
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);

    /* Hearts (HUD) */
    draw_hearts(state->lives, half - HEART_SPACING, half - HEART_SPACING);

    /* Ground */
    draw_textured_quad(tex->tex_ground, 0, (GROUND_Y_TOP + GROUND_Y_BOTTOM) / 2,
        half, (GROUND_Y_TOP - GROUND_Y_BOTTOM) / 2, 1);

    /* Progress bar */
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, tex->tex_white);
    float bar_left = -(half - 10);
    float bar_bottom = -(half - 10);
    float bar_top = -(half - 20);
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0); glVertex3f(bar_left,          bar_bottom, 1);
        glTexCoord2f(1, 0); glVertex3f(state->progression, bar_bottom, 1);
        glTexCoord2f(1, 1); glVertex3f(state->progression, bar_top,    1);
        glTexCoord2f(0, 1); glVertex3f(bar_left,          bar_top,    1);
    glEnd();
    glPopMatrix();

    /* Game Over overlay */
    if (game_is_over(state)) {
        draw_textured_quad(tex->tex_gameover, 0, 0, half, half, 3);
    }

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

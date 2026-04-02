#include "platform.h"
#include "game.h"
#include "input.h"
#include "renderer.h"

static GameState g_state;
static Textures  g_textures;

/* Forward declaration — needed before on_key_down references it */
void on_timer_cb(int value);

static void on_display(void) {
    renderer_draw_scene(&g_state, &g_textures);
}

static void on_reshape(int w, int h) {
    renderer_reshape(w, h);
}

static void on_key_down(unsigned char key, int x, int y) {
    (void)x; (void)y;
    int was_paused = g_state.is_paused;
    input_key_down(&g_state, key);

    /* Re-start the timer if we just unpaused */
    if (was_paused && !g_state.is_paused) {
        glutTimerFunc(FRAME_DELAY_MS, on_timer_cb, 0);
    }
}

static void on_key_up(unsigned char key, int x, int y) {
    (void)x; (void)y;
    input_key_up(&g_state, key);
}

void on_timer_cb(int value) {
    (void)value;
    game_update(&g_state);
    glutPostRedisplay();
    if (!g_state.is_paused) {
        glutTimerFunc(FRAME_DELAY_MS, on_timer_cb, 0);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
#ifndef __APPLE__
    glutInitContextVersion(1, 1);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
#endif
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(WINDOW_TITLE);

    game_init(&g_state);
    renderer_init(&g_textures);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_key_down);
    glutKeyboardUpFunc(on_key_up);
    glutTimerFunc(0, on_timer_cb, 0);
    glutMainLoop();

    return 0;
}

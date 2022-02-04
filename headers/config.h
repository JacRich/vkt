#ifndef CONFIG_H
#define CONFIG_H

#include "globaldef.h"
#include <GLFW/glfw3.h>

struct config_t {
    bool fullbright = false;
    bool hideGUI = false;
    bool recordingMode = false;

    int width = 1280;
    int height = 720;

    float fov = 90.0f;
    float looksens = 0.05f;
    float renderdist = 32.0f * 8.0f;

    float moveSpeed = 60.0f;
    float friction = 12.0f;

    vec spawnpos = {0, 0, 0};
};

struct config_key_t {
    int forward = GLFW_KEY_W;
    int backward = GLFW_KEY_S;
    int left = GLFW_KEY_A;
    int right = GLFW_KEY_D;

    int switchmov = GLFW_KEY_LEFT_SHIFT;
    int jump = GLFW_KEY_SPACE;

    int toggle_fullscreen = GLFW_KEY_F11;
    int toggle_coll = GLFW_KEY_KP_0;
    int toggle_fullbright = GLFW_KEY_KP_1;

    int im_blocks = GLFW_KEY_1;
    int im_range = GLFW_KEY_2;
    int im_range_replace = GLFW_KEY_3;

    int toggle_cursor = GLFW_KEY_GRAVE_ACCENT;
    int input_id = GLFW_KEY_TAB;

    int toggle_gui = GLFW_KEY_F1;
    int toggle_recording = GLFW_KEY_F2;
};

struct config_mouse_t {
    int place = GLFW_MOUSE_BUTTON_2;
    int destroy = GLFW_MOUSE_BUTTON_1;
    int pickblock = GLFW_MOUSE_BUTTON_3;

};

extern config_t config;
extern config_key_t keys;
extern config_mouse_t mouse;

void config_init();

#endif

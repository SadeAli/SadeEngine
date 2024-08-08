#pragma once

typedef struct WindowSettings
{
    char *title;

    // size
    struct
    {
        int width;
        int height;
    };
    
    int fps;
} WindowSettings;

/**
 * @brief an initializer for window calls init window with default window settings
 */
#define INIT_WINDOW(...)                                                                                               \
    initWindow(&(WindowSettings){.width = 1920, .height = 1080, .fps = 60, .title = "myLeaf", __VA_ARGS__})

void initWindow(const WindowSettings ws[static 1]);
WindowSettings initDefaultWindow();

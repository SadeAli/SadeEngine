#pragma once

typedef struct WindowSettings
{
    int width;
    int height;
    char *title;
    int fps;
} WindowSettings;

void initWindow(WindowSettings ws[static 1]);
WindowSettings initDefaultWindow();
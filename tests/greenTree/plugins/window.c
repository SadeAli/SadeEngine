#include "plugin.h"
#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>

typedef struct WindowProperties
{
    struct
    {
        int width;
        int height;
    } size;

    int fps;

} WindowProperties;

void render(Plugin *plugin);

Plugin plugin_init()
{
    WindowProperties *properties = malloc(sizeof(WindowProperties));
    if (!properties) {
        fprintf(stderr, "Failed to allocate memory for WindowProperties\n");
        exit(1);
    }
    *properties = (WindowProperties){
        .fps = 60,
        .size = {
            .width = 1920,
            .height = 1080,
        },
    };

    Plugin self = {
        .handle = NULL,
        .name = "plugWindow",
        .state = {
            .exitRequested = 0,
            .exitCode = 1,
        },
        .update = render,
        .exit = NULL,
        .data = properties,
    };

    InitWindow(properties->size.width, properties->size.height, "WindowPlugin");
    SetTargetFPS(properties->fps);

    return self;
}

void render(Plugin *plugin)
{
    WindowProperties *wp = (WindowProperties *)plugin->data;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(100, 100, 200, 150, RED);
    EndDrawing();
}

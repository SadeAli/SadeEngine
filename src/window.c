#include "window.h"
#include <raylib.h>

void initWindow(const WindowSettings ws[static 1])
{
    InitWindow(ws->width, ws->height, ws->title);
    SetTargetFPS(ws->fps);
}

WindowSettings initDefaultWindow() {
    WindowSettings ws = {
        .title = "myLeaf",
        .width = 1920,
        .height = 1080,
        .fps = 60
    };

    InitWindow(ws.width, ws.height, ws.title);
    SetTargetFPS(ws.fps);

    return ws;
}

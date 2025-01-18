#include "window.h"
#include <raylib.h>

void initWindow(WindowSettings *ws)
{
    InitWindow(ws->width, ws->height, ws->title);
    SetTargetFPS(ws->fps);
}
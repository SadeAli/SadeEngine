#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>

#include "relativePosition.h"
#include "vec.h"
#include "window.h"

#include "editorClasses/dragable.h"

typedef struct TrailLine2D_t
{
    // line storage
#define MAX 200
    struct
    {
        vec2 points[MAX];
        u32 pointIndex;
    };

} TrailLine2D;

struct Engine
{
    WindowSettings windowSettings;
} engine = {
    .windowSettings =
        {
            .fps = 60,
            .height = 1080,
            .width = 1920,
            .title = "myLeaf",
        },
};

int main(void)
{
    TrailLine2D trail = {0};

    double myTime = 0;

    RelativePosition2D a = init_rp2d((vec2){.data = {100, 100}});
    RelativePosition2D b = init_rp2d((vec2){.data = {50, 50}});
    rp2d_addChild(&a, b);

    initWindow(&engine.windowSettings);

    vec2 position = {.data = {10, 10}};
    vec2 position2 = {.data = {10, 10}};
    vec2 position3 = {.data = {10, 10}};

    Dragable2D m1 = init_dragable2d(init_vec2(500, 500), init_vec2(60, 60));
    Dragable2D m2 = init_dragable2d(init_vec2(100, 100), init_vec2(160, 160));


    while (!WindowShouldClose())
    {
        myTime += GetFrameTime();
        const float timeScale = 10;

        position.y = 230 * (-sin(myTime * 20.2 * timeScale));
        position.x = 230 * (-cos(myTime * 20.3 * timeScale));

        position2.y = 90 * (sin(myTime * 51.3 * timeScale));
        position2.x = 90 * (cos(myTime * 51.1 * timeScale));

        position3.x = 400 * (sin(myTime * 10.1 * timeScale)) + 960;
        position3.y = 200 * (cos(myTime * 10.9 * timeScale)) + 540;

        rp2d_setAnchorPosition(&a, position3);

        rp2d_setRelativePosition(&a, position);
        rp2d_setRelativePosition(&a.children.data[0], position2);

        dragable2d_update(&m1);
        dragable2d_update(&m2);

        trail.points[trail.pointIndex] = rp2d_globalPosition(a.children.data[0]);
        trail.pointIndex = (trail.pointIndex + 1) % MAX;

        BeginDrawing();
        ClearBackground(DARKBLUE);

        DrawText("hello\nworld\nfrom sade ali", 10, 10, 20, RED);
        DrawFPS(10, 80);

        DrawCircleV(*(Vector2 *)rp2d_globalPosition(a).data, 10, RED);
        for (u32 i = 0; i < a.children.count; i++)
        {
            DrawCircleV(*(Vector2 *)rp2d_globalPosition(a.children.data[i]).data, 10, RED);
        }

        dragable2d_draw(&m1);
        dragable2d_draw(&m2);

        DrawLineStrip((Vector2 *)trail.points, trail.pointIndex, GREEN);
        if (trail.pointIndex != 0)
        {
            DrawLineV(trail.points[0].raylibV, trail.points[MAX - 1].raylibV, GREEN);
        }
        DrawLineStrip((Vector2 *)trail.points + trail.pointIndex, MAX - trail.pointIndex, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

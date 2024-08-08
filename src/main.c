#include <assert.h>
#include <raymath.h>
#include <stdint.h>

#include "relativePosition.h"
#include "vec.h"
#include "window.h"
#include "editorClasses/dragable.h"
#include "2d/trailLine2d.h"
#include "shapes/rect.h"

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

typedef struct UiPanel_t {
    vec2 position;
    RenderTexture2D texture;
    Color bgColor;
} UiPanel;

UiPanel init_UiPanel(vec2 pos, vec2 size, Color bgColor) {
    const UiPanel panel = {
        .position = pos,
        .texture = LoadRenderTexture(size.x, size.y),
        .bgColor = bgColor,
    };

    return panel;
}

void uiPanel_update(UiPanel *panel) {
    BeginTextureMode(panel->texture);
    
    ClearBackground(panel->bgColor);

    EndTextureMode();
}

void uiPanel_draw(UiPanel *panel) {
    DrawTextureV(panel->texture.texture, panel->position.raylibV, WHITE);
}

int main(void)
{
    TrailLine2D trail = init_trailLine2d(100);

    double myTime = 0;

    initWindow(&engine.windowSettings);

    rp2d rp1 = init_rp2d(init_vec2(500, 500));
    rp2d_addChild(&rp1, init_rp2d(init_vec2(-200, -200)));

    Dragable2D m1 = init_dragable2d(&rp1, init_vec2(160, 160));
    Dragable2D m2 = init_dragable2d(&rp1.children.data[0], init_vec2(60, 60));

    UiPanel panel = init_UiPanel(init_vec2(10,10), init_vec2(100, 400), RED);

    while (!WindowShouldClose())
    {
        myTime += GetFrameTime();

        dragable2d_update(&m1);
        dragable2d_update(&m2);

        trailLine2d_nextPoint(&trail, rp2d_globalPosition(m2.position));

        uiPanel_update(&panel);

        BeginDrawing();
        {
            ClearBackground(DARKBLUE);
            uiPanel_draw(&panel);

            DrawFPS(12, 12);

            dragable2d_draw(&m1);
            dragable2d_draw(&m2);

            trailLine2d_draw(&trail);

            DrawText(TextFormat("%f - %f", rp2d_globalPosition(m2.position).x, rp2d_globalPosition(m2.position).y), 150, 10, 20, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

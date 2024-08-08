#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "defaultBehaviour.h"
#include "secondOrderSystem.h"
#include "window.h"

#include "3d/camera3D.h"

#include "ui/button.h"
#include <raymath.h>

int main(void)
{
    int fps = 120;
    INIT_WINDOW(.fps = fps);

    Camera camera = INIT_CAMERA3D(.position = (Vector3){10, 10, 0});

    Button b = {
        .rect = {10, 10, 100, 70},
        .color = RED,
        .onPress = buttonTestPressAction,
        .onRelease = buttonTestReleaseAction,
    };

    BoundingBox bb = {
        {-1, 0, -1},
        {1, 2, 1},
    };

    Ray r = GetMouseRay(GetMousePosition(), camera);
    RayCollision c = GetRayCollisionBox(r, bb);

    while (!WindowShouldClose())
    {
        updateButton(&b);

        UpdateCamera(&camera, CAMERA_ORBITAL);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            r = GetMouseRay(GetMousePosition(), camera);
            c = GetRayCollisionBox(r, bb);
        }

        BeginDrawing();
        {
            ClearBackground(DARKBLUE);

            BeginMode3D(camera);
            {
                DrawGrid(10, 1);

                DrawBoundingBox(bb, GREEN);
                DrawSphere(c.point, 0.1, RED);
            }
            EndMode3D();

            buttonDraw(&b);
        }
        EndDrawing();
    }

    return 0;
}
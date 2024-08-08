#pragma clang diagnostic ignored "-Winitializer-overrides"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <rgestures.h>
#include <rlgl.h>
#include <stdio.h>

#include "camera.h"
#include "node.h"
#include "relativePosition2D.h"

#include "defaultBehaviour.h"

int relativePositionExample(void);
int NodeTreeTest(void);

int main()
{
    return NodeTreeTest();
    // return relativePositionExample();
}

int relativePositionExample()
{
    INIT_WINDOW();
    Camera2D camera = (Camera2D){{960, 540}, {960, 540}, 0, 1};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
            {
                DrawRectangleV((Vector2){10, 10}, (Vector2){1900, 1060}, COLOR_INIT(.b = 120, .g = 100, .r = 95));
            }
            EndMode2D();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

int NodeTreeTest()
{
    Node root = {.nodeType = NODE_TYPE_CHAR_PTR,
                 .data = "hello world",
                 .children = NULL,
                 .child_capacity = 0,
                 .depth = 0,
                 .child_count = 0};

    NodeAddChild(&root, "hello", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root, "dogdok", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root, "ali", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root, "world", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0], "bir suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0].children[0], "2 suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0], "bir suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0], "bir suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0].children[1], "2 suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0].children[1], "2 suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0].children[1], "2 suru ben", NODE_TYPE_CHAR_PTR);
    NodeAddChild(&root.children[0], "bruh", NODE_TYPE_CHAR_PTR);

    NodeTreePrint(&root);

    return 0;
}

int RayTest()
// int main()
{
    INIT_WINDOW();

    // scene display settings
    rlSetLineWidth(5);

    // load resources
    RenderTexture2D viewport = LoadRenderTexture(1920, 1080);

    // scene settings
    Camera3D camera = CAMERA3D_INIT(.position.x = 20, //
                                    .position.y = 20);

    const int quadSize = 40;
    Ray ray = {};
    RayCollision collision = {};

    BoundingBox box = {{-1, 0, -1}, {1, 2, 1}};

    while (!WindowShouldClose())
    {
        // ---------------------------------------------------------------------------------------------------------------
        // handle events
        bool isBoxSelected = false;

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            updateCameraCustom(&camera, 1, 0.005, 0.03);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            ray = GetMouseRay(GetMousePosition(), camera);

            collision = GetRayCollisionBox(ray, box);
            if (collision.hit)
            {
                isBoxSelected = true;
            }
            else
            {
                collision = GetRayCollisionQuad(ray,
                                                (Vector3){-quadSize, 0, -quadSize},
                                                (Vector3){quadSize, 0, -quadSize},
                                                (Vector3){quadSize, 0, quadSize},
                                                (Vector3){-quadSize, 0, quadSize});
            }
        }

        // ---------------------------------------------------------------------------------------------------------------
        // render things on viewport
        BeginTextureMode(viewport);
        {
            ClearBackground(WHITE);

            BeginMode3D(camera);
            {
                // ClearBackground(WHITE);

                DrawGrid(5, 10);

                DrawRay(ray, RED);

                DrawSphere(collision.point, 0.3, RED);

                DrawCube((Vector3){0, 1, 0}, 2, 2, 2, LIGHTGRAY);
                if (isBoxSelected)
                {
                    DrawBoundingBox(box, GREEN);
                }
            }

            EndMode3D();
        }
        EndTextureMode();

        // ---------------------------------------------------------------------------------------------------------------
        // render viewport
        BeginDrawing();
        {
            ClearBackground(BLACK);

            DrawTextureRec(
                viewport.texture, (Rectangle){.width = 1920, .height = -1080, .x = 0, .y = 0}, (Vector2){}, WHITE);
        }
        EndDrawing();
    }

    UnloadRenderTexture(viewport);
    CloseWindow();

    return 0;
}
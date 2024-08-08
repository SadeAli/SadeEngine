#include <raylib.h>
#include <raymath.h>

#define COLOR_PRIMARY                                                          \
    (Color) { 0xED, 0x56, 0x45, 0xff }
#define COLOR_SECONDARY                                                        \
    (Color) { 0xE9, 0xB6, 0x49, 0xff }
#define COLOR_LIGHT                                                            \
    (Color) { 0xE4, 0xd7, 0xea, 0xff }
#define COLOR_MID                                                              \
    (Color) { 0x51, 0x67, 0x66, 0xff }
#define COLOR_DARK                                                             \
    (Color) { 0x24, 0x33, 0x3d, 0xff }

int main() {

    // setup window
    const Vector2 windowSize = {.x = 1920, .y = 1080};
    InitWindow(windowSize.x, windowSize.y, "myLeaf");
    SetTargetFPS(60);

    int randomSeed = 0;

    // main loop
    while (!WindowShouldClose()) {

        // UpdateCamera(&cameraTP, CAMERA_FREE);

        BeginDrawing();
        {
            ClearBackground(COLOR_MID);

            BeginMode3D(cameraTP);
            {
                DrawCapsule((Vector3){0, 0, 0}, (Vector3){0, 2, 0}, 0.45, 4, 1,
                            COLOR_PRIMARY);

                DrawSphere(
                    Vector3Add((Vector3){0, 1, 0},
                               Vector3Normalize(Vector3Subtract(
                                   cameraTP.position, (Vector3){0, 1, 0}))),
                    0.25, COLOR_SECONDARY);

                DrawModel(monster, (Vector3){0, 1, 0}, 1, WHITE);

                BeginShaderMode(myShader);
                { DrawMesh(head, defaultMaterial, MatrixIdentity()); }
                EndShaderMode();
            }
            EndMode3D();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    UnloadMaterial(defaultMaterial);

    return 0;
}
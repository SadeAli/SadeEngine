
#include <math.h>
#include <raylib.h>
#include <raymath.h>

void drawTree(int depth, Vector3 startPos, Vector3 direction, float splitChance,
              float lenght, float radius);

int main() {

    // window settings
    int screenWidth = 1920;
    int screenHeight = 1080;

    SetTargetFPS(60);
    DrawFPS(10, 10);

    // window initialization
    InitWindow(screenWidth, screenHeight, "myLeaf");

    // craete cameras for each viewport
    Camera3D cam[4];

    cam[0] = (Camera3D){.position = (Vector3){0, 0, 0},
                        .fovy = 40,
                        .target = (Vector3){0, 10, 0},
                        .up = (Vector3){0, -1, 0},
                        .projection = CAMERA_PERSPECTIVE};

    cam[3] = cam[2] = cam[1] = cam[0];
    cam[2].up = (Vector3){1, 0, 0};

    const int watchDistance = 300;

    cam[1].position.x += watchDistance;
    cam[2].position.y += watchDistance;
    cam[3].position.z += watchDistance;

    int gapSize = 5;

    // create viewport textures
    RenderTexture2D viewport[4] = {
        LoadRenderTexture(screenWidth / 2 - gapSize,
                          screenHeight / 2 - gapSize),
        LoadRenderTexture(screenWidth / 2 - gapSize,
                          screenHeight / 2 - gapSize),
        LoadRenderTexture(screenWidth / 2 - gapSize,
                          screenHeight / 2 - gapSize),
        LoadRenderTexture(screenWidth / 2 - gapSize,
                          screenHeight / 2 - gapSize),
    };

    int seed = 0;

    while (!WindowShouldClose()) {

        // handle inputs
        if (GetKeyPressed() == KEY_SPACE ||
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            seed += 1;
        }

        cam[0].position.x = sin(GetTime()) * watchDistance;
        cam[0].position.z = cos(GetTime()) * watchDistance;

        // render viewports to RenderTextures
        for (int i = 0; i < 4; i++) {
            BeginTextureMode(viewport[i]);
            {
                ClearBackground(WHITE);
                BeginMode3D(cam[i]);
                {
                    SetRandomSeed(seed);
                    drawTree(300, (Vector3){0, 0, 0}, (Vector3){0, 1, 0}, 0.2,
                             10, 10);
                }
                EndMode3D();
            }
            EndTextureMode();
        }

        // set viewport offsets (we dont want to draw viewports on top of
        // each other)
        const Vector2 viewportOffset[4] = {
            (Vector2){0, 0},
            (Vector2){screenWidth / 2.0f, 0},
            (Vector2){0, screenHeight / 2.0},
            (Vector2){screenWidth / 2.0f, screenHeight / 2.0},
        };

        // render all viewports
        BeginDrawing();
        {
            ClearBackground(BLACK);

            for (int i = 0; i < 4; i++) {
                DrawTexture(viewport[i].texture,
                            viewportOffset[i].x + gapSize / 2.0,
                            viewportOffset[i].y + gapSize / 2.0, WHITE);
            }

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    for (int i = 0; i < 4; i++)
        UnloadRenderTexture(viewport[i]);
    CloseWindow();
    return 0;
}

void drawTree(int depth, Vector3 startPos, Vector3 direction, float splitChance,
              float lenght, float radius) {

    // exit if depth reached
    if (depth <= 0) {
        return;
    }

    depth -= 1;
    lenght *= 0.9;
    float endRadius = radius * 0.9;
    Vector3 endPos = Vector3Add(startPos, Vector3Scale(direction, lenght));

    Vector3 randomVector = (Vector3){
        GetRandomValue(0, 1000) / 1000.0f - 0.5f,
        GetRandomValue(0, 1000) / 1000.0f - 0.3f,
        GetRandomValue(0, 1000) / 1000.0f - 0.5f,
    };

    // normalize direction
    direction = Vector3Add(direction, randomVector);
    direction = Vector3Normalize(direction);

    DrawCylinderEx(startPos, endPos, radius, endRadius, 10,
                   GetColor(0xe6bc63ff));
    float currentChance = GetRandomValue(0, 1000) / 1000.0f;
    if (splitChance > currentChance) {
        drawTree(depth, endPos, direction, 0, lenght, endRadius);
    }

    drawTree(depth, endPos, direction, splitChance, lenght, endRadius);
}
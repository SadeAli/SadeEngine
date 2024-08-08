#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#include "defines.h"
// #include "utilities/Vector2.h"

// fluid simulation
// even though all of the individual particles movement can't be predicted
// their average movement can be predicted efficiently

typedef struct Ruler
{
    Vector2 p1;
    Vector2 p2;
    bool isMeasuring : 1;
} Ruler;

typedef struct MovingPoint
{
    Vector2 position;
    Vector2 velocity;
} MovingPoint;

typedef struct Scene
{
    // triangulated mesh from point cloud
    RenderTexture2D viewport;
    int pointCount;
    Vector2 *pointCloud;
    Vector2 *pointCloudSource;
    Vector2 *pointCloudTarget;
    float dampProgress;
} Scene;

Scene initScene();
void update(Scene *scene);
void exitScene(Scene *scene);

int main()
{
    const int windowWidth = 1920;
    const int windowHeight = 1080;
    const char *windowTitle = "progen";

    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(10);

    bool isPaused = true;

    Scene scene = initScene();
    update(&scene);

    Ruler ruler = {};

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ruler.p1 = GetMousePosition();
            ruler.isMeasuring = true;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            ruler.isMeasuring = false;
        }

        if (!isPaused)
        {
            update(&scene);
        }

        if (IsKeyPressed(KEY_P))
        {
            isPaused = !isPaused;
            DrawText(isPaused ? "Pause" : "Play", 10, 10, 20, WHITE);
        }

        if (IsKeyPressed(KEY_F5))
        {
            update(&scene);
        }

        BeginDrawing();
        {
            ClearBackground(GRAY);

            const Texture viewportTexture = scene.viewport.texture;

            DrawTexture(viewportTexture,
                        (windowWidth - viewportTexture.width) / 2,
                        (windowHeight - viewportTexture.height) / 2, WHITE);

            if (ruler.isMeasuring)
            {
                ruler.p2 = GetMousePosition();

                char buffer[30];
                const Vector2 p1 = ruler.p1;
                const Vector2 p2 = ruler.p2;

                sprintf(buffer, "%20.5f", Vector2Distance(ruler.p1, ruler.p2));

                DrawText(buffer, (p1.x + p2.x) / 2, (p1.y + p2.y) / 2, 10,
                         GREEN);
                DrawLine(p1.x, p1.y, p2.x, p2.y, GREEN);

                DrawCircle(p1.x, p1.y, 5, GREEN);
                DrawCircle(p2.x, p2.y, 5, GREEN);
            }

            DrawFPS(10, 20);
        }
        EndDrawing();
    }
    exitScene(&scene);

    CloseWindow();
}

Scene initScene()
{
    // triangulated mesh from point cloud
    int pointCount = 20;
    Vector2 *pointCloud = MemAlloc(pointCount * sizeof(Vector2));
    Vector2 *pointCloudSource = MemAlloc(pointCount * sizeof(Vector2));
    Vector2 *pointCloudTarget = MemAlloc(pointCount * sizeof(Vector2));

    RenderTexture2D viewport = LoadRenderTexture(1000, 1000);

    return (Scene){
        .pointCount = pointCount,
        .pointCloud = pointCloud,
        .pointCloudSource = pointCloudSource,
        .pointCloudTarget = pointCloudTarget,
        .viewport = viewport,
    };
};

void update(Scene *scene)
{
    int pointCount = scene->pointCount;
    Vector2 *const pointCloud = scene->pointCloudSource;
    Vector2 *const pointCloud2 = scene->pointCloudTarget;
    RenderTexture const texture = scene->viewport;

    // generate points
    for (int i = 0; i < pointCount; i++)
    {
        pointCloud[i].x = GetRandomValue(0, texture.texture.width);
        pointCloud[i].y = GetRandomValue(0, texture.texture.height);
    }

    // if 2 points are too close delete one
    for (int i = 0; i < pointCount; i++)
    {
        for (int j = i + 1; j < pointCount; j++)
        {
            if (200.0f > Vector2Distance(pointCloud[i], pointCloud[j]))
            {
                pointCloud[j] = pointCloud[pointCount - 1];
                pointCount -= 1;
                j -= 1;
            }
        }
    }

    BeginTextureMode(texture);
    {
        ClearBackground(BLACK);
        for (int i = 0; i < pointCount; i++)
        {
            for (int j = i + 1; j < pointCount; j++)
            {
                float pointDistance =
                    Vector2Distance(pointCloud[i], pointCloud[j]);

                float maxDistancce =
                    1 + Vector2Distance((Vector2){0, 0},
                                        (Vector2){texture.texture.height,
                                                  texture.texture.width});

                u8 alpha = (1 - (pointDistance + 1) / maxDistancce) * 255;

                DrawLineV(pointCloud[i], pointCloud[j],
                          (Color){alpha, 0, 255 - alpha, alpha / 2 + 120});
            }
        }

        for (int i = 0; i < pointCount; i++)
        {
            DrawCircle(pointCloud[i].x, pointCloud[i].y, 5, ORANGE);
        }
    }
    EndTextureMode();
}

void exitScene(Scene *scene)
{
    MemFree(scene->pointCloud);
    MemFree(scene->pointCloudSource);
    MemFree(scene->pointCloudTarget);
    UnloadRenderTexture(scene->viewport);
}
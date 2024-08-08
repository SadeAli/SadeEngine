#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#include "defines.h"
#include "utilities/Vector2.h"

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
    RenderTexture viewport;
} Scene;

Scene initScene();
void update(Scene *scene);
void exitScene(Scene *scene);

Scene initScene()
{
    const int layerCount = 5;

    // calculate vertex count for a hexagonal chunk
    int vertexCount = 1;
    int verticesInNextLayer = 6;
    for (int i = 0; i < layerCount; i++)
    {
        vertexCount += verticesInNextLayer;
        verticesInNextLayer += 6;
    }

    // calculate triangle count for a hexagonal grid
    int triangleCount = 0;
    int trianglesInNextLayer = 6;
    for (int i = 0; i < layerCount; i++)
    {
        triangleCount += trianglesInNextLayer;
        trianglesInNextLayer *= 3;
    }

    // index count
    int indexCount = triangleCount * 3;

    Vector2 *vertices = MemAlloc(sizeof(Vector2) * vertexCount);
    Vector2 *indices = MemAlloc(sizeof(Vector2) * indexCount);

    // place vertices
    vertices[0] = (Vector2){0, 0};
    int placedVertices = 1;

    verticesInNextLayer = 6;
    for (int i = 0; i < layerCount; i++)
    {
        Vector2 pointPos = {.x = 0, .y = 50 * (i + 1)};
        float anglePerSegment = 2 * PI / verticesInNextLayer;
        for (int j = 0; j < verticesInNextLayer; j++)
        {
            vertices[placedVertices] = pointPos;
            pointPos = Vector2Rotate(pointPos, anglePerSegment);

            placedVertices += 1;
        }
        verticesInNextLayer += 6;
    }

    RenderTexture viewport = LoadRenderTexture(1000, 1000);
    BeginTextureMode(viewport);
    {
        for (int i = 0; i < vertexCount; i++)
        {
            DrawCircle(vertices[i].x + (viewport.texture.width / 2),
                       vertices[i].y + (viewport.texture.height / 2), 5, WHITE);
        }
    }
    EndTextureMode();

    return (Scene){.viewport = viewport};
};

void update(Scene *scene) {}

void exitScene(Scene *scene) {}

int main()
{
    const int windowWidth = 1920;
    const int windowHeight = 1080;
    const char *windowTitle = "progen";

    InitWindow(windowWidth, windowHeight, windowTitle);
    SetTargetFPS(60);

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
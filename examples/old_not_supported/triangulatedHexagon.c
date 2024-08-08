#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>

#include "defines.h"
#include "utilities/Ruler.h"
#include "utilities/Vector2.h"

// fluid simulation
// even though all of the individual particles movement can't be predicted
// their average movement can be predicted efficiently

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
    const int layerCount = 6;
    const float distancebetweenLayers = 50.0f;

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
        trianglesInNextLayer += 2 * 6;
    }

    // index count
    int indexCount = triangleCount * 3;

    Vector2 *vertices = MemAlloc(sizeof(Vector2) * vertexCount);
    int *indices = MemAlloc(sizeof(int) * indexCount);

    // place vertices
    // ----------------------------------------------------------------------
    vertices[0] = (Vector2){0, 0};
    int placedVertices = 1;

    while (!IsKeyPressed(KEY_ENTER))
    {
        PollInputEvents();
    }
    PollInputEvents();

    bool skip = false;

    SetTargetFPS(60);
    verticesInNextLayer = 6;
    // for each layer
    for (int i = 0; i < layerCount; i++)
    {
        // travel trough each edges
        for (int j = 0; j < 6; j++)
        {
            // select an edge
            const Vector2 startingEdge = Vector2Rotate(
                (Vector2){.x = 0, .y = distancebetweenLayers * (i + 1)},
                PI * j / 3);
            Vector2 target = startingEdge;
            Vector2 step = Vector2Scale(
                Vector2Normalize(Vector2Rotate(startingEdge, PI * 2 / 3)),
                distancebetweenLayers);

            // travel trough the edge
            for (int k = 0; k < (i + 1); k++)
            {
                vertices[placedVertices] = target;
                target = Vector2Add(target, step);
                placedVertices += 1;

                if (IsKeyPressed(KEY_ENTER))
                {
                    skip = true;
                }

                if (!skip)
                {
                    BeginDrawing();
                    {
                        for (int i = 0; i < placedVertices; i++)
                        {
                            DrawCircle(vertices[i].x + 960, vertices[i].y + 540,
                                       5, WHITE);
                        }
                    }
                    EndDrawing();
                }
            }
        }
    }

    // create triangles
    // -------------------------------------------------------------------------------------------
    // clockwise
    int passedVertices = 1;
    int placedIndices = 0;
    int up = 1;
    int down = 0;
    for (int i = 0; i < layerCount; i++)
    {
        verticesInNextLayer = (i + 1) * 6;

        for (int j = 0; j < 6; j++)
        {
            PollInputEvents();

            // first triangle
            indices[placedIndices++] = up++;
            indices[placedIndices++] = down++;
            indices[placedIndices++] = up++;

            // generate the rest using strip method
            for (int k = 0; k < i; k++)
            {
                // strip
                indices[placedIndices] = indices[placedIndices - 1];
                indices[placedIndices + 1] = indices[placedIndices - 2];

                // new index
                indices[placedIndices + 2] = down++;
                placedIndices += 3;

                // strip
                indices[placedIndices] = indices[placedIndices - 3];
                indices[placedIndices + 1] = indices[placedIndices - 1];

                // new index
                indices[placedIndices + 2] = up++;
                placedIndices += 3;
            }

            up--;
            down--;
        }

        down = passedVertices;

        if (i != 0)
        {
            indices[placedIndices - 4] -= i * 6;
            indices[placedIndices - 2] -= i * 6;
        }
        indices[placedIndices - 1] -= (i + 1) * 6;

        passedVertices += verticesInNextLayer;
    }

    SetTargetFPS(60);
    PollInputEvents();

    bool isPaused = false;

    for (int i = 0; i < indexCount; i += 3)
    {
        while (isPaused)
        {
            PollInputEvents();

            if (IsKeyPressed(KEY_P))
            {
                isPaused = !isPaused;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                break;
            }
        }

        if (IsKeyPressed(KEY_P))
        {
            isPaused = !isPaused;
        }

        BeginDrawing();
        {
            DrawLine(vertices[indices[i]].x + 960, vertices[indices[i]].y + 540,
                     vertices[indices[i + 1]].x + 960,
                     vertices[indices[i + 1]].y + 540, RED);
            DrawLine(vertices[indices[i + 1]].x + 960,
                     vertices[indices[i + 1]].y + 540,
                     vertices[indices[i + 2]].x + 960,
                     vertices[indices[i + 2]].y + 540, GREEN);
            DrawLine(vertices[indices[i + 2]].x + 960,
                     vertices[indices[i + 2]].y + 540,
                     vertices[indices[i]].x + 960, vertices[indices[i]].y + 540,
                     BLUE);
            DrawCircle(vertices[indices[i + 0]].x + 960,
                       vertices[indices[i + 0]].y + 540, 10, RED);
            DrawCircle(vertices[indices[i + 1]].x + 960,
                       vertices[indices[i + 1]].y + 540, 10, GREEN);
            DrawCircle(vertices[indices[i + 2]].x + 960,
                       vertices[indices[i + 2]].y + 540, 10, BLUE);
        }
        EndDrawing();
    }

    PollInputEvents();
    while (!IsKeyPressed(KEY_ENTER))
    {
        PollInputEvents();
    }
    PollInputEvents();

    SetTargetFPS(10);

    RenderTexture viewport = LoadRenderTexture(1000, 1000);

    BeginTextureMode(viewport);
    {
        Vector2 drawOffset = (Vector2){((float)viewport.texture.width / 2),
                                       ((float)viewport.texture.height / 2)};

        for (int i = 0; i < indexCount; i += 1)
        {
            DrawLine(vertices[indices[i]].x + drawOffset.x,
                     vertices[indices[i]].y + drawOffset.y,
                     vertices[indices[i + 1]].x + drawOffset.x,
                     vertices[indices[i + 1]].y + drawOffset.y, RED);
            DrawLine(vertices[indices[i + 1]].x + drawOffset.x,
                     vertices[indices[i + 1]].y + drawOffset.y,
                     vertices[indices[i + 2]].x + drawOffset.x,
                     vertices[indices[i + 2]].y + drawOffset.y, GREEN);
            DrawLine(vertices[indices[i + 2]].x + drawOffset.x,
                     vertices[indices[i + 2]].y + drawOffset.y,
                     vertices[indices[i]].x + drawOffset.x,
                     vertices[indices[i]].y + drawOffset.y, BLUE);
        }

        for (int i = 0; i < vertexCount; i++)
        {
            Vector2 drawPos = {.x = vertices[i].x + drawOffset.x,
                               .y = vertices[i].y + drawOffset.y};

            DrawCircle(drawPos.x, drawPos.y, 5, WHITE);

            drawPos.y += 10;
            DrawText(TextFormat("%d", i), drawPos.x, drawPos.y, 5, GREEN);
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
        updateRuler(&ruler);

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
            ClearBackground(BLACK);

            const Texture viewportTexture = scene.viewport.texture;

            DrawTextureRec(
                viewportTexture,
                (Rectangle){0, 0, viewportTexture.width,
                            -viewportTexture.height},
                (Vector2){(float)(windowWidth - viewportTexture.width) / 2,
                          (float)(windowHeight - viewportTexture.height) / 2},
                WHITE);

            drawRuler(&ruler);

            DrawFPS(10, 20);
        }
        EndDrawing();
    }

    exitScene(&scene);

    CloseWindow();
}
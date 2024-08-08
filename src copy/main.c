#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

#include "3D/CameraDefaults.h"
#include "utilities/Ruler.h"
#include <raylib.h>
#include <stdio.h>

typedef struct GameState
{
    bool isPaused : 1;
} GameState;

typedef struct Scene
{
    RenderTexture viewport;
    GameState gameState;
    Ruler ruler;
    Camera3D camera;
    Ray ray;
} Scene;

typedef struct WindowSettings
{
    int width;
    int height;
    char *title;
    int fps;
} WindowSettings;

void initWindow(WindowSettings);
void closeWindow();

Scene intializeScene();
void updateScene(Scene *s);
void exitScene(Scene *scene);

Scene intializeScene()
{
    DisableCursor();
    HideCursor();

    int dataSize = sizeof(Scene);
    Scene *loaded = (Scene *)LoadFileData("data/scene.dat", &dataSize);
    if (loaded)
    {
        loaded->viewport = LoadRenderTexture(1920, 1080);
        Scene s = *loaded;
        MemFree(loaded);
        return s;
    }

    GameState gs = {
        .isPaused = 0,
    };

    Camera3D camera = createFreeCamera();
    camera.position = (Vector3){10, 2, 0};

    return (Scene){.viewport = LoadRenderTexture(1920, 1080),
                   .camera = camera,
                   .ruler = {},
                   .gameState = gs,
                   .ray = {}};
}

void updateScene(Scene *s)
{
    Scene scene = *s;
    GameState game = scene.gameState;

    Ruler ruler = scene.ruler;
    Camera camera = scene.camera;

    updateRuler(&ruler);

    if (IsKeyPressed(KEY_P))
    {
        game.isPaused = !game.isPaused;
    }

    if (IsKeyPressed(KEY_F5))
    {
        updateScene(&scene);
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        scene.ray = (Ray){
            camera.position,
            Vector3Normalize(Vector3Subtract(camera.target, camera.position))};
    }

    UpdateCamera(&camera, CAMERA_FIRST_PERSON);

    BeginDrawing();
    {
        ClearBackground(BLACK);

        // 3D rendering
        BeginMode3D(camera);
        {
            DrawGrid(10, 10);

            DrawCube((Vector3){0, 0, 0}, 1, 1, 1, ORANGE);

            DrawRay(scene.ray, RED);
        }
        EndMode3D();

        // external texture rendering
        const Texture viewportTexture = scene.viewport.texture;
        DrawTextureRec(
            viewportTexture,
            (Rectangle){0, 0, viewportTexture.width, -viewportTexture.height},
            (Vector2){(float)(1920 - viewportTexture.width) / 2,
                      (float)(1080 - viewportTexture.height) / 2},
            WHITE);

        // on screen surface rendering
        drawRuler(&ruler);

        if (game.isPaused)
        {
            DrawText("Paused", 10, 10, 30, WHITE);
        }

        DrawFPS(10, 45);
    }
    EndDrawing();

    scene.camera = camera;
    scene.ruler = ruler;
    scene.gameState = game;
    *s = scene;
}

void exitScene(Scene *scene)
{
    if (!SaveFileData("data/scene.dat", scene, sizeof(Scene)))
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i",
                 "data/scene.dat", scene);

        printf("fail\n");
    }

    UnloadRenderTexture(scene->viewport);
}

int main()
{
    WindowSettings window = (WindowSettings){
        .width = 1920,
        .height = 1080,
        .title = "myLeaf",
        .fps = 60,
    };

    initWindow(window);

    Scene scene = intializeScene();

    while (!WindowShouldClose())
    {
        updateScene(&scene);
    }

    exitScene(&scene);

    CloseWindow();
}

void initWindow(WindowSettings ws)
{
    InitWindow(ws.width, ws.height, ws.title);
    SetTargetFPS(ws.fps);
}
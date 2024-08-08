#include <assert.h>
#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

#include "defaultBehaviour.h"

#include <raymath.h>

typedef struct TrailLine2D_t
{

} TrailLine2D;

typedef struct PhysicsMotion_t
{
    Vector2 velocity;
    Vector2 accel;
} PhysicsMotion;

typedef struct AnimationCurve
{
    int steps;
    Vector2 *values;
} AnimationCurve;

AnimationCurve getAC(Vector2 p[static 4], int steps)
{

    AnimationCurve ac = {
        .steps = steps,
        .values = malloc(sizeof(Vector2) * ac.steps),
    };

    for (int i = 0; i < ac.steps; i++)
    {
        float t = (float)i / ac.steps;
        ac.values[i] = GetSplinePointBezierCubic(p[0], p[1], p[2], p[3], t);
    }

    return ac;
}

void updateStepInterpolation(Vector2 *pos, Vector2 target, float stepSize)
{
    float frameTime = GetFrameTime();

    if (Vector2Distance(target, *pos) < (stepSize * frameTime))
    {
        *pos = target;
    }

    const Vector2 direction = Vector2Normalize(Vector2Subtract(target, *pos));
    const Vector2 velocity = Vector2Scale(direction, stepSize * frameTime);
    *pos = Vector2Add(*pos, velocity);
}

Vector2 easeOutVec2(Vector2 start, Vector2 end, float t)
{
    float progression = t * GetFrameTime();

    return (Vector2){.x = start.x * (1 - progression) + end.x * progression,
                     .y = start.y * (1 - progression) + end.y * progression};
}

void customPhysicsBased(Vector2 *pos, PhysicsMotion *motion, Vector2 target, Vector2 targetVelocity)
{
    // x input y output
    // y + k[1]*y' + k[2]y" = x + k[3]*x'

    const Vector2 dx = Vector2Scale(targetVelocity, 1 / GetFrameTime());

    float zheta = 0.4;
    float f = 2;
    float r = -1.5;

    float k1 = zheta / (PI * f);
    float k2 = 1 / ((2 * PI * f) * (2 * PI * f));
    float k3 = r * zheta / (2 * PI * f);

    const Vector2 y = Vector2Add(*pos, Vector2Scale(motion->velocity, GetFrameTime()));

    const Vector2 x = target;
    const Vector2 k3_dx = Vector2Scale(dx, k3);

    const Vector2 k1_old_dy = Vector2Scale(motion->velocity, k1);

    const Vector2 tmp = Vector2Subtract(Vector2Subtract(Vector2Add(x, k3_dx), y), k1_old_dy);
    const Vector2 dy = Vector2Add(motion->velocity, Vector2Scale(tmp, GetFrameTime() / k2));

    *pos = y;
    motion->velocity = dy;
}

int main(void)
{
#define MAX 1000
    Vector2 pointArray[MAX] = {0};
    Vector2 pointArray2[MAX] = {0};
    int pointIndex = {0};

    int fps = 120;

#pragma clang diagnostic ignored "-Winitializer-overrides"
    INIT_WINDOW(.fps = fps);

    Vector2 targetPos = GetMousePosition();
    GetMouseDelta();

    Vector2 stepInterpolation = targetPos;
    Vector2 easeInt = targetPos;
    Vector2 pba = targetPos;

    PhysicsMotion motion = {0};

    while (!WindowShouldClose())
    {
        targetPos = GetMousePosition();
        Vector2 targetDelta = GetMouseDelta();

        if (IsKeyPressed(KEY_F5))
        {
            targetDelta = Vector2Zero();
            targetPos = GetMousePosition();
            pba = targetPos;
            motion = (PhysicsMotion){0};
        }

        if (IsKeyDown(KEY_MINUS))
        {
            fps--;
            SetTargetFPS(fps);
        }

        if (IsKeyDown(KEY_KP_ADD))
        {
            fps++;
            SetTargetFPS(fps);
        }

        updateStepInterpolation(&stepInterpolation, targetPos, MAX);
        easeInt = easeOutVec2(easeInt, targetPos, 5);
        customPhysicsBased(&pba, &motion, targetPos, targetDelta);

        pointArray[pointIndex] = pba;
        pointArray2[pointIndex] = GetMousePosition();
        pointIndex = (pointIndex + 1) % MAX;

        BeginDrawing();
        {
            ClearBackground(DARKBLUE);

            DrawCircleV(targetPos, 20, YELLOW);
            // DrawCircleV(stepInterpolation, 20, WHITE);
            // DrawCircleV(easeInt, 20, RED);
            DrawCircleV(pba, 20, GREEN);

            DrawLineStrip(pointArray, pointIndex, GREEN);
            if (pointIndex != 0)
            {
                DrawLineV(pointArray[0], pointArray[MAX - 1], GREEN);
            }
            DrawLineStrip(pointArray + pointIndex, MAX - pointIndex, GREEN);

            DrawLineStrip(pointArray2, pointIndex, YELLOW);
            if (pointIndex != 0)
            {
                DrawLineV(pointArray2[0], pointArray2[MAX - 1], YELLOW);
            }
            DrawLineStrip(pointArray2 + pointIndex, MAX - pointIndex, YELLOW);
        }
        EndDrawing();
    }

    return 0;
}
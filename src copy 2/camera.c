#include <raylib.h>
#include <raymath.h>

void updateCameraCustom(Camera3D *camera, float moveSpeed, float rotationSpeed,
                        float rollSpeed)
{
    if (IsKeyDown(KEY_R))
    {
        camera->up = (Vector3){0, 1, 0};
    }

    Vector3 movementInput = {
        (IsKeyDown(KEY_D) - IsKeyDown(KEY_A)),
        (IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)),
        (IsKeyDown(KEY_W) - IsKeyDown(KEY_S)),
    };

    float roll = (IsKeyDown(KEY_E) - IsKeyDown(KEY_Q));

    Vector3 forward =
        Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    Vector3 up = camera->up;

    Vector3 movement = Vector3Scale(forward, movementInput.z);
    movement =
        Vector3Add(movement, Vector3Scale(Vector3CrossProduct(forward, up),
                                          movementInput.x));
    movement = Vector3Add(movement, Vector3Scale(up, movementInput.y));
    movement = Vector3Scale(movement, moveSpeed);

    // up direction
    up = Vector3RotateByAxisAngle(camera->up, Vector3CrossProduct(up, forward),
                                  GetMouseDelta().y * rotationSpeed);
    up = Vector3RotateByAxisAngle(up, forward, roll * rollSpeed);

    // target direction
    forward = Vector3RotateByAxisAngle(forward, camera->up,
                                       -GetMouseDelta().x * rotationSpeed);
    forward =
        Vector3RotateByAxisAngle(forward, Vector3CrossProduct(up, forward),
                                 GetMouseDelta().y * rotationSpeed);

    camera->position = Vector3Add(camera->position, movement);
    camera->up = up;
    camera->target = Vector3Add(camera->position, forward);
}
#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

typedef unsigned int u32;

// fluid simulation
// even though all of the individual particles movement can't be predicted
// their average movement can be predicted efficiently

typedef struct Cell
{
    u32 neighbour;
    bool value;
} Cell;

int main()
{
    const u32 gridWidth = 400, gridHeight = 400;
    const u32 gridScale = 4;

    InitWindow(gridWidth * gridScale, gridHeight * gridScale, "Game of Life");

    SetTargetFPS(10);

    Cell(*grid)[gridHeight] = MemAlloc(sizeof(Cell) * gridWidth * gridHeight);

    bool isPaused = false;

    // init grid with 0
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            grid[x][y].value = 1;
            grid[x][y].neighbour = 0;
        }
    }


    while (!WindowShouldClose())
    {
        
        if (IsKeyPressed(KEY_P))
        {
            isPaused = !isPaused;
            DrawText(isPaused? "Pause" : "Play", 10, 10, 20, WHITE);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();

            u32 x = (u32)(mousePos.x / gridScale) % gridWidth;
            u32 y = (u32)(mousePos.y / gridScale) % gridHeight;

            grid[x][y].value = 1 - grid[x][y].value;
        }

        if (!isPaused)
        {
            for (int y = 0; y < gridHeight; y++)
            {
                for (int x = 0; x < gridWidth; x++)
                {
                    if (grid[x][y].value != 0)
                    {
                        for (int i = -1; i <= 1; i++)
                        {
                            for (int j = -1; j <= 1; j++)
                            {
                                const u32 a = x + i;
                                const u32 b = y + j;

                                if (a < gridWidth && a >= 0 && b < gridHeight &&
                                    b >= 0)
                                {
                                    grid[a][b].neighbour += 1;
                                }
                            }
                        }
                        grid[x][y].neighbour -= 1;
                    }
                }
            }
        }

        if (!isPaused)
        {
            for (int y = 0; y < gridHeight; y++)
            {
                for (int x = 0; x < gridWidth; x++)
                {
                    switch (grid[x][y].neighbour)
                    {
                    case 0:
                    case 1:
                        grid[x][y].value = 0;
                        break;
                    case 2:
                        break;
                    case 3:
                        grid[x][y].value = 1;
                        break;
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        grid[x][y].value = 0;
                        break;
                    default:
                        break;
                    }

                    grid[x][y].neighbour = 0;
                }
            }
        }

        BeginDrawing();
        {
            ClearBackground(BLACK);
            
            for (int y = 0; y < gridHeight; y++)
            {
                for (int x = 0; x < gridWidth; x++)
                {
                    if (grid[x][y].value != 0)
                    {
                        DrawRectangle(x * gridScale + (gridScale / 2),
                                      y * gridScale + (gridScale / 2),
                                      gridScale, gridScale, ORANGE);
                    }
                }
            }
        }
        EndDrawing();
    }

    MemFree(grid);

    CloseWindow();
}
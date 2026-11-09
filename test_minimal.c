#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "test");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(300, 250, 200, 100, RED);
        DrawText("If you see this, raylib works!", 150, 400, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
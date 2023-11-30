#include "raylib.h"

constexpr auto SCREEN_WIDTH  = 1600;
constexpr auto SCREEN_HEIGHT = 900;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pataprout et bilboquet");
    SetTargetFPS(30);

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);

        const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
        const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;
        DrawTexture(texture, texture_x, texture_y, WHITE);

        const char* text = "EDGAR";
        const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, BLACK);

        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow();
    return 0;
}

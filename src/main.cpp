#include "raylib.h"

constexpr auto SCREEN_WIDTH  = 1600;
constexpr auto SCREEN_HEIGHT = 900;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pataprout et bilboquet");
    SetTargetFPS(30);

    Texture2D sub_texture = LoadTexture(ASSETS_PATH"sub_asset.png");

    int sub_x = SCREEN_WIDTH / 2 - sub_texture.width / 2;
    int sub_y = SCREEN_HEIGHT / 2 - sub_texture.height / 2;
    bool direction = true;
    int speed = 0;
    int max_speed = 20;
    int acceleration = 1;

    bool is_near(int speed){

    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if(sub_x <= 0 && !direction){
            direction = true;
            speed = 0;
        }
        if(sub_x >= SCREEN_WIDTH-sub_texture.width && direction){
            direction = false;
            speed = 0;
        }

        speed += acceleration;

        if(speed > max_speed){speed = max_speed;}
        sub_x += direction ? speed:-speed;

        Vector2 coordinates;
        coordinates.x = (float)sub_x;
        coordinates.y = (float)sub_y;

        DrawTextureEx(sub_texture, coordinates, 0, 1, WHITE);

        const char* text = "PATAPROUT";
        const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, sub_y + sub_texture.height + text_size.y + 10, 20, BLACK);

        EndDrawing();
    }

    UnloadTexture(sub_texture);

    CloseWindow();
    return 0;
}

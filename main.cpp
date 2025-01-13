#include <raylib.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <algorithm>

#define LO(a, b) (a < b) ? a : b
#define HI(a, b) (a > b) ? a : b

int main(int argc, char* argv[]) {
    const int RWIDTH = (argc > 1) ? atoi(argv[1]) : 128;
    const int RHEIGHT = (argc > 2) ? atoi(argv[2]) : 128;
    int windowWidth = 600, windowHeight = windowWidth;
    bool paused = false;
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(windowWidth, windowHeight, "LoTrix");
	SetTargetFPS(60);
	RenderTexture2D surface = LoadRenderTexture(RWIDTH, RHEIGHT);
	SetTextureFilter(surface.texture, TEXTURE_FILTER_POINT);
    Font font = LoadFont("font.png");
    srand(time(NULL));

    struct Grid { std::vector<int>x, y; }; struct Grid grid;
    for (int i = 0; i < RWIDTH / 8; i++) {
        grid.x.push_back(i * 8);
        for (int j = 0; j < RHEIGHT / 8; j++) {
            grid.y.push_back(j * 8);
        }
    }

    const int MAX = (argc > 3) ? atoi(argv[3]) : HI(RWIDTH / 4, RHEIGHT / 4);

    struct C {
        float x, y, speed, delay;
        char type[2];
        int tail, r, g, b;
    }; struct C c[MAX];

    for (int i = 0; i < MAX; i++) {
        c[i].x = grid.x[rand() % RWIDTH / 8];
        c[i].y = grid.y[rand() % RHEIGHT / 8] - RHEIGHT;
        c[i].speed = 8 + rand() % 32;
        c[i].delay = 1;
        c[i].type[0] = { 32 + rand() % 95 };
        c[i].type[1] = '\0';
        c[i].tail = 2 + rand() % 8;

        if (argc > 4 && std::string(argv[4]) == "true") {
            c[i].r = rand() % 256;
            c[i].g = rand() % 256;
            c[i].b = rand() % 256;
        } else {
            c[i].r = 0;
            c[i].g = 255;
            c[i].b = 0;
        }
    }

	while (!WindowShouldClose()) {
        float surfaceScale = LO(float(GetScreenWidth()) / RWIDTH, float(GetScreenHeight()) / RHEIGHT);
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)) { paused = (!paused) ? true : false; }
        if (IsKeyPressed(KEY_F)) { ToggleBorderlessWindowed(); }
        HideCursor();

        BeginTextureMode(surface);
                if (!paused) {
                    ClearBackground(Color { 0, 0, 0, 25 });

                    for (int i = 0; i < MAX; i++) {
                        DrawRectangle(c[i].x, c[i].y, 7, 7, Color { 0, 0, 0, 255 });
                        DrawRectangle(c[i].x, c[i].y, 7, 7, Color { c[i].r, c[i].g, c[i].b, 20 * c[i].tail });

                        for (int j = 0; j < c[i].tail; j++) {
                            DrawRectangle(c[i].x, c[i].y - 8 - j * 8, 7, 7, Color { 0, 0, 0, 255 });
                            DrawTextEx(font, c[i].type, (Vector2) { c[i].x, c[i].y - 8 - j * 8 }, 8, 0, Color { c[i].r, c[i].g, c[i].b, std::clamp(255 - j * 32, 0, 255) });
                        }

                        DrawTextEx(font, c[i].type, (Vector2) { c[i].x, c[i].y }, 8, 0, WHITE);
                        c[i].delay -= GetFrameTime() * c[i].speed;

                        if (c[i].delay <= 0) {
                            c[i].y += 8;
                            c[i].type[0] = { 32 + rand() % 95 };
                            c[i].delay = 1;
                        }

                        if (c[i].y - c[i].tail * 8 + 8 > RHEIGHT) {
                            c[i].x = grid.x[rand() % RWIDTH / 8];
                            c[i].y = grid.y[rand() % RHEIGHT / 8] - RHEIGHT;

                            if (argc > 4 && std::string(argv[4]) == "true") {
                                c[i].r = rand() % 256;
                                c[i].g = rand() % 256;
                                c[i].b = rand() % 256;
                            }
                        }
                    }
                }
        EndTextureMode();
        BeginDrawing();
            DrawTexturePro(
                surface.texture, (Rectangle) { 0, 0, float(surface.texture.width), float(-surface.texture.height) },
                (Rectangle) { (GetScreenWidth() - float(RWIDTH) * surfaceScale) / 2,
                (GetScreenHeight() - (float(RHEIGHT) * surfaceScale)) / 2,
                float(RWIDTH) * surfaceScale,
                float(RHEIGHT) * surfaceScale },
                (Vector2) { 0, 0 }, 0, WHITE
            );
        EndDrawing();
	}
	UnloadRenderTexture(surface);
	CloseWindow();
	return 0;
}

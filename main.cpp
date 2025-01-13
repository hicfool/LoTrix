#include <raylib.h>
#include <vector>
#include <random>
#include <string>
#include <algorithm>

#define LO(a, b) (a < b) ? a : b
#define HI(a, b) (a > b) ? a : b

int genRand(int min, int max, std::mt19937& gen) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

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
    std::random_device rd;
    std::mt19937 gen(rd());

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
        int r, g, b;
    }; struct C c[MAX];

    for (int i = 0; i < MAX; i++) {
        c[i].x = grid.x[genRand(0, RWIDTH / 8, gen)];
        c[i].y = grid.y[genRand(0, RHEIGHT / 8, gen)] - RHEIGHT;
        c[i].speed = genRand(8, 32, gen);
        c[i].delay = 1;
        c[i].type[0] = { genRand(32, 127, gen) };
        c[i].type[1] = '\0';

        if (argc > 4 && std::string(argv[4]) == "true") {
            c[i].r = genRand(50, 255, gen);
            c[i].g = genRand(50, 255, gen);
            c[i].b = genRand(50, 255, gen);
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
                        DrawRectangle(c[i].x, c[i].y - 8, 7, 7, Color { 0, 0, 0, 255 });
                        DrawRectangle(c[i].x, c[i].y, 7, 7, Color { c[i].r, c[i].g, c[i].b, 75 });
                        DrawTextEx(font, c[i].type, (Vector2) { c[i].x, c[i].y - 8 }, 8, 0, Color { c[i].r, c[i].g, c[i].b, 255 });
                        DrawTextEx(font, c[i].type, (Vector2) { c[i].x, c[i].y }, 8, 0, WHITE);

                        if (c[i].delay <= 0) {
                            c[i].y += 8;
                            c[i].type[0] = { genRand(32, 127, gen) };
                            c[i].delay = 1;
                        } else {
                            c[i].delay -= GetFrameTime() * c[i].speed;
                        }

                        if (c[i].y - 8 > RHEIGHT) {
                            c[i].x = grid.x[genRand(0, RWIDTH / 8, gen)];
                            c[i].y = grid.y[genRand(0, RHEIGHT / 8, gen)] - RHEIGHT;

                            if (argc > 4 && std::string(argv[4]) == "true") {
                                c[i].r = genRand(50, 255, gen);
                                c[i].g = genRand(50, 255, gen);
                                c[i].b = genRand(50, 255, gen);
                            }
                        }
                    }
                } else {
                    ClearBackground(Color { 0, 0, 0, 0 });
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

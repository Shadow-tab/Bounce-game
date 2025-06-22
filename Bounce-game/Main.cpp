#include "raylib.h"
#include "raymath.h"
const int screenWidth = 900;
const int screenHeight = 600;
const int brickrows = 5;
const int brickcols = 10;
const int brickwidth = 70;
const int brickheight = 20;
const int brickspacing = 10;
const int brickoffsety = 50;
int score = 0;

Rectangle bricks[brickrows][brickcols];
bool brickactive[brickrows][brickcols];
Rectangle paddle = { 350,550 ,100,20 };
Rectangle initialpaddle = { 350, 550, 100, 20 };

float paddlespeed = 8.0f;
int lives = 2;
bool isgameover = false;

Vector2 ballposition = { 400,300 };
Vector2 initialballposition = { 400,300 };
Vector2 ballspeed = { 4.0f, -4.0f };
float ballradius = 15;

int main() {
	InitWindow(screenWidth, screenHeight, "Bounce");
	InitAudioDevice();
	Sound hitsound = LoadSound("hit.wav");
	Sound misssound = LoadSound("miss.wav");
	Sound gameoversound = LoadSound("gameover.wav");
	SetTargetFPS(60);
	for (int row = 0;row < brickrows;row++) {
		for (int col = 0;col < brickcols;col++) {
			bricks[row][col] = {
				(float)(col * (brickwidth + brickspacing) + 35),
				(float)(row * (brickheight + brickspacing) + brickoffsety),
				(float)brickwidth,
				(float)brickheight
			};
			brickactive[row][col] = true;
		}
	}
	while (!WindowShouldClose()) {
		if (isgameover && IsKeyPressed(KEY_R)) {
			lives = 2;
			isgameover = false;
			score = 0;
			ballposition = initialballposition;
			paddle = initialpaddle;
			for (int row = 0;row < brickrows;row++) {
				for (int col = 0;col < brickcols;col++) {
					brickactive[row][col] = true;
				}
			}
		}
		
		if (!isgameover) {


			if (IsKeyDown(KEY_LEFT) && paddle.x > 0) {
				paddle.x -= paddlespeed;
			}
			if (IsKeyDown(KEY_RIGHT) && paddle.x + paddle.width < screenWidth) {
				paddle.x += paddlespeed;
			}
			ballposition.x += ballspeed.x;
			ballposition.y += ballspeed.y;
			if (ballposition.x - ballradius <= 0 || ballposition.x + ballradius >= screenWidth) {
				ballspeed.x *= -1;
			}
			if (ballposition.y - ballradius <= 0) {
				ballspeed.y *= -1;
			}
			if (CheckCollisionCircleRec(ballposition, ballradius, paddle) && ballspeed.y > 0) {
				ballspeed.y *= -1;
				float hitpos = (ballposition.x - paddle.x) / paddle.width - 0.5f;
				ballspeed.x = hitpos * 8.9f;
				PlaySound(hitsound);
			}
			for (int row = 0;row < brickrows;row++) {
				for (int col = 0;col < brickcols;col++) {
					if (brickactive[row][col] && CheckCollisionCircleRec(ballposition, ballradius, bricks[row][col])) {
						brickactive[row][col] = false;
						ballspeed.y *= -1;
						score += 10;
						PlaySound(hitsound);
						break;
					}
				}
			}
			for (int row = 0;row < brickrows;row++) {
				for (int col = 0;col < brickcols;col++) {
					if (brickactive[row][col]) {
						DrawRectangleRec(bricks[row][col], YELLOW);
					}
				}
			}
			if (ballposition.y - ballradius > screenHeight) {
				PlaySound(misssound);
				lives--;
				if (lives == 0) {
					isgameover = true;
					PlaySound(gameoversound);
				}
				else {
					ballposition = initialballposition;
					ballspeed = { 4.0f,-4.0f };
					paddle = initialpaddle;
				}
			}
		}

		BeginDrawing();
		ClearBackground(BLUE);
		DrawRectangleRec(paddle, GREEN);
		DrawCircleV(ballposition, ballradius, RED);
		DrawText(TextFormat("Lives: %d", lives),30,5,20, RED);
		DrawText(TextFormat("Score: %d", score), 750, 5, 20, BLACK);
		if (isgameover) {
			DrawText("GAME OVER!", screenWidth / 2 - 100, screenHeight / 2, 40, RED);
			DrawText("Press R to Restart", screenWidth / 2 - 110, screenHeight / 2 + 50, 20, GRAY);
		}
		EndDrawing();
	}
	UnloadSound(hitsound);
	UnloadSound(misssound);
	UnloadSound(gameoversound);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
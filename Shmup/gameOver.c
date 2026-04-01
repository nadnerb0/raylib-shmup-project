#include "raylib.h"
#include "utils.h"

typedef enum
{
	GAMEOVER,
	TRYAGAIN
}GameOver;

void CreateGameOverRecs(Rectangle gameOverRecs[])
{
	Rectangle Title =
	{
		0,
		0,
		300,
		50
	};
	Rectangle TryAgain =
	{
		0,
		0,
		200,
		20
	};

	Title.x = xRes / 2 - Title.width / 2;
	Title.y = yRes / 2 - 100;
	TryAgain.x = xRes / 2 - TryAgain.width / 2;
	TryAgain.y = yRes / 2 + 50;

	gameOverRecs[GAMEOVER] = Title;
	gameOverRecs[TRYAGAIN] = TryAgain;
}

void GameOverDrawing(Rectangle gameOverRecs[])
{
	// Title
	int gameOverFontSize = 50;
	DrawRectangleRec(gameOverRecs[GAMEOVER], WHITE);
	int gameOverWidth = MeasureText("YOU DED", gameOverFontSize);
	DrawText("YOU DED", gameOverRecs[GAMEOVER].x + gameOverRecs[GAMEOVER].width / 2 - gameOverWidth / 2, gameOverRecs[GAMEOVER].y, gameOverFontSize, BLACK);

	// Play
	int tryAgainFontSize = 20;
	DrawRectangleRec(gameOverRecs[TRYAGAIN], WHITE);
	int tryAgainWidth = MeasureText("TRY AGAIN", tryAgainFontSize);
	DrawText("TRY AGAIN", gameOverRecs[TRYAGAIN].x + gameOverRecs[TRYAGAIN].width / 2 - tryAgainWidth / 2, gameOverRecs[TRYAGAIN].y, tryAgainFontSize, BLACK);
}

GameState GameOverToMenu(Rectangle gameOverRecs[], Vector2 mousePos)
{
	if (CheckCollisionPointRec(mousePos, gameOverRecs[TRYAGAIN]))
	{
		SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			return MENU;
	}
	else
	{
		SetMouseCursor(MOUSE_CURSOR_ARROW);
	}

	return DEAD;
}
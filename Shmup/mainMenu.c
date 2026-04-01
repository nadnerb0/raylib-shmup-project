#include "raylib.h"
#include "utils.h"

void CreateMainMenu(Rectangle rectangles[])
{
    Rectangle Title = 
    { 
        0, 
        0, 
        500, 
        100 
    };
    Rectangle Play = 
    { 
        0, 
        0, 
        250, 
        50 
    };
    Rectangle Mute = 
    { 
        0,
        0, 
        50,
        50 
    };

    Title.x = xRes / 2 - Title.width / 2;
    Title.y = yRes / 2 - 150;

    Play.x = xRes / 2 - Play.width / 2;
    Play.y = yRes / 2;

    Mute.x = 0;
    Mute.y = 0;

    rectangles[PLAY] = Play;
    rectangles[TITLE] = Title;
    rectangles[MUTE] = Mute;
}

void DrawMainMenu(Rectangle rectangles[])
{
    // Title
    int titleFontSize = 100;
    DrawRectangleRec(rectangles[TITLE], WHITE);
    int titleWidth = MeasureText("SMHUP", titleFontSize);
    DrawText("SHMUP", rectangles[TITLE].x + rectangles[TITLE].width / 2 - titleWidth / 2, rectangles[TITLE].y, titleFontSize, BLACK);

    // Play
    int playFontSize = 50;
    DrawRectangleRec(rectangles[PLAY], WHITE);
    int playWidth = MeasureText("PLAY", playFontSize);
    DrawText("PLAY", rectangles[PLAY].x + rectangles[PLAY].width / 2 - playWidth / 2, rectangles[PLAY].y, playFontSize, BLACK);

    DrawRectangleRec(rectangles[MUTE], WHITE);
}

GameState UpdateGameState(Rectangle rectangles[], Vector2 mousePos)
{
    if (CheckCollisionPointRec(mousePos, rectangles[PLAY]))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return PLAYING;
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
    }

    return MENU;
}
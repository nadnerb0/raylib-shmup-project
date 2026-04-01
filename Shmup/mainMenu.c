#include "raylib.h"
#include "utils.h"

typedef enum
{
    TITLE,
    PLAY,
    MUTE
}MainMenuRecs;

void CreateMainMenu(Rectangle mainMenuRecs[])
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

    mainMenuRecs[PLAY] = Play;
    mainMenuRecs[TITLE] = Title;
    mainMenuRecs[MUTE] = Mute;
}

void DrawMainMenu(Rectangle mainMenuRecs[])
{
    // Title
    int titleFontSize = 100;
    DrawRectangleRec(mainMenuRecs[TITLE], WHITE);
    int titleWidth = MeasureText("SMHUP", titleFontSize);
    DrawText("SHMUP", mainMenuRecs[TITLE].x + mainMenuRecs[TITLE].width / 2 - titleWidth / 2, mainMenuRecs[TITLE].y, titleFontSize, BLACK);

    // Play
    int playFontSize = 50;
    DrawRectangleRec(mainMenuRecs[PLAY], WHITE);
    int playWidth = MeasureText("PLAY", playFontSize);
    DrawText("PLAY", mainMenuRecs[PLAY].x + mainMenuRecs[PLAY].width / 2 - playWidth / 2, mainMenuRecs[PLAY].y, playFontSize, BLACK);

    DrawRectangleRec(mainMenuRecs[MUTE], WHITE);
}

GameState MenuToPlaying(Rectangle mainMenuRecs[], Vector2 mousePos)
{
    if (CheckCollisionPointRec(mousePos, mainMenuRecs[PLAY]))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
            return PLAYING;
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_ARROW);
    }

    return MENU;
}
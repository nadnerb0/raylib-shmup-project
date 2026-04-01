#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "utils.h"

void CreateMainMenu(Rectangle mainMenuRecs[]);
void DrawMainMenu(Rectangle mainMenuRecs[]);
GameState MenuToPlaying(Rectangle mainMenuRecs[], Vector2 mousePos);

#endif // MAINMENU_H
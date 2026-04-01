#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include "utils.h"

void CreateMainMenu(Rectangle rectangles[]);
void DrawMainMenu(Rectangle rectangles[]);
GameState UpdateGameState(Rectangle rectangles[], Vector2 mousePos);

#endif // MAINMENU_H
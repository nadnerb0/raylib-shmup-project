#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "raylib.h"
#include "utils.h"

void CreateGameOverRecs(Rectangle gameOverRecs[]);
void GameOverDrawing(Rectangle gameOverRecs[]);
GameState GameOverToMenu(Rectangle gameOverRecs[], Vector2 mousePos);

#endif // GAMEOVER_H
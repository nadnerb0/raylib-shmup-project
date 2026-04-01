#ifndef DEBUG_H
#define DEBUG_H

#include "raylib.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

void DebugModeToggle(bool* debugMode);
void HitboxDrawing(Player player, Enemy enemies[], Bullet bullets[], Laser lasers[], int maxEnemies, int maxBullets, int maxLasers, Color hitboxColor);

#endif // DEBUG_H
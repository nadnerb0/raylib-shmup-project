#include "raylib.h"
#include "debug.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

void HitboxDrawing(Player player, Enemy enemies[], Bullet bullets[], Laser lasers[], int maxEnemies, int maxBullets, int maxLasers, Color hitboxColor)
{
    // Enemies
    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemies[i].active)
        {
            DrawRectangleRec(enemies[i].hitbox, hitboxColor);
        }
    }
    //Bullets
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            DrawRectangleRec(bullets[i].hitbox, hitboxColor);
        }
    }
    //Laser
    for (int i = 0; i < maxLasers; i++)
    {
        if (lasers[i].active)
        {
            DrawRectangleRec(lasers[i].hitbox, hitboxColor);
        }
    }
    // Player
    DrawCircleLines(player.position.x, player.position.y, player.hitboxRadius, GREEN);
}

void DebugModeToggle(bool* debugMode)
{
    if (IsKeyPressed(KEY_P)) *debugMode = !*debugMode;
}
#include "raylib.h"
#include "utils.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include <stdlib.h>

void BulletSpawning(Player* player, Bullet bullets[], Laser lasers[], Enemy enemies[], int maxBullets, int maxLasers, int maxEnemies, float dt, Sound playerPew, Textures textures)
{
    player->shootTimer += dt;
    // Player bullets
    if (IsKeyDown(KEY_SPACE) && (player->shootTimer >= player->fireRate))
    {
        bool skipThis = false;
        for (int i = 0; i < maxBullets; i++)
        {
            // Bullet 1
            if (!bullets[i].active && !skipThis)
            {
                bullets[i] = (Bullet)
                {
                    .yDir = -1.0f,
                    .texture = textures.bullet,
                    .speed = 1000.0f,
                    .width = 5.0f,
                    .height = 15.0f,
                    .offset = -12.0f,
                    .active = true,
                    .isPlayerBullet = true
                };
                bullets[i].position.x = player->position.x - bullets[i].width / 2 + bullets[i].offset;
                bullets[i].position.y = player->position.y - bullets[i].height / 2 - 5;
                bullets[i].hitbox = (Rectangle){ bullets[i].position.x, bullets[i].position.y, bullets[i].width, bullets[i].height };
                PlayFX(playerPew);
                skipThis = true;    // Doe in plaats van dit een bullet counter, dan kan multishot ook werken
                player->shootTimer = 0.0f;
                continue;
            }
            // Bullet 2
            if (!bullets[i].active)
            {
                bullets[i] = (Bullet)
                {
                    .yDir = -1.0f,
                    .texture = textures.bullet,
                    .speed = 1000.0f,
                    .width = 5.0f,
                    .height = 15.0f,
                    .offset = 13.0f,
                    .active = true,
                    .isPlayerBullet = true
                };
                bullets[i].position.x = player->position.x - bullets[i].width / 2 + bullets[i].offset;
                bullets[i].position.y = player->position.y - bullets[i].height / 2 - 5;
                bullets[i].hitbox = (Rectangle){ bullets[i].position.x,  bullets[i].position.y, bullets[i].width, bullets[i].height };
                break;
            }
        }
    }
    // Enemy bullets
    for (int i = 0; i < maxEnemies; i++)
    {
        enemies[i].shootTimer += dt;

        if (enemies[i].active && enemies[i].shootTimer >= enemies[i].fireRate)
        {
            if (enemies[i].enemyType == GRUNT)
            {
                for (int j = 0; j < maxBullets; j++)
                {
                    if (!bullets[j].active)
                    {
                        bullets[j] = (Bullet)
                        {
                            .position = enemies[i].position,
                            .texture = textures.enemyBullet,
                            .yDir = 1.0f,
                            .speed = 500.0f,
                            .width = 2.0f,
                            .height = 20.0f,
                            .offset = 0.0f,
                            .active = true,
                            .isPlayerBullet = false
                        };
                        bullets[j].position.x = enemies[i].position.x - bullets[j].width / 2;
                        bullets[j].position.y = enemies[i].position.y + 5;
                        bullets[j].hitbox = (Rectangle){ bullets[j].position.x, bullets[j].position.y, bullets[j].width, bullets[j].height };
                        enemies[i].shootTimer = 0.0f;
                        break;
                    }
                }
            }
            if (enemies[i].enemyType == SNIPER)
            {
                for (int j = 0; j < maxLasers; j++)
                {
                    if (!lasers[j].active)
                    {
                        lasers[j] = (Laser)
                        {
                            .texture = textures.laser,
                            .chargeTexture = textures.laserCharge,
                            .laserTimer = 0.0f,
                            .chargeTime = 0.5f,
                            .fireTime = 1.0f,
                            .width = 32.0f,
                            .height = 700.0f,
                            .offset = 0.0f,
                            .sniper = &enemies[i],
                            .isDamaging = false,
                            .active = true,
                            .isPlayerLaser = false
                        };
                        lasers[j].position.y = enemies[i].position.y;
                        lasers[j].position.x = enemies[i].position.x - lasers[j].width / 2;
                        enemies[i].shootTimer = 0.0f;
                        enemies[i].isSniping = true;
                        break;
                    }
                }
            }
        }
    }
}

void LaserUpdate(Laser lasers[], int maxLasers, float dt)
{
    for (int i = 0; i < maxLasers; i++)
    {
        if (lasers[i].active && !lasers[i].isPlayerLaser)
        {
            if (!lasers[i].sniper->active)
            {
                lasers[i] = (Laser){ 0 };
                continue;
            }

            lasers[i].laserTimer += dt;

            if (lasers[i].laserTimer < lasers[i].chargeTime)
            {
                lasers[i].isDamaging = false;
            }
            else
            {
                lasers[i].hitbox = (Rectangle){ lasers[i].position.x + lasers[i].width/6, lasers[i].position.y, lasers[i].width/3*2, lasers[i].height };
                lasers[i].isDamaging = true;
            }

            if (lasers[i].laserTimer > lasers[i].chargeTime + lasers[i].fireTime)
            {
                lasers[i].sniper->isSniping = false;
                lasers[i] = (Laser) { 0 };
                continue;
            }
        }
    }
}

void LaserDrawing(Laser lasers[], int maxLasers)
{
    for (int i = 0; i < maxLasers; i++)
    {
        if (lasers[i].active && !lasers[i].isPlayerLaser)
        {
            if (lasers[i].laserTimer < lasers[i].chargeTime)
            {
                DrawTexture(lasers[i].chargeTexture, lasers[i].position.x - lasers[i].width/2, lasers[i].position.y, WHITE);
                //DrawRectangle(lasers[i].position.x + lasers[i].width / 4, lasers[i].position.y, lasers[i].width / 2, lasers[i].height, WHITE);
            }
            else
            {
                DrawTexture(lasers[i].texture, lasers[i].position.x, lasers[i].position.y, WHITE);
                //DrawRectangle(lasers[i].position.x, lasers[i].position.y, lasers[i].width, lasers[i].height, RED);
            }
        }
    }
}

void BulletUpdate(Bullet bullets[], int maxBullets, float dt)
{
    for (int i = 0; i < maxBullets; i++)
    {
        // Player bullets
        if (bullets[i].active && bullets[i].isPlayerBullet)
        {
            bullets[i].position.y += bullets[i].yDir * bullets[i].speed * dt;
            bullets[i].hitbox.y += bullets[i].yDir * bullets[i].speed * dt;
            if (bullets[i].position.y < 0)
            {
                bullets[i].active = false;
            }
        }
        // Enemy Bullets
        if (bullets[i].active && !bullets[i].isPlayerBullet)
        {
            bullets[i].position.y += bullets[i].yDir * bullets[i].speed * dt;
            bullets[i].hitbox.y += bullets[i].yDir * bullets[i].speed * dt;
            if (bullets[i].position.y > yRes)
            {
                bullets[i].active = false;
            }
        }
    }
}

void BulletDrawing(Bullet bullets[], int maxBullets, Textures textures)
{
    // Drawing bullets
    for (int i = 0; i < maxBullets; i++)
    {
        // Player bullets
        if (bullets[i].active && bullets[i].isPlayerBullet)
        {
            DrawTexture(bullets[i].texture, bullets[i].position.x, bullets[i].position.y, WHITE);
        }
        // Enemy bullets
        else if (bullets[i].active)
        {
            DrawTexture(bullets[i].texture, bullets[i].position.x - bullets[i].width / 2, bullets[i].position.y, WHITE);
        }
    }
}
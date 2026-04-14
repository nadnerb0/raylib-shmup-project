#include "raylib.h"
#include "utils.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include <stdlib.h>

void SpawnPlayerBullet(Player* player, Bullet bullets[], int maxBullets, Sound playerPew, Texture2D texture, float dt)
{
    player->shootTimer += dt;
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
                    .texture = texture,
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
                    .texture = texture,
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
}

void SpawnEnemyBullet(Enemy* enemy, Bullet bullets[], int maxBullets, Sound enemyPew, Texture2D texture, float dt)
{
    if (enemy->active && (enemy->shootTimer >= enemy->fireRate))
    {
        if (enemy->enemyType == GRUNT)
        {
            for (int j = 0; j < maxBullets; j++)
            {
                if (!bullets[j].active)
                {
                    bullets[j] = (Bullet)
                    {
                        .position = enemy->position,
                        .texture = texture,
                        .yDir = 1.0f,
                        .speed = 500.0f,
                        .width = 2.0f,
                        .height = 20.0f,
                        .offset = 0.0f,
                        .active = true,
                        .isPlayerBullet = false
                    };
                    bullets[j].position.x = enemy->position.x - bullets[j].width / 2;
                    bullets[j].position.y = enemy->position.y;
                    bullets[j].hitbox = (Rectangle){ bullets[j].position.x, bullets[j].position.y, bullets[j].width, bullets[j].height };
                    enemy->shootTimer = 0.0f;
                    break;
                }
            }
        }
    }
}

void SpawnEnemyLaser(Enemy* enemy, Laser lasers[], int maxLasers, Sound enemyLaserSound, Texture2D textureLaser, Texture2D textureCharge, float dt)
{
    if (enemy->enemyType == SNIPER && !enemy->isSniping && (enemy->shootTimer >= enemy->fireRate))
    {
        for (int i = 0; i < maxLasers; i++)
        {
            if (!lasers[i].active)
            {
                lasers[i] = (Laser)
                {
                    .texture = textureLaser,
                    .chargeTexture = textureCharge,
                    .laserTimer = 0.0f,
                    .chargeTime = 0.5f,
                    .fireTime = 1.0f,
                    .width = 32.0f,
                    .height = 700.0f,
                    .offset = 0.0f,
                    .sniper = enemy,
                    .isDamaging = false,
                    .active = true,
                    .isPlayerLaser = false
                };
                lasers[i].position.y = enemy->position.y - 10;
                lasers[i].position.x = enemy->position.x - lasers[i].width / 2;
                enemy->isSniping = true;
                break;
            }
        }
    }
}

void LaserUpdate(Laser* laser, float dt)
{
    if (laser->active && !laser->isPlayerLaser)
    {
        if (!laser->sniper->active)
        {
            *laser = (Laser){ 0 };
            return;
        }

        laser->laserTimer += dt;

        if (laser->laserTimer < laser->chargeTime)
        {
            laser->isDamaging = false;
        }
        else
        {
            laser->hitbox = (Rectangle){ laser->position.x + laser->width/6, laser->position.y, laser->width/3*2, laser->height };
            laser->isDamaging = true;
        }

        if (laser->laserTimer > laser->chargeTime + laser->fireTime)
        {
            laser->sniper->isSniping = false;
            laser->sniper->shootTimer = 0.0f;
            *laser = (Laser) { 0 };
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
            }
            else
            {
                DrawTexture(lasers[i].texture, lasers[i].position.x, lasers[i].position.y, WHITE);
            }
        }
    }
}

void BulletUpdate(Bullet* bullet, float dt)
{
    bullet->position.y += bullet->yDir * bullet->speed * dt;
    bullet->hitbox.y += bullet->yDir * bullet->speed * dt;

    if (bullet->position.y < -50 || bullet->position.y > yRes)
    {
        bullet->active = false;
    }
}

void BulletDrawing(Bullet bullets[], int maxBullets)
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
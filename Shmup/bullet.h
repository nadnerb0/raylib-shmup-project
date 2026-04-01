#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"

// Probleem met bullets: ze kunnen meerdere keren 1 enemy hitten
// Getal toevoegen voor "pierce", kan maar bij hogere waarden blijft hij dezelfde enemy kunnen hitten, ipv erdoorheen gaan.

typedef struct
{
    Vector2 position;
    Texture2D texture;
    Rectangle hitbox;
    //Rectangle texture;
    float yDir;
    float speed;
    float width;
    float height;
    float offset;
    bool active;
    bool isPlayerBullet;
}Bullet;

typedef struct
{
    Vector2 position;
    Texture2D texture;
    Texture2D chargeTexture;
    Rectangle hitbox;
    //Rectangle texture;
    float laserTimer;
    float chargeTime;
    float fireTime;
    float width;
    float height;
    float offset;
    Enemy* sniper;
    bool isDamaging;
    bool active;
    bool isPlayerLaser;
}Laser;

void BulletSpawning(Player* player, Bullet bullets[], Laser lasers[], Enemy enemies[], int maxBullets, int maxLasers, int maxEnemies, float dt, Sound playerPew, Textures textures);

void BulletUpdate(Bullet bullets[], int maxBullets, float dt);

void BulletDrawing(Bullet bullets[], int maxBullets, Textures textures);

//void LaserSpawning();

void LaserUpdate(Laser lasers[], int maxLasers, float dt);

void LaserDrawing(Laser lasers[], int maxLasers);

#endif // BULLET_H
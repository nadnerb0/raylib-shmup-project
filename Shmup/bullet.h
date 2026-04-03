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

void SpawnPlayerBullet(Player* player, Bullet bullets[], int maxBullets, Sound playerPew, Texture2D texture, float dt);

void SpawnEnemyBullet(Enemy* enemy, Bullet bullets[], int maxBullets, Sound enemyPew, Texture2D texture, float dt);

void SpawnEnemyLaser(Enemy* enemy, Laser lasers[], int maxlasers, Sound enemyLaserSound, Texture2D textureLaser, Texture2D textureCharge, float dt);

void BulletUpdate(Bullet* bullet, float dt);

void BulletDrawing(Bullet bullets[], int maxBullets);

void LaserUpdate(Laser* laser, float dt);

void LaserDrawing(Laser lasers[], int maxLasers);

#endif // BULLET_H
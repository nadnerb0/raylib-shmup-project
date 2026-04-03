#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

void HandlePlayerCollision(Player* player, Enemy* enemy);

void HandleBulletCollision(Player* player, Enemy enemies[], Bullet* bullet, int maxEnemies, int* bulletsHit, float dt);

void HandleLaserCollision(Player* player, Laser laser, float dt);

#endif // COLLISION_H
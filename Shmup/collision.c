#include "raylib.h"
#include "utils.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

void HandlePlayerCollision(Player* player, Enemy* enemy)
{
	if (player->invincibility > 0)
		return;

	else if (CheckCollisionCircleRec(player->position, player->hitboxRadius, enemy->hitbox))
	{
		PlayerDamaged(player);
		enemy->hp -= 1;
	}
}

void HandleBulletCollision(Player* player, Enemy enemies[], Bullet* bullet, int maxEnemies, int* bulletsHit, float dt)
{
	// Create checkBullet at next frame position for better collision detection
	Bullet checkBullet = *bullet;
	checkBullet.position.y += checkBullet.yDir * bullet->speed * dt;

	// Enemy and playerBullet collision
	if (bullet->isPlayerBullet)
	{
		for (int i = 0; i < maxEnemies; i++)
		{
			if (!enemies[i].active)
				continue;

			if (CheckCollisionRecs(enemies[i].hitbox, checkBullet.hitbox))
			{
				(*bulletsHit)++;
				enemies[i].hp -= 1;
				bullet->active = false;
			}
		}
	}
	// Player and enemyBullet collision
	else
	{
		if (player->invincibility > 0)
			return;

		else if (CheckCollisionCircleRec(player->position, player->hitboxRadius, checkBullet.hitbox))
		{
			(*bulletsHit)++;
			PlayerDamaged(player);
			bullet->active = false;
		}
	}
}

void HandleLaserCollision(Player* player, Laser laser, float dt)
{
	if (player->invincibility > 0)
		return;

	if (laser.isDamaging)
	{
		if (CheckCollisionCircleRec(player->position, player->hitboxRadius, laser.hitbox))
		{
			PlayerDamaged(player);
		}
	}
}
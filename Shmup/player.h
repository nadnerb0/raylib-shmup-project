#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "utils.h"

typedef struct
{
    Vector2 position;
    Texture2D playerSprite;
    Rectangle sourceRec;
    Rectangle destRec;
    Vector2 origin;
    float rotation;
    float speed;
    float width;
    float height;
    float hp;
    float hitboxRadius;
    float fireRate;
    float shootTimer;
    float invincibility;
    bool isDead;
}Player;

void PlayerInit(Player* player, Texture2D redSprite);

void UpdatePlayer(Player* player, float dt);

void ShipMovement(Player* player, float dt, float xRes, float yRes);

void PlayerDamaged(Player* player);

void PlayerDeath(Player* player);

void PlayerDrawing(Player* player, Texture2D redSprite, Texture2D whiteSprite);

#endif // PLAYER_H
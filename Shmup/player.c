#include "raylib.h"
#include "utils.h"
#include "player.h"

void PlayerInit(Player* player, Texture2D redSprite)
{
    // Ship initialization
    player->speed = 250.0f;
    player->hp = 3.0f;
    player->hitboxRadius = 13.0f;
    player->fireRate = 0.2f;
    player->shootTimer = 0.0f;
    player->playerSprite = redSprite;
    player->width = player->playerSprite.width;
    player->height = player->playerSprite.height;
    player->invincibility = 0.0f;
    // Initial player position
    player->position.x = xRes / 2;
    player->position.y = yRes / 1.5;
    // For DrawTexturePro
    player->sourceRec = (Rectangle){ 0.0f, 0.0f, player->playerSprite.width, player->playerSprite.height };
    player->destRec = (Rectangle){ player->position.x, player->position.y, player->width, player->height };
    player->origin = (Vector2){ player->playerSprite.width / 2, player->playerSprite.height / 2 };
    player->rotation = 0.0f;
}

void PlayerDrawing(Player* player, Texture2D redSprite, Texture2D whiteSprite)
{
    // Sprite
    if (player->invincibility > 0 && (int)(GetTime() * 10) % 2 == 0 && !player->isDead)
    {
        player->playerSprite = whiteSprite;
    }
    else
    {
        player->playerSprite = redSprite;
    }

    // Drawing player sprite
    DrawTexturePro(player->playerSprite, player->sourceRec, player->destRec, player->origin, player->rotation, WHITE);
}

void PlayerDeath(Player* player)
{
    player->isDead = true;
}

void ShipMovement(Player* player, float dt, float xRes, float yRes)
{
    // Direction initialization
    float xVel = 0.0f;
    float yVel = 0.0f;

    // Movement vector determination
    // UP
    if (IsKeyDown(KEY_W)) yVel -= 1.0f;
    // DOWN
    if (IsKeyDown(KEY_S)) yVel += 1.0f;
    // LEFT
    if (IsKeyDown(KEY_A)) xVel -= 1.0f;
    // RIGHT
    if (IsKeyDown(KEY_D)) xVel += 1.0f;

    // Diagonal speed fix
    if (xVel != 0.0f && yVel != 0.0f)
    {
        xVel *= 0.7071f;
        yVel *= 0.7071f;
    }

    // For rotating or squishing player sprite
    if (xVel == 1.0f)
    {
    }
    else if (xVel == -1.0f)
    {
    }
    else if (xVel == 0.7071f)
    {
    }
    else if (xVel == -0.7071f)
    {
    }
    else
    {
    }

    // Ship movement
    player->position.x += xVel * player->speed * dt;
    player->position.y += yVel * player->speed * dt;
    player->destRec = (Rectangle){ player->position.x, player->position.y, player->width, player->height };

    // Limit movement to window borders
    ClampB(&player->position.x, 0.0f + player->width / 2, xRes - player->width / 2);  
    ClampB(&player->position.y, 0.0f + player->height / 2, yRes - player->height / 2);
}


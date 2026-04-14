#include "raylib.h"
#include "utils.h"
#include "enemy.h"

void InitEnemy(Enemy* enemy, Enemy enemySpawn, Textures textures)
{
    *enemy = (Enemy){ 0 };
    switch (enemySpawn.enemyType)
    {
    case GRUNT:
        enemy->hp = 2;
        enemy->position = enemySpawn.position;
        enemy->movement = enemySpawn.movement;
        enemy->movementSpeed = 100;
        enemy->width = 30;
        enemy->height = 20;
        enemy->hitbox = (Rectangle){ enemy->position.x - enemy->width / 2, enemy->position.y - enemy->height, enemy->width, enemy->height };
        enemy->texture = textures.enemyGrunt;
        enemy->enemyType = GRUNT;
        enemy->fireRate = 0.5f;
        enemy->shootTimer = 0.0f;
        enemy->hasEnteredBounds = false;
        enemy->active = true;
        break;
    case FODDER:
        enemy->hp = 1;
        enemy->position = enemySpawn.position;
        enemy->movement = enemySpawn.movement;
        enemy->movementSpeed = 150;
        enemy->width = 24;
        enemy->height = 24;
        enemy->hitbox = (Rectangle){ enemy->position.x - enemy->width / 2, enemy->position.y - enemy->height, enemy->width, enemy->height };
        enemy->texture = textures.enemyFodder;
        enemy->enemyType = FODDER;
        enemy->hasEnteredBounds = false;
        enemy->active = true;
        break;
    case SNIPER:
        enemy->hp = 5;
        enemy->position = enemySpawn.position;
        enemy->movement = enemySpawn.movement;
        enemy->movementSpeed = 50;
        enemy->width = 40;
        enemy->height = 40;
        enemy->hitbox = (Rectangle){ enemy->position.x - enemy->width / 2, enemy->position.y - enemy->height, enemy->width, enemy->height };
        enemy->texture = textures.enemySniper;
        enemy->enemyType = SNIPER;
        enemy->fireRate = 3.0f;
        enemy->shootTimer = 0.0f;
        enemy->isSniping = false;
        enemy->hasEnteredBounds = false;
        enemy->active = true;
        break;
    }
}

void EnemyUpdate(Enemy* enemy, int* playerScore, float dt)
{
    EnemyMovement(enemy, dt);

    enemy->shootTimer += dt;

    // Set hasEnteredBounds flag once enemy enters bounds
    if (!enemy->hasEnteredBounds)
    {
        if (IsInBounds(*enemy)) 
            enemy->hasEnteredBounds = true;
    }

    // Despawning enemies that are out of screen
    if (enemy->hasEnteredBounds && 
        IsInBounds(*enemy) == false &&
        enemy->enemyType != SNIPER)         // Snipers bounce between border, no despawn on bounds check
    {
        *enemy = (Enemy){ 0 };
    }
    // Check hp
    else if (enemy->hp <= 0)
    {
        EnemyDeath(enemy, playerScore);
    }
}

void EnemyMovement(Enemy* enemy, float dt)
{
    switch (enemy->movement.tag)
    {
        case LINE_MOVEMENT:
        {
            if (enemy->enemyType == SNIPER && !enemy->isSniping)    // No movement while sniping
            {
                enemy->position.x += enemy->movement.line.direction.x * enemy->movementSpeed * dt;
                enemy->position.y += enemy->movement.line.direction.y * enemy->movementSpeed * dt;
                enemy->hitbox.x += enemy->movement.line.direction.x * enemy->movementSpeed * dt;
                enemy->hitbox.y += enemy->movement.line.direction.y * enemy->movementSpeed * dt;

                // Bouncing left and right
                if (IsInBounds(*enemy) == false && enemy->hasEnteredBounds)
                {
                    enemy->movement.line.direction.x *= -1.0;
                }
            }
            else if (enemy->enemyType != SNIPER)
            {
                enemy->position.x += enemy->movement.line.direction.x * enemy->movementSpeed * dt;
                enemy->position.y += enemy->movement.line.direction.y * enemy->movementSpeed * dt;
                enemy->hitbox.x += enemy->movement.line.direction.x * enemy->movementSpeed * dt;
                enemy->hitbox.y += enemy->movement.line.direction.y * enemy->movementSpeed * dt;
            }
            break;
        }
        case SINE_MOVEMENT:
        {
            enemy->movement.sine.sineTimer += dt;
            
            break;
        }
    }
}

void EnemyDeath(Enemy* enemy, int* playerScore)
{
    if (enemy->enemyType == GRUNT)  
        *playerScore += 10;
    else if (enemy->enemyType == FODDER) 
        *playerScore += 5;
    else if (enemy->enemyType == SNIPER)  
        *playerScore += 20;

    *enemy = (Enemy){ 0 };
}

void EnemyDrawing(Enemy enemies[], int maxEnemies, Textures textures)
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemies[i].active)
        {
            switch (enemies[i].enemyType)
            {
            case GRUNT:
                DrawTexture(enemies[i].texture, enemies[i].position.x - enemies[i].texture.width / 2, enemies[i].position.y - enemies[i].texture.height, WHITE);
                break;
            case SNIPER:
                DrawTexture(enemies[i].texture, enemies[i].position.x - enemies[i].texture.width / 2, enemies[i].position.y - enemies[i].texture.height, WHITE);
                break;
            case FODDER:
                DrawTexture(enemies[i].texture, enemies[i].position.x - enemies[i].texture.width / 2, enemies[i].position.y - enemies[i].texture.height, WHITE);
                break;
            }
        }
    }
}

bool IsInBounds(Enemy enemy)
{
    // Sniper enemies have different (smaller) bounds for bouncing left/right
    if (enemy.enemyType == SNIPER)
    {
        if (enemy.position.x < 0 + enemy.texture.width / 2 ||
            enemy.position.y < 0 + enemy.texture.height / 2 ||
            enemy.position.x > xRes - enemy.texture.width / 2 ||
            enemy.position.y > yRes - enemy.texture.height / 2)
        {
            return false;
        }
        else return true;
    }
    // Other enemies have bigger bounds
    else if (enemy.position.x < 0 - enemy.texture.width || 
        enemy.position.y < 0 - enemy.texture.height || 
        enemy.position.x > xRes + enemy.texture.width || 
        enemy.position.y > yRes + enemy.texture.height)
    {
        return false;
    }
    else return true;
}
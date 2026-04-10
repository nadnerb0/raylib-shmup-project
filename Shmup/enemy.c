#include "raylib.h"
#include "utils.h"
#include "enemy.h"

void InitEnemy(Enemy* enemy, EnemyType enemyType, Vector2 position, Vector2 velocity, Textures textures)
{
    *enemy = (Enemy){ 0 };
    switch (enemyType)
    {
    case GRUNT:
        enemy->hp = 2;
        enemy->position = position;
        enemy->velocity = velocity;
        enemy->width = 30;
        enemy->height = 20;
        enemy->hitbox = (Rectangle){ position.x - enemy->width / 2, position.y - enemy->height, enemy->width, enemy->height };
        enemy->texture = textures.enemyGrunt;
        enemy->enemyType = GRUNT;
        enemy->fireRate = 0.5f;
        enemy->shootTimer = 0.0f;
        enemy->hasEnteredBounds = false;
        enemy->active = true;
        break;
    case FODDER:
        enemy->hp = 1;
        enemy->position = position;
        enemy->velocity = velocity;
        enemy->width = 24;
        enemy->height = 24;
        enemy->hitbox = (Rectangle){ position.x - enemy->width / 2, position.y - enemy->height, enemy->width, enemy->height };
        enemy->texture = textures.enemyFodder;
        enemy->enemyType = FODDER;
        enemy->active = true;
        break;
    case SNIPER:
        enemy->hp = 5;
        enemy->position = position;
        enemy->velocity = velocity;
        enemy->width = 40;
        enemy->height = 40;
        enemy->hitbox = (Rectangle){ position.x - enemy->width / 2, position.y - enemy->height, enemy->width, enemy->height };
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

void EnemySpawning(Enemy enemies[], int maxEnemies, SpawnEvent wave[], int waveSize, float* waveTimer, float dt, Textures textures)
{
    *waveTimer += dt; 
    for (int j = 0; j < waveSize; j++)
    {
        for (int i = 0; i < maxEnemies; i++)
        {
            if (enemies[i].active)
                continue;

            if (*waveTimer >= wave[j].spawnTime && !wave[j].hasSpawned)
            {
                InitEnemy(&enemies[i], wave[j].enemyType, wave[j].position, wave[j].velocity, textures);
                wave[j].hasSpawned = true;
            }
        }
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
    // Position and hitbox update
    if (enemy->enemyType == GRUNT || enemy->enemyType == FODDER)
    {
        enemy->position.x += enemy->velocity.x * dt;
        enemy->position.y += enemy->velocity.y * dt;
        enemy->hitbox.x += enemy->velocity.x * dt;
        enemy->hitbox.y += enemy->velocity.y * dt;
    }

    else if (enemy->enemyType == SNIPER && !enemy->isSniping)    // No movement while sniping
    {
        enemy->position.x += enemy->velocity.x * dt;
        enemy->position.y += enemy->velocity.y * dt;
        enemy->hitbox.x += enemy->velocity.x * dt;
        enemy->hitbox.y += enemy->velocity.y * dt;

        // Bouncing left and right
        if (IsInBounds(*enemy) == false && enemy->hasEnteredBounds)
        {
            enemy->velocity.x *= -1.0;
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
    // Other enemies have "bigger" bounds
    else if (enemy.position.x < 0 - enemy.texture.width || 
        enemy.position.y < 0 - enemy.texture.height || 
        enemy.position.x > xRes + enemy.texture.width || 
        enemy.position.y > yRes + enemy.texture.height)
    {
        return false;
    }
    else return true;
}
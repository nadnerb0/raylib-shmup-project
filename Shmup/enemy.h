#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "utils.h"

typedef enum
{
    GRUNT,
    KAMIKAZE,
    FODDER,
    SNIPER
}EnemyType;

typedef struct
{
    EnemyType enemyType;
    Vector2 position;
    Vector2 velocity;
    float spawnTime;
    bool hasSpawned;
}SpawnEvent;

/*
Fix voor collision/piercing bullets:
elke enemy heeft enemyID, gebaseerd op enemyCount (dus eerste spawn = 0, 100e spawn = 100), dit kan elke stage resetten of niet.
elke Bullet heeft een lijst met ID's (hitList) waarmee hij gecollide heeft, als hij hetzelfde ID weer tegenkomt -> geen collision
dan kan ook: if(sizeof(hitList)>pierceAmount) -> Bullet = {0};ss
*/

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    Rectangle hitbox;
    Texture2D texture;
    float width;
    float height;
    float hp;
    float fireRate;
    float shootTimer;
    EnemyType enemyType;
    bool isSniping;
    bool hasEnteredBounds;
    bool active;
}Enemy;

void InitEnemy(Enemy* enemy, EnemyType enemyType, Vector2 position, Vector2 velocity, Textures textures);

void EnemySpawning(Enemy enemies[], int maxEnemies, SpawnEvent wave[], int waveSize, float* waveTimer, float dt, Textures textures);

void EnemyUpdate(Enemy* enemy, int* playerScore, float dt);

void EnemyMovement(Enemy* enemy, float dt);

void EnemyDeath(Enemy* enemy, int* playerScore);

void EnemyDrawing(Enemy enemies[], int maxEnemies, Textures textures);

bool IsInBounds(Enemy enemy);

#endif // ENEMY_H
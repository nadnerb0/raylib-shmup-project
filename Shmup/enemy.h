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

/*
Fix voor collision/piercing bullets:
elke enemy heeft enemyID, gebaseerd op enemyCount (dus eerste spawn = 0, 100e spawn = 100), dit kan elke stage resetten of niet.
elke Bullet heeft een lijst met ID's (hitList) waarmee hij gecollide heeft, als hij hetzelfde ID weer tegenkomt -> geen collision
dan kan ook: if(sizeof(hitList)>pierceAmount) -> Bullet = {0};
*/

typedef enum
{
    LINE_MOVEMENT,
    SINE_MOVEMENT
}MovementTypeTag;

typedef struct
{
    Vector2 direction;
}LineMovement;

typedef struct
{
    float frequency;
    float amplitude;
    Vector2 direction;
    float sineTimer;
}SineMovement;

typedef struct
{
    MovementTypeTag tag;
    union
    {
        LineMovement line;
        SineMovement sine;
    };
}MovementType;

typedef struct
{
    Vector2 position;
    MovementType movement;
    Rectangle hitbox;
    Texture2D texture;
    EnemyType enemyType;
    float movementSpeed;
    float width;
    float height;
    float hp;
    float fireRate;
    float shootTimer;
    bool isSniping;
    bool hasEnteredBounds;
    bool active;
}Enemy;

// Initializes enemy based on type
void InitEnemy(Enemy* enemy, Enemy enemySpawn, Textures textures);

// Update enemy state
void EnemyUpdate(Enemy* enemy, int* playerScore, float dt);

// Update enemy movement
void EnemyMovement(Enemy* enemy, float dt);

// Process enemy death
void EnemyDeath(Enemy* enemy, int* playerScore);

// Draws all enemies
void EnemyDrawing(Enemy enemies[], int maxEnemies, Textures textures);

bool IsInBounds(Enemy enemy);

#endif // ENEMY_H
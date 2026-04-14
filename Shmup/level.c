#include "raylib.h"
#include "utils.h"
#include "enemy.h"
#include "level.h"

SpawnEvent level1wave1[WAVE1_SIZE];

void WaveLoop(Enemy enemies[], int maxEnemies, SpawnEvent spawnEvent[], int waveSize, float* waveTimer, float dt, Textures textures)
{
    *waveTimer += dt;
    for (int i = 0; i < waveSize; i++)
    {
        if (spawnEvent[i].hasSpawned || *waveTimer <= spawnEvent[i].spawnTime)
            continue;

        EnemySpawning(enemies, maxEnemies, &spawnEvent[i], dt, textures);
        break;
    }
}

void EnemySpawning(Enemy enemies[], int maxEnemies, SpawnEvent* spawnEvent, float dt, Textures textures)
{
    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemies[i].active)
            continue;

        InitEnemy(&enemies[i], spawnEvent->enemy, textures);
        spawnEvent->hasSpawned = true;
        break;
    }
}

void NextWave(Enemy enemies[], int maxEnemies, SpawnEvent enemySpawn[], int waveSize)
{
    // if alle enemies gespawned en dood (of bijna dood?) start volgende wave
}

void AddEnemyFormation(SpawnEvent enemySpawn[], EnemyType enemyType, Vector2 position, MovementType movement, int startIndex, int endIndex, float spawnTime, float spawnTimeDif)
{
    for (int i = startIndex; i <= endIndex; i++)
    {
        enemySpawn[i] = (SpawnEvent)
        {
            .enemy =
            {
                .enemyType = enemyType,
                .position = position,
                .movement = movement
            },
            .spawnTime = spawnTime,
            .hasSpawned = false
        };
        spawnTime += spawnTimeDif;
    }
}

void InitWave1(SpawnEvent enemySpawn[])
{
    // First sniper
    enemySpawn[0] = (SpawnEvent)
    {
        .enemy =
        {
            .enemyType = SNIPER,
            .position = (Vector2){ -50,50 },
            .movement =
            {
                .tag = LINE_MOVEMENT,
                .line.direction = (Vector2){1,0}
            }
        },
        .spawnTime = 0.0f,
        .hasSpawned = false
    };

    // 5 Grunts in formation
    MovementType movement =
    {
        .tag = LINE_MOVEMENT,
        .line.direction = (Vector2){1,0}
    };
    AddEnemyFormation(enemySpawn, GRUNT, (Vector2) { -50, 100 }, movement, 1, 5, 0.0f, 0.5f);

    // Second sniper
    enemySpawn[6] = (SpawnEvent)
    {
        .enemy =
        {
            .enemyType = SNIPER,
            .position = (Vector2) {xRes+50,100},
            .movement =
            {
                .tag = LINE_MOVEMENT,
                .line.direction = (Vector2){-1,0}
            }
        },
        .spawnTime = 2.0f,
        .hasSpawned = false
    };

    // 9 Fodder in formation
    movement = (MovementType)
    {
        .tag = LINE_MOVEMENT,
        .line.direction = (Vector2){-1,0}
    };
    AddEnemyFormation(enemySpawn, FODDER, (Vector2) { xRes+50, 150 }, movement, 7, 15, 0.0f, 0.5f);

    // Total: 16 enemies
}
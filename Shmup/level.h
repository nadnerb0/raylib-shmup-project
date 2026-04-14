#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "enemy.h"

#define WAVE1_SIZE 16

typedef struct
{
    Enemy enemy;
    float spawnTime;
    bool hasSpawned;
}SpawnEvent;

// Wave definitions
extern SpawnEvent level1wave1[WAVE1_SIZE];

void InitWave1(SpawnEvent enemySpawn[]);

// Updates wave logic
void WaveLoop(Enemy enemies[], int maxEnemies, SpawnEvent enemySpawn[], int waveSize, float* waveTimer, float dt, Textures textures);

// Spawns enemies when ready
void EnemySpawning(Enemy enemies[], int maxEnemies, SpawnEvent* wave, float dt, Textures textures);

// NextWave met if (alle enemies gespawned en dood) -> volgende wave
void NextWave(Enemy enemies[], int maxEnemies, SpawnEvent wave[], int waveSize);

// Adds a line of enemies that follow the same movement pattern
void AddEnemyFormation(SpawnEvent enemySpawn[], EnemyType enemyType, Vector2 position, MovementType movement, int startIndex, int endIndex, float spawnTime, float spawnTimeDif);

#endif //LEVEL_H
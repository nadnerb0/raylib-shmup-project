#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

extern float yRes;
extern float xRes;

typedef enum
{
    MENU,
    PLAYING,
    PAUSED,
    DEAD
}GameState;

typedef struct
{
    Texture2D laser;
    Texture2D bullet;
    Texture2D laserCharge;
    Texture2D enemySniper;
    Texture2D enemyGrunt;
    Texture2D enemyBullet;
}Textures;

void PlayFX(Sound effect);
void ClampB(float* toClamp, float minValue, float maxValue);

#endif // UTILS_H
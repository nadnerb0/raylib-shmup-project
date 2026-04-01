#include "raylib.h"

float xRes = 640.0f;
float yRes = 480.0f;

void PlayFX(Sound effect)
{
    float pitch = (8 + rand() % 5) / 10.0f;
    SetSoundPitch(effect, pitch);
    PlaySound(effect);
}

void ClampB(float* toClamp, float minValue, float maxValue)
{
    if (*toClamp > maxValue) *toClamp = maxValue;
    if (*toClamp < minValue) *toClamp = minValue;
}
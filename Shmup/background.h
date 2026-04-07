#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"
#include "utils.h"

typedef struct
{
	Texture2D texture;
	Vector2 pos;
	Rectangle sourceRec;
	Rectangle destRec;
	Vector2 origin;
	float speed;
	int size;
	int layer;
	bool active;
}Star;

typedef enum
{
	FRONT,
	BACK
}ParallaxLayer;

Star InitStar(Star* star, Texture2D starTexture, int starLayer);

void AddStar(Star stars[], int maxStars, Texture2D starTexture, int starLayer);

void UpdateStarPos(Star* star, float dt);

void InitBackground(Star stars[]);

void UpdateBackground(Star stars[], int maxStars, float* starTimerFront, float* starTimerBack, Texture2D starTexture, float dt);

void DrawStar(Star star);

void DrawBackground(Star stars[], int maxStars);

#endif //BACKGROUND_H
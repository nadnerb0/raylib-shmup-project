#include "raylib.h"
#include "utils.h"
#include "background.h"

Star InitStar(Star* star, Texture2D starTexture, int starLayer)
{
	*star = (Star){ 0 };
	star->texture = starTexture;
	if (starLayer == FRONT)
	{
		star->size = 10;
		star->speed = 100;
		star->layer = FRONT;
	}
	else
	{
		star->size = 5;
		star->speed = 50;
		star->layer = BACK;
	}
	star->sourceRec = (Rectangle){ 0.0f, 0.0f, star->texture.width, star->texture.height };
	star->pos.y = -star->size / 2;
	star->pos.x = GetRandomValue(0, xRes) - star->size / 2;
	star->destRec = (Rectangle){ star->pos.x, star->pos.y, star->size, star->size };
	star->origin = (Vector2){ star->size / 2, star->size / 2 };
	star->active = true;

	return *star;
}

void AddStar(Star stars[], int maxStars, Texture2D starTexture, int starLayer)
{
	for (int i = 0; i < maxStars; i++)
	{
		if (!stars[i].active)
		{
			stars[i] = InitStar(&stars[i], starTexture, starLayer);
			break;
		}
	}
}

void UpdateStarPos(Star* star, float dt)
{
	star->pos.y += star->speed * dt;
	star->destRec.y = star->pos.y;
}

void InitBackground(Star stars[])
{
	// Add random stars for first background appearance
}

void UpdateBackground(Star stars[], int maxStars, float* starTimerFront, float* starTimerBack, Texture2D starTexture, float dt)
{
	// Updating star timers
	*starTimerFront += dt;
	*starTimerBack += dt;

	// Adding new stars when timer is up and resetting timer
	if (*starTimerFront >= 0.3f)
	{
		AddStar(stars, maxStars, starTexture, FRONT);
		*starTimerFront = 0.0f;
	}
	if (*starTimerBack >= 0.1f)
	{
		AddStar(stars, maxStars, starTexture, BACK);
		*starTimerBack = 0.0f;
	}

	// Updating stars
	for (int i = 0; i < maxStars; i++)
	{
		if (!stars[i].active)
			continue;

		UpdateStarPos(&stars[i], dt);
		
		// Remove stars outside of window
		if (stars[i].pos.y > yRes)
			stars[i] = (Star){ 0 };
	}
}

void DrawStar(Star star)
{
	DrawTexturePro(star.texture, star.sourceRec, star.destRec, star.origin, 0.0f, WHITE);
}

void DrawBackground(Star stars[], int maxStars)
{
	for (int i = 0; i < maxStars; i++)
	{
		if (stars[i].active && stars[i].layer == BACK)
		{
			DrawStar(stars[i]);
		}
	}
	for (int i = 0; i < maxStars; i++)
	{
		if (stars[i].active && stars[i].layer == FRONT)
		{
			DrawStar(stars[i]);
		}
	}
}
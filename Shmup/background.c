#include "raylib.h"
#include "utils.h"
#include "background.h"

Star InitStar(Star* star, Texture2D starTexture, int starLayer)
{
	*star = (Star){ 0 };
	star->texture = starTexture;
	if (starLayer == FRONT)
	{
		star->size = GetRandomValue(12,15);
		star->speed = 150;
		star->layer = FRONT;
	}
	else if (starLayer == MIDDLE)
	{
		star->size = GetRandomValue(8, 10);
		star->speed = 100;
		star->layer = MIDDLE;
	}
	else
	{
		star->size = GetRandomValue(4, 6);
		star->speed = 50;
		star->layer = BACK;
	}
	star->sourceRec = (Rectangle){ 0.0f, 0.0f, star->texture.width, star->texture.height };
	star->destRec = (Rectangle){ 0, 0, star->size, star->size };
	star->destRec.y = -star->size / 2;
	star->destRec.x = GetRandomValue(0, xRes) - star->size / 2;
	star->rotation = 0.0f;
	star->origin = (Vector2){ star->size / 2, star->size / 2 };
	star->active = true;

	return *star;
}

// voor spawnen: split x-as op in x aantal delen, volgende ster spawn mag niet in dezelfde zijn als degene ervoor

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
	star->destRec.y += star->speed*dt;
}

// Initializes first stars
void InitBackground(Star stars[], Texture2D starTexture)
{
	int yValue = 0;
	int xValue = 0;
	int index = 0;
	// Creating front layer stars
	for (int i = 0; i < 12; i++, index++)
	{
		// y-axis is divided in 3 segments to have better coverage
		if (i < 4) 
			yValue = GetRandomValue(0, yRes / 3);
		else if (i < 8) 
			yValue = GetRandomValue(yRes / 3, yRes / 3 * 2);
		else 
			yValue = GetRandomValue(yRes / 3 * 2, yRes);

		xValue = GetRandomValue(0, xRes);

		Star star = InitStar(&stars[index], starTexture, FRONT);

		stars[index].destRec.y = yValue;
		stars[index].destRec.x = xValue;
	}
	// Creating middle layer stars
	for (int i = 0; i < 24; i++, index++)
	{
		if (i < 8)
			yValue = GetRandomValue(0, yRes / 3);
		else if (i < 16)
			yValue = GetRandomValue(yRes / 3, yRes / 3 * 2);
		else
			yValue = GetRandomValue(yRes / 3 * 2, yRes);

		xValue = GetRandomValue(0, xRes);

		Star star = InitStar(&stars[index], starTexture, MIDDLE);

		stars[index].destRec.y = yValue;
		stars[index].destRec.x = xValue;
	}
	// Creating back layer stars
	for (int i = 0; i < 66; i++, index++)
	{
		if (i < 22)
			yValue = GetRandomValue(0, yRes / 3);
		else if (i < 44)
			yValue = GetRandomValue(yRes / 3, yRes / 3 * 2);
		else
			yValue = GetRandomValue(yRes / 3 * 2, yRes);

		xValue = GetRandomValue(0, xRes);

		Star star = InitStar(&stars[index], starTexture, BACK);

		stars[index].destRec.y = yValue;
		stars[index].destRec.x = xValue;
	}
}

void UpdateBackground(Star stars[], int maxStars, float* starTimerFront, float* starTimerMid, float* starTimerBack, Texture2D starTexture, float dt)
{
	// Updating star timers
	*starTimerFront += dt;
	*starTimerMid += dt;
	*starTimerBack += dt;

	// Adding new stars when timer is up and resetting timer
	if (*starTimerFront >= 0.3f)
	{
		AddStar(stars, maxStars, starTexture, FRONT);
		*starTimerFront = 0.0f;
	}
	if (*starTimerMid >= 0.2f)
	{
		AddStar(stars, maxStars, starTexture, MIDDLE);
		*starTimerMid = 0.0f;
	}
	if (*starTimerBack >= 0.15f)
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
		if (stars[i].destRec.y > yRes)
			stars[i] = (Star){ 0 };
	}
}

void DrawStar(Star star)
{
	DrawTexturePro(star.texture, star.sourceRec, star.destRec, star.origin, star.rotation, WHITE);
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
		if (stars[i].active && stars[i].layer == MIDDLE)
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
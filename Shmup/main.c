#include "raylib.h"
#include "raymath.h"
#include "bullet.h"
#include "utils.h"
#include "enemy.h"
#include "player.h"
#include "debug.h"
#include "mainMenu.h"
#include "gameOver.h"
#include "collision.h"
#include "background.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define MAX_BULLETS 100
#define MAX_ENEMIES 20
#define MAX_LASERS 10
#define MAX_STARS 100

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();

    // Random variables
    int waveNum = 1;
    float waveTimer = 0.0f;
    Color hitboxColor = GREEN;
    bool debugMode = false;

    float screenHeight = GetScreenHeight();
    float screenWidth = GetScreenWidth();

    // Window initilization
    InitWindow(screenWidth, screenHeight, "shmup");
    SetWindowMinSize(320, 240);
    SetTargetFPS(60);
    RenderTexture2D target = LoadRenderTexture(xRes, yRes);

    
    // Gamestate initialization
    GameState gameState = MENU;

    // Player initialization
    Player player = { 0 };
    Texture2D whiteShip = LoadTexture("Sprites/whiteShip_64.png");
    Texture2D redShip = LoadTexture("Sprites/redShip_64.png");
    PlayerInit(&player, redShip);

    // Texture loading
    Texture2D laserTexture = LoadTexture("Sprites/laserGreen.png");
    Texture2D bulletTexture = LoadTexture("Sprites/bullet.png");
    Texture2D laserCharge = LoadTexture("Sprites/laserCharge.png");
    Texture2D enemySniper = LoadTexture("Sprites/enemySniper.png");
    Texture2D enemyGrunt = LoadTexture("Sprites/enemygrunt.png");
    Texture2D enemyBullet = LoadTexture("Sprites/enemyBullet.png");
    Texture2D star = LoadTexture("Sprites/star.png");

    Textures textures = 
    {
        .laser = laserTexture,
        .bullet = bulletTexture,
        .laserCharge = laserCharge,
        .enemySniper = enemySniper,
        .enemyGrunt = enemyGrunt,
        .enemyBullet = enemyBullet,
        .star = star
    };

    // Loading sounds
    Sound playerShootSound = LoadSound("Audio/playerPew.wav");

    // Star pool initialization
    Star stars[MAX_STARS] = { 0 };
    // Enemy pool initialization
    Enemy enemies[MAX_ENEMIES] = { 0 };
    // Bullet pool initialization
    Bullet bullets[MAX_BULLETS] = { 0 };
    // Laser pool initialization
    Laser lasers[MAX_LASERS] = { 0 };

    // Initialize starting background
    InitBackground(stars);
    // Background timers initialization
    float starTimerFront = 0.0f;
    float starTimerBack = 0.0f;

    // Statistic instantiation
    int bulletsHit = 0;
    int playerKills = 0;
    int playerScore = 0;

    // Initiating wave structure
    SpawnEvent wave1[] =
    {
        {.enemyType = SNIPER, .position = (Vector2) { -40,50 }, .velocity = (Vector2) { 60,0 }, .spawnTime = 0.0f, .hasSpawned = false},
        {.enemyType = GRUNT, .position = (Vector2) { 0,100 }, .velocity = (Vector2) { 100,0 }, .spawnTime = 0.0f, .hasSpawned = false},
        {.enemyType = GRUNT, .position = (Vector2) { xRes,70 }, .velocity = (Vector2) { -100,0 }, .spawnTime = 0.0f, .hasSpawned = false},
        {.enemyType = GRUNT, .position = (Vector2) { 0,100 }, .velocity = (Vector2) { 100,0 }, .spawnTime = 1.0f, .hasSpawned = false},
        {.enemyType = GRUNT, .position = (Vector2) { xRes,70 }, .velocity = (Vector2) { -100,0 }, .spawnTime = 1.0f, .hasSpawned = false},
        {.enemyType = GRUNT, .position = (Vector2) { 0,100 }, .velocity = (Vector2) { 100,0 }, .spawnTime = 2.0f, .hasSpawned = false},
        {.enemyType = GRUNT, .position = (Vector2) { xRes,70 }, .velocity = (Vector2) { -100,0 }, .spawnTime = 2.0f, .hasSpawned = false}
    };
    
    Rectangle mainMenuRecs[3] = { 0 };
    CreateMainMenu(mainMenuRecs);

    Rectangle gameOverRecs[2] = { 0 };
    CreateGameOverRecs(gameOverRecs);

    // GAMELOOP
    while (!WindowShouldClose())
    {
        // Manual GameState setting
        //gameState = DEAD;

        float dt = GetFrameTime();

        float scale = MIN((float)GetScreenWidth() / xRes, (float)GetScreenHeight() / yRes);

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (xRes * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (yRes * scale)) * 0.5f) / scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2) { 0, 0 }, (Vector2) { (float)xRes, (float)yRes });

        // Debug mode
        DebugModeToggle(&debugMode);

        // Updating game logic
        switch (gameState)
        {
        case MENU:
            // Menu behaviour
            gameState = MenuToPlaying(mainMenuRecs, virtualMouse);
            if (gameState == PLAYING)
            {
                // Reinitialize player/waves
            }
            break;
        case PLAYING:
        {
            // Player movement
            ShipMovement(&player, dt, xRes, yRes);
            UpdatePlayer(&player, dt);

            // Player bullet spawning
            SpawnPlayerBullet(&player, bullets, MAX_BULLETS, playerShootSound, textures.bullet, dt);

            // Temporary spawning mechanics
            int waveSize = sizeof(wave1) / sizeof(wave1[0]);
            EnemySpawning(enemies, MAX_ENEMIES, wave1, waveSize, &waveTimer, dt, textures);

            // Enemy loop
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                // moet spawnen? -> spawn

                if (!enemies[i].active)
                    continue;

                EnemyUpdate(&enemies[i], wave1, &playerScore, dt);
                if (enemies[i].enemyType == GRUNT)
                {
                    SpawnEnemyBullet(&enemies[i], bullets, MAX_BULLETS, playerShootSound, textures.enemyBullet, dt);
                }
                if (enemies[i].enemyType == SNIPER)
                {
                    SpawnEnemyLaser(&enemies[i], lasers, MAX_LASERS, playerShootSound, textures.laser, textures.laserCharge, dt);
                }
                HandlePlayerCollision(&player, &enemies[i]);
            }
            // Bullet loop
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!bullets[i].active)
                    continue;

                BulletUpdate(&bullets[i], dt);
                HandleBulletCollision(&player, enemies, &bullets[i], MAX_ENEMIES, &bulletsHit, dt);
            }
            // Laser loop
            for (int i = 0; i < MAX_LASERS; i++)
            {
                if (!lasers[i].active)
                    continue;

                LaserUpdate(&lasers[i], dt);
                HandleLaserCollision(&player, lasers[i], dt);
            }

            // Background logic
            UpdateBackground(stars, MAX_STARS, &starTimerFront, &starTimerBack, textures.star, dt);
            
            // Player death
            if (player.hp <= 0)
            {
                PlayerDeath(&player);
                gameState = DEAD;
            }
            break;
        }
        case PAUSED:
            // Paused behaviour

            break;
        case DEAD:
            // Gameover
            gameState = GameOverToMenu(gameOverRecs, virtualMouse);
            break;
        }

        // Create Texture for game
        BeginTextureMode(target);
        ClearBackground(BLACK);  // Clear render texture background color
        switch (gameState)
        {
        case MENU:
            // Menu drawing
            DrawMainMenu(mainMenuRecs);
            break;
        case PLAYING:
            // Background drawing
            DrawBackground(stars, MAX_STARS);
            // Entity drawing
            BulletDrawing(bullets, MAX_BULLETS, textures);
            PlayerDrawing(&player, redShip, whiteShip);
            LaserDrawing(lasers, MAX_LASERS);
            EnemyDrawing(enemies, MAX_ENEMIES, textures);
            // Score drawing
            DrawText(TextFormat("Score: %d", playerScore), xRes - 100, 0, 10, WHITE);
            DrawText(TextFormat("Player HP: %.0f", player.hp), xRes - 100, 10, 10, WHITE);
            break;
        case PAUSED:
            // Paused drawing (freeze all and PAUSED in middle?)
            break;
        case DEAD:
            // Keeps drawing final frame
            BulletDrawing(bullets, MAX_BULLETS, textures);
            PlayerDrawing(&player, redShip, whiteShip);
            LaserDrawing(lasers, MAX_LASERS);
            EnemyDrawing(enemies, MAX_ENEMIES, textures);
            // Score drawing
            DrawText(TextFormat("Score: %d", playerScore), xRes - 100, 0, 10, WHITE);
            DrawText(TextFormat("Player HP: %.0f", player.hp), xRes - 100, 10, 10, WHITE);

            // GameOver drawing/overlay
            GameOverDrawing(gameOverRecs);

            break;
        }

        // Telemetry
        if (debugMode)
        {
            DrawText(TextFormat("x: %.2f y: %.2f", player.position.x, player.position.y), 0, 0, 10, WHITE);
            DrawText(TextFormat("Bullets hit: %d", bulletsHit), 0, 10, 10, WHITE);
            HitboxDrawing(player, enemies, bullets, lasers, MAX_ENEMIES, MAX_BULLETS, MAX_LASERS, hitboxColor);
            int bulletAmountActive = 0;
            for (int idx = 0; idx < MAX_BULLETS; idx++)
            {
                if (bullets[idx].active)
                {
                    bulletAmountActive++;
                }
            }
            int enemiesActive = 0;
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    enemiesActive++;
                }
            }
            int lasersActive = 0;
            for (int i = 0; i < MAX_LASERS; i++)
            {
                if (lasers[i].active)
                {
                    lasersActive++;
                }
            }
            int starsActive = 0;
            for (int i = 0; i < MAX_STARS; i++)
            {
                if (stars[i].active)
                {
                    starsActive++;
                }
            }
            DrawText(TextFormat("Bullets active: %d", bulletAmountActive), 0, 20, 10, WHITE);
            DrawText(TextFormat("Enemies active: %d", enemiesActive), 0, 30, 10, WHITE);
            DrawText(TextFormat("Lasers active: %d", lasersActive), 0, 40, 10, WHITE);
            DrawText(TextFormat("GameState: %d", gameState), 0, 50, 10, WHITE);
            DrawText(TextFormat("Stars active: %d", starsActive), 0, 60, 10, WHITE);
            //player.hp = 100;
        }
        EndTextureMode();

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        // Drawing texture with game independent of window size
        DrawTexturePro(target.texture, (Rectangle) { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
            (Rectangle) {(GetScreenWidth() - ((float)xRes * scale)) * 0.5f, (GetScreenHeight() - ((float)yRes * scale)) * 0.5f, (float)xRes* scale, (float)yRes* scale}, (Vector2) { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLines((GetScreenWidth() - ((float)xRes * scale)) * 0.5f, (GetScreenHeight() - ((float)yRes * scale)) * 0.5f, (float)xRes* scale, (float)yRes* scale - 1, WHITE);

        EndDrawing();
    }

    // Unloading textures etc
    UnloadTexture(player.playerSprite);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
#include "raylib.h"
#include "raymath.h"
#include "bullet.h"
#include "utils.h"
#include "enemy.h"
#include "player.h"
#include "debug.h"
#include "mainMenu.h"
#include "gameOver.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define MAX_BULLETS 100
#define MAX_ENEMIES 20
#define MAX_LASERS 10

void HandleCollision(Player* player, Enemy enemies[], Bullet bullets[], Laser lasers[], int* bulletsHit, float dt);


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

    Textures textures = 
    {
        .laser = laserTexture,
        .bullet = bulletTexture,
        .laserCharge = laserCharge,
        .enemySniper = enemySniper,
        .enemyGrunt = enemyGrunt,
        .enemyBullet = enemyBullet
    };

    // Loading sounds
    Sound playerShootSound = LoadSound("Audio/playerPew.wav");

    // Enemy pool initialization
    Enemy enemies[MAX_ENEMIES] = { 0 };
    // Bullet pool initialization
    Bullet bullets[MAX_BULLETS] = { 0 };
    // Laser pool initialization
    Laser lasers[MAX_LASERS] = { 0 };

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
        case PLAYING:               // Verandering: in veel van de volgende functies wordt elke keer geloopt door de lijsten, herschrijf het dat er het liefst maar 1x per lijst geloopt wordt per frame
        {
            // Playing movement
            ShipMovement(&player, dt, xRes, yRes);
            // Spawning bullets, enemies etc.
            BulletSpawning(&player, bullets, lasers, enemies, MAX_BULLETS, MAX_LASERS, MAX_ENEMIES, dt, playerShootSound, textures);
            int waveSize = sizeof(wave1) / sizeof(wave1[0]);
            EnemySpawning(enemies, MAX_ENEMIES, wave1, waveSize, &waveTimer, dt, textures);
            // Updating position and state of entities
            EnemyUpdate(enemies, wave1, MAX_ENEMIES, &playerScore, dt);
            BulletUpdate(bullets, MAX_BULLETS, dt);
            LaserUpdate(lasers, MAX_LASERS, dt);
            // Check collisions
            HandleCollision(&player, enemies, bullets, lasers, &bulletsHit, dt);
            // Player death handling
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
            DrawText(TextFormat("Bullets active: %d", bulletAmountActive), 0, 20, 10, WHITE);
            DrawText(TextFormat("Enemies active: %d", enemiesActive), 0, 30, 10, WHITE);
            DrawText(TextFormat("Lasers active: %d", lasersActive), 0, 40, 10, WHITE);
            DrawText(TextFormat("GameState: %d", gameState), 0, 50, 10, WHITE);
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

void HandleCollision(Player* player, Enemy enemies[], Bullet bullets[], Laser lasers[], int* bulletsHit, float dt)
{
    player->invincibility -= dt;
    // Enemies
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        // Skip inactive enemies
        if (!enemies[i].active)
            continue;

        // Player/enemy collision
        // NOTE: needs a timer before registering again, otherwise 1 collision = complete death and destruction
        if (CheckCollisionCircleRec(player->position, player->hitboxRadius, enemies[i].hitbox) )
        {
            player->hp -= 1;
            enemies[i].hp -= 1;
            player->invincibility = 1.0f;
        }

        // Bullet collision
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (!bullets[j].active)
                continue;

            // Player bullets
            if (bullets[j].isPlayerBullet)
            {
                Bullet checkBullet = bullets[j];
                checkBullet.position.y += checkBullet.yDir * bullets[j].speed * dt;
                if (CheckCollisionRecs(enemies[i].hitbox, checkBullet.hitbox))
                {
                    (*bulletsHit)++;
                    enemies[i].hp -= 1;
                    bullets[j].active = false;
                }
            }
            // Enemy bullets
            else if (!bullets[j].isPlayerBullet)
            {
                if (!(player->invincibility <= 0))
                    continue;

                Bullet checkBullet = bullets[j];
                checkBullet.position.y += checkBullet.yDir * bullets[j].speed * dt;
                if (CheckCollisionCircleRec(player->position, player->hitboxRadius, checkBullet.hitbox))
                {
                    (*bulletsHit)++;
                    player->hp -= 1;
                    player->invincibility = 1.0f;
                    bullets[j].active = false;
                }
            }
        }
    }
    // Lasers
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (lasers[i].isDamaging)
        {
            if (!(player->invincibility <= 0))
                continue;

            if (CheckCollisionCircleRec(player->position, player->hitboxRadius, lasers[i].hitbox))
            {
                (*bulletsHit)++;
                player->hp -= 1;
                player->invincibility = 1.0f;
            }
        }
    }
}


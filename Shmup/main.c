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
#include "level.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define MAX_BULLETS 100
#define MAX_ENEMIES 100
#define MAX_LASERS 10
#define MAX_STARS 150

void EnemyLoop(Player* player, Enemy enemies[], Bullet bullets[], Laser lasers[], int maxEnemies, int maxBullets, int maxLasers, int* playerScore, Sound playerShootSound, Textures textures, float dt);
void BulletLoop(Player* player, Enemy enemies[], Bullet bullets[], int* bulletsHit, float dt);
void LaserLoop(Player* player, Laser lasers[], float dt);

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();

    // Random variables
    int waveNum = 1;
    float waveTimer = 0.0f;
    Color hitboxColor = GREEN;
    bool debugMode = false;

    int screenHeight = GetScreenHeight();
    int screenWidth = GetScreenWidth();

    // Window initilization
    InitWindow(screenWidth, screenHeight, "shmup");
    SetWindowMinSize(320, 240);
    SetTargetFPS(60);
    RenderTexture2D target = LoadRenderTexture(xRes, yRes);
    
    // Gamestate initialization
    GameState gameState = MENU;

    // Texture loading
    Texture2D whiteShip = LoadTexture("Sprites/whiteShip_64.png");
    Texture2D redShip = LoadTexture("Sprites/redShip_64.png");
    Texture2D laserTexture = LoadTexture("Sprites/laserGreen.png");
    Texture2D bulletTexture = LoadTexture("Sprites/bullet.png");
    Texture2D laserCharge = LoadTexture("Sprites/laserCharge.png");
    Texture2D enemySniper = LoadTexture("Sprites/enemySniper.png");
    Texture2D enemyGrunt = LoadTexture("Sprites/enemygrunt.png");
    Texture2D enemyFodder = LoadTexture("Sprites/enemyFodder.png");
    Texture2D enemyBullet = LoadTexture("Sprites/enemyBullet.png");
    Texture2D star = LoadTexture("Sprites/star.png");

    Textures textures = 
    {
        .laser = laserTexture,
        .bullet = bulletTexture,
        .laserCharge = laserCharge,
        .enemySniper = enemySniper,
        .enemyGrunt = enemyGrunt,
        .enemyFodder = enemyFodder,
        .enemyBullet = enemyBullet,
        .star = star
    };

    // Player initialization
    Player player = { 0 };

    // Loading sounds
    Sound playerShootSound = LoadSound("Audio/playerPew.wav");

    // Star pool initialization
    static Star stars[MAX_STARS] = { 0 };
    // Enemy pool initialization
    static Enemy enemies[MAX_ENEMIES] = { 0 };
    // Bullet pool initialization
    static Bullet bullets[MAX_BULLETS] = { 0 };
    // Laser pool initialization
    static Laser lasers[MAX_LASERS] = { 0 };

    // Background timers initialization
    float starTimerFront = 0.0f;
    float starTimerMid = 0.0f;
    float starTimerBack = 0.0f;

    // Initialize starting background
    InitBackground(stars, textures.star);

    // Statistic instantiation
    int bulletsHit = 0;
    int playerKills = 0;
    int playerScore = 0;
    
    // Rectangles for menus
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

            UpdateBackground(stars, MAX_STARS, &starTimerFront, &starTimerMid, &starTimerBack, textures.star, dt);

            if (gameState == PLAYING)
            {
                DisableCursor();

                // Empty arrays
                memset(enemies, 0, sizeof(enemies));
                memset(bullets, 0, sizeof(bullets));
                memset(lasers, 0, sizeof(lasers));
                // Reset stats
                bulletsHit = 0;
                playerKills = 0;
                playerScore = 0;
                // Reset wave timer
                waveTimer = 0;
                // Initialize player
                PlayerInit(&player, redShip);
                // Initialize first wave
                InitWave1(level1wave1);
            }
            break;
        case PLAYING:
        {
            // Player movement
            ShipMovement(&player, dt, xRes, yRes);
            UpdatePlayer(&player, dt);

            // Player bullet spawning
            SpawnPlayerBullet(&player, bullets, MAX_BULLETS, playerShootSound, textures.bullet, dt);

            // Updating world state
            EnemyLoop(&player, enemies, bullets, lasers, MAX_ENEMIES, MAX_BULLETS, MAX_LASERS, &playerScore, playerShootSound, textures, dt);
            BulletLoop(&player, enemies, bullets, &bulletsHit, dt);
            LaserLoop(&player, lasers, dt);
            WaveLoop(enemies, MAX_ENEMIES, level1wave1, WAVE1_SIZE, &waveTimer, dt, textures);

            // Background logic
            UpdateBackground(stars, MAX_STARS, &starTimerFront, &starTimerMid, &starTimerBack, textures.star, dt);
            
            // Player death
            if (player.hp <= 0)
            {
                PlayerDeath(&player);
                EnableCursor();
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
            DrawBackground(stars, MAX_STARS);
            DrawMainMenu(mainMenuRecs);
            break;
        case PLAYING:
            // Background drawing
            DrawBackground(stars, MAX_STARS);
            // Entity drawing
            LaserDrawing(lasers, MAX_LASERS);
            EnemyDrawing(enemies, MAX_ENEMIES, textures);
            PlayerDrawing(&player, redShip, whiteShip);
            BulletDrawing(bullets, MAX_BULLETS, textures);
            // Score drawing
            DrawText(TextFormat("Score: %d", playerScore), xRes - 100, 0, 10, WHITE);
            DrawText(TextFormat("Player HP: %.0f", player.hp), xRes - 100, 10, 10, WHITE);
            break;
        case PAUSED:
            // Paused drawing (freeze all and PAUSED in middle?)
            break;
        case DEAD:
            // Keeps drawing final frame
            DrawBackground(stars, MAX_STARS);
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
            int snipersActive = 0;
            int foddersActive = 0;
            int gruntsActive = 0;
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    enemiesActive++;
                    if (enemies[i].enemyType == FODDER)
                    {
                        foddersActive++;
                    }
                    else if (enemies[i].enemyType == GRUNT)
                    {
                        gruntsActive++;
                    }
                    else if (enemies[i].enemyType == SNIPER)
                    {
                        snipersActive++;
                    }
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
            DrawText(TextFormat("Fodders active: %d", foddersActive), 0, 37, 10, WHITE);
            DrawText(TextFormat("Grunts active: %d", gruntsActive), 0, 44, 10, WHITE);
            DrawText(TextFormat("Snipers active: %d", snipersActive), 0, 52, 10, WHITE);
            DrawText(TextFormat("Lasers active: %d", lasersActive), 0, 60, 10, WHITE);
            DrawText(TextFormat("GameState: %d", gameState), 0, 70, 10, WHITE);
            DrawText(TextFormat("Stars active: %d", starsActive), 0, 80, 10, WHITE);
            //player.hp = 100;
        }
        EndTextureMode();

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        // Drawing texture with game based on window size
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

void EnemyLoop(Player* player, Enemy enemies[], Bullet bullets[], Laser lasers[], int maxEnemies, int maxBullets, int maxLasers, int* playerScore, Sound playerShootSound, Textures textures, float dt)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        // moet spawnen? -> spawn

        if (!enemies[i].active)
            continue;

        EnemyUpdate(&enemies[i], playerScore, dt);
        if (enemies[i].enemyType == GRUNT)
        {
            SpawnEnemyBullet(&enemies[i], bullets, MAX_BULLETS, playerShootSound, textures.enemyBullet, dt);
        }
        if (enemies[i].enemyType == SNIPER)
        {
            SpawnEnemyLaser(&enemies[i], lasers, MAX_LASERS, playerShootSound, textures.laser, textures.laserCharge, dt);
        }
        HandlePlayerCollision(player, &enemies[i]);
    }
}

void BulletLoop(Player* player, Enemy enemies[], Bullet bullets[], int* bulletsHit, float dt)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
            continue;

        BulletUpdate(&bullets[i], dt);
        HandleBulletCollision(player, enemies, &bullets[i], MAX_ENEMIES, bulletsHit, dt);
    }
}

void LaserLoop(Player* player, Laser lasers[], float dt)
{
    // Laser loop
    for (int i = 0; i < MAX_LASERS; i++)
    {
        if (!lasers[i].active)
            continue;

        LaserUpdate(&lasers[i], dt);
        HandleLaserCollision(player, lasers[i], dt);
    }
}
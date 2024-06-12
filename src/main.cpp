#include <raylib.h>
#include <vector>
#include <string>

int playerScore = 0;
int playerLives = 3;
int speedMultiplier = 1;

class Player {
    public:
        float x, y;
        int size;
        float speedX, speedY;
        Color playerColour;
    
        void Draw() {
            DrawCircle(x, y, size, playerColour);
        }
    
        void Update() {
        
            // Check Movement
            if(IsKeyDown(KEY_RIGHT)) x += speedX;
            if(IsKeyDown(KEY_LEFT))  x -= speedX;
            if(IsKeyDown(KEY_UP))    y -= speedY;
            if(IsKeyDown(KEY_DOWN))  y += speedY;
    
            // Check Boundaries
            if(y <= size ) y = size;
            if(y + size >= GetScreenHeight()) y = GetScreenHeight() - size;
            if(x <= size ) x = size;
            if(x + size >= GetScreenWidth()) x = GetScreenWidth() - size;
        }

        void Reset() {
            x = 400;
            y = 400;
        }
};

class Enenmy: public Player {
    public: 
        void Update(Player &player, bool &gameRunning) {
            // Check Movement
            if(y < player.y) y += speedY;
            if(y > player.y) y -= speedY;
            if(x < player.x) x += speedX;
            if(x > player.x) x -= speedX;
            // Check Player Collision
            if(CheckCollisionCircles({x, y}, size, {player.x, player.y}, player.size)) {
                playerLives--;
                if(playerLives > 0) {
                    player.Reset();
                    Reset();
                } else {
                    gameRunning = false;
                }
            }
            // Check Wall Boundaries
            if(y <= size ) y = size;
            if(y + size >= GetScreenHeight()) y = GetScreenHeight() - size;
            if(x <= size ) x = size;
            if(x + size >= GetScreenWidth()) x = GetScreenWidth() - size;
        }
        void Reset() {
            x = 400;
            y = 200;
        }
};

class Drop {
    public:
        Vector2 position;
        int size;
        Color dropColour;

        void Draw() {
            DrawCircle(position.x, position.y, size, dropColour);
        }

        void Update(Player &player, Enenmy &enemy) {
            // Check Player Collision
            if(CheckCollisionCircles({position.x, position.y}, size, {player.x, player.y}, player.size)) {
                playerScore++;
                Reset();

                if(playerScore % 5 == 0) {
                    speedMultiplier++;
                    enemy.speedX = speedMultiplier * 0.5;
                    enemy.speedY = speedMultiplier * 0.5;
                }
            }
        }

        void Reset() {
            position.x = GetRandomValue(0, GetScreenWidth());
            position.y = GetRandomValue(0, GetScreenHeight());
        }
};

Player player;
Enenmy enemy;
Drop drop;
bool gameRunning = true;

void ResetGame() {
    playerScore = 0;
    playerLives = 3;
    speedMultiplier = 1;

    // Reset player
    player.Reset();
    player.x = 400;
    player.y = 400;
    player.size = 25;
    player.speedX = 3;
    player.speedY = 3;
    player.playerColour = {40, 170, 245, 255};

    // Reset enemy
    enemy.Reset();
    enemy.x = 400;
    enemy.y = 200;
    enemy.size = 50;
    enemy.speedX = 0.5;
    enemy.speedY = 0.5;
    enemy.playerColour = {208, 42, 75, 255};

    // Reset drop
    drop.Reset();
    drop.position.x = 600;
    drop.position.y = 600;
    drop.size = 10;
    drop.dropColour = {68, 208, 42, 255};
}

int main(void) {
    // Initialization
    InitWindow(800, 800, "My First Raylib Game");
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    // DisableCursor(); // Disable cursor
    // ToggleFullscreen(); // Toggle fullscreen

    Color gameBG = {58, 58, 59, 255};

    // Main loop for the "Play Again" feature
    while (!WindowShouldClose()) {
        ResetGame();
        bool gameRunning = true;

        // Main game loop
        while (!WindowShouldClose() && gameRunning) {
            // 1. Event handling

            // 2. Updating position
            player.Update();
            enemy.Update(player, gameRunning);
            drop.Update(player, enemy);

            // 3. Drawing
            BeginDrawing();
            ClearBackground(gameBG);
            player.Draw();
            enemy.Draw();
            drop.Draw();

            // UI
            std::string coordString = std::to_string((int)player.x) + ", " + std::to_string((int) player.y);
            DrawText(coordString.c_str(), 10, 10, 20, WHITE);
            DrawText(TextFormat("Score: %i", playerScore), 10, 30, 20, WHITE);
            DrawText(TextFormat("Lives: %i", playerLives), 10, 50, 20, WHITE);

            EndDrawing();
        }

        // Display end game message
        bool playAgain = false;
        while (!WindowShouldClose() && !playAgain) {
            BeginDrawing();
            ClearBackground(gameBG);

            DrawText("Game Over", 300, 350, 40, RED);
            DrawText(TextFormat("Final Score: %i", playerScore), 300, 400, 20, DARKGRAY);
            DrawText("Press ENTER to play again", 300, 430, 20, DARKGRAY);
            DrawText("Press ESC to exit", 300, 460, 20, DARKGRAY);

            EndDrawing();

            if (IsKeyPressed(KEY_ENTER)) {
                playAgain = true;
            } else if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow();
                return 0;
            }
        }
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}
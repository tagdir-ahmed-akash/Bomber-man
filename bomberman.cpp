#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <winuser.h>
#include <tchar.h> // For Unicode support
#include <vector>

// Define maximum number of enemies
#define MAX_ENEMIES 5

// Enemy structure with target positions for grid-based movement
struct Enemy {
    int x;          // Current X position in pixels
    int y;          // Current Y position in pixels
    int gridX;      // Current grid X position
    int gridY;      // Current grid Y position
    int targetX;    // Target X position in pixels
    int targetY;    // Target Y position in pixels
    int direction;  // Current movement direction (0: Up, 1: Down, 2: Left, 3: Right)
    int moveTimer;  // Timer to control movement frequency
};

// Global Variables
int playerX = 0, playerY = 0;
const int w = 850, h = 850;
const int r = 20;
int speed = 5;
bool Box[17][17] = { false };
bool HasGiftBag[17][17] = { false }; // Tracks which bricks have gift bags
bool GiftBag[17][17] = { false };    // Tracks active gift bags on the grid
bool bombActive = false;
int bombX, bombY;
int bombTimer = 0;
const int bombDelay = 30; // Bomb explodes after 3 seconds (30 frames)
int bombRange = 2;        // Initial bomb explosion range

// Enemies array
Enemy enemies[MAX_ENEMIES] = { 0 };

// Function Prototypes
void KeyControl();
void DrawMap();
void DrawGiftBags();
void CreateBox();
void InitializeEnemies();
void MoveEnemies();
void DrawEnemies();
bool CheckPlayerCollision();
bool CheckExplosionCollision();
bool ExplodeBomb();
bool isWalkable(int i, int j);
void CollectGiftBag();

int main()
{
    // Initialize graphics
    initgraph(w, h);	// Create screen
    ShowCursor(FALSE); // Hide cursor for better gaming experience
    setbkcolor(RGB(0, 120, 0)); // Set background color
    cleardevice();

    srand((unsigned int)time(NULL));

    // Start the player in the center cell (8,8)
    playerX = 8 * 50 + 25; // center x
    playerY = 8 * 50 + 25; // center y

    CreateBox();
    InitializeEnemies();

    BeginBatchDraw();
    bool gameOver = false;

    while (!gameOver)
    {
        DrawMap();  // Create map

        // Draw gift bags
        DrawGiftBags();

        // Draw player
        setfillcolor(RED);
        fillcircle(playerX, playerY, r);

        // Draw enemies
        DrawEnemies();

        // Handle player input
        KeyControl();  // Handle movement and bomb placement

        // Collect gift bag if on the same cell
        CollectGiftBag();

        // Move enemies
        MoveEnemies();

        // Check for player collision with enemies
        if (CheckPlayerCollision()) {
            gameOver = true;
        }

        if (bombActive)
        {
            // Draw the bomb at its current location
            setfillcolor(BLACK);
            fillcircle(bombX * 50 + 25, bombY * 50 + 25, 20);

            // Increment the timer
            bombTimer++;

            // Check if the bomb should explode
            if (bombTimer >= bombDelay)
            {
                // Trigger explosion effect and check if player is hit
                bool playerHit = ExplodeBomb();
                bombActive = false;  // Bomb explodes, deactivate it

                if (playerHit)
                {
                    gameOver = true;
                }
            }
        }

        FlushBatchDraw();
        Sleep(100);
        cleardevice();  // Clear screen for next frame
    }

    // Display Game Over Message
    settextcolor(WHITE);
    settextstyle(40, 0, _T("Arial"));
    outtextxy(w / 2 - 150, h / 2 - 20, _T("Game Over!"));

    FlushBatchDraw();
    Sleep(2000); // Wait for 2 seconds before closing

    closegraph();
    return 0;
}

// Helper function to determine if a cell is walkable
bool isWalkable(int i, int j)
{
    if (i < 0 || i >= 17 || j < 0 || j >= 17)
        return false;
    if ((i % 2 != 0) && (j % 2 != 0)) // Fixed obstacles
        return false;
    if (Box[i][j]) // Destructible bricks
        return false;
    return true;
}

// Handle player input and movement
void KeyControl()
{
    int gridX = (playerX - 25) / 50;  // Get current grid position
    int gridY = (playerY - 25) / 50;

    // Handle movement based on key states
    if (GetAsyncKeyState(VK_UP) & 0x8000)   // Up
    {
        if (gridY > 0 && isWalkable(gridX, gridY - 1))
        {
            playerY -= 50;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) // Down
    {
        if (gridY < 16 && isWalkable(gridX, gridY + 1))
        {
            playerY += 50;
        }
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) // Left
    {
        if (gridX > 0 && isWalkable(gridX - 1, gridY))
        {
            playerX -= 50;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // Right
    {
        if (gridX < 16 && isWalkable(gridX + 1, gridY))
        {
            playerX += 50;
        }
    }

    // Place bomb with space key
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !bombActive) // Place bomb with space key
    {
        bombX = gridX;
        bombY = gridY;
        bombActive = true;
        bombTimer = 0;  // Reset the timer when the bomb is placed
    }
}

// Draw the game map, including bricks
void DrawMap()
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if ((i % 2 != 0) && (j % 2 != 0))
            {
                int left = i * 50;
                int top = j * 50;
                int right = left + 50;
                int bottom = top + 50;
                setfillcolor(RGB(190, 190, 190)); // Fixed obstacles color
                setlinecolor(RGB(150, 150, 150));
                fillrectangle(left, top, right, bottom);
            }
            else if (Box[i][j])
            {
                int left_b = i * 50 + 5;
                int top_b = j * 50 + 5;
                int right_b = left_b + 40;
                int bottom_b = top_b + 40;
                setlinecolor(RGB(50, 50, 50)); // Destructible bricks color
                setfillcolor(RGB(50, 50, 50));
                fillrectangle(left_b, top_b, right_b, bottom_b);
            }
        }
    }
}

// Draw gift bags as green circles
void DrawGiftBags()
{
    setfillcolor(GREEN); // Gift bags color
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (GiftBag[i][j])
            {
                fillcircle(i * 50 + 25, j * 50 + 25, 10); // Smaller circle for gift bag
            }
        }
    }
}

// Generate the bricks on the map and assign gift bags
void CreateBox()
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (((i + j) % 2 != 0) && (rand() % 3 == 0))
            {
                Box[i][j] = true;  // Destructible brick

                // Assign a gift bag with 20% probability
                if (rand() % 5 == 0) // 20% chance
                {
                    HasGiftBag[i][j] = true;
                }
            }
            else
            {
                Box[i][j] = false; // No brick
            }
        }
    }
}

// Initialize enemies at random walkable grid positions
void InitializeEnemies() {
    for (int e = 0; e < MAX_ENEMIES; e++) {
        bool valid = false;
        while (!valid) {
            enemies[e].gridX = rand() % 17;
            enemies[e].gridY = rand() % 17;
            // Ensure the enemy does not spawn on a brick, fixed obstacle, or the center cell
            if (isWalkable(enemies[e].gridX, enemies[e].gridY) &&
                !(enemies[e].gridX == 8 && enemies[e].gridY == 8)) {
                enemies[e].x = enemies[e].gridX * 50 + 25;
                enemies[e].y = enemies[e].gridY * 50 + 25;
                enemies[e].direction = rand() % 4; // Random initial direction
                enemies[e].moveTimer = 0;
                enemies[e].targetX = enemies[e].x;
                enemies[e].targetY = enemies[e].y;
                valid = true;
            }
        }
    }
}

// Move enemies with grid-based logic, ensuring they stay on walkable cells
void MoveEnemies() {
    for (int e = 0; e < MAX_ENEMIES; e++) {
        Enemy& enemy = enemies[e];

        // If enemy is inactive (destroyed), skip
        if (enemy.gridX == -1 && enemy.gridY == -1)
            continue;

        // Check if enemy has reached the target position
        if (enemy.x == enemy.targetX && enemy.y == enemy.targetY)
        {
            // Choose a new direction
            std::vector<int> possibleDirections;

            // Check all four directions for possible movement
            // Up
            if (enemy.gridY > 0 && isWalkable(enemy.gridX, enemy.gridY - 1) &&
                !(enemy.gridX == (playerX - 25) / 50 && (enemy.gridY - 1) == (playerY - 25) / 50))
                possibleDirections.push_back(0);
            // Down
            if (enemy.gridY < 16 && isWalkable(enemy.gridX, enemy.gridY + 1) &&
                !(enemy.gridX == (playerX - 25) / 50 && (enemy.gridY + 1) == (playerY - 25) / 50))
                possibleDirections.push_back(1);
            // Left
            if (enemy.gridX > 0 && isWalkable(enemy.gridX - 1, enemy.gridY) &&
                !((enemy.gridX - 1) == (playerX - 25) / 50 && enemy.gridY == (playerY - 25) / 50))
                possibleDirections.push_back(2);
            // Right
            if (enemy.gridX < 16 && isWalkable(enemy.gridX + 1, enemy.gridY) &&
                !((enemy.gridX + 1) == (playerX - 25) / 50 && enemy.gridY == (playerY - 25) / 50))
                possibleDirections.push_back(3);

            if (!possibleDirections.empty())
            {
                // Randomly choose a direction from possible directions
                int randIndex = rand() % possibleDirections.size();
                enemy.direction = possibleDirections[randIndex];

                // Set target positions based on the chosen direction
                switch (enemy.direction)
                {
                case 0: // Up
                    enemy.targetY -= 50;
                    break;
                case 1: // Down
                    enemy.targetY += 50;
                    break;
                case 2: // Left
                    enemy.targetX -= 50;
                    break;
                case 3: // Right
                    enemy.targetX += 50;
                    break;
                }
            }
            else
            {
                // No available directions, stay in place
                enemy.direction = -1;
            }
        }

        // Move towards the target position
        if (enemy.x < enemy.targetX)
            enemy.x += speed;
        else if (enemy.x > enemy.targetX)
            enemy.x -= speed;

        if (enemy.y < enemy.targetY)
            enemy.y += speed;
        else if (enemy.y > enemy.targetY)
            enemy.y -= speed;

        // Ensure enemy does not overshoot the target
        if (abs(enemy.x - enemy.targetX) < speed)
            enemy.x = enemy.targetX;
        if (abs(enemy.y - enemy.targetY) < speed)
            enemy.y = enemy.targetY;

        // Update grid positions
        enemy.gridX = (enemy.x - 25) / 50;
        enemy.gridY = (enemy.y - 25) / 50;
    }
}

// Draw enemies as blue circles
void DrawEnemies() {
    setfillcolor(BLUE);
    for (int e = 0; e < MAX_ENEMIES; e++) {
        // Only draw active enemies
        if (enemies[e].gridX != -1 && enemies[e].gridY != -1) {
            fillcircle(enemies[e].x, enemies[e].y, r);
        }
    }
}

// Check if player collides with any enemy
bool CheckPlayerCollision() {
    int playerGridX = (playerX - 25) / 50;
    int playerGridY = (playerY - 25) / 50;

    for (int e = 0; e < MAX_ENEMIES; e++) {
        if (enemies[e].gridX == playerGridX && enemies[e].gridY == playerGridY) {
            return true; // Collision detected
        }
    }
    return false;
}

// Check if explosion hits any enemy
bool CheckExplosionCollision() {
    bool enemyHit = false;
    if (!bombActive) return enemyHit;

    for (int e = 0; e < MAX_ENEMIES; e++) {
        // Skip inactive enemies
        if (enemies[e].gridX == -1 && enemies[e].gridY == -1)
            continue;

        // Check vertical explosion
        if (enemies[e].gridX == bombX &&
            abs(enemies[e].gridY - bombY) <= bombRange)
        {
            // Destroy enemy
            enemies[e].gridX = -1; // Mark as destroyed
            enemies[e].x = -50;     // Move off-screen
            enemies[e].y = -50;
            enemyHit = true;
            continue;
        }

        // Check horizontal explosion
        if (enemies[e].gridY == bombY &&
            abs(enemies[e].gridX - bombX) <= bombRange)
        {
            // Destroy enemy
            enemies[e].gridX = -1;
            enemies[e].x = -50;
            enemies[e].y = -50;
            enemyHit = true;
            continue;
        }
    }

    return enemyHit;
}

// Handle bomb explosion effects and check if player is hit
bool ExplodeBomb()
{
    bool playerHit = false;

    // Destroy bricks in the plus (+) pattern
    for (int k = -bombRange; k <= bombRange; k++)
    {
        // Destroy bricks vertically
        if (bombY + k >= 0 && bombY + k < 17 && Box[bombX][bombY + k])
        {
            Box[bombX][bombY + k] = false; // Destroy brick

            // If brick had a gift bag, drop it
            if (HasGiftBag[bombX][bombY + k])
            {
                GiftBag[bombX][bombY + k] = true;
                HasGiftBag[bombX][bombY + k] = false; // Remove gift bag from brick
            }
        }

        // Destroy bricks horizontally
        if (bombX + k >= 0 && bombX + k < 17 && Box[bombX + k][bombY])
        {
            Box[bombX + k][bombY] = false; // Destroy brick

            // If brick had a gift bag, drop it
            if (HasGiftBag[bombX + k][bombY])
            {
                GiftBag[bombX + k][bombY] = true;
                HasGiftBag[bombX + k][bombY] = false; // Remove gift bag from brick
            }
        }
    }

    // Check if any enemy is hit by the explosion
    CheckExplosionCollision();

    // Check if player is hit by the explosion
    int playerGridX = (playerX - 25) / 50;
    int playerGridY = (playerY - 25) / 50;

    if ((playerGridX == bombX && abs(playerGridY - bombY) <= bombRange) ||
        (playerGridY == bombY && abs(playerGridX - bombX) <= bombRange))
    {
        playerHit = true;
    }

    // Visualize explosion
    setfillcolor(WHITE); // Change color to visualize explosion
    for (int k = -bombRange; k <= bombRange; k++)
    {
        // Vertical explosion
        if (bombY + k >= 0 && bombY + k < 17)
        {
            fillrectangle(bombX * 50, (bombY + k) * 50, (bombX + 1) * 50, (bombY + k + 1) * 50);
        }

        // Horizontal explosion
        if (bombX + k >= 0 && bombX + k < 17)
        {
            fillrectangle((bombX + k) * 50, bombY * 50, (bombX + k + 1) * 50, (bombY + 1) * 50);
        }
    }

    return playerHit;
}

// Collect gift bag if on the same cell as the player
void CollectGiftBag()
{
    int playerGridX = (playerX - 25) / 50;
    int playerGridY = (playerY - 25) / 50;

    if (GiftBag[playerGridX][playerGridY])
    {
        bombRange += 1; // Increase bomb explosion range by one cell
        GiftBag[playerGridX][playerGridY] = false; // Remove the collected gift bag
    }
}

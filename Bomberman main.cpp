//2021521460115
//MD AKASH HOSSAIN
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <winuser.h>
#include <tchar.h> 
#include <vector>


#define MAX_ENEMIES 8


struct Enemy {
    int x;          
    int y;          
    int gridX;     
    int gridY;      
    int targetX;    
    int targetY;    
    int direction;  
    int moveTimer; 
};


int playerX = 0, playerY = 0;
const int w = 850, h = 850;
const int r = 20;
int speed = 5;
bool Box[17][17] = { false };
bool HasGiftBag[17][17] = { false }; 
bool GiftBag[17][17] = { false };    
bool bombActive = false;
int bombX, bombY;
int bombTimer = 0;
const int bombDelay = 30; 
int bombRange = 2;        


Enemy enemies[MAX_ENEMIES] = { 0 };

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
   
    initgraph(w, h);	
    ShowCursor(FALSE); 
    setbkcolor(RGB(0, 120, 0)); 
    cleardevice();

    srand((unsigned int)time(NULL));

    
    playerX = 8 * 50 + 25; 
    playerY = 8 * 50 + 25; 

    CreateBox();
    InitializeEnemies();

    BeginBatchDraw();
    bool gameOver = false;

    while (!gameOver)
    {
        DrawMap();  

       
        DrawGiftBags();

       
        setfillcolor(BLACK);
        fillcircle(playerX, playerY, r);

       
        DrawEnemies();

        KeyControl();  

         MoveEnemies();

       
        if (CheckPlayerCollision()) {
            gameOver = true;
        }

        if (bombActive)
        {
            
            setfillcolor(RED);
            fillcircle(bombX * 50 + 25, bombY * 50 + 25, 20);

          
            bombTimer++;

           
            if (bombTimer >= bombDelay)
            {
               
                bool playerHit = ExplodeBomb();
                bombActive = false;  

                if (playerHit)
                {
                    gameOver = true;
                }
            }
        }

        FlushBatchDraw();
        Sleep(100);
        cleardevice();  
    }

    
    settextcolor(WHITE);
    settextstyle(40, 0, _T("Arial"));
    outtextxy(w / 2 - 150, h / 2 - 20, _T("Game Loss!"));

    FlushBatchDraw();
    Sleep(2000); 

    closegraph();
    return 0;
}
bool isWalkable(int i, int j)
{
    if (i < 0 || i >= 17 || j < 0 || j >= 17)
        return false;
    if ((i % 2 != 0) && (j % 2 != 0)) 
        return false;
    if (Box[i][j]) 
        return false;
    return true;
}

void KeyControl()
{
    int gridX = (playerX - 25) / 50;
    int gridY = (playerY - 25) / 50;

    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        if (gridY > 0 && isWalkable(gridX, gridY - 1))
        {
            playerY -= 50;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        if (gridY < 16 && isWalkable(gridX, gridY + 1))
        {
            playerY += 50;
        }
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if (gridX > 0 && isWalkable(gridX - 1, gridY))
        {
            playerX -= 50;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if (gridX < 16 && isWalkable(gridX + 1, gridY))
        {
            playerX += 50;
        }
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !bombActive)
    {
        bombX = gridX;
        bombY = gridY;
        bombActive = true;
        bombTimer = 0;
    }
}
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
                setfillcolor(RGB(190, 190, 190)); 
                setlinecolor(RGB(150, 150, 150));
                fillrectangle(left, top, right, bottom);
            }
            else if (Box[i][j])
            {
                int left_b = i * 50 + 5;
                int top_b = j * 50 + 5;
                int right_b = left_b + 40;
                int bottom_b = top_b + 40;
                setlinecolor(RGB(50, 50, 50)); 
                setfillcolor(RGB(50, 50, 50));
                fillrectangle(left_b, top_b, right_b, bottom_b);
            }
        }
    }
}

void DrawGiftBags()
{
    setfillcolor( WHITE); 
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (GiftBag[i][j])
            {
                fillcircle(i * 50 + 25, j * 50 + 25, 10); 
            }
        }
    }
}
void CreateBox()
{
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            if (((i + j) % 2 != 0) && (rand() % 3 == 0))
            {
                Box[i][j] = true;  

               
                if (rand() % 5 == 0) // 30% chance
                {
                    HasGiftBag[i][j] = true;
                }
            }
            else
            {
                Box[i][j] = false; 
            }
        }
    }
}
void InitializeEnemies() {
    for (int e = 0; e < MAX_ENEMIES; e++) {
        bool valid = false;
        while (!valid) {
            enemies[e].gridX = rand() % 17;
            enemies[e].gridY = rand() % 17;
           
            if (isWalkable(enemies[e].gridX, enemies[e].gridY) &&
                !(enemies[e].gridX == 8 && enemies[e].gridY == 8)) {
                enemies[e].x = enemies[e].gridX * 50 + 25;
                enemies[e].y = enemies[e].gridY * 50 + 25;
                enemies[e].direction = rand() % 4; 
                enemies[e].moveTimer = 0;
                enemies[e].targetX = enemies[e].x;
                enemies[e].targetY = enemies[e].y;
                valid = true;
            }
        }
    }
}
void MoveEnemies() {
    for (int e = 0; e < MAX_ENEMIES; e++) {
        Enemy& enemy = enemies[e];
        if (enemy.gridX == -1 && enemy.gridY == -1)
            continue;
        if (enemy.x == enemy.targetX && enemy.y == enemy.targetY)
        {
            std::vector<int> possibleDirections;
            if (enemy.gridY > 0 && isWalkable(enemy.gridX, enemy.gridY - 1) &&
                !(enemy.gridX == (playerX - 25) / 50 && (enemy.gridY - 1) == (playerY - 25) / 50))
                possibleDirections.push_back(0);
            if (enemy.gridY < 16 && isWalkable(enemy.gridX, enemy.gridY + 1) &&
                !(enemy.gridX == (playerX - 25) / 50 && (enemy.gridY + 1) == (playerY - 25) / 50))
                possibleDirections.push_back(1);
            if (enemy.gridX > 0 && isWalkable(enemy.gridX - 1, enemy.gridY) &&
                !((enemy.gridX - 1) == (playerX - 25) / 50 && enemy.gridY == (playerY - 25) / 50))
                possibleDirections.push_back(2);
           if (enemy.gridX < 16 && isWalkable(enemy.gridX + 1, enemy.gridY) &&
                !((enemy.gridX + 1) == (playerX - 25) / 50 && enemy.gridY == (playerY - 25) / 50))
                possibleDirections.push_back(3);
           if (!possibleDirections.empty())
           {
            int randIndex = rand() % possibleDirections.size();
                enemy.direction = possibleDirections[randIndex];

                switch (enemy.direction)
                {
                case 0: 
                    enemy.targetY -= 50;
                    break;
                case 1: 
                    enemy.targetY += 50;
                    break;
                case 2: 
                    enemy.targetX -= 50;
                    break;
                case 3: 
                    enemy.targetX += 50;
                    break;
                }
            }
            else
            {
                
                enemy.direction = -1;
            }
        }
        if (enemy.x < enemy.targetX)
            enemy.x += speed;
        else if (enemy.x > enemy.targetX)
            enemy.x -= speed;

        if (enemy.y < enemy.targetY)
            enemy.y += speed;
        else if (enemy.y > enemy.targetY)
            enemy.y -= speed;

       if (abs(enemy.x - enemy.targetX) < speed)
            enemy.x = enemy.targetX;
        if (abs(enemy.y - enemy.targetY) < speed)
            enemy.y = enemy.targetY;
         enemy.gridX = (enemy.x - 25) / 50;
        enemy.gridY = (enemy.y - 25) / 50;
    }
}
     void DrawEnemies() {
    setfillcolor(BLUE);
    for (int e = 0; e < MAX_ENEMIES; e++) {
       
        if (enemies[e].gridX != -1 && enemies[e].gridY != -1) {
            fillcircle(enemies[e].x, enemies[e].y, r);
        }
    }
}
    bool CheckPlayerCollision() {
    int playerGridX = (playerX - 25) / 50;
    int playerGridY = (playerY - 25) / 50;

    for (int e = 0; e < MAX_ENEMIES; e++) {
        if (enemies[e].gridX == playerGridX && enemies[e].gridY == playerGridY) {
            return true; 
        }
    }
    return false;
}
   bool CheckExplosionCollision() {
    bool enemyHit = false;
    if (!bombActive) return enemyHit;

    for (int e = 0; e < MAX_ENEMIES; e++) {
       
        if (enemies[e].gridX == -1 && enemies[e].gridY == -1)
            continue;
        if (enemies[e].gridX == bombX &&
            abs(enemies[e].gridY - bombY) <= bombRange)
        {
            
            enemies[e].gridX = -1; 
            enemies[e].x = -50;     
            enemies[e].y = -50;
            enemyHit = true;
            continue;
        }

       if (enemies[e].gridY == bombY &&
            abs(enemies[e].gridX - bombX) <= bombRange)
        {
           
            enemies[e].gridX = -1;
            enemies[e].x = -50;
            enemies[e].y = -50;
            enemyHit = true;
            continue;
        }
    }

    return enemyHit;
}


bool ExplodeBomb()
{
    bool playerHit = false;

   
    for (int k = -bombRange; k <= bombRange; k++)
    {
        
        if (bombY + k >= 0 && bombY + k < 17 && Box[bombX][bombY + k])
        {
            Box[bombX][bombY + k] = false; 

            if (HasGiftBag[bombX][bombY + k])
            {
                GiftBag[bombX][bombY + k] = true;
                HasGiftBag[bombX][bombY + k] = false; 
            }
        }
         if (bombX + k >= 0 && bombX + k < 17 && Box[bombX + k][bombY])
        {
            Box[bombX + k][bombY] = false; 

           
            if (HasGiftBag[bombX + k][bombY])
            {
                GiftBag[bombX + k][bombY] = true;
                HasGiftBag[bombX + k][bombY] = false; 
            }
        }
    }

   
    CheckExplosionCollision();

    
    int playerGridX = (playerX - 25) / 50;
    int playerGridY = (playerY - 25) / 50;

    if ((playerGridX == bombX && abs(playerGridY - bombY) <= bombRange) ||
        (playerGridY == bombY && abs(playerGridX - bombX) <= bombRange))
    {
        playerHit = true;
    }

   
    setfillcolor(WHITE); 
    for (int k = -bombRange; k <= bombRange; k++)
    {
       
        if (bombY + k >= 0 && bombY + k < 17)
        {
            fillrectangle(bombX * 50, (bombY + k) * 50, (bombX + 1) * 50, (bombY + k + 1) * 50);
        }
       if (bombX + k >= 0 && bombX + k < 17)
        {
            fillrectangle((bombX + k) * 50, bombY * 50, (bombX + k + 1) * 50, (bombY + 1) * 50);
        }
    }

    return playerHit;
}
    void CollectGiftBag()
{
    int playerGridX = (playerX - 25) / 50;
    int playerGridY = (playerY - 25) / 50;

    if (GiftBag[playerGridX][playerGridY])
    {
        bombRange += 1; 
        GiftBag[playerGridX][playerGridY] = false; 
    }
}

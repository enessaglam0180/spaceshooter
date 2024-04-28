#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
using namespace std;
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


class SpaceShooter
{
public:
    int width, height;
    int x, y;
    int score;
    bool gameover;

    struct Coordinate
    {
        int x, y;
    };

    vector<Coordinate> bullets;
    vector<Coordinate> enemies;
    
    SpaceShooter(int width, int height) : width(width), height(height), x(width / 2), y(height - 2), score(0), gameover(false) {}

    void drawPlayer()
    {

        gotoxy(x, y);
        cout << "<=^=>";
    }

    void erasePlayer()
    {
        gotoxy(x, y);
        cout << "     ";
    }

    void moveLeft()
    {
        if (x > 5)
            x -= 3;
    }

    void moveRight()
    {
        if (x < width - 5)
            x += 3;
    }

    void shoot()
    {
        bullets.push_back({x, y - 1});
    }

    void drawBullets()
    {
        for (int i = 0; i < bullets.size(); i++)
        {
            gotoxy(bullets[i].x + 2, bullets[i].y);
            cout << "|";
        }
    }

    void moveBullets()
    {
        for (int i = 0; i < bullets.size(); i++)
        {
            bullets[i].y--;

            // collision check with enemies
            for (int j = 0; j < enemies.size(); j++)
            {
                if ((enemies[j].x - 1) <= bullets[i].x <= (enemies[j].x + 1) && bullets[i].y == enemies[j].y)
                {
                    bullets.erase(bullets.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    score += 10;
                }
            }
            if (bullets[i].y <= 1)
            {
                bullets.erase(bullets.begin() + i);
            }
        }
    }

    void drawEnemies()
    {
        for (int i = 0; i < enemies.size(); i++)
        {
            gotoxy(enemies[i].x, enemies[i].y);
            cout << "(___)";
        }
    }

    void moveEnemies()
    {
        for (int i = 0; i < enemies.size(); i++)
        {
            enemies[i].y++;

            // collision check with player
            if (enemies[i].x == x && enemies[i].y == y - 1)
            {
                gameover = true;
                break;
            }

            if (enemies[i].y >= height - 1)
            {
                enemies.erase(enemies.begin() + i);
                i--;
            }
        }
    }

    void generateEnemies()
    {
        // chance of generating an enemy with 5% percentage in each frame
        if (rand() % 100 < 5)
        {
            int enemyX = rand() % (width - 2) + 1;
            enemies.push_back({enemyX, 1});
        }
    }

    bool isGameOver() const
    {
        return gameover;
    }

    int getScore() const
    {
        return score;
    }
};

int main()
{
    system("mode con: lines=40 cols=40");

    // hide cursor
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = FALSE;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    srand(time(0));
    int width = 40, height = 40;
    
    SpaceShooter game(width, height);

    cout << "Instructions" << endl;
    cout << "************" << endl;
    cout << "1.Use RIGHT and LEFT arrow keys for move" << endl;
    cout << "2.Enter SPACE for fire" << endl;
    cout << "3.Enter ESC to quit game" << endl
         << endl;

    system("pause");

    while (!game.isGameOver())
    {
        
        
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 75)
                game.moveLeft();
            else if (ch == 77)
                game.moveRight();
            else if (ch == ' ')
                game.shoot();
            else if (ch == 27)
                break;
        }

        game.generateEnemies();
        game.drawPlayer();
        game.drawBullets();
        game.drawEnemies();

        game.moveBullets();
        game.moveEnemies();

        Sleep(50);

        system("cls");
        
    }
    gotoxy(width / 2 - 5, height / 2);
    cout << "Game Over!";

    gotoxy(width / 2 - 7, height / 2 + 1);
    cout << "Your Score: " << game.getScore() << endl;
 
    return 0;
}
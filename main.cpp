// Settings for Windows

#include "Enemy.h"
#include "Player.h"
#include "Object.h"
#include "Position.h"
#include <time.h>

// Settings for MacBook
/*
#include "Enemy.cpp"
#include "Player.cpp"
#include "Object.cpp"
#include "Position.cpp"
#include <time.h>
*/

void board(char map[][10], int size);

int main()
{
	// Settings of the code
	srand((unsigned)time(NULL));
	setlocale(LC_ALL, "");

	// Creating the map
	char map[10][10]{};
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j] = '*';
		}
	}

	// Creating you :)
	Player *p1 = new Player();
	int prevX = p1->xLocation();
	int prevY = p1->yLocation();
	map[p1->xLocation()][p1->yLocation()] = 'P';

	// Creting enemies >:(
	int random = 0;
	random = rand() % 9 + 1;

	Enemy *enemy = new Enemy(random, random / 2);
	Position *p = new Position(enemy->xLocation(), enemy->yLocation());
	p1->add(p);
	map[enemy->xLocation()][enemy->yLocation()] = '#';

	random = rand() % 9 + 1;
	Enemy* enemy2 = new Enemy(random / 2, random);
	Position* p2 = new Position(enemy2->xLocation(), enemy2->yLocation());
	p1->add(p2);
	map[enemy2->xLocation()][enemy2->yLocation()] = '#';

	// Gameplay condition
	bool game = 1;
	while (game)
	{
		// clears terminal, so I can draw the map now
		cout << "\033[2J\033[H";
		board(map, 10);
		// Shows my health
		cout << "Your health: " << p1->checkHealth() << endl;
		// Player takes a step
		prevX = p1->xLocation();
		prevY = p1->yLocation();
		p1->step();
		map[p1->xLocation()][p1->yLocation()] = 'P';
		map[prevX][prevY] = '*';
		// Checks for collision
		if (p1->isCollided(p1->xLocation(), p1->yLocation()))
		{
			cout << "You encounter an enemy!" << endl;
			// Checks is you survived the encounter
			if (p1->checkHealth() > 0)
			{
				cout << "You are still alive!\n";
				// Checks if there are enemies still alive
				if (p1->winner())
				{
					cout << p1->checkHealth() << endl;
					cout << "Congtrats!!! You defeated all of yours enemies!";
					game = 0;
				}
				cout << "Move forward and don't look back..." << endl;
			}
			else
			{
				cout << p1->checkHealth() << endl;
				cout << "Game Over!\nYou died before defeating all of your enemies\n";
				game = 0;
			}
		}
		else
		{
			cout << "You didn't encounter an enemy!" << endl;
			cout << "You should move along..." << endl;
		}
	}
}

void board(char map[][10], int size)
{
	for (int i = size-1; i >= 0; i--)
	{
		for (int j = size-1; j >= 0; j--)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
}
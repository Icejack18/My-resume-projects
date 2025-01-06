// Settings for Windows
#include "Enemy.h"
#include "Player.h"
#include "Object.h"
#include "Position.h"

// Settings for MacBook
#include "Enemy.cpp"
#include "Player.cpp"
#include "Object.cpp"
#include "Position.cpp"

#include <time.h>

void board(char map[][10], int size);

int main()
{
	// Settings of the code
	srand((unsigned)time(NULL));
	setlocale(LC_ALL, "");

	// Creating the map
	char map[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j] = '.';
		}
	}

	// Creating you :D
	Player *p_player = new Player();
	int prevX = p_player->xLocation();
	int prevY = p_player->yLocation();
	map[p_player->xLocation()][p_player->yLocation()] = 'P';

	// Creating health packs :)

	// Creting enemies >:(
	int randomX = 0, randomY = 0;
	randomX = rand() % 8 + 1;
	randomY = rand() % 9;

	Enemy *troll = new Enemy(randomX, randomY, 15, 10);
	Position *p_troll = new Position(troll->xLocation(), troll->yLocation());
	p_player->add(p_troll);
	map[troll->xLocation()][troll->yLocation()] = '#';

	randomX = rand() % 9;
	randomY = rand() % 8 + 1;
	Enemy* goblin = new Enemy(randomX, randomY, 5, 5);
	Position* p_goblin = new Position(goblin->xLocation(), goblin->yLocation());
	p_player->add(p_goblin);
	map[goblin->xLocation()][goblin->yLocation()] = '#';

	// Gameplay loop + condition
	bool game = 1;
	while (game)
	{
		// clears terminal, so I can draw the map after each move in top corner
		cout << "\033[2J\033[H";

		// Draws the map
		board(map, 10);

		// Shows health
		cout << "Your health: " << p_player->checkHealth() << endl;
		cout << "Troll's health: " << troll->checkHealth() << endl;
		cout << "Goblin's health: " << goblin->checkHealth() << endl;

		// Player takes a step and we keeping the previous step coordinates
		prevX = p_player->xLocation();
		prevY = p_player->yLocation();
		p_player->step();
		map[p_player->xLocation()][p_player->yLocation()] = 'P';
		map[prevX][prevY] = '.';

		// Checks for collision
		if (p_player->isCollided(troll) || p_player->isCollided(goblin))
		{
			cout << "You encounter an enemy!" << endl;
			// Checks is you survived the encounter
			if (p_player->checkHealth() > 0)
			{
				cout << "You are still alive!\n";
				// Checks if there are enemies still alive
				if (p_player->winner())
				{
					cout << p_player->checkHealth() << endl;
					cout << "Congtrats!!! You defeated all of yours enemies!\n";
					game = 0;
				}
				cout << "Move forward and don't look back..." << endl;
			}
			else
			{
				cout << p_player->checkHealth() << endl;
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
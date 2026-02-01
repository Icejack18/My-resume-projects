// Settings for Windows
#include "Enemy.h"
#include "Player.h"
#include "Object.h"
#include "Position.h"
#include "Healthpack.h"

// Settings for MacBook
#include "Enemy.cpp"
#include "Player.cpp"
#include "Object.cpp"
#include "Position.cpp"
#include "Healthpack.cpp"

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
	Player *player = new Player();
	int prevX = player->xLocation();
	int prevY = player->yLocation();
	map[player->xLocation()][player->yLocation()] = 'P';

	// Creating health packs :)
	int randomX = 0, randomY = 0;
	randomX = rand() % 8 + 1;
	randomY = rand() % 9;

	Healthpack *healthpack = new Healthpack(randomX, randomY, 2);
	Position *p_healthpack = new Position(healthpack->xLocation(), healthpack->yLocation());
	player->addHealthpack(p_healthpack);
	map[healthpack->xLocation()][healthpack->yLocation()] = 'H';

	// Creting enemies >:(
	randomX = rand() % 8 + 1;
	randomY = rand() % 9;

	Enemy *troll = new Enemy(randomX, randomY, 15, 10);
	Position *p_troll = new Position(troll->xLocation(), troll->yLocation());
	player->addEnemy(p_troll);
	map[troll->xLocation()][troll->yLocation()] = 'T';

	randomX = rand() % 9;
	randomY = rand() % 8 + 1;
	Enemy* goblin = new Enemy(randomX, randomY, 5, 5);
	Position* p_goblin = new Position(goblin->xLocation(), goblin->yLocation());
	player->addEnemy(p_goblin);
	map[goblin->xLocation()][goblin->yLocation()] = 'G';

	// Gameplay loop
	while (true)
	{
		// clears terminal, so I can draw the map after each move in top corner
		cout << "\033[2J\033[H";

		// Draws the map
		board(map, 10);

		// Checks for collision with Enemies
		if (player->isCollided(troll) || player->isCollided(goblin))
		{
			cout << "You encounter an enemy!" << endl;
			// Checks is you survived the encounter
			if (player->checkHealth() > 0)
			{
				cout << "You are still alive!\n";
				// Checks if there are enemies still alive
				if (player->winner())
				{
					cout << "Congtrats!!! You defeated all of yours enemies!\n";
					break;
				}
				cout << "Move forward and don't look back..." << endl;
			}
			else
			{
				cout << "Game Over!\nYou died before defeating all of your enemies\n";
				break;
			}
		}
		else if (player->isCollided(healthpack))
		{
			cout << "Congrats! You found health pack! \nYour hp was increased. Now go on\n";
		}
		else
		{
			cout << "You look around and do not see anything interesting." << endl;
			cout << "You should continue with your journey" << endl;
		}

		// Shows health
		cout << "Your health: " << player->checkHealth() << endl;
		cout << "Troll's health: " << troll->checkHealth() << endl;
		cout << "Goblin's health: " << goblin->checkHealth() << endl;

		// Player takes a step and we keeping the previous step coordinates
		prevX = player->xLocation();
		prevY = player->yLocation();
		player->step();
		map[player->xLocation()][player->yLocation()] = 'P';
		map[prevX][prevY] = '.';
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
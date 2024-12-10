#include "Player.h"

Player::Player() : Object(0, 0)
{
	PosX = 0;
	PosY = 0;
	health = 10;
}

Player::~Player(void)
{
}

void Player::step()
{
	int ans;
	bool test = 1;
	do
	{
		if (xLocation() > 0 && yLocation() > 0 && xLocation() == 9 && yLocation() == 9){
			cout << "Do you wanna go: (3)RIGHT or (4)DOWN ? \n >>> ";
			if(cin >> ans && ans > 2 && ans < 5)
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() > 0 && yLocation() == 0 && xLocation() == 9 && yLocation() < 9) {
			
			cout << "Do you wanna go: (2)LEFT or (4)DOWN ? \n >>> ";
			if (cin >> ans && (ans == 2 || ans == 4))
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() == 0 && yLocation() == 0 && xLocation() < 9 && yLocation() < 9){
			cout << "Do you wanna go: (1)UP or (2)LEFT ? \n >>> ";
			if (cin >> ans && ans > 0 && ans < 3)
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() == 0 && yLocation() > 0 && xLocation() < 9 && yLocation() == 9) {
			cout << "Do you wanna go: (1)UP or (3)RIGHT ? \n >>> ";
			if (cin >> ans && (ans == 1 || ans == 3))
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() == 0 && yLocation() > 0 && xLocation() < 9 && yLocation() < 9){
			cout << "Do you wanna go: (1)UP, (2)LEFT or (3)RIGHT ? \n >>> ";
			if(cin >> ans && ans > 0 && ans < 4)
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() > 0 && yLocation() == 0 && xLocation() < 9 && yLocation() < 9){
			cout << "Do you wanna go: (1)UP, (2)LEFT or (4)DOWN ? \n >>> ";
			if(cin >> ans && (ans == 1 || ans == 2 || ans == 4))
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() > 0 && yLocation() > 0 && xLocation() < 9 && yLocation() == 9){
			
			cout << "Do you wanna go: (1)UP, (3)RIGHT or (4)DOWN ? \n >>> ";
			if (cin >> ans && (ans == 1 || ans == 3 || ans == 4))
				test = 0;
			else cin.ignore();
		}
		else if (xLocation() > 0 && yLocation() > 0 && xLocation() == 9 && yLocation() < 9){
			cout << "Do you wanna go: (2)LEFT, (3)RIGHT or (4)DOWN ? \n >>> ";
			if (cin >> ans && ans > 1 && ans < 5)
				test = 0;
			else cin.ignore();
		}
		else{
			cout << "Do you wanna go: (1)UP, (2)LEFT, (3)RIGHT or (4)DOWN ? \n >>> ";
			if (cin >> ans && ans > 0 && ans < 5)
				test = 0;
			else cin.ignore();
		}
	} while (test);

	switch (ans)
	{
	case 1:
		PosX++;
		break;
	case 2:
		PosY++;
		break;
	case 3:
		PosY--;
		break;
	case 4:
		PosX--;
		break;
	}
}

bool Player::winner()
{
	if (position.empty())
		return true;
	else
		return false;
}

void Player::print()
{
	for (int i = 0; i < position.size(); i++)
	{
		cout << position[i]->getX() << ", " << position[i]->getY() << endl;
	}
}

bool Player::isCollided(int xP, int yP)
{
	for (int i = 0; i < position.size(); i++)
	{
		Enemy *en = new Enemy(position[i]->getX(), position[i]->getY());
		if (en->isCollided(xP, yP))
		{
			int damage = 0;
			damage = rand() % 11;
			health -= damage;
			delete position[i];
			position.erase(position.begin() + i);
			delete en;
			return true;
		}
		delete en;
	}
	return false;
}

void Player::add(Position *p)
{
	position.push_back(p);
}

int Player::checkHealth()
{
	return health;
}

int Player::xLocation()
{
	return PosX;
}

int Player::yLocation()
{
	return PosY;
}

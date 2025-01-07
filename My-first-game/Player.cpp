#include "Player.h"

Player::Player() : Object(0, 0)
{
	PosX = 0;
	PosY = 0;
	health = 10;
	power = 10;
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
	if (p_enemies.empty())
		return true;
	else
		return false;
}

bool Player::isCollided(Enemy *e)
{
	for (int i = 0; i < p_enemies.size(); i++)
	{
		if (e->xLocation() == xLocation() && e->yLocation() == yLocation())
		{
			int damageP = 0, damageE = 0;
			do
			{
			damageP = rand() % e->checkPower();
			damageE = rand() % power;
			health -= damageP;
			e->damage(damageE);
			} while (e->checkHealth() > 0 && health > 0);

			if(e->checkHealth() <= 0){
			delete p_enemies[i];
			p_enemies.erase(p_enemies.begin() + i);
			}
			else cout << "Enemy has defeated you!\n";
			return true;
		}
	}
	return false;
}

bool Player::isCollided(Healthpack *hp)
{
    for (int i = 0; i < p_healthpacks.size(); i++)
	{
		if (hp->xLocation() == xLocation() && hp->yLocation() == yLocation())
		{
			health += hp->addedHealth();
			return true;
		}
	}
	return false;
}

void Player::addEnemy(Position *p)
{
	p_enemies.push_back(p);
}

void Player::addHealthpack(Position *p)
{
	p_healthpacks.push_back(p);
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

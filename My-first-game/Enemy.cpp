#include "Enemy.h"

Enemy::Enemy(int xP, int yP) : Object(xP, yP)
{
	Position *posE = new Position(xP, yP);
	position.push_back(posE);
	PosX = xP;
	PosY = yP;
	hp = 2;
}

Enemy::~Enemy(void)
{
}

bool Enemy::isCollided(int xP, int yP)
{
	bool isHit = false;
	double dx = xLocation() - xP;
	double dy = yLocation() - yP;

	if (dx == 0 && dy == 0)
	{

		isHit = true;
	}
	return isHit;
}

int Enemy::xLocation()
{
	return PosX;
}

int Enemy::yLocation()
{
	return PosY;
}
#include "Enemy.h"

Enemy::Enemy(int xP, int yP, int h) : Object(xP, yP)
{
	PosX = xP;
	PosY = yP;
	hp = h;
}

Enemy::~Enemy(void)
{
}

void Enemy::damage(int d)
{
	hp -= d;
}

int Enemy::checkHealth()
{
    return hp;
}

int Enemy::xLocation()
{
	return PosX;
}

int Enemy::yLocation()
{
	return PosY;
}
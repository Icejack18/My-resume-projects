#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(int xP, int yP);
	~Enemy(void);
	bool isCollided(int xP, int yP);
	int xLocation();
	int yLocation();

protected:
	int PosX;
	int PosY;
	int hp;
};
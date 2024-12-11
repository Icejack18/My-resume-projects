#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(int xP, int yP, int h);
	~Enemy(void);
	void damage(int d);
	int checkHealth();
	int xLocation();
	int yLocation();

protected:
	int PosX;
	int PosY;
	int hp;
};
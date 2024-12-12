#pragma once
#include "Object.h"

class Enemy : public Object
{
public:
	Enemy(int xP, int yP, int h, int p);
	~Enemy(void);
	void damage(int d);
	int checkHealth();
	int checkPower();
	int xLocation();
	int yLocation();

protected:
	int PosX;
	int PosY;
	int hp;
	int pwr;
};
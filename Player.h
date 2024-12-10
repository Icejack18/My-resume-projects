#pragma once
#include "Object.h"
#include "Enemy.h"

class Player : public Object
{
public:
	Player();
	~Player(void);
	void step();
	bool winner();
	void print();
	bool isCollided(int xP, int yP);
	void add(Position *p);
	int checkHealth();
	int xLocation();
	int yLocation();

protected:
	int PosX;
	int PosY;
	int health;
};
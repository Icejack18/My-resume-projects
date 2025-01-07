#pragma once
#include "Object.h"
#include "Enemy.h"
#include "Healthpack.h"

class Player : public Object
{
public:
	Player();
	~Player(void);
	void step();
	bool winner();
	bool isCollided(Enemy *e);
	bool isCollided(Healthpack *hp);
	void addEnemy(Position *p);
	void addHealthpack(Position *p);
	int checkHealth();
	int xLocation();
	int yLocation();

protected:
	int PosX;
	int PosY;
	int health;
	int power;
};
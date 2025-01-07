#pragma once
#include "Object.h"

class Healthpack : public Object
{
public:
	Healthpack(int xP, int yP, int pP);
    ~Healthpack(void);
	int addedHealth();
	int xLocation();
	int yLocation();

protected:
	int PosX;
	int PosY;
    int packPower;
};
#pragma once
#include "Position.h"

class Object
{
public:
	Object(int xi, int yi);
	virtual ~Object(void);
	virtual int checkHealth() = 0;
	virtual int xLocation() = 0;
	virtual int yLocation() = 0;

protected:
	vector<Position *> position;
};

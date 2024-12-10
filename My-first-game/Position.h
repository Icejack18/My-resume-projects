#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Position
{
public:
	Position(int ix, int iy);
	void setX(int ix);
	void setY(int iy);
	int getX();
	int getY();

protected:
	int xPosition;
	int yPosition;
};
#include "Position.h"

Position::Position(int ix, int iy)
{
	xPosition = ix;
	yPosition = iy;
}

void Position::setX(int ix)
{
	xPosition = ix;
}

void Position::setY(int iy)
{
	yPosition = iy;
}

int Position::getX()
{
	return xPosition;
}

int Position::getY()
{
	return yPosition;
}
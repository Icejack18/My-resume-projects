#include "Object.h"

Object::Object(int xi, int yi)
{
	Position *p = new Position(xi, yi);
}

Object::~Object(void)
{
}

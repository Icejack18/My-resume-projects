#include "Heathpack.h"

Healthpack::Healthpack(int xP, int yP, int pP) : Object (xP, yP)
{
    packPower = pP;
}

Healthpack::~Healthpack(void)
{
}

int Healthpack::xLocation()
{
    return PosX;
}

int Healthpack::yLocation()
{
    return PosY;
}

#include "Healthpack.h"

Healthpack::Healthpack(int xP, int yP, int pP) : Object (xP, yP)
{
    PosX = xP;
    PosY = yP;
    packPower = pP;
}

Healthpack::~Healthpack(void)
{
}

int Healthpack::addedHealth()
{
    return packPower;
}

int Healthpack::xLocation()
{
    return PosX;
}

int Healthpack::yLocation()
{
    return PosY;
}

#pragma once

#include "LogicalGameObjects/Gravity/Planet.h"

class PlanetSrv : public Planet
{
public:
    PlanetSrv(sf::Vector2f pos, int radius, int power);
};
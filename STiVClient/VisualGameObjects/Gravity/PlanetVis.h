#pragma once

#include "LogicalGameObjects/Gravity/Planet.h"

class PlanetVis : public Planet
{
public:
    PlanetVis(sf::Vector2f pos, int radius, int power);
};
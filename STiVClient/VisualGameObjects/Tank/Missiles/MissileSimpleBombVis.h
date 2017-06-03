#pragma once

#include "LogicalGameObjects/Tank/Missiles/MissileSimpleBomb.h"

class MissileSimpleBombVis : public MissileSimpleBomb
{
public:
    MissileSimpleBombVis(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


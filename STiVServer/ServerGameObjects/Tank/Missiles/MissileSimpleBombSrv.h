#pragma once

#include "LogicalGameObjects/Tank/Missiles/MissileSimpleBomb.h"

class MissileSimpleBombSrv : public MissileSimpleBomb
{
public:
	MissileSimpleBombSrv(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


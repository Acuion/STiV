#pragma once

#include "LogicalGameObjects/Tank/Missiles/MissileSniper.h"

class MissileSniperVis : public MissileSniper
{
public:
	MissileSniperVis(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


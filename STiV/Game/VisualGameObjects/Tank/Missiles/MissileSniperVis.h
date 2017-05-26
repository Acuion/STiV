#pragma once

#include "Game/LogicalGameObjects/Tank/Missiles/TankMissile.h"

class MissileSniper : public TankMissile
{
public:
	const static float mCd;
	MissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


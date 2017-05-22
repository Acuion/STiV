#pragma once

#include "TankMissile.h"

class MissileSniper : public TankMissile
{
public:
	const static float mCd;
	MissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


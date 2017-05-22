#pragma once

#include "TankMissile.h"

class MissileSimpleBomb : public TankMissile 
{
public:
	const static float mCd;
	MissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


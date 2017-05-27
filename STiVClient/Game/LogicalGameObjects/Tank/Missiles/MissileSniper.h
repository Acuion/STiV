#pragma once

#include "Game/LogicalGameObjects/Tank/Missiles/TankMissile.h"

class MissileSniper : public TankMissile
{
protected:
	MissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
	virtual ~MissileSniper();
public:
	const static float mCd;
};


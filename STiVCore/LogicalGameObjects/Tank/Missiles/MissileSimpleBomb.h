#pragma once

#include "LogicalGameObjects/Tank/Missiles/TankMissile.h"

class MissileSimpleBomb : public TankMissile
{
protected:
    MissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
    virtual ~MissileSimpleBomb();
public:
    const static float mCd;
};


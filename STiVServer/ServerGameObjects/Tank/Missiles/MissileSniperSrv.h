#pragma once

#include "LogicalGameObjects/Tank/Missiles/MissileSniper.h"

class MissileSniperSrv : public MissileSniper
{
public:
    MissileSniperSrv(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


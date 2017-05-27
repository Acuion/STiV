#pragma once

#include "Game/LogicalGameObjects/Tank/Missiles/MissileSniper.h"

class MissileSniperVis : public MissileSniper
{
public:
	MissileSniperVis(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);//todo: virt destr in logical set here a sprite
};


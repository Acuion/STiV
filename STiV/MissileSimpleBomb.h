// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include "TankMissile.h"

class MissileSimpleBomb : public TankMissile 
{
public:
	const static float mCd;
	MissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel);
};


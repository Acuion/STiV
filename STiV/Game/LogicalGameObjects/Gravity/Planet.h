#pragma once

#include "GravityPoint.h"
#include "Game/LogicalGameObjects/GameObject.h"

class Planet : public GameObject
{
	GravityPoint mGravityPoint;
protected:
	Planet(sf::Vector2f pos, int radius, int power);
	virtual ~Planet();
};

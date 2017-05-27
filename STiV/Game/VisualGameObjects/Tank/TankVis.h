#pragma once

#include "Game/LogicalGameObjects/Tank/Tank.h"

class TankVis : public Tank
{
	~TankVis();
public:
	TankVis(sf::Vector2f pos, bool client = false);
	TankVis();
};

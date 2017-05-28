#pragma once

#include "LogicalGameObjects/Tank/Tank.h"

class TankSrv : public Tank
{
public:
	TankSrv(sf::Vector2f pos, bool client = false);
	TankSrv();
	~TankSrv();
};

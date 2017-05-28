#include "ServerGameObjects/BonusSrv.h"
#include "LogicalGameObjects/Tank/Missiles/TankMissile.h"

BonusSrv::BonusSrv(BonusType bt, int value, sf::Vector2f pos)
	: Bonus(bt, value, pos)
{
}

#pragma once

#include "GameObject.h"
#include "TankMissile.h"

enum BonusType
{
	bt_heal,
	bt_weapon,
};

class Bonus : public GameObject
{
	BonusType mBonusType;
	int mValue;//HP / weaponNum
public:
	Bonus(BonusType bt, int value, sf::Vector2f pos);
	BonusType getType();
	int getVal();
};


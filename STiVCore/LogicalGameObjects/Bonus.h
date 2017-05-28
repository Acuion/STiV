#pragma once

#include "LogicalGameObjects/GameObject.h"

enum BonusType
{
	bt_heal,
	bt_weapon,
};

class Bonus : public GameObject
{
	BonusType mBonusType;
	int mValue;//HP / weaponNum
protected:
	Bonus(BonusType bt, int value, sf::Vector2f pos);
	virtual ~Bonus();
public:
	BonusType getType();
	int getVal();
};


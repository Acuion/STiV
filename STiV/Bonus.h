// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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


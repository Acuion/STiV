#pragma once

#include "LogicalGameObjects/Bonus.h"

class BonusVis : public Bonus
{
public:
	BonusVis(BonusType bt, int value, sf::Vector2f pos);
};


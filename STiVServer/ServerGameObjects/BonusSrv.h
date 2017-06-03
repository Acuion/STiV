#pragma once

#include "LogicalGameObjects/Bonus.h"

class BonusSrv : public Bonus
{
public:
    BonusSrv(BonusType bt, int value, sf::Vector2f pos);
};


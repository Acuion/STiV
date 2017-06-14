#pragma once

#include "LogicalGameObjects/Tank/Tank.h"

class TankVis : public Tank
{
public:
    TankVis(sf::Vector2f pos, const sf::Font& font, const std::string& nickname);
    TankVis();
    ~TankVis();

    void update() override;
    bool tryToLaunchMissile() override;
private:
    void setMissle(MissleType mt) override;

    Sprite* mBarrel = nullptr;
    sf::Text mNickname;
};

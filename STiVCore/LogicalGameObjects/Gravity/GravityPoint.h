#pragma once
#include <SFML/System/Vector2.hpp>

class GameObject;

class GravityPoint
{
protected:
    int mPower;
    sf::Vector2f mPosition;

public:
    GravityPoint(sf::Vector2f pos, int power);
    virtual ~GravityPoint();

    void affect(GameObject & go) const;
};

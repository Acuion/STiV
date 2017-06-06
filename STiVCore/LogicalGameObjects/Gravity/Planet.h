#pragma once

#include "LogicalGameObjects/Gravity/GravityPoint.h"
#include "LogicalGameObjects/GameObject.h"

class Planet : public GameObject
{
public:
    sf::Int32 getRadius() const;
    sf::Int32 getPower() const;
protected:
    Planet(sf::Vector2f pos, int radius, int power);
    virtual ~Planet();
private:
    GravityPoint mGravityPoint;
    sf::Int32 mRadius, mPower;
};

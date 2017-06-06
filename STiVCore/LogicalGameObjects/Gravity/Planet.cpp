#include "LogicalGameObjects/Gravity/Planet.h"
#include "Misc/Utilites.h"

Planet::Planet(sf::Vector2f pos, int radius, int power)
    : mGravityPoint(pos, power)
    , mRadius(radius)
    , mPower(power)
{
    auto planetbdef = new b2BodyDef();
    planetbdef->type = b2_staticBody;
    planetbdef->position.Set(pos.x * Utilites::b2scale, pos.y * Utilites::b2scale);
    auto planetfdef = new b2FixtureDef();
    b2CircleShape* planetcs = new b2CircleShape();
    planetcs->m_radius = radius * Utilites::b2scale;
    planetfdef->shape = planetcs;

    create(ObjectRealType::rt_Static, planetbdef, planetfdef, 100, false);
}

Planet::~Planet()
{
}

sf::Int32 Planet::getRadius() const
{
    return mRadius;
}

sf::Int32 Planet::getPower() const
{
    return mPower;
}

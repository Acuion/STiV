#include "Planet.h"

Planet::Planet(sf::Vector2f pos, int radius, int power)
	: mGravityPoint(pos, power)
{
	auto planetbdef = new b2BodyDef();
	planetbdef->type = b2_staticBody;
	planetbdef->position.Set(pos.x * tgMath::b2scale, pos.y * tgMath::b2scale);
	auto planetfdef = new b2FixtureDef();
	b2CircleShape* planetcs = new b2CircleShape();
	planetcs->m_radius = radius * tgMath::b2scale;
	planetfdef->shape = planetcs;

	create(ObjectRealType::rt_Static, planetbdef, planetfdef, 100, false);
}

Planet::~Planet()
{
}

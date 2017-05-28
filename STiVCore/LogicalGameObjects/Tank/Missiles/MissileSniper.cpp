#include "LogicalGameObjects/Tank/Missiles/MissileSniper.h"
#include "LogicalGameObjects/Tank/Missiles/MissileSimpleBomb.h"

const float MissileSniper::mCd = 2000;

MissileSniper::MissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
{
	b2FixtureDef* fdef = MaterialStorage::getMaterial("default");
	b2CircleShape* cs = new b2CircleShape();
	cs->m_radius = 6 * tgMath::b2scale;
	fdef->shape = cs;

	create(fdef, 100, pos, angle, 100, 10000, tankLinVel, true);
}

MissileSniper::~MissileSniper()
{
}

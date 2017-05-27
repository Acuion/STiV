#include "MissileSniper.h"
#include "MissileSimpleBomb.h"

const float MissileSniper::mCd = 2000;

MissileSniper::MissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
{
	b2FixtureDef* fdef = MaterialStorage::getMaterial("default");
	b2CircleShape* cs = new b2CircleShape();
	cs->m_radius = 6 * tgMath::b2scale;
	fdef->shape = cs;

	create(Sprite("missiles\\sniperMissile.png", pos,
        2, { Sprite::Animation(2, 30), Sprite::Animation(3, 30, PostProcessingManager::MultitargetShaderType::Explosion) },
        { 25, 24 }), fdef, 100, pos, angle, 100, 10000, tankLinVel, true);
}

MissileSniper::~MissileSniper()
{
}

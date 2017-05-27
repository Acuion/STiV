#include "MissileSimpleBomb.h"

const float MissileSimpleBomb::mCd = 200;

MissileSimpleBomb::MissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
{
	b2FixtureDef* fdef = MaterialStorage::getMaterial("default");
	b2CircleShape* cs = new b2CircleShape();
	cs->m_radius = 10 * tgMath::b2scale;
	fdef->shape = cs;

	create(Sprite("missiles\\simpleBombMissile.png", pos,
        2, { Sprite::Animation(3, 2), Sprite::Animation(3, 30, PostProcessingManager::MultitargetShaderType::Explosion) }, { 20, 20 }),
        fdef, 15, pos, angle, 10, 2000, tankLinVel);
}

MissileSimpleBomb::~MissileSimpleBomb()
{
}

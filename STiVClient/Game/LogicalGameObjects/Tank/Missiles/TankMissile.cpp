#include "TankMissile.h"
#include "Game/VisualGameObjects/Visualizer.h"

void TankMissile::create(b2FixtureDef* fdef, float force, const sf::Vector2f& pos, float angle,
    int damage, float kick, b2Vec2 tankLinVel, bool fixedRotation)
{
	b2BodyDef* bdef = new b2BodyDef();
	bdef->type = b2_dynamicBody;

	mDamage = damage;
	mBody = GameObjectManager::registerObject(bdef, fdef, this, ObjectRealType::rt_Missile);
	mBody->SetLinearVelocity(tankLinVel + b2Vec2(force * cos(angle), force * sin(angle)));
	mBody->SetTransform({ pos.x * tgMath::b2scale, pos.y * tgMath::b2scale }, angle);
	mBody->SetBullet(true);
	mBody->SetFixedRotation(fixedRotation);

	mKick = kick;
}

void TankMissile::onColide(ObjectRealTypeData* with)
{
	with->second->applyForce(mDamage * 100, tgMath::atan2Points(getPosition(), with->second->getPosition()));
	mBody->SetLinearVelocity({ 0,0 });
	mHP = 0;
	with->second->doDamage(mDamage);
}

TankMissile::TankMissile()
	: GameObject()
{
	mHP = 50;
}

TankMissile::~TankMissile()
{
}

float TankMissile::getKick()
{
	return -mKick;
}

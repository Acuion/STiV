#include "Game/LogicalGameObjects/Bonus.h"
#include "Game/LogicalGameObjects/Tank/Missiles/TankMissile.h"

Bonus::Bonus(BonusType bt, int value, sf::Vector2f pos)
{
	mBonusType = bt;
	mValue = value;

	b2BodyDef* bdef = new b2BodyDef();
	bdef->type = b2_dynamicBody;
	bdef->position.Set(pos.x * tgMath::b2scale, pos.y * tgMath::b2scale);

	b2FixtureDef* fdef = MaterialStorage::getMaterial("missle");
	b2CircleShape* cs = new b2CircleShape();
	cs->m_radius = 25 * tgMath::b2scale;
	fdef->shape = cs;

	create(ObjectRealType::rt_Bonus, bdef, fdef, 30, true);
	applyForce(1000, (rand() % 600) * 0.01);
}

Bonus::~Bonus()
{
}

BonusType Bonus::getType()
{
	return mBonusType;
}

int Bonus::getVal()
{
	return mValue;
}

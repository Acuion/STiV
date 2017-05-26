#include "Game/LogicalGameObjects/Bonus.h"
#include "Game/LogicalGameObjects/Tank/Missiles/TankMissile.h"

Bonus::Bonus(BonusType bt, int value, sf::Vector2f pos)
{
	mBonusType = bt;
	mValue = value;
	Sprite sprt;
	switch (bt)
	{
	case BonusType::bt_heal:
		sprt = Sprite("bonuses\\heal.png", pos, 2, { Sprite::Animation(2, 1), Sprite::Animation(2, 18) }, { 50, 50 });
		break;
	case BonusType::bt_weapon:
		switch (value)
		{
		case MissleType::simpleBomb:
			sprt = Sprite("bonuses\\weapon_simpleBomb.png", pos, 2, { Sprite::Animation(2, 1), Sprite::Animation(2, 18) }, { 50, 50 });
			break;
		case MissleType::sniper:
			sprt = Sprite("bonuses\\weapon_sniper.png", pos, 2, { Sprite::Animation(2, 1), Sprite::Animation(2, 18) }, { 50, 50 });
			break;
		}
		break;
	}

	b2BodyDef* bdef = new b2BodyDef();
	bdef->type = b2_dynamicBody;
	bdef->position.Set(pos.x * tgMath::b2scale, pos.y * tgMath::b2scale);

	b2FixtureDef* fdef = MaterialStorage::getMaterial("missle");
	b2CircleShape* cs = new b2CircleShape();
	cs->m_radius = 25 * tgMath::b2scale;
	fdef->shape = cs;

	create(ObjectRealType::rt_Bonus, sprt, bdef, fdef, 30, true);
	applyForce(1000, (rand() % 600) * 0.01);
}

BonusType Bonus::getType()
{
	return mBonusType;
}

int Bonus::getVal()
{
	return mValue;
}

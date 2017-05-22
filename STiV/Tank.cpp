#include "Tank.h"

void Tank::setMissle(MissleType mt)
{
	mCurrMissleType = mt;
}

void Tank::launchMissile()
{
	TankMissile* fired = 0;
	switch (mCurrMissleType)
	{
	case MissleType::simpleBomb:
		if (clock() - mLastFire < MissileSimpleBomb::mCd)
			break;
		fired = new MissileSimpleBomb(sf::Vector2f(getPosition().x + (50 + 30) * cos(mAngle), getPosition().y + (50 + 30) * sin(mAngle)), mAngle, mBody->GetLinearVelocity());
		break;
	case MissleType::sniper:
		if (clock() - mLastFire < MissileSniper::mCd)
			break;
		fired = new MissileSniper(sf::Vector2f(getPosition().x + (50 + 30) * cos(mAngle), getPosition().y + (50 + 30) * sin(mAngle)), mAngle, mBody->GetLinearVelocity());
		break;
	}
	if (fired)
	{
		mBody->ApplyForceToCenter({ fired->getKick() * cos(mAngle), fired->getKick() * sin(mAngle) }, true);
		mBarrel[mCurrMissleType].playAnimationOnce(1);
		mLastFire = clock();
	}
}

void Tank::respawnAt(const sf::Vector2i& spawnPoint)
{
	mHP = mMaxHP;
	mBody->SetTransform(tgMath::f2tob2({ spawnPoint.x * tgMath::b2scale, spawnPoint.y * tgMath::b2scale }), 0);
}

void Tank::onColide(ObjectRealTypeData * with)
{
	if (with->first == ObjectRealType::rt_Bonus)
	{
		Bonus* bn = (Bonus*)with->second;
		bn->doDamage(50);//bonus hp
		switch (bn->getType())
		{
		case BonusType::bt_heal:
			mHP = std::min(mMaxHP, mHP + bn->getVal());
			break;
		case BonusType::bt_weapon:
			GameObjectManager::unregisterSprite(&mBarrel[mCurrMissleType]);
			mCurrMissleType = (MissleType)bn->getVal();
			GameObjectManager::registerSprite(&mBarrel[mCurrMissleType]);
			break;
		}
	}
}

Tank::Tank(sf::Vector2f pos, bool client) : GameObject()
{
	mClient = client;
	b2BodyDef* bdef = new b2BodyDef();
	bdef->type = b2_dynamicBody;
	bdef->position.Set(pos.x * tgMath::b2scale, pos.y * tgMath::b2scale);
	b2FixtureDef* fdef = MaterialStorage::getMaterial("default");
	auto shape = new b2CircleShape();
	shape->m_radius = 35 * tgMath::b2scale;
	fdef->shape = shape;

    create(ObjectRealType::rt_Tank, Sprite("tankBody.png", pos, 2, {Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 70,70 }), bdef, fdef, 500, true);


	mBarrel = new Sprite[2];
	mBarrel[0] = Sprite("missiles\\simpleBombBarrel.png", pos, 2, { Sprite::Animation(3, 2), Sprite::Animation(3, 30) }, { 70, 40 });
	mBarrel[0].setOrigin({ 0, 20 });

	mBarrel[1] = Sprite("missiles\\sniperBarrel.png", pos, 2, { Sprite::Animation(3, 15), Sprite::Animation(6, 23) }, { 100, 40 });
	mBarrel[1].setOrigin({ 0, 20 });

	mCurrMissleType = MissleType::simpleBomb;
	GameObjectManager::registerSprite(&mBarrel[0]);
}

Tank::Tank()
{
	mClient = true;
}

Tank::~Tank()
{
	GameObjectManager::unregisterSprite(&mBarrel[mCurrMissleType]);
}

void Tank::handleEvents(const sf::Event & ev)
{
	switch (ev.type)
	{
	case sf::Event::MouseButtonPressed:
		if (!mClient)
			launchMissile();
		else
			mWantLaunchMissile = true;
		break;
	}
}

void Tank::updateMousePos(const sf::Vector2f& mousePos)
{
	mMousePos = mousePos;
}

void Tank::update()
{
	GameObject::update();
	if (!mClient)
		mAngle = tgMath::atan2Points(getPosition(), mMousePos);
	mBarrel[mCurrMissleType].setRotation(tgMath::radToDeg(mAngle));
	mBarrel[mCurrMissleType].setPosition(getPosition());
	mBarrel[mCurrMissleType].update();
	if (!mBarrel[mCurrMissleType].isPlaying())
	{
		mBarrel[mCurrMissleType].setAnimation(0);
	}
}

void Tank::setBarrelAngle(float angle)
{
	assert(mClient);

	mAngle = angle;
}

bool Tank::wantToLaunchMissile()
{
	assert(mClient);
	bool ans = mWantLaunchMissile;
	mWantLaunchMissile = false;
	return ans;
}

void Tank::updateFromNetwork(unsigned char * data)
{
	assert(mClient);

	int ptr = 0;
	mSprite.setPosition({ (float)tgMath::read2Bytes(data, ptr), (float)tgMath::read2Bytes(data, ptr) });
	mHP = data[ptr++] * 10;
}

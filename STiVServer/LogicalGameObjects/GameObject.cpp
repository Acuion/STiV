#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/GameObjectManager.h"

void GameObject::create(ObjectRealType type, b2BodyDef * bdef, b2FixtureDef * fixture, int HP, bool damageable)
{
	mHP = HP;
	mDamageable = damageable;

	mBody = GameObjectManager::registerObject(bdef, fixture, this, type);
}

GameObject::GameObject(bool damageable)
{
	mDamageable = damageable;
	mHP = 0;
}

GameObject::GameObject(ObjectRealType type, b2BodyDef* bdef, b2FixtureDef* fixture, int HP, bool damageable)
{
	create(type, bdef, fixture, HP, damageable);
}

GameObject::~GameObject()
{
	GameObjectManager::unregisterObject(mBody);
}

bool GameObject::isDamageable() const
{
	return mDamageable;
}

bool GameObject::mayBeDeleted() const
{
	return mMayBeDeleted;
}

int GameObject::getHP() const
{
	return mHP;
}

sf::Vector2f GameObject::getPosition() const
{
	return { mBody->GetPosition().x / tgMath::b2scale, mBody->GetPosition().y / tgMath::b2scale };
}

void GameObject::doDamage(int dmg)
{
	mHP -= dmg;
}

void GameObject::applyForce(float force, float angle)
{
	mBody->ApplyForceToCenter({ force * cos(angle), force * sin(angle) }, true);
}

void GameObject::update()
{
	if (mHP <= 0 && mDamageable)
	{
		mMayBeDeleted = true;
	}

	for (auto cs = mBody->GetContactList(); cs; cs = cs->next)
	{
		if (!cs->contact->IsTouching())
			continue;
		auto b1 = cs->contact->GetFixtureA()->GetBody();
		auto b2 = cs->contact->GetFixtureB()->GetBody();
		if (b1 != mBody)
		{
			onColide(((std::pair<ObjectRealType, GameObject*>*)b1->GetUserData()));
		}
		else
		{
			onColide(((std::pair<ObjectRealType, GameObject*>*)b2->GetUserData()));
		}
	}
}
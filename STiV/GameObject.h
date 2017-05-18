// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <Box2D\Box2D.h>
#include "Sprite.h"
#include "tgMath.h"
#include "MaterialStorage.h"

enum ObjectRealType
{
	rt_Tank,
	rt_Missile,
	rt_Bonus,
	rt_Static
};

class GameObject
{
	bool mDamageable;
	bool mDeathAnimPlaying = false;
	bool mDeathAnimFinished = false;

protected:
	Sprite mSprite;
	int mHP;
	b2Body* mBody = nullptr;

	virtual void onColide(std::pair<ObjectRealType, GameObject*>* with) {};

	GameObject(bool damageable = true);
	void create(ObjectRealType type, const Sprite& sprite, b2BodyDef* bdef, b2FixtureDef* fixture, int HP, bool damageable = true);
public:
	GameObject(ObjectRealType type, Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fixture, int HP, bool damageable = true);
	virtual ~GameObject();

	bool isDamageable() const;

	bool mayBeDeleted();
	int getHP();

	sf::Vector2f getPosition() const;

	void doDamage(int dmg);

	void applyForce(float force, float angle);

	virtual void update();

	friend class GravityObject;
	friend class GameObjectManager;
};

using ObjectRealTypeData = std::pair<ObjectRealType, GameObject*>;
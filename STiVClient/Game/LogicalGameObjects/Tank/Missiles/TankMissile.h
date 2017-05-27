#pragma once

#include "Game/LogicalGameObjects/GameObject.h"
#include "Game/LogicalGameObjects/GameObjectManager.h"

enum MissleType // cannot be enum class
{
    simpleBomb,
    sniper
};

class TankMissile : public GameObject
{
    float mKick;
protected:
    int mDamage;
    void create(b2FixtureDef * fdef, float force, const sf::Vector2f& pos,
        float angle, int damage, float kick, b2Vec2 tankLinVel, bool fixedRotation = false);
    virtual void onColide(ObjectRealTypeData* with);

	TankMissile();
	virtual ~TankMissile();
public:
    float getKick();
};
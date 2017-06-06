#pragma once

#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/GameObjectManager.h"

enum MissleType // cannot be enum class
{
    simpleBomb,
    sniper
};

class TankMissile : public GameObject
{
    float mKick;
    float mStartAngle;
    b2Vec2 mStartLinVel;
protected:
    int mDamage;
    void create(b2FixtureDef * fdef, float force, const sf::Vector2f& pos,
        float angle, int damage, float kick, b2Vec2 tankLinVel, bool fixedRotation = false);
    void onColide(ObjectRealTypeData* with) override;

    TankMissile();
    virtual ~TankMissile();
public:
    float getKick() const;
    float getAngle() const;
    b2Vec2 getTankLinVel() const;
};
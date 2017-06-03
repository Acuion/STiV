#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Misc/MaterialStorage.h"

enum class ObjectRealType
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
    bool mMayBeDeleted = false;

protected:
    int mHP;
    b2Body* mBody = nullptr;

    virtual void onColide(std::pair<ObjectRealType, GameObject*>* with) {};

    explicit GameObject(bool damageable = true);
    void create(ObjectRealType type, b2BodyDef* bdef, b2FixtureDef* fixture, int HP, bool damageable = true);
public:

    GameObject(ObjectRealType type, b2BodyDef* bdef, b2FixtureDef* fixture, int HP, bool damageable = true);
    virtual ~GameObject();

    bool isDamageable() const;

    bool mayBeDeleted() const;
    int getHP() const;

    sf::Vector2f getPosition() const;

    void doDamage(int dmg);

    void applyForce(float force, float angle);

    virtual void update();

    friend class GravityPoint;
    friend class GameObjectManager;
};

using ObjectRealTypeData = std::pair<ObjectRealType, GameObject*>;
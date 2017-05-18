// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include "GameObject.h"
#include "GameObjectManager.h"

enum MissleType
{
    simpleBomb,
    sniper
};

class TankMissile : public GameObject
{
    float mKick;
protected:
    int mDamage;
    void create(const Sprite& sprite, b2FixtureDef * fdef, float force, const sf::Vector2f& pos,
        float angle, int damage, float kick, b2Vec2 tankLinVel, bool fixedRotation = false);
    virtual void onColide(ObjectRealTypeData* with);
public:
    TankMissile();
    float getKick();
};
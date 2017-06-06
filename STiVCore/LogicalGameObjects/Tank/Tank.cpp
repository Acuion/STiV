#include "LogicalGameObjects/Tank/Tank.h"
#include "LogicalGameObjects/Tank/Missiles/MissileSimpleBomb.h"
#include "LogicalGameObjects/Tank/Missiles/MissileSniper.h"
#include "LogicalGameObjects/Bonus.h"
#include "GameObjectsFactory.h"
#include "Misc/Timer.h"
#include "Misc/Utilites.h"

void Tank::setMissle(MissleType mt)
{
    mCurrMissleType = mt;
}

bool Tank::tryToLaunchMissile()
{
    TankMissile* fired = nullptr;
    switch (mCurrMissleType)
    {
    case MissleType::simpleBomb:
        if (Timer::getElapsedTime() - mLastFire < MissileSimpleBomb::mCd)
            break;
        fired = GameObjectsFactory::newMissileSimpleBomb(sf::Vector2f(getPosition().x + (50 + 30) * cos(mBarrelAngle), getPosition().y + (50 + 30) * sin(mBarrelAngle)), mBarrelAngle, mBody->GetLinearVelocity());
        break;
    case MissleType::sniper:
        if (Timer::getElapsedTime() - mLastFire < MissileSniper::mCd)
            break;
        fired = GameObjectsFactory::newMissileSniper(sf::Vector2f(getPosition().x + (50 + 30) * cos(mBarrelAngle), getPosition().y + (50 + 30) * sin(mBarrelAngle)), mBarrelAngle, mBody->GetLinearVelocity());
        break;
    }
    if (fired)
    {
        mBody->ApplyForceToCenter({ fired->getKick() * cos(mBarrelAngle), fired->getKick() * sin(mBarrelAngle) }, true);
        mLastFire = Timer::getElapsedTime();
        return true;
    }
    else
        return false;
}

void Tank::respawnAt(const sf::Vector2i& spawnPoint)
{
    mHP = mMaxHP;
    mBody->SetTransform(Utilites::f2tob2({ spawnPoint.x * Utilites::b2scale, spawnPoint.y * Utilites::b2scale }), 0);
}

void Tank::onColide(ObjectRealTypeData * with)
{
    if (with->first == ObjectRealType::rt_Bonus)
    {
        Bonus* bn = static_cast<Bonus*>(with->second);
        bn->doDamage(50);//bonus hp
        switch (bn->getType())
        {
        case BonusType::bt_heal:
            mHP = std::min(mMaxHP, mHP + bn->getVal());
            break;
        case BonusType::bt_weapon:
            setMissle(static_cast<MissleType>(bn->getVal()));
            break;
        }
    }
}

Tank::Tank(sf::Vector2f pos, bool client) : GameObject()
{
    mClient = client;
    b2BodyDef* bdef = new b2BodyDef();
    bdef->type = b2_dynamicBody;
    bdef->position.Set(pos.x * Utilites::b2scale, pos.y * Utilites::b2scale);
    b2FixtureDef* fdef = MaterialStorage::getMaterial("default");
    auto shape = new b2CircleShape();
    shape->m_radius = 35 * Utilites::b2scale;
    fdef->shape = shape;

    create(ObjectRealType::rt_Tank, bdef, fdef, 500, true);

    mCurrMissleType = MissleType::simpleBomb;
}

Tank::Tank()
{
    mClient = true;
}

Tank::~Tank()
{
}

void Tank::handleEvents(const sf::Event & ev)
{
    switch (ev.type)
    {
    case sf::Event::MouseButtonPressed:
        if (!mClient)
            tryToLaunchMissile();
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
        mBarrelAngle = Utilites::atan2Points(getPosition(), mMousePos);
}

void Tank::setBarrelAngle(float angle)
{
    assert(mClient);

    mBarrelAngle = angle;
}

float Tank::getBarrelAngle() const
{
    return mBarrelAngle;
}

bool Tank::wantToLaunchMissile()
{
    assert(mClient);
    bool ans = mWantLaunchMissile;
    mWantLaunchMissile = false;
    return ans;
}

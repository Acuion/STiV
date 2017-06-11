#pragma once

#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/Tank/Missiles/TankMissile.h"

class Tank : public GameObject
{
    sf::Vector2f mMousePos;

    int mLastFire = 0;

    void onColide(ObjectRealTypeData* with) override;

    bool mPlayerControlled = false, mWantLaunchMissile = false;
protected:
    float mBarrelAngle = 0;
    MissleType mCurrMissleType;

    virtual void setMissle(MissleType mt);

    Tank(sf::Vector2f pos);
    Tank();
    virtual ~Tank();

public:
    static const int mMaxHP = 500;

    virtual bool tryToLaunchMissile();
    void respawnAt(const sf::Vector2i& spawnPoint);

    void handleEvents(const sf::Event& ev);
    void updateMousePos(const sf::Vector2f& mousePos);
    void update() override;

    void setPlayerControlled(bool playerControlled);

    void setBarrelAngle(float angle);
    float getBarrelAngle() const;
    bool wantToLaunchMissile();
};


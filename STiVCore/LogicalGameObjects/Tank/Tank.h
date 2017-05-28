#pragma once

#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/Tank/Missiles/TankMissile.h"

class Tank : public GameObject
{
	sf::Vector2f mMousePos;

	int mLastFire = 0;

	void onColide(ObjectRealTypeData* with);

	bool mClient = false, mWantLaunchMissile = false;
protected:
	float mAngle = 0;
	MissleType mCurrMissleType;

	virtual void setMissle(MissleType mt);

	Tank(sf::Vector2f pos, bool client = false);
	Tank();
	virtual ~Tank();

public:
	static const int mMaxHP = 500;
	
	virtual bool tryToLaunchMissile();
	void respawnAt(const sf::Vector2i& spawnPoint);

	void handleEvents(const sf::Event& ev);
	void updateMousePos(const sf::Vector2f& mousePos);
	void update() override;

	void setBarrelAngle(float angle);
	bool wantToLaunchMissile();

	void updateFromNetwork(unsigned char* data);
};


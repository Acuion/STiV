#pragma once

#include "Game/LogicalGameObjects/GameObject.h"
#include "Game/LogicalGameObjects/Tank/Missiles/TankMissile.h"

class Tank : public GameObject
{
	Sprite* mBarrel = nullptr;
	sf::Vector2f mMousePos;
	float mAngle = 0;

	int mLastFire = 0;

	MissleType mCurrMissleType;

	void setMissle(MissleType mt);
	void onColide(ObjectRealTypeData* with);

	bool mClient = false, mWantLaunchMissile = false;
	~Tank();
public:
	static const int mMaxHP = 500;
	
	Tank(sf::Vector2f pos, bool client = false);
	Tank();
	
	void launchMissile();
	void respawnAt(const sf::Vector2i& spawnPoint);

	void handleEvents(const sf::Event& ev);
	void updateMousePos(const sf::Vector2f& mousePos);
	void update();

	void setBarrelAngle(float angle);
	bool wantToLaunchMissile();

	void updateFromNetwork(unsigned char* data);
};


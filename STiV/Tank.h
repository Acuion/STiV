// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include "GameObject.h"
#include <list>
#include "TankMissile.h"
#include "MissileSimpleBomb.h"
#include "MissileSniper.h"
#include "Bonus.h"

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


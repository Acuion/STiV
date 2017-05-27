#pragma once
#include "LogicalGameObjects/Tank/Missiles/MissileSimpleBomb.h"
#include "VisualGameObjects/Tank/Missiles/MissileSimpleBombVis.h"
#include "LogicalGameObjects/Tank/Missiles/MissileSniper.h"
#include "VisualGameObjects/Tank/Missiles/MissileSniperVis.h"
#include "VisualGameObjects/Tank/TankVis.h"
#include "VisualGameObjects/BonusVis.h"
#include "VisualGameObjects/GravityObjectVis.h"

class GameObjectsFactory
{
public:
	static MissileSimpleBomb* newMissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
	{
		return new MissileSimpleBombVis(pos, angle, tankLinVel);
	}

	static MissileSniper* newMissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
	{
		return new MissileSniperVis(pos, angle, tankLinVel);
	}

	static Tank* newTank(sf::Vector2f pos, bool client = false)
	{
		return new TankVis(pos, client);
	}

	static Tank* newTank()
	{
		return new TankVis();
	}

	static Bonus* newBonus(BonusType bt, int value, sf::Vector2f pos)
	{
		return new BonusVis(bt, value, pos);
	}

	static GravityObject* newGravityObject(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power)
	{
		return new GravityObjectVis(sprite, bdef, fdef, power);
	}

private:
	GameObjectsFactory();
};

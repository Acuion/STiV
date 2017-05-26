#pragma once

#include "Game/LogicalGameObjects/GameObject.h"

class GravityObject : public GameObject
{
	int mPower;
public:
	GravityObject(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power);
	void affect(GameObject & go);
};

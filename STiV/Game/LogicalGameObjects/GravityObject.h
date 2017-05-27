#pragma once

#include "Game/LogicalGameObjects/GameObject.h"

class GravityObject : public GameObject
{
	int mPower;
protected:
	GravityObject(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power);
	virtual ~GravityObject();
public:
	void affect(GameObject & go);
};

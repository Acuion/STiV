// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include "GameObject.h"

class GravityObject : public GameObject
{
	int mPower;
public:
	GravityObject(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power);
	void affect(GameObject & go);
};

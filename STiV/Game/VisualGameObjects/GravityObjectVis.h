#pragma once
#include "Game/LogicalGameObjects/GravityObject.h"

class GravityObjectVis : public GravityObject
{
public:
	GravityObjectVis(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power);
};

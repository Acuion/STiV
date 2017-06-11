#pragma once
#include "LogicalGameObjects/GameObject.h"

class StaticObject : public GameObject
{
public:
    StaticObject(b2BodyDef* bdef, b2FixtureDef* fixture);
};

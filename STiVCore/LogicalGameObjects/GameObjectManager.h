#pragma once

#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/Gravity/GravityPoint.h"
#include <list>

class GameObjectManager
{
protected:
    b2World* mWorld = nullptr;
    std::list<GameObject*> mObjects;
    std::list<GravityPoint*> mGravityObjects;
    GameObjectManager() {};

public:
    void reset(int sizeX, int sizeY);

    b2Body* registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type);

    void unregisterObject(b2Body* body);

    void registerGravityPoint(GravityPoint* gravo);

    void unregisterGravityPoint(GravityPoint* gravo);

    void update(int dt);

    void forcedDelete(GameObject* obj);
};

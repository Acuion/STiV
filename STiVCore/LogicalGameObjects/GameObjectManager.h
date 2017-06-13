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
    virtual void createBorder(b2BodyDef* b2Def, b2FixtureDef* b2FixDef);
    virtual void doGravity();
public:
    virtual ~GameObjectManager();

    void reset(int sizeX, int sizeY);

    virtual void registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type);

    virtual void unregisterObject(GameObject* go);

    void registerGravityPoint(GravityPoint* gravo);

    void unregisterGravityPoint(GravityPoint* gravo);

    virtual void update(int dt);

    void forcedDelete(GameObject* obj);
};

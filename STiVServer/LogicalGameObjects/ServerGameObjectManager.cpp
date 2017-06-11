#include "ServerGameObjectManager.h"

ServerGameObjectManager & ServerGameObjectManager::getInstance()
{
    static ServerGameObjectManager singleton;
    return singleton;
}

b2Body* ServerGameObjectManager::registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type)
{
    mNewObjects.push_back(go);
    return GameObjectManager::registerObject(bdef, fixture, go, type);
}

std::list<GameObject*> ServerGameObjectManager::getGameObjects() const
{
    return mObjects;
}

const std::vector<GameObject*>& ServerGameObjectManager::getNewObjects() const
{
    return mNewObjects;
}

void ServerGameObjectManager::clearNewObjects()
{
    mNewObjects.clear();
}

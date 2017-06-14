#include "ServerGameObjectManager.h"
#include "STGClient.h"

ServerGameObjectManager & ServerGameObjectManager::getInstance()
{
    static ServerGameObjectManager singleton;
    return singleton;
}

void ServerGameObjectManager::registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type)
{
    GameObjectManager::registerObject(bdef, fixture, go, type);;
    for (auto& x : mNewObjSubscribers)
        x->procNewObject(go);
}

void ServerGameObjectManager::unregisterObject(GameObject* object)
{
    GameObjectManager::unregisterObject(object);
    for (auto& x : mNewObjSubscribers)
        x->procDelObject(object);
}

const std::list<GameObject*>& ServerGameObjectManager::getGameObjects() const
{
    return mObjects;
}

void ServerGameObjectManager::update(int dt)
{
    mObjectsLock.lock();
    GameObjectManager::update(dt);
    mObjectsLock.unlock();
}

void ServerGameObjectManager::lockObjects()
{
    mObjectsLock.lock();
}

void ServerGameObjectManager::unlockObjects()
{
    mObjectsLock.unlock();
}

void ServerGameObjectManager::subscribeClient(STGClient* client)
{
    mNewObjSubscribers.push_back(client);
}

void ServerGameObjectManager::unsubscribeClient(STGClient* client)
{
    mNewObjSubscribers.remove(client);
}

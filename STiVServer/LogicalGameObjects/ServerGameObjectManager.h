#pragma once
#include "LogicalGameObjects/GameObjectManager.h"
#include <mutex>

class STGClient;

class ServerGameObjectManager : public GameObjectManager
{
public:
    static ServerGameObjectManager& getInstance();

    void registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type) override;
    void unregisterObject(GameObject* object) override;
    const std::list<GameObject*>& getGameObjects() const;
    void update(int dt) override;

    void lockObjects();
    void unlockObjects();

    void subscribeClient(STGClient* client);
    void unsubscribeClient(STGClient* client);
private:
    std::list<STGClient*> mNewObjSubscribers;
    std::mutex mObjectsLock;
};

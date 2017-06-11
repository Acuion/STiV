#pragma once
#include "LogicalGameObjects/GameObjectManager.h"

class ServerGameObjectManager : public GameObjectManager
{
public:
    static ServerGameObjectManager& getInstance();

    b2Body* registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type) override;

    std::list<GameObject*> getGameObjects() const;
    const std::vector<GameObject*>& getNewObjects() const;
    void clearNewObjects();
private:
    std::vector<GameObject*> mNewObjects;
};

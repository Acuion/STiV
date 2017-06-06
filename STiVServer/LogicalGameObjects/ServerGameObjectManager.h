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
protected:
    void createBorder(b2BodyDef* b2Def, b2FixtureDef* b2FixDef) override;
private:
    std::vector<GameObject*> mNewObjects;
};

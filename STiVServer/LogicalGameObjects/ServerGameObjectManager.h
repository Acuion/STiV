#pragma once
#include "LogicalGameObjects/GameObjectManager.h"

class ServerGameObjectManager : public GameObjectManager
{
public:
    static ServerGameObjectManager& getInstance();
};

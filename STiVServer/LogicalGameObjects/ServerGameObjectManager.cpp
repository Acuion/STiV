#include "ServerGameObjectManager.h"

ServerGameObjectManager & ServerGameObjectManager::getInstance()
{
    static ServerGameObjectManager singleton;
    return singleton;
}
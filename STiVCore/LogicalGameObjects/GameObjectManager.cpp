#include "LogicalGameObjects/GameObjectManager.h"

std::list<GravityPoint*> GameObjectManager::mGravityObjects;
std::list<GameObject*> GameObjectManager::mObjects;
std::mutex GameObjectManager::mObjectsAccess;
b2World* GameObjectManager::mWorld = nullptr;
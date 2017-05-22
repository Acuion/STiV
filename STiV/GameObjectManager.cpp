#include "GameObjectManager.h"

std::list<GravityObject*> GameObjectManager::mGravityObjects;
std::list<GameObject*> GameObjectManager::mObjects;
std::mutex GameObjectManager::mObjectsAccess;
std::list<Sprite*> GameObjectManager::mDrawLayer;
b2World* GameObjectManager::mWorld = nullptr;
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "GameObjectManager.h"

std::list<GravityObject*> GameObjectManager::mGravityObjects;
std::list<GameObject*> GameObjectManager::mObjects;
std::mutex GameObjectManager::mObjectsAccess;
std::list<Sprite*> GameObjectManager::mDrawLayer;
b2World* GameObjectManager::mWorld = nullptr;
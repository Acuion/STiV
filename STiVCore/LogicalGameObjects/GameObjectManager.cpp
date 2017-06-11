#include "LogicalGameObjects/GameObjectManager.h"
#include "Misc/Utilites.h"
#include "StaticObject.h"

void GameObjectManager::createBorder(b2BodyDef* b2Def, b2FixtureDef* b2FixDef)
{
    new StaticObject(b2Def, new b2FixtureDef(*b2FixDef));
}

GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::reset(int sizeX, int sizeY)
{
    //GameObject::resetNumeration();
    auto iter = mObjects.begin();
    while (iter != mObjects.end())
    {
        delete *iter;
        iter = mObjects.erase(iter);
    }

    delete mWorld;
    mWorld = new b2World({ 0, 0 });

    b2BodyDef* borderDef = new b2BodyDef();
    borderDef->type = b2_staticBody;

    b2FixtureDef* fdefH = new b2FixtureDef();
    b2PolygonShape* shapeH = new b2PolygonShape();
    shapeH->SetAsBox(static_cast<float>(sizeX * Utilites::b2scale), 20 * Utilites::b2scale);
    fdefH->shape = shapeH;

    b2FixtureDef* fdefV = new b2FixtureDef();
    b2PolygonShape* shapeV = new b2PolygonShape();
    shapeV->SetAsBox(20 * Utilites::b2scale, static_cast<float>(sizeY * Utilites::b2scale));
    fdefV->shape = shapeV;

    b2BodyDef* bh1 = new b2BodyDef(*borderDef);
    bh1->position.Set(sizeX * Utilites::b2scale / 2, -20 * Utilites::b2scale);
    createBorder(bh1, fdefH);
    b2BodyDef* bv1 = new b2BodyDef(*borderDef);
    bv1->position.Set(-20 * Utilites::b2scale, sizeY * Utilites::b2scale / 2);
    createBorder(bv1, fdefV);

    b2BodyDef* bh2 = new b2BodyDef(*borderDef);
    bh2->position.Set(sizeX * Utilites::b2scale / 2, sizeY * Utilites::b2scale + 20 * Utilites::b2scale);
    createBorder(bh2, fdefH);
    b2BodyDef* bv2 = new b2BodyDef(*borderDef);
    bv2->position.Set(sizeX * Utilites::b2scale + 20 * Utilites::b2scale, sizeY * Utilites::b2scale / 2);
    createBorder(bv2, fdefV);

    delete fdefH;
    delete fdefV;
}

b2Body* GameObjectManager::registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type)
{
    b2Body* body = mWorld->CreateBody(bdef);
    body->CreateFixture(fixture);
    body->SetUserData(new std::pair<ObjectRealType, GameObject*>(type, go));
    mObjects.push_back(go);
    return body;
}

void GameObjectManager::unregisterObject(b2Body* body)
{
    mWorld->DestroyBody(body);
}

void GameObjectManager::registerGravityPoint(GravityPoint* gravo)
{
    mGravityObjects.push_back(gravo);
}

void GameObjectManager::unregisterGravityPoint(GravityPoint* gravo)
{
    mGravityObjects.remove(gravo);
}

void GameObjectManager::update(int dt)
{
    for (auto gb : mGravityObjects)
    {
        for (auto o : mObjects)
            gb->affect(*o);
    }

    for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
    {
        if (!(*iter)->mayBeDeleted())
            (*iter)->update();
        else
        {
            delete *iter;
            iter = mObjects.erase(iter);
        }
        if (iter == mObjects.end())
            break;
    }

    mWorld->Step(0.001f*dt, 8, 3);
}

void GameObjectManager::forcedDelete(GameObject* obj)
{
    for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
    {
        if (*iter == obj)
        {
            delete *iter;
            iter = mObjects.erase(iter);
            return;
        }
    }
}
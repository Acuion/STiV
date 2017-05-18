// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once
#include "GameObject.h"
#include "GravityObject.h"
#include "tgMath.h"
#include <iostream>
#include <list>
#include <mutex>

class GameObjectManager
{
    static b2World* mWorld;
    static std::list<GameObject*> mObjects;
    static std::list<GravityObject*> mGravityObjects;
    static std::list<Sprite*> mDrawLayer;
    static std::mutex mObjectsAccess;
    GameObjectManager() {};
public:
    static const size_t mSizeofSpecificClientData = 5;
    static const size_t mSizeofPackedSprite = 10;

    static void reset(int sizeX, int sizeY)
    {
        auto iter = mObjects.begin();
        while (iter != mObjects.end())
        {
            mWorld->DestroyBody((*iter)->mBody);
            delete *iter;
            iter = mObjects.erase(iter);
        }

        delete mWorld;
        mWorld = new b2World({ 0, 0 });

        b2BodyDef* borderDef = new b2BodyDef();
        borderDef->type = b2_staticBody;

        b2FixtureDef* fdefH = new b2FixtureDef();
        b2PolygonShape* shapeH = new b2PolygonShape();
        shapeH->SetAsBox((float)sizeX * tgMath::b2scale, 20 * tgMath::b2scale);
        fdefH->shape = shapeH;

        b2FixtureDef* fdefV = new b2FixtureDef();
        b2PolygonShape* shapeV = new b2PolygonShape();
        shapeV->SetAsBox(20 * tgMath::b2scale, (float)sizeY* tgMath::b2scale);
        fdefV->shape = shapeV;

        b2BodyDef* bh1 = new b2BodyDef(*borderDef);
        bh1->position.Set(sizeX * tgMath::b2scale / 2, -20 * tgMath::b2scale);
        new GameObject(ObjectRealType::rt_Static, Sprite("borderPixel.png", { 0,0 }, { (float)sizeX + 40, 3 }), bh1, new b2FixtureDef(*fdefH), 0, false);
        b2BodyDef* bv1 = new b2BodyDef(*borderDef);
        bv1->position.Set(-20 * tgMath::b2scale, sizeY * tgMath::b2scale / 2);
        new GameObject(ObjectRealType::rt_Static, Sprite("borderPixel.png", { 0,0 }, { 3, (float)sizeY + 40 }), bv1, new b2FixtureDef(*fdefV), 0, false);

        b2BodyDef* bh2 = new b2BodyDef(*borderDef);
        bh2->position.Set(sizeX * tgMath::b2scale / 2, sizeY * tgMath::b2scale + 20 * tgMath::b2scale);
        new GameObject(ObjectRealType::rt_Static, Sprite("borderPixel.png", { 0, (float)sizeY }, { (float)sizeX + 40, 3 }), bh2, new b2FixtureDef(*fdefH), 0, false);
        b2BodyDef* bv2 = new b2BodyDef(*borderDef);
        bv2->position.Set(sizeX * tgMath::b2scale + 20 * tgMath::b2scale, sizeY * tgMath::b2scale / 2);
        new GameObject(ObjectRealType::rt_Static, Sprite("borderPixel.png", { (float)sizeX, 0 }, { 3, (float)sizeY + 40 }), bv2, new b2FixtureDef(*fdefV), 0, false);

        delete fdefH;
        delete fdefV;
    }

    static b2Body* registerObject(b2BodyDef* bdef, b2FixtureDef* fixture, GameObject* go, ObjectRealType type)
    {
        b2Body* body = mWorld->CreateBody(bdef);
        body->CreateFixture(fixture);
        body->SetUserData(new std::pair<ObjectRealType, GameObject*>(type, go));
        mObjects.push_back(go);
        return body;
    }

    static void registerGravityObject(GravityObject* gravo)
    {
        mGravityObjects.push_back(gravo);
    }

    static void registerSprite(Sprite* sp)
    {
        mDrawLayer.push_back(sp);
    }

    static void unregisterSprite(Sprite* sp)
    {
        mDrawLayer.remove(sp);
    }

    static void update(int dt)
    {
        mObjectsAccess.lock();
        for (auto gb : mGravityObjects)
        {
            for (auto o : mObjects)
                if (gb != o)
                    gb->affect(*o);
        }

        for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
        {
            if (!(*iter)->mayBeDeleted())
                (*iter)->update();
            else
            {
                mWorld->DestroyBody((*iter)->mBody);
                mDrawLayer.remove(&(*iter)->mSprite);
                delete *iter;
                iter = mObjects.erase(iter);
            }
            if (iter == mObjects.end())
                break;
        }

        mWorld->Step(0.001*dt, 8, 3);
        mObjectsAccess.unlock();
    }

    static void forcedDelete(GameObject* obj)
    {
        for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
        {
            if (*iter == obj)
            {
                mWorld->DestroyBody((*iter)->mBody);
                mDrawLayer.remove(&(*iter)->mSprite);
                delete *iter;
                iter = mObjects.erase(iter);
                return;
            }
        }
    }

    static void draw(sf::RenderTexture& wnd, sf::FloatRect viewRect)
    {
        mObjectsAccess.lock();
        for (auto iter = mDrawLayer.rbegin(); iter != mDrawLayer.rend(); ++iter)
            (*iter)->draw(wnd, viewRect);
        mObjectsAccess.unlock();
    }

    static unsigned char* pack(int& ptr)
    {
        mObjectsAccess.lock();
        unsigned char* data = new unsigned char[mDrawLayer.size() * mSizeofPackedSprite + mSizeofSpecificClientData + 2];//2bytes for sizeof msg
        for (auto s : mDrawLayer)
            s->pack(data, ptr);
        mObjectsAccess.unlock();
        return data;
    }

    static void unpack(unsigned char* data, int count)
    {
        mObjectsAccess.lock();
        while (mDrawLayer.size())
        {
            delete *mDrawLayer.begin();
            mDrawLayer.pop_front();
        }
        count /= mSizeofPackedSprite;
        Sprite* sp;
        int ptr = 0;
        for (int i = 0; i < count; ++i)
        {
            sp = new Sprite();
            sp->unpack(data, ptr);
            mDrawLayer.push_back(sp);
        }
        mObjectsAccess.unlock();
    }
};

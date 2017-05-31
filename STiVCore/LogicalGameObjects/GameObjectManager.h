#pragma once

#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/Gravity/GravityPoint.h"
#include "Misc/Utilites.h"
#include <list>
#include <mutex>

class GameObjectManager
{
	static b2World* mWorld;
	static std::list<GameObject*> mObjects;
	static std::list<GravityPoint*> mGravityObjects;
	static std::mutex mObjectsAccess;
	GameObjectManager() {};
public:
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
		shapeH->SetAsBox((float)sizeX * Utilites::b2scale, 20 * Utilites::b2scale);
		fdefH->shape = shapeH;

		b2FixtureDef* fdefV = new b2FixtureDef();
		b2PolygonShape* shapeV = new b2PolygonShape();
		shapeV->SetAsBox(20 * Utilites::b2scale, (float)sizeY* Utilites::b2scale);
		fdefV->shape = shapeV;

		b2BodyDef* bh1 = new b2BodyDef(*borderDef);
		bh1->position.Set(sizeX * Utilites::b2scale / 2, -20 * Utilites::b2scale);
		new GameObject(ObjectRealType::rt_Static, bh1, new b2FixtureDef(*fdefH), 0, false);
		b2BodyDef* bv1 = new b2BodyDef(*borderDef);
		bv1->position.Set(-20 * Utilites::b2scale, sizeY * Utilites::b2scale / 2);
		new GameObject(ObjectRealType::rt_Static, bv1, new b2FixtureDef(*fdefV), 0, false);

		b2BodyDef* bh2 = new b2BodyDef(*borderDef);
		bh2->position.Set(sizeX * Utilites::b2scale / 2, sizeY * Utilites::b2scale + 20 * Utilites::b2scale);
		new GameObject(ObjectRealType::rt_Static, bh2, new b2FixtureDef(*fdefH), 0, false);
		b2BodyDef* bv2 = new b2BodyDef(*borderDef);
		bv2->position.Set(sizeX * Utilites::b2scale + 20 * Utilites::b2scale, sizeY * Utilites::b2scale / 2);
		new GameObject(ObjectRealType::rt_Static, bv2, new b2FixtureDef(*fdefV), 0, false);

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

	static void unregisterObject(b2Body* body)
	{
		mWorld->DestroyBody(body);
	}

	static void registerGravityPoint(GravityPoint* gravo)
	{
		mGravityObjects.push_back(gravo);
	}

	static void unregisterGravityPoint(GravityPoint* gravo)
	{
		mGravityObjects.remove(gravo);
	}

	static void update(int dt)
	{
		mObjectsAccess.lock();
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

		mWorld->Step(0.001*dt, 8, 3);
		mObjectsAccess.unlock();
	}

	static void forcedDelete(GameObject* obj)
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
};
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "GravityObject.h"
#include "GameObjectManager.h"

GravityObject::GravityObject(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power) : mPower(power), GameObject(ObjectRealType::rt_Static, sprite, bdef, fdef, 0, false)
{
	mPower = power;
	GameObjectManager::registerGravityObject(this);
}

void GravityObject::affect(GameObject& go)
{
	float angle = tgMath::atan2Points(go.getPosition(), getPosition());
	float dist = tgMath::distBetweenPoints(go.getPosition(), getPosition());
	dist *= dist;
	if (!dist)
		return;
	go.mBody->ApplyForceToCenter({ mPower * go.mBody->GetMass() * cos(angle) / dist, mPower * go.mBody->GetMass() * sin(angle) / dist }, true);
}

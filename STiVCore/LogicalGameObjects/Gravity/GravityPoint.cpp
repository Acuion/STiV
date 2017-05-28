#include "LogicalGameObjects/Gravity/GravityPoint.h"
#include "LogicalGameObjects/GameObjectManager.h"

GravityPoint::GravityPoint(sf::Vector2f pos, int power)
	: mPower(power)
	, mPosition(pos)
{
	mPower = power;
	GameObjectManager::registerGravityPoint(this);
}

GravityPoint::~GravityPoint()
{
	GameObjectManager::unregisterGravityPoint(this);
}

void GravityPoint::affect(GameObject& go) const
{
	float angle = tgMath::atan2Points(go.getPosition(), mPosition);
	float dist = tgMath::distBetweenPoints(go.getPosition(), mPosition);
	dist *= dist;
	if (!dist)
		return;
	go.mBody->ApplyForceToCenter({ mPower * go.mBody->GetMass() * cos(angle) / dist, mPower * go.mBody->GetMass() * sin(angle) / dist }, true);
}

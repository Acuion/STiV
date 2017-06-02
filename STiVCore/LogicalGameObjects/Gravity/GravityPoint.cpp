#include "LogicalGameObjects/Gravity/GravityPoint.h"
#include "LogicalGameObjects/GameObjectManager.h"
#include "GameObjectsFactory.h"

GravityPoint::GravityPoint(sf::Vector2f pos, int power)
	: mPower(power)
	, mPosition(pos)
{
	mPower = power;
	GameObjectsFactory::instanceOfGoM().registerGravityPoint(this);
}

GravityPoint::~GravityPoint()
{
	GameObjectsFactory::instanceOfGoM().unregisterGravityPoint(this);
}

void GravityPoint::affect(GameObject& go) const
{
	float angle = Utilites::atan2Points(go.getPosition(), mPosition);
	float dist = Utilites::distBetweenPoints(go.getPosition(), mPosition);
	dist *= dist;
	if (!dist)
		return;
	go.mBody->ApplyForceToCenter({ mPower * go.mBody->GetMass() * cos(angle) / dist, mPower * go.mBody->GetMass() * sin(angle) / dist }, true);
}

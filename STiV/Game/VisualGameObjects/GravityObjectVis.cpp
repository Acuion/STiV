#include "Game/VisualGameObjects/GravityObjectVis.h"

GravityObjectVis::GravityObjectVis(Sprite sprite, b2BodyDef* bdef, b2FixtureDef* fdef, int power)
	: GravityObject(sprite, bdef, fdef, power)
{
}


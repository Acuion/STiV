#include "StaticObject.h"

StaticObject::StaticObject(b2BodyDef* bdef, b2FixtureDef* fixture)
    : GameObject(ObjectRealType::rt_Static, bdef, fixture, 0, false)
{
}

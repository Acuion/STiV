#include "Misc/Utilites.h"
#include <Box2D/Box2D.h>

bool Utilites::floatEq(float a, float b)
{
	return fabs(a - b) < 0.001;
}

float Utilites::distBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return sqrt(dx*dx + dy*dy);
}

float Utilites::atan2Points(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return atan2(p2.y - p1.y, p2.x - p1.x);
}

float Utilites::degToRad(float deg)
{
	return deg / 180 * M_PI;
}

float Utilites::radToDeg(float rad)
{
	return rad / M_PI * 180;
}

sf::Vector2f Utilites::b2to2f(const b2Vec2& vec)
{
	return { vec.x, vec.y };
}

b2Vec2 Utilites::f2tob2(const sf::Vector2f& vec)
{
	return { vec.x, vec.y };
}


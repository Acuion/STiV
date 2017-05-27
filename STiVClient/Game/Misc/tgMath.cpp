#include "Game/Misc/tgMath.h"
#include <Box2D\Box2D.h>

bool tgMath::floatEq(float a, float b)
{
	return fabs(a - b) < 0.001;
}

float tgMath::distBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return sqrt(dx*dx + dy*dy);
}

float tgMath::atan2Points(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return atan2(p2.y - p1.y, p2.x - p1.x);
}

float tgMath::degToRad(float deg)
{
	return deg / 180 * M_PI;
}

float tgMath::radToDeg(float rad)
{
	return rad / M_PI * 180;
}

sf::Vector2f tgMath::b2to2f(const b2Vec2& vec)
{
	return { vec.x, vec.y };
}

b2Vec2 tgMath::f2tob2(const sf::Vector2f& vec)
{
	return{ vec.x, vec.y };
}

int tgMath::read2Bytes(unsigned char * data, int & ptr)
{
	int num = data[ptr++];
	num += data[ptr++] * 256;
	return num;
}

void tgMath::write2Bytes(int num, unsigned char * data, int & ptr)
{
	data[ptr++] = num;
	num /= 256;
	data[ptr++] = num;
}

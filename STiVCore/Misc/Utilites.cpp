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

short Utilites::readShort(unsigned char * data, int & ptr)
{
	ShortBytes sb;
	sb.bytesRep[0] = data[ptr++];
	sb.bytesRep[1] = data[ptr++];
	return sb.val;
}

void Utilites::writeShort(short num, unsigned char * data, int & ptr)
{
	ShortBytes sb;
	sb.val = num;
	data[ptr++] = sb.bytesRep[0];
	data[ptr++] = sb.bytesRep[1];
}

void Utilites::writeFloat(float num, unsigned char * data, int & ptr)
{
	FloatBytes fb;
	fb.val = num;
	data[ptr++] = fb.bytesRep[0];
	data[ptr++] = fb.bytesRep[1];
	data[ptr++] = fb.bytesRep[2];
	data[ptr++] = fb.bytesRep[3];
}

float Utilites::readFloat(unsigned char * data, int & ptr)
{
	FloatBytes fb;
	fb.bytesRep[0] = data[ptr++];
	fb.bytesRep[1] = data[ptr++];
	fb.bytesRep[2] = data[ptr++];
	fb.bytesRep[3] = data[ptr++];
	return fb.val;
}

void Utilites::writeVector2f(sf::Vector2f vec, unsigned char * data, int & ptr)
{
	writeFloat(vec.x, data, ptr);
	writeFloat(vec.y, data, ptr);
}

sf::Vector2f Utilites::readVector2f(unsigned char * data, int & ptr)
{
	sf::Vector2f toRet;
	toRet.x = readFloat(data, ptr);
	toRet.y = readFloat(data, ptr);
	return toRet;
}

void Utilites::writeVectorb2(b2Vec2 vec, unsigned char * data, int & ptr)
{
	writeFloat(vec.x, data, ptr);
	writeFloat(vec.y, data, ptr);
}

b2Vec2 Utilites::readVectorb2(unsigned char * data, int & ptr)
{
	b2Vec2 toRet;
	toRet.x = readFloat(data, ptr);
	toRet.y = readFloat(data, ptr);
	return toRet;
}

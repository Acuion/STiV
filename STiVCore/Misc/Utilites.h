#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#define M_PI 3.14

namespace Utilites
{
	 union ShortBytes
	 {
		 short val;
		 unsigned char bytesRep[2];
	 };

	 union FloatBytes
	 {
		 float val;
		 unsigned char bytesRep[4];
	 };

	const float b2scale = 0.04;

	bool floatEq(float a, float b);

	float distBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float atan2Points(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float degToRad(float deg);
	float radToDeg(float rad);

	sf::Vector2f b2to2f(const b2Vec2& vec);
	b2Vec2 f2tob2(const sf::Vector2f& vec);

	short readShort(unsigned char * data, int & ptr);
	void writeShort(short num, unsigned char * data, int & ptr);

	void writeFloat(float num, unsigned char * data, int & ptr);
	float readFloat(unsigned char * data, int & ptr);

	void writeVector2f(sf::Vector2f vec, unsigned char * data, int & ptr);
	sf::Vector2f readVector2f(unsigned char * data, int & ptr);

	void writeVectorb2(b2Vec2 vec, unsigned char * data, int & ptr);
	b2Vec2 readVectorb2(unsigned char * data, int & ptr);
}

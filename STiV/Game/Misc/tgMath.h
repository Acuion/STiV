#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#define M_PI 3.14

namespace tgMath
{
	const float b2scale = 0.04;

	bool floatEq(float a, float b);

	float distBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float atan2Points(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float degToRad(float deg);
	float radToDeg(float rad);

	sf::Vector2f b2to2f(const b2Vec2& vec);
	b2Vec2 f2tob2(const sf::Vector2f& vec);

	int read2Bytes(unsigned char * data, int & ptr);
	void write2Bytes(int num, unsigned char * data, int & ptr);
}
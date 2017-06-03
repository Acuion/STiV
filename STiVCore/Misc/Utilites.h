#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#define M_PI 3.141592f

namespace Utilites
{
	using byte = unsigned char;

	const float b2scale = 0.04f;

	bool floatEq(float a, float b);

	float distBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float atan2Points(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float degToRad(float deg);
	float radToDeg(float rad);

	sf::Vector2f b2to2f(const b2Vec2& vec);
	b2Vec2 f2tob2(const sf::Vector2f& vec);
}

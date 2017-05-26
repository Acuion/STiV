#pragma once

#include <SFML/Graphics.hpp>
#include "Game/Graphics/Sprite.h"

class UIButton
{
	sf::IntRect mButtonRect;
	enum State
	{
		normal,
		inside,
		pressedInside
	} mCurrState;
	Sprite mSprites;
public:
	UIButton() {};
	UIButton(sf::IntRect buttonRect, const Sprite& buttonStates);
	bool wasPressed(const sf::Event& ev, const sf::Vector2f& mousePos);
	void setPosition(const sf::Vector2f& pos);
	void update();
	void draw(sf::RenderWindow& wnd);
};

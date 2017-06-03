#include "UI/UIButton.h"

UIButton::UIButton(sf::IntRect buttonRect, const Sprite& buttonStates)
{
    buttonRect.top -= buttonRect.height;
    buttonRect.left -= buttonRect.width;
    buttonRect.width *= 2;
    buttonRect.height *= 2;
    mButtonRect = buttonRect;
    mSprites = buttonStates;
    mCurrState = State::normal;
}

bool UIButton::wasPressed(const sf::Event& ev, const sf::Vector2f& mousePos)
{
    switch (ev.type)
    {
    case sf::Event::MouseButtonPressed:
        if (mCurrState == State::inside)
            mCurrState = State::pressedInside;
        break;
    case sf::Event::MouseButtonReleased:
        if (mCurrState == State::pressedInside)
        {
            mCurrState = State::inside;
            mSprites.setAnimation(mCurrState);
            return true;
        }
        break;
    case sf::Event::MouseMoved:
        if (mButtonRect.contains(static_cast<sf::Vector2i>(mousePos)))
        {
            if (mCurrState != State::pressedInside)
                mCurrState = State::inside;
        }
        else
            mCurrState = State::normal;
        break;
    default:
        break;
    }
    mSprites.setAnimation(mCurrState);
    return false;
}

void UIButton::setPosition(const sf::Vector2f& pos)
{
    mSprites.setPosition(pos);
    mButtonRect.left = static_cast<int>(pos.x - mButtonRect.width / 2);
    mButtonRect.top = static_cast<int>(pos.y - mButtonRect.height / 2);
}

void UIButton::update()
{
    mSprites.update();
}

void UIButton::draw(sf::RenderWindow & wnd)
{
    mSprites.draw(wnd);
}

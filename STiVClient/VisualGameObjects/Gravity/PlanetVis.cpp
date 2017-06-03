#include "VisualGameObjects/Gravity/PlanetVis.h"
#include "VisualGameObjects/Visualizer.h"

PlanetVis::PlanetVis(sf::Vector2f pos, int radius, int power)
    : Planet(pos, radius, power)
{
    mSprite = Sprite("planet.png", { pos.x, pos.y }, radius / 200.0f);
    Visualizer::registerSprite(&mSprite);
}

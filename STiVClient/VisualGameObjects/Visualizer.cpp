#include "VisualGameObjects/Visualizer.h"

std::mutex Visualizer::mObjectsAccess;
std::list<Sprite*> Visualizer::mDrawLayer;
std::list<sf::Text*> Visualizer::mTextLayer;
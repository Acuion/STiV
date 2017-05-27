#include "Game/VisualGameObjects/Visualizer.h"

std::mutex Visualizer::mObjectsAccess;
std::list<Sprite*> Visualizer::mDrawLayer;
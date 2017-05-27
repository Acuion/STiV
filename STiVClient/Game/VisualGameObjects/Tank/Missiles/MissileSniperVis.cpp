#include "MissileSniperVis.h"
#include "Game/VisualGameObjects/Visualizer.h"

MissileSniperVis::MissileSniperVis(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
	: MissileSniper(pos, angle, tankLinVel)
{
	mSprite = Sprite("missiles\\sniperMissile.png", pos,
		2, { Sprite::Animation(2, 30), Sprite::Animation(3, 30, PostProcessingManager::MultitargetShaderType::Explosion) },
		{ 25, 24 });
	Visualizer::registerSprite(&mSprite);
}

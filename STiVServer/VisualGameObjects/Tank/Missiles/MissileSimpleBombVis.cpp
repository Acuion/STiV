#include "VisualGameObjects/Tank/Missiles/MissileSimpleBombVis.h"
#include "VisualGameObjects/Visualizer.h"

MissileSimpleBombVis::MissileSimpleBombVis(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
	: MissileSimpleBomb(pos, angle, tankLinVel)
{
	mSprite = Sprite("missiles\\simpleBombMissile.png", pos,
		2, { Sprite::Animation(3, 2),
			Sprite::Animation(3, 30, PostProcessingManager::MultitargetShaderType::Explosion) },
			{ 20, 20 });
	Visualizer::registerSprite(&mSprite);
}

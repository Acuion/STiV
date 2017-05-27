#include "Game/VisualGameObjects/BonusVis.h"
#include "Game/LogicalGameObjects/Tank/Missiles/TankMissile.h"
#include "Visualizer.h"

BonusVis::BonusVis(BonusType bt, int value, sf::Vector2f pos)
	: Bonus(bt, value, pos)
{
	switch (bt)
	{
	case BonusType::bt_heal:
		mSprite = Sprite("bonuses\\heal.png", pos, 2, { Sprite::Animation(2, 1), Sprite::Animation(2, 18) }, { 50, 50 });
		break;
	case BonusType::bt_weapon:
		switch (value)
		{
		case MissleType::simpleBomb:
			mSprite = Sprite("bonuses\\weapon_simpleBomb.png", pos, 2, { Sprite::Animation(2, 1), Sprite::Animation(2, 18) }, { 50, 50 });
			break;
		case MissleType::sniper:
			mSprite = Sprite("bonuses\\weapon_sniper.png", pos, 2, { Sprite::Animation(2, 1), Sprite::Animation(2, 18) }, { 50, 50 });
			break;
		}
		break;
	}

	Visualizer::registerSprite(&mSprite);
}

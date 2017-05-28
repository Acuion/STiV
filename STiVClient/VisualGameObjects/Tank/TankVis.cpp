#include "TankVis.h"
#include "VisualGameObjects/Visualizer.h"

TankVis::TankVis(sf::Vector2f pos, bool client = false)
	: Tank(pos, client)
{
	mSprite = Sprite("tankBody.png", pos, 2, { Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 70,70 });
	Visualizer::registerSprite(&mSprite);

	mBarrel = new Sprite[2];
	mBarrel[0] = Sprite("missiles\\simpleBombBarrel.png", pos, 2, { Sprite::Animation(3, 2), Sprite::Animation(3, 30) }, { 70, 40 });
	mBarrel[0].setOrigin({ 0, 20 });

	mBarrel[1] = Sprite("missiles\\sniperBarrel.png", pos, 2, { Sprite::Animation(3, 15), Sprite::Animation(6, 23) }, { 100, 40 });
	mBarrel[1].setOrigin({ 0, 20 });

	Visualizer::registerSprite(&mBarrel[0]);
}

TankVis::TankVis()
	: Tank()
{
}

TankVis::~TankVis()
{
	Visualizer::unregisterSprite(&mBarrel[mCurrMissleType]);
	delete[] mBarrel;
}

void TankVis::update()
{
	Tank::update();

	mBarrel[mCurrMissleType].setRotation(tgMath::radToDeg(mAngle));
	mBarrel[mCurrMissleType].setPosition(getPosition());
	mBarrel[mCurrMissleType].update();
	if (!mBarrel[mCurrMissleType].isPlaying())
	{
		mBarrel[mCurrMissleType].setAnimation(0);
	}
}

bool TankVis::tryToLaunchMissile()
{
	if (Tank::tryToLaunchMissile())
	{
		mBarrel[mCurrMissleType].playAnimationOnce(1);
		return true;
	}
	return false;
}

void TankVis::setMissle(MissleType mt)
{
	Visualizer::unregisterSprite(&mBarrel[mCurrMissleType]);
	Tank::setMissle(mt);
	Visualizer::registerSprite(&mBarrel[mCurrMissleType]);
}

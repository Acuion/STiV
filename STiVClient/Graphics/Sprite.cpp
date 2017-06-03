#include "Sprite.h"

std::map<std::string, sf::Texture> TextureManager::mTextures;

Sprite::Sprite(std::string texture, sf::Vector2f pos, int animationsCount, std::vector<Animation> animations,
    sf::Vector2i frameSize, float rotation)
{
    mSprite.setTexture(TextureManager::GetTexture(texture));
    mCenterPos = pos;
    mAnimationsCount = animationsCount;
    mAnimations = animations;
    mFrameSize = frameSize;
    mAnimTim = Timer(1000 / mAnimations[0].mFps);
    mSprite.setRotation(rotation);
    mSprite.setOrigin({ frameSize.x / 2.0f, frameSize.y / 2.0f });
    mSprite.setTextureRect(sf::IntRect(0, 0, mFrameSize.x, mFrameSize.y));
    mStaticSprite = false;
    move({ 0,0 });
}

Sprite::Sprite(std::string texture, sf::Vector2f pos, float scale, float rotation)
{
    sf::Texture& txt = TextureManager::GetTexture(texture);
    mSprite.setTexture(txt);
    mSprite.setScale(scale, scale);
    mCenterPos = pos;
    mSprite.setRotation(rotation);
    mFrameSize = { static_cast<int>(mSprite.getTexture()->getSize().x), static_cast<int>(mSprite.getTexture()->getSize().y) };
    mSprite.setOrigin({ mFrameSize.x / 2.0f, mFrameSize.y / 2.0f });
    move({ 0,0 });
}

Sprite::Sprite(std::string texture, sf::Vector2f pos, sf::Vector2f size, float rotation)
{
    sf::Texture& txt = TextureManager::GetTexture(texture);
    txt.setRepeated(true);
    mSprite.setTexture(txt);
    mCenterPos = pos;
    mSprite.setRotation(rotation);
    mSprite.setTextureRect({ 0,0,static_cast<int>(size.x), static_cast<int>(size.y) });
    mSprite.setOrigin({ size.x / 2, size.y / 2 });
    move({ 0,0 });
}

Sprite::Sprite(sf::Vector2f size, sf::Color fill, sf::Vector2f pos, float rotation)
{
    mSprite.setTexture(TextureManager::GetPlaceholder(size, fill));
    mSprite.setTextureRect({ 0,0,static_cast<int>(size.x),static_cast<int>(size.y) });
    mCenterPos = pos;
    mSprite.setRotation(rotation);
    mFrameSize = { static_cast<int>(size.x), static_cast<int>(size.y) };
    mSprite.setOrigin({ static_cast<float>(mFrameSize.x / 2.0f), static_cast<float>(mFrameSize.y / 2.0f) });
    move({ 0,0 });
}

void Sprite::setAnimation(int index)
{
    mAnimTim = Timer(1000 / mAnimations[index].mFps);
    mPlayingOnce = false;
    mAnimationIndex = index;
    mFrame = 0;
    mSprite.setTextureRect(sf::IntRect(mFrame * mFrameSize.x, mAnimationIndex * mFrameSize.y, mFrameSize.x, mFrameSize.y));
    mAnimations[index].mStartTime = Timer::getElapsedTime();
    mRunning = true;
}

void Sprite::setPosition(const sf::Vector2f& pos)
{
    mCenterPos = pos;
    mSprite.setPosition(mCenterPos);
}

void Sprite::move(const sf::Vector2f& delta)
{
    mCenterPos += delta;
    mSprite.setPosition(mCenterPos);
}

void Sprite::setRotation(float deg)
{
    mSprite.setRotation(deg);
}

void Sprite::setScale(const sf::Vector2f& scale)
{
    mSprite.setScale(scale);
}

void Sprite::setOrigin(const sf::Vector2f& orig)
{
    mSprite.setOrigin(orig);
}

void Sprite::playAnimationOnce(int index)
{
    mAnimTim = Timer(1000 / mAnimations[index].mFps);
    mPlayingOnce = true;
    mAnimationIndex = index;
    mFrame = 0;
    mSprite.setTextureRect(sf::IntRect(mFrame * mFrameSize.x, mAnimationIndex * mFrameSize.y, mFrameSize.x, mFrameSize.y));
    mAnimations[index].mStartTime = Timer::getElapsedTime();
    mRunning = true;
}

bool Sprite::isPlaying() const
{
    return mRunning;
}

void Sprite::rotate(float deltaDeg)
{
    mSprite.rotate(deltaDeg);
}

sf::Vector2f Sprite::getPosition() const
{
    return mCenterPos;
}

float Sprite::getRotation() const
{
    return mSprite.getRotation();
}

void Sprite::update()
{
    if (!mStaticSprite && mAnimTim.isExpired() && (mRunning || mFrame))//TODO: animation with shader
    {
        mSprite.setTextureRect(sf::IntRect(mFrame * mFrameSize.x, mAnimationIndex * mFrameSize.y, mFrameSize.x, mFrameSize.y));
        mFrame = (mFrame + 1) % mAnimations[mAnimationIndex].mFramesCount;
        if (!mFrame && mPlayingOnce)
            mRunning = false;
    }
}

void Sprite::draw(sf::RenderTexture & txt, const sf::FloatRect& viewRect)
{
    if (!mSprite.getGlobalBounds().intersects(viewRect))
        return;
    if (mAnimations.size())
        PostProcessingManager::addTargetToShaderProcessing(mAnimations[mAnimationIndex].mShaderToApply,
            mCenterPos, mAnimations[mAnimationIndex].mStartTime);
    txt.draw(mSprite);
}

void Sprite::draw(sf::RenderWindow & wnd)
{
    if (mAnimations.size())
        PostProcessingManager::addTargetToShaderProcessing(mAnimations[mAnimationIndex].mShaderToApply,
            mCenterPos, mAnimations[mAnimationIndex].mStartTime);
    wnd.draw(mSprite);
}

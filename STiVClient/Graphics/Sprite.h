#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Misc/Timer.h"
#include "Misc/Utilites.h"
#include "Graphics/Shaders/PostProcessingManager.h"

class TextureManager
{
	static std::map<std::string, sf::Texture> mTextures;
public:
	static sf::Texture& GetTexture(std::string& path)
	{
		path = "Content\\" + path;
		if (mTextures.find(path) == mTextures.end())
		{
			mTextures[path] = sf::Texture();
			mTextures[path].loadFromFile(path);
			mTextures[path].setSmooth(true);
		}
		return mTextures[path];
	}

	static sf::Texture& GetPlaceholder(const sf::Vector2f& size, sf::Color fill)
	{
		std::string plh = std::to_string(size.x) + std::to_string(size.y) + std::to_string(fill.toInteger());
		if (mTextures.find(plh) == mTextures.end())
		{
			mTextures[plh] = sf::Texture();
			mTextures[plh].create(1, 1);
			mTextures[plh].setRepeated(true);
			mTextures[plh].update(new sf::Uint8[4]{ fill.r, fill.g, fill.b, fill.a });
			mTextures[plh].setSmooth(true);
		}
		return mTextures[plh];
	}
};

class Sprite
{
public:
    struct Animation
    {
        int mFramesCount;
        int mFps;
        PostProcessingManager::MultitargetShaderType mShaderToApply;
        int mStartTime;

        Animation(int framesCount, int fps,
            PostProcessingManager::MultitargetShaderType shader = PostProcessingManager::MultitargetShaderType::None)
            : mFramesCount(framesCount)
            , mFps(fps)
            , mShaderToApply(shader)
            , mStartTime(0)
        {
        }
    };

	Sprite() {}
	Sprite(std::string texture, sf::Vector2f pos, int animationsCount, std::vector<Animation> animations,
        sf::Vector2i frameSize, float rotation = 0);//animated
	Sprite(std::string texture, sf::Vector2f pos, float scale = 1, float rotation = 0);//static
	Sprite(std::string texture, sf::Vector2f pos, sf::Vector2f size, float rotation = 0);//static rep
	Sprite(sf::Vector2f size, sf::Color fill, sf::Vector2f pos, float rotation = 0);//placeholder

	void setAnimation(int index);
	void setPosition(const sf::Vector2f& pos);
	void setRotation(float deg);
	void setScale(const sf::Vector2f& scale);
	void setOrigin(const sf::Vector2f& orig);

	void playAnimationOnce(int index);
	bool isPlaying() const;

	void move(const sf::Vector2f& delta);
	void rotate(float deltaDeg);

	sf::Vector2f getPosition() const;
	float getRotation() const;

	void update();
	void draw(sf::RenderTexture & txt, const sf::FloatRect& viewRect);
    void draw(sf::RenderWindow& wnd);
private:
    sf::Sprite mSprite;
    sf::Vector2f mCenterPos;
    int mAnimationsCount;
    int mAnimationIndex = 0;//y
    int mFrame = 0;//x
    sf::Vector2i mFrameSize;
    std::vector<Animation> mAnimations;
    Timer mAnimTim;
    bool mRunning = true, mPlayingOnce = false, mStaticSprite = true;
};

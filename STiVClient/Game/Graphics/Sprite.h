#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Game/Misc/Timer.h"
#include "Game/Misc/tgMath.h"
#include "Game/Graphics/Shaders/PostProcessingManager.h"

class TextureManager
{
	static std::map<std::string, sf::Texture> mTextures;
	static std::map<char, std::pair<sf::Vector2f, std::string>> mTxtInfos;
	static std::map<std::string, char> mNumByName;
public:
	static void InfosInit()
	{
		mTxtInfos[1] = { {70, 70}, "tankBody.png" };
		mNumByName["Content\\tankBody.png"] = 1;
		mTxtInfos[2] = { {400, 400}, "planet.png" };
		mNumByName["Content\\planet.png"] = 2;
		mTxtInfos[3] = { {70, 40}, "missiles\\simpleBombBarrel.png" };
		mNumByName["Content\\missiles\\simpleBombBarrel.png"] = 3;
		mTxtInfos[4] = { {20, 20}, "missiles\\simpleBombMissile.png" };
		mNumByName["Content\\missiles\\simpleBombMissile.png"] = 4;
		mTxtInfos[5] = { {100, 40}, "missiles\\SniperBarrel.png" };
		mNumByName["Content\\missiles\\sniperBarrel.png"] = 5;
		mTxtInfos[6] = { {25, 24}, "missiles\\sniperMissile.png" };
		mNumByName["Content\\missiles\\sniperMissile.png"] = 6;
		mTxtInfos[7] = { {50, 50}, "bonuses\\heal.png" };
		mNumByName["Content\\bonuses\\heal.png"] = 7;
		mTxtInfos[8] = { { 50, 50 }, "bonuses\\weapon_simpleBomb.png" };
		mNumByName["Content\\bonuses\\weapon_simpleBomb.png"] = 8;
		mTxtInfos[9] = { { 50, 50 }, "bonuses\\weapon_sniper.png" };
		mNumByName["Content\\bonuses\\weapon_sniper.png"] = 9;
	}

	static char numByTxtName(const std::string& name)
	{
        //TODO: register new textures
		return mNumByName[name];
	}

	static std::pair<sf::Vector2f, std::string>& GetTextureInfoByNum(char num)
	{
		return mTxtInfos[num];
	}

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
        float mStartTime;

        Animation(int framesCount, int fps,
            PostProcessingManager::MultitargetShaderType shader = PostProcessingManager::MultitargetShaderType::None)
            : mFramesCount(framesCount)
            , mFps(fps)
            , mShaderToApply(shader)
            , mStartTime(0)
        {
        }
    };

	Sprite() {};
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
	bool isPlaying();

	void move(const sf::Vector2f& delta);
	void rotate(float deltaDeg);

	sf::Vector2f getPosition() const;
	float getRotation() const;

	void update();
	void draw(sf::RenderTexture & txt, const sf::FloatRect& viewRect);
    void draw(sf::RenderWindow& wnd);

	void pack(unsigned char* data, int& ptr);
	void unpack(unsigned char* data, int& ptr);
private:
    sf::Sprite mSprite;
    sf::Vector2f mCenterPos;
    int mAnimationsCount;
    int mAnimationIndex = 0;//y
    int mFrame = 0;//x
    int mTextureNumber;
    sf::Vector2i mFrameSize;
    std::vector<Animation> mAnimations;
    Timer mAnimTim;
    bool mRunning = true, mPlayingOnce = false, mStaticSprite = true;
};

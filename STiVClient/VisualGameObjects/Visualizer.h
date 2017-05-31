#pragma once

#include "LogicalGameObjects/GameObject.h"
#include <list>
#include <mutex>

class Visualizer
{
    static std::list<Sprite*> mDrawLayer;
	static std::mutex mObjectsAccess;
public:
	static const size_t mSizeofSpecificClientData = 5;
	static const size_t mSizeofPackedSprite = 10;

    static void registerSprite(Sprite* sp)
    {
        mDrawLayer.push_back(sp);
    }

    static void unregisterSprite(Sprite* sp)
    {
        mDrawLayer.remove(sp);
    }

    static void draw(sf::RenderTexture& wnd, sf::FloatRect viewRect)
    {
        mObjectsAccess.lock();
        for (auto iter = mDrawLayer.rbegin(); iter != mDrawLayer.rend(); ++iter)
            (*iter)->draw(wnd, viewRect);
        mObjectsAccess.unlock();
    }
};

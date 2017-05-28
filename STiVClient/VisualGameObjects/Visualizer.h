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

    static unsigned char* pack(int& ptr)
    {
        mObjectsAccess.lock();
        unsigned char* data = new unsigned char[mDrawLayer.size() * mSizeofPackedSprite + mSizeofSpecificClientData + 2];//2bytes for sizeof msg
        for (auto s : mDrawLayer)
            s->pack(data, ptr);
        mObjectsAccess.unlock();
        return data;
    }

    static void unpack(unsigned char* data, int count)
    {
        mObjectsAccess.lock();
        while (mDrawLayer.size())
        {
            delete *mDrawLayer.begin();
            mDrawLayer.pop_front();
        }
        count /= mSizeofPackedSprite;
        Sprite* sp;
        int ptr = 0;
        for (int i = 0; i < count; ++i)
        {
            sp = new Sprite();
            sp->unpack(data, ptr);
            mDrawLayer.push_back(sp);
        }
        mObjectsAccess.unlock();
    }
};

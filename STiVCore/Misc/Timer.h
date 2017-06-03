#pragma once

#include <ctime>
#include <SFML/System.hpp>

class Timer
{
    int mLastTick, mPeriod;
    static sf::Clock mClock;
public:
    Timer() {};
    Timer(int periodInMs)
    {
        mPeriod = periodInMs;
        mLastTick = clock();
    }
    bool isExpired()
    {
        if (clock() - mLastTick >= mPeriod)
        {
            mLastTick = clock();
            return true;
        }
        return false;
    }
    static int getElapsedTime()
    {
        return mClock.getElapsedTime().asMilliseconds();
    }
};
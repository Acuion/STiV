#pragma once

#include <ctime>
#include <SFML/System.hpp>

class CallTimer
{
    int mPeriod, mCallsMade;
public:
    CallTimer(int periodInCalls)
    {
        mPeriod = periodInCalls;
        mCallsMade = 0;
    }

    bool isExpired()
    {
        mCallsMade++;
        if (mCallsMade == mPeriod)
        {
            mCallsMade = 0;
            return true;
        }
        return false;
    }
};

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
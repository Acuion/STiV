#include "GameLevel.h"
#include "Network/NetworkUtils.h"
#include <fstream>

using namespace NetworkUtils;

GameLevel::GameLevel(const std::string& levelName)
{
    mCurrLevelName = levelName;
    std::ifstream levelfile("levels\\" + levelName + ".tgl");
    int goc;
    int x, y, radius, power;
    levelfile >> x >> y;
    mCurrLevelSize.x = x;
    mCurrLevelSize.y = y;
    levelfile >> x >> y;
    mSpawnPoint.x = x;
    mSpawnPoint.y = y;

    levelfile >> goc;
    for (int i = 0; i < goc; ++i)
    {
        levelfile >> x >> y >> radius >> power;
        mPlanets.push_back({ {x,y}, radius, power });
    }

    levelfile >> goc;
    for (int i = 0; i < goc; ++i)
    {
        levelfile >> x >> y;
        mBonusSpawnPoints.push_back(sf::Vector2i(static_cast<int>(x), static_cast<int>(y)));
    }
}

const sf::Vector2i & GameLevel::getCurrLevelSize() const
{
    return mCurrLevelSize;
}

const sf::Vector2i & GameLevel::getSpawnPoint() const
{
    return mSpawnPoint;
}

const std::string& GameLevel::getCurrLevelName() const
{
    return mCurrLevelName;
}

const std::vector<GameLevel::PlanetInfo>& GameLevel::getPlanets() const
{
    return mPlanets;
}

const std::vector<sf::Vector2i>& GameLevel::getBonusSpawnPoints() const
{
    return mBonusSpawnPoints;
}

sf::Packet& operator<<(sf::Packet& packet, const GameLevel::PlanetInfo& planetInfo)
{
    return packet << planetInfo.mPos << planetInfo.mPower << planetInfo.mRadius;
}

sf::Packet& operator>>(sf::Packet& packet, GameLevel::PlanetInfo& planetInfo)
{
    return packet >> planetInfo.mPos >> planetInfo.mPower >> planetInfo.mRadius;
}

sf::Packet& operator<<(sf::Packet& packet, const GameLevel& gameLevel)
{
    packet << gameLevel.mCurrLevelSize << gameLevel.mSpawnPoint;
    packet << gameLevel.mCurrLevelName;
    packet << gameLevel.mPlanets;
    packet << gameLevel.mBonusSpawnPoints;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, GameLevel& gameLevel)
{
    packet >> gameLevel.mCurrLevelSize >> gameLevel.mSpawnPoint;
    packet >> gameLevel.mCurrLevelName;
    packet >> gameLevel.mPlanets;
    packet >> gameLevel.mBonusSpawnPoints;
    return packet;
}

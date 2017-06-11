#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <SFML/Network/Packet.hpp>

class GameLevel
{
public:
    struct PlanetInfo
    {
        sf::Vector2i mPos;
        sf::Int32 mRadius, mPower;
    };

    explicit GameLevel(const std::string& levelName);

    const sf::Vector2i& getCurrLevelSize() const;
    const sf::Vector2i& getSpawnPoint() const;

    const std::string& getCurrLevelName() const;

    const std::vector<PlanetInfo>& getPlanets() const;
    const std::vector<sf::Vector2i>& getBonusSpawnPoints() const;

    friend sf::Packet& operator<<(sf::Packet& packet, const GameLevel& gameLevel);
    friend sf::Packet& operator>>(sf::Packet& packet, GameLevel& gameLevel);
private:
    sf::Vector2i mCurrLevelSize, mSpawnPoint;
    std::string mCurrLevelName;
    std::vector<PlanetInfo> mPlanets;
    std::vector<sf::Vector2i> mBonusSpawnPoints;
};

sf::Packet& operator<<(sf::Packet& packet, const GameLevel::PlanetInfo& planetInfo);
sf::Packet& operator>>(sf::Packet& packet, GameLevel::PlanetInfo& planetInfo);
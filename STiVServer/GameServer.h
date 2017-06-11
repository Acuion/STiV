#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include "LogicalGameObjects/GameObjectManager.h"
#include "STGClient.h"
#include "Misc/Timer.h"
#include <mutex>
#include "GameLevel.h"

class GameServer
{
    sf::Vector2i mCurrLevelSize;
    sf::Vector2i mSpawnPoint;

    Timer mSpawnBonus;
    std::vector<sf::Vector2f> mBonusSpawnPoints;

    sf::TcpListener mTcpServer;
    std::thread* mAccThread = nullptr;
    std::list<STGClient> mClients;
    std::mutex mClientsWork;
    Timer mSendTimer;
    GameLevel mCurrLevel;

    bool mIsWorking = true;

    void acceptClients();
public:
    explicit GameServer(const std::string& levelName);

    ~GameServer();

    bool listen(int srvPort);

    void update(int dt);
};


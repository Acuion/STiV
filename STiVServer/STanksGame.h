#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include "LogicalGameObjects/GameObjectManager.h"
#include "STGClient.h"
#include "Misc/Timer.h"
#include <mutex>

class STanksGame
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

    bool mIsWorking = true;

    void acceptClients();
public:
    STanksGame();

    ~STanksGame();

    bool listen(int srvPort);

    void update(int dt);

    void loadLevel(std::string name);
};


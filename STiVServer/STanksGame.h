#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include <chrono>
#include "LogicalGameObjects/GameObjectManager.h"
#include "STGClient.h"
#include "Misc/Timer.h"
#include <mutex>

using namespace std::chrono_literals;

class STanksGame
{
    sf::Vector2i mScreenSize;
    sf::Vector2f mHalfScreen;
    sf::Vector2i mCurrLevelSize;
    sf::Vector2i mSpawnPoint;

    Timer mSpawnBonus;
    std::vector<sf::Vector2f> mBonusSpawnPoints;

    sf::TcpListener mTcpServer;
    std::thread* mAccThread = nullptr, *mLsnThread = nullptr;
    std::list<STGClient> mClients;
    std::mutex mClientsWork;
    sf::SocketSelector mSocketSelector;
    Timer mSendTimer;

    sf::TcpSocket mTcpClient;

    bool mIsWorking = true;

    void acceptClients();
    void listenClients();
public:
    STanksGame();

    ~STanksGame();

    bool listen(int srvPort);

    void update(int dt);

    void loadLevel(std::string name);
};


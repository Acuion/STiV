#include "GameServer.h"
#include "LogicalGameObjects/Bonus.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include "GameObjectsFactory.h"
#include "GameLevel.h"

GameServer::GameServer(const std::string& levelName)
    : mSpawnBonus(3000)
    , mSendTimer(50)
    , mCurrLevel(levelName)
{
}

GameServer::~GameServer()
{
    mIsWorking = false;
    if (mAccThread)
        mAccThread->join();
}

void GameServer::acceptClients()
{
    using namespace std::chrono_literals;

    while (mIsWorking)
    {
        sf::TcpSocket *curSock = new sf::TcpSocket();
        while (mTcpServer.accept(*curSock) != sf::Socket::Status::Done)
        {
            std::this_thread::sleep_for(100ms);
            if (!mIsWorking)
                return;
        }
        //hello!
        mClientsWork.lock();
        mClients.emplace_back(mCurrLevel, mSpawnPoint, curSock, mCurrLevelSize);
        std::cout << "New client!\n";
        mClientsWork.unlock();
    }
}

bool GameServer::listen(int srvPort)
{
    if (mTcpServer.listen(srvPort) != sf::Socket::Done)
        return false;
    mTcpServer.setBlocking(false);
    mAccThread = new std::thread(&GameServer::acceptClients, this);
    return true;
}

void GameServer::update(int dt)
{
    if (mSpawnBonus.isExpired())
    {
        sf::Vector2f bonusPos = mBonusSpawnPoints[rand() % mBonusSpawnPoints.size()];
        int rnd = rand();
        if (rnd % 2)
            GameObjectsFactory::newBonus(BonusType::bt_heal, 10, bonusPos);
        else
        {
            switch (rnd % 3)
            {
            case 0:
                GameObjectsFactory::newBonus(BonusType::bt_weapon, MissleType::sniper, bonusPos);
                break;
            case 1:
                GameObjectsFactory::newBonus(BonusType::bt_weapon, MissleType::simpleBomb, bonusPos);
                break;
            }
        }
    }

    mClientsWork.lock();
    for (auto& client : mClients)
        client.applyEvents(mSpawnPoint);

    ServerGameObjectManager::getInstance().update(dt);

    if (mSendTimer.isExpired())
    {
        for (auto& client : mClients)
            client.sendNewObjects(ServerGameObjectManager::getInstance().getNewObjects());
        ServerGameObjectManager::getInstance().clearNewObjects();
    }

    mClientsWork.unlock();
}

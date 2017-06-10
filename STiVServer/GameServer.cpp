#include "GameServer.h"
#include "LogicalGameObjects/Bonus.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include "GameObjectsFactory.h"

GameServer::GameServer()
    : mSpawnBonus(3000)
    , mSendTimer(50)
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
        mClients.emplace_back(mCurrLevelName, mSpawnPoint, curSock, mCurrLevelSize);
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

void GameServer::loadLevel(const std::string& name)
{
    mCurrLevelName = name;
    std::ifstream levelfile("levels\\" + name + ".tgl");
    int goc;
    int x, y, radius, power;
    levelfile >> x >> y;
    ServerGameObjectManager::getInstance().reset(x, y);
    mCurrLevelSize.x = x;
    mCurrLevelSize.y = y;
    levelfile >> x >> y;
    mSpawnPoint.x = x;
    mSpawnPoint.y = y;

    levelfile >> goc;
    for (int i = 0; i < goc; ++i)
    {
        levelfile >> x >> y >> radius >> power;

        GameObjectsFactory::newPlanet(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)), radius, power);
    }

    levelfile >> goc;
    mBonusSpawnPoints.clear();
    for (int i = 0; i < goc; ++i)
    {
        levelfile >> x >> y;
        mBonusSpawnPoints.push_back(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    }
}

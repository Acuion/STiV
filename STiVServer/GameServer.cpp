#include "GameServer.h"
#include "LogicalGameObjects/Bonus.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include "GameObjectsFactory.h"
#include "GameLevel.h"

GameServer::GameServer(const std::string& levelName)
    : mSpawnBonus(3000)
    , mCurrLevel(levelName)
{
    ServerGameObjectManager::getInstance().reset(mCurrLevel.getCurrLevelSize().x, mCurrLevel.getCurrLevelSize().y);
    for (auto planetInfo : mCurrLevel.getPlanets())
    {
        GameObjectsFactory::newPlanet(static_cast<sf::Vector2f>(planetInfo.mPos), planetInfo.mRadius, planetInfo.mPower);
    }
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
        mClients.push_back(new STGClient(mCurrLevel, curSock));
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
        sf::Vector2f bonusPos = static_cast<sf::Vector2f>(mCurrLevel.getBonusSpawnPoints()[rand() % mCurrLevel.getBonusSpawnPoints().size()]);
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
    auto begin = mClients.begin();
    while (begin != mClients.end())
    {
        if ((*begin)->isDisconnected())
        {
            delete *begin;
            begin = mClients.erase(begin);
        }
        else
            ++begin;
    }

    for (auto& client : mClients)
        client->applyEvents();
    
    ServerGameObjectManager::getInstance().update(dt);
    mClientsWork.unlock();
}

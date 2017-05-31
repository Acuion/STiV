#include "STanksGame.h"
#include "LogicalGameObjects/Bonus.h"
#include <iostream>
#include <fstream>
#include "GameObjectsFactory.h"

STanksGame::STanksGame()
	: mSpawnBonus(3000)
	, mSendTimer(30)
{

}

STanksGame::~STanksGame()
{
	mIsWorking = false;
	if (mAccThread)
		mAccThread->join();
	if (mLsnThread)
		mLsnThread->join();
}

void STanksGame::acceptClients()
{
	while (mIsWorking)
	{
		sf::TcpSocket* curSock = new sf::TcpSocket();
		while (mTcpServer.accept(*curSock) != sf::Socket::Status::Done)
		{

			std::this_thread::sleep_for(100ms);
			if (!mIsWorking)
				return;
		}
		//hello!
		mClientsWork.lock();
		mSocketSelector.add(*curSock);
		mClients.emplace_back(mSpawnPoint, curSock, mCurrLevelSize);
		std::cout << "New client!\n";
		mClientsWork.unlock();
	}
}

void STanksGame::listenClients()
{
	bool wasDeleted;
	while (mIsWorking)
	{
		if (!mClients.size())
			std::this_thread::sleep_for(100ms);
		mSocketSelector.wait(sf::seconds(1));
		for (auto iter = mClients.begin(); iter != mClients.end();)
		{
			wasDeleted = false;
			STGClient& client = *iter;
			if (client.isReadyToUpdate(mSocketSelector))
			{
				mClientsWork.lock();
				client.updateFromNetwork();
				if (client.isDisconnected())
				{
					wasDeleted = true;
					client.unselectSocket(mSocketSelector);
					iter = mClients.erase(iter);
				}
				mClientsWork.unlock();
			}
			if (!wasDeleted)
				++iter;
		}
	}
}

bool STanksGame::listen(int srvPort)
{
	if (mTcpServer.listen(srvPort) != sf::Socket::Done)
		return false;
	mTcpServer.setBlocking(false);
	mAccThread = new std::thread(&STanksGame::acceptClients, this);
	mLsnThread = new std::thread(&STanksGame::listenClients, this);
	return true;
}

void STanksGame::update(int dt)
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
		client.applyEvents();

	for (auto& client : mClients)
		client.checkHP(mSpawnPoint);

	GameObjectManager::update(dt);

	if (mSendTimer.isExpired())
	{
		int ptr = 2;
		unsigned char* data = Visualizer::pack(ptr);
		int pt0 = 0;
		Utilites::write2Bytes(ptr - 2, data, pt0);
		for (auto& client : mClients)
			client.sendWorld(data, ptr);
		delete[] data;
	}
	mClientsWork.unlock();
}

void STanksGame::loadLevel(std::string name)
{
	std::ifstream levelfile("levels\\" + name + ".tgl");
	int goc;
	int x, y, radius, power;
	levelfile >> x >> y;
	GameObjectManager::reset(x, y);
	mCurrLevelSize.x = x;
	mCurrLevelSize.y = y;
	levelfile >> x >> y;
	mSpawnPoint.x = x;
	mSpawnPoint.y = y;

	levelfile >> goc;
	for (int i = 0; i < goc; ++i)
	{
		levelfile >> x >> y >> radius >> power;

		GameObjectsFactory::newPlanet({ (float)x, (float)y }, radius, power);
	}

	levelfile >> goc;
	mBonusSpawnPoints.clear();
	for (int i = 0; i < goc; ++i)
	{
		levelfile >> x >> y;
		mBonusSpawnPoints.push_back({ (float)x,(float)y });
	}
}

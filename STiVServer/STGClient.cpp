#include "STGClient.h"
#include <iostream>
#include "GameObjectsFactory.h"

STGClient::~STGClient()
{
	GameObjectManager::forcedDelete(mTank);
	delete mSocket;
}

STGClient::STGClient(sf::Vector2i spawnPoint, sf::TcpSocket* socket, sf::Vector2i levelSize)
{
	mTank = GameObjectsFactory::newTank((sf::Vector2f)spawnPoint, true);
	mSocket = socket;

	unsigned char* data = new unsigned char[4];
	int ptr = 0;
	tgMath::write2Bytes(levelSize.x, data, ptr);
	tgMath::write2Bytes(levelSize.y, data, ptr);
	mSocket->send(data, 4);
	delete[] data;
}

void STGClient::unselectSocket(sf::SocketSelector & selector)
{
	selector.remove(*mSocket);
}

void STGClient::sendWorld(unsigned char * world, int size)
{
	tgMath::write2Bytes(mTank->getPosition().x, world, size);
	tgMath::write2Bytes(mTank->getPosition().y, world, size);
	world[size++] = mTank->getHP() / 10;
	mSocket->send(world, size);
}

bool STGClient::isReadyToUpdate(sf::SocketSelector& selector)
{
	return selector.isReady(*mSocket);
}

void STGClient::updateFromNetwork()
{
	unsigned char data[5];
	unsigned int rcv;
	int angle;

	int ptr = 0;
	if (mSocket->receive(data, 3, rcv) == sf::Socket::Disconnected)
	{
		mDisconnected = true;
		std::cout << "Client disconnected\n";
		return;
	}
	angle = tgMath::read2Bytes(data, ptr) - 600;
	mMousePressed = data[ptr++];

	mTank->setBarrelAngle(tgMath::degToRad(angle));
}

bool STGClient::isDisconnected()
{
	return mDisconnected;
}

void STGClient::applyEvents()
{
	if (mMousePressed)
		mTank->tryToLaunchMissile();
}

void STGClient::checkHP(const sf::Vector2i& spawnPoint)
{
	if (mTank->getHP() <= 0)
		mTank->respawnAt(spawnPoint);
}

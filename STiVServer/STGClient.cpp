#include "STGClient.h"
#include <iostream>
#include "GameObjectsFactory.h"

STGClient::~STGClient()
{
    GameObjectManager::forcedDelete(mTank);
    delete mSocket;
}

STGClient::STGClient(sf::Vector2i spawnPoint, sf::TcpSocket& socket, sf::Vector2i levelSize)
    : mSocket(socket)
{
    mTank = GameObjectsFactory::newTank(static_cast<sf::Vector2f>(spawnPoint), true);


    mSocket.send(data, 4);
}

void STGClient::sendWorld(unsigned char * world, int size)
{
    Utilites::write2Bytes(mTank->getPosition().x, world, size);
    Utilites::write2Bytes(mTank->getPosition().y, world, size);
    world[size++] = mTank->getHP() / 10;
    mSocket->send(world, size);
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
    angle = Utilites::read2Bytes(data, ptr) - 600;
    mMousePressed = data[ptr++];

    mTank->setBarrelAngle(Utilites::degToRad(angle));
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

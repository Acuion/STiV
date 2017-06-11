#pragma once

#include "LogicalGameObjects/Tank/Tank.h"
#include <SFML/Network.hpp>
#include <mutex>
#include "GameLevel.h"

class STGClient
{
public:
    STGClient(const GameLevel& gameLevel, sf::TcpSocket* socket);
    ~STGClient();

    void sendNewObjects(const std::vector<GameObject*> objects);

    void applyEvents();

    bool isDisconnected() const;
    Tank* getPlayerTank() const;
private:
    Tank* mTank = nullptr;
    sf::TcpSocket* mSocket;
    bool mMousePressed = false;
    bool mDisconnected = false;

    static void packConstructorObjectsInfo(sf::Packet& packet, GameObject* obj);
    static void packChangingObjectsInfo(sf::Packet& packet, GameObject* obj);

    std::mutex mNewObjectsLock;
    std::vector<GameObject*> mNewObjects;

    const GameLevel& mCurrGameLevel;

    std::thread *mCommThread = nullptr;
    void clientComm();
};

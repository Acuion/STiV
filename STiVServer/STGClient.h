#pragma once

#include "LogicalGameObjects/Tank/Tank.h"
#include <SFML/Network.hpp>
#include <mutex>

class GameLevel;

class STGClient
{
public:
    STGClient(const GameLevel& gameLevel, sf::Vector2i spawnPoint, sf::TcpSocket* socket, sf::Vector2i levelSize);
    ~STGClient();

    void sendNewObjects(const std::vector<GameObject*> objects);

    void applyEvents(sf::Vector2i spawnPoint);

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

    std::thread *mCommThread = nullptr;
    void clientComm();
};

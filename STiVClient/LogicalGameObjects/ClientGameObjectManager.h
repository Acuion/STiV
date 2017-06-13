#pragma once

#include "LogicalGameObjects/GameObjectManager.h"
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>

class Tank;

class ClientGameObjectManager : public GameObjectManager
{
public:
    static ClientGameObjectManager& getInstance();

    void updateFromServer(sf::TcpSocket& socket);
    Tank* fillFromServerAndGetPlayerTank(sf::Packet& packet);
protected:
    void doGravity() override;
private:
    ClientGameObjectManager();
    void operator=(const ClientGameObjectManager&) = delete;

    std::map<int, GameObject*> mObjectsIndex;
    sf::Int32 mPlayerTankNum;
    sf::Packet mPrvPacket;
};

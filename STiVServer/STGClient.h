#pragma once

#include "LogicalGameObjects/Tank/Tank.h"
#include <SFML/Network.hpp>

class STGClient
{
    Tank* mTank = nullptr;
    sf::TcpSocket& mSocket;
    bool mMousePressed = false;
    bool mDisconnected = false;
public:
    STGClient(sf::Vector2i spawnPoint, sf::TcpSocket& socket, sf::Vector2i levelSize);
    ~STGClient();
    //todo: передавать список новых объектов, чтобы клиент потом отправил в своЄм потоке
    //хранить их список внутри GoM
    //в клиенте интерпол€цию таки сделать
    void sendWorld(unsigned char* world, int size);
    void updateFromNetwork();
    bool isDisconnected();
    void applyEvents();
    void checkHP(const sf::Vector2i& spawnPoint);
};
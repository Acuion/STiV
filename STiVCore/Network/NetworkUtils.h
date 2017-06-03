#pragma once

#include <SFML/Network.hpp>

namespace NetworkUtils
{
    enum ObjectTypeEnum
    {
        ObjBonus,
        ObjTank,
        ObjMissileSniper,
        ObjMissileSimpleBomb,
        ObjPlanet
    };

    template<typename T>
    void receiveDisconnectCheck(sf::TcpSocket& rcvSocket, T& obj)
    {
        size_t size;
        if (rcvSocket.receive(&obj, sizeof(obj), size) != sf::Socket::Status::Done)
            throw sf::Socket::Status::Disconnected;
    }

    template<typename T>
    void sendDisconenctCheck(sf::TcpSocket& sndSocket, const T& obj)
    {
        size_t size;
        if (sndSocket.send(&obj, sizeof(obj), size) != sf::Socket::Status::Done)
            throw sf::Socket::Status::Disconnected;
    }
}
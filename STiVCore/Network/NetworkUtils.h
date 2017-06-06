#pragma once

#include <SFML/Network.hpp>

namespace NetworkUtils
{
    enum ObjectTypeEnum
    {
        ObjStatic,
        ObjBonus,
        ObjTank,
        ObjMissileSniper,
        ObjMissileSimpleBomb,
        ObjPlanet
    };

    inline sf::Packet readPacket(sf::TcpSocket& socket)
    {
        sf::Packet packet;
        socket.receive(packet);
        return packet;
    }

    inline sf::Packet& operator>>(sf::Packet& packet, sf::Vector2f& vector)
    {
        return packet >> vector.x >> vector.y;
    }

    inline sf::Packet& operator>>(sf::Packet& packet, sf::Vector2i& vector)
    {
        sf::Int32 x, y;
        packet >> x >> y;
        vector.x = x;
        vector.y = y;
        return packet;
    }

    inline sf::Packet& operator>>(sf::Packet& packet, b2Vec2& vector)
    {
        float x, y;
        packet >> x >> y;
        vector.x = x;
        vector.y = y;
        return packet;
    }

    inline sf::Packet& operator<<(sf::Packet& packet, sf::Vector2f& vector)
    {
        return packet << vector.x << vector.y;
    }

    inline sf::Packet& operator<<(sf::Packet& packet, sf::Vector2i& vector)
    {
        sf::Int32 x = vector.x, y = vector.y;
        packet << x << y;
        return packet;
    }

    inline sf::Packet& operator<<(sf::Packet& packet, b2Vec2& vector)
    {
        float x = vector.x, y = vector.y;
        packet << x << y;
        return packet;
    }
}
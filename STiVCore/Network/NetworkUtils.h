#pragma once
#include <vector>
#include <Box2D/Box2D.h>
#include <SFML/Network.hpp>

namespace NetworkUtils
{
    const int cNetworkDelayInCalls = 2;//1call ~ 10ms

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

    inline sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2f& vector)
    {
        return packet << vector.x << vector.y;
    }

    inline sf::Packet& operator<<(sf::Packet& packet, const sf::Vector2i& vector)
    {
        sf::Int32 x = vector.x, y = vector.y;
        packet << x << y;
        return packet;
    }

    inline sf::Packet& operator<<(sf::Packet& packet, const b2Vec2& vector)
    {
        float x = vector.x, y = vector.y;
        packet << x << y;
        return packet;
    }

    template<typename T>
    sf::Packet& operator<<(sf::Packet& packet, const std::vector<T>& vec)
    {
        packet << static_cast<sf::Uint32>(vec.size());
        for (auto& x : vec)
            packet << x;
        return packet;
    }
    
    template<typename T>
    sf::Packet& operator>>(sf::Packet& packet, std::vector<T>& vec)
    {
        sf::Uint32 size;
        T temp;
        packet >> size;

        while (size--)
        {
            packet >> temp;
            vec.push_back(temp);
        }

        return packet;
    }
}
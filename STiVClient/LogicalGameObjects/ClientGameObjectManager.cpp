﻿#include "ClientGameObjectManager.h"
#include "Network/NetworkUtils.h"
#include "../GameObjectsFactory.h"

using namespace NetworkUtils;

ClientGameObjectManager::ClientGameObjectManager()
{
}

ClientGameObjectManager & ClientGameObjectManager::getInstance()
{
    static ClientGameObjectManager singleton;
    return singleton;
}

Tank* ClientGameObjectManager::fillFromServerAndGetPlayerTank(sf::TcpSocket& socket)
{
    auto packet = readPacket(socket);
    sf::Uint32 objectsInTheWorldCount;
    packet >> objectsInTheWorldCount;
    while (objectsInTheWorldCount--)
    {
        sf::Uint16 objectType;
        sf::Uint32 objectNum;
        packet >> objectType >> objectNum;
        switch (objectType)
        {
        case NetworkUtils::ObjBonus:
        {
            sf::Uint16 bonusType;
            sf::Int32 value;
            sf::Vector2f pos;
            packet >> bonusType;
            packet >> value;
            packet >> pos;
            mObjectsIndex[objectNum] = GameObjectsFactory::newBonus(static_cast<BonusType>(bonusType), value, pos);
        }
        break;
        case  NetworkUtils::ObjTank:
        {
            sf::Vector2f pos;
            packet >> pos;
            mObjectsIndex[objectNum] = GameObjectsFactory::newTank(pos, true); //todo: w/o true
        }
        break;
        case  NetworkUtils::ObjMissileSniper:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            packet >> pos;
            packet >> angle;
            packet >> tankLinVel;
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSniper(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjMissileSimpleBomb:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            packet >> pos;
            packet >> angle;
            packet >> tankLinVel;
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSimpleBomb(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjPlanet:
        {
            sf::Vector2f pos;
            sf::Int32 radius;
            sf::Int32 power;
            packet >> pos;
            packet >> radius;
            packet >> power;
            mObjectsIndex[objectNum] = GameObjectsFactory::newPlanet(pos, radius, power);
        }
        break;
        default:
            assert(false);
        }

        //info

        sf::Int32 hp;
        b2Vec2 pos, linVel;
        float32 angle, angVel;

        packet >> pos;
        packet >> linVel;
        packet >> angle;
        packet >> angVel;
        packet >> hp;

        mObjectsIndex[objectNum]->mBody->SetTransform(pos, angle);
        mObjectsIndex[objectNum]->mBody->SetLinearVelocity(linVel);
        mObjectsIndex[objectNum]->mBody->SetAngularVelocity(angVel);
        mObjectsIndex[objectNum]->mHP = hp;
    }

    sf::Int32 playerTankNum;
    packet >> playerTankNum;

    return static_cast<Tank*>(mObjectsIndex[playerTankNum]);
}

void ClientGameObjectManager::updateFromServer(sf::TcpSocket& socket)
{
    auto packet = readPacket(socket);
    sf::Uint32 newObjectsCount;
    packet >> newObjectsCount;
    while (newObjectsCount--)
    {
        sf::Int16 objectType;
        sf::Uint32 objectNum;
        packet >> objectType;
        packet >> objectNum;
        switch (objectType)
        {
        case NetworkUtils::ObjBonus:
        {
            sf::Int16 bonusType;
            sf::Int32 value;
            sf::Vector2f pos;
            packet >> bonusType;
            packet >> value;
            packet >> pos;
            mObjectsIndex[objectNum] = GameObjectsFactory::newBonus(static_cast<BonusType>(bonusType), value, pos);
        }
        break;
        case  NetworkUtils::ObjTank:
        {
            sf::Vector2f pos;
            packet >> pos;
            mObjectsIndex[objectNum] = GameObjectsFactory::newTank(pos, true); //todo: w/o true
        }
        break;
        case  NetworkUtils::ObjMissileSniper:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            packet >> pos;
            packet >> angle;
            packet >> tankLinVel;
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSniper(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjMissileSimpleBomb:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            packet >> pos;
            packet >> angle;
            packet >> tankLinVel;
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSimpleBomb(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjPlanet:
        {
            sf::Vector2f pos;
            sf::Int32 radius;
            sf::Int32 power;
            packet >> pos;
            packet >> radius;
            packet >> power;
            mObjectsIndex[objectNum] = GameObjectsFactory::newPlanet(pos, radius, power);
        }
        break;
        default:
            assert(false);
        }
    }

    sf::Uint32 objectsToUpdateCount;
    packet >> objectsToUpdateCount;

    while (objectsToUpdateCount--)
    {
        sf::Int32 objNum, hp;
        b2Vec2 pos, linVel;
        float32 angle, angVel;

        packet >> objNum;
        packet >> pos;
        packet >> linVel;
        packet >> angle;
        packet >> angVel;
        packet >> hp;

        mObjectsIndex[objNum]->mBody->SetTransform(pos, angle);
        mObjectsIndex[objNum]->mBody->SetLinearVelocity(linVel);
        mObjectsIndex[objNum]->mBody->SetAngularVelocity(angVel);
        mObjectsIndex[objNum]->mHP = hp;
    }
}

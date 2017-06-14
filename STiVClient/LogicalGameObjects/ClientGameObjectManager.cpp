#include "ClientGameObjectManager.h"
#include "Network/NetworkUtils.h"
#include "../GameObjectsFactory.h"
#include <iostream>

using namespace NetworkUtils;

ClientGameObjectManager::ClientGameObjectManager()
{
}

ClientGameObjectManager & ClientGameObjectManager::getInstance()
{
    static ClientGameObjectManager singleton;
    return singleton;
}

Tank* ClientGameObjectManager::fillFromServerAndGetPlayerTank(sf::Packet& packet)
{
    sf::Uint32 objectsInTheWorldCount;
    packet >> objectsInTheWorldCount;
    while (objectsInTheWorldCount--)
    {
        sf::Uint16 objectType;
        sf::Uint32 objectNum;
        packet >> objectType >> objectNum;
        switch (objectType)
        {
        case NetworkUtils::ObjStatic:
            mObjectsIndex[objectNum] = nullptr;
            break;
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
            std::string nickname;
            packet >> pos >> nickname;
            mObjectsIndex[objectNum] = GameObjectsFactory::newTank(pos, nickname);
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

        if (!mObjectsIndex[objectNum])
            continue;//static object
        mObjectsIndex[objectNum]->mBody->SetTransform(pos, angle);
        mObjectsIndex[objectNum]->mBody->SetLinearVelocity(linVel);
        mObjectsIndex[objectNum]->mBody->SetAngularVelocity(angVel);
        mObjectsIndex[objectNum]->mHP = hp;
    }

    packet >> mPlayerTankNum;

    auto tankPtr = dynamic_cast<Tank*>(mObjectsIndex[mPlayerTankNum]);
    tankPtr->setPlayerControlled(true);
    return tankPtr;
}

b2Vec2 operator*(const b2Vec2& lhs, const int& rhs)
{
    return { lhs.x * rhs, lhs.y * rhs };
}

void ClientGameObjectManager::updateFromServer(sf::TcpSocket& socket)
{
    if (!mPrvPacket.endOfPacket())
    {
        sf::Uint32 objectsToUpdateCount;
        mPrvPacket >> objectsToUpdateCount;
        while (objectsToUpdateCount--)
        {
            sf::Int32 objNum, hp;
            b2Vec2 pos, linVel;
            float32 angle, angVel;

            mPrvPacket >> objNum;
            mPrvPacket >> pos;
            mPrvPacket >> linVel;
            mPrvPacket >> angle;
            mPrvPacket >> angVel;
            mPrvPacket >> hp;

            if (!mObjectsIndex[objNum])
                continue;//static object
            mObjectsIndex[objNum]->mBody->SetTransform(pos, angle);
            mObjectsIndex[objNum]->mBody->SetLinearVelocity(linVel);
            mObjectsIndex[objNum]->mBody->SetAngularVelocity(angVel);
            mObjectsIndex[objNum]->mHP = hp;
        }

        sf::Uint32 newObjectsCount;
        mPrvPacket >> newObjectsCount;
        while (newObjectsCount--)
        {
            sf::Int16 objectType;
            sf::Uint32 objectNum;
            mPrvPacket >> objectType;
            mPrvPacket >> objectNum;
            switch (objectType)
            {
            case NetworkUtils::ObjBonus:
            {
                sf::Int16 bonusType;
                sf::Int32 value;
                sf::Vector2f pos;
                mPrvPacket >> bonusType;
                mPrvPacket >> value;
                mPrvPacket >> pos;
                mObjectsIndex[objectNum] = GameObjectsFactory::newBonus(static_cast<BonusType>(bonusType), value, pos);
            }
            break;
            case  NetworkUtils::ObjTank:
            {
                sf::Vector2f pos;
                std::string nickname;
                mPrvPacket >> pos >> nickname;
                if (objectNum != mPlayerTankNum)
                    mObjectsIndex[objectNum] = GameObjectsFactory::newTank(pos, nickname);
            }
            break;
            case  NetworkUtils::ObjMissileSniper:
            {
                sf::Vector2f pos;
                float angle;
                b2Vec2 tankLinVel;
                mPrvPacket >> pos;
                mPrvPacket >> angle;
                mPrvPacket >> tankLinVel;
                mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSniper(pos, angle, tankLinVel);
            }
            break;
            case  NetworkUtils::ObjMissileSimpleBomb:
            {
                sf::Vector2f pos;
                float angle;
                b2Vec2 tankLinVel;
                mPrvPacket >> pos;
                mPrvPacket >> angle;
                mPrvPacket >> tankLinVel;
                mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSimpleBomb(pos, angle, tankLinVel);
            }
            break;
            case  NetworkUtils::ObjPlanet:
            {
                sf::Vector2f pos;
                sf::Int32 radius;
                sf::Int32 power;
                mPrvPacket >> pos;
                mPrvPacket >> radius;
                mPrvPacket >> power;
                mObjectsIndex[objectNum] = GameObjectsFactory::newPlanet(pos, radius, power);
            }
            break;
            default:
                assert(false);
            }
        }

        sf::Uint32 playersCount;
        mPrvPacket >> playersCount;
        while (playersCount--)
        {
            sf::Int32 tankId, score, hp;
            float barrelAngle;
            sf::Int16 barrelType;
            mPrvPacket >> tankId >> score >> hp >> barrelAngle >> barrelType;

            if (tankId == mPlayerTankNum)
                continue;

            Tank * tank = dynamic_cast<Tank*>(mObjectsIndex[tankId]);
            tank->mHP = hp;
            tank->setBarrelAngle(barrelAngle);
            tank->setMissle(static_cast<MissleType>(barrelType));
        }

        std::vector<sf::Uint32> objectsToDel;
        mPrvPacket >> objectsToDel;
        for (auto& x : objectsToDel)
        {
            mObjectsIndex[x]->playDeathAnim();
        }
    }

    auto packet = readPacket(socket);
    mPrvPacket = packet;

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

        if (!mObjectsIndex[objNum])
            continue;//static object
        auto prvPos = mObjectsIndex[objNum]->mBody->GetPosition();
        auto prvAngle = mObjectsIndex[objNum]->mBody->GetAngle();
        mObjectsIndex[objNum]->mBody->SetLinearVelocity((pos - prvPos) * (100 / cNetworkDelayInCalls));
        mObjectsIndex[objNum]->mBody->SetAngularVelocity((angle - prvAngle) * (100 / cNetworkDelayInCalls));
    }
}

void ClientGameObjectManager::doGravity()
{
    //no grvity in client
}

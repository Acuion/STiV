#include "ClientGameObjectManager.h"
#include "Network/NetworkUtils.h"
#include "../GameObjectsFactory.h"

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
    unsigned int objectsInTheWorldCount;
    NetworkUtils::receiveDisconnectCheck(socket, objectsInTheWorldCount);
    while (objectsInTheWorldCount--)
    {
        NetworkUtils::ObjectTypeEnum objectType;
        unsigned int objectNum;
        NetworkUtils::receiveDisconnectCheck(socket, objectType);
        NetworkUtils::receiveDisconnectCheck(socket, objectNum);
        switch (objectType)
        {
        case NetworkUtils::ObjBonus:
        {
            BonusType bonusType;
            int value;
            sf::Vector2f pos;
            NetworkUtils::receiveDisconnectCheck(socket, bonusType);
            NetworkUtils::receiveDisconnectCheck(socket, value);
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            mObjectsIndex[objectNum] = GameObjectsFactory::newBonus(bonusType, value, pos);
        }
        break;
        case  NetworkUtils::ObjTank:
        {
            sf::Vector2f pos;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            mObjectsIndex[objectNum] = GameObjectsFactory::newTank(pos, true); //todo: w/o true
        }
        break;
        case  NetworkUtils::ObjMissileSniper:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            NetworkUtils::receiveDisconnectCheck(socket, angle);
            NetworkUtils::receiveDisconnectCheck(socket, tankLinVel);
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSniper(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjMissileSimpleBomb:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            NetworkUtils::receiveDisconnectCheck(socket, angle);
            NetworkUtils::receiveDisconnectCheck(socket, tankLinVel);
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSimpleBomb(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjPlanet:
        {
            sf::Vector2f pos;
            int radius;
            int power;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            NetworkUtils::receiveDisconnectCheck(socket, radius);
            NetworkUtils::receiveDisconnectCheck(socket, power);
            mObjectsIndex[objectNum] = GameObjectsFactory::newPlanet(pos, radius, power);
        }
        break;
        default:
            assert(false);
        }

        //info

        int hp;
        b2Vec2 pos, linVel;
        float32 angle, angVel;

        NetworkUtils::receiveDisconnectCheck(socket, pos);
        NetworkUtils::receiveDisconnectCheck(socket, linVel);
        NetworkUtils::receiveDisconnectCheck(socket, angle);
        NetworkUtils::receiveDisconnectCheck(socket, angVel);
        NetworkUtils::receiveDisconnectCheck(socket, hp);

        mObjectsIndex[objectNum]->mBody->SetTransform(pos, angle);
        mObjectsIndex[objectNum]->mBody->SetLinearVelocity(linVel);
        mObjectsIndex[objectNum]->mBody->SetAngularVelocity(angVel);
        mObjectsIndex[objectNum]->mHP = hp;
    }

    int playerTankNum;
    NetworkUtils::receiveDisconnectCheck(socket, playerTankNum);

    return static_cast<Tank*>(mObjectsIndex[playerTankNum]);
}

void ClientGameObjectManager::updateFromServer(sf::TcpSocket& socket)
{
    unsigned int newObjectsCount;
    NetworkUtils::receiveDisconnectCheck(socket, newObjectsCount);
    while (newObjectsCount--)
    {
        NetworkUtils::ObjectTypeEnum objectType;
        unsigned int objectNum;
        NetworkUtils::receiveDisconnectCheck(socket, objectType);
        NetworkUtils::receiveDisconnectCheck(socket, objectNum);
        switch (objectType)
        {
        case NetworkUtils::ObjBonus:
        {
            BonusType bonusType;
            int value;
            sf::Vector2f pos;
            NetworkUtils::receiveDisconnectCheck(socket, bonusType);
            NetworkUtils::receiveDisconnectCheck(socket, value);
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            mObjectsIndex[objectNum] = GameObjectsFactory::newBonus(bonusType, value, pos);
        }
        break;
        case  NetworkUtils::ObjTank:
        {
            sf::Vector2f pos;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            mObjectsIndex[objectNum] = GameObjectsFactory::newTank(pos, true); //todo: w/o true
        }
        break;
        case  NetworkUtils::ObjMissileSniper:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            NetworkUtils::receiveDisconnectCheck(socket, angle);
            NetworkUtils::receiveDisconnectCheck(socket, tankLinVel);
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSniper(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjMissileSimpleBomb:
        {
            sf::Vector2f pos;
            float angle;
            b2Vec2 tankLinVel;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            NetworkUtils::receiveDisconnectCheck(socket, angle);
            NetworkUtils::receiveDisconnectCheck(socket, tankLinVel);
            mObjectsIndex[objectNum] = GameObjectsFactory::newMissileSimpleBomb(pos, angle, tankLinVel);
        }
        break;
        case  NetworkUtils::ObjPlanet:
        {
            sf::Vector2f pos;
            int radius;
            int power;
            NetworkUtils::receiveDisconnectCheck(socket, pos);
            NetworkUtils::receiveDisconnectCheck(socket, radius);
            NetworkUtils::receiveDisconnectCheck(socket, power);
            mObjectsIndex[objectNum] = GameObjectsFactory::newPlanet(pos, radius, power);
        }
        break;
        default:
            assert(false);
        }
    }

    unsigned int objectsToUpdateCount;
    NetworkUtils::receiveDisconnectCheck(socket, objectsToUpdateCount);

    while (objectsToUpdateCount--)
    {
        int objNum, hp;
        b2Vec2 pos, linVel;
        float32 angle, angVel;

        NetworkUtils::receiveDisconnectCheck(socket, objNum);
        NetworkUtils::receiveDisconnectCheck(socket, pos);
        NetworkUtils::receiveDisconnectCheck(socket, linVel);
        NetworkUtils::receiveDisconnectCheck(socket, angle);
        NetworkUtils::receiveDisconnectCheck(socket, angVel);
        NetworkUtils::receiveDisconnectCheck(socket, hp);

        mObjectsIndex[objNum]->mBody->SetTransform(pos, angle);
        mObjectsIndex[objNum]->mBody->SetLinearVelocity(linVel);
        mObjectsIndex[objNum]->mBody->SetAngularVelocity(angVel);
        mObjectsIndex[objNum]->mHP = hp;
    }
}

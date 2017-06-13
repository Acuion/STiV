#include "STGClient.h"
#include "Network/NetworkUtils.h"
#include "GameObjectsFactory.h"
#include "LogicalGameObjects/StaticObject.h"
#include <iostream>

using namespace NetworkUtils;
using namespace std::chrono_literals;

STGClient::~STGClient()
{
    ServerGameObjectManager::getInstance().unsubscribeClient(this);
    ServerGameObjectManager::getInstance().forcedDelete(mTank);
    delete mSocket;
}

void STGClient::procNewObject(GameObject* object)
{
    mNewObjectsLock.lock();
    packConstructorObjectsInfo(mNewObjectsPacket, object);
    mUnitsInNewoPacket++;
    mNewObjectsLock.unlock();
}

void STGClient::procDelObject(GameObject* object)
{
    mObjectsToDeleteLock.lock();
    mObjectsToDelete.push_back(object->getObjectNum());
    mObjectsToDeleteLock.unlock();
}

STGClient::STGClient(const GameLevel& gameLevel, sf::TcpSocket* socket)
    : mSocket(socket)
    , mCurrGameLevel(gameLevel)
{
    ServerGameObjectManager::getInstance().subscribeClient(this);

    mTank = GameObjectsFactory::newTank(static_cast<sf::Vector2f>(mCurrGameLevel.getSpawnPoint()));

    sf::Packet packet;
    packet << mCurrGameLevel;
    const auto& objects = ServerGameObjectManager::getInstance().getGameObjects();
    sf::Uint32 objectsInTheWorldCount = static_cast<sf::Uint32>(objects.size());
    packet << objectsInTheWorldCount;
    for (auto obj : objects)
    {
        packConstructorObjectsInfo(packet, obj);
        packChangingObjectsInfo(packet, obj);
    }

    sf::Int32 playerTankNum = mTank->getObjectNum();
    packet << playerTankNum;

    mSocket->send(packet);
}

bool STGClient::isDisconnected() const
{
    return mDisconnected;
}

Tank* STGClient::getPlayerTank() const
{
    return mTank;
}

void STGClient::applyEvents()
{
    if (mMousePressed)
        mTank->tryToLaunchMissile();
    if (mTank->getHP() <= 0)
        mTank->respawnAt(mCurrGameLevel.getSpawnPoint());
}

void STGClient::packConstructorObjectsInfo(sf::Packet& packet, GameObject* obj)
{
    sf::Uint16 objectType;
    if (dynamic_cast<Bonus*>(obj))
        objectType = NetworkUtils::ObjBonus;
    else if (dynamic_cast<Tank*>(obj))
        objectType = NetworkUtils::ObjTank;
    else if (dynamic_cast<MissileSniper*>(obj))
        objectType = NetworkUtils::ObjMissileSniper;
    else if (dynamic_cast<MissileSimpleBomb*>(obj))
        objectType = NetworkUtils::ObjMissileSimpleBomb;
    else if (dynamic_cast<Planet*>(obj))
        objectType = NetworkUtils::ObjPlanet;
    else if (dynamic_cast<StaticObject*>(obj))
        objectType = NetworkUtils::ObjStatic;
    else assert(false);

    packet << objectType << static_cast<sf::Uint32>(obj->getObjectNum());
    switch (objectType)
    {
    case NetworkUtils::ObjStatic:
        break;
    case NetworkUtils::ObjBonus:
    {
        auto bonus = dynamic_cast<Bonus*>(obj);
        sf::Uint16 bonusType = bonus->getBonusType();
        sf::Int32 value = bonus->getValue();
        sf::Vector2f pos = bonus->getPosition();
        packet << bonusType;
        packet << value;
        packet << pos;
    }
    break;
    case  NetworkUtils::ObjTank:
    {
        auto tank = dynamic_cast<Tank*>(obj);
        sf::Vector2f pos = tank->getPosition();
        packet << pos;
    }
    break;
    case  NetworkUtils::ObjMissileSniper:
    {
        auto sniperMissile = dynamic_cast<MissileSniper*>(obj);
        sf::Vector2f pos = sniperMissile->getPosition();
        float angle = sniperMissile->getAngle();
        b2Vec2 tankLinVel = sniperMissile->getTankLinVel();
        packet << pos;
        packet << angle;
        packet << tankLinVel;
    }
    break;
    case  NetworkUtils::ObjMissileSimpleBomb:
    {
        auto simpleBomb = dynamic_cast<MissileSimpleBomb*>(obj);
        sf::Vector2f pos = simpleBomb->getPosition();
        float angle = simpleBomb->getAngle();
        b2Vec2 tankLinVel = simpleBomb->getTankLinVel();
        packet << pos;
        packet << angle;
        packet << tankLinVel;
    }
    break;
    case  NetworkUtils::ObjPlanet:
    {
        auto planet = dynamic_cast<Planet*>(obj);
        sf::Vector2f pos = planet->getPosition();
        sf::Int32 radius = planet->getRadius();
        sf::Int32 power = planet->getPower();
        packet << pos;
        packet << radius;
        packet << power;
    }
    break;
    default:
        assert(false);
    }
}

void STGClient::packChangingObjectsInfo(sf::Packet& packet, GameObject* obj)
{
    sf::Int32 hp = obj->getHP();
    b2Vec2 pos = obj->mBody->GetPosition(), linVel = obj->mBody->GetLinearVelocity();
    float32 angle = obj->mBody->GetAngle(), angVel = obj->mBody->GetAngularVelocity();

    packet << pos;
    packet << linVel;
    packet << angle;
    packet << angVel;
    packet << hp;
}

void STGClient::transive()
{
    sf::Packet packet;

    //send:

    //existing objects:
    ServerGameObjectManager::getInstance().lockObjects();
    const auto& exisitingObjects = ServerGameObjectManager::getInstance().getGameObjects();
    sf::Uint32 objectsCount = static_cast<sf::Uint32>(exisitingObjects.size());
    packet << objectsCount;
    for (auto obj : exisitingObjects)
    {
        packet << static_cast<sf::Int32>(obj->getObjectNum());//todo: hp to client, rm useless params (velocity)
        packChangingObjectsInfo(packet, obj);
    }
    ServerGameObjectManager::getInstance().unlockObjects();

    //new objects:
    mNewObjectsLock.lock();
    packet << mUnitsInNewoPacket;
    packet.append(mNewObjectsPacket.getData(), mNewObjectsPacket.getDataSize());
    mNewObjectsPacket.clear();
    mUnitsInNewoPacket = 0;
    mNewObjectsLock.unlock();

    //objects to del:
    mObjectsToDeleteLock.lock();
    packet << mObjectsToDelete;
    mObjectsToDelete.clear();
    mObjectsToDeleteLock.unlock();

    //todo: players info:
    //tank id
    //nickname
    //score
    //barrel angle
    //barrel type

    mSocket->send(packet);

    //recieve:

    auto status = mSocket->receive(packet);
    if (status != sf::Socket::Status::Done)
    {
        mDisconnected = true;
        return;
    }

    float angle;
    packet >> angle >> mMousePressed;
    mTank->setBarrelAngle(angle);
}

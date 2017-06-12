#include "STGClient.h"
#include "Network/NetworkUtils.h"
#include "GameObjectsFactory.h"
#include "LogicalGameObjects/StaticObject.h"

using namespace NetworkUtils;
using namespace std::chrono_literals;

STGClient::~STGClient()
{
    ServerGameObjectManager::getInstance().forcedDelete(mTank);
    mSocket->disconnect();
    mCommThread->join();
    delete mCommThread;
    delete mSocket;
}

void STGClient::sendNewObjects(const std::vector<GameObject*> objects)
{
    mNewObjectsLock.lock();
    for (auto& x : objects)
        mNewObjects.push_back(x);
    mNewObjectsLock.unlock();
}

STGClient::STGClient(const GameLevel& gameLevel, sf::TcpSocket* socket)
    : mSocket(socket)
    , mCurrGameLevel(gameLevel)
{
    mTank = GameObjectsFactory::newTank(static_cast<sf::Vector2f>(mCurrGameLevel.getSpawnPoint()));

    sf::Packet packet;
    packet << mCurrGameLevel;
    const auto& objects = ServerGameObjectManager::getInstance().getGameObjects();
    sf::Uint32 objectsInTheWorldCount = objects.size();
    packet << objectsInTheWorldCount;
    for (auto obj : objects)
    {
        packConstructorObjectsInfo(packet, obj);
        packChangingObjectsInfo(packet, obj);
    }

    sf::Int32 playerTankNum = mTank->getObjectNum();
    packet << playerTankNum;

    mSocket->send(packet);
    mCommThread = new std::thread(&STGClient::clientComm, this);
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

void STGClient::clientComm()
{
    mSocket->setBlocking(false);

    sf::Packet packet;

    while (true)
    {
        packet.clear();

        mNewObjectsLock.lock();
        sf::Uint32 objectsCount = mNewObjects.size();
        packet << objectsCount;
        for (auto& x : mNewObjects)
            packConstructorObjectsInfo(packet, x);
        mNewObjects.clear();
        mNewObjectsLock.unlock();

        const auto& exisitingObjects = ServerGameObjectManager::getInstance().getGameObjects();//todo: sync mutex
        objectsCount = exisitingObjects.size();
        packet << objectsCount;
        for (auto obj : exisitingObjects)
        {
            packet << static_cast<sf::Int32>(obj->getObjectNum());
            packChangingObjectsInfo(packet, obj);
        }
        
        mSocket->send(packet);

        packet.clear();

        while (auto status = mSocket->receive(packet))// != sf::Socket::Status::Done
        {
            if (status != sf::Socket::Status::NotReady)
            {
                mDisconnected = true;
                return;
            }
            std::this_thread::sleep_for(10ms);
        }

        float angle;
        packet >> angle >> mMousePressed;
        mTank->setBarrelAngle(angle);
    }
}

#pragma once

#include <SFML/Network.hpp>
#include <mutex>
#include "GameLevel.h"

class Tank;
class GameObject;

class STGClient
{
public:
    STGClient(const GameLevel& gameLevel, sf::TcpSocket* socket);
    ~STGClient();

    void procNewObject(GameObject* object);
    void procDelObject(GameObject* object);
    
    void applyEvents();
    void transive(sf::Packet playersInfo);

    bool isDisconnected() const;
    const Tank* getPlayerTank() const;
    sf::Int32 getScore() const;
    const std::string& getNickname() const;
private:
    Tank* mTank = nullptr;
    std::string mNickname;
    sf::Int32 mScore = 0;
    sf::TcpSocket* mSocket;
    bool mMousePressed = false;
    bool mDisconnected = false;

    static void packConstructorObjectsInfo(sf::Packet& packet, GameObject* obj);
    static void packChangingObjectsInfo(sf::Packet& packet, GameObject* obj);

    std::mutex mNewObjectsLock, mObjectsToDeleteLock;
    sf::Uint32 mUnitsInNewoPacket = 0;
    sf::Packet mNewObjectsPacket;
    std::vector<sf::Uint32> mObjectsToDelete;

    const GameLevel& mCurrGameLevel;
};

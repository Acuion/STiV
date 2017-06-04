#pragma once
#include "ServerGameObjects/Tank/Missiles/MissileSimpleBombSrv.h"
#include "ServerGameObjects/Tank/Missiles/MissileSniperSrv.h"
#include "ServerGameObjects/Tank/TankSrv.h"
#include "ServerGameObjects/BonusSrv.h"
#include "ServerGameObjects/Gravity/PlanetSrv.h"
#include "LogicalGameObjects/ServerGameObjectManager.h"

class GameObjectsFactory
{
public:
    static MissileSimpleBomb* newMissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
    {
        return new MissileSimpleBombSrv(pos, angle, tankLinVel);
    }

    static MissileSniper* newMissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
    {
        return new MissileSniperSrv(pos, angle, tankLinVel);
    }

    static Tank* newTank(sf::Vector2f pos, bool client = false)
    {
        return new TankSrv(pos, client);
    }

    static Tank* newTank()
    {
        return new TankSrv();
    }

    static Bonus* newBonus(BonusType bt, int value, sf::Vector2f pos)
    {
        return new BonusSrv(bt, value, pos);
    }

    static Planet* newPlanet(sf::Vector2f pos, int radius, int power)
    {
        return new PlanetSrv(pos, radius, power);
    }

    static GameObjectManager& instanceOfGoM()
    {
        return ServerGameObjectManager::getInstance();
    }

private:
    GameObjectsFactory();
};

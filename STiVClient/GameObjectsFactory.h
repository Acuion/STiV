#pragma once
#include "VisualGameObjects/Tank/Missiles/MissileSimpleBombVis.h"
#include "visualGameObjects/Tank/Missiles/MissileSniperVis.h"
#include "VisualGameObjects/Tank/TankVis.h"
#include "VisualGameObjects/BonusVis.h"
#include "VisualGameObjects/Gravity/PlanetVis.h"
#include "LogicalGameObjects/ClientGameObjectManager.h"
#include "FontStorage.h"

class GameObjectsFactory
{
public:
    static MissileSimpleBomb* newMissileSimpleBomb(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
    {
        return new MissileSimpleBombVis(pos, angle, tankLinVel);
    }

    static MissileSniper* newMissileSniper(sf::Vector2f pos, float angle, b2Vec2 tankLinVel)
    {
        return new MissileSniperVis(pos, angle, tankLinVel);
    }

    static Tank* newTank(sf::Vector2f pos, const std::string nickname)
    {
        return new TankVis(pos, FontStorage::getFont("consolas"), nickname);
    }

    static Tank* newTank()
    {
        return new TankVis();
    }

    static Bonus* newBonus(BonusType bt, int value, sf::Vector2f pos)
    {
        return new BonusVis(bt, value, pos);
    }

    static Planet* newPlanet(sf::Vector2f pos, int radius, int power)
    {
        return new PlanetVis(pos, radius, power);
    }

    static GameObjectManager& instanceOfGoM()
    {
        return ClientGameObjectManager::getInstance();
    }

private:
    GameObjectsFactory();
};

#include <iostream>
#include "GameServer.h"

int main()
{
    std::ios_base::sync_with_stdio(false);
    srand(static_cast<unsigned int>(time(nullptr)));

    MaterialStorage::initMaterials();

    GameServer* STG = new GameServer();
    STG->loadLevel("test");
    STG->listen(58000);

    Timer updateTimer(10);

    while (true)
    {
        if (updateTimer.isExpired())
            STG->update(10);
    }

    return 0;
}
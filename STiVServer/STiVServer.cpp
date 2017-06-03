#include <iostream>
#include "STanksGame.h"

int main()
{
    std::ios_base::sync_with_stdio(0);
    srand(time(0));

    MaterialStorage::initMaterials();

    STanksGame* STG = new STanksGame();
    STG->loadLevel("test");
    STG->listen(58000);

    while (true)
    {
        STG->update(10);
    }

    return 0;
}
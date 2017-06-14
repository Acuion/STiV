#include "TankSrv.h"

TankSrv::TankSrv(sf::Vector2f pos, const std::string& nickname)
    : Tank(pos, nickname)
{
}

TankSrv::TankSrv()
    : Tank()
{
}

TankSrv::~TankSrv()
{
}

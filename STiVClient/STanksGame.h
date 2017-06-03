#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <chrono>
#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/GameObjectManager.h"
#include "LogicalGameObjects/Tank/Tank.h"

using namespace std::chrono_literals;

class STanksGame
{
public:
    STanksGame(sf::RenderWindow & wnd);

    ~STanksGame();

    bool STanksGame::connect(std::string srvIp, int srvPort);

    void update(int dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void setResolution(const sf::Vector2i& res);

    bool isReturningToMainMenu() const;
private:
    sf::RenderWindow & mWindow;
    sf::RenderTexture mScene, mSceneToWindow;
    sf::Sprite mSceneSprite, mSceneToWindowSprite;

    sf::Vector2i mScreenSize;
    sf::Vector2f mHalfScreen;
    sf::Vector2i mCurrLevelSize;
    sf::Vector2i mSpawnPoint;

    sf::Vector2i mMousePos;

    Tank *mPlayerTank = nullptr;
    sf::View mCenteredView;

    sf::TcpSocket mTcpClient;

    bool mIsReturningToMainMenu = false;

    Sprite mLevelBackground;
    sf::Text mHpText;
    sf::Font mConsolas;
};


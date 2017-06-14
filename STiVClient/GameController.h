#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <chrono>
#include "LogicalGameObjects/GameObject.h"
#include "LogicalGameObjects/GameObjectManager.h"
#include "LogicalGameObjects/Tank/Tank.h"
#include "GameLevel.h"

using namespace std::chrono_literals;

class GameController
{
public:
    explicit GameController(sf::RenderWindow & wnd);

    ~GameController();

    bool connect(const std::string& srvIp, int srvPort, const std::string& nickname);

    void update(int dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void setResolution(const sf::Vector2i& res);

    bool isReturningToMainMenu() const;
private:
    sf::RenderWindow & mWindow;
    sf::RenderTexture mScene, mSceneToWindow;
    sf::Sprite mSceneSprite, mSceneToWindowSprite;

    GameLevel mCurrGameLevel;
    sf::Vector2i mScreenSize;
    sf::Vector2f mHalfScreen;
    sf::Vector2i mMousePos;

    Tank *mPlayerTank = nullptr;
    sf::View mCenteredView;

    sf::TcpSocket mTcpClient;
    CallTimer mTransiveTimer;

    bool mIsReturningToMainMenu = false;

    Sprite mLevelBackground;
    sf::Text mHpText;
};


#include "GameController.h"
#include "LogicalGameObjects/Bonus.h"
#include "VisualGameObjects/Visualizer.h"
#include "GameObjectsFactory.h"
#include "Network/NetworkUtils.h"
#include "LogicalGameObjects/ClientGameObjectManager.h"

using namespace NetworkUtils;

GameController::GameController(sf::RenderWindow & wnd)
    : mWindow(wnd)
    , mTransiveTimer(50)
{
    mConsolas.loadFromFile("Content\\consola.ttf");
    mHpText.setFont(mConsolas);
    mHpText.setFillColor(sf::Color::Black);
    mHpText.setPosition(35, 35);
    mHpText.setCharacterSize(50);
}

GameController::~GameController()
{
}

bool GameController::connect(std::string srvIp, int srvPort)
{
    auto res = mTcpClient.connect(sf::IpAddress(srvIp.c_str()), srvPort, sf::seconds(5));
    if (res != sf::Socket::Done)
        return false;

    try
    {
        auto packet = readPacket(mTcpClient);
        packet >> mCurrGameLevel;
        ClientGameObjectManager::getInstance().reset(mCurrGameLevel.getCurrLevelSize().x, mCurrGameLevel.getCurrLevelSize().y);
        mPlayerTank = ClientGameObjectManager::getInstance().fillFromServerAndGetPlayerTank(packet);
    }
    catch (...)
    {
        return false;
    }

    mScene.create(mCurrGameLevel.getCurrLevelSize().x, mCurrGameLevel.getCurrLevelSize().y);
    mSceneSprite = sf::Sprite(mScene.getTexture());

    mCenteredView.setSize(static_cast<float>(mScreenSize.x), static_cast<float>(mScreenSize.y));

    mLevelBackground = Sprite("globalBackground.png", { mCurrGameLevel.getCurrLevelSize().x / 2.0f, mCurrGameLevel.getCurrLevelSize().y / 2.0f },
    { 1.0f * mCurrGameLevel.getCurrLevelSize().x, 1.0f * mCurrGameLevel.getCurrLevelSize().y });//TODO: load background from server

    return true;
}

void GameController::update(int dt)
{
    if (mTransiveTimer.isExpired())
    {
        try
        {
            ClientGameObjectManager::getInstance().updateFromServer(mTcpClient);

            sf::Packet packet;
            packet << mPlayerTank->getBarrelAngle() << mPlayerTank->wantToLaunchMissile();
            mTcpClient.send(packet);
        }
        catch (...)
        {
            mIsReturningToMainMenu = true;
            return;
        }
    }

    mPlayerTank->updateMousePos(mSceneToWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow)));
    mPlayerTank->update();
    ClientGameObjectManager::getInstance().update(dt);

    mCenteredView.setCenter({ std::min(std::max(mPlayerTank->getPosition().x, mHalfScreen.x), mCurrGameLevel.getCurrLevelSize().x - mHalfScreen.x),
        std::min(std::max(mPlayerTank->getPosition().y, mHalfScreen.y), mCurrGameLevel.getCurrLevelSize().y - mHalfScreen.y) });
    mSceneToWindow.setView(mCenteredView);
    mHpText.setString(std::to_string(mPlayerTank->getHP()));
}

void GameController::draw()
{
    sf::FloatRect viewRect = sf::FloatRect(mCenteredView.getCenter().x - mCenteredView.getSize().x / 2 - 100,
        mCenteredView.getCenter().y - mCenteredView.getSize().y / 2 - 100,
        mCenteredView.getSize().x + 200,
        mCenteredView.getSize().y + 200);

    mScene.clear(sf::Color::Green);
    mLevelBackground.draw(mScene, viewRect);
    Visualizer::draw(mScene, viewRect);
    mScene.display();
    PostProcessingManager::postprocess(mScene);
    mSceneToWindow.draw(mSceneSprite);
    mSceneToWindow.display();
    mWindow.draw(mSceneToWindowSprite);

    mWindow.draw(mHpText); //TODO: UI
}

void GameController::setResolution(const sf::Vector2i& res)
{
    mScreenSize = res;
    mCenteredView.setSize(static_cast<sf::Vector2f>(res));
    mSceneToWindow.create(res.x, res.y);
    mSceneToWindowSprite.setTexture(mSceneToWindow.getTexture());
    mHalfScreen = { res.x / 2.0f,res.y / 2.0f };
}

bool GameController::isReturningToMainMenu() const
{
    return mIsReturningToMainMenu;
}

void GameController::handleEvent(const sf::Event & event)
{
    switch (event.type)
    {
    case sf::Event::MouseMoved:
        mMousePos = sf::Mouse::getPosition(mWindow);
        break;
    }
    mPlayerTank->handleEvents(event);
}

#include "STanksGame.h"
#include "LogicalGameObjects/Bonus.h"
#include "VisualGameObjects/Visualizer.h"
#include "GameObjectsFactory.h"
#include "Network/NetworkUtils.h"
#include "LogicalGameObjects/ClientGameObjectManager.h"

STanksGame::STanksGame(sf::RenderWindow & wnd)
	: mWindow(wnd)
{
	mConsolas.loadFromFile("Content\\consola.ttf");
	mHpText.setFont(mConsolas);
	mHpText.setFillColor(sf::Color::Black);
	mHpText.setPosition(35, 35);
	mHpText.setCharacterSize(50);
}

STanksGame::~STanksGame()
{
}

bool STanksGame::connect(std::string srvIp, int srvPort)
{
	auto res = mTcpClient.connect(sf::IpAddress(srvIp.c_str()), srvPort, sf::seconds(5));
	if (res != sf::Socket::Done)
		return false;

	try
	{
		NetworkUtils::receiveDisconnectCheck(mTcpClient, mCurrLevelSize);
		mPlayerTank = ClientGameObjectManager::getInstance().fillFromServerAndGetPlayerTank(mTcpClient);
	}
	catch(...)
	{
		return false;
	}

	mScene.create(mCurrLevelSize.x, mCurrLevelSize.y);
	mSceneSprite = sf::Sprite(mScene.getTexture());

	mCenteredView.setSize(static_cast<float>(mScreenSize.x), static_cast<float>(mScreenSize.y));

	mPlayerTank = GameObjectsFactory::newTank();

	mLevelBackground = Sprite("globalBackground.png", { mCurrLevelSize.x / 2.0f, mCurrLevelSize.y / 2.0f },
	{ 1.0f * mCurrLevelSize.x, 1.0f * mCurrLevelSize.y });//TODO: load background from server

	return true;
}

void STanksGame::update(int dt)
{
	try
	{
		ClientGameObjectManager::getInstance().updateFromServer(mTcpClient);
	}
	catch(...)
	{
		mIsReturningToMainMenu = true;
		return;
	}

	ClientGameObjectManager::getInstance().update(dt);

	try
	{
		NetworkUtils::sendDisconenctCheck(mTcpClient, mPlayerTank->getBarrelAngle());
		NetworkUtils::sendDisconenctCheck(mTcpClient, mPlayerTank->wantToLaunchMissile());
	}
	catch(...)
	{
		mIsReturningToMainMenu = true;
		return;
	}

	mCenteredView.setCenter({ std::min(std::max(mPlayerTank->getPosition().x, mHalfScreen.x), mCurrLevelSize.x - mHalfScreen.x),
		std::min(std::max(mPlayerTank->getPosition().y, mHalfScreen.y), mCurrLevelSize.y - mHalfScreen.y) });
	mSceneToWindow.setView(mCenteredView);
	mHpText.setString(std::to_string(mPlayerTank->getHP()));
}

void STanksGame::draw()
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

void STanksGame::setResolution(const sf::Vector2i& res)
{
	mScreenSize = res;
	mCenteredView.setSize(static_cast<sf::Vector2f>(res));
	mSceneToWindow.create(res.x, res.y);
	mSceneToWindowSprite.setTexture(mSceneToWindow.getTexture());
	mHalfScreen = { res.x / 2.0f,res.y / 2.0f };
}

bool STanksGame::isReturningToMainMenu() const
{
	return mIsReturningToMainMenu;
}

void STanksGame::handleEvent(const sf::Event & event)
{
	switch (event.type)
	{
	case sf::Event::MouseMoved:
		mMousePos = sf::Mouse::getPosition(mWindow);
		break;
	}
	mPlayerTank->handleEvents(event);
}

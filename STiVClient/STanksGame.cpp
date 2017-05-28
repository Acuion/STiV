#include "STanksGame.h"
#include "LogicalGameObjects/Bonus.h"
#include <fstream>
#include "VisualGameObjects/Visualizer.h"
#include "GameObjectsFactory.h"

STanksGame::STanksGame(sf::RenderWindow & wnd)
	: mWindow(wnd)
	//TODO: load background from server
{
	mConsolas.loadFromFile("Content\\consola.ttf");
	mHpText.setFont(mConsolas);
	mHpText.setFillColor(sf::Color::Black);
	mHpText.setPosition(35, 35);
	mHpText.setCharacterSize(50);
}

STanksGame::~STanksGame()
{
	delete mLevelBackground;
}

bool STanksGame::connect(std::string srvIp, int srvPort)
{
	auto res = mTcpClient.connect(sf::IpAddress(srvIp.c_str()), srvPort, sf::seconds(5));
	if (res != sf::Socket::Done)
		return false;

	size_t rec;
	unsigned char* data = new unsigned char[4];
	mTcpClient.receive(data, 4, rec);
	int ptr = 0;
	mCurrLevelSize.x = tgMath::read2Bytes(data, ptr);
	mCurrLevelSize.y = tgMath::read2Bytes(data, ptr);
	mScene.create(mCurrLevelSize.x, mCurrLevelSize.y);
	mSceneSprite = sf::Sprite(mScene.getTexture());
	delete[] data;

	mCenteredView.setSize(mScreenSize.x, mScreenSize.y);

	mPlayerTank = GameObjectsFactory::newTank();

	mLevelBackground = Sprite("globalBackground.png", { mCurrLevelSize.x / 2.0f, mCurrLevelSize.y / 2.0f },
		{ (float)mCurrLevelSize.x, (float)mCurrLevelSize.y });

	return true;
}

void STanksGame::update(int dt)
{
	unsigned char* data = new unsigned char[2];
	unsigned int size = 0;
	mTcpClient.receive(data, 2, size);//количество байт в состоянии мира
	int ptr = 0;
	size = tgMath::read2Bytes(data, ptr);
	delete[] data;
	data = new unsigned char[size];
	if (mTcpClient.receive(data, size, size) == sf::Socket::Disconnected)//состояние мира
	{
		mIsReturningToMainMenu = true;
		delete[] data;
		return;
	}
	Visualizer::unpack(data, size);

	mTcpClient.receive(data, Visualizer::mSizeofSpecificClientData, size);//x y hp

	ptr = 0;
	int angle = tgMath::radToDeg(tgMath::atan2Points(mPlayerTank->getPosition(), mSceneToWindow.mapPixelToCoords(mMousePos))) + 600;
	tgMath::write2Bytes(angle, data, ptr);
	data[ptr++] = mPlayerTank->wantToLaunchMissile();
	mTcpClient.send(data, 3, size);//угол нажатость

	delete[] data;

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
	mCenteredView.setSize((sf::Vector2f)res);
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

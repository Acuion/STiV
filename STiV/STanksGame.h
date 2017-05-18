// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>
#include "MaterialStorage.h"
#include "UIButton.h"
#include "GameObject.h"
#include "GravityObject.h"
#include "GameObjectManager.h"
#include "Tank.h"
#include "Bonus.h"
#include "STGClient.h"
#include "PostProcessingManager.h"

using namespace std::chrono_literals;

class STanksGame
{
	sf::RenderWindow & mWindow;
    sf::RenderTexture mScene, mSceneToWindow;
    sf::Sprite mSceneSprite, mSceneToWindowSprite;

	sf::Vector2i mScreenSize;
	sf::Vector2f mHalfScreen;
	sf::Vector2i mCurrLevelSize;
	sf::Vector2i mSpawnPoint;

	bool mPaused = false;

	sf::Vector2i mMousePos;

	Timer mSpawnBonus;
	std::vector<sf::Vector2f> mBonusSpawnPoints;

	Tank *mPlayerTank = nullptr;
	sf::View mCenteredView;

	sf::TcpListener mTcpServer;
	std::thread* mAccThread = nullptr, *mLsnThread = nullptr;
	std::list<STGClient> mClients;
	std::mutex mClientsWork;
	sf::SocketSelector mSocketSelector;
	Timer mSendTimer;

	sf::TcpSocket mTcpClient;

	bool mIsServer = true;
	bool mIsWorking = true;
	bool mIsReturningToMainMenu = false;

    Sprite mLevelBackground;
	sf::Text mHpText;
	sf::Font mConsolas;
public:
	STanksGame(sf::RenderWindow & wnd);

	~STanksGame();

	void acceptClients();
	void listenClients();

	bool setNetworkMode(bool server);

	void update(int dt);
	void draw();
	void handleEvent(const sf::Event& event);

	void setResolution(const sf::Vector2i& res);

	void loadLevel(std::string name);

	bool isReturningToMainMenu();
};


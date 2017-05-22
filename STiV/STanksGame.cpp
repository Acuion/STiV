#include "STanksGame.h"

STanksGame::STanksGame(sf::RenderWindow & wnd)
    : mWindow(wnd)
    , mSpawnBonus(3000)
    , mSendTimer(30)
{
    mConsolas.loadFromFile("Content\\consola.ttf");
    mHpText.setFont(mConsolas);
    mHpText.setFillColor(sf::Color::Black);
    mHpText.setPosition(35, 35);
    mHpText.setCharacterSize(50);
}

STanksGame::~STanksGame()
{
    mIsWorking = false;
    if (mIsServer)
    {
        if (mAccThread)
            mAccThread->join();
        if (mLsnThread)
            mLsnThread->join();
    }
}

void STanksGame::acceptClients()
{
    while (mIsWorking)
    {
        sf::TcpSocket* curSock = new sf::TcpSocket();
        while (mTcpServer.accept(*curSock) != sf::Socket::Status::Done)
        {

            std::this_thread::sleep_for(100ms);
            if (!mIsWorking)
                return;
        }
        //hello!
        mClientsWork.lock();
        mSocketSelector.add(*curSock);
        mClients.emplace_back(mSpawnPoint, curSock, mCurrLevelSize);
        std::cout << "New client!\n";
        mClientsWork.unlock();
    }
}

void STanksGame::listenClients()
{
    bool wasDeleted;
    while (mIsWorking)
    {
        if (!mClients.size())
            std::this_thread::sleep_for(100ms);
        mSocketSelector.wait(sf::seconds(1));
        for (auto iter = mClients.begin(); iter != mClients.end();)
        {
            wasDeleted = false;
            STGClient& client = *iter;
            if (client.isReadyToUpdate(mSocketSelector))
            {
                mClientsWork.lock();
                client.updateFromNetwork();
                if (client.isDisconnected())
                {
                    wasDeleted = true;
                    client.unselectSocket(mSocketSelector);
                    iter = mClients.erase(iter);
                }
                mClientsWork.unlock();
            }
            if (!wasDeleted)
                ++iter;
        }
    }
}

bool STanksGame::setNetworkMode(bool server)
{
    std::string srvIp;
    int srvPort;

    std::ifstream f("serverIp.txt");
    f >> srvIp >> srvPort;

    if (server)
    {
        if (mTcpServer.listen(srvPort) != sf::Socket::Done)
            return false;
        mTcpServer.setBlocking(false);
        mAccThread = new std::thread(&STanksGame::acceptClients, this);
        mLsnThread = new std::thread(&STanksGame::listenClients, this);
    }
    else
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

        mPlayerTank = new Tank();
    }
    mIsServer = server;
    return true;
}

void STanksGame::update(int dt)
{
    if (mPaused)
        return;
    if (mIsServer)
    {
        if (mSpawnBonus.isExpired())
        {
            sf::Vector2f bonusPos = mBonusSpawnPoints[rand() % mBonusSpawnPoints.size()];
            int rnd = rand();
            if (rnd % 2)
                new Bonus(BonusType::bt_heal, 10, bonusPos);
            else
            {
                switch (rnd % 3)
                {
                case 0:
                    new Bonus(BonusType::bt_weapon, MissleType::sniper, bonusPos);
                    break;
                case 1:
                    new Bonus(BonusType::bt_weapon, MissleType::simpleBomb, bonusPos);
                    break;
                }
            }
        }
        mPlayerTank->updateMousePos(mSceneToWindow.mapPixelToCoords(mMousePos));
        mClientsWork.lock();
        for (auto& client : mClients)
            client.applyEvents();

        if (mPlayerTank->getHP() <= 0)
            mPlayerTank->respawnAt(mSpawnPoint);

        for (auto& client : mClients)
            client.checkHP(mSpawnPoint);

        GameObjectManager::update(dt);

        if (mSendTimer.isExpired())
        {
            int ptr = 2;
            unsigned char* data = GameObjectManager::pack(ptr);
            int pt0 = 0;
            tgMath::write2Bytes(ptr - 2, data, pt0);
            for (auto& client : mClients)
                client.sendWorld(data, ptr);
            delete[] data;
        }
        mClientsWork.unlock();
    }
    else
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
        GameObjectManager::unpack(data, size);

        mTcpClient.receive(data, GameObjectManager::mSizeofSpecificClientData, size);//x y hp
        mPlayerTank->updateFromNetwork(data);

        ptr = 0;
        int angle = tgMath::radToDeg(tgMath::atan2Points(mPlayerTank->getPosition(), mSceneToWindow.mapPixelToCoords(mMousePos))) + 600;
        tgMath::write2Bytes(angle, data, ptr);
        data[ptr++] = mPlayerTank->wantToLaunchMissile();
        mTcpClient.send(data, 3, size);//угол нажатость

        delete[] data;
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
    GameObjectManager::draw(mScene, viewRect);
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

void STanksGame::loadLevel(std::string name)
{
    std::ifstream levelfile("levels\\" + name + ".tgl");
    int goc;
    int x, y, radius, power;
    levelfile >> x >> y;
    mLevelBackground = Sprite("globalBackground.png", { x / 2.0f, y / 2.0f }, { (float)x, (float)y });
    mScene.create(x, y);
    mSceneSprite = sf::Sprite(mScene.getTexture());
    GameObjectManager::reset(x, y);
    mCurrLevelSize.x = x;
    mCurrLevelSize.y = y;
    levelfile >> x >> y;
    mSpawnPoint.x = x;
    mSpawnPoint.y = y;
    mPlayerTank = new Tank({ (float)x, (float)y });
    mCenteredView.setSize(mScreenSize.x, mScreenSize.y);
    mCenteredView.setCenter(x, y);

    levelfile >> goc;
    for (int i = 0; i < goc; ++i)
    {
        levelfile >> x >> y >> radius >> power;

        auto planetbdef = new b2BodyDef();
        planetbdef->type = b2_staticBody;
        planetbdef->position.Set(x * tgMath::b2scale, y * tgMath::b2scale);
        auto planetfdef = new b2FixtureDef();
        b2CircleShape* planetcs = new b2CircleShape();
        planetcs->m_radius = radius * tgMath::b2scale;
        planetfdef->shape = planetcs;

        new GravityObject(Sprite("planet.png", { (float)x, (float)y }, radius / 200.0f), planetbdef, planetfdef, power);
    }

    levelfile >> goc;
    mBonusSpawnPoints.clear();
    for (int i = 0; i < goc; ++i)
    {
        levelfile >> x >> y;
        mBonusSpawnPoints.push_back({ (float)x,(float)y });
    }
}

bool STanksGame::isReturningToMainMenu()
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
    case sf::Event::LostFocus://Окно теряет фокус
                              //paused = true;
        break;
    case sf::Event::GainedFocus:
        //paused = false;
        break;
    }
    mPlayerTank->handleEvents(event);
}

#include <iostream>
#include "STanksGame.h"
#include "Graphics/Shaders/PostProcessingManager.h"
#include "UI/UIButton.h"

namespace Game
{
    sf::RenderWindow window;
    bool fullscreen = false;
    sf::Vector2i currRes = { 1280, 700 }, srcRes = { 1280, 700 };
    sf::Vector2f mousePos;

    Sprite menuBackground;

    enum GameState
    {
        game,
        mainMenu,
        mainMenuSelectMode
    } currentGameState;

    STanksGame* STG = nullptr;

    //
    Sprite menu_Logo, menu_MissileI, menu_leftBarrel, menu_rightBarrel;
    UIButton menu_newGame, menu_toggleFullscreen, menu_exit;
    UIButton menu_asClient, menu_Back;
    //

    void createMenu()
    {
        menuBackground = Sprite("globalBackground.png", { currRes.x / 2.0f, currRes.y / 2.0f }, { 5000, 5000 });
        menu_Logo = Sprite("menu\\STIVLogo.png", { currRes.x / 2.0f, 200 });
        menu_MissileI = Sprite("missiles\\simpleBombMissile.png", { currRes.x / 2.0f + 61, 60 }, 1, { Sprite::Animation(3, 2) }, { 20, 20 });
        menu_leftBarrel = Sprite("missiles\\simpleBombBarrel.png", { currRes.x / 2.0f - 197, 186 }, 2, { Sprite::Animation(3, 2), Sprite::Animation(3, 30) }, { 70, 40 }, -160);
        menu_leftBarrel.setOrigin({ 0, 20 });
        menu_rightBarrel = Sprite("missiles\\sniperBarrel.png", { currRes.x / 2.0f + 192, 186 }, 2, { Sprite::Animation(3, 15), Sprite::Animation(6, 23) }, { 100, 40 }, 50);
        menu_rightBarrel.setOrigin({ 0, 20 });

        menu_newGame = UIButton(sf::IntRect(currRes.x / 2.0f, 400, 210, 35), Sprite("menu\\menu_newGame.png", { currRes.x / 2.0f, 400 }, 3, { Sprite::Animation(1, 1), Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 500, 100 }));
        menu_toggleFullscreen = UIButton(sf::IntRect(currRes.x / 2.0f, 500, 365, 35), Sprite("menu\\menu_toggleFullscreen.png", { currRes.x / 2.0f, 500 }, 3, { Sprite::Animation(1, 1), Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 730, 100 }));
        menu_exit = UIButton(sf::IntRect(currRes.x / 2.0f, 600, 110, 35), Sprite("menu\\menu_exit.png", { currRes.x / 2.0f, 600 }, 3, { Sprite::Animation(1, 1), Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 220, 100 }));

        menu_asClient = UIButton(sf::IntRect(currRes.x / 2.0f, 500, 300, 35), Sprite("menu\\menu_asClient.png", { currRes.x / 2.0f, 500 }, 3, { Sprite::Animation(1, 1), Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 600, 100 }));
        menu_Back = UIButton(sf::IntRect(currRes.x / 2.0f, 600, 150, 35), Sprite("menu\\menu_back.png", { currRes.x / 2.0f, 600 }, 3, { Sprite::Animation(1, 1), Sprite::Animation(1, 1), Sprite::Animation(1, 1) }, { 300, 100 }));
    }

    void setResolution(const sf::Vector2i& res)
    {
        window.setSize((sf::Vector2u)res);

        menuBackground.setPosition({ currRes.x / 2.0f, currRes.y / 2.0f });
        menu_Logo.setPosition({ currRes.x / 2.0f, 200 });
        menu_MissileI.setPosition({ currRes.x / 2.0f + 61, 60 });
        menu_leftBarrel.setPosition({ currRes.x / 2.0f - 197, 186 });
        menu_rightBarrel.setPosition({ currRes.x / 2.0f + 192, 186 });
        menu_newGame.setPosition({ currRes.x / 2.0f, 400 });
        menu_toggleFullscreen.setPosition({ currRes.x / 2.0f, 500 });
        menu_exit.setPosition({ currRes.x / 2.0f, 600 });
        menu_asClient.setPosition({ currRes.x / 2.0f, 500 });
        menu_Back.setPosition({ currRes.x / 2.0f, 600 });
    }

    void toggleFullscreen()
    {
        fullscreen = !fullscreen;
        if (!fullscreen)
        {
            window.create(sf::VideoMode(srcRes.x, srcRes.y), "STiV!", sf::Style::Close | sf::Style::Titlebar);
            currRes = srcRes;
        }
        else
        {
            window.create(sf::VideoMode::getDesktopMode(), "STiV!", sf::Style::Fullscreen);
            currRes = { (int)sf::VideoMode::getDesktopMode().width, (int)sf::VideoMode::getDesktopMode().height };
        }
        window.setVerticalSyncEnabled(true);
        window.setMouseCursorGrabbed(true);
        setResolution(currRes);
    }

    //Загрузка контента, который должен присутствовать на момент первой отрсовки окна
    void preloadContent()
    {
        srand(time(0));
        MaterialStorage::initMaterials();
        PostProcessingManager::init();

        createMenu();

        currentGameState = GameState::mainMenu;
    }

    //Вызывается каждое событие
    void handleEvent(const sf::Event& event)
    {
        switch (event.type)
        {
        case sf::Event::MouseMoved:
            mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
            break;
        case sf::Event::Closed:
            window.close();
            break;
        }

        switch (currentGameState)
        {
        case GameState::game:
            STG->handleEvent(event);
            break;
        case GameState::mainMenu:
            if (menu_newGame.wasPressed(event, mousePos))
                currentGameState = GameState::mainMenuSelectMode;
            if (menu_toggleFullscreen.wasPressed(event, mousePos))
            {
                toggleFullscreen();
                break;
            }
            if (menu_exit.wasPressed(event, mousePos))
                window.close();
            break;
        case GameState::mainMenuSelectMode:
            if (menu_asClient.wasPressed(event, mousePos))
            {
                STG = new STanksGame(window);
                if (!STG->connect("127.0.0.1", 58000))
                {
                    delete STG;
                    break;
                }
                STG->setResolution(currRes);
                currentGameState = GameState::game;
            }
            if (menu_Back.wasPressed(event, mousePos))
                currentGameState = GameState::mainMenu;
            break;
        }
    }

    //Обновление типа анимаций
    void update(int dt)
    {
        switch (currentGameState)
        {
        case GameState::game:
            STG->update(dt);
            if (STG->isReturningToMainMenu())
            {
                currentGameState = GameState::mainMenu;
                delete STG;
            }
            break;
        case GameState::mainMenu:
            menu_leftBarrel.setRotation(Utilites::radToDeg(Utilites::atan2Points(menu_leftBarrel.getPosition(), mousePos)));
            menu_rightBarrel.setRotation(Utilites::radToDeg(Utilites::atan2Points(menu_rightBarrel.getPosition(), mousePos)));
            menu_Logo.update();
            menu_MissileI.update();
            menu_leftBarrel.update();
            menu_rightBarrel.update();

            menu_newGame.update();
            menu_toggleFullscreen.update();
            menu_exit.update();
            break;
        case GameState::mainMenuSelectMode:
            menu_leftBarrel.setRotation(Utilites::radToDeg(Utilites::atan2Points(menu_leftBarrel.getPosition(), mousePos)));
            menu_rightBarrel.setRotation(Utilites::radToDeg(Utilites::atan2Points(menu_rightBarrel.getPosition(), mousePos)));
            menu_Logo.update();
            menu_MissileI.update();
            menu_leftBarrel.update();
            menu_rightBarrel.update();

            menu_asClient.update();
            menu_Back.update();
            break;
        }
    }

    //Отрисовка
    void draw()
    {
        window.clear(sf::Color::Green);
        switch (currentGameState)
        {
        case GameState::game:
            STG->draw();
            break;
        case GameState::mainMenu:
            menuBackground.draw(window);

            menu_leftBarrel.draw(window);
            menu_rightBarrel.draw(window);
            menu_Logo.draw(window);
            menu_MissileI.draw(window);

            menu_newGame.draw(window);
            menu_toggleFullscreen.draw(window);
            menu_exit.draw(window);
            break;
        case GameState::mainMenuSelectMode:
            menuBackground.draw(window);

            menu_leftBarrel.draw(window);
            menu_rightBarrel.draw(window);
            menu_Logo.draw(window);
            menu_MissileI.draw(window);

            menu_asClient.draw(window);
            menu_Back.draw(window);
            break;
        }

        window.display();
    }
}

int main()
{
    std::ios_base::sync_with_stdio(0);

    Game::preloadContent();

    Game::window.create(sf::VideoMode(Game::srcRes.x, Game::srcRes.y), "STiV!", sf::Style::Close | sf::Style::Titlebar);
    Game::window.setVerticalSyncEnabled(true);
    Game::window.setMouseCursorGrabbed(true);

	Timer updateTimer(10);

    while (Game::window.isOpen())
    {
        sf::Event event;
        while (Game::window.pollEvent(event))
            Game::handleEvent(event);
		if (updateTimer.isExpired())
			Game::update(10);
        Game::draw();
    }

    return 0;
}
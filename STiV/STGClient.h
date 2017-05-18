// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#pragma once

#include "Tank.h"
#include <SFML/Network.hpp>

class STGClient
{
	Tank* mTank = nullptr;
	sf::TcpSocket* mSocket = nullptr;
	bool mMousePressed = false;
	bool mDisconnected = false;
public:
	STGClient(sf::Vector2i spawnPoint, sf::TcpSocket* socket, sf::Vector2i levelSize);
	~STGClient();

	void unselectSocket(sf::SocketSelector& selector);

	void sendWorld(unsigned char* world, int size);
	bool isReadyToUpdate(sf::SocketSelector& selector);
	void updateFromNetwork();
	bool isDisconnected();
	void applyEvents();
	void checkHP(const sf::Vector2i& spawnPoint);
};
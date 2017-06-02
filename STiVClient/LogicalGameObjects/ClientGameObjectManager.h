#pragma once

#include "LogicalGameObjects/GameObjectManager.h"
#include <SFML/Network/TcpSocket.hpp>

class ClientGameObjectManager : public GameObjectManager
{
public:
	static ClientGameObjectManager& getInstance();

	void updateFromServer(sf::TcpSocket& socket);
private:
	ClientGameObjectManager();
	ClientGameObjectManager operator=(const ClientGameObjectManager&) = delete;

	std::map<int, GameObject*> mObjectsIndex;
};

#include "ConnectionManager.h"

ConnectionManager* ConnectionManager::instance = nullptr;

int ConnectionManager::addConnection(httplib::ws::WebSocket* ws)
{
    std::lock_guard lock(mx);

    connections[nextPlayerId] = ws;
    std::cout << "[Game Manager] Connection created (player id=" << nextPlayerId << ")" << std::endl;

    return nextPlayerId++;
}

void ConnectionManager::deleteConnection(uint32_t playerId)
{
    std::lock_guard lock (mx);

    if (connections.contains(playerId)) {
        std::cout << "[Game Manager] Connection deleted (player id=" << playerId << ")" << std::endl;
        connections.erase(playerId);
    }
}

void ConnectionManager::sendMessage(uint32_t playerId, const std::string &msg)
{
    std::lock_guard lock(mx);

    if (!connections.contains(playerId) && connections[playerId]) {
        connections[playerId]->send(msg);
        std::cout << "[Game Manager] Send message: " << msg << " to player id=" << playerId << std::endl;
    }
}

#include "ConnectionManager.h"

ConnectionManager* ConnectionManager::instance = nullptr;

int ConnectionManager::addConnection(httplib::ws::WebSocket* ws)
{
    std::lock_guard lock(mx);

    connections[nextPlayerId] = ws;
    std::cout << "[Connection Manager] Connection created (player id=" << nextPlayerId << ")" << std::endl;

    return nextPlayerId++;
}

void ConnectionManager::deleteConnection(uint32_t playerId)
{
    std::lock_guard lock (mx);

    if (connections.contains(playerId)) {
        std::cout << "[Connection Manager] Connection deleted (player id=" << playerId << ")" << std::endl;
        connections.erase(playerId);
    }
}

void ConnectionManager::sendMessage(uint32_t playerId, const std::string &msg)
{
    std::lock_guard lock(mx);

    if (connections.contains(playerId) && connections[playerId]) {
        connections[playerId]->send(msg);
        std::cout << "[Connection Manager] Send message: " << msg << " to player id=" << playerId << std::endl;
    }
}

void ConnectionManager::addUserByPlayer(uint32_t playerId, uint32_t userId)
{
    std::lock_guard lock(mx);

    userIdByPlayerId[playerId] = userId;
}

std::optional<uint32_t> ConnectionManager::getUserByPlayer(uint32_t playerId) const
{
    std::lock_guard lock(mx);
    
    auto it = userIdByPlayerId.find(playerId);
    if (it == userIdByPlayerId.end()) {
        std::cout << "[Connection Manager] No user for player id=" << playerId << std::endl;
        return std::nullopt;
    }

    return it->second;
}

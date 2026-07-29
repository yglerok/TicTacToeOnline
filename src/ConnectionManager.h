#pragma once

#include "httplib/httplib.h"

#include <unordered_map>
#include <mutex>

class ConnectionManager {
public:
    static ConnectionManager* getInstance() {
        if (instance == nullptr)
            instance = new ConnectionManager();
        return instance;
    }

    int addConnection(httplib::ws::WebSocket* ws);
    void deleteConnection(uint32_t playerId);
    void sendMessage(uint32_t playerId, const std::string& msg);
    // uint32_t getGameByConnection(httplib::ws::WebSocket* ws) {
    //     if (gameByWs.contains(ws)) return gameByWs[ws];
    // };
    // httplib::ws::WebSocket* getConnectionByGame(uint32_t gameId) {
    //     if (wsByGame.contains(gameId)) return wsByGame[gameId];
    // };

    //std::unordered_map<httplib::ws::WebSocket*, uint32_t> getAllGamesByConnections() const {return gameByWs;}
    //std::unordered_map<uint32_t, httplib::ws::WebSocket*> getAllConnectionsByGame() const {return wsByGame;}

private:
    static ConnectionManager* instance;
    std::mutex mx;
    uint32_t nextPlayerId = 1;

    ConnectionManager() = default;
    ~ConnectionManager() = default;
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    std::unordered_map<uint32_t, httplib::ws::WebSocket*> connections;
    //std::unordered_map<uint32_t, httplib::ws::WebSocket*> wsByGame;
};
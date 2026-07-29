#pragma once

#include "Game.h"

#include <unordered_map>
#include <memory>
#include <optional>
#include <mutex>

class GameManager
{
public:
    static GameManager* getInstance() {
        if (instance == nullptr)
            instance = new GameManager();
        return instance;
    }

    uint32_t createNewGame(uint32_t playerId);
    void joinGame(uint32_t gameId, uint32_t playerId);
    void disconnectPlayer(uint32_t gameId, uint32_t playerId);
    int findWaitingGame(); // If no waiting games, return 0
    void deleteFinishedGame(uint32_t id);
    
    std::shared_ptr<Game> getGame(uint32_t id) {
        return ( (games.contains(id)) ? games[id] : nullptr );
    }
    int getGameByPlayer(uint32_t playerId) {
        return ( (gameByPlayer.contains(playerId)) ? gameByPlayer[playerId] : 0 );
    }
    std::unordered_map<uint32_t, std::shared_ptr<Game>> getAllGames() const {
        return games;
    }


private:
    static GameManager* instance;
    uint32_t nextGameId = 1;

    std::mutex mx;

    GameManager() = default;
    ~GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    std::unordered_map<uint32_t, std::shared_ptr<Game>> games;
    std::unordered_map<uint32_t, uint32_t> gameByPlayer;
};
#pragma once

#include "Game.h"

#include <unordered_map>
#include <memory>
#include <optional>

class GameManager
{
public:
    static GameManager* getInstance() {
        if (instance == nullptr)
            instance = new GameManager();
        return instance;
    }

    void createNewGame(uint32_t firstPlayerId, uint32_t secondPlayerId);
    void daleteFinishedGame(uint32_t id);
    std::optional<std::shared_ptr<Game>> getGame(uint32_t id) {
        return ( (games.contains(id)) ? games[id] : nullptr );
    }
    std::unordered_map<uint32_t, std::shared_ptr<Game>> getAllGames() const {
        return games;
    }


private:
    static GameManager* instance;
    static uint32_t id;

    GameManager() = default;
    ~GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    std::unordered_map<uint32_t, std::shared_ptr<Game>> games;
};
#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

uint32_t GameManager::createNewGame(uint32_t playerId)
{
    std::lock_guard lock(mx);

    std::shared_ptr<Game> game = std::make_shared<Game>(nextGameId++, playerId);
    // std::cout << "Created new game id = " << id << std::endl;
    // games[id++] = game;
    std::cout << "[Game Manager] Created new game (id = " << game->getId() << ") by player (id = " << playerId << ")" << std::endl;
    games[game->getId()] = game;
    gameByPlayer[playerId] = game->getId();

    return game->getId();
}

void GameManager::joinGame(uint32_t gameId, uint32_t playerId)
{
    std::lock_guard lock(mx);

    if (!getGame(gameId)) return;

    if (!games[gameId]->addPlayer(playerId)) {
        int a = (gameByPlayer.contains(playerId)) ? gameByPlayer[playerId] : -1;
        std::cout << "[Game Manager] Cannot join the game (id=" << gameId << "), player (id=" << playerId << ") is already in game (id=" << a << ")" << std::endl;
    }
    gameByPlayer[playerId] = gameId;
}

void GameManager::disconnectPlayer(uint32_t gameId, uint32_t playerId)
{
    std::lock_guard lock(mx);

    if (!getGame(gameId)) return;

    // delete player
    // game status -> ended
    // if deleted last player -> finish game
    // deleteFinishedGame()
    int id = getGameByPlayer(playerId);
    if (id == 0 || id != gameId) return;

    // delete player from player to game table
    gameByPlayer.erase(playerId);
    std::cout << "[Game Manager] Player (id=" << playerId << ") disconnected from game (id=" << gameId << ")" << std::endl;
    // then check if game already has been finished
    auto game = getGame(gameId);
    if (game->getStatus() == Game::Status::Ended) {
        // if true (deleted first player before) -> deleteFinishedGame()
        deleteFinishedGame(gameId);
    } else {
        // false (now deleting first time) -> should just finish cur game and wait until second player disconnect
        game->finish();
    }
}

int GameManager::findWaitingGame()
{
    std::lock_guard lock(mx);
    for (const auto& [id, game] : games) {
        if (game->getStatus() == Game::Status::Waiting)
            return id;
    }

    return 0;
}

void GameManager::deleteFinishedGame(uint32_t id)
{
    std::lock_guard lock(mx);

    if (!getGame(id)) return;

    games[id].reset();
    games.erase(id);
    std::cout << "[Game Manager] Deleted game id = " << id << std::endl;
}

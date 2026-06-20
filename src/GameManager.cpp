#include "GameManager.h"

GameManager* GameManager::instance = nullptr;
uint32_t GameManager::id = 0;

void GameManager::createNewGame(uint32_t firstPlayerId, uint32_t secondPlayerId)
{
    std::shared_ptr<Game> game = std::make_shared<Game>(firstPlayerId, secondPlayerId);
    std::cout << "Created new game id = " << id << std::endl;
    games[id++] = game;
}

void GameManager::daleteFinishedGame(uint32_t id)
{
    if (!getGame(id)) return;

    games[id].reset();
    games.erase(id);
    std::cout << "Deleted game id = " << id << std::endl;
}

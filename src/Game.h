#pragma once

#include <iostream>
#include <array>
#include <inttypes.h>
#include <vector>
#include <random>
#include <mutex>

// When one player connected to server game created (init player's ids as -1, check if xId == -1, else game already exists)
// When another player connected (maybe entering room code) add him to game by id (set oId = id we got, also check it was -1 (or check status?))
// check status in Game Manager!
// Make a draw (who will X and who O)
// Start game (change status on Playing) and wait for a move
// Parse json from WS and call makeMove(), then send result board to client
// Sometime need to change player id? Or it will be client problem?
// When game finished change status on Ended, save result in DB and delete game object (in Game Manager)
// And delete connections in Connection Manager (also if player disconnected ? think about it)

class Game
{
public:
    Game(uint32_t gameId, uint32_t playerId);

    enum class Cell {Empty, X, O};
    enum class Status {Waiting, Playing, Finished};

    bool addPlayer(uint32_t playerId);
    void draw();
    bool makeMove(int row, int col);
    void switchPlayers();
    std::vector<std::string> getBoard();
    bool checkWin(uint32_t playerId);
    bool isBoardFull();
    void finish();
    void play();
    std::pair<uint32_t, uint32_t> getPlayersIds();
    // uint32_t getXId() const {return xPlayerId;}
    // uint32_t getOId() const {return oPlayerId;}
    Status getStatus();
    uint32_t getId();
    uint32_t getCurrentPlayerId(); 

private:
    uint32_t id;
    std::array<std::array<Cell, 3>, 3> board;
    Status status = Status::Waiting;
    int filledCells = 0;
    uint32_t currentPlayerId;
    uint32_t xPlayerId = 0;
    uint32_t oPlayerId = 0;

    std::mutex mx;

    void start();
    bool isValidMove(int row, int col);
    void drawBoard();
    char charFromId(int playerId);
    char charFromCell(Cell cell);
};
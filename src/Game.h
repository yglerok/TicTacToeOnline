#pragma once

#include <iostream>
#include <array>
#include <inttypes.h>

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
    enum class Status {Waiting, Playing, Ended};

    bool addPlayer(uint32_t playerId);
    void draw();
    void start();
    bool makeMove(uint32_t playerId, int row, int col);
    void finish();
    void play();
    std::pair<uint32_t, uint32_t> getPlayersIds() const {
        return {xPlayerId, oPlayerId};
    }
    Status getStatus() const {return status;}
    uint32_t getId() const {return id;}

private:
    uint32_t id;
    std::array<std::array<Cell, 3>, 3> board;
    Status status = Status::Waiting;
    int filledCells = 0;
    uint32_t currentPlayerId;
    uint32_t xPlayerId = 0;
    uint32_t oPlayerId = 0;

    bool isValidMove(int row, int col);
    bool isFull() const { return filledCells == 9; }
    void drawBoard();
    void switchPlayers();
    bool checkWin(int playerId);
    char charFromId(int playerId);
    char charFromCell(Cell cell);
};
#pragma once

#include <iostream>
#include <array>
#include <inttypes.h>

class Game
{
public:
    Game(uint32_t xId, uint32_t oId);

    enum class Cell {Empty, X, O};

    bool makeMove(uint32_t playerId, int row, int col);
    void play();
    std::pair<uint32_t, uint32_t> getPlayersIds() const {
        return {xPlayerId, oPlayerId};
    }

private:
    std::array<std::array<Cell, 3>, 3> board;
    int filledCells = 0;
    uint32_t currentPlayerId;
    uint32_t xPlayerId;
    uint32_t oPlayerId;

    bool draw(uint32_t id1, uint32_t id2);
    bool isValidMove(int row, int col);
    bool isFull() const { return filledCells == 9; }
    void drawBoard();
    void switchPlayers();
    bool checkWin(int playerId);
    char charFromId(int playerId);
    char charFromCell(Cell cell);
};
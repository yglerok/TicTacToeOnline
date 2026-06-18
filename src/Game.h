#pragma once

#include <iostream>
#include <array>

class Game
{
public:
    Game(int xId, int oId);

    enum class Cell {Empty, X, O};

    bool makeMove(int playerId, int row, int col);
    void play();

private:
    std::array<std::array<Cell, 3>, 3> board;
    int filledCells = 0;
    int currentPlayerId;
    int xPlayerId;
    int oPlayerId;

    bool isValidMove(int row, int col);
    bool isFull() const { return filledCells == 9; }
    void drawBoard();
    void switchPlayers();
    bool checkWin(int playerId);
    char charFromId(int playerId);
    char charFromCell(Cell cell);
};
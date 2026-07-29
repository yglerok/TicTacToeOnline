#include "Game.h"

Game::Game(uint32_t gameId, uint32_t playerId) : id(gameId)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = Cell::Empty;

    if (xPlayerId == 0)
        xPlayerId = playerId;
    else 
        std::cout << "[Game] Player (id=" << playerId << ") is already in game" << std::endl;

}

bool Game::makeMove(uint32_t playerId, int row, int col)
{
    if (!isValidMove(row, col)) return false;

    board[row][col] = (playerId == xPlayerId) ? Cell::X : Cell::O;
    filledCells++;
    return true;
}

void Game::finish()
{
    switch (status)
    {
    case Status::Waiting:
    case Status::Playing:
        status = Status::Ended;
        std::cout << "[Game] Game (id=" << id << ") changed status to Ended" << std::endl;
        break;
    case Status::Ended:
        std::cout << "[Game] Game (id=" << id <<") already ended" << std::endl;
        break;
    default:
        break;
    }
}

void Game::play()
{
    int row, col;
    std::cout << "Game started" << std::endl;

    srand(time(0));

    currentPlayerId = (rand() % 2 == 1) ? xPlayerId : oPlayerId;

    while (!isFull()) {
        drawBoard();

        while (true) {
            std::cout << currentPlayerId << " (" << charFromId(currentPlayerId) << ") enter row (1-3) and column (1-3): ";
            std::cin >> row >> col;
            row--; col--;

            if (isValidMove(row, col)) 
                break;
            else
                std::cout << "Invalid move. Try again." << std::endl;    
        }

        makeMove(currentPlayerId, row, col);

        if (checkWin(currentPlayerId)) {
            drawBoard();
            std::cout << charFromId(currentPlayerId) << " wins!" << std::endl;
            return;
        }

        switchPlayers();
    }

    drawBoard();
    std::cout << "It's a draw!" << std::endl;
}

bool Game::addPlayer(uint32_t playerId)
{
    if (oPlayerId == 0) {
        oPlayerId = playerId;
        std::cout << "[Game] Player (id=" << playerId << ") added in game (id=" << id << ")" << std::endl;
        start();
    } else {
        std::cout << "[Game] Player (id=" << playerId << ") is already in game" << std::endl;
        return false;
    }
    return true;
}

void Game::draw()
{
    // simple algo, just imitating random
    if ((xPlayerId + oPlayerId) % 2 == 0)
        std::swap(xPlayerId, oPlayerId);
}

void Game::start()
{
    switch (status)
    {
    case Status::Waiting:
        status = Status::Playing;
        std::cout << "[Game] Game (id=" << id << ") changed status to Playing" << std::endl;
        break;
    case Status::Playing:
        std::cout << "[Game] Game (id=" << id <<") already playing" << std::endl;
        break;
    case Status::Ended:
        std::cout << "[Game] Game (id=" << id <<") already ended" << std::endl;
        break;
    default:
        break;
    }
}

bool Game::isValidMove(int row, int col)
{
    return (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == Cell::Empty);
}

void Game::drawBoard()
{
    std::cout << "-------" << std::endl;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j)
            std::cout << charFromCell(board[i][j]) << " ";
        std::cout << std::endl;
    }
    std::cout << "-------" << std::endl;
}

void Game::switchPlayers()
{
    currentPlayerId = (currentPlayerId == xPlayerId) ? oPlayerId : xPlayerId;
}

bool Game::checkWin(int playerId)
{
    Cell sym = (playerId == xPlayerId) ? Cell::X : Cell::O;
    
    for (int i = 0; i < 3; ++i)
        if (board[i][0] == sym && board[i][1] == sym && board[i][2] == sym)
            return true;
    
    for (int i = 0; i < 3; ++i)
        if (board[0][i] == sym && board[1][i] == sym && board[2][i] == sym)
            return true;

    if (board[0][0] == sym && board[1][1] == sym && board[2][2] == sym)
        return true;
    if (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)
        return true;

    return false;
}

char Game::charFromId(int playerId)
{
    Cell cell = (playerId == xPlayerId) ? Cell::X : Cell::O;

    char c = ' ';
    if (cell == Cell::X) c = 'X';
    if (cell == Cell::O) c = 'O';
    return c;
}

char Game::charFromCell(Cell cell)
{
    char c = ' ';
    if (cell == Cell::X) c = 'X';
    if (cell == Cell::O) c = 'O';
    return c;
}

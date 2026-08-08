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

bool Game::makeMove(int row, int col)
{
    std::lock_guard lock(mx);

    if (!isValidMove(row, col)) return false;

    board[row][col] = (currentPlayerId == xPlayerId) ? Cell::X : Cell::O;
    filledCells++;
    // switchPlayers();
    return true;
}

std::vector<std::string> Game::getBoard()
{
    std::lock_guard lock(mx);

    std::vector<std::string> boardVector;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            boardVector.push_back((board[i][j] == Cell::Empty) ? " " : ((board[i][j] == Cell::X) ? "X" : "O"));
        }
    }
    return boardVector;
}

void Game::finish()
{
    switch (status)
    {
    case Status::Waiting:
    case Status::Playing:
        status = Status::Finished;
        std::cout << "[Game] Game (id=" << id << ") changed status to Finished" << std::endl;
        break;
    case Status::Finished:
        std::cout << "[Game] Game (id=" << id <<") already finished" << std::endl;
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

    while (!isBoardFull()) {
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

        makeMove(row, col);

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

std::pair<uint32_t, uint32_t> Game::getPlayersIds()
{
    std::lock_guard lock(mx);
    return {xPlayerId, oPlayerId};
}

Game::Status Game::getStatus()
{
    std::lock_guard lock(mx);
    return status;
}

uint32_t Game::getCurrentPlayerId()
{
    std::lock_guard lock(mx);
    return currentPlayerId;
}

bool Game::addPlayer(uint32_t playerId)
{
    std::lock_guard lock(mx);

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
    std::lock_guard lock(mx);

    std::random_device rd;
    std::mt19937 gen(rd());

    currentPlayerId = (gen() % 2) ? xPlayerId : oPlayerId;
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
    case Status::Finished:
        std::cout << "[Game] Game (id=" << id <<") already finished" << std::endl;
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
    std::lock_guard lock(mx);

    currentPlayerId = (currentPlayerId == xPlayerId) ? oPlayerId : xPlayerId;
}

bool Game::checkWin(uint32_t playerId)
{
    std::lock_guard lock(mx);
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

bool Game::isBoardFull()
{
    std::lock_guard lock(mx);
    return (filledCells == 9);
}

uint32_t Game::getId()
{
    std::lock_guard lock(mx);
    return id;
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

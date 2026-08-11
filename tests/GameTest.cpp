#include <gtest/gtest.h>
#include "Game.h"

TEST(GameTest, FirstPlayerBecomesX) {
    Game game(1, 42);

    auto [xId, oId] = game.getPlayersIds();
    EXPECT_EQ(xId, 42u);
    EXPECT_EQ(oId, 0u);
}

TEST(GameTest, NewGameBoardIsEmpty) {
    Game game(1, 42);

    auto board = game.getBoard();
    EXPECT_EQ(board.size(), 9);
    for (const auto& cell : board) 
        EXPECT_EQ(cell, " ");
}

TEST(GameTest, NewGameStartsInWatingStatus) {
    Game game(1, 42);

    EXPECT_EQ(game.getStatus(), Game::Status::Waiting);
}

TEST(GameTest, AddingSecondPlayerStartsTheGame) {
    Game game(1, 42);
    
    game.addPlayer(24);
    EXPECT_EQ(game.getStatus(), Game::Status::Playing);
}
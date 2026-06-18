cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME TicTacToe)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp src/Game.cpp)
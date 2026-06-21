cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_NAME TicTacToe)

project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_executable(${PROJECT_NAME} src/main.cpp src/Game.cpp src/GameManager.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE third_party)
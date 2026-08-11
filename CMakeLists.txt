cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_NAME TicTacToe)

project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_library(TicTacToeCore 
    src/Game.cpp 
    src/GameManager.cpp
    src/ConnectionManager.cpp
)   

target_include_directories(TicTacToeCore PUBLIC src third_party)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE TicTacToeCore)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
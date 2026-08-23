cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)
set(PROJECT_NAME TicTacToe)

project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include(FetchContent)

FetchContent_Declare(
    libpqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx.git
    GIT_TAG 7.8.1
)

set(PQXX_LIBRARIES "pqxx")
set(SKIP_BUILD_TEST ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(libpqxx)

add_library(TicTacToeCore 
    src/Game.cpp 
    src/GameManager.cpp
    src/ConnectionManager.cpp
    src/Database.cpp
)   

target_include_directories(TicTacToeCore PUBLIC src third_party)
target_link_libraries(TicTacToeCore PUBLIC pqxx)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE TicTacToeCore)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/public
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/public
    COMMENT "Copying public/ next to the built binary"
)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
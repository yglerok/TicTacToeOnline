include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    main.cpp
    GameTest.cpp)

target_link_libraries(unit_tests PRIVATE TicTacToeCore GTest::gtest)

include(GoogleTest)
gtest_discover_tests(unit_tests)
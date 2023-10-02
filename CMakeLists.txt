cmake_minimum_required(VERSION 3.27)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


add_executable(${PROJECT_NAME}
   src/main.cpp
   src/gameoflife.h
   src/gameoflife.cpp
 )

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

enable_testing()

add_executable(
    gol_test
    src/gameoflife.h
    src/gameoflife.cpp
    src/test.cpp
)
target_link_libraries(gol_test
    ${SDL2_LIBRARIES}
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gol_test)

  

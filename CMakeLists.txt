cmake_minimum_required(VERSION 3.14)

project(battlesnake_hungry)

include(FetchContent)

# Import BattleSnake Engine library
FetchContent_Declare(battlesnake-engine-cpp
  GIT_REPOSITORY https://github.com/TheApX/battlesnake-engine-cpp
  GIT_TAG v0.6.0)
FetchContent_MakeAvailable(battlesnake-engine-cpp)


set(CMAKE_CXX_STANDARD 20)

# Suppress long and annoying warning.
set(GCC_NO_PSABI_WARNING "-Wno-psabi")
add_definitions(${GCC_NO_PSABI_WARNING})

set(battlesnake_hungry_SRCS_BASE
    snake_hungry.cpp
    move_comparator.cpp
)

set(battlesnake_hungry_SRCS
    ${battlesnake_hungry_SRCS_BASE}
    main.cpp
)

add_executable(battlesnake_hungry ${battlesnake_hungry_SRCS})

target_link_libraries(battlesnake_hungry
    libbattlesnakeinterface
    libbattlesnakeserver
    libbattlesnakejson
    simple-web-server
)


set(battlesnake_hungry_test_SRCS
    ${battlesnake_hungry_SRCS_BASE}
    snake_hungry_test.cpp
)

add_executable(battlesnake_hungry_test ${battlesnake_hungry_test_SRCS})

target_link_libraries(battlesnake_hungry_test
    libbattlesnakeinterface
    libbattlesnakejson
    gtest_main
    gmock_main
)

# Copy test data to binaries directory
add_custom_target(battlesnake_hungry_test_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/testdata
    ${CMAKE_CURRENT_BINARY_DIR}/testdata
    DEPENDS ${battlesnake_hungry_test})

enable_testing()
add_test(NAME battlesnake_hungry_test
         COMMAND battlesnake_hungry_test)

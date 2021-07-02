# Project structure

* BattleSnake source
  * [main.cpp](../main.cpp)
    * Main entrance point.
    * Just creates `SnakeHungry` object, server object and runs the server.
  * [snake_hungry.h](../snake_hungry.h) and [snake_hungry.cpp](../snake_hungry.cpp)
    * Implementation of the `Battlesnake` interface.
    * Returns snake customization.
    * Creates `MoveComparator` and compares all possible moves to find the best one.
  * [move_comparator.h](../move_comparator.h) and [move_comparator.cpp](../move_comparator.cpp)
    * Main decision-making logic - `MoveComparator`.
* Unit tests
  * [snake_hungry_test.cpp](../snake_hungry_test.cpp)
    * Unit tests source code
  * testdata
    * Directory for json files with test data.
* Build configuration
  * [CMakeLists.txt](../CMakeLists.txt)
    * CMake project defition
  * [Dockerfile](../Dockerfile) and [docker-compose.yml](../docker-compose.yml)
    * Configuration files for building and running in [Docker](https://www.docker.com/) container.

[Back to contents](../README.md#contents)

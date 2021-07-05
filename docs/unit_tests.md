# Unit testing

Unit tests are implemented in [snake_hungry_test.cpp](../snake_hungry_test.cpp). Instead of running and testing the whole web-server it creates the `SnakeHungry` class and tests its methods directly.

* `SnakeIsNotBoring` test case is one of the most important! It checks that the snake is not boring and doesn't use default color, head and tail.
* 4 `...OutOfBounds` test cases check that the snake doesn't go outside of the game board.
* `OtherSnakeBody` test case makes sure that the snake doesn't run into other snake's body.
* `DontBreakYourNeck` test case checks that the snake doesn't turn 180 degree.
* `GoToFood` checks that the snakes chooses area where a food is reachable.
* `GoToClosestFood` makes sure that the snake prefers the closest food.

All test cases construct game state manually. But it is also possible to load game state from json files stored in the `testdata` directory. `LoadState("foo")` loads json file `testdata/foo.json` and creates game state object from it. You can log all games in your battlesnake and use real-life games data in unit tests.

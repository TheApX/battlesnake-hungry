# The Very Hungry Caterpillar 🍊🍏🍑🍒🍎🐛

Hungry rule-based [BattleSnake](http://play.battlesnake.com) written in C++. Knows how to avoid walls, other snakes and find the closest food. Made for testing and educational purposes. [Public](https://play.battlesnake.com/u/theapx/the-very-hungry-caterpillar/). Can pass a few [challenges](https://play.battlesnake.com/challenges/) and has reached Gold medal on [Global Arena](https://play.battlesnake.com/arena/global/).

![Gold medal on Global Arena](docs/the-very-hungry-caterpillar-global-arena-card.png)

This BattleSnake is based on the web-server provided with the unofficial [C++ BattleSnake engine](https://github.com/TheApX/battlesnake-engine-cpp).

The Very Hungry Caterpillar is a simple battlesnake that knows how to
* Avoid walls
* Avoid other snakes
* Find the closest food

You can build and run it as is, if you want. There are even Docker files included to make deployment easier. You can fork and modify it to make it smarter. But I **really** hope that you:
* Carefully read this doc and source code.
* Understand how it works.
* Implement your own battlesnake from scratch.
* Use your favorite language. C++ isn't the best choice if you just learn to code, really.

# Why?

The Very Hungry Caterpillar isn't very strong battlesnake. Its older brothers [Hungry Caterpillar](https://play.battlesnake.com/u/theapx/hungry-caterpillar/) and [Smart Caterpillar](https://play.battlesnake.com/u/theapx/smart-caterpillar/) show much better results in arenas. So why was it created?
* To test the server provided with the unofficial [C++ BattleSnake engine](https://github.com/TheApX/battlesnake-engine-cpp).
* To show how to use the server in your battlesnakes.
* To educate you about basic BattleSnake strategies and useful algorithms.

# Contents

* [Backstory](docs/backstory.md)
* [Build, test and run](docs/build_run.md)
* [Project structure](docs/project_structure.md)
* [Rule-based battlesnake design](docs/design.md)
* [Rules of The Very Hungry Caterpillar](docs/basic_rules.md)
* [Finding the shortest path to food with BFS](docs/path_to_food_bfs.md)
* [Unit testing](docs/unit_tests.md)
* [What's next](docs/whats_next.md)

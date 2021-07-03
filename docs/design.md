# Rule-based battlesnake design

Design described here is not the only possible design. In fact, the not-so-hungry brother [Hungry Caterpillar](https://play.battlesnake.com/u/theapx/hungry-caterpillar/) is implemented in a different way. But the approach described here allows for cleaner, more flexible, better structured and better optimized code.

[Smart Caterpillar](https://play.battlesnake.com/u/theapx/smart-caterpillar/) is designed this way and proved to be very [competitive](backstory.md). The only difference is that Smart Caterpillar computes more things and has a few extra rules.

## High-level design

High-level design is simple:
* Function that compares two possible moves based on a set of rules.
* Pre-compute everything needed to compare moves with each other.
* Check all possible moves, compare them and find the best one.

Comparison function and all necessary precomputations are done in the [MoveComparator](../move_comparator.cpp) class. Its constructor and `initBoardMatrix` method pre-compute everything needed. `IsBetter` compares two moves together.

All possible moves are checked in the `Move` method of the [SnakeHungry](../snake_hungry.cpp) class.

## Check all possible moves

Let's take a closer look at the [SnakeHungry::Move](../snake_hungry.cpp) method. This method:
1. Creates `MoveComparator` object. It pre-computes everything needed in constructor.
2. Initializes the variable where the best move founds will be stored. Sets it to "unknown" move initially.
3. Creates list of all possible moves (left, right, up and down).
4. Iterates through all moves and compares each of them with the previously found best move using `MoveComparator::IsBetter`. If a better move is founds, stores it.
5. Returns the best move found.

Note that `SnakeHungry::Move` takes a `GameState` object and returns `Battlesnake::MoveResponse`. HTTP requests handling, json conversions and everything else is handled by the server class.

## Moves comparison function

The main decision-making code is the [MoveComparator::IsBetter](../move_comparator.cpp) function. This function takes two arguments: candidate move and previously found best move, and returns `true` if the candidate move is "better" than the previously found best.

The definition of "better" defines snake's behavior. For example, the snake will prefer to move to a free cell over other snake's body if this function tells that a move to a free cell is "better" than a move to a body. It must also correctly compare the reverse case - a move to a body is "worse" (not "better") than a more to a free cell.

This function is normally defined as a set of rules like these:
* "A move that `{rule A}` is worse/better than any other move"
* "Among other moves a move that `{rule B}` is worse/better then any other move"
* ...
* "Among other moves a move that `{rule Z}` is worse/better then any other move"

When all rules are checked, there may be two moves that are equally "good". It is up to you which move to prefer in this case. You can always prefer the new candidate or the old best move (like The Very Hungry Caterpillar does). Or you can choose random one. Another option is to say that move up is better than any other, down is better than left and right, and left is better than right. Use your fantasy. Try different approaches. Maybe you will come up with an idea of another rule.

We will talk about actual rules that The Very Hungry Caterpillar uses in the [next section](basic_rules.md).

## Pre-compute everything needed

This step depends on what information the comparison function needs. For example, if the comparison function tries to follow path to the closest food, it's better to pre-compute and store some useful information instead of looking for a path on each comparison.

A fancy battlesnake may be smart enough to compute and cache only what is really needed. No precomputations may be needed in this case, but comparison function becomes more complex. For example, there is no need to find a path to food, if there is only one possible next move that doesn't lead to immediate elimination.

But The Very Hungry Caterpillar is not that smart, and it always pre-computes information needed to find the path to the closest food. We will talk about it [later](path_to_food_bfs.md).

But first let's talk about [basic safety](basic_rules.md).

[Back to contents](../README.md#contents)

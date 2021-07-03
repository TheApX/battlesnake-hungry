# Rules of The Very Hungry Caterpillar

Basic "safety" rules are the first rules checked by the comparison function. Rules checked earlier have higher priority, and basic safety rules make sure that the snake doesn't do obviously dangerous things.

Let's look at the [MoveComparator::IsBetter](../move_comparator.cpp) implementation.

## Rules are symmetrical

Note that all rules are symmetrical for the next move candidate and the best move found so far. If the comparison function returns `false` for "unknown" candidate move, it returns `true` for "unknown" best move found so far. This is straightforward for candidate move - if it's worse than anything else, we don't even check other rules.

But it is a bit less obvious for the best move found so far. Consider two moves `a` and `b`. The comparison function must not depend on the order of moves, so `IsBetter(a, b)` must be the opposite of `IsBetter(b, a)`. That's why each rule must check not only the candidate, but the best move found as well, and return the opposite result for it.

For example, if the best move founds so far is "unknown" move, then any other move will be better than it, and the function must return `true` regardless of the candidate move.

## Equally good moves from a rule's perspective

What if two moves are equally good or bad from the perspective of some rule? Ideally, this rule must make no decision and let subsequent rules decide. This is what the rule that compares steps to the closest food does - it makes a decision only if one step is closer to a food than another. If distances are equal, it doesn't make any decision.

But the initial set of basic safety rules can ignore it. The difference between  higher-level and basic safety rules is like the difference between "A food is close here, but it's equally close there, I don't know..." and "Don't do it no matter what! You will be eliminated otherwise!"

Some basic safety rules may even protect higher-level rules from doing illegal things if they always make a decision when violated. For example, out-of-bounds check doesn't let any other rule check numbers of steps to cells that are not even part of the game board.

# Pre-calculations

Before we dive into actual rules, let's take a look at the pre-calculations first. The `initBoardMatrix()` function is the pre-calculation we need. `MoveComparator` has `board_matrix_` vector, which stores some information about each cell on the game board.

The board is 2-dimensional, but `board_matrix_` is a simple 1-dimensional vector. In order to keep 2-dimensional matrix in a 1-dimensional array, we store it line by line. If the matrix has width `W` and height `H`, then:
* Elements [0..W-1] store the first line (line index 0)
* Elements [W...2*W-1] store the second line (line index 1)
* ...
* Elements [l*W...l*W+W-1] store the line index l

So given coordinates `x` and `y` we can calculate the index using formula `i = y*W+x`. And total size of the array is equal to the total area of the board - `W * H`. The `step(x,y)` function manages translations from matrix coordinates to vector indexes.

## Initialize the board matrix

The first thing `initBoardMatrix()` does is resizes the vector from empty to `W * H` and fills it with `kMatrixUninitialized` value. This value essentially means "empty" cells on the board.

## Mark cells occupied by snakes bodies

The next step of pre-calculation is to mark cells occupied by snakes bodies. This will allow to quickly look up if a move leads to a snake's body. `markSnakeBodiesOnMatrix()` iterates over all snakes and all body elements except the last one, and sets the matrix value to `kMatrixSnakeBody`. The tail will move on the next step, and we can step there safely.

## Compute distances to closest food

The last step is to compute distance from each cell to the food closest to that cell. Distances are stored in the same vector where we marked snake bodies. We will discus it in detail in the [next section](path_to_food_bfs.md).

## Pre-calculations results

After these pre-calculations the `board_matrix_` vector contains these values for each cell:
* `kMatrixSnakeBody` if it is occupied by a body.
* Distance from the closest food, if food is reachable from it.
* `kMatrixUninitialized` if food is unreachable.

# Rules

## "Unknown" moves

Search for the best move starts with the "unknown" move, which is worse than any other move. If our snake responds with "unknown" move, BattleSnake will decide where out snake will go, and that's not what we want. We want to be in control of our snake.

## Get next coordinates of moves

Move direction itself doesn't tell much. Position where the head will be after this move does. `MoveComparator` constructor (`MoveComparator::MoveComparator` method) pre-calculates positions for each direction, and we use them here. This is not a rule, but these positions are used by all moves below.

## Don't "break your neck"

As long as our battlesnake avoids snakes bodies, this rule is redundant. We just want to make sure that our snake never turns 180 degree and steps on its own neck. It just doesn't look good.

## Don't go out of bounds

This rule is simple. If a move leads to coordinates outside of the board, don't go there. It just checks if head coordinates after the move are negative or higher than size of the board.

## Don't step on snake body

This step uses pre-calculated board matrix. If the board matrix for the next step contains value `kMatrixSnakeBody`, it is occupied by a snake, and we shouldn't go there.

## Prefer to go where a food is reachable

This is the first rule that is about preference, but not just safety. There are situations where one steps leads to a food, but the other one doesn't. Prefer to go where we can reach food. If both steps don't lead to any food, don't make any decision. If both steps lead to some food, don't make any decision here, either.

Cells where food is reachable have distances from the closest food. Cells that are not reachable, have `kMatrixUninitialized` value.

## Prefer cells closer to a food

This rule can only be applied to cells where food is reachable. Just compare distances from both cells to the closest food, and if they are different, choose one. If they are the same, this rule doesn't make any decision, and lets other rules decide.

You may wonder, why don't just pick one cell here if they are both equally good? Because we want to make smarter decisions. Yes, cells are equally close to a food. But what if one of them is too close to a larger snake, and thus dangerous? Or what if one of them also blocks other snakes' path to this food? We try to make our snake as smart as possible, so we don't want to select next move randomly.

## The last rule

As we discussed in the [previous section](design.md), the comparison function has to make a decision, when all rules are checked. This happens when two cells are equally good from the perspective of all rules. The Very Hungry Caterpillar always prefers the already found best move - it always returns `false` after all rules are checked. Try different options and see how your snake behaves.

[Back to contents](../README.md#contents)

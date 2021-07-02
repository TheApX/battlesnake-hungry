#include "move_comparator.h"

#include <limits>
#include <queue>

using namespace battlesnake::rules;

// Values are chosen so that they can be compared with regular numbers of
// steps from food.
enum SpecialMatrixValues {
  // Snake body is the "furthest" from any food. Don't go there if there is
  // any other choice.
  kMatrixSnakeBody = std::numeric_limits<int>::max(),
  // Uninitialized value means that there is no path from a food there. Better
  // than a snake body, but still worse than any other food.
  kMatrixUninitialized = std::numeric_limits<int>::max() - 1,
};

MoveComparator::MoveComparator(const battlesnake::rules::BoardState* board,
                               const battlesnake::rules::Snake* you)
    : board_(*board), you_(*you) {
  // Pre-calculate next points for moves.
  const Point& head = you_.Head();
  move_points_[Move::Left] = head.Left();
  move_points_[Move::Right] = head.Right();
  move_points_[Move::Up] = head.Up();
  move_points_[Move::Down] = head.Down();

  // Pre-calculate steps from food.
  initBoardMatrix();
}

bool MoveComparator::IsBetter(Move move, Move best) {
  if (move == Move::Unknown) {
    // Unknown move is worse than everything.
    return false;
  }
  if (best == Move::Unknown) {
    // Everything is better than unknown move.
    return true;
  }

  const Point& move_point = move_points_[move];
  const Point& best_point = move_points_[best];

  if (you_.Length() >= 2) {
    // If we have a neck, never go to it. "Broken" neck just doesn't look good.
    const Point& neck = you_.body[1];
    if (move_point == neck) {
      return false;
    }
    if (best_point == neck) {
      return true;
    }
  }

  // Don't go out of bounds.
  if (isOutOfBounds(move_point)) {
    return false;
  }
  if (isOutOfBounds(best_point)) {
    return true;
  }

  // Don't step on snake body.
  if (steps(move_point) == kMatrixSnakeBody) {
    return false;
  }
  if (steps(best_point) == kMatrixSnakeBody) {
    return true;
  }

  // Prefer not to go where there is no reachable food.
  if (steps(move_point) == kMatrixUninitialized) {
    return false;
  }
  if (steps(best_point) == kMatrixUninitialized) {
    return true;
  }

  // Prefer move that is closer to food.
  if (steps(move_point) < steps(best_point)) {
    return true;
  }
  if (steps(move_point) > steps(best_point)) {
    return false;
  }

  // If moves are equally good, prefer the old best.
  return false;
}

// Convenience function for easy access to the value by x and y. Note that it
// returns reference, so the value can be modified.
int& MoveComparator::steps(int x, int y) {
  return this->board_matrix_[y * this->board_.width + x];
}

void MoveComparator::initBoardMatrix() {
  // Initialize board matrix with zeroes. Two-dimensional matrix is represented
  // as one-dimensional vector, which holds values line by line.
  board_matrix_.resize(board_.width * board_.height, kMatrixUninitialized);

  // Mark snake bodies as unreachable.
  markSnakeBodiesOnMatrix();

  // Compute steps from food.
  computeStepsFromFood();
}

void MoveComparator::markSnakeBodiesOnMatrix() {
  for (const Snake& snake : board_.snakes) {
    // Ignore tail because we can step on it.
    for (int i = 0; i < snake.body.size() - 1; ++i) {
      Point p = snake.body[i];
      steps(p.x, p.y) = kMatrixSnakeBody;
    }
  }
}

void MoveComparator::computeStepsFromFood() {
  // Use BFS with multiple starting points to fill the matrix with distances to
  // the closest food.
  std::queue<Point> bfs;

  for (const Point& food : board_.food) {
    // Push each food coordinates to the queue.
    bfs.push(food);
    // Mark that the food is reachable in zero steps there.
    steps(food) = 0;
  }

  // Process BFS queue until it is empty.
  while (!bfs.empty()) {
    Point current = bfs.front();
    int current_steps = steps(current);
    bfs.pop();

    // Initialize the "adjacency list" for current position. Some positions may
    // be on a snake body or out of bounds - they will be filtered out below.
    auto around = {
        current.Left(),
        current.Right(),
        current.Up(),
        current.Down(),
    };

    for (const Point& next : around) {
      if (isOutOfBounds(next)) {
        // Ignore positions out of bounds.
        continue;
      }
      if (steps(next) == kMatrixSnakeBody) {
        // Ignore moves to snake bodies.
        continue;
      }
      if (steps(next) != kMatrixUninitialized) {
        // Ignore positions where number of steps is already calculated. Due to
        // the nature of BFS, the first calculated distance is the lowest.
        continue;
      }

      // Found a position that wasn't visited before. Add it to the BFS queue.
      bfs.push(next);
      // And mark that it can be reached on the next step.
      steps(next) = current_steps + 1;
    }
  }
}

bool MoveComparator::isOutOfBounds(const Point& point) {
  return point.x < 0 || point.y < 0 || point.x >= board_.width ||
         point.y >= board_.height;
}

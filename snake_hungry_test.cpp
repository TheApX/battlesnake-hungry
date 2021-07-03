#include "snake_hungry.h"

#include <battlesnake/json/converter.h>
#include <unistd.h>

#include <fstream>
#include <streambuf>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::battlesnake::rules;
using namespace ::battlesnake::interface;

using ::testing::AnyOf;
using ::testing::Ne;

GameState LoadState(const std::string& test_name) {
  std::ifstream file_in("testdata/" + test_name + ".json");
  std::string str((std::istreambuf_iterator<char>(file_in)),
                  std::istreambuf_iterator<char>());
  return battlesnake::json::ParseJsonGameState(nlohmann::json::parse(str));
}

TEST(BattleSnakeHungryTest, SnakeIsNotBoring) {
  SnakeHungry battlesnake;
  Customization customization = battlesnake.GetCustomization();

  // Anything but default!
  EXPECT_THAT(customization.color, Ne("#888888"));
  EXPECT_THAT(customization.head, Ne("default"));
  EXPECT_THAT(customization.tail, Ne("default"));
}

TEST(BattleSnakeHungryTest, BottomLeftOutOfBounds) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(0, 0),
                              Point(0, 1),
                              Point(0, 2),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Don't go out of bounds.
  EXPECT_THAT(move.move, AnyOf(Move::Right, Move::Up));
}

TEST(BattleSnakeHungryTest, BottomRightOutOfBounds) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(4, 0),
                              Point(4, 1),
                              Point(4, 2),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Don't go out of bounds.
  EXPECT_THAT(move.move, AnyOf(Move::Left, Move::Up));
}

TEST(BattleSnakeHungryTest, TopLeftOutOfBounds) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(0, 4),
                              Point(0, 3),
                              Point(0, 2),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Don't go out of bounds.
  EXPECT_THAT(move.move, AnyOf(Move::Right, Move::Down));
}

TEST(BattleSnakeHungryTest, TopRightOutOfBounds) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(4, 4),
                              Point(4, 3),
                              Point(4, 2),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Don't go out of bounds.
  EXPECT_THAT(move.move, AnyOf(Move::Left, Move::Down));
}

TEST(BattleSnakeHungryTest, OtherSnakeBody) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(1, 2),
                              Point(2, 2),
                              Point(3, 2),
                          },
                      .health = 100,
                  },
                  Snake{
                      .id = "Smart Caterpillar",
                      .body =
                          {
                              Point(0, 0),
                              Point(0, 1),
                              Point(0, 2),
                              Point(0, 3),
                              Point(0, 4),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Don't go left - there is another snake.
  // Don't go right - there is our own body.
  // | . . . .
  // | . . . .
  // | o---- .
  // | . . . .
  // o . . . .
  EXPECT_THAT(move.move, AnyOf(Move::Up, Move::Down));
}

TEST(BattleSnakeHungryTest, DontBreakYourNeck) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(0, 0),
                              Point(1, 0),
                              Point(1, 1),
                              Point(0, 1),
                              Point(0, 2),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Any move leads to elimination. Make sure that we don't go back and "break"
  // our neck. Neck is on the right.
  // . . . . .
  // . . . . .
  // | . . . .
  // \-\ . . .
  // o / . . .
  EXPECT_THAT(move.move, AnyOf(Move::Left, Move::Up, Move::Down));
}

TEST(BattleSnakeHungryTest, GoToFood) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .food =
              {
                  Point(0, 0),
              },
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(2, 0),
                              Point(2, 1),
                              Point(2, 2),
                              Point(2, 3),
                              Point(2, 4),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Food is on the left, and there is no reachable food on the right:
  // . . | . .
  // . . | . .
  // . . | . .
  // . . | . .
  // x . o . .
  EXPECT_THAT(move.move, AnyOf(Move::Left));
}

TEST(BattleSnakeHungryTest, GoToClosestFood) {
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .food =
              {
                  Point(0, 4),
                  Point(3, 3),
              },
          .snakes =
              {
                  Snake{
                      .id = "The Very Hungry Caterpillar",
                      .body =
                          {
                              Point(2, 0),
                              Point(2, 1),
                              Point(2, 2),
                          },
                      .health = 100,
                  },
              },
      },
  };
  state.you = state.board.snakes.front();

  SnakeHungry battlesnake;
  Battlesnake::MoveResponse move = battlesnake.Move(state);

  // Food on the right is closer:
  // x . . . .
  // . . . x .
  // . . | . .
  // . . | . .
  // . . o . .
  EXPECT_THAT(move.move, AnyOf(Move::Right));
}

// --------------------------

#include <queue>
#include <unordered_map>
#include <unordered_set>

struct Node {
  // Array of adjacent nodes, ending with nullptr.
  Node* adjacent_nodes;
};

bool IsGoal(Node*);

int BFS(Node* root) {
  std::queue<Node*> q;
  std::unordered_map<Node*, int> distances;

  // Push the root initially and mark it as visited.
  q.push(root);
  distances[root] = 0;

  // Process queue until it's empty.
  while (!q.empty()) {
    // Pop the first element from the queue.
    Node* current = q.front();
    q.pop();
    // If we found the goal, return it.
    if (IsGoal(current)) {
      return distances[current];
    }
    // Add all not visited adjacent nodes to the queue and mark them as visited.
    for (Node* adjacent = current->adjacent_nodes; adjacent != nullptr;
         ++adjacent) {
      if (distances.find(adjacent) == distances.end()) {
        distances[adjacent] = distances[current] + 1;
        q.push(adjacent);
      }
    }
  }

  // If no goal found, return -1, which is not a valid distance.
  return -1;
}

std::vector<Node*> ReconstructPath(std::unordered_map<Node*, int>& distances,
                                   Node* destination) {
  std::vector<Node*> reversed_result;

  // Start with the last step, and find the previous ones in cycle.
  Node* current = destination;
  while (distances[current] != 0) {
    // Remember current step.
    reversed_result.push_back(current);
    // Find the previous step.
    // This assumes that the graph is non-directional.
    for (Node* adjacent = current->adjacent_nodes; adjacent != nullptr;
         ++adjacent) {
      // We are looking for a node at distance N-1.
      if (distances[adjacent] == distances[current] - 1) {
        current = adjacent;
        break;
      }
    }
  }

  // `reversed_result` now contains all steps except the initial node in
  // the reversed order. Reverse it and return.
  std::vector<Node*> result;
  for (int i = reversed_result.size() - 1; i >= 0; --i) {
    result.push_back(reversed_result[i]);
  }
  return result;
}

bool IsGoal(Node* n) { return false; }
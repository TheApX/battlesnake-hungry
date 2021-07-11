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

GameState LoadState(const std::string& test_name, StringPool& pool) {
  std::ifstream file_in("testdata/" + test_name + ".json");
  std::string str((std::istreambuf_iterator<char>(file_in)),
                  std::istreambuf_iterator<char>());
  return battlesnake::json::ParseJsonGameState(nlohmann::json::parse(str),
                                               pool);
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{0, 0},
                              Point{0, 1},
                              Point{0, 2},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{4, 0},
                              Point{4, 1},
                              Point{4, 2},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{0, 4},
                              Point{0, 3},
                              Point{0, 2},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{4, 4},
                              Point{4, 3},
                              Point{4, 2},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{1, 2},
                              Point{2, 2},
                              Point{3, 2},
                          },
                      .health = 100,
                  },
                  Snake{
                      .id = pool.Add("Smart Caterpillar"),
                      .body =
                          {
                              Point{0, 0},
                              Point{0, 1},
                              Point{0, 2},
                              Point{0, 3},
                              Point{0, 4},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{0, 0},
                              Point{1, 0},
                              Point{1, 1},
                              Point{0, 1},
                              Point{0, 2},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .food =
              {
                  Point{0, 0},
              },
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{2, 0},
                              Point{2, 1},
                              Point{2, 2},
                              Point{2, 3},
                              Point{2, 4},
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
  StringPool pool;
  GameState state{
      .board{
          .width = 5,
          .height = 5,
          .food =
              {
                  Point{0, 4},
                  Point{3, 3},
              },
          .snakes =
              {
                  Snake{
                      .id = pool.Add("The Very Hungry Caterpillar"),
                      .body =
                          {
                              Point{2, 0},
                              Point{2, 1},
                              Point{2, 2},
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

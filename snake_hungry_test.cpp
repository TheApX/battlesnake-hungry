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

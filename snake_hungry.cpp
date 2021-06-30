#include "snake_hungry.h"

#include <iostream>
#include <random>
#include <vector>

using namespace battlesnake::rules;
using namespace battlesnake::interface;

Customization SnakeHungry::GetCustomization() {
  std::cout << "Customization" << std::endl;

  return Customization{
      .color = "#de2ac9",
      .head = "rudolph",
      .tail = "bonhomme",
  };
};

void SnakeHungry::Start(const GameState& game_state) {
  std::cout << "Start: " << game_state.game.id << std::endl;
};

void SnakeHungry::End(const GameState& game_state) {
  std::cout << "End: " << game_state.game.id << std::endl;
};

Battlesnake::MoveResponse SnakeHungry::Move(const GameState& game_state) {
  std::vector<battlesnake::rules::Move> possible_moves{
      Move::Left,
      Move::Right,
      Move::Up,
      Move::Down,
  };

  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distribution(0, possible_moves.size() - 1);

  Battlesnake::MoveResponse result{
      .move = possible_moves[distribution(generator)],
  };

  std::cout << "Move: " << game_state.game.id << " turn " << game_state.turn
            << "  -  " << result.move << std::endl;
  return result;
};

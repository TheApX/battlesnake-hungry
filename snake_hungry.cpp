#include "snake_hungry.h"

#include <iostream>
#include <random>
#include <vector>

#include "move_comparator.h"

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

  MoveComparator move_comparator(&game_state.board, &game_state.you);

  Battlesnake::MoveResponse result{
      .move = Move::Unknown,
  };

  for (auto move : possible_moves) {
    if (move_comparator.IsBetter(move, result.move)) {
      result.move = move;
    }
  }

  std::cout << "Move: " << game_state.game.id << " turn " << game_state.turn
            << "  -  " << result.move << std::endl;
  return result;
};

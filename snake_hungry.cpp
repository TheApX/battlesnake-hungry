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
      .color = "#2e8244",  // The Very Hungry Caterpillar color.
      .head = "rudolph",   // Not exactly the same "face", but good enough :)
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
  // Initialize the main decision making logic with current board state.
  MoveComparator move_comparator(&game_state.board, &game_state.you);

  // Start with "unknown" move.
  Battlesnake::MoveResponse result{
      .move = Move::Unknown,
  };

  // Try all possible moves, and find the best one.
  std::vector<battlesnake::rules::Move> possible_moves{
      Move::Left,
      Move::Right,
      Move::Up,
      Move::Down,
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

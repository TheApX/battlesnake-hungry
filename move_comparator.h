#include <battlesnake/rules/data_types.h>

#include <unordered_map>

class MoveComparator {
 public:
  MoveComparator(const battlesnake::rules::BoardState* board,
                 const battlesnake::rules::Snake* you);

  bool IsBetter(battlesnake::rules::Move move, battlesnake::rules::Move best);

 private:
  const battlesnake::rules::BoardState& board_;
  const battlesnake::rules::Snake& you_;
  std::unordered_map<battlesnake::rules::Move, battlesnake::rules::Point>
      move_points_;
  std::vector<int> board_matrix_;

  int& steps(int x, int y);
  int& steps(const battlesnake::rules::Point& p) { return steps(p.x, p.y); }
  void initBoardMatrix();
  void markSnakeBodiesOnMatrix();
  void computeStepsFromFood();
  bool isOutOfBounds(const battlesnake::rules::Point& point);
};

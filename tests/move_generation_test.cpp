#include <gtest/gtest.h>
#include "board.h"
#include "move_generation.h"
#include "eval.h"

// Demonstrate some basic assertions.
TEST(MoveGenerationTest, CountGeneratedMoves) {
  Board board;
  init_eval_center();
  board.init_board(BoardConstants::DefaultFEN);
  bool isWhite = true;
  vector<Move> moves = generate_moves(board, isWhite);
  int count = moves.size();
  EXPECT_EQ(count, 104);
}



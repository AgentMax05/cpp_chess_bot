#include <gtest/gtest.h>
#include "board.h"
#include "eval.h"
#include <iostream>

using namespace std;

// Demonstrate some basic assertions.
TEST(EvalTest, DefaultBoardValue) {
  Board board;
  init_eval_center();
  board.init_board(BoardConstants::DefaultFEN);
  double value = value_board(board);
  EXPECT_EQ(value, 0);
}

TEST(EvalTest, OtherBoardValue) {
  Board board;
  init_eval_center();
  // Here, I just chose a random FEN - JAL
  board.init_board("r1b1k1nr/p2p1pNp/n2B4/1p1NP2P/6P1/3P1Q2/P1P1K3/q5b1");
  double value = value_board(board);
  ASSERT_NEAR(value, -9.6, 0.001);
}


#include <gtest/gtest.h>
#include "board.h"
#include "move_generation.h"
#include "eval.h"
#include "minimax.h"

Move minimax_test_get_board_move(string fen, bool useWhite, int depth) {
  Board board;
  init_eval_center();
  board.init_board(fen);
  Move move = best_move(board, useWhite, depth);
  return move;
}

TEST(MinimaxTest, MinimaxWhiteZeroDepth) {
  bool WHITE = true;
  Move move = minimax_test_get_board_move(BoardConstants::DefaultFEN, WHITE, 0);
  EXPECT_EQ(move.piece, 5);
  EXPECT_EQ(move.type, 0);
  for(size_t i=0; i<move.move.size(); i++) {
    if(i==40 || i==48) {
      EXPECT_EQ(move.move[i], 1);
    } else {
      EXPECT_EQ(move.move[i], 0);
    }
  }
}

TEST(MinimaxTest, MinimaxWhiteDepth1) {
  bool WHITE = true;
  Move move = minimax_test_get_board_move(BoardConstants::DefaultFEN, WHITE, 1);
  EXPECT_EQ(move.piece, 5);
  EXPECT_EQ(move.type, 0);
  for(size_t i=0; i<move.move.size(); i++) {
    if(i==40 || i==48) {
      EXPECT_EQ(move.move[i], 1);
    } else {
      EXPECT_EQ(move.move[i], 0);
    }
  }
}


TEST(MinimaxTest, MinimaxBlackZeroDepth) {
  bool BLACK = false;
  Move move = minimax_test_get_board_move(BoardConstants::DefaultFEN, BLACK, 0);
  EXPECT_EQ(move.piece, 5);
  EXPECT_EQ(move.type, 0);
  for(size_t i=0; i<move.move.size(); i++) {
    if(i==8 || i==16) {
      EXPECT_EQ(move.move[i], 1);
    } else {
      EXPECT_EQ(move.move[i], 0);
    }
  }
}

TEST(MinimaxTest, MinimaxBlackDepth1) {
  bool BLACK = false;
  Move move = minimax_test_get_board_move(BoardConstants::DefaultFEN, BLACK, 1);
  EXPECT_EQ(move.piece, 5);
  EXPECT_EQ(move.type, 0);
  for(size_t i=0; i<move.move.size(); i++) {
    if(i==8 || i==16) {
      EXPECT_EQ(move.move[i], 1);
    } else {
      EXPECT_EQ(move.move[i], 0);
    }
  }
}




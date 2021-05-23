#include <gtest/gtest.h>
#include "board.h"
#include "move_generation.h"
#include "eval.h"
#include "check_legal.h"
#include "set_attacking.h"

#include <vector>

// Demonstrate some basic assertions.
TEST(MoveGenerationTest, CountGeneratedMoves) {
  Board board;
  init_eval_center();
  board.init_board(BoardConstants::DefaultFEN);
  bool isWhite = true;
  vector<Move> moves = generate_moves(board, isWhite);
  size_t count = moves.size();
  EXPECT_EQ(count, 104);
}


int possible_moves(Board board, bool isWhite, int depth) {
  vector<Move> moves = generate_moves(board, isWhite);
  filter_moves(board, moves, isWhite);

  if (depth == 0) {

    int legal_moves = 0;

    for (int i = 0; i < moves.size(); i++) {
      board.make_move(moves[i].piece, moves[i].move, isWhite);
      set_attacking(board);

      if (!king_check(board, isWhite)) {
        legal_moves++;
      }
      // undo the move here
      board.make_move(moves[i].piece, moves[i].move, isWhite);
    }

    return moves.size();
  }

  int found_moves = 0;

  for (int i = 0; i < moves.size(); i++) {
    board.make_move(moves[i].piece, moves[i].move, isWhite);
    set_attacking(board);

    if (!king_check(board, isWhite)) {
      found_moves += possible_moves(board, isWhite == false, depth - 1);
    }
    // undo made move
    board.make_move(moves[i].piece, moves[i].move, isWhite);
  }

  return found_moves;

}


int moves_test(int depth) {
  Board board;
  board.init_board(BoardConstants::DefaultFEN);
  set_attacking(board);

  return possible_moves(board, true, depth);
}

// proven move counts taken from https://en.wikipedia.org/wiki/Shannon_number

// depth | move count
// 1     | 20
// 2     | 400
// 3     | 8,902
// 4     | 197,281
// 5     | 4,865,609
// 6     | 119,060,324

// Compare number of generated moves against proven number at each depth (depth 0 = depth 1 or 1 move for white)
TEST(MoveGenerationTest, CompareGeneratedMovesD1) {
  EXPECT_EQ(moves_test(0), 20);
}

TEST(MoveGenerationTest, CompareGeneratedMovesD2) {
  EXPECT_EQ(moves_test(1), 400);
}

TEST(MoveGenerationTest, DISABLED_CompareGeneratedMovesD3) {
  EXPECT_EQ(moves_test(2), 8902);
}

TEST(MoveGenerationTest, DISABLED_CompareGeneratedMovesD4) {
  EXPECT_EQ(moves_test(3), 197281);
}
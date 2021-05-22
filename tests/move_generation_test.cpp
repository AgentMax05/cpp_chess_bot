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


// Compare number of generated moves against proven number at each depth
TEST(MoveGenerationTest, CompareGeneratedMoves) {

  // proven number of moves at depth index (depth 0 is really depth 1, or 1 move for white)
  // proven move counts taken from https://en.wikipedia.org/wiki/Shannon_number
  vector<int> proven_number = {20, 400, 8902, 197281, 4865609, 119060324};

  Board board;
  board.init_board(BoardConstants::DefaultFEN);
  board.update_boards();
  set_attacking(board);

  for (int i = 0; i < 4; i++) {
    EXPECT_EQ(possible_moves(board, true, i), proven_number[i]);
  }
}

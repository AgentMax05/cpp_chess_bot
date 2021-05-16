#include <gtest/gtest.h>
#include "board.h"
#include "BoardRenderer.h"
#include "eval.h"
#include <iostream>

using namespace std;

// Demonstrate some basic assertions.
TEST(DisplayTest, DefaultBoard) {
  Board board;
  init_eval_center();
  board.init_board(BoardConstants::DefaultFEN);
  UnicodeBoardRenderer renderer;
  string result = renderer.render(&board);
  cout << result << "\n";
  EXPECT_EQ(result, "♜♞♝♛♚♝♞♜\n"
"♟︎♟︎♟︎♟︎♟︎♟︎♟︎♟︎\n"
"________\n"
"________\n"
"________\n"
"________\n"
"♙♙♙♙♙♙♙♙\n"
"♖♘♗♕♔♗♘♖");
}

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "board.h"
#include "move_generation.h"
#include "eval.h"
#include "check_legal.h"
#include "set_attacking.h"

#include <vector>

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

std::string get_all_white_moves();
std::string read_file(string filename);
std::string get_testdata_path(string filename);
Board get_default_board();
string save_moves(vector<Move>* moves, string filename);

const bool WHITE = true;

// Demonstrate some basic assertions.
TEST(MoveGenerationTest, CountGeneratedMoves) {
  vector<Move> moves = generate_moves(get_default_board(), WHITE);
  size_t count = moves.size();
  EXPECT_EQ(count, 104);
}

Board copy(Board board) {
  return board;
}

int possible_moves(Board board, bool isWhite, int depth) {
  vector<Move> moves = generate_moves(board, isWhite);
  filter_moves(board, moves, isWhite);

  if (depth == 0) {

    int legal_moves = 0;

    for (int i = 0; i < moves.size(); i++) {
      Board board_copy = copy(board);
      board_copy.make_move(moves[i].piece, moves[i].move, isWhite);
      set_attacking(board_copy);


      if (!king_check(board_copy, isWhite)) {
        legal_moves++;
      }
      // undo the move here
      // board.make_move(moves[i].piece, moves[i].move, isWhite);
    }

    return legal_moves;
  }

  int found_moves = 0;

  for (int i = 0; i < moves.size(); i++) {
    Board board_copy = copy(board);
    board_copy.make_move(moves[i].piece, moves[i].move, isWhite);
    set_attacking(board_copy);

    if (!king_check(board_copy, isWhite)) {
      found_moves += possible_moves(board_copy, isWhite == false, depth - 1);
    }
    // undo made move
    // board.make_move(moves[i].piece, moves[i].move, isWhite);
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

TEST(MoveGenerationTest, CompareGeneratedMovesD3) {
  EXPECT_EQ(moves_test(2), 8902);
}

TEST(MoveGenerationTest, CompareGeneratedMovesD4) {
  EXPECT_EQ(moves_test(3), 197281);
}

TEST(MoveGenerationTest, ListAllMovesWhite) {
    vector<Move> moves = generate_moves(get_default_board(), WHITE);
    string actual = save_moves(&moves, "result_list_all_moves_white.txt");
    string expected = read_file(get_testdata_path("all_white_moves_0.txt"));
    
    EXPECT_EQ(actual, expected);
}

TEST(MoveGenerationTest, ListAllMovesBlack) {
    vector<Move> moves = generate_moves(get_default_board(), WHITE);
    string actual = save_moves(&moves, "result_list_all_moves_black.txt");
    string expected = read_file(get_testdata_path("all_black_moves_0.txt"));
    
    EXPECT_EQ(actual, expected);
}

Board get_default_board() {
    Board board;
    init_eval_center();
    board.init_board(BoardConstants::DefaultFEN);
    return board;
}

string save_moves(vector<Move>* moves, string filename) {
    std::stringstream ss;
    for(Move move: *moves) {
        ss << move_to_string(&move) << std::endl;
    }
    
    string actual = ss.str();
    
    std::ofstream outfile;
    outfile.open(filename);
    outfile << actual;
    outfile.close();
    
    return actual;
}

std::string get_testdata_path(string filename) {
    fs::path test_data_path = fs::path("..") / "tests" / "data" / filename;
    return test_data_path.string();
}

std::string read_file(string filename) {
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    
    return str;
}


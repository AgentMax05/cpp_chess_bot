#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include "board.h"
#include "move_generation.h"
#include "minimax.h"
#include "check_legal.h"
#include "eval.h"
#include "set_attacking.h"

typedef std::bitset<64> Bitboard;
using std::cout;
using std::cin;
using std::string;

void display_board(Bitboard board);

int main() {

    string fen_notation = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    string input;
    cout << "Enter FEN notation ([default] for default): ";
    cin >> input;

    if (input != "default") {fen_notation = input;}

    // initialize board object and set attackW and attackB boards
    Board board;
    init_eval_center();
    board.init_board(fen_notation);
    set_attacking(board);

    // display the representation of the board we loaded
    std::cerr << board.getDisplayString() << "\n";

    // search for best move with given depth
    Move move = best_move(board, true, 8);
    display_board(move.move);
    
    // cout << "\n" << move.piece;
    
    // now make that move so we can display the updated board
    board.make_move(move.piece, move.move, true);
    std::cerr << "\n" << board.getDisplayString() << "\n";
    
}
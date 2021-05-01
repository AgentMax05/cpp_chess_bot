#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include "board.h"
#include "move_generation.h"
#include "minimax.h"

typedef std::bitset<64> Bitboard;
using std::cout;
using std::cin;
using std::string;

void display_board(Bitboard board);

int main() {
    // string fen_notation = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR";
    string fen_notation = "8/8/7p/3KNN1k/2p4p/8/3P2p1/8";
    // string fen_notation = "7k/pp6/8/8/8/8/5PP1/2K5";
    string input;
    cout << "Enter FEN notation ([default] for default): ";
    cin >> input;

    if (input != "default") {fen_notation = input;}

    Board board;
    board.init_board(fen_notation);

    Bitboard move = best_move(board, true, 4);
    display_board(move);

    // std::vector<Move> moves = generate_moves(board, false);
    // for (int i = 0; i < moves.size(); i++) {
    //     if (moves[i].piece != 4) {continue;}
    //     display_board(moves[i].legal_check);
    //     cout << "\n";
    //     display_board(moves[i].move);
    //     std::cout << "\n-------------------------\n";
    // }
    
    // display_board(board.rookB);

}
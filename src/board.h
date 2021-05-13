#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <bitset>

typedef std::bitset<64> Bitboard;
using std::string;

class Board {

    public:

    Bitboard pawnW, rookW, bishopW, knightW, queenW, kingW;
    Bitboard pawnB, rookB, bishopB, knightB, queenB, kingB;
    Bitboard boardW, boardB, complete_board;
    Bitboard attackW, attackB;

    void init_board(string FEN);
    void update_boards();
    void make_move(int piece, Bitboard move, bool White);

    string getDisplayString();
};

struct Move {
    int piece;
    int type = 0;
    Bitboard move;
    Bitboard legal_check;
};

#if !defined(BOARD_CONSTANTS_H)
#define BOARD_CONSTANTS_H

namespace BoardConstants {
    const string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
}


#endif 

#endif

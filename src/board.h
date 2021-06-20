#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include <bitset>
#include <iostream>

typedef std::bitset<64> Bitboard;
using std::string;
using std::vector;
using std::cout;

class Board {

    public:

    Bitboard pawnW, rookW, bishopW, knightW, queenW, kingW;
    Bitboard pawnB, rookB, bishopB, knightB, queenB, kingB;
    Bitboard boardW, boardB, complete_board;
    Bitboard attackW, attackB;
    // epPawns stores previous pawn structure to check for en passant
    Bitboard epPawns; // ep = en passant

    // bools for whether rook has moved or not
    bool lCastleW = false;
    bool rCastleW = false;
    bool lCastleB = false;
    bool rCastleB = false;

    // Bitboard repesentations of spaces in between king and rook
    Bitboard lCastleWBoard = Bitboard("0000111000000000000000000000000000000000000000000000000000000000");
    Bitboard rCastleWBoard = Bitboard("0110000000000000000000000000000000000000000000000000000000000000");
    Bitboard lCastleBBoard = Bitboard("0000000000000000000000000000000000000000000000000000000000001110");
    Bitboard rCastleBBoard = Bitboard("0000000000000000000000000000000000000000000000000000000001100000");

    // index 0: king move
    // index 1: rook move
    vector<Bitboard> lCastleWMove = {
        Bitboard("0001010000000000000000000000000000000000000000000000000000000000"),
        Bitboard("0000100100000000000000000000000000000000000000000000000000000000")
    };

    vector<Bitboard> rCastleWMove = {
        Bitboard("0101000000000000000000000000000000000000000000000000000000000000"),
        Bitboard("1010000000000000000000000000000000000000000000000000000000000000")
    };

    vector<Bitboard> lCastleBMove = {
        Bitboard("0000000000000000000000000000000000000000000000000000000000010100"),
        Bitboard("0000000000000000000000000000000000000000000000000000000000001001")
    };

    vector<Bitboard> rCastleBMove = {
        Bitboard("0000000000000000000000000000000000000000000000000000000001010000"),
        Bitboard("0000000000000000000000000000000000000000000000000000000010100000")
    };

    void init_board(string FEN);
    void update_boards();  
    void update_epPawns();
    void make_move(int piece, Bitboard move, bool White, int type = 0);  // REGULAR_MOVE is 0
};

struct Move {
    int piece;
    int type = 0;
    Bitboard move;
    Bitboard legal_check;
};

string move_to_string(Move* move);

#if !defined(BOARD_CONSTANTS_H)
#define BOARD_CONSTANTS_H

namespace BoardConstants {
    const string DefaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
}


#endif 

#endif

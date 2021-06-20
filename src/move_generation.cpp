#include <vector>
#include <bitset>
#include <cmath>
#include <iostream>

#include "board.h"
#include "move_generation.h"
#include "database.h"

/*
 *  Bitboard Numbering:
 *  RANK | LBIT    -    RBIT
 *    8     0 > 00000000 <7
 *    7     8 > 00000000 <15
 *    6     16> 00000000 <23
 *    5     24> 00000000 <31
 *    4     32> 00000000 <39
 *    3     40> 00000000 <47
 *    2     48> 00000000 <55
 *    1     56> 00000000 <63
 *    FILE      ABCDEFGH
 * 
 *  Right shift (>>) moves pieces up
 *  Left shift (<<) moves pieces down
*/

size_t add_move_generic(vector<Move>* moves, int i, int x, int piece, int type);

typedef std::bitset<64> Bitboard;
using std::cout;
using std::vector;

vector<Move> add_move_board(int sq, Bitboard move_board, int piece, int type= REGULAR_MOVE) {
    vector<Move> moves = {};
    for (int i = 0; i < move_board.size(); i++) {
        if (move_board[i]) {
            add_move_generic(&moves, sq, i, piece, type);            
        }
    }
    return moves;
}

bool in_board(int row) {
    if (row >= 0 && row <= 7) {
        return true;
    }
    return false;
}

int get_row(int index) {
    if (index < 0) {return -1;}
    return std::trunc(index / 8);
}

/**
 Returns the index of the move just added.
 */
size_t add_move_generic(vector<Move>* moves, int i, int x, int piece, int type) {
    Bitboard move_board;
    move_board[i] = move_board[x] = 1;
    Move move;
    move.move = move_board;
    move.piece = piece;
    move.type = type;
    moves->push_back(move);

    return moves->size() - 1;
}

void generate_moves_for_pawn(vector<Move>* moves, Board board, bool White) {
    Bitboard pawns = White ? board.pawnW : board.pawnB;
    Bitboard enemy = White ? board.boardB : board.boardW;
    vector<vector<Bitboard>>& pawn_db = White ? pawn_dbW : pawn_dbB;

    for (int i = 0; i < pawns.size(); i++) {
        if (pawns[i]) {
            Bitboard regular = pawn_db[i][0] & ~board.complete_board;
            Bitboard takes = pawn_db[i][2] & enemy;
            Bitboard double_move = pawn_db[i][1] & ~board.complete_board;
            Bitboard enPass = pawn_db[i][3];

            if (White) {
                double_move = double_move & ~((board.complete_board & RANK3) >> 8);
                enPass = enPass & (((board.pawnB & RANK5) & ((board.epPawns & RANK7) << 16)) >> 8);

            } else {
                double_move = double_move & ~((board.complete_board & RANK6) << 8);
                enPass = enPass & (((board.pawnB & RANK4) & ((board.epPawns & RANK2) >> 16)) << 8);
            }
            vector<Move> regularM = add_move_board(i, regular, pPawn);
            vector<Move> doubleM = add_move_board(i, double_move, pPawn, PAWN_DOUBLE);
            vector<Move> takesM = add_move_board(i, takes, pPawn, PAWN_EAT);
            vector<Move> enPassM = add_move_board(i, enPass, pPawn, PAWN_ENPASSANT);

            moves->insert(moves->end(), regularM.begin(), regularM.end());
            moves->insert(moves->end(), doubleM.begin(), doubleM.end());
            moves->insert(moves->end(), takesM.begin(), takesM.end());
            moves->insert(moves->end(), enPassM.begin(), enPassM.end());
        }
    }
}

void generate_moves_for_knights(vector<Move>* moves, Board board, bool White) {
    Bitboard knight = White ? board.knightW : board.knightB;
    Bitboard friendly = White ? board.boardW : board.boardB;

    for (int i = 0; i < knight.size(); i++) {
        if (knight[i]) {
            Bitboard move_board = knight_db[i] & ~friendly;
            vector<Move> new_moves = add_move_board(i, move_board, pKnight);
            moves->insert(moves->end(), new_moves.begin(), new_moves.end());
        }
    }
}

void generate_moves_for_king(vector<Move>* moves, Board board, bool White) {
    Bitboard king = White ? board.kingW : board.kingB;
    Bitboard friendly = White ? board.boardW : board.boardB;

    for (int i = 0; i < king.size(); i++) {
        if (king[i]) {
            Bitboard move_board = king_db[i] & ~friendly;
            vector<Move> new_moves = add_move_board(i, move_board, pKing);
            moves->insert(moves->end(), new_moves.begin(), new_moves.end());
        }
    }   
}

void generate_moves_for_rook(vector<Move>* moves, Board board, bool White) {
    Bitboard rook = White ? board.rookW : board.rookB;

    // get moves from horizontal db
    for (int i = 0; i < rook.size(); i++) {
        if (rook[i] == 1) {
            Bitboard moves_board = rook_db[i][get_index(i, pRook, rook_masks[i] & board.complete_board)] & ~(White ? board.boardW : board.boardB);
            vector<Move> new_moves = add_move_board(i, moves_board, pRook);
            moves->insert(moves->end(), new_moves.begin(), new_moves.end());
        }
    }
}

void generate_moves_for_bishop(vector<Move>* moves, Board board, bool White) {
    Bitboard bishop = White ? board.bishopW : board.bishopB;

    // get moves from diagnol db
    for (int i = 0; i < bishop.size(); i++) {
        if (bishop[i] == 1) {
            Bitboard moves_board = bishop_db[i][get_index(i, pBishop, bishop_masks[i] & board.complete_board)] & ~(White ? board.boardW : board.boardB);
            vector<Move> new_moves = add_move_board(i, moves_board, pBishop);
            moves->insert(moves->end(), new_moves.begin(), new_moves.end());
        }
    }
}

void generate_moves_for_queen(vector<Move>* moves, Board board, bool White) {
    Bitboard queen = White ? board.queenW : board.queenB;
    for (int i = 0; i < queen.size(); i++) {
        if (queen[i] == 1) {
            Bitboard moves_board = (rook_db[i][get_index(i, pRook, rook_masks[i] & board.complete_board)] | bishop_db[i][get_index(i, pBishop, bishop_masks[i] & board.complete_board)]) & ~(White ? board.boardW : board.boardB);
            vector<Move> new_moves = add_move_board(i, moves_board, pQueen);
            moves->insert(moves->end(), new_moves.begin(), new_moves.end());
        }
    }
}

void generate_castles(vector<Move>* moves, Board board, bool White) {
    if (White) {
        if (board.lCastleW) {
            if ((board.complete_board & board.lCastleWBoard).none()) {
                if ((board.attackB & board.lCastleWBoard).none()) {
                    Move new_move;
                    new_move.type = L_CASTLE;
                    new_move.piece = pRook;
                    moves->push_back(new_move);
                }
            }
        }
        if (board.rCastleW) {
            if ((board.complete_board & board.rCastleWBoard).none()) {
                if ((board.attackB & board.rCastleWBoard).none()) {
                    Move new_move;
                    new_move.type = R_CASTLE;
                    new_move.piece = pRook;
                    moves->push_back(new_move);
                }
            }
        }
    } else {
        if (board.lCastleB) {
            if ((board.complete_board & board.lCastleBBoard).none()) {
                if ((board.attackW & board.lCastleBBoard).none()) {
                    Move new_move;
                    new_move.type = L_CASTLE;
                    new_move.piece = pRook;
                    moves->push_back(new_move);
                }
            }
        }
        if (board.rCastleB) {
            if ((board.complete_board & board.rCastleBBoard).none()) {
                if ((board.attackW & board.rCastleBBoard).none()) {
                    Move new_move;
                    new_move.type = R_CASTLE;
                    new_move.piece = pRook;
                    moves->push_back(new_move);
                }
            }
        }
    }
}

// generate all moves for certain side
vector<Move> generate_moves(Board board, bool White) {
    vector<Move> moves = {};

    generate_moves_for_pawn(&moves, board, White);
    generate_moves_for_knights(&moves, board, White);
    generate_moves_for_king(&moves, board, White);
    generate_moves_for_rook(&moves, board, White);
    generate_moves_for_bishop(&moves, board, White);
    generate_moves_for_queen(&moves, board, White);
    generate_castles(&moves, board, White);

    return moves;
}

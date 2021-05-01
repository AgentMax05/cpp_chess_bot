#include "board.h"
#include "move_generation.h"
#include "set_attacking.h"

bool check_legal(Board board, Move move, bool White) {
    Bitboard friendly_board, enemy_board;
    if (White) {
        friendly_board = board.boardW;
        enemy_board = board.boardB;
    } else {
        friendly_board = board.boardB;
        enemy_board = board.boardW;
    }

    // check for moving to occupied square
    Bitboard overlap = friendly_board & move.move;
    if (overlap.count() != 1) {return false;}

    // check pawn moves
    if (move.piece == pPawn) {

        // check if taking is legal
        if (move.type == PAWN_EAT) {
            if ((enemy_board & move.move).count() != 1) {return false;}
        }
    }

    // no need to check knight moves


    // check rook moves
    if (move.piece == pRook) {
        Bitboard sliding_legal = (enemy_board | friendly_board) & move.legal_check;
        if (!sliding_legal.none()) {return false;}
    }

    // check bishop moves
    if (move.piece == pBishop) {
        Bitboard diagnol_legal = (enemy_board | friendly_board) & move.legal_check;
        if (!diagnol_legal.none()) {return false;}
    }

    if (move.piece == pQueen) {
        Bitboard queen_legal = (enemy_board | friendly_board) & move.legal_check;
        if (!queen_legal.none()) {return false;}
    }

    // check if king is in check after move:
    set_attacking(board, move, White == false);

    if (White) {
        if ((board.kingW & board.attackB).none() == false) {return false;}
    } else {
        if ((board.kingB & board.attackW).none() == false) {return false;}
    }

    return true;

}

void filter_moves(Board board, vector<Move> &moves, bool White) {
    vector<Move> filtered = {};
    for (int i = 0; i < moves.size(); i++) {
        if (check_legal(board, moves[i], White)) {
            filtered.push_back(moves[i]);
        }
    }
    moves = filtered;
}
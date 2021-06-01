#include "board.h"
#include "move_generation.h"
#include "set_attacking.h"

bool king_check(Board board, bool isWhite) {
    if (isWhite) {
        if ((board.kingW & board.attackB).none() == false) {return true;}
    } else {
        if ((board.kingB & board.attackW).none() == false) {return true;}
    }
    return false;
}

bool check_legal(Board board, Move move, bool White) {
    Bitboard friendly_board, enemy_board;
    if (White) {
        friendly_board = board.boardW;
        enemy_board = board.boardB;
    } else {
        friendly_board = board.boardB;
        enemy_board = board.boardW;
    }
    
    /*
        Requirements for castling:
        1. King and rook have not previously moved
        2. King cannot move through check
        3. King cannot be in check
        4. King does not end up in check (as for any other move, checked in minimax.cpp)
        5. No pieces between the king and the rook
    */
    // check castling moves
    if (move.piece == pNone && (move.type == L_CASTLE || move.type == R_CASTLE)) {
        if (White) {
            if (king_check(board, true)) {return false;}

            if (move.type == L_CASTLE) {
                if (board.lCastleW == false) {return false;}
            }
            else if (move.type == R_CASTLE) {
                if (board.rCastleW == false) {return false;}
            }
            if (!(board.complete_board & move.legal_check).none()) {return false;}
            if (!(board.attackB & move.legal_check).none()) {return false;}
            return true;
        } 
        
        else {
            if (king_check(board, false)) {return false;}

            if (move.type == L_CASTLE) {
                if (board.lCastleB == false) {return false;}
            }
            else if (move.type == R_CASTLE) {
                if (board.rCastleB == false) {return false;}
            }
            if (!(board.complete_board & move.legal_check).none()) {return false;}
            if (!(board.attackW & move.legal_check).none()) {return false;}
            return true;
        }
    }


    // check for moving to occupied square
    Bitboard overlap = friendly_board & move.move;

    // if piece is a pawn and not taking make sure pawn can't take forwards
    if (move.piece == pPawn && move.type != PAWN_EAT) {
        overlap = (friendly_board | enemy_board) & move.move;
    }

    if (overlap.count() != 1) {return false;}

    // check pawn moves
    if (move.piece == pPawn) {
        
        // check en passant moves
        /*
            Requirements for en passant:
            1. Captured pawn must be on adjacent file and have moved up 2 in one move
            2. En passant must be made next move after captured pawn moves up 2
        */
        if (move.type == PAWN_ENPASSANT) {
            if (White) {
                // bitboard representing black pawn that is being taken
                Bitboard taken = (move.move ^ board.pawnW) << 8;
                // make sure taken pawn is there
                if ((board.pawnB & taken).none()) {
                    return false;
                }
                // check if pawn was there previous move
                if (!(board.epPawns & taken).none()) {
                    return false;
                } 
                // check if pawn moved up 2
                if ((board.epPawns & (taken >> 16)).none()) {
                    return false;
                }
            } else {
                // bitboard representing white pawn that is being taken
                Bitboard taken = (move.move ^ board.pawnB) >> 8;
                // make sure taken pawn exists
                if ((board.pawnW & taken).none()) {
                    return false;
                }
                // check if pawn was there previous move
                if ((board.epPawns & taken).none() == false) {
                    return false;
                }
                // check if pawn moved up 2
                if ((board.epPawns & (taken << 16)).none()) {
                    return false;
                }
            }
            return true;
        }

        // check if pawn can move up two
        if (move.type == PAWN_DOUBLE) {
            if (!((friendly_board | enemy_board) & move.legal_check).none()) {return false;}
        }

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
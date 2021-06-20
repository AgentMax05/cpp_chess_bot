#include "board.h"
#include "move_generation.h"
#include "set_attacking.h"
#include "database.h"

// sets attackW and attackB boards
void set_attacking(Board &board) {
    Bitboard pawn, rook, knight, bishop, queen, king, attacking, full_board, friendly_board, enemy_board;
    int pawn_factor;
    full_board = board.complete_board;

    board.attackW.reset();
    board.attackB.reset();

    // cycles through 0 and 1, 0 being black (false), 1 being white (true)
    for (int i = 0; i < 2; i++) {
        bool White = i;

        if (White) {
            pawn =  board.pawnW;
            rook = board.rookW;
            knight = board.knightW;
            bishop = board.bishopW;
            queen = board.queenW;
            king = board.kingW;
            attacking = board.attackW;
            pawn_factor = -8;

            friendly_board = board.boardW;
            enemy_board = board.boardB;
        }

        else {
            pawn = board.pawnB;
            rook = board.rookB;
            knight = board.knightB;
            bishop = board.bishopB;
            queen = board.queenB;
            king = board.kingB;
            attacking = board.attackB;
            pawn_factor = 8;

            friendly_board = board.boardB;
            enemy_board = board.boardW;
        }


        // set attacking for pawns
        for (int i = 0; i < pawn.size(); i++) {
            if (pawn[i]) {
                attacking = attacking | (White ? pawn_dbW : pawn_dbB)[i][2];
            }
        }

        // set attacking for knights
        for (int i = 0; i < knight.size(); i++) {
            if (knight[i]) {
                attacking = attacking | knight_db[i];
            }
        }

        // set attacking for rooks
        for (int i = 0; i < rook.size(); i++) {
            if (rook[i]) {
                uint64_t index = get_index(i, pRook, rook_masks[i] & board.complete_board);
                attacking = attacking | rook_db[i][index];
            }
        }

        // set attacking for bishops
        for (int i = 0; i < bishop.size(); i++) {
            if (bishop[i]) {
                uint64_t index = get_index(i, pBishop, bishop_masks[i] & board.complete_board);
                attacking = attacking | bishop_db[i][index];
            }
        }

        // set attacking for queens
        for (int i = 0; i < queen.size(); i++) {
            if (queen[i]) {
                uint64_t indexR = get_index(i, pRook, rook_masks[i] & board.complete_board);
                uint64_t indexB = get_index(i, pBishop, bishop_masks[i] & board.complete_board);
                attacking = attacking | rook_db[i][indexR] | bishop_db[i][indexB];
            }
        }

        // set attacking for king
        for (int i = 0; i < king.size(); i++) {
            if (king[i]) {
                attacking = attacking | king_db[i];
            }
        }

        if (White) {
            board.attackW = attacking;
        } else {
            board.attackB = attacking;
        }
    }
}
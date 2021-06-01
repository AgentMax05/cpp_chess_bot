#include "board.h"
#include "eval.h"
#include "move_generation.h"
#include "minimax.h"
#include "check_legal.h"
#include "set_attacking.h"
#include <float.h>

int nodes = 0;

double CHECKMATE = DBL_MAX;
double STALEMATE = 0;

double minimax(Board board, Move move, bool White, int depth, double alpha, double beta) {
    nodes++;
    // make move:
    board.make_move(move.piece, move.move, White);
    set_attacking(board);

    // check if king is in check (check is here instead of check_legal to improve performance):
    if (king_check(board, White)) {
        if (White) {
            return alpha;
        }
        return beta;
    }

    // set castling bool to false if rook is moved
    if (move.piece == pRook) {
        if (White) {
            if (board.rookW[56] != 1 && board.lCastleW == true) {board.lCastleW = false;}
            if (board.rookW[63] != 1 && board.rCastleW == true) {board.rCastleW = false;}
        } else {
            if (board.rookB[0] != 1 && board.lCastleB == true) {board.lCastleB = false;}
            if (board.rookB[7] != 1 && board.rCastleB == true) {board.rCastleB = false;}
        }
    }

    // set castling bools to false if king is moved
    else if (move.piece == pKing) {
        if (White) {
            board.lCastleW = false;
            board.rCastleW = false;
        } else {
            board.lCastleB = false;
            board.rCastleB = false;
        }
    }

    // set castling bools to false if current move is a castle
    else if (move.piece == pNone && (move.type == L_CASTLE || move.type == R_CASTLE)) {
        if (White) {
            board.lCastleW = false;
            board.rCastleW = false;
        } else {
            board.lCastleB = false;
            board.rCastleB = false;
        }
    }

    if (depth <= 0) {
        return value_board(board);
    }

    // generate moves for other side (White == false)
    vector<Move> moves = generate_moves(board, White == false);
    filter_moves(board, moves, White == false);

    // check if king is in checkmate or stalemate
    if (moves.size() == 0) {
        if (king_check(board, White == false)) {
            if (White) {
                // return CHECKMATE for white meaning black has no legal moves
                return CHECKMATE;
            } else {
                // return CHECKMATE for black meaning white has no legal moves
                return (-CHECKMATE);
            }
        } else {
            // if other side has no moves but is not in check, return STALEMATE (draw)
            return STALEMATE;
        }
    }

    if (White) {
        double highest = -DBL_MAX;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], White == false, depth - 1, alpha, beta);
            if (score > highest) {
                highest = score;
                best_move = moves[i];
            }
            if (score > alpha) {
                alpha = score;
            }
            if (beta <= alpha) {
                return highest;
            }
        }
        return highest;

    } else {
        double lowest = DBL_MAX;
        Move best_move;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], White == false, depth - 1, alpha, beta);
            if (score < lowest) {
                lowest = score;
                best_move = moves[i];
            }
            if (lowest < beta) {
                beta = lowest;
            }
            if (beta <= alpha) {
                return lowest;
            }
        }
        return lowest;
    }
}


Move best_move(Board board, bool White, int depth) {
    vector<Move> moves = generate_moves(board, White);
    filter_moves(board, moves, White);

    nodes = 0;

    if (White) {
        double highest = -DBL_MAX;
        Move best;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], true, depth - 1, -DBL_MAX, DBL_MAX);
            if (score > highest) {
                highest = score;
                best = moves[i];
            }
        }
        cout << "\nnodes searched: " << nodes;
        return best;

    } else {
        double lowest = DBL_MAX;
        Move best;

        for (int i = 0; i < moves.size(); i++) {
            double score = minimax(board, moves[i], false, depth - 1, -DBL_MAX, DBL_MAX);
            if (score < lowest) {
                lowest = score;
                best = moves[i];
            }
        }
        cout << "\nnodes searched: " << nodes;
        return best;
    }
}

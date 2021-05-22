#include <vector>
#include <bitset>
#include <cmath>
#include <iostream>

#include "board.h"
#include "move_generation.h"

typedef std::bitset<64> Bitboard;
using std::cout;
using std::vector;

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

void generate_moves_for_pawn(vector<Move>* moves, Board board, bool White) {
    if (White) {
        // create all moves for pawns
        for (int i = 0; i < board.pawnW.size(); i++) {
            int current = board.pawnW[i];
            if (current == 1) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board = move_board >> 8;
                move_board[i] = 1;

                new_move.move = move_board;
                new_move.piece = pPawn;
                moves->push_back(new_move);

                // check if pawn can move up 2
                if (48 <= i && i <= 55) {
                    Move new_double;
                    Bitboard double_move;
                    double_move[i] = 1;
                    double_move = double_move >> 16;
                    double_move[i] = 1;
                    
                    Bitboard legal_check;
                    legal_check[i - 8] = 1;

                    new_double.move = double_move;
                    new_double.piece = pPawn;
                    new_double.type = PAWN_DOUBLE;
                    new_double.legal_check = legal_check;
                    moves->push_back(new_double);
                }

                bool space_left = get_row(i - 1) == get_row(i);
                bool space_right = get_row(i + 1) == get_row(i);
                bool space_above = in_board(get_row(i - 8));

                // eat up left
                if (space_left && space_above) {
                    Move m_eat_left;
                    m_eat_left.type = PAWN_EAT;
                    Bitboard eat_left;
                    eat_left[i] = 1;
                    eat_left[i - 8 - 1] = 1;

                    m_eat_left.move = eat_left;
                    m_eat_left.piece = pPawn;
                    moves->push_back(m_eat_left);
                }

                // eat up right
                if (space_right && space_above) {
                    Move m_eat_right;
                    m_eat_right.type = PAWN_EAT;
                    Bitboard eat_right;
                    eat_right[i] = 1;
                    eat_right[i - 8 + 1] = 1;

                    m_eat_right.move = eat_right;
                    m_eat_right.piece = pPawn;
                    moves->push_back(m_eat_right);
                }

            }
        }
    
    } else {
        // create all moves for black pawns
        for (int i = 0; i < board.pawnB.size(); i++) {
            int current = board.pawnB[i];
            if (current == 1) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 8] = 1;

                new_move.move = move_board;
                new_move.piece = pPawn;
                moves->push_back(new_move);

                // move down 2
                if (get_row(i) == 1) {
                    Move new_double;
                    Bitboard double_move;
                    double_move[i] = 1;
                    double_move[i + 16] = 1;

                    Bitboard legal_check;
                    legal_check[i + 8] = 1;

                    new_double.move = double_move;
                    new_double.piece = pPawn;
                    new_double.type = PAWN_DOUBLE;
                    new_double.legal_check = legal_check;
                    moves->push_back(new_double);
                }

                bool space_left = get_row(i - 1) == get_row(i);
                bool space_right = get_row(i + 1) == get_row(i);
                bool space_below = in_board(get_row(i + 8));

                // eat left
                if (space_left && space_below) {
                    Move m_eat_left;
                    Bitboard eat_left;
                    eat_left[i] = 1;
                    eat_left[i + 8 - 1] = 1;
                    m_eat_left.type = PAWN_EAT;

                    m_eat_left.move = eat_left;
                    m_eat_left.piece = pPawn;
                    moves->push_back(m_eat_left);
                }

                // eat right
                if (space_right && space_below) {
                    Move m_eat_right;
                    Bitboard eat_right;
                    eat_right[i] = 1;
                    eat_right[i + 8 + 1] = 1;
                    m_eat_right.type = PAWN_EAT;

                    m_eat_right.move = eat_right;
                    m_eat_right.piece = pPawn;
                    moves->push_back(m_eat_right);
                }

            }
        }
    
    }
}

void generate_moves_for_knights(vector<Move>* moves, Board board, bool White) {
    Bitboard knight = White ? board.knightW : board.knightB;

    // create all moves for knights
    for (int i = 0; i < knight.size(); i++) {
        int current = knight[i];
        if (current == 1) {
            int row = get_row(i);

            bool space_left = get_row(i - 2) == row;
            bool space_right = get_row(i + 2) == row;
            bool space_above = 0 <= get_row(i - 8) && get_row(i - 8) <= 7;
            bool space_below = 0 <= get_row(i + 8) && get_row(i + 8) <= 7;
            
            bool single_space_left = get_row(i - 1) == row;
            bool single_space_right = get_row(i + 1) == row;
            bool double_space_above = 0 <= get_row(i - 16) && get_row(i - 16) <= 7;
            bool double_space_below = 0 <= get_row(i + 16) && get_row(i + 16) <= 7;

            // cout << "left" << space_left << " " << "right " << space_right << " " <<  "above " << space_above<< " " << "below " << space_below << " " << "sleft " << single_space_left<< " "  << "sright " << single_space_right<< " " << "dabove " << double_space_above << " " << "dbelow " << double_space_below << "end";
            
            // move left 2 down 1
            if (space_left && space_below) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 2 + 8] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

            // move left 2 up 1
            if (space_left && space_above) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 2 - 8] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

            // move right 2 down 1
            if (space_right && space_below) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 2 + 8] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

            // move right 2 up 1
            if (space_right && space_above) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 2 - 8] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }


            // move up 2 left 1
            if (double_space_above && single_space_left) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 1 - 16] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

            // move up 2 right 1
            if (double_space_above && single_space_right) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 1 - 16] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

            // move down 2 left 1
            if (double_space_below && single_space_left) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 1 + 16] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

            // move down 2 right 1
            if (double_space_below && single_space_right) {
                Move new_move;
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 1 + 16] = 1;
                new_move.move = move_board;
                new_move.piece = pKnight;
                moves->push_back(new_move);
            }

        }
    }
}

void generate_moves_for_king(vector<Move>* moves, Board board, bool White) {
    Bitboard king = White ? board.kingW : board.kingB;
    // create all moves for king
    for (int i = 0; i < king.size(); i++) {
        int current = king[i];
        if (current == 1) {
            int row = get_row(i);

            bool space_above = 0 <= get_row(i - 8) && get_row(i - 8) <= 7;
            bool space_below = 0 <= get_row(i + 8) && get_row(i + 8) <= 7;
            bool space_left = get_row(i - 1) == row;
            bool space_right = get_row(i + 1) == row;

            // move up
            if (space_above) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 8] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }


            // move down
            if (space_below) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 8] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

            // move left
            if (space_left) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 1] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

            // move right
            if (space_right) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 1] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

            // move up left
            if (space_left && space_above) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 8 - 1] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

            // move up right
            if (space_right && space_above) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i - 8 + 1] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

            // move down left
            if (space_left && space_below) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 8 - 1] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

            // move down right
            if (space_right && space_below) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[i + 8 + 1] = 1;
                Move new_move;
                new_move.move = move_board;
                new_move.piece = pKing;
                moves->push_back(new_move);
            }

        }
    }
}

// generate all moves for certain side
vector<Move> generate_moves(Board board, bool White) {
    vector<Move> moves = {};

    // setting boards based on color

    generate_moves_for_pawn(&moves, board, White);
    generate_moves_for_knights(&moves, board, White);
    generate_moves_for_king(&moves, board, White);

    Bitboard rook = White ? board.rookW : board.rookB;
    // create all moves for rook
    for (int i = 0; i < rook.size(); i++) {
        int current = rook[i];
        if (current == 1) {
            int x = i % 8;

            // vertical moves:
            Bitboard legal_check;

            for (int z = 0; z < 8; z++) {
                if (x == i) {x += 8; continue;}
                Bitboard move_board;
                move_board[i] = 1;
                move_board[x] = 1;

                // set move check bitboard:
                int factor;
                if (i < x) {factor = 8;} else {factor = -8;}
                int check = i + factor;
                for (int counter =  0; counter < abs(i - x) / 8; counter++) {
                    if (check == i || check == x) {check += factor; continue;}
                    legal_check[check] = 1;
                    check += factor;
                }

                Move new_move;
                new_move.move = move_board;
                new_move.piece = pRook;
                new_move.legal_check = legal_check;
                moves.push_back(new_move);
                legal_check.reset();
                x += 8;
            }

            // horizontal moves:
            legal_check.reset();

            x = std::trunc(i / 8) * 8;
            for (int z = 0; z < 8; z++) {
                if (x == i) {x++; continue;}
                Bitboard move_board;
                move_board[i] = 1;
                move_board[x] = 1;

                // set move check bitboard:
                int factor;
                if (i < x) {factor = 1;} else {factor = -1;}
                int check = i + factor;
                for (int counter = 0; counter < abs(i - x); counter++) {
                    if (check == i || check == x) {check += factor; continue;}
                    legal_check[check] = 1;
                    check += factor;
                }

                Move new_move;
                new_move.move = move_board;
                new_move.piece = pRook;
                new_move.legal_check = legal_check;
                moves.push_back(new_move);
                legal_check.reset();
                x++;
            }
        }
    }

    Bitboard bishop = White ? board.bishopW : board.bishopB;
    // create all moves for bishop
    for (int i = 0; i < bishop.size(); i++) {
        int current = bishop[i];
        if (current == 1) {
            
            Bitboard legal_check;

            // moving up right
            int row = get_row(i);
            int x = i + 1;
            int y = i - 8 + 1;
            while (get_row(x) == row && get_row(y) >= 0) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[y] = 1;

                if ((y + 8 - 1) != i) {
                    legal_check[y + 8 - 1] = 1;
                }

                Move new_move;
                new_move.move = move_board;
                new_move.piece = pBishop;
                new_move.legal_check = legal_check;

                moves.push_back(new_move);
                x++;
                y = y - 8 + 1;
            }

            // moving up left
            legal_check.reset();

            x = i - 1;
            y = i - 8 - 1;
            while(get_row(x) == row && get_row(y) >= 0) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[y] = 1;

                if ((y + 8 + 1) != i) {
                    legal_check[y + 8 + 1] = 1;
                }

                Move new_move;
                new_move.move = move_board;
                new_move.piece = pBishop;
                new_move.legal_check = legal_check;

                moves.push_back(new_move);
                x--;
                y = y - 8 - 1;
            }

            // moving down right
            legal_check.reset();

            x = i + 1;
            y = i + 8 + 1;
            while (get_row(x) == row && get_row(y) <= 7) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[y] = 1;

                if ((y - 8 - 1) != i) {
                    legal_check[y - 8 - 1] = 1;
                }

                Move new_move;
                new_move.move = move_board;
                new_move.piece = pBishop;
                new_move.legal_check = legal_check;

                moves.push_back(new_move);
                x ++;
                y = y + 8 + 1;
            }

            // moving down left
            legal_check.reset();

            x = i - 1;
            y = i + 8 - 1;
            while (get_row(x) == row && get_row(y) <= 7) {
                Bitboard move_board;
                move_board[i] = 1;
                move_board[y] = 1;

                if ((y - 8 + 1) != i) {
                    legal_check[y - 8 + 1] = 1;
                }

                Move new_move;
                new_move.move = move_board;
                new_move.piece = pBishop;
                new_move.legal_check = legal_check;

                moves.push_back(new_move);
                x --;
                y = y + 8 - 1;
            }

        }
    }

    Bitboard queen = White ? board.queenW : board.queenB;
    // create all moves for queen
    for (int i = 0; i < queen.size(); i++) {
            int current = queen[i];
            if (current == 1) {
                int x = i % 8;

                // check horizontal and vertical moves (same as rook)

                // vertical moves:
                Bitboard legal_check;

                for (int z = 0; z < 8; z++) {
                    if (x == i) {x += 8; continue;}
                    Bitboard move_board;
                    move_board[i] = 1;
                    move_board[x] = 1;

                    // set move check bitboard:
                    int factor;
                    if (i < x) {factor = 8;} else {factor = -8;}
                    int check = i + factor;
                    for (int counter =  0; counter < abs(i - x) / 8; counter++) {
                        if (check == i || check == x) {check += factor; continue;}
                        legal_check[check] = 1;
                        check += factor;
                    }

                    Move new_move;
                    new_move.move = move_board;
                    new_move.piece = pQueen;
                    new_move.legal_check = legal_check;
                    new_move.type = QUEEN_STRAIGHT;

                    moves.push_back(new_move);
                    legal_check.reset();
                    x += 8;
                }

                // horizontal moves:
                legal_check.reset();

                x = std::trunc(i / 8) * 8;
                for (int z = 0; z < 8; z++) {
                    if (x == i) {x++; continue;}
                    Bitboard move_board;
                    move_board[i] = 1;
                    move_board[x] = 1;

                    // set move check bitboard:
                    int factor;
                    if (i < x) {factor = 1;} else {factor = -1;}
                    int check = i + factor;
                    for (int counter = 0; counter < abs(i - x); counter++) {
                        if (check == i || check == x) {check += factor; continue;}
                        legal_check[check] = 1;
                        check += factor;
                    }

                    Move new_move;
                    new_move.move = move_board;
                    new_move.piece = pQueen;
                    new_move.legal_check = legal_check;
                    new_move.type = QUEEN_STRAIGHT;

                    moves.push_back(new_move);
                    legal_check.reset();
                    x++;
                }
                
                // check diagnol moves (same as bishop)

                legal_check.reset();

                // moving up right
                int row = get_row(i);
                x = i + 1;
                int y = i - 8 + 1;
                while (get_row(x) == row && get_row(y) >= 0) {
                    Bitboard move_board;
                    move_board[i] = 1;
                    move_board[y] = 1;

                    if ((y + 8 - 1) != i) {
                        legal_check[y + 8 - 1] = 1;
                    }

                    Move new_move;
                    new_move.move = move_board;
                    new_move.piece = pQueen;
                    new_move.legal_check = legal_check;
                    new_move.type = QUEEN_DIAGNOL;

                    moves.push_back(new_move);
                    x++;
                    y = y - 8 + 1;
                }

                // moving up left
                legal_check.reset();

                x = i - 1;
                y = i - 8 - 1;
                while(get_row(x) == row && get_row(y) >= 0) {
                    Bitboard move_board;
                    move_board[i] = 1;
                    move_board[y] = 1;

                    if ((y + 8 + 1) != i) {
                        legal_check[y + 8 + 1] = 1;
                    }

                    Move new_move;
                    new_move.move = move_board;
                    new_move.piece = pQueen;
                    new_move.legal_check = legal_check;
                    new_move.type = QUEEN_DIAGNOL;

                    moves.push_back(new_move);
                    x--;
                    y = y - 8 - 1;
                }

                // moving down right
                legal_check.reset();

                x = i + 1;
                y = i + 8 + 1;
                while (get_row(x) == row && get_row(y) <= 7) {
                    Bitboard move_board;
                    move_board[i] = 1;
                    move_board[y] = 1;

                    if ((y - 8 - 1) != i) {
                        legal_check[y - 8 - 1] = 1;
                    }

                    Move new_move;
                    new_move.move = move_board;
                    new_move.piece = pQueen;
                    new_move.legal_check = legal_check;
                    new_move.type = QUEEN_DIAGNOL;

                    moves.push_back(new_move);
                    x ++;
                    y = y + 8 + 1;
                }

                // moving down left
                legal_check.reset();

                x = i - 1;
                y = i + 8 - 1;
                while (get_row(x) == row && get_row(y) <= 7) {
                    Bitboard move_board;
                    move_board[i] = 1;
                    move_board[y] = 1;

                    if ((y - 8 + 1) != i) {
                        legal_check[y - 8 + 1] = 1;
                    }

                    Move new_move;
                    new_move.move = move_board;
                    new_move.piece = pQueen;
                    new_move.legal_check = legal_check;
                    new_move.type = QUEEN_DIAGNOL;

                    moves.push_back(new_move);
                    x --;
                    y = y + 8 - 1;
                }

            }
        }

    return moves;
}

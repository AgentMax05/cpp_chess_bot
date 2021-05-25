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
    auto add_move = [&moves](int i, int x, int type = REGULAR_MOVE) {
        return add_move_generic(moves, i, x, pPawn, type);
    };
    
    if (White) {
        // create all moves for pawns
        for (int i = 0; i < board.pawnW.size(); i++) {
            int current = board.pawnW[i];
            if (current == 1) {
                add_move(i, i-8);

                // check if pawn can move up 2
                if (48 <= i && i <= 55) {
                    Bitboard legal_check;
                    legal_check[i - 8] = 1;

                    size_t index = add_move(i, i-16, PAWN_DOUBLE);
                    (*moves)[index].legal_check = legal_check;
                }

                bool space_left = get_row(i - 1) == get_row(i);
                bool space_right = get_row(i + 1) == get_row(i);
                bool space_above = in_board(get_row(i - 8));

                // eat up left
                if (space_left && space_above) {
                    add_move(i, i-8-1, PAWN_EAT);
                }

                // eat up right
                if (space_right && space_above) {
                    add_move(i, i-8+1, PAWN_EAT);
                }

            }
        }
    
    } else {
        // create all moves for black pawns
        for (int i = 0; i < board.pawnB.size(); i++) {
            int current = board.pawnB[i];
            if (current == 1) {
                add_move(i, i+8);

                // move down 2
                if (get_row(i) == 1) {
                    Bitboard legal_check;
                    legal_check[i + 8] = 1;
                    size_t index = add_move(i, i+16, PAWN_DOUBLE);
                    (*moves)[index].legal_check = legal_check;
                }

                bool space_left = get_row(i - 1) == get_row(i);
                bool space_right = get_row(i + 1) == get_row(i);
                bool space_below = in_board(get_row(i + 8));

                // eat left
                if (space_left && space_below) {
                    add_move(i, i+8-1, PAWN_EAT);
                }

                // eat right
                if (space_right && space_below) {
                    add_move(i, i+8+1, PAWN_EAT);
                }
            }
        }
    
    }
}

void generate_moves_for_knights(vector<Move>* moves, Board board, bool White) {
    Bitboard knight = White ? board.knightW : board.knightB;
    
    auto add_move = [&moves](int i, int x, int type = REGULAR_MOVE) {
        add_move_generic(moves, i, x, pKnight, type);
    };

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
                add_move(i, i-2+8);
            }

            // move left 2 up 1
            if (space_left && space_above) {
                add_move(i, i-2-8);
            }

            // move right 2 down 1
            if (space_right && space_below) {
                add_move(i, i+2+8);
            }

            // move right 2 up 1
            if (space_right && space_above) {
                add_move(i, i+2-8);
            }


            // move up 2 left 1
            if (double_space_above && single_space_left) {
                add_move(i, i-1-16);
            }

            // move up 2 right 1
            if (double_space_above && single_space_right) {
                add_move(i, i+1-16);
            }

            // move down 2 left 1
            if (double_space_below && single_space_left) {
                add_move(i, i-1+16);
            }

            // move down 2 right 1
            if (double_space_below && single_space_right) {
                add_move(i, i+1+16);
            }

        }
    }
}

void generate_moves_for_king(vector<Move>* moves, Board board, bool White) {
    Bitboard king = White ? board.kingW : board.kingB;
    
    auto add_move = [&moves](int i, int x, int type = REGULAR_MOVE) {
        add_move_generic(moves, i, x, pKing, type);
    };
    
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
                add_move(i, i-8);
            }


            // move down
            if (space_below) {
                add_move(i, i+8);
            }

            // move left
            if (space_left) {
                add_move(i, i-1);
            }

            // move right
            if (space_right) {
                add_move(i, i+1);
            }

            // move up left
            if (space_left && space_above) {
                add_move(i, i-8-1);
            }

            // move up right
            if (space_right && space_above) {
                add_move(i, i-8+1);
            }

            // move down left
            if (space_left && space_below) {
                add_move(i, i+8-1);
            }

            // move down right
            if (space_right && space_below) {
                add_move(i, i+8+1);
            }
        }
    }
}

void generate_moves_for_rook(vector<Move>* moves, Board board, bool White) {
    Bitboard rook = White ? board.rookW : board.rookB;
    auto add_move = [&moves](int i, int x, Bitboard legal_check) {
        size_t index = add_move_generic(moves, i, x, pRook, REGULAR_MOVE);
        (*moves)[index].legal_check = legal_check;
    };
    // create all moves for rook
    for (int i = 0; i < rook.size(); i++) {
        int current = rook[i];
        if (current == 1) {
            int x = i % 8;

            // vertical moves:
            Bitboard legal_check;

            for (int z = 0; z < 8; z++) {
                if (x == i) {x += 8; continue;}

                // set move check bitboard:
                int factor;
                if (i < x) {factor = 8;} else {factor = -8;}
                int check = i + factor;
                for (int counter =  0; counter < abs(i - x) / 8; counter++) {
                    if (check == i || check == x) {check += factor; continue;}
                    legal_check[check] = 1;
                    check += factor;
                }

                add_move(i, x, legal_check);
                legal_check.reset();
                x += 8;
            }

            // horizontal moves:
            legal_check.reset();

            x = std::trunc(i / 8) * 8;
            for (int z = 0; z < 8; z++) {
                if (x == i) {x++; continue;}

                // set move check bitboard:
                int factor;
                if (i < x) {factor = 1;} else {factor = -1;}
                int check = i + factor;
                for (int counter = 0; counter < abs(i - x); counter++) {
                    if (check == i || check == x) {check += factor; continue;}
                    legal_check[check] = 1;
                    check += factor;
                }

                add_move(i, x, legal_check);
                legal_check.reset();
                x++;
            }
        }
    }
}

void generate_moves_for_bishop(vector<Move>* moves, Board board, bool White) {
    Bitboard bishop = White ? board.bishopW : board.bishopB;
    
    auto add_move = [&moves](int i, int x, Bitboard legal_check) {
        size_t index = add_move_generic(moves, i, x, pBishop, REGULAR_MOVE);
        (*moves)[index].legal_check = legal_check;
    };
    
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
                if ((y + 8 - 1) != i) {
                    legal_check[y + 8 - 1] = 1;
                }

                add_move(i, y, legal_check);
                x++;
                y = y - 8 + 1;
            }

            // moving up left
            legal_check.reset();

            x = i - 1;
            y = i - 8 - 1;
            while(get_row(x) == row && get_row(y) >= 0) {
                if ((y + 8 + 1) != i) {
                    legal_check[y + 8 + 1] = 1;
                }

                add_move(i, y, legal_check);
                x--;
                y = y - 8 - 1;
            }

            // moving down right
            legal_check.reset();

            x = i + 1;
            y = i + 8 + 1;
            while (get_row(x) == row && get_row(y) <= 7) {
                if ((y - 8 - 1) != i) {
                    legal_check[y - 8 - 1] = 1;
                }

                add_move(i, y, legal_check);
                x ++;
                y = y + 8 + 1;
            }

            // moving down left
            legal_check.reset();

            x = i - 1;
            y = i + 8 - 1;
            while (get_row(x) == row && get_row(y) <= 7) {
                if ((y - 8 + 1) != i) {
                    legal_check[y - 8 + 1] = 1;
                }

                add_move(i, y, legal_check);
                x --;
                y = y + 8 - 1;
            }

        }
    }
}

void generate_moves_for_queen(vector<Move>* moves, Board board, bool White) {
    Bitboard queen = White ? board.queenW : board.queenB;
    
    auto add_move = [&moves](int i, int x, Bitboard legal_check, int type) {
        size_t index = add_move_generic(moves, i, x, pQueen, type);
        (*moves)[index].legal_check = legal_check;
    };
    
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
                // set move check bitboard:
                int factor;
                if (i < x) {factor = 8;} else {factor = -8;}
                int check = i + factor;
                for (int counter =  0; counter < abs(i - x) / 8; counter++) {
                    if (check == i || check == x) {check += factor; continue;}
                    legal_check[check] = 1;
                    check += factor;
                }

                add_move(i, x, legal_check, QUEEN_STRAIGHT);
                legal_check.reset();
                x += 8;
            }

            // horizontal moves:
            legal_check.reset();

            x = std::trunc(i / 8) * 8;
            for (int z = 0; z < 8; z++) {
                if (x == i) {x++; continue;}
                
                // set move check bitboard:
                int factor;
                if (i < x) {factor = 1;} else {factor = -1;}
                int check = i + factor;
                for (int counter = 0; counter < abs(i - x); counter++) {
                    if (check == i || check == x) {check += factor; continue;}
                    legal_check[check] = 1;
                    check += factor;
                }

                add_move(i, x, legal_check, QUEEN_STRAIGHT);
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
                if ((y + 8 - 1) != i) {
                    legal_check[y + 8 - 1] = 1;
                }

                add_move(i, y, legal_check, QUEEN_DIAGNOL);
                x++;
                y = y - 8 + 1;
            }

            // moving up left
            legal_check.reset();

            x = i - 1;
            y = i - 8 - 1;
            while(get_row(x) == row && get_row(y) >= 0) {
                if ((y + 8 + 1) != i) {
                    legal_check[y + 8 + 1] = 1;
                }

                add_move(i, y, legal_check, QUEEN_DIAGNOL);
                x--;
                y = y - 8 - 1;
            }

            // moving down right
            legal_check.reset();

            x = i + 1;
            y = i + 8 + 1;
            while (get_row(x) == row && get_row(y) <= 7) {
                if ((y - 8 - 1) != i) {
                    legal_check[y - 8 - 1] = 1;
                }

                add_move(i, y, legal_check, QUEEN_DIAGNOL);
                x ++;
                y = y + 8 + 1;
            }

            // moving down left
            legal_check.reset();

            x = i - 1;
            y = i + 8 - 1;
            while (get_row(x) == row && get_row(y) <= 7) {
                if ((y - 8 + 1) != i) {
                    legal_check[y - 8 + 1] = 1;
                }

                add_move(i, y, legal_check, QUEEN_DIAGNOL);
                x --;
                y = y + 8 - 1;
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

    return moves;
}

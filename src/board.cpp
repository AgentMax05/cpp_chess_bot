#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include "board.h"
#include "move_generation.h"

typedef std::bitset<64> Bitboard;
using namespace std;

bool is_lowercase(char input);
bool is_num(char input);
char to_lowercase(char input);
int char_to_int(char input);

void Board::update_boards() {
    Board::boardW = Board::pawnW | Board::bishopW | Board::rookW | Board::knightW | Board::kingW | Board:: queenW;
    Board::boardB = Board::pawnB | Board::bishopB | Board::rookB | Board::knightB | Board::kingB | Board:: queenB;
    Board::complete_board = Board::boardW | Board::boardB;
}

void Board::update_epPawns() {
    Board::epPawns = Board::pawnW | Board::pawnB;
}

void Board::make_move(int piece, Bitboard move, bool White, int type /*= 0*/) {
    update_epPawns();
    if (White) {

        // make castling move
        if (piece == pNone  && type == L_CASTLE) {
            Board::kingW = Board::kingW ^ Board::lCastleWMove[0];
            Board::rookW = Board::rookW ^ Board::lCastleWMove[1];
        }

        else if (piece == pNone && type == R_CASTLE) {
            Board::kingW = Board::kingW ^ Board::rCastleWMove[0];
            Board::rookW = Board::rookW ^ Board::rCastleWMove[1];
        }

        // make en passant move
        if (piece == pPawn && type == PAWN_ENPASSANT) {
            Bitboard bPawn = (move ^ Board::pawnW) << 8;
            Board::pawnB = Board::pawnB ^ bPawn;
            Board::pawnW = Board::pawnW ^ move;
        }

        // checks if a piece is being eaten
        if (!(move & Board::boardB).none()) {
            Bitboard eaten = move & Board::boardB;

            // removes piece from board if board has piece
            Board::pawnB = Board::pawnB ^ (eaten & Board::pawnB);
            Board::rookB = Board::rookB ^ (eaten & Board::rookB);
            Board::bishopB = Board::bishopB ^ (eaten & Board::bishopB);
            Board::knightB = Board::knightB ^ (eaten & Board::knightB);
            Board::queenB = Board::queenB ^ (eaten & Board::queenB);
        } 

        if (piece == pPawn) {Board::pawnW = Board::pawnW ^ move;}
        else if (piece == pRook) {Board::rookW = Board::rookW ^ move;}
        else if (piece == pKnight) {Board::knightW = Board::knightW ^ move;}
        else if (piece == pBishop) {Board::bishopW = Board::bishopW ^ move;}
        else if (piece == pQueen) {Board::queenW = Board::queenW ^ move;}
        else if (piece == pKing) {Board::kingW = Board::kingW ^ move;}
    } else {

        // make castling move
        if (piece == pNone && type == L_CASTLE) {
            Board::kingB = Board::kingB ^ Board::lCastleBMove[0];
            Board::rookB = Board::rookB ^ Board::lCastleBMove[1];
        }

        else if (piece == pNone && type == R_CASTLE) {
            Board::kingB = Board::kingB ^ Board::rCastleBMove[0];
            Board::rookB = Board::rookB ^ Board::rCastleBMove[1];
        }

        // make en passant move
        if (piece == pPawn && type == PAWN_ENPASSANT) {
            Bitboard wPawn = (move ^ Board::pawnB) >> 8;
            Board::pawnW = Board::pawnW ^ wPawn;
            Board::pawnB = Board::pawnB ^ move;
        }

        // checks if a piece is being eaten
        if (!(move & Board::boardW).none()) {
            Bitboard eaten = move & Board::boardW;

            // removes piece from board if board has piece
            Board::pawnW = Board::pawnW ^ (eaten & Board::pawnW);
            Board::rookW = Board::rookW ^ (eaten & Board::rookW);
            Board::bishopW = Board::bishopW ^ (eaten & Board::bishopW);
            Board::knightW = Board::knightW ^ (eaten & Board::knightW);
            Board::queenW = Board::queenW ^ (eaten & Board::queenW);
        }

        if (piece == pPawn) {Board::pawnB = Board::pawnB ^ move;}
        else if (piece == pRook) {Board::rookB = Board::rookB ^ move;}
        else if (piece == pKnight) {Board::knightB = Board::knightB ^ move;}
        else if (piece == pBishop) {Board::bishopB = Board::bishopB ^ move;}
        else if (piece == pQueen) {Board::queenB = Board::queenB ^ move;}
        else if (piece == pKing) {Board::kingB = Board::kingB ^ move;}
    }
    update_boards();
}

void Board::init_board(string FEN) {
    load_dbs();

    string current = "";
    std::vector<string> fen_notation1 = {};
    for (int i = 0; i < FEN.size(); i++) {
        if (FEN[i] != '/') {
            current += FEN[i];
        } else {
            fen_notation1.push_back(current);
            current = "";
        }
    }
    if (current != "") {
        fen_notation1.push_back(current);
        current = "";
    }

    // convert spaces to '0':
    std::vector<string> fen_notation = {};
    for (int i = 0; i < fen_notation1.size(); i++) {
        string current_fen = "";
        string current = fen_notation1[i];
        for (int x = 0; x < current.size(); x++) {
            char curr_char = current[x];
            if (is_num(curr_char)) {
                for (int z = 0; z < char_to_int(curr_char); z++) {
                    current_fen += "0";
                }
            } else {
                current_fen += curr_char;
            }
        }
        fen_notation.push_back(current_fen);
    }

    for (int i = 0; i < fen_notation.size(); i++) {
        string current = fen_notation[i];
        int spaces = 0;

        for (int x = 0; x < current.size(); x++) {
            char current_char = current[x];
            bool lowercase = is_lowercase(current_char);
            char piece = to_lowercase(current_char);

            int board_space = (i * 8) + x;

            if (current_char == '0') {continue;}

            if (piece == 'p') {
                if (lowercase) {
                    Board::pawnB[board_space] = 1;
                } else {
                    Board::pawnW[board_space] = 1;
                }
            }
            else if (piece == 'r') {
                if (lowercase) {
                    Board::rookB[board_space] = 1;
                } else {
                    Board::rookW[board_space] = 1;
                }
            }
            else if (piece == 'b') {
                if (lowercase) {
                    Board::bishopB[board_space] = 1;
                } else {
                    Board::bishopW[board_space] = 1;
                }
            }
            else if (piece == 'n') {
                if (lowercase) {
                    Board::knightB[board_space] = 1;
                } else {
                    Board::knightW[board_space] = 1;
                }
            }
            else if (piece == 'q') {
                if (lowercase) {
                    Board::queenB[board_space] = 1;
                } else {
                    Board::queenW[board_space] = 1;
                }
            }
            else if (piece == 'k') {
                if (lowercase) {
                    Board::kingB[board_space] = 1;
                } else {
                    Board::kingW[board_space] = 1;
                }
            }
        }
    }

    update_boards();

    // init castling bools
    if (rookW[56] == 1) {lCastleW = true;}
    if (rookW[63] == 1) {rCastleW = true;}
    if (rookB[0] == 1) {lCastleB = true;}
    if (rookB[7] == 1) {rCastleB = true;}
}

string move_to_string(Move* move) {
    std::stringstream ss;
    ss << "{Move piece=" << move->piece
       << " type=" << move->type
       << " move=" << move->move
       << " legal_check=" << move->legal_check
       << "}";
    return ss.str();
}

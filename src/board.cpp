#include <string>
#include <vector>
#include <bitset>

#include "board.h"

typedef std::bitset<64> Bitboard;
using std::string;

const int pKing = 0;
const int pQueen = 1;
const int pRook = 2;
const int pKnight = 3;
const int pBishop = 4;
const int pPawn = 5;

bool is_lowercase(char input);
bool is_num(char input);
char to_lowercase(char input);
int char_to_int(char input);

void Board::update_boards() {
    Board::boardW = Board::pawnW | Board::bishopW | Board::rookW | Board::knightW | Board::kingW | Board:: queenW;
    Board::boardB = Board::pawnB | Board::bishopB | Board::rookB | Board::knightB | Board::kingB | Board:: queenB;
    Board::complete_board = Board::boardW | Board::boardB;
}

void Board::make_move(int piece, Bitboard move, bool White) {
    if (White) {
        if (piece == pPawn) {Board::pawnW = Board::pawnW ^ move;}
        else if (piece == pRook) {Board::rookW = Board::rookW ^ move;}
        else if (piece == pKnight) {Board::knightW = Board::knightW ^ move;}
        else if (piece == pBishop) {Board::bishopW = Board::bishopW ^ move;}
        else if (piece == pQueen) {Board::queenW = Board::queenW ^ move;}
        else if (piece == pKing) {Board::kingW = Board::kingW ^ move;}
    } else {
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

    Board::boardW = Board::pawnW | Board::rookW | Board::bishopW | Board::knightW | Board::queenW | Board::kingW;
    Board::boardB = Board::pawnB | Board::rookB | Board::bishopB | Board::knightB | Board::queenB | Board::kingB;

}
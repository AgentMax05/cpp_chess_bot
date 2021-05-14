#include <string>
#include <vector>
#include <bitset>

#include "board.h"

typedef std::bitset<64> Bitboard;
using namespace std;

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

/**
 Displays a human-readable board
 */
string Board::getDisplayString() {
    vector<pair<string, Bitboard*>> boards = {
        pair<string, Bitboard*>("♙", &pawnW),
        pair<string, Bitboard*>("♔", &kingW),
        pair<string, Bitboard*>("♕", &queenW),
        pair<string, Bitboard*>("♖", &rookW),
        pair<string, Bitboard*>("♗", &bishopW),
        pair<string, Bitboard*>("♘", &knightW),
        pair<string, Bitboard*>("♟︎", &pawnB),
        pair<string, Bitboard*>("♚", &kingB),
        pair<string, Bitboard*>("♛", &queenB),
        pair<string, Bitboard*>("♜", &rookB),
        pair<string, Bitboard*>("♝", &bishopB),
        pair<string, Bitboard*>("♞", &knightB)
    };
    
    // initialize the empty board
    vector<vector<string>> displayBoard;
    for(int i=0; i<8; i++) {
        vector<string> row;
        
        for(int j=0; j<8; j++) {
            row.push_back("_");
        }
        displayBoard.push_back(row);
    }
    
    // fill in pieces
    for(pair<string, Bitboard*> item: boards) {
        string marker = item.first;
        Bitboard board = *item.second;
        
        for(int i=0; i<board.size(); i++) {
            if(board[i] == 0) continue;
            
            int row = i / 8;
            int col = i % 8;
            displayBoard[row][col] = marker;
        }
    }
    
    // combine into a string
    string display = "";
    for(int row=0; row<8; row++) {
        for(int col=0; col<8; col++) {
            display += displayBoard[row][col];
        }
        if(row != 7) {
            display += "\n";
        }
    }
     
    return display;
}


void Board::make_move(int piece, Bitboard move, bool White) {
    if (White) {

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
}
#include "BoardRenderer.h"

/**
 On Windows, we return an ASCII Renderer by default since
 the Windows terminal has difficulties displaying Unicode.
 */
BoardRenderer* BoardRenderer::getDefaultRenderer() {
    static BoardRenderer* instance = NULL;
    
    if(instance != NULL) {
        return instance;
    }
    
#ifdef _WIN32
    instance = new AsciiBoardRenderer();
#else
    instance = new UnicodeBoardRenderer();
#endif
    
    return instance;
}

vector<pair<string, Bitboard*>> UnicodeBoardRenderer::getDisplayBoardRepr(Board *board) {
    return {
        pair<string, Bitboard*>("♙", &board->pawnW),
        pair<string, Bitboard*>("♔", &board->kingW),
        pair<string, Bitboard*>("♕", &board->queenW),
        pair<string, Bitboard*>("♖", &board->rookW),
        pair<string, Bitboard*>("♗", &board->bishopW),
        pair<string, Bitboard*>("♘", &board->knightW),
        pair<string, Bitboard*>("♟︎", &board->pawnB),
        pair<string, Bitboard*>("♚", &board->kingB),
        pair<string, Bitboard*>("♛", &board->queenB),
        pair<string, Bitboard*>("♜", &board->rookB),
        pair<string, Bitboard*>("♝", &board->bishopB),
        pair<string, Bitboard*>("♞", &board->knightB)
    };
}

vector<pair<string, Bitboard*>> AsciiBoardRenderer::getDisplayBoardRepr(Board *board) {
    return {
        pair<string, Bitboard*>("P", &board->pawnW),
        pair<string, Bitboard*>("K", &board->kingW),
        pair<string, Bitboard*>("Q", &board->queenW),
        pair<string, Bitboard*>("R", &board->rookW),
        pair<string, Bitboard*>("B", &board->bishopW),
        pair<string, Bitboard*>("N", &board->knightW),
        pair<string, Bitboard*>("p", &board->pawnB),
        pair<string, Bitboard*>("k", &board->kingB),
        pair<string, Bitboard*>("q", &board->queenB),
        pair<string, Bitboard*>("r", &board->rookB),
        pair<string, Bitboard*>("b", &board->bishopB),
        pair<string, Bitboard*>("n", &board->knightB)
    };
}

string BoardRenderer::render(Board* board) {
    vector<pair<string, Bitboard*>> boards = getDisplayBoardRepr(board);
    
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

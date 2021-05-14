#include <string>
#include "board.h"

using namespace std;

class BoardRenderer {
public:
    string render(Board* board);
    static BoardRenderer* getDefaultRenderer();
protected:
    virtual vector<pair<string, Bitboard*>> getDisplayBoardRepr(Board* board) = 0;
};

/**
 Renders a board using only ASCII characters.
 */
class AsciiBoardRenderer: public BoardRenderer {
public:
    vector<pair<string, Bitboard*>> getDisplayBoardRepr(Board* board);
};

/**
 Renders a board using Unicode characters for the
 pieces.
 */
class UnicodeBoardRenderer: public BoardRenderer {
public:
    vector<pair<string, Bitboard*>> getDisplayBoardRepr(Board* board);
};

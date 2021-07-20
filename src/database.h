#include <random>

// bitboards for each file used in move generation (file is a column, so FILEA = a1-a8)
extern Bitboard FILEA;
extern Bitboard FILEB;
extern Bitboard FILEC;
extern Bitboard FILED;
extern Bitboard FILEE;
extern Bitboard FILEF;
extern Bitboard FILEG;
extern Bitboard FILEH;

// bitboard for each rank used in move generation (rank numberings start at 1 from bottom, bit 63 is in rank 1)
extern Bitboard RANK1;
extern Bitboard RANK2;
extern Bitboard RANK3;
extern Bitboard RANK4;
extern Bitboard RANK5;
extern Bitboard RANK6;
extern Bitboard RANK7;
extern Bitboard RANK8;

extern vector<Bitboard> rook_masks;
extern vector<Bitboard> bishop_masks;
extern vector<uint64_t> rook_magics;
extern vector<uint64_t> bishop_magics;
extern vector<vector<Bitboard>> rook_db;
extern vector<vector<Bitboard>> bishop_db;
extern vector<int> rook_shifts;
extern vector<int> bishop_shifts;

// pawn db consists of 4 bitboards: {regular move by 1, double move, taking moves, en passant moves}
extern vector<vector<Bitboard>> pawn_dbW;
extern vector<vector<Bitboard>> pawn_dbB;
extern vector<Bitboard> knight_db;
extern vector<Bitboard> king_db;

void init_dbs();
uint64_t get_index(int sq, int piece, Bitboard blockerboard);
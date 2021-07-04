#include <iostream>
#include "board.h"
#include "move_generation.h"
#include "database.h"

/*
  rook and bishop moves are stored using magic bitboards, in which moves are calculated
  for every square with every possible combination of blockers, and then hashed with a
  'magic number' to an index in a vector. The hash function can be found in get_index. 
  These magic numbers are obtained by brute force until one that works is found. This 
  program uses pre-calculated magic numbers that were already found and proven to work. 

  Terms used in magic bitboards:

  blocker mask - Bitboard containing 1s in squares that can block the piece  

  example for rook at c4:
  00000000
  00100000
  00100000
  00100000
  01011110
  00100000
  00100000
  00000000  
  edges are never a 1 as they always block the piece, meaning the piece can never move past them

  blocker board - Bitboard containing 1s in occupied squares that are in the blockermask
  blocker_board = occupied_squares & blockermask  

  move board - Bitboard containing 1s in all squares where piece can move to 
  with edge squares counted if they are accessible
*/

Bitboard FILEA = Bitboard("0000000100000001000000010000000100000001000000010000000100000001");
Bitboard FILEB = Bitboard("0000001000000010000000100000001000000010000000100000001000000010");
Bitboard FILEC = Bitboard("0000010000000100000001000000010000000100000001000000010000000100");
Bitboard FILED = Bitboard("0000100000001000000010000000100000001000000010000000100000001000");
Bitboard FILEE = Bitboard("0001000000010000000100000001000000010000000100000001000000010000");
Bitboard FILEF = Bitboard("0010000000100000001000000010000000100000001000000010000000100000");
Bitboard FILEG = Bitboard("0100000001000000010000000100000001000000010000000100000001000000");
Bitboard FILEH = Bitboard("1000000010000000100000001000000010000000100000001000000010000000");

Bitboard RANK1 = Bitboard("1111111100000000000000000000000000000000000000000000000000000000");
Bitboard RANK2 = Bitboard("0000000011111111000000000000000000000000000000000000000000000000");
Bitboard RANK3 = Bitboard("0000000000000000111111110000000000000000000000000000000000000000");
Bitboard RANK4 = Bitboard("0000000000000000000000001111111100000000000000000000000000000000");
Bitboard RANK5 = Bitboard("0000000000000000000000000000000011111111000000000000000000000000");
Bitboard RANK6 = Bitboard("0000000000000000000000000000000000000000111111110000000000000000");
Bitboard RANK7 = Bitboard("0000000000000000000000000000000000000000000000001111111100000000");
Bitboard RANK8 = Bitboard("0000000000000000000000000000000000000000000000000000000011111111");

// magic numbers obtained through brute-force by Dr. Kannan (http://pradu.us/old/Nov27_2008/Buzz/)
vector<uint64_t> rook_magics = {
    36028866279506048, 18014467231055936, 36037661833560192, 36033229426262144,
    36031013222613120, 36029905120788608, 36029346791555584, 36028935540048128, 
    140738029420672,   70369281069056,    140806209929344,   140771849142400,
    140754668748928,   140746078552192,   140741783453824,   140738570486016,
    35734132097152,    70643624185856,    141287512612864,   141287378391040,
    141287311280128,   141287277724672,   1103806726148,     2199027482884, 
    35736275402752,    35185445851136,    17594335625344,    8798241554560,  
    4400194519168,     2201171001472,     1101667500544,     140739635855616,   
    35459258384512,    35185450029056,    17594341924864,    8798248898560,  
    4400202385408,     2201179128832,     2199040098308,     140738570486016,   
    35459258417152,    35185445863552,    17592722948224,    8796361490560,  
    4398180761728,     2199090397312,     1099545215104,     277042298884,      
    35459258384512,    35185445863552,    17592722948224,    8796361490560,
    4398180761728,     2199090397312,     140741783453824,   140738578874496,
    72054149422608714, 36025352403644746, 18014183758528662, 4432674693377, 
    281483634278417,   281483633756161,   281477157749761,   562927068959138
};

vector<uint64_t> bishop_magics = {
    565157600297472,   565157600296960,   1127008041959424,  1130300100837376, 
    299342040662016,   143006304829440,   71485708304384,    17871427092480,     
    4415293752320,     2207646876160,     4402375163904,     4415234768896,
    1169304846336,     558618378240,      279241048064,      139620524032,
    1125934401325056,  562967200662528,   281483600331264,   140771881664512,  
    140754678710272,   35188675985408,    70370925748224,    35185462874112,      
    571746315931648,   285873157965824,   35734195078144,    70643689259520,     
    145135543263232,   70643655708672,    141287261212672,   70643630606336,  
    285941744803840,   142970872401920,   17884244346880,    2201171263616,   
    70644696088832,    141291539267840,   282578783438848,   141289391719424, 
    142971408236544,   71485704118272,    8935813681152,     137724168192,      
    8800392184832,     282578800083456,   565157600166912,   282578800083456,  
    71485708304384,    35742854152192,    139654594560,      545783808,
    68753162240,       4415360729088,     1130315200593920,  565157600296960,     
    17871427092480,    139620524032,      545525760,         2131968,
    268567040,         17247502848,       4415293752320,     565157600297472
};

vector<Bitboard> rook_masks = {};
vector<Bitboard> bishop_masks = {};
vector<vector<Bitboard>> rook_db(64);
vector<vector<Bitboard>> bishop_db(64);
vector<int> rook_shifts(64, 0);
vector<int> bishop_shifts(64, 0);
vector<vector<Bitboard>> pawn_dbW(64);
vector<vector<Bitboard>> pawn_dbB(64);
vector<Bitboard> knight_db(64);
vector<Bitboard> king_db(64);

// fill rook_masks and bishop_masks with blocker masks
void load_masks() {
    std::cerr << "load_masks :: BEGIN\n" << std::endl;
    for (int i = 0; i < 64; i++) {
        // generate rook blocker mask for square i
        Bitboard rook;
        
        int row = i / 8;
        int column = i % 8;

        int left_edge = row * 8;
        int right_edge = left_edge + 7;
        int bottom_edge = column + 56;
        int top_edge = column;

        int current = i + 1;
        while ((current / 8) == row && current < right_edge) {
            rook.set(current);
            current++;
        }

        current = i - 1;
        while (int(current / 8) == row && current > left_edge) {
            rook.set(current);
            current--;
        }

        current = i - 8;
        while (current >= 0 && current != top_edge) {
            rook.set(current);
            current -= 8;
        }

        current = i + 8;
        while (current < 64 && current != bottom_edge) {
            rook.set(current);
            current += 8;
        }

        rook_masks.push_back(rook);

        // generate bishop blocker mask for square i
        Bitboard bishop;

        current = i + 1 - 8;
        column = i + 1;
        while (current >= 8 && column < right_edge) {
            bishop.set(current);
            current += 1 - 8;
            column++;
        }

        current = i - 1 - 8;
        column = i - 1;
        while (current >= 8 && column > left_edge) {
            bishop.set(current);
            current += -1 - 8;
            column--;
        }

        current = i + 1 + 8;
        column = i + 1;
        while (current < 56 && column < right_edge) {
            bishop.set(current);
            current += 1 + 8;
            column++;
        }

        current = i - 1 + 8;
        column = i - 1;
        while (current < 56 && column > left_edge) {
            bishop.set(current);
            current += -1 + 8;
            column--;
        }

        bishop_masks.push_back(bishop);        
    }
    std::cerr << "load_masks :: END\n" << std::endl;
}

// fill rook_shifts and bishop_shifts with >> value
void load_shifts() {
    std::cerr << "load_shifts :: BEGIN\n" << std::endl;
    for (int i = 0; i < rook_masks.size(); i++) {
        rook_shifts[i] = 64 - rook_masks[i].count();
    }
    for (int i = 0; i < bishop_masks.size(); i++) {
        bishop_shifts[i] = 64 - bishop_masks[i].count();
    }
    std::cerr << "load_shifts :: END\n" << std::endl;
}

// used to generate all blockerboards for a blocker mask
Bitboard gen_blockerboard(uint64_t index, Bitboard blockermask) {
    std::cerr << "gen_blockerboard :: BEGIN\n" << std::endl;
    Bitboard blockerboard = blockermask;
    int bitindex = 0;
    for (int i = 0; i < blockermask.size(); i++) {
        if (blockermask[i]) {
            if (!Bitboard(index)[bitindex]) {
            // if (!(index & (1 << bitindex))) {
                blockerboard[i] = 0;
            }
            bitindex++;
        }
    }
    std::cerr << "gen_blockerboard :: END\n" << std::endl;
    return blockerboard;
}

// used to generate moveboard for a square, blocker board, and piece
Bitboard gen_moveboard(int sq, Bitboard blockerboard, int piece) {
    std::cerr << "gen_moveboard :: BEGIN\n" << std::endl;
    Bitboard moveboard;
    int left_edge = int(sq / 8) * 8;
    int right_edge = left_edge + 7;
    int column = sq % 8;

    if (piece == pRook) {
        int current = sq + 1;
        while (current <= right_edge) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current++;
        }

        current = sq - 1;
        while (current >= left_edge) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current--;
        }

        current = sq - 8;
        while (current >= 0) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current -= 8;
        }

        current = sq + 8;
        while (current <= 63) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current += 8;
        }
    } else {
        int current = sq + 1 - 8;
        column = sq + 1;
        while (current >= 0 && column <= right_edge) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current += 1 - 8;
            column++;
        }

        current = sq - 1 - 8;
        column = sq - 1;
        while (current >= 0 && column >= left_edge) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current += - 1 - 8;
            column--;
        }

        current = sq + 1 + 8;
        column = sq + 1;
        while (current <= 63 && column <= right_edge) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current += 1 + 8;
            column++;
        }

        current = sq - 1 + 8;
        column = sq - 1;
        while (current <= 63 && column >= left_edge) {
            moveboard.set(current);
            if (blockerboard[current]) break;
            current += -1 + 8;
            column--;
        }
    }

    std::cerr << "gen_moveboard :: END\n" << std::endl;
    return moveboard;
}

// returns rook_db or bishop_db index with square, piece, and blockerboard
uint64_t get_index(int sq, int piece, Bitboard blockerboard) {
    return (blockerboard.to_ullong() * (piece == pRook ? rook_magics[sq] : bishop_magics[sq])) >> (piece == pRook ? rook_shifts[sq] : bishop_shifts[sq]);
}

// used to fill rook_db and bishop_db with every possible blocker board for every square
void load_dbs(int piece) {
    std::cerr << "load_dbs :: BEGIN\n" << std::endl;
    for (int sq = 0; sq < 64; sq++) {
        Bitboard mask = piece == pRook ? rook_masks[sq] : bishop_masks[sq];
        vector<Bitboard> db(pow(2, mask.count()));

        for (uint64_t i = 0; i < pow(2, mask.count()); i++) {
            Bitboard blockerboard = gen_blockerboard(i, mask);
            Bitboard moveboard = gen_moveboard(sq, blockerboard, piece);
            uint64_t index = get_index(sq, piece, blockerboard);
            db[index] = moveboard;
        }
        (piece == pRook ? rook_db : bishop_db)[sq] = db;
    }
    std::cerr << "load_dbs :: END\n" << std::endl;
}

// used to fill pawn_dbW and pawn_dbB with all pawn moves at each square
void load_pawns() {
    std::cerr << "load_pawns :: BEGIN\n" << std::endl;
    for (int sq = 0; sq < 64; sq++) {
        Bitboard piece;
        piece.set(sq);

        // white pawn moves
        Bitboard forward_w = (piece & ~RANK8) >> 8;
        Bitboard eatL_w = ((piece & ~FILEA) & ~RANK8) >> 9;
        Bitboard eatR_w = ((piece & ~FILEH) & ~RANK8) >> 7;
        Bitboard double_forward_w = (piece & RANK2) >> 16;
        Bitboard enPassL_w = ((piece & RANK5) & ~FILEA) >> 9;
        Bitboard enPassR_w = ((piece & RANK5) & ~FILEH) >> 7;

        pawn_dbW[sq].reserve(4);
        pawn_dbW[sq][0] = forward_w;
        pawn_dbW[sq][1] = double_forward_w;
        pawn_dbW[sq][2] = eatL_w | eatR_w;
        pawn_dbW[sq][3] = enPassL_w | enPassR_w;

        // black pawn moves
        Bitboard forward_b = (piece & ~RANK1) << 8;
        Bitboard eatL_b = ((piece & ~FILEA) & ~RANK1) << 7;
        Bitboard eatR_b = ((piece & ~FILEH) & ~RANK1) << 9;
        Bitboard double_forward_b = (piece & RANK7) << 16;
        Bitboard enPassL_b = ((piece & RANK4) & ~FILEA) << 7;
        Bitboard enPassR_b = ((piece & RANK4) & ~FILEH) << 9;

        pawn_dbB[sq].reserve(4);
        pawn_dbB[sq][0] = forward_b;
        pawn_dbB[sq][1] = double_forward_b;
        pawn_dbB[sq][2] = eatL_b | eatR_b;
        pawn_dbB[sq][3] = enPassL_b | enPassR_b;
    }
    std::cerr << "load_pawns :: END\n" << std::endl;
}

// used to fill knigh_db with all knight moves at each square
void load_knight() {
    std::cerr << "load_knight :: BEGIN\n" << std::endl;
    for (int sq = 0; sq < 64; sq++) {
        Bitboard piece;
        piece.set(sq);

        // u = up, d = down, l = left, r = right
        Bitboard u2_l1 = (((piece & ~RANK8) & ~RANK7) & ~FILEA) >> 17;
        Bitboard u2_r1 = (((piece & ~RANK8) & ~RANK7) & ~FILEH) >> 15;
        Bitboard d2_l1 = (((piece & ~RANK1) & ~RANK2) & ~FILEA) << 15;
        Bitboard d2_r1 = (((piece & ~RANK1) & ~RANK2) & ~FILEH) << 17;
        Bitboard u1_l2 = (((piece & ~FILEA) & ~FILEB) & ~RANK8) >> 10;
        Bitboard u1_r2 = (((piece & ~FILEH) & ~FILEG) & ~RANK8) >> 6;
        Bitboard d1_l2 = (((piece & ~FILEA) & ~FILEB) & ~RANK1) << 6;
        Bitboard d1_r2 = (((piece & ~FILEH) & ~FILEG) & ~RANK1) << 10;

        knight_db[sq] = (u2_l1 | u2_r1 | d2_l1 | d2_r1 | u1_l2 | u1_r2 | d1_l2 | d1_r2);
    }
    std::cerr << "load_knight :: END\n" << std::endl;
}

// used to fill king_db with all king moves at each square
void load_king() {
    std::cerr << "load_king :: BEGIN\n" << std::endl;
    for (int sq = 0; sq < 64; sq++) {
        Bitboard piece;
        piece.set(sq);

        Bitboard u1 = (piece & ~RANK8) >> 8;
        Bitboard d1 = (piece & ~RANK1) << 8;
        Bitboard l1 = (piece & ~FILEA) >> 1;
        Bitboard r1 = (piece & ~FILEH) << 1;
        Bitboard u1_l1 = ((piece & ~RANK8) & ~FILEA) >> 9;
        Bitboard u1_r1 = ((piece & ~RANK8) & ~FILEH) >> 7;
        Bitboard d1_l1 = ((piece & ~RANK1) & ~FILEA) << 7;
        Bitboard d1_r1 = ((piece & ~RANK1) & ~FILEH) << 9;

        king_db[sq] = (u1 | d1 | l1 | r1 | u1_l1 | u1_r1 | d1_l1 | d1_r1);
    }
    std::cerr << "load_king :: END\n" << std::endl;
}

// used to initialize all dbs
void init_dbs() {
    std::cerr << "init_dbs :: BEGIN\n" << std::endl;
    load_masks();
    load_shifts();
    load_dbs(pRook);
    load_dbs(pBishop);
    load_knight();
    load_king();
    load_pawns();
    std::cerr << "init_dbs :: END\n" << std::endl;
}
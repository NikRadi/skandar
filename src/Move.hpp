#ifndef SKANDAR_MOVE_HPP
#define SKANDAR_MOVE_HPP
#include "Common.hpp"


typedef uint8 PieceType;


enum : uint8 {
    A8 = 56, B8, C8, D8, E8, F8, G8, H8,
    A7 = 48, B7, C7, D7, E7, F7, G7, H7,
    A6 = 40, B6, C6, D6, E6, F6, G6, H6,
    A5 = 32, B5, C5, D5, E5, F5, G5, H5,
    A4 = 24, B4, C4, D4, E4, F4, G4, H4,
    A3 = 16, B3, C3, D3, E3, F3, G3, H3,
    A2 =  8, B2, C2, D2, E2, F2, G2, H2,
    A1 =  0, B1, C1, D1, E1, F1, G1, H1,
};

enum MoveType : uint8 {
    QUIET_MOVE,
    CAPTURE_MOVE,
    KING_SIDE_CASTLING,
    QUEEN_SIDE_CASTLING,
    TWO_STEP_MOVE,
    EN_PASSANT_MOVE,
    PROMOTION_QUEEN,
    PROMOTION_ROOK,
    PROMOTION_KNIGHT,
    PROMOTION_BISHOP,
};

struct Move {
    PieceType piece_type;
    uint8 from;
    uint8 to;
    MoveType type;
    PieceType captured_piece_type;
    int score;


    bool operator<(const Move &move) const {
        return score < move.score;
    }
};


Move InitMove(PieceType piece_type, ulong from, ulong to, MoveType move_type, PieceType captured_piece_type);

Move InitMove(PieceType piece_type, ulong from, ulong to, MoveType move_type);

Move InitMove(PieceType piece_type, ulong from, ulong to);

int File(ulong square);

int Rank(ulong square);

std::string ToString(Move move);

std::string ToLongString(Move move);

#endif // SKANDAR_MOVE_HPP
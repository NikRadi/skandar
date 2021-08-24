#include "Move.hpp"
#include "Board.hpp"


Move InitMove(PieceType piece_type, ulong from, ulong to, MoveType move_type, PieceType captured_piece_type) {
    Move move;
    move.piece_type = piece_type;
    move.from = static_cast<uint8>(from);
    move.to = static_cast<uint8>(to);
    move.type = move_type;
    move.captured_piece_type = captured_piece_type;
    move.score = 0;
    return move;
}

Move InitMove(PieceType piece_type, ulong from, ulong to, MoveType move_type) {
    return InitMove(piece_type, from, to, move_type, NONE_PIECE);
}

Move InitMove(PieceType piece_type, ulong from, ulong to) {
    return InitMove(piece_type, from, to, QUIET_MOVE);
}

int File(ulong square) {
    return square & 0b111;
}

int Rank(ulong square) {
    return (square & 0b111000) >> 3;
}

std::string ToString(Move move) {
    uint8 move_from_file = 'a' + (move.from % 8);
    uint8 move_from_rank = move.from / 8 + 1;

    uint8 move_to_file   = 'a' + (move.to % 8);
    uint8 move_to_rank   = move.to / 8 + 1;

    char buffer[5];
    snprintf(
        buffer, sizeof(buffer), "%c%d%c%d",
        move_from_file, move_from_rank,
        move_to_file, move_to_rank
    );

    std::string result(buffer);
    switch (move.type) {
        case PROMOTION_QUEEN:  { result += "q"; } break;
        case PROMOTION_ROOK:   { result += "r"; } break;
        case PROMOTION_KNIGHT: { result += "n"; } break;
        case PROMOTION_BISHOP: { result += "b"; } break;
    }

    return result;
}

std::string ToLongString(Move move) {
    std::string result = ToString(move);
    result += "\n";
    result += "piece type: " + std::to_string(move.piece_type) + "\n";
    result += "captured piece type: " + std::to_string(move.captured_piece_type);
    return result;
}
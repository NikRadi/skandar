#include "Bitboard.hpp"


Bitboard ToBitboard(int idx) {
    return 1ull << idx;
}

bool HasPieceAt(Bitboard bitboard, int idx) {
    return ((bitboard >> idx) & 1) != 0;
}

std::string ToPrettyString(Bitboard bitboard) {
    std::string result;
    int idx = 56;
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            int bit = static_cast<int>((bitboard >> idx) & 1);
            result += std::to_string(bit);
            idx += 1;
        }

        idx -= 16;
        result += "\n";
    }

    result += "\n";
    return result;
}

std::string ToActualString(Bitboard bitboard) {
    std::string result;
    int idx = 63;
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            int bit = static_cast<int>((bitboard >> idx) & 1);
            result += std::to_string(bit);
            idx -= 1;
        }

        result += "\n";
    }

    result += "\n";
    return result;
}

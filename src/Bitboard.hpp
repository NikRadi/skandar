#ifndef SKANDAR_BITBOARD_HPP
#define SKANDAR_BITBOARD_HPP
#include <string>


typedef unsigned long long Bitboard;


Bitboard ToBitboard(int idx);

bool HasPieceAt(Bitboard bitboard, int idx);

std::string ToPrettyString(Bitboard bitboard);

std::string ToActualString(Bitboard bitboard);

#endif // SKANDAR_BITBOARD_HPP
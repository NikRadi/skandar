#ifndef SKANDAR_ZOOBRISTHASHING_HPP
#define SKANDAR_ZOOBRISTHASHING_HPP
#include "Board.hpp"


void InitZoobristHashing();

uint64 Hash(const Board &board);

#endif // SKANDAR_ZOOBRISTHASHING_HPP
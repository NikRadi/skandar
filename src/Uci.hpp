#ifndef SKANDAR_UCI_HPP
#define SKANDAR_UCI_HPP
#include "Board.hpp"


uint64 PerftRecursion(Board &board, int current_depth, int max_depth, bool print_info);

void StartUciLoop();

#endif // SKANDAR_UCI_HPP
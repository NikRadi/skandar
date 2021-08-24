#ifndef SKANDAR_SEARCH_HPP
#define SKANDAR_SEARCH_HPP
#include "Board.hpp"
#include "Move.hpp"


Move SearchForBestMove(const Board &board, int max_search_time_ms, bool print_info=true);

#endif // SKANDAR_SEARCH_HPP
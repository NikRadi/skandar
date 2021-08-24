#ifndef SKANDAR_TRANSPOSITIONTABLE_HPP
#define SKANDAR_TRANSPOSITIONTABLE_HPP
#include "Board.hpp"


struct Evaluation {
    uint64 hash;
    int depth;
    int score;
};

void InitTranspositionTable();

void ClearEvaluations();

void StoreEvaluation(const Board &board, int board_score, int depth);

Evaluation *LookupEvaluation(const Board &board, int depth);

#endif // SKANDAR_TRANSPOSITIONTABLE_HPP
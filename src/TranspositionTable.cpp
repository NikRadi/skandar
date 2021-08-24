#include "TranspositionTable.hpp"
#include "Common.hpp"
#include "ZoobristHashing.hpp"



constexpr int NUM_ENTRIES = 64000;
static Evaluation *g_evaluations;


void InitTranspositionTable() {
    g_evaluations = new Evaluation[NUM_ENTRIES];
}

void ClearEvaluations() {
    delete[] g_evaluations;
    InitTranspositionTable();
}

void StoreEvaluation(const Board &board, int board_score, int depth) {
    uint64 board_hash = Hash(board);

    Evaluation evaluation;
    evaluation.hash = board_hash;
    evaluation.depth = depth;
    evaluation.score = board_score;

    int idx = board_hash % NUM_ENTRIES;
    g_evaluations[idx] = evaluation;
}

Evaluation *LookupEvaluation(const Board &board, int depth) {
    uint64 board_hash = Hash(board);
    int idx = board_hash % NUM_ENTRIES;
    Evaluation *evaluation = &g_evaluations[idx];
    if (evaluation->hash == board_hash && evaluation->depth >= depth) {
        return evaluation;
    }

    return NULL;
}

#ifndef SKANDAR_MOVEGENERATOR_HPP
#define SKANDAR_MOVEGENERATOR_HPP
#include "Board.hpp"
#include "Move.hpp"
#include <vector>


void InitMoveGenerator();

bool IsLegalMove(const Board &board, Move move);

bool CanAttackSquare(const Board &board, ulong square);

std::vector<Move> GeneratePseudoLegalMoves(const Board &board);

std::vector<Move> GenerateMoves(Board &board);

#endif // SKANDAR_MOVEGENERATOR_HPP
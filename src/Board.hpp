#ifndef SKANDAR_BOARD_HPP
#define SKANDAR_BOARD_HPP
#include <string>
#include "Bitboard.hpp"
#include "Move.hpp"


typedef uint8 Color;

constexpr Color WHITE = 0;
constexpr Color BLACK = 1;

// Do not change the order of these
// PIECE_VALUES in Search.cpp depends on it
constexpr PieceType PAWN       = 0;
constexpr PieceType KNIGHT     = 1;
constexpr PieceType BISHOP     = 2;
constexpr PieceType ROOK       = 3;
constexpr PieceType QUEEN      = 4;
constexpr PieceType KING       = 5;
constexpr PieceType ALL_PIECES = 6;
constexpr PieceType NONE_PIECE = 7;

constexpr uint8 WHITE_CASTLE_KING  = 0b0001;
constexpr uint8 WHITE_CASTLE_QUEEN = 0b0010;
constexpr uint8 BLACK_CASTLE_KING  = 0b0100;
constexpr uint8 BLACK_CASTLE_QUEEN = 0b1000;
constexpr uint8 WHITE_CASTLING     = WHITE_CASTLE_KING | WHITE_CASTLE_QUEEN;
constexpr uint8 BLACK_CASTLING     = BLACK_CASTLE_KING | BLACK_CASTLE_QUEEN;
constexpr uint8 ALL_CASTLING       = WHITE_CASTLING | BLACK_CASTLING;


struct Board {
    Bitboard pieces[2][7] = { { 0 }, { 0 } };
    Color color_to_move = WHITE;
    uint8 castling_rights = ALL_CASTLING;
    uint8 en_passant_square = 0;
};


Board InitBoard(const std::string &fen);

Board InitBoard();

std::string ToString(const Board &board);

void MakeMove(Board &board, Move move);

void UndoMove(Board &board, Move move, uint8 castling_rights, uint8 en_passant_square);

PieceType FindPieceTypeOnSquare(const Board &board, ulong square);

bool HasCastlingRights(uint8 castling_rights, uint8 flag);

bool IsEmpty(const Board &board, uint8 square);

#endif // SKANDAR_BOARD_HPP
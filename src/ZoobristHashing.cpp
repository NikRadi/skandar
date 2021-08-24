#include "ZoobristHashing.hpp"
#include "Bitboard.hpp"
#include "Board.hpp"
#include <random>


static std::mt19937_64 g_rand;
static uint64 g_random_nums_pieces[2][6][64];
static uint64 g_random_nums_en_passant_file[8];
static uint64 g_random_nums_castling[16];
static uint64 g_random_nums_color;


void InitZoobristHashing() {
    g_rand.seed(1);
    for (int color = WHITE; color <= BLACK; ++color) {
        for (int piece = PAWN; piece <= KING; ++piece) {
            for (int square = 0; square < 64; ++square) {
                g_random_nums_pieces[color][piece][square] = g_rand();
            }
        }
    }

    for (int file = 0; file < 8; ++file) {
        g_random_nums_en_passant_file[file] = g_rand();
    }

    for (int i = 0; i < 16; ++i) {
        g_random_nums_castling[i] = g_rand();
    }

    g_random_nums_color = g_rand();
}

uint64 Hash(const Board &board) {
    uint64 hash = 0;
    for (int color = WHITE; color <= BLACK; ++color) {
        for (int piece = PAWN; piece <= KING; ++piece) {
            Bitboard pieces = board.pieces[color][piece];
            ulong square;
            while (pieces) {
                _BitScanForward64(&square, pieces);
                hash ^= g_random_nums_pieces[color][piece][square];
                pieces &= pieces - 1;
            }
        }
    }

    if (board.en_passant_square != 0) {
        hash ^= g_random_nums_en_passant_file[board.en_passant_square % 8];
    }

    hash ^= g_random_nums_castling[board.castling_rights];
    if (board.color_to_move == BLACK) {
        hash ^= g_random_nums_color;
    }

    return hash;
}

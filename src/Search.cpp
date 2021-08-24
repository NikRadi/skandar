#include "Search.hpp"
#include "MoveGenerator.hpp"
#include "Timer.hpp"
#include <algorithm>
#include <intrin.h>
#include <thread>


constexpr int PAWN_POSITION_VALUES[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
     50,  50,  50,  50,  50,  50,  50,  50,
     10,  10,  20,  30,  30,  20,  10,  10,
      5,   5,  10,  25,  25,  10,   5,   5,
      0,   0,   0,  20,  20,   0,   0,   0,
      5,  -5, -10,   0,   0, -10,  -5,   5,
      5,  10,  10, -20, -20,  10,  10,   5,
      0,   0,   0,   0,   0,   0,   0,   0
};

constexpr int KNIGHT_POSITION_VALUES[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20,   0,   0,   0,   0, -20, -40,
    -30,   0,  10,  15,  15,  10,   0, -30,
    -30,   5,  15,  20,  20,  15,   5, -30,
    -30,   0,  15,  20,  20,  15,   0, -30,
    -30,   5,  10,  15,  15,  10,   5, -30,
    -40, -20,   0,   5,   5,   0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50,
};

constexpr int BISHOP_POSITION_VALUES[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10,   0,   0,   0,   0,   0,   0, -10,
    -10,   0,   5,  10,  10,   5,   0, -10,
    -10,   5,   5,  10,  10,   5,   5, -10,
    -10,   0,  10,  10,  10,  10,   0, -10,
    -10,  10,  10,  10,  10,  10,  10, -10,
    -10,   5,   0,   0,   0,   0,   5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20,
};

constexpr int ROOK_POSITION_VALUES[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
      5,  10,  10,  10,  10,  10,  10,   5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
     -5,   0,   0,   0,   0,   0,   0,  -5,
      0,   0,   0,   5,   5,   0,   0,   0
};

constexpr int QUEEN_POSITION_VALUES[64] = {
    -20, -10, -10, -5,  -5, -10, -10, -20,
    -10,   0,   0,  0,   0,   0,   0, -10,
    -10,   0,   5,  5,   5,   5,   0, -10,
     -5,   0,   5,  5,   5,   5,   0,  -5,
      0,   0,   5,  5,   5,   5,   0,  -5,
    -10,   5,   5,  5,   5,   5,   0, -10,
    -10,   0,   5,  0,   0,   0,   0, -10,
    -20, -10, -10, -5,  -5, -10, -10, -20
};

constexpr int KING_POSITION_VALUES[64] = {
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
     20,  20,   0,   0,   0,   0,  20,  20,
     20,  30,  10,   0,   0,  10,  30,  20
};

constexpr int INFINITY_VALUE = 9999999;
constexpr int PAWN_VALUE   = 100;
constexpr int KNIGHT_VALUE = 300;
constexpr int BISHOP_VALUE = 310;
constexpr int ROOK_VALUE   = 500;
constexpr int QUEEN_VALUE  = 900;
constexpr int PIECE_VALUES[6] = {
    // Do not change the order of these
    // The order depends on variables in Board.hpp
    PAWN_VALUE,
    KNIGHT_VALUE,
    BISHOP_VALUE,
    ROOK_VALUE,
    QUEEN_VALUE,
    0 // KING
};


static int g_nodes_searched;
static bool g_has_ended_search;
static Timer g_timer;
static int64 g_max_search_time_ms;


static int CalculatePieceValues(const Board &board, Color piece_color) {
    uint64 value = 0;
    value += __popcnt64(board.pieces[piece_color][PAWN])   * PAWN_VALUE;
    value += __popcnt64(board.pieces[piece_color][KNIGHT]) * KNIGHT_VALUE;
    value += __popcnt64(board.pieces[piece_color][BISHOP]) * BISHOP_VALUE;
    value += __popcnt64(board.pieces[piece_color][ROOK])   * ROOK_VALUE;
    value += __popcnt64(board.pieces[piece_color][QUEEN])  * QUEEN_VALUE;
    return static_cast<int>(value);
}

static int CalculatePiecePositionValues(Bitboard pieces, const int *position_values, bool is_white) {
    int value = 0;
    ulong piece_square;
    while (pieces) {
        _BitScanForward64(&piece_square, pieces);
        if (is_white) {
            int file = File(piece_square);
            int rank = Rank(piece_square);
            piece_square = (7 - rank) * 8 + file;
        }

        value += position_values[piece_square];
        pieces &= pieces - 1;
    }

    return value;
}

static int CalculatePlayerScore(const Board &board, Color player_color) {
    bool is_white = player_color == WHITE;
    int player_score = CalculatePieceValues(board, player_color);
    player_score += CalculatePiecePositionValues(board.pieces[player_color][PAWN], PAWN_POSITION_VALUES, is_white);
    player_score += CalculatePiecePositionValues(board.pieces[player_color][KNIGHT], KNIGHT_POSITION_VALUES, is_white);
    player_score += CalculatePiecePositionValues(board.pieces[player_color][BISHOP], BISHOP_POSITION_VALUES, is_white);
    player_score += CalculatePiecePositionValues(board.pieces[player_color][ROOK], ROOK_POSITION_VALUES, is_white);
    player_score += CalculatePiecePositionValues(board.pieces[player_color][QUEEN], QUEEN_POSITION_VALUES, is_white);
    player_score += CalculatePiecePositionValues(board.pieces[player_color][KING], KING_POSITION_VALUES, is_white);

    return player_score;
}

static int CalculateBoardScore(const Board &board) {
    int score = CalculatePlayerScore(board, WHITE) - CalculatePlayerScore(board, BLACK);
    return (board.color_to_move == WHITE) ? score : -score;
}

static void SortMoves(std::vector<Move> &moves) {
    std::sort(moves.rbegin(), moves.rend());
}

static void ScoreAndSortMoves(std::vector<Move> &moves) {
    for (int i = 0; i < moves.size(); ++i) {
        int score = 0;
        if (moves[i].captured_piece_type != NONE_PIECE) {
            score += PIECE_VALUES[moves[i].captured_piece_type] - PIECE_VALUES[moves[i].piece_type];
        }

        if (moves[i].piece_type == PAWN) {
            switch (moves[i].type) {
                case PROMOTION_QUEEN:  { score += QUEEN_VALUE;  } break;
                case PROMOTION_ROOK:   { score += ROOK_VALUE;   } break;
                case PROMOTION_KNIGHT: { score += KNIGHT_VALUE; } break;
                case PROMOTION_BISHOP: { score += BISHOP_VALUE; } break;
            }
        }

        moves[i].score = score;
    }

    SortMoves(moves);
}

static int QuiescenceSearch(const Board &board, int alpha, int beta) {
    int score = CalculateBoardScore(board);
    if (score >= beta) {
        return beta;
    }

    g_nodes_searched += 1;
    alpha = std::max(alpha, score);
    Board board_copy = board;
    std::vector<Move> pseudo_legal_moves = GeneratePseudoLegalMoves(board_copy);
    ScoreAndSortMoves(pseudo_legal_moves);
    for (int i = 0; i < pseudo_legal_moves.size(); ++i) {
        Move move = pseudo_legal_moves[i];
        if (move.captured_piece_type == NONE_PIECE || !IsLegalMove(board_copy, move)) {
            continue;
        }

        MakeMove(board_copy, move);
        score = -QuiescenceSearch(board_copy, -beta, -alpha);
        if (score >= beta) {
            return beta;
        }

        alpha = std::max(alpha, score);
        board_copy = board;
    }

    return alpha;
}

static int NegamaxWithAlphabetaPruning(const Board &board, int depth, int alpha, int beta, bool last_move_was_capture) {
    if (depth == 0) {
        return (last_move_was_capture) ? QuiescenceSearch(board, alpha, beta) : CalculateBoardScore(board);
    }

    g_nodes_searched += 1;
    Board board_copy = board;
    std::vector<Move> pseudo_legal_moves = GeneratePseudoLegalMoves(board_copy);
    ScoreAndSortMoves(pseudo_legal_moves);
    int num_legal_moves_made = 0;
    for (int i = 0; i < pseudo_legal_moves.size(); ++i) {
        if (g_has_ended_search) {
            return alpha;
        }

        if (!IsLegalMove(board_copy, pseudo_legal_moves[i])) {
            continue;
        }

        MakeMove(board_copy, pseudo_legal_moves[i]);
        num_legal_moves_made += 1;

        int score = -NegamaxWithAlphabetaPruning(board_copy, depth - 1, -beta, -alpha, pseudo_legal_moves[i].captured_piece_type != NONE_PIECE);
        if (score >= beta) {
            return beta;
        }

        board_copy = board;
        alpha = std::max(alpha, score);
    }

    if (num_legal_moves_made == 0) {
        ulong king_square;
        _BitScanForward64(&king_square, board_copy.pieces[board.color_to_move][KING]);
        board_copy.color_to_move = !board_copy.color_to_move;
        if (CanAttackSquare(board_copy, king_square)) {
            return -INFINITY_VALUE;
        }

        return -10;
    }

    return alpha;
}

static void IterativeDeepeningSearch(const Board &board, Move *best_move, bool print_info) {
    Board board_copy = board;
    std::vector<Move> moves = GenerateMoves(board_copy);
    if (moves.size() == 1) {
        *best_move = moves[0];
        return;
    }

    ScoreAndSortMoves(moves);
    int depth = 1;
    while (!g_has_ended_search) {
        int best_score = -INFINITY_VALUE;
        for (int i = 0; i < moves.size(); ++i) {
            if (print_info && depth >= 6) {
                printf(
                    "info depth %d currmove %s currmovenumber %d\n",
                    depth,
                    ToString(moves[i]).c_str(),
                    i
                );
            }

            MakeMove(board_copy, moves[i]);
            int score = -NegamaxWithAlphabetaPruning(board_copy, depth - 1, -INFINITY_VALUE, INFINITY_VALUE, moves[i].captured_piece_type != NONE_PIECE);
            g_nodes_searched += 1;
            if (g_has_ended_search) {
                break;
            }

            moves[i].score = score;
            board_copy = board;
            if (score > best_score) {
                best_score = score;
                *best_move = moves[i];
            }
        }

        if (print_info) {
            int64 time_ms = g_timer.GetTimeElapsedMilliSeconds();
            printf(
                "info depth %d nodes %d time %llu nps %llu\n",
                depth,
                g_nodes_searched,
                time_ms,
                g_nodes_searched * 1000 / (time_ms + 1) // time_ms + 1 to avoid division with 0
            );
        }

        SortMoves(moves);
        depth += 1;
    }
}

Move SearchForBestMove(const Board &board, int max_search_time_ms, bool print_info) {
    g_nodes_searched = 0;
    g_has_ended_search = false;
    g_timer = Timer();
    g_max_search_time_ms = max_search_time_ms;

    g_timer.Start();
    Move best_move;
    std::thread thread(IterativeDeepeningSearch, board, &best_move, print_info);
    while (!g_has_ended_search) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        g_has_ended_search = g_timer.GetTimeElapsedMilliSeconds() > g_max_search_time_ms;
    }

    thread.join();
    return best_move;
}

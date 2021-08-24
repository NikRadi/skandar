#include "MoveGenerator.hpp"
#include <intrin.h>


constexpr uint8 RANK_1 = 0;
constexpr uint8 RANK_2 = 1;
constexpr uint8 RANK_3 = 2;
constexpr uint8 RANK_4 = 3;
constexpr uint8 RANK_5 = 4;
constexpr uint8 RANK_6 = 5;
constexpr uint8 RANK_7 = 6;
constexpr uint8 RANK_8 = 7;

constexpr uint8 FILE_A = 0;
constexpr uint8 FILE_B = 1;
constexpr uint8 FILE_C = 2;
constexpr uint8 FILE_D = 3;
constexpr uint8 FILE_E = 4;
constexpr uint8 FILE_F = 5;
constexpr uint8 FILE_G = 6;
constexpr uint8 FILE_H = 7;

constexpr Bitboard CLEAR_RANK[8] = {
    0b1111111111111111111111111111111111111111111111111111111100000000,
    0b1111111111111111111111111111111111111111111111110000000011111111,
    0b1111111111111111111111111111111111111111000000001111111111111111,
    0b1111111111111111111111111111111100000000111111111111111111111111,
    0b1111111111111111111111110000000011111111111111111111111111111111,
    0b1111111111111111000000001111111111111111111111111111111111111111,
    0b1111111100000000111111111111111111111111111111111111111111111111,
    0b0000000011111111111111111111111111111111111111111111111111111111,
};

constexpr Bitboard MASK_RANK[8] = {
    0b0000000000000000000000000000000000000000000000000000000011111111,
    0b0000000000000000000000000000000000000000000000001111111100000000,
    0b0000000000000000000000000000000000000000111111110000000000000000,
    0b0000000000000000000000000000000011111111000000000000000000000000,
    0b0000000000000000000000001111111100000000000000000000000000000000,
    0b0000000000000000111111110000000000000000000000000000000000000000,
    0b0000000011111111000000000000000000000000000000000000000000000000,
    0b1111111100000000000000000000000000000000000000000000000000000000,
};

constexpr Bitboard CLEAR_FILE[8] = {
    0b1111111011111110111111101111111011111110111111101111111011111110,
    0b1111110111111101111111011111110111111101111111011111110111111101,
    0b1111101111111011111110111111101111111011111110111111101111111011,
    0b1111011111110111111101111111011111110111111101111111011111110111,
    0b1110111111101111111011111110111111101111111011111110111111101111,
    0b1101111111011111110111111101111111011111110111111101111111011111,
    0b1011111110111111101111111011111110111111101111111011111110111111,
    0b0111111101111111011111110111111101111111011111110111111101111111,
};

constexpr Bitboard MASK_FILE[8] = {
    0b0000000100000001000000010000000100000001000000010000000100000001,
    0b0000001000000010000000100000001000000010000000100000001000000010,
    0b0000010000000100000001000000010000000100000001000000010000000100,
    0b0000100000001000000010000000100000001000000010000000100000001000,
    0b0001000000010000000100000001000000010000000100000001000000010000,
    0b0010000000100000001000000010000000100000001000000010000000100000,
    0b0100000001000000010000000100000001000000010000000100000001000000,
    0b1000000010000000100000001000000010000000100000001000000010000000,
};

constexpr Bitboard A1H8_DIAG = 0b1000000001000000001000000001000000001000000001000000001000000001;

constexpr Bitboard H1A8_ANTIDIAG = 0b0000000100000010000001000000100000010000001000000100000010000000;

Bitboard MASK_DIAG[64];

Bitboard MASK_ANTIDIAG[64];

uint8 FIRST_RANK_ATTACKS[8][256];


static Bitboard GenerateKingPseudoLegalMoves(Bitboard king_piece, Bitboard friendly_pieces) {
    Bitboard clip_file_a = king_piece & CLEAR_FILE[FILE_A];
    Bitboard clip_file_h = king_piece & CLEAR_FILE[FILE_H];

    Bitboard king_pseudo_legal_moves = clip_file_a << 7;
    king_pseudo_legal_moves |= king_piece << 8;
    king_pseudo_legal_moves |= clip_file_h << 9;
    king_pseudo_legal_moves |= clip_file_h << 1;

    king_pseudo_legal_moves |= clip_file_h >> 7;
    king_pseudo_legal_moves |= king_piece  >> 8;
    king_pseudo_legal_moves |= clip_file_a >> 9;
    king_pseudo_legal_moves |= clip_file_a >> 1;

    return king_pseudo_legal_moves & ~friendly_pieces;
}

static Bitboard GenerateKnightPseudoLegalMoves(Bitboard knight_piece, Bitboard friendly_pieces) {
    constexpr Bitboard clip_spot_1 = CLEAR_FILE[FILE_A] & CLEAR_FILE[FILE_B];
    constexpr Bitboard clip_spot_2 = CLEAR_FILE[FILE_A];
    constexpr Bitboard clip_spot_3 = CLEAR_FILE[FILE_H];
    constexpr Bitboard clip_spot_4 = CLEAR_FILE[FILE_H] & CLEAR_FILE[FILE_G];
    constexpr Bitboard clip_spot_5 = clip_spot_4;
    constexpr Bitboard clip_spot_6 = clip_spot_3;
    constexpr Bitboard clip_spot_7 = clip_spot_2;
    constexpr Bitboard clip_spot_8 = clip_spot_1;

    Bitboard knight_pseudo_legal_moves = (knight_piece & clip_spot_1) << 6;
    knight_pseudo_legal_moves |= (knight_piece & clip_spot_2) << 15;
    knight_pseudo_legal_moves |= (knight_piece & clip_spot_3) << 17;
    knight_pseudo_legal_moves |= (knight_piece & clip_spot_4) << 10;

    knight_pseudo_legal_moves |= (knight_piece & clip_spot_5) >> 6;
    knight_pseudo_legal_moves |= (knight_piece & clip_spot_6) >> 15;
    knight_pseudo_legal_moves |= (knight_piece & clip_spot_7) >> 17;
    knight_pseudo_legal_moves |= (knight_piece & clip_spot_8) >> 10;

    return knight_pseudo_legal_moves & ~friendly_pieces;
}

static Bitboard GenerateRookPseudoLegalMoves(ulong rook_square, Bitboard all_pieces, Bitboard non_friendly_squares) {
    int rook_file = File(rook_square);
    int rook_rank = Rank(rook_square);

    int bitshift_amount = 8 * rook_rank;
    uint8 pieces_on_rook_rank = static_cast<uint8>((all_pieces >> bitshift_amount) & MASK_RANK[RANK_1]);
    Bitboard rook_rank_moves = static_cast<Bitboard>(FIRST_RANK_ATTACKS[7 - rook_file][pieces_on_rook_rank]) << bitshift_amount;

    Bitboard pieces_on_rook_file = (all_pieces >> rook_file) & MASK_FILE[FILE_A];
    uint8 pieces_on_rook_file_mapped_to_rank_1 = (pieces_on_rook_file * A1H8_DIAG) >> 56;
    uint8 rook_file_moves_on_rank_1 = FIRST_RANK_ATTACKS[rook_rank][pieces_on_rook_file_mapped_to_rank_1];
    Bitboard rook_file_moves = ((rook_file_moves_on_rank_1 * A1H8_DIAG) & MASK_FILE[FILE_H]) >> (rook_file ^ 0b111);

    return (rook_rank_moves | rook_file_moves) & non_friendly_squares;
}

static Bitboard GenerateBishopPseudoLegalMoves(ulong bishop_square, Bitboard all_pieces, Bitboard non_friendly_squares) {
    int bishop_file = File(bishop_square);

    Bitboard pieces_on_bishop_diag = MASK_DIAG[bishop_square] & all_pieces;
    uint8 pieces_on_bishop_diag_mapped_to_rank_1 = (MASK_FILE[FILE_A] * pieces_on_bishop_diag) >> 56;
    Bitboard bishop_diag_moves_on_rank_1 = MASK_FILE[FILE_A] * FIRST_RANK_ATTACKS[7 - bishop_file][pieces_on_bishop_diag_mapped_to_rank_1];
    Bitboard bishop_diag_moves = MASK_DIAG[bishop_square] & bishop_diag_moves_on_rank_1;

    Bitboard pieces_on_bishop_antidiag = MASK_ANTIDIAG[bishop_square] & all_pieces;
    uint8 pieces_on_bishop_antidiag_mapped_to_rank_1 = (MASK_FILE[FILE_A] * pieces_on_bishop_antidiag) >> 56;
    Bitboard bishop_antidiag_moves_on_rank_1 = MASK_FILE[FILE_A] * FIRST_RANK_ATTACKS[7 - bishop_file][pieces_on_bishop_antidiag_mapped_to_rank_1];
    Bitboard bishop_antidiag_moves = MASK_ANTIDIAG[bishop_square] & bishop_antidiag_moves_on_rank_1;

    return (bishop_diag_moves | bishop_antidiag_moves) & non_friendly_squares;
}

static void AddCaptureMoves(PieceType piece_type, Bitboard capture_moves, ulong from, std::vector<Move> &moves, const Board &board) {
    ulong to;
    while (capture_moves) {
        _BitScanForward64(&to, capture_moves);
        PieceType captured_piece_type = FindPieceTypeOnSquare(board, to);
        Move move = InitMove(piece_type, from, to, CAPTURE_MOVE, captured_piece_type);
        moves.push_back(move);
        capture_moves &= capture_moves - 1;
    }
}

static void AddQuietMoves(PieceType piece_type, Bitboard quiet_moves, ulong from, std::vector<Move> &moves) {
    ulong to;
    while (quiet_moves) {
        _BitScanForward64(&to, quiet_moves);
        Move move = InitMove(piece_type, from, to);
        moves.push_back(move);
        quiet_moves &= quiet_moves - 1;
    }
}

static void AddKingPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard king_piece = board.pieces[board.color_to_move][KING];
    ASSERT(king_piece != 0);
    Bitboard friendly_pieces = board.pieces[board.color_to_move][ALL_PIECES];
    ulong from;
    _BitScanForward64(&from, king_piece);
    Bitboard king_pseudo_legal_moves = GenerateKingPseudoLegalMoves(king_piece, friendly_pieces);

    Bitboard enemy_pieces = board.pieces[!board.color_to_move][ALL_PIECES];
    Bitboard capture_moves = king_pseudo_legal_moves & enemy_pieces;
    Bitboard quiet_moves = king_pseudo_legal_moves ^ capture_moves;
    AddCaptureMoves(KING, capture_moves, from, pseudo_legal_moves, board);
    AddQuietMoves(KING, quiet_moves, from, pseudo_legal_moves);

    if (board.color_to_move == WHITE) {
        if (HasCastlingRights(board.castling_rights, WHITE_CASTLE_KING) &&
            IsEmpty(board, F1) && IsEmpty(board, G1)) {

            Move move = InitMove(KING, E1, G1, KING_SIDE_CASTLING);
            pseudo_legal_moves.push_back(move);
        }

        if (HasCastlingRights(board.castling_rights, WHITE_CASTLE_QUEEN) &&
            IsEmpty(board, B1) && IsEmpty(board, C1) && IsEmpty(board, D1)) {

            Move move = InitMove(KING, E1, C1, QUEEN_SIDE_CASTLING);
            pseudo_legal_moves.push_back(move);
        }
    }
    else {
        if (HasCastlingRights(board.castling_rights, BLACK_CASTLE_KING) &&
            IsEmpty(board, F8) && IsEmpty(board, G8)) {

            Move move = InitMove(KING, E8, G8, KING_SIDE_CASTLING);
            pseudo_legal_moves.push_back(move);
        }

        if (HasCastlingRights(board.castling_rights, BLACK_CASTLE_QUEEN) &&
            IsEmpty(board, B8) && IsEmpty(board, C8) && IsEmpty(board, D8)) {

            Move move = InitMove(KING, E8, C8, QUEEN_SIDE_CASTLING);
            pseudo_legal_moves.push_back(move);
        }
    }
}

static void AddKnightPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard knight_pieces = board.pieces[board.color_to_move][KNIGHT];
    Bitboard friendly_pieces = board.pieces[board.color_to_move][ALL_PIECES];
    Bitboard enemy_pieces = board.pieces[!board.color_to_move][ALL_PIECES];
    ulong from;
    while (knight_pieces) {
        _BitScanForward64(&from, knight_pieces);
        Bitboard knight_piece = ToBitboard(from);
        Bitboard knight_pseudo_legal_moves = GenerateKnightPseudoLegalMoves(knight_piece, friendly_pieces);

        Bitboard capture_moves = knight_pseudo_legal_moves & enemy_pieces;
        Bitboard quiet_moves = knight_pseudo_legal_moves ^ capture_moves;
        AddCaptureMoves(KNIGHT, capture_moves, from, pseudo_legal_moves, board);
        AddQuietMoves(KNIGHT, quiet_moves, from, pseudo_legal_moves);

        knight_pieces &= knight_pieces - 1;
    }
}

static void AddRookPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard rook_pieces = board.pieces[board.color_to_move][ROOK];
    Bitboard friendly_pieces = board.pieces[board.color_to_move][ALL_PIECES];
    Bitboard enemy_pieces = board.pieces[!board.color_to_move][ALL_PIECES];
    Bitboard all_pieces = friendly_pieces | enemy_pieces;

    ulong from;
    while (rook_pieces) {
        _BitScanForward64(&from, rook_pieces);
        Bitboard rook_pseudo_legal_moves = GenerateRookPseudoLegalMoves(from, all_pieces, ~friendly_pieces);

        Bitboard capture_moves = rook_pseudo_legal_moves & enemy_pieces;
        Bitboard quiet_moves = rook_pseudo_legal_moves ^ capture_moves;
        AddCaptureMoves(ROOK, capture_moves, from, pseudo_legal_moves, board);
        AddQuietMoves(ROOK, quiet_moves, from, pseudo_legal_moves);

        rook_pieces &= rook_pieces - 1;
    }
}

static void AddBishopPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard bishop_pieces = board.pieces[board.color_to_move][BISHOP];
    Bitboard friendly_pieces = board.pieces[board.color_to_move][ALL_PIECES];
    Bitboard enemy_pieces = board.pieces[!board.color_to_move][ALL_PIECES];
    Bitboard all_pieces = friendly_pieces | enemy_pieces;

    ulong from;
    while (bishop_pieces) {
        _BitScanForward64(&from, bishop_pieces);
        Bitboard bishop_pseudo_legal_moves = GenerateBishopPseudoLegalMoves(from, all_pieces, ~friendly_pieces);

        Bitboard capture_moves = bishop_pseudo_legal_moves & enemy_pieces;
        Bitboard quiet_moves = bishop_pseudo_legal_moves ^ capture_moves;
        AddCaptureMoves(BISHOP, capture_moves, from, pseudo_legal_moves, board);
        AddQuietMoves(BISHOP, quiet_moves, from, pseudo_legal_moves);

        bishop_pieces &= bishop_pieces - 1;
    }
}

static void AddQueenPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard queen_pieces = board.pieces[board.color_to_move][QUEEN];
    Bitboard friendly_pieces = board.pieces[board.color_to_move][ALL_PIECES];
    Bitboard enemy_pieces = board.pieces[!board.color_to_move][ALL_PIECES];
    Bitboard all_pieces = friendly_pieces | enemy_pieces;

    ulong from;
    while (queen_pieces) {
        _BitScanForward64(&from, queen_pieces);
        Bitboard queen_pseudo_legal_moves = GenerateBishopPseudoLegalMoves(from, all_pieces, ~friendly_pieces) | GenerateRookPseudoLegalMoves(from, all_pieces, ~friendly_pieces);

        Bitboard capture_moves = queen_pseudo_legal_moves & enemy_pieces;
        Bitboard quiet_moves = queen_pseudo_legal_moves ^ capture_moves;
        AddCaptureMoves(QUEEN, capture_moves, from, pseudo_legal_moves, board);
        AddQuietMoves(QUEEN, quiet_moves, from, pseudo_legal_moves);

        queen_pieces &= queen_pieces - 1;
    }
}

static void AddPawnEnPassantMove(const Board &board, std::vector<Move> &pseudo_legal_moves, Bitboard capture_squares, ulong from) {
    if (board.en_passant_square) {
        Bitboard en_passant_square = ToBitboard(board.en_passant_square);
        Bitboard en_passant_move = capture_squares & en_passant_square;
        if (en_passant_move) {
            ulong to;
            _BitScanForward64(&to, en_passant_move);
            Move move = InitMove(PAWN, from, to, EN_PASSANT_MOVE, PAWN);
            pseudo_legal_moves.push_back(move);
        }
    }
}

static void AddPawnPromotionMoves(Move promotion_move, std::vector<Move> &pseudo_legal_moves) {
    promotion_move.type = PROMOTION_QUEEN;
    pseudo_legal_moves.push_back(promotion_move);

    promotion_move.type = PROMOTION_ROOK;
    pseudo_legal_moves.push_back(promotion_move);

    promotion_move.type = PROMOTION_KNIGHT;
    pseudo_legal_moves.push_back(promotion_move);

    promotion_move.type = PROMOTION_BISHOP;
    pseudo_legal_moves.push_back(promotion_move);
}

static void AddWhitePawnPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard white_pawn_pieces = board.pieces[WHITE][PAWN];
    Bitboard black_pieces = board.pieces[BLACK][ALL_PIECES];
    Bitboard empty_squares = ~(black_pieces | board.pieces[WHITE][ALL_PIECES]);
    ulong from;
    while (white_pawn_pieces) {
        _BitScanForward64(&from, white_pawn_pieces);
        Bitboard white_pawn_piece = ToBitboard(from);

        Bitboard one_step_move = (white_pawn_piece << 8) & empty_squares;
        if (one_step_move) {
            Bitboard promotion_move = one_step_move & MASK_RANK[RANK_8];
            Move move = InitMove(PAWN, from, from + 8);
            if (promotion_move) {
                AddPawnPromotionMoves(move, pseudo_legal_moves);
            }
            else {
                pseudo_legal_moves.push_back(move);
            }

            Bitboard two_step_move = ((one_step_move & MASK_RANK[RANK_3]) << 8) & empty_squares;
            if (two_step_move) {
                move.to += 8;
                move.type = TWO_STEP_MOVE;
                pseudo_legal_moves.push_back(move);
            }
        }

        Bitboard capture_left = (white_pawn_piece & CLEAR_FILE[FILE_A]) << 7;
        Bitboard capture_right = (white_pawn_piece & CLEAR_FILE[FILE_H]) << 9;
        Bitboard capture_squares = capture_left | capture_right;
        Bitboard capture_moves = capture_squares & black_pieces;
        if (capture_moves) {
            Bitboard promotion_moves = capture_moves & MASK_RANK[RANK_8];
            if (promotion_moves) {
                ulong to;
                while (promotion_moves) {
                    _BitScanForward64(&to, promotion_moves);
                    Move move = InitMove(PAWN, from, to);
                    move.captured_piece_type = FindPieceTypeOnSquare(board, to);
                    AddPawnPromotionMoves(move, pseudo_legal_moves);
                    promotion_moves &= promotion_moves - 1;
                }
            }
            else {
                AddCaptureMoves(PAWN, capture_moves, from, pseudo_legal_moves, board);
            }
        }

        AddPawnEnPassantMove(board, pseudo_legal_moves, capture_squares, from);
        white_pawn_pieces &= white_pawn_pieces - 1;
    }
}

static void AddBlackPawnPseudoLegalMoves(const Board &board, std::vector<Move> &pseudo_legal_moves) {
    Bitboard black_pawn_pieces = board.pieces[BLACK][PAWN];
    Bitboard white_pieces = board.pieces[WHITE][ALL_PIECES];
    Bitboard empty_squares = ~(white_pieces | board.pieces[BLACK][ALL_PIECES]);
    ulong from;
    while (black_pawn_pieces) {
        _BitScanForward64(&from, black_pawn_pieces);
        Bitboard black_pawn_piece = ToBitboard(from);

        Bitboard one_step_move = (black_pawn_piece >> 8) & empty_squares;
        if (one_step_move) {
            Bitboard promotion_move = one_step_move & MASK_RANK[RANK_1];
            Move move = InitMove(PAWN, from, from - 8);
            if (promotion_move) {
                AddPawnPromotionMoves(move, pseudo_legal_moves);
            }
            else {
                pseudo_legal_moves.push_back(move);
            }

            Bitboard two_step_move = ((one_step_move & MASK_RANK[RANK_6]) >> 8) & empty_squares;
            if (two_step_move) {
                move.to -= 8;
                move.type = TWO_STEP_MOVE;
                pseudo_legal_moves.push_back(move);
            }
        }

        Bitboard capture_left = (black_pawn_piece & CLEAR_FILE[FILE_A]) >> 9;
        Bitboard capture_right = (black_pawn_piece & CLEAR_FILE[FILE_H]) >> 7;
        Bitboard capture_squares = capture_left | capture_right;
        Bitboard capture_moves = capture_squares & white_pieces;
        if (capture_moves) {
            Bitboard promotion_moves = capture_moves & MASK_RANK[RANK_1];
            if (promotion_moves) {
                ulong to;
                while (promotion_moves) {
                    _BitScanForward64(&to, promotion_moves);
                    Move move = InitMove(PAWN, from, to);
                    move.captured_piece_type = FindPieceTypeOnSquare(board, to);
                    AddPawnPromotionMoves(move, pseudo_legal_moves);
                    promotion_moves &= promotion_moves - 1;
                }
            }
            else {
                AddCaptureMoves(PAWN, capture_moves, from, pseudo_legal_moves, board);
            }
        }

        AddPawnEnPassantMove(board, pseudo_legal_moves, capture_squares, from);
        black_pawn_pieces &= black_pawn_pieces - 1;
    }
}

static bool CanAttackSquare(const std::vector<Move> &moves, ulong square) {
    for (int i = 0; i < moves.size(); ++i) {
        if (moves[i].to == square) {
            return true;
        }
    }

    return false;
}

static void MoveKingTo(Board &board, Color color, uint8 square) {
    Bitboard square_bb = ToBitboard(square);
    board.pieces[color][ALL_PIECES] &= ~board.pieces[color][KING];
    board.pieces[color][ALL_PIECES] |= square_bb;
    board.pieces[color][KING] = square_bb;
}

void InitMoveGenerator() {
    for (int file = 0; file < 8; ++file) {
        for (int occupation = 0; occupation <= 0xff; ++occupation) {
            uint8 square = 0b10000000 >> file;
            if ((square & occupation) == 0) {
                continue;
            }

            uint8 right_attacks = square - 1;
            uint8 right_blockers = right_attacks & occupation;
            if (right_blockers != 0) {
                ulong rightmost_blocker;
                _BitScanReverse64(&rightmost_blocker, right_blockers);
                right_attacks ^= (1 << rightmost_blocker) - 1;
            }

            uint8 left_attacks = ~square & ~(square - 1);
            uint8 left_blockers = left_attacks & occupation;
            if (left_blockers != 0) {
                ulong leftmost_blocker;
                _BitScanForward64(&leftmost_blocker, left_blockers);

                uint8 leftmost_blocker_bb = 1 << leftmost_blocker;
                left_attacks ^= ~leftmost_blocker_bb & ~(leftmost_blocker_bb - 1);
            }

            FIRST_RANK_ATTACKS[file][occupation] = right_attacks | left_attacks;
        }
    }

    for (int square = 0; square < 64; ++square) {
        int file = File(square);
        int rank_x_8 = square & 0b111000;

        int diag = 8 * file - rank_x_8;
        int diag_north = -diag & (diag >> 31);
        int diag_south = diag & (-diag >> 31);
        MASK_DIAG[square] = (A1H8_DIAG >> diag_south) << diag_north;

        int antidiag = 56 - 8 * file - rank_x_8;
        int antidiag_north = -antidiag & (antidiag >> 31);
        int antidiag_south = antidiag & (-antidiag >> 31);
        MASK_ANTIDIAG[square] = (H1A8_ANTIDIAG >> antidiag_south) << antidiag_north;
    }
}

bool IsLegalMove(const Board &board, Move move) {
    Board board_copy = board;
    switch (move.type) {
        case KING_SIDE_CASTLING: {
            ASSERT(move.from == E1 || move.from == E8);
            ASSERT(move.to == G1 || move.to == G8);
            board_copy.color_to_move = !board.color_to_move;
            if (move.from == E1) {
                if (CanAttackSquare(board_copy, E1)) return false;

                MoveKingTo(board_copy, WHITE, F1);
                if (CanAttackSquare(board_copy, F1)) return false;

                MoveKingTo(board_copy, WHITE, G1);
                return !CanAttackSquare(board_copy, G1);
            }
            else {
                if (CanAttackSquare(board_copy, E8)) return false;

                MoveKingTo(board_copy, BLACK, F8);
                if (CanAttackSquare(board_copy, F8)) return false;

                MoveKingTo(board_copy, BLACK, G8);
                return !CanAttackSquare(board_copy, G8);
            }
        } break;
        case QUEEN_SIDE_CASTLING: {
            ASSERT(move.from == E1 || move.from == E8);
            ASSERT(move.to == C1 || move.to == C8);
            board_copy.color_to_move = !board.color_to_move;
            if (move.from == E1) {
                if (CanAttackSquare(board_copy, E1)) return false;

                MoveKingTo(board_copy, WHITE, D1);
                if (CanAttackSquare(board_copy, D1)) return false;

                MoveKingTo(board_copy, WHITE, C1);
                return !CanAttackSquare(board_copy, C1);
            }
            else {
                if (CanAttackSquare(board_copy, E8)) return false;

                MoveKingTo(board_copy, BLACK, D8);
                if (CanAttackSquare(board_copy, D8)) return false;

                MoveKingTo(board_copy, BLACK, C8);
                return !CanAttackSquare(board_copy, C8);
            }
        } break;
        default: {
            MakeMove(board_copy, move);
            Bitboard king_piece = board_copy.pieces[!board_copy.color_to_move][KING];
            ulong king_square;
            _BitScanForward64(&king_square, king_piece);
            return !CanAttackSquare(board_copy, king_square);
        } break;
    }
}

bool CanAttackSquare(const Board &board, ulong square) {
    std::vector<Move> moves = GeneratePseudoLegalMoves(board);
    return CanAttackSquare(moves, square);
}

std::vector<Move> GeneratePseudoLegalMoves(const Board &board) {
    std::vector<Move> pseudo_legal_moves;
    pseudo_legal_moves.reserve(100);
    AddKingPseudoLegalMoves(board, pseudo_legal_moves);
    AddKnightPseudoLegalMoves(board, pseudo_legal_moves);
    AddRookPseudoLegalMoves(board, pseudo_legal_moves);
    AddBishopPseudoLegalMoves(board, pseudo_legal_moves);
    AddQueenPseudoLegalMoves(board, pseudo_legal_moves);
    if (board.color_to_move == WHITE) {
        AddWhitePawnPseudoLegalMoves(board, pseudo_legal_moves);
    }
    else {
        AddBlackPawnPseudoLegalMoves(board, pseudo_legal_moves);
    }

    return pseudo_legal_moves;
}

std::vector<Move> GenerateMoves(Board &board) {
    std::vector<Move> moves;
    moves.reserve(50);
    std::vector<Move> pseudo_legal_moves = GeneratePseudoLegalMoves(board);
    for (int i = 0; i < pseudo_legal_moves.size(); ++i) {
        Move move = pseudo_legal_moves[i];
        if (IsLegalMove(board, move)) {
            moves.push_back(move);
        }
    }

    return moves;
}

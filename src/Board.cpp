#include "Board.hpp"
#include "String.hpp"
#include <vector>


const char *START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


static PieceType ToPieceType(int c) {
    switch (c) {
        case 'p': return PAWN;
        case 'r': return ROOK;
        case 'n': return KNIGHT;
        case 'b': return BISHOP;
        case 'q': return QUEEN;
        case 'k': return KING;
        default: {
            ASSERT(false);
            return static_cast<PieceType>(-1);
        }
    }
}

static void MovePiece(Board &board, PieceType piece_type, uint8 from, uint8 to) {
    Bitboard from_bb = ToBitboard(from);
    Bitboard to_bb = ToBitboard(to);

    board.pieces[board.color_to_move][piece_type] &= ~from_bb;
    board.pieces[board.color_to_move][piece_type] |= to_bb;

    board.pieces[board.color_to_move][ALL_PIECES] &= ~from_bb;
    board.pieces[board.color_to_move][ALL_PIECES] |= to_bb;
}

static void RemoveEnemyPiece(Board &board, PieceType piece_type, uint8 square) {
    Color enemy_color = !board.color_to_move;
    Bitboard square_bb = ToBitboard(square);
    board.pieces[enemy_color][piece_type] &= ~square_bb;
    board.pieces[enemy_color][ALL_PIECES] &= ~square_bb;
}

static void AddEnemyPiece(Board &board, PieceType piece_type, uint8 square) {
    Color enemy_color = !board.color_to_move;
    Bitboard square_bb = ToBitboard(square);
    board.pieces[enemy_color][piece_type] |= square_bb;
    board.pieces[enemy_color][ALL_PIECES] |= square_bb;
}

static void ConvertPiece(Board &board, PieceType current_piece_type, PieceType new_piece_type, uint8 square) {
    Bitboard square_bb = ToBitboard(square);
    board.pieces[board.color_to_move][current_piece_type] &= ~square_bb;
    board.pieces[board.color_to_move][new_piece_type] |= square_bb;
}

static void PromotePawn(Board &board, Move move, PieceType promotion_type) {
    ConvertPiece(board, PAWN, promotion_type, move.to);
    if (move.captured_piece_type != NONE_PIECE) {
        RemoveEnemyPiece(board, move.captured_piece_type, move.to);
    }
}

static void UndoPawnPromotion(Board &board, Move move, PieceType promotion_type) {
    ConvertPiece(board, promotion_type, PAWN, move.to);
    if (move.captured_piece_type != NONE_PIECE) {
        AddEnemyPiece(board, move.captured_piece_type, move.to);
    }
}

Board InitBoard(const std::string &fen) {
    Board board;
    std::vector<std::string> fen_parts = Split(fen, ' ');
    std::string fen_board             = fen_parts[0];
    std::string fen_color_to_move     = fen_parts[1];
    std::string fen_castling_rights   = fen_parts[2];
    std::string fen_en_passant_square = fen_parts[3];

    int idx = A8;
    for (int i = 0; i < fen_board.length(); ++i) {
        char c = fen_board[i];
        if (c == '/') {
            idx = idx - (idx % 8) - 16;
        }
        else if (IsDigit(c)) {
            idx += ToInt(c);
        }
        else {
            Color piece_color = (IsUpper(c)) ? WHITE : BLACK;
            PieceType piece_type = ToPieceType(tolower(c));
            Bitboard piece_bb = ToBitboard(idx);
            board.pieces[piece_color][piece_type] |= piece_bb;
            board.pieces[piece_color][ALL_PIECES] |= piece_bb;
            idx += 1;
        }
    }

    board.color_to_move = (fen_color_to_move[0] == 'w') ? WHITE : BLACK;
    board.castling_rights = 0;
    if (fen_castling_rights[0] != '-') {
        for (int i = 0; i < fen_castling_rights.length(); ++i) {
            switch (fen_castling_rights[i]) {
                case 'K': { board.castling_rights |= WHITE_CASTLE_KING; } break;
                case 'Q': { board.castling_rights |= WHITE_CASTLE_QUEEN; } break;
                case 'k': { board.castling_rights |= BLACK_CASTLE_KING; } break;
                case 'q': { board.castling_rights |= BLACK_CASTLE_QUEEN; } break;
            }
        }
    }

    if (fen_en_passant_square[0] != '-') {
        int file = fen_en_passant_square[0] - 'a';
        int rank = fen_en_passant_square[1] - '1';
        board.en_passant_square = static_cast<uint8>(file * 8 + rank);
        ASSERT((A3 <= board.en_passant_square && board.en_passant_square <= H3) ||
               (A6 <= board.en_passant_square && board.en_passant_square <= H6));
    }

    return board;
}

Board InitBoard() {
    return InitBoard(START_FEN);
}

std::string ToString(const Board &board) {
    std::string result = "";
    result += "turn to move: ";
    result += (board.color_to_move == WHITE) ? "w\n" : "b\n";

    result += "en passant square: ";
    result += (board.en_passant_square == 0) ? "-" : std::to_string(board.en_passant_square);
    result += "\n";

    result += "castling rights: ";
    if (board.castling_rights == 0) result += "-\n";
    else {
        if (board.castling_rights & WHITE_CASTLE_KING) result += "K";
        if (board.castling_rights & WHITE_CASTLE_QUEEN) result += "Q";
        if (board.castling_rights & BLACK_CASTLE_KING) result += "k";
        if (board.castling_rights & BLACK_CASTLE_QUEEN) result += "q";
        result += "\n";
    }

    int idx = A8;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (HasPieceAt(board.pieces[WHITE][ALL_PIECES], idx)) {
                if      (HasPieceAt(board.pieces[WHITE][PAWN], idx))   result += "P";
                else if (HasPieceAt(board.pieces[WHITE][ROOK], idx))   result += "R";
                else if (HasPieceAt(board.pieces[WHITE][KNIGHT], idx)) result += "N";
                else if (HasPieceAt(board.pieces[WHITE][BISHOP], idx)) result += "B";
                else if (HasPieceAt(board.pieces[WHITE][QUEEN], idx))  result += "Q";
                else if (HasPieceAt(board.pieces[WHITE][KING], idx))   result += "K";
            }
            else if (HasPieceAt(board.pieces[BLACK][ALL_PIECES], idx)) {
                if      (HasPieceAt(board.pieces[BLACK][PAWN], idx))   result += "p";
                else if (HasPieceAt(board.pieces[BLACK][ROOK], idx))   result += "r";
                else if (HasPieceAt(board.pieces[BLACK][KNIGHT], idx)) result += "n";
                else if (HasPieceAt(board.pieces[BLACK][BISHOP], idx)) result += "b";
                else if (HasPieceAt(board.pieces[BLACK][QUEEN], idx))  result += "q";
                else if (HasPieceAt(board.pieces[BLACK][KING], idx))   result += "k";
            }
            else {
                result += ".";
            }

            idx += 1;
        }

        idx -= 16;
        if (idx >= 0) {
            result += "\n";
        }
    }

    return result;
}

void MakeMove(Board &board, Move move) {
    board.en_passant_square = 0;
    switch (move.piece_type) {
        case KING: {
            board.castling_rights &= (board.color_to_move == WHITE) ? ~WHITE_CASTLING : ~BLACK_CASTLING;
        } break;
        case ROOK: {
            if (board.color_to_move == WHITE) {
                if (move.from == A1)      board.castling_rights &= ~WHITE_CASTLE_QUEEN;
                else if (move.from == H1) board.castling_rights &= ~WHITE_CASTLE_KING;
            }
            else {
                if (move.from == A8)      board.castling_rights &= ~BLACK_CASTLE_QUEEN;
                else if (move.from == H8) board.castling_rights &= ~BLACK_CASTLE_KING;
            }
        } break;
    }

    MovePiece(board, move.piece_type, move.from, move.to);
    switch (move.type) {
        case CAPTURE_MOVE: {
            RemoveEnemyPiece(board, move.captured_piece_type, move.to);
            ASSERTM(move.captured_piece_type != KING, "%s\n%s", ToLongString(move).c_str(), ToString(board).c_str());
            if (move.captured_piece_type == ROOK) {
                if      (move.to == A1) board.castling_rights &= ~WHITE_CASTLE_QUEEN;
                else if (move.to == H1) board.castling_rights &= ~WHITE_CASTLE_KING;
                else if (move.to == A8) board.castling_rights &= ~BLACK_CASTLE_QUEEN;
                else if (move.to == H8) board.castling_rights &= ~BLACK_CASTLE_KING;
            }
        } break;
        case KING_SIDE_CASTLING: {
            if (board.color_to_move == WHITE) MovePiece(board, ROOK, H1, F1);
            else MovePiece(board, ROOK, H8, F8);
        } break;
        case QUEEN_SIDE_CASTLING: {
            if (board.color_to_move == WHITE) MovePiece(board, ROOK, A1, D1);
            else MovePiece(board, ROOK, A8, D8);
        } break;
        case TWO_STEP_MOVE: {
            if (board.color_to_move == WHITE) board.en_passant_square = move.to - 8;
            else board.en_passant_square = move.to + 8;
        } break;
        case EN_PASSANT_MOVE: {
            if (board.color_to_move == WHITE) RemoveEnemyPiece(board, PAWN, move.to - 8);
            else RemoveEnemyPiece(board, PAWN, move.to + 8);
        } break;
        case PROMOTION_QUEEN:  { PromotePawn(board, move, QUEEN);  } break;
        case PROMOTION_ROOK:   { PromotePawn(board, move, ROOK);   } break;
        case PROMOTION_KNIGHT: { PromotePawn(board, move, KNIGHT); } break;
        case PROMOTION_BISHOP: { PromotePawn(board, move, BISHOP); } break;
    }

    ASSERTM(
        (board.pieces[board.color_to_move][KING] & (board.pieces[board.color_to_move][KING] - 1)) == 0,
        "player shouldn't have two kings\n%s\n%s",
        ToLongString(move).c_str(),
        ToString(board).c_str()
    );

    board.color_to_move = !board.color_to_move;
}

void UndoMove(Board &board, Move move, uint8 castling_rights, uint8 en_passant_square) {
    board.color_to_move = !board.color_to_move;
    switch (move.type) {
        case CAPTURE_MOVE: {
            AddEnemyPiece(board, move.captured_piece_type, move.to);
        } break;
        case KING_SIDE_CASTLING: {
            if (board.color_to_move == WHITE) MovePiece(board, ROOK, F1, H1);
            else MovePiece(board, ROOK, F8, H8);
        } break;
        case QUEEN_SIDE_CASTLING: {
            if (board.color_to_move == WHITE) MovePiece(board, ROOK, D1, A1);
            else MovePiece(board, ROOK, D8, A8);
        } break;
        case EN_PASSANT_MOVE: {
            if (board.color_to_move == WHITE) AddEnemyPiece(board, PAWN, move.to - 8);
            else AddEnemyPiece(board, PAWN, move.to + 8);
            board.en_passant_square = move.to;
        } break;
        case PROMOTION_QUEEN:  { UndoPawnPromotion(board, move, QUEEN);  } break;
        case PROMOTION_ROOK:   { UndoPawnPromotion(board, move, ROOK);   } break;
        case PROMOTION_KNIGHT: { UndoPawnPromotion(board, move, KNIGHT); } break;
        case PROMOTION_BISHOP: { UndoPawnPromotion(board, move, BISHOP); } break;
    }

    MovePiece(board, move.piece_type, move.to, move.from);
    board.castling_rights = castling_rights;
    board.en_passant_square = en_passant_square;
}

PieceType FindPieceTypeOnSquare(const Board &board, ulong square) {
    Bitboard square_bb = ToBitboard(square);
    for (PieceType piece_type = PAWN; piece_type <= KING; ++piece_type) {
        if ((board.pieces[WHITE][piece_type] & square_bb) > 0 || (board.pieces[BLACK][piece_type] & square_bb) > 0) {
            return piece_type;
        }
    }

    ASSERTM(false, "square: %d", square);
    return NONE_PIECE;
}

bool HasCastlingRights(uint8 castling_rights, uint8 flag) {
    return (castling_rights & flag) != 0;
}

bool IsEmpty(const Board &board, uint8 square) {
    Bitboard square_bb = ToBitboard(square);
    bool has_no_white_pieces = (board.pieces[WHITE][ALL_PIECES] & square_bb) == 0;
    bool has_no_black_pieces = (board.pieces[BLACK][ALL_PIECES] & square_bb) == 0;
    return has_no_white_pieces && has_no_black_pieces;
}

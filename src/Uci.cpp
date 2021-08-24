#include "Uci.hpp"
#include "Common.hpp"
#include "MoveGenerator.hpp"
#include "Search.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


static int g_max_search_time_ms = 4000;


static void Perft(Board &board, int depth) {
    uint64 nodes_searched = (depth <= 0) ? 1 : PerftRecursion(board, depth, depth, true);
    printf("\n");
    printf("Total nodes searched: %llu\n\n", nodes_searched);
}

static void Moves(Board &board, std::istringstream &stream) {
    std::string token;
    while (stream >> token) {
        std::vector<Move> moves = GenerateMoves(board);
        bool has_move_been_found = false;
        for (int i = 0; i < moves.size(); ++i) {
            Move move = moves[i];
            if (ToString(move) == token) {
                MakeMove(board, move);
                has_move_been_found = true;
                break;
            }
        }

        if (!has_move_been_found) {
            printf("error: didn't fint move %s\n\n", token.c_str());
            ASSERT(false);
            break;
        }
    }
}

static void Position(Board &board, std::istringstream &stream) {
    std::string token;
    stream >> token;
    board = InitBoard();
    if (token == "startpos") {
        stream >> token; // Consume "moves"
        Moves(board, stream);
    }
    else if (token == "fen") {
        stream >> token;
        std::string fen = token;
        while (stream >> token) {
            if (token == "moves") {
                break;
            }

            fen += " " + token;
        }

        board = InitBoard(fen);
        Moves(board, stream);
    }
}

static void Go(Board &board, std::istringstream &stream) {
    std::string token;
    stream >> token;
    if (token == "perft") {
        int depth;
        stream >> depth;
        Perft(board, depth);
    }
    else {
        Move move = SearchForBestMove(board, g_max_search_time_ms);
        printf("bestmove %s\n\n", ToString(move).c_str());
    }
}

static void Time(std::istringstream &stream) {
    int max_search_time_ms;
    stream >> max_search_time_ms;
    g_max_search_time_ms = max_search_time_ms;
}

uint64 PerftRecursion(Board &board, int current_depth, int max_depth, bool print_info) {
    if (current_depth == 0) {
        return 1ull;
    }

    std::vector<Move> moves = GenerateMoves(board);
    uint64 nodes_searched = 0;
    for (int i = 0; i < moves.size(); ++i) {
        Move move = moves[i];
        Board board_copy = board;
        MakeMove(board_copy, move);
        uint64 sub_nodes_searched = PerftRecursion(board_copy, current_depth - 1, max_depth, print_info);
        nodes_searched += sub_nodes_searched;
        if (current_depth == max_depth && print_info) {
            printf("%s: %llu\n", ToString(move).c_str(), sub_nodes_searched);
        }
    }

    return nodes_searched;
}

void StartUciLoop() {
    Board board = InitBoard();
    std::string cmd;
    std::string token;
    while (std::getline(std::cin, cmd)) {
        std::istringstream stream(cmd);
        stream >> token;
        if      (token == "position")   Position(board, stream);
        else if (token == "go")         Go(board, stream);
        else if (token == "uci")        printf("id name Skandar\nid author Slik\n\n");
        else if (token == "isready")    printf("readyok\n\n");
        else if (token == "ucinewgame") continue;
        else if (token == "quit")       break;
        else if (token == "time")       Time(stream);
        else if (!token.empty())        printf("Invalid command: %s\n\n", cmd.c_str());
    }
}

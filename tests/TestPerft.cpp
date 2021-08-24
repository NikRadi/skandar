#include <stdio.h>
#include <string>
#include "..\src\Board.hpp"
#include "..\src\MoveGenerator.hpp"
#include "..\src\Timer.hpp"
#include "..\src\Uci.hpp"


static float g_total_time_elapsed_s = 0.0f;
static int g_num_tests = 0;
static int g_num_failed_tests = 0;


static void TestPerft(int depth, uint64 expected_nodes_searched, std::string fen) {
    g_num_tests += 1;
    Timer timer;
    Board board = InitBoard(fen);

    timer.Start();
    uint64 nodes_searched = PerftRecursion(board, depth, depth, false);
    int64 time_elapsed_ms = timer.GetTimeElapsedMilliSeconds();
    float time_elapsed_s = static_cast<float>(time_elapsed_ms) / 1000.0f;
    g_total_time_elapsed_s += time_elapsed_s;

    printf("depth: %d   result: %8llu   time: %.3fs", depth, nodes_searched, time_elapsed_s);
    if (nodes_searched != expected_nodes_searched) {
        g_num_failed_tests += 1;
        printf(" FAIL - expected %lld", expected_nodes_searched);
    }

    printf("\n");
}

int main() {
    InitMoveGenerator();

    TestPerft(1,      20, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    TestPerft(2,     400, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    TestPerft(3,    8902, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    TestPerft(4,  197281, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    TestPerft(5, 4865609, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    printf("\n");
    TestPerft(1,      48, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    TestPerft(2,    2039, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    TestPerft(3,   97862, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    TestPerft(4, 4085603, "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    printf("\n");
    TestPerft(1,       14, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    TestPerft(2,      191, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    TestPerft(3,     2812, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    TestPerft(4,    43238, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    TestPerft(5,   674624, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    TestPerft(6, 11030083, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
    printf("\n");
    TestPerft(1,        6, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    TestPerft(2,      264, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    TestPerft(3,     9467, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    TestPerft(4,   422333, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    TestPerft(5, 15833292, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    printf("\n");
    TestPerft(1,        6, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    TestPerft(2,      264, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    TestPerft(3,     9467, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    TestPerft(4,   422333, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    TestPerft(5, 15833292, "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    printf("\n");
    TestPerft(1,      44, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    TestPerft(2,    1486, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    TestPerft(3,   62379, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    TestPerft(4, 2103487, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    printf("\n");
    TestPerft(1,      46, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    TestPerft(2,    2079, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    TestPerft(3,   89890, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    TestPerft(4, 3894594, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    printf("\n");
    TestPerft(1,      24, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    TestPerft(2,     496, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    TestPerft(3,    9483, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    TestPerft(4,  182838, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    TestPerft(5, 3605103, "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
    printf("\n");
    printf("Total time elapsed: %.3fs\n", g_total_time_elapsed_s);
    printf("%d of %d tests failed\n", g_num_failed_tests, g_num_tests);
}
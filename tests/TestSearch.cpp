#include <stdio.h>
#include <string>
#include "..\src\Board.hpp"
#include "..\src\Search.hpp"
#include "..\src\Timer.hpp"


static float g_total_time_elapsed_s = 0.0f;
static int g_num_tests = 0;
static int g_num_failed_tests = 0;


static void TestSearch(std::string fen, std::string expected_move_str) {
    g_num_tests += 1;
    Timer timer;
    Board board = InitBoard(fen);

    timer.Start();
    Move move = SearchForBestMove(board, false);
    int64 time_elapsed_ms = timer.GetTimeElapsedMilliSeconds();
    float time_elapsed_s = static_cast<float>(time_elapsed_ms) / 1000.0f;
    g_total_time_elapsed_s += time_elapsed_s;

    std::string move_str = ToString(move).c_str();
    printf("result: %s   time: %.3fs", move_str.c_str(), time_elapsed_s);
    if (move_str != expected_move_str) {
        g_num_failed_tests += 1;
        printf(" FAIL - expected %s", expected_move_str.c_str());
    }

    printf("\n");
}

int main() {
    TestSearch("r1bqkb1r/ppp1pppp/2Bp4/4P3/6n1/5N2/PPPP1PPP/RNBQK2R b KQkq - 0 1", "b7c6");
    printf("\n");
    printf("Total time elapsed: %.3fs\n", g_total_time_elapsed_s);
    printf("%d of %d tests failed\n", g_num_failed_tests, g_num_tests);
}

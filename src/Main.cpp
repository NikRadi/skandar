#include "Uci.hpp"
#include "MoveGenerator.hpp"
#include "TranspositionTable.hpp"
#include "ZoobristHashing.hpp"
#include <iostream>


int main() {
    std::cout << std::endl; // Is required for some reason to communicate with Arena
    InitMoveGenerator();
    // InitTranspositionTable();
    // InitZoobristHashing();
    StartUciLoop();
}
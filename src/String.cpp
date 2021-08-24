#include "String.hpp"
#include <sstream>


std::vector<std::string> Split(const std::string &str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream stream(str);
    std::string segment;
    while (std::getline(stream, segment, delimiter)) {
        result.push_back(segment);
    }

    return result;
}

std::string ToString(std::string str) {
    return str;
}

int ToInt(char c) {
    return c - '0';
}

bool IsDigit(char c) {
    return '0' <= c && c <= '9';
}

bool IsUpper(char c) {
    return 'A' <= c && c <= 'Z';
}

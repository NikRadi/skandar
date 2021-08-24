#ifndef SKANDAR_STRING_HPP
#define SKANDAR_STRING_HPP
#include <string>
#include <vector>


std::vector<std::string> Split(const std::string &str, char delimiter);

std::string ToString(std::string str);

std::string ToString(int i);

int ToInt(char c);

bool IsDigit(char c);

bool IsUpper(char c);

#endif // SKANDAR_STRING_HPP
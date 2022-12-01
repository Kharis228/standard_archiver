#pragma once

#include <vector>
#include <string>

using Oper = void(const std::vector<std::string>&);

class Parser {
public:
    Parser(int argc, char** argv);

    std::pair<Oper*, std::vector<std::string>> GetParsed();

private:
    std::vector<std::string> strs_;
};
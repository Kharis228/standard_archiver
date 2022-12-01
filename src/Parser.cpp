#include <map>
#include "Parser.h"
#include "Exceptions.h"

extern const std::map<std::string, Oper*> PARAMETERS_GOTO;

Parser::Parser(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        strs_.push_back(argv[i]);
    }
}

std::pair<Oper*, std::vector<std::string>> Parser::GetParsed() {
    if (strs_.empty()) {
        throw EmptyException();
    }
    auto opt = strs_[0];
    strs_.erase(strs_.begin());
    auto it = PARAMETERS_GOTO.find(opt);
    if (it == PARAMETERS_GOTO.end()) {
        throw OptionException();
    }
    return {it->second, strs_};
}
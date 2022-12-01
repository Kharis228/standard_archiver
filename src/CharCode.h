#pragma once
#include <cstdint>
#include <vector>

using Char9b = u_int16_t;
using Code = std::vector<bool>;

enum Symbols {
    FILENAME_END = static_cast<int16_t>(256),
    ONE_MORE_FILE = static_cast<int16_t>(257),
    ARCHIVE_END = static_cast<int16_t>(258)
};
constexpr Char9b MaxSymbols = 259;

struct CharCode {
    Char9b ch;
    Code code;
};
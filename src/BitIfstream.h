#pragma once
#include <fstream>
#include <vector>
using Code = std::vector<bool>;

class BitIfstream {
public:
    BitIfstream();
    explicit BitIfstream(const std::string& file);

    void Open(const std::string& file);
    void Close();
    bool GetBit();
    Code GetBits(int64_t cnt);

private:
    std::ifstream fin_;
    u_char last_char_;
    int last_bit_;
};
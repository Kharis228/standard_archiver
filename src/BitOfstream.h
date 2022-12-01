#pragma once
#include <fstream>
#include <vector>
using Code = std::vector<bool>;

class BitOfstream {
public:
    BitOfstream();
    explicit BitOfstream(const std::string& file);

    void Open(const std::string& file);
    void Close();
    void WriteBit(bool bit);
    void WriteBits(const Code& bits);

private:
    std::ofstream fout_;
    u_char last_char_;
    int last_bit_;
};
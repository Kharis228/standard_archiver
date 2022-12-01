#include "BitIfstream.h"

BitIfstream::BitIfstream() : last_char_(0), last_bit_(-1) {
}
BitIfstream::BitIfstream(const std::string& file) : BitIfstream() {
    Open(file);
}

void BitIfstream::Open(const std::string& file) {
    fin_.open(file, std::ios::binary);
}
void BitIfstream::Close() {
    fin_.close();
}

bool BitIfstream::GetBit() {
    if (last_bit_ == -1) {
        last_char_ = fin_.get();
        last_bit_ = 7;
    }
    return (last_char_ >> last_bit_--) & 1;
}
Code BitIfstream::GetBits(int64_t cnt) {
    Code res;
    res.reserve(cnt);
    for (int64_t i = 0; i < cnt; ++i) {
        res.push_back(GetBit());
    }
    return res;
}

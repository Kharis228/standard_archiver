#include "BitOfstream.h"

BitOfstream::BitOfstream() : last_char_(0), last_bit_(7) {
}
BitOfstream::BitOfstream(const std::string& file) : BitOfstream() {
    Open(file);
}
void BitOfstream::Open(const std::string& file) {
    fout_.open(file, std::ios::binary);
}
void BitOfstream::Close() {
    if (last_bit_ != 7) {
        fout_.put(static_cast<char>(last_char_));
    }
    fout_.close();
}
void BitOfstream::WriteBit(bool bit) {
    if (last_bit_ == -1) {
        fout_.put(static_cast<char>(last_char_));
        last_bit_ = 7;
        last_char_ = 0;
    }
    last_char_ |= static_cast<char>(bit) << last_bit_--;
}
void BitOfstream::WriteBits(const Code& bits) {
    for (auto i : bits) {
        WriteBit(i);
    }
}

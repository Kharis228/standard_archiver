#include <string_view>
#include <vector>
#include <fstream>
#include <queue>
#include <iostream>
#include "Trie.h"
#include "BitOfstream.h"
#include "Pack.h"
#include "Heap.h"

std::string Packer::FileName(const std::string& file) {
    auto pos = file.find_last_of('/');
    return file.substr(pos == file.npos ? 0 : pos + 1);
}

void Packer::PrintChar9b(Char9b a) {
    Code res;
    for (int i = 8; i >= 0; --i) {
        res.push_back(a >> i & 1);
    }
    fout_.WriteBits(res);
}

std::vector<Code> Packer::CodesForChars() {
    std::vector<Code> code(MaxSymbols);
    for (const auto& sym : symbols_) {
        code[sym.ch] = sym.code;
    }
    return code;
}

std::vector<Char9b> Packer::CodeSizeCnt() {
    std::vector<Char9b> cnt(symbols_.back().code.size());
    for (const auto& sym : symbols_) {
        ++cnt[sym.code.size() - 1];
    }
    return cnt;
}

void Packer::PrintSymbols() {
    PrintChar9b(symbols_.size());
    for (const auto& i : symbols_) {
        PrintChar9b(i.ch);
    }
}

void Packer::PrintCodeSizeCnt() {
    for (auto i : CodeSizeCnt()) {
        PrintChar9b(i);
    }
}

void Packer::PrintFileName(const std::string& file, const std::vector<Code>& code) {
    for (u_char c : FileName(file)) {
        fout_.WriteBits(code[c]);
    }
}

void Packer::PrintFileContent(const std::string& file, const std::vector<Code>& code) {
    fout_.WriteBits(code[FILENAME_END]);
    std::ifstream fin(file, std::ios::binary);  // if (!fin.is_open)) ...

    while (fin.peek(), !fin.eof()) {
        fout_.WriteBits(code[fin.get()]);
    }
    fin.close();
}

void Packer::PrintFile(const std::string& file, bool next_file) {
    std::vector<Code> code = CodesForChars();
    PrintFileName(file, code);
    PrintFileContent(file, code);
    fout_.WriteBits(code[next_file ? ONE_MORE_FILE : ARCHIVE_END]);
}

void Packer::PrintPack(const std::string& file, bool next_file) {
    PrintSymbols();
    PrintCodeSizeCnt();
    PrintFile(file, next_file);
}

void Packer::CanonicalHuffman(Trie trie) {
    symbols_ = trie.Bypass();
    sort(symbols_.begin(), symbols_.end(), [](const CharCode& u, const CharCode& v) {
        if (u.code.size() != v.code.size()) {
            return u.code.size() < v.code.size();
        }
        return u.ch < v.ch;
    });
    Code code;
    for (auto& sym : symbols_) {
        while (sym.code.size() > code.size()) {
            code.push_back(false);
        }
        sym.code = code;
        NextCode(code);
    }
}

std::vector<int64_t> Packer::FindFrequency(const std::string& file) {
    std::vector<int64_t> frequency(MaxSymbols);
    frequency[FILENAME_END] = frequency[ONE_MORE_FILE] = frequency[ARCHIVE_END] = 1;

    for (u_char c : FileName(file)) {
        ++frequency[c];
    }
    std::ifstream fin(file, std::ios::binary);
    while (fin.peek(), !fin.eof()) {
        ++frequency[fin.get()];
    }
    fin.close();
    return frequency;
}

Trie Packer::BuildHuffmanTrie(const std::string& file) {
    auto lambda = [](const Trie& u, const Trie& v) {
        auto uu = u.GetRoot();
        auto vv = v.GetRoot();
        if (uu->cnt != vv->cnt) {
            return uu->cnt < vv->cnt;
        }
        return uu->min_char < vv->min_char;
    };
    Heap<Trie> pq(lambda);

    std::vector<int64_t> frequency = FindFrequency(file);

    for (Char9b i = 0; i < MaxSymbols; ++i) {
        if (frequency[i] > 0) {
            pq.Push(Trie(i, frequency[i]));
        }
    }

    while (pq.Size() > 1) {
        Trie u = pq.Top();
        pq.Pop();
        Trie v = pq.Top();
        pq.Pop();
        pq.Push(Trie(u, v));
    }
    return pq.Top();
}

void Packer::Pack() {
    fout_.Open(archive_name_);
    for (size_t i = 0; i < files_.size(); ++i) {
        const std::string& file = files_[i];
        Trie trie = BuildHuffmanTrie(file);
        CanonicalHuffman(trie);
        PrintPack(file, i + 1 < files_.size());
    }
    fout_.Close();
}

Packer::Packer(const std::string& archive_name, const std::vector<std::string>& files)
    : archive_name_(archive_name), files_(files) {
}

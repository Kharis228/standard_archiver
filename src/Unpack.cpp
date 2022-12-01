#include <fstream>
#include "Unpack.h"
#include "Parser.h"

Char9b Unpacker::GetChar9b() {
    auto bits = fin_.GetBits(9);
    Char9b res = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        res |= bits[i] ? (1 << (8 - i)) : 0;
    }
    return res;
}

std::vector<Char9b> Unpacker::GetSymbols() {
    size_t symbols_count = GetChar9b();
    std::vector<Char9b> symbols(symbols_count);
    for (size_t i = 0; i < symbols_count; ++i) {
        symbols[i] = GetChar9b();
    }
    return symbols;
}

Trie Unpacker::GetTrie(const std::vector<Char9b>& symbols) {
    Trie trie(false);
    size_t sym = 0;
    Code code;
    do {
        code.push_back(false);
        size_t cnt = GetChar9b();
        for (size_t i = 0; i < cnt; ++i) {
            trie.AddTerminal(code, symbols[sym++]);
            NextCode(code);
        }
    } while (sym < symbols.size());
    return trie;
}

std::string Unpacker::GetFileName(Trie trie) {
    std::string file;
    Char9b ch = 0;
    while ((ch = trie.GetSymbol(fin_)) != FILENAME_END) {
        file += ch;
    }
    return file;
}

Char9b Unpacker::PrintFile(const std::string& file, Trie trie) {
    Char9b ch = 0;
    std::ofstream fout(file, std::ios::binary);
    while ((ch = trie.GetSymbol(fin_)) != ONE_MORE_FILE && ch != ARCHIVE_END) {
        fout.put(static_cast<char>(ch));
    }
    fout.close();
    return ch;
}

void Unpacker::Unpack() {

    fin_.Open(archive_name_);
    while (true) {
        std::vector<Char9b> symbols = GetSymbols();
        Trie trie = GetTrie(symbols);
        std::string file = GetFileName(trie);
        if (PrintFile(file, trie) == ARCHIVE_END) {
            break;
        }
    }
    fin_.Close();
}

Unpacker::Unpacker(const std::string& archive_name) : archive_name_(archive_name) {
}

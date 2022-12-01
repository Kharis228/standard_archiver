#pragma once

#include <string_view>
#include <vector>
#include "Archiver.h"
#include "Trie.h"

class Unpacker : Archiver {
public:
    explicit Unpacker(const std::string& archive_name);

    void Unpack();
    Char9b PrintFile(const std::string& file, Trie trie);
    std::string GetFileName(Trie trie);
    Trie GetTrie(const std::vector<Char9b>& symbols);
    std::vector<Char9b> GetSymbols();
    Char9b GetChar9b();

private:
    const std::string archive_name_;
    BitIfstream fin_;
};
#pragma once

#include <string>
#include <vector>
#include "Archiver.h"
#include "Trie.h"
#include "BitOfstream.h"

class Packer : Archiver {
public:
    Packer(const std::string& archive_name, const std::vector<std::string>& files);

    void Pack();
    Trie BuildHuffmanTrie(const std::string& file);
    std::vector<int64_t> FindFrequency(const std::string& file);
    void CanonicalHuffman(Trie trie);
    void PrintPack(const std::string& file, bool next_file);

    void PrintCodeSizeCnt();

    void PrintSymbols();
    void PrintFile(const std::string& file, bool next_file);
    void PrintFileName(const std::string& file, const std::vector<Code>& code);
    void PrintFileContent(const std::string& file, const std::vector<Code>& code);

    std::vector<Code> CodesForChars();
    std::vector<Char9b> CodeSizeCnt();
    void PrintChar9b(Char9b a);
    std::string FileName(const std::string& file);

private:
    const std::string archive_name_;
    const std::vector<std::string> files_;
    BitOfstream fout_;
    std::vector<CharCode> symbols_;
};
#pragma once
#include <memory>
#include "CharCode.h"
#include "BitIfstream.h"

class Trie {
public:
    struct Node {
        Char9b min_char;
        int64_t cnt;
        bool terminal;
        std::shared_ptr<Node> l = nullptr;
        std::shared_ptr<Node> r = nullptr;

        explicit Node(bool terminal = true);
        Node(Char9b c, int64_t cnt);
        Node(Char9b c, int64_t cnt, std::shared_ptr<Node> l, std::shared_ptr<Node> r);

        void Bypass(Code& cur, std::vector<CharCode>& chars);
    };
    explicit Trie(bool terminal = true);
    Trie(Char9b c, int64_t cnt);
    Trie(const Trie l, const Trie r);

    const Node* GetRoot() const;

    std::vector<CharCode> Bypass();
    void AddTerminal(const Code& code, Char9b ch);
    Char9b GetSymbol(BitIfstream& fin);

private:
    std::shared_ptr<Node> root_ = nullptr;
};
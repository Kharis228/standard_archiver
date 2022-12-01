#include <iostream>
#include "Trie.h"

Trie::Node::Node(bool terminal) : min_char(0), cnt(0), terminal(terminal), l(nullptr), r(nullptr) {
}
Trie::Node::Node(Char9b c, int64_t cnt) : min_char(c), cnt(cnt), terminal(true), l(nullptr), r(nullptr) {
}
Trie::Node::Node(Char9b c, int64_t cnt, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
    : min_char(c), cnt(cnt), terminal(false), l(l), r(r) {
}

void Trie::Node::Bypass(Code& cur, std::vector<CharCode>& chars) {
    if (terminal) {
        chars.push_back({.ch = min_char, .code = cur});
        return;
    }
    cur.push_back(false);
    l->Bypass(cur, chars);
    cur.pop_back();

    cur.push_back(true);
    r->Bypass(cur, chars);
    cur.pop_back();
}

Trie::Trie(bool terminal) : root_(new Node(terminal)) {
}
Trie::Trie(Char9b c, int64_t cnt) : root_(new Node(c, cnt)) {
}
Trie::Trie(const Trie l, const Trie r) {
    auto ll = l.root_;
    auto rr = r.root_;
    root_ = std::shared_ptr<Node>(new Node(std::min(ll->min_char, rr->min_char), ll->cnt + rr->cnt, ll, rr));
}

std::vector<CharCode> Trie::Bypass() {
    Code tmp;
    std::vector<CharCode> res;
    root_->Bypass(tmp, res);
    return res;
}
void Trie::AddTerminal(const Code& code, Char9b ch) {
    auto node = root_;
    for (u_char i : code) {
        if (!i) {
            node = node->l ? node->l : node->l = std::shared_ptr<Node>(new Node(false));
        } else {
            node = node->r ? node->r : node->r = std::shared_ptr<Node>(new Node(false));
        }
    }
    node->min_char = ch;
    node->terminal = true;
}
Char9b Trie::GetSymbol(BitIfstream& fin) {
    auto node = root_;
    while (!node->terminal) {
        node = !fin.GetBit() ? node->l : node->r;
    }
    return node->min_char;
}
const Trie::Node* Trie::GetRoot() const {
    return root_.get();
}

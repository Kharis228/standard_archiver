#pragma once

#include <memory>
#include <vector>
template <typename V>
class Heap {
public:
    struct Node {
        V val;

        Node() {
        }
        explicit Node(const V& val) : val(val) {
        }
        Node(const Node& a) : val(a.val) {
        }
        Node& operator=(const Node& a) {
            val = a.val;
            return *this;
        }
    };

    explicit Heap(bool (*over)(const V&, const V&) = [](const V& u, const V& v) { return u > v; })
        : over_(over), heap_({Node()}) {
    }

    size_t Size() {
        return heap_.size() - 1;
    }
    const V& Top() {
        return heap_[1].val;
    }
    void Pop() {
        std::swap(heap_[1], heap_.back());
        heap_.pop_back();
        for (size_t v = 1;;) {
            size_t to = v;
            for (size_t ch = v * 2; ch <= v * 2 + 1; ++ch) {
                to = (ch < heap_.size() && over_(heap_[ch].val, heap_[to].val)) ? ch : to;
            }
            std::swap(heap_[v], heap_[to]);
            if (v == to) {
                break;
            }
            v = to;
        }
    }

    void Push(const V val) {
        heap_.push_back(Node(val));
        size_t v = Size();
        while (v > 1 && over_(heap_[v].val, heap_[v / 2].val)) {
            std::swap(heap_[v], heap_[v / 2]);
            v = v / 2;
        }
    }

private:
    bool (*over_)(const V& u, const V& v);
    std::vector<Node> heap_;
};
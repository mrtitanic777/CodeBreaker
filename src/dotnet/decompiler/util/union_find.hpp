// dotnet/decompiler/util/union_find.hpp -- union-find (disjoint sets) over values, with path compression and union by rank.
//
//     UnionFind<ILVariable*> uf;  uf.merge(a, b);  uf.find(b) == uf.find(a);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Util: UnionFind<T>), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

namespace cb::dotnet::decompiler::util {

template <class T, class Hash = std::hash<T>, class Eq = std::equal_to<T>>
class UnionFind {
public:
    T find(const T& element) { return find_root(get_node(element))->value; }
    void merge(const T& a, const T& b) {
        Node* root_a = find_root(get_node(a));
        Node* root_b = find_root(get_node(b));
        if (root_a == root_b) return;
        if (root_a->rank < root_b->rank) {
            root_a->parent = root_b;
        } else if (root_a->rank > root_b->rank) {
            root_b->parent = root_a;
        } else {
            root_b->parent = root_a;
            ++root_a->rank;
        }
    }

private:
    struct Node {
        explicit Node(T v) : value(std::move(v)) { parent = this; }
        int rank = 0;
        Node* parent;
        T value;
    };
    Node* get_node(const T& element) {
        auto it = mapping_.find(element);
        if (it == mapping_.end()) it = mapping_.emplace(element, std::make_unique<Node>(element)).first;
        return it->second.get();
    }
    Node* find_root(Node* node) {
        if (node->parent != node) node->parent = find_root(node->parent);
        return node->parent;
    }
    std::unordered_map<T, std::unique_ptr<Node>, Hash, Eq> mapping_;
};

}  // namespace cb::dotnet::decompiler::util

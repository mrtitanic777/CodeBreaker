// java/decompiler/dominators.hpp -- immediate dominators of a graph (blocks or statements), found by iterating over the nodes in reverse post order until
// nothing changes; a node with no single dominator (a root, or where two trees meet) is its own.
//
//     GenericDominatorEngine<BasicBlock*> engine(graph.reverse_post_order(), {graph.first()}, [](BasicBlock* b) { return b->predecessors(); });
//     engine.is_dominator(block, handler);
//
// Ported from Fernflower (de.fernflower.modules.decompiler.decompose.GenericDominatorEngine), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0
// (see NOTICE in this folder).
#pragma once

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

#include "java/decompiler/keyed_list.hpp"

namespace cb::java::decompiler {

template <class Node>
class GenericDominatorEngine {
public:
    using Preds = std::function<std::vector<Node>(Node)>;

    GenericDominatorEngine(std::vector<Node> reverse_post_order, std::vector<Node> roots, Preds preds)
        : order_(std::move(reverse_post_order)), roots_(std::move(roots)), preds_(std::move(preds)) {
        for (Node n : order_) idoms_.add_with_key(Node{}, n);
        calc_idoms();
    }

    // Whether `dom` dominates `node`. std::runtime_error when the dominators are inconsistent.
    bool is_dominator(Node node, Node dom) const {
        while (node != dom) {
            const Node idom = idoms_.get_with_key(node);
            if (idom == node) return false;   // a root or a merging point
            if (idom == Node{}) throw std::runtime_error("Inconsistent idom sequence discovered!");
            node = idom;
        }
        return true;
    }
    const KeyedList<Node, Node>& ordered_idoms() const { return idoms_; }

private:
    Node common_idom(Node n1, Node n2) const {
        if (n1 == Node{}) return n2;
        if (n2 == Node{}) return n1;
        std::size_t i1 = idoms_.index_by_key(n1), i2 = idoms_.index_by_key(n2);
        while (i1 != i2) {
            if (i1 > i2) {
                const Node old = n1;
                n1 = idoms_.get_with_key(n1);
                if (old == n1) return Node{};
                i1 = idoms_.index_by_key(n1);
            } else {
                const Node old = n2;
                n2 = idoms_.get_with_key(n2);
                if (old == n2) return Node{};
                i2 = idoms_.index_by_key(n2);
            }
        }
        return n1;
    }

    void calc_idoms() {
        for (;;) {
            bool changed = false;
            for (Node node : order_) {
                Node idom{};
                if (std::find(roots_.begin(), roots_.end(), node) == roots_.end()) {
                    for (Node pred : preds_(node)) {
                        if (idoms_.get_with_key(pred) != Node{}) {
                            idom = common_idom(idom, pred);
                            if (idom == Node{}) break;   // no idom: where two trees meet
                        }
                    }
                }
                if (idom == Node{}) idom = node;
                const auto old = idoms_.put_with_key(idom, node);
                if (!old || *old != idom) changed = true;
            }
            if (!changed) break;
        }
    }

    std::vector<Node> order_, roots_;
    Preds preds_;
    KeyedList<Node, Node> idoms_;
};

}  // namespace cb::java::decompiler

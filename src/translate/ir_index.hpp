// translate/ir_index.hpp -- the derived state of an L2 tree, for a back end that reads a lowered method the way its own IR is read:
// each node's parent, each variable's uses, each label's block and jumps (ILAST_TO_L2.md section 2.8). The tree states none of this
// (a lowering stores only what is declared); a TreeIndex computes it on demand, and keeps it current through its own rewrites.
//
//     ir::TreeIndex index(method_body);
//     index.place(node).parent;                 // the node that holds it, and at which of its children
//     index.uses(sym.get()).stores.size();      // the stores, loads and address-takings of a variable, in preorder
//     index.incoming(label);                    // the jumps to a block (a region's entry block counts its region's entry, as ILSpy does)
//     index.replace(old, now);                  // a rewrite: the tree changes and what the index says is computed again when next asked
//
// Uses are classified by where a Name stands: the target of a plain Assign is a store (a compound one, with an `op` or an `eval`,
// reads it: a load), the operand of a Unary `&` an address (unless the `&` takes the address of a value, NodeFlag::RValueAddr: then
// the operand is read), any other Name a load; a Local, a Catch, a PatternMatch with a variable and a Fixed's Local are stores of
// their symbol. A symbol whose value on entry is used (`Sym::entry_used`) counts one store more, as ILSpy's StoreCount does.
//
// Language-neutral: any lane's tree has parents and uses; the jump counts are those of a Region of Labeled blocks (a lowered bytecode,
// a binary), and nothing for a tree without one.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "translate/ir.hpp"

namespace cb::translate::ir {

class TreeIndex {
public:
    explicit TreeIndex(Node* root) : root_(root) {}

    Node* root() const { return root_; }

    // Where a node stands: the node holding it (null for the root, or a node not in the tree) and its index in `children(parent)`.
    struct Place {
        Node* parent = nullptr;
        std::uint32_t index = 0;
    };
    Place place(const Node* node);
    bool contains(const Node* node);

    // A variable's uses, in preorder. `stores` holds the nodes that store it (an Assign, a Local, a Catch, a PatternMatch).
    struct Uses {
        std::vector<Node*> loads;
        std::vector<Node*> stores;
        std::vector<Node*> addresses;
    };
    const Uses& uses(const Sym* sym);
    std::size_t store_count(const Sym* sym);   // the stores, and one for a used entry value
    bool is_single_definition(const Sym* sym) { return store_count(sym) == 1 && uses(sym).addresses.empty(); }

    // The Labeled block a label names (null for none), the jumps to it, and the Breaks and Returns that leave the region whose entry it is.
    Node* block(std::string_view label);
    std::uint32_t incoming(std::string_view label);
    std::uint32_t leaving(std::string_view label);

    // Rewrites. Each changes the tree and marks the index stale: the next question recomputes it (lazily: a run of rewrites costs one
    // recomputation). `replace` puts `now` where `old` stands (`old` is detached; it may sit under `now`); `insert_stmt` puts `stmt` at
    // `at` in a Block's or BlockExpr's statements.
    void replace(Node* old, Node* now);
    void insert_stmt(Node* block, std::size_t at, Node* stmt);
    // For a rewrite made some other way (a symbol's uses changed, a slot set directly).
    void invalidate() { ++generation_; }
    // Counts the changes made through this index: a reader that caches an answer keeps the generation it read it at.
    std::uint64_t generation() const { return generation_; }

private:
    void build();
    void ensure() {
        if (built_ != generation_ + 1) build();
    }

    struct Jumps {
        Node* block = nullptr;
        std::uint32_t incoming = 0;
        std::uint32_t leaving = 0;
    };
    Node* root_;
    std::uint64_t generation_ = 0;
    std::uint64_t built_ = 0;   // generation + 1 of the last build (0: never built)
    std::unordered_map<const Node*, Place> places_;
    std::unordered_map<const Sym*, Uses> uses_;
    std::unordered_map<std::string, Jumps> jumps_;
};

}  // namespace cb::translate::ir

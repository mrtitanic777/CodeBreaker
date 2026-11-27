// translate/ir_index.cpp -- see ir_index.hpp.
#include "translate/ir_index.hpp"

#include <algorithm>
#include <stdexcept>

namespace cb::translate::ir {

namespace ir_index_detail {

// The entry label of a function's body region (what its Returns leave), or empty.
const std::string* body_entry(const Node* body) {
    while (body != nullptr) {
        if (const auto* e = get<ExprStmt>(body)) body = e->expr;
        else break;
    }
    const auto* region = get<Region>(body);
    if (region == nullptr || region->blocks.empty()) return nullptr;
    const auto* first = get<Labeled>(region->blocks.front());
    return first != nullptr && first->label ? &*first->label : nullptr;
}

const Node* function_body(const Node& n) {
    if (const auto* m = get<MethodDecl>(&n)) return std::holds_alternative<Node*>(m->body) ? std::get<Node*>(m->body) : nullptr;
    if (const auto* l = get<Lambda>(&n)) return l->body;
    return nullptr;
}

}  // namespace ir_index_detail

void TreeIndex::build() {
    places_.clear();
    uses_.clear();
    jumps_.clear();
    built_ = generation_ + 1;
    if (root_ == nullptr) return;
    struct Item {
        Node* node;
        Node* parent;
        std::uint32_t index;
        const std::string* returns_leave;   // the entry label of the innermost function's body region
    };
    std::vector<Item> stack{{root_, nullptr, 0, nullptr}};
    std::vector<Node*> kids;
    while (!stack.empty()) {
        const Item item = stack.back();
        stack.pop_back();
        Node* n = item.node;
        places_.emplace(n, Place{item.parent, item.index});
        const std::string* returns_leave = item.returns_leave;
        if (const Node* body = ir_index_detail::function_body(*n)) returns_leave = ir_index_detail::body_entry(body);

        if (const auto* name = get<Name>(n); name != nullptr && name->sym) {
            Uses& u = uses_[name->sym.get()];
            const Node* p = item.parent;
            // (a compound assignment, an `op` or an `eval`, reads its target: a load)
            if (const auto* a = get<Assign>(p); a != nullptr && a->target == n && !a->op && !a->eval) u.stores.push_back(item.parent);
            else if (const auto* un = get<Unary>(p); un != nullptr && un->op == "&" && !has_flag(*p, NodeFlag::RValueAddr)) u.addresses.push_back(item.parent);
            else u.loads.push_back(n);
        } else if (const auto* local = get<Local>(n); local != nullptr && local->sym) {
            uses_[local->sym.get()].stores.push_back(n);
        } else if (const auto* c = get<Catch>(n); c != nullptr && c->sym) {
            uses_[c->sym.get()].stores.push_back(n);
        } else if (const auto* pm = get<PatternMatch>(n); pm != nullptr && pm->var) {
            uses_[pm->var.get()].stores.push_back(n);
        } else if (const auto* labeled = get<Labeled>(n); labeled != nullptr && labeled->label) {
            Jumps& j = jumps_[*labeled->label];
            j.block = n;
            // the entry block of a region is entered once by the region itself
            if (const auto* r = get<Region>(item.parent); r != nullptr && !r->blocks.empty() && r->blocks.front() == n) ++j.incoming;
        } else if (const auto* g = get<Goto>(n); g != nullptr && g->label) {
            ++jumps_[*g->label].incoming;
        } else if (const auto* b = get<Break>(n); b != nullptr && b->label) {
            ++jumps_[*b->label].leaving;
        } else if (is<Return>(n) && returns_leave != nullptr) {
            ++jumps_[*returns_leave].leaving;
        }

        kids = children(*n);
        for (std::size_t i = kids.size(); i-- > 0;) stack.push_back({kids[i], n, static_cast<std::uint32_t>(i), returns_leave});
    }
}

TreeIndex::Place TreeIndex::place(const Node* node) {
    ensure();
    auto it = places_.find(node);
    return it != places_.end() ? it->second : Place{};
}

bool TreeIndex::contains(const Node* node) {
    ensure();
    return places_.contains(node);
}

const TreeIndex::Uses& TreeIndex::uses(const Sym* sym) {
    static const Uses kNone;
    ensure();
    auto it = uses_.find(sym);
    return it != uses_.end() ? it->second : kNone;
}

std::size_t TreeIndex::store_count(const Sym* sym) { return uses(sym).stores.size() + (sym != nullptr && sym->entry_used ? 1 : 0); }

Node* TreeIndex::block(std::string_view label) {
    ensure();
    auto it = jumps_.find(std::string(label));
    return it != jumps_.end() ? it->second.block : nullptr;
}

std::uint32_t TreeIndex::incoming(std::string_view label) {
    ensure();
    auto it = jumps_.find(std::string(label));
    return it != jumps_.end() ? it->second.incoming : 0;
}

std::uint32_t TreeIndex::leaving(std::string_view label) {
    ensure();
    auto it = jumps_.find(std::string(label));
    return it != jumps_.end() ? it->second.leaving : 0;
}

void TreeIndex::replace(Node* old, Node* now) {
    if (old == root_) {
        root_ = now;
        invalidate();
        return;
    }
    const Place p = place(old);
    if (p.parent == nullptr) throw std::logic_error("ir::TreeIndex::replace: the node is not in the tree");
    bool done = false;
    visit(*p.parent, [&](auto& k) {
        k.each_child([&](Node*& child) {
            if (!done && child == old) {
                child = now;
                done = true;
            }
        });
    });
    invalidate();
}

void TreeIndex::insert_stmt(Node* block, std::size_t at, Node* stmt) {
    std::vector<Node*>* stmts = nullptr;
    if (auto* b = get<Block>(block)) stmts = &b->stmts;
    else if (auto* e = get<BlockExpr>(block)) stmts = &e->stmts;
    if (stmts == nullptr) throw std::logic_error("ir::TreeIndex::insert_stmt: not a Block or BlockExpr");
    if (at > stmts->size()) throw std::logic_error("ir::TreeIndex::insert_stmt: past the end");
    stmts->insert(stmts->begin() + static_cast<std::ptrdiff_t>(at), stmt);
    invalidate();
}

}  // namespace cb::translate::ir

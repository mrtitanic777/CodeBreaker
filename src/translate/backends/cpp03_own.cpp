// translate/backends/cpp03_own.cpp -- the no-GC ownership rules of the C++03 back end: Java's collector, by hand. A throwaway
// owned scalar is freed where it is replaced, a container whose elements nobody keeps is freed on scope exit, a temporary passed to
// a callee that only reads it is freed after the call (see cpp03.hpp).

#include <format>

#include "core/text.hpp"
#include "java/emit/ownership.hpp"
#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

using cb::java::ledger::Category;
using cb::java::ledger::Severity;

namespace {
namespace own_impl {

// The chain of ancestors of a node, nearest first: [0] is its parent.
class Ancestors {
public:
    explicit Ancestors(const std::vector<const ir::Node*>& stack) : stack_(stack) {}
    std::size_t size() const { return stack_.size(); }
    bool empty() const { return stack_.empty(); }
    const ir::Node* operator[](std::size_t k) const { return stack_[stack_.size() - 1 - k]; }

private:
    const std::vector<const ir::Node*>& stack_;
};

// Every node under `node`, pre-order, with its ancestors (the children of a node are read after the visitor has run on it).
template <class F> void with_ancestors(const ir::Node* node, std::vector<const ir::Node*>& stack, F& visit) {
    visit(node, Ancestors(stack));
    stack.push_back(node);
    for (const ir::Node* child : ir::children(*node)) with_ancestors(child, stack, visit);
    stack.pop_back();
}

// The ancestor chains of every reference to the local `sym` in `stmts`.
std::vector<std::vector<const ir::Node*>> uses(std::span<const ir::Node* const> stmts, const ir::Sym& sym) {
    std::vector<std::vector<const ir::Node*>> out;
    std::vector<const ir::Node*> stack;
    auto visit = [&](const ir::Node* n, const Ancestors& chain) {
        const auto* nm = ir::get<ir::Name>(n);
        if (nm != nullptr && nm->sym && nm->sym->name == sym.name) {   // by name: a lowered use has its own Symbol
            std::vector<const ir::Node*> nearest_first;
            for (std::size_t k = 0; k < chain.size(); ++k) nearest_first.push_back(chain[k]);
            out.push_back(std::move(nearest_first));
        }
    };
    for (const ir::Node* s : stmts) with_ancestors(s, stack, visit);
    return out;
}

// A container use that lets no element outlive it: x.size() / x.isEmpty(), or x.get(i) used only as the receiver of a call or a
// field read (through casts and parentheses).
bool transient_use(const std::vector<const ir::Node*>& chain) {
    if (chain.empty()) return false;
    const auto* c0 = ir::get<ir::Call>(chain[0]);
    if (c0 == nullptr || !c0->sym) return false;
    const std::string& name = c0->sym->name;
    if (name == "size" || name == "isEmpty") return true;
    if (name != "get") return false;
    std::size_t k = 1;
    while (k < chain.size() && (ir::is<ir::Paren>(chain[k]) || ir::is<ir::Cast>(chain[k]))) ++k;
    if (k >= chain.size()) return false;
    const ir::Node* up = chain[k];
    const ir::Node* below = chain[k - 1];
    if (const auto* uc = ir::get<ir::Call>(up); uc != nullptr && uc->target == below) return true;
    if (const auto* us = ir::get<ir::Select>(up)) return us->obj == below;
    return false;
}

// `create...`, `make...`, `alloc...`, `clone...` followed by a letter: a factory's name.
bool factory_name(std::string_view name) {
    for (const std::string_view prefix : {"create", "make", "alloc", "clone"}) {
        if (name.starts_with(prefix) && name.size() > prefix.size()) {
            const char c = name[prefix.size()];
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        }
    }
    return false;
}

}  // namespace own_impl
}  // namespace

const std::set<std::string, std::less<>>& Writer::owned_scalar() {
    if (!owned_scalar_) {
        std::set<std::string, std::less<>> scalars;
        if (const cb::java::ir::JClass* parsed = ctx_.parsed_class(cname_)) {
            for (const auto& [field, is_array] : cb::java::emit::ownership::owned_pointers(*parsed, r_)) {
                if (!is_array) scalars.insert(field);
            }
        }
        owned_scalar_ = std::move(scalars);
    }
    return *owned_scalar_;
}

void Writer::free_owned_reassign(const ir::Node* e_, Lines& out, std::string_view ind) {
    const auto* asg = ir::get<ir::Assign>(strip_paren(e_));
    if (!owning_ || asg == nullptr || asg->op) return;
    const auto* tgt = ir::get<ir::Select>(asg->target);
    if (!(tgt != nullptr && ir::is<ir::This>(tgt->obj) && tgt->sym && owned_scalar().contains(tgt->sym->name))) return;
    const ir::Node* v = asg->value;
    while (true) {   // through parentheses and casts
        if (const auto* p = ir::get<ir::Paren>(v)) v = p->x;
        else if (const auto* c = ir::get<ir::Cast>(v)) v = c->x;
        else break;
    }
    bool fresh = ir::is<ir::New>(v);
    if (!fresh) {
        if (const auto* vc = ir::get<ir::Call>(v)) fresh = ir::is<ir::TypeName>(vc->target) && vc->sym && own_impl::factory_name(vc->sym->name);
    }
    const std::string freed = std::format("delete this->{};", tgt->sym->name);
    if (fresh && !(!out.empty() && text::trim(out.back()) == freed)) {
        out.push_back(std::format("{}delete this->{};   // J2X no-gc-reassign-leak: free the previous owned {}", ind, tgt->sym->name,
                                  tgt->sym->name));
    }
}

void Writer::free_owned_return(const ir::Local& s, std::string_view ct, std::string_view name, Lines& out, std::string_view ind) {
    static const Regex owned_container(R"(^(JList|JSet)\s*<\s*([A-Za-z_][\w:]*)\s*\*\s*>$)");
    const auto m = owned_container.match(ct);
    const auto* ic = ir::get<ir::Call>(strip_paren(s.init));
    if (!owning_ || !m || ic == nullptr || !ic->sym || !r_.data().own_catalog.return_methods.contains(ic->sym->name)) return;
    if (!s.sym) return;
    for (const auto& chain : own_impl::uses(after_, *s.sym)) {
        if (!own_impl::transient_use(chain)) return;
    }
    out.push_back(std::format("{}J2X_ScopedFreeElems<{}<{}*> > _j2xfree_{}({});   // J2X no-gc-return-leak: free the heap elements {}() "
                              "handed back (caller-frees; Java GC'd them)",
                              ind, m->group(1), m->group(2), name, name, ic->sym->name));
}

void Writer::hoist_owned_args(const ir::Node* root, Lines& out, std::string_view ind) {
    if (!owning_) return;
    const cb::java::types::OwnershipCatalog& cat = r_.data().own_catalog;
    if (cat.scalar_methods.empty()) return;
    // (inner call, outer call, argument position) for each owned-scalar builder call passed directly as an argument of a call to a
    // method of the program that does not retain that position -- and evaluated unconditionally (not under ?:, && or ||).
    std::vector<const ir::Node*> stack;
    auto visit = [&](const ir::Node* n, const own_impl::Ancestors& chain) {
        const auto* inner = ir::get<ir::Call>(n);
        if (!(inner != nullptr && inner->sym && cat.scalar_methods.contains(inner->sym->name))) return;
        if (chain.empty()) return;
        const auto* outer = ir::get<ir::Call>(chain[0]);
        if (outer == nullptr || !outer->sym) return;
        std::optional<std::size_t> pos;
        for (std::size_t i = 0; i < outer->args.size(); ++i) {
            if (outer->args[i] == n) {
                pos = i;
                break;
            }
        }
        if (!pos) return;
        if (!cat.corpus_methods.contains(outer->sym->name)) return;
        if (const auto it = cat.retained_params.find(outer->sym->name); it != cat.retained_params.end() && it->second.contains(*pos)) return;
        for (std::size_t k = 0; k < chain.size(); ++k) {
            const auto* b = ir::get<ir::Binary>(chain[k]);
            if (ir::is<ir::Cond>(chain[k]) || (b != nullptr && (b->op == "&&" || b->op == "||")) || ir::is<ir::Lambda>(chain[k])) return;
        }
        const std::string& elem = cat.scalar_methods.at(inner->sym->name);
        const auto [code, ct] = expr(n);
        const int k = ++arg_temps_;
        out.push_back(std::format("{}{}* _j2xarg_{} = {};", ind, elem, k, code));
        out.push_back(std::format("{}J2X_ScopedDelete<{} > _j2xargd_{}(_j2xarg_{});   // J2X no-gc-arg-leak: free the heap {} that {}() "
                                  "returned ({}() only reads it)",
                                  ind, elem, k, k, elem, inner->sym->name, outer->sym->name));
        subst_[n] = Code{std::format("_j2xarg_{}", k), ct};
    };
    own_impl::with_ancestors(root, stack, visit);
}

void Writer::review(const ir::Node* e, Lines& out, std::string_view ind) {
    std::optional<std::string> reflective;
    detail::any_node(e, [&](const ir::Node* n) {
        const auto* c = ir::get<ir::Call>(n);
        if (c != nullptr && c->sym && detail::reflective_methods().contains(c->sym->name) && !ctx_.is_game(c->sym->owner)) {
            reflective = c->sym->name;
            return true;
        }
        return false;
    });
    if (!reflective) return;
    out.push_back(std::format("{}// J2X-REVIEW[reflection-unsupported]: {}() has no C++ equivalent -- the runtime stub returns NULL. Replace "
                              "with a hand-written factory (a class-keyed switch / registry), then null-check the result.",
                              ind, *reflective));
    note(Category::Risk, Severity::High, "reflection-unsupported",
         std::format("reflection {}() has no C++ equivalent -- stub returns NULL; needs a hand-written factory "
                     "(J2X-REVIEW[reflection-unsupported])",
                     *reflective));
}

std::string Writer::cond(const ir::Node* e) { return expr(strip_paren(e)).code; }

}  // namespace cb::translate::backends::cpp03

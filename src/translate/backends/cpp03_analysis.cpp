// translate/backends/cpp03_analysis.cpp -- the questions the C++03 back end's writers ask of a tree: does evaluating this change
// anything, in what order must its operands be evaluated, does this statement leave normally (see cpp03.hpp).

#include "translate/backends/cpp03_internal.hpp"

namespace cb::translate::backends::cpp03 {

using detail::any_node;
using detail::for_each_node;

const ir::Node* strip_paren(const ir::Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

bool has_unsupported(const ir::Node* root) {
    return any_node(root, [](const ir::Node* n) { return ir::is<ir::Unsupported>(n); });
}

bool has_effects(const ir::Node* n) {
    // (an array's allocation is not observable: its elements are walked like any operand)
    return any_node(n, [](const ir::Node* x) {
        if (ir::is<ir::Assign>(x) || ir::is<ir::New>(x)) return true;
        if (const auto* u = ir::get<ir::Unary>(x)) return u->op == "++" || u->op == "--";
        if (const auto* c = ir::get<ir::Call>(x)) return !(c->sym && c->sym->owner && detail::pure_owners().contains(*c->sym->owner));
        return false;
    });
}

std::set<std::string, std::less<>> names_written(const ir::Node* n) {
    std::set<std::string, std::less<>> out;
    for_each_node(n, [&](const ir::Node* x) {
        const ir::Node* target = nullptr;
        if (const auto* a = ir::get<ir::Assign>(x)) target = a->target;
        else if (const auto* u = ir::get<ir::Unary>(x)) {
            if (u->op == "++" || u->op == "--") target = u->x;
        }
        target = strip_paren(target);
        if (const auto* nm = ir::get<ir::Name>(target)) {
            if (nm->sym) out.insert(nm->sym->name);
        } else if (const auto* sel = ir::get<ir::Select>(target)) {
            if (sel->sym) out.insert(sel->sym->name);
        }
    });
    return out;
}

std::set<std::string, std::less<>> names_read(const ir::Node* n) {
    std::set<std::string, std::less<>> out;
    for_each_node(n, [&](const ir::Node* x) {
        if (const auto* nm = ir::get<ir::Name>(x)) {
            if (nm->sym) out.insert(nm->sym->name);
        } else if (const auto* sel = ir::get<ir::Select>(x)) {
            if (sel->sym) out.insert(sel->sym->name);
        }
    });
    return out;
}

bool order_matters(std::span<const ir::Node* const> nodes) {
    std::vector<const ir::Node*> with_effects;
    for (const ir::Node* n : nodes) {
        if (has_effects(n)) with_effects.push_back(n);
    }
    if (with_effects.size() >= 2) return true;
    if (with_effects.size() == 1) {   // one operand writes what another reads
        const auto written = names_written(with_effects[0]);
        if (written.empty()) return false;
        for (const ir::Node* n : nodes) {
            if (n == with_effects[0]) continue;
            for (const std::string& name : names_read(n)) {
                if (written.contains(name)) return true;
            }
        }
    }
    return false;
}

bool is_constant(const ir::Node* n_) {
    const ir::Node* n = strip_paren(n_);
    if (ir::is<ir::Lit>(n) || ir::is<ir::ClassLit>(n) || ir::is<ir::This>(n) || ir::is<ir::TypeName>(n)) return true;
    const auto* sel = ir::get<ir::Select>(n);
    return sel != nullptr && ir::is<ir::TypeName>(sel->obj) && sel->sym && sel->sym->kind == ir::SymKind::EnumConst;
}

std::vector<const ir::Node*> operands(const ir::Node* e) {
    std::vector<const ir::Node*> out;
    if (const auto* c = ir::get<ir::Call>(e)) {
        if (c->target != nullptr && !(ir::is<ir::This>(c->target) || ir::is<ir::Super>(c->target) || ir::is<ir::TypeName>(c->target)))
            out.push_back(c->target);
        out.insert(out.end(), c->args.begin(), c->args.end());
    } else if (const auto* n = ir::get<ir::New>(e)) {
        out.insert(out.end(), n->args.begin(), n->args.end());
    } else if (const auto* b = ir::get<ir::Binary>(e)) {
        if (b->op != "&&" && b->op != "||") {
            out.push_back(b->left);
            out.push_back(b->right);
        }
    } else if (const auto* cat = ir::get<ir::Concat>(e)) {
        out.insert(out.end(), cat->parts.begin(), cat->parts.end());
    } else if (const auto* a = ir::get<ir::Assign>(e)) {
        if (const auto* ix = ir::get<ir::Index>(strip_paren(a->target))) {   // a[i] = v: the index before the value
            out.push_back(detail::single(ix->index));
            out.push_back(a->value);
        }
    }
    return out;
}

bool is_abrupt(const ir::Node* s) {
    if (ir::is<ir::Return>(s) || ir::is<ir::Throw>(s)) return true;
    if (const auto* b = ir::get<ir::Block>(s)) return !b->stmts.empty() && is_abrupt(b->stmts.back());
    if (const auto* i = ir::get<ir::If>(s)) {
        if (std::holds_alternative<std::vector<ir::Node*>>(i->orelse) || std::get<ir::Node*>(i->orelse) == nullptr) return false;
        const ir::Node* then = std::holds_alternative<ir::Node*>(i->then) ? std::get<ir::Node*>(i->then) : nullptr;
        return is_abrupt(then) && is_abrupt(std::get<ir::Node*>(i->orelse));
    }
    if (const auto* t = ir::get<ir::Try>(s)) {
        if (!is_abrupt(t->body)) return false;
        for (const ir::Node* c : t->catches) {
            const auto* ct = ir::get<ir::Catch>(c);
            if (ct == nullptr || !is_abrupt(ct->body)) return false;
        }
        return true;
    }
    return false;
}

namespace analysis_impl {

bool leaves_walk(const ir::Node* n, int loops, const std::set<std::string, std::less<>>& labels) {
    if (n == nullptr) return false;
    if (ir::is<ir::Return>(n)) return true;
    if (const auto* b = ir::get<ir::Break>(n)) return b->label && !b->label->empty() ? !labels.contains(*b->label) : loops == 0;
    if (const auto* c = ir::get<ir::Continue>(n)) return c->label && !c->label->empty() ? !labels.contains(*c->label) : loops == 0;
    if (ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n) || ir::is<ir::LocalClass>(n)) return false;
    const int inner = loops + (ir::is<ir::While>(n) || ir::is<ir::DoWhile>(n) || ir::is<ir::For>(n) || ir::is<ir::ForEach>(n) ||
                               ir::is<ir::Switch>(n));
    std::set<std::string, std::less<>> labs = labels;
    if (const auto* lb = ir::get<ir::Labeled>(n)) {
        if (lb->label) labs.insert(*lb->label);
    }
    for (const ir::Node* c : ir::children(*n)) {
        if (leaves_walk(c, inner, labs)) return true;
    }
    return false;
}

}  // namespace analysis_impl

bool leaves(std::span<const ir::Node* const> stmts) {
    for (const ir::Node* s : stmts) {
        if (analysis_impl::leaves_walk(s, 0, {})) return true;
    }
    return false;
}

}  // namespace cb::translate::backends::cpp03

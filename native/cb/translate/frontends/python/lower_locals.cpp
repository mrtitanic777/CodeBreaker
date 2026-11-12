// cb-unity: alone (file-local helpers named for this module)
// Where each local of a function is declared in the C++.
#include <algorithm>
#include <unordered_set>

#include "cb/translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

namespace {

// One step down a path of nested statement lists: the list, the place in it, and the statement there.
struct Step {
    const std::vector<syntax::Stmt*>* list = nullptr;
    std::size_t index = 0;
    const syntax::Stmt* stmt = nullptr;
    bool same_place(const Step& other) const { return list == other.list && index == other.index; }
};
using Path = std::vector<Step>;

// A statement that mentions the name, and where it is.
struct Use {
    Path path;
    const syntax::Stmt* stmt = nullptr;
};

// The statement lists a statement holds, in the order the locals are met in: body, else-part, final part, then the handlers' bodies.
std::vector<const std::vector<syntax::Stmt*>*> blocks_of(const syntax::Stmt* st) {
    using K = syntax::StmtKind;
    std::vector<const std::vector<syntax::Stmt*>*> out;
    switch (st->kind) {
        case K::FunctionDef:
        case K::AsyncFunctionDef: out.push_back(&syntax::cast<syntax::FunctionDef>(st)->body); break;
        case K::ClassDef: out.push_back(&syntax::cast<syntax::ClassDef>(st)->body); break;
        case K::For:
        case K::AsyncFor: {
            const auto* f = syntax::cast<syntax::For>(st);
            out = {&f->body, &f->orelse};
            break;
        }
        case K::While: out = {&syntax::cast<syntax::While>(st)->body, &syntax::cast<syntax::While>(st)->orelse}; break;
        case K::If: out = {&syntax::cast<syntax::If>(st)->body, &syntax::cast<syntax::If>(st)->orelse}; break;
        case K::With:
        case K::AsyncWith: out.push_back(&syntax::cast<syntax::With>(st)->body); break;
        case K::Try:
        case K::TryStar: {
            const auto* t = syntax::cast<syntax::Try>(st);
            out = {&t->body, &t->orelse, &t->finalbody};
            for (const syntax::ExceptHandler* h : t->handlers) out.push_back(&h->body);
            break;
        }
        default: break;
    }
    return out;
}

bool mentions(const syntax::Expr* e, const std::string& name) {
    for (const pyast::AnyNode& n : pyast::walk(e))
        if (const auto* id = syntax::as<syntax::Name>(n.expr()); id != nullptr && id->id == name) return true;
    return false;
}

}  // namespace

std::vector<std::string> Lowerer::target_names(const syntax::Expr* target) {
    if (const auto* n = syntax::as<syntax::Name>(target)) return {n->id};
    const std::vector<syntax::Expr*>* elements = nullptr;
    if (const auto* t = syntax::as<syntax::Tuple>(target)) elements = &t->elts;
    else if (const auto* l = syntax::as<syntax::List>(target)) elements = &l->elts;
    std::vector<std::string> out;
    if (elements != nullptr)
        for (const syntax::Expr* e : *elements) {
            const std::vector<std::string> inner = target_names(e);
            out.insert(out.end(), inner.begin(), inner.end());
        }
    return out;
}

// The nodes of a statement outside its nested statement blocks.
std::vector<pyast::AnyNode> Lowerer::own_nodes(const syntax::Stmt* st) {
    std::vector<pyast::AnyNode> out;
    std::vector<pyast::AnyNode> todo = {pyast::AnyNode(st)};
    while (!todo.empty()) {
        const pyast::AnyNode n = todo.back();
        todo.pop_back();
        out.push_back(n);
        std::vector<pyast::AnyNode> kids;
        pyast::children(n, kids);
        for (const pyast::AnyNode& kid : kids) {
            const bool nested_block = n.node == st && (kid.category == pyast::Category::Stmt || kid.category == pyast::Category::Handler ||
                                                       kid.category == pyast::Category::MatchCase);
            if (!nested_block) todo.push_back(kid);
        }
    }
    return out;
}

// A local's C++ type is the join of everything stored to it. It is declared at its first assignment when that is a statement of
// the innermost block holding every use, else just before the statement of that block that first touches it. A loop's variable
// used only inside the loop is declared by the loop itself (and one never read, like `_`, by each loop).
void Lowerer::plan_locals() {
    OrderedMap<std::string, std::vector<Use>> occurrences;
    std::unordered_map<std::string, std::vector<const syntax::Name*>> stores;
    std::unordered_set<std::string> loads;
    const std::vector<syntax::Stmt*>& body = fn->node->body;

    const std::function<void(const std::vector<syntax::Stmt*>&, const Path&)> visit = [&](const std::vector<syntax::Stmt*>& stmts, const Path& path) {
        for (std::size_t i = 0; i < stmts.size(); ++i) {
            Path here = path;
            here.push_back({&stmts, i, stmts[i]});
            for (const pyast::AnyNode& n : own_nodes(stmts[i])) {
                const auto* name = syntax::as<syntax::Name>(n.expr());
                if (name == nullptr || !ev.locals.contains(name->id) || declared.contains(name->id)) continue;
                occurrences[name->id].push_back({here, stmts[i]});
                if (name->ctx == syntax::ExprCtx::Store) stores[name->id].push_back(name);
                else loads.insert(name->id);
            }
            for (const std::vector<syntax::Stmt*>* sub : blocks_of(stmts[i]))
                if (!sub->empty()) visit(*sub, here);
        }
    };
    visit(body, {});

    for (const auto& [name, uses] : occurrences) {
        std::vector<Type> stored;
        for (const syntax::Name* n : stores[name])
            if (const auto t = ev.types.find(n); t != ev.types.end()) stored.push_back(t->second);
        if (stored.empty()) continue;   // only read where no store reached: not lowered
        Type joined = stored.front();
        for (std::size_t i = 1; i < stored.size(); ++i) joined = p.join(joined, stored[i]);
        local_types[name] = ty(joined, stores[name].front(), "local " + name);

        // a variable only a loop's target stores, and never read, is declared by each loop
        std::unordered_set<const syntax::Node*> in_targets;
        std::vector<const syntax::For*> loop_targets;
        for (const Use& u : uses) {
            const auto* loop = u.stmt->kind == syntax::StmtKind::For ? syntax::cast<syntax::For>(u.stmt) : nullptr;
            if (loop == nullptr) continue;
            const auto names = target_names(loop->target);
            if (std::find(names.begin(), names.end(), name) == names.end()) continue;
            loop_targets.push_back(loop);
            for (const pyast::AnyNode& x : pyast::walk(loop->target)) in_targets.insert(x.node);
        }
        if (!loads.contains(name) && !stores[name].empty() &&
            std::all_of(stores[name].begin(), stores[name].end(), [&](const syntax::Name* n) { return in_targets.contains(n); })) {
            loopvars.insert(name);
            continue;
        }

        // the innermost statement list that holds every use
        std::size_t common = 0;
        const auto every_path_reaches = [&](std::size_t level) {
            return std::all_of(uses.begin(), uses.end(), [&](const Use& u) { return u.path.size() > level; });
        };
        while (every_path_reaches(common)) {
            const Step& first_step = uses.front().path[common];
            if (!std::all_of(uses.begin(), uses.end(), [&](const Use& u) { return u.path[common].same_place(first_step); })) break;
            ++common;
        }
        const Path& first_path = uses.front().path;
        const syntax::Stmt* anchor;
        if (every_path_reaches(common) &&
            std::all_of(uses.begin(), uses.end(), [&](const Use& u) { return u.path[common].list == first_path[common].list; })) {
            anchor = first_path[common].stmt;   // siblings in one block: the first of them
        } else {
            anchor = first_path[common > 0 ? common - 1 : first_path.size() - 1].stmt;   // the statement whose blocks hold them all
        }
        if (anchor->kind == syntax::StmtKind::For) {
            const auto* loop = syntax::cast<syntax::For>(anchor);
            const auto names = target_names(loop->target);
            std::unordered_set<const syntax::Node*> inside;
            for (const pyast::AnyNode& x : pyast::walk(anchor)) inside.insert(x.node);
            if (std::find(names.begin(), names.end(), name) != names.end() &&
                std::all_of(uses.begin(), uses.end(), [&](const Use& u) { return inside.contains(u.stmt); }))
                continue;   // the loop declares it
        }
        const syntax::Stmt* first_stmt = uses.front().stmt;
        const auto* assign = anchor == first_stmt ? syntax::as<syntax::Assign>(first_stmt) : nullptr;
        if (assign != nullptr && assign->targets.size() == 1 && syntax::as<syntax::Name>(assign->targets[0]) != nullptr &&
            syntax::as<syntax::Name>(assign->targets[0])->id == name && !mentions(assign->value, name)) {
            decl_init[first_stmt] = name;
        } else if (assign != nullptr && assign->targets.size() == 1 && syntax::as<syntax::Tuple>(assign->targets[0]) != nullptr) {
            const auto names = target_names(assign->targets[0]);
            if (std::find(names.begin(), names.end(), name) != names.end() && !mentions(assign->value, name)) decl_tuple[first_stmt].insert(name);
            else decl_at[anchor].push_back(name);
        } else {
            decl_at[anchor].push_back(name);
        }
    }
}

}  // namespace cb::translate::frontends::python::lower

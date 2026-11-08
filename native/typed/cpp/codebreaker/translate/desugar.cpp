// codebreaker/translate/desugar.cpp -- typed C++ port of codebreaker.translate.desugar (see desugar.hpp).

#include "codebreaker/translate/desugar.hpp"

#include <unordered_map>
#include <vector>

#include "codebreaker/translate/backends/cpp03.hpp"

namespace codebreaker::translate::desugar {

namespace {

using ir::NodeP;
using ir::NodeOrList;
using ir::Nodes;
using ir::SymP;
using ir::TypeP;
using ir::Val;

const py::list<py::str> make_forms() {
    py::list<py::str> out;
    for (const char* s : {"foreach", "switchexpr", "stringswitch", "pattern"}) out.append(py::str(s));
    return out;
}

// ---------------------------------------------------------------------------------------------- small helpers
[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}
[[noreturn]] void not_iterable() { throw py::TypeError("'NoneType' object is not iterable"); }

template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

// the list a Nodes slot holds (TypeError for None, as iterating None is)
const py::list<NodeP>& lst(const Nodes& v) {
    if (!v) not_iterable();
    return *v;
}

// the node a body slot holds; a bare list of statements is not something the Python can read a line of
NodeP body_node(const NodeOrList& v, const char* attr) {
    if (std::holds_alternative<py::list<NodeP>>(v))
        throw py::AttributeError(std::string("'list' object has no attribute '") + attr + "'");
    return std::get<NodeP>(v);
}

struct Pos {
    int64_t line;
    int64_t col;
};
Pos pos_of(const NodeP& s) {
    if (!s) none_attr("line");
    return {s->line, s->col};
}
template <class N> py::ref<N> mk(const Pos& p) {
    auto n = std::make_shared<N>();
    n->line = p.line;
    n->col = p.col;
    return n;
}

// `e.type` of an expression (AttributeError for a node that is not one)
TypeP type_of(const NodeP& e) {
    if (!e) none_attr("type");
    if (auto x = as_a<ir::Expr>(e)) return x->type;
    throw py::AttributeError("'" + e->class_name().utf8() + "' object has no attribute 'type'");
}

SymP sym_of(const NodeP& n) {
    if (auto l = as_a<ir::Local>(n)) return l->sym;
    if (!n) none_attr("sym");
    throw py::AttributeError("'" + n->class_name().utf8() + "' object has no attribute 'sym'");
}

py::ref<ir::Local> as_local(const NodeP& n) {
    auto l = as_a<ir::Local>(n);
    if (!l) {
        if (!n) none_attr("sym");
        throw py::AttributeError("'" + n->class_name().utf8() + "' object has no attribute 'sym'");
    }
    return l;
}

// ---- the slots of a node, by name (Python's getattr / setattr over node.fields)
template <class T, class F> bool each_as(ir::Node& n, F& f) {
    if (T* p = dynamic_cast<T*>(&n)) {
        p->each(f);
        return true;
    }
    return false;
}
template <class... T, class F> bool each_class(ir::Node& n, F& f) { return (each_as<T>(n, f) || ...); }
template <class F> void each_slot_of(ir::Node& n, F&& f) {
    each_class<ir::Module, ir::Unit, ir::ClassDecl, ir::FieldDecl, ir::MethodDecl, ir::Param, ir::EnumConst,
               ir::Property, ir::TypeParam, ir::Block, ir::Local, ir::ExprStmt, ir::If, ir::While, ir::DoWhile,
               ir::For, ir::ForEach, ir::Switch, ir::Case, ir::Break, ir::Continue, ir::Return, ir::Throw, ir::Try,
               ir::Catch, ir::Labeled, ir::Goto, ir::MachineFragment, ir::Sync, ir::Empty, ir::Comment,
               ir::LocalClass, ir::Assert, ir::CtorCall, ir::Yield, ir::Lit, ir::Name, ir::Select, ir::TypeName,
               ir::This, ir::Super, ir::Call, ir::New, ir::NewArray, ir::Index, ir::Length, ir::Unary, ir::Binary,
               ir::Concat, ir::Assign, ir::Cond, ir::Cast, ir::InstanceOf, ir::ClassLit, ir::Lambda, ir::MethodRef,
               ir::SwitchExpr, ir::AddressOf, ir::FarAddress, ir::Piece, ir::Join, ir::Paren, ir::Unsupported>(n, f);
}

// ---------------------------------------------------------------------------------------------- helpers
NodeP strip(NodeP e) {
    while (auto p = as_a<ir::Paren>(e)) e = p->x;
    return e;
}

bool is_string(const TypeP& t) {
    return t && t->kind == py::str("class") && t->name && *t->name == py::str("java.lang.String");
}

// A name, or a field of this / of a class / of a plain name: evaluating it twice is reading it twice.
bool plain(const NodeP& e_) {
    NodeP e = strip(e_);
    if (is_a<ir::Name>(e)) return true;
    if (auto s = as_a<ir::Select>(e)) return is_a<ir::This>(s->obj) || is_a<ir::TypeName>(s->obj) || plain(s->obj);
    return false;
}

NodeP copy_target(const NodeP& e_) {
    NodeP e = strip(e_);
    if (auto n = as_a<ir::Name>(e)) {
        auto r = mk<ir::Name>({n->line, n->col});
        r->sym = n->sym;
        r->type = n->type;
        return r;
    }
    if (auto s = as_a<ir::Select>(e)) {
        auto r = mk<ir::Select>({s->line, s->col});
        r->obj = (is_a<ir::This>(s->obj) || is_a<ir::TypeName>(s->obj)) ? s->obj : copy_target(s->obj);
        r->sym = s->sym;
        r->type = s->type;
        return r;
    }
    return e;
}

bool leaves_arm(const NodeP& s) {
    if (is_a<ir::Break>(s) || is_a<ir::Return>(s) || is_a<ir::Throw>(s) || is_a<ir::Continue>(s)) return true;
    if (auto b = as_a<ir::Block>(s)) return b->stmts && !b->stmts->empty() && leaves_arm(b->stmts->operator[](-1));
    if (auto i = as_a<ir::If>(s)) {
        if (std::holds_alternative<py::list<NodeP>>(i->orelse)) return false;   // (a list is not an If's `orelse is not None` form here)
        NodeP o = std::get<NodeP>(i->orelse);
        return o && std::holds_alternative<NodeP>(i->then) && leaves_arm(std::get<NodeP>(i->then)) && leaves_arm(o);
    }
    return false;
}

// The nodes of an expression, not entering lambdas, switch expressions or anonymous classes (their
// statements are another level's).
py::list<NodeP> own_nodes(const NodeP& root) {
    py::list<NodeP> out;
    std::vector<NodeP> stack{root};
    while (!stack.empty()) {
        NodeP n = stack.back();
        stack.pop_back();
        out.append(n);
        if (is_a<ir::Lambda>(n) || is_a<ir::SwitchExpr>(n) || is_a<ir::ClassDecl>(n)) continue;
        if (!n) none_attr("children");
        py::list<NodeP> kids = n->children();
        for (int64_t i = py::len(kids); i-- > 0;) stack.push_back(kids[i]);
    }
    return out;
}

// The nodes from root down to target's parent, or None when target is not under root.
std::optional<py::list<NodeP>> ancestors(const NodeP& root, const NodeP& target) {
    if (root == target) return py::list<NodeP>();
    if (!root) none_attr("children");
    for (const NodeP& c : root->children()) {
        if (is_a<ir::Lambda>(c) || is_a<ir::ClassDecl>(c)) continue;
        auto got = ancestors(c, target);
        if (got) {
            py::list<NodeP> out;
            out.append(root);
            out.extend(*got);
            return out;
        }
    }
    return std::nullopt;
}

// Every operand of the chain's nodes that is not on the path to target (each evaluated whole).
py::list<NodeP> siblings(const py::list<NodeP>& chain, const NodeP& target) {
    std::unordered_map<const ir::Node*, bool> path;
    for (const NodeP& n : chain) path[n.get()] = true;
    path[target.get()] = true;
    py::list<NodeP> out;
    for (const NodeP& n : chain) {
        for (const NodeP& c : n->children()) {
            if (!path.count(c.get())) out.append(c);
        }
    }
    return out;
}

using Swap = std::unordered_map<const ir::Node*, NodeP>;
void replace_in(const NodeP& node, const Swap& swap);

void replace_child(NodeP& m, const Swap& swap) {
    if (!m) return;
    auto it = swap.find(m.get());
    if (it != swap.end()) m = it->second;
    else replace_in(m, swap);
}
void replace_list(const py::list<NodeP>& v, const Swap& swap) {
    for (int64_t i = 0; i < py::len(v); ++i) {
        NodeP x = v[i];
        if (!x) continue;
        auto it = swap.find(x.get());
        if (it != swap.end()) v[i] = it->second;
        else replace_in(x, swap);
    }
}
void replace_slot(NodeP& m, const Swap& swap) { replace_child(m, swap); }
void replace_slot(Nodes& m, const Swap& swap) {
    if (m) replace_list(*m, swap);
}
void replace_slot(NodeOrList& m, const Swap& swap) {
    if (std::holds_alternative<NodeP>(m)) {
        NodeP v = std::get<NodeP>(m);
        replace_child(v, swap);
        m = v;
    } else {
        replace_list(std::get<py::list<NodeP>>(m), swap);
    }
}
template <class T> void replace_slot(T&, const Swap&) {}   // (a slot of `fields` always holds one of the above)

// Replace, in place under `node`, each child whose id is in `swap` by its replacement.
void replace_in(const NodeP& node, const Swap& swap) {
    if (!node) none_attr("fields");
    each_slot_of(*node, [&](const char*, auto& member, auto rank) {
        if (static_cast<int>(rank) != 0) replace_slot(member, swap);
    });
}

}  // namespace

const py::list<py::str> FORMS = make_forms();

namespace {

bool contains(const py::set<py::str>& s, const char* k) { return s.contains(py::str(k)); }

class Desugar {
  public:
    py::set<py::str> forms;
    int64_t count = 0;
    int64_t n = 0;
    py::str owner;

    explicit Desugar(const py::set<py::str>& f) : forms(f), owner(py::str("")) {}

    py::str fresh(const char* stem) {
        n += 1;
        return py::percent(py::str("j2x_%s%d"), py::str(stem), n);
    }

    // ---- declarations
    void cls(const NodeP& c_) {
        auto c = as_a<ir::ClassDecl>(c_);
        if (!c) {
            if (!c_) none_attr("members");
            throw py::AttributeError("'" + c_->class_name().utf8() + "' object has no attribute 'members'");
        }
        for (const NodeP& m : lst(c->members)) {
            if (auto k = as_a<ir::ClassDecl>(m)) {
                cls(k);
            } else if (auto md = as_a<ir::MethodDecl>(m)) {
                if (std::holds_alternative<py::list<NodeP>>(md->body) || std::get<NodeP>(md->body)) {
                    py::str q = (c->qname && py::truthy(*c->qname)) ? *c->qname : (c->name && py::truthy(*c->name) ? *c->name : py::str(""));
                    owner = q + py::str(".") + (md->name && py::truthy(*md->name) ? *md->name : py::str(""));
                    NodeP body = body_node(md->body, "stmts");
                    block(body);
                    nested(body);
                }
            } else if (auto ec = as_a<ir::EnumConst>(m)) {
                if (ec->body) cls(ec->body);
            }
        }
    }

    // The bodies of the lambdas and anonymous classes inside `root`'s expressions.
    void nested(const NodeP& root) {
        for (const NodeP& nd : ir::walk(root)) {
            if (auto l = as_a<ir::Lambda>(nd)) {
                if (is_a<ir::Block>(l->body)) block(l->body);
            } else if (auto nw = as_a<ir::New>(nd)) {
                if (nw->body) cls(nw->body);
            }
        }
    }

    // ---- statements: each list is rewritten, a statement may become several
    void block(const NodeP& b_) {
        if (auto b = as_a<ir::Block>(b_)) b->stmts = stmts(lst(b->stmts));
    }

    py::list<NodeP> stmts(const py::list<NodeP>& ss) {
        py::list<NodeP> out;
        py::dict<py::str, TypeP> declared;
        for (const NodeP& s : ss) {
            out.extend(stmt(s, declared));
            if (auto l = as_a<ir::Local>(s)) {
                if (l->sym) declared[l->sym->name] = l->type;
            }
        }
        return out;
    }

    // A nested statement (a loop's or an if's body): one statement back.
    NodeP sub(const NodeP& s) {
        if (!s) return nullptr;
        py::dict<py::str, TypeP> none;
        py::list<NodeP> got = stmt(s, none);
        if (py::len(got) == 1) return got[0];
        auto b = mk<ir::Block>(pos_of(s));
        b->stmts = got;
        return b;
    }
    NodeP sub(const NodeOrList& s) { return sub(body_node(s, "line")); }

    py::list<NodeP> stmt(const NodeP& s, const py::dict<py::str, TypeP>& declared) {
        py::list<NodeP> pre;
        if (contains(forms, "pattern")) pre = patterns(s, declared);
        py::list<NodeP> out = pre;
        out.extend(stmt_(s));
        return out;
    }

    py::list<NodeP> stmt_(const NodeP& s) {
        Pos p = pos_of(s);
        if (auto b = as_a<ir::Block>(s)) {
            block(b);
            return py::list<NodeP>{s};
        }
        if (auto i = as_a<ir::If>(s)) {
            NodeP then = sub(i->then);
            NodeP orelse = sub(i->orelse);
            i->then = then;
            i->orelse = orelse;
            return py::list<NodeP>{s};
        }
        if (auto w = as_a<ir::While>(s)) {
            w->body = sub(w->body);
            return py::list<NodeP>{s};
        }
        if (auto w = as_a<ir::DoWhile>(s)) {
            w->body = sub(w->body);
            return py::list<NodeP>{s};
        }
        if (auto f = as_a<ir::For>(s)) {
            f->body = sub(f->body);
            return py::list<NodeP>{s};
        }
        if (auto l = as_a<ir::Labeled>(s)) {
            if (auto fe = as_a<ir::ForEach>(std::holds_alternative<NodeP>(l->body) ? std::get<NodeP>(l->body) : NodeP())) {
                if (contains(forms, "foreach")) {
                    auto got = foreach(fe);
                    if (got) {                                   // the label goes on the loop, not its set-up
                        auto& [pre, loop] = *got;
                        l->body = loop;
                        if (!pre.empty()) {
                            auto blk = mk<ir::Block>(p);
                            blk->stmts = pre + py::list<NodeP>{s};
                            return py::list<NodeP>{blk};
                        }
                        return py::list<NodeP>{s};
                    }
                }
            }
            l->body = sub(l->body);
            return py::list<NodeP>{s};
        }
        if (auto fe = as_a<ir::ForEach>(s)) {
            fe->body = sub(fe->body);
            if (contains(forms, "foreach")) {
                auto got = foreach(fe);
                if (got) {
                    auto& [pre, loop] = *got;
                    if (!pre.empty()) {
                        auto blk = mk<ir::Block>(p);
                        blk->stmts = pre + py::list<NodeP>{loop};
                        return py::list<NodeP>{blk};
                    }
                    return py::list<NodeP>{loop};
                }
            }
            return py::list<NodeP>{s};
        }
        if (auto sw = as_a<ir::Switch>(s)) {
            for (const NodeP& c_ : lst(sw->cases)) {
                auto c = as_a<ir::Case>(c_);
                if (!c) none_attr("body");
                c->body = stmts(lst(c->body));
            }
            if (contains(forms, "stringswitch") && is_string(type_of(sw->expr))) {
                auto got = string_switch(sw, std::nullopt);
                if (got) return *got;
            }
            return py::list<NodeP>{s};
        }
        if (auto t = as_a<ir::Try>(s)) {
            block(t->body);
            for (const NodeP& c : lst(t->catches)) {
                auto ct = as_a<ir::Catch>(c);
                if (!ct) none_attr("body");
                block(ct->body);
            }
            if (t->final) block(t->final);
            return py::list<NodeP>{s};
        }
        if (auto sy = as_a<ir::Sync>(s)) {
            block(sy->body);
            return py::list<NodeP>{s};
        }
        if (auto lc = as_a<ir::LocalClass>(s)) {
            cls(lc->decl);
            return py::list<NodeP>{s};
        }
        if (contains(forms, "switchexpr")) {
            auto got = switch_expr_stmt(s);
            if (got) return *got;
        }
        return py::list<NodeP>{s};
    }

    // ---- for-each
    // (set-up statements, the loop) for a ForEach, or None.
    std::optional<std::tuple<py::list<NodeP>, NodeP>> foreach(const py::ref<ir::ForEach>& s) {
        Pos p = pos_of(s);
        NodeP it = s->iterable;
        TypeP t = type_of(it);
        py::ref<ir::Local> var = as_local(s->var);
        py::ref<ir::Block> body = as_a<ir::Block>(s->body);
        if (!body) {
            body = mk<ir::Block>(p);
            body->stmts = py::list<NodeP>{s->body};
        }
        if (t && t->kind == py::str("array")) {
            SymP arr = std::make_shared<ir::Sym>(py::str("local"), fresh("a"), std::optional<py::str>(owner), false, t);
            SymP idx = std::make_shared<ir::Sym>(py::str("local"), fresh("i"), std::optional<py::str>(owner), false, ir::I32);
            auto a = [&]() {
                auto x = mk<ir::Name>(p);
                x->sym = arr;
                x->type = t;
                return x;
            };
            auto i = [&]() {
                auto x = mk<ir::Name>(p);
                x->sym = idx;
                x->type = ir::I32;
                return x;
            };
            auto index = mk<ir::Index>(p);
            index->arr = a();
            index->index = NodeP(i());
            index->type = t->elem;
            auto elem = std::make_shared<ir::Local>();
            elem->sym = var->sym;
            elem->type = var->type;
            elem->init = index;
            elem->mods = var->mods;
            elem->line = var->line;
            elem->col = var->col;
            auto zero = mk<ir::Lit>(p);
            zero->value = Val(int64_t(0));
            zero->type = ir::I32;
            auto idx_local = mk<ir::Local>(p);
            idx_local->sym = idx;
            idx_local->type = ir::I32;
            idx_local->init = zero;
            idx_local->mods = py::list<py::str>();
            auto len = mk<ir::Length>(p);
            len->arr = a();
            len->type = ir::I32;
            auto cond = mk<ir::Binary>(p);
            cond->op = py::str("<");
            cond->left = i();
            cond->right = len;
            cond->type = ir::BOOL;
            auto upd = mk<ir::Unary>(p);
            upd->op = py::str("++");
            upd->x = i();
            upd->postfix = true;
            upd->type = ir::I32;
            auto lbody = mk<ir::Block>(p);
            py::list<NodeP> ss{elem};
            ss.extend(lst(body->stmts));
            lbody->stmts = ss;
            auto loop = mk<ir::For>(p);
            loop->init = py::list<NodeP>{idx_local};
            loop->cond = cond;
            loop->update = py::list<NodeP>{upd};
            loop->body = NodeP(lbody);
            count += 1;
            auto arr_local = mk<ir::Local>(p);
            arr_local->sym = arr;
            arr_local->type = t;
            arr_local->init = it;
            arr_local->mods = py::list<py::str>();
            return std::make_tuple(py::list<NodeP>{arr_local}, NodeP(loop));
        }
        if (t && t->kind == py::str("class")) {
            TypeP elem_t = !t->args.empty() ? t->args[0] : std::make_shared<ir::Type>(py::str("class"), std::optional<py::str>(py::str("java.lang.Object")));
            if (elem_t->kind == py::str("wildcard") || elem_t->kind == py::str("typevar")) {
                elem_t = (elem_t->kind == py::str("wildcard") && elem_t->bound && std::get<0>(*elem_t->bound) == py::str("extends"))
                             ? std::get<1>(*elem_t->bound)
                             : std::make_shared<ir::Type>(py::str("class"), std::optional<py::str>(py::str("java.lang.Object")));
            }
            TypeP it_t = std::make_shared<ir::Type>(py::str("class"), std::optional<py::str>(py::str("java.util.Iterator")),
                                                    py::list<TypeP>{elem_t});
            SymP itsym = std::make_shared<ir::Sym>(py::str("local"), fresh("it"), std::optional<py::str>(owner), false, it_t);
            auto nm = [&]() {
                auto x = mk<ir::Name>(p);
                x->sym = itsym;
                x->type = it_t;
                return x;
            };
            auto mcall = [&](const NodeP& target, const SymP& sym, const TypeP& ty) {
                auto c = mk<ir::Call>(p);
                c->target = target;
                c->sym = sym;
                c->args = py::list<NodeP>();
                c->typeargs = py::list<TypeP>();
                c->type = ty;
                return c;
            };
            SymP iterator_sym = std::make_shared<ir::Sym>(py::str("method"), py::str("iterator"), t->name, false, nullptr,
                                                          py::list<TypeP>(), it_t);
            auto iterator = mcall(it, iterator_sym, it_t);
            SymP has_sym = std::make_shared<ir::Sym>(py::str("method"), py::str("hasNext"),
                                                     std::optional<py::str>(py::str("java.util.Iterator")), false, nullptr,
                                                     py::list<TypeP>(), ir::BOOL);
            auto has = mcall(nm(), has_sym, ir::BOOL);
            SymP next_sym = std::make_shared<ir::Sym>(py::str("method"), py::str("next"),
                                                      std::optional<py::str>(py::str("java.util.Iterator")), false, nullptr,
                                                      py::list<TypeP>(),
                                                      std::make_shared<ir::Type>(py::str("typevar"), std::optional<py::str>(py::str("E"))));
            auto nxt = mcall(nm(), next_sym, elem_t);
            auto elem = std::make_shared<ir::Local>();
            elem->sym = var->sym;
            elem->type = var->type;
            elem->init = nxt;
            elem->mods = var->mods;
            elem->line = var->line;
            elem->col = var->col;
            auto it_local = mk<ir::Local>(p);
            it_local->sym = itsym;
            it_local->type = it_t;
            it_local->init = iterator;
            it_local->mods = py::list<py::str>();
            auto lbody = mk<ir::Block>(p);
            py::list<NodeP> ss{elem};
            ss.extend(lst(body->stmts));
            lbody->stmts = ss;
            auto loop = mk<ir::For>(p);
            loop->init = py::list<NodeP>{it_local};
            loop->cond = has;
            loop->update = py::list<NodeP>();
            loop->body = NodeP(lbody);
            count += 1;
            return std::make_tuple(py::list<NodeP>(), NodeP(loop));
        }
        return std::nullopt;
    }

    // ---- switch expressions
    std::optional<py::list<NodeP>> switch_expr_stmt(const NodeP& s) {
        Pos p = pos_of(s);
        if (auto l = as_a<ir::Local>(s)) {
            if (auto sw = as_a<ir::SwitchExpr>(strip(l->init))) {
                auto decl = mk<ir::Local>(p);
                decl->sym = l->sym;
                decl->type = l->type;
                decl->mods = l->mods;
                auto target = mk<ir::Name>(p);
                target->sym = l->sym;
                target->type = l->type;
                py::list<NodeP> out{decl};
                out.extend(switch_into(sw, target));
                return out;
            }
        }
        if (auto r = as_a<ir::Return>(s)) {
            if (auto sw = as_a<ir::SwitchExpr>(strip(r->expr))) {
                SymP tmp = std::make_shared<ir::Sym>(py::str("local"), fresh("sw"), std::optional<py::str>(owner), false, sw->type);
                auto decl = mk<ir::Local>(p);
                decl->sym = tmp;
                decl->type = sw->type;
                decl->mods = py::list<py::str>();
                auto tn = mk<ir::Name>(p);
                tn->sym = tmp;
                tn->type = sw->type;
                py::list<NodeP> got = switch_into(sw, tn);
                auto rn = mk<ir::Name>(p);
                rn->sym = tmp;
                rn->type = sw->type;
                auto ret = mk<ir::Return>(p);
                ret->expr = rn;
                py::list<NodeP> ss{decl};
                ss.extend(got);
                ss.append(ret);
                auto blk = mk<ir::Block>(p);
                blk->stmts = ss;
                return py::list<NodeP>{blk};
            }
        }
        if (auto es = as_a<ir::ExprStmt>(s)) {
            if (auto a = as_a<ir::Assign>(es->expr)) {
                if (!a->op) {
                    if (auto sw = as_a<ir::SwitchExpr>(strip(a->value))) {
                        if (plain(a->target)) return switch_into(sw, a->target);
                    }
                }
            }
        }
        return switch_expr_inside(s);
    }

    // A switch expression deeper in a statement's expression (`list.add(switch ...)`): taken out into a
    // temporary before the statement, when that cannot change what happens -- it is the only one, nothing
    // else in the expression has an effect but the calls and assignments it is an operand of (they run after
    // it anyway), and the switch writes nothing the rest reads.
    std::optional<py::list<NodeP>> switch_expr_inside(const NodeP& s) {
        NodeP root;
        if (auto es = as_a<ir::ExprStmt>(s)) root = es->expr;
        else if (auto r = as_a<ir::Return>(s)) root = r->expr;
        else if (auto l = as_a<ir::Local>(s)) root = l->init;
        else return std::nullopt;
        if (!root) return std::nullopt;
        py::list<NodeP> found;
        for (const NodeP& nd : own_nodes(root)) {
            if (is_a<ir::SwitchExpr>(nd)) found.append(nd);
        }
        if (py::len(found) != 1 || found[0] == root) return std::nullopt;
        auto sw = as_a<ir::SwitchExpr>(found[0]);
        auto chain = ancestors(root, sw);
        if (!chain) return std::nullopt;
        py::list<NodeP> others = siblings(*chain, sw);
        for (const NodeP& nd : others) {
            if (backends::cpp03::_effects(nd)) return std::nullopt;
        }
        py::set<py::str> reads;
        for (const NodeP& nd : others) reads.update(backends::cpp03::_read(nd));
        py::set<py::str> written = backends::cpp03::_written(sw);
        if (!(written & reads).empty()) return std::nullopt;
        Pos p = pos_of(s);
        SymP tmp = std::make_shared<ir::Sym>(py::str("local"), fresh("sw"), std::optional<py::str>(owner), false, sw->type);
        auto decl = mk<ir::Local>(p);
        decl->sym = tmp;
        decl->type = sw->type;
        decl->mods = py::list<py::str>();
        auto tn = mk<ir::Name>(p);
        tn->sym = tmp;
        tn->type = sw->type;
        py::list<NodeP> got = switch_into(sw, tn);
        auto rn = mk<ir::Name>(p);
        rn->sym = tmp;
        rn->type = sw->type;
        Swap swap;
        swap[sw.get()] = rn;
        replace_in(s, swap);
        if (!is_a<ir::Local>(s)) {
            py::list<NodeP> ss{decl};
            ss.extend(got);
            ss.append(s);
            auto blk = mk<ir::Block>(p);
            blk->stmts = ss;
            return py::list<NodeP>{blk};
        }
        py::list<NodeP> out{decl};
        out.extend(got);
        out.append(s);
        return out;
    }

    // The statements of switch expression `sw` assigning its value to `target` (a Name or plain Select).
    py::list<NodeP> switch_into(const py::ref<ir::SwitchExpr>& sw, const NodeP& target) {
        Pos p = pos_of(sw);
        py::str label = fresh("sw");
        py::list<NodeP> cases;
        for (const NodeP& c_ : lst(sw->cases)) {
            auto c = as_a<ir::Case>(c_);
            if (!c) none_attr("body");
            py::list<NodeP> ys;
            for (const NodeP& b : lst(c->body)) ys.append(yields(b, target, label));
            auto nc = std::make_shared<ir::Case>();
            nc->labels = c->labels;
            nc->default_ = c->default_;
            nc->body = stmts(ys);
            nc->line = c->line;
            nc->col = c->col;
            cases.append(nc);
        }
        auto st = mk<ir::Switch>(p);
        st->expr = sw->expr;
        st->cases = cases;
        count += 1;
        if (contains(forms, "stringswitch") && is_string(type_of(sw->expr))) {
            auto got = string_switch(st, label);
            if (got) return *got;
        }
        auto lab = mk<ir::Labeled>(p);
        lab->label = label;
        lab->body = NodeP(st);
        return py::list<NodeP>{lab};
    }

    // `s` with each Yield of this switch expression (not of one nested in it) as an assignment and a
    // labelled break.
    NodeP yields(const NodeP& s, const NodeP& target, const py::str& label) {
        Pos p = pos_of(s);
        if (auto y = as_a<ir::Yield>(s)) {
            auto asg = mk<ir::Assign>(p);
            asg->op = std::nullopt;
            asg->target = copy_target(target);
            asg->value = y->expr;
            asg->overflow = std::nullopt;
            asg->type = type_of(target);
            auto es = mk<ir::ExprStmt>(p);
            es->expr = asg;
            auto br = mk<ir::Break>(p);
            br->label = label;
            auto blk = mk<ir::Block>(p);
            blk->stmts = py::list<NodeP>{es, br};
            return blk;
        }
        each_slot_of(*s, [&](const char*, auto& member, auto rank) {
            if (static_cast<int>(rank) != 0) yields_slot(member, target, label);
        });
        return s;
    }

    void yields_slot(NodeP& m, const NodeP& target, const py::str& label) {
        if (m && is_a<ir::Stmt>(m) && !is_a<ir::Case>(m)) m = yields(m, target, label);
    }
    void yields_list(py::list<NodeP>& v, const NodeP& target, const py::str& label) {
        py::list<NodeP> out;
        for (const NodeP& x : v) out.append((x && is_a<ir::Stmt>(x)) ? yields(x, target, label) : x);
        v = out;
    }
    void yields_slot(Nodes& m, const NodeP& target, const py::str& label) {
        if (m) {
            py::list<NodeP> v = *m;
            yields_list(v, target, label);
            m = v;
        }
    }
    void yields_slot(NodeOrList& m, const NodeP& target, const py::str& label) {
        if (std::holds_alternative<NodeP>(m)) {
            NodeP v = std::get<NodeP>(m);
            yields_slot(v, target, label);
            m = v;
        } else {
            py::list<NodeP> v = std::get<py::list<NodeP>>(m);
            yields_list(v, target, label);
            m = v;
        }
    }
    template <class T> void yields_slot(T&, const NodeP&, const py::str&) {}

    // ---- a switch on a String
    // An if/else chain for a switch on a String whose every arm leaves it (break, return, throw,
    // yield-break), or None.
    std::optional<py::list<NodeP>> string_switch(const py::ref<ir::Switch>& s, const std::optional<py::str>& label_in) {
        Pos p = pos_of(s);
        const py::list<NodeP>& cases = lst(s->cases);
        for (int64_t k = 0; k < py::len(cases); ++k) {
            auto c = as_a<ir::Case>(cases[k]);
            if (!c) none_attr("default");
            bool has_labels = c->labels && !c->labels->empty();
            bool is_default = c->default_ && *c->default_;
            if ((is_default && has_labels) ||
                (k < py::len(cases) - 1 && (lst(c->body).empty() || !leaves_arm(lst(c->body)[-1]))))
                return std::nullopt;                      // a fall-through, or labels sharing the default
        }
        py::str label = (label_in && py::truthy(*label_in)) ? *label_in : fresh("sw");
        TypeP xt = type_of(s->expr);
        SymP sel = std::make_shared<ir::Sym>(py::str("local"), fresh("s"), std::optional<py::str>(owner), false, xt);
        SymP eqs = std::make_shared<ir::Sym>(py::str("method"), py::str("equals"),
                                             std::optional<py::str>(py::str("java.lang.String")), false, nullptr,
                                             py::list<TypeP>{std::make_shared<ir::Type>(py::str("class"), std::optional<py::str>(py::str("java.lang.Object")))},
                                             ir::BOOL);
        NodeP default_body;
        std::vector<std::tuple<NodeP, NodeP>> arms;
        for (const NodeP& c_ : cases) {
            auto c = as_a<ir::Case>(c_);
            py::list<NodeP> bs;
            for (const NodeP& b : lst(c->body)) bs.append(label_breaks(b, label));
            auto body = mk<ir::Block>({c->line, c->col});
            body->stmts = bs;
            if (c->default_ && *c->default_) {
                default_body = body;
                continue;
            }
            NodeP cond;
            for (const NodeP& lab : lst(c->labels)) {
                auto sn = mk<ir::Name>(p);
                sn->sym = sel;
                sn->type = xt;
                auto test = mk<ir::Call>(p);
                test->target = lab;
                test->sym = eqs;
                test->args = py::list<NodeP>{sn};
                test->typeargs = py::list<TypeP>();
                test->type = ir::BOOL;
                if (!cond) {
                    cond = test;
                } else {
                    auto o = mk<ir::Binary>(p);
                    o->op = py::str("||");
                    o->left = cond;
                    o->right = test;
                    o->type = ir::BOOL;
                    cond = o;
                }
            }
            arms.emplace_back(cond, body);
        }
        NodeP chain = default_body;
        for (size_t i = arms.size(); i-- > 0;) {
            auto iff = mk<ir::If>(p);
            iff->cond = std::get<0>(arms[i]);
            iff->then = std::get<1>(arms[i]);
            iff->orelse = chain;
            chain = iff;
        }
        if (!chain) chain = mk<ir::Empty>(p);
        count += 1;
        auto decl = mk<ir::Local>(p);
        decl->sym = sel;
        decl->type = xt;
        decl->init = s->expr;
        decl->mods = py::list<py::str>();
        auto inner = mk<ir::Block>(p);
        inner->stmts = py::list<NodeP>{chain};
        auto lab = mk<ir::Labeled>(p);
        lab->label = label;
        lab->body = NodeP(inner);
        auto outer = mk<ir::Block>(p);
        outer->stmts = py::list<NodeP>{decl, lab};
        return py::list<NodeP>{outer};
    }

    // `s` with each unlabelled break that leaves the switch (not an inner loop or switch) labelled.
    NodeP label_breaks(const NodeP& s, const py::str& label) {
        if (auto b = as_a<ir::Break>(s)) {
            if (!b->label) {
                auto r = mk<ir::Break>({b->line, b->col});
                r->label = label;
                return r;
            }
        }
        if (is_a<ir::While>(s) || is_a<ir::DoWhile>(s) || is_a<ir::For>(s) || is_a<ir::ForEach>(s) || is_a<ir::Switch>(s))
            return s;
        if (!s) none_attr("fields");
        each_slot_of(*s, [&](const char*, auto& member, auto rank) {
            if (static_cast<int>(rank) != 0) breaks_slot(member, label);
        });
        return s;
    }
    void breaks_slot(NodeP& m, const py::str& label) {
        if (m && is_a<ir::Stmt>(m)) m = label_breaks(m, label);
    }
    void breaks_list(py::list<NodeP>& v, const py::str& label) {
        py::list<NodeP> out;
        for (const NodeP& x : v) out.append((x && is_a<ir::Stmt>(x)) ? label_breaks(x, label) : x);
        v = out;
    }
    void breaks_slot(Nodes& m, const py::str& label) {
        if (m) {
            py::list<NodeP> v = *m;
            breaks_list(v, label);
            m = v;
        }
    }
    void breaks_slot(NodeOrList& m, const py::str& label) {
        if (std::holds_alternative<NodeP>(m)) {
            NodeP v = std::get<NodeP>(m);
            breaks_slot(v, label);
            m = v;
        } else {
            py::list<NodeP> v = std::get<py::list<NodeP>>(m);
            breaks_list(v, label);
            m = v;
        }
    }
    template <class T> void breaks_slot(T&, const py::str&) {}

    // ---- instanceof patterns
    // Declarations of the pattern variables of statement `s`'s own expressions (not those of nested
    // statements, handled at their own level), each pattern rewritten to assign its variable.
    py::list<NodeP> patterns(const NodeP& s, const py::dict<py::str, TypeP>& declared) {
        py::list<NodeP> exprs;
        if (auto i = as_a<ir::If>(s)) exprs.append(i->cond);
        else if (auto w = as_a<ir::While>(s)) exprs.append(w->cond);
        else if (is_a<ir::DoWhile>(s)) {}
        else if (auto es = as_a<ir::ExprStmt>(s)) exprs.append(es->expr);
        else if (auto r = as_a<ir::Return>(s)) exprs.append(r->expr);
        else if (auto l = as_a<ir::Local>(s)) exprs.append(l->init);
        py::list<NodeP> out;
        Swap swap;
        for (const NodeP& root : exprs) {
            if (!root) continue;
            for (const NodeP& nd : own_nodes(root)) {
                auto io = as_a<ir::InstanceOf>(nd);
                if (!(io && io->binding && plain(io->x))) continue;
                auto b = as_a<ir::Local>(io->binding);
                if (!b) none_attr("sym");
                Pos p{io->line, io->col};
                if (!b->sym) none_attr("name");
                TypeP have = declared.get(b->sym->name, nullptr);
                if (!have) {
                    auto decl = std::make_shared<ir::Local>();
                    decl->sym = b->sym;
                    decl->type = b->type;
                    decl->mods = py::list<py::str>();
                    decl->line = b->line;
                    decl->col = b->col;
                    out.append(decl);
                    declared[b->sym->name] = b->type;
                } else if (!ir::same(have, b->type)) {
                    continue;                              // another type under the same name: left as it is
                }
                auto test = mk<ir::InstanceOf>(p);
                test->x = io->x;
                test->test = io->test;
                test->binding = nullptr;
                test->type = ir::BOOL;
                auto tgt = mk<ir::Name>(p);
                tgt->sym = b->sym;
                tgt->type = b->type;
                auto cast = mk<ir::Cast>(p);
                cast->x = copy_target(io->x);
                cast->type = b->type;
                auto assign = mk<ir::Assign>(p);
                assign->op = std::nullopt;
                assign->target = tgt;
                assign->value = cast;
                assign->overflow = std::nullopt;
                assign->type = b->type;
                auto par = mk<ir::Paren>(p);
                par->x = assign;
                par->type = b->type;
                auto nul = mk<ir::Lit>(p);
                nul->value = Val();
                nul->type = ir::NULL_;
                auto bind = mk<ir::Binary>(p);
                bind->op = py::str("!=");
                bind->left = par;
                bind->right = nul;
                bind->type = ir::BOOL;
                auto both = mk<ir::Binary>(p);
                both->op = py::str("&&");
                both->left = test;
                both->right = bind;
                both->type = ir::BOOL;
                auto outer = mk<ir::Paren>(p);
                outer->x = both;
                outer->type = ir::BOOL;
                swap[io.get()] = outer;
                count += 1;
            }
        }
        if (!swap.empty()) replace_in(s, swap);
        return out;
    }
};

}  // namespace

// Lower the modern forms in every method of `units` (in place). -> how many were lowered.
int64_t run(const py::list<ir::NodeP>& units, const py::list<py::str>& keep) {
    Desugar d(py::set<py::str>::from(FORMS) - py::set<py::str>::from(keep));
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        if (!u) {
            if (!un) none_attr("types");
            throw py::AttributeError("'" + un->class_name().utf8() + "' object has no attribute 'types'");
        }
        for (const NodeP& c : lst(u->types)) d.cls(c);
    }
    return d.count;
}

}  // namespace codebreaker::translate::desugar

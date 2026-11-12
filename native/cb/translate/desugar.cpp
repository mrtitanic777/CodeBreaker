// cb/translate/desugar.cpp -- the lowering pass (see desugar.hpp).
#include "cb/translate/desugar.hpp"

#include <format>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cb::translate::desugar {
namespace {

using ir::Node;
using ir::SymP;
using ir::TypeP;

constexpr std::string_view kObject = "java.lang.Object";
constexpr std::string_view kString = "java.lang.String";
constexpr std::string_view kIterator = "java.util.Iterator";

// What a position says: the line and column the new nodes of a rewrite are given (those of the node it stands for).
struct Pos {
    std::uint32_t line = 0;
    std::uint32_t col = 0;
};
Pos pos_of(const Node* n) { return {n->line, n->col}; }

// ---- reading the tree --------------------------------------------------------------------------------------------------

Node* strip(Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

bool is_string(const TypeP& t) {
    if (!t) return false;
    const auto* c = std::get_if<ir::ClassType>(&t->body);
    return c != nullptr && c->name == kString;
}

// A name, or a field of this / of a class / of a plain name: evaluating it twice is reading it twice.
bool plain(Node* e) {
    e = strip(e);
    if (ir::is<ir::Name>(e)) return true;
    if (const auto* s = ir::get<ir::Select>(e)) return ir::is<ir::This>(s->obj) || ir::is<ir::TypeName>(s->obj) || plain(s->obj);
    return false;
}

bool is_statement(const Node* n) { return n != nullptr && ir::category(*n) == ir::Category::Statement; }

// Calls f(Node*&) for every child slot of `n`, in source order, so a pass can rewire it.
template <class F>
void each_slot(Node* n, F&& f) {
    ir::visit(*n, [&f](auto& k) { k.each_child(f); });
}

// The nodes of an expression, not entering lambdas, switch expressions or anonymous classes (their statements are another
// level's), in source order.
std::vector<Node*> own_nodes(Node* root) {
    std::vector<Node*> out;
    std::vector<Node*> stack{root};
    while (!stack.empty()) {
        Node* n = stack.back();
        stack.pop_back();
        out.push_back(n);
        if (ir::is<ir::Lambda>(n) || ir::is<ir::SwitchExpr>(n) || ir::is<ir::ClassDecl>(n)) continue;
        const std::vector<Node*> kids = ir::children(*n);
        stack.insert(stack.end(), kids.rbegin(), kids.rend());
    }
    return out;
}

// The nodes from root down to target's parent, or nothing when target is not under root.
bool ancestors(Node* root, const Node* target, std::vector<Node*>& chain) {
    if (root == target) return true;
    for (Node* c : ir::children(*root)) {
        if (ir::is<ir::Lambda>(c) || ir::is<ir::ClassDecl>(c)) continue;
        chain.push_back(root);
        if (ancestors(c, target, chain)) return true;
        chain.pop_back();
    }
    return false;
}

// Every operand of the chain's nodes that is not on the path to target (each evaluated whole).
std::vector<Node*> siblings(const std::vector<Node*>& chain, Node* target) {
    std::set<const Node*> path(chain.begin(), chain.end());
    path.insert(target);
    std::vector<Node*> out;
    for (Node* n : chain) {
        for (Node* c : ir::children(*n)) {
            if (!path.contains(c)) out.push_back(c);
        }
    }
    return out;
}

// Replace, in place under `node`, each child that is a key of `swap` by its replacement.
void replace_under(Node* node, const std::unordered_map<const Node*, Node*>& swap) {
    each_slot(node, [&](Node*& slot) {
        const auto it = swap.find(slot);
        if (it != swap.end()) slot = it->second;
        else replace_under(slot, swap);
    });
}

// ---- what evaluating a node does -----------------------------------------------------------------------------------------

// Owners whose methods are lookups: no state they change.
bool pure_owner(const std::optional<std::string>& owner) {
    static const std::set<std::string, std::less<>> kPure = {
        "java.lang.String", "java.lang.Math", "java.lang.Integer", "java.lang.Float", "java.lang.Double", "java.lang.Long",
        "java.lang.Character", "java.lang.Boolean", "java.lang.Class", "java.lang.reflect.Constructor"};
    return owner && kPure.contains(*owner);
}

bool is_step(const ir::Unary& u) { return u.op == "++" || u.op == "--"; }

// Does evaluating `n` change anything (a call into the program, an allocation, an assignment, ++/--)? An array's allocation
// is not observable: its elements are walked like any operand.
bool has_effects(Node* n) {
    for (const Node* x : ir::walk(n)) {
        if (ir::is<ir::Assign>(x) || ir::is<ir::New>(x)) return true;
        if (const auto* u = ir::get<ir::Unary>(x); u != nullptr && is_step(*u)) return true;
        if (const auto* c = ir::get<ir::Call>(x); c != nullptr && !(c->sym && pure_owner(c->sym->owner))) return true;
    }
    return false;
}

// Names (locals, fields) that evaluating `n` assigns or increments.
std::set<std::string> written(Node* n) {
    std::set<std::string> out;
    for (Node* x : ir::walk(n)) {
        Node* target = nullptr;
        if (const auto* a = ir::get<ir::Assign>(x)) target = a->target;
        else if (const auto* u = ir::get<ir::Unary>(x); u != nullptr && is_step(*u)) target = u->x;
        target = strip(target);
        if (const auto* name = ir::get<ir::Name>(target); name != nullptr && name->sym) out.insert(name->sym->name);
        else if (const auto* sel = ir::get<ir::Select>(target); sel != nullptr && sel->sym) out.insert(sel->sym->name);
    }
    return out;
}

std::set<std::string> names_read(Node* n) {
    std::set<std::string> out;
    for (Node* x : ir::walk(n)) {
        if (const auto* name = ir::get<ir::Name>(x); name != nullptr && name->sym) out.insert(name->sym->name);
        else if (const auto* sel = ir::get<ir::Select>(x); sel != nullptr && sel->sym) out.insert(sel->sym->name);
    }
    return out;
}

// ---- the pass -----------------------------------------------------------------------------------------------------------

class Desugar {
public:
    Desugar(Arena& arena, std::set<Form> forms) : arena_(arena), forms_(std::move(forms)) {}

    void type_decl(Node* c);
    std::size_t count() const { return count_; }

private:
    bool has(Form f) const { return forms_.contains(f); }
    std::string fresh(std::string_view stem) { return std::format("j2x_{}{}", stem, ++n_); }
    SymP local(std::string_view stem, TypeP type) {
        return ir::make_sym(ir::SymKind::Local, fresh(stem), owner_, false, std::move(type));
    }

    template <class K>
    Node* mk(K kind, Pos p, TypeP type = {}) {
        return ir::make(arena_, std::move(kind), std::move(type), p.line, p.col);
    }
    Node* name_of(const SymP& sym, TypeP type, Pos p) { return mk(ir::Name{.sym = sym}, p, std::move(type)); }
    Node* block_of(std::vector<Node*> stmts, Pos p) { return mk(ir::Block{.stmts = std::move(stmts)}, p); }
    Node* local_decl(const SymP& sym, TypeP type, Node* init, Pos p) {
        return mk(ir::Local{.sym = sym, .type = std::move(type), .init = init}, p);
    }

    // declarations
    void nested(const ir::Suite& body);
    void block(Node* b);
    void block_suite(ir::Suite& body);

    // statements: each list is rewritten, a statement may become several
    std::vector<Node*> stmts(const std::vector<Node*>& ss);
    Node* sub(Node* s);
    ir::Suite sub_suite(ir::Suite s);
    std::vector<Node*> stmt(Node* s, std::map<std::string, TypeP>& declared);
    std::vector<Node*> stmt_core(Node* s);

    struct Loop {
        std::vector<Node*> setup;
        Node* loop = nullptr;
    };
    std::optional<Loop> foreach_loop(Node* s);

    std::optional<std::vector<Node*>> switch_expr_stmt(Node* s);
    std::optional<std::vector<Node*>> switch_expr_inside(Node* s);
    std::vector<Node*> switch_into(Node* sw, Node* target);
    Node* yields(Node* s, Node* target, const std::string& label);
    std::optional<std::vector<Node*>> string_switch(Node* s, const std::optional<std::string>& label = std::nullopt);
    std::vector<Node*> patterns(Node* s, std::map<std::string, TypeP>& declared);

    Node* copy_target(Node* e);
    Node* label_breaks(Node* s, const std::string& label);

    Arena& arena_;
    std::set<Form> forms_;
    std::size_t count_ = 0;
    int n_ = 0;
    std::string owner_;
};

// ---- declarations

void Desugar::type_decl(Node* c) {
    const auto* decl = ir::get<ir::ClassDecl>(c);
    if (decl == nullptr) return;
    for (Node* m : decl->members) {
        if (ir::is<ir::ClassDecl>(m)) {
            type_decl(m);
        } else if (auto* md = ir::get<ir::MethodDecl>(m); md != nullptr && ir::present(md->body)) {
            owner_ = decl->qname.value_or(decl->name.value_or("")) + "." + md->name.value_or("");
            block_suite(md->body);
            nested(md->body);
        } else if (const auto* ec = ir::get<ir::EnumConst>(m); ec != nullptr && ec->body != nullptr) {
            type_decl(ec->body);
        }
    }
}

// The bodies of the lambdas and anonymous classes inside a method's body.
void Desugar::nested(const ir::Suite& body) {
    for (Node* root : ir::statements(body)) {
        for (Node* n : ir::walk(root)) {
            if (const auto* lam = ir::get<ir::Lambda>(n); lam != nullptr && ir::is<ir::Block>(lam->body)) block(lam->body);
            else if (const auto* nw = ir::get<ir::New>(n); nw != nullptr && nw->body != nullptr) type_decl(nw->body);
        }
    }
}

void Desugar::block(Node* b) {
    if (auto* blk = ir::get<ir::Block>(b)) blk->stmts = stmts(blk->stmts);
}

// A body that is a block, or (as the decompiler's and the Python front end's are) a bare list of statements.
void Desugar::block_suite(ir::Suite& body) {
    if (auto* one = std::get_if<Node*>(&body)) block(*one);
    else body = stmts(std::get<std::vector<Node*>>(body));
}

// ---- statements

std::vector<Node*> Desugar::stmts(const std::vector<Node*>& ss) {
    std::vector<Node*> out;
    std::map<std::string, TypeP> declared;
    for (Node* s : ss) {
        if (s == nullptr) continue;
        std::vector<Node*> got = stmt(s, declared);
        out.insert(out.end(), got.begin(), got.end());
        if (const auto* l = ir::get<ir::Local>(s); l != nullptr && l->sym) declared[l->sym->name] = l->type;
    }
    return out;
}

// A nested statement (a loop's or an if's body): one statement back.
Node* Desugar::sub(Node* s) {
    if (s == nullptr) return nullptr;
    std::map<std::string, TypeP> declared;
    std::vector<Node*> got = stmt(s, declared);
    if (got.size() == 1) return got[0];
    return block_of(std::move(got), pos_of(s));
}

ir::Suite Desugar::sub_suite(ir::Suite s) {
    if (auto* one = std::get_if<Node*>(&s)) return sub(*one);
    return stmts(std::get<std::vector<Node*>>(s));
}

std::vector<Node*> Desugar::stmt(Node* s, std::map<std::string, TypeP>& declared) {
    std::vector<Node*> out;
    if (has(Form::Pattern)) out = patterns(s, declared);
    std::vector<Node*> core = stmt_core(s);
    out.insert(out.end(), core.begin(), core.end());
    return out;
}

std::vector<Node*> Desugar::stmt_core(Node* s) {
    const Pos p = pos_of(s);
    if (ir::is<ir::Block>(s)) {
        block(s);
        return {s};
    }
    if (auto* i = ir::get<ir::If>(s)) {
        i->then = sub_suite(std::move(i->then));
        i->orelse = sub_suite(std::move(i->orelse));
        return {s};
    }
    if (auto* w = ir::get<ir::While>(s)) {
        w->body = sub_suite(std::move(w->body));
        return {s};
    }
    if (auto* w = ir::get<ir::DoWhile>(s)) {
        w->body = sub_suite(std::move(w->body));
        return {s};
    }
    if (auto* f = ir::get<ir::For>(s)) {
        f->body = sub_suite(std::move(f->body));
        return {s};
    }
    if (auto* l = ir::get<ir::Labeled>(s)) {
        if (auto* inner = std::get_if<Node*>(&l->body); inner != nullptr && ir::is<ir::ForEach>(*inner) && has(Form::ForEach)) {
            // (the loop's own body is lowered first, as for a loop with no label)
            auto* fe = ir::get<ir::ForEach>(*inner);
            fe->body = sub(fe->body);
            if (auto got = foreach_loop(*inner)) {   // the label goes on the loop, not its set-up
                l->body = got->loop;
                if (got->setup.empty()) return {s};
                std::vector<Node*> parts = std::move(got->setup);
                parts.push_back(s);
                return {block_of(std::move(parts), p)};
            }
            return {s};
        }
        l->body = sub_suite(std::move(l->body));
        return {s};
    }
    if (auto* fe = ir::get<ir::ForEach>(s)) {
        fe->body = sub(fe->body);
        if (has(Form::ForEach)) {
            if (auto got = foreach_loop(s)) {
                if (got->setup.empty()) return {got->loop};
                std::vector<Node*> parts = std::move(got->setup);
                parts.push_back(got->loop);
                return {block_of(std::move(parts), p)};
            }
        }
        return {s};
    }
    if (auto* sw = ir::get<ir::Switch>(s)) {
        for (Node* c : sw->cases) {
            if (auto* cc = ir::get<ir::Case>(c)) cc->body = stmts(cc->body);
        }
        if (has(Form::StringSwitch) && sw->expr != nullptr && is_string(sw->expr->type)) {
            if (auto got = string_switch(s)) return *got;
        }
        return {s};
    }
    if (auto* t = ir::get<ir::Try>(s)) {
        block(t->body);
        for (Node* c : t->catches) {
            if (const auto* cc = ir::get<ir::Catch>(c)) block(cc->body);
        }
        if (t->finally_body != nullptr) block(t->finally_body);
        return {s};
    }
    if (const auto* y = ir::get<ir::Sync>(s)) {
        block(y->body);
        return {s};
    }
    if (const auto* lc = ir::get<ir::LocalClass>(s)) {
        type_decl(lc->decl);
        return {s};
    }
    if (has(Form::SwitchExpr)) {
        if (auto got = switch_expr_stmt(s)) return *got;
    }
    return {s};
}

// ---- for-each

// The set-up statements and the loop for a ForEach, or nothing when it is over something that is neither an array nor a class.
std::optional<Desugar::Loop> Desugar::foreach_loop(Node* s) {
    const auto* fe = ir::get<ir::ForEach>(s);
    const Pos p = pos_of(s);
    Node* iterable = fe->iterable;
    const TypeP t = iterable != nullptr ? iterable->type : TypeP{};
    const auto* var = ir::get<ir::Local>(fe->var);
    if (var == nullptr || !t) return std::nullopt;

    // the loop's body as a block
    std::vector<Node*> body;
    if (const auto* b = ir::get<ir::Block>(fe->body)) body = b->stmts;
    else if (fe->body != nullptr) body.push_back(fe->body);

    if (const auto* arr = std::get_if<ir::ArrayType>(&t->body)) {
        const SymP array = local("a", t);
        const SymP index = local("i", ir::i32_type());
        Node* element = mk(ir::Local{.sym = var->sym,
                                     .type = var->type,
                                     .init = mk(ir::Index{.arr = name_of(array, t, p), .index = name_of(index, ir::i32_type(), p)}, p, arr->elem),
                                     .mods = var->mods},
                           pos_of(fe->var));
        ir::For loop;
        loop.init.push_back(local_decl(index, ir::i32_type(), mk(ir::Lit{.value = std::int64_t{0}}, p, ir::i32_type()), p));
        loop.cond = mk(ir::Binary{.op = "<",
                                  .left = name_of(index, ir::i32_type(), p),
                                  .right = mk(ir::Length{.arr = name_of(array, t, p)}, p, ir::i32_type())},
                       p, ir::bool_type());
        loop.update.push_back(mk(ir::Unary{.op = "++", .x = name_of(index, ir::i32_type(), p), .postfix = true}, p, ir::i32_type()));
        std::vector<Node*> stmts_of_body{element};
        stmts_of_body.insert(stmts_of_body.end(), body.begin(), body.end());
        loop.body = block_of(std::move(stmts_of_body), p);
        ++count_;
        return Loop{{local_decl(array, t, iterable, p)}, mk(std::move(loop), p)};
    }

    if (const auto* cls = std::get_if<ir::ClassType>(&t->body)) {
        TypeP elem_t = cls->args.empty() ? ir::class_type(std::string(kObject)) : cls->args[0];
        if (elem_t && (std::holds_alternative<ir::WildcardType>(elem_t->body) || std::holds_alternative<ir::TypeVarType>(elem_t->body))) {
            const auto* wild = std::get_if<ir::WildcardType>(&elem_t->body);
            elem_t = (wild != nullptr && wild->bound && wild->bound->mode == ir::BoundMode::Extends) ? wild->bound->type
                                                                                                    : ir::class_type(std::string(kObject));
        }
        const TypeP it_t = ir::class_type(std::string(kIterator), {elem_t});
        const SymP it = local("it", it_t);
        const auto method = [&](std::string name, std::string owner, TypeP ret) {
            SymP m = ir::make_sym(ir::SymKind::Method, std::move(name), std::move(owner));
            m->ret = std::move(ret);
            return m;
        };
        Node* iterator = mk(ir::Call{.target = iterable, .sym = method("iterator", cls->name, it_t)}, p, it_t);
        Node* has_next = mk(ir::Call{.target = name_of(it, it_t, p), .sym = method("hasNext", std::string(kIterator), ir::bool_type())}, p, ir::bool_type());
        Node* next = mk(ir::Call{.target = name_of(it, it_t, p), .sym = method("next", std::string(kIterator), ir::typevar_type("E"))}, p, elem_t);
        Node* element = mk(ir::Local{.sym = var->sym, .type = var->type, .init = next, .mods = var->mods}, pos_of(fe->var));
        ir::For loop;
        loop.init.push_back(local_decl(it, it_t, iterator, p));
        loop.cond = has_next;
        std::vector<Node*> stmts_of_body{element};
        stmts_of_body.insert(stmts_of_body.end(), body.begin(), body.end());
        loop.body = block_of(std::move(stmts_of_body), p);
        ++count_;
        return Loop{{}, mk(std::move(loop), p)};
    }
    return std::nullopt;
}

// ---- switch expressions

std::optional<std::vector<Node*>> Desugar::switch_expr_stmt(Node* s) {
    const Pos p = pos_of(s);
    if (const auto* l = ir::get<ir::Local>(s); l != nullptr && ir::is<ir::SwitchExpr>(strip(l->init))) {
        Node* sw = strip(l->init);
        std::vector<Node*> out{mk(ir::Local{.sym = l->sym, .type = l->type, .init = nullptr, .mods = l->mods}, p)};
        const std::vector<Node*> got = switch_into(sw, name_of(l->sym, l->type, p));
        out.insert(out.end(), got.begin(), got.end());
        return out;
    }
    if (const auto* r = ir::get<ir::Return>(s); r != nullptr && ir::is<ir::SwitchExpr>(strip(r->expr))) {
        Node* sw = strip(r->expr);
        const SymP tmp = local("sw", sw->type);
        std::vector<Node*> body{local_decl(tmp, sw->type, nullptr, p)};
        const std::vector<Node*> got = switch_into(sw, name_of(tmp, sw->type, p));
        body.insert(body.end(), got.begin(), got.end());
        body.push_back(mk(ir::Return{.expr = name_of(tmp, sw->type, p)}, p));
        return std::vector<Node*>{block_of(std::move(body), p)};
    }
    if (const auto* e = ir::get<ir::ExprStmt>(s)) {
        const auto* a = ir::get<ir::Assign>(e->expr);
        if (a != nullptr && !a->op && ir::is<ir::SwitchExpr>(strip(a->value)) && plain(a->target)) return switch_into(strip(a->value), a->target);
    }
    return switch_expr_inside(s);
}

// A switch expression deeper in a statement's expression (`list.add(switch ...)`): taken out into a temporary before the
// statement, when that cannot change what happens -- it is the only one, nothing else in the expression has an effect but
// the calls and assignments it is an operand of (they run after it anyway), and the switch writes nothing the rest reads.
std::optional<std::vector<Node*>> Desugar::switch_expr_inside(Node* s) {
    Node* root = nullptr;
    if (const auto* e = ir::get<ir::ExprStmt>(s)) root = e->expr;
    else if (const auto* r = ir::get<ir::Return>(s)) root = r->expr;
    else if (const auto* l = ir::get<ir::Local>(s)) root = l->init;
    if (root == nullptr) return std::nullopt;
    std::vector<Node*> found;
    for (Node* n : own_nodes(root)) {
        if (ir::is<ir::SwitchExpr>(n)) found.push_back(n);
    }
    if (found.size() != 1 || found[0] == root) return std::nullopt;
    Node* sw = found[0];
    std::vector<Node*> chain;
    if (!ancestors(root, sw, chain)) return std::nullopt;
    const std::vector<Node*> others = siblings(chain, sw);
    for (Node* n : others) {
        if (has_effects(n)) return std::nullopt;
    }
    const std::set<std::string> writes = written(sw);
    for (Node* n : others) {
        for (const std::string& name : names_read(n)) {
            if (writes.contains(name)) return std::nullopt;
        }
    }
    const Pos p = pos_of(s);
    const SymP tmp = local("sw", sw->type);
    Node* decl = local_decl(tmp, sw->type, nullptr, p);
    std::vector<Node*> got = switch_into(sw, name_of(tmp, sw->type, p));
    replace_under(s, {{sw, name_of(tmp, sw->type, p)}});
    std::vector<Node*> out;
    if (ir::is<ir::Local>(s)) {
        out.push_back(decl);
        out.insert(out.end(), got.begin(), got.end());
        out.push_back(s);
        return out;
    }
    out.push_back(decl);
    out.insert(out.end(), got.begin(), got.end());
    out.push_back(s);
    return std::vector<Node*>{block_of(std::move(out), p)};
}

// The statements of switch expression `sw` assigning its value to `target` (a Name or plain Select).
std::vector<Node*> Desugar::switch_into(Node* sw, Node* target) {
    const Pos p = pos_of(sw);
    const auto* expr = ir::get<ir::SwitchExpr>(sw);
    const std::string label = fresh("sw");
    std::vector<Node*> cases;
    for (Node* c : expr->cases) {
        const auto* cc = ir::get<ir::Case>(c);
        std::vector<Node*> yielded;
        for (Node* b : cc->body) yielded.push_back(yields(b, target, label));
        cases.push_back(mk(ir::Case{.labels = cc->labels, .is_default = cc->is_default, .body = stmts(yielded)}, pos_of(c)));
    }
    Node* st = mk(ir::Switch{.expr = expr->expr, .cases = std::move(cases)}, p);
    ++count_;
    if (has(Form::StringSwitch) && expr->expr != nullptr && is_string(expr->expr->type)) {
        if (auto got = string_switch(st, label)) return *got;
    }
    return {mk(ir::Labeled{.label = label, .body = st}, p)};
}

// `s` with each Yield of this switch expression (not of one nested in it) as an assignment and a labelled break.
Node* Desugar::yields(Node* s, Node* target, const std::string& label) {
    if (s == nullptr) return s;
    const Pos p = pos_of(s);
    if (const auto* y = ir::get<ir::Yield>(s)) {
        Node* assign = mk(ir::Assign{.op = std::nullopt, .target = copy_target(target), .value = y->expr}, p, target->type);
        return block_of({mk(ir::ExprStmt{.expr = assign}, p), mk(ir::Break{.label = label}, p)}, p);
    }
    each_slot(s, [&](Node*& slot) {
        if (is_statement(slot)) slot = yields(slot, target, label);
    });
    return s;
}

// ---- a switch on a String

// An if/else chain for a switch on a String whose every arm leaves it (break, return, throw, yield-break), or nothing.
std::optional<std::vector<Node*>> Desugar::string_switch(Node* s, const std::optional<std::string>& label_in) {
    const Pos p = pos_of(s);
    const auto* sw = ir::get<ir::Switch>(s);
    const auto leaves_arm = [](auto&& self, const Node* n) -> bool {
        if (ir::is<ir::Break>(n) || ir::is<ir::Return>(n) || ir::is<ir::Throw>(n) || ir::is<ir::Continue>(n)) return true;
        if (const auto* b = ir::get<ir::Block>(n)) return !b->stmts.empty() && self(self, b->stmts.back());
        if (const auto* i = ir::get<ir::If>(n)) {
            const auto* then = std::get_if<Node*>(&i->then);
            const auto* orelse = std::get_if<Node*>(&i->orelse);
            return then != nullptr && orelse != nullptr && *orelse != nullptr && self(self, *then) && self(self, *orelse);
        }
        return false;
    };
    for (std::size_t k = 0; k < sw->cases.size(); ++k) {
        const auto* c = ir::get<ir::Case>(sw->cases[k]);
        if ((c->is_default && !c->labels.empty()) || (k + 1 < sw->cases.size() && (c->body.empty() || !leaves_arm(leaves_arm, c->body.back()))))
            return std::nullopt;   // a fall-through, or labels sharing the default
    }
    const std::string label = label_in ? *label_in : fresh("sw");
    const TypeP selector_type = sw->expr->type;
    const SymP selector = local("s", selector_type);
    SymP equals = ir::make_sym(ir::SymKind::Method, "equals", std::string(kString));
    equals->params = {ir::class_type(std::string(kObject))};
    equals->ret = ir::bool_type();
    Node* default_body = nullptr;
    std::vector<std::pair<Node*, Node*>> arms;
    for (Node* cn : sw->cases) {
        const auto* c = ir::get<ir::Case>(cn);
        std::vector<Node*> labelled;
        for (Node* b : c->body) labelled.push_back(label_breaks(b, label));
        Node* body = mk(ir::Block{.stmts = std::move(labelled)}, pos_of(cn));
        if (c->is_default) {
            default_body = body;
            continue;
        }
        Node* cond = nullptr;
        for (Node* lab : c->labels) {
            Node* test = mk(ir::Call{.target = lab, .sym = equals, .args = {name_of(selector, selector_type, p)}}, p, ir::bool_type());
            cond = cond == nullptr ? test : mk(ir::Binary{.op = "||", .left = cond, .right = test}, p, ir::bool_type());
        }
        arms.emplace_back(cond, body);
    }
    Node* chain = default_body;
    for (auto it = arms.rbegin(); it != arms.rend(); ++it) chain = mk(ir::If{.cond = it->first, .then = it->second, .orelse = chain}, p);
    if (chain == nullptr) chain = mk(ir::Empty{}, p);
    ++count_;
    Node* decl = local_decl(selector, selector_type, sw->expr, p);
    Node* labelled_chain = mk(ir::Labeled{.label = label, .body = block_of({chain}, p)}, p);
    return std::vector<Node*>{block_of({decl, labelled_chain}, p)};
}

// `s` with each unlabelled break that leaves the switch (not an inner loop or switch) labelled.
Node* Desugar::label_breaks(Node* s, const std::string& label) {
    if (s == nullptr) return s;
    if (const auto* b = ir::get<ir::Break>(s); b != nullptr && !b->label) return mk(ir::Break{.label = label}, pos_of(s));
    if (ir::is<ir::While>(s) || ir::is<ir::DoWhile>(s) || ir::is<ir::For>(s) || ir::is<ir::ForEach>(s) || ir::is<ir::Switch>(s)) return s;
    each_slot(s, [&](Node*& slot) {
        if (is_statement(slot)) slot = label_breaks(slot, label);
    });
    return s;
}

// ---- instanceof patterns

// Declarations of the pattern variables of statement `s`'s own expressions (not those of nested statements, handled at their
// own level), each pattern rewritten to assign its variable.
std::vector<Node*> Desugar::patterns(Node* s, std::map<std::string, TypeP>& declared) {
    std::vector<Node*> roots;
    if (const auto* i = ir::get<ir::If>(s)) roots.push_back(i->cond);
    else if (const auto* w = ir::get<ir::While>(s)) roots.push_back(w->cond);
    else if (const auto* e = ir::get<ir::ExprStmt>(s)) roots.push_back(e->expr);
    else if (const auto* r = ir::get<ir::Return>(s)) roots.push_back(r->expr);
    else if (const auto* l = ir::get<ir::Local>(s)) roots.push_back(l->init);
    std::vector<Node*> out;
    std::unordered_map<const Node*, Node*> swap;
    for (Node* root : roots) {
        if (root == nullptr) continue;
        for (Node* n : own_nodes(root)) {
            const auto* io = ir::get<ir::InstanceOf>(n);
            if (io == nullptr || io->binding == nullptr || !plain(io->x)) continue;
            const auto* b = ir::get<ir::Local>(io->binding);
            if (b == nullptr || !b->sym) continue;
            const Pos p = pos_of(n);
            const auto have = declared.find(b->sym->name);
            if (have == declared.end() || !have->second) {
                out.push_back(mk(ir::Local{.sym = b->sym, .type = b->type, .init = nullptr}, pos_of(io->binding)));
                declared[b->sym->name] = b->type;
            } else if (!(have->second == b->type)) {
                continue;   // another type under the same name: left as it is
            }
            Node* test = mk(ir::InstanceOf{.x = io->x, .test = io->test, .binding = nullptr}, p, ir::bool_type());
            Node* assign = mk(ir::Assign{.op = std::nullopt,
                                         .target = name_of(b->sym, b->type, p),
                                         .value = mk(ir::Cast{.x = copy_target(io->x)}, p, b->type)},
                              p, b->type);
            Node* bind = mk(ir::Binary{.op = "!=",
                                       .left = mk(ir::Paren{.x = assign}, p, b->type),
                                       .right = mk(ir::Lit{}, p, ir::null_type())},
                            p, ir::bool_type());
            swap[n] = mk(ir::Paren{.x = mk(ir::Binary{.op = "&&", .left = test, .right = bind}, p, ir::bool_type())}, p, ir::bool_type());
            ++count_;
        }
    }
    if (!swap.empty()) replace_under(s, swap);
    return out;
}

// ---- small constructions

Node* Desugar::copy_target(Node* e) {
    e = strip(e);
    if (const auto* n = ir::get<ir::Name>(e)) return mk(ir::Name{.sym = n->sym}, pos_of(e), e->type);
    if (const auto* sel = ir::get<ir::Select>(e)) {
        Node* obj = (ir::is<ir::This>(sel->obj) || ir::is<ir::TypeName>(sel->obj)) ? sel->obj : copy_target(sel->obj);
        return mk(ir::Select{.obj = obj, .sym = sel->sym}, pos_of(e), e->type);
    }
    return e;
}

}  // namespace

std::size_t run(Arena& arena, std::span<ir::Node* const> units, std::span<const Form> keep) {
    std::set<Form> forms = {Form::ForEach, Form::SwitchExpr, Form::StringSwitch, Form::Pattern};
    for (Form f : keep) forms.erase(f);
    Desugar d(arena, std::move(forms));
    for (ir::Node* u : units) {
        const auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr) continue;
        for (ir::Node* c : unit->types) d.type_decl(c);
    }
    return d.count();
}

}  // namespace cb::translate::desugar

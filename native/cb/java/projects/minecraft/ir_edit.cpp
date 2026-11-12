#include "cb/java/projects/minecraft/ir_edit.hpp"

namespace cb::java::projects::minecraft::ir_edit {

ir::Node* strip(ir::Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

namespace {

void collect_classes(ir::Node* decl, std::vector<ir::ClassDecl*>& out) {
    ir::ClassDecl* c = ir::get<ir::ClassDecl>(decl);
    if (c == nullptr) return;
    out.push_back(c);
    for (ir::Node* member : c->members) collect_classes(member, out);
}

}  // namespace

std::vector<MethodInClass> methods(const std::vector<ir::Node*>& units) {
    std::vector<ir::ClassDecl*> classes;
    for (ir::Node* u : units) {
        if (const ir::Unit* unit = ir::get<ir::Unit>(u)) {
            for (ir::Node* t : unit->types) collect_classes(t, classes);
        }
    }
    std::vector<MethodInClass> out;
    for (ir::ClassDecl* c : classes) {
        for (ir::Node* member : c->members) {
            ir::MethodDecl* m = ir::get<ir::MethodDecl>(member);
            if (m != nullptr && ir::present(m->body)) out.push_back({c, m});
        }
    }
    return out;
}

std::vector<std::vector<ir::Node*>*> stmt_lists(ir::Node* node) {
    std::vector<std::vector<ir::Node*>*> out;
    for (ir::Node* n : ir::walk(node)) {
        if (ir::Block* b = ir::get<ir::Block>(n)) out.push_back(&b->stmts);
        else if (ir::Case* c = ir::get<ir::Case>(n)) out.push_back(&c->body);
    }
    return out;
}

void replace(ir::Node* node, const std::function<ir::Node*(ir::Node*)>& fn) {
    ir::visit(*node, [&](auto& kind) {
        if constexpr (requires { kind.each_child([](ir::Node*&) {}); }) {
            kind.each_child([&](ir::Node*& slot) {
                replace(slot, fn);
                if (ir::Node* r = fn(slot)) slot = r;
            });
        }
    });
}

bool is_name(ir::Node* e, const ir::Sym& sym) {
    const ir::Name* n = ir::get<ir::Name>(strip(e));
    return n != nullptr && n->sym != nullptr && n->sym->name == sym.name;
}

bool mentions(ir::Node* node, const ir::Sym& sym) {
    for (ir::Node* n : ir::walk(node)) {
        const ir::Name* nm = ir::get<ir::Name>(n);
        if (nm != nullptr && nm->sym != nullptr && nm->sym->name == sym.name) return true;
    }
    return false;
}

bool is_call(ir::Node* e, std::string_view name, std::string_view on_field) {
    const ir::Call* c = ir::get<ir::Call>(strip(e));
    if (c == nullptr || c->sym == nullptr || c->sym->name != name) return false;
    if (on_field.empty()) return true;
    const ir::Select* t = c->target != nullptr ? ir::get<ir::Select>(strip(c->target)) : nullptr;
    return t != nullptr && ir::is<ir::This>(t->obj) && t->sym != nullptr && t->sym->name == on_field;
}

bool is_null_test(ir::Node* e, const ir::Sym& sym, std::string_view op) {
    const ir::Binary* b = ir::get<ir::Binary>(strip(e));
    if (b == nullptr || b->op != op || !is_name(b->left, sym)) return false;
    const ir::Lit* l = ir::get<ir::Lit>(strip(b->right));
    return l != nullptr && std::holds_alternative<std::monostate>(l->value);
}

std::vector<ir::Node*> block_or_single(const ir::Suite& suite) {
    if (const auto* one = std::get_if<ir::Node*>(&suite)) {
        if (const ir::Block* b = ir::get<ir::Block>(*one)) return b->stmts;
    }
    return ir::statements(suite);
}

ir::Node* lit_int(Arena& arena, std::int64_t value, ir::TypeP type) {
    return ir::make(arena, ir::Lit{.value = value}, std::move(type));
}

ir::Node* null_lit(Arena& arena) { return ir::make(arena, ir::Lit{}, ir::null_type()); }

ir::Node* true_lit(Arena& arena) { return ir::make(arena, ir::Lit{.value = true}, ir::bool_type()); }

ir::Node* name_of(Arena& arena, ir::SymP sym, ir::TypeP type) {
    ir::TypeP t = type ? std::move(type) : sym->type;
    return ir::make(arena, ir::Name{.sym = std::move(sym)}, std::move(t));
}

ir::Node* binary(Arena& arena, std::string_view op, ir::Node* left, ir::Node* right, ir::TypeP type) {
    return ir::make(arena, ir::Binary{.op = std::string(op), .left = left, .right = right}, std::move(type));
}

ir::Node* if_then(Arena& arena, ir::Node* cond, ir::Node* then) {
    if (!ir::is<ir::Block>(then)) then = ir::make(arena, ir::Block{.stmts = {then}});
    return ir::make(arena, ir::If{.cond = cond, .then = then});
}

}  // namespace cb::java::projects::minecraft::ir_edit

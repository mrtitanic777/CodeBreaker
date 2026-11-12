// Builders for the translator-IR trees the Minecraft fix-pass tests run on (header only; used by the *_test.cpp files).
#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/translate/ir.hpp"

namespace cb::java::projects::minecraft::ir_test_support {

namespace ir = cb::translate::ir;

inline ir::SymP sym(ir::SymKind kind, std::string name, ir::TypeP type = {}) {
    return ir::make_sym(kind, std::move(name), std::nullopt, false, std::move(type));
}

inline ir::Node* name(Arena& a, const ir::SymP& s, ir::TypeP type = {}) {
    return ir::make(a, ir::Name{.sym = s}, type ? type : s->type);
}

inline ir::Node* null_lit(Arena& a) { return ir::make(a, ir::Lit{}, ir::null_type()); }

inline ir::Node* int_lit(Arena& a, std::int64_t v) { return ir::make(a, ir::Lit{.value = v}, ir::i32_type()); }

inline ir::Node* float_lit(Arena& a, double v) { return ir::make(a, ir::Lit{.value = v}, ir::prim_type(ir::Prim::F32)); }

inline ir::Node* this_node(Arena& a) { return ir::make(a, ir::This{}); }

// `this.<field>`
inline ir::Node* this_field(Arena& a, const std::string& field, ir::TypeP type = {}) {
    return ir::make(a, ir::Select{.obj = this_node(a), .sym = sym(ir::SymKind::Field, field, type)}, type);
}

// `<obj>.<field>`
inline ir::Node* field(Arena& a, ir::Node* obj, const std::string& field, ir::TypeP type = {}) {
    return ir::make(a, ir::Select{.obj = obj, .sym = sym(ir::SymKind::Field, field, type)}, type);
}

// `<target>.<method>(args)`; target may be null (an own method)
inline ir::Node* call(Arena& a, ir::Node* target, const std::string& method, std::vector<ir::Node*> args = {},
                      ir::TypeP type = {}) {
    return ir::make(a, ir::Call{.target = target, .sym = sym(ir::SymKind::Method, method), .args = std::move(args)}, type);
}

inline ir::Node* binary(Arena& a, const std::string& op, ir::Node* l, ir::Node* r, ir::TypeP type = {}) {
    return ir::make(a, ir::Binary{.op = op, .left = l, .right = r}, type ? type : ir::bool_type());
}

inline ir::Node* expr_stmt(Arena& a, ir::Node* e) { return ir::make(a, ir::ExprStmt{.expr = e}); }

inline ir::Node* block(Arena& a, std::vector<ir::Node*> stmts) { return ir::make(a, ir::Block{.stmts = std::move(stmts)}); }

inline ir::Node* assign(Arena& a, ir::Node* target, ir::Node* value) {
    return ir::make(a, ir::Assign{.target = target, .value = value});
}

inline ir::Node* local(Arena& a, const ir::SymP& s, ir::Node* init, ir::TypeP type = {}) {
    return ir::make(a, ir::Local{.sym = s, .init = init}, type ? type : s->type);
}

inline ir::Node* if_(Arena& a, ir::Node* cond, ir::Node* then, ir::Node* orelse = nullptr) {
    ir::If node{.cond = cond};
    node.then = then;
    node.orelse = orelse;
    return ir::make(a, std::move(node));
}

// A class `name` with one method `method` whose body is `stmts`, in a unit.
struct Program {
    ir::Node* unit = nullptr;
    ir::Block* body = nullptr;   // the method body, for the assertions
    ir::Node* body_node = nullptr;
};

inline Program program(Arena& a, const std::string& cls, const std::string& method, std::vector<ir::Node*> stmts) {
    Program p;
    p.body_node = block(a, std::move(stmts));
    p.body = ir::get<ir::Block>(p.body_node);
    ir::MethodDecl m;
    m.sym = sym(ir::SymKind::Method, method);
    m.name = method;
    m.body = p.body_node;
    ir::Node* method_node = ir::make(a, std::move(m));
    ir::ClassDecl c;
    c.name = cls;
    c.members = {method_node};
    ir::Node* class_node = ir::make(a, std::move(c));
    ir::Unit u;
    u.types = {class_node};
    p.unit = ir::make(a, std::move(u));
    return p;
}

}  // namespace cb::java::projects::minecraft::ir_test_support

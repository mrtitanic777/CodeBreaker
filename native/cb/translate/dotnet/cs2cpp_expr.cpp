// cb/translate/dotnet/cs2cpp_expr.cpp -- C# into the C++03 back end: expressions, and the rebinding of .NET library calls to Java's (see
// cs2cpp.hpp). An expression is rewritten in place and answered; a library call is replaced by a call node of Java's.

#include <format>

#include "cb/translate/dotnet/cs2cpp_internal.hpp"

namespace cb::translate::dotnet::cs2cpp {

using namespace impl;

namespace {
namespace expr2_impl {

const ir::ClassType* class_of_type(const ir::TypeP& t) { return t ? std::get_if<ir::ClassType>(&t->body) : nullptr; }

ir::Node* lit_text(Arena& arena, std::string text) { return ir::make(arena, ir::Lit{.value = std::move(text)}, string_type()); }

bool is_null_literal(const ir::Node* x) {
    const auto* l = ir::get<ir::Lit>(x);
    return l != nullptr && std::holds_alternative<std::monostate>(l->value);
}

bool has_flag(const ir::Sym& s, std::string_view flag) { return std::find(s.flags.begin(), s.flags.end(), flag) != s.flags.end(); }

}  // namespace expr2_impl
}  // namespace

ir::Node* ToJava::typed(ir::Node* e) {
    if (e->type) e->type = ty(e->type);
    return e;
}

ir::Node* ToJava::call(ir::Node* target, ir::SymP sym_, std::vector<ir::Node*> args, ir::TypeP type) {
    return make(ir::Call{.target = target, .sym = std::move(sym_), .args = std::move(args)}, std::move(type));
}

ir::Node* ToJava::expr(ir::Node* e) {
    if (e == nullptr) throw Refuse("NoneType", "a missing expression");
    if (ir::is<ir::Lit>(e) || ir::is<ir::This>(e) || ir::is<ir::Super>(e) || ir::is<ir::TypeName>(e)) return typed(e);
    if (auto* p = ir::get<ir::Paren>(e)) {
        p->x = expr(p->x);
        return typed(e);
    }
    if (auto* n = ir::get<ir::Name>(e)) {
        n->sym = sym(n->sym);
        return typed(e);
    }
    if (ir::is<ir::Select>(e)) return write_select(e);
    if (auto* l = ir::get<ir::Length>(e)) {
        l->arr = expr(l->arr);
        return typed(e);
    }
    if (ir::is<ir::Index>(e)) return write_index(e);
    if (auto* u = ir::get<ir::Unary>(e)) {
        if (u->overflow == ir::Overflow::Trap) throw Refuse("checked", "checked arithmetic");
        if ((u->op == "++" || u->op == "--") && property_sym(u->x) != nullptr) throw Refuse("property", "++/-- on a property inside an expression");
        u->x = expr(u->x);
        return typed(e);
    }
    if (auto* b = ir::get<ir::Binary>(e)) {
        if (b->overflow == ir::Overflow::Trap) throw Refuse("checked", "checked arithmetic");
        if (b->op == "??") throw Refuse("coalesce", "??");
        b->left = expr(b->left);
        b->right = expr(b->right);
        return typed(e);
    }
    if (auto* a = ir::get<ir::Assign>(e)) {
        if (property_sym(a->target) != nullptr) throw Refuse("property", "a property assigned inside an expression");
        if (a->op && *a->op == "??") throw Refuse("coalesce", "??=");
        a->target = expr(a->target);
        a->value = expr(a->value);
        return typed(e);
    }
    if (auto* c = ir::get<ir::Cond>(e)) {
        c->cond = expr(c->cond);
        c->then = expr(c->then);
        c->orelse = expr(c->orelse);
        return typed(e);
    }
    if (auto* c = ir::get<ir::Cast>(e)) {
        if (c->overflow == ir::Overflow::Trap) throw Refuse("checked", "a checked conversion");
        c->x = expr(c->x);
        return typed(e);
    }
    if (auto* i = ir::get<ir::InstanceOf>(e)) {
        if (i->binding != nullptr) throw Refuse("pattern", "a type pattern that declares a name");
        i->x = expr(i->x);
        i->test = ty(i->test);
        return typed(e);
    }
    if (ir::is<ir::New>(e)) return write_new(e);
    if (auto* n = ir::get<ir::NewArray>(e)) {
        n->elem = ty(n->elem);
        for (ir::Node*& d : n->dims) d = expr(d);
        for (ir::Node*& x : n->init) x = expr(x);
        return typed(e);
    }
    if (auto* c = ir::get<ir::Concat>(e)) {
        for (ir::Node*& p : c->parts) p = text(p);
        return typed(e);
    }
    if (ir::is<ir::Call>(e)) return write_call(e);
    if (ir::is<ir::Lambda>(e)) throw Refuse("lambda", "a lambda: the C++03 back end has none");
    if (ir::is<ir::MethodRef>(e)) throw Refuse("delegate", "a method group as a delegate");
    if (const auto* u = ir::get<ir::Unsupported>(e)) throw Refuse(or_none(u->kind), u->raw && !u->raw->empty() ? *u->raw : or_none(u->kind));
    const std::string k(ir::kind_name(*e));
    throw Refuse(k, std::format("the expression {} on the C++03 path", k));
}

ir::Node* ToJava::text(ir::Node* p) {
    const ir::TypeP t = p->type;
    if (t && t->kind() == "bool") {
        ir::Node* test = expr(p);
        return make(ir::Cond{.cond = test, .then = expr2_impl::lit_text(arena_, "True"), .orelse = expr2_impl::lit_text(arena_, "False")}, string_type());
    }
    if (t && one_of(t->kind(), {"f32", "f64"})) throw Refuse("double-text", std::format("a {} turned into text: C#'s format is not Java's", t->kind()));
    const auto* cls = expr2_impl::class_of_type(t);
    if (cls != nullptr && cls->name != "System.String") {
        if (classes_.contains(cls->name)) return call(expr(p), method_sym("toString", cls->name, {}, string_type()), {}, string_type());
        throw Refuse("object-text", std::format("a {} turned into text", cls->name));
    }
    return expr(p);
}

ir::Node* ToJava::write_select(ir::Node* en) {
    auto* e = ir::get<ir::Select>(en);
    const ir::SymP s = e->sym;
    if (!s) throw Refuse("field", "a member access with no symbol");
    if (s->kind == ir::SymKind::Property) {
        if (is_str(s->owner, "System.String") && s->name == "Length")
            return call(expr(e->obj), method_sym("length", "java.lang.String", {}, ir::i32_type()), {}, ir::i32_type());
        if (is_str(s->owner, "System.Collections.Generic.List") && s->name == "Count")
            return call(expr(e->obj), method_sym("size", "java.util.ArrayList", {}, ir::i32_type()), {}, ir::i32_type());
        if (is_str(s->owner, "System.Exception") && s->name == "Message")
            throw Refuse("runtime", "an exception's Message: the C++03 runtime's exceptions keep no message (RuntimeException(...) {})");
        const std::optional<Accessors> acc = prop(s->owner, s->name);
        if (!acc) throw Refuse("library", std::format("the .NET property {}.{}", or_none(s->owner), s->name));
        ir::Node* obj = expr(e->obj);
        return ir::make(arena_, ir::Call{.target = ir::is<ir::This>(obj) ? nullptr : obj, .sym = acc->getter}, acc->getter->ret, en->line, en->col);
    }
    if (!s->owner || !classes_.contains(*s->owner)) throw Refuse("library", std::format("the .NET field {}.{}", or_none(s->owner), s->name));
    e->obj = expr(e->obj);
    e->sym = sym(s);
    return typed(en);
}

ir::Node* ToJava::write_index(ir::Node* en) {
    auto* e = ir::get<ir::Index>(en);
    if (e->sym) {
        const auto list_index = [&] {
            if (!std::holds_alternative<ir::Node*>(e->index)) throw Refuse("list", "the expression list on the C++03 path");
            return std::get<ir::Node*>(e->index);
        };
        if (is_str(e->sym->owner, "System.String")) {
            ir::Node* arr = expr(e->arr);
            ir::Node* idx = expr(list_index());
            return call(arr, method_sym("charAt", "java.lang.String", {ir::i32_type()}, type_of_kind("u16")), {idx}, type_of_kind("u16"));
        }
        if (is_str(e->sym->owner, "System.Collections.Generic.List")) {
            const ir::TypeP t = ty(en->type);
            ir::Node* arr = expr(e->arr);
            ir::Node* idx = expr(list_index());
            return call(arr, method_sym("get", "java.util.ArrayList", {ir::i32_type()}, ir::typevar_type("E")), {idx}, t);
        }
        throw Refuse("indexer", "an indexer of " + or_none(e->sym->owner));
    }
    if (std::holds_alternative<std::vector<ir::Node*>>(e->index)) throw Refuse("array", "a rectangular array");
    e->arr = expr(e->arr);
    e->index = expr(std::get<ir::Node*>(e->index));
    return typed(en);
}

ir::Node* ToJava::write_new(ir::Node* en) {
    auto* e = ir::get<ir::New>(en);
    const ir::TypeP t = ty(en->type);
    e->sym = sym(e->sym);
    if (e->sym) {
        const auto* created = expr2_impl::class_of_type(en->type);   // (the C# class, as the front end named it)
        if (created != nullptr) {
            if (const auto it = library_classes().find(created->name); it != library_classes().end()) e->sym->owner = it->second;
        }
    }
    for (ir::Node*& a : e->args) a = expr(a);
    en->type = t;
    return en;
}

// ---------------------------------------------------------------------------------------------- library calls
ir::Node* ToJava::write_call(ir::Node* en) {
    auto* e = ir::get<ir::Call>(en);
    const ir::SymP s = e->sym;
    if (!s) throw Refuse("call", "a call with no symbol");
    const std::optional<std::string>& o = s->owner;
    const std::vector<ir::Node*> args = e->args;
    if (is_str(o, "System.Console") && one_of(s->name, {"WriteLine", "Write"})) return write_console(*s, args);
    if (is_str(o, "System.Math") && math_functions().contains(s->name)) {
        ir::Node* type_name = make(ir::TypeName{}, ir::class_type("java.lang.Math"));
        std::vector<ir::TypeP> params;
        for (const ir::TypeP& p : s->params) params.push_back(ty(p));
        const ir::SymP ms = method_sym(math_functions().at(s->name), "java.lang.Math", std::move(params), ty(s->ret), true);
        std::vector<ir::Node*> xs;
        for (ir::Node* a : args) xs.push_back(expr(a));
        return call(type_name, ms, std::move(xs), ty(en->type));
    }
    if (is_str(o, "System.String")) return write_string_call(*en, *e, *s, args);
    if (is_str(o, "System.Collections.Generic.List")) return write_list_call(*en, *e, *s, args);
    if (is_str(o, "System.Object") && s->name == "ToString" && args.empty())
        return call(expr(e->target), method_sym("toString", "java.lang.Object", {}, string_type()), {}, string_type());
    if (!o || !classes_.contains(*o)) throw Refuse("library", std::format("the .NET call {}.{}", or_none(o), s->name));
    for (const std::string& f : s->flags) {
        if (one_of(f, {"operator", "extension", "delegate"})) throw Refuse("call", "an operator, extension or delegate call");
    }
    e->sym = sym(s);
    if (const auto it = object_methods().find(s->name); it != object_methods().end()) e->sym->name = it->second;
    else e->sym->name = s->name;
    if (e->target != nullptr) e->target = expr(e->target);
    for (ir::Node*& a : e->args) a = expr(a);
    return typed(en);
}

ir::Node* ToJava::write_console(const ir::Sym& s, const std::vector<ir::Node*>& args) {
    // System.out
    const ir::TypeP stream = ir::class_type("java.io.PrintStream");
    ir::SymP out_sym = ir::make_sym(ir::SymKind::Field, "out", "java.lang.System", true, stream);
    ir::Node* system = make(ir::TypeName{}, ir::class_type("java.lang.System"));
    ir::Node* out = make(ir::Select{.obj = system, .sym = out_sym}, stream);
    const std::string name = s.name == "WriteLine" ? "println" : "print";
    if (args.empty()) return call(out, method_sym(name, "java.io.PrintStream", {}, ir::void_type()), {}, ir::void_type());
    if (args.size() != 1) throw Refuse("library", std::format("Console.{} with a format", s.name));
    ir::Node* a = text(args[0]);
    const ir::TypeP at = a->type;
    const ir::TypeP param = at && one_of(at->kind(), {"i32", "i64", "u16"}) ? at : string_type();
    return call(out, method_sym(name, "java.io.PrintStream", {param}, ir::void_type()), {a}, ir::void_type());
}

ir::Node* ToJava::write_string_call(const ir::Node& e, const ir::Call& c, const ir::Sym& s, const std::vector<ir::Node*>& args) {
    if (expr2_impl::has_flag(s, "operator") && one_of(s.name, {"op_Equality", "op_Inequality"})) {
        if (args.size() != 2) throw Refuse("call", std::format("a string comparison with {} operands", args.size()));
        ir::Node* left = args[0];
        ir::Node* right = args[1];
        if (expr2_impl::is_null_literal(left) || expr2_impl::is_null_literal(right)) {   // against null: the reference comparison
            ir::Node* l = expr(left);
            ir::Node* r = expr(right);
            return make(ir::Binary{.op = s.name == "op_Equality" ? "==" : "!=", .left = l, .right = r}, ir::bool_type());
        }
        ir::Node* lx = expr(left);
        ir::Node* rx = expr(right);
        ir::Node* equal = call(lx, method_sym("equals", "java.lang.String", {object_type()}, ir::bool_type()), {rx}, ir::bool_type());
        if (s.name == "op_Equality") return equal;
        return make(ir::Unary{.op = "!", .x = equal}, ir::bool_type());
    }
    struct Row {
        std::string_view name;
        std::size_t arity;
        std::string_view java;
    };
    static constexpr Row kRows[] = {{"ToUpper", 0, "toUpperCase"}, {"ToLower", 0, "toLowerCase"}, {"Contains", 1, "contains"},
                                    {"Substring", 1, "substring"}, {"Equals", 1, "equals"},       {"Trim", 0, "trim"}};
    std::optional<std::string_view> java;
    for (const Row& r : kRows) {
        if (s.name == r.name && args.size() == r.arity) java = r.java;
    }
    if (!java && s.name == "Substring" && args.size() == 2 && (ir::is<ir::Lit>(args[0]) || ir::is<ir::Name>(args[0]))) {   // s.Substring(start, length)
        ir::Node* start = expr(args[0]);
        ir::Node* length = expr(args[1]);
        ir::Node* end = make(ir::Binary{.op = "+", .left = ir::clone(arena_, start), .right = length}, ir::i32_type());   // substring(start, start + length)
        ir::Node* target = expr(c.target);
        return call(target, method_sym("substring", "java.lang.String", {ir::i32_type(), ir::i32_type()}, string_type()), {start, end}, string_type());
    }
    if (!java) throw Refuse("library", std::format("string.{}/{}", s.name, args.size()));
    ir::Node* target = expr(c.target);
    std::vector<ir::TypeP> params;
    for (const ir::TypeP& p : s.params) params.push_back(ty(p));
    const ir::SymP ms = method_sym(std::string(*java), "java.lang.String", std::move(params), ty(s.ret));
    std::vector<ir::Node*> xs;
    for (ir::Node* a : args) xs.push_back(expr(a));
    return call(target, ms, std::move(xs), ty(e.type));
}

ir::Node* ToJava::write_list_call(const ir::Node& e, const ir::Call& c, const ir::Sym& s, const std::vector<ir::Node*>& args) {
    const ir::TypeP element = ir::typevar_type("E");
    const ir::TypeP object = object_type();
    struct Row {
        std::string_view name;
        std::size_t arity;
        std::string_view java;
        std::vector<ir::TypeP> params;
        ir::TypeP ret;
    };
    const std::vector<Row> rows = {{"Add", 1, "add", {element}, ir::bool_type()},
                                   {"Contains", 1, "contains", {object}, ir::bool_type()},
                                   {"Clear", 0, "clear", {}, ir::void_type()},
                                   {"Insert", 2, "add", {ir::i32_type(), element}, ir::void_type()},
                                   {"RemoveAt", 1, "remove", {ir::i32_type()}, element},
                                   {"IndexOf", 1, "indexOf", {object}, ir::i32_type()}};
    const Row* got = nullptr;
    for (const Row& r : rows) {
        if (s.name == r.name && args.size() == r.arity) got = &r;
    }
    if (got == nullptr) throw Refuse("library", std::format("List.{}/{}", s.name, args.size()));
    ir::Node* target = expr(c.target);
    std::vector<ir::Node*> xs;
    for (ir::Node* a : args) xs.push_back(expr(a));
    const ir::TypeP rt = e.type && e.type->kind() != "void" ? ty(e.type) : ir::void_type();
    return call(target, method_sym(std::string(got->java), "java.util.ArrayList", got->params, got->ret), std::move(xs), rt);
}

}  // namespace cb::translate::dotnet::cs2cpp

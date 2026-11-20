// translate/dotnet/cs2cpp.cpp -- C# into the C++03 back end: the tables, the mapping of types and symbols to Java's, and the program as a
// whole (see cs2cpp.hpp).

#include "translate/dotnet/cs2cpp.hpp"

#include <format>
#include <set>

#include "translate/dotnet/cs2cpp_internal.hpp"

namespace cb::translate::dotnet::cs2cpp {

using namespace impl;

// ---------------------------------------------------------------------------------------------- tables
namespace impl {

const std::map<std::string, std::string, std::less<>>& primitive_java() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"bool", "boolean"}, {"i8", "byte"}, {"i16", "short"}, {"u16", "char"}, {"i32", "int"},
        {"i64", "long"},     {"f32", "float"}, {"f64", "double"}, {"void", "void"}};
    return m;
}

const std::map<std::string, std::string, std::less<>>& boxed_classes() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"bool", "java.lang.Boolean"}, {"i8", "java.lang.Byte"},    {"i16", "java.lang.Short"}, {"u16", "java.lang.Character"},
        {"i32", "java.lang.Integer"},  {"i64", "java.lang.Long"},   {"f32", "java.lang.Float"}, {"f64", "java.lang.Double"}};
    return m;
}

const std::map<std::string, std::string, std::less<>>& library_classes() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"System.String", "java.lang.String"},
        {"System.Object", "java.lang.Object"},
        {"System.Collections.Generic.List", "java.util.ArrayList"},
        {"System.Exception", "java.lang.RuntimeException"},
        {"System.InvalidOperationException", "java.lang.IllegalStateException"},
        {"System.ArgumentException", "java.lang.IllegalArgumentException"},
        {"System.DivideByZeroException", "java.lang.ArithmeticException"},
        {"System.ArithmeticException", "java.lang.ArithmeticException"},
        {"System.NullReferenceException", "java.lang.NullPointerException"},
        {"System.IndexOutOfRangeException", "java.lang.ArrayIndexOutOfBoundsException"},
        {"System.Math", "java.lang.Math"},
    };
    return m;
}

const std::map<std::string, std::string, std::less<>>& math_functions() {
    static const std::map<std::string, std::string, std::less<>> m = {
        {"Sqrt", "sqrt"}, {"Abs", "abs"},     {"Max", "max"},     {"Min", "min"}, {"Sin", "sin"}, {"Cos", "cos"},
        {"Floor", "floor"}, {"Pow", "pow"},   {"Atan2", "atan2"}, {"Tan", "tan"}, {"Exp", "exp"}, {"Log", "log"}};
    return m;
}

const std::map<std::string, std::string, std::less<>>& object_methods() {
    static const std::map<std::string, std::string, std::less<>> m = {{"ToString", "toString"}, {"Equals", "equals"}, {"GetHashCode", "hashCode"}};
    return m;
}

ir::TypeP string_type() {
    static const ir::TypeP t = ir::class_type("java.lang.String");
    return t;
}
ir::TypeP object_type() {
    static const ir::TypeP t = ir::class_type("java.lang.Object");
    return t;
}

ir::TypeP type_of_kind(std::string_view kind) {
    static const std::map<std::string, ir::Prim, std::less<>> prims = {
        {"bool", ir::Prim::Bool}, {"i8", ir::Prim::I8},   {"i16", ir::Prim::I16}, {"u16", ir::Prim::U16},
        {"i32", ir::Prim::I32},   {"i64", ir::Prim::I64}, {"f32", ir::Prim::F32}, {"f64", ir::Prim::F64}};
    if (kind == "void") return ir::void_type();
    const auto it = prims.find(kind);
    CB_ASSERT(it != prims.end(), "a primitive kind");
    return ir::prim_type(it->second);
}

ir::SymP method_sym(std::string name, std::optional<std::string> owner, std::vector<ir::TypeP> params, ir::TypeP ret, bool is_static) {
    ir::SymP s = ir::make_sym(ir::SymKind::Method, std::move(name), std::move(owner), is_static);
    s->params = std::move(params);
    s->ret = std::move(ret);
    return s;
}

}  // namespace impl

// ---------------------------------------------------------------------------------------------- ToJava
ToJava::ToJava(Arena& arena, std::span<ir::Node* const> units) : arena_(arena), units_(units.begin(), units.end()) {
    for (ir::Node* un : units_) {
        const auto* u = ir::get<ir::Unit>(un);
        CB_ASSERT(u != nullptr, "the program's units are Unit nodes");
        for (ir::Node* c : u->types) {
            const auto* cd = ir::get<ir::ClassDecl>(c);
            if (cd != nullptr && cd->qname) classes_.insert_or_assign(*cd->qname, c);
        }
    }
}

// ---- types
ir::TypeP ToJava::ty(const ir::TypeP& t, bool boxed) {
    if (!t) return {};
    const std::string_view k = t->kind();
    if (one_of(k, {"u8", "u32", "u64"}))
        throw Refuse("unsigned", std::format("an unsigned integer ({}): Java and its C++ runtime have none", k));
    if (const auto it = primitive_java().find(k); it != primitive_java().end()) {
        if (boxed && k != "void") return ir::class_type(boxed_classes().at(std::string(k)));
        return type_of_kind(k);
    }
    if (const auto* arr = std::get_if<ir::ArrayType>(&t->body)) {
        if (arr->rank != 1) throw Refuse("array", "a rectangular array");
        return ir::array_of(ty(arr->elem));
    }
    if (const auto* cls = std::get_if<ir::ClassType>(&t->body)) {
        const std::string& name = cls->name;
        if (name.starts_with("java.")) return t;   // (already Java's)
        if (name == "System.Nullable") throw Refuse("nullable", "a nullable value type");
        if (const auto c = classes_.find(name)) {
            const auto* decl = ir::get<ir::ClassDecl>(*c);
            if (!decl->kind || !one_of(*decl->kind, {"class", "interface"}))
                throw Refuse(or_none(decl->kind), std::format("a C# {}: no faithful form on the C++03 path", or_none(decl->kind)));
            return ir::class_type(name);
        }
        if (const auto it = library_classes().find(name); it != library_classes().end()) {
            std::vector<ir::TypeP> args;
            for (const ir::TypeP& a : cls->args) args.push_back(ty(a, true));
            return ir::class_type(it->second, std::move(args));
        }
        throw Refuse("library", std::format("the .NET type {} has no mapping onto Java's runtime", name));
    }
    if (k == "null" || k == "any") return t;
    if (const auto* tv = std::get_if<ir::TypeVarType>(&t->body)) throw Refuse("generic", std::format("a type parameter ({})", tv->name));
    throw Refuse("type", "the type " + ir::to_text(*t));
}

ir::SymP ToJava::sym(const ir::SymP& s) {
    if (!s) return {};
    std::optional<std::string> owner = s->owner;
    if (owner) {
        if (const auto it = library_classes().find(*owner); it != library_classes().end()) owner = it->second;
    }
    std::vector<ir::TypeP> params;
    for (const ir::TypeP& p : s->params) params.push_back(ty(p));
    ir::SymP out = ir::make_sym(s->kind, s->name, std::move(owner), s->is_static, s->type ? ty(s->type) : ir::TypeP());
    out->params = std::move(params);
    out->ret = s->ret ? ty(s->ret) : ir::TypeP();
    out->varargs = s->varargs;
    return out;
}

// ---- the program
std::vector<Refusal> ToJava::run() {
    for (ir::Node* un : units_) {
        auto* u = ir::get<ir::Unit>(un);
        std::vector<ir::Node*> kept;
        for (ir::Node* c : u->types) {
            try {
                check_class(*c);
                kept.push_back(c);
            } catch (const Refuse& r) {
                refused_.push_back({ir::get<ir::ClassDecl>(c)->name, r.category(), r.why()});
            }
        }
        u->types = std::move(kept);
    }
    // (every class of the program, a class refused above too)
    for (const auto& [qname, node] : classes_) {
        const auto& c = *ir::get<ir::ClassDecl>(node);
        try {
            collect_props(c);
        } catch (const Refuse& r) {
            refused_.push_back({c.name, r.category(), r.why()});
        }
    }
    for (ir::Node* un : units_) {
        auto* u = ir::get<ir::Unit>(un);
        for (ir::Node* c : u->types) {
            try {
                klass(*c);
            } catch (const Refuse& r) {
                refused_.push_back({ir::get<ir::ClassDecl>(c)->name, r.category(), r.why()});
            }
        }
        u->lang = "java";
    }
    return refused_;
}

void ToJava::check_class(const ir::Node& cn) {
    const auto& c = *ir::get<ir::ClassDecl>(&cn);
    if (!c.kind || !one_of(*c.kind, {"class", "interface"}))
        throw Refuse(or_none(c.kind), std::format("a C# {}: no faithful form on the C++03 path", or_none(c.kind)));
    if (!c.typeparams.empty()) throw Refuse("generic", std::format("a generic class ({})", or_none(c.name)));
    for (const ir::Node* m : c.members) {
        if (const auto* nested = ir::get<ir::ClassDecl>(m))
            throw Refuse("nested", std::format("a nested class ({}.{}): the transpiler finds bodies by top-level class", or_none(c.name), or_none(nested->name)));
        if (const auto* un = ir::get<ir::Unsupported>(m)) {
            const std::string raw = un->raw && !un->raw->empty() ? *un->raw : or_none(un->kind);
            throw Refuse(or_none(un->kind), raw);
        }
    }
}

void ToJava::collect_props(const ir::ClassDecl& c) {
    const std::string owner = c.qname.value_or("");
    for (const ir::Node* mn : c.members) {
        const auto* m = ir::get<ir::Property>(mn);
        if (m == nullptr) continue;
        if (!m->params.empty()) throw Refuse("indexer", "an indexer");
        if (!m->sym) throw Refuse("property", "a property with no symbol");
        const ir::TypeP t = ty(m->type);
        const bool is_static = m->sym->is_static;
        ir::SymP g = method_sym("get" + m->sym->name, c.qname, {}, t, is_static);
        bool writable = false;
        bool all_absent = true;
        for (const ir::Node* an : m->accessors) {
            const auto* a = ir::get<ir::MethodDecl>(an);
            if (a == nullptr) throw Refuse("property", "an accessor that is no method");
            if (a->kind && (*a->kind == "set" || *a->kind == "init")) writable = true;
            if (!body_absent(a->body)) all_absent = false;
        }
        ir::SymP s = writable ? method_sym("set" + m->sym->name, c.qname, {t}, ir::void_type(), is_static) : ir::SymP();
        const PropKey key(owner, m->sym->name);
        props_[key] = Accessors{g, s, is_static};
        const bool abstract_ = std::find(m->mods.begin(), m->mods.end(), "abstract") != m->mods.end();
        if (all_absent && !abstract_ && !(c.kind && *c.kind == "interface"))
            auto_[key] = ir::make_sym(ir::SymKind::Field, "_" + m->sym->name, c.qname, is_static, t);
    }
}

std::optional<Accessors> ToJava::prop(const std::optional<std::string>& owner_in, const std::string& name) const {
    std::set<std::string> seen;
    std::optional<std::string> owner = owner_in;
    while (owner && !owner->empty() && !seen.contains(*owner)) {
        seen.insert(*owner);
        if (const auto it = props_.find(PropKey(*owner, name)); it != props_.end()) return it->second;
        const auto c = classes_.find(*owner);
        if (!c) break;
        const auto& decl = *ir::get<ir::ClassDecl>(*c);
        owner = std::nullopt;
        if (decl.extends) {
            if (const auto* e = std::get_if<ir::ClassType>(&decl.extends->body)) owner = e->name;
        }
        for (const ir::TypeP& i : decl.implements) {
            if (const auto* it_class = std::get_if<ir::ClassType>(&i->body)) {
                if (const auto found = props_.find(PropKey(it_class->name, name)); found != props_.end()) return found->second;
            }
        }
    }
    return std::nullopt;
}

}  // namespace cb::translate::dotnet::cs2cpp

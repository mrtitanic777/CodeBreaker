// translate/backends/cpp17_tree/core.cpp -- the C++17 back end's shared part on the typed tree (cpp17tree.hpp; the L2 core
// library's cpp17_core.cc): reading the tree by its slots' names, ir.Type's parts, the emitter's C++17 forms, and the choice of the
// source language's rules.
// cb-unity: alone (its file-local helpers share names with the rule sets')
#include <algorithm>
#include <cmath>
#include <limits>
#include <variant>

#include <nlohmann/json.hpp>

#include "translate/backends/cpp17_tree/cpp17tree.hpp"
#include "translate/l2.hpp"

#if defined(_MSC_VER)
#pragma warning(push, 3)
#endif
#include "translate/l2/src/json.hpp"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace cb::translate::cpp17tree {

Lang semantics(const V& lang) {
    if (lang.is_none()) return Lang::Java;              // L2 from the Java front end says no language
    std::string l = lang.is_str() ? lang.str() : std::string();
    if (l == "python") return Lang::Python;
    if (l == "java") return Lang::Java;
    throw Decline("no C++17 rules for " + (l.empty() ? py::repr(lang) : l) + " yet");
}

// ---------------------------------------------------------------------------------------------- the arena of the call
namespace {
thread_local Arena* made_in = nullptr;
}
Made::Made(Arena& a) : was(made_in) { made_in = &a; }
Made::~Made() { made_in = was; }
Arena& arena() {
    if (made_in == nullptr) throw std::runtime_error("cpp17tree: no arena for the nodes a writer makes");
    return *made_in;
}

// ---------------------------------------------------------------------------------------------- the tree
namespace {
const std::vector<NodeP> NO_NODES;

V text_v(const std::optional<std::string>& s) { return s ? V(*s) : V(); }

V lit_v(const ir::LitValue& v) {
    switch (v.index()) {
        case 1: return V(std::get<bool>(v));
        case 2: return V(static_cast<int64_t>(std::get<std::int64_t>(v)));
        case 3: return V(static_cast<unsigned long long>(std::get<std::uint64_t>(v)));
        case 4: return V(std::get<double>(v));
        case 5: return V(std::get<std::string>(v));
        default: return V();
    }
}

V type_v(const TypeP& t) { return py::json_parse(l2::type_to_json(t).dump()); }
}  // namespace

NodeP node(NodeP n, const char* slot) {
    if (n == nullptr) return nullptr;
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<ir::Node**>(&s)) return **p;
    if (const auto* p = std::get_if<ir::Suite*>(&s))
        if (const auto* one = std::get_if<ir::Node*>(*p)) return *one;
    return nullptr;
}

const std::vector<NodeP>& nodes(NodeP n, const char* slot) {
    if (n == nullptr) return NO_NODES;
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<std::vector<ir::Node*>*>(&s)) return **p;
    if (const auto* p = std::get_if<ir::Suite*>(&s))
        if (const auto* many = std::get_if<std::vector<ir::Node*>>(*p)) return *many;
    return NO_NODES;
}

TypeP type(NodeP n, const char* slot) {
    if (n == nullptr) return TypeP();
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<ir::TypeP*>(&s)) return **p;
    return TypeP();
}

std::vector<TypeP> types(NodeP n, const char* slot) {
    if (n == nullptr) return {};
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<std::vector<ir::TypeP>*>(&s)) return **p;
    if (const auto* p = std::get_if<ir::TypeP*>(&s)) {
        if (**p) return {**p};
        return {};
    }
    if (const auto* p = std::get_if<std::vector<ir::TypeArg>*>(&s)) {
        // (a list of types only: the library held it as types; with a text or a number in it, as its raw value)
        std::vector<TypeP> out;
        for (const ir::TypeArg& a : **p) {
            const auto* t = std::get_if<ir::TypeP>(&a);
            if (t == nullptr || !*t) return {};
            out.push_back(*t);
        }
        return out;
    }
    return {};
}

SymP sym(NodeP n, const char* slot) {
    if (n == nullptr) return SymP();
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<ir::SymP*>(&s)) return **p;
    return SymP();
}

V raw(NodeP n, const char* slot) {
    if (n == nullptr) return V();
    const l2::SlotPtr s = l2::slot(*n, slot);
    return std::visit(
        [&](auto p) -> V {
            using P = std::remove_cv_t<std::remove_pointer_t<decltype(p)>>;
            if constexpr (std::is_same_v<decltype(p), std::monostate>) return V();
            else if constexpr (std::is_same_v<P, std::optional<std::string>>) return text_v(*p);
            else if constexpr (std::is_same_v<P, std::string>) return V(*p);
            else if constexpr (std::is_same_v<P, std::vector<std::string>>) {
                V out = V::list();
                for (const std::string& x : *p) out.append(V(x));
                return out;
            } else if constexpr (std::is_same_v<P, bool>) return V(*p);
            else if constexpr (std::is_same_v<P, std::optional<bool>>) return *p ? V(**p) : V();
            else if constexpr (std::is_same_v<P, std::optional<std::int64_t>>) return *p ? V(static_cast<int64_t>(**p)) : V();
            else if constexpr (std::is_same_v<P, std::optional<std::uint64_t>>) return *p ? V(static_cast<unsigned long long>(**p)) : V();
            else if constexpr (std::is_same_v<P, std::uint32_t>) return V(static_cast<int64_t>(*p));
            else if constexpr (std::is_same_v<P, std::optional<ir::Overflow>>) return *p ? V(std::string(ir::to_text(**p))) : V();
            else if constexpr (std::is_same_v<P, std::optional<ir::MemoryModel>>) return *p ? V(std::string(ir::to_text(**p))) : V();
            else if constexpr (std::is_same_v<P, ir::LitValue>) return lit_v(*p);
            else if constexpr (std::is_same_v<P, std::vector<ir::TypeArg>>) {
                // (as the library held it: types only are types (no raw value); a list with a text or a number in it is raw)
                bool all_types = !p->empty();
                for (const ir::TypeArg& a : *p) all_types = all_types && std::holds_alternative<ir::TypeP>(a);
                if (all_types) return V();
                V out = V::list();
                for (const ir::TypeArg& a : *p) {
                    if (const auto* t = std::get_if<ir::TypeP>(&a)) out.append(type_v(*t));
                    else if (const auto* x = std::get_if<std::string>(&a)) out.append(V(*x));
                    else out.append(V(static_cast<int64_t>(std::get<std::int64_t>(a))));
                }
                return out;
            } else return V();
        },
        s);
}

void set_type(NodeP n, const TypeP& t, const char* slot) {
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<ir::TypeP*>(&s)) **p = t;
    else py::raise("AttributeError", std::string(ir::kind_name(*n)) + " has no slot " + slot);
}

void put(NodeP n, const char* slot, NodeP child) {
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<ir::Node**>(&s)) **p = child;
    else if (const auto* q = std::get_if<ir::Suite*>(&s)) **q = child;
    else py::raise("AttributeError", std::string(ir::kind_name(*n)) + " has no slot " + slot);
}

void put_sym(NodeP n, const SymP& sy, const char* slot) {
    const l2::SlotPtr s = l2::slot(*n, slot);
    if (const auto* p = std::get_if<ir::SymP*>(&s)) **p = sy;
    else py::raise("AttributeError", std::string(ir::kind_name(*n)) + " has no slot " + slot);
}

NodeP make_node(const char* kind) {
    NodeP n = ir::make_node(arena(), kind);
    if (n == nullptr) py::raise("KeyError", py::repr(V(std::string(kind))));
    return n;
}

std::vector<std::string> mods(NodeP n) {
    if (n == nullptr) return {};
    const l2::SlotPtr s = l2::slot(*n, "mods");
    if (const auto* p = std::get_if<std::vector<std::string>*>(&s)) return **p;
    return {};
}

bool has_mod(NodeP n, const std::string& m) {
    if (n == nullptr) return false;
    const l2::SlotPtr s = l2::slot(*n, "mods");
    if (const auto* p = std::get_if<std::vector<std::string>*>(&s))
        for (const std::string& x : **p)
            if (x == m) return true;
    return false;
}

std::string str(const V& v) { return v.is_str() ? v.str() : std::string(); }

std::vector<NodeP> children(NodeP n) {
    if (n == nullptr) return {};
    return ir::children(*n);
}

std::vector<NodeP> tree_walk(NodeP root) {
    std::vector<NodeP> out, stack{root};
    while (!stack.empty()) {
        NodeP n = stack.back();
        stack.pop_back();
        out.push_back(n);
        auto c = children(n);
        for (auto it = c.rbegin(); it != c.rend(); ++it) stack.push_back(*it);
    }
    return out;
}

// ---------------------------------------------------------------------------------------------- types
namespace {
const std::vector<TypeP> NO_TYPES;
}

std::string tkind(const TypeP& t) { return t ? std::string(t->kind()) : std::string(); }

std::string tname(const TypeP& t) {
    if (!t) return std::string();
    if (const auto* c = std::get_if<ir::ClassType>(&t->body)) return c->name;
    if (const auto* v = std::get_if<ir::TypeVarType>(&t->body)) return v->name;
    if (const auto* r = std::get_if<ir::RecordType>(&t->body)) return r->name.value_or(std::string());
    return std::string();
}

const std::vector<TypeP>& targs(const TypeP& t) {
    if (t)
        if (const auto* c = std::get_if<ir::ClassType>(&t->body)) return c->args;
    return NO_TYPES;
}

TypeP telem(const TypeP& t) {
    if (!t) return TypeP();
    if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) return a->elem;
    if (const auto* p = std::get_if<ir::PtrType>(&t->body)) return p->to;
    return TypeP();
}

bool tinfo(const TypeP& t, const char* key) {
    if (!t) return false;
    const std::string_view k = key;
    if (const auto* c = std::get_if<ir::ClassType>(&t->body)) {
        if (k == "ref") return c->ref.value_or(false);
        if (k == "record") return c->record.value_or(false);
        if (k == "exception") return c->exception.value_or(false);
        if (k == "nullable") return c->nullable;
        return false;
    }
    if (const auto* p = std::get_if<ir::PrimType>(&t->body)) {
        if (k == "signed") return p->is_signed.value_or(false);
        if (k == "enc") return p->encoding.has_value();
        return false;
    }
    if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) {
        if (k == "count") return a->count.value_or(0) != 0;
        if (k == "rank") return a->rank != 1;
        return false;
    }
    return false;
}

const ir::WildcardBound* tbound(const TypeP& t) {
    if (!t) return nullptr;
    if (const auto* w = std::get_if<ir::WildcardType>(&t->body))
        if (w->bound) return &*w->bound;
    return nullptr;
}

bool type_eq(const TypeP& a, const TypeP& b) { return a == b; }

TypeP mk_type(const std::string& kind, const std::string& name, std::vector<TypeP> args, TypeP elem) {
    static const std::pair<const char*, ir::Prim> prims[] = {
        {"bool", ir::Prim::Bool}, {"i8", ir::Prim::I8},   {"i16", ir::Prim::I16}, {"i32", ir::Prim::I32}, {"i64", ir::Prim::I64},
        {"u8", ir::Prim::U8},     {"u16", ir::Prim::U16}, {"u32", ir::Prim::U32}, {"u64", ir::Prim::U64}, {"f32", ir::Prim::F32},
        {"f64", ir::Prim::F64},   {"f80", ir::Prim::F80}, {"char", ir::Prim::Char}};
    for (const auto& [k, p] : prims)
        if (kind == k) return ir::prim_type(p);
    if (kind == "class") {
        ir::ClassType c;
        c.name = name;
        c.args = std::move(args);
        return ir::make_type(std::move(c));
    }
    if (kind == "typevar") return ir::typevar_type(name);
    if (kind == "array") {
        ir::ArrayType a;
        a.elem = std::move(elem);
        return ir::make_type(std::move(a));
    }
    if (kind == "void") return ir::void_type();
    if (kind == "null") return ir::null_type();
    if (kind == "any") return ir::any_type();
    if (kind == "dynamic") return ir::make_type(ir::DynamicType{});
    if (kind == "wildcard") return ir::make_type(ir::WildcardType{});
    py::raise("ValueError", "no type kind " + kind);
}

bool is_prim(const TypeP& t) { return t && t->is_prim(); }
bool is_ref_type(const TypeP& t) { return t && t->is_ref(); }

std::string type_repr(const TypeP& t) {
    if (!t) return "None";
    const std::string k = tkind(t);
    if (k == "array") return type_repr(telem(t)) + "[]";
    if (k == "class" || k == "typevar") {
        std::string s = tname(t);
        if (!targs(t).empty()) {
            Strs a;
            for (auto& x : targs(t)) a.push_back(type_repr(x));
            s += "<" + join(a, ", ") + ">";
        }
        return s;
    }
    if (k == "wildcard") {
        const ir::WildcardBound* b = tbound(t);
        return "?" + (b ? " " + std::string(b->mode == ir::BoundMode::Extends ? "extends" : "super") + " " + type_repr(b->type) : "");
    }
    return k;
}

std::string sname(const SymP& s) { return s ? s->name : std::string(); }
std::string sowner(const SymP& s) { return s ? s->owner.value_or(std::string()) : std::string(); }
std::string skind(const SymP& s) { return s ? std::string(ir::to_text(s->kind)) : std::string(); }
bool sstatic(const SymP& s) { return s && s->is_static; }

// ---------------------------------------------------------------------------------------------- the emitter
void Emitter::emit_block(NodeP s, Strs& out, const std::string& level) {
    out.push_back(level + "{");
    nested(s, out, level);
    out.push_back(level + "}");
}

void Emitter::emit_if(NodeP s, Strs& out, const std::string& level) {
    // if / else if / else: an If whose orelse is an If is one chain
    out.push_back(level + "if (" + cond_text(node(s, "cond")) + ") {");
    nested(node(s, "then"), out, level);
    NodeP e = node(s, "orelse");
    while (is(e, "If")) {
        out.push_back(level + "} else if (" + cond_text(node(e, "cond")) + ") {");
        nested(node(e, "then"), out, level);
        e = node(e, "orelse");
    }
    if (e) {
        out.push_back(level + "} else {");
        nested(e, out, level);
    }
    out.push_back(level + "}");
}

void Emitter::emit_while(NodeP s, Strs& out, const std::string& level) {
    out.push_back(level + "while (" + cond_text(node(s, "cond")) + ") {");
    loop_nested(node(s, "body"), out, level);
    out.push_back(level + "}");
}

std::string Emitter::binding(const Strs& names, const std::string& qual) { return qual + " [" + join(names, ", ") + "]"; }

// ---------------------------------------------------------------------------------------------- text
std::string join(const Strs& v, const std::string& sep) {
    std::string s;
    for (size_t i = 0; i < v.size(); i++) {
        if (i) s += sep;
        s += v[i];
    }
    return s;
}

std::string fmt_int(int64_t v) { return std::to_string(v); }
bool starts(const std::string& s, const std::string& p) { return s.compare(0, p.size(), p) == 0 && s.size() >= p.size(); }
bool ends(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(s.size() - p.size(), p.size(), p) == 0;
}

std::string float_text(double v, const std::string& suffix) {
    if (std::isnan(v) || std::isinf(v)) throw Decline("non-finite literal");
    std::string s = py::float_repr(v);
    size_t e = s.find_first_of("eE");
    if (e != std::string::npos) {
        std::string mant = s.substr(0, e), exp = s.substr(e + 1);
        if (mant.find('.') == std::string::npos) mant += ".0";
        s = mant + "e" + exp;
    } else if (s.find('.') == std::string::npos) {
        s += ".0";
    }
    return s + suffix;
}

bool lit_float(NodeP lit, double& out) {
    const V v = raw(lit, "value");
    if (v.is_float()) {
        out = v.f;
        return true;
    }
    if (v.is_int()) {
        out = (double)v.as_int();
        return true;
    }
    if (v.is_str()) {                            // l2.to_json writes a float that is not finite as its repr
        const std::string& s = v.str();
        if (s == "inf") out = std::numeric_limits<double>::infinity();
        else if (s == "-inf") out = -std::numeric_limits<double>::infinity();
        else if (s == "nan") out = std::numeric_limits<double>::quiet_NaN();
        else return false;
        return true;
    }
    return false;
}

}  // namespace cb::translate::cpp17tree

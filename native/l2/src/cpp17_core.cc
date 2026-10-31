// cpp17_core.cc -- the C++17 back end's shared part: reading the L2 tree, ir.Type's equality, the
// emitter's C++17 forms, and the choice of the source language's rules (cpp17.hpp).
#include <algorithm>
#include <cmath>
#include <limits>

#include "cpp17.hpp"

namespace cpp17 {

Lang semantics(const V& lang) {
    if (lang.is_none()) return Lang::Java;              // L2 from the Java front end says no language
    std::string l = lang.is_str() ? lang.str() : std::string();
    if (l == "python") return Lang::Python;
    if (l == "java") return Lang::Java;
    throw Decline("no C++17 rules for " + (l.empty() ? py::repr(lang) : l) + " yet");
}

// ---------------------------------------------------------------------------------------------- the tree
static const std::vector<NodeP> NO_NODES;
static const V NONE_V;

const l2::Item* item(const NodeP& n, const char* slot) { return n ? n->find(slot) : nullptr; }

NodeP node(const NodeP& n, const char* slot) {
    const l2::Item* it = item(n, slot);
    return it && it->kind == l2::Item::NodeK ? it->node : nullptr;
}

const std::vector<NodeP>& nodes(const NodeP& n, const char* slot) {
    const l2::Item* it = item(n, slot);
    return it && it->kind == l2::Item::Nodes ? it->nodes : NO_NODES;
}

TypeP type(const NodeP& n, const char* slot) {
    const l2::Item* it = item(n, slot);
    return it && it->kind == l2::Item::TypeK ? it->type : nullptr;
}

std::vector<TypeP> types(const NodeP& n, const char* slot) {
    const l2::Item* it = item(n, slot);
    if (it && it->kind == l2::Item::Types) return it->types;
    if (it && it->kind == l2::Item::TypeK) return {it->type};
    return {};
}

SymP sym(const NodeP& n, const char* slot) {
    const l2::Item* it = item(n, slot);
    return it && it->kind == l2::Item::SymK ? it->sym : nullptr;
}

const V& raw(const NodeP& n, const char* slot) {
    const l2::Item* it = item(n, slot);
    return it && it->kind == l2::Item::Raw ? it->raw : NONE_V;
}

void set_type(const NodeP& n, const TypeP& t, const char* slot) {
    l2::Item& it = n->slot(slot);
    it = l2::Item();
    if (t) {
        it.kind = l2::Item::TypeK;
        it.type = t;
    }
}

std::vector<std::string> mods(const NodeP& n) {
    std::vector<std::string> out;
    const V& m = raw(n, "mods");
    if (m.is_seq())
        for (auto& x : m.seq())
            if (x.is_str()) out.push_back(x.str());
    return out;
}

bool has_mod(const NodeP& n, const std::string& m) {
    for (auto& x : mods(n))
        if (x == m) return true;
    return false;
}

std::string str(const V& v) { return v.is_str() ? v.str() : std::string(); }

std::vector<NodeP> children(const NodeP& n) {
    std::vector<NodeP> out;
    if (!n) return out;
    for (auto& f : n->cls->fields) {
        const l2::Item* it = n->find(f);
        if (!it) continue;
        if (it->kind == l2::Item::NodeK && it->node) out.push_back(it->node);
        else if (it->kind == l2::Item::Nodes)
            for (auto& x : it->nodes)
                if (x) out.push_back(x);
    }
    return out;
}

std::vector<NodeP> walk(const NodeP& root) {
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
std::string tkind(const TypeP& t) { return t ? t->kind : std::string(); }
std::string tname(const TypeP& t) { return t && t->name.is_str() ? t->name.str() : std::string(); }
bool tinfo(const TypeP& t, const char* key) { return t && py::truthy(t->info.get(key)); }

static std::string info_key(const TypeP& t) {
    // repr(sorted(t.info.items())) of what a Type of the Java and Python front ends holds
    std::vector<std::pair<std::string, std::string>> kv;
    for (auto& p : t->info.items()) kv.push_back({py::str_of(p.first), py::repr(p.second)});
    std::sort(kv.begin(), kv.end());
    std::string s;
    for (auto& p : kv) s += p.first + "\x01" + p.second + "\x02";
    return s;
}

bool type_eq(const TypeP& a, const TypeP& b) {
    if (!a || !b) return !a && !b;
    if (a.get() == b.get()) return true;
    if (a->kind != b->kind || !py::eq(a->name, b->name) || a->args.size() != b->args.size()) return false;
    for (size_t i = 0; i < a->args.size(); i++)
        if (!type_eq(a->args[i], b->args[i])) return false;
    if (!type_eq(a->elem, b->elem)) return false;
    if (a->has_bound != b->has_bound) return false;
    if (a->has_bound && (!py::eq(a->bound, b->bound) || !type_eq(a->bound_type, b->bound_type))) return false;
    return info_key(a) == info_key(b);
}

TypeP mk_type(const std::string& kind, const std::string& name, std::vector<TypeP> args, TypeP elem) {
    auto t = std::make_shared<l2::Type>();
    t->kind = kind;
    if (!name.empty()) t->name = V(name);
    t->args = std::move(args);
    t->elem = elem;
    return t;
}

static const char* PRIMS[] = {"bool", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "f80", "char"};

bool is_prim(const TypeP& t) {
    if (!t) return false;
    for (auto p : PRIMS)
        if (t->kind == p) return true;
    return false;
}

bool is_ref_type(const TypeP& t) {
    if (!t) return false;
    const std::string& k = t->kind;
    return k == "class" || k == "array" || k == "typevar" || k == "null" || k == "wildcard";
}

std::string type_repr(const TypeP& t) {
    if (!t) return "None";
    if (t->kind == "array") return type_repr(t->elem) + "[]";
    if (t->kind == "class" || t->kind == "typevar") {
        std::string s = tname(t);
        if (!t->args.empty()) {
            Strs a;
            for (auto& x : t->args) a.push_back(type_repr(x));
            s += "<" + join(a, ", ") + ">";
        }
        return s;
    }
    if (t->kind == "wildcard") return "?" + (t->has_bound ? " " + py::str_of(t->bound) + " " + type_repr(t->bound_type) : "");
    return t->kind;
}

std::string sname(const SymP& s) { return s ? str(s->name) : std::string(); }
std::string sowner(const SymP& s) { return s ? str(s->owner) : std::string(); }
std::string skind(const SymP& s) { return s ? str(s->kind) : std::string(); }
bool sstatic(const SymP& s) { return s && py::truthy(s->stat); }

// ---------------------------------------------------------------------------------------------- the emitter
void Emitter::emit_block(const NodeP& s, Strs& out, const std::string& level) {
    out.push_back(level + "{");
    nested(s, out, level);
    out.push_back(level + "}");
}

void Emitter::emit_if(const NodeP& s, Strs& out, const std::string& level) {
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

void Emitter::emit_while(const NodeP& s, Strs& out, const std::string& level) {
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

bool lit_float(const NodeP& lit, double& out) {
    const V& v = raw(lit, "value");
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

}  // namespace cpp17

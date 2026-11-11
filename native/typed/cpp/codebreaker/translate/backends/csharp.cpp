// codebreaker/translate/backends/csharp.cpp -- typed C++ port of codebreaker.translate.backends.csharp
// (see csharp.hpp).
#include "codebreaker/translate/backends/csharp.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>

#include "codebreaker/translate/backends/csharp_java.hpp"

namespace codebreaker::translate::backends::csharp {

using ir::NodeOrList;
using ir::Nodes;
using py::str;
using ClassP = Writer::ClassP;
using MethodP = Writer::MethodP;

namespace {

str S(const char* s) { return str(s); }

template <class T> T* as(const NodeP& n) { return dynamic_cast<T*>(n.get()); }
template <class T> py::ref<T> ref_as(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }

[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

const py::list<NodeP>& nodes_of(const Nodes& v) {
    static const py::list<NodeP> none;
    return v ? *v : none;
}
py::list<str> strs_of(const ir::Strs& v) { return v ? *v : py::list<str>(); }
bool in_strs(const ir::Strs& v, const char* x) { return v && v->contains(S(x)); }
bool nonempty(const Nodes& v) { return v && !v->empty(); }
bool truthy(const std::optional<str>& a) { return a && !a->empty(); }
bool eq(const std::optional<str>& a, const char* b) { return a && *a == b; }
str or_empty(const std::optional<str>& a) { return a ? *a : str(); }

// `e.type` of an expression or a Local
TypeP type_of(const NodeP& e) {
    if (!e) none_attr("type");
    if (auto x = as<ir::Expr>(e)) return x->type;
    if (auto l = as<ir::Local>(e)) return l->type;
    throw py::AttributeError("'" + e->class_name().utf8() + "' object has no attribute 'type'");
}
const ir::Type& K(const TypeP& t) {
    if (!t) none_attr("kind");
    return *t;
}

str mods_text(const py::list<str>& mods) {
    str out;
    for (const str& m : mods) out += m + S(" ");
    return out;
}
str jn(const str& sep, const py::list<str>& parts) { return py::join(sep, parts); }
str lstr(const std::string& s) { return str(s); }

bool val_truthy(const ir::Val& v) {
    if (v.is_none()) return false;
    if (auto b = std::get_if<bool>(&v.v)) return *b;
    if (auto i = std::get_if<int64_t>(&v.v)) return *i != 0;
    if (auto d = std::get_if<double>(&v.v)) return *d != 0;
    if (auto s = std::get_if<str>(&v.v)) return !s->empty();
    if (auto l = std::get_if<ir::Val::List>(&v.v)) return !l->empty();
    if (auto d = std::get_if<ir::Val::Dict>(&v.v)) return !d->empty();
    return true;
}
bool info_truthy(const TypeP& t, const char* key) {
    ir::Val v = t->info.get(S(key), ir::Val());
    return val_truthy(v);
}
int64_t info_int(const TypeP& t, const char* key, int64_t dflt) {
    ir::Val v = t->info.get(S(key), ir::Val(dflt));
    if (auto i = std::get_if<int64_t>(&v.v)) return *i;
    if (auto b = std::get_if<bool>(&v.v)) return *b ? 1 : 0;
    throw py::TypeError("unsupported operand type(s) for the type's info");
}

// -------------------------------------------------------------------------------------- tables
const std::unordered_map<std::string, const char*>& PRIM() {
    static const std::unordered_map<std::string, const char*> m = {
        {"bool", "bool"}, {"i8", "sbyte"}, {"u8", "byte"}, {"i16", "short"}, {"i32", "int"}, {"u32", "uint"}, {"i64", "long"},
        {"u64", "ulong"}, {"f32", "float"}, {"f64", "double"}, {"void", "void"}, {"char", "byte"}};
    return m;
}
const std::unordered_map<std::string, const char*>& ALIAS() {
    static const std::unordered_map<std::string, const char*> m = {
        {"System.String", "string"}, {"System.Object", "object"}, {"System.Boolean", "bool"}, {"System.Int32", "int"},
        {"System.Int64", "long"}, {"System.Double", "double"}, {"System.Single", "float"}, {"System.Char", "char"},
        {"System.Byte", "byte"}, {"System.SByte", "sbyte"}, {"System.Int16", "short"}, {"System.UInt16", "ushort"},
        {"System.UInt32", "uint"}, {"System.UInt64", "ulong"}, {"System.Decimal", "decimal"}, {"System.Void", "void"}};
    return m;
}
const std::unordered_map<std::string, int64_t>& BIN_PREC() {
    static const std::unordered_map<std::string, int64_t> m = {
        {"*", P_MUL}, {"/", P_MUL}, {"%", P_MUL}, {"+", P_ADD}, {"-", P_ADD}, {"<<", P_SHIFT}, {">>", P_SHIFT},
        {">>>", P_SHIFT}, {"<", P_REL}, {">", P_REL}, {"<=", P_REL}, {">=", P_REL}, {"==", P_EQ}, {"!=", P_EQ},
        {"&", P_AND}, {"^", P_XOR}, {"|", P_OR}, {"&&", P_LAND}, {"||", P_LOR}, {"??", P_COALESCE}};
    return m;
}
const std::unordered_map<std::string, const char*>& OPERATOR_METHODS() {
    static const std::unordered_map<std::string, const char*> m = {
        {"op_Addition", "+"}, {"op_Subtraction", "-"}, {"op_Multiply", "*"}, {"op_Division", "/"}, {"op_Modulus", "%"},
        {"op_Equality", "=="}, {"op_Inequality", "!="}, {"op_LessThan", "<"}, {"op_GreaterThan", ">"},
        {"op_LessThanOrEqual", "<="}, {"op_GreaterThanOrEqual", ">="}, {"op_BitwiseAnd", "&"}, {"op_BitwiseOr", "|"},
        {"op_ExclusiveOr", "^"}, {"op_LeftShift", "<<"}, {"op_RightShift", ">>"}, {"op_UnsignedRightShift", ">>>"}};
    return m;
}
const std::unordered_map<std::string, const char*>& UNARY_METHODS() {
    static const std::unordered_map<std::string, const char*> m = {
        {"op_UnaryNegation", "-"}, {"op_UnaryPlus", "+"}, {"op_LogicalNot", "!"}, {"op_OnesComplement", "~"}};
    return m;
}
const std::unordered_map<std::string, const char*>& DECL_OPERATORS() {
    static const std::unordered_map<std::string, const char*> m = [] {
        std::unordered_map<std::string, const char*> r = OPERATOR_METHODS();
        for (auto& kv : UNARY_METHODS()) r[kv.first] = kv.second;
        r["op_Increment"] = "++";
        r["op_Decrement"] = "--";
        r["op_True"] = "true";
        r["op_False"] = "false";
        return r;
    }();
    return m;
}

str g_here;

}  // namespace

// -------------------------------------------------------------------------------------- module
void set_here(const str& here) { g_here = here; }

str RUNTIME() {
    // os.path.join(dirname(dirname(abspath(__file__))), 'dotnet', 'runtime')
    str here = g_here.empty() ? py::os::path::dirname(py::os::path::abspath(str(__FILE__))) : g_here;
    return py::os::path::join(py::os::path::join(py::os::path::dirname(here), S("dotnet")), S("runtime"));
}

const py::set<str> KEYWORDS = [] {
    py::set<str> s;
    for (const char* k : {"abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class",
                          "const", "continue", "decimal", "default", "delegate", "do", "double", "else", "enum", "event",
                          "explicit", "extern", "false", "finally", "fixed", "float", "for", "foreach", "goto", "if",
                          "implicit", "in", "int", "interface", "internal", "is", "lock", "long", "namespace", "new", "null",
                          "object", "operator", "out", "override", "params", "private", "protected", "public", "readonly",
                          "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct",
                          "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe",
                          "ushort", "using", "virtual", "void", "volatile", "while"})
        s.add(S(k));
    return s;
}();

str ident(const str& name) { return KEYWORDS.contains(name) ? S("@") + name : name; }

namespace {

str cstr(const str& s) {
    std::u32string out = U"\"";
    char buf[16];
    for (char32_t ch : s.u32()) {
        int64_t o = ch;
        if (ch == U'"') out += U"\\\"";
        else if (ch == U'\\') out += U"\\\\";
        else if (ch == U'\n') out += U"\\n";
        else if (ch == U'\r') out += U"\\r";
        else if (ch == U'\t') out += U"\\t";
        else if (ch == U'\0') out += U"\\0";
        else if (o < 0x20 || o > 0x7e) {
            if (o > 0xffff) {
                o -= 0x10000;
                std::snprintf(buf, sizeof buf, "\\u%04x\\u%04x", static_cast<unsigned>(0xd800 + (o >> 10)), static_cast<unsigned>(0xdc00 + (o & 0x3ff)));
            } else {
                std::snprintf(buf, sizeof buf, "\\u%04x", static_cast<unsigned>(o));
            }
            for (const char* p = buf; *p; ++p) out.push_back(static_cast<char32_t>(*p));
        } else out.push_back(ch);
    }
    out.push_back(U'"');
    return str(out);
}

str cchar(const str& ch) {
    if (ch == S("'")) return S("'\\''");
    if (ch == S("\"")) return S("'\"'");
    str s = cstr(ch);
    return S("'") + s.slice(1, s.size() - 1) + S("'");
}

double num_of(const ir::Val& v) {
    if (auto i = std::get_if<int64_t>(&v.v)) return static_cast<double>(*i);
    if (auto b = std::get_if<bool>(&v.v)) return *b ? 1.0 : 0.0;
    if (auto d = std::get_if<double>(&v.v)) return *d;
    if (auto s = std::get_if<str>(&v.v)) {
        try {
            return std::stod(s->utf8());
        } catch (...) {
            throw py::ValueError("could not convert string to float");
        }
    }
    throw py::TypeError("float() argument must be a string or a real number");
}

float pack_f(double x) {   // struct.pack('f', x): OverflowError for a finite value that is too large
    float y = static_cast<float>(x);
    if (std::isinf(y) && !std::isinf(x)) throw py::OverflowError("float too large to pack with f format");
    return y;
}

str float_text(double v, const TypeP& t) {
    bool single = t && t->kind == "f32";
    if (std::isnan(v)) return S(single ? "float.NaN" : "double.NaN");
    if (std::isinf(v)) return S(single ? "float" : "double") + S(v > 0 ? ".PositiveInfinity" : ".NegativeInfinity");
    if (single) {
        float f = pack_f(v);
        std::string r = py::repr(static_cast<double>(f)).utf8();
        // the shortest text that is still this float
        for (int digits = 1; digits < 18; ++digits) {
            char buf[64];
            std::snprintf(buf, sizeof buf, "%.*g", digits, v);
            float c = pack_f(std::strtod(buf, nullptr));
            if (c == f) {
                r = buf;
                break;
            }
        }
        if (r.find('e') == std::string::npos && r.find('.') == std::string::npos && r.find("inf") == std::string::npos) r += ".0";
        for (char& ch : r) if (ch == 'e') ch = 'E';
        return lstr(r) + S("f");
    }
    std::string r = py::repr(v).utf8();
    if (r.find('e') == std::string::npos && r.find('.') == std::string::npos) r += ".0";
    for (char& ch : r) if (ch == 'e') ch = 'E';
    bool plain = r.find('.') != std::string::npos || r.find('e') != std::string::npos || r.find('E') != std::string::npos;
    return lstr(r) + S(plain ? "" : "D");
}

// library constants a constant expression may use (Java's Integer.MAX_VALUE is a field, not a literal)
std::optional<int64_t> lib_const_value(const NodeP& e) {
    auto s = as<ir::Select>(e);
    if (!s || !s->sym || !s->sym->owner) return std::nullopt;
    const std::string o = s->sym->owner->utf8(), n = s->sym->name.utf8();
    static const std::map<std::pair<std::string, std::string>, int64_t> m = {
        {{"java.lang.Integer", "MAX_VALUE"}, 2147483647LL}, {{"java.lang.Integer", "MIN_VALUE"}, -2147483648LL},
        {{"java.lang.Long", "MAX_VALUE"}, INT64_MAX}, {{"java.lang.Long", "MIN_VALUE"}, INT64_MIN},
        {{"java.lang.Short", "MAX_VALUE"}, 32767}, {{"java.lang.Short", "MIN_VALUE"}, -32768},
        {{"java.lang.Byte", "MAX_VALUE"}, 127}, {{"java.lang.Byte", "MIN_VALUE"}, -128},
        {{"java.lang.Character", "MAX_VALUE"}, 65535}, {{"java.lang.Character", "MIN_VALUE"}, 0}};
    auto it = m.find({o, n});
    if (it == m.end()) return std::nullopt;
    return it->second;
}
bool lib_const(const NodeP& e) { return lib_const_value(e).has_value(); }

// A compile-time constant expression (where Java folds with wrapping and C# refuses to overflow).
bool is_const(const NodeP& e0) {
    NodeP e = strip(e0);
    if (lib_const(e)) return true;
    if (auto l = as<ir::Lit>(e)) {
        const ir::Val& v = l->value;
        bool is_str = std::holds_alternative<str>(v.v);
        return (!v.is_none() && !is_str) || (is_str && std::get<str>(v.v).size() == 1);
    }
    if (auto u = as<ir::Unary>(e)) {
        if (u->op && (*u->op == "-" || *u->op == "+" || *u->op == "~")) return is_const(u->x);
        return false;
    }
    if (auto b = as<ir::Binary>(e)) {
        if (b->op && BIN_PREC().count(b->op->utf8()) && *b->op != "&&" && *b->op != "||" && *b->op != "??")
            return is_const(b->left) && is_const(b->right);
        return false;
    }
    if (auto c = as<ir::Cast>(e)) return c->type && c->type->is_prim() && is_const(c->x);
    return false;
}

struct Range {
    int64_t lo, hi;
};
std::optional<Range> range_of(const std::string& k) {
    if (k == "i8") return Range{-128, 127};
    if (k == "u8") return Range{0, 255};
    if (k == "i16") return Range{-32768, 32767};
    if (k == "u16") return Range{0, 65535};
    if (k == "i32") return Range{-2147483648LL, 2147483647LL};
    if (k == "u32") return Range{0, 4294967295LL};
    if (k == "i64") return Range{INT64_MIN, INT64_MAX};
    if (k == "u64") return Range{0, INT64_MAX};   // (2**64-1 is not an int64_t; see csharp.hpp)
    return std::nullopt;
}

// Does folding this constant integer expression leave its type's range anywhere along the way?
struct Folder {
    bool bad = false;

    // a + b etc. in 64 bits: the value, and whether the true result left int64 (then the value is the wrapped one)
    static int64_t wrap_add(int64_t a, int64_t b) { return static_cast<int64_t>(static_cast<uint64_t>(a) + static_cast<uint64_t>(b)); }
    static int64_t wrap_sub(int64_t a, int64_t b) { return static_cast<int64_t>(static_cast<uint64_t>(a) - static_cast<uint64_t>(b)); }
    static int64_t wrap_mul(int64_t a, int64_t b) { return static_cast<int64_t>(static_cast<uint64_t>(a) * static_cast<uint64_t>(b)); }

    // r is the exact result when `big` is false; when true it left int64 and r is its wrap
    std::optional<int64_t> val(const NodeP& x0) {
        NodeP x = strip(x0);
        TypeP xt = type_of(x);
        std::string t = xt ? xt->kind.utf8() : "i32";
        if (auto l = as<ir::Lit>(x)) {
            const ir::Val& v = l->value;
            if (auto s = std::get_if<str>(&v.v)) return py::ord(*s);
            if (auto i = std::get_if<int64_t>(&v.v)) return *i;
            if (auto b = std::get_if<bool>(&v.v)) return *b ? 1 : 0;
            return std::nullopt;
        }
        if (auto lc = lib_const_value(x)) return lc;
        std::optional<int64_t> r;
        bool big = false;
        if (auto u = as<ir::Unary>(x)) {
            auto a = val(u->x);
            if (!a) return std::nullopt;
            std::string op = u->op ? u->op->utf8() : "";
            if (op == "-") {
                if (*a == INT64_MIN) {
                    big = true;
                    r = INT64_MIN;
                } else r = -*a;
            } else if (op == "+") r = *a;
            else if (op == "~") r = ~*a;
            else throw py::KeyError(py::repr(or_empty(u->op)).utf8());
        } else if (auto b = as<ir::Binary>(x)) {
            auto a = val(b->left);
            auto c = val(b->right);
            if (!a || !c) return std::nullopt;
            std::string op = b->op ? b->op->utf8() : "";
            int bits = (t == "i64" || t == "u64") ? 64 : 32;
            int64_t av = *a, cv = *c;
            if (op == "<<" || op == ">>" || op == ">>>") cv &= bits - 1;
            if (op == "+") {
                r = wrap_add(av, cv);
                big = (cv > 0 && av > INT64_MAX - cv) || (cv < 0 && av < INT64_MIN - cv);
            } else if (op == "-") {
                r = wrap_sub(av, cv);
                big = (cv < 0 && av > INT64_MAX + cv) || (cv > 0 && av < INT64_MIN + cv);
            } else if (op == "*") {
                r = wrap_mul(av, cv);
                if (av != 0 && cv != 0) {
                    big = (av == INT64_MIN && cv == -1) || (cv == INT64_MIN && av == -1) ||
                          (av != INT64_MIN && cv != INT64_MIN && std::llabs(av) > INT64_MAX / std::llabs(cv));
                }
            } else if (op == "/") {
                if (cv) {
                    // int(a / b): a true division, then truncation
                    r = static_cast<int64_t>(static_cast<double>(av) / static_cast<double>(cv));
                }
            } else if (op == "%") {
                if (cv) {
                    uint64_t ua = av < 0 ? 0 - static_cast<uint64_t>(av) : static_cast<uint64_t>(av);
                    uint64_t uc = cv < 0 ? 0 - static_cast<uint64_t>(cv) : static_cast<uint64_t>(cv);
                    int64_t m = static_cast<int64_t>(ua % uc);
                    r = av >= 0 ? m : -m;
                }
            } else if (op == "<<") {
                r = static_cast<int64_t>(static_cast<uint64_t>(av) << cv);
                if (cv >= 63) big = av != 0;
                else big = ((static_cast<int64_t>(static_cast<uint64_t>(av) << cv)) >> cv) != av;
            } else if (op == ">>") {
                r = av >> cv;
            } else if (op == ">>>") {
                if (bits == 64) {
                    uint64_t u = static_cast<uint64_t>(av) >> cv;
                    r = static_cast<int64_t>(u);
                    big = u > static_cast<uint64_t>(INT64_MAX);
                } else {
                    int64_t m = ((av % 4294967296LL) + 4294967296LL) % 4294967296LL;
                    r = m >> cv;
                }
            } else if (op == "&") r = av & cv;
            else if (op == "|") r = av | cv;
            else if (op == "^") r = av ^ cv;
            if (op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=") return std::nullopt;
        } else if (auto cst = as<ir::Cast>(x)) {
            r = val(cst->x);
        } else {
            return std::nullopt;
        }
        if (!r) return std::nullopt;
        if (auto rg = range_of(t)) {
            if (big || !(rg->lo <= *r && *r <= rg->hi)) {
                bad = true;
                // r = (r - lo) % span + lo, in the type's width
                int bitsz = (t == "i8" || t == "u8") ? 8 : (t == "i16" || t == "u16") ? 16 : (t == "i32" || t == "u32") ? 32 : 64;
                if (bitsz == 64) {
                    // the wrap of a 64-bit value is the value itself (big: already wrapped)
                    return *r;
                }
                int64_t span = 1LL << bitsz;
                int64_t v = *r - rg->lo;
                v = ((v % span) + span) % span;
                return v + rg->lo;
            }
        }
        return *r;
    }
};

bool overflows(const NodeP& e) {
    Folder f;
    f.val(e);
    return f.bad;
}

// Does this statement list never fall off its end?
bool loop_has_break(const NodeP& body);
bool abrupt_node(const NodeP& s) {
    if (!s) return false;
    if (is_a<ir::Return>(s) || is_a<ir::Throw>(s) || is_a<ir::Break>(s) || is_a<ir::Continue>(s) || is_a<ir::Goto>(s)) return true;
    if (auto b = as<ir::Block>(s)) {
        const py::list<NodeP>& l = nodes_of(b->stmts);
        return !l.empty() && abrupt_node(l[-1]);
    }
    if (auto i = as<ir::If>(s)) {
        bool has_else = std::holds_alternative<py::list<NodeP>>(i->orelse) || std::get<NodeP>(i->orelse) != nullptr;
        if (!has_else) return false;
        // _abrupt([s.then]): a bare list in place of a node is not a statement
        NodeP th = std::holds_alternative<NodeP>(i->then) ? std::get<NodeP>(i->then) : NodeP();
        NodeP el = std::holds_alternative<NodeP>(i->orelse) ? std::get<NodeP>(i->orelse) : NodeP();
        return abrupt_node(th) && abrupt_node(el);
    }
    if (auto w = as<ir::While>(s)) {
        NodeP c = strip(w->cond);
        auto l = as<ir::Lit>(c);
        if (l && std::holds_alternative<bool>(l->value.v) && std::get<bool>(l->value.v)) {
            NodeP body = std::holds_alternative<NodeP>(w->body) ? std::get<NodeP>(w->body) : NodeP();
            if (!body) throw py::AttributeError("'list' object has no attribute 'children'");
            return !loop_has_break(body);
        }
        return false;
    }
    return false;
}
bool abrupt(const py::list<NodeP>& stmts) {
    if (stmts.empty()) return false;
    return abrupt_node(stmts[-1]);
}

// The unlabelled breaks that leave this loop (not those of nested loops or switches).
void loop_breaks_walk(const NodeP& n, int depth, bool& found) {
    if (auto b = as<ir::Break>(n)) {
        if (depth == 0 && !b->label) found = true;
    }
    for (const NodeP& c : n->children()) {
        bool nested = is_a<ir::While>(c) || is_a<ir::DoWhile>(c) || is_a<ir::For>(c) || is_a<ir::ForEach>(c) || is_a<ir::Switch>(c);
        if (is_a<ir::Lambda>(c) || is_a<ir::ClassDecl>(c)) continue;
        loop_breaks_walk(c, depth + (nested ? 1 : 0), found);
    }
}
bool loop_has_break(const NodeP& body) {
    bool found = false;
    loop_breaks_walk(body, 0, found);
    return found;
}

// -------------------------------------------------------------------------------------- _Init
// (the back end's own) an object or collection initialiser: what the front end hoisted, put back
struct InitItem {
    bool is_set;                 // 'set' (a member = value) or 'add' (a collection item)
    SymP sym;
    NodeP value;                 // the 'set' value
    py::list<NodeP> args;        // the 'add' arguments
};
struct Init final : ir::Expr {
    NodeP new_;
    std::vector<InitItem> items;
    py::str class_name() const override { return py::str("_Init"); }
    py::list<py::str> slots() const override {
        py::list<py::str> out;
        out.append(py::str("type"));
        out.append(py::str("new"));
        out.append(py::str("items"));
        return out;
    }
    py::list<py::str> fields() const override { return py::list<py::str>(); }
    py::str repr() const override { return py::str("_Init(...)"); }
    py::list<NodeP> children() const override { return py::list<NodeP>(); }
};

bool is_name(const NodeP& e, const SymP& s) {
    auto n = as<ir::Name>(e);
    return n && n->sym && n->sym->name == s->name && n->sym->kind == s->kind;
}
bool uses(const NodeP& e, const SymP& s) {
    for (const NodeP& n : ir::walk(e))
        if (is_name(n, s)) return true;
    return false;
}

// the slots a node of any class holds: tries each node class of the IR in turn
struct Replacer {
    NodeP old_, new_;
    bool done = false;

    void slot(NodeP& v) {
        if (!done && v == old_) {
            v = new_;
            done = true;
        }
    }
    void slot(Nodes& v) {
        if (done || !v) return;
        for (int64_t k = 0; k < v->size(); ++k) {
            if ((*v)[k] == old_) {
                (*v)[k] = new_;
                done = true;
                return;
            }
        }
    }
    void slot(NodeOrList& v) {
        if (done) return;
        if (std::holds_alternative<NodeP>(v)) slot(std::get<NodeP>(v));
        else {
            auto& l = std::get<py::list<NodeP>>(v);
            for (int64_t k = 0; k < l.size(); ++k) {
                if (l[k] == old_) {
                    l[k] = new_;
                    done = true;
                    return;
                }
            }
        }
    }
    template <class T> void slot(T&) {}

    template <class N> bool try_(const NodeP& n) {
        auto p = dynamic_cast<N*>(n.get());
        if (!p) return false;
        p->each([&](const char*, auto& member, int) { slot(member); });
        return true;
    }
    template <class... N> void any(const NodeP& n) { (void)(try_<N>(n) || ...); }
};

bool replace_node(const NodeP& root, const NodeP& old_, const NodeP& new_) {
    Replacer r;
    r.old_ = old_;
    r.new_ = new_;
    for (const NodeP& n : ir::walk(root)) {
        r.any<ir::Module, ir::Unit, ir::ClassDecl, ir::FieldDecl, ir::MethodDecl, ir::Param, ir::EnumConst, ir::Property,
              ir::TypeParam, ir::Block, ir::Local, ir::ExprStmt, ir::If, ir::While, ir::DoWhile, ir::For, ir::ForEach,
              ir::Switch, ir::Case, ir::Break, ir::Continue, ir::Return, ir::Throw, ir::Try, ir::Catch, ir::Labeled,
              ir::Goto, ir::MachineFragment, ir::Sync, ir::Empty, ir::Comment, ir::LocalClass, ir::Assert, ir::CtorCall,
              ir::Yield, ir::Lit, ir::Name, ir::Select, ir::TypeName, ir::This, ir::Super, ir::Call, ir::New,
              ir::NewArray, ir::Index, ir::Length, ir::Unary, ir::Binary, ir::Concat, ir::Assign, ir::Cond, ir::Cast,
              ir::InstanceOf, ir::ClassLit, ir::Lambda, ir::MethodRef, ir::SwitchExpr, ir::AddressOf, ir::FarAddress,
              ir::Piece, ir::Join, ir::Paren, ir::Unsupported>(n);
        if (r.done) return true;
    }
    return false;
}

// var t = new T(); t.A = 1; t.Add(x); ... S(t)  ->  S(new T { A = 1 }) / S(new T { x }): the statements a
// C# object or collection initialiser was hoisted into, put back as the initialiser.
py::list<NodeP> initializers(const py::list<NodeP>& in) {
    std::vector<NodeP> stmts;
    for (const NodeP& n : in) stmts.push_back(n);
    bool changed = true;
    while (changed) {
        changed = false;
        for (size_t i = 0; i < stmts.size(); ++i) {
            auto st = as<ir::Local>(stmts[i]);
            if (!st || !st->sym || !st->sym->name.startswith(S("__t"))) continue;
            auto init_new = as<ir::New>(st->init);
            if (!init_new || init_new->body) continue;
            std::vector<InitItem> items;
            size_t j = i + 1;
            while (j < stmts.size() && is_a<ir::ExprStmt>(stmts[j])) {
                NodeP e = as<ir::ExprStmt>(stmts[j])->expr;
                auto asg = as<ir::Assign>(e);
                auto call = as<ir::Call>(e);
                auto tsel = asg ? as<ir::Select>(asg->target) : nullptr;
                if (asg && !asg->op && tsel && is_name(tsel->obj, st->sym) && tsel->sym &&
                    (tsel->sym->kind == "field" || tsel->sym->kind == "property") && !uses(asg->value, st->sym)) {
                    items.push_back(InitItem{true, tsel->sym, asg->value, py::list<NodeP>()});
                } else if (call && is_name(call->target, st->sym) && call->sym && call->sym->name == "Add") {
                    bool used = false;
                    for (const NodeP& a : nodes_of(call->args))
                        if (uses(a, st->sym)) used = true;
                    if (used) break;
                    items.push_back(InitItem{false, call->sym, NodeP(), nodes_of(call->args).copy()});
                } else {
                    break;
                }
                ++j;
            }
            if (j >= stmts.size() || items.empty()) continue;
            bool mixed = false;
            for (const InitItem& it : items)
                if (it.is_set != items[0].is_set) mixed = true;
            if (mixed) continue;
            std::vector<NodeP> use;
            for (const NodeP& n : ir::walk(stmts[j]))
                if (is_name(n, st->sym)) use.push_back(n);
            if (use.size() != 1) continue;
            auto init = std::make_shared<Init>();
            init->type = type_of(st->init);
            init->line = st->line;
            init->col = st->col;
            init->new_ = st->init;
            init->items = items;
            replace_node(stmts[j], use[0], init);
            stmts.erase(stmts.begin() + static_cast<std::ptrdiff_t>(i), stmts.begin() + static_cast<std::ptrdiff_t>(j));
            changed = true;
            break;
        }
    }
    py::list<NodeP> out;
    for (const NodeP& s : stmts) out.append(s);
    return out;
}

bool interpolatable(const py::list<NodeP>& parts) {
    bool any_str = false;
    for (const NodeP& p : parts) {
        auto l = as<ir::Lit>(p);
        if (l && std::holds_alternative<str>(l->value.v)) any_str = true;
    }
    if (!any_str) return false;
    for (const NodeP& p : parts) {
        auto l = as<ir::Lit>(p);
        if (l && std::holds_alternative<str>(l->value.v) && std::get<str>(l->value.v).contains(S("\n"))) return false;
    }
    return true;
}

// Does C# infer this generic call's type arguments from its arguments? (Every type argument appears
// in a parameter's type, and no argument is a lambda or null.)
bool inferable(const py::ref<ir::Call>& e) {
    const SymP& s = e->sym;
    if (s->params.empty()) return false;
    for (const NodeP& a0 : nodes_of(e->args)) {
        NodeP a = strip(a0);
        if (is_a<ir::Lambda>(a) || is_a<ir::MethodRef>(a)) return false;
        auto l = as<ir::Lit>(a);
        if (l && l->value.is_none()) return false;
    }
    return true;
}

std::vector<str> sig_of(const py::ref<ir::MethodDecl>& m) {
    std::vector<str> out;
    for (const NodeP& p : nodes_of(m->params)) {
        auto pp = as<ir::Param>(p);
        out.push_back(ir::repr_of(pp->type));
    }
    return out;
}

// the qualified name's namespace: qname[:-len(name)].rstrip('.') when qname ends with name
str drop_suffix(const str& qname, const str& name) {
    if (name.empty()) return str();
    return qname.slice(0, qname.size() - name.size()).rstrip(S("."));
}

std::pair<str, str> rsplit_dot(const str& s) {
    int64_t at = s.rfind(S("."));
    return {s.slice(0, at), s.slice(at + 1, s.size())};
}

}  // namespace

NodeP strip(const NodeP& e0) {
    NodeP e = e0;
    while (auto p = as<ir::Paren>(e)) e = p->x;
    return e;
}

bool is_string(const TypeP& t) {
    return t && t->kind == "class" && t->name && (*t->name == "System.String" || *t->name == "java.lang.String");
}

// -------------------------------------------------------------------------------------- Program
Program::Program(const py::list<NodeP>& units_) : units(units_.copy()) {
    for (const NodeP& n : units) {
        auto u = as<ir::Unit>(n);
        for (const NodeP& c : nodes_of(u->types)) index_(c);
    }
}

void Program::index_(const NodeP& c) {
    auto cd = ref_as<ir::ClassDecl>(c);
    if (!cd) return;
    classes.set(*cd->qname, cd);
    for (const NodeP& m : nodes_of(cd->members))
        if (is_a<ir::ClassDecl>(m)) index_(m);
}

py::dict<str, str> Program::files() {
    if (!files_) {
        bool any_java = false;
        for (const NodeP& n : units) {
            auto u = as<ir::Unit>(n);
            if (!truthy(u->lang) || *u->lang == "java") any_java = true;
        }
        if (any_java) csharp_java::prepare(*this);
        py::dict<str, str> out;
        for (const NodeP& n : units) {
            auto u = ref_as<ir::Unit>(n);
            Writer w(*this, u);
            str base = py::os::path::basename(u->path && !u->path->empty() ? *u->path : S("Unit"));
            str name = std::get<0>(py::os::path::splitext(base));
            if (name.empty()) name = S("Unit");
            out.set(name + S(".cs"), w.unit());
        }
        std::vector<str> rts(runtime.begin(), runtime.end());
        std::sort(rts.begin(), rts.end());
        for (const str& r : rts) {
            py::file f = py::open(py::os::path::join(RUNTIME(), r + S(".cs")), S("r"), S("utf-8"));
            str text = f.read_text();
            f.close();
            out.set(r + S(".cs"), text);
        }
        files_ = out;
    }
    return *files_;
}

py::list<str> Program::write(const str& out_dir) {
    py::os::makedirs(out_dir, 0777, true);
    py::list<str> paths;
    auto fs = files();
    for (const str& n : fs) {
        str text = fs.at(n);
        str p = py::os::path::join(out_dir, n);
        py::file f = py::open(p, S("w"), S("utf-8"), std::nullopt, S("\n"));
        f.write(text);
        f.close();
        paths.append(p);
    }
    return paths;
}

ClassP Program::base_of(const ClassP& c) const {
    if (!c->extends || c->extends->kind != "class") return nullptr;
    if (!c->extends->name) return nullptr;
    return classes.get(*c->extends->name);
}

bool Program::overrides(const ClassP& c, const MethodP& m) const {
    ClassP b = base_of(c);
    std::vector<str> sig = sig_of(m);
    std::set<std::string> seen;
    while (b && !seen.count(b->qname->utf8())) {
        seen.insert(b->qname->utf8());
        for (const NodeP& xn : nodes_of(b->members)) {
            auto x = as<ir::MethodDecl>(xn);
            if (x && eq(x->kind, "method") && x->name == m->name && sig_of(std::dynamic_pointer_cast<ir::MethodDecl>(xn)) == sig &&
                !in_strs(x->mods, "static") && !in_strs(x->mods, "private"))
                return true;
        }
        b = base_of(b);
    }
    return false;
}

py::ref<Program> write(const py::list<NodeP>& units) { return std::make_shared<Program>(units); }

// -------------------------------------------------------------------------------------- Writer
Writer::Writer(Program& prog_, const py::ref<ir::Unit>& unit_) : prog(prog_), u(unit_) {
    lang = truthy(unit_->lang) ? *unit_->lang : S("java");
    if (lang == "java") java = std::make_shared<JavaLib>(*this);
    block_names.clear();
    scopes.push_back(py::dict<str, str>());
}

str Writer::declare(const str& name, bool nested) {
    locals.add(name);
    std::vector<const py::set<str>*> enclosing;
    size_t n = block_names.size();
    for (size_t i = 0; i < (nested ? n : (n ? n - 1 : 0)); ++i) enclosing.push_back(&block_names[i]);
    str nw = name;
    bool clash = false;
    for (auto b : enclosing)
        if (b->contains(name)) clash = true;
    if (clash) {
        py::set<str> used;
        for (auto& b : block_names)
            for (const str& x : b) used.add(x);
        int64_t k = 2;
        for (;; ++k) {
            str cand = name + py::to_str(k);
            if (used.contains(cand)) continue;
            bool in_scope = false;
            for (auto& sc : scopes)
                for (const str& key : sc)
                    if (sc.at(key) == cand) in_scope = true;
            if (!in_scope) break;
        }
        nw = name + py::to_str(k);
    }
    scopes.back().set(name, nw);
    return ident(nw);
}

str Writer::lookup(const str& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        if (it->contains(name)) return ident(it->at(name));
    return ident(name);
}

void Writer::push() { scopes.push_back(py::dict<str, str>()); }
void Writer::pop() { scopes.pop_back(); }

void Writer::note(const str& kind, const str& why) {
    str where = cls ? or_empty(cls->qname) : or_empty(u->path);
    if (method && truthy(method->name)) where = where + S(".") + *method->name;
    prog.ledger.append(std::make_tuple(where, kind, why));
}

// ---- types
str Writer::ctype(const TypeP& t) {
    if (!t) return S("void");
    const std::string k = t->kind.utf8();
    if (k == "u16") return S(info_truthy(t, "enc") || lang == "java" ? "char" : "ushort");
    auto pit = PRIM().find(k);
    if (pit != PRIM().end()) return S(pit->second);
    if (k == "array") {
        int64_t r = info_int(t, "rank", 1);
        TypeP inner = t->elem;
        str suffix = S("[");
        for (int64_t i = 0; i < r - 1; ++i) suffix += S(",");
        suffix += S("]");
        while (inner->kind == "array") {
            suffix += S("[");
            int64_t ir_ = info_int(inner, "rank", 1);
            for (int64_t i = 0; i < ir_ - 1; ++i) suffix += S(",");
            suffix += S("]");
            inner = inner->elem;
        }
        return ctype(inner) + suffix;
    }
    if (k == "typevar") return ident(or_empty(t->name));
    if (k == "wildcard") {
        if (t->bound && std::get<0>(*t->bound) == "extends") return ctype(std::get<1>(*t->bound));
        return S("object");
    }
    if (k == "null") return S("object");
    if (k == "dynamic") return S("dynamic");
    if (k == "class") {
        if (java) {
            auto got = java->type_name(t);
            if (got) return *got;
        }
        return class_name(t);
    }
    throw Decline(S("type"), S("the type ") + t->repr());
}

str Writer::class_name(const TypeP& t) {
    str name = or_empty(t->name);
    auto al = ALIAS().find(name.utf8());
    if (al != ALIAS().end() && t->args.empty()) return S(al->second) + S(info_truthy(t, "nullable") ? "?" : "");
    if (name == "System.Nullable" && t->args.size() == 1) return ctype(t->args[0]) + S("?");
    str simple = short_(name);
    if (!t->args.empty()) {
        py::list<str> parts;
        for (const TypeP& a : t->args) parts.append(ctype(a));
        simple += S("<") + jn(S(", "), parts) + S(">");
    }
    if (info_truthy(t, "nullable")) simple += S("?");
    return simple;
}

str Writer::short_(const str& qname) {
    ClassP c = prog.classes.get(qname);
    if (c) {
        std::vector<ClassP> chain{c};
        for (;;) {
            const ClassP& last = chain.back();
            if (!truthy(last->outer)) break;
            ClassP o = prog.classes.get(*last->outer);
            if (!o) break;
            chain.push_back(prog.classes.at(*last->outer));
        }
        ClassP top = chain.back();
        str ns = (top->qname && top->name && top->qname->endswith(*top->name)) ? drop_suffix(*top->qname, *top->name) : str();
        py::list<str> relp;
        for (auto it = chain.rbegin(); it != chain.rend(); ++it) relp.append(ident(or_empty((*it)->name)));
        str rel = jn(S("."), relp);
        if (!ns.empty() && ns != namespace_of(cls)) usings.add(ns);
        // inside the class itself or its outer classes, the nested name alone
        ClassP cur = cls;
        while (cur) {
            if (truthy(c->outer) && cur->qname && *cur->qname == *c->outer) return ident(or_empty(c->name));
            cur = cur->outer ? prog.classes.get(*cur->outer) : nullptr;
        }
        return rel;
    }
    if (qname.contains(S("."))) {
        auto parts = rsplit_dot(qname);
        if (prog.classes.contains(parts.first)) return short_(parts.first) + S(".") + ident(parts.second);
        usings.add(parts.first);
        return ident(parts.second);
    }
    return ident(qname);
}

str Writer::namespace_of(const ClassP& c0) {
    if (!c0) return truthy(u->package) ? *u->package : str();
    ClassP c = c0;
    while (truthy(c->outer) && prog.classes.contains(*c->outer)) c = prog.classes.at(*c->outer);
    if (truthy(c->qname) && c->name && c->qname->endswith(*c->name)) return drop_suffix(*c->qname, *c->name);
    return str();
}

// ---- declarations
str Writer::unit() {
    py::list<str> body;
    std::vector<std::pair<str, py::list<NodeP>>> groups;
    for (const NodeP& c : nodes_of(u->types)) {
        auto cd = ref_as<ir::ClassDecl>(c);
        str ns = cd ? namespace_of(cd) : str();
        if (!groups.empty() && groups.back().first == ns) groups.back().second.append(c);
        else {
            py::list<NodeP> l;
            l.append(c);
            groups.emplace_back(ns, l);
        }
    }
    for (auto& g : groups) {
        py::list<str> lines;
        for (const NodeP& c : g.second) {
            try {
                lines.extend(class_decl(ref_as<ir::ClassDecl>(c), str()));
            } catch (const Decline& d) {
                note(d.kind, d.why);
                auto cd = ref_as<ir::ClassDecl>(c);
                lines.append(S("// CB-GAP: ") + (cd ? or_empty(cd->name) : S("?")) + S(" was not translated: ") + d.why);
            }
            lines.append(str());
        }
        while (!lines.empty() && lines[-1].empty()) lines.pop();
        if (!g.first.empty()) {
            body.append(S("namespace ") + g.first);
            body.append(S("{"));
            for (const str& l : lines) body.append(l.empty() ? str() : S("    ") + l);
            body.append(S("}"));
            body.append(str());
        } else {
            body.extend(lines);
            body.append(str());
        }
    }
    py::set<str> own;
    for (const NodeP& c : nodes_of(u->types))
        if (auto cd = ref_as<ir::ClassDecl>(c)) own.add(namespace_of(cd));
    std::vector<str> us;
    for (const str& x : usings) {
        if (x.empty() || own.contains(x)) continue;
        bool sub_of = false;
        for (const NodeP& c : nodes_of(u->types))
            if (auto cd = ref_as<ir::ClassDecl>(c))
                if (namespace_of(cd).startswith(x + S("."))) sub_of = true;
        if (!sub_of) us.push_back(x);
    }
    std::sort(us.begin(), us.end());
    py::list<str> head;
    head.append(S("// Translated by CodeBreaker from ") +
                py::os::path::basename(u->path && !u->path->empty() ? *u->path : S("?")) + S(" (") + lang + S(")."));
    if (lang == "java") head.append(S("#nullable disable"));
    for (const str& x : us) head.append(S("using ") + x + S(";"));
    py::list<str> all = head;
    all = all.copy();
    all.append(str());
    all.extend(body);
    return jn(S("\n"), all).rstrip() + S("\n");
}

py::list<str> Writer::class_decl(const ClassP& c, const str& ind) {
    ClassP prev = cls;
    cls = c;
    try {
        auto r = class_decl_(c, ind);
        cls = prev;
        return r;
    } catch (...) {
        cls = prev;
        throw;
    }
}

py::list<str> Writer::class_mods(const ClassP& c) {
    if (lang == "java") return java->class_mods(c);
    py::list<str> out;
    for (const str& m : strs_of(c->mods))
        if (m != "IMPLICIT") out.append(m);
    return out;
}

bool Writer::lib_interface(const TypeP& t) { return lang != "java" ? true : java->keep_interface(t); }

str Writer::tparams_text(const py::list<str>& names, const Nodes& tparams) {
    if (names.empty()) return str();
    std::map<std::string, py::ref<ir::TypeParam>> tps;
    for (const NodeP& tp : nodes_of(tparams)) {
        auto t = ref_as<ir::TypeParam>(tp);
        tps[or_empty(t->name).utf8()] = t;
    }
    py::list<str> out;
    for (const str& n : names) {
        auto it = tps.find(n.utf8());
        str v;
        if (it != tps.end() && truthy(it->second->variance)) {
            if (*it->second->variance == "Out") v = S("out ");
            else if (*it->second->variance == "In") v = S("in ");
        }
        out.append(v + ident(n));
    }
    return S("<") + jn(S(", "), out) + S(">");
}

str Writer::where_text(const NodeP& tpn) {
    auto tp = ref_as<ir::TypeParam>(tpn);
    py::list<str> parts;
    if (in_strs(tp->flags, "class")) parts.append(S("class"));
    if (in_strs(tp->flags, "struct")) parts.append(S("struct"));
    if (tp->bounds) {
        for (const TypeP& b : *tp->bounds) {
            if (b->kind == "class" && b->name && (*b->name == "java.lang.Object" || *b->name == "System.Object")) continue;
            parts.append(ctype(b));
        }
    }
    if (in_strs(tp->flags, "new")) parts.append(S("new()"));
    if (parts.empty()) return str();
    return S("where ") + ident(or_empty(tp->name)) + S(" : ") + jn(S(", "), parts);
}

py::list<str> Writer::class_decl_(const ClassP& c, const str& ind) {
    const std::string ckind = or_empty(c->kind).utf8();
    if (ckind == "delegate") {
        py::ref<ir::MethodDecl> inv;
        for (const NodeP& m : nodes_of(c->members))
            if (auto md = ref_as<ir::MethodDecl>(m)) {
                inv = md;
                break;
            }
        if (!inv) throw py::StopIteration();
        py::list<str> mods = class_mods(c);
        str a = ctype(inv->ret);
        str tp = tparams_text(strs_of(c->typeparams), c->tparams);
        str ps = params_text(inv->params);
        py::list<str> r;
        r.append(ind + mods_text(mods) + S("delegate ") + a + S(" ") + ident(or_empty(c->name)) + tp + S("(") + ps + S(");"));
        return r;
    }
    const char* kind = nullptr;
    if (ckind == "class") kind = "class";
    else if (ckind == "interface") kind = "interface";
    else if (ckind == "enum") kind = "enum";
    else if (ckind == "struct") kind = "struct";
    else if (ckind == "record") kind = "record";
    else if (ckind == "record struct") kind = "record struct";
    if (!kind) throw Decline(S("class-kind"), S("a ") + or_empty(c->kind));
    if (lang == "java") java->check_class(c);
    py::list<str> mods = class_mods(c);
    str head = ind + mods_text(mods) + S(kind) + S(" ") + ident(or_empty(c->name)) + tparams_text(strs_of(c->typeparams), c->tparams);
    py::ref<ir::MethodDecl> primary;
    for (const NodeP& m : nodes_of(c->members))
        if (auto md = ref_as<ir::MethodDecl>(m))
            if (in_strs(md->mods, "primary")) {
                primary = md;
                break;
            }
    py::set<str> positional;
    if (primary) {
        head += S("(") + params_text(primary->params) + S(")");
        for (const NodeP& p : nodes_of(primary->params)) {
            auto pp = as<ir::Param>(p);
            if (pp->sym) positional.add(pp->sym->name);
        }
    }
    py::list<str> bases;
    if (ckind == "enum") {
        if (c->extends) bases.append(ctype(c->extends));
    } else {
        if (c->extends) {
            str b = ctype(c->extends);
            if (primary) {
                auto body = std::holds_alternative<NodeP>(primary->body) ? as<ir::Block>(std::get<NodeP>(primary->body)) : nullptr;
                if (!body) throw py::AttributeError("'NoneType' object has no attribute 'stmts'");
                py::ref<ir::CtorCall> call;
                for (const NodeP& s : nodes_of(body->stmts)) {
                    auto cc = ref_as<ir::CtorCall>(s);
                    if (cc && !(cc->kind && cc->kind->endswith(S(":implicit")))) {
                        call = cc;
                        break;
                    }
                }
                if (call) b += S("(") + args_text(call->args, call->sym) + S(")");
            }
            bases.append(b);
        }
        if (c->implements)
            for (const TypeP& i : *c->implements)
                if (lib_interface(i)) bases.append(ctype(i));
    }
    if (!bases.empty()) head += S(" : ") + jn(S(", "), bases);
    py::list<str> lines;
    lines.append(head);
    for (const NodeP& tp : nodes_of(c->tparams)) {
        str w = where_text(tp);
        if (!w.empty()) lines.append(ind + S("    ") + w);
    }
    py::list<NodeP> members;
    for (const NodeP& m : nodes_of(c->members)) {
        auto pr = as<ir::Property>(m);
        if (pr && in_strs(pr->mods, "positional") && pr->sym && positional.contains(pr->sym->name)) continue;
        if (primary && m == NodeP(primary)) continue;
        members.append(m);
    }
    if (primary && members.empty()) {
        lines[-1] += S(";");
        return lines;
    }
    lines.append(ind + S("{"));
    str inner = ind + S("    ");
    if (ckind == "enum") {
        py::list<NodeP> consts, rest;
        for (const NodeP& m : members) (is_a<ir::EnumConst>(m) ? consts : rest).append(m);
        for (int64_t i = 0; i < consts.size(); ++i) {
            auto e = as<ir::EnumConst>(consts[i]);
            str text = inner + ident(e->sym->name);
            if (e->value) text += S(" = ") + py::to_str(*e->value);
            else if (nonempty(e->args) || e->body) throw Decline(S("enum"), S("an enum constant with constructor arguments or a body"));
            lines.append(text + S(i < consts.size() - 1 ? "," : ""));
        }
        members = rest;
        if (!members.empty() && lang != "java") throw Decline(S("enum"), S("an enum with members"));
    }
    if (lang == "java") members = java->arrange_members(c, members);
    bool first = true;
    for (const NodeP& m : members) {
        py::list<str> text = member(m, c, inner);
        if (text.empty()) continue;
        if (!first && (text.size() > 1 || is_a<ir::MethodDecl>(m) || is_a<ir::ClassDecl>(m) || is_a<ir::Property>(m))) lines.append(str());
        lines.extend(text);
        first = false;
    }
    lines.append(ind + S("}"));
    return lines;
}

py::list<str> Writer::member(const NodeP& m, const ClassP& c, const str& ind) {
    if (auto cd = ref_as<ir::ClassDecl>(m)) return class_decl(cd, ind);
    if (auto f = ref_as<ir::FieldDecl>(m)) return field(f, c, ind);
    if (auto p = ref_as<ir::Property>(m)) return prop(p, c, ind);
    if (auto md = ref_as<ir::MethodDecl>(m)) return method_decl(md, c, ind);
    if (auto un = as<ir::Unsupported>(m)) {
        str why = truthy(un->raw) ? *un->raw : or_empty(un->kind);
        note(or_empty(un->kind), why);
        py::list<str> r;
        r.append(ind + S("// CB-GAP: ") + why);
        return r;
    }
    if (is_a<ir::EnumConst>(m)) throw Decline(S("enum"), S("an enum constant outside an enum"));
    throw Decline(S("member"), S("a member ") + m->class_name());
}

py::list<str> Writer::field(const py::ref<ir::FieldDecl>& f, const ClassP& c, const str& ind) {
    py::list<str> mods = lang == "java" ? java->field_mods(f, c) : strs_of(f->mods);
    str text = ind + mods_text(mods) + ctype(f->type) + S(" ") + ident(f->sym->name);
    NodeP init = f->init;
    auto it = prog.inits.find(static_cast<const ir::Node*>(f.get()));
    if (it != prog.inits.end()) init = it->second;
    if (init) {
        method = nullptr;
        try {
            text += S(" = ") + coerce(init, f->type);
        } catch (const Decline& d) {
            note(d.kind, d.why);
            text += S(" = default /* CB-GAP: ") + d.why + S(" */");
        }
    }
    py::list<str> r;
    r.append(text + S(";"));
    return r;
}

py::list<str> Writer::prop(const py::ref<ir::Property>& p, const ClassP& c, const str& ind) {
    (void)c;
    py::list<str> mods;
    for (const str& m : strs_of(p->mods))
        if (m != "positional") mods.append(m);
    str name = nonempty(p->params) ? S("this[") + params_text(p->params) + S("]") : ident(p->sym->name);
    str head = ind + mods_text(mods) + ctype(p->type) + S(" ") + name;
    const py::list<NodeP>& accs = nodes_of(p->accessors);
    if (accs.size() == 1) {
        auto a0 = as<ir::MethodDecl>(accs[0]);
        if (eq(a0->kind, "get") && ir::truthy(a0->body) && !(a0->mods && !a0->mods->empty())) {
            auto blk = std::holds_alternative<NodeP>(a0->body) ? as<ir::Block>(std::get<NodeP>(a0->body)) : nullptr;
            if (!blk) throw py::AttributeError("'list' object has no attribute 'stmts'");
            const py::list<NodeP>& b = nodes_of(blk->stmts);
            if (b.size() == 1) {
                auto r = as<ir::Return>(b[0]);
                if (r && r->expr) {
                    try {
                        method = std::dynamic_pointer_cast<ir::MethodDecl>(accs[0]);
                        py::list<str> out;
                        out.append(head + S(" => ") + expr(r->expr).first + S(";"));
                        return out;
                    } catch (const Decline&) {
                    }
                }
            }
        }
    }
    bool all_bodyless = true;
    for (const NodeP& a : accs)
        if (ir::truthy(as<ir::MethodDecl>(a)->body)) all_bodyless = false;
    if (all_bodyless) {
        py::list<str> parts;
        for (const NodeP& a : accs) {
            auto am = as<ir::MethodDecl>(a);
            parts.append(mods_text(strs_of(am->mods)) + or_empty(am->kind) + S(";"));
        }
        str text = head + S(" { ") + jn(S(" "), parts) + S(" }");
        if (p->init) {
            method = nullptr;
            text += S(" = ") + coerce(p->init, p->type) + S(";");
        }
        py::list<str> out;
        out.append(text);
        return out;
    }
    py::list<str> lines;
    lines.append(head);
    lines.append(ind + S("{"));
    for (const NodeP& an : accs) {
        auto a = std::dynamic_pointer_cast<ir::MethodDecl>(an);
        str amods = mods_text(strs_of(a->mods));
        if (!ir::truthy(a->body)) {
            lines.append(ind + S("    ") + amods + or_empty(a->kind) + S(";"));
            continue;
        }
        method = a;
        py::list<str> body = method_body(a, ind + S("    "));
        auto blk = std::holds_alternative<NodeP>(a->body) ? as<ir::Block>(std::get<NodeP>(a->body)) : nullptr;
        if (!blk) throw py::AttributeError("'list' object has no attribute 'stmts'");
        const py::list<NodeP>& st = nodes_of(blk->stmts);
        auto ret = st.size() == 1 ? as<ir::Return>(st[0]) : nullptr;
        if (ret && ret->expr && body.size() == 3) {
            lines.append(ind + S("    ") + amods + or_empty(a->kind) + S(" => ") + body[1].strip().slice_from(7));
        } else if (st.size() == 1 && is_a<ir::ExprStmt>(st[0]) && body.size() == 3) {
            lines.append(ind + S("    ") + amods + or_empty(a->kind) + S(" => ") + body[1].strip());
        } else {
            lines.append(ind + S("    ") + amods + or_empty(a->kind));
            lines.extend(body);
        }
    }
    lines.append(ind + S("}"));
    if (p->init) lines[-1] += S(" = ") + coerce(p->init, p->type) + S(";");
    return lines;
}

str Writer::params_text(const Nodes& params, bool varargs) {
    py::list<str> out;
    const py::list<NodeP>& ps = nodes_of(params);
    for (int64_t i = 0; i < ps.size(); ++i) {
        auto p = as<ir::Param>(ps[i]);
        py::list<str> mods;
        for (const str& m : strs_of(p->mods))
            if (m == "ref" || m == "out" || m == "in" || m == "params" || m == "this" || m == "scoped") mods.append(m);
        if (varargs && i == ps.size() - 1 && !mods.contains(S("params"))) mods.append(S("params"));
        if (p->sym && !p->sym->refs.empty() && !p->sym->refs[0].empty() && !mods.contains(p->sym->refs[0]))
            mods.insert(0, p->sym->refs[0]);
        str t = ctype(p->type);
        out.append(mods_text(mods) + t + S(" ") + ident(p->sym ? p->sym->name : S("p")));
    }
    return jn(S(", "), out);
}

py::list<str> Writer::method_decl(const MethodP& m, const ClassP& c, const str& ind) {
    method = m;
    labels.clear();
    const std::string mkind = or_empty(m->kind).utf8();
    if (mkind == "static_init") {
        if (lang == "java") return py::list<str>();   // (folded into the static constructor)
        str head = ind + S("static ") + ident(or_empty(c->name)) + S("()");
        py::list<str> r;
        r.append(head);
        r.extend(method_body(m, ind));
        return r;
    }
    if (mkind == "init") return py::list<str>();   // (Java's instance blocks run in each constructor)
    if (mkind == "cctor") {                          // Java's statics, in their one textual order
        py::list<str> r;
        r.append(ind + S("static ") + ident(or_empty(c->name)) + S("()"));
        r.extend(method_body(m, ind));
        return r;
    }
    bool varargs = lang == "java" && m->sym && m->sym->varargs;
    if (mkind == "ctor") {
        if (in_strs(m->mods, "IMPLICIT") && !(lang == "java" && java->ctor_needed(c, m))) return py::list<str>();
        py::list<str> mods;
        if (lang == "java") mods = java->method_mods(m, c);
        else
            for (const str& x : strs_of(m->mods))
                if (x != "IMPLICIT") mods.append(x);
        str head = ind + mods_text(mods) + ident(or_empty(c->name)) + S("(") + params_text(m->params, varargs) + S(")");
        std::vector<NodeP> stmts;
        if (ir::truthy(m->body)) {
            auto blk = std::holds_alternative<NodeP>(m->body) ? as<ir::Block>(std::get<NodeP>(m->body)) : nullptr;
            if (!blk) throw py::AttributeError("'list' object has no attribute 'stmts'");
            for (const NodeP& s : nodes_of(blk->stmts)) stmts.push_back(s);
        }
        py::ref<ir::CtorCall> cc = stmts.empty() ? nullptr : ref_as<ir::CtorCall>(stmts[0]);
        if (cc) {
            stmts.erase(stmts.begin());
            if (!(cc->kind && cc->kind->endswith(S(":implicit"))) && !cc->target) {
                try {
                    str a = args_text(cc->args, cc->sym);
                    head += S(" : ") + S(eq(cc->kind, "this") ? "this" : "base") + S("(") + a + S(")");
                } catch (const Decline& d) {
                    return gap(head, d, ind);
                }
            }
        }
        py::list<NodeP> all;
        if (lang == "java") all.extend(java->ctor_prologue(c, m, cc));
        for (const NodeP& s : stmts) all.append(s);
        auto body = std::make_shared<ir::Block>();
        body->stmts = all;
        py::list<str> r;
        r.append(head);
        r.extend(body_lines(body, ind, m));
        return r;
    }
    py::list<str> mods = lang == "java" ? java->method_mods(m, c) : strs_of(m->mods);
    str name = lang == "java" ? java->method_name(m, c) : ident(or_empty(m->name));
    str head;
    const std::string mname = or_empty(m->name).utf8();
    if (mods.contains(S("operator")) && DECL_OPERATORS().count(mname)) {
        py::list<str> rest;
        for (const str& x : mods)
            if (x != "operator") rest.append(x);
        head = ind + mods_text(rest) + ctype(m->ret) + S(" operator ") + S(DECL_OPERATORS().at(mname));
    } else if (mods.contains(S("operator")) && (mname == "op_Implicit" || mname == "op_Explicit")) {
        py::list<str> rest;
        for (const str& x : mods)
            if (x != "operator") rest.append(x);
        head = ind + mods_text(rest) + S(mname == "op_Implicit" ? "implicit" : "explicit") + S(" operator ") + ctype(m->ret);
    } else {
        str rt = ctype(m->ret);
        head = ind + mods_text(mods) + rt + S(" ") + name + tparams_text(strs_of(m->typeparams), m->tparams);
    }
    head += S("(") + params_text(m->params, varargs) + S(")");
    py::list<str> wheres;
    for (const NodeP& tp : nodes_of(m->tparams)) {
        str w = where_text(tp);
        if (!w.empty()) wheres.append(w);
    }
    py::list<str> r;
    if (!ir::truthy(m->body)) {
        r.append(head + (wheres.empty() ? str() : S(" ") + jn(S(" "), wheres)) + S(";"));
        return r;
    }
    py::list<str> lines;
    lines.append(head);
    for (const str& w : wheres) lines.append(ind + S("    ") + w);
    auto blk = std::holds_alternative<NodeP>(m->body) ? as<ir::Block>(std::get<NodeP>(m->body)) : nullptr;
    if (!blk) throw py::AttributeError("'list' object has no attribute 'stmts'");
    const py::list<NodeP>& st = nodes_of(blk->stmts);
    if (st.size() == 1 && lang == "csharp" && wheres.empty()) {
        auto ret = as<ir::Return>(st[0]);
        if (ret && ret->expr) {
            try {
                str e = ret_expr(ret->expr);
                r.append(head + S(" => ") + e + S(";"));
                return r;
            } catch (const Decline&) {
            }
        }
    }
    lines.extend(method_body(m, ind));
    return lines;
}

str Writer::ret_expr(const NodeP& e) { return coerce(e, method ? method->ret : TypeP()); }

py::list<str> Writer::method_body(const MethodP& m, const str& ind) {
    NodeP body = std::holds_alternative<NodeP>(m->body) ? std::get<NodeP>(m->body) : NodeP();
    if (!body) throw py::AttributeError("'list' object has no attribute 'stmts'");
    return body_lines(body, ind, m);
}

py::list<str> Writer::body_lines(const NodeP& body, const str& ind, const MethodP& m) {
    auto un = ir::unsupported(body);
    if (!un.empty()) {
        auto u0 = un[0];
        return gap(std::nullopt, Decline(or_empty(u0->kind), truthy(u0->raw) ? *u0->raw : or_empty(u0->kind)), ind);
    }
    try {
        py::list<str> out;
        locals = py::set<str>();
        if (m)
            for (const NodeP& p : nodes_of(m->params)) {
                auto pp = as<ir::Param>(p);
                if (pp->sym) locals.add(pp->sym->name);
            }
        block_names.clear();
        block_names.push_back(py::set<str>::from(locals));
        scopes.clear();
        scopes.push_back(py::dict<str, str>());
        block_into(body, out, ind);
        return out;
    } catch (const Decline& d) {
        return gap(std::nullopt, d, ind);
    }
}

py::list<str> Writer::gap(const std::optional<str>& head, const Decline& d, const str& ind) {
    note(d.kind, d.why);
    py::list<str> lines;
    if (head && !head->empty()) lines.append(*head);
    lines.append(ind + S("{"));
    lines.append(ind + S("    throw new System.NotSupportedException(") + cstr(S("CB-GAP: ") + d.why) + S(");"));
    lines.append(ind + S("}"));
    return lines;
}

// ---- statements
void Writer::block_into(const NodeP& b, py::list<str>& out, const str& ind) {
    out.append(ind + S("{"));
    py::list<NodeP> stmts;
    if (auto blk = as<ir::Block>(b)) stmts = nodes_of(blk->stmts);
    else stmts.append(b);
    if (lang == "csharp") stmts = initializers(stmts);
    py::set<str> names;
    for (const NodeP& x : stmts)
        if (auto l = as<ir::Local>(x))
            if (l->sym) names.add(l->sym->name);
    block_names.push_back(names);
    push();
    try {
        for (const NodeP& s : stmts) stmt(s, out, ind + S("    "));
    } catch (...) {
        pop();
        block_names.pop_back();
        throw;
    }
    pop();
    block_names.pop_back();
    out.append(ind + S("}"));
}

// A statement in a place that holds one (if, loop): always a block, for clarity.
void Writer::sub(const NodeOrList& s, py::list<str>& out, const str& ind) {
    if (std::holds_alternative<py::list<NodeP>>(s)) {
        // Block(stmts=[<a list>]): the block opens, then a list is not a statement
        throw Decline(S("list"), S("the statement list"));
    }
    NodeP n = std::get<NodeP>(s);
    if (as<ir::Block>(n)) {
        block_into(n, out, ind);
    } else {
        auto blk = std::make_shared<ir::Block>();
        py::list<NodeP> l;
        l.append(n);
        blk->stmts = l;
        block_into(blk, out, ind);
    }
}

str Writer::cond(const NodeP& e) { return coerce(strip(e), ir::BOOL); }   // (the parentheses an if or a loop writes anyway)

// A loop's body, with the label a Java `continue label` jumps to at its end.
void Writer::loop_body(const NodeP& loop, const NodeOrList& body, py::list<str>& out, const str& ind) {
    auto it = loop_labels.find(loop.get());
    bool has_cont = false;
    if (it != loop_labels.end()) {
        auto lb = labels.find(it->second.utf8());
        has_cont = lb != labels.end() && lb->second.second;
    }
    if (it == loop_labels.end() || !has_cont) {
        sub(body, out, ind);
        return;
    }
    py::list<NodeP> stmts;
    if (std::holds_alternative<py::list<NodeP>>(body)) stmts.append(NodeP());   // (the list as one "statement")
    else if (auto blk = as<ir::Block>(std::get<NodeP>(body))) stmts = nodes_of(blk->stmts);
    else stmts.append(std::get<NodeP>(body));
    out.append(ind + S("{"));
    for (const NodeP& x : stmts) {
        if (!x) throw Decline(S("list"), S("the statement list"));
        stmt(x, out, ind + S("    "));
    }
    out.append(ind + it->second + S("_continue:;"));
    out.append(ind + S("}"));
}

void Writer::for_(const py::ref<ir::For>& s, py::list<str>& out, const str& ind) {
    std::vector<std::pair<std::optional<str>, str>> inits;
    for (const NodeP& i : nodes_of(s->init)) {
        if (auto l = as<ir::Local>(i)) {
            std::optional<str> v;
            if (l->init) v = coerce(l->init, l->type);
            str t = ctype(l->type);
            str d = declare(l->sym->name, true);
            inits.emplace_back(t, d + (v ? S(" = ") + *v : str()));
        } else if (auto es = as<ir::ExprStmt>(i)) {
            inits.emplace_back(std::nullopt, expr(es->expr, true).first);
        } else {
            throw Decline(S("for"), S("a for initialiser ") + i->class_name());
        }
    }
    str init;
    if (!inits.empty() && inits[0].first) {
        for (auto& p : inits)
            if (!p.first || *p.first != *inits[0].first) {
                if (!p.first) break;
                throw Decline(S("for"), S("a for initialiser declaring locals of several types"));
            }
        py::list<str> xs;
        for (auto& p : inits) xs.append(p.second);
        init = *inits[0].first + S(" ") + jn(S(", "), xs);
    } else {
        for (auto& p : inits)
            if (p.first) throw Decline(S("for"), S("a for initialiser mixing declarations and expressions"));
        py::list<str> xs;
        for (auto& p : inits) xs.append(p.second);
        init = jn(S(", "), xs);
    }
    str c = s->cond ? cond(s->cond) : str();
    py::list<str> ups;
    for (const NodeP& u_ : nodes_of(s->update)) ups.append(expr(u_, true).first);
    out.append(ind + S("for (") + init + S("; ") + c + S("; ") + jn(S(", "), ups) + S(")"));
    loop_body(s, s->body, out, ind);
}

std::pair<bool, bool> Writer::label_uses(const NodeOrList& body, const str& label) {
    if (std::holds_alternative<py::list<NodeP>>(body)) throw py::AttributeError("'list' object has no attribute 'children'");
    std::pair<bool, bool> uses{false, false};
    for (const NodeP& n : ir::walk(std::get<NodeP>(body))) {
        if (auto b = as<ir::Break>(n))
            if (b->label && *b->label == label) uses.first = true;
        if (auto c = as<ir::Continue>(n))
            if (c->label && *c->label == label) uses.second = true;
    }
    return uses;
}

void Writer::s_Local(const py::ref<ir::Local>& s, py::list<str>& out, const str& ind) {
    str mods = (in_strs(s->mods, "const") && lang == "csharp") ? S("const ") : str();
    if (!s->init) {
        str name = declare(s->sym->name);
        out.append(ind + mods + ctype(s->type) + S(" ") + name + S(";"));
        return;
    }
    str init = coerce(s->init, s->type);
    str name = declare(s->sym->name);
    if (lang == "java") java->record(s->sym, s->init);
    str tn = ctype(s->type);
    NodeP si = strip(s->init);
    if ((is_a<ir::New>(si) || is_a<Init>(si)) && ir::same(type_of(s->init), s->type) && mods.empty()) {
        out.append(ind + S("var ") + name + S(" = ") + init + S(";"));
    } else {
        out.append(ind + mods + tn + S(" ") + name + S(" = ") + init + S(";"));
    }
}

void Writer::catch_(const py::ref<ir::Catch>& c, const TypeP& t, py::list<str>& out, const str& ind) {
    std::optional<str> tn;
    if (lang == "java") {
        if (t) tn = ctype(t);
    } else if (t) tn = ctype(t);
    if (!tn) {
        out.append(ind + S("catch"));
        block_into(c->body, out, ind);
    } else if (c->sym) {
        push();
        try {
            str d = declare(c->sym->name, true);
            out.append(ind + S("catch (") + *tn + S(" ") + d + S(")"));
            block_into(c->body, out, ind);
        } catch (...) {
            pop();
            throw;
        }
        pop();
    } else {
        out.append(ind + S("catch (") + *tn + S(")"));
        block_into(c->body, out, ind);
    }
}

void Writer::using_(const py::list<NodeP>& res, const NodeP& body, py::list<str>& out, const str& ind) {
    NodeP r = res[0];
    str head;
    auto loc = as<ir::Local>(r);
    if (loc) {
        str init = coerce(loc->init, loc->type);
        push();
        NodeP si = strip(loc->init);
        str tn = (is_a<ir::New>(si) && ir::same(type_of(loc->init), loc->type)) ? S("var") : ctype(loc->type);
        str d = declare(loc->sym->name, true);
        head = S("using (") + tn + S(" ") + d + S(" = ") + init + S(")");
    } else if (auto es = as<ir::ExprStmt>(r)) {
        head = S("using (") + expr(es->expr).first + S(")");
    } else {
        throw Decline(S("using"), S("a resource ") + r->class_name());
    }
    out.append(ind + head);
    try {
        if (res.size() > 1) {
            out.append(ind + S("{"));
            using_(res.slice_from(1), body, out, ind + S("    "));
            out.append(ind + S("}"));
        } else {
            block_into(body, out, ind);
        }
    } catch (...) {
        if (loc) pop();
        throw;
    }
    if (loc) pop();
}

void Writer::s_Try(const py::ref<ir::Try>& s, py::list<str>& out, const str& ind) {
    py::list<NodeP> res = nodes_of(s->resources);
    if (!res.empty() && !nonempty(s->catches) && !s->final) {
        using_(res, s->body, out, ind);
        return;
    }
    out.append(ind + S("try"));
    if (!res.empty()) {
        out.append(ind + S("{"));
        using_(res, s->body, out, ind + S("    "));
        out.append(ind + S("}"));
    } else {
        block_into(s->body, out, ind);
    }
    for (const NodeP& cn : nodes_of(s->catches)) {
        auto c = ref_as<ir::Catch>(cn);
        py::list<TypeP> types = c->types ? *c->types : py::list<TypeP>();
        if (types.size() > 1) {
            // a multi-catch: one clause per type, each with the same body
            for (const TypeP& t : types) catch_(c, t, out, ind);
            continue;
        }
        catch_(c, types.empty() ? TypeP() : types[0], out, ind);
    }
    if (s->final) {
        out.append(ind + S("finally"));
        block_into(s->final, out, ind);
    }
}

str Writer::case_label(const NodeP& lab0, const NodeP& scrutinee) {
    NodeP lab = strip(lab0);
    if (auto sel = as<ir::Select>(lab)) {
        if (sel->sym && sel->sym->kind == "enumconst") {
            auto t = std::make_shared<ir::Type>(S("class"), sel->sym->owner);
            return ctype(t) + S(".") + ident(sel->sym->name);
        }
    }
    if (auto l = as<ir::Lit>(lab)) {
        if (auto iv = std::get_if<int64_t>(&l->value.v)) {
            if (scrutinee) {
                TypeP st = type_of(scrutinee);
                if (st && st->kind == "u16") return cchar(py::chr(*iv));
            }
        }
    }
    return expr(lab).first;
}

void Writer::s_Switch(const py::ref<ir::Switch>& s, py::list<str>& out, const str& ind) {
    out.append(ind + S("switch (") + expr(s->expr).first + S(")"));
    out.append(ind + S("{"));
    const py::list<NodeP>& cases = nodes_of(s->cases);
    for (int64_t i = 0; i < cases.size(); ++i) {
        auto c = as<ir::Case>(cases[i]);
        for (const NodeP& lab : nodes_of(c->labels)) out.append(ind + S("    case ") + case_label(lab, s->expr) + S(":"));
        if (c->default_ && *c->default_) out.append(ind + S("    default:"));
        if (nodes_of(c->body).empty() && i < cases.size() - 1) continue;   // labels sharing the next section
        py::list<NodeP> body = nodes_of(c->body);
        str inner = ind + S("        ");
        for (const NodeP& x : body) stmt(x, out, inner);
        if (!abrupt(body)) {
            if (i + 1 >= cases.size()) {
                out.append(inner + S("break;"));
            } else {
                auto nxt = as<ir::Case>(cases[i + 1]);
                if (nonempty(nxt->labels)) out.append(inner + S("goto case ") + case_label((*nxt->labels)[0], s->expr) + S(";"));
                else out.append(inner + S("goto default;"));
            }
        }
    }
    out.append(ind + S("}"));
}

void Writer::stmt(const NodeP& s, py::list<str>& out, const str& ind) {
    if (!s) throw Decline(S("NoneType"), S("the statement NoneType"));
    if (auto x = as<ir::ExprStmt>(s)) {
        NodeP e = strip(x->expr);
        if (auto un = as<ir::Unsupported>(e)) throw Decline(or_empty(un->kind), truthy(un->raw) ? *un->raw : or_empty(un->kind));
        std::optional<str> text;
        if (lang == "java" && is_a<ir::Call>(e)) text = java->stmt_call(ref_as<ir::Call>(e));
        if (!text) text = expr(e, true).first;
        out.append(ind + *text + S(";"));
        return;
    }
    if (auto x = ref_as<ir::Local>(s)) return s_Local(x, out, ind);
    if (auto x = ref_as<ir::Return>(s)) {
        if (!x->expr) out.append(ind + S("return;"));
        else out.append(ind + S("return ") + ret_expr(x->expr) + S(";"));
        return;
    }
    if (auto x = ref_as<ir::If>(s)) {
        out.append(ind + S("if (") + cond(x->cond) + S(")"));
        sub(x->then, out, ind);
        // e = s.orelse; while e is not None: ...
        NodeOrList e = x->orelse;
        for (;;) {
            if (std::holds_alternative<NodeP>(e) && !std::get<NodeP>(e)) break;
            if (std::holds_alternative<NodeP>(e)) {
                if (auto ei = ref_as<ir::If>(std::get<NodeP>(e))) {
                    out.append(ind + S("else if (") + cond(ei->cond) + S(")"));
                    sub(ei->then, out, ind);
                    e = ei->orelse;
                    continue;
                }
            }
            out.append(ind + S("else"));
            sub(e, out, ind);
            break;
        }
        return;
    }
    if (auto x = ref_as<ir::Block>(s)) return block_into(x, out, ind);
    if (is_a<ir::Empty>(s)) {
        out.append(ind + S(";"));
        return;
    }
    if (auto x = as<ir::Comment>(s)) {
        out.append(ind + S("// ") + or_empty(x->text));
        return;
    }
    if (auto x = ref_as<ir::While>(s)) {
        out.append(ind + S("while (") + cond(x->cond) + S(")"));
        loop_body(s, x->body, out, ind);
        return;
    }
    if (auto x = ref_as<ir::DoWhile>(s)) {
        out.append(ind + S("do"));
        loop_body(s, x->body, out, ind);
        out.append(ind + S("while (") + cond(x->cond) + S(");"));
        return;
    }
    if (auto x = ref_as<ir::For>(s)) {
        push();
        try {
            for_(x, out, ind);
        } catch (...) {
            pop();
            throw;
        }
        pop();
        return;
    }
    if (auto x = ref_as<ir::ForEach>(s)) {
        auto v = ref_as<ir::Local>(x->var);
        std::optional<str> coll;
        if (lang == "java") coll = java->iterable(x->iterable);
        if (!coll) coll = expr(x->iterable).first;
        push();
        try {
            str t = ctype(v->type);
            str d = declare(v->sym->name, true);
            out.append(ind + S("foreach (") + t + S(" ") + d + S(" in ") + *coll + S(")"));
            loop_body(s, x->body, out, ind);
        } catch (...) {
            pop();
            throw;
        }
        pop();
        return;
    }
    if (auto x = ref_as<ir::Labeled>(s)) {
        str lab = ident(or_empty(x->label));
        NodeOrList body = x->body;
        NodeP bn = std::holds_alternative<NodeP>(body) ? std::get<NodeP>(body) : NodeP();
        if (bn && (is_a<ir::While>(bn) || is_a<ir::DoWhile>(bn) || is_a<ir::For>(bn) || is_a<ir::ForEach>(bn))) {
            auto uses = label_uses(body, or_empty(x->label));
            labels[lab.utf8()] = uses;
            loop_labels[bn.get()] = lab;
            stmt(bn, out, ind);
            if (uses.first) out.append(ind + lab + S("_break:;"));
            return;
        }
        if (lang == "csharp" || !label_uses(body, or_empty(x->label)).first) {
            out.append(ind + lab + S(":"));
            if (!bn) throw Decline(S("list"), S("the statement list"));
            stmt(bn, out, ind);
            return;
        }
        labels[lab.utf8()] = {true, false};
        if (!bn) throw Decline(S("list"), S("the statement list"));
        stmt(bn, out, ind);
        out.append(ind + lab + S("_break:;"));
        return;
    }
    if (auto x = as<ir::Break>(s)) {
        if (truthy(x->label)) out.append(ind + S("goto ") + ident(*x->label) + S("_break;"));
        else out.append(ind + S("break;"));
        return;
    }
    if (auto x = as<ir::Continue>(s)) {
        if (truthy(x->label)) out.append(ind + S("goto ") + ident(*x->label) + S("_continue;"));
        else out.append(ind + S("continue;"));
        return;
    }
    if (auto x = as<ir::Goto>(s)) {
        out.append(ind + S("goto ") + ident(or_empty(x->label)) + S(";"));
        return;
    }
    if (auto x = as<ir::Throw>(s)) {
        if (!x->expr) out.append(ind + S("throw;"));
        else out.append(ind + S("throw ") + expr(x->expr).first + S(";"));
        return;
    }
    if (auto x = ref_as<ir::Sync>(s)) {
        out.append(ind + S("lock (") + expr(x->lock).first + S(")"));
        sub(NodeOrList(x->body), out, ind);
        return;
    }
    if (auto x = as<ir::Assert>(s)) {
        if (lang == "java") {
            out.append(ind + S("// assert ") + expr(x->cond).first + S(" (Java runs asserts only with -ea)"));
            return;
        }
        usings.add(S("System.Diagnostics"));
        out.append(ind + S("Debug.Assert(") + cond(x->cond) + S(");"));
        return;
    }
    if (is_a<ir::LocalClass>(s)) throw Decline(S("local-class"), S("a class declared in a method"));
    if (is_a<ir::CtorCall>(s)) throw Decline(S("ctor-call"), S("a constructor call that is not the first statement"));
    if (auto x = ref_as<ir::Switch>(s)) return s_Switch(x, out, ind);
    if (auto x = ref_as<ir::Try>(s)) return s_Try(x, out, ind);
    if (is_a<ir::Yield>(s)) throw Decline(S("yield"), S("a switch-expression arm with statements"));
    if (auto x = as<ir::MachineFragment>(s))
        throw Decline(S("machine"), S("machine code (") + (truthy(x->reason) ? *x->reason : S("asm")) + S(")"));
    str k = s->class_name();
    throw Decline(k, S("the statement ") + k);
}

// ---- expressions
str Writer::wrap(const NodeP& e, int64_t prec, bool right) {
    Ex r = expr(e);
    if (r.second < prec || (right && r.second == prec)) return S("(") + r.first + S(")");
    return r.first;
}

str Writer::coerce(const NodeP& e, const TypeP& t) {
    TypeP et = type_of(e);
    if (t && t->kind == "u16" && et && (et->kind == "i32" || et->kind == "i8" || et->kind == "i16" || et->kind == "u8") &&
        ctype(t) == "char") {
        NodeP se = strip(e);
        if (auto l = as<ir::Lit>(se))
            if (auto iv = std::get_if<int64_t>(&l->value.v)) return cchar(py::chr(*iv & 0xffff));
            else if (auto bv = std::get_if<bool>(&l->value.v)) return cchar(py::chr(*bv ? 1 : 0));
        return S("(char)") + wrap(e, P_UNARY);
    }
    if (t && (t->kind == "i8" || t->kind == "i16" || t->kind == "u8") && et && et->kind == "u16" && is_a<ir::Lit>(strip(e)) && lang == "java") {
        auto l = as<ir::Lit>(strip(e));
        if (auto s = std::get_if<str>(&l->value.v)) return py::to_str(py::ord(*s));
        if (auto iv = std::get_if<int64_t>(&l->value.v)) return py::to_str(*iv);
        if (auto b = std::get_if<bool>(&l->value.v)) return S(*b ? "True" : "False");
        if (std::holds_alternative<double>(l->value.v)) return py::to_str(std::get<double>(l->value.v));
        return S("None");
    }
    if (lang == "java") {
        auto got = java->coerce(e, t);
        if (got) return *got;
    }
    return expr(e).first;
}

str Writer::member_name(const SymP& s) {
    if (lang == "java") return java->member_name(s);
    return ident(s->name);
}

bool Writer::shadowed(const str& name) { return locals.contains(name); }

str Writer::args_text(const Nodes& args, const SymP& s) {
    py::list<str> out;
    std::vector<str> refs;
    if (s && !s->refs.empty()) for (const str& r : s->refs) refs.push_back(r);
    std::vector<TypeP> params;
    if (s) for (const TypeP& p : s->params) params.push_back(p);
    const py::list<NodeP>& as_ = nodes_of(args);
    for (int64_t i = 0; i < as_.size(); ++i) {
        const NodeP& a = as_[i];
        str r = i < static_cast<int64_t>(refs.size()) ? refs[i] : str();
        TypeP pt;
        if (i < static_cast<int64_t>(params.size()) && !(s->varargs && i >= static_cast<int64_t>(params.size()) - 1)) pt = params[i];
        str text = pt ? coerce(a, pt) : expr(a).first;
        out.append((!r.empty() && r != "in" ? r + S(" ") : str()) + text);
    }
    return jn(S(", "), out);
}

std::optional<Ex> Writer::null_conditional(const py::ref<ir::Cond>& e) {
    // r != null ? r.m : null, written r?.m (C#'s own form of the test and the access)
    NodeP c = strip(e->cond);
    if (lang != "csharp") return std::nullopt;
    auto cb = as<ir::Binary>(c);
    if (!cb || !eq(cb->op, "!=")) return std::nullopt;
    auto rl = as<ir::Lit>(strip(cb->right));
    if (!rl || !rl->value.is_none()) return std::nullopt;
    auto ol = as<ir::Lit>(strip(e->orelse));
    if (!ol || !ol->value.is_none()) return std::nullopt;
    str r = expr(cb->left).first;
    Ex then = expr(e->then);
    if (then.second == P_PRIMARY && then.first.startswith(r + S(".")) && !then.first.startswith(r + S(".") + r))
        return Ex{r + S("?.") + then.first.slice_from(r.size() + 1), P_PRIMARY};
    return std::nullopt;
}

str Writer::interpolate(const py::list<NodeP>& parts) {
    str out = S("$\"");
    for (const NodeP& p : parts) {
        auto l = as<ir::Lit>(p);
        if (l && std::holds_alternative<str>(l->value.v)) {
            str s = cstr(std::get<str>(l->value.v));
            out += s.slice(1, s.size() - 1).replace(S("{"), S("{{")).replace(S("}"), S("}}"));
        } else {
            str text = expr(p).first;
            if (text.contains(S(":")) || text.contains(S("?"))) text = S("(") + text + S(")");
            out += S("{") + text + S("}");
        }
    }
    return out + S("\"");
}

Ex Writer::expr(const NodeP& e, bool statement) {
    Ex r = dispatch_(e);
    if (!statement) {
        NodeP se = strip(e);
        if (!is_a<ir::Lit>(se) && !is_a<ir::Paren>(se) && !lib_const(se)) {
            TypeP t = type_of(e);
            if (t && range_of(t->kind.utf8()) && is_const(e) && overflows(e)) return Ex{S("unchecked(") + r.first + S(")"), P_PRIMARY};
        }
    }
    return r;
}

Ex Writer::dispatch_(const NodeP& e) {
    if (!e) throw Decline(S("NoneType"), S("the expression NoneType"));
    if (auto x = as<ir::Name>(e)) return Ex{lookup(x->sym->name), P_PRIMARY};
    if (auto x = as<ir::Lit>(e)) {
        const ir::Val& v = x->value;
        const TypeP& t = x->type;
        if (v.is_none()) return Ex{S("null"), P_PRIMARY};
        if (auto b = std::get_if<bool>(&v.v)) return Ex{S(*b ? "true" : "false"), P_PRIMARY};
        if (auto sv = std::get_if<str>(&v.v)) {
            if (t && t->kind == "u16" && sv->size() == 1) {
                if (ctype(t) == "ushort") return Ex{py::to_str(py::ord(*sv)), P_PRIMARY};
                return Ex{cchar(*sv), P_PRIMARY};
            }
            return Ex{cstr(*sv), P_PRIMARY};
        }
        if (std::holds_alternative<double>(v.v) || (t && (t->kind == "f32" || t->kind == "f64"))) {
            str s = float_text(num_of(v), t);
            return Ex{s, s.startswith(S("-")) ? P_UNARY : P_PRIMARY};
        }
        if (auto iv = std::get_if<int64_t>(&v.v)) {
            int64_t val = *iv;
            std::string k = t ? t->kind.utf8() : "i32";
            if (k == "u16" && ctype(t) == "char") return Ex{cchar(py::chr(val & 0xffff)), P_PRIMARY};
            const char* suffix = k == "i64" ? "L" : k == "u32" ? "u" : k == "u64" ? "UL" : "";
            str s = py::to_str(val) + S(suffix);
            if (k == "i32" && val == INT32_MIN) s = S("int.MinValue");
            else if (k == "i32" && val == INT32_MAX) s = S("int.MaxValue");
            else if (k == "i64" && val == INT64_MIN) s = S("long.MinValue");
            else if (k == "i64" && val == INT64_MAX) s = S("long.MaxValue");
            else if (k == "u32" && val == 4294967295LL) s = S("uint.MaxValue");
            return Ex{s, s.startswith(S("-")) ? P_UNARY : P_PRIMARY};
        }
        throw Decline(S("literal"), S("a literal ") + v.repr());
    }
    if (auto x = as<ir::Paren>(e)) return expr(x->x);   // (grouping is written from the operators' precedence, where needed)
    if (auto x = as<ir::Call>(e)) {
        auto ce = ref_as<ir::Call>(e);
        if (java) {
            auto got = java->call(ce, false);
            if (got) return *got;
        }
        const SymP& s = x->sym;
        if (!s) throw Decline(S("call"), S("a call with no resolved method"));
        auto has_flag = [&](const char* f) { return s->flags.contains(S(f)); };
        const std::string sname = s->name.utf8();
        const py::list<NodeP>& args = nodes_of(x->args);
        if (has_flag("operator")) {
            if (OPERATOR_METHODS().count(sname) && args.size() == 2) {
                str op = S(OPERATOR_METHODS().at(sname));
                int64_t prec = BIN_PREC().at(op.utf8());
                str a = wrap(args[0], prec);
                str b = wrap(args[1], prec, true);
                return Ex{a + S(" ") + op + S(" ") + b, prec};
            }
            if (UNARY_METHODS().count(sname) && args.size() == 1)
                return Ex{S(UNARY_METHODS().at(sname)) + wrap(args[0], P_UNARY), P_UNARY};
            if (sname == "op_Implicit" && args.size() == 1) return expr(args[0]);
            if (sname == "op_Explicit" && args.size() == 1) return Ex{S("(") + ctype(x->type) + S(")") + wrap(args[0], P_UNARY), P_UNARY};
        }
        if (has_flag("delegate") && x->target) {
            str t = wrap(x->target, P_PRIMARY);
            return Ex{t + S("(") + args_text(x->args, s) + S(")"), P_PRIMARY};
        }
        str targs;
        bool has_targs = false;
        if (x->typeargs) {
            if (auto tl = std::get_if<py::list<TypeP>>(&*x->typeargs)) has_targs = !tl->empty();
            else has_targs = !std::get<py::list<ir::Val>>(*x->typeargs).empty();
        }
        if (has_targs && !has_flag("inferred") && (lang == "csharp" || !inferable(ce))) {
            py::list<str> ts;
            if (auto tl = std::get_if<py::list<TypeP>>(&*x->typeargs)) {
                for (const TypeP& t : *tl) ts.append(ctype(t));
            } else {
                throw py::AttributeError("'int' object has no attribute 'kind'");
            }
            targs = S("<") + jn(S(", "), ts) + S(">");
        }
        str name = member_name(s) + targs;
        if (has_flag("extension") && !args.empty() && x->target && is_a<ir::TypeName>(x->target)) {
            py::list<TypeP> rest_params = s->params.slice_from(1);
            py::list<str> rest_refs = !s->refs.empty() ? s->refs.slice_from(1) : py::list<str>();
            auto rest = std::make_shared<ir::Sym>(s->kind, s->name, s->owner, s->static_, s->type, rest_params, s->ret, s->varargs, rest_refs, s->flags);
            ClassP c = s->owner ? prog.classes.get(*s->owner) : nullptr;
            str ns;
            if (c) ns = namespace_of(c);
            else if (s->owner && s->owner->contains(S("."))) ns = rsplit_dot(*s->owner).first;
            if (!ns.empty()) usings.add(ns);
            str recv = wrap(args[0], P_PRIMARY);
            Nodes tail = args.slice_from(1);
            return Ex{recv + S(".") + name + S("(") + args_text(tail, rest) + S(")"), P_PRIMARY};
        }
        if (!x->target) return Ex{name + S("(") + args_text(x->args, s) + S(")"), P_PRIMARY};
        if (is_a<ir::TypeName>(x->target) && cls) {
            TypeP tt = type_of(x->target);
            if (tt && tt->name && cls->qname && *tt->name == *cls->qname) return Ex{name + S("(") + args_text(x->args, s) + S(")"), P_PRIMARY};
        }
        str t = wrap(x->target, P_PRIMARY);
        return Ex{t + S(".") + name + S("(") + args_text(x->args, s) + S(")"), P_PRIMARY};
    }
    if (auto x = as<ir::Select>(e)) {
        if (java) {
            auto got = java->select(ref_as<ir::Select>(e));
            if (got) return *got;
        }
        const SymP& s = x->sym;
        auto th = as<ir::This>(x->obj);
        if (th && !th->qualifier && !shadowed(s->name)) return Ex{member_name(s), P_PRIMARY};   // this.x, where no local hides x
        if (is_a<ir::TypeName>(x->obj) && cls) {
            TypeP tt = type_of(x->obj);
            if (tt && tt->name && cls->qname && *tt->name == *cls->qname) return Ex{ident(s->name), P_PRIMARY};   // a static of this class
        }
        str o = wrap(x->obj, P_PRIMARY);
        return Ex{o + S(".") + member_name(s), P_PRIMARY};
    }
    if (auto x = as<ir::This>(e)) {
        if (x->qualifier && cls && x->qualifier->name != cls->qname) throw Decline(S("outer-this"), S("an outer instance (Outer.this)"));
        return Ex{S("this"), P_PRIMARY};
    }
    if (is_a<ir::Super>(e)) return Ex{S("base"), P_PRIMARY};
    if (is_a<ir::TypeName>(e)) return Ex{ctype(type_of(e)), P_PRIMARY};
    if (auto x = as<ir::Length>(e)) return Ex{wrap(x->arr, P_PRIMARY) + S(".Length"), P_PRIMARY};
    if (auto x = as<ir::Index>(e)) {
        if (x->sym && lang == "java") throw Decline(S("indexer"), S("an indexer from Java"));
        py::list<NodeP> idx;
        if (std::holds_alternative<py::list<NodeP>>(x->index)) idx = std::get<py::list<NodeP>>(x->index);
        else idx.append(std::get<NodeP>(x->index));
        str a = wrap(x->arr, P_PRIMARY);
        py::list<str> is;
        for (const NodeP& i : idx) is.append(expr(i).first);
        return Ex{a + S("[") + jn(S(", "), is) + S("]"), P_PRIMARY};
    }
    if (auto x = as<ir::Unary>(e)) {
        str op = or_empty(x->op);
        if ((op == "++" || op == "--") && x->postfix && *x->postfix) return Ex{wrap(x->x, P_PRIMARY) + op, P_PRIMARY};
        str v = wrap(x->x, P_UNARY);
        if ((op == "-" || op == "+") && v.startswith(op)) v = S("(") + v + S(")");   // - -x, not --x
        str text = op + v;
        if (eq(x->overflow, "trap")) return Ex{S("checked(") + text + S(")"), P_PRIMARY};
        return Ex{text, P_UNARY};
    }
    if (auto x = as<ir::Binary>(e)) {
        str op = or_empty(x->op);
        if (java) {
            auto got = java->binary(ref_as<ir::Binary>(e));
            if (got) return *got;
        }
        auto pit = BIN_PREC().find(op.utf8());
        if (pit == BIN_PREC().end()) throw Decline(S("operator"), S("the operator ") + op);
        int64_t prec = pit->second;
        str text;
        if (op == "??") {
            str a = wrap(x->left, prec, true);
            str b = wrap(x->right, prec);
            text = a + S(" ?? ") + b;
        } else {
            str a = wrap(x->left, prec);
            str b = wrap(x->right, prec, true);
            text = a + S(" ") + op + S(" ") + b;
        }
        if (eq(x->overflow, "trap")) return Ex{S("checked(") + text + S(")"), P_PRIMARY};
        return Ex{text, prec};
    }
    if (auto x = as<ir::Concat>(e)) {
        py::list<NodeP> parts = nodes_of(x->parts).copy();
        py::list<str> texts;
        for (int64_t i = 0; i < parts.size(); ++i) {
            std::optional<str> got;
            if (lang == "java") got = java->concat_part(parts[i]);
            if (got) {
                texts.append(*got);
                continue;
            }
            texts.append(wrap(parts[i], P_ADD, i > 0));
        }
        if (parts.size() >= 2 && !is_string(type_of(parts[0])) && !is_string(type_of(parts[1])) &&
            !(texts[0].startswith(S("JavaRt.Str(")) || texts[1].startswith(S("JavaRt.Str("))))
            texts.insert(0, S("\"\""));
        if (texts.size() == 1) texts.insert(0, S("\"\""));
        if (lang == "csharp" && parts.size() > 2) {
            bool all_ok = true;
            for (const NodeP& p : parts) {
                auto l = as<ir::Lit>(p);
                if (l && !std::holds_alternative<str>(l->value.v)) all_ok = false;
            }
            if (all_ok && interpolatable(parts)) return Ex{interpolate(parts), P_PRIMARY};
        }
        return Ex{jn(S(" + "), texts), P_ADD};
    }
    if (auto x = as<ir::Assign>(e)) {
        str target = wrap(x->target, P_UNARY);
        if (!x->op) {
            str value = coerce(x->value, type_of(x->target));
            return Ex{target + S(" = ") + value, P_ASSIGN};
        }
        str op = *x->op + S("=");
        str value = expr(x->value).first;
        if (eq(x->overflow, "trap")) return Ex{S("checked(") + target + S(" ") + op + S(" ") + value + S(")"), P_PRIMARY};
        return Ex{target + S(" ") + op + S(" ") + value, P_ASSIGN};
    }
    if (auto x = as<ir::Cond>(e)) {
        auto got = null_conditional(ref_as<ir::Cond>(e));
        if (got) return *got;
        str c = wrap(x->cond, P_COND + 1);
        str a = x->type ? coerce(x->then, x->type) : expr(x->then).first;
        str b = !x->type ? wrap(x->orelse, P_COND) : coerce(x->orelse, x->type);
        if (a.contains(S(" = "))) a = S("(") + a + S(")");
        return Ex{c + S(" ? ") + a + S(" : ") + b, P_COND};
    }
    if (auto x = as<ir::Cast>(e)) {
        str tn = ctype(x->type);
        str v = wrap(x->x, P_UNARY);
        if (v.startswith(S("-")) || v.startswith(S("+"))) v = S("(") + v + S(")");
        str text = S("(") + tn + S(")") + v;
        if (eq(x->overflow, "trap")) return Ex{S("checked(") + text + S(")"), P_PRIMARY};
        return Ex{text, P_UNARY};
    }
    if (auto x = as<ir::InstanceOf>(e)) {
        str text = wrap(x->x, P_REL) + S(" is ") + ctype(x->test);
        if (x->binding) {
            auto bl = as<ir::Local>(x->binding);
            if (!bl) throw py::AttributeError("'" + x->binding->class_name().utf8() + "' object has no attribute 'sym'");
            if (bl->sym) text += S(" ") + declare(bl->sym->name);
        }
        return Ex{text, P_REL};
    }
    if (auto x = as<ir::ClassLit>(e)) return Ex{S("typeof(") + ctype(x->of) + S(")"), P_PRIMARY};
    if (auto x = as<ir::Lambda>(e)) {
        if (java) java->check_lambda(ref_as<ir::Lambda>(e));
        // untyped parameters: a lambda here always has a target type (an argument, a typed local, a return)
        py::set<str> saved = locals;
        locals = py::set<str>::from(saved);
        push();
        try {
            py::list<str> names;
            for (const NodeP& p : nodes_of(x->params)) names.append(declare(as<ir::Param>(p)->sym->name, true));
            str ps = names.size() == 1 ? names[0] : S("(") + jn(S(", "), names) + S(")");
            Ex r;
            if (as<ir::Block>(x->body)) {
                py::list<str> out;
                block_into(x->body, out, str());
                // keep it on lines of its own, re-indented by the caller's line
                r = Ex{ps + S(" =>\n") + jn(S("\n"), out), P_ASSIGN};
            } else {
                r = Ex{ps + S(" => ") + expr(x->body).first, P_ASSIGN};
            }
            locals = saved;
            pop();
            return r;
        } catch (...) {
            locals = saved;
            pop();
            throw;
        }
    }
    if (auto x = as<ir::MethodRef>(e)) {
        if (eq(x->mode, "new")) throw Decline(S("method-ref"), S("a constructor reference"));
        if (is_a<ir::TypeName>(x->target) && !x->sym->static_) throw Decline(S("method-ref"), S("an unbound method reference (Type::instanceMethod)"));
        return Ex{wrap(x->target, P_PRIMARY) + S(".") + member_name(x->sym), P_PRIMARY};
    }
    if (auto x = as<ir::SwitchExpr>(e)) {
        py::list<str> arms;
        for (const NodeP& cn : nodes_of(x->cases)) {
            auto c = as<ir::Case>(cn);
            if (nodes_of(c->body).size() != 1) throw Decline(S("switch-expression"), S("a switch-expression arm with statements"));
            NodeP b = nodes_of(c->body)[0];
            str val;
            if (auto y = as<ir::Yield>(b)) val = coerce(y->expr, x->type);
            else if (auto t = as<ir::Throw>(b)) val = S("throw ") + expr(t->expr).first;
            else if (auto es = as<ir::ExprStmt>(b)) val = expr(es->expr).first;
            else throw Decline(S("switch-expression"), S("a switch-expression arm ") + b->class_name());
            py::list<str> pats;
            for (const NodeP& l : nodes_of(c->labels)) pats.append(case_label(l, x->expr));
            if (c->default_ && *c->default_) {
                pats = py::list<str>();
                pats.append(S("_"));
            }
            arms.append(jn(S(" or "), pats) + S(" => ") + val);
        }
        return Ex{wrap(x->expr, P_PRIMARY) + S(" switch { ") + jn(S(", "), arms) + S(" }"), P_PRIMARY};
    }
    if (auto x = as<ir::New>(e)) {
        if (x->body) throw Decline(S("anonymous-class"), S("an anonymous class"));
        if (x->outer) throw Decline(S("inner-class"), S("an inner class made on an outer instance"));
        if (java) {
            auto got = java->new_(ref_as<ir::New>(e));
            if (got) return *got;
        }
        str t = ctype(x->type);
        return Ex{S("new ") + t + S("(") + args_text(x->args, x->sym) + S(")"), P_PRIMARY};
    }
    if (auto x = as<ir::NewArray>(e)) {
        TypeP t = x->type;
        if (x->init) {
            if (nonempty(x->dims)) throw Decline(S("array"), S("an array with sizes and items"));
            TypeP elem = t->kind == "array" ? t->elem : x->elem;
            py::list<str> items;
            for (const NodeP& y : *x->init) items.append(coerce(y, elem));
            str it = jn(S(", "), items);
            if (!it.empty()) return Ex{S("new ") + ctype(t) + S(" { ") + it + S(" }"), P_PRIMARY};
            return Ex{S("new ") + ctype(t) + S(" { }"), P_PRIMARY};
        }
        if (t->kind == "array" && info_int(t, "rank", 1) != 1) {
            str tn = ctype(t->elem);
            py::list<str> ds;
            for (const NodeP& d : nodes_of(x->dims)) ds.append(expr(d).first);
            return Ex{S("new ") + tn + S("[") + jn(S(", "), ds) + S("]"), P_PRIMARY};
        }
        int64_t n;
        TypeP base;
        if (t->kind == "array") std::tie(n, base) = t->dims();
        else {
            n = 1;
            base = x->elem;
        }
        str sizes;
        for (const NodeP& d : nodes_of(x->dims)) sizes += S("[") + expr(d).first + S("]");
        str rest;
        for (int64_t i = 0; i < n - py::len(nodes_of(x->dims)); ++i) rest += S("[]");
        return Ex{S("new ") + ctype(base) + sizes + rest, P_PRIMARY};
    }
    if (auto x = dynamic_cast<Init*>(e.get())) {
        str nw = dispatch_(x->new_).first;
        if (nw.endswith(S("()"))) nw = nw.slice(0, nw.size() - 2);
        py::list<str> items;
        for (const InitItem& it : x->items) {
            if (it.is_set) {
                str m = member_name(it.sym);
                items.append(m + S(" = ") + coerce(it.value, it.sym->type));
            } else if (it.args.size() == 1) {
                items.append(expr(it.args[0]).first);
            } else {
                py::list<str> xs;
                for (const NodeP& y : it.args) xs.append(expr(y).first);
                items.append(S("{ ") + jn(S(", "), xs) + S(" }"));
            }
        }
        return Ex{nw + S(" { ") + jn(S(", "), items) + S(" }"), P_PRIMARY};
    }
    if (auto x = as<ir::Unsupported>(e)) throw Decline(or_empty(x->kind), truthy(x->raw) ? *x->raw : or_empty(x->kind));
    str k = e->class_name();
    throw Decline(k, S("the expression ") + k);
}

}  // namespace codebreaker::translate::backends::csharp

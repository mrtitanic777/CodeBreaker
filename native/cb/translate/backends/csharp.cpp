// cb/translate/backends/csharp.cpp -- the C# back end (see csharp.hpp).
//
// The writer walks one unit's classes and writes lines of C#, each expression with the precedence of its outermost operator so
// that parentheses are written only where they are needed. A construct it does not carry throws Decline, which the nearest
// place that can write a gap (a method body, a field initialiser, a class) catches and turns into a ledger entry and a
// `throw new System.NotSupportedException("CB-GAP: ...")` -- an isolation boundary, as docs/CPP_DESIGN.md 3.6 allows.
#include "cb/translate/backends/csharp.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <limits>
#include <unordered_set>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::translate::backends::csharp {

using ir::Node;
using ir::TypeP;
using Lines = std::vector<std::string>;

namespace {

namespace text = cb::text;

bool contains(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }

bool has(const std::vector<std::string>& v, std::string_view x) { return std::find(v.begin(), v.end(), x) != v.end(); }

std::string mods_text(const std::vector<std::string>& mods) {
    std::string out;
    for (const std::string& m : mods) out += m + " ";
    return out;
}

void extend(Lines& to, const Lines& more) { to.insert(to.end(), more.begin(), more.end()); }

// ---- text: UTF-8 in, C# literals out ------------------------------------------------------------------------------------

// The code points of UTF-8 text. A byte that is not valid UTF-8 stands for itself (as Latin-1); a surrogate written as three
// bytes (a Java char that is half of a pair) is kept, and written back as its \u escape.
std::vector<char32_t> code_points(std::string_view s) {
    std::vector<char32_t> out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size();) {
        const auto b = static_cast<unsigned char>(s[i]);
        std::size_t len = b < 0x80 ? 1 : (b >> 5) == 0x6 ? 2 : (b >> 4) == 0xe ? 3 : (b >> 3) == 0x1e ? 4 : 0;
        char32_t cp = b;
        bool ok = len != 0 && i + len <= s.size();
        if (ok && len > 1) {
            cp = b & (0xff >> (len + 1));
            for (std::size_t k = 1; k < len; ++k) {
                const auto c = static_cast<unsigned char>(s[i + k]);
                if ((c & 0xc0) != 0x80) {
                    ok = false;
                    break;
                }
                cp = (cp << 6) | (c & 0x3f);
            }
        }
        if (!ok) {
            out.push_back(b);
            ++i;
        } else {
            out.push_back(cp);
            i += len;
        }
    }
    return out;
}

// One code point as it goes between the quotes of a C# literal.
void escape_cp(std::string& out, char32_t cp) {
    switch (cp) {
    case U'"': out += "\\\""; return;
    case U'\\': out += "\\\\"; return;
    case U'\n': out += "\\n"; return;
    case U'\r': out += "\\r"; return;
    case U'\t': out += "\\t"; return;
    case U'\0': out += "\\0"; return;
    default: break;
    }
    if (cp >= 0x20 && cp <= 0x7e) {
        out.push_back(static_cast<char>(cp));
    } else if (cp > 0xffff) {
        const std::uint32_t v = cp - 0x10000;
        out += std::format("\\u{:04x}\\u{:04x}", 0xd800 + (v >> 10), 0xdc00 + (v & 0x3ff));
    } else {
        out += std::format("\\u{:04x}", static_cast<std::uint32_t>(cp));
    }
}

std::string cstr(std::string_view s) {
    std::string out = "\"";
    for (char32_t cp : code_points(s)) escape_cp(out, cp);
    out += '"';
    return out;
}

std::string cchar_cp(char32_t cp) {
    if (cp == U'\'') return "'\\''";
    if (cp == U'"') return "'\"'";
    std::string out = "'";
    escape_cp(out, cp);
    return out + "'";
}

// A char literal for the code point a number names (its low 16 bits).
std::string cchar_number(std::int64_t value) { return cchar_cp(static_cast<char32_t>(value & 0xffff)); }

std::string float_text(double v, const TypeP& t) {
    const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    const bool single = p != nullptr && p->prim == ir::Prim::F32;
    if (std::isnan(v)) return single ? "float.NaN" : "double.NaN";
    if (std::isinf(v)) return std::string(single ? "float" : "double") + (v > 0 ? ".PositiveInfinity" : ".NegativeInfinity");
    char buf[64];
    std::to_chars_result r{};
    if (single) {
        const float f = static_cast<float>(v);
        if (std::isinf(f)) throw Decline("literal", "a float literal beyond the range of float");
        r = std::to_chars(buf, buf + sizeof buf, f);   // the shortest text that is still this float
    } else {
        r = std::to_chars(buf, buf + sizeof buf, v);
    }
    std::string out(buf, r.ptr);
    if (out.find('e') == std::string::npos && out.find('.') == std::string::npos) out += ".0";
    std::replace(out.begin(), out.end(), 'e', 'E');
    return single ? out + "f" : out;
}

std::optional<double> number_of(const ir::LitValue& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return static_cast<double>(*i);
    if (const auto* u = std::get_if<std::uint64_t>(&v)) return static_cast<double>(*u);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1.0 : 0.0;
    if (const auto* d = std::get_if<double>(&v)) return *d;
    if (const auto* s = std::get_if<std::string>(&v)) {
        double d = 0;
        const char* end = s->data() + s->size();
        const auto r = std::from_chars(s->data(), end, d);
        if (r.ec == std::errc() && r.ptr == end) return d;
    }
    return std::nullopt;
}

bool is_text(const ir::LitValue& v) { return std::holds_alternative<std::string>(v); }
bool is_null(const ir::LitValue& v) { return std::holds_alternative<std::monostate>(v); }

// ---- types and nodes ----------------------------------------------------------------------------------------------------

std::optional<ir::Prim> prim_of(const TypeP& t) {
    if (!t) return std::nullopt;
    if (const auto* p = std::get_if<ir::PrimType>(&t->body)) return p->prim;
    return std::nullopt;
}
bool is_prim(const TypeP& t, ir::Prim prim) { return prim_of(t) == prim; }
// The class name of a class type, or null.
const std::string* class_of(const TypeP& t) {
    if (!t) return nullptr;
    const auto* c = std::get_if<ir::ClassType>(&t->body);
    return c != nullptr ? &c->name : nullptr;
}
bool nullable(const ir::ClassType& c) { return c.nullable; }

struct IntRange {
    std::int64_t lo;
    std::int64_t hi;
    int bits;
};
// The integer types a constant expression can leave the range of (a u64 only up to int64's maximum: the folding is 64-bit).
std::optional<IntRange> range_of(const TypeP& t) {
    const auto p = prim_of(t);
    if (!p) return std::nullopt;
    switch (*p) {
    case ir::Prim::I8: return IntRange{-128, 127, 8};
    case ir::Prim::U8: return IntRange{0, 255, 8};
    case ir::Prim::I16: return IntRange{-32768, 32767, 16};
    case ir::Prim::U16: return IntRange{0, 65535, 16};
    case ir::Prim::I32: return IntRange{std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max(), 32};
    case ir::Prim::U32: return IntRange{0, 4294967295LL, 32};
    case ir::Prim::I64: return IntRange{std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max(), 64};
    case ir::Prim::U64: return IntRange{0, std::numeric_limits<std::int64_t>::max(), 64};
    default: return std::nullopt;
    }
}

// An expression's type: a local's is its declared type.
TypeP plain_type_of(Node* e) {
    if (e == nullptr) return {};
    if (const auto* l = ir::get<ir::Local>(e)) return l->type;
    return e->type;
}

std::string_view op_of(const ir::Binary& b) { return b.op; }

// ---- tables ---------------------------------------------------------------------------------------------------------------

const std::unordered_map<std::string_view, std::string_view>& alias_table() {
    static const std::unordered_map<std::string_view, std::string_view> m = {
        {"System.String", "string"}, {"System.Object", "object"}, {"System.Boolean", "bool"}, {"System.Int32", "int"},
        {"System.Int64", "long"}, {"System.Double", "double"}, {"System.Single", "float"}, {"System.Char", "char"},
        {"System.Byte", "byte"}, {"System.SByte", "sbyte"}, {"System.Int16", "short"}, {"System.UInt16", "ushort"},
        {"System.UInt32", "uint"}, {"System.UInt64", "ulong"}, {"System.Decimal", "decimal"}, {"System.Void", "void"}};
    return m;
}

const std::unordered_map<std::string_view, int>& binary_precedence() {
    static const std::unordered_map<std::string_view, int> m = {
        {"*", kMul}, {"/", kMul}, {"%", kMul}, {"+", kAdd}, {"-", kAdd}, {"<<", kShift}, {">>", kShift}, {">>>", kShift},
        {"<", kRel}, {">", kRel}, {"<=", kRel}, {">=", kRel}, {"==", kEq}, {"!=", kEq}, {"&", kAnd}, {"^", kXor}, {"|", kOr},
        {"&&", kLand}, {"||", kLor}, {"??", kCoalesce}};
    return m;
}

const std::unordered_map<std::string_view, std::string_view>& operator_methods() {
    static const std::unordered_map<std::string_view, std::string_view> m = {
        {"op_Addition", "+"}, {"op_Subtraction", "-"}, {"op_Multiply", "*"}, {"op_Division", "/"}, {"op_Modulus", "%"},
        {"op_Equality", "=="}, {"op_Inequality", "!="}, {"op_LessThan", "<"}, {"op_GreaterThan", ">"},
        {"op_LessThanOrEqual", "<="}, {"op_GreaterThanOrEqual", ">="}, {"op_BitwiseAnd", "&"}, {"op_BitwiseOr", "|"},
        {"op_ExclusiveOr", "^"}, {"op_LeftShift", "<<"}, {"op_RightShift", ">>"}, {"op_UnsignedRightShift", ">>>"}};
    return m;
}

const std::unordered_map<std::string_view, std::string_view>& unary_methods() {
    static const std::unordered_map<std::string_view, std::string_view> m = {
        {"op_UnaryNegation", "-"}, {"op_UnaryPlus", "+"}, {"op_LogicalNot", "!"}, {"op_OnesComplement", "~"}};
    return m;
}

// every operator a class can declare, as the name of its method says it
const std::unordered_map<std::string_view, std::string_view>& declared_operators() {
    static const std::unordered_map<std::string_view, std::string_view> m = [] {
        auto r = operator_methods();
        for (const auto& [k, v] : unary_methods()) r[k] = v;
        r["op_Increment"] = "++";
        r["op_Decrement"] = "--";
        r["op_True"] = "true";
        r["op_False"] = "false";
        return r;
    }();
    return m;
}

const std::unordered_set<std::string_view>& keywords() {
    static const std::unordered_set<std::string_view> k = {
        "abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue",
        "decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally",
        "fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock", "long",
        "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", "protected", "public",
        "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct", "switch",
        "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual", "void",
        "volatile", "while"};
    return k;
}

// ---- constants: where Java folds with wrapping and C# refuses to overflow ---------------------------------------------

// library constants a constant expression may use (Java's Integer.MAX_VALUE is a field, not a literal)
std::optional<std::int64_t> lib_const_value(Node* e) {
    const auto* s = ir::get<ir::Select>(e);
    if (s == nullptr || !s->sym || !s->sym->owner) return std::nullopt;
    static const std::map<std::pair<std::string_view, std::string_view>, std::int64_t> m = {
        {{"java.lang.Integer", "MAX_VALUE"}, 2147483647LL}, {{"java.lang.Integer", "MIN_VALUE"}, -2147483648LL},
        {{"java.lang.Long", "MAX_VALUE"}, std::numeric_limits<std::int64_t>::max()},
        {{"java.lang.Long", "MIN_VALUE"}, std::numeric_limits<std::int64_t>::min()},
        {{"java.lang.Short", "MAX_VALUE"}, 32767}, {{"java.lang.Short", "MIN_VALUE"}, -32768},
        {{"java.lang.Byte", "MAX_VALUE"}, 127}, {{"java.lang.Byte", "MIN_VALUE"}, -128},
        {{"java.lang.Character", "MAX_VALUE"}, 65535}, {{"java.lang.Character", "MIN_VALUE"}, 0}};
    const auto it = m.find({*s->sym->owner, s->sym->name});
    if (it == m.end()) return std::nullopt;
    return it->second;
}
bool lib_const(Node* e) { return lib_const_value(e).has_value(); }

// A compile-time constant expression.
bool is_const(Node* e0) {
    Node* e = strip(e0);
    if (lib_const(e)) return true;
    if (const auto* l = ir::get<ir::Lit>(e)) {
        if (is_null(l->value)) return false;
        if (const auto* s = std::get_if<std::string>(&l->value)) return code_points(*s).size() == 1;
        return true;
    }
    if (const auto* u = ir::get<ir::Unary>(e)) return (u->op == "-" || u->op == "+" || u->op == "~") && is_const(u->x);
    if (const auto* b = ir::get<ir::Binary>(e)) {
        const std::string_view op = op_of(*b);
        return binary_precedence().contains(op) && op != "&&" && op != "||" && op != "??" && is_const(b->left) && is_const(b->right);
    }
    if (const auto* c = ir::get<ir::Cast>(e)) return c->x != nullptr && e->type && e->type->is_prim() && is_const(c->x);
    return false;
}

// Does folding this constant integer expression leave its type's range anywhere along the way? The arithmetic is 64-bit: a
// result that leaves int64 is reported as leaving the type's range and wraps as the type does.
class Folder {
public:
    bool out_of_range = false;

    std::optional<std::int64_t> value(Node* x0) {
        Node* x = strip(x0);
        const TypeP xt = plain_type_of(x);
        if (const auto* l = ir::get<ir::Lit>(x)) {
            if (const auto* s = std::get_if<std::string>(&l->value)) {
                const auto cps = code_points(*s);
                if (cps.empty()) return std::nullopt;
                return static_cast<std::int64_t>(cps.front());
            }
            if (const auto* i = std::get_if<std::int64_t>(&l->value)) return *i;
            if (const auto* b = std::get_if<bool>(&l->value)) return *b ? 1 : 0;
            return std::nullopt;
        }
        if (const auto lc = lib_const_value(x)) return lc;
        std::optional<std::int64_t> r;
        bool big = false;   // the exact result left int64; `r` is its wrap
        if (const auto* u = ir::get<ir::Unary>(x)) {
            const auto a = value(u->x);
            if (!a) return std::nullopt;
            if (u->op == "-") {
                if (*a == std::numeric_limits<std::int64_t>::min()) big = true;
                r = wrap_sub(0, *a);
            } else if (u->op == "+") {
                r = *a;
            } else if (u->op == "~") {
                r = ~*a;
            } else {
                return std::nullopt;
            }
        } else if (const auto* b = ir::get<ir::Binary>(x)) {
            const auto a = value(b->left);
            const auto c = value(b->right);
            if (!a || !c) return std::nullopt;
            r = binary(b->op, *a, *c, xt, big);
        } else if (const auto* cst = ir::get<ir::Cast>(x)) {
            r = value(cst->x);
        } else {
            return std::nullopt;
        }
        if (!r) return std::nullopt;
        if (const auto rg = range_of(xt)) {
            if (big || *r < rg->lo || *r > rg->hi) {
                out_of_range = true;
                if (rg->bits == 64) return *r;   // the wrap of a 64-bit value is the value itself (big: already wrapped)
                const std::int64_t span = std::int64_t{1} << rg->bits;
                std::int64_t v = (*r - rg->lo) % span;
                if (v < 0) v += span;
                return v + rg->lo;
            }
        }
        return r;
    }

private:
    static std::int64_t wrap_add(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b)); }
    static std::int64_t wrap_sub(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) - static_cast<std::uint64_t>(b)); }
    static std::int64_t wrap_mul(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b)); }

    static std::optional<std::int64_t> binary(std::string_view op, std::int64_t av, std::int64_t cv, const TypeP& t, bool& big) {
        constexpr std::int64_t kMin = std::numeric_limits<std::int64_t>::min();
        constexpr std::int64_t kMax = std::numeric_limits<std::int64_t>::max();
        const auto p = prim_of(t);
        const int bits = (p == ir::Prim::I64 || p == ir::Prim::U64) ? 64 : 32;
        if (op == "<<" || op == ">>" || op == ">>>") cv &= bits - 1;
        if (op == "+") {
            big = (cv > 0 && av > kMax - cv) || (cv < 0 && av < kMin - cv);
            return wrap_add(av, cv);
        }
        if (op == "-") {
            big = (cv < 0 && av > kMax + cv) || (cv > 0 && av < kMin + cv);
            return wrap_sub(av, cv);
        }
        if (op == "*") {
            if (av != 0 && cv != 0) {
                big = (av == kMin && cv == -1) || (cv == kMin && av == -1) ||
                      (av != kMin && cv != kMin && std::llabs(av) > kMax / std::llabs(cv));
            }
            return wrap_mul(av, cv);
        }
        if (op == "/") {
            if (cv == 0) return std::nullopt;
            if (av == kMin && cv == -1) {
                big = true;
                return kMin;
            }
            return av / cv;
        }
        if (op == "%") {
            if (cv == 0) return std::nullopt;
            return cv == -1 ? 0 : av % cv;
        }
        if (op == "<<") {
            const std::int64_t r = static_cast<std::int64_t>(static_cast<std::uint64_t>(av) << cv);
            big = cv >= 63 ? av != 0 : (r >> cv) != av;
            return r;
        }
        if (op == ">>") return av >> cv;
        if (op == ">>>") {
            if (bits == 64) {
                const std::uint64_t u = static_cast<std::uint64_t>(av) >> cv;
                big = u > static_cast<std::uint64_t>(kMax);
                return static_cast<std::int64_t>(u);
            }
            const std::int64_t m = ((av % 4294967296LL) + 4294967296LL) % 4294967296LL;
            return m >> cv;
        }
        if (op == "&") return av & cv;
        if (op == "|") return av | cv;
        if (op == "^") return av ^ cv;
        return std::nullopt;   // a comparison or a logical operator: not folded
    }
};

bool overflows(Node* e) {
    Folder f;
    f.value(e);
    return f.out_of_range;
}

// ---- control flow -----------------------------------------------------------------------------------------------------

bool loop_has_break(const std::vector<Node*>& body);

// The unlabelled breaks that leave this loop (not those of nested loops or switches).
void loop_breaks_walk(Node* n, int depth, bool& found) {
    if (const auto* b = ir::get<ir::Break>(n)) {
        if (depth == 0 && !b->label) found = true;
    }
    for (Node* c : ir::children(*n)) {
        const bool nested = ir::is<ir::While>(c) || ir::is<ir::DoWhile>(c) || ir::is<ir::For>(c) || ir::is<ir::ForEach>(c) || ir::is<ir::Switch>(c);
        if (ir::is<ir::Lambda>(c) || ir::is<ir::ClassDecl>(c)) continue;
        loop_breaks_walk(c, depth + (nested ? 1 : 0), found);
    }
}
bool loop_has_break(const std::vector<Node*>& body) {
    bool found = false;
    for (Node* s : body) loop_breaks_walk(s, 0, found);
    return found;
}

bool abrupt(const std::vector<Node*>& stmts);

// Does this statement never fall off its end?
bool abrupt_node(Node* s) {
    if (s == nullptr) return false;
    if (ir::is<ir::Return>(s) || ir::is<ir::Throw>(s) || ir::is<ir::Break>(s) || ir::is<ir::Continue>(s) || ir::is<ir::Goto>(s)) return true;
    if (const auto* b = ir::get<ir::Block>(s)) return abrupt(b->stmts);
    if (const auto* i = ir::get<ir::If>(s)) {
        if (!ir::present(i->orelse)) return false;
        return abrupt(ir::statements(i->then)) && abrupt(ir::statements(i->orelse));
    }
    if (const auto* w = ir::get<ir::While>(s)) {
        const auto* l = ir::get<ir::Lit>(strip(w->cond));
        const auto* flag = l != nullptr ? std::get_if<bool>(&l->value) : nullptr;
        if (flag != nullptr && *flag) return !loop_has_break(ir::statements(w->body));
    }
    return false;
}
bool abrupt(const std::vector<Node*>& stmts) { return !stmts.empty() && abrupt_node(stmts.back()); }

// ---- names ------------------------------------------------------------------------------------------------------------

bool is_name(Node* e, const ir::Sym& s) {
    const auto* n = ir::get<ir::Name>(e);
    return n != nullptr && n->sym && n->sym->name == s.name && n->sym->kind == s.kind;
}
bool uses(Node* e, const ir::Sym& s) {
    for (Node* n : ir::walk(e)) {
        if (is_name(n, s)) return true;
    }
    return false;
}

// A C# object or collection initialiser, hoisted by the front end into `var t = new T(); t.A = 1; t.Add(x); ... S(t)`: what
// the writer puts back as `S(new T { A = 1 })` / `S(new T { x })`.
struct InitItem {
    bool is_set = false;           // `member = value` (else a collection `Add`)
    ir::SymP sym;
    Node* value = nullptr;         // the value set
    std::vector<Node*> args;       // the arguments of the Add
};
struct InitGroup {
    Node* made = nullptr;          // the `new T()`
    std::vector<InitItem> items;
};

bool interpolatable(const std::vector<Node*>& parts) {
    bool any_text = false;
    for (Node* p : parts) {
        const auto* l = ir::get<ir::Lit>(p);
        if (l != nullptr && is_text(l->value)) any_text = true;
    }
    if (!any_text) return false;
    for (Node* p : parts) {
        const auto* l = ir::get<ir::Lit>(p);
        if (l != nullptr && is_text(l->value) && contains(std::get<std::string>(l->value), "\n")) return false;
    }
    return true;
}

// Does C# infer this generic call's type arguments from its arguments? (No argument is a lambda or null.)
bool inferable(const ir::Call& c) {
    if (!c.sym || c.sym->params.empty()) return false;
    for (Node* a0 : c.args) {
        Node* a = strip(a0);
        if (ir::is<ir::Lambda>(a) || ir::is<ir::MethodRef>(a)) return false;
        const auto* l = ir::get<ir::Lit>(a);
        if (l != nullptr && is_null(l->value)) return false;
    }
    return true;
}

// The qualified name's namespace: the name without its last segment, which the type's own name is.
std::string drop_suffix(const std::string& qname, const std::string& name) {
    if (name.empty()) return {};
    std::string ns = qname.substr(0, qname.size() - name.size());
    while (!ns.empty() && ns.back() == '.') ns.pop_back();
    return ns;
}

std::pair<std::string, std::string> rsplit_dot(const std::string& s) {
    const std::size_t at = s.rfind('.');
    return {s.substr(0, at), s.substr(at + 1)};
}

bool ends_with_name(const ir::ClassDecl& c) { return c.qname && c.name && c.qname->ends_with(*c.name); }

std::string pass_text(ir::PassMode m) {
    switch (m) {
    case ir::PassMode::Ref: return "ref";
    case ir::PassMode::Out: return "out";
    case ir::PassMode::In: return "in";
    case ir::PassMode::ByValue: break;
    }
    return {};
}

std::string describe(Node* n) { return std::string(ir::kind_name(*n)); }

// The kind and reason an Unsupported node gives.
Decline declined_for(const ir::Unsupported& u) {
    const std::string kind = u.kind.value_or("");
    return Decline(kind, u.raw && !u.raw->empty() ? *u.raw : kind);
}

}  // namespace

// ---- module-level helpers -----------------------------------------------------------------------------------------------

std::string ident(std::string_view name) { return keywords().contains(name) ? "@" + std::string(name) : std::string(name); }

bool is_string(const ir::TypeP& t) {
    const std::string* n = class_of(t);
    return n != nullptr && (*n == "System.String" || *n == "java.lang.String");
}

Node* strip(Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

// ---- Program ------------------------------------------------------------------------------------------------------------------

Program::Program(std::span<Node* const> units, Options options) : units_(units.begin(), units.end()), options_(std::move(options)) {
    for (Node* n : units_) {
        auto* u = ir::get<ir::Unit>(n);
        if (u == nullptr) throw UsageError("the C# back end writes Unit nodes");
        for (Node* c : u->types) index(c);
    }
}

Program::~Program() = default;

void Program::index(Node* class_node) {
    auto* cd = ir::get<ir::ClassDecl>(class_node);
    if (cd == nullptr) return;
    if (cd->qname) classes_[*cd->qname] = cd;
    for (Node* m : cd->members) index(m);
}

ir::ClassDecl* Program::class_named(std::string_view qname) const {
    const auto it = classes_.find(std::string(qname));
    return it == classes_.end() ? nullptr : it->second;
}

ir::ClassDecl* Program::base_of(const ir::ClassDecl& c) const {
    const std::string* name = class_of(c.extends);
    return name != nullptr ? class_named(*name) : nullptr;
}

bool Program::overrides(const ir::ClassDecl& c, const ir::MethodDecl& m) const {
    const auto signature = [](const ir::MethodDecl& md) {
        std::vector<TypeP> sig;
        for (Node* p : md.params) {
            if (const auto* pp = ir::get<ir::Param>(p)) sig.push_back(pp->type);
        }
        return sig;
    };
    const auto mine = signature(m);
    std::set<std::string> seen;
    for (const ir::ClassDecl* b = base_of(c); b != nullptr && b->qname && seen.insert(*b->qname).second; b = base_of(*b)) {
        for (Node* xn : b->members) {
            const auto* x = ir::get<ir::MethodDecl>(xn);
            if (x != nullptr && x->kind == "method" && x->name == m.name && signature(*x) == mine && !has(x->mods, "static") &&
                !has(x->mods, "private"))
                return true;
        }
    }
    return false;
}

Library* Program::library_for(const std::string& lang) {
    if (const auto it = libraries_.find(lang); it != libraries_.end()) return it->second.get();
    const auto f = options_.libraries.find(lang);
    if (f == options_.libraries.end()) return nullptr;
    return libraries_.emplace(lang, f->second()).first->second.get();
}

// ---- Writer ---------------------------------------------------------------------------------------------------------------------

// Writes one unit. Java's scope of a local begins at its declaration, C#'s is its whole block, so a name an inner block
// declares before an outer one declares it is given a number (`declare`).
class Writer final : public Context {
public:
    Writer(Program& prog, Node* unit_node) : prog_(prog), unit_node_(unit_node), u_(*ir::get<ir::Unit>(unit_node)) {
        lang_ = u_.lang && !u_.lang->empty() ? *u_.lang : "java";
        java_ = lang_ == "java";
        csharp_ = lang_ == "csharp";
        lib_ = prog.library_for(lang_);
        scopes_.emplace_back();
    }

    // The unit's file.
    std::string unit_text();

    // ---- Context
    Program& program() override { return prog_; }
    const std::string& lang() const override { return lang_; }
    std::string ctype(const TypeP& t) override;
    Ex expr(Node* e, bool statement = false) override;
    std::string wrap(Node* e, int prec, bool right = false) override;
    std::string coerce(Node* e, const TypeP& t) override;
    void add_using(std::string_view ns) override { usings_.insert(std::string(ns)); }

private:
    // a source language with no library registered is declined where it would be asked
    Library& lib() {
        if (lib_ == nullptr) throw Decline("library", "no library for " + lang_ + " source");
        return *lib_;
    }

    // ---- scopes
    std::string declare(const std::string& name, bool nested = false);
    std::string lookup(const std::string& name) const;
    void push() { scopes_.emplace_back(); }
    void pop() { scopes_.pop_back(); }
    struct Scope {
        Writer& w;
        explicit Scope(Writer& writer) : w(writer) { w.push(); }
        ~Scope() { w.pop(); }
    };
    void note(const std::string& kind, const std::string& why);
    bool shadowed(const std::string& name) const { return locals_.contains(name); }

    // ---- types
    std::string class_name(const TypeP& t, const ir::ClassType& c);
    std::string short_name(const std::string& qname);
    std::string namespace_of(const ir::ClassDecl* c);
    TypeP type_of(Node* e) const;

    // ---- declarations
    Lines class_decl(ir::ClassDecl& c, const std::string& ind);
    Lines class_decl_body(ir::ClassDecl& c, const std::string& ind);
    std::vector<std::string> class_mods(ir::ClassDecl& c);
    std::string tparams_text(const std::vector<std::string>& names, const std::vector<Node*>& tparams);
    std::string where_text(Node* tp);
    Lines member(Node* m, ir::ClassDecl& c, const std::string& ind);
    Lines field(Node* node, ir::ClassDecl& c, const std::string& ind);
    Lines prop(ir::Property& p, const std::string& ind);
    std::string params_text(const std::vector<Node*>& params, bool varargs = false);
    Lines method_decl(Node* node, ir::ClassDecl& c, const std::string& ind);
    Lines method_body(ir::MethodDecl& m, const std::string& ind);
    Lines body_lines(const ir::Suite& body, const std::string& ind, const ir::MethodDecl* m);
    Lines gap(const std::optional<std::string>& head, const Decline& d, const std::string& ind);
    std::string ret_expr(Node* e);

    // ---- statements
    void block_into(Node* b, Lines& out, const std::string& ind);
    void block_stmts(std::vector<Node*> stmts, Lines& out, const std::string& ind);
    void sub(const ir::Suite& s, Lines& out, const std::string& ind);
    void stmt(Node* s, Lines& out, const std::string& ind);
    std::string cond(Node* e);
    std::string case_label(Node* lab, Node* scrutinee);
    void loop_body(Node* loop, const ir::Suite& body, Lines& out, const std::string& ind);
    void for_stmt(const ir::For& s, Node* node, Lines& out, const std::string& ind);
    std::pair<bool, bool> label_uses(const std::vector<Node*>& body, const std::string& label);
    void local_stmt(const ir::Local& s, Lines& out, const std::string& ind);
    void catch_clause(const ir::Catch& c, const TypeP& t, Lines& out, const std::string& ind);
    void using_stmt(std::span<Node* const> res, Node* body, Lines& out, const std::string& ind);
    void try_stmt(const ir::Try& s, Lines& out, const std::string& ind);
    void switch_stmt(const ir::Switch& s, Lines& out, const std::string& ind);
    std::vector<Node*> initializers(const std::vector<Node*>& in);

    // ---- expressions
    Ex dispatch(Node* e);
    Ex call_expr(Node* e, const ir::Call& c);
    Ex literal(const ir::Lit& l, const TypeP& t);
    std::string member_name(const ir::Sym& s);
    std::string args_text(std::span<Node* const> args, const ir::Sym* s);
    std::optional<Ex> null_conditional(const ir::Cond& e);
    std::string interpolate(const std::vector<Node*>& parts);

    Program& prog_;
    Node* unit_node_;
    ir::Unit& u_;
    std::string lang_;
    bool java_ = false;
    bool csharp_ = false;
    Library* lib_ = nullptr;
    std::set<std::string> usings_;
    ir::ClassDecl* cls_ = nullptr;
    ir::MethodDecl* method_ = nullptr;
    std::map<std::string, std::pair<bool, bool>> labels_;                // a Java label -> its uses (break, continue)
    std::unordered_map<const Node*, std::string> loop_labels_;           // a loop -> its label; lookup only
    std::unordered_set<std::string> locals_;
    std::vector<std::unordered_set<std::string>> block_names_;           // the locals each enclosing block declares (all of the block)
    std::vector<std::map<std::string, std::string>> scopes_;             // a local's name -> the name it is written with
    std::unordered_map<const Node*, InitGroup> init_uses_;               // the use of a hoisted initialiser's local -> what it stands for
};

// ---- scopes -------------------------------------------------------------------------------------------------------------

std::string Writer::declare(const std::string& name, bool nested) {
    locals_.insert(name);
    const std::size_t n = block_names_.size();
    const std::size_t enclosing = nested ? n : (n != 0 ? n - 1 : 0);
    bool clash = false;
    for (std::size_t i = 0; i < enclosing; ++i) clash = clash || block_names_[i].contains(name);
    std::string written = name;
    if (clash) {
        std::unordered_set<std::string> used;
        for (const auto& b : block_names_) used.insert(b.begin(), b.end());
        for (int k = 2;; ++k) {
            std::string candidate = name + std::to_string(k);
            if (used.contains(candidate)) continue;
            // (the answer is "is any in scope", so the order of the scopes does not matter)
            const bool in_scope = std::any_of(scopes_.begin(), scopes_.end(), [&](const auto& sc) {
                return std::any_of(sc.begin(), sc.end(), [&](const auto& kv) { return kv.second == candidate; });
            });
            if (!in_scope) {
                written = std::move(candidate);
                break;
            }
        }
    }
    scopes_.back()[name] = written;
    return ident(written);
}

std::string Writer::lookup(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (const auto f = it->find(name); f != it->end()) return ident(f->second);
    }
    return ident(name);
}

void Writer::note(const std::string& kind, const std::string& why) {
    std::string where = cls_ != nullptr ? cls_->qname.value_or("") : u_.path.value_or("");
    if (method_ != nullptr && method_->name && !method_->name->empty()) where += "." + *method_->name;
    prog_.note(Gap{where, kind, why});
}

TypeP Writer::type_of(Node* e) const {
    if (const auto it = init_uses_.find(e); it != init_uses_.end()) return plain_type_of(it->second.made);
    return plain_type_of(e);
}

// ---- types ----------------------------------------------------------------------------------------------------------------

std::string Writer::ctype(const TypeP& t) {
    if (!t) return "void";
    struct Visitor {
        Writer& w;
        const TypeP& t;
        std::string operator()(const ir::PrimType& p) const {
            switch (p.prim) {
            case ir::Prim::Bool: return "bool";
            case ir::Prim::I8: return "sbyte";
            case ir::Prim::U8: return "byte";
            case ir::Prim::I16: return "short";
            case ir::Prim::I32: return "int";
            case ir::Prim::U32: return "uint";
            case ir::Prim::I64: return "long";
            case ir::Prim::U64: return "ulong";
            case ir::Prim::F32: return "float";
            case ir::Prim::F64: return "double";
            case ir::Prim::Char: return "byte";   // C's plain char
            case ir::Prim::U16: return p.encoding || w.java_ ? "char" : "ushort";
            case ir::Prim::F80: break;
            }
            throw Decline("type", "the type " + ir::to_text(*t));
        }
        std::string operator()(const ir::VoidType&) const { return "void"; }
        std::string operator()(const ir::NullType&) const { return "object"; }
        std::string operator()(const ir::DynamicType&) const { return "dynamic"; }
        std::string operator()(const ir::TypeVarType& v) const { return ident(v.name); }
        std::string operator()(const ir::WildcardType& wc) const {
            if (wc.bound && wc.bound->mode == ir::BoundMode::Extends) return w.ctype(wc.bound->type);
            return "object";
        }
        std::string operator()(const ir::ArrayType& a) const {
            const auto brackets = [](std::uint32_t rank) { return "[" + std::string(rank > 1 ? rank - 1 : 0, ',') + "]"; };
            std::string suffix = brackets(a.rank);
            TypeP inner = a.elem;
            while (const auto* ia = inner ? std::get_if<ir::ArrayType>(&inner->body) : nullptr) {
                suffix += brackets(ia->rank);
                inner = ia->elem;
            }
            return w.ctype(inner) + suffix;
        }
        std::string operator()(const ir::ClassType& c) const {
            if (w.java_) {
                if (auto got = w.lib().type_name(w, t)) return *got;
            }
            return w.class_name(t, c);
        }
        std::string operator()(const ir::AnyType&) const { return other(); }
        std::string operator()(const ir::PtrType&) const { return other(); }
        std::string operator()(const ir::SelectorType&) const { return other(); }
        std::string operator()(const ir::FnType&) const { return other(); }
        std::string operator()(const ir::FnPtrType&) const { return other(); }
        std::string operator()(const ir::RecordType&) const { return other(); }
        [[noreturn]] std::string other() const { throw Decline("type", "the type " + ir::to_text(*t)); }
    };
    return std::visit(Visitor{*this, t}, t->body);
}

std::string Writer::class_name(const TypeP&, const ir::ClassType& c) {
    const std::string suffix = nullable(c) ? "?" : "";
    if (c.args.empty()) {
        if (const auto al = alias_table().find(c.name); al != alias_table().end()) return std::string(al->second) + suffix;
    }
    if (c.name == "System.Nullable" && c.args.size() == 1) return ctype(c.args[0]) + "?";
    std::string simple = short_name(c.name);
    if (!c.args.empty()) {
        Lines parts;
        for (const TypeP& a : c.args) parts.push_back(ctype(a));
        simple += "<" + text::join(parts, ", ") + ">";
    }
    return simple + suffix;
}

// A type's name as written in this unit: relative to its namespace, with a using for it.
std::string Writer::short_name(const std::string& qname) {
    if (ir::ClassDecl* c = prog_.class_named(qname)) {
        std::vector<ir::ClassDecl*> chain{c};
        while (chain.back()->outer) {
            ir::ClassDecl* o = prog_.class_named(*chain.back()->outer);
            if (o == nullptr) break;
            chain.push_back(o);
        }
        const ir::ClassDecl& top = *chain.back();
        const std::string ns = ends_with_name(top) ? drop_suffix(*top.qname, *top.name) : std::string();
        Lines relp;
        for (auto it = chain.rbegin(); it != chain.rend(); ++it) relp.push_back(ident((*it)->name.value_or("")));
        const std::string rel = text::join(relp, ".");
        if (!ns.empty() && ns != namespace_of(cls_)) usings_.insert(ns);
        // inside the class itself or its outer classes, the nested name alone
        for (const ir::ClassDecl* cur = cls_; cur != nullptr; cur = cur->outer ? prog_.class_named(*cur->outer) : nullptr) {
            if (c->outer && cur->qname && *cur->qname == *c->outer) return ident(c->name.value_or(""));
        }
        return rel;
    }
    if (contains(qname, ".")) {
        const auto [owner, name] = rsplit_dot(qname);
        if (prog_.class_named(owner) != nullptr) return short_name(owner) + "." + ident(name);
        usings_.insert(owner);
        return ident(name);
    }
    return ident(qname);
}

std::string Writer::namespace_of(const ir::ClassDecl* c0) {
    if (c0 == nullptr) return u_.package.value_or("");
    const ir::ClassDecl* c = c0;
    while (c->outer && prog_.class_named(*c->outer) != nullptr) c = prog_.class_named(*c->outer);
    return ends_with_name(*c) ? drop_suffix(*c->qname, *c->name) : std::string();
}

// ---- the unit ---------------------------------------------------------------------------------------------------------------

std::string Writer::unit_text() {
    // consecutive classes of one namespace share its braces
    std::vector<std::pair<std::string, std::vector<Node*>>> groups;
    for (Node* c : u_.types) {
        const auto* cd = ir::get<ir::ClassDecl>(c);
        const std::string ns = cd != nullptr ? namespace_of(cd) : std::string();
        if (!groups.empty() && groups.back().first == ns) groups.back().second.push_back(c);
        else groups.emplace_back(ns, std::vector<Node*>{c});
    }
    Lines body;
    for (auto& [ns, nodes] : groups) {
        Lines lines;
        for (Node* c : nodes) {
            auto* cd = ir::get<ir::ClassDecl>(c);
            try {
                if (cd == nullptr) throw Decline("member", "a unit member " + describe(c));
                extend(lines, class_decl(*cd, ""));
            } catch (const Decline& d) {
                note(d.decline_kind(), d.why());
                lines.push_back("// CB-GAP: " + (cd != nullptr ? cd->name.value_or("") : std::string("?")) + " was not translated: " + d.why());
            }
            lines.emplace_back();
        }
        while (!lines.empty() && lines.back().empty()) lines.pop_back();
        if (!ns.empty()) {
            body.push_back("namespace " + ns);
            body.emplace_back("{");
            for (const std::string& l : lines) body.push_back(l.empty() ? std::string() : "    " + l);
            body.emplace_back("}");
        } else {
            extend(body, lines);
        }
        body.emplace_back();
    }
    std::set<std::string> own;
    for (Node* c : u_.types) {
        if (const auto* cd = ir::get<ir::ClassDecl>(c)) own.insert(namespace_of(cd));
    }
    Lines head;
    const std::string path_name = u_.path && !u_.path->empty() ? *u_.path : "?";
    head.push_back("// Translated by CodeBreaker from " + fs::to_text(fs::from_text(path_name).filename()) + " (" + lang_ + ").");
    if (java_) head.emplace_back("#nullable disable");
    for (const std::string& x : usings_) {   // (a set: sorted)
        if (x.empty() || own.contains(x)) continue;
        const bool parent_of_own = std::any_of(u_.types.begin(), u_.types.end(), [&](Node* c) {
            const auto* cd = ir::get<ir::ClassDecl>(c);
            return cd != nullptr && namespace_of(cd).starts_with(x + ".");
        });
        if (!parent_of_own) head.push_back("using " + x + ";");
    }
    head.emplace_back();
    extend(head, body);
    return std::string(text::rtrim(text::join(head, "\n"))) + "\n";
}

// ---- declarations -------------------------------------------------------------------------------------------------------------

Lines Writer::class_decl(ir::ClassDecl& c, const std::string& ind) {
    struct Restore {
        ir::ClassDecl*& slot;
        ir::ClassDecl* saved;
        ~Restore() { slot = saved; }
    } restore{cls_, cls_};
    cls_ = &c;
    return class_decl_body(c, ind);
}

std::vector<std::string> Writer::class_mods(ir::ClassDecl& c) {
    if (java_) return lib().class_mods(*this, c);
    std::vector<std::string> out;
    for (const std::string& m : c.mods) {
        if (m != "IMPLICIT") out.push_back(m);
    }
    return out;
}

std::string Writer::tparams_text(const std::vector<std::string>& names, const std::vector<Node*>& tparams) {
    if (names.empty()) return {};
    Lines out;
    for (const std::string& n : names) {
        std::string variance;
        for (Node* tp : tparams) {
            const auto* t = ir::get<ir::TypeParam>(tp);
            if (t == nullptr || t->name != n || !t->variance) continue;
            if (*t->variance == "Out") variance = "out ";
            else if (*t->variance == "In") variance = "in ";
        }
        out.push_back(variance + ident(n));
    }
    return "<" + text::join(out, ", ") + ">";
}

std::string Writer::where_text(Node* tpn) {
    const auto* tp = ir::get<ir::TypeParam>(tpn);
    if (tp == nullptr) return {};
    Lines parts;
    if (has(tp->flags, "class")) parts.emplace_back("class");
    if (has(tp->flags, "struct")) parts.emplace_back("struct");
    for (const TypeP& b : tp->bounds) {
        const std::string* n = class_of(b);
        if (n != nullptr && (*n == "java.lang.Object" || *n == "System.Object")) continue;
        parts.push_back(ctype(b));
    }
    if (has(tp->flags, "new")) parts.emplace_back("new()");
    if (parts.empty()) return {};
    return "where " + ident(tp->name.value_or("")) + " : " + text::join(parts, ", ");
}

Lines Writer::class_decl_body(ir::ClassDecl& c, const std::string& ind) {
    const std::string ckind = c.kind.value_or("");
    if (ckind == "delegate") {
        ir::MethodDecl* invoke = nullptr;
        for (Node* m : c.members) {
            if ((invoke = ir::get<ir::MethodDecl>(m)) != nullptr) break;
        }
        if (invoke == nullptr) throw Decline("delegate", "a delegate with no Invoke method");
        return {ind + mods_text(class_mods(c)) + "delegate " + ctype(invoke->ret) + " " + ident(c.name.value_or("")) +
                tparams_text(c.typeparams, c.tparams) + "(" + params_text(invoke->params) + ");"};
    }
    std::string_view kind;
    for (const std::string_view k : {"class", "interface", "enum", "struct", "record", "record struct"}) {
        if (ckind == k) kind = k;
    }
    if (kind.empty()) throw Decline("class-kind", "a " + c.kind.value_or(""));
    if (java_) lib().check_class(*this, c);
    const std::vector<std::string> mods = class_mods(c);
    std::string head = ind + mods_text(mods) + std::string(kind) + " " + ident(c.name.value_or("")) + tparams_text(c.typeparams, c.tparams);
    ir::MethodDecl* primary = nullptr;
    Node* primary_node = nullptr;
    for (Node* m : c.members) {
        auto* md = ir::get<ir::MethodDecl>(m);
        if (md != nullptr && has(md->mods, "primary")) {
            primary = md;
            primary_node = m;
            break;
        }
    }
    std::set<std::string> positional;
    if (primary != nullptr) {
        head += "(" + params_text(primary->params) + ")";
        for (Node* p : primary->params) {
            const auto* pp = ir::get<ir::Param>(p);
            if (pp != nullptr && pp->sym) positional.insert(pp->sym->name);
        }
    }
    Lines bases;
    if (ckind == "enum") {
        if (c.extends) bases.push_back(ctype(c.extends));
    } else {
        if (c.extends) {
            std::string b = ctype(c.extends);
            if (primary != nullptr) {
                // the base class takes the arguments of the primary constructor's base call
                ir::CtorCall* call = nullptr;
                for (Node* s : ir::statements(primary->body)) {
                    const auto* blk = ir::get<ir::Block>(s);
                    for (Node* st : blk != nullptr ? blk->stmts : std::vector<Node*>{s}) {
                        auto* cc = ir::get<ir::CtorCall>(st);
                        if (cc != nullptr && !(cc->kind && cc->kind->ends_with(":implicit"))) {
                            call = cc;
                            break;
                        }
                    }
                    if (call != nullptr) break;
                }
                if (call != nullptr) b += "(" + args_text(call->args, call->sym.get()) + ")";
            }
            bases.push_back(b);
        }
        for (const TypeP& i : c.implements) {
            if (!java_ || lib().keep_interface(i)) bases.push_back(ctype(i));
        }
    }
    if (!bases.empty()) head += " : " + text::join(bases, ", ");
    Lines lines{head};
    for (Node* tp : c.tparams) {
        const std::string w = where_text(tp);
        if (!w.empty()) lines.push_back(ind + "    " + w);
    }
    std::vector<Node*> members;
    for (Node* m : c.members) {
        const auto* pr = ir::get<ir::Property>(m);
        if (pr != nullptr && has(pr->mods, "positional") && pr->sym && positional.contains(pr->sym->name)) continue;
        if (primary_node != nullptr && m == primary_node) continue;
        members.push_back(m);
    }
    if (primary != nullptr && members.empty()) {
        lines.back() += ";";
        return lines;
    }
    lines.push_back(ind + "{");
    const std::string inner = ind + "    ";
    if (ckind == "enum") {
        std::vector<Node*> consts;
        std::vector<Node*> rest;
        for (Node* m : members) (ir::is<ir::EnumConst>(m) ? consts : rest).push_back(m);
        for (std::size_t i = 0; i < consts.size(); ++i) {
            const auto* e = ir::get<ir::EnumConst>(consts[i]);
            std::string line = inner + ident(e->sym->name);
            if (e->value) line += " = " + std::to_string(*e->value);
            else if (!e->args.empty() || e->body != nullptr) throw Decline("enum", "an enum constant with constructor arguments or a body");
            lines.push_back(line + (i + 1 < consts.size() ? "," : ""));
        }
        members = std::move(rest);
        if (!members.empty() && !java_) throw Decline("enum", "an enum with members");
    }
    if (java_) members = lib().arrange_members(*this, c, std::move(members));
    bool first = true;
    for (Node* m : members) {
        const Lines written = member(m, c, inner);
        if (written.empty()) continue;
        if (!first && (written.size() > 1 || ir::is<ir::MethodDecl>(m) || ir::is<ir::ClassDecl>(m) || ir::is<ir::Property>(m))) lines.emplace_back();
        extend(lines, written);
        first = false;
    }
    lines.push_back(ind + "}");
    return lines;
}

Lines Writer::member(Node* m, ir::ClassDecl& c, const std::string& ind) {
    if (auto* cd = ir::get<ir::ClassDecl>(m)) return class_decl(*cd, ind);
    if (ir::is<ir::FieldDecl>(m)) return field(m, c, ind);
    if (auto* p = ir::get<ir::Property>(m)) return prop(*p, ind);
    if (ir::is<ir::MethodDecl>(m)) return method_decl(m, c, ind);
    if (const auto* un = ir::get<ir::Unsupported>(m)) {
        const std::string why = un->raw && !un->raw->empty() ? *un->raw : un->kind.value_or("");
        note(un->kind.value_or(""), why);
        return {ind + "// CB-GAP: " + why};
    }
    if (ir::is<ir::EnumConst>(m)) throw Decline("enum", "an enum constant outside an enum");
    throw Decline("member", "a member " + describe(m));
}

Lines Writer::field(Node* node, ir::ClassDecl& c, const std::string& ind) {
    auto& f = *ir::get<ir::FieldDecl>(node);
    const std::vector<std::string> mods = java_ ? lib().field_mods(*this, f, c) : f.mods;
    std::string line = ind + mods_text(mods) + ctype(f.type) + " " + ident(f.sym->name);
    Node* init = prog_.has_init_override(node) ? prog_.init_override(node) : f.init;
    if (init != nullptr) {
        method_ = nullptr;
        try {
            line += " = " + coerce(init, f.type);
        } catch (const Decline& d) {
            note(d.decline_kind(), d.why());
            line += " = default /* CB-GAP: " + d.why() + " */";
        }
    }
    return {line + ";"};
}

Lines Writer::prop(ir::Property& p, const std::string& ind) {
    std::vector<std::string> mods;
    for (const std::string& m : p.mods) {
        if (m != "positional") mods.push_back(m);
    }
    const std::string name = !p.params.empty() ? "this[" + params_text(p.params) + "]" : ident(p.sym->name);
    const std::string head = ind + mods_text(mods) + ctype(p.type) + " " + name;
    const auto accessor = [](Node* a) -> ir::MethodDecl& { return *ir::get<ir::MethodDecl>(a); };
    const auto body_stmts = [](ir::MethodDecl& a) {
        std::vector<Node*> out;
        for (Node* s : ir::statements(a.body)) {
            if (const auto* blk = ir::get<ir::Block>(s)) out.insert(out.end(), blk->stmts.begin(), blk->stmts.end());
            else out.push_back(s);
        }
        return out;
    };
    // a lone getter that returns an expression: `T P => expr;`
    if (p.accessors.size() == 1) {
        ir::MethodDecl& a0 = accessor(p.accessors[0]);
        if (a0.kind == "get" && ir::present(a0.body) && a0.mods.empty()) {
            const auto b = body_stmts(a0);
            if (b.size() == 1) {
                const auto* r = ir::get<ir::Return>(b[0]);
                if (r != nullptr && r->expr != nullptr) {
                    try {
                        method_ = &a0;
                        return {head + " => " + expr(r->expr).text + ";"};
                    } catch (const Decline&) {
                        // (written the long way below)
                    }
                }
            }
        }
    }
    const bool all_bodyless = std::none_of(p.accessors.begin(), p.accessors.end(), [&](Node* a) { return ir::present(accessor(a).body); });
    if (all_bodyless) {
        Lines parts;
        for (Node* a : p.accessors) parts.push_back(mods_text(accessor(a).mods) + accessor(a).kind.value_or("") + ";");
        std::string line = head + " { " + text::join(parts, " ") + " }";
        if (p.init != nullptr) {
            method_ = nullptr;
            line += " = " + coerce(p.init, p.type) + ";";
        }
        return {line};
    }
    Lines lines{head, ind + "{"};
    for (Node* an : p.accessors) {
        ir::MethodDecl& a = accessor(an);
        const std::string amods = mods_text(a.mods);
        if (!ir::present(a.body)) {
            lines.push_back(ind + "    " + amods + a.kind.value_or("") + ";");
            continue;
        }
        method_ = &a;
        const Lines body = method_body(a, ind + "    ");
        const auto st = body_stmts(a);
        const auto* ret = st.size() == 1 ? ir::get<ir::Return>(st[0]) : nullptr;
        if (ret != nullptr && ret->expr != nullptr && body.size() == 3) {
            // `get => expr;`: the middle line is `return expr;`
            lines.push_back(ind + "    " + amods + a.kind.value_or("") + " => " + std::string(text::trim(body[1])).substr(7));
        } else if (st.size() == 1 && ir::is<ir::ExprStmt>(st[0]) && body.size() == 3) {
            lines.push_back(ind + "    " + amods + a.kind.value_or("") + " => " + std::string(text::trim(body[1])));
        } else {
            lines.push_back(ind + "    " + amods + a.kind.value_or(""));
            extend(lines, body);
        }
    }
    lines.push_back(ind + "}");
    if (p.init != nullptr) lines.back() += " = " + coerce(p.init, p.type) + ";";
    return lines;
}

std::string Writer::params_text(const std::vector<Node*>& params, bool varargs) {
    Lines out;
    for (std::size_t i = 0; i < params.size(); ++i) {
        const auto* p = ir::get<ir::Param>(params[i]);
        if (p == nullptr) throw Decline("param", "a parameter " + describe(params[i]));
        std::vector<std::string> mods;
        for (const std::string& m : p->mods) {
            if (m == "ref" || m == "out" || m == "in" || m == "params" || m == "this" || m == "scoped") mods.push_back(m);
        }
        if (varargs && i + 1 == params.size() && !has(mods, "params")) mods.emplace_back("params");
        if (p->sym && !p->sym->refs.empty()) {
            const std::string pass = pass_text(p->sym->refs[0]);
            if (!pass.empty() && !has(mods, pass)) mods.insert(mods.begin(), pass);
        }
        out.push_back(mods_text(mods) + ctype(p->type) + " " + ident(p->sym ? p->sym->name : "p"));
    }
    return text::join(out, ", ");
}

Lines Writer::method_decl(Node* node, ir::ClassDecl& c, const std::string& ind) {
    auto& m = *ir::get<ir::MethodDecl>(node);
    method_ = &m;
    labels_.clear();
    const std::string mkind = m.kind.value_or("");
    const std::string cname = ident(c.name.value_or(""));
    if (mkind == "static_init") {
        if (java_) return {};   // (folded into the static constructor)
        Lines r{ind + "static " + cname + "()"};
        extend(r, method_body(m, ind));
        return r;
    }
    if (mkind == "init") return {};   // (Java's instance blocks run in each constructor)
    if (mkind == "cctor") {           // Java's statics, in their one textual order
        Lines r{ind + "static " + cname + "()"};
        extend(r, method_body(m, ind));
        return r;
    }
    const bool varargs = java_ && m.sym && m.sym->varargs;
    if (mkind == "ctor") {
        if (has(m.mods, "IMPLICIT") && !(java_ && lib().ctor_needed(*this, c, m))) return {};
        std::vector<std::string> mods;
        if (java_) {
            mods = lib().method_mods(*this, m, c);
        } else {
            for (const std::string& x : m.mods) {
                if (x != "IMPLICIT") mods.push_back(x);
            }
        }
        std::string head = ind + mods_text(mods) + cname + "(" + params_text(m.params, varargs) + ")";
        std::vector<Node*> stmts;
        for (Node* s : ir::statements(m.body)) {
            if (const auto* blk = ir::get<ir::Block>(s)) stmts.insert(stmts.end(), blk->stmts.begin(), blk->stmts.end());
            else stmts.push_back(s);
        }
        ir::CtorCall* cc = stmts.empty() ? nullptr : ir::get<ir::CtorCall>(stmts[0]);
        if (cc != nullptr) {
            stmts.erase(stmts.begin());
            if (!(cc->kind && cc->kind->ends_with(":implicit")) && cc->target == nullptr) {
                try {
                    head += std::string(" : ") + (cc->kind == "this" ? "this" : "base") + "(" + args_text(cc->args, cc->sym.get()) + ")";
                } catch (const Decline& d) {
                    return gap(head, d, ind);
                }
            }
        }
        std::vector<Node*> all;
        if (java_) all = lib().ctor_prologue(*this, c, m, cc);
        all.insert(all.end(), stmts.begin(), stmts.end());
        Lines r{head};
        extend(r, body_lines(ir::Suite(std::move(all)), ind, &m));
        return r;
    }
    const std::vector<std::string> mods = java_ ? lib().method_mods(*this, m, c) : m.mods;
    const std::string name = java_ ? lib().method_name(*this, m, c) : ident(m.name.value_or(""));
    std::string head;
    const std::string mname = m.name.value_or("");
    const auto drop_operator = [&] {
        std::vector<std::string> rest;
        for (const std::string& x : mods) {
            if (x != "operator") rest.push_back(x);
        }
        return rest;
    };
    if (has(mods, "operator") && declared_operators().contains(mname)) {
        head = ind + mods_text(drop_operator()) + ctype(m.ret) + " operator " + std::string(declared_operators().at(mname));
    } else if (has(mods, "operator") && (mname == "op_Implicit" || mname == "op_Explicit")) {
        head = ind + mods_text(drop_operator()) + (mname == "op_Implicit" ? "implicit" : "explicit") + " operator " + ctype(m.ret);
    } else {
        const std::string rt = ctype(m.ret);
        head = ind + mods_text(mods) + rt + " " + name + tparams_text(m.typeparams, m.tparams);
    }
    head += "(" + params_text(m.params, varargs) + ")";
    Lines wheres;
    for (Node* tp : m.tparams) {
        const std::string w = where_text(tp);
        if (!w.empty()) wheres.push_back(w);
    }
    if (!ir::present(m.body)) return {head + (wheres.empty() ? std::string() : " " + text::join(wheres, " ")) + ";"};
    const auto st = ir::statements(m.body);
    // a lone `return expr;` of a C# method is an expression body
    if (csharp_ && wheres.empty() && !st.empty()) {
        const auto* blk = ir::get<ir::Block>(st[0]);
        const std::vector<Node*>& stmts = blk != nullptr ? blk->stmts : st;
        if (stmts.size() == 1) {
            const auto* ret = ir::get<ir::Return>(stmts[0]);
            if (ret != nullptr && ret->expr != nullptr) {
                try {
                    return {head + " => " + ret_expr(ret->expr) + ";"};
                } catch (const Decline&) {
                    // (written the long way below)
                }
            }
        }
    }
    Lines lines{head};
    for (const std::string& w : wheres) lines.push_back(ind + "    " + w);
    extend(lines, method_body(m, ind));
    return lines;
}

std::string Writer::ret_expr(Node* e) { return coerce(e, method_ != nullptr ? method_->ret : TypeP()); }

Lines Writer::method_body(ir::MethodDecl& m, const std::string& ind) { return body_lines(m.body, ind, &m); }

Lines Writer::body_lines(const ir::Suite& body, const std::string& ind, const ir::MethodDecl* m) {
    for (Node* s : ir::statements(body)) {
        const auto un = ir::unsupported(s);
        if (!un.empty()) return gap(std::nullopt, declined_for(*ir::get<ir::Unsupported>(un[0])), ind);
    }
    try {
        Lines out;
        locals_.clear();
        if (m != nullptr) {
            for (Node* p : m->params) {
                const auto* pp = ir::get<ir::Param>(p);
                if (pp != nullptr && pp->sym) locals_.insert(pp->sym->name);
            }
        }
        block_names_.clear();
        block_names_.push_back(locals_);
        scopes_.clear();
        scopes_.emplace_back();
        sub(body, out, ind);
        return out;
    } catch (const Decline& d) {
        return gap(std::nullopt, d, ind);
    }
}

Lines Writer::gap(const std::optional<std::string>& head, const Decline& d, const std::string& ind) {
    note(d.decline_kind(), d.why());
    Lines lines;
    if (head && !head->empty()) lines.push_back(*head);
    lines.push_back(ind + "{");
    lines.push_back(ind + "    throw new System.NotSupportedException(" + cstr("CB-GAP: " + d.why()) + ");");
    lines.push_back(ind + "}");
    return lines;
}

// ---- statements -----------------------------------------------------------------------------------------------------------

void Writer::block_into(Node* b, Lines& out, const std::string& ind) {
    if (const auto* blk = ir::get<ir::Block>(b)) block_stmts(blk->stmts, out, ind);
    else block_stmts(b != nullptr ? std::vector<Node*>{b} : std::vector<Node*>{}, out, ind);
}

void Writer::block_stmts(std::vector<Node*> stmts, Lines& out, const std::string& ind) {
    out.push_back(ind + "{");
    if (csharp_) stmts = initializers(stmts);
    std::unordered_set<std::string> names;
    for (Node* x : stmts) {
        if (const auto* l = ir::get<ir::Local>(x)) {
            if (l->sym) names.insert(l->sym->name);
        }
    }
    block_names_.push_back(std::move(names));
    struct Pop {
        Writer& w;
        ~Pop() {
            w.pop();
            w.block_names_.pop_back();
        }
    } pop_on_exit{(push(), *this)};
    for (Node* s : stmts) stmt(s, out, ind + "    ");
    out.push_back(ind + "}");
}

// A statement in a place that holds one (if, loop): always a block, for clarity.
void Writer::sub(const ir::Suite& s, Lines& out, const std::string& ind) {
    if (const auto* one = std::get_if<Node*>(&s)) block_into(*one, out, ind);
    else block_stmts(std::get<std::vector<Node*>>(s), out, ind);
}

std::string Writer::cond(Node* e) { return coerce(strip(e), ir::bool_type()); }   // (the parentheses an if or a loop writes anyway)

// A loop's body, with the label a Java `continue label` jumps to at its end.
void Writer::loop_body(Node* loop, const ir::Suite& body, Lines& out, const std::string& ind) {
    const auto it = loop_labels_.find(loop);
    bool has_continue = false;
    if (it != loop_labels_.end()) {
        const auto lb = labels_.find(it->second);
        has_continue = lb != labels_.end() && lb->second.second;
    }
    if (!has_continue) {
        sub(body, out, ind);
        return;
    }
    std::vector<Node*> stmts;
    for (Node* s : ir::statements(body)) {
        if (const auto* blk = ir::get<ir::Block>(s); blk != nullptr && std::holds_alternative<Node*>(body)) stmts.insert(stmts.end(), blk->stmts.begin(), blk->stmts.end());
        else stmts.push_back(s);
    }
    out.push_back(ind + "{");
    for (Node* x : stmts) stmt(x, out, ind + "    ");
    out.push_back(ind + it->second + "_continue:;");
    out.push_back(ind + "}");
}

void Writer::for_stmt(const ir::For& s, Node* node, Lines& out, const std::string& ind) {
    struct Init {
        std::optional<std::string> type;   // a declaration's type; none for an expression
        std::string text;
    };
    std::vector<Init> inits;
    for (Node* i : s.init) {
        if (const auto* l = ir::get<ir::Local>(i)) {
            std::optional<std::string> v;
            if (l->init != nullptr) v = coerce(l->init, l->type);
            std::string t = ctype(l->type);
            const std::string d = declare(l->sym->name, true);
            inits.push_back({std::move(t), d + (v ? " = " + *v : std::string())});
        } else if (const auto* es = ir::get<ir::ExprStmt>(i)) {
            inits.push_back({std::nullopt, expr(es->expr, true).text});
        } else {
            throw Decline("for", "a for initialiser " + describe(i));
        }
    }
    Lines texts;
    for (const Init& i : inits) texts.push_back(i.text);
    std::string init;
    if (!inits.empty() && inits[0].type) {
        for (const Init& i : inits) {
            if (!i.type) break;
            if (*i.type != *inits[0].type) throw Decline("for", "a for initialiser declaring locals of several types");
        }
        init = *inits[0].type + " " + text::join(texts, ", ");
    } else {
        for (const Init& i : inits) {
            if (i.type) throw Decline("for", "a for initialiser mixing declarations and expressions");
        }
        init = text::join(texts, ", ");
    }
    const std::string c = s.cond != nullptr ? cond(s.cond) : std::string();
    Lines ups;
    for (Node* u : s.update) ups.push_back(expr(u, true).text);
    out.push_back(ind + "for (" + init + "; " + c + "; " + text::join(ups, ", ") + ")");
    loop_body(node, s.body, out, ind);
}

std::pair<bool, bool> Writer::label_uses(const std::vector<Node*>& body, const std::string& label) {
    std::pair<bool, bool> found{false, false};
    for (Node* root : body) {
        for (Node* n : ir::walk(root)) {
            if (const auto* b = ir::get<ir::Break>(n); b != nullptr && b->label == label) found.first = true;
            if (const auto* c = ir::get<ir::Continue>(n); c != nullptr && c->label == label) found.second = true;
        }
    }
    return found;
}

void Writer::local_stmt(const ir::Local& s, Lines& out, const std::string& ind) {
    const std::string mods = has(s.mods, "const") && csharp_ ? "const " : "";
    if (s.init == nullptr) {
        const std::string name = declare(s.sym->name);
        out.push_back(ind + mods + ctype(s.type) + " " + name + ";");
        return;
    }
    const std::string init = coerce(s.init, s.type);
    const std::string name = declare(s.sym->name);
    if (java_) lib().record(*this, s.sym, s.init);
    const std::string tn = ctype(s.type);
    Node* si = strip(s.init);
    const bool made = ir::is<ir::New>(si) || init_uses_.contains(si);
    if (made && type_of(s.init) == s.type && mods.empty()) out.push_back(ind + "var " + name + " = " + init + ";");
    else out.push_back(ind + mods + tn + " " + name + " = " + init + ";");
}

void Writer::catch_clause(const ir::Catch& c, const TypeP& t, Lines& out, const std::string& ind) {
    std::optional<std::string> tn;
    if (t) tn = ctype(t);
    if (!tn) {
        out.push_back(ind + "catch");
        block_into(c.body, out, ind);
    } else if (c.sym) {
        Scope scope(*this);
        const std::string d = declare(c.sym->name, true);
        out.push_back(ind + "catch (" + *tn + " " + d + ")");
        block_into(c.body, out, ind);
    } else {
        out.push_back(ind + "catch (" + *tn + ")");
        block_into(c.body, out, ind);
    }
}

void Writer::using_stmt(std::span<Node* const> res, Node* body, Lines& out, const std::string& ind) {
    Node* r = res[0];
    std::string head;
    const auto* loc = ir::get<ir::Local>(r);
    std::optional<Scope> scope;
    if (loc != nullptr) {
        const std::string init = coerce(loc->init, loc->type);
        scope.emplace(*this);
        Node* si = strip(loc->init);
        const std::string tn = ir::is<ir::New>(si) && type_of(loc->init) == loc->type ? "var" : ctype(loc->type);
        const std::string d = declare(loc->sym->name, true);
        head = "using (" + tn + " " + d + " = " + init + ")";
    } else if (const auto* es = ir::get<ir::ExprStmt>(r)) {
        head = "using (" + expr(es->expr).text + ")";
    } else {
        throw Decline("using", "a resource " + describe(r));
    }
    out.push_back(ind + head);
    if (res.size() > 1) {
        out.push_back(ind + "{");
        using_stmt(res.subspan(1), body, out, ind + "    ");
        out.push_back(ind + "}");
    } else {
        block_into(body, out, ind);
    }
}

void Writer::try_stmt(const ir::Try& s, Lines& out, const std::string& ind) {
    if (!s.resources.empty() && s.catches.empty() && s.finally_body == nullptr) {
        using_stmt(s.resources, s.body, out, ind);
        return;
    }
    out.push_back(ind + "try");
    if (!s.resources.empty()) {
        out.push_back(ind + "{");
        using_stmt(s.resources, s.body, out, ind + "    ");
        out.push_back(ind + "}");
    } else {
        block_into(s.body, out, ind);
    }
    for (Node* cn : s.catches) {
        const auto* c = ir::get<ir::Catch>(cn);
        if (c->types.size() > 1) {
            // a multi-catch: one clause per type, each with the same body
            for (const TypeP& t : c->types) catch_clause(*c, t, out, ind);
        } else {
            catch_clause(*c, c->types.empty() ? TypeP() : c->types[0], out, ind);
        }
    }
    if (s.finally_body != nullptr) {
        out.push_back(ind + "finally");
        block_into(s.finally_body, out, ind);
    }
}

std::string Writer::case_label(Node* lab0, Node* scrutinee) {
    Node* lab = strip(lab0);
    if (const auto* sel = ir::get<ir::Select>(lab)) {
        if (sel->sym && sel->sym->kind == ir::SymKind::EnumConst) {
            return ctype(ir::class_type(sel->sym->owner.value_or(""))) + "." + ident(sel->sym->name);
        }
    }
    if (const auto* l = ir::get<ir::Lit>(lab)) {
        if (const auto* iv = std::get_if<std::int64_t>(&l->value)) {
            if (scrutinee != nullptr && is_prim(type_of(scrutinee), ir::Prim::U16)) return cchar_cp(static_cast<char32_t>(*iv));
        }
    }
    return expr(lab).text;
}

void Writer::switch_stmt(const ir::Switch& s, Lines& out, const std::string& ind) {
    out.push_back(ind + "switch (" + expr(s.expr).text + ")");
    out.push_back(ind + "{");
    for (std::size_t i = 0; i < s.cases.size(); ++i) {
        const auto* c = ir::get<ir::Case>(s.cases[i]);
        for (Node* lab : c->labels) out.push_back(ind + "    case " + case_label(lab, s.expr) + ":");
        if (c->is_default) out.push_back(ind + "    default:");
        if (c->body.empty() && i + 1 < s.cases.size()) continue;   // labels sharing the next section
        const std::string inner = ind + "        ";
        for (Node* x : c->body) stmt(x, out, inner);
        if (abrupt(c->body)) continue;
        if (i + 1 >= s.cases.size()) {
            out.push_back(inner + "break;");
        } else {
            const auto* next = ir::get<ir::Case>(s.cases[i + 1]);
            if (!next->labels.empty()) out.push_back(inner + "goto case " + case_label(next->labels[0], s.expr) + ";");
            else out.push_back(inner + "goto default;");
        }
    }
    out.push_back(ind + "}");
}

// var t = new T(); t.A = 1; t.Add(x); ... S(t)  ->  S(new T { A = 1 }) / S(new T { x }): the statements a C# object or
// collection initialiser was hoisted into, put back as the initialiser. The tree is not changed: the one use of `t` is
// recorded in `init_uses_`, where `dispatch` writes the initialiser in its place.
std::vector<Node*> Writer::initializers(const std::vector<Node*>& in) {
    std::vector<Node*> stmts = in;
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t i = 0; i < stmts.size(); ++i) {
            const auto* st = ir::get<ir::Local>(stmts[i]);
            if (st == nullptr || !st->sym || !st->sym->name.starts_with("__t")) continue;
            const auto* made = ir::get<ir::New>(st->init);
            if (made == nullptr || made->body != nullptr) continue;
            std::vector<InitItem> items;
            std::size_t j = i + 1;
            for (; j < stmts.size() && ir::is<ir::ExprStmt>(stmts[j]); ++j) {
                Node* e = ir::get<ir::ExprStmt>(stmts[j])->expr;
                const auto* asg = ir::get<ir::Assign>(e);
                const auto* call = ir::get<ir::Call>(e);
                const auto* target = asg != nullptr ? ir::get<ir::Select>(asg->target) : nullptr;
                if (asg != nullptr && !asg->op && target != nullptr && is_name(target->obj, *st->sym) && target->sym &&
                    (target->sym->kind == ir::SymKind::Field || target->sym->kind == ir::SymKind::Property) && !uses(asg->value, *st->sym)) {
                    items.push_back({true, target->sym, asg->value, {}});
                } else if (call != nullptr && is_name(call->target, *st->sym) && call->sym && call->sym->name == "Add") {
                    const bool self_use = std::any_of(call->args.begin(), call->args.end(), [&](Node* a) { return uses(a, *st->sym); });
                    if (self_use) break;
                    items.push_back({false, call->sym, nullptr, call->args});
                } else {
                    break;
                }
            }
            if (j >= stmts.size() || items.empty()) continue;
            if (std::any_of(items.begin(), items.end(), [&](const InitItem& it) { return it.is_set != items[0].is_set; })) continue;
            std::vector<Node*> use;
            for (Node* n : ir::walk(stmts[j])) {
                if (is_name(n, *st->sym)) use.push_back(n);
            }
            if (use.size() != 1) continue;
            init_uses_[use[0]] = InitGroup{st->init, std::move(items)};
            stmts.erase(stmts.begin() + static_cast<std::ptrdiff_t>(i), stmts.begin() + static_cast<std::ptrdiff_t>(j));
            changed = true;
            break;
        }
    }
    return stmts;
}

void Writer::stmt(Node* s, Lines& out, const std::string& ind) {
    if (s == nullptr) throw Decline("null", "a missing statement");
    if (const auto* x = ir::get<ir::ExprStmt>(s)) {
        Node* e = strip(x->expr);
        if (const auto* un = ir::get<ir::Unsupported>(e)) throw declined_for(*un);
        std::optional<std::string> line;
        if (java_ && ir::is<ir::Call>(e)) line = lib().stmt_call(*this, e);
        if (!line) line = expr(e, true).text;
        out.push_back(ind + *line + ";");
        return;
    }
    if (const auto* x = ir::get<ir::Local>(s)) return local_stmt(*x, out, ind);
    if (const auto* x = ir::get<ir::Return>(s)) {
        out.push_back(x->expr == nullptr ? ind + "return;" : ind + "return " + ret_expr(x->expr) + ";");
        return;
    }
    if (const auto* x = ir::get<ir::If>(s)) {
        out.push_back(ind + "if (" + cond(x->cond) + ")");
        sub(x->then, out, ind);
        ir::Suite e = x->orelse;
        while (ir::present(e)) {
            if (const auto* one = std::get_if<Node*>(&e)) {
                if (const auto* ei = ir::get<ir::If>(*one)) {
                    out.push_back(ind + "else if (" + cond(ei->cond) + ")");
                    sub(ei->then, out, ind);
                    e = ei->orelse;
                    continue;
                }
            }
            out.push_back(ind + "else");
            sub(e, out, ind);
            break;
        }
        return;
    }
    if (ir::is<ir::Block>(s)) return block_into(s, out, ind);
    if (ir::is<ir::Empty>(s)) {
        out.push_back(ind + ";");
        return;
    }
    if (const auto* x = ir::get<ir::Comment>(s)) {
        out.push_back(ind + "// " + x->text.value_or(""));
        return;
    }
    if (const auto* x = ir::get<ir::While>(s)) {
        out.push_back(ind + "while (" + cond(x->cond) + ")");
        loop_body(s, x->body, out, ind);
        return;
    }
    if (const auto* x = ir::get<ir::DoWhile>(s)) {
        out.push_back(ind + "do");
        loop_body(s, x->body, out, ind);
        out.push_back(ind + "while (" + cond(x->cond) + ");");
        return;
    }
    if (const auto* x = ir::get<ir::For>(s)) {
        Scope scope(*this);
        for_stmt(*x, s, out, ind);
        return;
    }
    if (const auto* x = ir::get<ir::ForEach>(s)) {
        const auto* v = ir::get<ir::Local>(x->var);
        if (v == nullptr) throw Decline("foreach", "a for-each variable " + describe(x->var));
        std::optional<std::string> collection;
        if (java_) collection = lib().iterable(*this, x->iterable);
        if (!collection) collection = expr(x->iterable).text;
        Scope scope(*this);
        const std::string t = ctype(v->type);
        const std::string d = declare(v->sym->name, true);
        out.push_back(ind + "foreach (" + t + " " + d + " in " + *collection + ")");
        loop_body(s, ir::Suite(x->body), out, ind);
        return;
    }
    if (const auto* x = ir::get<ir::Labeled>(s)) {
        const std::string lab = ident(x->label.value_or(""));
        const auto* single = std::get_if<Node*>(&x->body);
        Node* bn = single != nullptr ? *single : nullptr;
        if (bn == nullptr && ir::present(x->body)) throw Decline("list", "the statement list");
        const auto uses_of = label_uses(ir::statements(x->body), x->label.value_or(""));
        if (bn != nullptr && (ir::is<ir::While>(bn) || ir::is<ir::DoWhile>(bn) || ir::is<ir::For>(bn) || ir::is<ir::ForEach>(bn))) {
            labels_[lab] = uses_of;
            loop_labels_[bn] = lab;
            stmt(bn, out, ind);
            if (uses_of.first) out.push_back(ind + lab + "_break:;");
            return;
        }
        if (csharp_ || !uses_of.first) {
            out.push_back(ind + lab + ":");
            stmt(bn, out, ind);
            return;
        }
        labels_[lab] = {true, false};
        stmt(bn, out, ind);
        out.push_back(ind + lab + "_break:;");
        return;
    }
    if (const auto* x = ir::get<ir::Break>(s)) {
        out.push_back(x->label && !x->label->empty() ? ind + "goto " + ident(*x->label) + "_break;" : ind + "break;");
        return;
    }
    if (const auto* x = ir::get<ir::Continue>(s)) {
        out.push_back(x->label && !x->label->empty() ? ind + "goto " + ident(*x->label) + "_continue;" : ind + "continue;");
        return;
    }
    if (const auto* x = ir::get<ir::Goto>(s)) {
        out.push_back(ind + "goto " + ident(x->label.value_or("")) + ";");
        return;
    }
    if (const auto* x = ir::get<ir::Throw>(s)) {
        out.push_back(x->expr == nullptr ? ind + "throw;" : ind + "throw " + expr(x->expr).text + ";");
        return;
    }
    if (const auto* x = ir::get<ir::Sync>(s)) {
        out.push_back(ind + "lock (" + expr(x->lock).text + ")");
        block_into(x->body, out, ind);
        return;
    }
    if (const auto* x = ir::get<ir::Assert>(s)) {
        if (java_) {
            out.push_back(ind + "// assert " + expr(x->cond).text + " (Java runs asserts only with -ea)");
            return;
        }
        usings_.insert("System.Diagnostics");
        out.push_back(ind + "Debug.Assert(" + cond(x->cond) + ");");
        return;
    }
    if (ir::is<ir::LocalClass>(s)) throw Decline("local-class", "a class declared in a method");
    if (ir::is<ir::CtorCall>(s)) throw Decline("ctor-call", "a constructor call that is not the first statement");
    if (const auto* x = ir::get<ir::Switch>(s)) return switch_stmt(*x, out, ind);
    if (const auto* x = ir::get<ir::Try>(s)) return try_stmt(*x, out, ind);
    if (ir::is<ir::Yield>(s)) throw Decline("yield", "a switch-expression arm with statements");
    if (const auto* x = ir::get<ir::MachineFragment>(s)) throw Decline("machine", "machine code (" + (x->reason && !x->reason->empty() ? *x->reason : std::string("asm")) + ")");
    const std::string k = describe(s);
    throw Decline(k, "the statement " + k);
}

// ---- expressions -------------------------------------------------------------------------------------------------------------

std::string Writer::wrap(Node* e, int prec, bool right) {
    Ex r = expr(e);
    if (r.prec < prec || (right && r.prec == prec)) return "(" + r.text + ")";
    return r.text;
}

std::string Writer::coerce(Node* e, const TypeP& t) {
    const TypeP et = type_of(e);
    const auto tp = prim_of(t);
    const auto ep = prim_of(et);
    // a char from an int, as Java converts a constant implicitly
    if (tp == ir::Prim::U16 && ep && (*ep == ir::Prim::I32 || *ep == ir::Prim::I8 || *ep == ir::Prim::I16 || *ep == ir::Prim::U8) && ctype(t) == "char") {
        if (const auto* l = ir::get<ir::Lit>(strip(e))) {
            if (const auto* iv = std::get_if<std::int64_t>(&l->value)) return cchar_number(*iv);
            if (const auto* bv = std::get_if<bool>(&l->value)) return cchar_number(*bv ? 1 : 0);
        }
        return "(char)" + wrap(e, kUnary);
    }
    // a char constant narrowed to a byte or short is its number
    if (java_ && (tp == ir::Prim::I8 || tp == ir::Prim::I16 || tp == ir::Prim::U8) && ep == ir::Prim::U16) {
        if (const auto* l = ir::get<ir::Lit>(strip(e))) {
            if (const auto* s = std::get_if<std::string>(&l->value)) {
                const auto cps = code_points(*s);
                if (!cps.empty()) return std::to_string(static_cast<std::uint32_t>(cps.front()));
            }
            if (const auto* iv = std::get_if<std::int64_t>(&l->value)) return std::to_string(*iv);
        }
    }
    if (java_) {
        if (auto got = lib().coerce(*this, e, t)) return *got;
    }
    return expr(e).text;
}

std::string Writer::member_name(const ir::Sym& s) { return java_ ? lib().member_name(*this, s) : ident(s.name); }

std::string Writer::args_text(std::span<Node* const> args, const ir::Sym* s) {
    Lines out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        std::string pass;
        TypeP pt;
        if (s != nullptr) {
            if (i < s->refs.size()) pass = pass_text(s->refs[i]);
            if (i < s->params.size() && !(s->varargs && i + 1 >= s->params.size())) pt = s->params[i];
        }
        const std::string value = pt ? coerce(args[i], pt) : expr(args[i]).text;
        out.push_back((!pass.empty() && pass != "in" ? pass + " " : std::string()) + value);
    }
    return text::join(out, ", ");
}

// r != null ? r.m : null, written r?.m (C#'s own form of the test and the access)
std::optional<Ex> Writer::null_conditional(const ir::Cond& e) {
    if (!csharp_) return std::nullopt;
    const auto* cb = ir::get<ir::Binary>(strip(e.cond));
    if (cb == nullptr || cb->op != "!=") return std::nullopt;
    const auto* rl = ir::get<ir::Lit>(strip(cb->right));
    if (rl == nullptr || !is_null(rl->value)) return std::nullopt;
    const auto* ol = ir::get<ir::Lit>(strip(e.orelse));
    if (ol == nullptr || !is_null(ol->value)) return std::nullopt;
    const std::string r = expr(cb->left).text;
    const Ex then = expr(e.then);
    if (then.prec == kPrimary && then.text.starts_with(r + ".") && !then.text.starts_with(r + "." + r))
        return Ex{r + "?." + then.text.substr(r.size() + 1), kPrimary};
    return std::nullopt;
}

std::string Writer::interpolate(const std::vector<Node*>& parts) {
    std::string out = "$\"";
    for (Node* p : parts) {
        const auto* l = ir::get<ir::Lit>(p);
        if (l != nullptr && is_text(l->value)) {
            const std::string s = cstr(std::get<std::string>(l->value));
            out += text::replace_all(text::replace_all(s.substr(1, s.size() - 2), "{", "{{"), "}", "}}");
        } else {
            std::string t = expr(p).text;
            if (contains(t, ":") || contains(t, "?")) t = "(" + t + ")";
            out += "{" + t + "}";
        }
    }
    return out + "\"";
}

Ex Writer::expr(Node* e, bool statement) {
    Ex r = dispatch(e);
    if (!statement) {
        Node* se = strip(e);
        if (!ir::is<ir::Lit>(se) && !ir::is<ir::Paren>(se) && !lib_const(se)) {
            if (range_of(type_of(e)) && is_const(e) && overflows(e)) return Ex{"unchecked(" + r.text + ")", kPrimary};
        }
    }
    return r;
}

Ex Writer::literal(const ir::Lit& l, const TypeP& t) {
    const ir::LitValue& v = l.value;
    const auto prim = prim_of(t);
    if (is_null(v)) return {"null", kPrimary};
    if (const auto* b = std::get_if<bool>(&v)) return {*b ? "true" : "false", kPrimary};
    if (const auto* sv = std::get_if<std::string>(&v)) {
        const auto cps = code_points(*sv);
        if (prim == ir::Prim::U16 && cps.size() == 1) {
            if (ctype(t) == "ushort") return {std::to_string(static_cast<std::uint32_t>(cps[0])), kPrimary};
            return {cchar_cp(cps[0]), kPrimary};
        }
        return {cstr(*sv), kPrimary};
    }
    if (std::holds_alternative<double>(v) || prim == ir::Prim::F32 || prim == ir::Prim::F64) {
        const auto d = number_of(v);
        if (!d) throw Decline("literal", "a literal that is not a number");
        std::string s = float_text(*d, t);
        return {s, s.starts_with("-") ? kUnary : kPrimary};
    }
    if (const auto* uv = std::get_if<std::uint64_t>(&v)) return {std::to_string(*uv) + "UL", kPrimary};
    if (const auto* iv = std::get_if<std::int64_t>(&v)) {
        const std::int64_t val = *iv;
        if (prim == ir::Prim::U16 && ctype(t) == "char") return {cchar_number(val), kPrimary};
        const char* suffix = prim == ir::Prim::I64 ? "L" : prim == ir::Prim::U32 ? "u" : prim == ir::Prim::U64 ? "UL" : "";
        std::string s = std::to_string(val) + suffix;
        if (!prim || prim == ir::Prim::I32) {
            if (val == std::numeric_limits<std::int32_t>::min()) s = "int.MinValue";
            else if (val == std::numeric_limits<std::int32_t>::max()) s = "int.MaxValue";
        } else if (prim == ir::Prim::I64) {
            if (val == std::numeric_limits<std::int64_t>::min()) s = "long.MinValue";
            else if (val == std::numeric_limits<std::int64_t>::max()) s = "long.MaxValue";
        } else if (prim == ir::Prim::U32 && val == 4294967295LL) {
            s = "uint.MaxValue";
        }
        return {s, s.starts_with("-") ? kUnary : kPrimary};
    }
    throw Decline("literal", "a literal of an unknown kind");
}

Ex Writer::call_expr(Node* e, const ir::Call& x) {
    if (java_) {
        if (auto got = lib().call(*this, e, false)) return *got;
    }
    const ir::SymP& s = x.sym;
    if (!s) throw Decline("call", "a call with no resolved method");
    const auto flag = [&](std::string_view f) { return has(s->flags, f); };
    const std::string_view sname = s->name;
    const std::vector<Node*>& args = x.args;
    if (flag("operator")) {
        if (const auto op = operator_methods().find(sname); op != operator_methods().end() && args.size() == 2) {
            const int prec = binary_precedence().at(op->second);
            const std::string a = wrap(args[0], prec);
            const std::string b = wrap(args[1], prec, true);
            return {a + " " + std::string(op->second) + " " + b, prec};
        }
        if (const auto op = unary_methods().find(sname); op != unary_methods().end() && args.size() == 1)
            return {std::string(op->second) + wrap(args[0], kUnary), kUnary};
        if (sname == "op_Implicit" && args.size() == 1) return expr(args[0]);
        if (sname == "op_Explicit" && args.size() == 1) return {"(" + ctype(e->type) + ")" + wrap(args[0], kUnary), kUnary};
    }
    if (flag("delegate") && x.target != nullptr) return {wrap(x.target, kPrimary) + "(" + args_text(x.args, s.get()) + ")", kPrimary};
    std::string targs;
    if (!x.typeargs.empty() && !flag("inferred") && (csharp_ || !inferable(x))) {
        Lines ts;
        for (const ir::TypeArg& a : x.typeargs) {
            const auto* t = std::get_if<TypeP>(&a);
            if (t == nullptr) throw Decline("call", "a type argument that is not a type");
            ts.push_back(ctype(*t));
        }
        targs = "<" + text::join(ts, ", ") + ">";
    }
    const std::string name = member_name(*s) + targs;
    if (flag("extension") && !args.empty() && x.target != nullptr && ir::is<ir::TypeName>(x.target)) {
        // a static method called as if on its first argument: written on the receiver
        ir::Sym rest = *s;
        if (!rest.params.empty()) rest.params.erase(rest.params.begin());
        if (!rest.refs.empty()) rest.refs.erase(rest.refs.begin());
        ir::ClassDecl* c = s->owner ? prog_.class_named(*s->owner) : nullptr;
        std::string ns;
        if (c != nullptr) ns = namespace_of(c);
        else if (s->owner && contains(*s->owner, ".")) ns = rsplit_dot(*s->owner).first;
        if (!ns.empty()) usings_.insert(ns);
        const std::string recv = wrap(args[0], kPrimary);
        return {recv + "." + name + "(" + args_text(std::span<Node* const>(args).subspan(1), &rest) + ")", kPrimary};
    }
    if (x.target == nullptr) return {name + "(" + args_text(x.args, s.get()) + ")", kPrimary};
    if (ir::is<ir::TypeName>(x.target) && cls_ != nullptr) {
        // a static of this class, written bare
        const std::string* tn = class_of(type_of(x.target));
        if (tn != nullptr && cls_->qname && *tn == *cls_->qname) return {name + "(" + args_text(x.args, s.get()) + ")", kPrimary};
    }
    return {wrap(x.target, kPrimary) + "." + name + "(" + args_text(x.args, s.get()) + ")", kPrimary};
}

Ex Writer::dispatch(Node* e) {
    if (e == nullptr) throw Decline("null", "a missing expression");
    if (const auto it = init_uses_.find(e); it != init_uses_.end()) {
        // the local of a hoisted object or collection initialiser, where it is used: the initialiser itself
        const InitGroup& g = it->second;
        std::string nw = dispatch(g.made).text;
        if (nw.ends_with("()")) nw.resize(nw.size() - 2);
        Lines items;
        for (const InitItem& item : g.items) {
            if (item.is_set) {
                items.push_back(member_name(*item.sym) + " = " + coerce(item.value, item.sym->type));
            } else if (item.args.size() == 1) {
                items.push_back(expr(item.args[0]).text);
            } else {
                Lines xs;
                for (Node* y : item.args) xs.push_back(expr(y).text);
                items.push_back("{ " + text::join(xs, ", ") + " }");
            }
        }
        return {nw + " { " + text::join(items, ", ") + " }", kPrimary};
    }
    if (const auto* x = ir::get<ir::Name>(e)) {
        if (!x->sym) throw Decline("name", "a name with no symbol");
        return {lookup(x->sym->name), kPrimary};
    }
    if (const auto* x = ir::get<ir::Lit>(e)) return literal(*x, e->type);
    if (const auto* x = ir::get<ir::Paren>(e)) return expr(x->x);   // (grouping is written from the operators' precedence, where needed)
    if (const auto* x = ir::get<ir::Call>(e)) return call_expr(e, *x);
    if (const auto* x = ir::get<ir::Select>(e)) {
        if (java_) {
            if (auto got = lib().select(*this, e)) return *got;
        }
        const ir::SymP& s = x->sym;
        if (!s) throw Decline("select", "a member access with no symbol");
        const auto* th = ir::get<ir::This>(x->obj);
        if (th != nullptr && !th->qualifier && !shadowed(s->name)) return {member_name(*s), kPrimary};   // this.x, where no local hides x
        if (ir::is<ir::TypeName>(x->obj) && cls_ != nullptr) {
            const std::string* tn = class_of(type_of(x->obj));
            if (tn != nullptr && cls_->qname && *tn == *cls_->qname) return {ident(s->name), kPrimary};   // a static of this class
        }
        return {wrap(x->obj, kPrimary) + "." + member_name(*s), kPrimary};
    }
    if (const auto* x = ir::get<ir::This>(e)) {
        if (x->qualifier && cls_ != nullptr) {
            const std::string* qn = class_of(x->qualifier);
            if (qn == nullptr || qn->empty() || !cls_->qname || *qn != *cls_->qname) throw Decline("outer-this", "an outer instance (Outer.this)");
        }
        return {"this", kPrimary};
    }
    if (ir::is<ir::Super>(e)) return {"base", kPrimary};
    if (ir::is<ir::TypeName>(e)) return {ctype(e->type), kPrimary};
    if (const auto* x = ir::get<ir::Length>(e)) return {wrap(x->arr, kPrimary) + ".Length", kPrimary};
    if (const auto* x = ir::get<ir::Index>(e)) {
        if (x->sym && java_) throw Decline("indexer", "an indexer from Java");
        Lines is;
        for (Node* i : ir::statements(x->index)) is.push_back(expr(i).text);
        return {wrap(x->arr, kPrimary) + "[" + text::join(is, ", ") + "]", kPrimary};
    }
    if (const auto* x = ir::get<ir::Unary>(e)) {
        const std::string& op = x->op;
        if ((op == "++" || op == "--") && x->postfix) return {wrap(x->x, kPrimary) + op, kPrimary};
        std::string v = wrap(x->x, kUnary);
        if ((op == "-" || op == "+") && v.starts_with(op)) v = "(" + v + ")";   // - -x, not --x
        const std::string t = op + v;
        if (x->overflow == ir::Overflow::Trap) return {"checked(" + t + ")", kPrimary};
        return {t, kUnary};
    }
    if (const auto* x = ir::get<ir::Binary>(e)) {
        if (java_) {
            if (auto got = lib().binary(*this, e)) return *got;
        }
        const auto pit = binary_precedence().find(x->op);
        if (pit == binary_precedence().end()) throw Decline("operator", "the operator " + x->op);
        const int prec = pit->second;
        std::string t;
        if (x->op == "??") t = wrap(x->left, prec, true) + " ?? " + wrap(x->right, prec);   // right-associative
        else t = wrap(x->left, prec) + " " + x->op + " " + wrap(x->right, prec, true);
        if (x->overflow == ir::Overflow::Trap) return {"checked(" + t + ")", kPrimary};
        return {t, prec};
    }
    if (const auto* x = ir::get<ir::Concat>(e)) {
        Lines texts;
        for (std::size_t i = 0; i < x->parts.size(); ++i) {
            std::optional<std::string> got;
            if (java_) got = lib().concat_part(*this, x->parts[i]);
            texts.push_back(got ? *got : wrap(x->parts[i], kAdd, i > 0));
        }
        // a concatenation of two non-strings starts from the empty string, so that + is not an addition
        if (x->parts.size() >= 2 && !is_string(type_of(x->parts[0])) && !is_string(type_of(x->parts[1])) &&
            !(texts[0].starts_with("JavaRt.Str(") || texts[1].starts_with("JavaRt.Str(")))
            texts.insert(texts.begin(), "\"\"");
        if (texts.size() == 1) texts.insert(texts.begin(), "\"\"");
        if (csharp_ && x->parts.size() > 2) {
            const bool literals_are_text = std::all_of(x->parts.begin(), x->parts.end(), [](Node* p) {
                const auto* l = ir::get<ir::Lit>(p);
                return l == nullptr || is_text(l->value);
            });
            if (literals_are_text && interpolatable(x->parts)) return {interpolate(x->parts), kPrimary};
        }
        return {text::join(texts, " + "), kAdd};
    }
    if (const auto* x = ir::get<ir::Assign>(e)) {
        const std::string target = wrap(x->target, kUnary);
        if (!x->op) return {target + " = " + coerce(x->value, type_of(x->target)), kAssign};
        const std::string op = *x->op + "=";
        const std::string value = expr(x->value).text;
        if (x->overflow == ir::Overflow::Trap) return {"checked(" + target + " " + op + " " + value + ")", kPrimary};
        return {target + " " + op + " " + value, kAssign};
    }
    if (const auto* x = ir::get<ir::Cond>(e)) {
        if (auto got = null_conditional(*x)) return *got;
        const std::string c = wrap(x->cond, kCond + 1);
        std::string a = e->type ? coerce(x->then, e->type) : expr(x->then).text;
        const std::string b = !e->type ? wrap(x->orelse, kCond) : coerce(x->orelse, e->type);
        if (contains(a, " = ")) a = "(" + a + ")";
        return {c + " ? " + a + " : " + b, kCond};
    }
    if (const auto* x = ir::get<ir::Cast>(e)) {
        const std::string tn = ctype(e->type);
        std::string v = wrap(x->x, kUnary);
        if (v.starts_with("-") || v.starts_with("+")) v = "(" + v + ")";
        const std::string t = "(" + tn + ")" + v;
        if (x->overflow == ir::Overflow::Trap) return {"checked(" + t + ")", kPrimary};
        return {t, kUnary};
    }
    if (const auto* x = ir::get<ir::InstanceOf>(e)) {
        std::string t = wrap(x->x, kRel) + " is " + ctype(x->test);
        if (x->binding != nullptr) {
            const auto* bl = ir::get<ir::Local>(x->binding);
            if (bl == nullptr) throw Decline("pattern", "a pattern binding " + describe(x->binding));
            if (bl->sym) t += " " + declare(bl->sym->name);
        }
        return {t, kRel};
    }
    if (const auto* x = ir::get<ir::ClassLit>(e)) return {"typeof(" + ctype(x->of) + ")", kPrimary};
    if (const auto* x = ir::get<ir::Lambda>(e)) {
        if (java_) {
            auto* lam = ir::get<ir::Lambda>(e);
            lib().check_lambda(*this, *lam);
        }
        // untyped parameters: a lambda here always has a target type (an argument, a typed local, a return)
        struct RestoreLocals {
            std::unordered_set<std::string>& slot;
            std::unordered_set<std::string> saved;
            ~RestoreLocals() { slot = std::move(saved); }
        } restore{locals_, locals_};
        Scope scope(*this);
        Lines names;
        for (Node* p : x->params) names.push_back(declare(ir::get<ir::Param>(p)->sym->name, true));
        const std::string ps = names.size() == 1 ? names[0] : "(" + text::join(names, ", ") + ")";
        if (ir::is<ir::Block>(x->body)) {
            Lines out;
            block_into(x->body, out, "");
            // on lines of its own, re-indented by the caller's line
            return {ps + " =>\n" + text::join(out, "\n"), kAssign};
        }
        return {ps + " => " + expr(x->body).text, kAssign};
    }
    if (const auto* x = ir::get<ir::MethodRef>(e)) {
        if (x->mode == "new") throw Decline("method-ref", "a constructor reference");
        if (ir::is<ir::TypeName>(x->target) && !x->sym->is_static) throw Decline("method-ref", "an unbound method reference (Type::instanceMethod)");
        return {wrap(x->target, kPrimary) + "." + member_name(*x->sym), kPrimary};
    }
    if (const auto* x = ir::get<ir::SwitchExpr>(e)) {
        Lines arms;
        for (Node* cn : x->cases) {
            const auto* c = ir::get<ir::Case>(cn);
            if (c->body.size() != 1) throw Decline("switch-expression", "a switch-expression arm with statements");
            Node* b = c->body[0];
            std::string val;
            if (const auto* y = ir::get<ir::Yield>(b)) val = coerce(y->expr, e->type);
            else if (const auto* t = ir::get<ir::Throw>(b)) val = "throw " + expr(t->expr).text;
            else if (const auto* es = ir::get<ir::ExprStmt>(b)) val = expr(es->expr).text;
            else throw Decline("switch-expression", "a switch-expression arm " + describe(b));
            Lines pats;
            if (c->is_default) {
                pats.emplace_back("_");
            } else {
                for (Node* l : c->labels) pats.push_back(case_label(l, x->expr));
            }
            arms.push_back(text::join(pats, " or ") + " => " + val);
        }
        return {wrap(x->expr, kPrimary) + " switch { " + text::join(arms, ", ") + " }", kPrimary};
    }
    if (const auto* x = ir::get<ir::New>(e)) {
        if (x->body != nullptr) throw Decline("anonymous-class", "an anonymous class");
        if (x->outer != nullptr) throw Decline("inner-class", "an inner class made on an outer instance");
        if (java_) {
            if (auto got = lib().new_object(*this, e)) return *got;
        }
        return {"new " + ctype(e->type) + "(" + args_text(x->args, x->sym.get()) + ")", kPrimary};
    }
    if (const auto* x = ir::get<ir::NewArray>(e)) {
        const TypeP& t = e->type;
        const auto* arr = t ? std::get_if<ir::ArrayType>(&t->body) : nullptr;
        // an array with its items: `new T[] { a, b }`
        if (!x->init.empty() || x->dims.empty()) {
            if (!x->dims.empty()) throw Decline("array", "an array with sizes and items");
            const TypeP elem = arr != nullptr ? arr->elem : x->elem;
            Lines items;
            for (Node* y : x->init) items.push_back(coerce(y, elem));
            const std::string list = text::join(items, ", ");
            return {"new " + ctype(t) + (list.empty() ? " { }" : " { " + list + " }"), kPrimary};
        }
        if (arr != nullptr && arr->rank != 1) {
            Lines ds;
            for (Node* d : x->dims) ds.push_back(expr(d).text);
            return {"new " + ctype(arr->elem) + "[" + text::join(ds, ", ") + "]", kPrimary};
        }
        std::size_t n = 1;
        TypeP base = x->elem;
        if (arr != nullptr) std::tie(n, base) = ir::dims(t);
        std::string sizes;
        for (Node* d : x->dims) sizes += "[" + expr(d).text + "]";
        std::string rest;
        for (std::size_t i = x->dims.size(); i < n; ++i) rest += "[]";
        return {"new " + ctype(base) + sizes + rest, kPrimary};
    }
    if (const auto* x = ir::get<ir::Unsupported>(e)) throw declined_for(*x);
    const std::string k = describe(e);
    throw Decline(k, "the expression " + k);
}

// ---- the files ----------------------------------------------------------------------------------------------------------------

const std::vector<File>& Program::files() {
    if (files_) return *files_;
    // every language's library sees the whole program before anything is written
    std::set<std::string> langs;
    for (Node* n : units_) {
        const auto& lang = ir::get<ir::Unit>(n)->lang;
        langs.insert(lang && !lang->empty() ? *lang : "java");
    }
    for (const std::string& lang : langs) {
        if (Library* l = library_for(lang)) l->prepare(*this);
    }
    std::vector<File> out;
    std::set<std::string> taken;
    for (Node* n : units_) {
        Writer w(*this, n);
        const auto& path = ir::get<ir::Unit>(n)->path;
        std::string stem = fs::to_text(fs::from_text(path && !path->empty() ? *path : "Unit").stem());
        if (stem.empty()) stem = "Unit";
        std::string name = stem;
        for (int k = 2; taken.contains(name); ++k) name = stem + "_" + std::to_string(k);   // (two units of one stem would be one file)
        taken.insert(name);
        out.push_back({name + ".cs", w.unit_text()});
    }
    for (const std::string& r : runtime_) {   // (a set: by name)
        out.push_back({r + ".cs", text::replace_all(fs::read_text(options_.runtime_dir / (r + ".cs")), "\r\n", "\n")});
    }
    files_ = std::move(out);
    return *files_;
}

const std::vector<Gap>& Program::ledger() {
    files();
    return ledger_;
}

std::vector<std::filesystem::path> Program::write(const std::filesystem::path& dir) {
    fs::make_dirs(dir);
    std::vector<std::filesystem::path> paths;
    for (const File& f : files()) {
        paths.push_back(dir / f.name);
        fs::write_text(paths.back(), f.text);
    }
    return paths;
}

}  // namespace cb::translate::backends::csharp

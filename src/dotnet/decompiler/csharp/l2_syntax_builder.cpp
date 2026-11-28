// dotnet/decompiler/csharp/l2_syntax_builder.cpp -- the C# back end's plain lowering (see l2_syntax_builder.hpp).
//
// One UnitBuilder per unit walks its classes and makes the syntax tree: each L2 declaration, statement and expression becomes the node C#
// writes it with. A construct it does not carry throws Declined, which the nearest place that can hold a gap (a method body, a field
// initialiser, a class) catches and turns into a ledger entry and a `throw new System.NotSupportedException("CB-GAP: ...")`.
#include "dotnet/decompiler/csharp/l2_syntax_builder.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <map>
#include <set>
#include <unordered_set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "dotnet/decompiler/csharp/output_visitor/csharp_output_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/formatting_options_factory.hpp"
#include "dotnet/decompiler/csharp/output_visitor/generic_grammar_ambiguity_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/insert_parentheses_visitor.hpp"
#include "dotnet/decompiler/csharp/output_visitor/text_writer.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "translate/rules/java_csharp.hpp"

namespace cb::dotnet::decompiler::csharp {

namespace l2_plain {

using namespace syntax;
using il::make;
using ir::Node;
using ir::TypeP;
namespace text = cb::text;

// This construct is not written: the place that can hold a gap catches it.
struct Declined {
    std::string kind;
    std::string why;
};

bool has(const std::vector<std::string>& v, std::string_view x) { return std::find(v.begin(), v.end(), x) != v.end(); }
bool contains(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }
bool is_text(const ir::LitValue& v) { return std::holds_alternative<std::string>(v); }
bool is_null(const ir::LitValue& v) { return std::holds_alternative<std::monostate>(v); }
std::string describe(Node* n) { return std::string(ir::kind_name(*n)); }

Node* strip(Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

std::optional<ir::Prim> prim_of(const TypeP& t) {
    if (!t) return std::nullopt;
    if (const auto* p = std::get_if<ir::PrimType>(&t->body)) return p->prim;
    return std::nullopt;
}
bool is_prim(const TypeP& t, ir::Prim prim) { return prim_of(t) == prim; }
const std::string* class_of(const TypeP& t) {
    if (!t) return nullptr;
    const auto* c = std::get_if<ir::ClassType>(&t->body);
    return c != nullptr ? &c->name : nullptr;
}
// A .NET class a number is itself of (or boxed in): a literal of it is the number, not an enum's value.
bool numeric_box(std::string_view n) {
    static const std::unordered_set<std::string_view> names = {"System.Int32", "System.Int64", "System.Int16", "System.SByte", "System.Byte", "System.UInt16",
                                                               "System.UInt32", "System.UInt64", "System.Char", "System.Boolean", "System.Single", "System.Double",
                                                               "System.Decimal", "System.IntPtr", "System.UIntPtr", "System.Object", "System.ValueType",
                                                               "System.Enum", "System.Nullable", "System.String"};
    return names.contains(n);
}
bool is_string(const TypeP& t) {
    const std::string* n = class_of(t);
    return n != nullptr && (*n == "System.String" || *n == "java.lang.String");
}
TypeP plain_type_of(Node* e) {
    if (e == nullptr) return {};
    if (const auto* l = ir::get<ir::Local>(e)) return l->type;
    return e->type;
}

// A name as C# takes it: the compiler's own names (`<>h__TransparentIdentifier0`) are not written by a person, so a plain name. (The `@`
// before a keyword is the output visitor's.)
std::string clean(std::string_view name) {
    std::string out(name);
    for (char& c : out) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_' && static_cast<unsigned char>(c) < 0x80) c = '_';
    }
    return out;
}

std::vector<char32_t> code_points(std::string_view s) {
    std::vector<char32_t> out;
    const std::u16string u = util::utf16(s);
    for (std::size_t i = 0; i < u.size(); ++i) {
        char32_t c = u[i];
        if (c >= 0xD800 && c <= 0xDBFF && i + 1 < u.size() && u[i + 1] >= 0xDC00 && u[i + 1] <= 0xDFFF) {
            c = 0x10000 + ((c - 0xD800) << 10) + (u[i + 1] - 0xDC00);
            ++i;
        }
        out.push_back(c);
    }
    return out;
}

std::optional<double> number_of(const ir::LitValue& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return static_cast<double>(*i);
    if (const auto* u = std::get_if<std::uint64_t>(&v)) return static_cast<double>(*u);
    if (const auto* b = std::get_if<bool>(&v)) return *b ? 1.0 : 0.0;
    if (const auto* d = std::get_if<double>(&v)) return *d;
    if (const auto* s = std::get_if<std::string>(&v)) {
        if (*s == "nan") return std::numeric_limits<double>::quiet_NaN();
        if (*s == "inf") return std::numeric_limits<double>::infinity();
        if (*s == "-inf") return -std::numeric_limits<double>::infinity();
        char* end = nullptr;
        const double d = std::strtod(s->c_str(), &end);
        if (!s->empty() && end == s->c_str() + s->size()) return d;
    }
    return std::nullopt;
}

struct IntRange {
    std::int64_t lo;
    std::int64_t hi;
    int bits;
};
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

// ---- tables -------------------------------------------------------------------------------------------------------------------

const std::unordered_map<std::string_view, std::string_view>& alias_table() {
    static const std::unordered_map<std::string_view, std::string_view> m = {
        {"System.String", "string"}, {"System.Object", "object"}, {"System.Boolean", "bool"}, {"System.Int32", "int"},
        {"System.Int64", "long"}, {"System.Double", "double"}, {"System.Single", "float"}, {"System.Char", "char"},
        {"System.Byte", "byte"}, {"System.SByte", "sbyte"}, {"System.Int16", "short"}, {"System.UInt16", "ushort"},
        {"System.UInt32", "uint"}, {"System.UInt64", "ulong"}, {"System.Decimal", "decimal"}, {"System.Void", "void"}};
    return m;
}

const std::unordered_map<std::string_view, BinaryOperatorType>& binary_operators() {
    static const std::unordered_map<std::string_view, BinaryOperatorType> m = {
        {"*", BinaryOperatorType::Multiply}, {"/", BinaryOperatorType::Divide}, {"%", BinaryOperatorType::Modulus},
        {"+", BinaryOperatorType::Add}, {"-", BinaryOperatorType::Subtract}, {"<<", BinaryOperatorType::ShiftLeft},
        {">>", BinaryOperatorType::ShiftRight}, {">>>", BinaryOperatorType::UnsignedShiftRight}, {"<", BinaryOperatorType::LessThan},
        {">", BinaryOperatorType::GreaterThan}, {"<=", BinaryOperatorType::LessThanOrEqual}, {">=", BinaryOperatorType::GreaterThanOrEqual},
        {"==", BinaryOperatorType::Equality}, {"!=", BinaryOperatorType::InEquality}, {"&", BinaryOperatorType::BitwiseAnd},
        {"^", BinaryOperatorType::ExclusiveOr}, {"|", BinaryOperatorType::BitwiseOr}, {"&&", BinaryOperatorType::ConditionalAnd},
        {"||", BinaryOperatorType::ConditionalOr}, {"??", BinaryOperatorType::NullCoalescing}};
    return m;
}

const std::unordered_map<std::string_view, AssignmentOperatorType>& assignment_operators() {
    static const std::unordered_map<std::string_view, AssignmentOperatorType> m = {
        {"+", AssignmentOperatorType::Add}, {"-", AssignmentOperatorType::Subtract}, {"*", AssignmentOperatorType::Multiply},
        {"/", AssignmentOperatorType::Divide}, {"%", AssignmentOperatorType::Modulus}, {"<<", AssignmentOperatorType::ShiftLeft},
        {">>", AssignmentOperatorType::ShiftRight}, {">>>", AssignmentOperatorType::UnsignedShiftRight},
        {"&", AssignmentOperatorType::BitwiseAnd}, {"|", AssignmentOperatorType::BitwiseOr}, {"^", AssignmentOperatorType::ExclusiveOr}};
    return m;
}

// a user-defined operator's method, called as a method: the operator it is
const std::unordered_map<std::string_view, std::string_view>& operator_methods() {
    static const std::unordered_map<std::string_view, std::string_view> m = {
        {"op_Addition", "+"}, {"op_Subtraction", "-"}, {"op_Multiply", "*"}, {"op_Division", "/"}, {"op_Modulus", "%"},
        {"op_Equality", "=="}, {"op_Inequality", "!="}, {"op_LessThan", "<"}, {"op_GreaterThan", ">"},
        {"op_LessThanOrEqual", "<="}, {"op_GreaterThanOrEqual", ">="}, {"op_BitwiseAnd", "&"}, {"op_BitwiseOr", "|"},
        {"op_ExclusiveOr", "^"}, {"op_LeftShift", "<<"}, {"op_RightShift", ">>"}, {"op_UnsignedRightShift", ">>>"}};
    return m;
}

const std::unordered_map<std::string_view, UnaryOperatorType>& unary_methods() {
    static const std::unordered_map<std::string_view, UnaryOperatorType> m = {
        {"op_UnaryNegation", UnaryOperatorType::Minus}, {"op_UnaryPlus", UnaryOperatorType::Plus},
        {"op_LogicalNot", UnaryOperatorType::Not}, {"op_OnesComplement", UnaryOperatorType::BitNot}};
    return m;
}

// every operator a class can declare, by the name of its method
const std::unordered_map<std::string_view, OperatorType>& declared_operators() {
    static const std::unordered_map<std::string_view, OperatorType> m = {
        {"op_Addition", OperatorType::Addition}, {"op_Subtraction", OperatorType::Subtraction}, {"op_Multiply", OperatorType::Multiply},
        {"op_Division", OperatorType::Division}, {"op_Modulus", OperatorType::Modulus}, {"op_Equality", OperatorType::Equality},
        {"op_Inequality", OperatorType::Inequality}, {"op_LessThan", OperatorType::LessThan}, {"op_GreaterThan", OperatorType::GreaterThan},
        {"op_LessThanOrEqual", OperatorType::LessThanOrEqual}, {"op_GreaterThanOrEqual", OperatorType::GreaterThanOrEqual},
        {"op_BitwiseAnd", OperatorType::BitwiseAnd}, {"op_BitwiseOr", OperatorType::BitwiseOr}, {"op_ExclusiveOr", OperatorType::ExclusiveOr},
        {"op_LeftShift", OperatorType::LeftShift}, {"op_RightShift", OperatorType::RightShift},
        {"op_UnsignedRightShift", OperatorType::UnsignedRightShift}, {"op_UnaryNegation", OperatorType::UnaryNegation},
        {"op_UnaryPlus", OperatorType::UnaryPlus}, {"op_LogicalNot", OperatorType::LogicalNot}, {"op_OnesComplement", OperatorType::OnesComplement},
        {"op_Increment", OperatorType::Increment}, {"op_Decrement", OperatorType::Decrement}, {"op_True", OperatorType::True},
        {"op_False", OperatorType::False}, {"op_Implicit", OperatorType::Implicit}, {"op_Explicit", OperatorType::Explicit}};
    return m;
}

Modifiers modifiers_of(const std::vector<std::string>& mods) {
    static const std::unordered_map<std::string_view, Modifiers> m = {
        {"public", Modifiers::Public}, {"private", Modifiers::Private}, {"protected", Modifiers::Protected},
        {"internal", Modifiers::Internal}, {"abstract", Modifiers::Abstract}, {"virtual", Modifiers::Virtual},
        {"sealed", Modifiers::Sealed}, {"static", Modifiers::Static}, {"override", Modifiers::Override},
        {"readonly", Modifiers::Readonly}, {"const", Modifiers::Const}, {"new", Modifiers::New}, {"partial", Modifiers::Partial},
        {"extern", Modifiers::Extern}, {"volatile", Modifiers::Volatile}, {"unsafe", Modifiers::Unsafe}, {"async", Modifiers::Async},
        {"ref", Modifiers::Ref}, {"required", Modifiers::Required}};
    Modifiers out = Modifiers::None;
    for (const std::string& s : mods) {
        if (const auto it = m.find(s); it != m.end()) out |= it->second;
    }
    return out;
}

// ---- constants: where a constant expression leaves its type's range ---------------------------------------------------

// `int.MaxValue` and the other limits of an integer type (what the rule set java writes Java's Integer.MAX_VALUE as): a constant.
std::optional<std::int64_t> limit_value(Node* e) {
    const auto* s = ir::get<ir::Select>(e);
    if (s == nullptr || !s->sym || !ir::is<ir::TypeName>(s->obj)) return std::nullopt;
    const auto rg = range_of(s->obj->type);
    if (!rg || rg->bits == 64 && prim_of(s->obj->type) == ir::Prim::U64) return std::nullopt;
    if (s->sym->name == "MaxValue") return rg->hi;
    if (s->sym->name == "MinValue") return rg->lo;
    return std::nullopt;
}

bool is_const(Node* e0) {
    Node* e = strip(e0);
    if (limit_value(e)) return true;
    if (const auto* l = ir::get<ir::Lit>(e)) {
        if (is_null(l->value)) return false;
        if (const auto* s = std::get_if<std::string>(&l->value)) return code_points(*s).size() == 1;
        return true;
    }
    if (const auto* u = ir::get<ir::Unary>(e)) return (u->op == "-" || u->op == "+" || u->op == "~") && is_const(u->x);
    if (const auto* b = ir::get<ir::Binary>(e)) {
        return binary_operators().contains(b->op) && b->op != "&&" && b->op != "||" && b->op != "??" && is_const(b->left) && is_const(b->right);
    }
    if (const auto* c = ir::get<ir::Cast>(e)) return c->x != nullptr && e->type && e->type->is_prim() && is_const(c->x);
    return false;
}

// Does folding this constant integer expression leave its type's range anywhere along the way? The arithmetic is 64-bit: a result that
// leaves int64 is reported as leaving the type's range and wraps as the type does.
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
        if (const auto lc = limit_value(x)) return lc;
        std::optional<std::int64_t> r;
        bool big = false;
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
                if (rg->bits == 64) return *r;
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
                big = (av == kMin && cv == -1) || (cv == kMin && av == -1) || (av != kMin && cv != kMin && std::llabs(av) > kMax / std::llabs(cv));
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
        return std::nullopt;
    }
};

bool overflows(Node* e) {
    Folder f;
    f.value(e);
    return f.out_of_range;
}

// ---- control flow -------------------------------------------------------------------------------------------------------

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

// A C# object or collection initialiser, hoisted by the front end into `var t = new T(); t.A = 1; t.Add(x); ... S(t)`: put back as
// `S(new T { A = 1 })` / `S(new T { x })`.
struct InitItem {
    bool is_set = false;
    ir::SymP sym;
    Node* value = nullptr;
    std::vector<Node*> args;
};
struct InitGroup {
    Node* made = nullptr;
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

// A body of one `return expr;` (in a block of its own or not): written as an expression body.
bool single_return(const ir::Suite& body) {
    const auto st = ir::statements(body);
    if (st.empty()) return false;
    const auto* blk = ir::get<ir::Block>(st[0]);
    const std::vector<Node*>& stmts = blk != nullptr ? blk->stmts : st;
    if (stmts.size() != 1 || (blk != nullptr && st.size() != 1)) return false;
    const auto* ret = ir::get<ir::Return>(stmts[0]);
    return ret != nullptr && ret->expr != nullptr;
}

bool ends_with_name(const ir::ClassDecl& c) { return c.qname && c.name && c.qname->ends_with(*c.name); }

typesystem::ReferenceKind pass_kind(ir::PassMode m) {
    switch (m) {
    case ir::PassMode::Ref: return typesystem::ReferenceKind::Ref;
    case ir::PassMode::Out: return typesystem::ReferenceKind::Out;
    case ir::PassMode::In: return typesystem::ReferenceKind::In;
    case ir::PassMode::ByValue: break;
    }
    return typesystem::ReferenceKind::None;
}

Declined declined_for(const ir::Unsupported& u) {
    const std::string kind = u.kind.value_or("");
    return Declined{kind, u.raw && !u.raw->empty() ? *u.raw : kind};
}

// ---- syntax helpers -------------------------------------------------------------------------------------------------------

Expression* ident_expr(const std::string& name) { return make<IdentifierExpression>(clean(name)); }
Expression* member(Expression* target, const std::string& name) { return make<MemberReferenceExpression>(target, clean(name)); }
AstType* keyword_type(std::string_view kw) { return make<PrimitiveType>(std::string(kw)); }
Expression* string_lit(std::string_view s) { return make<PrimitiveExpression>(typesystem::ConstantValue(util::utf16(s))); }
Expression* int_lit(std::int32_t v) { return make<PrimitiveExpression>(typesystem::ConstantValue(v)); }

// A statement where one statement goes (an if's branch, a loop's body): always a block, for clarity.
BlockStatement* as_block(const std::vector<Statement*>& stmts) {
    if (stmts.size() == 1) {
        if (auto* b = as<BlockStatement>(stmts[0])) return b;
    }
    return make<BlockStatement>(stmts);
}

// `throw new System.NotSupportedException("CB-GAP: why");` in a block.
BlockStatement* gap_block(const std::string& why) {
    auto* type = make<MemberType>(make<SimpleType>("System"), "NotSupportedException");
    auto* made = make<ObjectCreateExpression>(type, std::vector<Expression*>{string_lit("CB-GAP: " + why)});
    return make<BlockStatement>(std::vector<Statement*>{make<ThrowStatement>(made)});
}

Comment* line_comment(const std::string& content) { return make<Comment>(" " + content, CommentType::SingleLine); }

// ---- the writer of one unit -------------------------------------------------------------------------------------------------

class UnitBuilder {
public:
    UnitBuilder(L2CSharpProgram& prog, Node* unit_node) : prog_(prog), u_(*ir::get<ir::Unit>(unit_node)) {
        lang_ = u_.lang && !u_.lang->empty() ? *u_.lang : "java";
        java_ = lang_ == "java" && prog.java_rules();
        csharp_ = lang_ == "csharp";
        scopes_.emplace_back();
    }

    SyntaxTree* unit();

private:
    // ---- scopes (C#'s scope of a local is its whole block: a name an inner block declares before an outer one does is renumbered)
    std::string declare(const std::string& name, bool nested = false);
    std::string lookup(const std::string& name) const;
    void push() { scopes_.emplace_back(); }
    void pop() { scopes_.pop_back(); }
    struct Scope {
        UnitBuilder& w;
        explicit Scope(UnitBuilder& writer) : w(writer) { w.push(); }
        ~Scope() { w.pop(); }
    };
    void note(const std::string& kind, const std::string& why);
    bool shadowed(const std::string& name) const { return locals_.contains(name); }
    TypeP type_of(Node* e) const;

    // ---- types
    AstType* type(const TypeP& t);
    AstType* class_type(const ir::ClassType& c);
    AstType* named(const std::string& dotted, const std::vector<AstType*>& args = {});
    std::string short_name(const std::string& qname);
    std::string namespace_of(const ir::ClassDecl* c);

    // ---- declarations
    EntityDeclaration* class_decl(ir::ClassDecl& c);
    EntityDeclaration* class_decl_body(ir::ClassDecl& c);
    void type_parameters(const std::vector<std::string>& names, const std::vector<Node*>& tparams, AstNodeCollection<TypeParameterDeclaration>& out);
    Constraint* constraint(Node* tp);
    // A member's declarations: none (a member C# has no place for), or one.
    EntityDeclaration* member_decl(Node* m, ir::ClassDecl& c, std::vector<Comment*>& pending);
    EntityDeclaration* field(Node* node);
    EntityDeclaration* prop(ir::Property& p);
    std::vector<ParameterDeclaration*> params(const std::vector<Node*>& ps);
    EntityDeclaration* method_decl(Node* node, ir::ClassDecl& c);
    BlockStatement* body_block(const ir::Suite& body, const ir::MethodDecl* m);
    BlockStatement* gap(const Declined& d);
    Expression* ret_expr(Node* e);

    // ---- statements
    BlockStatement* block_of(Node* b);
    BlockStatement* block_stmts(std::vector<Node*> stmts);
    BlockStatement* sub(const ir::Suite& s);
    void stmt(Node* s, std::vector<Statement*>& out);
    Expression* cond(Node* e);
    Expression* case_label(Node* lab, Node* scrutinee);
    BlockStatement* loop_body(Node* loop, const ir::Suite& body);
    Statement* for_stmt(const ir::For& s, Node* node);
    std::pair<bool, bool> label_uses(const std::vector<Node*>& body, const std::string& label);
    void local_stmt(const ir::Local& s, std::vector<Statement*>& out);
    CatchClause* catch_clause(const ir::Catch& c, const TypeP& t);
    Statement* using_stmt(std::span<Node* const> res, Node* body);
    Statement* try_stmt(const ir::Try& s);
    Statement* switch_stmt(const ir::Switch& s);
    std::vector<Node*> initializers(const std::vector<Node*>& in);

    // ---- expressions
    Expression* expr(Node* e, bool statement = false);
    Expression* dispatch(Node* e);
    Expression* coerce(Node* e, const TypeP& t);
    Expression* call_expr(Node* e, const ir::Call& c);
    Expression* literal(const ir::Lit& l, const TypeP& t);
    std::vector<Expression*> args(std::span<Node* const> as, const ir::Sym* s);
    Expression* null_conditional(const ir::Cond& e);
    Expression* interpolate(const std::vector<Node*>& parts);
    Expression* char_lit(char32_t cp);

    L2CSharpProgram& prog_;
    ir::Unit& u_;
    std::string lang_;
    bool java_ = false;     // a Java unit the rule set restated
    bool csharp_ = false;   // a C# unit: the source's own forms are put back (initialisers, expression bodies, `?.`)
    std::set<std::string> usings_;
    ir::ClassDecl* cls_ = nullptr;
    ir::MethodDecl* method_ = nullptr;
    std::unordered_set<std::string> locals_;
    std::vector<std::unordered_set<std::string>> block_names_;
    std::vector<std::map<std::string, std::string>> scopes_;
    std::unordered_map<const Node*, InitGroup> init_uses_;
};

// ---- scopes -------------------------------------------------------------------------------------------------------------

std::string UnitBuilder::declare(const std::string& name, bool nested) {
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
    return clean(written);
}

std::string UnitBuilder::lookup(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        if (const auto f = it->find(name); f != it->end()) return clean(f->second);
    }
    return clean(name);
}

void UnitBuilder::note(const std::string& kind, const std::string& why) {
    std::string where = cls_ != nullptr ? cls_->qname.value_or("") : u_.path.value_or("");
    if (method_ != nullptr && method_->name && !method_->name->empty()) where += "." + *method_->name;
    prog_.note(L2Gap{where, kind, why});
}

TypeP UnitBuilder::type_of(Node* e) const {
    if (const auto it = init_uses_.find(e); it != init_uses_.end()) return plain_type_of(it->second.made);
    return plain_type_of(e);
}

// ---- types --------------------------------------------------------------------------------------------------------------

AstType* UnitBuilder::type(const TypeP& t) {
    if (!t) return keyword_type("void");
    struct Visitor {
        UnitBuilder& w;
        const TypeP& t;
        AstType* operator()(const ir::PrimType& p) const {
            switch (p.prim) {
            case ir::Prim::Bool: return keyword_type("bool");
            case ir::Prim::I8: return keyword_type("sbyte");
            case ir::Prim::U8: return keyword_type("byte");
            case ir::Prim::I16: return keyword_type("short");
            case ir::Prim::I32: return keyword_type("int");
            case ir::Prim::U32: return keyword_type("uint");
            case ir::Prim::I64: return keyword_type("long");
            case ir::Prim::U64: return keyword_type("ulong");
            case ir::Prim::F32: return keyword_type("float");
            case ir::Prim::F64: return keyword_type("double");
            case ir::Prim::Char: return keyword_type("byte");   // C's plain char
            case ir::Prim::U16: return keyword_type(p.encoding ? "char" : "ushort");
            case ir::Prim::F80: break;
            }
            throw Declined{"type", "the type " + ir::to_text(*t)};
        }
        AstType* operator()(const ir::VoidType&) const { return keyword_type("void"); }
        AstType* operator()(const ir::NullType&) const { return keyword_type("object"); }
        AstType* operator()(const ir::DynamicType&) const { return keyword_type("dynamic"); }
        AstType* operator()(const ir::TypeVarType& v) const { return make<SimpleType>(clean(v.name)); }
        AstType* operator()(const ir::WildcardType& wc) const {
            if (wc.bound && wc.bound->mode == ir::BoundMode::Extends) return w.type(wc.bound->type);
            return keyword_type("object");
        }
        AstType* operator()(const ir::ArrayType& a) const {
            std::vector<std::uint32_t> ranks{a.rank};
            TypeP inner = a.elem;
            while (const auto* ia = inner ? std::get_if<ir::ArrayType>(&inner->body) : nullptr) {
                ranks.push_back(ia->rank);
                inner = ia->elem;
            }
            auto* composed = make<ComposedType>();
            composed->set_base_type(w.type(inner));
            for (const std::uint32_t r : ranks) composed->array_specifiers().add(make<ArraySpecifier>(static_cast<int>(r == 0 ? 1 : r)));
            return composed;
        }
        AstType* operator()(const ir::ClassType& c) const { return w.class_type(c); }
        AstType* operator()(const ir::AnyType&) const { return other(); }
        AstType* operator()(const ir::PtrType&) const { return other(); }
        AstType* operator()(const ir::SelectorType&) const { return other(); }
        AstType* operator()(const ir::FnType&) const { return other(); }
        AstType* operator()(const ir::FnPtrType&) const { return other(); }
        AstType* operator()(const ir::RecordType&) const { return other(); }
        AstType* operator()(const ir::TupleType&) const { return other(); }
        AstType* operator()(const ir::ModType&) const { return other(); }
        [[noreturn]] AstType* other() const { throw Declined{"type", "the type " + ir::to_text(*t)}; }
    };
    return std::visit(Visitor{*this, t}, t->body);
}

AstType* UnitBuilder::class_type(const ir::ClassType& c) {
    const auto nullable = [&](AstType* inner) -> AstType* {
        if (!c.nullable) return inner;
        auto* composed = make<ComposedType>();
        composed->set_base_type(inner);
        composed->set_has_nullable_specifier(true);
        return composed;
    };
    if (c.args.empty()) {
        if (const auto al = alias_table().find(c.name); al != alias_table().end()) return nullable(keyword_type(al->second));
    }
    if (c.name == "System.Nullable" && c.args.size() == 1) {
        auto* composed = make<ComposedType>();
        composed->set_base_type(type(c.args[0]));
        composed->set_has_nullable_specifier(true);
        return composed;
    }
    const std::string simple = short_name(c.name);
    std::vector<AstType*> targs;
    for (const TypeP& a : c.args) targs.push_back(type(a));
    // more arguments than the class declares: a class nested in a generic one (Java's Outer<T>.Inner), whose name C# writes as
    // Outer<int>.Inner; inside the outer class, its arguments are the outer's own and are left out
    if (const ir::ClassDecl* d = prog_.class_named(c.name); d != nullptr && d->typeparams.size() < c.args.size()) {
        std::vector<const ir::ClassDecl*> chain{d};   // innermost first
        while (chain.back()->outer) {
            const ir::ClassDecl* o = prog_.class_named(*chain.back()->outer);
            if (o == nullptr) break;
            chain.push_back(o);
        }
        const std::vector<std::string_view> parts = text::split(simple, '.');
        if (parts.size() <= chain.size()) {
            std::size_t total = 0;
            for (std::size_t i = 0; i < parts.size(); ++i) total += chain[i]->typeparams.size();
            if (total <= c.args.size()) {
                std::size_t next = targs.size() - total;
                AstType* out = nullptr;
                for (std::size_t k = 0; k < parts.size(); ++k) {
                    const ir::ClassDecl* level = chain[parts.size() - 1 - k];
                    std::vector<AstType*> own(targs.begin() + static_cast<std::ptrdiff_t>(next),
                                              targs.begin() + static_cast<std::ptrdiff_t>(next + level->typeparams.size()));
                    next += level->typeparams.size();
                    const std::string part(parts[k]);
                    out = out == nullptr ? static_cast<AstType*>(make<SimpleType>(part, own)) : static_cast<AstType*>(make<MemberType>(out, part, own));
                }
                return nullable(out);
            }
        }
    }
    return nullable(named(simple, targs));
}

// A dotted name ("Outer.Inner", "global::java.util.Stack") as a type, the arguments on its last part.
AstType* UnitBuilder::named(const std::string& dotted, const std::vector<AstType*>& args) {
    std::string rest = dotted;
    AstType* out = nullptr;
    bool double_colon = false;
    if (rest.starts_with("global::")) {
        out = make<SimpleType>("global");
        rest = rest.substr(8);
        double_colon = true;
    }
    const std::vector<std::string_view> parts = text::split(rest, '.');
    for (std::size_t i = 0; i < parts.size(); ++i) {
        const std::string part(parts[i]);
        const bool last = i + 1 == parts.size();
        if (out == nullptr) {
            out = last ? make<SimpleType>(part, args) : make<SimpleType>(part);
        } else {
            auto* m = last ? make<MemberType>(out, part, args) : make<MemberType>(out, part);
            if (double_colon) {
                m->set_is_double_colon(true);
                double_colon = false;
            }
            out = m;
        }
    }
    return out;
}

// A type's name as written in this unit: relative to its namespace, with a using for it.
std::string UnitBuilder::short_name(const std::string& qname) {
    if (ir::ClassDecl* c = prog_.class_named(qname)) {
        std::vector<ir::ClassDecl*> chain{c};
        while (chain.back()->outer) {
            ir::ClassDecl* o = prog_.class_named(*chain.back()->outer);
            if (o == nullptr) break;
            chain.push_back(o);
        }
        const ir::ClassDecl& top = *chain.back();
        const std::string ns = ends_with_name(top) ? drop_suffix(*top.qname, *top.name) : std::string();
        std::vector<std::string> relp;
        for (auto it = chain.rbegin(); it != chain.rend(); ++it) relp.push_back(clean((*it)->name.value_or("")));
        const std::string rel = text::join(relp, ".");
        if (java_ && ns.starts_with("java.") && (rel == "Stack" || rel == "Queue") && ns != namespace_of(cls_)) {   // (.NET has one of the name in System.Collections.Generic)
            std::vector<std::string> parts;
            for (std::string_view part : text::split(ns, '.')) parts.push_back(clean(std::string(part)));
            return "global::" + text::join(parts, ".") + "." + rel;
        }
        if (!ns.empty() && ns != namespace_of(cls_)) usings_.insert(ns);
        // inside the class itself or its outer classes, the nested name alone
        for (const ir::ClassDecl* cur = cls_; cur != nullptr; cur = cur->outer ? prog_.class_named(*cur->outer) : nullptr) {
            if (c->outer && cur->qname && *cur->qname == *c->outer) return clean(c->name.value_or(""));
        }
        return rel;
    }
    if (contains(qname, ".")) {
        const auto [owner, name] = rsplit_dot(qname);
        if (prog_.class_named(owner) != nullptr) return short_name(owner) + "." + clean(name);
        usings_.insert(owner);
        return clean(name);
    }
    return clean(qname);
}

std::string UnitBuilder::namespace_of(const ir::ClassDecl* c0) {
    if (c0 == nullptr) return u_.package.value_or("");
    const ir::ClassDecl* c = c0;
    while (c->outer && prog_.class_named(*c->outer) != nullptr) c = prog_.class_named(*c->outer);
    return ends_with_name(*c) ? drop_suffix(*c->qname, *c->name) : std::string();
}

// ---- the unit -------------------------------------------------------------------------------------------------------------

SyntaxTree* UnitBuilder::unit() {
    auto* tree = make<SyntaxTree>();
    // consecutive classes of one namespace share its declaration
    std::vector<std::pair<std::string, std::vector<Node*>>> groups;
    for (Node* c : u_.types) {
        const auto* cd = ir::get<ir::ClassDecl>(c);
        const std::string ns = cd != nullptr ? namespace_of(cd) : std::string();
        if (!groups.empty() && groups.back().first == ns) groups.back().second.push_back(c);
        else groups.emplace_back(ns, std::vector<Node*>{c});
    }
    std::vector<AstNode*> tops;
    std::vector<Comment*> trailing;   // declined classes after the last one written
    for (auto& [ns, nodes] : groups) {
        std::vector<AstNode*> written;
        std::vector<Comment*> pending;
        for (Node* c : nodes) {
            auto* cd = ir::get<ir::ClassDecl>(c);
            try {
                if (const auto* un = ir::get<ir::Unsupported>(c)) throw Declined{un->kind.value_or(""), un->raw.value_or(un->kind.value_or(""))};
                if (cd == nullptr) throw Declined{"member", "a unit member " + describe(c)};
                if (!csharp_ && !java_) throw Declined{"library", "no rule set for " + lang_ + " source in the plain lowering yet"};
                EntityDeclaration* d = class_decl(*cd);
                for (Comment* p : pending) d->add_leading_trivia(p);
                pending.clear();
                written.push_back(d);
            } catch (const Declined& d) {
                note(d.kind, d.why);
                pending.push_back(line_comment(cd != nullptr ? "CB-GAP: " + cd->name.value_or("") + " was not translated: " + d.why : "CB-GAP: " + d.why));
            }
        }
        if (!ns.empty()) {
            auto* decl = make<NamespaceDeclaration>(ns);
            for (AstNode* n : written) decl->members().add(n);
            if (!pending.empty()) {
                if (written.empty()) {
                    for (Comment* p : pending) decl->add_leading_trivia(p);
                } else {
                    for (Comment* p : pending) written.back()->add_trailing_trivia(p);
                }
            }
            tops.push_back(decl);
        } else {
            if (!pending.empty()) {
                if (!written.empty()) {
                    for (Comment* p : pending) written.back()->add_trailing_trivia(p);
                } else {
                    trailing.insert(trailing.end(), pending.begin(), pending.end());
                }
            }
            tops.insert(tops.end(), written.begin(), written.end());
        }
    }
    std::set<std::string> own;
    for (Node* c : u_.types) {
        if (const auto* cd = ir::get<ir::ClassDecl>(c)) own.insert(namespace_of(cd));
    }
    for (const std::string& x : usings_) {   // (a set: sorted)
        if (x.empty() || own.contains(x)) continue;
        const bool parent_of_own = std::any_of(u_.types.begin(), u_.types.end(), [&](Node* c) {
            const auto* cd = ir::get<ir::ClassDecl>(c);
            return cd != nullptr && namespace_of(cd).starts_with(x + ".");
        });
        if (!parent_of_own) tree->members().add(make<UsingDeclaration>(x));
    }
    for (AstNode* n : tops) tree->members().add(n);
    const std::string path_name = u_.path && !u_.path->empty() ? *u_.path : "?";
    Comment* head = line_comment("Translated by CodeBreaker from " + fs::to_text(fs::from_text(path_name).filename()) + " (" + lang_ + ").");
    if (AstNode* first = tree->first_child()) {
        first->prepend_leading_trivia(head);
        for (Comment* t : trailing) tree->last_child()->add_trailing_trivia(t);
    } else {
        tree->add_leading_trivia(head);
        for (Comment* t : trailing) tree->add_trailing_trivia(t);
    }
    return tree;
}

// ---- declarations -------------------------------------------------------------------------------------------------------------

EntityDeclaration* UnitBuilder::class_decl(ir::ClassDecl& c) {
    struct Restore {
        ir::ClassDecl*& slot;
        ir::ClassDecl* saved;
        ~Restore() { slot = saved; }
    } restore{cls_, cls_};
    cls_ = &c;
    return class_decl_body(c);
}

void UnitBuilder::type_parameters(const std::vector<std::string>& names, const std::vector<Node*>& tparams, AstNodeCollection<TypeParameterDeclaration>& out) {
    for (const std::string& n : names) {
        auto variance = typesystem::VarianceModifier::Invariant;
        for (Node* tp : tparams) {
            const auto* t = ir::get<ir::TypeParam>(tp);
            if (t == nullptr || t->name != n || !t->variance) continue;
            if (*t->variance == "Out") variance = typesystem::VarianceModifier::Covariant;
            else if (*t->variance == "In") variance = typesystem::VarianceModifier::Contravariant;
        }
        out.add(make<TypeParameterDeclaration>(std::vector<AttributeSection*>{}, variance, clean(n)));
    }
}

Constraint* UnitBuilder::constraint(Node* tpn) {
    const auto* tp = ir::get<ir::TypeParam>(tpn);
    if (tp == nullptr) return nullptr;
    std::vector<AstType*> parts;
    if (has(tp->flags, "class")) parts.push_back(keyword_type("class"));
    if (has(tp->flags, "struct")) parts.push_back(keyword_type("struct"));
    for (const TypeP& b : tp->bounds) {
        const std::string* n = class_of(b);
        if (n != nullptr && (*n == "java.lang.Object" || *n == "System.Object")) continue;
        parts.push_back(type(b));
    }
    if (has(tp->flags, "new")) parts.push_back(keyword_type("new"));
    if (parts.empty()) return nullptr;
    return make<Constraint>(make<SimpleType>(clean(tp->name.value_or(""))), parts);
}

EntityDeclaration* UnitBuilder::class_decl_body(ir::ClassDecl& c) {
    const std::string ckind = c.kind.value_or("");
    std::vector<std::string> cmods;
    for (const std::string& m : c.mods) {
        if (m != "IMPLICIT") cmods.push_back(m);
    }
    if (ckind == "delegate") {
        ir::MethodDecl* invoke = nullptr;
        for (Node* m : c.members) {
            if ((invoke = ir::get<ir::MethodDecl>(m)) != nullptr) break;
        }
        if (invoke == nullptr) throw Declined{"delegate", "a delegate with no Invoke method"};
        auto* d = make<DelegateDeclaration>(std::vector<AttributeSection*>{}, type(invoke->ret), Identifier::create(clean(c.name.value_or(""))));
        d->set_modifiers(modifiers_of(cmods));
        type_parameters(c.typeparams, c.tparams, d->type_parameters());
        for (ParameterDeclaration* p : params(invoke->params)) d->parameters().add(p);
        return d;
    }
    static const std::map<std::string_view, ClassType> kinds = {{"class", ClassType::Class}, {"interface", ClassType::Interface},
                                                                {"enum", ClassType::Enum}, {"struct", ClassType::Struct},
                                                                {"record", ClassType::RecordClass}, {"record struct", ClassType::RecordStruct}};
    const auto kind = kinds.find(ckind);
    if (kind == kinds.end()) throw Declined{"class-kind", "a " + c.kind.value_or("")};
    auto* decl = make<TypeDeclaration>(kind->second, std::vector<AttributeSection*>{}, Identifier::create(clean(c.name.value_or(""))));
    decl->set_modifiers(modifiers_of(cmods));
    type_parameters(c.typeparams, c.tparams, decl->type_parameters());
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
        decl->set_has_primary_constructor(true);
        for (ParameterDeclaration* p : params(primary->params)) decl->primary_constructor_parameters().add(p);
        for (Node* p : primary->params) {
            const auto* pp = ir::get<ir::Param>(p);
            if (pp != nullptr && pp->sym) positional.insert(pp->sym->name);
        }
    }
    if (ckind == "enum") {
        if (c.extends) decl->base_types().add(type(c.extends));
    } else {
        if (c.extends) {
            AstType* b = type(c.extends);
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
                if (call != nullptr) b = make<InvocationAstType>(args(call->args, call->sym.get()), b);
            }
            decl->base_types().add(b);
        }
        for (const TypeP& i : c.implements) decl->base_types().add(type(i));
    }
    for (Node* tp : c.tparams) {
        if (Constraint* k = constraint(tp)) decl->constraints().add(k);
    }
    std::vector<Node*> members;
    for (Node* m : c.members) {
        const auto* pr = ir::get<ir::Property>(m);
        if (pr != nullptr && has(pr->mods, "positional") && pr->sym && positional.contains(pr->sym->name)) continue;
        if (primary_node != nullptr && m == primary_node) continue;
        members.push_back(m);
    }
    if (ckind == "enum") {
        std::vector<Node*> rest;
        for (Node* m : members) {
            if (const auto* e = ir::get<ir::EnumConst>(m)) {
                if (!e->value && (!e->args.empty() || e->body != nullptr)) throw Declined{"enum", "an enum constant with constructor arguments or a body"};
                auto* em = make<EnumMemberDeclaration>(std::vector<AttributeSection*>{}, Identifier::create(clean(e->sym->name)));
                if (e->value) {
                    const std::int64_t v = *e->value;
                    if (v >= std::numeric_limits<std::int32_t>::min() && v <= std::numeric_limits<std::int32_t>::max()) em->set_initializer(int_lit(static_cast<std::int32_t>(v)));
                    else em->set_initializer(make<PrimitiveExpression>(typesystem::ConstantValue(v)));
                }
                decl->members().add(em);
            } else {
                rest.push_back(m);
            }
        }
        if (!rest.empty()) throw Declined{"enum", "an enum with members"};
        return decl;
    }
    std::vector<Comment*> pending;
    for (Node* m : members) {
        EntityDeclaration* d = member_decl(m, c, pending);
        if (d == nullptr) continue;
        for (Comment* p : pending) d->add_leading_trivia(p);
        pending.clear();
        decl->members().add(d);
    }
    if (!pending.empty()) {
        if (decl->members().empty()) {
            for (Comment* p : pending) decl->add_trailing_trivia(p);
        } else {
            for (Comment* p : pending) decl->members().last_or_null()->add_trailing_trivia(p);
        }
    }
    return decl;
}

EntityDeclaration* UnitBuilder::member_decl(Node* m, ir::ClassDecl& c, std::vector<Comment*>& pending) {
    if (auto* cd = ir::get<ir::ClassDecl>(m)) {
        try {
            return class_decl(*cd);
        } catch (const Declined& d) {
            note(d.kind, d.why);
            pending.push_back(line_comment("CB-GAP: " + cd->name.value_or("") + " was not translated: " + d.why));
            return nullptr;
        }
    }
    if (ir::is<ir::FieldDecl>(m)) return field(m);
    if (auto* p = ir::get<ir::Property>(m)) return prop(*p);
    if (ir::is<ir::MethodDecl>(m)) return method_decl(m, c);
    if (const auto* un = ir::get<ir::Unsupported>(m)) {
        const std::string why = un->raw && !un->raw->empty() ? *un->raw : un->kind.value_or("");
        note(un->kind.value_or(""), why);
        pending.push_back(line_comment("CB-GAP: " + why));
        return nullptr;
    }
    if (ir::is<ir::EnumConst>(m)) throw Declined{"enum", "an enum constant outside an enum"};
    throw Declined{"member", "a member " + describe(m)};
}

EntityDeclaration* UnitBuilder::field(Node* node) {
    auto& f = *ir::get<ir::FieldDecl>(node);
    auto* var = make<VariableInitializer>(clean(f.sym->name));
    auto* decl = make<FieldDeclaration>(std::vector<AttributeSection*>{}, type(f.type), std::vector<VariableInitializer*>{var});
    decl->set_modifiers(modifiers_of(f.mods));
    if (f.init != nullptr) {
        method_ = nullptr;
        try {
            var->set_initializer(coerce(f.init, f.type));
        } catch (const Declined& d) {
            note(d.kind, d.why);
            auto* dflt = make<DefaultValueExpression>(type(f.type));
            dflt->add_trailing_trivia(make<Comment>(" CB-GAP: " + d.why + " ", CommentType::MultiLine));
            var->set_initializer(dflt);
        }
    }
    return decl;
}

EntityDeclaration* UnitBuilder::prop(ir::Property& p) {
    std::vector<std::string> mods;
    for (const std::string& m : p.mods) {
        if (m != "positional") mods.push_back(m);
    }
    const auto accessor_of = [](Node* a) -> ir::MethodDecl& { return *ir::get<ir::MethodDecl>(a); };
    const auto body_stmts = [](ir::MethodDecl& a) {
        std::vector<Node*> out;
        for (Node* s : ir::statements(a.body)) {
            if (const auto* blk = ir::get<ir::Block>(s)) out.insert(out.end(), blk->stmts.begin(), blk->stmts.end());
            else out.push_back(s);
        }
        return out;
    };
    const bool indexer = !p.params.empty();
    PropertyDeclaration* property = nullptr;
    IndexerDeclaration* index = nullptr;
    EntityDeclaration* decl = nullptr;
    if (indexer) {
        index = make<IndexerDeclaration>(std::vector<AttributeSection*>{}, type(p.type));
        for (ParameterDeclaration* pd : params(p.params)) index->parameters().add(pd);
        decl = index;
    } else {
        property = make<PropertyDeclaration>(std::vector<AttributeSection*>{}, type(p.type), Identifier::create(clean(p.sym->name)));
        decl = property;
    }
    decl->set_modifiers(modifiers_of(mods));
    // a lone getter that returns an expression: `T P => expr;`
    if (csharp_ && p.accessors.size() == 1) {
        ir::MethodDecl& a0 = accessor_of(p.accessors[0]);
        if (a0.kind == "get" && ir::present(a0.body) && a0.mods.empty()) {
            const auto b = body_stmts(a0);
            if (b.size() == 1) {
                const auto* r = ir::get<ir::Return>(b[0]);
                if (r != nullptr && r->expr != nullptr) {
                    try {
                        method_ = &a0;
                        Expression* value = expr(r->expr);
                        if (property != nullptr) property->set_expression_body(value);
                        else index->set_expression_body(value);
                        return decl;
                    } catch (const Declined&) {
                        // (written the long way below)
                    }
                }
            }
        }
    }
    for (Node* an : p.accessors) {
        ir::MethodDecl& a = accessor_of(an);
        const std::string k = a.kind.value_or("");
        auto* acc = make<Accessor>(k == "get" ? AccessorKind::Getter : k == "init" ? AccessorKind::Init : AccessorKind::Setter);
        acc->set_modifiers(modifiers_of(a.mods));
        if (ir::present(a.body)) {
            method_ = &a;
            acc->set_body(body_block(a.body, &a));
            const auto st = body_stmts(a);
            const auto* ret = st.size() == 1 ? ir::get<ir::Return>(st[0]) : nullptr;
            acc->set_expression_bodied(csharp_ && ((ret != nullptr && ret->expr != nullptr) || (st.size() == 1 && ir::is<ir::ExprStmt>(st[0]))));   // `get => expr;`
        }
        if (acc->accessor_kind() == AccessorKind::Getter) {
            if (property != nullptr) property->set_getter(acc);
            else index->set_getter(acc);
        } else {
            if (property != nullptr) property->set_setter(acc);
            else index->set_setter(acc);
        }
    }
    if (p.init != nullptr && property != nullptr) {
        method_ = nullptr;
        property->set_initializer(coerce(p.init, p.type));
    }
    return decl;
}

std::vector<ParameterDeclaration*> UnitBuilder::params(const std::vector<Node*>& ps) {
    std::vector<ParameterDeclaration*> out;
    for (Node* pn : ps) {
        const auto* p = ir::get<ir::Param>(pn);
        if (p == nullptr) throw Declined{"param", "a parameter " + describe(pn)};
        auto kind = typesystem::ReferenceKind::None;
        if (has(p->mods, "ref")) kind = typesystem::ReferenceKind::Ref;
        else if (has(p->mods, "out")) kind = typesystem::ReferenceKind::Out;
        else if (has(p->mods, "in")) kind = typesystem::ReferenceKind::In;
        if (kind == typesystem::ReferenceKind::None && p->sym && !p->sym->refs.empty()) kind = pass_kind(p->sym->refs[0]);
        auto* d = make<ParameterDeclaration>(std::vector<AttributeSection*>{}, kind, type(p->type), clean(p->sym ? p->sym->name : "p"));
        d->set_is_params(has(p->mods, "params"));
        d->set_has_this_modifier(has(p->mods, "this"));
        d->set_is_scoped_ref(has(p->mods, "scoped"));
        out.push_back(d);
    }
    return out;
}

EntityDeclaration* UnitBuilder::method_decl(Node* node, ir::ClassDecl& c) {
    auto& m = *ir::get<ir::MethodDecl>(node);
    method_ = &m;
    const std::string mkind = m.kind.value_or("");
    const std::string cname = clean(c.name.value_or(""));
    if (mkind == "static_init" || mkind == "cctor") {
        auto* ctor = make<ConstructorDeclaration>(std::vector<AttributeSection*>{}, Identifier::create(cname));
        ctor->set_modifiers(Modifiers::Static);
        ctor->set_body(body_block(m.body, &m));
        return ctor;
    }
    if (mkind == "init") return nullptr;
    if (mkind == "ctor") {
        if (has(m.mods, "IMPLICIT")) return nullptr;
        std::vector<std::string> mods;
        for (const std::string& x : m.mods) {
            if (x != "IMPLICIT") mods.push_back(x);
        }
        auto* ctor = make<ConstructorDeclaration>(std::vector<AttributeSection*>{}, Identifier::create(cname), params(m.params));
        ctor->set_modifiers(modifiers_of(mods));
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
                    ctor->set_initializer(make<ConstructorInitializer>(cc->kind == "this" ? ConstructorInitializerType::This : ConstructorInitializerType::Base,
                                                                       args(cc->args, cc->sym.get())));
                } catch (const Declined& d) {
                    ctor->set_body(gap(d));
                    return ctor;
                }
            }
        }
        ctor->set_body(body_block(ir::Suite(std::move(stmts)), &m));
        return ctor;
    }
    std::vector<std::string> mods = m.mods;
    std::erase(mods, "iterator");   // (the IR's note that the body holds `yield`: C# says it by the statement alone)
    const std::string mname = m.name.value_or("");
    BlockStatement* body = ir::present(m.body) ? body_block(m.body, &m) : nullptr;
    if (has(mods, "operator") && declared_operators().contains(mname)) {
        auto* op = make<OperatorDeclaration>(std::vector<AttributeSection*>{}, type(m.ret), nullptr, declared_operators().at(mname), params(m.params));
        op->set_modifiers(modifiers_of(mods));
        op->set_body(body);
        op->set_expression_bodied(csharp_ && single_return(m.body));
        return op;
    }
    auto* md = make<MethodDeclaration>();
    md->set_modifiers(modifiers_of(mods));
    md->set_return_type(type(m.ret));
    if (const std::size_t dot = mname.rfind('.'); dot != std::string::npos) {   // `Iface.Name`: an explicit implementation of the interface's method
        md->set_private_implementation_type(named(mname.substr(0, dot)));
        md->set_name(clean(mname.substr(dot + 1)));
    } else {
        md->set_name(clean(mname));
    }
    type_parameters(m.typeparams, m.tparams, md->type_parameters());
    for (ParameterDeclaration* p : params(m.params)) md->parameters().add(p);
    for (Node* tp : m.tparams) {
        if (Constraint* k = constraint(tp)) md->constraints().add(k);
    }
    md->set_body(body);
    md->set_expression_bodied(csharp_ && md->constraints().empty() && single_return(m.body));   // a lone `return expr;` of a C# method
    return md;
}

Expression* UnitBuilder::ret_expr(Node* e) { return coerce(e, method_ != nullptr ? method_->ret : TypeP()); }

BlockStatement* UnitBuilder::body_block(const ir::Suite& body, const ir::MethodDecl* m) {
    for (Node* s : ir::statements(body)) {
        const auto un = ir::unsupported(s);
        if (!un.empty()) return gap(declined_for(*ir::get<ir::Unsupported>(un[0])));
    }
    try {
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
        return sub(body);
    } catch (const Declined& d) {
        return gap(d);
    }
}

BlockStatement* UnitBuilder::gap(const Declined& d) {
    note(d.kind, d.why);
    return gap_block(d.why);
}

// ---- statements -----------------------------------------------------------------------------------------------------------

BlockStatement* UnitBuilder::block_of(Node* b) {
    if (const auto* blk = ir::get<ir::Block>(b)) return block_stmts(blk->stmts);
    return block_stmts(b != nullptr ? std::vector<Node*>{b} : std::vector<Node*>{});
}

BlockStatement* UnitBuilder::block_stmts(std::vector<Node*> stmts) {
    stmts = initializers(stmts);
    std::unordered_set<std::string> names;
    for (Node* x : stmts) {
        if (const auto* l = ir::get<ir::Local>(x)) {
            if (l->sym) names.insert(l->sym->name);
        }
    }
    block_names_.push_back(std::move(names));
    struct Pop {
        UnitBuilder& w;
        ~Pop() {
            w.pop();
            w.block_names_.pop_back();
        }
    } pop_on_exit{(push(), *this)};
    std::vector<Statement*> out;
    for (Node* s : stmts) stmt(s, out);
    return make<BlockStatement>(out);
}

BlockStatement* UnitBuilder::sub(const ir::Suite& s) {
    if (const auto* one = std::get_if<Node*>(&s)) return block_of(*one);
    return block_stmts(std::get<std::vector<Node*>>(s));
}

Expression* UnitBuilder::cond(Node* e) { return coerce(strip(e), ir::bool_type()); }

// A loop's body, with the label a `continue label` jumps to at its end. (C# has labelled jumps only as goto; from C# source there is no
// labelled continue, so the body is the loop's own.)
BlockStatement* UnitBuilder::loop_body(Node*, const ir::Suite& body) { return sub(body); }

Statement* UnitBuilder::for_stmt(const ir::For& s, Node* node) {
    std::vector<Statement*> inits;
    AstType* decl_type = nullptr;
    std::string decl_text;
    VariableDeclarationStatement* declared = nullptr;
    for (Node* i : s.init) {
        if (const auto* l = ir::get<ir::Local>(i)) {
            Expression* v = l->init != nullptr ? coerce(l->init, l->type) : nullptr;
            AstType* t = type(l->type);
            const std::string text_of_type = ir::to_text(*l->type);
            const std::string d = declare(l->sym->name, true);
            if (declared == nullptr) {
                if (!inits.empty()) throw Declined{"for", "a for initialiser mixing declarations and expressions"};
                declared = make<VariableDeclarationStatement>(Modifiers::None, t);
                decl_type = t;
                decl_text = text_of_type;
                inits.push_back(declared);
            } else if (text_of_type != decl_text) {
                throw Declined{"for", "a for initialiser declaring locals of several types"};
            }
            declared->variables().add(make<VariableInitializer>(d, v));
        } else if (const auto* es = ir::get<ir::ExprStmt>(i)) {
            if (declared != nullptr) throw Declined{"for", "a for initialiser mixing declarations and expressions"};
            inits.push_back(make<ExpressionStatement>(expr(es->expr, true)));
        } else {
            throw Declined{"for", "a for initialiser " + describe(i)};
        }
    }
    (void)decl_type;
    Expression* c = s.cond != nullptr ? cond(s.cond) : nullptr;
    std::vector<Statement*> ups;
    for (Node* u : s.update) ups.push_back(make<ExpressionStatement>(expr(u, true)));
    return make<ForStatement>(inits, c, ups, loop_body(node, s.body));
}

std::pair<bool, bool> UnitBuilder::label_uses(const std::vector<Node*>& body, const std::string& label) {
    std::pair<bool, bool> found{false, false};
    for (Node* root : body) {
        for (Node* n : ir::walk(root)) {
            if (const auto* b = ir::get<ir::Break>(n); b != nullptr && b->label == label) found.first = true;
            if (const auto* c = ir::get<ir::Continue>(n); c != nullptr && c->label == label) found.second = true;
        }
    }
    return found;
}

void UnitBuilder::local_stmt(const ir::Local& s, std::vector<Statement*>& out) {
    const Modifiers mods = has(s.mods, "const") ? Modifiers::Const : Modifiers::None;
    if (s.init == nullptr) {
        const std::string name = declare(s.sym->name);
        out.push_back(make<VariableDeclarationStatement>(mods, type(s.type), std::vector<VariableInitializer*>{make<VariableInitializer>(name)}));
        return;
    }
    Expression* init = coerce(s.init, s.type);
    const std::string name = declare(s.sym->name);
    Node* si = strip(s.init);
    const bool made = ir::is<ir::New>(si) || init_uses_.contains(si);
    AstType* t = made && type_of(s.init) == s.type && mods == Modifiers::None ? make<SimpleType>("var") : type(s.type);
    out.push_back(make<VariableDeclarationStatement>(mods, t, std::vector<VariableInitializer*>{make<VariableInitializer>(name, init)}));
}

CatchClause* UnitBuilder::catch_clause(const ir::Catch& c, const TypeP& t) {
    if (!t) return make<CatchClause>(nullptr, "", nullptr, block_of(c.body));
    AstType* tn = type(t);
    if (c.sym) {
        Scope scope(*this);
        const std::string d = declare(c.sym->name, true);
        return make<CatchClause>(tn, d, nullptr, block_of(c.body));
    }
    return make<CatchClause>(tn, "", nullptr, block_of(c.body));
}

Statement* UnitBuilder::using_stmt(std::span<Node* const> res, Node* body) {
    Node* r = res[0];
    AstNode* resource = nullptr;
    std::optional<Scope> scope;
    if (const auto* loc = ir::get<ir::Local>(r)) {
        Expression* init = coerce(loc->init, loc->type);
        scope.emplace(*this);
        Node* si = strip(loc->init);
        AstType* tn = ir::is<ir::New>(si) && type_of(loc->init) == loc->type ? make<SimpleType>("var") : type(loc->type);
        const std::string d = declare(loc->sym->name, true);
        resource = make<VariableDeclarationStatement>(Modifiers::None, tn, std::vector<VariableInitializer*>{make<VariableInitializer>(d, init)});
    } else if (const auto* es = ir::get<ir::ExprStmt>(r)) {
        resource = expr(es->expr);
    } else {
        throw Declined{"using", "a resource " + describe(r)};
    }
    Statement* inner = res.size() > 1 ? make<BlockStatement>(std::vector<Statement*>{using_stmt(res.subspan(1), body)}) : static_cast<Statement*>(block_of(body));
    return make<UsingStatement>(resource, inner);
}

Statement* UnitBuilder::try_stmt(const ir::Try& s) {
    if (!s.resources.empty() && s.catches.empty() && s.finally_body == nullptr) return using_stmt(s.resources, s.body);
    BlockStatement* tried = !s.resources.empty() ? make<BlockStatement>(std::vector<Statement*>{using_stmt(s.resources, s.body)}) : block_of(s.body);
    auto* t = make<TryCatchStatement>(tried);
    for (Node* cn : s.catches) {
        const auto* c = ir::get<ir::Catch>(cn);
        if (c->types.size() > 1) {
            for (const TypeP& ty : c->types) t->catch_clauses().add(catch_clause(*c, ty));   // a multi-catch: one clause per type, each with the same body
        } else {
            t->catch_clauses().add(catch_clause(*c, c->types.empty() ? TypeP() : c->types[0]));
        }
    }
    if (s.finally_body != nullptr) t->set_finally_block(block_of(s.finally_body));
    return t;
}

Expression* UnitBuilder::case_label(Node* lab0, Node* scrutinee) {
    Node* lab = strip(lab0);
    if (const auto* sel = ir::get<ir::Select>(lab)) {
        if (sel->sym && sel->sym->kind == ir::SymKind::EnumConst) return member(make<TypeReferenceExpression>(type(ir::class_type(sel->sym->owner.value_or("")))), sel->sym->name);
    }
    if (const auto* l = ir::get<ir::Lit>(lab)) {
        if (const auto* iv = std::get_if<std::int64_t>(&l->value)) {
            if (scrutinee != nullptr && is_prim(type_of(scrutinee), ir::Prim::U16)) return char_lit(static_cast<char32_t>(*iv));
        }
    }
    return expr(lab);
}

Statement* UnitBuilder::switch_stmt(const ir::Switch& s) {
    auto* sw = make<SwitchStatement>(expr(s.expr));
    SwitchSection* section = nullptr;
    for (std::size_t i = 0; i < s.cases.size(); ++i) {
        const auto* c = ir::get<ir::Case>(s.cases[i]);
        if (section == nullptr) section = make<SwitchSection>();
        for (Node* lab : c->labels) section->case_labels().add(make<CaseLabel>(case_label(lab, s.expr)));
        if (c->is_default) section->case_labels().add(make<CaseLabel>());
        if (c->body.empty() && i + 1 < s.cases.size()) continue;   // labels sharing the next section
        std::vector<Statement*> body;
        for (Node* x : c->body) stmt(x, body);
        if (!abrupt(c->body)) {
            if (i + 1 >= s.cases.size()) {
                body.push_back(make<BreakStatement>());
            } else {
                const auto* next = ir::get<ir::Case>(s.cases[i + 1]);
                if (!next->labels.empty()) body.push_back(make<GotoCaseStatement>(case_label(next->labels[0], s.expr)));
                else body.push_back(make<GotoDefaultStatement>());
            }
        }
        for (Statement* b : body) section->statements().add(b);
        sw->switch_sections().add(section);
        section = nullptr;
    }
    return sw;
}

// var __t = new T(); __t.A = 1; __t.Add(x); ... S(__t)  ->  S(new T { A = 1 }) / S(new T { x }): the statements a C# object or collection
// initialiser was hoisted into, put back as the initialiser. The tree is not changed: the one use of `__t` is recorded in `init_uses_`,
// where `dispatch` writes the initialiser in its place.
std::vector<Node*> UnitBuilder::initializers(const std::vector<Node*>& in) {
    std::vector<Node*> stmts = in;
    if (!csharp_) return stmts;
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

void UnitBuilder::stmt(Node* s, std::vector<Statement*>& out) {
    if (s == nullptr) throw Declined{"null", "a missing statement"};
    if (const auto* x = ir::get<ir::ExprStmt>(s)) {
        Node* e = strip(x->expr);
        if (const auto* un = ir::get<ir::Unsupported>(e)) throw declined_for(*un);
        out.push_back(make<ExpressionStatement>(expr(e, true)));
        return;
    }
    if (const auto* x = ir::get<ir::Local>(s)) return local_stmt(*x, out);
    if (const auto* x = ir::get<ir::Return>(s)) {
        out.push_back(x->expr == nullptr ? make<ReturnStatement>() : make<ReturnStatement>(ret_expr(x->expr)));
        return;
    }
    if (const auto* x = ir::get<ir::If>(s)) {
        Expression* c = cond(x->cond);
        BlockStatement* then = sub(x->then);
        Statement* orelse = nullptr;
        if (ir::present(x->orelse)) {
            const auto* one = std::get_if<Node*>(&x->orelse);
            if (one != nullptr && ir::is<ir::If>(*one)) {   // else if
                std::vector<Statement*> chain;
                stmt(*one, chain);
                orelse = chain.at(0);
            } else {
                orelse = sub(x->orelse);
            }
        }
        out.push_back(make<IfElseStatement>(c, then, orelse));
        return;
    }
    if (ir::is<ir::Block>(s)) {
        out.push_back(block_of(s));
        return;
    }
    if (ir::is<ir::Empty>(s)) {
        out.push_back(make<EmptyStatement>());
        return;
    }
    if (const auto* x = ir::get<ir::Comment>(s)) {
        // a comment stands before the statement after it; at the end of a block, after the one before it
        auto* e = make<EmptyStatement>();
        e->add_leading_trivia(line_comment(x->text.value_or("")));
        out.push_back(e);
        return;
    }
    if (const auto* x = ir::get<ir::While>(s)) {
        out.push_back(make<WhileStatement>(cond(x->cond), loop_body(s, x->body)));
        return;
    }
    if (const auto* x = ir::get<ir::DoWhile>(s)) {
        BlockStatement* body = loop_body(s, x->body);
        out.push_back(make<DoWhileStatement>(body, cond(x->cond)));
        return;
    }
    if (const auto* x = ir::get<ir::For>(s)) {
        Scope scope(*this);
        out.push_back(for_stmt(*x, s));
        return;
    }
    if (const auto* x = ir::get<ir::ForEach>(s)) {
        const auto* v = ir::get<ir::Local>(x->var);
        if (v == nullptr) throw Declined{"foreach", "a for-each variable " + describe(x->var)};
        Expression* collection = expr(x->iterable);
        Scope scope(*this);
        AstType* t = type(v->type);
        const std::string d = declare(v->sym->name, true);
        out.push_back(make<ForeachStatement>(t, make<SingleVariableDesignation>(d), collection, loop_body(s, ir::Suite(x->body))));
        return;
    }
    if (const auto* x = ir::get<ir::Labeled>(s)) {
        const std::string lab = clean(x->label.value_or(""));
        const auto* single = std::get_if<Node*>(&x->body);
        Node* bn = single != nullptr ? *single : nullptr;
        if (bn == nullptr && ir::present(x->body)) throw Declined{"list", "the statement list"};
        out.push_back(make<LabelStatement>(lab));
        stmt(bn, out);
        return;
    }
    if (const auto* x = ir::get<ir::Break>(s)) {
        if (x->label && !x->label->empty()) out.push_back(make<GotoStatement>(clean(*x->label) + "_break"));
        else out.push_back(make<BreakStatement>());
        return;
    }
    if (const auto* x = ir::get<ir::Continue>(s)) {
        if (x->label && !x->label->empty()) out.push_back(make<GotoStatement>(clean(*x->label) + "_continue"));
        else out.push_back(make<ContinueStatement>());
        return;
    }
    if (const auto* x = ir::get<ir::Goto>(s)) {
        out.push_back(make<GotoStatement>(clean(x->label.value_or(""))));
        return;
    }
    if (const auto* x = ir::get<ir::Throw>(s)) {
        out.push_back(x->expr == nullptr ? make<ThrowStatement>() : make<ThrowStatement>(expr(x->expr)));
        return;
    }
    if (const auto* x = ir::get<ir::Sync>(s)) {
        Expression* lock = expr(x->lock);
        out.push_back(make<LockStatement>(lock, block_of(x->body)));
        return;
    }
    if (const auto* x = ir::get<ir::Assert>(s)) {
        usings_.insert("System.Diagnostics");
        out.push_back(make<ExpressionStatement>(make<InvocationExpression>(member(ident_expr("Debug"), "Assert"), std::vector<Expression*>{cond(x->cond)})));
        return;
    }
    if (ir::is<ir::LocalClass>(s)) throw Declined{"local-class", "a class declared in a method"};
    if (ir::is<ir::CtorCall>(s)) throw Declined{"ctor-call", "a constructor call that is not the first statement"};
    if (const auto* x = ir::get<ir::Switch>(s)) {
        out.push_back(switch_stmt(*x));
        return;
    }
    if (const auto* x = ir::get<ir::Try>(s)) {
        out.push_back(try_stmt(*x));
        return;
    }
    if (ir::is<ir::Yield>(s)) throw Declined{"yield", "a switch-expression arm with statements"};
    if (const auto* x = ir::get<ir::YieldReturn>(s)) {
        out.push_back(make<YieldReturnStatement>(expr(x->expr)));
        return;
    }
    if (ir::is<ir::YieldBreak>(s)) {
        out.push_back(make<YieldBreakStatement>());
        return;
    }
    if (const auto* x = ir::get<ir::MachineFragment>(s)) throw Declined{"machine", "machine code (" + (x->reason && !x->reason->empty() ? *x->reason : std::string("asm")) + ")"};
    const std::string k = describe(s);
    throw Declined{k, "the statement " + k};
}

// ---- expressions -------------------------------------------------------------------------------------------------------------

Expression* UnitBuilder::char_lit(char32_t cp) {
    if (cp <= 0xFFFF) return make<PrimitiveExpression>(typesystem::ConstantValue(static_cast<char16_t>(cp)));
    return make<CastExpression>(keyword_type("char"), make<PrimitiveExpression>(typesystem::ConstantValue(static_cast<std::int32_t>(cp))));
}

Expression* UnitBuilder::coerce(Node* e, const TypeP& t) {
    const TypeP et = type_of(e);
    const auto tp = prim_of(t);
    const auto ep = prim_of(et);
    // a char from an int, as a constant converts implicitly
    if (tp == ir::Prim::U16 && ep && (*ep == ir::Prim::I32 || *ep == ir::Prim::I8 || *ep == ir::Prim::I16 || *ep == ir::Prim::U8) &&
        std::get<ir::PrimType>(t->body).encoding) {
        if (const auto* l = ir::get<ir::Lit>(strip(e))) {
            if (const auto* iv = std::get_if<std::int64_t>(&l->value)) return char_lit(static_cast<char32_t>(*iv));
            if (const auto* bv = std::get_if<bool>(&l->value)) return char_lit(*bv ? 1 : 0);
        }
        return make<CastExpression>(keyword_type("char"), expr(e));
    }
    if (java_) {
        // a char constant narrowed to a byte or short is its number
        if ((tp == ir::Prim::I8 || tp == ir::Prim::I16 || tp == ir::Prim::U8) && ep == ir::Prim::U16) {
            if (const auto* l = ir::get<ir::Lit>(strip(e))) {
                if (const auto* sv = std::get_if<std::string>(&l->value)) {
                    const auto cps = code_points(*sv);
                    if (!cps.empty()) return int_lit(static_cast<std::int32_t>(cps.front()));
                }
                if (const auto* iv = std::get_if<std::int64_t>(&l->value)) return int_lit(static_cast<std::int32_t>(*iv));
            }
        }
        // a constant arm of ?: narrowed to a byte, short or char: C# types the whole expression int unless each arm is cast
        if (const auto* choice = ir::get<ir::Cond>(strip(e)); choice != nullptr && tp && ep && (tp == ir::Prim::I8 || tp == ir::Prim::I16 || tp == ir::Prim::U16)) {
            return make<ConditionalExpression>(expr(choice->cond), make<CastExpression>(type(t), expr(choice->then)), make<CastExpression>(type(t), expr(choice->orelse)));
        }
    }
    return expr(e);
}

std::vector<Expression*> UnitBuilder::args(std::span<Node* const> as, const ir::Sym* s) {
    std::vector<Expression*> out;
    for (std::size_t i = 0; i < as.size(); ++i) {
        auto pass = typesystem::ReferenceKind::None;
        TypeP pt;
        if (s != nullptr) {
            if (i < s->refs.size()) pass = pass_kind(s->refs[i]);
            if (i < s->params.size() && !(s->varargs && i + 1 >= s->params.size())) pt = s->params[i];
        }
        Expression* value = pt ? coerce(as[i], pt) : expr(as[i]);
        if (pass == typesystem::ReferenceKind::Ref) value = make<DirectionExpression>(FieldDirection::Ref, value);
        else if (pass == typesystem::ReferenceKind::Out) value = make<DirectionExpression>(FieldDirection::Out, value);
        out.push_back(value);
    }
    return out;
}

// r != null ? r.m : null, written r?.m (C#'s own form of the test and the access)
Expression* UnitBuilder::null_conditional(const ir::Cond& e) {
    const auto* cb = ir::get<ir::Binary>(strip(e.cond));
    if (cb == nullptr || cb->op != "!=") return nullptr;
    const auto* rl = ir::get<ir::Lit>(strip(cb->right));
    if (rl == nullptr || !is_null(rl->value)) return nullptr;
    const auto* ol = ir::get<ir::Lit>(strip(e.orelse));
    if (ol == nullptr || !is_null(ol->value)) return nullptr;
    // the access must be on the very thing tested: a name, read once
    const auto* tested = ir::get<ir::Name>(strip(cb->left));
    if (tested == nullptr || !tested->sym) return nullptr;
    Node* then = strip(e.then);
    Node* inner = nullptr;
    if (const auto* sel = ir::get<ir::Select>(then)) inner = sel->obj;
    else if (const auto* call = ir::get<ir::Call>(then); call != nullptr && call->target != nullptr && !(call->sym && has(call->sym->flags, "extension"))) inner = call->target;
    if (inner == nullptr || !is_name(strip(inner), *tested->sym)) return nullptr;
    Expression* access = expr(then);
    // the access's target is the name: wrap it in the null-conditional operator
    AstNode* target = nullptr;
    if (auto* m = as<MemberReferenceExpression>(access)) target = m->target();
    else if (auto* inv = as<InvocationExpression>(access)) {
        if (auto* mr = as<MemberReferenceExpression>(inv->target())) target = mr->target();
    }
    auto* target_expr = target != nullptr ? as<IdentifierExpression>(target) : nullptr;
    if (target_expr == nullptr) return nullptr;
    target_expr->replace_with(make<UnaryOperatorExpression>(UnaryOperatorType::NullConditional, make<IdentifierExpression>(target_expr->identifier())));
    return make<UnaryOperatorExpression>(UnaryOperatorType::NullConditionalRewrap, access);
}

Expression* UnitBuilder::interpolate(const std::vector<Node*>& parts) {
    std::vector<InterpolatedStringContent*> content;
    for (Node* p : parts) {
        const auto* l = ir::get<ir::Lit>(p);
        if (l != nullptr && is_text(l->value)) {
            content.push_back(make<InterpolatedStringText>(text::replace_all(text::replace_all(std::get<std::string>(l->value), "{", "{{"), "}", "}}")));
        } else {
            content.push_back(make<Interpolation>(expr(p)));
        }
    }
    return make<InterpolatedStringExpression>(content);
}

Expression* UnitBuilder::expr(Node* e, bool statement) {
    Expression* r = dispatch(e);
    if (!statement) {
        Node* se = strip(e);
        if (!ir::is<ir::Lit>(se) && !ir::is<ir::Paren>(se) && !limit_value(se)) {
            if (range_of(type_of(e)) && is_const(e) && overflows(e)) return make<UncheckedExpression>(r);
        }
    }
    return r;
}

Expression* UnitBuilder::literal(const ir::Lit& l, const TypeP& t) {
    const ir::LitValue& v = l.value;
    const auto prim = prim_of(t);
    const auto type_member = [](std::string_view kw, const std::string& name) { return member(make<TypeReferenceExpression>(keyword_type(kw)), name); };
    const auto floating = [&](double d) -> Expression* {
        const bool single = prim == ir::Prim::F32;
        if (std::isnan(d)) return type_member(single ? "float" : "double", "NaN");
        if (std::isinf(d)) return type_member(single ? "float" : "double", d > 0 ? "PositiveInfinity" : "NegativeInfinity");
        if (single && std::isinf(static_cast<float>(d))) throw Declined{"literal", "a float literal beyond the range of float"};   // (not the infinity C# would make of it)
        if (single) return make<PrimitiveExpression>(typesystem::ConstantValue(static_cast<float>(d)));
        return make<PrimitiveExpression>(typesystem::ConstantValue(d));
    };
    if (is_null(v)) return make<NullReferenceExpression>();
    if (const auto* b = std::get_if<bool>(&v)) return make<PrimitiveExpression>(typesystem::ConstantValue(*b));
    if (const auto* sv = std::get_if<std::string>(&v)) {
        const auto cps = code_points(*sv);
        if (prim == ir::Prim::U16 && cps.size() == 1) {
            if (!std::get<ir::PrimType>(t->body).encoding) return int_lit(static_cast<std::int32_t>(cps[0]));
            return char_lit(cps[0]);
        }
        if (prim == ir::Prim::F32 || prim == ir::Prim::F64) {
            if (const auto d = number_of(v)) return floating(*d);   // (L2 keeps a float that is not finite as its name)
        }
        return string_lit(*sv);
    }
    if (std::holds_alternative<double>(v) || prim == ir::Prim::F32 || prim == ir::Prim::F64) {
        const auto d = number_of(v);
        if (!d) throw Declined{"literal", "a literal that is not a number"};
        return floating(*d);
    }
    if (const auto* uv = std::get_if<std::uint64_t>(&v)) {
        if (*uv == std::numeric_limits<std::uint64_t>::max() && prim == ir::Prim::U64) return type_member("ulong", "MaxValue");
        return make<PrimitiveExpression>(typesystem::ConstantValue(*uv));
    }
    if (const auto* iv = std::get_if<std::int64_t>(&v)) {
        const std::int64_t val = *iv;
        if (const std::string* cls = class_of(t); cls != nullptr && !cls->starts_with("java.lang.") && !numeric_box(*cls)) {
            // a number of a class's type is an enum's value the front end knew only as its number (NumberStyles 4, DayOfWeek 1): C#
            // converts only 0 to an enum implicitly, so the number is cast to it
            Expression* number = val < std::numeric_limits<std::int32_t>::min() || val > std::numeric_limits<std::int32_t>::max()
                                     ? static_cast<Expression*>(make<PrimitiveExpression>(typesystem::ConstantValue(val)))
                                     : int_lit(static_cast<std::int32_t>(val));
            return make<CastExpression>(type(t), number);
        }
        if (prim == ir::Prim::U16 && std::get<ir::PrimType>(t->body).encoding) return char_lit(static_cast<char32_t>(val));
        if (prim == ir::Prim::I64) {
            if (val == std::numeric_limits<std::int64_t>::min()) return type_member("long", "MinValue");
            if (val == std::numeric_limits<std::int64_t>::max()) return type_member("long", "MaxValue");
            return make<PrimitiveExpression>(typesystem::ConstantValue(val));
        }
        if (prim == ir::Prim::U32) {
            if (val == 4294967295LL) return type_member("uint", "MaxValue");
            return make<PrimitiveExpression>(typesystem::ConstantValue(static_cast<std::uint32_t>(val)));
        }
        if (prim == ir::Prim::U64) return make<PrimitiveExpression>(typesystem::ConstantValue(static_cast<std::uint64_t>(val)));
        if (!prim || prim == ir::Prim::I32) {
            if (val == std::numeric_limits<std::int32_t>::min()) return type_member("int", "MinValue");
            if (val == std::numeric_limits<std::int32_t>::max()) return type_member("int", "MaxValue");
        }
        if (val < std::numeric_limits<std::int32_t>::min() || val > std::numeric_limits<std::int32_t>::max()) return make<PrimitiveExpression>(typesystem::ConstantValue(val));
        return int_lit(static_cast<std::int32_t>(val));
    }
    throw Declined{"literal", "a literal of an unknown kind"};
}

Expression* UnitBuilder::call_expr(Node* e, const ir::Call& x) {
    const ir::SymP& s = x.sym;
    if (!s) throw Declined{"call", "a call with no resolved method"};
    const auto flag = [&](std::string_view f) { return has(s->flags, f); };
    const std::string_view sname = s->name;
    const std::vector<Node*>& as = x.args;
    if (flag("operator")) {
        if (const auto op = operator_methods().find(sname); op != operator_methods().end() && as.size() == 2)
            return make<BinaryOperatorExpression>(expr(as[0]), binary_operators().at(op->second), expr(as[1]));
        if (const auto op = unary_methods().find(sname); op != unary_methods().end() && as.size() == 1)
            return make<UnaryOperatorExpression>(op->second, expr(as[0]));
        if (sname == "op_Implicit" && as.size() == 1) return expr(as[0]);
        if (sname == "op_Explicit" && as.size() == 1) return make<CastExpression>(type(e->type), expr(as[0]));
    }
    if (flag("delegate") && x.target != nullptr) return make<InvocationExpression>(expr(x.target), args(x.args, s.get()));
    std::vector<AstType*> targs;
    if (!x.typeargs.empty() && !flag("inferred")) {
        for (const ir::TypeArg& a : x.typeargs) {
            const auto* t = std::get_if<TypeP>(&a);
            if (t == nullptr) throw Declined{"call", "a type argument that is not a type"};
            targs.push_back(type(*t));
        }
    }
    const std::string name = clean(s->name);
    if (flag("extension") && !as.empty() && x.target != nullptr && ir::is<ir::TypeName>(x.target)) {
        // a static method called as if on its first argument: written on the receiver
        ir::Sym rest = *s;
        if (!rest.params.empty()) rest.params.erase(rest.params.begin());
        if (!rest.refs.empty()) rest.refs.erase(rest.refs.begin());
        ir::ClassDecl* c = s->owner ? prog_.class_named(*s->owner) : nullptr;
        std::string ns;
        if (c != nullptr) ns = namespace_of(c);
        else if (s->owner && contains(*s->owner, ".")) ns = rsplit_dot(*s->owner).first;
        if (!ns.empty()) usings_.insert(ns);
        Expression* recv = expr(as[0]);
        return make<InvocationExpression>(make<MemberReferenceExpression>(recv, name, targs), args(std::span<Node* const>(as).subspan(1), &rest));
    }
    if (x.target == nullptr) return make<InvocationExpression>(make<IdentifierExpression>(name, targs), args(x.args, s.get()));
    if (ir::is<ir::TypeName>(x.target) && cls_ != nullptr) {
        // a static of this class, written bare
        const std::string* tn = class_of(type_of(x.target));
        if (tn != nullptr && cls_->qname && *tn == *cls_->qname) return make<InvocationExpression>(make<IdentifierExpression>(name, targs), args(x.args, s.get()));
    }
    Expression* target = expr(x.target);
    return make<InvocationExpression>(make<MemberReferenceExpression>(target, name, targs), args(x.args, s.get()));
}

Expression* UnitBuilder::dispatch(Node* e) {
    if (e == nullptr) throw Declined{"null", "a missing expression"};
    if (const auto it = init_uses_.find(e); it != init_uses_.end()) {
        // the local of a hoisted object or collection initialiser, where it is used: the initialiser itself
        const InitGroup& g = it->second;
        auto* made = as<ObjectCreateExpression>(dispatch(g.made));
        if (made == nullptr) throw Declined{"initializer", "an initialiser of something not made with new"};
        auto* init = make<ArrayInitializerExpression>();
        for (const InitItem& item : g.items) {
            if (item.is_set) {
                init->elements().add(make<NamedExpression>(clean(item.sym->name), coerce(item.value, item.sym->type)));
            } else if (item.args.size() == 1) {
                init->elements().add(expr(item.args[0]));
            } else {
                auto* group = make<ArrayInitializerExpression>();
                for (Node* y : item.args) group->elements().add(expr(y));
                init->elements().add(group);
            }
        }
        made->set_initializer(init);
        return made;
    }
    if (const auto* x = ir::get<ir::Name>(e)) {
        if (!x->sym) throw Declined{"name", "a name with no symbol"};
        return make<IdentifierExpression>(lookup(x->sym->name));
    }
    if (const auto* x = ir::get<ir::Lit>(e)) return literal(*x, e->type);
    if (const auto* x = ir::get<ir::Paren>(e)) return expr(x->x);   // (grouping is the output visitor's, from the tree)
    if (const auto* x = ir::get<ir::Await>(e)) return make<UnaryOperatorExpression>(UnaryOperatorType::Await, expr(x->x));
    if (const auto* x = ir::get<ir::Call>(e)) return call_expr(e, *x);
    if (const auto* x = ir::get<ir::Select>(e)) {
        const ir::SymP& s = x->sym;
        if (!s) throw Declined{"select", "a member access with no symbol"};
        const auto* th = ir::get<ir::This>(x->obj);
        if (th != nullptr && !th->qualifier && !shadowed(s->name)) return ident_expr(s->name);   // this.x, where no local hides x
        if (ir::is<ir::TypeName>(x->obj) && cls_ != nullptr) {
            const std::string* tn = class_of(type_of(x->obj));
            if (tn != nullptr && cls_->qname && *tn == *cls_->qname) return ident_expr(s->name);   // a static of this class
        }
        Expression* obj = expr(x->obj);
        return member(obj, s->name);
    }
    if (const auto* x = ir::get<ir::This>(e)) {
        if (x->qualifier && cls_ != nullptr) {
            const std::string* qn = class_of(x->qualifier);
            if (qn == nullptr || qn->empty() || !cls_->qname || *qn != *cls_->qname) throw Declined{"outer-this", "an outer instance (Outer.this)"};
        }
        return make<ThisReferenceExpression>();
    }
    if (ir::is<ir::Super>(e)) return make<BaseReferenceExpression>();
    if (ir::is<ir::TypeName>(e)) return make<TypeReferenceExpression>(type(e->type));
    if (const auto* x = ir::get<ir::Length>(e)) return member(expr(x->arr), "Length");
    if (const auto* x = ir::get<ir::Index>(e)) {
        Expression* arr = expr(x->arr);
        std::vector<Expression*> is;
        for (Node* i : ir::statements(x->index)) is.push_back(expr(i));
        return make<IndexerExpression>(arr, is);
    }
    if (const auto* x = ir::get<ir::Unary>(e)) {
        const std::string& op = x->op;
        UnaryOperatorType kind;
        if (op == "++") kind = x->postfix ? UnaryOperatorType::PostIncrement : UnaryOperatorType::Increment;
        else if (op == "--") kind = x->postfix ? UnaryOperatorType::PostDecrement : UnaryOperatorType::Decrement;
        else if (op == "-") kind = UnaryOperatorType::Minus;
        else if (op == "+") kind = UnaryOperatorType::Plus;
        else if (op == "!") kind = UnaryOperatorType::Not;
        else if (op == "~") kind = UnaryOperatorType::BitNot;
        else throw Declined{"operator", "the operator " + op};
        Expression* t = make<UnaryOperatorExpression>(kind, expr(x->x));
        if (x->overflow == ir::Overflow::Trap) return make<CheckedExpression>(t);
        return t;
    }
    if (const auto* x = ir::get<ir::Binary>(e)) {
        // `(x = v, true)`, which a pattern variable of `var x` lowers to: C# says it `(x = v) is var _` (a `var` pattern always matches)
        if (x->op == ",") {
            auto* var_pattern = make<DeclarationExpression>(make<SimpleType>("var"), make<SingleVariableDesignation>("_"));
            return make<BinaryOperatorExpression>(expr(x->left), BinaryOperatorType::IsPattern, var_pattern);
        }
        const auto pit = binary_operators().find(x->op);
        if (pit == binary_operators().end()) throw Declined{"operator", "the operator " + x->op};
        Expression* left = expr(x->left);
        Expression* t = make<BinaryOperatorExpression>(left, pit->second, expr(x->right));
        if (x->overflow == ir::Overflow::Trap) return make<CheckedExpression>(t);
        return t;
    }
    if (const auto* x = ir::get<ir::Concat>(e)) {
        if (csharp_ && x->parts.size() > 2) {
            const bool literals_are_text = std::all_of(x->parts.begin(), x->parts.end(), [](Node* p) {
                const auto* l = ir::get<ir::Lit>(p);
                return l == nullptr || is_text(l->value);
            });
            if (literals_are_text && interpolatable(x->parts)) return interpolate(x->parts);
        }
        std::vector<Expression*> parts;
        for (Node* p : x->parts) parts.push_back(expr(p));
        // a concatenation of two non-strings starts from the empty string, so that + is not an addition
        if ((x->parts.size() >= 2 && !is_string(type_of(x->parts[0])) && !is_string(type_of(x->parts[1]))) || parts.size() == 1) parts.insert(parts.begin(), string_lit(""));
        Expression* out = parts[0];
        for (std::size_t i = 1; i < parts.size(); ++i) out = make<BinaryOperatorExpression>(out, BinaryOperatorType::Add, parts[i]);
        return out;
    }
    if (const auto* x = ir::get<ir::Assign>(e)) {
        Expression* target = expr(x->target);
        if (!x->op) return make<AssignmentExpression>(target, coerce(x->value, type_of(x->target)));
        if (*x->op == "??") {   // ILSpy's tree has no `??=`: `x = x ?? v`, the same where the place is a local or a field of this or a class
            Node* place = strip(x->target);
            const auto* sel = ir::get<ir::Select>(place);
            if (!ir::is<ir::Name>(place) && !(sel != nullptr && sel->sym && sel->sym->kind == ir::SymKind::Field && (ir::is<ir::This>(sel->obj) || ir::is<ir::TypeName>(sel->obj))))
                throw Declined{"operator", "the operator ??= on a place found by an expression"};
            Expression* either = make<BinaryOperatorExpression>(expr(x->target), BinaryOperatorType::NullCoalescing, coerce(x->value, type_of(x->target)));
            return make<AssignmentExpression>(target, either);
        }
        const auto op = assignment_operators().find(*x->op);
        if (op == assignment_operators().end()) throw Declined{"operator", "the operator " + *x->op + "="};
        Expression* t = make<AssignmentExpression>(target, op->second, expr(x->value));
        if (x->overflow == ir::Overflow::Trap) return make<CheckedExpression>(t);
        return t;
    }
    if (const auto* x = ir::get<ir::Cond>(e)) {
        if (csharp_) {
            if (Expression* got = null_conditional(*x)) return got;
        }
        Expression* c = expr(x->cond);
        Expression* a = e->type ? coerce(x->then, e->type) : expr(x->then);
        Expression* b = e->type ? coerce(x->orelse, e->type) : expr(x->orelse);
        // `c ? v : null` of a nullable value type: the value is made a T? (C# types a conditional by its branches, and `null` has no type)
        if (e->type) {
            const auto* null_else = ir::get<ir::Lit>(x->orelse);
            AstType* ct = type(e->type);
            auto* composed = as<ComposedType>(ct);
            if (null_else != nullptr && is_null(null_else->value) && composed != nullptr && composed->has_nullable_specifier() && composed->array_specifiers().empty()) {
                const TypeP then_type = type_of(x->then);
                auto* then_composed = then_type ? as<ComposedType>(type(then_type)) : nullptr;
                if (then_type && !(then_composed != nullptr && then_composed->has_nullable_specifier())) a = make<CastExpression>(ct, a);
            }
        }
        return make<ConditionalExpression>(c, a, b);
    }
    if (ir::is<ir::Cast>(e)) {
        const auto* x = ir::get<ir::Cast>(e);
        Expression* operand = expr(x->x);
        if (ir::is<ir::Lambda>(strip(x->x))) operand = make<ParenthesizedExpression>(operand);   // `(Func<T>)(() => ...)`: C# reads a cast lambda only in parentheses
        Expression* t = make<CastExpression>(type(e->type), operand);
        if (x->overflow == ir::Overflow::Trap) return make<CheckedExpression>(t);
        return t;
    }
    if (const auto* x = ir::get<ir::InstanceOf>(e)) {
        Expression* tested = expr(x->x);
        if (x->binding != nullptr) {
            const auto* bl = ir::get<ir::Local>(x->binding);
            if (bl == nullptr) throw Declined{"pattern", "a pattern binding " + describe(x->binding)};
            if (bl->sym) {
                AstType* t = type(x->test);
                auto* pattern = make<DeclarationExpression>(t, make<SingleVariableDesignation>(declare(bl->sym->name)));
                return make<BinaryOperatorExpression>(tested, BinaryOperatorType::IsPattern, pattern);
            }
        }
        return make<IsExpression>(tested, type(x->test));
    }
    if (const auto* x = ir::get<ir::ClassLit>(e)) return make<TypeOfExpression>(type(x->of));
    if (const auto* x = ir::get<ir::Lambda>(e)) {
        // untyped parameters: a lambda here always has a target type (an argument, a typed local, a return)
        struct RestoreLocals {
            std::unordered_set<std::string>& slot;
            std::unordered_set<std::string> saved;
            ~RestoreLocals() { slot = std::move(saved); }
        } restore{locals_, locals_};
        Scope scope(*this);
        std::vector<ParameterDeclaration*> ps;
        for (Node* p : x->params) {
            auto* pd = make<ParameterDeclaration>();
            pd->set_name(declare(ir::get<ir::Param>(p)->sym->name, true));
            ps.push_back(pd);
        }
        AstNode* body = ir::is<ir::Block>(x->body) ? static_cast<AstNode*>(block_of(x->body)) : static_cast<AstNode*>(expr(x->body));
        auto* lambda = make<LambdaExpression>(std::vector<AttributeSection*>{}, ps, body);
        lambda->set_is_async(has(x->mods, "async"));
        return lambda;
    }
    if (const auto* x = ir::get<ir::MethodRef>(e)) {
        if (x->mode == "new") throw Declined{"method-ref", "a constructor reference"};
        if (ir::is<ir::TypeName>(x->target) && !x->sym->is_static) throw Declined{"method-ref", "an unbound method reference (Type::instanceMethod)"};
        return member(expr(x->target), x->sym->name);
    }
    if (const auto* x = ir::get<ir::SwitchExpr>(e)) {
        auto* sw = make<SwitchExpression>(expr(x->expr));
        for (Node* cn : x->cases) {
            const auto* c = ir::get<ir::Case>(cn);
            if (c->body.size() != 1) throw Declined{"switch-expression", "a switch-expression arm with statements"};
            Node* b = c->body[0];
            Expression* val = nullptr;
            if (const auto* y = ir::get<ir::Yield>(b)) val = coerce(y->expr, e->type);
            else if (const auto* t = ir::get<ir::Throw>(b)) val = make<ThrowExpression>(expr(t->expr));
            else if (const auto* es = ir::get<ir::ExprStmt>(b)) val = expr(es->expr);
            else throw Declined{"switch-expression", "a switch-expression arm " + describe(b)};
            Expression* pattern = nullptr;
            if (c->is_default) {
                pattern = make<IdentifierExpression>("_");
            } else {
                for (Node* l : c->labels) {
                    Expression* one = case_label(l, x->expr);
                    pattern = pattern == nullptr ? one : make<BinaryOperatorExpression>(pattern, BinaryOperatorType::PatternOr, one);
                }
            }
            sw->switch_sections().add(make<SwitchExpressionSection>(pattern, val));
        }
        return sw;
    }
    if (const auto* x = ir::get<ir::New>(e)) {
        if (x->body != nullptr) throw Declined{"anonymous-class", "an anonymous class"};
        if (x->outer != nullptr) throw Declined{"inner-class", "an inner class made on an outer instance"};
        return make<ObjectCreateExpression>(type(e->type), args(x->args, x->sym.get()));
    }
    if (const auto* x = ir::get<ir::NewArray>(e)) {
        const TypeP& t = e->type;
        const auto* arr = t ? std::get_if<ir::ArrayType>(&t->body) : nullptr;
        // an array with its items: `new T[] { a, b }`
        if (!x->init.empty() || x->dims.empty()) {
            if (!x->dims.empty()) throw Declined{"array", "an array with sizes and items"};
            const TypeP elem = arr != nullptr ? arr->elem : x->elem;
            auto* items = make<ArrayInitializerExpression>();
            for (Node* y : x->init) items->elements().add(coerce(y, elem));
            // `new T[] { ... }`: the element type and the array's own specifier
            auto* made = make<ArrayCreateExpression>();
            const TypeP whole = t ? t : ir::array_of(x->elem);
            const auto* wa = std::get_if<ir::ArrayType>(&whole->body);
            made->set_type(type(wa != nullptr ? wa->elem : x->elem));
            made->additional_array_specifiers().add(make<ArraySpecifier>(static_cast<int>(wa != nullptr && wa->rank > 1 ? wa->rank : 1)));
            made->set_initializer(items);
            return made;
        }
        if (arr != nullptr && arr->rank != 1) {
            std::vector<Expression*> ds;
            for (Node* d : x->dims) ds.push_back(expr(d));
            return make<ArrayCreateExpression>(type(arr->elem), ds);
        }
        std::size_t n = 1;
        TypeP base = x->elem;
        if (arr != nullptr) std::tie(n, base) = ir::dims(t);
        std::vector<Expression*> sizes;
        for (Node* d : x->dims) sizes.push_back(expr(d));
        std::vector<ArraySpecifier*> rest;
        for (std::size_t i = x->dims.size(); i < n; ++i) rest.push_back(make<ArraySpecifier>(1));
        return make<ArrayCreateExpression>(type(base), sizes, rest);
    }
    if (const auto* x = ir::get<ir::Unsupported>(e)) throw declined_for(*x);
    const std::string k = describe(e);
    throw Declined{k, "the expression " + k};
}

// The text of a tree, as ilspycmd's C# is written but with four spaces.
std::string tree_text(SyntaxTree* tree) {
    output_visitor::InsertParenthesesVisitor insert_parentheses;
    insert_parentheses.insert_parentheses_for_readability = false;
    tree->accept_visitor(insert_parentheses);
    output_visitor::GenericGrammarAmbiguityVisitor::resolve_ambiguities(tree);
    output_visitor::StringTextWriter writer("\n");
    output_visitor::CSharpFormattingOptions options = output_visitor::FormattingOptionsFactory::create_allman();
    options.indentation_string = "    ";
    options.indent_switch_body = true;
    options.array_initializer_wrapping = output_visitor::Wrapping::WrapIfTooLong;
    options.auto_property_formatting = output_visitor::PropertyFormatting::SingleLine;
    output_visitor::CSharpOutputVisitor visitor(writer, options);
    tree->accept_visitor(visitor);
    // (the visitor leaves a space after a trailing comma, as in a switch expression's arms: no line ends in one)
    std::string out;
    for (const std::string_view line : text::split(writer.text(), '\n')) {
        out += text::rtrim(line);
        out += '\n';
    }
    while (!out.empty() && (out.back() == '\n' || out.back() == ' ')) out.pop_back();
    return out + "\n";
}

}  // namespace l2_plain

// ---- L2CSharpProgram ----------------------------------------------------------------------------------------------------------------

L2CSharpProgram::L2CSharpProgram(std::span<ir::Node* const> units, L2CSharpOptions options) : options_(std::move(options)), units_(units.begin(), units.end()) {
    for (ir::Node* n : units_) {
        if (ir::get<ir::Unit>(n) == nullptr) throw UsageError("the C# back end writes Unit nodes");
    }
    if (options_.java_registry != nullptr) {   // Java's library and semantics, restated as .NET's
        translate::rules::java_csharp::Result restated = translate::rules::java_csharp::lower(arena_, units_, *options_.java_registry);
        units_ = std::move(restated.units);
        runtime_ = std::move(restated.runtime);
    }
    for (ir::Node* n : units_) {
        for (ir::Node* c : ir::get<ir::Unit>(n)->types) index(c);
    }
}

L2CSharpProgram::~L2CSharpProgram() = default;

void L2CSharpProgram::index(ir::Node* class_node) {
    auto* cd = ir::get<ir::ClassDecl>(class_node);
    if (cd == nullptr) return;
    if (cd->qname) classes_[*cd->qname] = cd;
    for (ir::Node* m : cd->members) index(m);
}

ir::ClassDecl* L2CSharpProgram::class_named(std::string_view qname) const {
    const auto it = classes_.find(std::string(qname));
    return it == classes_.end() ? nullptr : it->second;
}

ir::ClassDecl* L2CSharpProgram::base_of(const ir::ClassDecl& c) const {
    const std::string* name = l2_plain::class_of(c.extends);
    return name != nullptr ? class_named(*name) : nullptr;
}

const std::vector<L2File>& L2CSharpProgram::files() {
    if (files_) return *files_;
    std::vector<L2File> out;
    std::set<std::string> taken;
    for (ir::Node* n : units_) {
        il::ILArena arena;   // the unit's syntax tree, freed once it is written
        il::ILArenaScope scope(arena);
        l2_plain::UnitBuilder builder(*this, n);
        syntax::SyntaxTree* tree = builder.unit();
        const auto& path = ir::get<ir::Unit>(n)->path;
        std::string stem = fs::to_text(fs::from_text(path && !path->empty() ? *path : "Unit").stem());
        if (stem.empty()) stem = "Unit";
        std::string name = stem;
        for (int k = 2; taken.contains(name); ++k) name = stem + "_" + std::to_string(k);   // (two units of one stem would be one file)
        taken.insert(name);
        out.push_back({name + ".cs", l2_plain::tree_text(tree)});
    }
    for (const std::string& r : runtime_) {   // (a set: by name)
        std::string runtime_text = fs::read_text(options_.runtime_dir / (r + ".cs"));
        if (r == "JavaCharTypes") {   // (the one table of Java's Unicode categories, which the C and C++ translations carry too)
            runtime_text = cb::text::replace_all(runtime_text, "/*JAVA_CHAR_TYPES*/", fs::read_text(options_.runtime_dir.parent_path().parent_path() / "runtime" / "java_char_types.inc"));
        }
        out.push_back({r + ".cs", cb::text::replace_all(runtime_text, "\r\n", "\n")});
    }
    files_ = std::move(out);
    return *files_;
}

const std::vector<L2Gap>& L2CSharpProgram::ledger() {
    files();
    return ledger_;
}

std::vector<std::filesystem::path> L2CSharpProgram::write(const std::filesystem::path& dir) {
    fs::make_dirs(dir);
    std::vector<std::filesystem::path> paths;
    for (const L2File& f : files()) {
        paths.push_back(dir / f.name);
        fs::write_text(paths.back(), f.text);
    }
    return paths;
}

}  // namespace cb::dotnet::decompiler::csharp

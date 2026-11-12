// cb-unity: alone
// cb/translate/frontends/csharp/lower.cpp -- Roslyn's bound tree (CsTreeDump's JSON) into ir::Unit. See lower.hpp.
//
// The dump is read where it stands (a `const Json&` per node, members looked up by name); the IR is built in the caller's
// arena. A construct the lowering cannot carry throws `Refuse` from wherever it is met, up to the statement (or declaration
// initialiser, or lambda body) that holds it, which becomes an ir::Unsupported: the one place an exception stands for a
// decision, because the construct may be many frames below the place that can say what became of it.
#include "cb/translate/frontends/csharp/lower.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <format>
#include <limits>
#include <map>
#include <set>
#include <span>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/json.hpp"

namespace cb::translate::frontends::csharp {

namespace {

using ir::Node;
using ir::SymP;
using ir::TypeP;

constexpr std::string_view kReader = "csharp dump";

// A construct that cannot be carried: its kind and why. Caught where a statement, an initialiser or a lambda body
// holds it; it never leaves lower().
class Refuse : public UnsupportedError {
public:
    Refuse(std::string kind, std::string why)
        : UnsupportedError(kReader, 0, why), kind_(std::move(kind)), why_(std::move(why)) {}
    const std::string& construct() const { return kind_; }
    const std::string& why() const { return why_; }
    CB_ERROR_BOILERPLATE(Refuse)

private:
    std::string kind_;
    std::string why_;
};

// ---- reading the dump ---------------------------------------------------------------------------------------------------

std::uint32_t line_of(const Json& at) {
    if (at.is_object()) {
        const auto it = at.find("l");
        if (it != at.end() && it->is_number_unsigned()) return static_cast<std::uint32_t>(it->get<std::uint64_t>());
    }
    return 0;
}

[[noreturn]] void bad(const Json& at, const std::string& what) {
    throw ParseError(kReader, line_of(at), 0, what);
}

// A member that may be absent: null when it is, or when the dump wrote null.
const Json* opt(const Json& j, const char* key) {
    if (!j.is_object()) return nullptr;
    const auto it = j.find(key);
    return it == j.end() || it->is_null() ? nullptr : &*it;
}

const Json& req(const Json& j, const char* key) {
    const Json* v = opt(j, key);
    if (v == nullptr) bad(j, std::format("a node lacks \"{}\"", key));
    return *v;
}

const std::string& text_of(const Json& j, const char* key) {
    const Json& v = req(j, key);
    if (!v.is_string()) bad(j, std::format("\"{}\" is not text", key));
    return v.get_ref<const std::string&>();
}

// The text of a member, or "" where it is absent.
std::string_view text_or_empty(const Json& j, const char* key) {
    const Json* v = opt(j, key);
    return v != nullptr && v->is_string() ? std::string_view(v->get_ref<const std::string&>()) : std::string_view();
}

std::optional<std::string> opt_text(const Json& j, const char* key) {
    const Json* v = opt(j, key);
    if (v == nullptr) return std::nullopt;
    if (!v->is_string()) bad(j, std::format("\"{}\" is not text", key));
    return v->get<std::string>();
}

bool flag(const Json& j, const char* key) {
    const Json* v = opt(j, key);
    return v != nullptr && v->is_boolean() && v->get<bool>();
}

bool flag_or(const Json& j, const char* key, bool dflt) {
    const Json* v = opt(j, key);
    return v != nullptr && v->is_boolean() ? v->get<bool>() : dflt;
}

const Json::array_t& list(const Json& j, const char* key) {
    static const Json::array_t kNone;
    const Json* v = opt(j, key);
    if (v == nullptr) return kNone;
    if (!v->is_array()) bad(j, std::format("\"{}\" is not a list", key));
    return v->get_ref<const Json::array_t&>();
}

std::vector<std::string> texts(const Json& j, const char* key) {
    std::vector<std::string> out;
    for (const Json& s : list(j, key)) {
        if (!s.is_string()) bad(j, std::format("\"{}\" holds a value that is not text", key));
        out.push_back(s.get<std::string>());
    }
    return out;
}

std::uint32_t coordinate(const Json& n, const char* key) {
    const Json* v = opt(n, key);
    if (v == nullptr) return 0;
    if (!v->is_number_unsigned()) bad(n, std::format("\"{}\" is not a position", key));
    const std::uint64_t x = v->get<std::uint64_t>();
    if (x > std::numeric_limits<std::uint32_t>::max()) bad(n, std::format("\"{}\" is out of range", key));
    return static_cast<std::uint32_t>(x);
}

struct Pos {
    std::uint32_t line = 0;
    std::uint32_t col = 0;
};

Pos pos_of(const Json& n) { return {coordinate(n, "l"), coordinate(n, "c")}; }

// ---- tables -------------------------------------------------------------------------------------------------------------

std::optional<ir::Prim> prim_named(std::string_view n) {
    static const std::unordered_map<std::string_view, ir::Prim> kPrims = {
        {"bool", ir::Prim::Bool}, {"sbyte", ir::Prim::I8},  {"byte", ir::Prim::U8},    {"short", ir::Prim::I16},
        {"ushort", ir::Prim::U16}, {"int", ir::Prim::I32},  {"uint", ir::Prim::U32},   {"long", ir::Prim::I64},
        {"ulong", ir::Prim::U64}, {"float", ir::Prim::F32}, {"double", ir::Prim::F64}};
    const auto it = kPrims.find(n);
    return it == kPrims.end() ? std::nullopt : std::optional<ir::Prim>(it->second);
}

// A binary operator Roslyn names -> how C# writes it; empty for one that is not an operator of the IR.
std::string_view binary_op(std::string_view kind) {
    static const std::unordered_map<std::string_view, std::string_view> kOps = {
        {"Add", "+"},          {"Subtract", "-"},       {"Multiply", "*"},       {"Divide", "/"},
        {"Remainder", "%"},    {"LeftShift", "<<"},     {"RightShift", ">>"},    {"UnsignedRightShift", ">>>"},
        {"And", "&"},          {"Or", "|"},             {"ExclusiveOr", "^"},    {"ConditionalAnd", "&&"},
        {"ConditionalOr", "||"}, {"Equals", "=="},      {"NotEquals", "!="},     {"LessThan", "<"},
        {"LessThanOrEqual", "<="}, {"GreaterThan", ">"}, {"GreaterThanOrEqual", ">="},
        {"ObjectValueEquals", "=="}, {"ObjectValueNotEquals", "!="}};
    const auto it = kOps.find(kind);
    return it == kOps.end() ? std::string_view() : it->second;
}

std::string_view unary_op(std::string_view kind) {
    if (kind == "Minus") return "-";
    if (kind == "Plus") return "+";
    if (kind == "Not") return "!";
    if (kind == "BitwiseNegation") return "~";
    return {};
}

// How a parameter is passed (RefKind's names); by value for None and what is not known.
ir::PassMode pass_mode(std::string_view ref_kind) {
    if (ref_kind == "Ref") return ir::PassMode::Ref;
    if (ref_kind == "Out") return ir::PassMode::Out;
    if (ref_kind == "In" || ref_kind == "RefReadOnly" || ref_kind == "RefReadOnlyParameter") return ir::PassMode::In;
    return ir::PassMode::ByValue;
}

// The modifier a parameter's pass mode is written as in a Param's mods ("" for by value).
std::string_view pass_word(ir::PassMode m) {
    switch (m) {
        case ir::PassMode::Ref: return "ref";
        case ir::PassMode::Out: return "out";
        case ir::PassMode::In: return "in";
        case ir::PassMode::ByValue: break;
    }
    return {};
}

// Binding strength of C#'s operators (higher binds tighter): what Roslyn's tree leaves out, parentheses, is put back from it.
int binary_precedence(std::string_view op) {
    static const std::unordered_map<std::string_view, int> kPrec = {
        {"*", 14}, {"/", 14}, {"%", 14}, {"+", 13}, {"-", 13}, {"<<", 12}, {">>", 12}, {">>>", 12}, {"<", 11}, {">", 11},
        {"<=", 11}, {">=", 11}, {"==", 10}, {"!=", 10}, {"&", 9}, {"^", 8}, {"|", 7}, {"&&", 6}, {"||", 5}, {"??", 4}};
    const auto it = kPrec.find(op);
    return it == kPrec.end() ? 0 : it->second;
}

// ---- types --------------------------------------------------------------------------------------------------------------

const TypeP& string_type() {
    static const TypeP t = ir::class_type("System.String");
    return t;
}

// A CsTreeDump type record -> ir type (an absent record: not resolved).
TypeP lower_type(const Json* d);

TypeP lower_type(const Json& d) {
    const std::string& k = text_of(d, "k");
    if (k == "prim") {
        const std::string& n = text_of(d, "n");
        if (n == "void") return ir::void_type();
        if (n == "char") return ir::prim_type(ir::Prim::U16, std::nullopt, ir::CharEncoding::Utf16);
        if (const auto p = prim_named(n)) return ir::prim_type(*p);
        return ir::class_type(n == "nint" ? "System.IntPtr" : "System.UIntPtr");
    }
    if (k == "array") {
        ir::ArrayType a;
        a.elem = lower_type(req(d, "elem"));
        if (const Json* rank = opt(d, "rank")) {
            if (!rank->is_number_unsigned() || rank->get<std::uint64_t>() == 0) bad(d, "an array type with a rank that is not a count");
            a.rank = static_cast<std::uint32_t>(rank->get<std::uint64_t>());
        }
        return ir::make_type(std::move(a));
    }
    if (k == "tv") return ir::typevar_type(text_of(d, "n"));
    if (k == "named") {
        ir::ClassType c;
        c.name = text_of(d, "n");
        for (const Json& a : list(d, "args")) c.args.push_back(lower_type(a));
        c.nullable = flag(d, "nullable");
        return ir::make_type(std::move(c));
    }
    if (k == "ptr") return ir::pointer_to(lower_type(opt(d, "elem")));
    if (k == "dynamic") return ir::make_type(ir::DynamicType{});
    return ir::any_type();   // "error", "other": the compiler could not say
}

TypeP lower_type(const Json* d) { return d == nullptr ? ir::any_type() : lower_type(*d); }

bool is_string(const TypeP& t) {
    const auto* c = t ? std::get_if<ir::ClassType>(&t->body) : nullptr;
    return c != nullptr && c->name == "System.String";
}

// Whether a type record is of this TypeKind (Class, Struct, Enum, Interface, Delegate).
bool type_kind_is(const Json* d, std::string_view tk) { return d != nullptr && text_or_empty(*d, "tk") == tk; }

bool is_class(const TypeP& t) { return t && std::holds_alternative<ir::ClassType>(t->body); }
bool is_any(const TypeP& t) { return !t || std::holds_alternative<ir::AnyType>(t->body); }

std::string class_name(const TypeP& t) { return is_class(t) ? std::get<ir::ClassType>(t->body).name : std::string(); }

TypeP class_of(const std::optional<std::string>& owner) { return ir::class_type(owner.value_or("")); }

// ---- symbols ------------------------------------------------------------------------------------------------------------

// A CsTreeDump symbol record -> ir::Sym; null for an empty record.
SymP lower_sym(const Json& d) {
    if (d.is_object() && d.empty()) return nullptr;
    const std::string& k = text_of(d, "kind");
    std::vector<std::string> flags;
    static constexpr std::pair<const char*, const char*> kFlagKeys[] = {
        {"ext", "extension"}, {"indexer", "indexer"}, {"readonly", "readonly"}, {"initonly", "initonly"},
        {"virtual", "virtual"}, {"abstract", "abstract"}, {"override", "override"}, {"sealed", "sealed"},
        {"implicit", "implicit"}, {"const", "const"}};
    for (const auto& [key, name] : kFlagKeys) {
        if (flag(d, key)) flags.emplace_back(name);
    }
    const std::optional<std::string> owner = opt_text(d, "owner");
    const std::string& name = text_of(d, "name");
    auto made = [&](ir::SymKind kind, bool is_static, TypeP type = {}) {
        SymP s = ir::make_sym(kind, name, owner, is_static, std::move(type));
        s->flags = flags;
        return s;
    };
    if (k == "Method") {
        const std::string_view mk = text_or_empty(d, "mk");
        const bool ctor = mk == "Constructor";
        if (mk == "UserDefinedOperator" || mk == "Conversion" || mk == "BuiltinOperator") flags.emplace_back("operator");
        if (mk == "LocalFunction") flags.emplace_back("local");
        if (mk == "StaticConstructor") flags.emplace_back("static_init");
        if (name == "Invoke" && type_kind_is(opt(d, "ownerType"), "Delegate")) flags.emplace_back("delegate");
        SymP s = made(ctor ? ir::SymKind::Ctor : ir::SymKind::Method, flag(d, "static"));
        for (const Json& p : list(d, "params")) s->params.push_back(lower_type(p));
        s->ret = lower_type(opt(d, "ret"));
        s->varargs = flag(d, "varargs");
        std::vector<ir::PassMode> refs;
        bool any_passed = false;
        for (const Json& r : list(d, "refs")) {
            if (!r.is_string()) bad(d, "a parameter's pass mode is not text");
            refs.push_back(pass_mode(r.get_ref<const std::string&>()));
            any_passed = any_passed || refs.back() != ir::PassMode::ByValue;
        }
        if (any_passed) s->refs = std::move(refs);
        return s;
    }
    if (k == "Field") {
        const bool enum_const = type_kind_is(opt(d, "ownerType"), "Enum") && flag(d, "const");
        return made(enum_const ? ir::SymKind::EnumConst : ir::SymKind::Field, flag(d, "static") || flag(d, "const"),
                    lower_type(opt(d, "type")));
    }
    if (k == "Property") {
        SymP s = made(ir::SymKind::Property, flag(d, "static"), lower_type(opt(d, "type")));
        for (const Json& p : list(d, "params")) s->params.push_back(lower_type(p));
        return s;
    }
    if (k == "Event") return made(ir::SymKind::Event, flag(d, "static"), lower_type(opt(d, "type")));
    if (k == "Local" || k == "RangeVariable" || k == "Discard") {
        if (opt(d, "ref") != nullptr) flags.emplace_back("ref");
        return made(ir::SymKind::Local, false, lower_type(opt(d, "type")));
    }
    if (k == "Parameter") {
        SymP s = made(ir::SymKind::Param, false, lower_type(opt(d, "type")));
        const ir::PassMode mode = pass_mode(text_or_empty(d, "ref"));
        if (mode != ir::PassMode::ByValue) s->refs = {mode};
        return s;
    }
    if (k == "NamedType") {
        const std::string_view qname = text_or_empty(d, "qname");
        SymP s = made(ir::SymKind::Class, false, lower_type(opt(d, "type")));
        if (!qname.empty()) s->name = std::string(qname);
        return s;
    }
    if (k == "TypeParameter") {
        SymP s = made(ir::SymKind::TypeVar, false, lower_type(opt(d, "type")));
        s->flags.clear();
        return s;
    }
    if (k == "Namespace") {
        SymP s = made(ir::SymKind::Package, false);
        s->flags.clear();
        return s;
    }
    if (k == "Label") return ir::make_sym(ir::SymKind::Label, name);
    // a kind the front end has no use for (an alias, an assembly ...): a global of that name, so a use still names it
    return made(ir::SymKind::Global, false);
}

SymP required_sym(const Json& d) {
    SymP s = lower_sym(d);
    if (!s) bad(d, "an empty symbol record where a symbol is needed");
    return s;
}
SymP optional_sym(const Json* d) { return d == nullptr ? nullptr : lower_sym(*d); }

// A symbol with other flags or staticness: a copy, since a symbol is shared by every node that names it.
SymP copy_of(const SymP& s) { return std::make_shared<ir::Sym>(*s); }

// ---- constants ----------------------------------------------------------------------------------------------------------

// A UTF-16 code unit as text. A half of a surrogate pair has no UTF-8 form of its own; it is written as the three bytes
// generalised UTF-8 gives it, as the dump's reader would give it back.
std::string utf8_of_unit(std::uint32_t cp) {
    std::string out;
    if (cp < 0x80) {
        out.push_back(static_cast<char>(cp));
    } else if (cp < 0x800) {
        out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    } else {
        out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
    return out;
}

ir::LitValue whole_number(const Json& c, const Json& v) {
    if (v.is_number_integer()) {
        if (v.is_number_unsigned()) {
            const std::uint64_t u = v.get<std::uint64_t>();
            if (u > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) return u;
            return static_cast<std::int64_t>(u);
        }
        return v.get<std::int64_t>();
    }
    if (v.is_string()) {   // longs and ulongs come as text
        const std::string& s = v.get_ref<const std::string&>();
        std::int64_t i = 0;
        auto r = std::from_chars(s.data(), s.data() + s.size(), i);
        if (r.ec == std::errc() && r.ptr == s.data() + s.size()) return i;
        std::uint64_t u = 0;
        r = std::from_chars(s.data(), s.data() + s.size(), u);
        if (r.ec == std::errc() && r.ptr == s.data() + s.size()) return u;
    }
    bad(c, "a constant that is not a whole number");
}

double real_number(const Json& c, const Json& v) {
    if (v.is_number()) return v.get<double>();
    if (v.is_string()) {
        const std::string& s = v.get_ref<const std::string&>();
        if (s == "Infinity" || s == "\xE2\x88\x9E") return std::numeric_limits<double>::infinity();
        if (s == "-Infinity" || s == "-\xE2\x88\x9E") return -std::numeric_limits<double>::infinity();
        if (s == "NaN") return std::numeric_limits<double>::quiet_NaN();
        double d = 0;
        const auto r = std::from_chars(s.data(), s.data() + s.size(), d);
        if (r.ec == std::errc() && r.ptr == s.data() + s.size()) return d;
    }
    bad(c, "a constant that is not a number");
}

// A CsTreeDump constant record -> what ir::Lit keeps (an absent record, and a null constant: null).
ir::LitValue lower_value(const Json* c) {
    if (c == nullptr) return std::monostate{};
    const std::string& t = text_of(*c, "t");
    if (t == "null") return std::monostate{};
    const Json* v = opt(*c, "v");
    if (v == nullptr) bad(*c, "a constant has no value");
    if (t == "bool") {
        if (!v->is_boolean()) bad(*c, "a bool constant that is not true or false");
        return v->get<bool>();
    }
    if (t == "string") {
        if (!v->is_string()) bad(*c, "a string constant that is not text");
        return v->get<std::string>();
    }
    if (t == "char") {
        if (!v->is_number_unsigned() || v->get<std::uint64_t>() > 0xFFFF) bad(*c, "a char constant that is not a code unit");
        return utf8_of_unit(static_cast<std::uint32_t>(v->get<std::uint64_t>()));
    }
    if (t == "float" || t == "double") return real_number(*c, *v);
    if (t == "decimal") throw Refuse("decimal", "a decimal literal: no IR number carries base-10 decimals");
    return whole_number(*c, *v);
}

// ---- helpers over the tree ----------------------------------------------------------------------------------------------

// The names a pattern declares, in the order the dump lists them.
void collect_bindings(const Json& n, std::vector<std::string>& out) {
    if (n.is_object()) {
        const std::string_view k = text_or_empty(n, "k");
        if (k == "DeclarationPattern" || k == "RecursivePattern") {
            if (const Json* ds = opt(n, "declaredSymbol")) out.push_back(text_of(*ds, "name"));
        }
        for (const auto& member : n.items()) collect_bindings(member.value(), out);
    } else if (n.is_array()) {
        for (const Json& e : n) collect_bindings(e, out);
    }
}

bool is_null_literal(const Node* e) {
    const auto* lit = ir::get<ir::Lit>(e);
    if (lit == nullptr || !std::holds_alternative<std::monostate>(lit->value)) return false;
    if (!e->type) return true;
    return std::holds_alternative<ir::NullType>(e->type->body) || std::holds_alternative<ir::ClassType>(e->type->body) ||
           std::holds_alternative<ir::AnyType>(e->type->body) || std::holds_alternative<ir::TypeVarType>(e->type->body);
}

// An expression that can be read twice: a name, this, a literal, a field of one.
bool can_read_twice(const Node* e) {
    if (ir::is<ir::Name>(e) || ir::is<ir::This>(e) || ir::is<ir::Lit>(e) || ir::is<ir::TypeName>(e)) return true;
    if (const auto* s = ir::get<ir::Select>(e)) return s->sym && s->sym->kind == ir::SymKind::Field && can_read_twice(s->obj);
    return false;
}

// Whether evaluating the expression does something: a call, a `new`, an assignment, ++ or --.
bool has_effects(Node* e) {
    for (const Node* x : ir::walk(e)) {
        if (ir::is<ir::Call>(x) || ir::is<ir::New>(x) || ir::is<ir::Assign>(x)) return true;
        if (const auto* u = ir::get<ir::Unary>(x)) {
            if (u->op == "++" || u->op == "--") return true;
        }
    }
    return false;
}

// A lambda whose body was an expression: Roslyn wraps it in an implicit block and return.
bool is_implicit_body(const Json& b) {
    if (text_or_empty(b, "k") != "Block") return false;
    const Json::array_t& ops = list(b, "operations");
    return flag(b, "implicit") && ops.size() == 1 && flag(ops[0], "implicit");
}

// ---- the lowering -------------------------------------------------------------------------------------------------------

class Lowering {
public:
    Lowering(Arena& arena, std::optional<std::string> path) : arena_(arena), path_(std::move(path)) {}

    std::vector<Refusal> refused;   // what became Unsupported

    Node* unit(const Json& n);

private:
    // What one statement's lowering holds: the statements hoisted before it, how many effects it has lowered, and how many
    // branches that might not run it is inside. A scope sets them for the span of a statement (or a declaration's
    // initialiser, a lambda body) and puts back the enclosing one's.
    class Scope {
    public:
        Scope(Lowering& l, std::vector<Node*>* pending) : l_(l), pending_(l.pending_), effects_(l.effects_), guard_(l.guard_) {
            l.pending_ = pending;
            l.effects_ = 0;
            l.guard_ = 0;
        }
        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;
        ~Scope() {
            l_.pending_ = pending_;
            l_.effects_ = effects_;
            l_.guard_ = guard_;
        }

    private:
        Lowering& l_;
        std::vector<Node*>* pending_;
        int effects_;
        int guard_;
    };
    // Inside a branch that might not run (the right of `&&`, an arm, a loop condition).
    class MaybeSkipped {
    public:
        explicit MaybeSkipped(Lowering& l) : l_(l) { ++l_.guard_; }
        MaybeSkipped(const MaybeSkipped&) = delete;
        MaybeSkipped& operator=(const MaybeSkipped&) = delete;
        ~MaybeSkipped() { --l_.guard_; }

    private:
        Lowering& l_;
    };

    Arena& arena_;
    std::optional<std::string> path_;
    ir::ClassDecl* cls_ = nullptr;                    // the ClassDecl being lowered
    std::vector<Node*>* pending_ = nullptr;           // statements hoisted before the statement being lowered; null: nowhere to hoist
    int guard_ = 0;                                   // > 0: inside a branch that might not run, a loop condition
    int effects_ = 0;                                 // effects lowered so far in this statement
    int temps_ = 0;
    std::map<std::string, std::string> rename_;       // a local's name -> the name it is given (bindings joined in one expression)
    const Node* receiver_now_ = nullptr;              // the object an initialiser's members are set on
    const Node* cond_recv_ = nullptr;                 // the receiver of the `?.` being lowered

    template <class K>
    Node* mk(K kind, Pos p, TypeP type = {}) {
        return ir::make(arena_, std::move(kind), std::move(type), p.line, p.col);
    }
    Node* copy(const Node* n) { return ir::clone(arena_, n); }
    Node* type_name(const TypeP& t, Pos p) { return mk(ir::TypeName{}, p, t); }
    Node* null_literal(Pos p) { return mk(ir::Lit{}, p, ir::null_type()); }
    Node* bool_literal(bool v, Pos p) { return mk(ir::Lit{.value = v}, p, ir::bool_type()); }
    Node* not_of(Node* x, Pos p) { return mk(ir::Unary{.op = "!", .x = x}, p, ir::bool_type()); }

    SymP renamed(SymP s) const {
        if (s && s->kind == ir::SymKind::Local) {
            const auto it = rename_.find(s->name);
            if (it != rename_.end()) {
                s = copy_of(s);
                s->name = it->second;
            }
        }
        return s;
    }

    Node* unsupported(const Json& n, std::optional<std::string> kind, std::string why, TypeP t = {}) {
        refused.push_back({line_of(n), kind, why});
        if (!t) t = lower_type(opt(n, "$type"));
        return mk(ir::Unsupported{.kind = std::move(kind), .raw = std::move(why)}, pos_of(n), std::move(t));
    }

    // ---- declarations
    Node* top_level(const Json& n);
    std::vector<Node*> tparams(const Json* tps);
    Node* decl(const Json& n, const std::optional<std::string>& outer);
    void primary(Node* class_node, const Json& n);
    std::vector<Node*> params(const Json* ps);
    std::vector<Node*> member(const Json& m, Node* class_node);

    // ---- bodies
    Node* method_body(const Json* op);
    Node* ctor_body(const Json* op, const Json& m);
    Node* ctor_init(const Json& st);
    Node* block(const Json& op);
    std::vector<Node*> stmts(std::span<const Json> ops);
    Node* one(const Json& op);
    std::vector<Node*> stmt(const Json& op);
    std::vector<Node*> stmt_inner(const Json& op);
    Node* as_block(const Json& op);
    Node* loop(const Json& op, Pos p);
    Node* switch_stmt(const Json& op, Pos p);
    Node* try_stmt(const Json& op, Pos p);

    // ---- expressions
    Node* expr_in_decl(const Json& op);
    void hoist(const std::vector<Node*>& stmts, bool needs_order = true);
    Node* temp(const TypeP& t, Node* init, const Json& n);
    Node* expr(const Json& n);
    Node* expr_inner(const Json& n);
    Node* receiver(const Json& n, const SymP& s);
    std::vector<Node*> args(const Json& n);
    Node* call(const Json& n, const TypeP& t, Pos p);
    Node* op_call(const SymP& s, std::vector<Node*> args, const TypeP& t, Pos p);
    Node* object_creation(const Json& n, const TypeP& t, Pos p);
    Node* array_creation(const Json& n, const TypeP& t, Pos p);
    Node* binary(const Json& n, const TypeP& t, Pos p);
    Node* conversion(const Json& n, const TypeP& t, Pos p);
    Node* cond_access(const Json& n, const TypeP& t, Pos p);
    Node* default_value(const Json& n, const TypeP& t, Pos p);
    Node* interpolated(const Json& n, Pos p);
    Node* lambda(const Json& n, const TypeP& t, Pos p);
    Node* concat(Node* left, Node* right, const TypeP& t, Pos p);

    // ---- patterns
    Node* pattern(const Json& pat, const Node* v, Pos p);
    Node* switch_expr(const Json& n, const TypeP& t, Pos p);
};

// ---- declarations -------------------------------------------------------------------------------------------------------

Node* Lowering::unit(const Json& n) {
    std::vector<Node*> types;
    for (const Json& t : list(n, "types")) types.push_back(decl(t, std::nullopt));
    std::string package;
    if (!types.empty()) {
        const std::string& qname = ir::get<ir::ClassDecl>(types[0])->qname.value_or(std::string());
        const std::size_t dot = qname.rfind('.');
        if (dot != std::string::npos) package = qname.substr(0, dot);
    }
    if (opt(n, "main") != nullptr) types.push_back(top_level(n));
    std::vector<std::string> imports;
    for (const Json& u : list(n, "usings")) {
        const std::string& name = text_of(u, "name");
        if (!name.empty() && text_or_empty(u, "alias").empty() && !flag(u, "alias")) imports.push_back(name);
    }
    ir::Unit u;
    u.path = path_;
    u.package = std::move(package);
    u.imports = std::move(imports);
    u.types = std::move(types);
    u.lang = std::string(kLang);
    return mk(std::move(u), pos_of(n));
}

// Top-level statements: the compiler's Program.Main, as a class.
Node* Lowering::top_level(const Json& n) {
    const Json* ms = opt(n, "mainSym");
    const std::string owner = ms != nullptr ? std::string(text_or_empty(*ms, "owner")) : std::string();
    const std::string qname = owner.empty() ? "Program" : owner;
    Node* class_node = mk(ir::ClassDecl{}, {});
    ir::ClassDecl& c = *ir::get<ir::ClassDecl>(class_node);
    c.name = qname.substr(qname.rfind('.') == std::string::npos ? 0 : qname.rfind('.') + 1);
    c.qname = qname;
    c.kind = "class";
    c.mods = {"static", "partial"};
    cls_ = &c;
    Node* body = method_body(&req(n, "main"));
    TypeP ret = ms != nullptr && !ms->empty() ? lower_type(opt(*ms, "ret")) : ir::void_type();
    SymP s = ir::make_sym(ir::SymKind::Method, "Main", qname, true);
    s->ret = ret;
    std::vector<Node*> parameters;
    if (ms != nullptr) {
        const Json::array_t& names = list(*ms, "pnames");
        const Json::array_t& types = list(*ms, "params");
        for (std::size_t i = 0; i < std::min(names.size(), types.size()); ++i) {
            TypeP pt = lower_type(types[i]);
            parameters.push_back(mk(ir::Param{.sym = ir::make_sym(ir::SymKind::Param, names[i].get<std::string>(), qname + ".Main", false, pt),
                                              .type = pt},
                                    {}));
        }
    }
    c.members.push_back(mk(ir::MethodDecl{.sym = s, .name = "Main", .kind = "method", .params = std::move(parameters), .ret = ret,
                                          .body = body, .mods = {"static"}},
                           {}));
    return class_node;
}

std::vector<Node*> Lowering::tparams(const Json* tps) {
    std::vector<Node*> out;
    if (tps == nullptr) return out;
    for (const Json& tp : *tps) {
        ir::TypeParam t;
        t.name = text_of(tp, "name");
        for (const Json& b : list(tp, "bounds")) t.bounds.push_back(lower_type(b));
        for (const char* f : {"class", "struct", "new"}) {
            if (flag(tp, f)) t.flags.emplace_back(f);
        }
        const std::string_view variance = text_or_empty(tp, "variance");
        if (!variance.empty() && variance != "None") t.variance = std::string(variance);
        out.push_back(mk(std::move(t), {}));
    }
    return out;
}

Node* Lowering::decl(const Json& n, const std::optional<std::string>& outer) {
    const Json* s = opt(n, "sym");
    const std::string& kind = text_of(n, "kind");
    const std::string_view sym_qname = s != nullptr ? text_or_empty(*s, "qname") : std::string_view();
    const std::string qname = sym_qname.empty() ? text_of(n, "name") : std::string(sym_qname);
    const Json* base = opt(n, "base");
    TypeP extends;
    if (base != nullptr) {
        const std::string_view bn = text_or_empty(*base, "n");
        if (bn != "System.Object" && bn != "System.ValueType" && bn != "System.Enum") extends = lower_type(*base);
    }
    if (const Json* underlying = opt(n, "underlying"); kind == "enum" && underlying != nullptr && text_or_empty(*underlying, "n") != "int") {
        extends = lower_type(*underlying);   // an enum's integer type, where not int
    }
    const Pos p = pos_of(n);
    Node* class_node = mk(ir::ClassDecl{}, p);
    ir::ClassDecl& c = *ir::get<ir::ClassDecl>(class_node);
    c.name = text_of(n, "name");
    c.qname = qname;
    c.kind = kind;
    c.mods = texts(n, "mods");
    for (const Json& tp : list(n, "typeparams")) c.typeparams.push_back(text_of(tp, "name"));
    c.extends = extends;
    const bool is_record = kind == "record" || kind == "record struct";
    for (const Json& i : list(n, "interfaces")) {
        if (text_or_empty(i, "n") != "System.IEquatable" || !is_record) c.implements.push_back(lower_type(i));
    }
    c.outer = outer;
    c.tparams = tparams(opt(n, "typeparams"));

    ir::ClassDecl* const prev = cls_;
    cls_ = &c;
    struct Restore {
        Lowering& l;
        ir::ClassDecl* prev;
        ~Restore() { l.cls_ = prev; }
    } restore{*this, prev};

    if (kind == "delegate") {
        const Json& invoke = req(n, "invoke");
        SymP inv = required_sym(invoke);
        std::vector<Node*> parameters;
        const Json::array_t& names = list(invoke, "pnames");
        const Json::array_t& types = list(invoke, "params");
        const Json::array_t& refs = list(invoke, "refs");
        for (std::size_t i = 0; i < std::min({names.size(), types.size(), refs.size()}); ++i) {
            TypeP pt = lower_type(types[i]);
            ir::Param param;
            param.sym = ir::make_sym(ir::SymKind::Param, names[i].get<std::string>(), qname + ".Invoke", false, pt);
            param.type = pt;
            if (const std::string_view word = pass_word(pass_mode(refs[i].get<std::string>())); !word.empty()) param.mods.emplace_back(word);
            parameters.push_back(mk(std::move(param), {}));
        }
        c.members.push_back(mk(ir::MethodDecl{.sym = inv, .name = "Invoke", .kind = "method", .params = std::move(parameters),
                                              .ret = inv->ret, .mods = {"public", "abstract"}},
                               p));
        return class_node;
    }
    if (opt(n, "params") != nullptr && (is_record || kind == "class" || kind == "struct")) primary(class_node, n);
    for (const Json& m : list(n, "members")) {
        for (Node* x : member(m, class_node)) c.members.push_back(x);
    }
    const bool is_static = std::find(c.mods.begin(), c.mods.end(), "static") != c.mods.end();
    const bool has_ctor = std::any_of(c.members.begin(), c.members.end(), [](const Node* x) {
        const auto* m = ir::get<ir::MethodDecl>(x);
        return m != nullptr && m->kind == "ctor";
    });
    if (kind == "class" && !is_static && !has_ctor) {
        // the compiler's default constructor (as javac's, marked IMPLICIT)
        SymP csym = ir::make_sym(ir::SymKind::Ctor, ".ctor", qname);
        csym->ret = ir::void_type();
        Node* body = mk(ir::Block{.stmts = {mk(ir::CtorCall{.kind = "super:implicit"}, p)}}, p);
        const bool is_abstract = std::find(c.mods.begin(), c.mods.end(), "abstract") != c.mods.end();
        c.members.push_back(mk(ir::MethodDecl{.sym = csym, .name = ".ctor", .kind = "ctor", .ret = ir::void_type(), .body = body,
                                              .mods = {is_abstract ? "protected" : "public", "IMPLICIT"}},
                               p));
    }
    return class_node;
}

// A record's (or class's) primary constructor: its parameters, and for a record the public properties the compiler makes
// of them (mods "positional").
void Lowering::primary(Node* class_node, const Json& n) {
    ir::ClassDecl& c = *ir::get<ir::ClassDecl>(class_node);
    const bool is_record = c.kind == "record" || c.kind == "record struct";
    std::map<std::string, const Json*> implicit;   // looked up by name only
    if (is_record) {
        if (const Json* s = opt(n, "sym")) {
            for (const Json& x : list(*s, "implicit")) {
                if (text_or_empty(x, "kind") == "Property") implicit[text_of(x, "name")] = &x;
            }
        }
    }
    std::vector<Node*> parameters;
    for (const Json& p : list(n, "params")) {
        SymP ps = required_sym(req(p, "sym"));
        parameters.push_back(mk(ir::Param{.sym = ps, .type = ps->type, .mods = texts(p, "mods")}, pos_of(p)));
        const auto found = implicit.find(ps->name);
        if (found != implicit.end()) {
            SymP psym = required_sym(*found->second);
            const bool init_only = c.kind == "record";
            std::vector<Node*> accessors;
            accessors.push_back(mk(ir::MethodDecl{.name = "get", .kind = "get", .ret = ps->type}, {}));
            accessors.push_back(mk(ir::MethodDecl{.name = init_only ? "init" : "set", .kind = init_only ? "init" : "set", .ret = ir::void_type()}, {}));
            c.members.push_back(mk(ir::Property{.sym = psym, .type = psym->type, .mods = {"public", "positional"},
                                                .accessors = std::move(accessors)},
                                   pos_of(p)));
        }
    }
    std::vector<Node*> body_stmts;
    if (const Json* base_call = opt(n, "baseCall")) body_stmts.push_back(ctor_init(*base_call));
    SymP csym = ir::make_sym(ir::SymKind::Ctor, ".ctor", c.qname);
    for (const Node* p : parameters) csym->params.push_back(ir::get<ir::Param>(p)->type);
    csym->ret = ir::void_type();
    c.members.push_back(mk(ir::MethodDecl{.sym = csym, .name = ".ctor", .kind = "ctor", .params = std::move(parameters),
                                          .ret = ir::void_type(), .body = mk(ir::Block{.stmts = std::move(body_stmts)}, pos_of(n)),
                                          .mods = {"public", "primary", "IMPLICIT"}},
                           pos_of(n)));
}

std::vector<Node*> Lowering::params(const Json* ps) {
    std::vector<Node*> out;
    if (ps == nullptr) return out;
    for (const Json& p : *ps) {
        SymP s = optional_sym(opt(p, "sym"));
        out.push_back(mk(ir::Param{.sym = s, .type = s ? s->type : ir::any_type(), .mods = texts(p, "mods")}, pos_of(p)));
    }
    return out;
}

std::vector<Node*> Lowering::member(const Json& m, Node* class_node) {
    ir::ClassDecl& c = *ir::get<ir::ClassDecl>(class_node);
    const std::string& k = text_of(m, "k");
    if (k == "type") return {decl(m, c.qname)};
    if (k == "field") {
        std::vector<Node*> out;
        for (const Json& v : list(m, "vars")) {
            SymP s = required_sym(req(v, "sym"));
            Node* init = opt(v, "init") != nullptr ? expr_in_decl(req(v, "init")) : nullptr;
            out.push_back(mk(ir::FieldDecl{.sym = s, .type = s->type, .init = init, .mods = texts(m, "mods")}, pos_of(v)));
        }
        return out;
    }
    if (k == "property" || k == "indexer") {
        SymP s = required_sym(req(m, "sym"));
        std::vector<Node*> accessors;
        if (opt(m, "expr") != nullptr) {   // int X => expr;
            Node* body = method_body(&req(m, "expr"));
            accessors.push_back(mk(ir::MethodDecl{.name = "get", .kind = "get", .ret = s->type, .body = body}, pos_of(m)));
        }
        for (const Json& a : list(m, "accessors")) {
            Node* body = opt(a, "body") != nullptr ? method_body(&req(a, "body")) : nullptr;
            const std::string& ak = text_of(a, "kind");
            accessors.push_back(mk(ir::MethodDecl{.name = ak, .kind = ak, .ret = ak == "get" ? s->type : ir::void_type(), .body = body,
                                                  .mods = texts(a, "mods")},
                                   pos_of(a)));
        }
        Node* init = opt(m, "init") != nullptr ? expr_in_decl(req(m, "init")) : nullptr;
        return {mk(ir::Property{.sym = s, .type = s->type, .mods = texts(m, "mods"), .params = params(opt(m, "params")),
                                .accessors = std::move(accessors), .init = init},
                   pos_of(m))};
    }
    if (k == "method" || k == "operator" || k == "conversion") {
        SymP s = optional_sym(opt(m, "sym"));
        Node* body = opt(m, "body") != nullptr ? method_body(&req(m, "body")) : nullptr;
        std::vector<std::string> mods = texts(m, "mods");
        if (k != "method") mods.emplace_back("operator");
        std::vector<std::string> typeparams;
        for (const Json& tp : list(m, "typeparams")) typeparams.push_back(text_of(tp, "name"));
        std::vector<Node*> parameters = params(opt(m, "params"));
        return {mk(ir::MethodDecl{.sym = s, .name = s ? s->name : "?", .kind = "method", .params = std::move(parameters),
                                  .ret = s ? s->ret : ir::void_type(), .body = body, .mods = std::move(mods),
                                  .typeparams = std::move(typeparams), .tparams = tparams(opt(m, "typeparams"))},
                   pos_of(m))};
    }
    if (k == "ctor") {
        SymP s = optional_sym(opt(m, "sym"));
        const bool is_static = s && std::find(s->flags.begin(), s->flags.end(), "static_init") != s->flags.end();
        Node* body = ctor_body(opt(m, "body"), m);
        return {mk(ir::MethodDecl{.sym = s, .name = is_static ? "<clinit>" : ".ctor", .kind = is_static ? "static_init" : "ctor",
                                  .params = params(opt(m, "params")), .ret = ir::void_type(), .body = body, .mods = texts(m, "mods")},
                   pos_of(m))};
    }
    if (k == "enummember") {
        SymP s = required_sym(req(m, "sym"));
        std::optional<std::int64_t> value;
        if (const Json* v = opt(m, "value")) {
            const ir::LitValue number = whole_number(*v, req(*v, "v"));
            if (const auto* i = std::get_if<std::int64_t>(&number)) {
                value = *i;
            } else {
                // a ulong constant above 2**63-1: the IR's number is signed 64-bit
                return {unsupported(m, k, "an enum constant above 2**63-1", ir::any_type())};
            }
        }
        return {mk(ir::EnumConst{.sym = s, .value = value}, pos_of(m))};
    }
    const char* why = "not lowered yet";
    if (k == "dtor") why = "a finalizer: no target runs code when the collector frees an object";
    if (k == "eventfield" || k == "event") why = "an event";
    return {unsupported(m, k, why, ir::any_type())};
}

// ---- bodies -------------------------------------------------------------------------------------------------------------

// An IMethodBodyOperation / IBlockOperation -> Block (null for none).
Node* Lowering::method_body(const Json* op) {
    if (op == nullptr) return nullptr;
    const std::string& k = text_of(*op, "k");
    if (k == "MethodBody" || k == "MethodBodyOperation" || k == "ConstructorBody" || k == "ConstructorBodyOperation") {
        const Json* b = opt(*op, "blockBody");
        if (b == nullptr) b = opt(*op, "expressionBody");
        return b != nullptr ? method_body(b) : mk(ir::Block{}, pos_of(*op));
    }
    if (k == "Block") return block(*op);
    return mk(ir::Block{.stmts = stmts(std::span<const Json>(op, 1))}, pos_of(*op));
}

Node* Lowering::ctor_body(const Json* op, const Json& m) {
    std::vector<Node*> out;
    if (op != nullptr && opt(*op, "initializer") != nullptr) out.push_back(ctor_init(req(*op, "initializer")));
    const Json* b = nullptr;
    if (op != nullptr) {
        b = opt(*op, "blockBody");
        if (b == nullptr) b = opt(*op, "expressionBody");
    }
    if (b != nullptr) {
        Node* inner = method_body(b);
        for (Node* s : ir::get<ir::Block>(inner)->stmts) out.push_back(s);
    }
    return mk(ir::Block{.stmts = std::move(out)}, pos_of(op != nullptr ? *op : m));
}

// `: base(...)` / `: this(...)` (or the compiler's own base()) -> CtorCall.
Node* Lowering::ctor_init(const Json& st) {
    const Json* inv = text_of(st, "k") == "ExpressionStatement" ? opt(st, "operation") : &st;
    if (inv == nullptr || text_or_empty(*inv, "k") != "Invocation") {
        return mk(ir::ExprStmt{.expr = unsupported(st, "ctor-init", "a constructor initialiser of another shape")}, pos_of(st));
    }
    SymP s = required_sym(req(*inv, "targetMethod"));
    std::string kind = cls_ != nullptr && s->owner == cls_->qname ? "this" : "super";
    if (flag(*inv, "implicit")) kind += ":implicit";
    std::vector<Node*> pre;
    std::vector<Node*> arguments;
    {
        Scope scope(*this, &pre);
        try {
            arguments = args(*inv);
        } catch (const Refuse& e) {
            // (an argument that cannot be carried: the initialiser, not the file, is what is left out)
            return mk(ir::ExprStmt{.expr = unsupported(st, e.construct(), e.why(), ir::any_type())}, pos_of(st));
        }
    }
    if (!pre.empty()) {
        return mk(ir::ExprStmt{.expr = unsupported(st, "ctor-init", "a constructor initialiser needing a statement")}, pos_of(st));
    }
    return mk(ir::CtorCall{.kind = kind, .sym = s, .args = std::move(arguments)}, pos_of(st));
}

Node* Lowering::block(const Json& op) { return mk(ir::Block{.stmts = stmts(list(op, "operations"))}, pos_of(op)); }

std::vector<Node*> Lowering::stmts(std::span<const Json> ops) {
    std::vector<Node*> out;
    for (std::size_t i = 0; i < ops.size(); ++i) {
        const Json& op = ops[i];
        if (text_of(op, "k") == "UsingDeclaration") {
            // `using var x = ...;` holds to the end of the block: a scoped resource around the rest
            const std::vector<Node*> res = stmt(req(op, "declarationGroup"));
            Node* body = mk(ir::Block{.stmts = stmts(ops.subspan(i + 1))}, pos_of(op));
            ir::Try t;
            for (Node* r : res) {
                if (ir::is<ir::Local>(r)) t.resources.push_back(r);
            }
            t.body = body;
            out.push_back(mk(std::move(t), pos_of(op)));
            if (std::any_of(res.begin(), res.end(), [](Node* r) { return !ir::is<ir::Local>(r); })) {
                out.push_back(mk(ir::ExprStmt{.expr = unsupported(op, "using", "a using declaration with a hoisted part")}, pos_of(op)));
            }
            return out;
        }
        for (Node* s : stmt(op)) out.push_back(s);
    }
    return out;
}

// A statement in a place that holds one (a loop's or an if's body).
Node* Lowering::one(const Json& op) {
    std::vector<Node*> ss = stmt(op);
    if (ss.size() == 1) return ss[0];
    return mk(ir::Block{.stmts = std::move(ss)}, pos_of(op));
}

// One operation in statement position -> statements (hoisted ones first).
std::vector<Node*> Lowering::stmt(const Json& op) {
    std::vector<Node*> pre;
    std::vector<Node*> s;
    {
        Scope scope(*this, &pre);
        try {
            s = stmt_inner(op);
        } catch (const Refuse& e) {
            s = {mk(ir::ExprStmt{.expr = unsupported(op, e.construct(), e.why(), ir::any_type())}, pos_of(op))};
        }
    }
    pre.insert(pre.end(), s.begin(), s.end());
    return pre;
}

std::vector<Node*> Lowering::stmt_inner(const Json& op) {
    const std::string& k = text_of(op, "k");
    const Pos p = pos_of(op);
    if (k == "Block") return {block(op)};
    if (k == "VariableDeclarationGroup") {
        std::vector<Node*> out;
        for (const Json& group : list(op, "declarations")) {
            for (const Json& d : list(group, "declarators")) {
                SymP s = required_sym(req(d, "symbol"));
                const Json* init = opt(d, "initializer");
                if (init == nullptr) init = opt(group, "initializer");
                Node* v = init != nullptr ? expr(req(*init, "value")) : nullptr;
                const bool is_const = std::find(s->flags.begin(), s->flags.end(), "const") != s->flags.end();
                ir::Local local{.sym = s, .type = s->type, .init = v};
                if (is_const) local.mods = {"const"};
                out.push_back(mk(std::move(local), pos_of(d)));
            }
        }
        return out;
    }
    if (k == "ExpressionStatement") {
        const Json& e = req(op, "operation");
        const std::string& ek = text_of(e, "k");
        if (ek == "Throw") return {mk(ir::Throw{.expr = opt(e, "exception") != nullptr ? expr(req(e, "exception")) : nullptr}, p)};
        if (ek == "SimpleAssignment" && text_of(req(e, "target"), "k") == "Discard") {
            return {mk(ir::ExprStmt{.expr = expr(req(e, "value"))}, p)};
        }
        return {mk(ir::ExprStmt{.expr = expr(e)}, p)};
    }
    if (k == "Throw") return {mk(ir::Throw{.expr = opt(op, "exception") != nullptr ? expr(req(op, "exception")) : nullptr}, p)};
    if (k == "Rethrow") return {mk(ir::Throw{}, p)};
    if (k == "Conditional") {
        Node* cond = expr(req(op, "condition"));
        Node* then = one(req(op, "whenTrue"));
        Node* orelse = opt(op, "whenFalse") != nullptr ? one(req(op, "whenFalse")) : nullptr;
        return {mk(ir::If{.cond = cond, .then = then, .orelse = orelse}, p)};
    }
    if (k == "Loop") return {loop(op, p)};
    if (k == "Branch") {
        const std::string_view bk = text_or_empty(op, "branchKind");
        if (bk == "Break") return {mk(ir::Break{}, p)};
        if (bk == "Continue") return {mk(ir::Continue{}, p)};
        if (bk == "GoTo") {
            const Json* target = opt(op, "target");
            const std::string tgt = target != nullptr ? std::string(text_or_empty(*target, "name")) : std::string();
            if (tgt.starts_with("case ") || tgt.starts_with("default")) throw Refuse("goto-case", "`goto case`: a jump between switch sections");
            return {mk(ir::Goto{.label = tgt}, p)};
        }
        throw Refuse("branch", std::format("a {} branch", bk));
    }
    if (k == "Labeled") {
        Node* inner = opt(op, "operation") != nullptr ? one(req(op, "operation")) : mk(ir::Empty{}, p);
        const Json* label = opt(op, "label");
        return {mk(ir::Labeled{.label = label != nullptr ? opt_text(*label, "name") : std::nullopt, .body = inner}, p)};
    }
    if (k == "Return") {
        const std::string_view return_kind = text_or_empty(op, "returnKind");
        if (!return_kind.empty() && return_kind != "Return") {
            throw Refuse("iterator", "`yield return` / `yield break`: an iterator (a state machine)");
        }
        return {mk(ir::Return{.expr = opt(op, "returnedValue") != nullptr ? expr(req(op, "returnedValue")) : nullptr}, p)};
    }
    if (k == "YieldReturn" || k == "YieldBreak") throw Refuse("iterator", "`yield return` / `yield break`: an iterator (a state machine)");
    if (k == "Switch") return {switch_stmt(op, p)};
    if (k == "Try") return {try_stmt(op, p)};
    if (k == "Using") {
        const Json& res = req(op, "resources");
        std::vector<Node*> resources;
        if (text_of(res, "k") == "VariableDeclarationGroup") resources = stmt(res);
        else resources.push_back(mk(ir::ExprStmt{.expr = expr(res)}, pos_of(res)));
        return {mk(ir::Try{.resources = std::move(resources), .body = as_block(req(op, "body"))}, p)};
    }
    if (k == "Lock") {
        Node* locked = expr(req(op, "lockedValue"));
        return {mk(ir::Sync{.lock = locked, .body = as_block(req(op, "body"))}, p)};
    }
    if (k == "Empty") return {mk(ir::Empty{}, p)};
    if (k == "LocalFunction") throw Refuse("local-function", "a local function");
    if (k == "UsingDeclaration") throw Refuse("using", "a using declaration out of a block");
    throw Refuse(k, std::format("the statement {}", k));
}

Node* Lowering::as_block(const Json& op) {
    Node* s = one(op);
    return ir::is<ir::Block>(s) ? s : mk(ir::Block{.stmts = {s}}, pos_of(op));
}

Node* Lowering::loop(const Json& op, Pos p) {
    const std::string_view lk = text_or_empty(op, "loopKind");
    if (lk == "While") {
        Node* cond = nullptr;
        {
            MaybeSkipped skipped(*this);
            cond = opt(op, "condition") != nullptr ? expr(req(op, "condition")) : bool_literal(true, p);
        }
        if (flag(op, "conditionIsUntil")) cond = not_of(cond, p);
        Node* body = one(req(op, "body"));
        if (flag_or(op, "conditionIsTop", true)) return mk(ir::While{.cond = cond, .body = body}, p);
        return mk(ir::DoWhile{.body = body, .cond = cond}, p);
    }
    if (lk == "For") {
        std::vector<Node*> init;
        for (const Json& b : list(op, "before")) {
            for (Node* s : stmt(b)) init.push_back(s);
        }
        Node* cond = nullptr;
        std::vector<Node*> update;
        {
            MaybeSkipped skipped(*this);
            cond = opt(op, "condition") != nullptr ? expr(req(op, "condition")) : nullptr;
            for (const Json& u : list(op, "atLoopBottom")) update.push_back(expr(text_of(u, "k") == "ExpressionStatement" ? req(u, "operation") : u));
        }
        Node* body = one(req(op, "body"));
        return mk(ir::For{.init = std::move(init), .cond = cond, .update = std::move(update), .body = body}, p);
    }
    if (lk == "ForEach") {
        const Json* v = opt(op, "loopControlVariable");
        if (v == nullptr || (text_of(*v, "k") != "VariableDeclarator" && text_of(*v, "k") != "LocalReference")) {
            throw Refuse("foreach", "a foreach that deconstructs its element");
        }
        const Json* sym_record = opt(*v, "symbol");
        if (sym_record == nullptr) sym_record = opt(*v, "local");
        if (sym_record == nullptr) bad(*v, "a foreach variable with no symbol");
        SymP s = required_sym(*sym_record);
        const Json* coll = &req(op, "collection");
        while (text_of(*coll, "k") == "Conversion" && flag(*coll, "implicit")) coll = &req(*coll, "operand");   // to IEnumerable: the compiler's own
        Node* var = mk(ir::Local{.sym = s, .type = s->type}, pos_of(*v));
        Node* iterable = expr(*coll);
        Node* body = one(req(op, "body"));
        return mk(ir::ForEach{.var = var, .iterable = iterable, .body = body}, p);
    }
    throw Refuse("loop", std::format("a {} loop", lk));
}

Node* Lowering::switch_stmt(const Json& op, Pos p) {
    std::vector<Node*> cases;
    for (const Json& c : list(op, "cases")) {
        std::vector<Node*> labels;
        bool is_default = false;
        for (const Json& cl : list(c, "clauses")) {
            std::string_view ck = text_or_empty(cl, "caseKind");
            if (ck.empty()) ck = text_of(cl, "k");
            const Json* pat = opt(cl, "pattern");
            if (ck == "Default" || ck == "DefaultCaseClause") {
                is_default = true;
            } else if (ck == "SingleValue" || ck == "SingleValueCaseClause") {
                labels.push_back(expr(req(cl, "value")));
            } else if ((ck == "Pattern" || ck == "PatternCaseClause") && pat != nullptr && text_or_empty(*pat, "k") == "ConstantPattern" &&
                       opt(cl, "guard") == nullptr) {
                labels.push_back(expr(req(*pat, "value")));
            } else {
                throw Refuse("switch-pattern", "a switch section matched by a pattern or a `when` guard");
            }
        }
        std::vector<Node*> body;
        for (const Json& b : list(c, "body")) {
            for (Node* s : stmt(b)) body.push_back(s);
        }
        cases.push_back(mk(ir::Case{.labels = std::move(labels), .is_default = is_default, .body = std::move(body)}, pos_of(c)));
    }
    // (the switched value is lowered after the sections, as the temporaries it may need are numbered after theirs)
    Node* value = expr(req(op, "value"));
    return mk(ir::Switch{.expr = value, .cases = std::move(cases)}, p);
}

Node* Lowering::try_stmt(const Json& op, Pos p) {
    std::vector<Node*> catches;
    for (const Json& c : list(op, "catches")) {
        if (opt(c, "filter") != nullptr) throw Refuse("catch-filter", "an exception filter (`catch ... when`)");
        const Json* d = opt(c, "exceptionDeclarationOrExpression");
        SymP s;
        if (d != nullptr && text_of(*d, "k") == "VariableDeclarator") s = optional_sym(opt(*d, "symbol"));
        ir::Catch catch_clause;
        catch_clause.sym = s;
        catch_clause.types = {lower_type(opt(c, "exceptionType"))};
        catch_clause.body = block(req(c, "handler"));
        catches.push_back(mk(std::move(catch_clause), pos_of(c)));
    }
    Node* body = block(req(op, "body"));
    Node* finally_body = opt(op, "finally") != nullptr ? block(req(op, "finally")) : nullptr;
    return mk(ir::Try{.body = body, .catches = std::move(catches), .finally_body = finally_body}, p);
}

// ---- expressions --------------------------------------------------------------------------------------------------------

// A field's or property's initialiser: an expression with no statement to hoist into.
Node* Lowering::expr_in_decl(const Json& op) {
    Scope scope(*this, nullptr);
    try {
        return expr(op);
    } catch (const Refuse& e) {
        return unsupported(op, e.construct(), e.why());
    }
}

// Put statements before the statement being lowered, where that cannot change what runs first.
void Lowering::hoist(const std::vector<Node*>& stmts, bool needs_order) {
    if (pending_ == nullptr) throw Refuse("hoist", "an initialiser needing a statement, in a declaration");
    if (guard_ != 0) throw Refuse("hoist", "an initialiser needing a statement, in a branch that might not run");
    if (needs_order && effects_ != 0) throw Refuse("hoist", "an initialiser needing a statement, after an effect in the same statement");
    pending_->insert(pending_->end(), stmts.begin(), stmts.end());
}

Node* Lowering::temp(const TypeP& t, Node* init, const Json& n) {
    ++temps_;
    SymP s = ir::make_sym(ir::SymKind::Local, std::format("__t{}", temps_), std::nullopt, false, t);
    hoist({mk(ir::Local{.sym = s, .type = t, .init = init}, pos_of(n))});
    return mk(ir::Name{.sym = s}, pos_of(n), t);
}

Node* Lowering::expr(const Json& n) {
    try {
        return expr_inner(n);
    } catch (const Refuse& e) {
        if (pending_ == nullptr) return unsupported(n, e.construct(), e.why());
        throw;
    }
}

Node* Lowering::expr_inner(const Json& n) {
    const std::string& k = text_of(n, "k");
    const Pos p = pos_of(n);
    const TypeP t = lower_type(opt(n, "$type"));
    if (k == "Literal") {
        ir::Lit lit{.value = lower_value(opt(n, "$const"))};
        return mk(std::move(lit), p, opt(n, "$type") != nullptr ? t : ir::null_type());
    }
    if (k == "NameOf") return mk(ir::Lit{.value = lower_value(opt(n, "$const"))}, p, string_type());
    if (k == "LocalReference") {
        SymP s = renamed(required_sym(req(n, "local")));
        if (flag(n, "isDeclaration")) {   // `out var x`
            hoist({mk(ir::Local{.sym = s, .type = s->type}, p)}, false);
        }
        return mk(ir::Name{.sym = s}, p, t);
    }
    if (k == "ParameterReference") return mk(ir::Name{.sym = required_sym(req(n, "parameter"))}, p, t);
    if (k == "FieldReference") {
        const Json& field = req(n, "field");
        if (flag(field, "const") && !flag(field, "source") && opt(n, "$const") != nullptr) {
            return mk(ir::Lit{.value = lower_value(opt(n, "$const"))}, p, t);   // a library constant (int.MaxValue): its value
        }
        SymP s = required_sym(field);
        Node* obj = receiver(n, s);
        return mk(ir::Select{.obj = obj, .sym = s}, p, t);
    }
    if (k == "PropertyReference") {
        SymP s = required_sym(req(n, "property"));
        Node* obj = receiver(n, s);
        if (s->owner == "System.Array" && s->name == "Length") return mk(ir::Length{.arr = obj}, p, t);
        if (std::find(s->flags.begin(), s->flags.end(), "indexer") != s->flags.end()) {
            std::vector<Node*> arguments = args(n);
            if (arguments.size() != 1) throw Refuse("indexer", std::format("an indexer of {} arguments", arguments.size()));
            return mk(ir::Index{.arr = obj, .index = arguments[0], .sym = s}, p, t);
        }
        return mk(ir::Select{.obj = obj, .sym = s}, p, t);
    }
    if (k == "EventReference") throw Refuse("event", "an event");
    if (k == "Invocation") return call(n, t, p);
    if (k == "ObjectCreation") return object_creation(n, t, p);
    if (k == "TypeParameterObjectCreation") throw Refuse("new-T", "`new T()` of a type parameter");
    if (k == "AnonymousObjectCreation") throw Refuse("anonymous-type", "an anonymous type");
    if (k == "ArrayCreation") return array_creation(n, t, p);
    if (k == "ArrayElementReference") {
        Node* arr = expr(req(n, "arrayReference"));
        std::vector<Node*> idx;
        for (const Json& i : list(n, "indices")) idx.push_back(expr(i));
        ir::Index index{.arr = arr};
        if (idx.size() == 1) index.index = idx[0];
        else index.index = std::move(idx);
        return mk(std::move(index), p, t);
    }
    if (k == "Binary") return binary(n, t, p);
    if (k == "Unary") {
        if (opt(n, "operatorMethod") != nullptr) {
            SymP s = required_sym(req(n, "operatorMethod"));
            return op_call(s, {expr(req(n, "operand"))}, t, p);
        }
        const std::string_view op = unary_op(text_or_empty(n, "operatorKind"));
        if (op.empty() || flag(n, "isLifted")) {
            const std::string_view kind = opt(n, "operatorKind") != nullptr ? text_or_empty(n, "operatorKind") : "None";
            throw Refuse("unary", std::format("the unary operator {}{}", kind, flag(n, "isLifted") ? " (lifted)" : ""));
        }
        ir::Unary u{.op = std::string(op), .x = expr(req(n, "operand"))};
        if (flag(n, "isChecked")) u.overflow = ir::Overflow::Trap;
        return mk(std::move(u), p, t);
    }
    if (k == "Increment" || k == "Decrement") {
        if (opt(n, "operatorMethod") != nullptr || flag(n, "isLifted")) throw Refuse("increment", "a user-defined or lifted ++/--");
        ++effects_;
        ir::Unary u{.op = k == "Increment" ? "++" : "--", .x = expr(req(n, "target")), .postfix = flag(n, "isPostfix")};
        if (flag(n, "isChecked")) u.overflow = ir::Overflow::Trap;
        return mk(std::move(u), p, t);
    }
    if (k == "SimpleAssignment") {
        const Json& target_record = req(n, "target");
        const std::string& tk = text_of(target_record, "k");
        if (tk == "Tuple" || tk == "Discard" || flag(n, "isRef")) throw Refuse("deconstruction", "a deconstructing, discarding or ref assignment");
        Node* target = expr(target_record);
        Node* value = expr(req(n, "value"));
        ++effects_;
        return mk(ir::Assign{.target = target, .value = value}, p, t);
    }
    if (k == "DeconstructionAssignment") throw Refuse("deconstruction", "a deconstruction");
    if (k == "CompoundAssignment") {
        if (opt(n, "operatorMethod") != nullptr && !is_string(t)) throw Refuse("compound", "a compound assignment through a user-defined operator");
        if (flag(n, "isLifted")) throw Refuse("lifted", "lifted nullable arithmetic");
        const std::string_view op = binary_op(text_or_empty(n, "operatorKind"));
        if (op.empty()) throw Refuse("compound", std::format("the compound operator {}", text_or_empty(n, "operatorKind")));
        Node* target = expr(req(n, "target"));
        Node* value = expr(req(n, "value"));
        ++effects_;
        ir::Assign a{.op = std::string(op), .target = target, .value = value};
        if (flag(n, "isChecked")) a.overflow = ir::Overflow::Trap;
        return mk(std::move(a), p, t);
    }
    if (k == "CoalesceAssignment") {
        Node* target = expr(req(n, "target"));
        Node* value = nullptr;
        {
            MaybeSkipped skipped(*this);
            value = expr(req(n, "value"));
        }
        ++effects_;
        return mk(ir::Assign{.op = "??", .target = target, .value = value}, p, t);
    }
    if (k == "Conditional") {
        Node* cond = expr(req(n, "condition"));
        Node* a = nullptr;
        Node* b = nullptr;
        {
            MaybeSkipped skipped(*this);
            a = expr(req(n, "whenTrue"));
            b = expr(req(n, "whenFalse"));
        }
        return mk(ir::Cond{.cond = cond, .then = a, .orelse = b}, p, t);
    }
    if (k == "Coalesce") {
        Node* left = expr(req(n, "value"));
        Node* right = nullptr;
        {
            MaybeSkipped skipped(*this);
            right = expr(req(n, "whenNull"));
        }
        return mk(ir::Binary{.op = "??", .left = left, .right = right}, p, t);
    }
    if (k == "ConditionalAccess") return cond_access(n, t, p);
    if (k == "Conversion") return conversion(n, t, p);
    if (k == "IsType") {
        Node* x = mk(ir::InstanceOf{.x = expr(req(n, "valueOperand")), .test = lower_type(opt(n, "typeOperand"))}, p, ir::bool_type());
        return flag(n, "isNegated") ? not_of(x, p) : x;
    }
    if (k == "IsPattern") {
        Node* v = expr(req(n, "value"));
        return pattern(req(n, "pattern"), v, p);
    }
    if (k == "SwitchExpression") return switch_expr(n, t, p);
    if (k == "DelegateCreation") {
        const Json& target = req(n, "target");
        const std::string& tk = text_of(target, "k");
        if (tk == "AnonymousFunction") return lambda(target, t, p);
        if (tk == "MethodReference") {
            SymP s = required_sym(req(target, "method"));
            const Json* inst = opt(target, "instance");
            Node* obj = nullptr;
            if (inst != nullptr && !flag(*inst, "implicit")) obj = expr(*inst);
            else if (s->is_static) obj = type_name(class_of(s->owner), p);
            else obj = mk(ir::This{}, p, class_of(s->owner));
            return mk(ir::MethodRef{.target = obj, .sym = s, .mode = "invoke"}, p, t);
        }
        throw Refuse("delegate", std::format("a delegate made from {}", tk));
    }
    if (k == "AnonymousFunction") return lambda(n, t, p);
    if (k == "InstanceReference") {
        if (text_or_empty(n, "referenceKind") == "ImplicitReceiver") {
            if (receiver_now_ == nullptr) throw Refuse("initializer", "an initialiser receiver outside its creation");
            return copy(receiver_now_);
        }
        return mk(ir::This{}, p, t);
    }
    if (k == "ConditionalAccessInstance") {
        if (cond_recv_ == nullptr) throw Refuse("null-conditional", "a `?.` receiver outside its access");
        return copy(cond_recv_);
    }
    if (k == "TypeOf") return mk(ir::ClassLit{.of = lower_type(opt(n, "typeOperand"))}, p, t);
    if (k == "DefaultValue") return default_value(n, t, p);
    if (k == "InterpolatedString") return interpolated(n, p);
    if (k == "Throw") throw Refuse("throw-expression", "a throw expression");
    if (k == "Await") throw Refuse("await", "`await`: an asynchronous method (a state machine)");
    if (k == "DeclarationExpression") {
        const Json* inner = opt(n, "expression");
        if (inner != nullptr && text_or_empty(*inner, "k") == "LocalReference") return expr(*inner);   // out T x
    }
    if (k == "Tuple" || k == "DeclarationExpression") throw Refuse("tuple", "a tuple");
    if (k == "Discard") throw Refuse("discard", "a discard");
    if (k == "SizeOf" || k == "AddressOf" || k == "PointerIndirectionReference") throw Refuse("unsafe", "unsafe code");
    if (k.starts_with("Dynamic")) throw Refuse("dynamic", "`dynamic`");
    if (k == "Invalid") throw Refuse("invalid", "code the compiler could not bind");
    if (k == "TranslatedQuery") return expr(req(n, "operation"));   // query syntax: the calls it means
    if (k == "Parenthesized") return mk(ir::Paren{.x = expr(req(n, "operand"))}, p, t);
    throw Refuse(k, std::format("the operation {}", k));
}

// The object a member is read on: This (implicit or written), its class for a static, the receiver of an object
// initialiser, or an expression.
Node* Lowering::receiver(const Json& n, const SymP& s) {
    const Json* inst = opt(n, "instance");
    const Pos p = pos_of(n);
    if (inst == nullptr || s->is_static) return type_name(class_of(s->owner), p);
    if (text_of(*inst, "k") == "InstanceReference") {
        if (text_or_empty(*inst, "referenceKind") == "ImplicitReceiver") return expr_inner(*inst);
        TypeP it = lower_type(opt(*inst, "$type"));
        if (cls_ != nullptr && is_class(it) && class_name(it) != cls_->qname && !flag(*inst, "implicit")) {
            return mk(ir::Super{}, p, it);   // base.X
        }
        return mk(ir::This{}, p, it);
    }
    return expr(*inst);
}

// A call's arguments in parameter order, as written: a `params` array spread back out, defaults the compiler filled in kept
// (every target then sees the value).
std::vector<Node*> Lowering::args(const Json& n) {
    struct Argument {
        std::uint64_t ordinal;
        std::vector<Node*> values;
    };
    std::vector<Argument> items;
    const Json::array_t& written = list(n, "arguments");
    for (std::size_t i = 0; i < written.size(); ++i) {
        const Json& a = written[i];
        const Json* parameter = opt(a, "parameter");
        std::uint64_t ordinal = i;
        if (parameter != nullptr) {
            if (const Json* o = opt(*parameter, "ordinal")) {
                if (!o->is_number_unsigned()) bad(a, "a parameter ordinal that is not a count");
                ordinal = o->get<std::uint64_t>();
            }
        }
        const Json& v = req(a, "value");
        Argument item{ordinal, {}};
        if (text_or_empty(a, "argumentKind") == "ParamArray" && text_of(v, "k") == "ArrayCreation" && flag(v, "implicit")) {
            if (const Json* init = opt(v, "initializer")) {
                for (const Json& e : list(*init, "elementValues")) item.values.push_back(expr(e));
            }
            items.push_back(std::move(item));
            continue;
        }
        Node* x = expr(v);
        if (text_or_empty(a, "argumentKind") == "DefaultValue" && parameter != nullptr) {
            const TypeP pt = lower_type(opt(*parameter, "type"));
            if (x->type != pt && !is_any(pt)) x = mk(ir::Cast{.x = x}, pos_of(n), pt);   // the default as the parameter's type, so an
                                                                                         // overload is chosen as the compiler chose it
        }
        item.values.push_back(x);
        items.push_back(std::move(item));
    }
    const bool in_order = std::is_sorted(items.begin(), items.end(), [](const Argument& a, const Argument& b) { return a.ordinal < b.ordinal; });
    if (!in_order) {
        // named arguments out of parameter order: C# evaluates them as written, so only where that is no observable change
        for (const Argument& a : items) {
            for (Node* v : a.values) {
                if (has_effects(v)) throw Refuse("named-args", "named arguments out of parameter order, with effects");
            }
        }
        std::stable_sort(items.begin(), items.end(), [](const Argument& a, const Argument& b) { return a.ordinal < b.ordinal; });
    }
    std::vector<Node*> out;
    for (const Argument& a : items) out.insert(out.end(), a.values.begin(), a.values.end());
    return out;
}

Node* Lowering::call(const Json& n, const TypeP& t, Pos p) {
    const Json& target_method = req(n, "targetMethod");
    SymP s = required_sym(target_method);
    const Json* inst = opt(n, "instance");
    Node* target = nullptr;
    if (inst == nullptr) {
        if (s->is_static) target = type_name(class_of(s->owner), p);
    } else if (text_of(*inst, "k") == "InstanceReference" && flag(*inst, "implicit") && text_or_empty(*inst, "referenceKind") != "ImplicitReceiver") {
        target = nullptr;   // own method, implicit this
    } else {
        target = receiver(n, s);
    }
    std::vector<Node*> arguments = args(n);
    ++effects_;
    std::vector<ir::TypeArg> typeargs;
    for (const Json& a : list(target_method, "typeargs")) typeargs.emplace_back(lower_type(a));
    if (!typeargs.empty() && !flag(n, "typeArgsWritten")) {   // the compiler inferred them: a back end may too
        s = copy_of(s);
        s->flags.emplace_back("inferred");
    }
    return mk(ir::Call{.target = target, .sym = s, .args = std::move(arguments), .typeargs = std::move(typeargs)}, p, t);
}

// A user-defined operator or conversion, or a BCL operator that is a method: a static call.
Node* Lowering::op_call(const SymP& sym, std::vector<Node*> arguments, const TypeP& t, Pos p) {
    SymP s = copy_of(sym);
    s->is_static = true;
    std::erase(s->flags, "operator");
    s->flags.emplace_back("operator");
    Node* target = type_name(class_of(s->owner), p);
    return mk(ir::Call{.target = target, .sym = s, .args = std::move(arguments)}, p, t);
}

Node* Lowering::object_creation(const Json& n, const TypeP& t, Pos p) {
    const Json* ctor = opt(n, "constructor");
    SymP s;
    if (ctor != nullptr) {
        s = required_sym(*ctor);
    } else {
        s = ir::make_sym(ir::SymKind::Ctor, ".ctor", is_class(t) ? std::optional<std::string>(class_name(t)) : std::nullopt);
        s->ret = ir::void_type();
    }
    std::vector<Node*> arguments;
    if (ctor != nullptr) arguments = args(n);
    ++effects_;
    Node* e = mk(ir::New{.sym = s, .args = std::move(arguments), .memory = ir::MemoryModel::Gc}, p, t);
    const Json* init = opt(n, "initializer");
    const Json* initializers = init != nullptr ? opt(*init, "initializers") : nullptr;
    if (initializers == nullptr || initializers->empty()) return e;
    // an object or collection initialiser: a temporary, then each member set or Add, before the statement
    const int effects = effects_;
    --effects_;
    Node* tmp = temp(t, e, n);
    effects_ = effects;
    const Node* const prev = receiver_now_;
    receiver_now_ = tmp;
    struct Restore {
        Lowering& l;
        const Node* prev;
        ~Restore() { l.receiver_now_ = prev; }
    } restore{*this, prev};
    std::vector<Node*> set_members;
    for (const Json& i : *initializers) {   // each member set or Add: a statement of its own
        Node* x = nullptr;
        {
            Scope scope(*this, &set_members);
            x = expr(i);
        }
        set_members.push_back(mk(ir::ExprStmt{.expr = x}, pos_of(i)));
    }
    pending_->insert(pending_->end(), set_members.begin(), set_members.end());
    return tmp;
}

Node* Lowering::array_creation(const Json& n, const TypeP& t, Pos p) {
    const auto* array = t ? std::get_if<ir::ArrayType>(&t->body) : nullptr;
    TypeP elem = ir::any_type();
    if (array != nullptr) elem = array->rank != 1 ? array->elem : ir::dims(t).second;   // jagged arrays nest: the innermost element
    const Json* init = opt(n, "initializer");
    std::vector<Node*> dims;
    std::vector<Node*> items;
    if (init == nullptr) {
        for (const Json& d : list(n, "dimensionSizes")) dims.push_back(expr(d));
    } else {
        if (array != nullptr && array->rank != 1) throw Refuse("array", "a rectangular array initialiser");
        for (const Json& e : list(*init, "elementValues")) items.push_back(expr(e));
        // (an IR slot with no items is an absent one, so an empty initialiser is said as the length it gives)
        if (items.empty()) dims.push_back(mk(ir::Lit{.value = std::int64_t{0}}, p, ir::i32_type()));
    }
    return mk(ir::NewArray{.elem = elem, .dims = std::move(dims), .init = std::move(items)}, p, t);
}

Node* Lowering::concat(Node* left, Node* right, const TypeP& t, Pos p) {
    std::vector<Node*> parts;
    for (Node* x : {left, right}) {
        if (const auto* c = ir::get<ir::Concat>(x)) parts.insert(parts.end(), c->parts.begin(), c->parts.end());
        else parts.push_back(x);
    }
    return mk(ir::Concat{.parts = std::move(parts)}, p, t);
}

Node* Lowering::binary(const Json& n, const TypeP& t, Pos p) {
    const std::string_view kind = text_or_empty(n, "operatorKind");
    if (opt(n, "operatorMethod") != nullptr) {
        SymP s = required_sym(req(n, "operatorMethod"));
        Node* left = expr(req(n, "leftOperand"));
        if (kind == "ConditionalAnd" || kind == "ConditionalOr") throw Refuse("operator", "a user-defined && or ||");
        Node* right = expr(req(n, "rightOperand"));
        if ((s->owner == "System.String" && s->name == "op_Addition") || (is_string(t) && kind == "Add")) return concat(left, right, t, p);
        return op_call(s, {left, right}, t, p);
    }
    const std::string_view op = binary_op(kind);
    if (op.empty()) throw Refuse("binary", std::format("the operator {}", opt(n, "operatorKind") != nullptr ? kind : std::string_view("None")));
    Node* left = expr(req(n, "leftOperand"));
    const bool short_circuit = op == "&&" || op == "||";
    Node* right = nullptr;
    {
        std::optional<MaybeSkipped> skipped;
        if (short_circuit) skipped.emplace(*this);
        right = expr(req(n, "rightOperand"));
    }
    if (flag(n, "isLifted") && !((op == "==" || op == "!=") && (is_null_literal(left) || is_null_literal(right)))) {
        throw Refuse("lifted", std::format("lifted nullable arithmetic ({} on a nullable value)", op));
    }
    if (op == "+" && is_string(t)) return concat(left, right, t, p);
    ir::Binary b{.op = std::string(op), .left = left, .right = right};
    if (flag(n, "isChecked")) b.overflow = ir::Overflow::Trap;
    return mk(std::move(b), p, t);
}

Node* Lowering::conversion(const Json& n, const TypeP& t, Pos p) {
    static const Json kNone = Json::object();
    const Json* conv = opt(n, "conversion");
    const Json& c = conv != nullptr ? *conv : kNone;
    const Json& operand = req(n, "operand");
    if (flag(c, "userDefined") && opt(c, "method") != nullptr) {
        SymP s = required_sym(req(c, "method"));
        return op_call(s, {expr(operand)}, t, p);
    }
    if (flag(n, "isTryCast")) {   // x as T
        Node* x = expr(operand);
        if (!can_read_twice(x)) throw Refuse("as", "`as` on an expression with effects");
        Node* test = mk(ir::InstanceOf{.x = x, .test = t}, p, ir::bool_type());
        Node* then = mk(ir::Cast{.x = copy(x)}, p, t);
        return mk(ir::Cond{.cond = test, .then = then, .orelse = null_literal(p)}, p, t);
    }
    Node* x = expr(operand);
    if (flag(n, "implicit")) {   // the compiler's, not written: left implicit
        if (auto* lit = ir::get<ir::Lit>(x); !flag(c, "identity") && lit != nullptr && std::holds_alternative<std::monostate>(lit->value)) {
            if (!is_any(t)) x->type = t;   // null as a typed null
        }
        return x;
    }
    ir::Cast cast{.x = x};
    if (flag(n, "isChecked")) cast.overflow = ir::Overflow::Trap;
    return mk(std::move(cast), p, t);
}

// a?.b: when `a` can be read twice (a name, this, a field of one), a test and the access.
Node* Lowering::cond_access(const Json& n, const TypeP& t, Pos p) {
    Node* recv = expr(req(n, "operation"));
    if (!can_read_twice(recv)) throw Refuse("null-conditional", "`?.` on an expression with effects");
    const Node* const prev = cond_recv_;
    cond_recv_ = recv;
    struct Restore {
        Lowering& l;
        const Node* prev;
        ~Restore() { l.cond_recv_ = prev; }
    } restore{*this, prev};
    Node* inner = nullptr;
    {
        MaybeSkipped skipped(*this);
        inner = expr(req(n, "whenNotNull"));
    }
    Node* test = mk(ir::Binary{.op = "!=", .left = recv, .right = null_literal(p)}, p, ir::bool_type());
    return mk(ir::Cond{.cond = test, .then = inner, .orelse = null_literal(p)}, p, t);
}

Node* Lowering::default_value(const Json& n, const TypeP& t, Pos p) {
    if (const auto* prim = t ? std::get_if<ir::PrimType>(&t->body) : nullptr) {
        switch (prim->prim) {
            case ir::Prim::I8: case ir::Prim::I16: case ir::Prim::I32: case ir::Prim::I64:
            case ir::Prim::U8: case ir::Prim::U16: case ir::Prim::U32: case ir::Prim::U64:
                // (a char is the one integer with an encoding: its default is the NUL character)
                return mk(ir::Lit{.value = prim->encoding ? ir::LitValue(std::string(1, '\0')) : ir::LitValue(std::int64_t{0})}, p, t);
            case ir::Prim::F32: case ir::Prim::F64: return mk(ir::Lit{.value = 0.0}, p, t);
            case ir::Prim::Bool: return mk(ir::Lit{.value = false}, p, t);
            default: break;
        }
    }
    const Json* c = opt(n, "$const");
    if (c != nullptr && text_or_empty(*c, "t") == "null") return mk(ir::Lit{}, p, t);
    throw Refuse("default", std::format("`default` of {}", t ? ir::to_text(*t) : std::string("?")));
}

Node* Lowering::interpolated(const Json& n, Pos p) {
    std::vector<Node*> parts;
    for (const Json& part : list(n, "parts")) {
        const std::string& pk = text_of(part, "k");
        if (pk == "InterpolatedStringText") {
            const Json& text = req(part, "text");
            parts.push_back(mk(ir::Lit{.value = lower_value(opt(text, "$const"))}, pos_of(part), string_type()));
        } else if (pk == "Interpolation") {
            if (opt(part, "alignment") != nullptr || opt(part, "formatString") != nullptr) {
                throw Refuse("interpolation-format", "an interpolation with an alignment or a format string");
            }
            parts.push_back(expr(req(part, "expression")));
        } else {
            throw Refuse("interpolation", std::format("an interpolated part {}", pk));
        }
    }
    if (parts.empty()) return mk(ir::Lit{.value = std::string()}, p, string_type());
    if (parts.size() == 1 && is_string(parts[0]->type)) parts.insert(parts.begin(), mk(ir::Lit{.value = std::string()}, p, string_type()));
    return mk(ir::Concat{.parts = std::move(parts)}, p, string_type());
}

Node* Lowering::lambda(const Json& n, const TypeP& t, Pos p) {
    const Json* ms = opt(n, "symbol");
    const std::string owner = (ms != nullptr ? std::string(text_or_empty(*ms, "owner")) : std::string()) + ".lambda";
    std::vector<Node*> parameters;
    if (ms != nullptr) {
        const Json::array_t& names = list(*ms, "pnames");
        const Json::array_t& types = list(*ms, "params");
        for (std::size_t i = 0; i < std::min(names.size(), types.size()); ++i) {
            TypeP pt = lower_type(types[i]);
            parameters.push_back(mk(ir::Param{.sym = ir::make_sym(ir::SymKind::Param, names[i].get<std::string>(), owner, false, pt), .type = pt}, {}));
        }
    }
    Node* body = nullptr;
    {
        Scope scope(*this, nullptr);
        body = method_body(&req(n, "body"));
    }
    if (const auto* b = ir::get<ir::Block>(body); b != nullptr && b->stmts.size() == 1 && is_implicit_body(req(n, "body"))) {
        if (const auto* r = ir::get<ir::Return>(b->stmts[0]); r != nullptr && r->expr != nullptr) body = r->expr;   // x => x + 1
    }
    return mk(ir::Lambda{.params = std::move(parameters), .body = body}, p, t);
}

// ---- patterns -----------------------------------------------------------------------------------------------------------

// `v` is read where the pattern needs it: each place it is put in the tree gets a copy.
Node* Lowering::pattern(const Json& pat, const Node* v, Pos p) {
    const std::string& k = text_of(pat, "k");
    const TypeP boolean = ir::bool_type();
    if (k == "ConstantPattern") {
        Node* c = expr(req(pat, "value"));
        if (is_null_literal(c)) return mk(ir::Binary{.op = "==", .left = copy(v), .right = c}, p, boolean);
        if (is_string(v->type) || is_string(c->type)) {
            SymP s = ir::make_sym(ir::SymKind::Method, "op_Equality", "System.String", true);
            s->params = {string_type(), string_type()};
            s->ret = boolean;
            s->flags = {"operator"};
            return mk(ir::Call{.target = type_name(string_type(), p), .sym = s, .args = {copy(v), c}}, p, boolean);
        }
        return mk(ir::Binary{.op = "==", .left = copy(v), .right = c}, p, boolean);
    }
    if (k == "DeclarationPattern") {
        const Json* matched = opt(pat, "matchedType");
        if (matched == nullptr) matched = opt(pat, "narrowedType");
        TypeP mt = lower_type(matched);
        const Json* ds = opt(pat, "declaredSymbol");
        if (flag(pat, "matchesNull")) throw Refuse("var-pattern", "a `var` pattern");
        Node* binding = ds != nullptr ? mk(ir::Local{.sym = renamed(required_sym(*ds)), .type = mt}, p) : nullptr;
        return mk(ir::InstanceOf{.x = copy(v), .test = mt, .binding = binding}, p, boolean);
    }
    if (k == "TypePattern") return mk(ir::InstanceOf{.x = copy(v), .test = lower_type(opt(pat, "matchedType"))}, p, boolean);
    if (k == "RelationalPattern") {
        const std::string_view op = binary_op(text_or_empty(pat, "operatorKind"));
        if (op.empty()) throw Refuse("pattern", std::format("the relational operator {}", text_or_empty(pat, "operatorKind")));
        return mk(ir::Binary{.op = std::string(op), .left = copy(v), .right = expr(req(pat, "value"))}, p, boolean);
    }
    if (k == "NegatedPattern") return not_of(pattern(req(pat, "pattern"), v, p), p);
    if (k == "BinaryPattern") {
        if (!can_read_twice(v)) throw Refuse("pattern", "an and/or pattern on an expression with effects");
        const char* op = text_or_empty(pat, "operatorKind") == "And" ? "&&" : "||";
        Node* left = pattern(req(pat, "leftPattern"), v, p);
        Node* right = pattern(req(pat, "rightPattern"), v, p);
        return mk(ir::Binary{.op = op, .left = left, .right = right}, p, boolean);
    }
    if (k == "DiscardPattern") return bool_literal(true, p);
    if (k == "RecursivePattern") {
        if (opt(pat, "deconstructionSubpatterns") != nullptr && !opt(pat, "deconstructionSubpatterns")->empty()) {
            throw Refuse("pattern", "a positional (deconstructing) pattern");
        }
        TypeP mt = lower_type(opt(pat, "matchedType"));
        const Json* ds = opt(pat, "declaredSymbol");
        const Json::array_t& subs = list(pat, "propertySubpatterns");
        if (!subs.empty() && ds == nullptr && !can_read_twice(v)) throw Refuse("pattern", "a property pattern on an expression with effects");
        Node* test = nullptr;
        const Node* object = nullptr;
        Node* made_object = nullptr;   // (the receiver the sub-patterns read: a new node, or `v` itself)
        if (!is_any(mt) && mt != v->type) {
            Node* binding = ds != nullptr ? mk(ir::Local{.sym = renamed(required_sym(*ds)), .type = mt}, p) : nullptr;
            test = mk(ir::InstanceOf{.x = copy(v), .test = mt, .binding = binding}, p, boolean);
            made_object = ds != nullptr ? mk(ir::Name{.sym = renamed(required_sym(*ds))}, p, mt) : mk(ir::Cast{.x = copy(v)}, p, mt);
            object = made_object;
        } else {
            if (is_class(v->type)) test = mk(ir::Binary{.op = "!=", .left = copy(v), .right = null_literal(p)}, p, boolean);
            object = v;
        }
        for (const Json& sp : subs) {
            const Json* m = opt(sp, "member");
            const std::string_view mk_kind = m != nullptr ? text_or_empty(*m, "k") : std::string_view();
            if (mk_kind != "PropertyReference" && mk_kind != "FieldReference") throw Refuse("pattern", "a nested property pattern");
            const Json* msym = opt(*m, "property");
            if (msym == nullptr) msym = opt(*m, "field");
            if (msym == nullptr) bad(*m, "a member reference with no symbol");
            SymP ms = required_sym(*msym);
            Node* val = mk(ir::Select{.obj = copy(object), .sym = ms}, p, ms->type);
            Node* sub = pattern(req(sp, "pattern"), val, p);
            test = test == nullptr ? sub : mk(ir::Binary{.op = "&&", .left = test, .right = sub}, p, boolean);
        }
        return test != nullptr ? test : bool_literal(true, p);
    }
    throw Refuse("pattern", std::format("the pattern {}", k));
}

Node* Lowering::switch_expr(const Json& n, const TypeP& t, Pos p) {
    Node* v = expr(req(n, "value"));
    const Json::array_t& arms = list(n, "arms");
    auto arm_kind = [](const Json& a) { return text_or_empty(req(a, "pattern"), "k"); };
    const bool plain = !arms.empty() && arm_kind(arms.back()) == "DiscardPattern" &&
                       std::all_of(arms.begin(), arms.end(), [&](const Json& a) {
                           const std::string_view ak = arm_kind(a);
                           return (ak == "ConstantPattern" || ak == "DiscardPattern") && opt(a, "guard") == nullptr;
                       });
    if (plain) {
        // the labels are lowered once and kept if none is null (a `null` arm is a test, not a case)
        std::vector<Node*> labels;
        bool has_null = false;
        for (const Json& a : arms) {
            if (arm_kind(a) != "ConstantPattern") {
                labels.push_back(nullptr);
                continue;
            }
            labels.push_back(expr(req(req(a, "pattern"), "value")));
            if (is_null_literal(labels.back())) {
                has_null = true;
                break;
            }
        }
        if (!has_null) {
            std::vector<Node*> cases;
            MaybeSkipped skipped(*this);
            for (std::size_t i = 0; i < arms.size(); ++i) {
                const Json& a = arms[i];
                const bool is_default = arm_kind(a) == "DiscardPattern";
                ir::Case c;
                c.is_default = is_default;
                if (!is_default) c.labels = {labels[i]};
                c.body = {mk(ir::Yield{.expr = expr(req(a, "value"))}, pos_of(a))};
                cases.push_back(mk(std::move(c), pos_of(a)));
            }
            return mk(ir::SwitchExpr{.expr = v, .cases = std::move(cases)}, p, t);
        }
    }
    if (!can_read_twice(v)) v = temp(v->type, v, n);
    if (arms.empty() || arm_kind(arms.back()) != "DiscardPattern" || opt(arms.back(), "guard") != nullptr) {
        throw Refuse("switch-expression", "a switch expression with no `_` arm (it throws when none matches)");
    }
    // the arms become one expression, so a name two arms both bind is given a number in the later ones
    std::set<std::string> bound;
    std::vector<std::map<std::string, std::string>> renames;
    for (std::size_t i = 0; i < arms.size(); ++i) {
        std::map<std::string, std::string> r;
        std::vector<std::string> names;
        collect_bindings(req(arms[i], "pattern"), names);
        for (const std::string& x : names) {
            if (bound.contains(x)) r[x] = std::format("{}{}", x, i + 1);
            bound.insert(x);
        }
        renames.push_back(std::move(r));
    }
    MaybeSkipped skipped(*this);
    const std::map<std::string, std::string> saved = rename_;
    struct Restore {
        Lowering& l;
        const std::map<std::string, std::string>& saved;
        ~Restore() { l.rename_ = saved; }
    } restore{*this, saved};
    auto renamed_with = [&](const std::map<std::string, std::string>& r) {
        std::map<std::string, std::string> m = saved;
        for (const auto& [from, to] : r) m[from] = to;
        return m;
    };
    rename_ = renamed_with(renames.back());
    Node* out = expr(req(arms.back(), "value"));
    for (std::size_t i = arms.size() - 1; i-- > 0;) {
        const Json& a = arms[i];
        rename_ = renamed_with(renames[i]);
        Node* test = pattern(req(a, "pattern"), v, pos_of(a));
        if (opt(a, "guard") != nullptr) test = mk(ir::Binary{.op = "&&", .left = test, .right = expr(req(a, "guard"))}, pos_of(a), ir::bool_type());
        Node* then = expr(req(a, "value"));
        out = mk(ir::Cond{.cond = test, .then = then, .orelse = out}, pos_of(a), t);
    }
    return out;
}

// ---- parentheses --------------------------------------------------------------------------------------------------------

// How tightly an expression binds, for deciding where Roslyn's bare tree needs parentheses.
int precedence(const Node* e) {
    if (const auto* b = ir::get<ir::Binary>(e)) return binary_precedence(b->op);
    if (ir::is<ir::Concat>(e)) return 13;
    if (ir::is<ir::Cond>(e)) return 3;
    if (ir::is<ir::Assign>(e) || ir::is<ir::Lambda>(e)) return 2;
    if (ir::is<ir::InstanceOf>(e)) return 11;
    if (const auto* u = ir::get<ir::Unary>(e)) return u->postfix ? 16 : 15;
    if (ir::is<ir::Cast>(e)) return 15;
    return 16;
}

}  // namespace

void group(Arena& arena, Node* root) {
    auto wrap = [&](Node*& child, int need) {
        if (child != nullptr && ir::category(*child) == ir::Category::Expression && !ir::is<ir::Paren>(child) && precedence(child) < need) {
            child = ir::make(arena, ir::Paren{.x = child}, child->type, child->line, child->col);
        }
    };
    for (Node* n : ir::walk(root)) {
        if (auto* b = ir::get<ir::Binary>(n)) {
            const int p = binary_precedence(b->op);
            const bool coalesce = b->op == "??";   // (right associative)
            wrap(b->left, coalesce ? p + 1 : p);
            wrap(b->right, coalesce ? p : p + 1);
        } else if (auto* c = ir::get<ir::Concat>(n)) {
            for (std::size_t i = 0; i < c->parts.size(); ++i) wrap(c->parts[i], i == 0 ? 13 : 14);
        } else if (auto* u = ir::get<ir::Unary>(n)) {
            wrap(u->x, 15);
        } else if (auto* cast = ir::get<ir::Cast>(n)) {
            wrap(cast->x, 15);
        } else if (auto* test = ir::get<ir::InstanceOf>(n)) {
            wrap(test->x, 12);
        } else if (auto* cond = ir::get<ir::Cond>(n)) {
            wrap(cond->cond, 4);
            wrap(cond->then, 3);
            wrap(cond->orelse, 3);
        } else if (auto* sel = ir::get<ir::Select>(n)) {
            wrap(sel->obj, 16);
        } else if (auto* call = ir::get<ir::Call>(n)) {
            wrap(call->target, 16);
        } else if (auto* index = ir::get<ir::Index>(n)) {
            wrap(index->arr, 16);
        } else if (auto* length = ir::get<ir::Length>(n)) {
            wrap(length->arr, 16);
        }
    }
}

Lowered lower(Arena& arena, const Json& tree, std::optional<std::string> path) {
    Lowering lowering(arena, std::move(path));
    Node* unit = lowering.unit(tree);
    group(arena, unit);
    return {unit, std::move(lowering.refused)};
}

Lowered lower_text(Arena& arena, std::string_view dump_text, std::optional<std::string> path) {
    return lower(arena, json::parse(dump_text), std::move(path));
}

}  // namespace cb::translate::frontends::csharp

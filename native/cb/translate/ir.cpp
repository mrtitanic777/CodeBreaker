// cb/translate/ir.cpp -- the translator's intermediate representation (see ir.hpp).
#include "cb/translate/ir.hpp"

#include <algorithm>
#include <functional>
#include <type_traits>

namespace cb::translate::ir {
namespace {

using std::string;
using std::string_view;

template <class K, class... Ts> inline constexpr bool kOneOf = (std::is_same_v<K, Ts> || ...);

// A boost-style combine; the hash only has to agree for equal types.
std::size_t combine(std::size_t seed, std::size_t value) { return seed ^ (value + 0x9E3779B97F4A7C15ull + (seed << 6) + (seed >> 2)); }

}  // namespace

// ---- enums as text -------------------------------------------------------------------------------------------------------

std::string_view to_text(Prim prim) {
    constexpr string_view names[] = {"bool", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "f80", "char"};
    return names[static_cast<std::size_t>(prim)];
}

std::string_view to_text(AddressSpace space) {
    constexpr string_view names[] = {"flat", "near", "far", "huge", "real", "io"};
    return names[static_cast<std::size_t>(space)];
}

std::string_view to_text(MemoryModel memory) {
    constexpr string_view names[] = {"gc", "owned", "borrowed", "stack", "manual", "emulated"};
    return names[static_cast<std::size_t>(memory)];
}

std::string_view to_text(Overflow overflow) {
    constexpr string_view names[] = {"wrap", "saturate", "trap"};
    return names[static_cast<std::size_t>(overflow)];
}

std::string_view to_text(CallingConvention conv) {
    constexpr string_view names[] = {"cdecl", "pascal", "stdcall", "fastcall", "thiscall", "register", "interrupt"};
    return names[static_cast<std::size_t>(conv)];
}

std::string_view to_text(SymKind kind) {
    constexpr string_view names[] = {"local", "param", "field", "method", "ctor", "class", "enumconst", "typevar",
                                     "package", "global", "function", "label", "property", "event"};
    return names[static_cast<std::size_t>(kind)];
}

std::string_view to_text(PassMode mode) {
    constexpr string_view names[] = {"", "ref", "out", "in"};
    return names[static_cast<std::size_t>(mode)];
}

// ---- types ---------------------------------------------------------------------------------------------------------------

bool operator==(const TypeP& a, const TypeP& b) {
    if (a.get() == b.get()) return true;
    if (a.get() == nullptr || b.get() == nullptr) return false;
    return *a == *b;
}

bool Type::is_ref() const {
    return std::holds_alternative<ClassType>(body) || std::holds_alternative<ArrayType>(body) ||
           std::holds_alternative<TypeVarType>(body) || std::holds_alternative<NullType>(body) ||
           std::holds_alternative<WildcardType>(body);
}

std::string_view Type::kind() const {
    struct Visitor {
        string_view operator()(const PrimType& t) const { return to_text(t.prim); }
        string_view operator()(const VoidType&) const { return "void"; }
        string_view operator()(const NullType&) const { return "null"; }
        string_view operator()(const AnyType&) const { return "any"; }
        string_view operator()(const DynamicType&) const { return "dynamic"; }
        string_view operator()(const ClassType&) const { return "class"; }
        string_view operator()(const TypeVarType&) const { return "typevar"; }
        string_view operator()(const WildcardType&) const { return "wildcard"; }
        string_view operator()(const ArrayType&) const { return "array"; }
        string_view operator()(const PtrType&) const { return "ptr"; }
        string_view operator()(const SelectorType&) const { return "selector"; }
        string_view operator()(const FnType&) const { return "fn"; }
        string_view operator()(const FnPtrType&) const { return "fnptr"; }
        string_view operator()(const RecordType&) const { return "record"; }
    };
    return std::visit(Visitor{}, body);
}

std::string Type::simple() const {
    const string* name = nullptr;
    if (const auto* c = std::get_if<ClassType>(&body)) name = &c->name;
    else if (const auto* v = std::get_if<TypeVarType>(&body)) name = &v->name;
    else if (const auto* r = std::get_if<RecordType>(&body)) name = r->name ? &*r->name : nullptr;
    if (name == nullptr || name->empty()) return string(kind());
    return name->substr(name->rfind('.') == string::npos ? 0 : name->rfind('.') + 1);
}

std::size_t Type::hash() const {
    std::size_t h = std::hash<string_view>{}(kind());
    if (const auto* c = std::get_if<ClassType>(&body)) {
        h = combine(h, std::hash<string>{}(c->name));
        for (const TypeP& a : c->args) h = combine(h, a ? a->hash() : 0);
    } else if (const auto* v = std::get_if<TypeVarType>(&body)) {
        h = combine(h, std::hash<string>{}(v->name));
    } else if (const auto* r = std::get_if<RecordType>(&body)) {
        h = combine(h, r->name ? std::hash<string>{}(*r->name) : 0);
    } else if (const auto* a = std::get_if<ArrayType>(&body)) {
        h = combine(h, a->elem ? a->elem->hash() : 0);
    } else if (const auto* p = std::get_if<PtrType>(&body)) {
        h = combine(h, p->to ? p->to->hash() : 0);
    }
    return h;
}

std::pair<std::size_t, TypeP> dims(const TypeP& type) {
    std::size_t n = 0;
    TypeP t = type;
    while (t) {
        const auto* a = std::get_if<ArrayType>(&t->body);
        if (a == nullptr) break;
        ++n;
        t = a->elem;
    }
    return {n, t};
}

std::string to_text(const Type& type) {
    if (const auto* a = std::get_if<ArrayType>(&type.body)) return (a->elem ? to_text(*a->elem) : string("?")) + "[]";
    if (const auto* c = std::get_if<ClassType>(&type.body)) {
        string out = c->name;
        if (!c->args.empty()) {
            out += '<';
            for (std::size_t i = 0; i < c->args.size(); ++i) {
                if (i > 0) out += ", ";
                out += c->args[i] ? to_text(*c->args[i]) : string("?");
            }
            out += '>';
        }
        return out;
    }
    if (const auto* v = std::get_if<TypeVarType>(&type.body)) return v->name;
    if (const auto* w = std::get_if<WildcardType>(&type.body)) {
        if (!w->bound) return "?";
        return string("? ") + (w->bound->mode == BoundMode::Extends ? "extends " : "super ") + (w->bound->type ? to_text(*w->bound->type) : string("?"));
    }
    return string(type.kind());
}

TypeP make_type(Type::Body body) { return TypeP(std::make_shared<const Type>(Type{std::move(body)})); }

TypeP prim_type(Prim prim, std::optional<bool> is_signed, std::optional<CharEncoding> encoding) {
    return make_type(PrimType{prim, is_signed, encoding});
}

TypeP class_type(std::string name, std::vector<TypeP> args) {
    ClassType c;
    c.name = std::move(name);
    c.args = std::move(args);
    return make_type(std::move(c));
}

TypeP typevar_type(std::string name) { return make_type(TypeVarType{std::move(name)}); }

TypeP pointer_to(TypeP to, AddressSpace space, std::optional<SegmentRef> seg) {
    PtrType p;
    p.to = std::move(to);
    p.space = space;
    p.seg = std::move(seg);
    return make_type(std::move(p));
}

TypeP array_of(TypeP elem, std::size_t dims) {
    for (std::size_t i = 0; i < dims; ++i) {
        ArrayType a;
        a.elem = std::move(elem);
        elem = make_type(std::move(a));
    }
    return elem;
}

const TypeP& void_type() {
    static const TypeP t = make_type(VoidType{});
    return t;
}
const TypeP& null_type() {
    static const TypeP t = make_type(NullType{});
    return t;
}
const TypeP& any_type() {
    static const TypeP t = make_type(AnyType{});
    return t;
}
const TypeP& bool_type() {
    static const TypeP t = prim_type(Prim::Bool);
    return t;
}
const TypeP& i32_type() {
    static const TypeP t = prim_type(Prim::I32);
    return t;
}

// ---- symbols -------------------------------------------------------------------------------------------------------------

std::optional<std::string> Sym::owner_simple() const {
    if (!owner || owner->empty()) return std::nullopt;
    const std::size_t dot = owner->rfind('.');
    return owner->substr(dot == string::npos ? 0 : dot + 1);
}

SymP make_sym(SymKind kind, std::string name, std::optional<std::string> owner, bool is_static, TypeP type) {
    auto s = std::make_shared<Sym>();
    s->kind = kind;
    s->name = std::move(name);
    s->owner = std::move(owner);
    s->is_static = is_static;
    s->type = std::move(type);
    return s;
}

// ---- nodes ---------------------------------------------------------------------------------------------------------------

std::string_view kind_name(const Node& node) {
    return visit(node, [](const auto& k) { return std::decay_t<decltype(k)>::kName; });
}

Category category(const Node& node) {
    return visit(
        node, [](const auto& k) {
            using K = std::decay_t<decltype(k)>;
            if constexpr (kOneOf<K, Module, Unit, ClassDecl, FieldDecl, MethodDecl, Param, EnumConst, Property, TypeParam>) {
                return Category::Declaration;
            } else if constexpr (kOneOf<K, Block, Local, ExprStmt, If, While, DoWhile, For, ForEach, Switch, Case, Break, Continue, Return,
                                        Throw, Try, Catch, Labeled, Goto, MachineFragment, Sync, Empty, Comment, LocalClass, Assert,
                                        CtorCall, Yield>) {
                return Category::Statement;
            } else {
                return Category::Expression;
            }
        });
}

std::vector<Node*> children(const Node& node) {
    std::vector<Node*> out;
    // (the slots are read, not changed: the walk only needs the same accessor a rewriting pass uses)
    visit(const_cast<Node&>(node), [&out](auto& k) { k.each_child([&out](Node*& child) { out.push_back(child); }); });
    return out;
}

std::vector<Node*> walk(Node* root) {
    std::vector<Node*> out;
    if (root == nullptr) return out;
    std::vector<Node*> stack{root};
    while (!stack.empty()) {
        Node* n = stack.back();
        stack.pop_back();
        out.push_back(n);
        const std::vector<Node*> kids = children(*n);
        stack.insert(stack.end(), kids.rbegin(), kids.rend());
    }
    return out;
}

std::vector<Node*> unsupported(Node* root) {
    std::vector<Node*> out;
    for (Node* n : walk(root)) {
        if (is<Unsupported>(n)) out.push_back(n);
    }
    return out;
}

std::vector<Node*> statements(const Suite& suite) {
    if (const auto* one = std::get_if<Node*>(&suite)) return *one != nullptr ? std::vector<Node*>{*one} : std::vector<Node*>{};
    return std::get<std::vector<Node*>>(suite);
}

bool present(const Suite& suite) {
    if (const auto* one = std::get_if<Node*>(&suite)) return *one != nullptr;
    return !std::get<std::vector<Node*>>(suite).empty();
}

#define CB_IR_KINDS(X)                                                                                                         \
    X(Module) X(Unit) X(ClassDecl) X(FieldDecl) X(MethodDecl) X(Param) X(EnumConst) X(Property) X(TypeParam) X(Block) X(Local) \
    X(ExprStmt) X(If) X(While) X(DoWhile) X(For) X(ForEach) X(Switch) X(Case) X(Break) X(Continue) X(Return) X(Throw) X(Try)  \
    X(Catch) X(Labeled) X(Goto) X(MachineFragment) X(Sync) X(Empty) X(Comment) X(LocalClass) X(Assert) X(CtorCall) X(Yield)    \
    X(Lit) X(Name) X(Select) X(TypeName) X(This) X(Super) X(Call) X(New) X(NewArray) X(Index) X(Length) X(Unary) X(Binary)    \
    X(Concat) X(Assign) X(Cond) X(Cast) X(InstanceOf) X(ClassLit) X(Lambda) X(MethodRef) X(SwitchExpr) X(AddressOf)           \
    X(FarAddress) X(Piece) X(Join) X(Paren) X(Unsupported)

Node* make_node(Arena& arena, std::string_view name) {
#define CB_IR_MAKE(K) \
    if (name == K::kName) return make(arena, K{});
    CB_IR_KINDS(CB_IR_MAKE)
#undef CB_IR_MAKE
    return nullptr;
}

Node* clone(Arena& arena, const Node* root) {
    if (root == nullptr) return nullptr;
    Node* copy = arena.make<Node>(*root);
    visit(*copy, [&arena](auto& k) { k.each_child([&arena](Node*& child) { child = clone(arena, child); }); });
    if (auto* test = get<InstanceOf>(copy)) test->binding = clone(arena, test->binding);   // (owned, though a walk does not enter it)
    return copy;
}

// ---- the Java front end's operator names ----------------------------------------------------------------------------------

std::optional<std::string_view> binary_operator(std::string_view java_name) {
    struct Entry {
        string_view name;
        string_view op;
    };
    static constexpr Entry kOperators[] = {
        {"PLUS", "+"},   {"MINUS", "-"},   {"MULTIPLY", "*"},  {"DIVIDE", "/"},           {"REMAINDER", "%"},
        {"LEFT_SHIFT", "<<"}, {"RIGHT_SHIFT", ">>"}, {"UNSIGNED_RIGHT_SHIFT", ">>>"}, {"AND", "&"}, {"OR", "|"},
        {"XOR", "^"},    {"CONDITIONAL_AND", "&&"}, {"CONDITIONAL_OR", "||"}, {"EQUAL_TO", "=="}, {"NOT_EQUAL_TO", "!="},
        {"LESS_THAN", "<"}, {"LESS_THAN_EQUAL", "<="}, {"GREATER_THAN", ">"}, {"GREATER_THAN_EQUAL", ">="},
    };
    for (const Entry& e : kOperators) {
        if (e.name == java_name) return e.op;
    }
    return std::nullopt;
}

std::optional<UnaryOperator> unary_operator(std::string_view java_name) {
    struct Entry {
        string_view name;
        UnaryOperator op;
    };
    static constexpr Entry kOperators[] = {
        {"UNARY_MINUS", {"-", false}},          {"UNARY_PLUS", {"+", false}},           {"LOGICAL_COMPLEMENT", {"!", false}},
        {"BITWISE_COMPLEMENT", {"~", false}},   {"PREFIX_INCREMENT", {"++", false}},    {"PREFIX_DECREMENT", {"--", false}},
        {"POSTFIX_INCREMENT", {"++", true}},    {"POSTFIX_DECREMENT", {"--", true}},
    };
    for (const Entry& e : kOperators) {
        if (e.name == java_name) return e.op;
    }
    return std::nullopt;
}

}  // namespace cb::translate::ir

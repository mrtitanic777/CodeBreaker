// codebreaker/translate/ir.cpp -- typed C++ port of codebreaker.translate.ir (see ir.hpp).

#include "codebreaker/translate/ir.hpp"

#include <functional>

namespace codebreaker::translate::ir {

namespace {

py::list<py::str> strs(std::initializer_list<const char*> items) {
    py::list<py::str> out;
    for (const char* s : items) out.append(py::str(s));
    return out;
}

bool contains(const py::list<py::str>& xs, const py::str& x) {
    for (const py::str& y : xs) {
        if (y == x) return true;
    }
    return false;
}

size_t mix(size_t seed, size_t h) {
    return seed ^ (h + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2));
}

size_t hash_of(const std::optional<py::str>& s) {
    return s ? std::hash<std::u32string>()(s->u32()) : size_t(0x5bd1e995);
}

// the text after the last '.' (s.rsplit('.', 1)[-1])
py::str after_dot(const py::str& s) {
    const std::u32string& u = s.u32();
    size_t at = u.rfind(U'.');
    return py::str(at == std::u32string::npos ? u : u.substr(at + 1));
}

}  // namespace

// ---------------------------------------------------------------------------------------------- Val
py::str Val::repr() const {
    return std::visit(
        [](const auto& x) -> py::str {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return py::str("None");
            else if constexpr (std::is_same_v<T, bool>) return py::repr(x);
            else if constexpr (std::is_same_v<T, int64_t>) return py::repr(x);
            else if constexpr (std::is_same_v<T, double>) return py::repr(x);
            else if constexpr (std::is_same_v<T, py::str>) return py::repr(x);
            else return repr_of(x);   // a Type, a list or a dict
        },
        v);
}

// ---------------------------------------------------------------------------------------------- types
const py::list<py::str> PRIMS = strs({"bool", "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64", "f80", "char"});

Type::Type(const py::str& kind_, const std::optional<py::str>& name_, const py::list<TypeP>& args_, const TypeP& elem_,
           const std::optional<std::tuple<py::str, TypeP>>& bound_, const Dict& info_)
    : kind(kind_), name(name_), args(args_.copy()), elem(elem_), bound(bound_), info(info_) {}

py::str Type::info_key() const {
    // repr(sorted(self.info.items())): the keys are distinct str, so the values are never compared
    auto items = info.items();
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    py::str out("[");
    bool first = true;
    for (const auto& kv : items) {
        out = out + py::str(first ? "" : ", ") + py::str("(") + py::repr(std::get<0>(kv)) + py::str(", ") +
              std::get<1>(kv).repr() + py::str(")");
        first = false;
    }
    return out + py::str("]");
}

bool same(const TypeP& a, const TypeP& b) {
    if (!a || !b) return !a && !b;
    return *a == *b;
}

bool Type::operator==(const Type& o) const {
    if (!(kind == o.kind)) return false;
    if (name.has_value() != o.name.has_value() || (name && !(*name == *o.name))) return false;
    if (py::len(args) != py::len(o.args)) return false;
    for (int64_t i = 0; i < py::len(args); ++i) {
        if (!same(args[i], o.args[i])) return false;
    }
    if (!same(elem, o.elem)) return false;
    if (bound.has_value() != o.bound.has_value()) return false;
    if (bound && (!(std::get<0>(*bound) == std::get<0>(*o.bound)) || !same(std::get<1>(*bound), std::get<1>(*o.bound)))) {
        return false;
    }
    return info_key() == o.info_key();
}

size_t Type::hash() const {
    size_t h = std::hash<std::u32string>()(kind.u32());
    h = mix(h, hash_of(name));
    for (const TypeP& a : args) h = mix(h, a ? a->hash() : size_t(1));
    return mix(h, elem ? elem->hash() : size_t(2));
}

py::str Type::repr() const {
    if (kind == py::str("array")) {
        return repr_of(elem) + py::str("[]");
    }
    if (kind == py::str("class") || kind == py::str("typevar")) {
        if (!name) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'str'");
        py::str tail;
        if (py::truthy(args)) {
            tail = py::str("<");
            for (int64_t i = 0; i < py::len(args); ++i) tail = tail + py::str(i ? ", " : "") + repr_of(args[i]);
            tail = tail + py::str(">");
        }
        return *name + tail;
    }
    if (kind == py::str("wildcard")) {
        py::str tail;
        if (bound) tail = py::str(" ") + std::get<0>(*bound) + py::str(" ") + repr_of(std::get<1>(*bound));
        return py::str("?") + tail;
    }
    return kind;
}

bool Type::is_prim() const {
    return contains(PRIMS, kind);
}

bool Type::is_ref() const {
    return kind == py::str("class") || kind == py::str("array") || kind == py::str("typevar") ||
           kind == py::str("null") || kind == py::str("wildcard");
}

py::str Type::simple() const {
    if (name && py::truthy(*name)) return after_dot(*name);
    return kind;
}

std::tuple<int64_t, TypeP> Type::dims() const {
    int64_t n = 0;
    TypeP t = std::static_pointer_cast<Type>(const_cast<Type*>(this)->shared_from_this());
    while (true) {
        if (!t) throw py::AttributeError("'NoneType' object has no attribute 'kind'");
        if (!(t->kind == py::str("array"))) break;
        n += 1;
        t = t->elem;
    }
    return std::make_tuple(n, t);
}

const py::list<py::str> SPACES = strs({"flat", "near", "far", "huge", "real", "io"});
const py::list<py::str> MEMORY = strs({"gc", "owned", "borrowed", "stack", "manual", "emulated"});
const py::list<py::str> OVERFLOW_ = strs({"wrap", "saturate", "trap"});

TypeP pointer_to(const TypeP& t, const py::str& space, const Val& seg) {
    Dict info;
    info.set(py::str("space"), Val(space));
    if (!seg.is_none()) info.set(py::str("seg"), seg);
    return std::make_shared<Type>(py::str("ptr"), std::nullopt, py::list<TypeP>(), t, std::nullopt, info);
}

TypeP array_of(TypeP t, int64_t dims) {
    for (int64_t i = 0; i < dims; ++i) {
        t = std::make_shared<Type>(py::str("array"), std::nullopt, py::list<TypeP>(), t);
    }
    return t;
}

const TypeP VOID = std::make_shared<Type>(py::str("void"));
const TypeP NULL_ = std::make_shared<Type>(py::str("null"));
const TypeP ANY = std::make_shared<Type>(py::str("any"));
const TypeP BOOL = std::make_shared<Type>(py::str("bool"));
const TypeP I32 = std::make_shared<Type>(py::str("i32"));

// ---------------------------------------------------------------------------------------------- symbols
Sym::Sym(const py::str& kind_, const py::str& name_, const std::optional<py::str>& owner_, bool static__, const TypeP& type_,
         const py::list<TypeP>& params_, const TypeP& ret_, bool varargs_, const py::list<py::str>& refs_,
         const py::list<py::str>& flags_)
    : kind(kind_), name(name_), owner(owner_), static_(static__), type(type_), params(params_.copy()), ret(ret_),
      varargs(varargs_), refs(refs_.copy()), flags(flags_.copy()) {}

py::str Sym::repr() const {
    return py::str("<") + kind + py::str(" ") + (owner ? *owner : py::str("None")) + py::str(".") + name + py::str(">");
}

std::optional<py::str> Sym::owner_simple() const {
    if (owner && py::truthy(*owner)) return after_dot(*owner);
    return std::nullopt;
}

// ---------------------------------------------------------------------------------------------- nodes
void unknown_fields(const py::str& class_name, const py::list<py::str>& names) {
    throw py::TypeError(class_name + py::str(": unknown fields ") + repr_of(py::sorted(names)));
}

namespace {

template <class T> NodeP make() {
    return std::make_shared<T>();
}

}  // namespace

NodeP new_node(const py::str& class_name) {
    static const std::vector<std::pair<const char*, NodeP (*)()>> table = {
        {"Module", make<Module>}, {"Unit", make<Unit>}, {"ClassDecl", make<ClassDecl>}, {"FieldDecl", make<FieldDecl>},
        {"MethodDecl", make<MethodDecl>}, {"Param", make<Param>}, {"EnumConst", make<EnumConst>},
        {"Property", make<Property>}, {"TypeParam", make<TypeParam>},
        {"Block", make<Block>}, {"Local", make<Local>}, {"ExprStmt", make<ExprStmt>}, {"If", make<If>},
        {"While", make<While>}, {"DoWhile", make<DoWhile>}, {"For", make<For>}, {"ForEach", make<ForEach>},
        {"Switch", make<Switch>}, {"Case", make<Case>}, {"Break", make<Break>}, {"Continue", make<Continue>},
        {"Return", make<Return>}, {"Throw", make<Throw>}, {"Try", make<Try>}, {"Catch", make<Catch>},
        {"Labeled", make<Labeled>}, {"Goto", make<Goto>}, {"MachineFragment", make<MachineFragment>},
        {"Sync", make<Sync>}, {"Empty", make<Empty>}, {"Comment", make<Comment>}, {"LocalClass", make<LocalClass>},
        {"Assert", make<Assert>}, {"CtorCall", make<CtorCall>}, {"Yield", make<Yield>},
        {"Lit", make<Lit>}, {"Name", make<Name>}, {"Select", make<Select>}, {"TypeName", make<TypeName>},
        {"This", make<This>}, {"Super", make<Super>}, {"Call", make<Call>}, {"New", make<New>},
        {"NewArray", make<NewArray>}, {"Index", make<Index>}, {"Length", make<Length>}, {"Unary", make<Unary>},
        {"Binary", make<Binary>}, {"Concat", make<Concat>}, {"Assign", make<Assign>}, {"Cond", make<Cond>},
        {"Cast", make<Cast>}, {"InstanceOf", make<InstanceOf>}, {"ClassLit", make<ClassLit>},
        {"Lambda", make<Lambda>}, {"MethodRef", make<MethodRef>}, {"SwitchExpr", make<SwitchExpr>},
        {"AddressOf", make<AddressOf>}, {"FarAddress", make<FarAddress>}, {"Piece", make<Piece>},
        {"Join", make<Join>}, {"Paren", make<Paren>}, {"Unsupported", make<Unsupported>},
    };
    for (const auto& e : table) {
        if (class_name == py::str(e.first)) return e.second();
    }
    throw py::AttributeError(py::str("module 'codebreaker.translate.ir' has no attribute ") + py::repr(class_name));
}

const py::dict<py::str, py::str> OPERATORS = [] {
    py::dict<py::str, py::str> d;
    const char* rows[][2] = {{"PLUS", "+"}, {"MINUS", "-"}, {"MULTIPLY", "*"}, {"DIVIDE", "/"}, {"REMAINDER", "%"},
                             {"LEFT_SHIFT", "<<"}, {"RIGHT_SHIFT", ">>"}, {"UNSIGNED_RIGHT_SHIFT", ">>>"},
                             {"AND", "&"}, {"OR", "|"}, {"XOR", "^"}, {"CONDITIONAL_AND", "&&"}, {"CONDITIONAL_OR", "||"},
                             {"EQUAL_TO", "=="}, {"NOT_EQUAL_TO", "!="}, {"LESS_THAN", "<"}, {"LESS_THAN_EQUAL", "<="},
                             {"GREATER_THAN", ">"}, {"GREATER_THAN_EQUAL", ">="}};
    for (const auto& r : rows) d.set(py::str(r[0]), py::str(r[1]));
    return d;
}();

const py::dict<py::str, std::tuple<py::str, bool>> UNARY = [] {
    py::dict<py::str, std::tuple<py::str, bool>> d;
    d.set(py::str("UNARY_MINUS"), std::make_tuple(py::str("-"), false));
    d.set(py::str("UNARY_PLUS"), std::make_tuple(py::str("+"), false));
    d.set(py::str("LOGICAL_COMPLEMENT"), std::make_tuple(py::str("!"), false));
    d.set(py::str("BITWISE_COMPLEMENT"), std::make_tuple(py::str("~"), false));
    d.set(py::str("PREFIX_INCREMENT"), std::make_tuple(py::str("++"), false));
    d.set(py::str("PREFIX_DECREMENT"), std::make_tuple(py::str("--"), false));
    d.set(py::str("POSTFIX_INCREMENT"), std::make_tuple(py::str("++"), true));
    d.set(py::str("POSTFIX_DECREMENT"), std::make_tuple(py::str("--"), true));
    return d;
}();

py::list<NodeP> walk(const NodeP& node) {
    py::list<NodeP> out;
    std::vector<NodeP> stack{node};
    while (!stack.empty()) {
        NodeP n = stack.back();
        stack.pop_back();
        out.append(n);
        if (!n) throw py::AttributeError("'NoneType' object has no attribute 'children'");
        py::list<NodeP> kids = n->children();
        for (int64_t i = py::len(kids); i-- > 0;) stack.push_back(kids[i]);
    }
    return out;
}

py::list<py::ref<Unsupported>> unsupported(const NodeP& node) {
    py::list<py::ref<Unsupported>> out;
    for (const NodeP& n : walk(node)) {
        if (auto u = std::dynamic_pointer_cast<Unsupported>(n)) out.append(u);
    }
    return out;
}

}  // namespace codebreaker::translate::ir



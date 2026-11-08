// codebreaker/translate/frontends/csharp/lower.cpp -- typed C++ port of
// codebreaker.translate.frontends.csharp.lower (see lower.hpp).

#include "codebreaker/translate/frontends/csharp/lower.hpp"

#include <functional>

#include "json.hpp"

namespace codebreaker::translate::frontends::csharp::lower {

namespace {

py::str K(const char* s) { return py::str(s); }

// ---------------------------------------------------------------------------------------------- tables
// (_PRIM) C#'s primitives in the IR's language-neutral kinds
const py::dict<py::str, py::str> PRIM_ = {{K("bool"), K("bool")}, {K("sbyte"), K("i8")},  {K("byte"), K("u8")},
                                          {K("short"), K("i16")}, {K("ushort"), K("u16")}, {K("int"), K("i32")},
                                          {K("uint"), K("u32")},  {K("long"), K("i64")},   {K("ulong"), K("u64")},
                                          {K("float"), K("f32")}, {K("double"), K("f64")}};
const py::dict<py::str, py::str> BINARY_ = {
    {K("Add"), K("+")},          {K("Subtract"), K("-")},          {K("Multiply"), K("*")},
    {K("Divide"), K("/")},       {K("Remainder"), K("%")},         {K("LeftShift"), K("<<")},
    {K("RightShift"), K(">>")},  {K("UnsignedRightShift"), K(">>>")}, {K("And"), K("&")},
    {K("Or"), K("|")},           {K("ExclusiveOr"), K("^")},       {K("ConditionalAnd"), K("&&")},
    {K("ConditionalOr"), K("||")}, {K("Equals"), K("==")},         {K("NotEquals"), K("!=")},
    {K("LessThan"), K("<")},     {K("LessThanOrEqual"), K("<=")},  {K("GreaterThan"), K(">")},
    {K("GreaterThanOrEqual"), K(">=")}, {K("ObjectValueEquals"), K("==")}, {K("ObjectValueNotEquals"), K("!=")}};
const py::dict<py::str, py::str> UNARY_ = {{K("Minus"), K("-")}, {K("Plus"), K("+")}, {K("Not"), K("!")},
                                           {K("BitwiseNegation"), K("~")}};
const py::dict<py::str, py::str> REF_ = {{K("None"), K("")},   {K("Ref"), K("ref")},
                                         {K("Out"), K("out")}, {K("In"), K("in")},
                                         {K("RefReadOnly"), K("in")}, {K("RefReadOnlyParameter"), K("in")}};
const py::dict<py::str, int64_t> PREC_ = {
    {K("*"), 14}, {K("/"), 14}, {K("%"), 14}, {K("+"), 13}, {K("-"), 13}, {K("<<"), 12}, {K(">>"), 12}, {K(">>>"), 12},
    {K("<"), 11}, {K(">"), 11}, {K("<="), 11}, {K(">="), 11}, {K("=="), 10}, {K("!="), 10}, {K("&"), 9}, {K("^"), 8},
    {K("|"), 7},  {K("&&"), 6}, {K("||"), 5},  {K("??"), 4}};

// ---------------------------------------------------------------------------------------------- JSON values
template <class T> bool holds(const Val& v) { return std::holds_alternative<T>(v.v); }
template <class T> const T& as(const Val& v) { return std::get<T>(v.v); }

// Python truthiness of a JSON value
bool is_truthy(const Val& v) {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            else if constexpr (std::is_same_v<T, bool>) return x;
            else if constexpr (std::is_same_v<T, int64_t>) return x != 0;
            else if constexpr (std::is_same_v<T, double>) return x != 0.0;
            else if constexpr (std::is_same_v<T, py::str>) return py::len(x) != 0;
            else if constexpr (std::is_same_v<T, ir::TypeP>) return true;
            else return !x.empty();   // a list or a dict
        },
        v.v);
}

// Python's == of two JSON values (numbers by value, the rest by their text)
bool same_val(const Val& a, const Val& b) {
    if (a.is_none() || b.is_none()) return a.is_none() && b.is_none();
    auto num = [](const Val& v, double& out) {
        if (holds<int64_t>(v)) { out = double(as<int64_t>(v)); return true; }
        if (holds<bool>(v)) { out = as<bool>(v) ? 1.0 : 0.0; return true; }
        if (holds<double>(v)) { out = as<double>(v); return true; }
        return false;
    };
    double x = 0, y = 0;
    if (num(a, x) && num(b, y)) {
        if (holds<int64_t>(a) && holds<int64_t>(b)) return as<int64_t>(a) == as<int64_t>(b);
        return x == y;
    }
    if (holds<py::str>(a) && holds<py::str>(b)) return as<py::str>(a) == as<py::str>(b);
    return a.repr() == b.repr();
}

[[noreturn]] void type_error(const char* what, const Val& v) { throw py::TypeError(py::str(what) + v.repr()); }

bool has(const Dict& d, const char* key) { return d.contains(K(key)); }
Val G(const Dict& d, const char* key) { return d.get(K(key), Val()); }     // d.get(key)
Val G(const Dict& d, const char* key, const Val& dflt) { return d.get(K(key), dflt); }   // d.get(key, dflt)
Val A(const Dict& d, const char* key) { return d.at(K(key)); }             // d[key]: KeyError

// a node that is subscripted (`x['k']`): a TypeError for None or anything that is not a dict
Dict dd(const Val& v) {
    if (!holds<Dict>(v)) type_error("not a JSON object: ", v);
    return as<Dict>(v);
}
// a node whose first use is `.get(...)`: an AttributeError for None or anything that is not a dict
Dict dg(const Val& v) {
    if (!holds<Dict>(v)) throw py::AttributeError("object has no attribute 'get': " + v.repr().utf8());
    return as<Dict>(v);
}
// `(v or {})` read with `.get`
Dict or_empty(const Val& v) { return is_truthy(v) ? dg(v) : Dict(); }

// the items of a JSON list that is iterated
py::list<Val> items_of(const Val& v) {
    if (!holds<Val::List>(v)) type_error("not iterable: ", v);
    return as<Val::List>(v);
}
// d.get(key, []) iterated
py::list<Val> L(const Dict& d, const char* key) {
    if (!has(d, key)) return py::list<Val>();
    return items_of(A(d, key));
}
// list(d.get(key, [])) of strings
py::list<py::str> strs_of(const Val& v) {
    py::list<py::str> out;
    for (const Val& x : items_of(v)) {
        if (!holds<py::str>(x)) type_error("not a str: ", x);
        out.append(as<py::str>(x));
    }
    return out;
}

std::optional<py::str> opt_str_val(const Val& v) {
    if (v.is_none()) return std::nullopt;
    if (!holds<py::str>(v)) type_error("not a str: ", v);
    return as<py::str>(v);
}
py::str str_val(const Val& v) {
    if (!holds<py::str>(v)) type_error("not a str: ", v);
    return as<py::str>(v);
}
// v == 'text'
bool veq(const Val& v, const char* s) { return holds<py::str>(v) && as<py::str>(v) == K(s); }
// k == 'text' for a kind that may be None
bool is_(const std::optional<py::str>& k, const char* s) { return k && *k == K(s); }
bool in_(const std::optional<py::str>& k, std::initializer_list<const char*> xs) {
    for (const char* s : xs) {
        if (is_(k, s)) return true;
    }
    return false;
}
bool vin(const Val& v, std::initializer_list<const char*> xs) {
    for (const char* s : xs) {
        if (veq(v, s)) return true;
    }
    return false;
}
// n['k']: a node's kind (a str, or None for a null)
std::optional<py::str> kind_of(const Dict& n) { return opt_str_val(A(n, "k")); }
// '%s' % v
py::str pstr(const Val& v) {
    if (v.is_none()) return K("None");
    if (holds<py::str>(v)) return as<py::str>(v);
    return v.repr();
}
py::str pstr(const std::optional<py::str>& v) { return v ? *v : K("None"); }

// n.get('l', 0) / n.get('c', 0)
int64_t number_of(const Val& v) {
    if (v.is_none()) return 0;
    if (holds<int64_t>(v)) return as<int64_t>(v);
    if (holds<bool>(v)) return as<bool>(v) ? 1 : 0;
    type_error("not a number: ", v);
}

// int(v)
int64_t int_of(const Val& v) {
    if (holds<int64_t>(v)) return as<int64_t>(v);
    if (holds<bool>(v)) return as<bool>(v) ? 1 : 0;
    if (holds<double>(v)) return py::int_from_float(as<double>(v));
    if (holds<py::str>(v)) return py::int_parse(as<py::str>(v));
    throw py::TypeError("int() argument must be a string, a bytes-like object or a real number");
}
// float(v)
double float_of(const Val& v) {
    if (holds<double>(v)) return as<double>(v);
    if (holds<int64_t>(v)) return double(as<int64_t>(v));
    if (holds<bool>(v)) return as<bool>(v) ? 1.0 : 0.0;
    if (holds<py::str>(v)) return py::float_from_str(as<py::str>(v));
    throw py::TypeError("float() argument must be a string or a real number");
}
// _REF.get(v, '')
py::str ref_of(const Val& v) {
    if (holds<py::str>(v) && REF_.contains(as<py::str>(v))) return REF_.at(as<py::str>(v));
    return K("");
}

// A node of class T at a place
template <class T> py::ref<T> make(const Pos& p) {
    auto n = std::make_shared<T>();
    n->line = p.line;
    n->col = p.col;
    return n;
}
template <class T> py::ref<T> make_expr(const Pos& p, const TypeP& t) {
    auto n = make<T>(p);
    n->type = t;
    return n;
}
template <class T> py::ref<T> as_node(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }

// `.type` of an expression node (AttributeError for None or a node with no type)
TypeP type_of(const NodeP& n) {
    if (!n) throw py::AttributeError("'NoneType' object has no attribute 'type'");
    auto e = std::dynamic_pointer_cast<ir::Expr>(n);
    if (!e) throw py::AttributeError("object has no attribute 'type'");
    return e->type;
}
// a symbol that is dereferenced (AttributeError for None)
const SymP& need(const SymP& s) {
    if (!s) throw py::AttributeError("'NoneType' object has no attribute 'kind'");
    return s;
}
const TypeP& need_type(const TypeP& t) {
    if (!t) throw py::AttributeError("'NoneType' object has no attribute 'kind'");
    return t;
}

TypeP mk_type(const char* kind, const std::optional<py::str>& name = std::nullopt) {
    return std::make_shared<ir::Type>(K(kind), name);
}
TypeP class_type(const std::optional<py::str>& name) { return mk_type("class", name); }

py::list<py::str> strs(std::initializer_list<const char*> xs) {
    py::list<py::str> out;
    for (const char* s : xs) out.append(K(s));
    return out;
}

// ir.Sym(...) with every slot
SymP mk_sym(const char* kind, const py::str& name, const std::optional<py::str>& owner = std::nullopt, bool static_ = false,
            const TypeP& type = nullptr, const py::list<TypeP>& params = py::list<TypeP>(), const TypeP& ret = nullptr,
            bool varargs = false, const py::list<py::str>& refs = py::list<py::str>(),
            const py::list<py::str>& flags = py::list<py::str>()) {
    return std::make_shared<ir::Sym>(K(kind), name, owner, static_, type, params, ret, varargs, refs, flags);
}

// an empty-bodied MethodDecl as the accessors and defaults are made
py::ref<ir::MethodDecl> mk_method(const SymP& s, const std::optional<py::str>& name, const char* kind,
                                  const py::list<NodeP>& params, const TypeP& ret, const NodeP& body,
                                  const py::list<py::str>& mods, const Pos& p) {
    auto d = make<ir::MethodDecl>(p);
    d->sym = s;
    d->name = name;
    d->kind = K(kind);
    d->params = params;
    d->ret = ret;
    d->body = body;
    d->mods = mods;
    d->throws = py::list<TypeP>();
    d->typeparams = py::list<py::str>();
    d->tparams = py::list<NodeP>();
    return d;
}

py::ref<ir::Lit> mk_lit(const Val& value, const TypeP& type, const Pos& p) {
    auto l = make_expr<ir::Lit>(p, type);
    l->value = value;
    return l;
}
py::ref<ir::Unary> mk_unary(const char* op, const NodeP& x, const TypeP& type, const Pos& p) {
    auto u = make_expr<ir::Unary>(p, type);
    u->op = K(op);
    u->x = x;
    u->postfix = false;
    return u;
}
py::ref<ir::Binary> mk_binary(const std::optional<py::str>& op, const NodeP& left, const NodeP& right, const TypeP& type,
                              const Pos& p) {
    auto b = make_expr<ir::Binary>(p, type);
    b->op = op;
    b->left = left;
    b->right = right;
    return b;
}
py::ref<ir::Cast> mk_cast(const NodeP& x, const TypeP& type, const Pos& p) {
    auto c = make_expr<ir::Cast>(p, type);
    c->x = x;
    return c;
}
py::ref<ir::InstanceOf> mk_instanceof(const NodeP& x, const TypeP& test, const NodeP& binding, const Pos& p) {
    auto i = make_expr<ir::InstanceOf>(p, ir::BOOL);
    i->x = x;
    i->test = test;
    i->binding = binding;
    return i;
}
py::ref<ir::Local> mk_local(const SymP& s, const TypeP& type, const NodeP& init, const py::list<py::str>& mods, const Pos& p) {
    auto l = make<ir::Local>(p);
    l->sym = s;
    l->type = type;
    l->init = init;
    l->mods = mods;
    return l;
}
py::ref<ir::Block> mk_block(const py::list<NodeP>& stmts, const Pos& p) {
    auto b = make<ir::Block>(p);
    b->stmts = stmts;
    return b;
}
py::ref<ir::ExprStmt> mk_exprstmt(const NodeP& e, const Pos& p) {
    auto s = make<ir::ExprStmt>(p);
    s->expr = e;
    return s;
}

// a Sym with the same slots and other flags / owner (the Python rebuilds a Sym by position)
SymP copy_sym(const SymP& s, bool static_, const py::list<py::str>& flags) {
    return std::make_shared<ir::Sym>(s->kind, s->name, s->owner, static_, s->type, s->params.copy(), s->ret, s->varargs,
                                     s->refs.copy(), flags);
}

// `with` / try-finally: runs f when the scope is left, by return or by exception
template <class F> struct Finally {
    F f;
    ~Finally() { f(); }
};
template <class F> Finally<F> finally_(F f) { return Finally<F>{f}; }

}  // namespace

const py::str LANG = K("csharp");
const TypeP STRING = std::make_shared<ir::Type>(K("class"), std::optional<py::str>(K("System.String")));
const TypeP OBJECT = std::make_shared<ir::Type>(K("class"), std::optional<py::str>(K("System.Object")));

// ---------------------------------------------------------------------------------------------- the JSON
namespace {

Val from_v(const py::V& v) {
    switch (v.k) {
        case py::K::None: return Val();
        case py::K::Bool: return Val(v.b);
        case py::K::Int: return Val(v.i);
        case py::K::Float: return Val(v.f);
        case py::K::Str: return Val(py::bytes(v.str()).decode("utf-8"));
        case py::K::List:
        case py::K::Tuple: {
            Val::List out;
            for (const py::V& x : v.seq()) out.append(from_v(x));
            return Val(out);
        }
        case py::K::Dict: {
            Dict out;
            for (const auto& kv : v.items()) out.set(py::bytes(kv.first.str()).decode("utf-8"), from_v(kv.second));
            return Val(out);
        }
        default: break;
    }
    throw py::TypeError("a set is not JSON");
}

}  // namespace

Dict load_tree(const py::str& text) {
    Val v = from_v(py::json_parse(text.utf8()));
    return dd(v);
}

// -------------------------------------------------------------------------------------- types
TypeP ty(const Val& dv) {
    if (dv.is_none()) return ir::ANY;
    Dict d = dd(dv);
    Val k = A(d, "k");
    if (veq(k, "prim")) {
        Val nv = A(d, "n");
        if (!holds<py::str>(nv)) {   // (not a name: it is none of the above, nor in _PRIM)
            if (holds<Val::List>(nv) || holds<Dict>(nv)) throw py::TypeError("unhashable type");
            return class_type(K("System.UIntPtr"));
        }
        py::str n = as<py::str>(nv);
        if (n == K("void")) return ir::VOID;
        if (n == K("char")) {
            auto t = mk_type("u16");
            t->info.set(K("enc"), Val(K("utf16")));
            return t;
        }
        if (PRIM_.contains(n)) return mk_type(PRIM_.at(n).utf8().c_str());
        return class_type(n == K("nint") ? K("System.IntPtr") : K("System.UIntPtr"));
    }
    if (veq(k, "array")) {
        TypeP elem = ty(A(d, "elem"));
        auto t = std::make_shared<ir::Type>(K("array"), std::nullopt, py::list<TypeP>(), elem);
        Val rank = has(d, "rank") ? A(d, "rank") : Val(1);
        if (!same_val(rank, Val(1))) t->info.set(K("rank"), A(d, "rank"));
        return t;
    }
    if (veq(k, "tv")) return mk_type("typevar", opt_str_val(A(d, "n")));
    if (veq(k, "named")) {
        std::optional<py::str> name = opt_str_val(A(d, "n"));
        py::list<TypeP> args;
        for (const Val& a : L(d, "args")) args.append(ty(a));
        auto t = std::make_shared<ir::Type>(K("class"), name, args);
        if (is_truthy(G(d, "nullable"))) t->info.set(K("nullable"), Val(true));
        return t;
    }
    if (veq(k, "ptr")) return ir::pointer_to(ty(A(d, "elem")));
    if (veq(k, "dynamic")) return mk_type("dynamic");
    return ir::ANY;
}

bool is_string(const TypeP& t) { return t && t->kind == K("class") && t->name == std::optional<py::str>(K("System.String")); }

Val tkind(const Val& d) {
    if (!is_truthy(d)) return Val();
    return G(dg(d), "tk");
}

// -------------------------------------------------------------------------------------- symbols
SymP sym(const Val& dv) {
    if (!is_truthy(dv)) return nullptr;
    Dict d = dg(dv);
    Val kindv = A(d, "kind");
    if (kindv.is_none()) throw py::AttributeError("'NoneType' object has no attribute 'lower'");   // (k.lower() ends the chain of tests)
    py::str k = str_val(kindv);
    py::list<py::str> flags;
    static const std::pair<const char*, const char*> FLAGS[] = {
        {"ext", "extension"}, {"indexer", "indexer"}, {"readonly", "readonly"}, {"initonly", "initonly"},
        {"virtual", "virtual"}, {"abstract", "abstract"}, {"override", "override"}, {"sealed", "sealed"},
        {"implicit", "implicit"}, {"const", "const"}};
    for (const auto& kf : FLAGS) {
        if (is_truthy(G(d, kf.first))) flags.append(K(kf.second));
    }
    Val ownerv = G(d, "owner");
    if (k == K("Method")) {
        Val mk = G(d, "mk");
        py::str kind = veq(mk, "Constructor") ? K("ctor") : K("method");
        if (vin(mk, {"UserDefinedOperator", "Conversion", "BuiltinOperator"})) flags.append(K("operator"));
        if (veq(mk, "LocalFunction")) flags.append(K("local"));
        if (veq(mk, "StaticConstructor")) flags.append(K("static_init"));
        if (veq(G(d, "name"), "Invoke") && veq(tkind(G(d, "ownerType")), "Delegate")) flags.append(K("delegate"));
        py::list<py::str> refs;
        bool any_ref = false;
        for (const Val& r : L(d, "refs")) {
            py::str x = ref_of(r);
            refs.append(x);
            if (py::len(x) != 0) any_ref = true;
        }
        py::str name = str_val(A(d, "name"));
        bool is_static = is_truthy(G(d, "static", Val(false)));
        py::list<TypeP> params;
        for (const Val& p : L(d, "params")) params.append(ty(p));
        TypeP ret = ty(G(d, "ret"));
        bool varargs = is_truthy(G(d, "varargs", Val(false)));
        return mk_sym(kind.utf8().c_str(), name, opt_str_val(ownerv), is_static, nullptr, params, ret, varargs,
                      any_ref ? refs : py::list<py::str>(), flags);
    }
    if (k == K("Field")) {
        py::str kind = (veq(tkind(G(d, "ownerType")), "Enum") && is_truthy(G(d, "const"))) ? K("enumconst") : K("field");
        py::str name = str_val(A(d, "name"));
        bool is_static = is_truthy(G(d, "static", Val(false))) || is_truthy(G(d, "const", Val(false)));
        return mk_sym(kind.utf8().c_str(), name, opt_str_val(ownerv), is_static, ty(G(d, "type")), py::list<TypeP>(),
                      nullptr, false, py::list<py::str>(), flags);
    }
    if (k == K("Property")) {
        py::str name = str_val(A(d, "name"));
        bool is_static = is_truthy(G(d, "static", Val(false)));
        TypeP type = ty(G(d, "type"));
        py::list<TypeP> params;
        for (const Val& p : L(d, "params")) params.append(ty(p));
        return mk_sym("property", name, opt_str_val(ownerv), is_static, type, params, nullptr, false,
                      py::list<py::str>(), flags);
    }
    if (k == K("Event")) {
        py::str name = str_val(A(d, "name"));
        bool is_static = is_truthy(G(d, "static", Val(false)));
        return mk_sym("event", name, opt_str_val(ownerv), is_static, ty(G(d, "type")), py::list<TypeP>(), nullptr, false,
                      py::list<py::str>(), flags);
    }
    if (k == K("Local") || k == K("RangeVariable") || k == K("Discard")) {
        py::str name = str_val(A(d, "name"));
        TypeP type = ty(G(d, "type"));
        if (is_truthy(G(d, "ref"))) flags.append(K("ref"));
        return mk_sym("local", name, opt_str_val(ownerv), false, type, py::list<TypeP>(), nullptr, false,
                      py::list<py::str>(), flags);
    }
    if (k == K("Parameter")) {
        py::str r = ref_of(G(d, "ref", Val("None")));
        py::str name = str_val(A(d, "name"));
        TypeP type = ty(G(d, "type"));
        py::list<py::str> refs;
        if (py::len(r) != 0) refs.append(r);
        return mk_sym("param", name, opt_str_val(ownerv), false, type, py::list<TypeP>(), nullptr, false, refs, flags);
    }
    if (k == K("NamedType")) {
        Val q = G(d, "qname");
        py::str name = is_truthy(q) ? str_val(q) : str_val(A(d, "name"));
        return mk_sym("class", name, opt_str_val(ownerv), false, ty(G(d, "type")), py::list<TypeP>(), nullptr, false,
                      py::list<py::str>(), flags);
    }
    if (k == K("TypeParameter")) {
        py::str name = str_val(A(d, "name"));
        return mk_sym("typevar", name, opt_str_val(ownerv), false, ty(G(d, "type")));
    }
    if (k == K("Namespace")) return mk_sym("package", str_val(A(d, "name")), opt_str_val(ownerv));
    if (k == K("Label")) return mk_sym("label", str_val(A(d, "name")));
    py::str name = str_val(A(d, "name"));
    return mk_sym(k.lower().utf8().c_str(), name, opt_str_val(ownerv), false, nullptr, py::list<TypeP>(), nullptr, false,
                  py::list<py::str>(), flags);
}

Val value_(const Val& cv) {
    if (cv.is_none()) return Val();
    Dict c = dd(cv);
    Val tt = A(c, "t");
    Val v = G(c, "v");
    if (veq(tt, "null")) return Val();
    if (veq(tt, "bool")) return Val(is_truthy(v));
    if (veq(tt, "string")) return v;
    if (veq(tt, "char")) {
        if (!holds<int64_t>(v) && !holds<bool>(v)) throw py::TypeError("an integer is required");
        return Val(py::chr(int_of(v)));
    }
    if (veq(tt, "float") || veq(tt, "double")) {
        // {'Infinity': 'inf', '-Infinity': '-inf', U+221E: 'inf', -U+221E: '-inf'}.get(v, v)
        Val w = v;
        if (holds<py::str>(v)) {
            const py::str& s = as<py::str>(v);
            if (s == K("Infinity") || s == K("\u221e")) w = Val(K("inf"));
            else if (s == K("-Infinity") || s == K("-\u221e")) w = Val(K("-inf"));
        }
        return Val(float_of(w));
    }
    if (veq(tt, "decimal")) throw Refuse{K("decimal"), K("a decimal literal: no IR number carries base-10 decimals")};
    return Val(int_of(v));
}

// -------------------------------------------------------------------------------------- the lowering
Pos Lowering::pos(const Dict& n) const { return Pos{number_of(G(n, "l", Val(0))), number_of(G(n, "c", Val(0)))}; }

SymP Lowering::renamed(const SymP& s) const {
    if (s && s->kind == K("local") && rename.contains(s->name)) {
        return std::make_shared<ir::Sym>(s->kind, rename.at(s->name), s->owner, s->static_, s->type, s->params.copy(), s->ret,
                                         s->varargs, s->refs.copy(), s->flags.copy());
    }
    return s;
}

py::ref<ir::Unsupported> Lowering::unsupported(const Dict& n, const std::optional<py::str>& kind, const py::str& why,
                                               const TypeP& t) {
    bool truthy_n = !n.empty();
    refused.append(std::make_tuple(truthy_n ? number_of(G(n, "l", Val(0))) : int64_t(0), kind, why));
    Pos p = truthy_n ? pos(n) : Pos{0, 0};
    TypeP type = t ? t : (truthy_n ? ty(G(n, "$type")) : ir::ANY);
    auto u = make_expr<ir::Unsupported>(p, type);
    u->kind = kind;
    u->raw = why;
    return u;
}

// ---- declarations
py::ref<ir::Unit> Lowering::unit(const Dict& n) {
    py::list<NodeP> types;
    for (const Val& t : L(n, "types")) types.append(decl(dd(t), std::nullopt));
    py::str package;
    if (!types.empty()) {
        auto first = as_node<ir::ClassDecl>(types[0]);
        if (first->qname && py::len(*first->qname) != 0 && first->qname->contains(K("."))) {
            package = first->qname->slice(int64_t(0), first->qname->rfind(K(".")));
        }
    }
    if (!G(n, "main").is_none()) types.append(top_level(n));
    py::list<py::str> imports;
    for (const Val& uv : L(n, "usings")) {
        Dict u = dd(uv);
        Val name = A(u, "name");
        if (is_truthy(name) && !is_truthy(G(u, "alias"))) imports.append(str_val(name));
    }
    auto u = make<ir::Unit>(pos(n));
    u->path = path;
    u->package = package;
    u->imports = imports;
    u->types = types;
    u->lang = LANG;
    return u;
}

py::ref<ir::ClassDecl> Lowering::top_level(const Dict& n) {
    Dict ms = or_empty(G(n, "mainSym"));
    Val ownerv = G(ms, "owner");
    py::str owner = is_truthy(ownerv) ? str_val(ownerv) : K("Program");
    auto c = make<ir::ClassDecl>(Pos{});
    c->name = owner.slice(owner.rfind(K(".")) + 1, owner.size());   // owner.rsplit('.', 1)[-1]
    c->qname = owner;
    c->kind = K("class");
    c->mods = strs({"static", "partial"});
    c->typeparams = py::list<py::str>();
    c->extends = nullptr;
    c->implements = py::list<TypeP>();
    c->members = py::list<NodeP>();
    c->outer = std::nullopt;
    c->tparams = py::list<NodeP>();
    cls = c;
    py::ref<ir::Block> body = method_body(A(n, "main"));
    TypeP ret = !ms.empty() ? ty(G(ms, "ret")) : ir::VOID;
    SymP s = mk_sym("method", K("Main"), std::optional<py::str>(owner), true, nullptr, py::list<TypeP>(), ret);
    py::list<NodeP> params;
    py::list<Val> pnames = L(ms, "pnames");
    py::list<Val> ptypes = L(ms, "params");
    for (int64_t i = 0; i < std::min(pnames.size(), ptypes.size()); ++i) {
        auto prm = make<ir::Param>(Pos{});
        prm->sym = mk_sym("param", str_val(pnames[i]), std::optional<py::str>(owner + K(".Main")), false, ty(ptypes[i]));
        prm->type = ty(ptypes[i]);
        prm->mods = py::list<py::str>();
        params.append(prm);
    }
    c->members->append(mk_method(s, K("Main"), "method", params, s->ret, body, strs({"static"}), Pos{}));
    return c;
}

py::list<NodeP> Lowering::tparams(const Val& tps) {
    py::list<NodeP> out;
    if (!is_truthy(tps)) return out;
    for (const Val& tpv : items_of(tps)) {
        Dict tp = dg(tpv);
        py::list<py::str> flags;
        for (const char* f : {"class", "struct", "new"}) {
            if (is_truthy(G(tp, f))) flags.append(K(f));
        }
        auto t = make<ir::TypeParam>(Pos{});
        t->name = opt_str_val(A(tp, "name"));
        py::list<TypeP> bounds;
        for (const Val& b : L(tp, "bounds")) bounds.append(ty(b));
        t->bounds = bounds;
        t->flags = flags;
        Val variance = G(tp, "variance");
        t->variance = (variance.is_none() || veq(variance, "None")) ? std::optional<py::str>() : opt_str_val(A(tp, "variance"));
        out.append(t);
    }
    return out;
}

py::ref<ir::ClassDecl> Lowering::decl(const Dict& n, const std::optional<py::str>& outer) {
    Dict s = or_empty(G(n, "sym"));
    Val kindv = A(n, "kind");
    py::str kind = str_val(kindv);
    Val qv = G(s, "qname");
    std::optional<py::str> qname = is_truthy(qv) ? opt_str_val(qv) : opt_str_val(A(n, "name"));
    Val base = G(n, "base");
    TypeP extends;
    if (is_truthy(base) && !vin(G(dg(base), "n"), {"System.Object", "System.ValueType", "System.Enum"})) extends = ty(base);
    Val underlying = G(n, "underlying");
    if (kind == K("enum") && is_truthy(underlying) && !veq(G(dg(underlying), "n"), "int")) extends = ty(underlying);
    auto c = make<ir::ClassDecl>(pos(n));
    c->name = opt_str_val(A(n, "name"));
    c->qname = qname;
    c->kind = kind;
    c->mods = has(n, "mods") ? strs_of(A(n, "mods")) : py::list<py::str>();
    py::list<py::str> typeparams;
    for (const Val& tp : L(n, "typeparams")) typeparams.append(str_val(A(dd(tp), "name")));
    c->typeparams = typeparams;
    c->extends = extends;
    py::list<TypeP> implements;
    for (const Val& iv : L(n, "interfaces")) {
        Dict i = dg(iv);
        if (!veq(G(i, "n"), "System.IEquatable") || !(kind == K("record") || kind == K("record struct"))) {
            implements.append(ty(iv));
        }
    }
    c->implements = implements;
    c->members = py::list<NodeP>();
    c->outer = outer;
    c->tparams = tparams(G(n, "typeparams"));
    py::ref<ir::ClassDecl> prev = cls;
    cls = c;
    auto restore = finally_([&] { cls = prev; });
    if (kind == K("delegate")) {
        Val invv = G(n, "invoke");
        SymP inv = sym(invv);
        py::list<NodeP> params;
        Dict invd = dg(A(n, "invoke"));
        py::list<Val> pn = L(invd, "pnames"), pt = L(invd, "params"), rk = L(invd, "refs");
        for (int64_t i = 0; i < std::min(pn.size(), std::min(pt.size(), rk.size())); ++i) {
            auto prm = make<ir::Param>(Pos{});
            prm->sym = mk_sym("param", str_val(pn[i]), std::optional<py::str>(*qname + K(".Invoke")), false, ty(pt[i]));
            prm->type = ty(pt[i]);
            py::str r = ref_of(rk[i]);
            py::list<py::str> mods;
            if (py::len(r) != 0) mods.append(r);
            prm->mods = mods;
            params.append(prm);
        }
        c->members->append(mk_method(inv, K("Invoke"), "method", params, need(inv)->ret, nullptr, strs({"public", "abstract"}),
                                     pos(n)));
        return c;
    }
    if (!G(n, "params").is_none() && (kind == K("record") || kind == K("record struct") || kind == K("class") || kind == K("struct"))) {
        primary(c, n);
    }
    for (const Val& mv : L(n, "members")) {
        for (const NodeP& x : member(dd(mv), c)) c->members->append(x);
    }
    if (kind == K("class") && !c->mods->contains(K("static"))) {
        bool any_ctor = false;
        for (const NodeP& x : *c->members) {
            auto md = as_node<ir::MethodDecl>(x);
            if (md && md->kind == std::optional<py::str>(K("ctor"))) {
                any_ctor = true;
                break;
            }
        }
        if (!any_ctor) {
            // the compiler's default constructor (as javac's, marked IMPLICIT)
            SymP csym = mk_sym("ctor", K(".ctor"), qname, false, nullptr, py::list<TypeP>(), ir::VOID);
            auto cc = make<ir::CtorCall>(pos(n));
            cc->kind = K("super:implicit");
            cc->sym = nullptr;
            cc->args = py::list<NodeP>();
            cc->target = nullptr;
            py::list<NodeP> bs;
            bs.append(cc);
            auto body = mk_block(bs, pos(n));
            py::list<py::str> mods;
            mods.append(!c->mods->contains(K("abstract")) ? K("public") : K("protected"));
            mods.append(K("IMPLICIT"));
            c->members->append(mk_method(csym, K(".ctor"), "ctor", py::list<NodeP>(), ir::VOID, body, mods, pos(n)));
        }
    }
    return c;
}

void Lowering::primary(const py::ref<ir::ClassDecl>& c, const Dict& n) {
    py::dict<py::str, Dict> implicit;
    if (c->kind == std::optional<py::str>(K("record")) || c->kind == std::optional<py::str>(K("record struct"))) {
        for (const Val& xv : L(or_empty(G(n, "sym")), "implicit")) {
            Dict x = dd(xv);
            if (veq(A(x, "kind"), "Property")) implicit.set(str_val(A(x, "name")), x);
        }
    }
    py::list<NodeP> params;
    for (const Val& pv : items_of(A(n, "params"))) {
        Dict p = dd(pv);
        SymP ps = sym(A(p, "sym"));
        auto prm = make<ir::Param>(pos(p));
        prm->sym = ps;
        prm->type = need(ps)->type;
        prm->mods = has(p, "mods") ? strs_of(A(p, "mods")) : py::list<py::str>();
        params.append(prm);
        if (implicit.contains(ps->name)) {
            SymP psym = sym(Val(implicit.at(ps->name)));
            bool is_record = c->kind == std::optional<py::str>(K("record"));
            py::list<NodeP> acc;
            acc.append(mk_method(nullptr, K("get"), "get", py::list<NodeP>(), ps->type, nullptr, py::list<py::str>(), Pos{}));
            acc.append(mk_method(nullptr, is_record ? K("init") : K("set"), is_record ? "init" : "set", py::list<NodeP>(),
                                 ir::VOID, nullptr, py::list<py::str>(), Pos{}));
            auto prop = make<ir::Property>(pos(p));
            prop->sym = psym;
            prop->type = need(psym)->type;
            prop->mods = strs({"public", "positional"});
            prop->params = py::list<NodeP>();
            prop->accessors = acc;
            prop->init = nullptr;
            c->members->append(prop);
        }
    }
    py::list<NodeP> stmts;
    if (!G(n, "baseCall").is_none()) stmts.append(ctor_init(dd(A(n, "baseCall"))));
    py::list<TypeP> ptypes;
    for (const NodeP& p : params) ptypes.append(as_node<ir::Param>(p)->type);
    SymP csym = mk_sym("ctor", K(".ctor"), c->qname, false, nullptr, ptypes, ir::VOID);
    c->members->append(mk_method(csym, K(".ctor"), "ctor", params, ir::VOID, mk_block(stmts, pos(n)),
                                 strs({"public", "primary", "IMPLICIT"}), pos(n)));
}

py::list<NodeP> Lowering::params(const Val& ps) {
    py::list<NodeP> out;
    if (!is_truthy(ps)) return out;
    for (const Val& pv : items_of(ps)) {
        Dict p = dg(pv);
        SymP s = sym(A(p, "sym"));
        auto prm = make<ir::Param>(pos(p));
        prm->sym = s;
        prm->type = s ? s->type : ir::ANY;
        prm->mods = has(p, "mods") ? strs_of(A(p, "mods")) : py::list<py::str>();
        out.append(prm);
    }
    return out;
}

py::list<NodeP> Lowering::member(const Dict& m, const py::ref<ir::ClassDecl>& c) {
    std::optional<py::str> k = kind_of(m);
    py::list<NodeP> out;
    if (is_(k, "type")) {
        out.append(decl(m, c->qname));
        return out;
    }
    if (is_(k, "field")) {
        for (const Val& vv : L(m, "vars")) {
            Dict v = dd(vv);
            SymP s = sym(A(v, "sym"));
            NodeP init = !G(v, "init").is_none() ? expr_in_decl(dd(A(v, "init"))) : NodeP();
            auto f = make<ir::FieldDecl>(pos(v));
            f->sym = s;
            f->type = need(s)->type;
            f->init = init;
            f->mods = has(m, "mods") ? strs_of(A(m, "mods")) : py::list<py::str>();
            out.append(f);
        }
        return out;
    }
    if (in_(k, {"property", "indexer"})) {
        SymP s = sym(A(m, "sym"));
        py::list<NodeP> accessors;
        if (!G(m, "expr").is_none()) {   // int X => expr;
            py::ref<ir::Block> body = method_body(A(m, "expr"));
            accessors.append(mk_method(nullptr, K("get"), "get", py::list<NodeP>(), need(s)->type, body, py::list<py::str>(), pos(m)));
        }
        for (const Val& av : L(m, "accessors")) {
            Dict a = dd(av);
            py::ref<ir::Block> body = !G(a, "body").is_none() ? method_body(A(a, "body")) : py::ref<ir::Block>();
            Val ak = A(a, "kind");
            py::str kind = str_val(ak);
            accessors.append(mk_method(nullptr, kind, kind.utf8().c_str(), py::list<NodeP>(), kind == K("get") ? need(s)->type : ir::VOID,
                                       body, has(a, "mods") ? strs_of(A(a, "mods")) : py::list<py::str>(), pos(a)));
        }
        NodeP init = !G(m, "init").is_none() ? expr_in_decl(dd(A(m, "init"))) : NodeP();
        auto prop = make<ir::Property>(pos(m));
        prop->sym = s;
        prop->type = need(s)->type;
        prop->mods = has(m, "mods") ? strs_of(A(m, "mods")) : py::list<py::str>();
        prop->params = params(G(m, "params"));
        prop->accessors = accessors;
        prop->init = init;
        out.append(prop);
        return out;
    }
    if (in_(k, {"method", "operator", "conversion"})) {
        SymP s = sym(A(m, "sym"));
        py::ref<ir::Block> body = !G(m, "body").is_none() ? method_body(A(m, "body")) : py::ref<ir::Block>();
        py::list<py::str> mods = has(m, "mods") ? strs_of(A(m, "mods")) : py::list<py::str>();
        if (!is_(k, "method")) mods.append(K("operator"));
        py::str name = s ? s->name : K("?");
        auto d = mk_method(s, name, "method", params(G(m, "params")), s ? s->ret : ir::VOID, body, mods, pos(m));
        py::list<py::str> typeparams;
        for (const Val& tp : L(m, "typeparams")) typeparams.append(str_val(A(dd(tp), "name")));
        d->typeparams = typeparams;
        d->tparams = tparams(G(m, "typeparams"));
        out.append(d);
        return out;
    }
    if (is_(k, "ctor")) {
        SymP s = sym(A(m, "sym"));
        bool static_ = s && s->flags.contains(K("static_init"));
        py::ref<ir::Block> body = ctor_body(G(m, "body"), m);
        auto d = mk_method(s, static_ ? K("<clinit>") : K(".ctor"), static_ ? "static_init" : "ctor", params(G(m, "params")),
                           ir::VOID, body, has(m, "mods") ? strs_of(A(m, "mods")) : py::list<py::str>(), pos(m));
        out.append(d);
        return out;
    }
    if (is_(k, "enummember")) {
        SymP s = sym(A(m, "sym"));
        Val v = G(m, "value");
        auto e = make<ir::EnumConst>(pos(m));
        e->sym = s;
        e->args = py::list<NodeP>();
        e->body = nullptr;
        e->ctor = nullptr;
        e->value = is_truthy(v) ? std::optional<int64_t>(int_of(A(dg(v), "v"))) : std::nullopt;
        out.append(e);
        return out;
    }
    py::str why = K("not lowered yet");
    if (is_(k, "dtor")) why = K("a finalizer: no target runs code when the collector frees an object");
    else if (is_(k, "eventfield") || is_(k, "event")) why = K("an event");
    out.append(unsupported(m, k, why, ir::ANY));
    return out;
}

// ---- bodies
py::ref<ir::Block> Lowering::method_body(const Val& opv) {
    if (opv.is_none()) return nullptr;
    Dict op = dd(opv);
    std::optional<py::str> k = kind_of(op);
    if (in_(k, {"MethodBody", "MethodBodyOperation", "ConstructorBody", "ConstructorBodyOperation"})) {
        Val b = G(op, "blockBody");
        if (!is_truthy(b)) b = G(op, "expressionBody");
        if (is_truthy(b)) return method_body(b);
        return mk_block(py::list<NodeP>(), pos(op));
    }
    if (is_(k, "Block")) return block(op);
    py::list<Val> one_op;
    one_op.append(Val(op));
    return mk_block(stmts(one_op), pos(op));
}

py::ref<ir::Block> Lowering::ctor_body(const Val& opv, const Dict& m) {
    py::list<NodeP> out;
    if (!opv.is_none() && !G(dg(opv), "initializer").is_none()) out.append(ctor_init(dd(A(dg(opv), "initializer"))));
    Val b;
    if (!opv.is_none()) {
        Dict op = dg(opv);
        b = G(op, "blockBody");
        if (!is_truthy(b)) b = G(op, "expressionBody");
    }
    if (!b.is_none()) {
        py::ref<ir::Block> inner = method_body(b);
        out.extend(*inner->stmts);
    }
    return mk_block(out, pos(is_truthy(opv) ? dg(opv) : m));
}

NodeP Lowering::ctor_init(const Dict& st) {
    Val inv_v = veq(A(st, "k"), "ExpressionStatement") ? G(st, "operation") : Val(st);
    if (inv_v.is_none() || !veq(A(dd(inv_v), "k"), "Invocation")) {
        return mk_exprstmt(unsupported(st, K("ctor-init"), K("a constructor initialiser of another shape")), pos(st));
    }
    Dict inv = dd(inv_v);
    SymP s = sym(A(inv, "targetMethod"));
    py::str kind = (cls && need(s)->owner == cls->qname) ? K("this") : K("super");
    if (is_truthy(G(inv, "implicit"))) kind = kind + K(":implicit");
    pending = py::list<NodeP>();
    effects = 0;
    py::list<NodeP> a = args(inv, s);
    py::list<NodeP> pre = *pending;
    pending = std::nullopt;
    auto cc = make<ir::CtorCall>(pos(st));
    cc->kind = kind;
    cc->sym = s;
    cc->args = a;
    cc->target = nullptr;
    if (!pre.empty()) {
        return mk_exprstmt(unsupported(st, K("ctor-init"), K("a constructor initialiser needing a statement")), pos(st));
    }
    return cc;
}

py::ref<ir::Block> Lowering::block(const Dict& op) { return mk_block(stmts(L(op, "operations")), pos(op)); }

py::list<NodeP> Lowering::stmts(const py::list<Val>& ops_in) {
    py::list<NodeP> out;
    py::list<Val> ops = ops_in.copy();
    for (int64_t i = 0; i < ops.size(); ++i) {
        Dict op = dd(ops[i]);
        if (veq(A(op, "k"), "UsingDeclaration")) {
            // `using var x = ...;` holds to the end of the block: a scoped resource around the rest
            py::list<NodeP> res = stmt(dd(A(op, "declarationGroup")));
            auto body = mk_block(stmts(ops.slice(i + 1, ops.size())), pos(op));
            auto t = make<ir::Try>(pos(op));
            py::list<NodeP> resources;
            for (const NodeP& r : res) {
                if (is_a<ir::Local>(r)) resources.append(r);
            }
            t->resources = resources;
            t->body = body;
            t->catches = py::list<NodeP>();
            t->final = nullptr;
            out.append(t);
            bool not_local = false;
            for (const NodeP& r : res) {
                if (!is_a<ir::Local>(r)) not_local = true;
            }
            if (not_local) {
                out.append(mk_exprstmt(unsupported(op, K("using"), K("a using declaration with a hoisted part")), pos(op)));
            }
            return out;
        }
        out.extend(stmt(op));
    }
    return out;
}

NodeP Lowering::one(const Dict& op) {
    py::list<NodeP> ss = stmt(op);
    if (ss.size() == 1) return ss[0];
    return mk_block(ss, pos(op));
}

py::list<NodeP> Lowering::stmt(const Dict& op) {
    auto saved_pending = pending;
    int64_t saved_effects = effects, saved_guard = guard;
    pending = py::list<NodeP>();
    effects = 0;
    guard = 0;
    py::list<NodeP> s;
    try {
        s = stmt_(op);
    } catch (const Refuse& e) {
        s = py::list<NodeP>();
        s.append(mk_exprstmt(unsupported(op, e.kind, e.why, ir::ANY), pos(op)));
    }
    py::list<NodeP> pre = pending ? *pending : py::list<NodeP>();
    bool pre_none = !pending;
    pending = saved_pending;
    effects = saved_effects;
    guard = saved_guard;
    if (pre_none) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'list'");
    return pre + s;
}

py::list<NodeP> Lowering::stmt_(const Dict& op) {
    std::optional<py::str> k = kind_of(op);
    Pos p = pos(op);
    py::list<NodeP> out;
    if (is_(k, "Block")) {
        out.append(block(op));
        return out;
    }
    if (is_(k, "VariableDeclarationGroup")) {
        for (const Val& declv : L(op, "declarations")) {
            Dict decl_ = dg(declv);
            for (const Val& dv : L(decl_, "declarators")) {
                Dict d = dd(dv);
                SymP s = sym(A(d, "symbol"));
                Val initv = G(d, "initializer");
                if (!is_truthy(initv)) initv = G(decl_, "initializer");
                NodeP v = !initv.is_none() ? expr(dd(A(dd(initv), "value"))) : NodeP();
                py::list<py::str> mods;
                if (need(s)->flags.contains(K("const"))) mods.append(K("const"));
                out.append(mk_local(s, s->type, v, mods, pos(d)));
            }
        }
        return out;
    }
    if (is_(k, "ExpressionStatement")) {
        Dict e = dd(A(op, "operation"));
        if (veq(A(e, "k"), "Throw")) {
            auto t = make<ir::Throw>(p);
            t->expr = is_truthy(G(e, "exception")) ? expr(dd(A(e, "exception"))) : NodeP();
            out.append(t);
            return out;
        }
        if (veq(A(e, "k"), "SimpleAssignment") && veq(A(dd(A(e, "target")), "k"), "Discard")) {
            out.append(mk_exprstmt(expr(dd(A(e, "value"))), p));
            return out;
        }
        out.append(mk_exprstmt(expr(e), p));
        return out;
    }
    if (is_(k, "Throw")) {
        auto t = make<ir::Throw>(p);
        t->expr = is_truthy(G(op, "exception")) ? expr(dd(A(op, "exception"))) : NodeP();
        out.append(t);
        return out;
    }
    if (is_(k, "Rethrow")) {
        auto t = make<ir::Throw>(p);
        t->expr = nullptr;
        out.append(t);
        return out;
    }
    if (is_(k, "Conditional")) {
        auto s = make<ir::If>(p);
        s->cond = expr(dd(A(op, "condition")));
        s->then = one(dd(A(op, "whenTrue")));
        s->orelse = is_truthy(G(op, "whenFalse")) ? one(dd(A(op, "whenFalse"))) : NodeP();
        out.append(s);
        return out;
    }
    if (is_(k, "Loop")) return loop(op, p);
    if (is_(k, "Branch")) {
        Val bk = G(op, "branchKind");
        if (veq(bk, "Break")) {
            auto b = make<ir::Break>(p);
            out.append(b);
            return out;
        }
        if (veq(bk, "Continue")) {
            auto b = make<ir::Continue>(p);
            out.append(b);
            return out;
        }
        if (veq(bk, "GoTo")) {
            Val tv = G(or_empty(G(op, "target")), "name", Val(K("")));
            if (!holds<py::str>(tv)) throw py::AttributeError("object has no attribute 'startswith'");
            const py::str& tgt = as<py::str>(tv);
            if (tgt.startswith(K("case ")) || tgt == K("default:") || tgt.startswith(K("default"))) {
                throw Refuse{K("goto-case"), K("`goto case`: a jump between switch sections")};
            }
            auto g = make<ir::Goto>(p);
            g->label = tgt;
            out.append(g);
            return out;
        }
        throw Refuse{K("branch"), K("a ") + pstr(bk) + K(" branch")};
    }
    if (is_(k, "Labeled")) {
        NodeP inner = is_truthy(G(op, "operation")) ? one(dd(A(op, "operation"))) : NodeP(make<ir::Empty>(p));
        auto l = make<ir::Labeled>(p);
        l->label = opt_str_val(G(or_empty(G(op, "label")), "name"));
        l->body = inner;
        out.append(l);
        return out;
    }
    if (is_(k, "Return")) {
        Val rk = G(op, "returnKind", Val(K("Return")));
        if (!veq(rk, "Return")) throw Refuse{K("iterator"), K("`yield return` / `yield break`: an iterator (a state machine)")};
        auto r = make<ir::Return>(p);
        r->expr = is_truthy(G(op, "returnedValue")) ? expr(dd(A(op, "returnedValue"))) : NodeP();
        out.append(r);
        return out;
    }
    if (is_(k, "YieldReturn") || is_(k, "YieldBreak")) {
        throw Refuse{K("iterator"), K("`yield return` / `yield break`: an iterator (a state machine)")};
    }
    if (is_(k, "Switch")) {
        out.append(switch_(op, p));
        return out;
    }
    if (is_(k, "Try")) {
        out.append(try_(op, p));
        return out;
    }
    if (is_(k, "Using")) {
        Dict res = dd(A(op, "resources"));
        py::list<NodeP> resources;
        if (veq(A(res, "k"), "VariableDeclarationGroup")) {
            resources = stmt(res);
        } else {
            resources.append(mk_exprstmt(expr(res), pos(res)));
        }
        auto t = make<ir::Try>(p);
        t->resources = resources;
        t->body = as_block(dd(A(op, "body")));
        t->catches = py::list<NodeP>();
        t->final = nullptr;
        out.append(t);
        return out;
    }
    if (is_(k, "Lock")) {
        auto s = make<ir::Sync>(p);
        s->lock = expr(dd(A(op, "lockedValue")));
        s->body = as_block(dd(A(op, "body")));
        out.append(s);
        return out;
    }
    if (is_(k, "Empty")) {
        out.append(make<ir::Empty>(p));
        return out;
    }
    if (is_(k, "LocalFunction")) throw Refuse{K("local-function"), K("a local function")};
    if (is_(k, "UsingDeclaration")) throw Refuse{K("using"), K("a using declaration out of a block")};
    throw Refuse{k, K("the statement ") + pstr(k)};
}

py::ref<ir::Block> Lowering::as_block(const Dict& op) {
    NodeP s = one(op);
    if (auto b = as_node<ir::Block>(s)) return b;
    py::list<NodeP> l;
    l.append(s);
    return mk_block(l, pos(op));
}

py::list<NodeP> Lowering::loop(const Dict& op, const Pos& p) {
    Val lk = G(op, "loopKind");
    py::list<NodeP> out;
    if (veq(lk, "While")) {
        guard += 1;
        NodeP cond = is_truthy(G(op, "condition")) ? expr(dd(A(op, "condition"))) : NodeP(mk_lit(Val(true), ir::BOOL, p));
        guard -= 1;
        if (is_truthy(G(op, "conditionIsUntil"))) cond = mk_unary("!", cond, ir::BOOL, p);
        NodeP body = one(dd(A(op, "body")));
        if (is_truthy(G(op, "conditionIsTop", Val(true)))) {
            auto w = make<ir::While>(p);
            w->cond = cond;
            w->body = body;
            out.append(w);
        } else {
            auto w = make<ir::DoWhile>(p);
            w->body = body;
            w->cond = cond;
            out.append(w);
        }
        return out;
    }
    if (veq(lk, "For")) {
        py::list<NodeP> init;
        for (const Val& b : L(op, "before")) init.extend(stmt(dd(b)));
        guard += 1;
        NodeP cond = is_truthy(G(op, "condition")) ? expr(dd(A(op, "condition"))) : NodeP();
        py::list<NodeP> update;
        for (const Val& uv : L(op, "atLoopBottom")) {
            Dict u = dd(uv);
            Dict e = veq(A(u, "k"), "ExpressionStatement") ? dd(A(u, "operation")) : u;
            update.append(expr(e));
        }
        guard -= 1;
        auto f = make<ir::For>(p);
        f->init = init;
        f->cond = cond;
        f->update = update;
        f->body = one(dd(A(op, "body")));
        out.append(f);
        return out;
    }
    if (veq(lk, "ForEach")) {
        Val vv = G(op, "loopControlVariable");
        if (vv.is_none() || !vin(A(dd(vv), "k"), {"VariableDeclarator", "LocalReference"})) {
            throw Refuse{K("foreach"), K("a foreach that deconstructs its element")};
        }
        Dict v = dd(vv);
        Val symv = G(v, "symbol");
        if (!is_truthy(symv)) symv = G(v, "local");
        SymP s = sym(symv);
        Dict coll = dd(A(op, "collection"));
        while (veq(A(coll, "k"), "Conversion") && is_truthy(G(coll, "implicit"))) {   // to IEnumerable: the compiler's own
            coll = dd(A(coll, "operand"));
        }
        auto var = mk_local(s, need(s)->type, nullptr, py::list<py::str>(), pos(v));
        auto fe = make<ir::ForEach>(p);
        fe->var = var;
        fe->iterable = expr(coll);
        fe->body = one(dd(A(op, "body")));
        out.append(fe);
        return out;
    }
    throw Refuse{K("loop"), K("a ") + pstr(lk) + K(" loop")};
}

py::ref<ir::Switch> Lowering::switch_(const Dict& op, const Pos& p) {
    py::list<NodeP> cases;
    for (const Val& cv : L(op, "cases")) {
        Dict c = dg(cv);
        py::list<NodeP> labels;
        bool default_flag = false;
        for (const Val& clv : L(c, "clauses")) {
            Dict cl = dg(clv);
            Val ck = G(cl, "caseKind");
            if (!is_truthy(ck)) ck = A(cl, "k");
            if (vin(ck, {"Default", "DefaultCaseClause"})) {
                default_flag = true;
            } else if (vin(ck, {"SingleValue", "SingleValueCaseClause"})) {
                labels.append(expr(dd(A(cl, "value"))));
            } else if (vin(ck, {"Pattern", "PatternCaseClause"}) &&
                       veq(G(or_empty(G(cl, "pattern")), "k"), "ConstantPattern") && G(cl, "guard").is_none()) {
                labels.append(expr(dd(A(dd(A(cl, "pattern")), "value"))));
            } else {
                throw Refuse{K("switch-pattern"), K("a switch section matched by a pattern or a `when` guard")};
            }
        }
        py::list<NodeP> body;
        for (const Val& b : L(c, "body")) body.extend(stmt(dd(b)));
        auto cs = make<ir::Case>(pos(c));
        cs->labels = labels;
        cs->default_ = default_flag;
        cs->body = body;
        cases.append(cs);
    }
    auto s = make<ir::Switch>(p);
    s->expr = expr(dd(A(op, "value")));
    s->cases = cases;
    return s;
}

py::ref<ir::Try> Lowering::try_(const Dict& op, const Pos& p) {
    py::list<NodeP> catches;
    for (const Val& cv : L(op, "catches")) {
        Dict c = dg(cv);
        if (!G(c, "filter").is_none()) throw Refuse{K("catch-filter"), K("an exception filter (`catch ... when`)")};
        Val d = G(c, "exceptionDeclarationOrExpression");
        SymP s;
        if (!d.is_none() && veq(A(dd(d), "k"), "VariableDeclarator")) s = sym(G(dd(d), "symbol"));
        auto ct = make<ir::Catch>(pos(c));
        ct->sym = s;
        py::list<TypeP> types;
        types.append(ty(G(c, "exceptionType")));
        ct->types = types;
        ct->body = block(dd(A(c, "handler")));
        catches.append(ct);
    }
    auto t = make<ir::Try>(p);
    t->resources = py::list<NodeP>();
    t->body = block(dd(A(op, "body")));
    t->catches = catches;
    t->final = is_truthy(G(op, "finally")) ? NodeP(block(dd(A(op, "finally")))) : NodeP();
    return t;
}

// ---- expressions
NodeP Lowering::expr_in_decl(const Dict& op) {
    auto saved_pending = pending;
    int64_t saved_effects = effects, saved_guard = guard;
    pending = std::nullopt;
    effects = 0;
    guard = 0;
    auto restore = finally_([&] {
        pending = saved_pending;
        effects = saved_effects;
        guard = saved_guard;
    });
    try {
        return expr(op);
    } catch (const Refuse& e) {
        return unsupported(op, e.kind, e.why);
    }
}

void Lowering::hoist(const Dict& n, const py::list<NodeP>& ss, bool needs_order) {
    (void)n;
    if (!pending) throw Refuse{K("hoist"), K("an initialiser needing a statement, in a declaration")};
    if (guard) throw Refuse{K("hoist"), K("an initialiser needing a statement, in a branch that might not run")};
    if (needs_order && effects) throw Refuse{K("hoist"), K("an initialiser needing a statement, after an effect in the same statement")};
    pending->extend(ss);
}

NodeP Lowering::temp(const TypeP& t, const NodeP& init, const Dict& n) {
    temps += 1;
    SymP s = mk_sym("local", K("__t") + py::to_str(temps), std::nullopt, false, t);
    py::list<NodeP> one_stmt;
    one_stmt.append(mk_local(s, t, init, py::list<py::str>(), pos(n)));
    hoist(n, one_stmt);
    auto nm = make_expr<ir::Name>(pos(n), t);
    nm->sym = s;
    return nm;
}

NodeP Lowering::expr(const Dict& n) {
    try {
        return expr_(n);
    } catch (const Refuse& e) {
        if (!pending) return unsupported(n, e.kind, e.why);
        throw;
    }
}

NodeP Lowering::expr_(const Dict& n) {
    std::optional<py::str> k = kind_of(n);
    Pos p = pos(n);
    TypeP t = ty(G(n, "$type"));
    if (is_(k, "Literal")) {
        Val v = value_(G(n, "$const"));
        return mk_lit(v, is_truthy(G(n, "$type")) ? t : ir::NULL_, p);
    }
    if (is_(k, "NameOf")) return mk_lit(value_(G(n, "$const")), STRING, p);
    if (is_(k, "LocalReference")) {
        SymP s = renamed(sym(A(n, "local")));
        if (is_truthy(G(n, "isDeclaration"))) {   // `out var x`
            py::list<NodeP> h;
            h.append(mk_local(s, need(s)->type, nullptr, py::list<py::str>(), p));
            hoist(n, h, false);
        }
        auto nm = make_expr<ir::Name>(p, t);
        nm->sym = s;
        return nm;
    }
    if (is_(k, "ParameterReference")) {
        auto nm = make_expr<ir::Name>(p, t);
        nm->sym = sym(A(n, "parameter"));
        return nm;
    }
    if (is_(k, "FieldReference")) {
        Dict field = dg(A(n, "field"));
        if (is_truthy(G(field, "const")) && !is_truthy(G(field, "source")) && !G(n, "$const").is_none()) {
            return mk_lit(value_(A(n, "$const")), t, p);   // a library constant (int.MaxValue): its value
        }
        SymP s = sym(A(n, "field"));
        auto sel = make_expr<ir::Select>(p, t);
        sel->obj = receiver(n, s);
        sel->sym = s;
        return sel;
    }
    if (is_(k, "PropertyReference")) {
        SymP s = sym(A(n, "property"));
        NodeP obj = receiver(n, s);
        if (need(s)->owner == std::optional<py::str>(K("System.Array")) && s->name == K("Length")) {
            auto l = make_expr<ir::Length>(p, t);
            l->arr = obj;
            return l;
        }
        if (s->flags.contains(K("indexer"))) {
            py::list<NodeP> a = args(n, s);
            if (a.size() != 1) throw Refuse{K("indexer"), K("an indexer of ") + py::to_str(a.size()) + K(" arguments")};
            auto ix = make_expr<ir::Index>(p, t);
            ix->arr = obj;
            ix->index = a[0];
            ix->sym = s;
            return ix;
        }
        auto sel = make_expr<ir::Select>(p, t);
        sel->obj = obj;
        sel->sym = s;
        return sel;
    }
    if (is_(k, "EventReference")) throw Refuse{K("event"), K("an event")};
    if (is_(k, "Invocation")) return call(n, t, p);
    if (is_(k, "ObjectCreation")) return new_(n, t, p);
    if (is_(k, "TypeParameterObjectCreation")) throw Refuse{K("new-T"), K("`new T()` of a type parameter")};
    if (is_(k, "AnonymousObjectCreation")) throw Refuse{K("anonymous-type"), K("an anonymous type")};
    if (is_(k, "ArrayCreation")) return new_array(n, t, p);
    if (is_(k, "ArrayElementReference")) {
        NodeP arr = expr(dd(A(n, "arrayReference")));
        py::list<NodeP> idx;
        for (const Val& i : L(n, "indices")) idx.append(expr(dd(i)));
        auto ix = make_expr<ir::Index>(p, t);
        ix->arr = arr;
        if (idx.size() == 1) ix->index = idx[0];
        else ix->index = idx;
        return ix;
    }
    if (is_(k, "Binary")) return binary(n, t, p);
    if (is_(k, "Unary")) {
        if (!G(n, "operatorMethod").is_none()) {
            SymP s = sym(A(n, "operatorMethod"));
            py::list<NodeP> a;
            a.append(expr(dd(A(n, "operand"))));
            return op_call(n, s, a, t, p);
        }
        Val ok = G(n, "operatorKind");
        std::optional<py::str> op;
        if (holds<py::str>(ok) && UNARY_.contains(as<py::str>(ok))) op = UNARY_.at(as<py::str>(ok));
        if (!op || is_truthy(G(n, "isLifted"))) {
            throw Refuse{K("unary"), K("the unary operator ") + pstr(ok) +
                                          (is_truthy(G(n, "isLifted")) ? K(" (lifted)") : K(""))};
        }
        auto u = make_expr<ir::Unary>(p, t);
        u->op = op;
        u->x = expr(dd(A(n, "operand")));
        u->postfix = false;
        u->overflow = is_truthy(G(n, "isChecked")) ? std::optional<py::str>(K("trap")) : std::nullopt;
        return u;
    }
    if (is_(k, "Increment") || is_(k, "Decrement")) {
        if (!G(n, "operatorMethod").is_none() || is_truthy(G(n, "isLifted"))) {
            throw Refuse{K("increment"), K("a user-defined or lifted ++/--")};
        }
        effects += 1;
        auto u = make_expr<ir::Unary>(p, t);
        u->op = is_(k, "Increment") ? K("++") : K("--");
        u->x = expr(dd(A(n, "target")));
        u->postfix = is_truthy(G(n, "isPostfix"));
        u->overflow = is_truthy(G(n, "isChecked")) ? std::optional<py::str>(K("trap")) : std::nullopt;
        return u;
    }
    if (is_(k, "SimpleAssignment")) {
        if (vin(A(dd(A(n, "target")), "k"), {"Tuple", "Discard"}) || is_truthy(G(n, "isRef"))) {
            throw Refuse{K("deconstruction"), K("a deconstructing, discarding or ref assignment")};
        }
        NodeP target = expr(dd(A(n, "target")));
        NodeP value = expr(dd(A(n, "value")));
        effects += 1;
        auto a = make_expr<ir::Assign>(p, t);
        a->op = std::nullopt;
        a->target = target;
        a->value = value;
        return a;
    }
    if (is_(k, "DeconstructionAssignment")) throw Refuse{K("deconstruction"), K("a deconstruction")};
    if (is_(k, "CompoundAssignment")) {
        if (!G(n, "operatorMethod").is_none() && !is_string(t)) {
            throw Refuse{K("compound"), K("a compound assignment through a user-defined operator")};
        }
        if (is_truthy(G(n, "isLifted"))) throw Refuse{K("lifted"), K("lifted nullable arithmetic")};
        Val ok = G(n, "operatorKind");
        std::optional<py::str> op;
        if (holds<py::str>(ok) && BINARY_.contains(as<py::str>(ok))) op = BINARY_.at(as<py::str>(ok));
        NodeP target = expr(dd(A(n, "target")));
        NodeP value = expr(dd(A(n, "value")));
        effects += 1;
        auto a = make_expr<ir::Assign>(p, t);
        a->op = op;
        a->target = target;
        a->value = value;
        a->overflow = is_truthy(G(n, "isChecked")) ? std::optional<py::str>(K("trap")) : std::nullopt;
        return a;
    }
    if (is_(k, "CoalesceAssignment")) {
        NodeP target = expr(dd(A(n, "target")));
        guard += 1;
        NodeP value = expr(dd(A(n, "value")));
        guard -= 1;
        effects += 1;
        auto a = make_expr<ir::Assign>(p, t);
        a->op = K("??");
        a->target = target;
        a->value = value;
        return a;
    }
    if (is_(k, "Conditional")) {
        NodeP cond = expr(dd(A(n, "condition")));
        guard += 1;
        NodeP a = expr(dd(A(n, "whenTrue")));
        NodeP b = expr(dd(A(n, "whenFalse")));
        guard -= 1;
        auto c = make_expr<ir::Cond>(p, t);
        c->cond = cond;
        c->then = a;
        c->orelse = b;
        return c;
    }
    if (is_(k, "Coalesce")) {
        NodeP left = expr(dd(A(n, "value")));
        guard += 1;
        NodeP right = expr(dd(A(n, "whenNull")));
        guard -= 1;
        return mk_binary(K("??"), left, right, t, p);
    }
    if (is_(k, "ConditionalAccess")) return cond_access(n, t, p);
    if (is_(k, "Conversion")) return conversion(n, t, p);
    if (is_(k, "IsType")) {
        NodeP x = mk_instanceof(expr(dd(A(n, "valueOperand"))), ty(G(n, "typeOperand")), nullptr, p);
        return is_truthy(G(n, "isNegated")) ? NodeP(mk_unary("!", x, ir::BOOL, p)) : x;
    }
    if (is_(k, "IsPattern")) {
        NodeP v = expr(dd(A(n, "value")));
        return pattern(dd(A(n, "pattern")), v, p);
    }
    if (is_(k, "SwitchExpression")) return switch_expr(n, t, p);
    if (is_(k, "DelegateCreation")) {
        Dict target = dd(A(n, "target"));
        if (veq(A(target, "k"), "AnonymousFunction")) return lam(target, t, p);
        if (veq(A(target, "k"), "MethodReference")) {
            SymP s = sym(A(target, "method"));
            Val inst = G(target, "instance");
            NodeP obj;
            if (!inst.is_none() && !is_truthy(G(dg(inst), "implicit"))) {
                obj = expr(dd(inst));
            } else if (need(s)->static_) {
                obj = make_expr<ir::TypeName>(p, class_type(s->owner));
            } else {
                auto th = make_expr<ir::This>(p, class_type(s->owner));
                th->qualifier = nullptr;
                obj = th;
            }
            auto mr = make_expr<ir::MethodRef>(p, t);
            mr->target = obj;
            mr->sym = s;
            mr->mode = K("invoke");
            return mr;
        }
        throw Refuse{K("delegate"), K("a delegate made from ") + pstr(A(target, "k"))};
    }
    if (is_(k, "AnonymousFunction")) return lam(n, t, p);
    if (is_(k, "InstanceReference")) {
        if (veq(G(n, "referenceKind"), "ImplicitReceiver")) {
            if (!receiver_now) throw Refuse{K("initializer"), K("an initialiser receiver outside its creation")};
            return receiver_now;
        }
        auto th = make_expr<ir::This>(p, t);
        th->qualifier = nullptr;
        return th;
    }
    if (is_(k, "ConditionalAccessInstance")) {
        if (!cond_recv) throw Refuse{K("null-conditional"), K("a `?.` receiver outside its access")};
        return cond_recv;
    }
    if (is_(k, "TypeOf")) {
        auto cl = make_expr<ir::ClassLit>(p, t);
        cl->of = ty(G(n, "typeOperand"));
        return cl;
    }
    if (is_(k, "DefaultValue")) return default_(n, t, p);
    if (is_(k, "InterpolatedString")) return interpolated(n, t, p);
    if (is_(k, "Throw")) throw Refuse{K("throw-expression"), K("a throw expression")};
    if (is_(k, "Await")) throw Refuse{K("await"), K("`await`: an asynchronous method (a state machine)")};
    if (is_(k, "DeclarationExpression") && veq(G(dg(G(n, "expression", Val(Dict()))), "k"), "LocalReference")) {
        return expr(dd(A(n, "expression")));   // out T x
    }
    if (is_(k, "Tuple") || is_(k, "DeclarationExpression")) throw Refuse{K("tuple"), K("a tuple")};
    if (is_(k, "Discard")) throw Refuse{K("discard"), K("a discard")};
    if (is_(k, "SizeOf") || is_(k, "AddressOf") || is_(k, "PointerIndirectionReference")) throw Refuse{K("unsafe"), K("unsafe code")};
    if (!k) throw py::AttributeError("'NoneType' object has no attribute 'startswith'");
    if (k->startswith(K("Dynamic"))) throw Refuse{K("dynamic"), K("`dynamic`")};
    if (is_(k, "Invalid")) throw Refuse{K("invalid"), K("code the compiler could not bind")};
    if (is_(k, "TranslatedQuery")) return expr(dd(A(n, "operation")));   // query syntax: the calls it means
    if (is_(k, "Parenthesized")) {
        auto pr = make_expr<ir::Paren>(p, t);
        pr->x = expr(dd(A(n, "operand")));
        return pr;
    }
    throw Refuse{k, K("the operation ") + pstr(k)};
}

NodeP Lowering::receiver(const Dict& n, const SymP& s) {
    Val instv = G(n, "instance");
    Pos p = pos(n);
    if (instv.is_none() || need(s)->static_) return make_expr<ir::TypeName>(p, class_type(s->owner));
    Dict inst = dd(instv);
    if (veq(A(inst, "k"), "InstanceReference")) {
        if (veq(G(inst, "referenceKind"), "ImplicitReceiver")) return expr_(inst);
        TypeP it = ty(G(inst, "$type"));
        if (cls && it->kind == K("class") && it->name != cls->qname && !is_truthy(G(inst, "implicit"))) {
            auto su = make_expr<ir::Super>(p, it);   // base.X
            su->qualifier = nullptr;
            return su;
        }
        auto th = make_expr<ir::This>(p, it);
        th->qualifier = nullptr;
        return th;
    }
    return expr(inst);
}

py::list<NodeP> Lowering::args(const Dict& n, const SymP& s) {
    (void)s;
    py::list<Val> items = L(n, "arguments").copy();
    std::vector<int64_t> order;
    for (int64_t i = 0; i < items.size(); ++i) {
        Dict a = dg(items[i]);
        if (is_truthy(G(a, "parameter"))) {
            Dict prm = dg(G(a, "parameter", Val(Dict())));
            order.push_back(number_of(G(prm, "ordinal", Val(i))));
        } else {
            order.push_back(i);
        }
    }
    std::vector<int64_t> sorted_order = order;
    std::stable_sort(sorted_order.begin(), sorted_order.end());
    if (order != sorted_order) {
        py::list<NodeP> vals;
        for (const Val& a : items) vals.append(expr(dd(A(dd(a), "value"))));
        for (const NodeP& v : vals) {
            if (effects_(v)) throw Refuse{K("named-args"), K("named arguments out of parameter order, with effects")};
        }
        std::vector<size_t> idx(items.size());
        for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
        std::stable_sort(idx.begin(), idx.end(), [&](size_t x, size_t y) { return order[x] < order[y]; });
        py::list<Val> re;
        for (size_t i : idx) re.append(items[int64_t(i)]);
        items = re;
    }
    py::list<NodeP> out;
    for (const Val& av : items) {
        Dict a = dg(av);
        Dict v = dd(A(a, "value"));
        if (veq(G(a, "argumentKind"), "ParamArray") && veq(A(v, "k"), "ArrayCreation") && is_truthy(G(v, "implicit"))) {
            Val init = G(v, "initializer");
            for (const Val& e : L(or_empty(init), "elementValues")) out.append(expr(dd(e)));
            continue;
        }
        NodeP x = expr(v);
        if (veq(G(a, "argumentKind"), "DefaultValue") && is_truthy(G(a, "parameter"))) {
            TypeP pt = ty(G(dg(A(a, "parameter")), "type"));
            if (!ir::same(type_of(x), pt) && pt->kind != K("any")) {   // the default as the parameter's type, so an
                x = mk_cast(x, pt, pos(n));                              // overload is chosen as the compiler chose it
            }
        }
        out.append(x);
    }
    return out;
}

NodeP Lowering::call(const Dict& n, const TypeP& t, const Pos& p) {
    SymP s = sym(A(n, "targetMethod"));
    Val instv = G(n, "instance");
    NodeP target;
    if (instv.is_none()) {
        target = need(s)->static_ ? NodeP(make_expr<ir::TypeName>(p, class_type(s->owner))) : NodeP();
    } else {
        Dict inst = dd(instv);
        if (veq(A(inst, "k"), "InstanceReference") && is_truthy(G(inst, "implicit")) &&
            !veq(G(inst, "referenceKind"), "ImplicitReceiver")) {
            target = nullptr;   // own method, implicit this
        } else {
            target = receiver(n, s);
        }
    }
    py::list<NodeP> a = args(n, s);
    effects += 1;
    py::list<TypeP> typeargs;
    for (const Val& ta : L(dg(A(n, "targetMethod")), "typeargs")) typeargs.append(ty(ta));
    if (!typeargs.empty() && !is_truthy(G(n, "typeArgsWritten"))) {   // the compiler inferred them: a back end may too
        py::list<py::str> fl = s->flags.copy();
        fl.append(K("inferred"));
        s = copy_sym(s, s->static_, fl);
    }
    auto c = make_expr<ir::Call>(p, t);
    c->target = target;
    c->sym = s;
    c->args = a;
    c->typeargs = ir::TypeArgs(std::variant<py::list<TypeP>, py::list<Val>>(typeargs));
    return c;
}

NodeP Lowering::op_call(const Dict& n, const SymP& s0, const py::list<NodeP>& a, const TypeP& t, const Pos& p) {
    (void)n;
    py::list<py::str> flags;
    for (const py::str& f : need(s0)->flags) {
        if (f != K("operator")) flags.append(f);
    }
    flags.append(K("operator"));
    SymP s = copy_sym(s0, true, flags);
    auto c = make_expr<ir::Call>(p, t);
    c->target = make_expr<ir::TypeName>(p, class_type(s->owner));
    c->sym = s;
    c->args = a;
    c->typeargs = ir::TypeArgs(std::variant<py::list<TypeP>, py::list<Val>>(py::list<TypeP>()));
    return c;
}

NodeP Lowering::new_(const Dict& n, const TypeP& t, const Pos& p) {
    SymP s;
    if (is_truthy(G(n, "constructor"))) {
        s = sym(A(n, "constructor"));
    } else {
        s = mk_sym("ctor", K(".ctor"), t->kind == K("class") ? t->name : std::optional<py::str>(), false, nullptr,
                   py::list<TypeP>(), ir::VOID);
    }
    py::list<NodeP> a = is_truthy(G(n, "constructor")) ? args(n, s) : py::list<NodeP>();
    effects += 1;
    auto e = make_expr<ir::New>(p, t);
    e->sym = s;
    e->args = a;
    e->body = nullptr;
    e->outer = nullptr;
    e->memory = K("gc");
    Val initv = G(n, "initializer");
    if (initv.is_none() || !is_truthy(G(dg(initv), "initializers"))) return e;
    Dict init = dd(initv);
    // an object or collection initialiser: a temporary, then each member set or Add, before the statement
    int64_t eff = effects;
    effects -= 1;
    NodeP tmp = temp(t, e, n);
    effects = eff;
    NodeP prev = receiver_now;
    receiver_now = tmp;
    py::list<NodeP> ss;
    {
        auto restore_recv = finally_([&] { receiver_now = prev; });
        for (const Val& iv : items_of(A(init, "initializers"))) {
            Dict i = dd(iv);
            auto saved_pending = pending;
            int64_t saved_effects = effects;
            pending = ss;   // each member set or Add: a statement of its own
            effects = 0;
            auto restore = finally_([&] {
                pending = saved_pending;
                effects = saved_effects;
            });
            NodeP x = expr(i);
            ss.append(mk_exprstmt(x, pos(i)));
        }
    }
    if (!pending) throw py::AttributeError("'NoneType' object has no attribute 'extend'");
    pending->extend(ss);
    return tmp;
}

NodeP Lowering::new_array(const Dict& n, const TypeP& t, const Pos& p) {
    TypeP elem = ir::ANY;
    if (t->kind == K("array")) elem = std::get<1>(t->dims());
    auto rank_is_one = [&]() { return same_val(t->info.get(K("rank"), Val(1)), Val(1)); };
    if (t->kind == K("array") && !rank_is_one()) elem = t->elem;
    Val initv = G(n, "initializer");
    py::list<NodeP> dims;
    if (initv.is_none()) {
        for (const Val& d : L(n, "dimensionSizes")) dims.append(expr(dd(d)));
    }
    std::optional<py::list<NodeP>> items;
    if (!initv.is_none()) {
        if (!rank_is_one()) throw Refuse{K("array"), K("a rectangular array initialiser")};
        py::list<NodeP> its;
        for (const Val& e : L(dg(initv), "elementValues")) its.append(expr(dd(e)));
        items = its;
    }
    auto na = make_expr<ir::NewArray>(p, t);
    na->elem = elem;
    na->dims = dims;
    na->init = items;
    return na;
}

NodeP Lowering::binary(const Dict& n, const TypeP& t, const Pos& p) {
    Val okv = G(n, "operatorKind");
    if (!G(n, "operatorMethod").is_none()) {
        SymP s = sym(A(n, "operatorMethod"));
        NodeP left = expr(dd(A(n, "leftOperand")));
        if (vin(okv, {"ConditionalAnd", "ConditionalOr"})) throw Refuse{K("operator"), K("a user-defined && or ||")};
        NodeP right = expr(dd(A(n, "rightOperand")));
        if ((need(s)->owner == std::optional<py::str>(K("System.String")) && s->name == K("op_Addition")) ||
            (is_string(t) && veq(okv, "Add"))) {
            return concat_(left, right, t, p);
        }
        py::list<NodeP> a;
        a.append(left);
        a.append(right);
        return op_call(n, s, a, t, p);
    }
    std::optional<py::str> op;
    if (holds<py::str>(okv) && BINARY_.contains(as<py::str>(okv))) op = BINARY_.at(as<py::str>(okv));
    if (!op) throw Refuse{K("binary"), K("the operator ") + pstr(okv)};
    NodeP left = expr(dd(A(n, "leftOperand")));
    bool logical = *op == K("&&") || *op == K("||");
    if (logical) guard += 1;
    NodeP right = expr(dd(A(n, "rightOperand")));
    if (logical) guard -= 1;
    if (is_truthy(G(n, "isLifted")) && !((*op == K("==") || *op == K("!=")) && (is_null_(left) || is_null_(right)))) {
        throw Refuse{K("lifted"), K("lifted nullable arithmetic (") + *op + K(" on a nullable value)")};
    }
    if (*op == K("+") && is_string(t)) return concat_(left, right, t, p);
    auto b = mk_binary(op, left, right, t, p);
    b->overflow = is_truthy(G(n, "isChecked")) ? std::optional<py::str>(K("trap")) : std::nullopt;
    return b;
}

NodeP Lowering::conversion(const Dict& n, const TypeP& t, const Pos& p) {
    Dict c = or_empty(G(n, "conversion"));
    Val operand = A(n, "operand");
    if (is_truthy(G(c, "userDefined")) && is_truthy(G(c, "method"))) {
        SymP s = sym(A(c, "method"));
        py::list<NodeP> a;
        a.append(expr(dd(operand)));
        return op_call(n, s, a, t, p);
    }
    if (is_truthy(G(n, "isTryCast"))) {   // x as T
        NodeP x = expr(dd(operand));
        if (!simple_(x)) throw Refuse{K("as"), K("`as` on an expression with effects")};
        auto cd = make_expr<ir::Cond>(p, t);
        cd->cond = mk_instanceof(x, t, nullptr, p);
        cd->then = mk_cast(x, t, p);
        cd->orelse = mk_lit(Val(), ir::NULL_, p);
        return cd;
    }
    NodeP x = expr(dd(operand));
    if (is_truthy(G(n, "implicit"))) {   // the compiler's, not written: left implicit
        auto lit = as_node<ir::Lit>(x);
        if (!is_truthy(G(c, "identity")) && lit && lit->value.is_none()) {
            lit->type = t->kind != K("any") ? t : lit->type;   // null as a typed null
        }
        return x;
    }
    auto ca = mk_cast(x, t, p);
    ca->overflow = is_truthy(G(n, "isChecked")) ? std::optional<py::str>(K("trap")) : std::nullopt;
    return ca;
}

NodeP Lowering::cond_access(const Dict& n, const TypeP& t, const Pos& p) {
    NodeP recv = expr(dd(A(n, "operation")));
    if (!simple_(recv)) throw Refuse{K("null-conditional"), K("`?.` on an expression with effects")};
    NodeP prev = cond_recv;
    cond_recv = recv;
    guard += 1;
    NodeP inner;
    {
        auto restore = finally_([&] {
            guard -= 1;
            cond_recv = prev;
        });
        inner = expr(dd(A(n, "whenNotNull")));
    }
    NodeP test = mk_binary(K("!="), recv, mk_lit(Val(), ir::NULL_, p), ir::BOOL, p);
    auto c = make_expr<ir::Cond>(p, t);
    c->cond = test;
    c->then = inner;
    c->orelse = mk_lit(Val(), ir::NULL_, p);
    return c;
}

NodeP Lowering::default_(const Dict& n, const TypeP& t, const Pos& p) {
    const py::str& kd = t->kind;
    if (kd == K("i8") || kd == K("i16") || kd == K("i32") || kd == K("i64") || kd == K("u8") || kd == K("u16") ||
        kd == K("u32") || kd == K("u64")) {
        return mk_lit(is_truthy(t->info.get(K("enc"), Val())) ? Val(py::chr(0)) : Val(0), t, p);
    }
    if (kd == K("f32") || kd == K("f64")) return mk_lit(Val(0.0), t, p);
    if (kd == K("bool")) return mk_lit(Val(false), t, p);
    Val cv = G(n, "$const");
    if (!cv.is_none() && veq(A(dd(cv), "t"), "null")) return mk_lit(Val(), t, p);
    throw Refuse{K("default"), K("`default` of ") + t->repr()};
}

NodeP Lowering::interpolated(const Dict& n, const TypeP& t, const Pos& p) {
    (void)t;
    py::list<NodeP> parts;
    for (const Val& pv : L(n, "parts")) {
        Dict part = dd(pv);
        Val pk = A(part, "k");
        if (veq(pk, "InterpolatedStringText")) {
            parts.append(mk_lit(value_(G(dg(A(part, "text")), "$const")), STRING, pos(part)));
        } else if (veq(pk, "Interpolation")) {
            if (!G(part, "alignment").is_none() || !G(part, "formatString").is_none()) {
                throw Refuse{K("interpolation-format"), K("an interpolation with an alignment or a format string")};
            }
            parts.append(expr(dd(A(part, "expression"))));
        } else {
            throw Refuse{K("interpolation"), K("an interpolated part ") + pstr(pk)};
        }
    }
    if (parts.empty()) return mk_lit(Val(K("")), STRING, p);
    if (parts.size() == 1 && is_string(type_of(parts[0]))) parts.insert(0, mk_lit(Val(K("")), STRING, p));
    auto c = make_expr<ir::Concat>(p, STRING);
    c->parts = parts;
    return c;
}

NodeP Lowering::lam(const Dict& n, const TypeP& t, const Pos& p) {
    Dict ms = or_empty(G(n, "symbol"));
    Val ov = G(ms, "owner");
    py::str owner = (is_truthy(ov) ? str_val(ov) : K("")) + K(".lambda");
    py::list<NodeP> params;
    py::list<Val> pn = L(ms, "pnames"), pt = L(ms, "params");
    for (int64_t i = 0; i < std::min(pn.size(), pt.size()); ++i) {
        auto prm = make<ir::Param>(Pos{});
        prm->sym = mk_sym("param", str_val(pn[i]), std::optional<py::str>(owner), false, ty(pt[i]));
        prm->type = ty(pt[i]);
        prm->mods = py::list<py::str>();
        params.append(prm);
    }
    auto saved_pending = pending;
    int64_t saved_effects = effects, saved_guard = guard;
    pending = std::nullopt;
    effects = 0;
    guard = 0;
    NodeP body;
    {
        auto restore = finally_([&] {
            pending = saved_pending;
            effects = saved_effects;
            guard = saved_guard;
        });
        body = method_body(A(n, "body"));
    }
    if (auto b = as_node<ir::Block>(body)) {
        if (b->stmts->size() == 1) {
            auto ret = as_node<ir::Return>((*b->stmts)[0]);
            if (ret && ret->expr) {
                Dict bd = dg(A(n, "body"));
                if (is_truthy(G(bd, "implicit", Val(true))) && implicit_body_(bd)) body = ret->expr;   // x => x + 1
            }
        }
    }
    auto l = make_expr<ir::Lambda>(p, t);
    l->params = params;
    l->body = body;
    return l;
}

// ---- patterns
NodeP Lowering::pattern(const Dict& pat, const NodeP& v, const Pos& p) {
    Val kv = A(pat, "k");
    if (veq(kv, "ConstantPattern")) {
        NodeP c = expr(dd(A(pat, "value")));
        if (is_null_(c)) return mk_binary(K("=="), v, c, ir::BOOL, p);
        if (is_string(type_of(v)) || is_string(type_of(c))) {
            py::list<TypeP> ps;
            ps.append(STRING);
            ps.append(STRING);
            py::list<py::str> fl;
            fl.append(K("operator"));
            SymP s = mk_sym("method", K("op_Equality"), std::optional<py::str>(K("System.String")), true, nullptr, ps, ir::BOOL,
                            false, py::list<py::str>(), fl);
            auto call_ = make_expr<ir::Call>(p, ir::BOOL);
            call_->target = make_expr<ir::TypeName>(p, STRING);
            call_->sym = s;
            py::list<NodeP> a;
            a.append(v);
            a.append(c);
            call_->args = a;
            call_->typeargs = ir::TypeArgs(std::variant<py::list<TypeP>, py::list<Val>>(py::list<TypeP>()));
            return call_;
        }
        return mk_binary(K("=="), v, c, ir::BOOL, p);
    }
    if (veq(kv, "DeclarationPattern")) {
        Val mtv = G(pat, "matchedType");
        if (!is_truthy(mtv)) mtv = G(pat, "narrowedType");
        TypeP mt = ty(mtv);
        Val ds = G(pat, "declaredSymbol");
        if (is_truthy(G(pat, "matchesNull"))) throw Refuse{K("var-pattern"), K("a `var` pattern")};
        NodeP binding = is_truthy(ds) ? NodeP(mk_local(renamed(sym(ds)), mt, nullptr, py::list<py::str>(), p)) : NodeP();
        return mk_instanceof(v, mt, binding, p);
    }
    if (veq(kv, "TypePattern")) return mk_instanceof(v, ty(G(pat, "matchedType")), nullptr, p);
    if (veq(kv, "RelationalPattern")) {
        Val ok = G(pat, "operatorKind");
        std::optional<py::str> op;
        if (holds<py::str>(ok) && BINARY_.contains(as<py::str>(ok))) op = BINARY_.at(as<py::str>(ok));
        return mk_binary(op, v, expr(dd(A(pat, "value"))), ir::BOOL, p);
    }
    if (veq(kv, "NegatedPattern")) return mk_unary("!", pattern(dd(A(pat, "pattern")), v, p), ir::BOOL, p);
    if (veq(kv, "BinaryPattern")) {
        if (!simple_(v)) throw Refuse{K("pattern"), K("an and/or pattern on an expression with effects")};
        py::str op = veq(G(pat, "operatorKind"), "And") ? K("&&") : K("||");
        NodeP l = pattern(dd(A(pat, "leftPattern")), v, p);
        NodeP r = pattern(dd(A(pat, "rightPattern")), v, p);
        return mk_binary(op, l, r, ir::BOOL, p);
    }
    if (veq(kv, "DiscardPattern")) return mk_lit(Val(true), ir::BOOL, p);
    if (veq(kv, "RecursivePattern")) {
        if (is_truthy(G(pat, "deconstructionSubpatterns"))) {
            throw Refuse{K("pattern"), K("a positional (deconstructing) pattern")};
        }
        TypeP mt = ty(G(pat, "matchedType"));
        Val ds = G(pat, "declaredSymbol");
        Val subsv = G(pat, "propertySubpatterns");
        py::list<Val> subs = is_truthy(subsv) ? items_of(subsv) : py::list<Val>();
        if (!subs.empty() && !is_truthy(ds) && !simple_(v)) {
            throw Refuse{K("pattern"), K("a property pattern on an expression with effects")};
        }
        NodeP test, obj;
        if (mt->kind != K("any") && !ir::same(mt, type_of(v))) {
            NodeP binding = is_truthy(ds) ? NodeP(mk_local(renamed(sym(ds)), mt, nullptr, py::list<py::str>(), p)) : NodeP();
            test = mk_instanceof(v, mt, binding, p);
            if (is_truthy(ds)) {
                auto nm = make_expr<ir::Name>(p, mt);
                nm->sym = renamed(sym(ds));
                obj = nm;
            } else {
                obj = mk_cast(v, mt, p);
            }
        } else {
            test = need_type(type_of(v))->kind == K("class") ? NodeP(mk_binary(K("!="), v, mk_lit(Val(), ir::NULL_, p), ir::BOOL, p))
                                                              : NodeP();
            obj = v;
        }
        for (const Val& spv : subs) {
            Dict sp = dg(spv);
            Dict m = or_empty(G(sp, "member"));
            if (!veq(G(m, "k"), "PropertyReference") && !veq(G(m, "k"), "FieldReference")) {
                throw Refuse{K("pattern"), K("a nested property pattern")};
            }
            Val msv = G(m, "property");
            if (!is_truthy(msv)) msv = G(m, "field");
            SymP ms = sym(msv);
            auto val = make_expr<ir::Select>(p, need(ms)->type);
            val->obj = obj;
            val->sym = ms;
            NodeP one_ = pattern(dd(A(sp, "pattern")), val, p);
            test = !test ? one_ : NodeP(mk_binary(K("&&"), test, one_, ir::BOOL, p));
        }
        return test ? test : NodeP(mk_lit(Val(true), ir::BOOL, p));
    }
    throw Refuse{K("pattern"), K("the pattern ") + pstr(kv)};
}

NodeP Lowering::switch_expr(const Dict& n, const TypeP& t, const Pos& p) {
    NodeP v = expr(dd(A(n, "value")));
    Val armsv = G(n, "arms", Val(Val::List()));
    py::list<Val> arms = items_of(armsv);
    auto pat_kind = [&](const Val& a) { return A(dd(A(dd(a), "pattern")), "k"); };
    bool all_simple = true;
    for (const Val& a : arms) {   // all(...) stops at the first arm that fails
        if (!(vin(pat_kind(a), {"ConstantPattern", "DiscardPattern"}) && G(dd(a), "guard").is_none())) {
            all_simple = false;
            break;
        }
    }
    bool constants = false;
    if (all_simple && !arms.empty() && veq(pat_kind(arms[arms.size() - 1]), "DiscardPattern")) {
        bool any_null = false;
        for (const Val& a : arms) {   // any(...) stops at the first null constant
            if (veq(pat_kind(a), "ConstantPattern") && is_null_(expr(dd(A(dd(A(dd(a), "pattern")), "value"))))) {
                any_null = true;
                break;
            }
        }
        constants = !any_null;
    }
    if (constants) {
        py::list<NodeP> cases;
        guard += 1;
        for (const Val& av : arms) {
            Dict a = dd(av);
            bool default_flag = veq(pat_kind(av), "DiscardPattern");
            py::list<NodeP> labels;
            if (!default_flag) labels.append(expr(dd(A(dd(A(a, "pattern")), "value"))));
            auto y = make<ir::Yield>(pos(a));
            y->expr = expr(dd(A(a, "value")));
            py::list<NodeP> body;
            body.append(y);
            auto cs = make<ir::Case>(pos(a));
            cs->labels = labels;
            cs->default_ = default_flag;
            cs->body = body;
            cases.append(cs);
        }
        guard -= 1;
        auto se = make_expr<ir::SwitchExpr>(p, t);
        se->expr = v;
        se->cases = cases;
        return se;
    }
    if (!simple_(v)) v = temp(type_of(v), v, n);
    if (arms.empty() || !veq(pat_kind(arms[arms.size() - 1]), "DiscardPattern") ||
        !G(dd(arms[arms.size() - 1]), "guard").is_none()) {
        throw Refuse{K("switch-expression"), K("a switch expression with no `_` arm (it throws when none matches)")};
    }
    // the arms become one expression, so a name two arms both bind is given a number in the later ones
    py::set<py::str> bound;
    std::vector<py::dict<py::str, py::str>> renames;
    for (int64_t i = 0; i < arms.size(); ++i) {
        py::dict<py::str, py::str> r;
        for (const py::str& x : bindings_(A(dd(arms[i]), "pattern"))) {
            if (bound.contains(x)) r.set(x, x + py::to_str(i + 1));
            bound.add(x);
        }
        renames.push_back(r);
    }
    guard += 1;
    py::dict<py::str, py::str> saved = rename;
    auto with_rename = [&](const py::dict<py::str, py::str>& r) {
        py::dict<py::str, py::str> d = saved.copy();
        d.update(r);
        return d;
    };
    NodeP out;
    {
        auto restore = finally_([&] {
            guard -= 1;
            rename = saved;
        });
        rename = with_rename(renames.back());
        out = expr(dd(A(dd(arms[arms.size() - 1]), "value")));
        for (int64_t i = arms.size() - 2; i >= 0; --i) {
            Dict a = dd(arms[i]);
            rename = with_rename(renames[size_t(i)]);
            NodeP test = pattern(dd(A(a, "pattern")), v, pos(a));
            if (!G(a, "guard").is_none()) test = mk_binary(K("&&"), test, expr(dd(A(a, "guard"))), ir::BOOL, pos(a));
            auto c = make_expr<ir::Cond>(pos(a), t);
            c->cond = test;
            c->then = expr(dd(A(a, "value")));
            c->orelse = out;
            out = c;
        }
    }
    return out;
}

// -------------------------------------------------------------------------------------- helpers
NodeP concat_(const NodeP& left, const NodeP& right, const TypeP& t, const Pos& p) {
    py::list<NodeP> parts;
    for (const NodeP& x : {left, right}) {
        if (auto c = as_node<ir::Concat>(x)) {
            parts.extend(*c->parts);
        } else {
            parts.append(x);
        }
    }
    auto c = make_expr<ir::Concat>(p, t);
    c->parts = parts;
    return c;
}

py::list<py::str> bindings_(const Val& pat) {
    py::list<py::str> out;
    std::function<void(const Val&)> walk = [&](const Val& n) {
        if (holds<Dict>(n)) {
            const Dict& d = as<Dict>(n);
            Val k = G(d, "k");
            if ((veq(k, "DeclarationPattern") || veq(k, "RecursivePattern")) && is_truthy(G(d, "declaredSymbol"))) {
                out.append(str_val(A(dg(A(d, "declaredSymbol")), "name")));
            }
            for (const Val& v : d.values()) walk(v);
        } else if (holds<Val::List>(n)) {
            for (const Val& v : as<Val::List>(n)) walk(v);
        }
    };
    walk(pat);
    return out;
}

bool is_null_(const NodeP& e) {
    auto l = as_node<ir::Lit>(e);
    if (!l || !l->value.is_none()) return false;
    if (!l->type) return true;
    const py::str& kd = l->type->kind;
    return kd == K("null") || kd == K("class") || kd == K("any") || kd == K("typevar");
}

bool simple_(const NodeP& e) {
    if (is_a<ir::Name>(e) || is_a<ir::This>(e) || is_a<ir::Lit>(e) || is_a<ir::TypeName>(e)) return true;
    if (auto s = as_node<ir::Select>(e)) {
        if (s->sym && s->sym->kind == K("field")) return simple_(s->obj);
    }
    return false;
}

bool effects_(const NodeP& e) {
    for (const NodeP& x : ir::walk(e)) {
        if (is_a<ir::Call>(x) || is_a<ir::New>(x) || is_a<ir::Assign>(x)) return true;
        if (auto u = as_node<ir::Unary>(x)) {
            if (u->op && (*u->op == K("++") || *u->op == K("--"))) return true;
        }
    }
    return false;
}

bool implicit_body_(const Dict& b) {
    if (!veq(G(b, "k"), "Block")) return false;
    py::list<Val> ops = L(b, "operations");
    return is_truthy(G(b, "implicit", Val(false))) && ops.size() == 1 && is_truthy(G(dg(ops[0]), "implicit", Val(false)));
}

int64_t prec_(const NodeP& e) {
    if (auto b = as_node<ir::Binary>(e)) return (b->op && PREC_.contains(*b->op)) ? PREC_.at(*b->op) : 0;
    if (is_a<ir::Concat>(e)) return 13;
    if (is_a<ir::Cond>(e)) return 3;
    if (is_a<ir::Assign>(e) || is_a<ir::Lambda>(e)) return 2;
    if (is_a<ir::InstanceOf>(e)) return 11;
    if (is_a<ir::Unary>(e) || is_a<ir::Cast>(e)) {
        auto u = as_node<ir::Unary>(e);
        return (u && u->postfix && *u->postfix) ? 16 : 15;
    }
    return 16;
}

NodeP group(const NodeP& node) {
    auto wrap = [](const NodeP& child, int64_t need_) -> NodeP {
        auto e = std::dynamic_pointer_cast<ir::Expr>(child);
        if (e && !is_a<ir::Paren>(child) && prec_(child) < need_) {
            auto pr = make_expr<ir::Paren>(Pos{e->line, e->col}, e->type);
            pr->x = child;
            return pr;
        }
        return child;
    };
    for (const NodeP& n : ir::walk(node)) {
        if (auto b = as_node<ir::Binary>(n)) {
            int64_t p = (b->op && PREC_.contains(*b->op)) ? PREC_.at(*b->op) : 0;
            bool coalesce = b->op == std::optional<py::str>(K("??"));
            b->left = wrap(b->left, !coalesce ? p : p + 1);
            b->right = wrap(b->right, !coalesce ? p + 1 : p);
        } else if (auto c = as_node<ir::Concat>(n)) {
            py::list<NodeP> parts;
            int64_t i = 0;
            for (const NodeP& x : *c->parts) {
                parts.append(wrap(x, i == 0 ? 13 : 14));
                ++i;
            }
            c->parts = parts;
        } else if (auto u = as_node<ir::Unary>(n)) {
            u->x = wrap(u->x, 15);
        } else if (auto ca = as_node<ir::Cast>(n)) {
            ca->x = wrap(ca->x, 15);
        } else if (auto io = as_node<ir::InstanceOf>(n)) {
            io->x = wrap(io->x, 12);
        } else if (auto cd = as_node<ir::Cond>(n)) {
            cd->cond = wrap(cd->cond, 4);
            cd->then = wrap(cd->then, 3);
            cd->orelse = wrap(cd->orelse, 3);
        } else if (auto s = as_node<ir::Select>(n)) {
            if (std::dynamic_pointer_cast<ir::Expr>(s->obj)) s->obj = wrap(s->obj, 16);
        } else if (auto cl = as_node<ir::Call>(n)) {
            if (std::dynamic_pointer_cast<ir::Expr>(cl->target)) cl->target = wrap(cl->target, 16);
        } else if (auto ix = as_node<ir::Index>(n)) {
            ix->arr = wrap(ix->arr, 16);
        } else if (auto ln = as_node<ir::Length>(n)) {
            ln->arr = wrap(ln->arr, 16);
        }
    }
    return node;
}

py::ref<ir::Unit> lower(const Dict& tree, const std::optional<py::str>& path) {
    Lowering L(path);
    return std::dynamic_pointer_cast<ir::Unit>(group(L.unit(tree)));
}

std::tuple<py::ref<ir::Unit>, py::list<std::tuple<int64_t, std::optional<py::str>, py::str>>> lower_with_refusals(
    const Dict& tree, const std::optional<py::str>& path) {
    Lowering L(path);
    py::ref<ir::Unit> u = std::dynamic_pointer_cast<ir::Unit>(group(L.unit(tree)));
    return {u, L.refused};
}

}  // namespace codebreaker::translate::frontends::csharp::lower

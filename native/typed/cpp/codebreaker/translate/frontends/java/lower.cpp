// codebreaker/translate/frontends/java/lower.cpp -- typed C++ port of
// codebreaker.translate.frontends.java.lower (see lower.hpp).

#include "codebreaker/translate/frontends/java/lower.hpp"

#include "json.hpp"

namespace codebreaker::translate::frontends::java::lower {

using ir::Nodes;

namespace {

py::str K(const char* s) { return py::str(s); }

// ---------------------------------------------------------------------------------------------- tables
// Java's primitives in the IR's language-neutral kinds
const py::dict<py::str, py::str> PRIM = {{K("boolean"), K("bool")}, {K("byte"), K("i8")},   {K("short"), K("i16")},
                                         {K("char"), K("u16")},     {K("int"), K("i32")},   {K("long"), K("i64")},
                                         {K("float"), K("f32")},    {K("double"), K("f64")}, {K("void"), K("void")}};
const py::list<py::str> LOCALS = {K("LOCAL_VARIABLE"), K("PARAMETER"), K("EXCEPTION_PARAMETER"), K("RESOURCE_VARIABLE"),
                                  K("BINDING_VARIABLE")};
const py::list<py::str> TYPES = {K("CLASS"), K("INTERFACE"), K("ENUM"), K("RECORD"), K("ANNOTATION_TYPE"), K("TYPE_PARAMETER")};
const py::dict<py::str, py::str> KIND = {{K("CLASS"), K("class")},   {K("INTERFACE"), K("interface")}, {K("ENUM"), K("enum")},
                                         {K("RECORD"), K("record")}, {K("ANNOTATION_TYPE"), K("annotation")}};

bool is_in(const py::list<py::str>& xs, const py::str& x) { return xs.contains(x); }

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
Val vget(const Dict& d, const char* key) { return d.get(K(key), Val()); }          // d.get(key)
Val vat(const Dict& d, const char* key) { return d.at(K(key)); }                   // d[key]: KeyError

const Dict& as_dict(const Val& v) {
    if (!holds<Dict>(v)) type_error("not a JSON object: ", v);
    return as<Dict>(v);
}

// d[key] as a node
Dict node_at(const Dict& d, const char* key) { return as_dict(vat(d, key)); }

// d[key] as a node whose first use is `.get(...)`: a null raises AttributeError (not TypeError, as for a subscript)
Dict node_at_get(const Dict& d, const char* key) {
    Val v = vat(d, key);
    if (v.is_none()) throw py::AttributeError("'NoneType' object has no attribute 'get'");
    return as_dict(v);
}

// d.get(key) read for its truth and then as a node: an empty Dict for None, absent, or any false value
Dict opt_node(const Dict& d, const char* key) {
    Val v = vget(d, key);
    if (!is_truthy(v)) return Dict();
    return as_dict(v);
}

// d.get(key, []) iterated: its nodes
py::list<Dict> nodes_at(const Dict& d, const char* key) {
    py::list<Dict> out;
    if (!has(d, key)) return out;
    Val v = vat(d, key);
    if (!holds<Val::List>(v)) type_error("not a list: ", v);
    for (const Val& x : as<Val::List>(v)) out.append(as_dict(x));
    return out;
}

// d.get(key) as an optional str (None for absent and null)
std::optional<py::str> opt_str(const Dict& d, const char* key) {
    Val v = vget(d, key);
    if (v.is_none()) return std::nullopt;
    if (!holds<py::str>(v)) type_error("not a str: ", v);
    return as<py::str>(v);
}

// d[key] as a str
py::str str_at(const Dict& d, const char* key) {
    Val v = vat(d, key);
    if (!holds<py::str>(v)) type_error("not a str: ", v);
    return as<py::str>(v);
}

// the str of a JSON value that must be one
py::str str_in(const Val& v) {
    if (!holds<py::str>(v)) type_error("not a str: ", v);
    return as<py::str>(v);
}

// d[key] as a str, None for a null (KeyError when absent)
std::optional<py::str> nullable_at(const Dict& d, const char* key) {
    Val v = vat(d, key);
    if (v.is_none()) return std::nullopt;
    return str_in(v);
}

// n['k'], a node's kind: a str, or None for a null (KeyError when absent)
std::optional<py::str> kind_at(const Dict& n) {
    Val v = vat(n, "k");
    if (v.is_none()) return std::nullopt;
    return str_in(v);
}

// `k in _KIND`
bool in_kinds(const std::optional<py::str>& k) { return k && KIND.contains(*k); }

// _KIND[n['k']]
py::str kind_name(const Dict& n) {
    std::optional<py::str> k = kind_at(n);
    if (!k) throw py::KeyError("None");
    return KIND.at(*k);
}

// n['key'] as a node, an empty Dict for a null
Dict node_or_none_at(const Dict& n, const char* key) {
    Val v = vat(n, key);
    return v.is_none() ? Dict() : as_dict(v);
}

// d.get(key) == s
bool is_str(const Dict& d, const char* key, const char* s) {
    Val v = vget(d, key);
    return holds<py::str>(v) && as<py::str>(v) == K(s);
}

// d.get(key, False), for a flag
bool flag(const Dict& d, const char* key) { return is_truthy(vget(d, key)); }

// n.get('l', 0) / n.get('c', 0)
int64_t number_of(const Val& v) {
    if (v.is_none()) return 0;
    if (holds<int64_t>(v)) return as<int64_t>(v);
    if (holds<bool>(v)) return as<bool>(v) ? 1 : 0;
    type_error("not a number: ", v);
}

// str.startswith(prefix, start)
bool starts_at(const py::str& s, const char* prefix, int64_t i) {
    if (i > s.size()) return false;
    return s.slice(i, s.size()).startswith(K(prefix));
}

// a Type with no details
TypeP mk_type(const char* kind, const std::optional<py::str>& name = std::nullopt, const py::list<TypeP>& args = py::list<TypeP>()) {
    return std::make_shared<ir::Type>(K(kind), name, args);
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

py::ref<ir::Unsupported> unsupported(const std::optional<py::str>& kind, const std::optional<py::str>& raw, const TypeP& t, const Pos& p) {
    auto u = make_expr<ir::Unsupported>(p, t);
    u->kind = kind;
    u->raw = raw;
    return u;
}

}  // namespace

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

Dict load_tree(const py::str& text) { return as_dict(from_v(py::json_parse(text.utf8()))); }

// -------------------------------------------------------------------------------------- type strings
TypeP parse_type(const std::optional<py::str>& s, const py::list<py::str>& typevars) {
    if (!s) return ir::ANY;
    py::set<py::str> tv;
    for (const py::str& x : typevars) tv.add(x);
    auto [t, rest] = type_(s->strip(), 0, tv);
    (void)rest;
    return t;
}

std::tuple<TypeP, int64_t> type_(const py::str& s, int64_t i, const py::set<py::str>& tv) {
    const int64_t n = s.size();
    while (i < n && s.at_(i) == U' ') i += 1;
    if (starts_at(s, "<nulltype>", i) || starts_at(s, "<nullType>", i)) return {ir::NULL_, i + 10};
    if (starts_at(s, "<any>", i)) return {ir::ANY, i + 5};
    if (starts_at(s, "?", i)) {
        i += 1;
        static const std::pair<const char*, const char*> WORDS[] = {{" extends ", "extends"}, {" super ", "super"}};
        for (const auto& wm : WORDS) {
            if (starts_at(s, wm.first, i)) {
                auto [b, after] = type_(s, i + int64_t(std::strlen(wm.first)), tv);
                auto w = mk_type("wildcard");
                w->bound = std::make_tuple(K(wm.second), b);
                return {w, after};
            }
        }
        return {mk_type("wildcard"), i};
    }
    int64_t j = i;
    while (j < n) {
        char32_t c = s.at_(j);
        if (c == U'<' || c == U'>' || c == U',' || c == U'[' || c == U']' || c == U'&') break;
        j += 1;
    }
    py::str name = s.slice(i, j).strip();
    if (name.startswith(K("capture#"))) name = K("java.lang.Object");   // a captured wildcard: its erasure
    py::list<TypeP> args;
    if (j < n && s.at_(j) == U'<') {
        j += 1;
        while (true) {
            auto [a, after] = type_(s, j, tv);
            j = after;
            args.append(a);
            while (j < n && s.at_(j) == U' ') j += 1;
            if (j < n && s.at_(j) == U',') {
                j += 1;
                continue;
            }
            j += 1;   // '>'
            break;
        }
    }
    TypeP t;
    if (PRIM.contains(name)) {
        t = name != K("void") ? mk_type(PRIM.at(name).utf8().c_str()) : ir::VOID;
    } else if (tv.contains(name) ||
               (!name.contains(K(".")) && name.size() <= 2 && name.slice(int64_t(0), int64_t(1)).isupper() && args.empty())) {
        t = mk_type("typevar", name);
    } else {
        t = mk_type("class", name, args);
    }
    while (starts_at(s, "[]", j)) {
        t = ir::array_of(t);
        j += 2;
    }
    if (j < n && s.at_(j) == U'&') {   // an intersection: its first bound
        auto [other, after] = type_(s, j + 1, tv);
        (void)other;
        j = after;
    }
    return {t, j};
}

// -------------------------------------------------------------------------------------- symbols
namespace {

// parse_type(v) of a JSON value: a str, or None
TypeP parse_type_val(const Val& v) {
    if (v.is_none()) return ir::ANY;
    if (!holds<py::str>(v)) throw py::AttributeError("object has no attribute 'strip': " + v.repr().utf8());
    return parse_type(as<py::str>(v));
}

}  // namespace

SymP sym(const Val& d) {
    if (!is_truthy(d)) return nullptr;
    const Dict& dd = as_dict(d);
    Val kv = vat(dd, "kind");
    if (kv.is_none()) throw py::AttributeError("'NoneType' object has no attribute 'lower'");
    py::str k = str_in(kv);
    py::str kind;
    if (is_in(LOCALS, k)) kind = k == K("PARAMETER") ? K("param") : K("local");
    else if (k == K("FIELD")) kind = K("field");
    else if (k == K("ENUM_CONSTANT")) kind = K("enumconst");
    else if (k == K("METHOD")) kind = K("method");
    else if (k == K("CONSTRUCTOR")) kind = K("ctor");
    else if (is_in(TYPES, k)) kind = k == K("TYPE_PARAMETER") ? K("typevar") : K("class");
    else if (k == K("PACKAGE")) kind = K("package");
    else kind = k.lower();
    if (has(dd, "params")) {
        py::str name = str_at(dd, "name");
        std::optional<py::str> owner = opt_str(dd, "owner");
        bool is_static = flag(dd, "static");
        py::list<TypeP> params;
        Val ps = vat(dd, "params");
        if (!holds<Val::List>(ps)) type_error("not a list: ", ps);
        for (const Val& p : as<Val::List>(ps)) params.append(parse_type_val(p));
        TypeP ret = parse_type_val(vget(dd, "ret"));
        return std::make_shared<ir::Sym>(kind, name, owner, is_static, nullptr, params, ret, flag(dd, "varargs"));
    }
    Val q = vget(dd, "qname");
    py::str name = is_truthy(q) ? str_in(q) : str_at(dd, "name");
    return std::make_shared<ir::Sym>(kind, name, opt_str(dd, "owner"), flag(dd, "static"), parse_type_val(vget(dd, "type")));
}

// -------------------------------------------------------------------------------------- the lowering
Pos Lowering::pos(const Dict& n) const { return Pos{number_of(vget(n, "l")), number_of(vget(n, "c"))}; }

TypeP Lowering::ty(const Dict& n) const { return parse_type_val(vget(n, "$type")); }

// ---- declarations
py::ref<ir::Unit> Lowering::unit(const Dict& n) {
    py::str pkg = is_truthy(vget(n, "packageName")) ? qualified(node_at(n, "packageName")) : K("");
    py::list<py::str> imports;
    for (const Dict& i : nodes_at(n, "imports")) imports.append(qualified(opt_node(i, "qualifiedIdentifier")));
    py::list<NodeP> types;
    for (const Dict& t : nodes_at(n, "typeDecls")) {
        if (in_kinds(kind_at(t))) types.append(decl(t, std::nullopt));
    }
    auto u = make<ir::Unit>(pos(n));
    u->path = path;
    u->package = pkg;
    u->imports = imports;
    u->types = types;
    return u;
}

py::str Lowering::qualified(const Dict& n) {
    if (n.empty()) return K("");
    std::optional<py::str> k = kind_at(n);
    if (k == K("IDENTIFIER")) return str_at(n, "name");
    if (k == K("MEMBER_SELECT")) return qualified(node_or_none_at(n, "expression")) + K(".") + str_at(n, "identifier");
    return K("");
}

py::list<py::str> Lowering::mods(const Dict& n) {
    py::list<py::str> out;
    if (!has(n, "flags")) return out;
    Val v = vat(n, "flags");
    if (!holds<Val::List>(v)) type_error("not a list: ", v);
    for (const Val& x : as<Val::List>(v)) out.append(str_in(x));
    return out;
}

py::ref<ir::ClassDecl> Lowering::decl(const Dict& n, const std::optional<py::str>& outer) {
    SymP s = sym(vget(n, "$sym"));
    auto c = make<ir::ClassDecl>(pos(n));
    c->name = opt_str(n, "simpleName");
    c->qname = s ? std::optional<py::str>(s->name) : opt_str(n, "simpleName");
    c->kind = kind_name(n);
    c->mods = mods(opt_node(n, "modifiers"));
    py::list<py::str> typeparams;
    for (const Dict& tp : nodes_at(n, "typeParameters")) typeparams.append(str_at(tp, "name"));
    c->typeparams = typeparams;
    Dict ext = opt_node(n, "extendsClause");
    c->extends = !ext.empty() ? ty(ext) : nullptr;
    py::list<TypeP> implements;
    for (const Dict& i : nodes_at(n, "implementsClause")) implements.append(ty(i));
    c->implements = implements;
    c->members = py::list<NodeP>();
    c->outer = outer;
    for (const Dict& m : nodes_at(n, "members")) c->members->append(member(m, c, n));
    if (*c->kind == K("record")) record_members(c);
    return c;
}

NodeP Lowering::member(const Dict& m, const py::ref<ir::ClassDecl>& cls, const Dict& raw_cls) {
    std::optional<py::str> k = kind_at(m);
    if (in_kinds(k)) return decl(m, cls->qname);
    if (k == K("VARIABLE")) {
        SymP s = sym(vget(m, "$sym"));
        if (s && s->kind == K("enumconst")) {
            Dict init = opt_node(m, "initializer");
            auto e = make<ir::EnumConst>(pos(m));
            e->sym = s;
            py::list<NodeP> args;
            for (const Dict& a : nodes_at(init, "arguments")) args.append(expr(a));
            e->args = args;
            e->body = is_truthy(vget(init, "classBody")) ? NodeP(decl(node_at(init, "classBody"), cls->qname)) : NodeP();
            e->ctor = sym(vget(init, "$sym"));
            return e;
        }
        auto f = make<ir::FieldDecl>(pos(m));
        f->sym = s;
        f->type = ty(m);
        Dict init = opt_node(m, "initializer");
        f->init = !init.empty() ? expr(init) : NodeP();
        f->mods = mods(opt_node(m, "modifiers"));
        return f;
    }
    if (k == K("METHOD")) return method(m, raw_cls);
    if (k == K("BLOCK")) {   // an initialiser block
        bool is_static = flag(m, "static");
        auto d = make<ir::MethodDecl>(pos(m));
        d->sym = nullptr;
        d->name = is_static ? K("<clinit>") : K("<init-block>");
        d->kind = is_static ? K("static_init") : K("init");
        d->params = py::list<NodeP>();
        d->ret = ir::VOID;
        d->body = NodeP(block(m));
        d->mods = py::list<py::str>();
        d->throws = py::list<TypeP>();
        d->typeparams = py::list<py::str>();
        return d;
    }
    return unsupported(k, std::nullopt, ir::ANY, pos(m));
}

py::ref<ir::MethodDecl> Lowering::method(const Dict& m, const Dict& raw_cls) {
    SymP s = sym(vget(m, "$sym"));
    bool ctor = is_str(m, "name", "<init>");
    py::list<NodeP> params;
    for (const Dict& p : nodes_at(m, "parameters")) {
        auto prm = make<ir::Param>(pos(p));
        prm->sym = sym(vget(p, "$sym"));
        prm->type = ty(p);
        prm->mods = mods(opt_node(p, "modifiers"));
        params.append(prm);
    }
    Dict bodyn = opt_node(m, "body");
    NodeP body = !bodyn.empty() ? NodeP(block(bodyn, ctor)) : NodeP();
    py::list<py::str> md = mods(opt_node(m, "modifiers"));
    // javac adds a default constructor to a class that declares none, at the class's own position
    if (ctor && same_val(vget(m, "l"), vget(raw_cls, "l")) && same_val(vget(m, "c"), vget(raw_cls, "c"))) {
        md.append(K("IMPLICIT"));
    }
    auto d = make<ir::MethodDecl>(pos(m));
    d->sym = s;
    d->name = opt_str(m, "name");
    d->kind = ctor ? K("ctor") : K("method");
    d->params = params;
    d->ret = (s && s->ret) ? s->ret : ir::VOID;
    d->body = body;
    d->mods = md;
    py::list<TypeP> throws;
    for (const Dict& t : nodes_at(m, "throws")) throws.append(ty(t));
    d->throws = throws;
    py::list<py::str> typeparams;
    for (const Dict& tp : nodes_at(m, "typeParameters")) typeparams.append(str_at(tp, "name"));
    d->typeparams = typeparams;
    return d;
}

// ---- statements
py::ref<ir::Block> Lowering::block(const Dict& n, bool ctor_body) {
    py::list<NodeP> stmts;
    for (const Dict& st : nodes_at(n, "statements")) {
        NodeP s = stmt(st);
        auto cc = as_node<ir::CtorCall>(s);
        if (ctor_body && cc && same_val(vget(st, "l"), vget(n, "l")) && same_val(vget(st, "c"), vget(n, "c"))) {
            cc->kind = *cc->kind + K(":implicit");   // javac's own super() at the body's position
        }
        stmts.append(s);
    }
    auto b = make<ir::Block>(pos(n));
    b->stmts = stmts;
    return b;
}

NodeP Lowering::stmt(const Dict& n) {
    std::optional<py::str> k = kind_at(n);
    Pos p = pos(n);
    if (k == K("BLOCK")) return block(n);
    if (k == K("VARIABLE")) {
        auto l = make<ir::Local>(p);
        l->sym = sym(vget(n, "$sym"));
        l->type = ty(n);
        Dict init = opt_node(n, "initializer");
        l->init = !init.empty() ? expr(init) : NodeP();
        l->mods = mods(opt_node(n, "modifiers"));
        return l;
    }
    if (k == K("EXPRESSION_STATEMENT")) {
        Dict e = node_at(n, "expression");
        Dict sel = kind_at(e) == K("METHOD_INVOCATION") ? opt_node(e, "methodSelect") : Dict();
        if (!sel.empty() && kind_at(sel) == K("IDENTIFIER") &&
            (str_at(sel, "name") == K("this") || str_at(sel, "name") == K("super"))) {
            auto c = make<ir::CtorCall>(p);
            c->kind = str_at(sel, "name");
            c->sym = sym(vget(e, "$sym"));
            py::list<NodeP> args;
            for (const Dict& a : nodes_at(e, "arguments")) args.append(expr(a));
            c->args = args;
            c->target = nullptr;
            return c;
        }
        if (!sel.empty() && kind_at(sel) == K("MEMBER_SELECT") &&
            (nullable_at(sel, "identifier") == K("this") || nullable_at(sel, "identifier") == K("super")) &&
            is_str(opt_node(sel, "$sym"), "kind", "CONSTRUCTOR")) {
            auto c = make<ir::CtorCall>(p);
            c->kind = str_at(sel, "identifier");
            c->sym = sym(vget(e, "$sym"));
            py::list<NodeP> args;
            for (const Dict& a : nodes_at(e, "arguments")) args.append(expr(a));
            c->args = args;
            c->target = expr(node_at(sel, "expression"));
            return c;
        }
        auto es = make<ir::ExprStmt>(p);
        es->expr = expr(e);
        return es;
    }
    if (k == K("IF")) {
        auto s = make<ir::If>(p);
        s->cond = expr(node_at(n, "condition"));
        s->then = stmt(node_at(n, "thenStatement"));
        Dict els = opt_node(n, "elseStatement");
        s->orelse = !els.empty() ? stmt(els) : NodeP();
        return s;
    }
    if (k == K("WHILE_LOOP")) {
        auto s = make<ir::While>(p);
        s->cond = expr(node_at(n, "condition"));
        s->body = stmt(node_at(n, "statement"));
        return s;
    }
    if (k == K("DO_WHILE_LOOP")) {
        auto s = make<ir::DoWhile>(p);
        s->body = stmt(node_at(n, "statement"));
        s->cond = expr(node_at(n, "condition"));
        return s;
    }
    if (k == K("FOR_LOOP")) {
        auto s = make<ir::For>(p);
        py::list<NodeP> init;
        for (const Dict& i : nodes_at(n, "initializer")) init.append(stmt(i));
        s->init = init;
        Dict cond = opt_node(n, "condition");
        s->cond = !cond.empty() ? expr(cond) : NodeP();
        py::list<NodeP> update;
        for (const Dict& u : nodes_at(n, "update")) update.append(expr(node_at(u, "expression")));
        s->update = update;
        s->body = stmt(node_at(n, "statement"));
        return s;
    }
    if (k == K("ENHANCED_FOR_LOOP")) {
        Dict v = node_at_get(n, "variable");
        auto var = make<ir::Local>(pos(v));
        var->sym = sym(vget(v, "$sym"));
        var->type = ty(v);
        var->init = nullptr;
        var->mods = mods(opt_node(v, "modifiers"));
        auto s = make<ir::ForEach>(p);
        s->var = var;
        s->iterable = expr(node_at(n, "expression"));
        s->body = stmt(node_at(n, "statement"));
        return s;
    }
    if (k == K("SWITCH")) {
        auto s = make<ir::Switch>(p);
        s->expr = expr(node_at(n, "expression"));
        py::list<NodeP> cases;
        for (const Dict& c : nodes_at(n, "cases")) cases.append(case_(c));
        s->cases = cases;
        return s;
    }
    if (k == K("BREAK")) {
        auto s = make<ir::Break>(p);
        s->label = opt_str(n, "label");
        return s;
    }
    if (k == K("CONTINUE")) {
        auto s = make<ir::Continue>(p);
        s->label = opt_str(n, "label");
        return s;
    }
    if (k == K("RETURN")) {
        auto s = make<ir::Return>(p);
        Dict e = opt_node(n, "expression");
        s->expr = !e.empty() ? expr(e) : NodeP();
        return s;
    }
    if (k == K("THROW")) {
        auto s = make<ir::Throw>(p);
        s->expr = expr(node_at(n, "expression"));
        return s;
    }
    if (k == K("TRY")) {
        py::list<NodeP> catches;
        for (const Dict& c : nodes_at(n, "catches")) {
            Dict v = node_at_get(c, "parameter");
            Dict tt = opt_node(v, "type");
            py::list<TypeP> types;
            if (is_str(tt, "k", "UNION_TYPE")) {
                for (const Dict& a : nodes_at(tt, "typeAlternatives")) types.append(ty(a));
            } else {
                types.append(ty(v));
            }
            auto ct = make<ir::Catch>(pos(c));
            ct->sym = sym(vget(v, "$sym"));
            ct->types = types;
            ct->body = block(node_at(c, "block"));
            catches.append(ct);
        }
        auto s = make<ir::Try>(p);
        py::list<NodeP> resources;
        for (const Dict& r : nodes_at(n, "resources")) {
            if (kind_at(r) == K("VARIABLE")) {
                resources.append(stmt(r));
            } else {
                auto es = make<ir::ExprStmt>(pos(r));
                es->expr = expr(r);
                resources.append(es);
            }
        }
        s->resources = resources;
        s->body = block(node_at(n, "block"));
        s->catches = catches;
        Dict fin = opt_node(n, "finallyBlock");
        s->final = !fin.empty() ? NodeP(block(fin)) : NodeP();
        return s;
    }
    if (k == K("LABELED_STATEMENT")) {
        auto s = make<ir::Labeled>(p);
        s->label = opt_str(n, "label");
        s->body = stmt(node_at(n, "statement"));
        return s;
    }
    if (k == K("SYNCHRONIZED")) {
        auto s = make<ir::Sync>(p);
        s->lock = expr(node_at(n, "expression"));
        s->body = block(node_at(n, "block"));
        return s;
    }
    if (k == K("EMPTY_STATEMENT")) return make<ir::Empty>(p);
    if (in_kinds(k)) {
        auto s = make<ir::LocalClass>(p);
        s->decl = decl(n, std::nullopt);
        return s;
    }
    if (k == K("ASSERT")) {
        auto s = make<ir::Assert>(p);
        s->cond = expr(node_at(n, "condition"));
        Dict detail = opt_node(n, "detail");
        s->detail = !detail.empty() ? expr(detail) : NodeP();
        return s;
    }
    if (k == K("YIELD")) {
        auto s = make<ir::Yield>(p);
        s->expr = expr(node_at(n, "value"));
        return s;
    }
    auto es = make<ir::ExprStmt>(p);
    es->expr = unsupported(k, std::nullopt, ir::ANY, p);
    return es;
}

py::ref<ir::Case> Lowering::case_(const Dict& c, bool in_expr) {
    py::list<NodeP> labels;
    bool default_ = false;
    for (const Dict& lab : nodes_at(c, "labels")) {
        std::optional<py::str> lk = kind_at(lab);
        if (lk == K("DEFAULT_CASE_LABEL")) {
            default_ = true;
        } else if (lk == K("CONSTANT_CASE_LABEL")) {
            labels.append(expr(node_at(lab, "constantExpression")));
        } else {
            labels.append(unsupported(lk, std::nullopt, ir::ANY, pos(lab)));
        }
    }
    if (!is_truthy(vget(c, "labels"))) {   // older javac: expression(s) or none
        py::list<Dict> exprs;
        if (is_truthy(vget(c, "expressions"))) {
            exprs = nodes_at(c, "expressions");
        } else if (is_truthy(vget(c, "expression"))) {
            exprs.append(node_at(c, "expression"));
        }
        labels = py::list<NodeP>();
        for (const Dict& e : exprs) labels.append(expr(e));
        default_ = exprs.empty();
    }
    py::list<NodeP> body;
    if (is_str(c, "caseKind", "RULE")) {   // an arrow arm: a value, a block or a throw
        Dict b = opt_node(c, "body");
        if (!has(c, "body") || vget(c, "body").is_none()) {
            // (no body: nothing)
        } else {
            std::optional<py::str> bk = kind_at(b);
            if (bk == K("BLOCK") || bk == K("THROW") || bk == K("EXPRESSION_STATEMENT")) {
                body.append(stmt(b));
            } else if (in_expr) {
                auto y = make<ir::Yield>(pos(b));
                y->expr = expr(b);
                body.append(y);
            } else {
                auto es = make<ir::ExprStmt>(pos(b));
                es->expr = expr(b);
                body.append(es);
            }
        }
        if (!in_expr) {
            auto br = make<ir::Break>(pos(c));   // an arrow arm does not fall through
            br->label = std::nullopt;
            body.append(br);
        }
    } else {
        for (const Dict& s : nodes_at(c, "statements")) body.append(stmt(s));
    }
    auto cs = make<ir::Case>(pos(c));
    cs->labels = labels;
    cs->default_ = default_;
    cs->body = body;
    return cs;
}

// ---- expressions
NodeP Lowering::expr(const Dict& n) {
    std::optional<py::str> kind = kind_at(n);
    Pos p = pos(n);
    TypeP t = ty(n);
    if (!kind) throw py::AttributeError("'NoneType' object has no attribute 'endswith'");
    py::str k = *kind;
    if (k.endswith(K("_LITERAL"))) return literal(n, t, p);
    if (k == K("PARENTHESIZED")) {
        auto e = make_expr<ir::Paren>(p, t);
        e->x = expr(node_at(n, "expression"));
        return e;
    }
    if (k == K("IDENTIFIER")) return identifier(n, t, p);
    if (k == K("MEMBER_SELECT")) return member_select(n, t, p);
    if (k == K("METHOD_INVOCATION")) return call(n, t, p);
    if (k == K("NEW_CLASS")) {
        py::ref<ir::ClassDecl> body;
        if (is_truthy(vget(n, "classBody"))) {
            Dict cb = node_at(n, "classBody").copy();
            cb.set(K("k"), Val("CLASS"));
            body = decl(cb, std::nullopt);
        }
        Dict ident = opt_node(n, "identifier");
        if (body && !body->extends && !ident.empty()) body->extends = ty(ident);   // what an anonymous class extends or implements
        auto e = make_expr<ir::New>(p, t);
        e->sym = sym(vget(n, "$sym"));
        py::list<NodeP> args;
        for (const Dict& a : nodes_at(n, "arguments")) args.append(expr(a));
        e->args = args;
        e->body = body;
        Dict outer = opt_node(n, "enclosingExpression");
        e->outer = !outer.empty() ? expr(outer) : NodeP();
        return e;
    }
    if (k == K("NEW_ARRAY")) {
        TypeP elem = t->kind == K("array") ? std::get<1>(t->dims()) : ir::ANY;
        auto e = make_expr<ir::NewArray>(p, t);
        e->elem = elem;
        py::list<NodeP> dims;
        for (const Dict& d : nodes_at(n, "dimensions")) dims.append(expr(d));
        e->dims = dims;
        if (!vget(n, "initializers").is_none()) {
            py::list<NodeP> init;
            for (const Dict& x : nodes_at(n, "initializers")) init.append(expr(x));
            e->init = init;
        }
        return e;
    }
    if (k == K("ARRAY_ACCESS")) {
        auto e = make_expr<ir::Index>(p, t);
        e->arr = expr(node_at(n, "expression"));
        e->index = expr(node_at(n, "index"));
        return e;
    }
    if (ir::OPERATORS.contains(k)) {
        NodeP l = expr(node_at(n, "leftOperand"));
        NodeP r = expr(node_at(n, "rightOperand"));
        if (k == K("PLUS") && t->kind == K("class") && t->name == K("java.lang.String")) {
            py::list<NodeP> parts;
            for (const NodeP& x : {l, r}) {
                auto cx = as_node<ir::Concat>(x);
                if (cx) {
                    if (!cx->parts) throw py::TypeError("'NoneType' object is not iterable");
                    parts.extend(*cx->parts);
                } else {
                    parts.append(x);
                }
            }
            auto e = make_expr<ir::Concat>(p, t);
            e->parts = parts;
            return e;
        }
        auto e = make_expr<ir::Binary>(p, t);
        e->op = ir::OPERATORS.at(k);
        e->left = l;
        e->right = r;
        return e;
    }
    if (ir::UNARY.contains(k)) {
        auto [op, post] = ir::UNARY.at(k);
        auto e = make_expr<ir::Unary>(p, t);
        e->op = op;
        e->x = expr(node_at(n, "expression"));
        e->postfix = post;
        return e;
    }
    if (k == K("ASSIGNMENT")) {
        auto e = make_expr<ir::Assign>(p, t);
        e->op = std::nullopt;
        e->target = expr(node_at(n, "variable"));
        e->value = expr(node_at(n, "expression"));
        return e;
    }
    if (k.endswith(K("_ASSIGNMENT"))) {
        py::str base = k.slice(0, k.size() - int64_t(std::strlen("_ASSIGNMENT")));
        if (ir::OPERATORS.contains(base) && !ir::OPERATORS.at(base).empty()) {
            auto e = make_expr<ir::Assign>(p, t);
            e->op = ir::OPERATORS.at(base);
            e->target = expr(node_at(n, "variable"));
            e->value = expr(node_at(n, "expression"));
            return e;
        }
    }
    if (k == K("CONDITIONAL_EXPRESSION")) {
        auto e = make_expr<ir::Cond>(p, t);
        e->cond = expr(node_at(n, "condition"));
        e->then = expr(node_at(n, "trueExpression"));
        e->orelse = expr(node_at(n, "falseExpression"));
        return e;
    }
    if (k == K("TYPE_CAST")) {
        auto e = make_expr<ir::Cast>(p, t);
        e->x = expr(node_at(n, "expression"));
        return e;
    }
    if (k == K("INSTANCE_OF")) {
        Dict pat = opt_node(n, "pattern");
        Dict test = opt_node(n, "type");
        if (test.empty()) test = opt_node(pat, "variable");
        if (test.empty()) test = opt_node(pat, "var");
        NodeP binding;
        if (is_str(pat, "k", "BINDING_PATTERN")) {   // `x instanceof T v`: v is bound where it matched
            Dict v = opt_node(pat, "variable");
            if (v.empty()) v = node_at(pat, "var");
            auto b = make<ir::Local>(pos(v));
            b->sym = sym(vget(v, "$sym"));
            b->type = ty(v);
            b->init = nullptr;
            b->mods = mods(opt_node(v, "modifiers"));
            binding = b;
            if (test.empty()) test = v;
        } else if (!pat.empty()) {   // a record pattern (deconstruction): not yet
            std::optional<py::str> pk = has(pat, "k") ? opt_str(pat, "k") : std::optional<py::str>(K("?"));
            if (!pk) throw py::TypeError("can only concatenate str (not \"NoneType\") to str");
            return unsupported(K("INSTANCE_OF:") + *pk, std::nullopt, t, p);
        }
        auto e = make_expr<ir::InstanceOf>(p, t);
        e->x = expr(node_at(n, "expression"));
        e->test = !test.empty() ? ty(test) : ir::ANY;
        e->binding = binding;
        return e;
    }
    if (k == K("SWITCH_EXPRESSION")) {
        auto e = make_expr<ir::SwitchExpr>(p, t);
        e->expr = expr(node_at(n, "expression"));
        py::list<NodeP> cases;
        for (const Dict& c : nodes_at(n, "cases")) cases.append(case_(c, true));
        e->cases = cases;
        return e;
    }
    if (k == K("LAMBDA_EXPRESSION")) {
        py::list<NodeP> params;
        for (const Dict& v : nodes_at(n, "parameters")) {
            auto prm = make<ir::Param>(pos(v));
            prm->sym = sym(vget(v, "$sym"));
            prm->type = ty(v);
            prm->mods = py::list<py::str>();
            params.append(prm);
        }
        Dict b = node_at(n, "body");
        NodeP body = kind_at(b) == K("BLOCK") ? NodeP(block(b)) : expr(b);
        auto e = make_expr<ir::Lambda>(p, t);
        e->params = params;
        e->body = body;
        return e;
    }
    if (k == K("MEMBER_REFERENCE")) {
        auto e = make_expr<ir::MethodRef>(p, t);
        e->target = expr(node_at(n, "qualifierExpression"));
        e->sym = sym(vget(n, "$sym"));
        e->mode = is_str(n, "mode", "NEW") ? K("new") : K("invoke");
        return e;
    }
    return unsupported(k, std::nullopt, t, p);
}

py::ref<ir::Lit> Lowering::literal(const Dict& n, const TypeP& t, const Pos& p) {
    Val v = vget(n, "value");
    std::optional<py::str> k = kind_at(n);
    if (k == K("NULL_LITERAL")) {
        v = Val();
    } else if (k == K("FLOAT_LITERAL") || k == K("DOUBLE_LITERAL")) {   // float(v)
        if (holds<py::str>(v)) v = Val(py::float_from_str(as<py::str>(v)));
        else if (holds<int64_t>(v)) v = Val(double(as<int64_t>(v)));
        else if (holds<bool>(v)) v = Val(as<bool>(v) ? 1.0 : 0.0);
        else if (!holds<double>(v)) throw py::TypeError("float() argument must be a string or a real number");
    } else if (k == K("INT_LITERAL") || k == K("LONG_LITERAL")) {   // int(v)
        if (holds<py::str>(v)) v = Val(py::int_parse(as<py::str>(v), 10));
        else if (holds<double>(v)) v = Val(py::int_from_float(as<double>(v)));
        else if (holds<bool>(v)) v = Val(int64_t(as<bool>(v) ? 1 : 0));
        else if (!holds<int64_t>(v)) throw py::TypeError("int() argument must be a string or a real number");
    } else if (k == K("BOOLEAN_LITERAL")) {
        v = Val(is_truthy(v));
    }
    auto e = make_expr<ir::Lit>(p, t);
    e->value = v;
    return e;
}

NodeP Lowering::identifier(const Dict& n, const TypeP& t, const Pos& p) {
    std::optional<py::str> name = opt_str(n, "name");
    if (name == K("this")) {
        auto e = make_expr<ir::This>(p, t);
        e->qualifier = nullptr;
        return e;
    }
    if (name == K("super")) {
        auto e = make_expr<ir::Super>(p, t);
        e->qualifier = nullptr;
        return e;
    }
    SymP s = sym(vget(n, "$sym"));
    if (!s) return unsupported(K("IDENTIFIER?"), name, t, p);
    if (s->kind == K("local") || s->kind == K("param")) {
        auto e = make_expr<ir::Name>(p, t);
        e->sym = s;
        return e;
    }
    if (s->kind == K("field") || s->kind == K("enumconst")) {   // a bare field name: on this, or on its class
        TypeP owner = mk_type("class", s->owner);
        NodeP obj;
        if (s->static_) {
            obj = make_expr<ir::TypeName>(p, owner);
        } else {
            auto th = make_expr<ir::This>(p, owner);
            th->qualifier = nullptr;
            obj = th;
        }
        auto e = make_expr<ir::Select>(p, t);
        e->obj = obj;
        e->sym = s;
        return e;
    }
    if (s->kind == K("class") || s->kind == K("typevar") || s->kind == K("package")) {
        return make_expr<ir::TypeName>(p, t->kind != K("any") ? t : mk_type("class", s->name));
    }
    return unsupported(K("IDENTIFIER:") + s->kind, name, t, p);
}

NodeP Lowering::member_select(const Dict& n, const TypeP& t, const Pos& p) {
    py::str ident = str_at(n, "identifier");
    SymP s = sym(vget(n, "$sym"));
    Dict inner = node_at(n, "expression");
    if (ident == K("class")) {
        auto e = make_expr<ir::ClassLit>(p, t);
        e->of = parse_type_val(vget(inner, "$type"));
        return e;
    }
    if (ident == K("length") && s && s->owner == K("Array")) {
        auto e = make_expr<ir::Length>(p, t);
        e->arr = expr(inner);
        return e;
    }
    if (ident == K("this")) {
        auto e = make_expr<ir::This>(p, t);
        e->qualifier = parse_type_val(vget(inner, "$type"));
        return e;
    }
    if (ident == K("super")) {
        auto e = make_expr<ir::Super>(p, t);
        e->qualifier = parse_type_val(vget(inner, "$type"));
        return e;
    }
    if (!s) return unsupported(K("MEMBER_SELECT?"), ident, t, p);
    if (s->kind == K("class") || s->kind == K("package") || s->kind == K("typevar")) {
        return make_expr<ir::TypeName>(p, t->kind != K("any") ? t : mk_type("class", s->name));
    }
    if (s->kind == K("field") || s->kind == K("enumconst")) {
        auto e = make_expr<ir::Select>(p, t);
        e->obj = expr(inner);
        e->sym = s;
        return e;
    }
    return unsupported(K("MEMBER_SELECT:") + s->kind, ident, t, p);
}

NodeP Lowering::call(const Dict& n, const TypeP& t, const Pos& p) {
    SymP s = sym(vget(n, "$sym"));
    Dict sel = node_at(n, "methodSelect");
    py::list<NodeP> args;
    for (const Dict& a : nodes_at(n, "arguments")) args.append(expr(a));
    NodeP target;
    std::optional<py::str> sk = kind_at(sel);
    if (sk == K("IDENTIFIER")) {
        if (s && s->kind == K("method") && s->static_) {
            target = make_expr<ir::TypeName>(pos(sel), mk_type("class", s->owner));
        }   // else: this class's own method, implicit this
    } else if (sk == K("MEMBER_SELECT")) {
        target = expr(node_at(sel, "expression"));
    } else {
        if (!sk) throw py::TypeError("can only concatenate str (not \"NoneType\") to str");
        return unsupported(K("METHOD_INVOCATION:") + *sk, std::nullopt, t, p);
    }
    auto e = make_expr<ir::Call>(p, t);
    e->target = target;
    e->sym = s;
    e->args = args;
    e->typeargs = py::list<TypeP>();
    return e;
}

// -------------------------------------------------------------------------------------- records
// What javac writes for a record only when it lowers it (after the tree TreeDump sees): the canonical
// constructor's field assignments, an accessor per component, equals, hashCode and toString -- made here as
// IR, as java.lang.runtime.ObjectMethods defines them, each marked GENERATED.
namespace {

const TypeP STRING_ = mk_type("class", K("java.lang.String"));
const TypeP OBJECT_ = mk_type("class", K("java.lang.Object"));
const py::dict<py::str, py::str> HASH_OWNER = {
    {K("i32"), K("java.lang.Integer")}, {K("i64"), K("java.lang.Long")},    {K("i16"), K("java.lang.Short")},
    {K("i8"), K("java.lang.Byte")},     {K("u16"), K("java.lang.Character")}, {K("bool"), K("java.lang.Boolean")},
    {K("f32"), K("java.lang.Float")},   {K("f64"), K("java.lang.Double")}};

py::str required(const std::optional<py::str>& s) {
    if (!s) throw py::TypeError("can only concatenate str (not \"NoneType\") to str");
    return *s;
}

}  // namespace

void record_members(const py::ref<ir::ClassDecl>& c) {
    py::list<py::ref<ir::FieldDecl>> comps;
    for (const NodeP& m : *c->members) {
        auto f = as_node<ir::FieldDecl>(m);
        if (f && f->sym && !f->sym->static_) comps.append(f);
    }
    TypeP me = mk_type("class", c->qname);
    const Pos pos{c->line, c->col};

    auto lit = [&](const Val& v, const TypeP& t) -> NodeP {
        auto e = make_expr<ir::Lit>(pos, t);
        e->value = v;
        return e;
    };
    auto name_of = [&](const SymP& s, const TypeP& t) -> NodeP {
        auto e = make_expr<ir::Name>(pos, t);
        e->sym = s;
        return e;
    };
    auto this_field = [&](const py::ref<ir::FieldDecl>& f) -> NodeP {
        auto th = make_expr<ir::This>(pos, me);
        th->qualifier = nullptr;
        auto e = make_expr<ir::Select>(pos, f->type);
        e->obj = th;
        e->sym = f->sym;
        return e;
    };
    auto static_call = [&](const py::str& owner, const py::str& name, const py::list<TypeP>& params, const TypeP& ret,
                           const py::list<NodeP>& args) -> NodeP {
        auto s = std::make_shared<ir::Sym>(K("method"), name, std::optional<py::str>(owner), true, nullptr, params, ret);
        auto e = make_expr<ir::Call>(pos, ret);
        e->target = make_expr<ir::TypeName>(pos, mk_type("class", owner));
        e->sym = s;
        e->args = args;
        e->typeargs = py::list<TypeP>();
        return e;
    };
    auto method = [&](const py::str& name, const py::list<NodeP>& params, const TypeP& ret, const py::list<NodeP>& stmts) {
        py::list<TypeP> ptypes;
        for (const NodeP& p : params) ptypes.append(as_node<ir::Param>(p)->type);
        auto s = std::make_shared<ir::Sym>(K("method"), name, c->qname, false, nullptr, ptypes, ret);
        auto b = make<ir::Block>(pos);
        b->stmts = stmts;
        auto d = make<ir::MethodDecl>(pos);
        d->sym = s;
        d->name = name;
        d->kind = K("method");
        d->params = params;
        d->ret = ret;
        d->body = NodeP(b);
        d->mods = py::list<py::str>{K("public"), K("GENERATED")};
        d->throws = py::list<TypeP>();
        d->typeparams = py::list<py::str>();
        c->members->append(d);
    };
    auto declared = [&](const py::str& name, int64_t nparams) {
        for (const NodeP& m : *c->members) {
            auto md = as_node<ir::MethodDecl>(m);
            if (md && md->name == name && (md->params ? md->params->size() : 0) == nparams) return true;
        }
        return false;
    };
    auto expr_stmt = [&](const NodeP& e) -> NodeP {
        auto s = make<ir::ExprStmt>(pos);
        s->expr = e;
        return s;
    };
    auto ret_stmt = [&](const NodeP& e) -> NodeP {
        auto s = make<ir::Return>(pos);
        s->expr = e;
        return s;
    };

    // the canonical constructor (javac's own, or a compact one) assigns each component from its parameter
    py::list<TypeP> want;
    for (const auto& f : comps) want.append(f->type);
    for (const NodeP& m : *c->members) {
        auto md = as_node<ir::MethodDecl>(m);
        if (!md || md->kind != K("ctor")) continue;
        py::list<TypeP> have;
        for (const NodeP& p : *md->params) have.append(as_node<ir::Param>(p)->type);
        if (have.size() != want.size()) continue;
        bool equal = true;
        for (int64_t i = 0; i < have.size(); ++i) {
            if (!ir::same(have[i], want[i])) equal = false;
        }
        if (!equal) continue;
        NodeP body = ir::node_of(md->body);
        bool assigned = false;
        if (body) {
            for (const NodeP& x : ir::walk(body)) {
                auto a = as_node<ir::Assign>(x);
                if (a && is_a<ir::Select>(a->target) && is_a<ir::This>(as_node<ir::Select>(a->target)->obj)) assigned = true;
            }
        }
        if (body && !assigned) {
            auto blk = as_node<ir::Block>(body);
            for (int64_t i = 0; i < std::min(comps.size(), md->params->size()); ++i) {
                auto p = as_node<ir::Param>((*md->params)[i]);
                auto a = make_expr<ir::Assign>(pos, comps[i]->type);
                a->op = std::nullopt;
                a->target = this_field(comps[i]);
                a->overflow = std::nullopt;
                a->value = name_of(p->sym, p->type);
                blk->stmts->append(expr_stmt(a));
            }
        }
    }
    for (const auto& f : comps) {   // x() for each component x
        if (!declared(f->sym->name, 0)) method(f->sym->name, py::list<NodeP>(), f->type, py::list<NodeP>{ret_stmt(this_field(f))});
    }
    if (!declared(K("toString"), 0)) {   // Point[x=1, y=2]
        py::list<NodeP> parts{lit(Val(required(c->name) + K("[")), STRING_)};
        for (int64_t i = 0; i < comps.size(); ++i) {
            const auto& f = comps[i];
            parts.append(lit(Val(K(i == 0 ? "" : ", ") + f->sym->name + K("=")), STRING_));
            parts.append(this_field(f));
        }
        parts.append(lit(Val(K("]")), STRING_));
        auto cat = make_expr<ir::Concat>(pos, STRING_);
        cat->parts = parts;
        method(K("toString"), py::list<NodeP>(), STRING_, py::list<NodeP>{ret_stmt(cat)});
    }
    if (!declared(K("hashCode"), 0)) {   // h = 31 * h + hash(component), from 0
        SymP h = std::make_shared<ir::Sym>(K("local"), K("h"), std::optional<py::str>(required(c->qname) + K(".hashCode")), false, ir::I32);
        auto decl_h = make<ir::Local>(pos);
        decl_h->sym = h;
        decl_h->type = ir::I32;
        decl_h->init = lit(Val(int64_t(0)), ir::I32);
        decl_h->mods = py::list<py::str>();
        py::list<NodeP> stmts{decl_h};
        for (const auto& f : comps) {
            bool prim = HASH_OWNER.contains(f->type->kind);
            NodeP call = static_call(prim ? HASH_OWNER.at(f->type->kind) : K("java.util.Objects"), K("hashCode"),
                                     py::list<TypeP>{prim ? f->type : OBJECT_}, ir::I32, py::list<NodeP>{this_field(f)});
            auto times = make_expr<ir::Binary>(pos, ir::I32);
            times->op = K("*");
            times->left = lit(Val(int64_t(31)), ir::I32);
            times->right = name_of(h, ir::I32);
            auto sum = make_expr<ir::Binary>(pos, ir::I32);
            sum->op = K("+");
            sum->left = times;
            sum->right = call;
            auto a = make_expr<ir::Assign>(pos, ir::I32);
            a->op = std::nullopt;
            a->target = name_of(h, ir::I32);
            a->overflow = std::nullopt;
            a->value = sum;
            stmts.append(expr_stmt(a));
        }
        stmts.append(ret_stmt(name_of(h, ir::I32)));
        method(K("hashCode"), py::list<NodeP>(), ir::I32, stmts);
    }
    if (!declared(K("equals"), 1)) {   // the same record type, every component equal
        SymP o = std::make_shared<ir::Sym>(K("param"), K("o"), std::optional<py::str>(required(c->qname) + K(".equals")), false, OBJECT_);
        SymP that = std::make_shared<ir::Sym>(K("local"), K("that"), std::optional<py::str>(required(c->qname) + K(".equals")), false, me);
        auto isit = make_expr<ir::InstanceOf>(pos, ir::BOOL);
        isit->x = name_of(o, OBJECT_);
        isit->test = me;
        isit->binding = nullptr;
        auto paren = make_expr<ir::Paren>(pos, ir::BOOL);
        paren->x = isit;
        auto neg = make_expr<ir::Unary>(pos, ir::BOOL);
        neg->op = K("!");
        neg->x = paren;
        neg->postfix = false;
        auto early = make<ir::Return>(pos);
        early->expr = lit(Val(false), ir::BOOL);
        auto guard = make<ir::If>(pos);
        guard->cond = neg;
        guard->then = NodeP(early);
        guard->orelse = NodeP();
        auto cast = make_expr<ir::Cast>(pos, me);
        cast->x = name_of(o, OBJECT_);
        auto decl_that = make<ir::Local>(pos);
        decl_that->sym = that;
        decl_that->type = me;
        decl_that->init = cast;
        decl_that->mods = py::list<py::str>();
        py::list<NodeP> stmts{guard, decl_that};
        NodeP cond;
        for (const auto& f : comps) {
            NodeP mine = this_field(f);
            auto theirs = make_expr<ir::Select>(pos, f->type);
            theirs->obj = name_of(that, me);
            theirs->sym = f->sym;
            NodeP eq;
            if (f->type->kind == K("f32") || f->type->kind == K("f64")) {   // Float.compare / Double.compare == 0 (NaN equals NaN)
                auto cmp = make_expr<ir::Binary>(pos, ir::BOOL);
                cmp->op = K("==");
                cmp->left = static_call(HASH_OWNER.at(f->type->kind), K("compare"), py::list<TypeP>{f->type, f->type}, ir::I32,
                                        py::list<NodeP>{mine, NodeP(theirs)});
                cmp->right = lit(Val(int64_t(0)), ir::I32);
                eq = cmp;
            } else if (f->type->is_prim()) {
                auto cmp = make_expr<ir::Binary>(pos, ir::BOOL);
                cmp->op = K("==");
                cmp->left = mine;
                cmp->right = theirs;
                eq = cmp;
            } else {
                eq = static_call(K("java.util.Objects"), K("equals"), py::list<TypeP>{OBJECT_, OBJECT_}, ir::BOOL,
                                 py::list<NodeP>{mine, NodeP(theirs)});
            }
            if (!cond) {
                cond = eq;
            } else {
                auto both = make_expr<ir::Binary>(pos, ir::BOOL);
                both->op = K("&&");
                both->left = cond;
                both->right = eq;
                cond = both;
            }
        }
        stmts.append(ret_stmt(cond ? cond : lit(Val(true), ir::BOOL)));
        auto prm = make<ir::Param>(pos);
        prm->sym = o;
        prm->type = OBJECT_;
        prm->mods = py::list<py::str>();
        method(K("equals"), py::list<NodeP>{NodeP(prm)}, ir::BOOL, stmts);
    }
}

py::ref<ir::Unit> lower(const Dict& tree, const std::optional<py::str>& path) { return Lowering(path).unit(tree); }

}  // namespace codebreaker::translate::frontends::java::lower

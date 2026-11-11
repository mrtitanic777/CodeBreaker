// codebreaker/translate/frontends/python/lower.cpp -- typed C++ from
// codebreaker.translate.frontends.python.lower. See lower.hpp.

#include "codebreaker/translate/frontends/python/lower.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <map>

#include "py_json.hpp"

namespace codebreaker::translate::frontends::python::lower {

using pyengine::AnyNode;
using program::at;
using program::Big;
using program::Tup;
using program::TupItem;

const char* const PY = "py";

namespace {

// ---------------------------------------------------------------------------------------- small helpers
Str S(const std::string& s) { return Str(s); }
Str S(const char* s) { return Str(s); }
Str i2s(int64_t v) { return py::to_str(v); }
Str bs(const P::Bound& b) { return b ? Str(b->str()) : Str("None"); }
bool eq(const Str& s, const char* lit) { return program::lit_eq(s, lit); }
bool starts(const Str& s, const char* lit) { return program::str_starts(s, lit); }

// the CPython class name of an expression / statement, lowered ("name", "functiondef")
std::string lower_ascii(std::string s) {
    for (char& c : s) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    }
    return s;
}
std::string tname(const A::Expr* e) { return pyengine::type_name(AnyNode(e)); }
std::string tname(const A::Stmt* s) { return pyengine::type_name(AnyNode(s)); }

// the key `lines[...]` of a str that is the Python's `repr(s)` (a KeyError's text)
Str krepr(const Str& s) { return program::repr_str(s); }

// the TupItem accessors of a resolution (r[i], 1-based)
std::optional<Big> big_item(const Tup& r, size_t i) {
    if (i < 1 || i > r.items.size()) throw py::IndexError("tuple index out of range");
    if (const Big* b = std::get_if<Big>(&r.items[i - 1])) return *b;
    return std::nullopt;
}
bool none_item_is(const Tup& r, size_t i) {
    if (i < 1 || i > r.items.size()) throw py::IndexError("tuple index out of range");
    return std::holds_alternative<std::monostate>(r.items[i - 1]);
}
std::optional<Str> opt_str_item(const Tup& r, size_t i) {
    if (i < 1 || i > r.items.size()) throw py::IndexError("tuple index out of range");
    if (const Str* s = std::get_if<Str>(&r.items[i - 1])) return *s;
    return std::nullopt;
}

template <class N> py::ref<N> mk(int64_t line = 0) {
    auto n = std::make_shared<N>();
    n->line = line;
    return n;
}

TypeP mkt(const char* kind) { return std::make_shared<ir::Type>(Str(kind)); }
TypeP clst(const Str& name, const std::vector<TypeP>& args = {}, const ir::Dict& info = ir::Dict()) {
    return std::make_shared<ir::Type>(Str("class"), std::optional<Str>(name), py::list<TypeP>(args), nullptr, std::nullopt, info);
}
ir::Dict info1(const char* k, bool v) {
    ir::Dict d;
    d.set(Str(k), Val(v));
    return d;
}
ir::Dict info2(const char* k1, bool v1, const char* k2, bool v2) {
    ir::Dict d;
    d.set(Str(k1), Val(v1));
    d.set(Str(k2), Val(v2));
    return d;
}
// Type.info.get(key) is truthy
bool info_true(const TypeP& t, const char* key) {
    if (!t) return false;
    if (!t->info.contains(Str(key))) return false;
    const Val& v = t->info.at(Str(key));
    if (const bool* b = std::get_if<bool>(&v.v)) return *b;
    return !v.is_none();
}
SymP msym(const char* kind, const Str& name, const std::optional<Str>& owner = std::nullopt, bool static_ = false,
          const TypeP& type = nullptr, const std::vector<TypeP>& params = {}, const TypeP& ret = nullptr) {
    return std::make_shared<ir::Sym>(Str(kind), name, owner, static_, type, py::list<TypeP>(params), ret);
}
bool tsame(const TypeP& a, const TypeP& b) { return ir::same(a, b); }
bool kind_is(const TypeP& t, const char* k) { return t && eq(t->kind, k); }
bool class_is(const TypeP& t, const char* name) { return t && eq(t->kind, "class") && t->name && eq(*t->name, name); }
bool tk(const T& t, const char* k) { return program::k_is(t, k); }

template <class N> py::ref<N> cast_to(const NodeP& n) { return std::dynamic_pointer_cast<N>(n); }
template <class N> bool is_a(const NodeP& n) { return std::dynamic_pointer_cast<N>(n) != nullptr; }

// an expression's type (every IR expression has one; None for the odd node that has none)
const TypeP& type_of(const NodeP& e) {
    static const TypeP none_;
    auto* x = dynamic_cast<ir::Expr*>(e.get());
    if (!x) return none_;
    return x->type;
}
void set_type(const NodeP& e, const TypeP& t) {
    auto* x = dynamic_cast<ir::Expr*>(e.get());
    if (x) x->type = t;
}

py::list<NodeP> nl(const std::vector<NodeP>& v) { return py::list<NodeP>(v); }
py::list<Str> strs(std::initializer_list<const char*> v) {
    py::list<Str> out;
    for (const char* s : v) out.append(Str(s));
    return out;
}

// ---- IR node makers (what the Python writes as ir.Foo(slot=...) -- every slot named there is set, the rest None)
NodeP mk_local(const SymP& sym, const TypeP& type, const NodeP& init, const py::list<Str>& mods, int64_t line = 0) {
    auto n = mk<ir::Local>(line);
    n->sym = sym;
    n->type = type;
    n->init = init;
    n->mods = mods;
    return n;
}
NodeP mk_name(const SymP& sym, const TypeP& type) {
    auto n = mk<ir::Name>();
    n->sym = sym;
    n->type = type;
    return n;
}
NodeP mk_expr_stmt(const NodeP& e, int64_t line = 0) {
    auto n = mk<ir::ExprStmt>(line);
    n->expr = e;
    return n;
}
NodeP mk_assign(const std::optional<Str>& op, const NodeP& target, const NodeP& value, const TypeP& type) {
    auto n = mk<ir::Assign>();
    n->op = op;
    n->target = target;
    n->value = value;
    n->type = type;
    return n;
}
NodeP mk_binary(const Str& op, const NodeP& l, const NodeP& r, const TypeP& type, int64_t line = 0) {
    auto n = mk<ir::Binary>(line);
    n->op = op;
    n->left = l;
    n->right = r;
    n->type = type;
    return n;
}
NodeP mk_unary(const Str& op, const NodeP& x, bool postfix, const TypeP& type, int64_t line = 0) {
    auto n = mk<ir::Unary>(line);
    n->op = op;
    n->x = x;
    n->postfix = postfix;
    n->type = type;
    return n;
}
NodeP mk_cast(const NodeP& x, const TypeP& type) {
    auto n = mk<ir::Cast>();
    n->x = x;
    n->type = type;
    return n;
}
NodeP mk_paren(const NodeP& x, const TypeP& type) {
    auto n = mk<ir::Paren>();
    n->x = x;
    n->type = type;
    return n;
}
py::ref<ir::Block> mk_block(const std::vector<NodeP>& stmts) {
    auto n = mk<ir::Block>();
    n->stmts = nl(stmts);
    return n;
}
NodeP mk_select(const NodeP& obj, const SymP& sym, const TypeP& type, int64_t line = 0) {
    auto n = mk<ir::Select>(line);
    n->obj = obj;
    n->sym = sym;
    n->type = type;
    return n;
}
NodeP mk_typename(const TypeP& type) {
    auto n = mk<ir::TypeName>();
    n->type = type;
    return n;
}
NodeP mk_index(const NodeP& arr, const NodeP& index, const TypeP& type, int64_t line = 0) {
    auto n = mk<ir::Index>(line);
    n->arr = arr;
    n->index = index;
    n->type = type;
    return n;
}
NodeP mk_concat(const std::vector<NodeP>& parts, const TypeP& type, int64_t line = 0) {
    auto n = mk<ir::Concat>(line);
    n->parts = nl(parts);
    n->type = type;
    return n;
}
NodeP mk_return(const NodeP& e, int64_t line = 0) {
    auto n = mk<ir::Return>(line);
    n->expr = e;
    return n;
}
NodeP mk_throw(const NodeP& e, int64_t line = 0) {
    auto n = mk<ir::Throw>(line);
    n->expr = e;
    return n;
}
NodeP mk_new(const SymP& sym, const std::vector<NodeP>& args, const TypeP& type, const std::optional<Str>& memory,
             int64_t line = 0) {
    auto n = mk<ir::New>(line);
    n->sym = sym;
    n->args = nl(args);
    n->type = type;
    n->memory = memory;
    return n;
}
NodeP mk_call(const NodeP& target, const SymP& sym, const std::vector<NodeP>& args, const TypeP& type, int64_t line = 0) {
    auto n = mk<ir::Call>(line);
    n->target = target;
    n->sym = sym;
    n->args = nl(args);
    n->typeargs = py::list<TypeP>();
    n->type = type;
    return n;
}
NodeP mk_break(int64_t line = 0) { return mk<ir::Break>(line); }
NodeP mk_continue(int64_t line = 0) { return mk<ir::Continue>(line); }

// the value of an optional str as a Val (None for the empty optional)
Val val_of(const std::optional<Str>& s) { return s ? Val(*s) : Val(); }

// -------------------------------------------------------------------------------------------- Python helpers
// a % b with Python's sign (b != 0)
int64_t pymod(int64_t a, int64_t b) {
    int64_t r = a % b;
    if (r != 0 && ((r < 0) != (b < 0))) r += b;
    return r;
}
// Big -> int64_t for a value the lowering keeps in the IR
int64_t as_i64(const Big& b) {
    if (!b.fits_i64()) throw py::OverflowError("Python int too large to convert to C long");
    return b.i64();
}
// the (lo, hi) indices of a[lo:hi] over n items (Python's slice rules, step 1)
std::pair<int64_t, int64_t> slice_bounds(int64_t n, const std::optional<Big>& a, const std::optional<Big>& b) {
    auto clamp = [&](const std::optional<Big>& x, int64_t dflt) -> int64_t {
        if (!x) return dflt;
        if (*x < Big(0)) {
            Big y = *x + Big(n);
            return y < Big(0) ? 0 : y.i64();
        }
        return *x > Big(n) ? n : x->i64();
    };
    int64_t lo = clamp(a, 0), hi = clamp(b, n);
    if (hi < lo) hi = lo;
    return {lo, hi};
}

// json.dumps of a default's value (a Constant of the source): None, bool, int, float, str, or {"bytes": ...}
Str json_default(const A::Constant* d) {
    const A::ConstValue& v = d->value;
    std::string o;
    switch (v.kind) {
        case A::ConstValue::Kind::None: return Str("null");
        case A::ConstValue::Kind::True: return Str("true");
        case A::ConstValue::Kind::False: return Str("false");
        case A::ConstValue::Kind::Int: return S(v.text);
        case A::ConstValue::Kind::Float: {
            if (std::isnan(v.f)) return Str("NaN");
            if (std::isinf(v.f)) return Str(v.f > 0 ? "Infinity" : "-Infinity");
            return py::to_str(v.f);
        }
        case A::ConstValue::Kind::Str: {
            py::json::put(o, S(v.text));
            return S(o);
        }
        case A::ConstValue::Kind::Bytes: {
            std::u32string u;
            for (unsigned char c : v.text) u.push_back(c);
            py::json::put(o, Str(u));
            return Str("{\"bytes\": ") + S(o) + Str("}");
        }
        case A::ConstValue::Kind::Ellipsis: throw py::TypeError("Object of type ellipsis is not JSON serializable");
        case A::ConstValue::Kind::Complex: throw py::TypeError("Object of type complex is not JSON serializable");
    }
    return Str();
}

// a constant's text as `str(value)` where it is a str, bytes decoded as latin1
Str latin1(const std::string& bytes) {
    std::u32string u;
    for (unsigned char c : bytes) u.push_back(c);
    return Str(u);
}

// isinstance(st, ast.For) (not ast.AsyncFor)
const A::For* as_for(const A::Stmt* st) { return st && st->kind == A::StmtKind::For ? A::cast<A::For>(st) : nullptr; }

bool name_is(const A::Expr* e, const std::string& id) {
    auto* n = A::as<A::Name>(e);
    return n && n->id == id;
}
bool is_none_const(const A::Expr* e) {
    auto* c = A::as<A::Constant>(e);
    return c && c->value.kind == A::ConstValue::Kind::None;
}
// any Name of this id in the tree of `root`
bool mentions(AnyNode root, const std::string& id) {
    for (const AnyNode& x : pyengine::walk(root)) {
        if (const A::Expr* e = x.expr()) {
            if (name_is(e, id)) return true;
        }
    }
    return false;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- types
const TypeP& I64() {
    static const TypeP t = mkt("i64");
    return t;
}
const TypeP& BOOL() { return ir::BOOL; }
const TypeP& F64() {
    static const TypeP t = mkt("f64");
    return t;
}
const TypeP& VOID() { return ir::VOID; }
const TypeP& STR() {
    static const TypeP t = clst(Str("py.str"));
    return t;
}
const TypeP& BYTES() {
    static const TypeP t = clst(Str("py.bytes"));
    return t;
}

NodeP pyfn(const Str& name, const std::vector<NodeP>& args, const TypeP& t, const NodeP& target, int64_t line,
           const py::list<Val>& typeargs) {
    auto n = mk<ir::Call>(line);
    n->target = target;
    n->sym = msym("function", name, std::optional<Str>(Str(PY)));
    n->args = nl(args);
    if (typeargs.empty()) {
        n->typeargs = py::list<TypeP>();
    } else {
        n->typeargs = typeargs;
    }
    n->type = t;
    return n;
}

NodeP lit(const Val& v, const TypeP& t, const std::optional<Str>& text) {
    auto n = mk<ir::Lit>();
    n->value = v;
    n->text = text;
    n->type = t;
    return n;
}

namespace {
py::list<Val> targs(std::initializer_list<Val> v) {
    py::list<Val> out;
    for (const Val& x : v) out.append(x);
    return out;
}
}  // namespace

Str camel(const Str& s) {
    // re.sub(r'[^0-9A-Za-z]+', ' ', s).strip() then ''.join(w[:1].upper() + w[1:] for w in s.split()) or 'Record'
    std::u32string out;
    bool in_word = false;
    for (char32_t c : s.u32()) {
        bool alnum = (c >= U'0' && c <= U'9') || (c >= U'A' && c <= U'Z') || (c >= U'a' && c <= U'z');
        if (alnum) {
            if (!in_word && c >= U'a' && c <= U'z') c = c - U'a' + U'A';
            out.push_back(c);
            in_word = true;
        } else {
            in_word = false;
        }
    }
    return out.empty() ? Str("Record") : Str(out);
}

Str singular(const Str& w) {
    if (w.endswith(Str("ies"))) return w.slice(0, w.size() - 3) + "y";
    if (w.endswith(Str("sses")) || w.endswith(Str("xes"))) return w.slice(0, w.size() - 2);
    if (w.endswith(Str("s")) && !w.endswith(Str("ss"))) return w.slice(0, w.size() - 1);
    return w;
}

namespace {
const std::vector<const char*>& VERBS() {
    static const std::vector<const char*> v = {"parse_", "read_", "get_", "make_", "build_",
                                               "load_",  "collect_", "find_", "scan_", "compute_"};
    return v;
}
const std::set<Str>& GENERIC() {
    static const std::set<Str> s = {Str("out"), Str("r"),    Str("d"),   Str("e"),    Str("x"),    Str("h"),
                                    Str("s"),   Str("o"),    Str("v"),   Str("res"),  Str("result"), Str("info"),
                                    Str("rec"), Str("item"), Str("entry"), Str("m"),  Str("p"),    Str("n"),
                                    Str("it"),  Str("data"), Str("obj"), Str("tmp"),  Str("row"),  Str("c"),
                                    Str("f"),   Str("t")};
    return s;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- Records
Records::Records(Program& prog) : p(&prog) {
    std::map<Str, int64_t> used_count;
    fn_lines();
    std::map<int64_t, std::vector<int64_t>> members;
    for (size_t x = 1; x < prog.parent.size(); ++x) {
        members[prog.find(static_cast<int64_t>(x))].push_back(static_cast<int64_t>(x));
    }
    for (auto& kv : members) {
        int64_t s = kv.first;
        if (!prog.is_record(s)) continue;
        std::vector<int64_t>& ms = kv.second;
        std::sort(ms.begin(), ms.end());
        std::optional<int64_t> hm;
        for (int64_t m : ms) {            // the site in a function that returns the record
            Str mod("?");
            int64_t line = 0;
            const auto& w = prog.site_where[static_cast<size_t>(m)];
            if (w) {
                mod = std::get<0>(*w);
                line = std::get<1>(*w);
            }
            std::optional<Str> fq;
            auto fi = fn_lines_.find(mod);
            if (fi != fn_lines_.end()) {
                auto li = fi->second.find(line);
                if (li != fi->second.end()) fq = li->second;
            }
            if (fq) {
                T rt = prog.returns.contains(*fq) ? prog.returns.at(*fq) : P::BOTTOM();
                if (holds_(rt, s)) {
                    hm = m;
                    break;
                }
            }
        }
        if (!hm) hm = ms.front();
        home[s] = *hm;
        Str mod("?");
        int64_t line = 0;
        const auto& w = prog.site_where[static_cast<size_t>(*hm)];
        if (w) {
            mod = std::get<0>(*w);
            line = std::get<1>(*w);
        }
        std::optional<Str> fq;
        auto fi = fn_lines_.find(mod);
        if (fi != fn_lines_.end()) {
            auto li = fi->second.find(line);
            if (li != fi->second.end()) fq = li->second;
        }
        owner_fn[s] = fq;
        module[s] = mod;
        Str name = name_(s, fq);
        Str k = mod + Str("\x01") + name;
        auto uc = used_count.find(k);
        if (uc != used_count.end()) {
            uc->second += 1;
            name = name + i2s(uc->second);
        } else {
            used_count[k] = 1;
        }
        names[s] = name;
    }
}

void Records::fn_lines() {
    for (const auto& kv : p->fns.items()) {
        Fn* f = std::get<1>(kv);
        auto& d = fn_lines_[f->module->name];
        int64_t end = f->node->pos.end_lineno ? f->node->pos.end_lineno : f->node->pos.lineno;
        for (int64_t line = f->node->pos.lineno; line < end + 1; ++line) d.emplace(line, f->qname);   // setdefault
    }
    // a method's lines are the method's (the innermost: the shortest span)
    std::vector<Fn*> all;
    for (const auto& kv : p->fns.items()) all.push_back(std::get<1>(kv));
    std::stable_sort(all.begin(), all.end(), [](Fn* a, Fn* b) { return -a->lines < -b->lines; });
    for (Fn* f : all) {
        auto& d = fn_lines_.at(f->module->name);
        int64_t end = f->node->pos.end_lineno ? f->node->pos.end_lineno : f->node->pos.lineno;
        for (int64_t line = f->node->pos.lineno; line < end + 1; ++line) d[line] = f->qname;
    }
}

Str Records::name_(int64_t s, const std::optional<Str>& fq) {
    std::vector<Str> hints;
    if (p->hints.contains(s)) {
        for (const Str& h : p->hints.at(s)) {
            if (!GENERIC().count(h.lower()) && h.size() > 2) hints.push_back(h);
        }
    }
    std::optional<Str> stem;
    if (fq) {
        Str fname = program::last_part(*fq).lstrip(Str("_"));
        for (const char* v : VERBS()) {
            if (starts(fname, v)) fname = fname.slice_from(static_cast<int64_t>(std::string(v).size()));
        }
        if (!fname.empty() && !eq(fname, "_init__")) stem = camel(singular(fname));
        std::optional<T> ret;
        if (p->returns.contains(*fq)) ret = p->returns.at(*fq);
        bool flows_out = ret.has_value() && holds_(*ret, s);
        if (flows_out && stem) return *stem;
        if (stem && !hints.empty()) return *stem + camel(singular(hints[0]));
        if (stem && p->hints.contains(s) && !p->hints.at(s).empty()) {
            Str h = p->hints.at(s)[0];
            return h.size() > 2 ? *stem + camel(h) : *stem + Str("Record");
        }
    }
    if (!hints.empty()) return camel(singular(hints[0]));
    return (stem ? *stem : Str("Anon")) + Str("Record");
}

bool Records::holds_(const T& t, int64_t s, int64_t depth) {
    if (depth > 4) return false;
    if (tk(t, "dict")) return p->find(*t.site) == s;
    if (tk(t, "list") || tk(t, "set")) return holds_(p->elem[static_cast<size_t>(p->find(*t.site))], s, depth + 1);
    for (const T& a : t.args) {
        if (holds_(a, s, depth + 1)) return true;
    }
    return false;
}

std::vector<Records::Field> Records::fields(int64_t s) {
    s = p->find(s);
    program::StrSet req;
    if (p->required[static_cast<size_t>(s)]) req = *p->required[static_cast<size_t>(s)];
    const py::dict<Str, T>& fs = p->fields[static_cast<size_t>(s)];
    std::vector<Str> order;
    auto hi = home.find(s);
    const A::Node* node = nullptr;
    if (hi != home.end() && static_cast<size_t>(hi->second) < p->site_node.size()) node = p->site_node[static_cast<size_t>(hi->second)];
    if (node) {
        auto lk = p->literal_keys.find(node);
        if (lk != p->literal_keys.end()) {
            for (const Str& k : lk->second) {
                if (fs.contains(k)) order.push_back(k);
            }
        }
    }
    std::vector<Str> extra;
    for (const auto& kv : fs.items()) {
        const Str& k = std::get<0>(kv);
        if (std::find(order.begin(), order.end(), k) == order.end()) extra.push_back(k);
    }
    for (const Str& k : extra) order.push_back(k);
    std::vector<Field> out;
    for (const Str& k : order) out.push_back(Field{k, fs.at(k), req.count(k) > 0});
    return out;
}

bool Records::is_ref(int64_t s) { return p->mutated.count(p->find(s)) > 0; }

// ---------------------------------------------------------------------------------------------- Context
Context::Context(Program& prog, const Str& int_policy) : p(prog), records(prog), int_policy(int_policy) {}

Str Context::qual_record(int64_t s) {
    s = p.find(s);
    if (!records.module.count(s)) throw py::KeyError(i2s(s).utf8());
    return records.module.at(s) + Str(".") + records.names.at(s);
}

TypeP Context::ty(const T& t, int64_t line, const Str& what) {
    const Str& k = t.kind;
    if (eq(k, "int")) {
        if (t.fits_i64() || eq(int_policy, "checked")) return I64();
        throw Refused(Str("int-range"), what + Str(" in [") + bs(t.lo) + Str(", ") + bs(t.hi) + Str("]"), line);
    }
    if (eq(k, "bool")) return BOOL();
    if (eq(k, "float")) return F64();
    if (eq(k, "none")) {
        if (!eq(what, "value") && !eq(what, "return value")) throw Refused(Str("untyped"), what + Str(" is only ever None"), line);
        return VOID();
    }
    if (eq(k, "str")) return STR();
    if (eq(k, "bytes")) return BYTES();
    if (eq(k, "bytearray")) return clst(Str("py.bytearray"));
    if (eq(k, "opt")) {
        TypeP inner = ty(t.args[0], line, what);
        return clst(Str("py.optional"), {inner});
    }
    if (eq(k, "tuple")) {
        std::vector<TypeP> a;
        for (const T& x : t.args) a.push_back(ty(x, line, what));
        return clst(Str("py.tuple"), a);
    }
    if ((eq(k, "list") || eq(k, "set") || eq(k, "dict")) && p.poisoned_sites.contains(p.find(*t.site))) {
        throw Refused(Str("untyped"), Str("a ") + k + Str(" also given items of no single type"), line);
    }
    if (eq(k, "list") || eq(k, "set")) {
        int64_t s = p.find(*t.site);
        if (p.dynamic.contains(s) && p.ckind[static_cast<size_t>(s)] != k) throw Refused(Str("conflict"), p.dynamic.at(s), line);
        const T& e = p.elem[static_cast<size_t>(s)];
        if (tk(e, "bottom")) throw Refused(Str("untyped"), Str("the items of an empty ") + k, line);
        return clst(Str("py.") + k, {ty(e, line, k + Str(" item"))});
    }
    if (eq(k, "dict")) {
        int64_t s = p.find(*t.site);
        if (!eq(p.ckind[static_cast<size_t>(s)], "dict")) {
            throw Refused(Str("conflict"), p.dynamic.contains(s) ? p.dynamic.at(s) : Str("not a dict"), line);
        }
        if (p.is_record(s)) {
            for (const Records::Field& f : records.fields(s)) {
                if (tk(f.type, "bottom")) throw Refused(Str("untyped"), Str("record field ") + krepr(f.key), line);
                if (!f.required && tk(f.type, "opt")) {
                    throw Refused(Str("conflict"), Str("record field ") + krepr(f.key) + Str(" both optional and None"), line);
                }
            }
            used_records.insert(s);
            return clst(qual_record(s), {}, info2("ref", records.is_ref(s), "record", true));
        }
        const T& kt = p.keys[static_cast<size_t>(s)];
        const T& vt = p.elem[static_cast<size_t>(s)];
        if (tk(kt, "bottom") || tk(vt, "bottom")) throw Refused(Str("untyped"), Str("the keys or values of an empty dict"), line);
        if (tk(vt, "top")) throw Refused(Str("conflict"), Str("dict values: ") + (vt.why ? *vt.why : Str("None")), line);
        TypeP kk = ty(kt, line, Str("dict key"));
        TypeP vv = ty(vt, line, Str("dict value"));
        return clst(Str("py.dict"), {kk, vv});
    }
    if (eq(k, "inst")) {
        if (starts(*t.name, "exc:")) {
            Str n = t.name->slice_from(4).replace(Str("struct.error"), Str("StructError"));
            return clst(Str("py.") + n);
        }
        Class* c = p.classes.contains(*t.name) ? p.classes.at(*t.name) : nullptr;
        if (c != nullptr && c->exception && !c->exception->empty()) return clst(*t.name, {}, info1("exception", true));
        return clst(*t.name, {}, info1("ref", true));
    }
    if (eq(k, "extern") && t.name && eq(*t.name, "file")) return clst(Str("py.file"));
    if (eq(k, "iter")) {
        // an iterator (range, enumerate, zip, dict.items ...) is consumed where it is made: a loop or a call
        // that takes an iterable; kept in a name, passed or returned it has no C++ type here
        if (!eq(what, "value")) throw Refused(Str("syntax"), Str("an iterator as a ") + what, line);
        return clst(Str("py.iter"), {ty(t.args[0], line, Str("value"))});
    }
    if (eq(k, "top")) {
        Str why = t.why ? *t.why : Str();
        if (why.contains(Str(":"))) {
            int64_t i = why.find(Str(":"));
            Str c = why.slice(0, i), d = why.slice_from(i + 1);
            static const char* known[] = {"dynamic", "stdlib",   "extern",  "syntax",  "unresolved", "int-range",
                                          "conflict", "untyped", "calls-refused", "frontend-error"};
            for (const char* kn : known) {
                if (eq(c, kn)) throw Refused(c, d, line);
            }
        }
        throw Refused(Str("conflict"), why, line);
    }
    if (eq(k, "bottom")) throw Refused(Str("untyped"), what, line);
    throw Refused(Str("conflict"), Str("a ") + k + Str(" as a value"), line);
}

bool is_opt(const TypeP& t) { return class_is(t, "py.optional"); }

// ---------------------------------------------------------------------------------------------- FnLower
FnLower::FnLower(Context& ctx, Fn& fn, std::shared_ptr<infer::Eval> ev)
    : ctx(ctx), p(ctx.p), f(&fn), ev(std::move(ev)) {
    qname = fn.qname;
    module = fn.module;
    cls = fn.cls;
    kind = fn.kind;
    params = fn.params;
    kwonly = fn.kwonly;
    src = fn.module->source;
}

FnLower::FnLower(Context& ctx, Module& m, std::shared_ptr<infer::Eval> ev)
    : ctx(ctx), p(ctx.p), f(nullptr), ev(std::move(ev)) {
    qname = m.name + Str(".<module>");
    module = &m;
    cls = nullptr;
    kind = Str("function");
    src = m.source;
}

const T& FnLower::Tn(const A::Expr* node) {
    auto it = ev->types.find(static_cast<const void*>(node));
    if (it == ev->types.end()) throw Refused(Str("untyped"), Str("no type reached ") + S(lower_ascii(tname(node))), node->pos.lineno);
    return it->second;
}
const T& FnLower::Tn(const A::Stmt* node) {
    auto it = ev->types.find(static_cast<const void*>(node));
    if (it == ev->types.end()) throw Refused(Str("untyped"), Str("no type reached ") + S(lower_ascii(tname(node))), node->pos.lineno);
    return it->second;
}
bool FnLower::has_type(const void* node) const { return ev->types.count(node) > 0; }

TypeP FnLower::ty(const T& t, const A::Node* node, const Str& what) { return ctx.ty(t, node ? node->pos.lineno : 0, what); }

std::optional<Res> FnLower::R(const void* node) const {
    auto it = ev->res.find(node);
    if (it == ev->res.end()) return std::nullopt;
    return it->second;
}

int64_t FnLower::line(const A::Node* n) const { return n ? n->pos.lineno : 0; }

namespace {
const Res& builtin_range() {
    static const Res r(Str("builtin"), {TupItem(Str("range"))});
    return r;
}
bool is_range(const std::optional<Res>& r) { return r && *r == builtin_range(); }
}  // namespace

// -- the function
py::ref<ir::MethodDecl> FnLower::lower() {
    for (const infer::Issue& i : ev->issues) throw Refused(i.cls, i.detail, i.line);
    if (p.crashes.contains(qname)) throw Refused(Str("frontend-error"), p.crashes.at(qname), f->node->pos.lineno);
    std::vector<NodeP> prm;
    size_t skip = (eq(kind, "method") || eq(kind, "property")) ? 1 : 0;
    if (eq(kind, "classmethod")) throw Refused(Str("dynamic"), Str("classmethod"), f->node->pos.lineno);
    std::vector<Str> names(params.begin() + static_cast<std::ptrdiff_t>(std::min(skip, params.size())), params.end());
    names.insert(names.end(), kwonly.begin(), kwonly.end());
    std::vector<TypeP> ptypes;
    for (const Str& name : names) {
        T t = p.param_type(*f, name);
        if (tk(t, "bottom")) {
            auto key = std::make_tuple(qname, name);
            if (p.param_poison.contains(key)) {
                const auto& got = p.param_poison.at(key);
                const Str& why = std::get<1>(got);
                Str head = why.slice(0, why.contains(Str(":")) ? why.find(Str(":")) : why.size());
                Str tail = why.contains(Str(":")) ? why.slice_from(why.find(Str(":")) + 1) : why;
                if (tail.size() > 40) tail = tail.slice(0, 40);
                throw Refused(Str("untyped"),
                              Str("parameter ") + name + Str(" (its callers pass values of no type: ") + head + Str(":") + tail +
                                  Str(" in ") + program::last_part(std::get<0>(got)) + Str(")"),
                              f->node->pos.lineno);
            }
            throw Refused(Str("untyped"), Str("parameter ") + name + Str(" (no annotation, no call site)"), f->node->pos.lineno);
        }
        TypeP it = ty(t, f->node, Str("parameter ") + name);
        local_types[name] = it;
        declared.insert(name);
        py::list<Str> mods;
        if (f->defaults.contains(name)) {
            if (auto* d = A::as<A::Constant>(f->defaults.at(name))) mods.append(Str("default=") + json_default(d));
        }
        auto pr = mk<ir::Param>();
        pr->sym = msym("param", name, std::optional<Str>(qname), false, it);
        pr->type = it;
        pr->mods = mods;
        prm.push_back(pr);
        ptypes.push_back(it);
    }
    T ret_t = p.returns.contains(qname) ? p.returns.at(qname) : P::NONE();
    if (tk(ret_t, "bottom")) ret_t = P::NONE();
    if (eq(f->name, "__init__")) {
        ret = VOID();
    } else {
        ret = ty(ret_t, f->node, Str("return value"));
    }
    plan_locals();
    std::vector<A::Stmt*> store_;
    auto body = block(body_stmts_of(f->node->body, store_));
    Str mkind = f->cls == nullptr ? Str("function") : (eq(f->name, "__init__") ? Str("ctor") : Str("method"));
    py::list<Str> mods;
    if (eq(f->kind, "static")) mods.append(Str("static"));
    if (eq(f->kind, "property")) mods.append(Str("property"));
    SymP sym = msym(f->cls == nullptr ? "function" : (eq(mkind, "ctor") ? "ctor" : "method"), f->name,
                    std::optional<Str>(f->cls ? f->cls->qname : f->module->name), eq(f->kind, "static"), nullptr, ptypes, ret);
    std::string doc;
    bool has_doc = pyengine::docstring(f->node->body, doc);
    auto m = mk<ir::MethodDecl>(f->node->pos.lineno);
    m->sym = sym;
    m->name = f->name;
    m->kind = mkind;
    m->params = nl(prm);
    m->ret = ret;
    m->body = NodeP(body);
    m->mods = mods;
    m->throws = py::list<TypeP>();
    m->typeparams = py::list<Str>();
    py::list<Str> comments;
    if (has_doc) comments.append(S(doc));
    m->comments = comments;
    return m;
}

const std::vector<A::Stmt*>& FnLower::body_stmts_of(const std::vector<A::Stmt*>& stmts, std::vector<A::Stmt*>& store_) const {
    if (!stmts.empty()) {
        if (auto* e = A::as<A::ExprStmt>(stmts[0])) {
            if (auto* c = A::as<A::Constant>(e->value)) {
                if (c->value.kind == A::ConstValue::Kind::Str) {
                    store_.assign(stmts.begin() + 1, stmts.end());
                    return store_;
                }
            }
        }
    }
    return stmts;
}

// -- where each local is declared
std::vector<Str> FnLower::target_names(const A::Expr* t) {
    if (auto* n = A::as<A::Name>(t)) return {S(n->id)};
    std::vector<Str> out;
    const std::vector<A::Expr*>* elts = nullptr;
    if (auto* tu = A::as<A::Tuple>(t)) elts = &tu->elts;
    else if (auto* li = A::as<A::List>(t)) elts = &li->elts;
    if (elts) {
        for (const A::Expr* e : *elts) {
            std::vector<Str> sub = target_names(e);
            out.insert(out.end(), sub.begin(), sub.end());
        }
    }
    return out;
}

// the nodes of a statement outside its nested statement blocks (the Python's stack walk, in its order)
std::vector<const A::Node*> FnLower::own_nodes(const A::Stmt* st) {
    std::vector<const A::Node*> out;
    std::vector<AnyNode> todo;
    todo.push_back(AnyNode(st));
    std::vector<AnyNode> kids;
    while (!todo.empty()) {
        AnyNode n = todo.back();
        todo.pop_back();
        out.push_back(n.p);
        kids.clear();
        pyengine::child_nodes(n, kids);
        for (const AnyNode& c : kids) {
            if (n.p == st && (c.cat == AnyNode::Stmt || c.cat == AnyNode::Handler)) continue;   // body / orelse / finalbody / handlers
            todo.push_back(c);
        }
    }
    return out;
}

namespace {
// the statement lists a statement holds, in the Python's order: body, orelse, finalbody, then its handlers' bodies
std::vector<const std::vector<A::Stmt*>*> blocks_of(const A::Stmt* st) {
    std::vector<const std::vector<A::Stmt*>*> out;
    auto add = [&](const std::vector<A::Stmt*>& v) {
        if (!v.empty()) out.push_back(&v);
    };
    switch (st->kind) {
        case A::StmtKind::FunctionDef:
        case A::StmtKind::AsyncFunctionDef: add(A::cast<A::FunctionDef>(st)->body); break;
        case A::StmtKind::ClassDef: add(A::cast<A::ClassDef>(st)->body); break;
        case A::StmtKind::For:
        case A::StmtKind::AsyncFor: {
            auto* x = A::cast<A::For>(st);
            add(x->body);
            add(x->orelse);
            break;
        }
        case A::StmtKind::While: {
            auto* x = A::cast<A::While>(st);
            add(x->body);
            add(x->orelse);
            break;
        }
        case A::StmtKind::If: {
            auto* x = A::cast<A::If>(st);
            add(x->body);
            add(x->orelse);
            break;
        }
        case A::StmtKind::With:
        case A::StmtKind::AsyncWith: add(A::cast<A::With>(st)->body); break;
        case A::StmtKind::Try:
        case A::StmtKind::TryStar: {
            auto* x = A::cast<A::Try>(st);
            add(x->body);
            add(x->orelse);
            add(x->finalbody);
            for (const A::ExceptHandler* h : x->handlers) out.push_back(&h->body);   // (visited even when empty: nothing in it)
            break;
        }
        default: break;
    }
    return out;
}
}  // namespace

void FnLower::plan_locals() {
    // name -> [(path, stmt)]; path: ((list id, i, stmt), ...)
    struct PathElem {
        const void* list;
        int64_t i;
        const A::Stmt* st;
    };
    using Path = std::vector<PathElem>;
    struct Use {
        Path path;
        const A::Stmt* st;
    };
    std::vector<Str> occ_order;
    std::unordered_map<Str, std::vector<Use>, py::str_hash> occ;
    std::unordered_map<Str, std::vector<const A::Name*>, py::str_hash> stores;
    std::unordered_set<Str, py::str_hash> loads;

    std::function<void(const std::vector<A::Stmt*>&, const Path&)> visit = [&](const std::vector<A::Stmt*>& stmts, const Path& path) {
        for (size_t i = 0; i < stmts.size(); ++i) {
            const A::Stmt* st = stmts[i];
            Path here = path;
            here.push_back(PathElem{&stmts, static_cast<int64_t>(i), st});
            for (const A::Node* n : own_nodes(st)) {
                const A::Expr* e = dynamic_cast<const A::Expr*>(n);
                auto* nm = A::as<A::Name>(e);
                if (!nm) continue;
                Str id = S(nm->id);
                if (ev->locals.count(id) && !declared.count(id)) {
                    if (!occ.count(id)) occ_order.push_back(id);
                    occ[id].push_back(Use{here, st});
                    if (nm->ctx == A::ExprCtx::Store) stores[id].push_back(nm);
                    else loads.insert(id);
                }
            }
            for (const std::vector<A::Stmt*>* sub : blocks_of(st)) {
                if (!sub->empty()) visit(*sub, here);
            }
        }
    };
    visit(f->node->body, Path());

    for (const Str& name : occ_order) {
        std::vector<Use>& uses = occ[name];
        std::vector<T> ts;
        std::vector<const A::Name*>& sts = stores[name];
        for (const A::Name* n : sts) {
            auto it = ev->types.find(static_cast<const void*>(n));
            if (it != ev->types.end()) ts.push_back(it->second);
        }
        if (ts.empty()) continue;   // only read where no store reached: not lowered
        T j = ts[0];
        for (size_t k = 1; k < ts.size(); ++k) j = p.join(j, ts[k]);
        local_types[name] = ty(j, sts[0], Str("local ") + name);
        std::vector<const A::For*> loop_targets;
        for (const Use& u : uses) {
            if (auto* fr = as_for(u.st)) {
                std::vector<Str> tn = target_names(fr->target);
                if (std::find(tn.begin(), tn.end(), name) != tn.end()) loop_targets.push_back(fr);
            }
        }
        if (!loads.count(name) && !sts.empty()) {
            bool all_in_targets = true;
            for (const A::Name* n : sts) {
                bool any = false;
                for (const A::For* st : loop_targets) {
                    for (const AnyNode& x : pyengine::walk(AnyNode(st->target))) {
                        if (x.p == n) any = true;
                    }
                }
                if (!any) {
                    all_in_targets = false;
                    break;
                }
            }
            if (all_in_targets) {
                loopvars.insert(name);   // never read: each loop declares its own
                continue;
            }
        }
        std::vector<std::vector<std::pair<const void*, int64_t>>> paths;
        for (const Use& u : uses) {
            std::vector<std::pair<const void*, int64_t>> pp;
            for (const PathElem& e : u.path) pp.push_back({e.list, e.i});
            paths.push_back(pp);
        }
        size_t L = 0;
        for (;;) {
            bool all_long = true;
            for (const auto& pp : paths) {
                if (!(pp.size() > L)) {
                    all_long = false;
                    break;
                }
            }
            if (!all_long) break;
            std::set<std::pair<const void*, int64_t>> one;
            for (const auto& pp : paths) one.insert(pp[L]);
            if (one.size() != 1) break;
            ++L;
        }
        const Path& first_path = uses[0].path;
        const A::Stmt* anchor;
        bool all_long = true;
        for (const auto& pp : paths) {
            if (!(pp.size() > L)) {
                all_long = false;
                break;
            }
        }
        std::set<const void*> lists_at_L;
        if (all_long) {
            for (const auto& pp : paths) lists_at_L.insert(pp[L].first);
        }
        if (all_long && lists_at_L.size() == 1) {
            anchor = first_path[L].st;   // siblings in one block: the first of them
        } else {
            int64_t idx = static_cast<int64_t>(L) - 1;
            if (idx < 0) idx += static_cast<int64_t>(first_path.size());
            anchor = first_path.at(static_cast<size_t>(idx)).st;   // the statement whose blocks hold them all
        }
        if (auto* afor = as_for(anchor)) {
            std::vector<Str> tn = target_names(afor->target);
            // (all(any(x is u[1] or x is anchor for x in ast.walk(anchor)) ...) is always true: anchor is in its own walk)
            if (std::find(tn.begin(), tn.end(), name) != tn.end()) continue;   // the loop declares it
        }
        const A::Stmt* first_stmt = uses[0].st;
        std::string nm = name.utf8();
        auto* asg = A::as<A::Assign>(first_stmt);
        if (anchor == first_stmt && asg && asg->targets.size() == 1 && name_is(asg->targets[0], nm) &&
            !mentions(AnyNode(asg->value), nm)) {
            decl_init[first_stmt] = name;
        } else if (anchor == first_stmt && asg && asg->targets.size() == 1 && A::as<A::Tuple>(asg->targets[0]) &&
                   [&] {
                       std::vector<Str> tn = target_names(asg->targets[0]);
                       return std::find(tn.begin(), tn.end(), name) != tn.end();
                   }() &&
                   !mentions(AnyNode(asg->value), nm)) {
            decl_tuple[first_stmt].insert(name);
        } else {
            decl_at[anchor].push_back(name);
        }
    }
}

// -- statements
py::ref<ir::Block> FnLower::block(const std::vector<A::Stmt*>& stmts) {
    std::vector<NodeP> out;
    for (const A::Stmt* st : stmts) {
        auto da = decl_at.find(static_cast<const void*>(st));
        if (da != decl_at.end()) {
            for (const Str& name : da->second) {
                const TypeP& t = local_types.at(name);
                out.push_back(mk_local(msym("local", name, std::optional<Str>(qname), false, t), t, nullptr, py::list<Str>()));
                declared.insert(name);
            }
        }
        pre.clear();
        std::vector<NodeP> got = stmt(st);
        out.insert(out.end(), pre.begin(), pre.end());
        pre.clear();
        out.insert(out.end(), got.begin(), got.end());
    }
    return mk_block(out);
}

std::vector<NodeP> FnLower::stmt(const A::Stmt* st) {
    switch (st->kind) {
        case A::StmtKind::Pass: return {};
        case A::StmtKind::Expr: return s_Expr(A::cast<A::ExprStmt>(st));
        case A::StmtKind::Assign: return s_Assign(A::cast<A::Assign>(st));
        case A::StmtKind::AnnAssign: return s_AnnAssign(A::cast<A::AnnAssign>(st));
        case A::StmtKind::AugAssign: return s_AugAssign(A::cast<A::AugAssign>(st));
        case A::StmtKind::Return: return s_Return(A::cast<A::Return>(st));
        case A::StmtKind::Raise: return s_Raise(A::cast<A::Raise>(st));
        case A::StmtKind::Assert: return s_Assert(A::cast<A::Assert>(st));
        case A::StmtKind::If: return s_If(A::cast<A::If>(st));
        case A::StmtKind::While: return s_While(A::cast<A::While>(st));
        case A::StmtKind::Break: return {mk_break(line(st))};
        case A::StmtKind::Continue: return {mk_continue(line(st))};
        case A::StmtKind::For: return s_For(A::cast<A::For>(st));
        case A::StmtKind::Try: return s_Try(A::cast<A::Try>(st));
        case A::StmtKind::With: return s_With(A::cast<A::With>(st));
        case A::StmtKind::Import:
        case A::StmtKind::ImportFrom: return {};
        default: throw Refused(Str("syntax"), S(lower_ascii(tname(st))), line(st));
    }
}

std::vector<NodeP> FnLower::s_Expr(const A::ExprStmt* st) {
    if (auto* c = A::as<A::Constant>(st->value)) {
        if (c->value.kind == A::ConstValue::Kind::Str) {
            auto cm = mk<ir::Comment>();
            cm->text = S(c->value.text);
            return {cm};
        }
        return {};
    }
    NodeP e = expr(st->value);
    return {mk_expr_stmt(e, line(st))};
}

std::vector<NodeP> FnLower::s_Assign(const A::Assign* st) {
    if (st->targets.size() != 1) throw Refused(Str("syntax"), Str("chained assignment"), line(st));
    const A::Expr* tgt = st->targets[0];
    auto kind = st->value->kind;
    if ((kind == A::ExprKind::ListComp || kind == A::ExprKind::SetComp || kind == A::ExprKind::DictComp) && A::as<A::Name>(tgt) &&
        !mentions(AnyNode(st->value), A::cast<A::Name>(tgt)->id)) {
        return comp_into(st, A::cast<A::Name>(tgt), st->value);
    }
    if (is_record_literal(st->value) && A::as<A::Name>(tgt) && decl_init.count(static_cast<const void*>(st))) {
        return record_builder(st, A::cast<A::Name>(tgt));
    }
    NodeP value = expr(st->value);
    return store(tgt, value, st);
}

std::vector<NodeP> FnLower::s_AnnAssign(const A::AnnAssign* st) {
    if (st->value == nullptr) return {};
    NodeP v = expr(st->value);
    return store(st->target, v, st);
}

std::vector<NodeP> FnLower::s_AugAssign(const A::AugAssign* st) {
    const T t = Tn(st);
    NodeP target = target_expr(st->target);
    NodeP value = expr(st->value);
    NodeP cur = expr_of_target(st->target, target);
    const T& tt = has_type(static_cast<const void*>(st->target)) ? Tn(st->target) : t;
    const T& tv = Tn(st->value);
    NodeP nw = binop(st, st->op, cur, value, tt, tv, t);
    auto bin = cast_to<ir::Binary>(nw);
    if (bin && bin->op) {
        static const char* ops[] = {"+", "-", "*", "&", "|", "^", "<<", ">>", "/", "%"};
        bool in = false;
        for (const char* o : ops) {
            if (eq(*bin->op, o)) in = true;
        }
        if (in && bin->left == cur) {
            return {mk_expr_stmt(mk_assign(bin->op, target, value, bin->type), line(st))};
        }
    }
    auto cc = cast_to<ir::Concat>(nw);
    if (cc && cc->parts && !cc->parts->empty() && (*cc->parts)[0] == cur) {
        NodeP v;
        if (cc->parts->size() > 2) {
            std::vector<NodeP> rest(cc->parts->vec().begin() + 1, cc->parts->vec().end());
            v = mk_concat(rest, cc->type);
        } else {
            v = (*cc->parts)[1];
        }
        return {mk_expr_stmt(mk_assign(Str("+"), target, v, cc->type), line(st))};
    }
    return {mk_expr_stmt(mk_assign(std::nullopt, target, nw, type_of(nw)), line(st))};
}

NodeP FnLower::expr_of_target(const A::Expr* tgt, const NodeP& lowered) {
    if (A::as<A::Name>(tgt)) {
        auto n = cast_to<ir::Name>(lowered);
        return mk_name(n->sym, n->type);
    }
    return lowered;
}

std::vector<NodeP> FnLower::store(const A::Expr* tgt, const NodeP& value_in, const A::Stmt* st) {
    int64_t ln = line(st);
    NodeP value = value_in;
    if (auto* nm = A::as<A::Name>(tgt)) {
        Str name = S(nm->id);
        auto lt = local_types.find(name);
        if (lt == local_types.end()) throw Refused(Str("untyped"), Str("local ") + name, ln);
        TypeP t = lt->second;
        value = coerce(value, t);
        if (decl_init.count(static_cast<const void*>(st)) && !declared.count(name)) {
            declared.insert(name);
            return {mk_local(msym("local", name, std::optional<Str>(qname), false, t), t, value, py::list<Str>(), ln)};
        }
        return {mk_expr_stmt(mk_assign(std::nullopt, mk_name(msym("local", name, std::optional<Str>(qname), false, t), t), value, t), ln)};
    }
    if (A::as<A::Tuple>(tgt) || A::as<A::List>(tgt)) return unpack(tgt, value, st);
    NodeP target = target_expr(tgt);
    value = coerce(value, type_of(target));
    return {mk_expr_stmt(mk_assign(std::nullopt, target, value, type_of(target)), ln)};
}

std::vector<NodeP> FnLower::unpack(const A::Expr* tgt, const NodeP& value, const A::Stmt* st) {
    int64_t ln = line(st);
    std::vector<Str> names;
    if (!A::as<A::Tuple>(tgt) && !A::as<A::List>(tgt)) throw py::AttributeError("'" + tname(tgt) + "' object has no attribute 'elts'");
    const std::vector<A::Expr*>& elts = A::as<A::Tuple>(tgt) ? A::cast<A::Tuple>(tgt)->elts : A::cast<A::List>(tgt)->elts;
    for (const A::Expr* e : elts) {
        auto* nm = A::as<A::Name>(e);
        if (!nm) throw Refused(Str("syntax"), Str("unpacking into ") + S(tname(e)), ln);
        names.push_back(S(nm->id));
    }
    const TypeP& vt = type_of(value);
    bool is_tuple = vt && eq(vt->kind, "class") && vt->name && eq(*vt->name, "py.tuple");
    if (!is_tuple || py::len(vt->args) != static_cast<int64_t>(names.size())) {
        throw Refused(Str("conflict"), Str("unpacking a non-tuple"), ln);
    }
    const py::list<TypeP>& tts = vt->args;
    auto sym_t = [&](const Str& n) { return msym("local", n, std::optional<Str>(qname), false, local_types.at(n)); };
    auto dt = decl_tuple.find(static_cast<const void*>(st));
    if (names.size() == 1) {   // `x, = t`: its one item
        const Str& n = names[0];
        NodeP g = pyfn(Str("get"), {value}, tts[0], nullptr, 0, targs({Val(int64_t(0))}));
        auto lt = local_types.find(n);
        NodeP item = coerce(g, lt == local_types.end() ? nullptr : lt->second);
        if (dt != decl_tuple.end() && dt->second.count(n) && !declared.count(n)) {
            declared.insert(n);
            const TypeP& t = local_types.at(n);
            return {mk_local(sym_t(n), t, item, py::list<Str>(), ln)};
        }
        return store(elts[0], item, st);
    }
    bool fresh_all = true;
    for (size_t i = 0; i < names.size(); ++i) {
        const Str& n = names[i];
        bool fresh = dt != decl_tuple.end() && dt->second.count(n);
        auto lt = local_types.find(n);
        if (!(fresh && !declared.count(n) && lt != local_types.end() && tsame(lt->second, tts[static_cast<int64_t>(i)]))) {
            fresh_all = false;
            break;
        }
    }
    if (fresh_all) {
        for (const Str& n : names) declared.insert(n);
        std::vector<NodeP> ns;
        for (const Str& n : names) ns.push_back(name_expr(n));
        return {mk_expr_stmt(mk_assign(std::nullopt, pyfn(Str("bind"), ns, vt), value, vt), ln)};
    }
    std::vector<NodeP> out;
    for (const Str& n : names) {
        if (!declared.count(n)) {
            const TypeP& t = local_types.at(n);
            out.push_back(mk_local(sym_t(n), t, nullptr, py::list<Str>()));
            declared.insert(n);
        }
    }
    std::vector<NodeP> ns;
    for (const Str& n : names) ns.push_back(name_expr(n));
    out.push_back(mk_expr_stmt(mk_assign(std::nullopt, pyfn(Str("tie"), ns, vt), value, vt), ln));
    return out;
}

NodeP FnLower::name_expr(const Str& name) {
    auto lt = local_types.find(name);
    if (lt == local_types.end()) throw py::KeyError(krepr(name).utf8());
    const TypeP& t = lt->second;
    return mk_name(msym("local", name, std::optional<Str>(qname), false, t), t);
}

NodeP FnLower::target_expr(const A::Expr* tgt) {
    int64_t ln = line(tgt);
    if (auto* nm = A::as<A::Name>(tgt)) {
        Str id = S(nm->id);
        if (!local_types.count(id)) throw Refused(Str("untyped"), Str("local ") + id, ln);
        if (!declared.count(id)) {
            const TypeP& t = local_types.at(id);
            pre.push_back(mk_local(msym("local", id, std::optional<Str>(qname), false, t), t, nullptr, py::list<Str>()));
            declared.insert(id);
        }
        return name_expr(id);
    }
    if (auto* at_ = A::as<A::Attribute>(tgt)) {
        auto r = R(tgt);
        if (!r || !eq(r->kind, "field")) throw Refused(Str("dynamic"), Str("attribute store"), ln);
        NodeP obj = expr(at_->value);
        auto at = p.attr_type(r->s(1), r->s(2));
        if (!at) throw py::TypeError("attr_type is None");
        TypeP ft = ctx.ty(*at, ln, Str("attribute ") + r->s(2));
        return mk_select(obj, msym("field", r->s(2), std::optional<Str>(r->s(1)), false, ft), ft, ln);
    }
    if (auto* sub = A::as<A::Subscript>(tgt)) {
        auto r = R(tgt);
        NodeP obj = expr(sub->value);
        if (r && eq(r->kind, "dict_store") && !none_item_is(*r, 1) && info_true(type_of(obj), "record")) {
            return field_ref(obj, r->s(1), sub->value, ln, true);
        }
        if (A::as<A::Slice>(sub->slice)) throw Refused(Str("syntax"), Str("slice assignment"), ln);
        NodeP idx = expr(sub->slice);
        TypeP et = elem_type(type_of(obj), ln);
        return mk_index(obj, idx, et, ln);
    }
    throw Refused(Str("syntax"), Str("assignment target ") + S(tname(tgt)), ln);
}

TypeP FnLower::elem_type(const TypeP& t, int64_t ln) {
    if (kind_is(t, "class") && t->name && (eq(*t->name, "py.list") || eq(*t->name, "py.set")) && !t->args.empty()) return t->args[0];
    if (class_is(t, "py.dict")) return t->args[1];
    if (kind_is(t, "class") && t->name && (eq(*t->name, "py.bytes") || eq(*t->name, "py.bytearray"))) return I64();
    if (class_is(t, "py.str")) return STR();
    throw Refused(Str("conflict"), Str("item of ") + (t ? t->repr() : Str("None")), ln);
}

std::vector<NodeP> FnLower::s_Return(const A::Return* st) {
    if (st->value == nullptr) {
        return {mk_return(tsame(ret, VOID()) ? NodeP() : lit(Val(), ret), line(st))};
    }
    auto k = st->value->kind;
    if (k == A::ExprKind::ListComp || k == A::ExprKind::SetComp || k == A::ExprKind::DictComp) {
        Str name = new_tmp(Str("result"));
        TypeP t = ty(Tn(st->value), st->value);
        std::vector<NodeP> out;
        out.push_back(mk_local(msym("local", name, std::optional<Str>(qname), false, t), t, empty(t), py::list<Str>()));
        local_types[name] = t;
        declared.insert(name);
        std::vector<NodeP> loops = comp_loops(st->value, name, t);
        out.insert(out.end(), loops.begin(), loops.end());
        out.push_back(mk_return(name_expr(name), line(st)));
        return out;
    }
    if (tsame(ret, VOID())) {
        NodeP e = expr(st->value);
        if (is_none_const(st->value)) return {mk_return(nullptr, line(st))};
        return {mk_expr_stmt(e), mk_return(nullptr, line(st))};
    }
    NodeP e = expr(st->value);
    return {mk_return(coerce(e, ret), line(st))};
}

std::vector<NodeP> FnLower::s_Raise(const A::Raise* st) {
    if (st->exc == nullptr) return {mk_throw(nullptr, line(st))};
    NodeP e = expr(st->exc);
    return {mk_throw(e, line(st))};
}

std::vector<NodeP> FnLower::s_Assert(const A::Assert* st) {
    NodeP c = cond(st->test);
    std::vector<NodeP> args;
    if (st->msg != nullptr) args.push_back(expr(st->msg));
    if (!args.empty() && !tsame(type_of(args[0]), STR())) args = {pyfn(Str("str"), args, STR())};
    NodeP exc = mk_new(msym("ctor", Str("AssertionError"), std::optional<Str>(Str(PY))), args, clst(Str("py.AssertionError")), std::nullopt);
    auto then = mk_block({mk_throw(exc)});
    auto n = mk<ir::If>(line(st));
    n->cond = mk_unary(Str("!"), mk_paren(c, BOOL()), false, BOOL());
    n->then = NodeP(then);
    n->orelse = NodeP();
    return {n};
}

std::vector<NodeP> FnLower::s_If(const A::If* st) {
    NodeP c = cond(st->test);
    std::vector<NodeP> pre_ = pre;
    pre.clear();
    auto then = block(st->body);
    NodeP orelse;
    if (!st->orelse.empty()) {
        const A::Stmt* only = st->orelse.size() == 1 ? st->orelse[0] : nullptr;
        if (only && A::as<A::If>(only) && !decl_at.count(static_cast<const void*>(only))) {
            std::vector<NodeP> saved = pre;
            pre.clear();
            NodeP inner = s_If(A::cast<A::If>(only))[0];
            if (!pre.empty()) {
                std::vector<NodeP> ss = pre;
                ss.push_back(inner);
                orelse = mk_block(ss);
            } else {
                orelse = inner;
            }
            pre = saved;
        } else {
            orelse = block(st->orelse);
        }
    }
    pre = pre_;
    auto n = mk<ir::If>(line(st));
    n->cond = c;
    n->then = NodeP(then);
    n->orelse = orelse;
    return {n};
}

std::vector<NodeP> FnLower::s_While(const A::While* st) {
    std::vector<NodeP> saved = pre;
    pre.clear();
    NodeP c = cond(st->test);
    if (!pre.empty()) throw Refused(Str("syntax"), Str("a while condition that needs statements"), line(st));
    pre = saved;
    auto body = block(st->body);
    auto n = mk<ir::While>(line(st));
    n->cond = c;
    n->body = NodeP(body);
    return {n};
}

std::vector<NodeP> FnLower::s_For(const A::For* st) {
    int64_t ln = line(st);
    const A::Expr* it = st->iter;
    auto r = R(it);
    // `for x in range(...)`: a counted loop
    if (is_range(r) && A::as<A::Name>(st->target)) return range_loop(st);
    NodeP iterable = expr(it);
    const A::Expr* tgt = st->target;
    std::vector<Str> names;
    const std::vector<A::Expr*>* elts = nullptr;
    if (auto* tu = A::as<A::Tuple>(tgt)) elts = &tu->elts;
    else if (auto* li = A::as<A::List>(tgt)) elts = &li->elts;
    if (auto* nm = A::as<A::Name>(tgt)) {
        names.push_back(S(nm->id));
    } else if (elts) {
        for (const A::Expr* e : *elts) {
            if (auto* n2 = A::as<A::Name>(e)) names.push_back(S(n2->id));
        }
    }
    if (!A::as<A::Name>(tgt) && (elts == nullptr || names.size() != elts->size())) {
        // (a target that is neither: Python's getattr(tgt, 'elts', []) is empty, so 0 != 0 is false)
        if (elts != nullptr) throw Refused(Str("syntax"), Str("loop target"), ln);
    }
    bool inner_only = true;
    for (const Str& n : names) {
        if (!loop_local(st, n)) {
            inner_only = false;
            break;
        }
    }
    TypeP elem_t = ty(Tn(tgt), tgt, Str("loop item"));
    if (inner_only) {
        for (const Str& n : names) declared.insert(n);
        NodeP var;
        if (auto* nm = A::as<A::Name>(tgt)) {
            Str id = S(nm->id);
            const TypeP& lt = local_types.at(id);
            var = mk_local(msym("local", id, std::optional<Str>(qname), false, lt), lt, nullptr, py::list<Str>());
        } else {
            std::vector<std::string> parts;
            std::string joined;
            for (size_t i = 0; i < names.size(); ++i) joined += (i ? "," : "") + names[i].utf8();
            var = mk_local(msym("local", S(joined), std::optional<Str>(qname), false, elem_t), elem_t, nullptr, strs({"bind"}));
        }
        auto body = block(st->body);
        auto n = mk<ir::ForEach>(ln);
        n->var = var;
        n->iterable = iterable;
        n->body = NodeP(body);
        return {n};
    }
    Str tmpn = new_tmp(Str("item"));
    local_types[tmpn] = elem_t;
    NodeP var = mk_local(msym("local", tmpn, std::optional<Str>(qname), false, elem_t), elem_t, nullptr, py::list<Str>());
    std::vector<NodeP> pre_;
    for (const Str& n : names) {
        if (!declared.count(n)) {
            const TypeP& t = local_types.at(n);
            pre_.push_back(mk_local(msym("local", n, std::optional<Str>(qname), false, t), t, nullptr, py::list<Str>()));
            declared.insert(n);
        }
    }
    std::vector<NodeP> first = A::as<A::Name>(tgt) ? store(tgt, name_expr(tmpn), st) : unpack(tgt, name_expr(tmpn), st);
    auto body = block(st->body);
    std::vector<NodeP> stmts = first;
    for (const NodeP& x : body->stmts->vec()) stmts.push_back(x);
    body->stmts = nl(stmts);
    auto n = mk<ir::ForEach>(ln);
    n->var = var;
    n->iterable = iterable;
    n->body = NodeP(body);
    pre_.push_back(n);
    return pre_;
}

// the loop variable is used only inside the loop (C++ can declare it in the loop's header)
bool FnLower::loop_local(const A::Stmt* st, const Str& name) {
    if (loopvars.count(name)) return true;
    std::string nm = name.utf8();
    std::unordered_set<const A::Node*> inside;
    for (const AnyNode& n : pyengine::walk(AnyNode(st))) inside.insert(n.p);
    for (const AnyNode& n : pyengine::walk(AnyNode(static_cast<const A::Stmt*>(f->node)))) {
        if (const A::Expr* e = n.expr()) {
            if (name_is(e, nm) && !inside.count(n.p)) return false;
        }
    }
    auto* fr = as_for(st);
    const A::Expr* target = fr->target;
    const std::vector<A::Expr*>* telts = A::as<A::Tuple>(target) ? &A::cast<A::Tuple>(target)->elts : nullptr;
    for (const AnyNode& n : pyengine::walk(AnyNode(st))) {   // nor stored again inside, nor in a nested loop's target
        const A::Expr* e = n.expr();
        auto* nn = A::as<A::Name>(e);
        if (nn && nn->id == nm && nn->ctx == A::ExprCtx::Store && n.p != target) {
            bool in_elts = telts && std::find(telts->begin(), telts->end(), e) != telts->end();
            if (!in_elts) return false;
        }
    }
    return !declared.count(name) || loopvars.count(name);
}

std::vector<NodeP> FnLower::range_loop(const A::For* st) {
    int64_t ln = line(st);
    auto* call = A::cast<A::Call>(st->iter);
    std::vector<NodeP> args;
    for (const A::Expr* a : call->args) args.push_back(expr(a));
    int64_t step = 1;
    if (call->args.size() == 3) {
        auto sc = program::int_const(call->args[2]);
        if (!sc) throw py::TypeError("'>' not supported between instances of 'NoneType' and 'int'");
        step = as_i64(*sc);
    }
    NodeP start, stop;
    if (args.size() == 1) {
        start = lit(Val(int64_t(0)), I64());
        stop = args[0];
    } else {
        start = args[0];
        stop = args[1];
    }
    Str name = S(A::cast<A::Name>(st->target)->id);
    auto lt = local_types.find(name);
    TypeP it_t = lt == local_types.end() ? I64() : lt->second;
    bool inner = loop_local(st, name);
    std::vector<NodeP> stmts;
    if (!pure(call->args.size() < 3 ? call->args.back() : call->args[1], st)) {
        Str sname = new_tmp(Str("stop"));
        local_types[sname] = I64();
        stmts.push_back(mk_local(msym("local", sname, std::optional<Str>(qname), false, I64()), I64(), stop, strs({"const"})));
        stop = name_expr(sname);
    }
    Str var;
    if (inner) {
        var = name;
        declared.insert(name);
    } else {
        var = new_tmp(name);
        local_types[var] = I64();
        if (!declared.count(name)) {
            stmts.push_back(mk_local(msym("local", name, std::optional<Str>(qname), false, it_t), it_t, nullptr, py::list<Str>()));
            declared.insert(name);
        }
    }
    NodeP v = mk_name(msym("local", var, std::optional<Str>(qname), false, I64()), I64());
    std::vector<NodeP> init = {mk_local(msym("local", var, std::optional<Str>(qname), false, I64()), I64(), start, py::list<Str>())};
    NodeP c = mk_binary(Str(step > 0 ? "<" : ">"), v, stop, BOOL());
    NodeP upd = step == 1 ? mk_unary(Str("++"), v, false, I64())
                          : (step == -1 ? mk_unary(Str("--"), v, false, I64())
                                        : mk_assign(Str("+"), v, lit(Val(step), I64()), I64()));
    auto body = block(st->body);
    if (!inner) {
        std::vector<NodeP> ss = {mk_expr_stmt(mk_assign(std::nullopt, name_expr(name), v, I64()))};
        for (const NodeP& x : body->stmts->vec()) ss.push_back(x);
        body->stmts = nl(ss);
    }
    auto fo = mk<ir::For>(ln);
    fo->init = nl(init);
    fo->cond = c;
    fo->update = nl({upd});
    fo->body = NodeP(body);
    stmts.push_back(fo);
    return stmts;
}

// a range bound C++ may evaluate on every pass: a constant, or a local the loop does not store
bool FnLower::pure(const A::Expr* node, const A::Stmt* loop) {
    if (A::as<A::Constant>(node)) return true;
    if (auto* nm = A::as<A::Name>(node)) {
        if (local_types.count(S(nm->id))) {
            for (const AnyNode& n : pyengine::walk(AnyNode(loop))) {
                auto* x = A::as<A::Name>(n.expr());
                if (x && x->id == nm->id && x->ctx == A::ExprCtx::Store) return false;
            }
            return true;
        }
    }
    return false;
}

std::vector<NodeP> FnLower::s_Try(const A::Try* st) {
    auto body = block(st->body);
    std::vector<NodeP> catches;
    for (const A::ExceptHandler* h : st->handlers) {
        std::vector<TypeP> types;
        if (h->type == nullptr) {
            types.push_back(clst(Str("py.Exception")));
        } else {
            std::vector<const A::Expr*> names;
            if (auto* tu = A::as<A::Tuple>(h->type)) names.assign(tu->elts.begin(), tu->elts.end());
            else names.push_back(h->type);
            for (const A::Expr* n : names) {
                std::optional<Str> lc;
                if (A::as<A::Name>(n)) lc = ev->lookup_class(n);
                if (A::as<A::Name>(n) && lc && !lc->empty()) {
                    types.push_back(clst(*lc, {}, info1("ref", false)));
                } else if (auto* nm = A::as<A::Name>(n)) {
                    types.push_back(clst(Str("py.") + S(nm->id)));
                } else {
                    Str u = program::unparse(n);
                    auto* at_ = A::as<A::Attribute>(n);
                    if (!at_) throw py::AttributeError("'" + tname(n) + "' object has no attribute 'attr'");
                    types.push_back(clst(eq(u, "struct.error") ? Str("py.StructError") : Str("py.") + S(at_->attr)));
                }
            }
        }
        auto hb = block(h->body);
        auto c = mk<ir::Catch>(h->pos.lineno);
        c->sym = nullptr;
        c->types = py::list<TypeP>(types);
        c->body = NodeP(hb);
        catches.push_back(c);
    }
    auto t = mk<ir::Try>(line(st));
    t->resources = py::list<NodeP>();
    t->body = NodeP(body);
    t->catches = nl(catches);
    t->final = nullptr;
    return {t};
}

std::vector<NodeP> FnLower::s_With(const A::With* st) {
    std::vector<NodeP> out;
    for (const A::withitem* item : st->items) {
        NodeP v = expr(item->context_expr);
        if (item->optional_vars != nullptr) {
            std::vector<NodeP> s = store(item->optional_vars, v, st);
            out.insert(out.end(), s.begin(), s.end());
        } else {
            out.push_back(mk_expr_stmt(v));
        }
    }
    auto b = block(st->body);
    for (const NodeP& x : b->stmts->vec()) out.push_back(x);
    return {NodeP(mk_block(out))};
}

// -- comprehensions
Str FnLower::new_tmp(const Str& base) {
    tmp += 1;
    Str name = (local_types.count(base) || ev->locals.count(base) || tmp > 1) ? base + Str("_") + i2s(tmp) : base;
    while (local_types.count(name) || ev->locals.count(name)) {
        tmp += 1;
        name = base + Str("_") + i2s(tmp);
    }
    return name;
}

NodeP FnLower::empty(const TypeP& t) { return pyfn(Str("empty"), {}, t); }

std::vector<NodeP> FnLower::comp_into(const A::Assign* st, const A::Name* tgt, const A::Expr* comp) {
    (void)st;
    Str name = S(tgt->id);
    TypeP t = local_types.at(name);
    std::vector<NodeP> out;
    if (!declared.count(name)) {
        out.push_back(mk_local(msym("local", name, std::optional<Str>(qname), false, t), t, empty(t), py::list<Str>()));
        declared.insert(name);
    } else {
        out.push_back(mk_expr_stmt(mk_assign(std::nullopt, name_expr(name), empty(t), t)));
    }
    std::vector<NodeP> loops = comp_loops(comp, name, t);
    out.insert(out.end(), loops.begin(), loops.end());
    return out;
}

// the loops of a comprehension, adding each item to the local `into`
std::vector<NodeP> FnLower::comp_loops(const A::Expr* comp, const Str& into, const TypeP& t) {
    NodeP acc = name_expr(into);
    const std::vector<A::comprehension*>* gens = nullptr;
    const A::Expr *elt = nullptr, *key = nullptr, *value = nullptr;
    bool is_dict = false;
    if (auto* x = A::as<A::ListComp>(comp)) {
        gens = &x->generators;
        elt = x->elt;
    } else if (auto* y = A::as<A::SetComp>(comp)) {
        gens = &y->generators;
        elt = y->elt;
    } else if (auto* z = A::as<A::DictComp>(comp)) {
        gens = &z->generators;
        key = z->key;
        value = z->value;
        is_dict = true;
    } else if (auto* w = A::as<A::GeneratorExp>(comp)) {
        gens = &w->generators;
        elt = w->elt;
    }
    auto add = [&]() -> NodeP {
        if (is_dict) {
            NodeP k = expr(key);
            NodeP v = coerce(expr(value), t->args[1]);
            return mk_expr_stmt(mk_assign(std::nullopt, mk_index(acc, k, t->args[1]), v, t->args[1]));
        }
        NodeP e = coerce(expr(elt), t->args[0]);
        return mk_expr_stmt(pyfn(eq(*t->name, "py.list") ? Str("list.append") : Str("set.add"), {e}, VOID(), acc));
    };
    return gen_loops(*gens, 0, add);
}

std::vector<NodeP> FnLower::gen_loops(const std::vector<A::comprehension*>& gens, size_t k, const std::function<NodeP()>& innermost) {
    const A::comprehension* g = gens[k];
    std::vector<NodeP> saved = pre;
    pre.clear();
    auto r = R(g->iter);
    NodeP iterable = !is_range(r) ? expr(g->iter) : NodeP();
    const A::Expr* tgt = g->target;
    std::vector<Str> names;
    const std::vector<A::Expr*>* elts = nullptr;
    if (auto* tu = A::as<A::Tuple>(tgt)) elts = &tu->elts;
    else if (auto* li = A::as<A::List>(tgt)) elts = &li->elts;
    if (auto* nm = A::as<A::Name>(tgt)) {
        names.push_back(S(nm->id));
    } else if (elts) {
        for (const A::Expr* e : *elts) {
            if (auto* n2 = A::as<A::Name>(e)) names.push_back(S(n2->id));
        }
    }
    if (!A::as<A::Name>(tgt) && elts != nullptr && names.size() != elts->size()) {
        throw Refused(Str("syntax"), Str("comprehension target"), line(tgt));
    }
    TypeP elem_t = ty(Tn(tgt), tgt, Str("comprehension item"));
    for (const Str& n : names) {
        auto lt = local_types.find(n);
        if (lt != local_types.end() && declared.count(n) && !tsame(lt->second, elem_t)) {
            throw Refused(Str("syntax"), Str("comprehension variable ") + n + Str(" shadows a local of another type"), line(tgt));
        }
    }
    NodeP var;
    if (auto* nm = A::as<A::Name>(tgt)) {
        Str id = S(nm->id);
        local_types[id] = elem_t;
        var = mk_local(msym("local", id, std::optional<Str>(qname), false, elem_t), elem_t, nullptr, py::list<Str>());
    } else {
        if (elts) {
            size_t n = std::min(elts->size(), static_cast<size_t>(py::len(elem_t->args)));
            for (size_t i = 0; i < n; ++i) {
                auto* en = A::as<A::Name>((*elts)[i]);
                if (!en) throw py::AttributeError("'" + tname((*elts)[i]) + "' object has no attribute 'id'");
                local_types[S(en->id)] = elem_t->args[static_cast<int64_t>(i)];
            }
        }
        std::string joined;
        for (size_t i = 0; i < names.size(); ++i) joined += (i ? "," : "") + names[i].utf8();
        var = mk_local(msym("local", S(joined), std::optional<Str>(qname), false, elem_t), elem_t, nullptr, strs({"bind"}));
    }
    std::vector<NodeP> body_stmts;
    if (gens.size() > k + 1) {
        std::vector<NodeP> inner = gen_loops(gens, k + 1, innermost);
        body_stmts.insert(body_stmts.end(), inner.begin(), inner.end());
    } else {
        pre.clear();
        NodeP s = innermost();
        body_stmts.insert(body_stmts.end(), pre.begin(), pre.end());
        body_stmts.push_back(s);
        pre.clear();
    }
    for (auto it = g->ifs.rbegin(); it != g->ifs.rend(); ++it) {
        pre.clear();
        NodeP c = cond(*it);
        if (!pre.empty()) throw Refused(Str("syntax"), Str("comprehension condition needing statements"), line(*it));
        auto iff = mk<ir::If>();
        iff->cond = c;
        iff->then = NodeP(mk_block(body_stmts));
        iff->orelse = NodeP();
        body_stmts = {iff};
    }
    NodeP loop;
    if (is_range(r) && A::as<A::Name>(tgt)) {
        auto* gcall = A::cast<A::Call>(g->iter);
        std::vector<NodeP> args;
        for (const A::Expr* a : gcall->args) args.push_back(expr(a));
        NodeP start, stop;
        if (args.size() == 1) {
            start = lit(Val(int64_t(0)), I64());
            stop = args[0];
        } else {
            start = args[0];
            stop = args[1];
        }
        int64_t step = 1;
        if (args.size() == 3) {
            auto sc = program::int_const(gcall->args[2]);
            if (!sc) throw py::TypeError("'>' not supported between instances of 'NoneType' and 'int'");
            step = as_i64(*sc);
        }
        NodeP v = name_expr(S(A::cast<A::Name>(tgt)->id));
        auto lv = cast_to<ir::Name>(v);
        auto fo = mk<ir::For>();
        fo->init = nl({mk_local(lv->sym, I64(), start, py::list<Str>())});
        fo->cond = mk_binary(Str(step > 0 ? "<" : ">"), v, stop, BOOL());
        fo->update = nl({step == 1 ? mk_unary(Str("++"), v, false, I64()) : mk_assign(Str("+"), v, lit(Val(step), I64()), I64())});
        fo->body = NodeP(mk_block(body_stmts));
        loop = fo;
    } else {
        auto fe = mk<ir::ForEach>();
        fe->var = var;
        fe->iterable = iterable;
        fe->body = NodeP(mk_block(body_stmts));
        loop = fe;
    }
    std::vector<NodeP> out = pre;
    out.push_back(loop);
    pre = saved;
    return out;
}

// a comprehension inside an expression: built into a temporary before the statement
NodeP FnLower::hoist_comp(const A::Expr* node, const char* kind) {
    TypeP t = ty(Tn(node), node);
    const char* base = eq(Str(kind), "dict") ? "table" : "items";
    Str name = new_tmp(Str(base));
    local_types[name] = t;
    declared.insert(name);
    std::vector<NodeP> stmts;
    stmts.push_back(mk_local(msym("local", name, std::optional<Str>(qname), false, t), t, empty(t), py::list<Str>()));
    std::vector<NodeP> loops = comp_loops(node, name, t);
    stmts.insert(stmts.end(), loops.begin(), loops.end());
    pre.insert(pre.end(), stmts.begin(), stmts.end());
    return name_expr(name);
}

// a generator expression consumed whole (sum, join, sorted, list, set, min, max): its items as a list
NodeP FnLower::gen_as_list(const A::GeneratorExp* gen) {
    TypeP et = ty(Tn(gen->elt), gen->elt, Str("generator item"));
    TypeP t = clst(Str("py.list"), {et});
    Str name = new_tmp(Str("items"));
    local_types[name] = t;
    declared.insert(name);
    NodeP acc = name_expr(name);
    std::vector<NodeP> stmts;
    stmts.push_back(mk_local(cast_to<ir::Name>(acc)->sym, t, empty(t), py::list<Str>()));
    auto add = [&]() -> NodeP {
        NodeP e = coerce(expr(gen->elt), et);
        return mk_expr_stmt(pyfn(Str("list.append"), {e}, VOID(), acc));
    };
    std::vector<NodeP> loops = gen_loops(gen->generators, 0, add);
    stmts.insert(stmts.end(), loops.begin(), loops.end());
    pre.insert(pre.end(), stmts.begin(), stmts.end());
    return acc;
}

// any(gen) / all(gen): a loop that stops at the first item deciding it (Python's short-circuit)
NodeP FnLower::any_all(const A::Call* node, const char* which) {
    const A::Expr* gen = node->args[0];
    auto* g = A::as<A::GeneratorExp>(gen);
    bool is_all = eq(Str(which), "all");
    if (!g) {
        NodeP seq = expr(gen);
        return pyfn(Str(which), {seq}, BOOL());
    }
    Str name = new_tmp(Str(is_all ? "every" : "found"));
    local_types[name] = BOOL();
    declared.insert(name);
    NodeP acc = name_expr(name);
    std::vector<NodeP> stmts;
    stmts.push_back(mk_local(cast_to<ir::Name>(acc)->sym, BOOL(), lit(Val(is_all), BOOL()), py::list<Str>()));
    auto decide = [&]() -> NodeP {
        NodeP c = cond(g->elt);
        if (is_all) c = mk_unary(Str("!"), mk_paren(c, BOOL()), false, BOOL());
        auto iff = mk<ir::If>();
        iff->cond = c;
        iff->then = NodeP(mk_block({mk_expr_stmt(mk_assign(std::nullopt, acc, lit(Val(!is_all), BOOL()), BOOL())), mk_break()}));
        iff->orelse = NodeP();
        return iff;
    };
    std::vector<NodeP> loops = gen_loops(g->generators, 0, decide);
    if (g->generators.size() > 1) {
        throw Refused(Str("syntax"), Str(which) + Str(" over nested loops (break leaves only the inner one)"), line(node));
    }
    stmts.insert(stmts.end(), loops.begin(), loops.end());
    pre.insert(pre.end(), stmts.begin(), stmts.end());
    return acc;
}

// -- records
bool FnLower::is_record_literal(const A::Expr* node) {
    auto it = ev->types.find(static_cast<const void*>(node));
    if (it == ev->types.end()) return false;
    const T& t = it->second;
    if (!(tk(t, "dict") && p.is_record(*t.site))) return false;
    if (A::as<A::Dict>(node)) return true;
    auto* c = A::as<A::Call>(node);
    return c && name_is(c->func, "dict");
}

// `h = {...}` (then `h["k"] = v` ...): the record declared, then filled as Python fills it
std::vector<NodeP> FnLower::record_builder(const A::Assign* st, const A::Name* tgt) {
    Str name = S(tgt->id);
    TypeP t = local_types.at(name);
    NodeP value = expr(st->value);
    declared.insert(name);
    return {mk_local(msym("local", name, std::optional<Str>(qname), false, t), t, value, py::list<Str>(), line(st))};
}

// a dict display / dict(...) call that is a record: its fields in the struct's order
NodeP FnLower::record_value(const A::Expr* node) {
    const T t = Tn(node);
    TypeP rt = ty(t, node);
    int64_t s = p.find(*t.site);
    std::vector<Records::Field> fields = ctx.records.fields(s);
    std::vector<std::pair<std::optional<Str>, NodeP>> given;   // (key, lowered) in source order
    if (auto* d = A::as<A::Dict>(node)) {
        for (size_t i = 0; i < d->keys.size(); ++i) {
            if (d->keys[i] == nullptr) {
                NodeP src = expr(d->values[i]);
                auto sp = spread_fields(src, d->values[i]);
                given.insert(given.end(), sp.begin(), sp.end());
            } else {
                auto key = program::str_const(d->keys[i]);
                NodeP v = expr(d->values[i]);
                given.push_back({key, v});
            }
        }
    } else {
        auto* c = A::cast<A::Call>(node);
        for (const A::Expr* a : c->args) {
            NodeP src = expr(a);
            auto sp = spread_fields(src, a);
            given.insert(given.end(), sp.begin(), sp.end());
        }
        for (const A::keyword* kw : c->keywords) {
            if (!kw->has_arg) {
                NodeP src = expr(kw->value);
                auto sp = spread_fields(src, kw->value);
                given.insert(given.end(), sp.begin(), sp.end());
            } else {
                NodeP v = expr(kw->value);
                given.push_back({std::optional<Str>(S(kw->arg)), v});
            }
        }
    }
    // by_key = dict(given): the last value of a key; order keeps repeats
    std::vector<std::optional<Str>> keys_seen;
    std::map<std::optional<Str>, NodeP> by_key;
    for (auto& g : given) by_key[g.first] = g.second;
    std::vector<std::optional<Str>> order;
    for (auto& g : given) order.push_back(g.first);
    std::vector<std::optional<Str>> field_filtered;
    for (const Records::Field& f : fields) {
        if (by_key.count(std::optional<Str>(f.key))) field_filtered.push_back(f.key);
    }
    int64_t impure = 0;
    for (auto& g : given) {
        if (effects(g.second)) ++impure;
    }
    if (order != field_filtered && impure > 1) {
        for (const auto& k : order) {            // keep Python's evaluation order
            NodeP e = by_key[k];
            if (effects(e)) {
                Str n = new_tmp(k ? *k : Str("None"));
                local_types[n] = type_of(e);
                declared.insert(n);
                pre.push_back(mk_local(msym("local", n, std::optional<Str>(qname), false, type_of(e)), type_of(e), e, py::list<Str>()));
                by_key[k] = name_expr(n);
            }
        }
    }
    std::vector<NodeP> args;
    for (const Records::Field& f : fields) {
        TypeP want = ty(f.type, node, Str("field ") + f.key);
        if (!f.required) want = !is_opt(want) ? clst(Str("py.optional"), {want}) : want;
        auto bk = by_key.find(std::optional<Str>(f.key));
        if (bk != by_key.end()) {
            args.push_back(coerce(bk->second, want));
        } else {
            args.push_back(lit(Val(), want));
        }
    }
    py::list<Val> ta;
    for (const Records::Field& f : fields) ta.append(Val(f.key));
    return pyfn(Str("record"), args, rt, nullptr, line(node), ta);
}

std::vector<std::pair<std::optional<Str>, NodeP>> FnLower::spread_fields(const NodeP& src, const A::Expr* node) {
    if (!info_true(type_of(src), "record")) throw Refused(Str("syntax"), Str("** of a dict"), line(node));
    const T st = Tn(node);
    std::vector<std::pair<std::optional<Str>, NodeP>> out;
    for (const Records::Field& f : ctx.records.fields(*st.site)) {
        out.push_back({std::optional<Str>(f.key), field_ref(src, f.key, node, line(node), false, true)});
    }
    return out;
}

NodeP FnLower::field_ref(const NodeP& obj, const Str& key, const A::Expr* node, int64_t ln, bool store_, bool raw) {
    const T t = Tn(node);
    int64_t s = p.find(*t.site);
    std::optional<Records::Field> found;
    for (const Records::Field& f : ctx.records.fields(s)) {
        if (f.key == key) found = f;   // dict(...): the last of a repeated key
    }
    if (!found) throw Refused(Str("conflict"), Str("no field ") + krepr(key), ln);
    TypeP ftt = ty(found->type, node, Str("field ") + key);
    if (!found->required && (raw || store_)) ftt = !is_opt(ftt) ? clst(Str("py.optional"), {ftt}) : ftt;
    const TypeP& ot = type_of(obj);
    NodeP sel = mk_select(obj, msym("field", key, ot ? ot->name : std::nullopt, false, ftt), ftt, ln);
    if (!found->required && !raw && !store_) return pyfn(Str("opt_value"), {sel}, ftt);   // an absent key raises (KeyError in Python)
    return sel;
}

// -- expressions
// an expression as a C++ condition (Python's truth)
NodeP FnLower::cond(const A::Expr* node) {
    in_cond = node;
    NodeP e = expr(node);
    return truthy(e);
}

NodeP FnLower::truthy(const NodeP& e) {
    if (tsame(type_of(e), BOOL())) return e;
    return pyfn(Str("truthy"), {e}, BOOL());
}

// a value where a `want` is expected: an optional unwrapped or wrapped, None typed
NodeP FnLower::coerce(const NodeP& e, const TypeP& want) {
    if (!want || tsame(type_of(e), want)) return e;
    const TypeP& et = type_of(e);
    if (auto l = cast_to<ir::Lit>(e)) {
        if (l->value.is_none()) return lit(Val(), want);
    }
    if (is_opt(et) && !is_opt(want)) {
        if (tsame(et->args[0], want)) return pyfn(Str("unwrap"), {e}, want);
    }
    if (is_opt(want) && tsame(et, want->args[0])) return e;
    if (eq(want->kind, "class") && et && eq(et->kind, "class") && info_true(want, "ref") && info_true(et, "ref")) return e;   // a subclass where its base is expected
    if (tsame(want, I64()) && tsame(et, BOOL())) return mk_cast(e, I64());
    if (class_is(want, "py.tuple") && class_is(et, "py.tuple")) return e;
    if (kind_is(want, "class") && want->name && (eq(*want->name, "py.bytes") || eq(*want->name, "py.bytearray")) && kind_is(et, "class") &&
        et->name && (eq(*et->name, "py.bytes") || eq(*et->name, "py.bytearray"))) {
        return e;
    }
    return e;
}

NodeP FnLower::expr(const A::Expr* node) {
    switch (node->kind) {
        case A::ExprKind::Constant: return e_Constant(A::cast<A::Constant>(node));
        case A::ExprKind::Name: return e_Name(A::cast<A::Name>(node));
        case A::ExprKind::Attribute: return e_Attribute(A::cast<A::Attribute>(node));
        case A::ExprKind::Subscript: return e_Subscript(A::cast<A::Subscript>(node));
        case A::ExprKind::BinOp: return e_BinOp(A::cast<A::BinOp>(node));
        case A::ExprKind::UnaryOp: return e_UnaryOp(A::cast<A::UnaryOp>(node));
        case A::ExprKind::BoolOp: return e_BoolOp(A::cast<A::BoolOp>(node));
        case A::ExprKind::Compare: return e_Compare(A::cast<A::Compare>(node));
        case A::ExprKind::IfExp: return e_IfExp(A::cast<A::IfExp>(node));
        case A::ExprKind::JoinedStr: return e_JoinedStr(A::cast<A::JoinedStr>(node));
        case A::ExprKind::Tuple: return e_Tuple(A::cast<A::Tuple>(node));
        case A::ExprKind::List: return e_List(A::cast<A::List>(node));
        case A::ExprKind::Set: return e_Set(A::cast<A::Set>(node));
        case A::ExprKind::Dict: return e_Dict(A::cast<A::Dict>(node));
        case A::ExprKind::ListComp: return hoist_comp(node, "list");
        case A::ExprKind::SetComp: return hoist_comp(node, "set");
        case A::ExprKind::DictComp: return hoist_comp(node, "dict");
        case A::ExprKind::GeneratorExp: throw Refused(Str("syntax"), Str("generator expression as a value"), node->pos.lineno);
        case A::ExprKind::Lambda: throw Refused(Str("dynamic"), Str("lambda"), node->pos.lineno);
        case A::ExprKind::Call: return e_Call(A::cast<A::Call>(node));
        default: throw Refused(Str("syntax"), S(lower_ascii(tname(node))), node->pos.lineno);
    }
}

std::optional<Str> FnLower::source_segment(const A::Node* n) {
    if (!have_src_) {
        utf8_src_ = src.utf8();
        line_starts_ = pyengine::line_starts(utf8_src_);
        have_src_ = true;
    }
    std::string out;
    if (!pyengine::source_segment(utf8_src_, line_starts_, n->pos, out)) return std::nullopt;
    return S(out);
}

NodeP FnLower::e_Constant(const A::Constant* n) {
    const A::ConstValue& v = n->value;
    const T t = Tn(n);
    using K = A::ConstValue::Kind;
    if (v.kind == K::None) return lit(Val(), VOID());
    if (v.kind == K::True || v.kind == K::False) return lit(Val(v.kind == K::True), BOOL());
    if (v.kind == K::Int) {
        if (!t.fits_i64()) throw Refused(Str("int-range"), Str("literal ") + S(v.text), n->pos.lineno);
        auto text = source_segment(n);
        bool ok = text.has_value() && !text->empty();
        if (ok) {
            for (char32_t c : text->u32()) {
                bool good = (c >= U'0' && c <= U'9') || (c >= U'a' && c <= U'f') || (c >= U'A' && c <= U'F') || c == U'x' || c == U'X' ||
                            c == U'o' || c == U'O' || c == U'b' || c == U'B' || c == U'_';
                if (!good) {
                    ok = false;
                    break;
                }
            }
        }
        return lit(Val(as_i64(Big::from_decimal(v.text))), I64(), ok ? text : std::nullopt);
    }
    if (v.kind == K::Float) return lit(Val(v.f), F64(), source_segment(n));
    if (v.kind == K::Str) return lit(Val(S(v.text)), STR());
    if (v.kind == K::Bytes) return lit(Val(latin1(v.text)), BYTES());
    throw Refused(Str("syntax"), Str("constant"), n->pos.lineno);
}

NodeP FnLower::e_Name(const A::Name* n) {
    auto r = R(n);
    const T t = Tn(n);
    if (!r || eq(r->kind, "local")) {
        Str name = S(n->id);
        if ((eq(kind, "method") || eq(kind, "property")) && !params.empty() && name == params[0]) {
            auto th = mk<ir::This>(n->pos.lineno);
            th->qualifier = nullptr;
            th->type = clst(cls->qname, {}, info1("ref", true));
            return th;
        }
        if (!local_types.count(name)) throw Refused(Str("untyped"), Str("local ") + name, n->pos.lineno);
        NodeP e = name_expr(name);
        TypeP flow = ty(t, n);
        return narrowed(e, flow);
    }
    if (eq(r->kind, "global")) {
        globals_used.insert(std::make_tuple(r->s(1), r->s(2)));
        TypeP gt = ty(p.global_type(r->s(1), r->s(2)), n, Str("global ") + r->s(2));
        return mk_select(mk_typename(clst(r->s(1))), msym("global", r->s(2), std::optional<Str>(r->s(1)), true, gt), gt, n->pos.lineno);
    }
    throw Refused(Str("dynamic"), r->kind + Str(" ") + S(n->id) + Str(" as a value"), n->pos.lineno);
}

NodeP FnLower::narrowed(const NodeP& e, const TypeP& flow) {
    if (is_opt(type_of(e)) && !is_opt(flow)) {
        if (tsame(flow, VOID())) return e;
        return pyfn(Str("unwrap"), {e}, flow);
    }
    return e;
}

NodeP FnLower::e_Attribute(const A::Attribute* n) {
    auto r = R(n);
    int64_t ln = n->pos.lineno;
    if (!r) throw Refused(Str("dynamic"), Str("attribute .") + S(n->attr), ln);
    if (eq(r->kind, "field")) {
        NodeP obj = expr(n->value);
        auto at_ = p.attr_type(r->s(1), r->s(2));
        if (!at_) throw py::TypeError("attr_type is None");
        TypeP ft = ctx.ty(*at_, ln, Str("attribute ") + r->s(2));
        return mk_select(obj, msym("field", r->s(2), std::optional<Str>(r->s(1)), false, ft), ft, ln);
    }
    if (eq(r->kind, "property")) {
        NodeP obj = expr(n->value);
        calls.insert(r->s(1));
        TypeP t = ty(Tn(n), n);
        Fn* fn = p.fns.at(r->s(1));
        return mk_call(obj, msym("method", fn->name, std::optional<Str>(fn->cls->qname), false, nullptr, {}, t), {}, t, ln);
    }
    if (eq(r->kind, "global")) {
        globals_used.insert(std::make_tuple(r->s(1), r->s(2)));
        TypeP gt = ty(p.global_type(r->s(1), r->s(2)), n, Str("global ") + r->s(2));
        return mk_select(mk_typename(clst(r->s(1))), msym("global", r->s(2), std::optional<Str>(r->s(1)), true, gt), gt, ln);
    }
    if (eq(r->kind, "classattr")) {
        TypeP t = ty(p.class_attr_type(r->s(1), r->s(2)), n, Str("class attribute ") + r->s(2));
        return mk_select(mk_typename(clst(r->s(1))), msym("field", r->s(2), std::optional<Str>(r->s(1)), true, t), t, ln);
    }
    if (eq(r->kind, "stdattr")) {
        TypeP t = ty(Tn(n), n);
        return pyfn(r->s(1), {}, t, nullptr, ln);
    }
    throw Refused(Str("dynamic"), Str("attribute ") + S(n->attr) + Str(" as a value"), ln);
}

NodeP FnLower::e_Subscript(const A::Subscript* n) {
    auto r0 = R(n);
    Res r = r0 ? *r0 : Res(Str("?"));
    int64_t ln = n->pos.lineno;
    TypeP t = ty(Tn(n), n);
    NodeP obj = expr(n->value);
    if (eq(r.kind, "slice")) {
        auto* sl = A::cast<A::Slice>(n->slice);
        NodeP lo = sl->lower != nullptr ? expr(sl->lower) : lit(Val(), VOID());
        NodeP hi = sl->upper != nullptr ? expr(sl->upper) : lit(Val(), VOID());
        std::vector<NodeP> args = {lo, hi};
        if (sl->step != nullptr) args.push_back(expr(sl->step));
        return pyfn(Str("slice"), args, t, obj, ln);
    }
    if (eq(r.kind, "tuple_slice")) {
        const TypeP& ot = type_of(obj);
        int64_t cnt = py::len(ot->args);
        auto sb = slice_bounds(cnt, big_item(r, 1), big_item(r, 2));
        if (effects(obj)) throw Refused(Str("syntax"), Str("slice of a computed tuple"), ln);
        std::vector<NodeP> parts;
        for (int64_t i = sb.first; i < sb.second; ++i) {
            parts.push_back(pyfn(Str("get"), {obj}, ot->args[i], nullptr, 0, targs({Val(i)})));
        }
        return pyfn(Str("tuple"), parts, t);
    }
    if (eq(r.kind, "tuple_index")) {
        auto bi = big_item(r, 1);
        if (!bi) throw Refused(Str("syntax"), Str("computed tuple index"), ln);
        int64_t cnt = py::len(type_of(obj)->args);
        if (cnt == 0) throw py::ZeroDivisionError("integer modulo by zero");
        Big m = floormod(*bi, Big(cnt));
        int64_t i = m.i64();
        return pyfn(Str("get"), {obj}, t, nullptr, ln, targs({Val(i)}));
    }
    if (eq(r.kind, "field_get")) return field_ref(obj, r.s(1), n->value, ln);
    NodeP idx = expr(n->slice);
    if (eq(r.kind, "dict_get")) return pyfn(Str("dict.at"), {idx}, t, obj, ln);
    return mk_index(obj, idx, t, ln);
}

NodeP FnLower::e_BinOp(const A::BinOp* n) {
    NodeP a = expr(n->left);
    NodeP b = expr(n->right);
    const T& ta = Tn(n->left);
    const T& tb = Tn(n->right);
    const T& tr = Tn(n);
    return binop(n, n->op, a, b, ta, tb, tr);
}

NodeP FnLower::binop(const A::Node* n, A::OpKind op, const NodeP& a_in, const NodeP& b_in, const T& ta_in, const T& tb_in, const T& tr) {
    int64_t ln = line(n);
    const char* o;
    switch (op) {
        case A::OpKind::Add: o = "+"; break;
        case A::OpKind::Sub: o = "-"; break;
        case A::OpKind::Mult: o = "*"; break;
        case A::OpKind::Div: o = "/"; break;
        case A::OpKind::FloorDiv: o = "//"; break;
        case A::OpKind::Mod: o = "%"; break;
        case A::OpKind::LShift: o = "<<"; break;
        case A::OpKind::RShift: o = ">>"; break;
        case A::OpKind::BitAnd: o = "&"; break;
        case A::OpKind::BitOr: o = "|"; break;
        case A::OpKind::BitXor: o = "^"; break;
        case A::OpKind::Pow: o = "**"; break;
        default: throw py::KeyError("<class 'ast.MatMult'>");
    }
    TypeP rt = ty(tr, n);
    NodeP a = coerce(a_in, type_of(a_in));
    NodeP b = b_in;
    T ta = ta_in, tb = tb_in;
    if (is_opt(type_of(a)) || is_opt(type_of(b))) throw Refused(Str("conflict"), Str("arithmetic on a value that may be None"), ln);
    Str ka = ta.kind, kb = tb.kind;
    if (eq(ka, "bool")) {
        ka = Str("int");
        ta = P::INT(Big(0), Big(1));
        a = mk_cast(a, I64());
    }
    if (eq(kb, "bool")) {
        kb = Str("int");
        tb = P::INT(Big(0), Big(1));
        b = mk_cast(b, I64());
    }
    if (eq(ka, "int") && eq(kb, "int")) return int_op(n, o, a, b, ta, tb, tr, rt);
    if (tsame(rt, F64())) {
        if (eq(Str(o), "/")) return pyfn(Str("truediv"), {a, b}, F64(), nullptr, ln);
        if (eq(Str(o), "//")) return pyfn(Str("floordiv"), {a, b}, F64(), nullptr, ln);
        if (eq(Str(o), "%")) return pyfn(Str("mod"), {a, b}, F64(), nullptr, ln);
        if (eq(Str(o), "**")) return pyfn(Str("pow"), {a, b}, F64(), nullptr, ln);
        NodeP l = as_double(a);
        return mk_binary(Str(o), l, b, F64(), ln);
    }
    if (eq(ka, "str") && eq(Str(o), "+")) {
        std::vector<NodeP> parts;
        if (auto ca = cast_to<ir::Concat>(a)) parts = ca->parts->vec();
        else parts.push_back(a);
        if (auto cb = cast_to<ir::Concat>(b)) {
            for (const NodeP& x : cb->parts->vec()) parts.push_back(x);
        } else {
            parts.push_back(b);
        }
        return mk_concat(parts, STR(), ln);
    }
    if (eq(ka, "str") && eq(Str(o), "%")) {
        std::vector<NodeP> vals;
        auto bc = cast_to<ir::Call>(b);
        if (bc && bc->sym && eq(bc->sym->name, "tuple") && bc->sym->owner && eq(*bc->sym->owner, PY)) vals = bc->args->vec();
        else vals.push_back(b);
        std::vector<NodeP> args = {a};
        args.insert(args.end(), vals.begin(), vals.end());
        return pyfn(Str("percent"), args, STR(), nullptr, ln);
    }
    if (eq(Str(o), "*") && (eq(ka, "str") || eq(ka, "bytes") || eq(ka, "list"))) return pyfn(Str("repeat"), {a, b}, rt, nullptr, ln);
    if (eq(Str(o), "+")) return pyfn(Str("concat"), {a, b}, rt, nullptr, ln);
    if (eq(ka, "set") && eq(kb, "set")) return pyfn(Str("set") + Str(o), {a, b}, rt, nullptr, ln);
    throw Refused(Str("conflict"), ka + Str(" ") + Str(o) + Str(" ") + kb, ln);
}

NodeP FnLower::as_double(const NodeP& e) {
    if (tsame(type_of(e), I64())) return mk_cast(e, F64());
    return e;
}

// an integer operation: C++'s own operator where the intervals show it agrees with Python's, else the support
// header's Python-exact one (and with int_policy 'checked', an overflow-checked one where the result is not shown
// to fit)
NodeP FnLower::int_op(const A::Node* n, const char* o_, const NodeP& a, const NodeP& b, const T& ta, const T& tb, const T& tr, const TypeP& rt) {
    (void)rt;
    int64_t ln = line(n);
    Str o(o_);
    bool checked = eq(ctx.int_policy, "checked") && !tr.fits_i64();
    if (eq(o, "+") || eq(o, "-") || eq(o, "*")) {
        if (checked) return pyfn(Str("checked_") + Str(eq(o, "+") ? "add" : (eq(o, "-") ? "sub" : "mul")), {a, b}, I64(), nullptr, ln);
        return mk_binary(o, a, b, I64(), ln);
    }
    if (eq(o, "&") || eq(o, "|") || eq(o, "^")) return mk_binary(o, a, b, I64(), ln);
    if (eq(o, "//") || eq(o, "%")) {
        if (ta.nonneg() && tb.positive()) return mk_binary(Str(eq(o, "//") ? "/" : "%"), a, b, I64(), ln);
        return pyfn(Str(eq(o, "//") ? "floordiv" : "mod"), {a, b}, I64(), nullptr, ln);
    }
    if (eq(o, "<<")) {
        if (!checked && ta.nonneg() && tb.nonneg() && tb.hi && *tb.hi < Big(63)) return mk_binary(Str("<<"), a, b, I64(), ln);
        return pyfn(Str(checked ? "checked_shl" : "shl"), {a, b}, I64(), nullptr, ln);
    }
    if (eq(o, ">>")) {
        if (ta.nonneg() && tb.nonneg() && tb.hi && *tb.hi < Big(63)) return mk_binary(Str(">>"), a, b, I64(), ln);
        return pyfn(Str("shr"), {a, b}, I64(), nullptr, ln);
    }
    if (eq(o, "**")) return pyfn(Str(checked ? "checked_pow" : "ipow"), {a, b}, I64(), nullptr, ln);
    if (eq(o, "/")) return pyfn(Str("truediv"), {a, b}, F64(), nullptr, ln);
    throw Refused(Str("conflict"), Str("int ") + o + Str(" int"), ln);
}

NodeP FnLower::e_UnaryOp(const A::UnaryOp* n) {
    NodeP x = expr(n->operand);
    if (n->op == A::UnaryOpKind::Not) return mk_unary(Str("!"), paren(truthy(x)), false, BOOL(), n->pos.lineno);
    TypeP t = ty(Tn(n), n);
    if (tsame(type_of(x), BOOL())) x = mk_cast(x, I64());
    if (n->op == A::UnaryOpKind::USub) {
        if (eq(ctx.int_policy, "checked") && tsame(t, I64()) && !Tn(n).fits_i64()) return pyfn(Str("checked_neg"), {x}, I64());
        return mk_unary(Str("-"), paren(x), false, t, n->pos.lineno);
    }
    if (n->op == A::UnaryOpKind::UAdd) return x;
    if (n->op == A::UnaryOpKind::Invert) return mk_unary(Str("~"), paren(x), false, t, n->pos.lineno);
    throw Refused(Str("syntax"), Str("unary"), n->pos.lineno);
}

NodeP FnLower::paren(const NodeP& x) {
    if (is_a<ir::Binary>(x) || is_a<ir::Cond>(x) || is_a<ir::Assign>(x)) return mk_paren(x, type_of(x));
    return x;
}

NodeP FnLower::e_BoolOp(const A::BoolOp* n) {
    const T t = Tn(n);
    const char* op = n->op == A::BoolOpKind::And ? "&&" : "||";
    auto r = R(n);
    if (r && eq(r->kind, "mixedbool") && in_cond != n) {
        auto w = opt_str_item(*r, 1);
        throw Refused(Str("conflict"), Str("value of and/or over ") + (w ? *w : Str("None")), n->pos.lineno);
    }
    if (tk(t, "bool")) {
        std::vector<NodeP> saved = pre;
        std::vector<NodeP> parts;
        for (size_t i = 0; i < n->values.size(); ++i) {
            pre.clear();
            NodeP c = cond(n->values[i]);
            if (!pre.empty() && i > 0) throw Refused(Str("syntax"), Str("a lazy operand that needs statements"), n->pos.lineno);
            saved.insert(saved.end(), pre.begin(), pre.end());
            parts.push_back(c);
        }
        pre = saved;
        NodeP e = parts[0];
        for (size_t i = 1; i < parts.size(); ++i) {
            NodeP l = paren_bool(e, op);
            NodeP rr = paren_bool(parts[i], op);
            e = mk_binary(Str(op), l, rr, BOOL(), n->pos.lineno);
        }
        return e;
    }
    // a value: `a or b` is a if a is true, else b (b only evaluated then)
    TypeP rt = ty(t, n);
    if (n->values.size() != 2) throw Refused(Str("syntax"), Str("value of a chained and/or"), n->pos.lineno);
    NodeP a = expr(n->values[0]);
    std::vector<NodeP> saved = pre;
    pre.clear();
    NodeP b = expr(n->values[1]);
    if (!pre.empty() || effects(b)) throw Refused(Str("syntax"), Str("value of and/or with a right side that has effects"), n->pos.lineno);
    pre = saved;
    if (is_opt(type_of(a)) && !is_opt(rt)) {
        return pyfn(Str(eq(Str(op), "||") ? "or_none" : "and_none"), {a, coerce(b, rt)}, rt, nullptr, n->pos.lineno);
    }
    NodeP ca = coerce(a, rt);
    NodeP cb = coerce(b, rt);
    return pyfn(Str(eq(Str(op), "||") ? "or" : "and"), {ca, cb}, rt, nullptr, n->pos.lineno);
}

NodeP FnLower::paren_bool(const NodeP& e, const char* op) {
    auto b = cast_to<ir::Binary>(e);
    if (b && b->op && (eq(*b->op, "&&") || eq(*b->op, "||")) && !eq(*b->op, op)) return mk_paren(e, BOOL());
    return e;
}

NodeP FnLower::e_Compare(const A::Compare* n) {
    const A::Expr* left_node = n->left;
    NodeP a = expr(left_node);
    std::vector<NodeP> parts;
    for (size_t i = 0; i < n->ops.size(); ++i) {
        const A::Expr* c = n->comparators[i];
        NodeP b = expr(c);
        if (i + 1 < n->ops.size() && effects(b)) throw Refused(Str("syntax"), Str("chained comparison with a computed middle"), n->pos.lineno);
        parts.push_back(compare(n, n->ops[i], a, b, left_node, c));
        a = b;
        left_node = c;
    }
    NodeP e = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) e = mk_binary(Str("&&"), e, parts[i], BOOL(), n->pos.lineno);
    return e;
}

NodeP FnLower::compare(const A::Node* n, A::CmpOpKind op, const NodeP& a_in, const NodeP& b_in, const A::Expr* an, const A::Expr* bn) {
    int64_t ln = line(n);
    NodeP a = a_in, b = b_in;
    bool is_op = op == A::CmpOpKind::Is || op == A::CmpOpKind::IsNot;
    bool eq_op = op == A::CmpOpKind::Eq || op == A::CmpOpKind::NotEq;
    if (is_op || (eq_op && is_none_const(bn))) {
        bool pos = op == A::CmpOpKind::Is || op == A::CmpOpKind::Eq;
        NodeP x = !is_none_const(an) ? a : b;
        return pyfn(Str(pos ? "is_none" : "is_not_none"), {x}, BOOL(), nullptr, ln);
    }
    if (op == A::CmpOpKind::In || op == A::CmpOpKind::NotIn) {
        bool neg = op == A::CmpOpKind::NotIn;
        if (A::as<A::Tuple>(bn) && !effects(a)) {
            std::vector<NodeP> parts;
            auto bc = cast_to<ir::Call>(b);
            bool is_tuple_call = bc && bc->sym && eq(bc->sym->name, "tuple");
            if (is_tuple_call) {
                for (const NodeP& x : bc->args->vec()) {
                    NodeP cx = coerce(x, type_of(a));
                    parts.push_back(mk_binary(Str(neg ? "!=" : "=="), a, cx, BOOL()));
                }
            }
            if (!parts.empty()) {
                NodeP e = parts[0];
                for (size_t i = 1; i < parts.size(); ++i) e = mk_binary(Str(neg ? "&&" : "||"), e, parts[i], BOOL());
                return parts.size() > 1 ? mk_paren(e, BOOL()) : e;
            }
        }
        const T bt = Tn(bn);
        if (tk(bt, "tuple")) {   // `x in t` of a tuple: each item compared
            NodeP e = pyfn(Str("in"), {a, b}, BOOL(), nullptr, ln);
            return neg ? mk_unary(Str("!"), e, false, BOOL()) : e;
        }
        if (tk(bt, "dict") && p.is_record(*bt.site)) {
            auto key = program::str_const(an);
            std::optional<bool> req;
            for (const Records::Field& f : ctx.records.fields(*bt.site)) {
                if (key && f.key == *key) req = f.required;
            }
            if (!req) return lit(Val(neg), BOOL());
            if (*req) {
                if (effects(b)) throw Refused(Str("syntax"), Str("membership in a computed record"), ln);
                return lit(Val(!neg), BOOL());
            }
            const TypeP& btp = type_of(b);
            NodeP sel = mk_select(b, msym("field", *key, btp ? btp->name : std::nullopt), clst(Str("py.optional"), {}));
            return pyfn(Str(!neg ? "is_not_none" : "is_none"), {sel}, BOOL());
        }
        NodeP e = pyfn(Str("contains"), {a}, BOOL(), b, ln);
        return neg ? mk_unary(Str("!"), e, false, BOOL()) : e;
    }
    const char* o = "==";
    switch (op) {
        case A::CmpOpKind::Eq: o = "=="; break;
        case A::CmpOpKind::NotEq: o = "!="; break;
        case A::CmpOpKind::Lt: o = "<"; break;
        case A::CmpOpKind::LtE: o = "<="; break;
        case A::CmpOpKind::Gt: o = ">"; break;
        case A::CmpOpKind::GtE: o = ">="; break;
        default: break;
    }
    if (is_opt(type_of(a)) || is_opt(type_of(b))) {
        if (eq(Str(o), "==") || eq(Str(o), "!=")) return mk_binary(Str(o), a, b, BOOL(), ln);
        throw Refused(Str("conflict"), Str("ordering a value that may be None"), ln);
    }
    if (tsame(type_of(a), BOOL()) && tsame(type_of(b), I64())) a = mk_cast(a, I64());
    if (tsame(type_of(b), BOOL()) && tsame(type_of(a), I64())) b = mk_cast(b, I64());
    const TypeP& ta = type_of(a);
    const TypeP& tb = type_of(b);
    if (ta && tb && ((eq(ta->kind, "i64") && eq(tb->kind, "f64")) || (eq(ta->kind, "f64") && eq(tb->kind, "i64")))) {
        a = as_double(a);
        b = as_double(b);
    }
    return mk_binary(Str(o), a, b, BOOL(), ln);
}

NodeP FnLower::e_IfExp(const A::IfExp* n) {
    TypeP t = ty(Tn(n), n);
    NodeP c = cond(n->test);
    std::vector<NodeP> saved = pre;
    pre.clear();
    NodeP a = coerce(expr(n->body), t);
    NodeP b = coerce(expr(n->orelse), t);
    if (!pre.empty()) throw Refused(Str("syntax"), Str("a conditional branch that needs statements"), n->pos.lineno);
    pre = saved;
    auto ce = mk<ir::Cond>(n->pos.lineno);
    ce->cond = c;
    ce->then = a;
    ce->orelse = b;
    ce->type = t;
    return ce;
}

NodeP FnLower::e_JoinedStr(const A::JoinedStr* n) {
    std::vector<NodeP> parts;
    for (const A::Expr* v : n->values) {
        if (auto* c = A::as<A::Constant>(v)) {
            bool truthy_ = !(c->value.kind == A::ConstValue::Kind::Str && c->value.text.empty());
            if (truthy_) parts.push_back(lit(Val(S(c->value.text)), STR()));
            continue;
        }
        auto* fv = A::cast<A::FormattedValue>(v);
        NodeP x = expr(fv->value);
        if (fv->format_spec != nullptr) {
            auto* spec_node = A::cast<A::JoinedStr>(fv->format_spec);
            Str spec;
            int64_t idx = 0;
            for (const A::Expr* c : spec_node->values) {
                auto* cc = A::as<A::Constant>(c);
                if (!cc) {
                    throw py::TypeError("sequence item " + std::to_string(idx) + ": expected str instance, " +
                                        tname(A::cast<A::FormattedValue>(c)->value) + " found");
                }
                spec = spec + S(cc->value.text);
                ++idx;
            }
            parts.push_back(pyfn(Str("format"), {x, lit(Val(spec), STR())}, STR()));
        } else if (fv->conversion == 114) {
            parts.push_back(pyfn(Str("repr"), {x}, STR()));
        } else if (tsame(type_of(x), STR())) {
            parts.push_back(x);
        } else {
            parts.push_back(pyfn(Str("str"), {x}, STR()));
        }
    }
    if (parts.empty()) return lit(Val(Str("")), STR());
    if (parts.size() == 1) return tsame(type_of(parts[0]), STR()) ? parts[0] : pyfn(Str("str"), parts, STR());
    return mk_concat(parts, STR(), n->pos.lineno);
}

NodeP FnLower::e_Tuple(const A::Tuple* n) {
    TypeP t = ty(Tn(n), n);
    std::vector<NodeP> es;
    for (const A::Expr* e : n->elts) es.push_back(expr(e));
    return pyfn(Str("tuple"), es, t, nullptr, n->pos.lineno);
}

NodeP FnLower::e_List(const A::List* n) {
    TypeP t = ty(Tn(n), n);
    std::vector<NodeP> es;
    for (const A::Expr* e : n->elts) {
        NodeP x = expr(e);
        es.push_back(coerce(x, t->args[0]));
    }
    return pyfn(Str("list"), es, t, nullptr, n->pos.lineno);
}

NodeP FnLower::e_Set(const A::Set* n) {
    TypeP t = ty(Tn(n), n);
    std::vector<NodeP> es;
    for (const A::Expr* e : n->elts) {
        NodeP x = expr(e);
        es.push_back(coerce(x, t->args[0]));
    }
    return pyfn(Str("set"), es, t, nullptr, n->pos.lineno);
}

NodeP FnLower::e_Dict(const A::Dict* n) {
    const T t = Tn(n);
    if (p.is_record(*t.site)) return record_value(n);
    TypeP rt = ty(t, n);
    std::vector<NodeP> args;
    for (size_t i = 0; i < n->keys.size(); ++i) {
        if (n->keys[i] == nullptr) throw Refused(Str("syntax"), Str("** in a dict"), n->pos.lineno);
        NodeP k = coerce(expr(n->keys[i]), rt->args[0]);
        NodeP v = coerce(expr(n->values[i]), rt->args[1]);
        args.push_back(pyfn(Str("tuple"), {k, v}, clst(Str("py.tuple"), std::vector<TypeP>(rt->args.vec().begin(), rt->args.vec().end()))));
    }
    return pyfn(Str("dict"), args, rt, nullptr, n->pos.lineno);
}

NodeP FnLower::lambda_expr(const A::Lambda* lam, const std::vector<TypeP>& ptypes) {
    std::vector<NodeP> params_;
    size_t n = std::min(lam->args->args.size(), ptypes.size());
    for (size_t i = 0; i < n; ++i) {
        Str an = S(lam->args->args[i]->arg);
        local_types[an] = ptypes[i];
        auto pr = mk<ir::Param>();
        pr->sym = msym("param", an, std::optional<Str>(qname), false, ptypes[i]);
        pr->type = ptypes[i];
        pr->mods = py::list<Str>();
        params_.push_back(pr);
    }
    std::vector<NodeP> saved = pre;
    pre.clear();
    NodeP body = expr(lam->body);
    if (!pre.empty()) throw Refused(Str("syntax"), Str("a lambda that needs statements"), lam->pos.lineno);
    pre = saved;
    auto l = mk<ir::Lambda>();
    l->params = nl(params_);
    l->body = body;
    l->type = type_of(body);
    return l;
}

// whether evaluating `e` can do anything but read (a call, an assignment)
bool FnLower::effects(const NodeP& e) {
    for (const NodeP& x : ir::walk(e)) {
        if (is_a<ir::Assign>(x) || is_a<ir::New>(x)) return true;
        if (auto c = cast_to<ir::Call>(x)) {
            if (c->sym && c->sym->owner && eq(*c->sym->owner, PY)) {
                static const char* pure_[] = {"tuple", "get", "unwrap", "truthy", "is_none", "is_not_none", "opt_value", "list", "str", "empty", "record", "len"};
                bool ok = false;
                for (const char* nm : pure_) {
                    if (eq(c->sym->name, nm)) ok = true;
                }
                if (ok) continue;
            }
            return true;
        }
        if (is_a<ir::Index>(x)) return true;   // can raise
    }
    return false;
}

// -- calls
NodeP FnLower::e_Call(const A::Call* n) {
    auto r0 = R(n);
    int64_t ln = n->pos.lineno;
    if (!r0) throw Refused(Str("dynamic"), Str("call"), ln);
    const Res& r = *r0;
    const Str& kind_ = r.kind;
    const T t = Tn(n);
    if (eq(kind_, "fn") || eq(kind_, "method") || eq(kind_, "super") || eq(kind_, "basecall")) {
        Fn* fn = p.fns.at(r.s(1));
        calls.insert(fn->qname);
        std::vector<NodeP> args = call_args(n, fn, !eq(kind_, "fn") || eq(fn->kind, "method"));
        TypeP rt = call_ret(fn, n);
        if (eq(kind_, "fn")) {
            if (fn->cls != nullptr && eq(fn->kind, "static")) {
                return mk_call(mk_typename(clst(fn->cls->qname)), msym("method", fn->name, std::optional<Str>(fn->cls->qname), true, nullptr, {}, rt), args, rt, ln);
            }
            return mk_call(nullptr, msym("function", fn->name, std::optional<Str>(fn->module->name), true, nullptr, {}, rt), args, rt, ln);
        }
        if (eq(kind_, "super") || eq(kind_, "basecall")) {
            if (eq(fn->name, "__init__")) {
                auto c = mk_call(nullptr, msym("ctor", Str("__init__"), std::optional<Str>(fn->cls->qname)), args, VOID(), ln);
                cast_to<ir::Call>(c)->typeargs = py::list<Val>({Val(Str("base"))});
                return c;
            }
            auto c = mk_call(mk_typename(clst(fn->cls->qname)), msym("method", fn->name, std::optional<Str>(fn->cls->qname), false, nullptr, {}, rt), args, rt, ln);
            cast_to<ir::Call>(c)->typeargs = py::list<Val>({Val(Str("base"))});
            return c;
        }
        NodeP recv;
        if (auto* at_ = A::as<A::Attribute>(n->func)) recv = expr(at_->value);
        return mk_call(recv, msym("method", fn->name, std::optional<Str>(fn->cls->qname), false, nullptr, {}, rt), args, rt, ln);
    }
    if (eq(kind_, "ctor")) {
        Str cq = r.s(1);
        Class* c = p.classes.at(cq);
        Fn* init = p.find_method(cq, Str("__init__"));
        std::vector<NodeP> args;
        if (init) args = call_args(n, init, true);
        if (init) calls.insert(init->qname);
        classes_made.insert(cq);
        if (c->exception && !c->exception->empty()) {
            if (!init) {
                args.clear();
                for (const A::Expr* a : n->args) args.push_back(expr(a));
                if (!args.empty() && !tsame(type_of(args[0]), STR())) args = {pyfn(Str("str"), args, STR())};
            }
            TypeP ct = clst(cq, {}, info1("exception", true));
            return mk_new(msym("ctor", c->name, std::optional<Str>(cq)), args, ct, std::optional<Str>(Str("stack")), ln);
        }
        TypeP ct = clst(cq, {}, info1("ref", true));
        return mk_new(msym("ctor", c->name, std::optional<Str>(cq)), args, ct, std::optional<Str>(Str("gc")), ln);
    }
    if (eq(kind_, "exc")) {
        std::vector<NodeP> args;
        for (const A::Expr* a : n->args) args.push_back(expr(a));
        if (!args.empty() && !tsame(type_of(args[0]), STR())) args = {pyfn(Str("str"), args, STR())};
        Str name = r.s(1).replace(Str("struct.error"), Str("StructError"));
        return mk_new(msym("ctor", name, std::optional<Str>(Str(PY))), args, clst(Str("py.") + name), std::nullopt, ln);
    }
    if (eq(kind_, "builtin")) return builtin_call(n, r.s(1), t);
    if (eq(kind_, "bmethod")) return method_call(n, r.s(1), t);
    if (eq(kind_, "std")) return std_call(n, r, t);
    if (eq(kind_, "isinstance")) {
        for (const A::Expr* a : n->args) {
            if (effects(expr(a))) throw Refused(Str("syntax"), Str("isinstance of a computed value"), ln);
        }
        bool ans = false;
        if (r.items.empty()) throw py::IndexError("tuple index out of range");
        const TupItem& it = r.items[0];
        if (auto* b = std::get_if<bool>(&it)) ans = *b;
        else if (auto* bg = std::get_if<Big>(&it)) ans = *bg != Big(0);
        else if (auto* s = std::get_if<Str>(&it)) ans = !s->empty();
        return lit(Val(ans), BOOL());
    }
    throw Refused(Str("dynamic"), Str("call (") + kind_ + Str(")"), ln);
}

TypeP FnLower::call_ret(Fn* fn, const A::Call* n) {
    const T t = Tn(n);
    if (eq(fn->name, "__init__")) return VOID();
    return !tk(t, "none") ? ty(t, n) : VOID();
}

std::vector<NodeP> FnLower::call_args(const A::Call* n, Fn* fn, bool bound) {
    std::vector<Str> names = (bound || !(eq(fn->kind, "method") || eq(fn->kind, "property"))) ? fn->bound() : fn->params;
    if (eq(fn->kind, "static")) names = fn->params;
    std::map<Str, const A::Expr*> given;
    size_t skip = 0;
    auto rr = R(n);
    if (A::as<A::Attribute>(n->func) && rr && eq(rr->kind, "basecall")) skip = 1;
    for (size_t i = 0; i < names.size() && skip + i < n->args.size(); ++i) given[names[i]] = n->args[skip + i];
    for (const A::keyword* k : n->keywords) {
        if (k->has_arg) given[S(k->arg)] = k->value;
    }
    std::vector<NodeP> out;
    std::vector<Str> all = names;
    all.insert(all.end(), fn->kwonly.begin(), fn->kwonly.end());
    for (const Str& name : all) {
        TypeP pt = ty(p.param_type(*fn, name), n, Str("parameter ") + name);
        auto g = given.find(name);
        if (g != given.end()) {
            NodeP e = expr(g->second);
            out.push_back(coerce(e, pt));
        } else if (fn->defaults.contains(name)) {
            const A::Expr* d = fn->defaults.at(name);
            auto* dc = A::as<A::Constant>(d);
            if (!dc) throw Refused(Str("syntax"), Str("a computed default of ") + name, line(n));
            out.push_back(coerce(default_lit(dc), pt));
        } else {
            throw Refused(Str("conflict"), Str("missing argument ") + name, line(n));
        }
    }
    return out;
}

NodeP FnLower::default_lit(const A::Constant* d) {
    using K = A::ConstValue::Kind;
    const A::ConstValue& v = d->value;
    if (v.kind == K::None) return lit(Val(), VOID());
    if (v.kind == K::True || v.kind == K::False) return lit(Val(v.kind == K::True), BOOL());
    if (v.kind == K::Int) return lit(Val(as_i64(Big::from_decimal(v.text))), I64());
    if (v.kind == K::Str) return lit(Val(S(v.text)), STR());
    if (v.kind == K::Bytes) return lit(Val(latin1(v.text)), BYTES());
    if (v.kind == K::Float) return lit(Val(v.f), F64());
    throw Refused(Str("syntax"), Str("default"), 0);
}

NodeP FnLower::builtin_call(const A::Call* n, const Str& name, const T& t) {
    int64_t ln = n->pos.lineno;
    TypeP rt = !tk(t, "none") ? ty(t, n) : VOID();
    if (eq(name, "any") || eq(name, "all")) return any_all(n, eq(name, "any") ? "any" : "all");
    std::vector<NodeP> args;
    for (const A::Expr* a : n->args) {
        if (auto* g = A::as<A::GeneratorExp>(a)) args.push_back(gen_as_list(g));
        else args.push_back(expr(a));
    }
    auto kw = [&](const char* nm) -> const A::Expr* {
        for (const A::keyword* k : n->keywords) {
            if (k->has_arg && k->arg == nm) return k->value;
        }
        return nullptr;
    };
    if (eq(name, "sorted") || eq(name, "list.sort")) {
        const A::Expr* key = kw("key");
        const A::Expr* rev = kw("reverse");
        std::vector<NodeP> extra;
        if (key != nullptr) {
            TypeP et = rt->args[0];
            auto* lam = A::as<A::Lambda>(key);
            if (!lam) throw Refused(Str("dynamic"), Str("key function"), ln);
            extra.push_back(lambda_expr(lam, {et}));
        } else {
            extra.push_back(lit(Val(), VOID()));
        }
        if (rev != nullptr) extra.push_back(truthy(expr(rev)));
        std::vector<NodeP> all = args;
        all.insert(all.end(), extra.begin(), extra.end());
        return pyfn(Str("sorted"), all, rt, nullptr, ln);
    }
    if (eq(name, "dict")) {
        if (is_record_literal(n)) return record_value(n);
        throw Refused(Str("syntax"), Str("dict() call"), ln);
    }
    if (eq(name, "min") || eq(name, "max") || eq(name, "min_iter") || eq(name, "max_iter")) {
        for (const A::keyword* k : n->keywords) {
            if (k->has_arg && k->arg == "default") throw Refused(Str("syntax"), name + Str("(default=)"), ln);
        }
        for (NodeP& a : args) a = coerce(a, rt);
    }
    if (eq(name, "print")) {
        for (const A::keyword* k : n->keywords) {
            NodeP v = expr(k->value);
            args.push_back(pyfn(Str("kw"), {v}, VOID(), nullptr, 0, targs({k->has_arg ? Val(S(k->arg)) : Val()})));
        }
    } else if (!n->keywords.empty() && !eq(name, "enumerate")) {
        throw Refused(Str("syntax"), name + Str(" keywords"), ln);
    }
    if (eq(name, "enumerate") && !n->keywords.empty()) args.push_back(expr(n->keywords[0]->value));
    bool plain = eq(name, "sorted") || eq(name, "enumerate") || eq(name, "print") || eq(name, "zip") || eq(name, "reversed");
    return pyfn(plain ? name : Str("builtins.") + name, args, rt, nullptr, ln);
}

NodeP FnLower::method_call(const A::Call* n, const Str& name, const T& t) {
    int64_t ln = n->pos.lineno;
    TypeP rt = !tk(t, "none") ? ty(t, n) : VOID();
    auto* fattr = A::cast<A::Attribute>(n->func);
    NodeP recv = expr(fattr->value);
    if (eq(name, "record.get")) {
        auto key = program::str_const(n->args[0]);
        const T st = Tn(fattr->value);
        std::optional<Records::Field> found;
        for (const Records::Field& f : ctx.records.fields(*st.site)) {
            if (key && f.key == *key) found = f;
        }
        if (!found) return n->args.size() > 1 ? coerce(expr(n->args[1]), rt) : lit(Val(), rt);
        NodeP sel = field_ref(recv, *key, fattr->value, ln, false, true);
        if (found->required) return coerce(sel, rt);
        if (n->args.size() < 2 || is_none_const(n->args[1])) return sel;   // .get(k) of an optional field: the optional itself
        NodeP d = n->args.size() > 1 ? coerce(expr(n->args[1]), rt) : lit(Val(), rt);
        return pyfn(Str("value_or"), {sel, d}, rt, nullptr, ln);
    }
    if (eq(name, "list.sort")) return builtin_call_sort(n, recv);
    std::vector<NodeP> args;
    for (const A::Expr* a : n->args) {
        if (auto* g = A::as<A::GeneratorExp>(a)) args.push_back(gen_as_list(g));
        else args.push_back(expr(a));
    }
    for (const A::keyword* k : n->keywords) {
        NodeP v = expr(k->value);
        args.push_back(pyfn(Str("kw"), {v}, VOID(), nullptr, 0, targs({k->has_arg ? Val(S(k->arg)) : Val()})));
    }
    const TypeP& rtp = type_of(recv);
    if ((eq(name, "list.append") || eq(name, "list.insert") || eq(name, "set.add")) && kind_is(rtp, "class") && !rtp->args.empty()) {
        args.back() = coerce(args.back(), rtp->args[0]);
    }
    if (eq(name, "bytes.decode") || eq(name, "str.encode")) {
        auto kwv = [&](const char* nm) -> const A::Expr* {
            for (const A::keyword* k : n->keywords) {
                if (k->has_arg && k->arg == nm) return k->value;
            }
            return nullptr;
        };
        std::optional<Str> enc, errs;
        if (!n->args.empty()) {
            enc = program::str_const(n->args[0]);
        } else if (kwv("encoding")) {
            enc = program::str_const(kwv("encoding"));
        } else {
            enc = Str("utf-8");
        }
        if (n->args.size() > 1) {
            errs = program::str_const(n->args[1]);
        } else if (kwv("errors")) {
            errs = program::str_const(kwv("errors"));
        } else {
            errs = Str("strict");
        }
        if (!enc) throw py::AttributeError("'NoneType' object has no attribute 'lower'");
        Str el = enc->lower();
        Str e2 = el;
        if (eq(el, "latin-1") || eq(el, "iso-8859-1")) e2 = Str("latin1");
        else if (eq(el, "utf8")) e2 = Str("utf-8");
        return pyfn(name, {lit(Val(e2), STR()), lit(val_of(errs), STR())}, rt, recv, ln);
    }
    return pyfn(name, args, rt, recv, ln);
}

NodeP FnLower::builtin_call_sort(const A::Call* n, const NodeP& recv) {
    const A::Expr *key = nullptr, *rev = nullptr;
    for (const A::keyword* k : n->keywords) {
        if (k->has_arg && k->arg == "key" && !key) key = k->value;
        if (k->has_arg && k->arg == "reverse" && !rev) rev = k->value;
    }
    std::vector<NodeP> args;
    TypeP et = type_of(recv)->args[0];
    if (key != nullptr) {
        auto* lam = A::as<A::Lambda>(key);
        if (!lam) throw Refused(Str("dynamic"), Str("key function"), n->pos.lineno);
        args.push_back(lambda_expr(lam, {et}));
    } else {
        args.push_back(lit(Val(), VOID()));
    }
    if (rev != nullptr) args.push_back(truthy(expr(rev)));
    return pyfn(Str("list.sort"), args, VOID(), recv, n->pos.lineno);
}

NodeP FnLower::std_call(const A::Call* n, const Res& r, const T& t) {
    int64_t ln = n->pos.lineno;
    TypeP rt = !tk(t, "none") ? ty(t, n) : VOID();
    Str name = r.s(1);
    std::vector<NodeP> args;
    for (const A::Expr* a : n->args) args.push_back(expr(a));
    if (starts(name, "struct.")) {
        Str fmt = r.s(2);
        return pyfn(name, args, rt, nullptr, ln, targs({Val(fmt)}));
    }
    if (eq(name, "int.from_bytes")) {
        std::optional<Str> order;
        const A::Expr* bo = nullptr;
        for (const A::keyword* k : n->keywords) {
            if (k->has_arg && k->arg == "byteorder") {
                bo = k->value;
                break;
            }
        }
        if (n->args.size() > 1) {
            order = program::str_const(n->args[1]);
        } else {
            std::optional<Str> o2 = bo ? program::str_const(bo) : std::nullopt;
            order = (o2 && !o2->empty()) ? o2 : std::optional<Str>(Str("big"));
        }
        bool signed_ = false;
        for (const A::keyword* k : n->keywords) {
            if (k->has_arg && k->arg == "signed") {
                if (auto* c = A::as<A::Constant>(k->value)) {
                    using K = A::ConstValue::Kind;
                    const A::ConstValue& v = c->value;
                    switch (v.kind) {
                        case K::None:
                        case K::False: signed_ = false; break;
                        case K::True:
                        case K::Ellipsis: signed_ = true; break;
                        case K::Int: signed_ = v.text != "0"; break;
                        case K::Float:
                        case K::Complex: signed_ = v.f != 0.0; break;
                        case K::Str:
                        case K::Bytes: signed_ = !v.text.empty(); break;
                    }
                } else if (A::as<A::Attribute>(k->value) || A::as<A::Subscript>(k->value) || A::as<A::Starred>(k->value)) {
                    signed_ = true;   // its .value is a node: true
                } else {
                    throw py::AttributeError("'" + tname(k->value) + "' object has no attribute 'value'");
                }
                break;
            }
        }
        return pyfn(name, {args[0], lit(val_of(order), STR()), lit(Val(signed_), BOOL())}, rt, nullptr, ln);
    }
    if (!n->keywords.empty()) throw Refused(Str("syntax"), name + Str(" keywords"), ln);
    return pyfn(name, args, rt, nullptr, ln);
}

// ------------------------------------------------------------------------------------------ modules
py::dict<Str, Lowered> lower_functions(Program& prog, Context& ctx, const std::vector<Fn*>& fns) {
    py::dict<Str, Lowered> out;
    for (Fn* f : fns) {
        std::shared_ptr<infer::Eval> ev;
        try {
            ev = prog.evaluate(*f);
        } catch (const py::Exception& e) {
            Lowered l;
            l.refused = Refused(Str("frontend-error"), Str(e.type()) + Str(": ") + Str(std::string(e.what())), f->node->pos.lineno);
            out.set(f->qname, l);
            continue;
        }
        auto fl = std::make_shared<FnLower>(ctx, *f, ev);
        Lowered l;
        l.fl = fl;
        try {
            l.decl = fl->lower();
        } catch (const Refused& r) {
            l.refused = r;
        } catch (const py::Exception& e) {   // a front end bug: refused, with the error
            l.refused = Refused(Str("frontend-error"), Str(e.type()) + Str(": ") + Str(std::string(e.what())), f->node->pos.lineno);
        }
        out.set(f->qname, l);
    }
    return out;
}

void close_refusals(Program& prog, py::dict<Str, Lowered>& lowered) {
    (void)prog;
    bool changed = true;
    while (changed) {
        changed = false;
        std::vector<Str> keys;
        for (const auto& kv : lowered.items()) keys.push_back(std::get<0>(kv));
        for (const Str& q : keys) {
            Lowered cur = lowered.at(q);
            if (cur.is_refused() || !cur.fl) continue;
            for (const Str& c : cur.fl->calls) {
                if (!lowered.contains(c)) {
                    Lowered nl_ = cur;
                    nl_.decl = nullptr;
                    nl_.refused = Refused(Str("calls-refused"), program::last_part(c) + Str(" (not in the translation)"), cur.decl->line);
                    lowered.set(q, nl_);
                    changed = true;
                    break;
                }
                const Lowered& got = lowered.at(c);
                if (got.is_refused()) {
                    // c.split('.', 2)[-1]: what follows the second dot
                    Str tail = c;
                    int64_t d1 = c.find(Str("."));
                    if (d1 >= 0) {
                        int64_t d2 = c.find(Str("."), d1 + 1);
                        tail = d2 >= 0 ? c.slice_from(d2 + 1) : c.slice_from(d1 + 1);
                    }
                    Str reason = got.refused->issue.reason();
                    if (reason.size() > 60) reason = reason.slice(0, 60);
                    Lowered nl_ = cur;
                    nl_.decl = nullptr;
                    nl_.refused = Refused(Str("calls-refused"), tail + Str(" (") + reason + Str(")"), cur.decl->line);
                    lowered.set(q, nl_);
                    changed = true;
                    break;
                }
            }
        }
    }
}

}  // namespace codebreaker::translate::frontends::python::lower

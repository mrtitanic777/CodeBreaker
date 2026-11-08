// codebreaker/translate/frontends/clang/lower.cpp -- typed C++ port of
// codebreaker.translate.frontends.clang.lower (see lower.hpp).

#include "codebreaker/translate/frontends/clang/lower.hpp"

#include <functional>
#include <vector>

#include "json.hpp"

namespace codebreaker::translate::frontends::clang::lower {

using ir::OptStr;

namespace {

py::str K(const char* s) { return py::str(s); }

// ---------------------------------------------------------------------------------------------- tables
// _LOC_KEYS
const char* const LOC_KEYS[] = {"spellingLoc", "expansionLoc"};

// _OPS
bool in_ops(const OptStr& op) {
    if (!op) return false;
    static const char* const OPS[] = {"+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^", "&&", "||", "==", "!=", "<", "<=", ">", ">=", ","};
    for (const char* o : OPS) {
        if (*op == o) return true;
    }
    return false;
}

bool in_list(const OptStr& s, std::initializer_list<const char*> xs) {
    if (!s) return false;
    for (const char* x : xs) {
        if (*s == x) return true;
    }
    return false;
}

// _DROP_CASTS
bool in_drop_casts(const OptStr& k) {
    return in_list(k, {"LValueToRValue", "NoOp", "FunctionToPointerDecay", "LValueToRValueBitCast", "AtomicToNonAtomic",
                       "NonAtomicToAtomic"});
}
// _WRAPPERS
bool in_wrappers(const OptStr& k) {
    return in_list(k, {"ExprWithCleanups", "MaterializeTemporaryExpr", "CXXBindTemporaryExpr", "FullExpr",
                       "SubstNonTypeTemplateParmExpr", "CXXDefaultInitExpr", "ConstantExpr"});
}
// _CASTS
bool in_casts(const OptStr& k) {
    return in_list(k, {"CStyleCastExpr", "CXXStaticCastExpr", "CXXReinterpretCastExpr", "CXXConstCastExpr", "CXXFunctionalCastExpr",
                       "CXXDynamicCastExpr", "BuiltinBitCastExpr"});
}

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

// type(v).__name__
std::string pyname(const Val& v) {
    if (v.is_none()) return "NoneType";
    if (holds<bool>(v)) return "bool";
    if (holds<int64_t>(v)) return "int";
    if (holds<double>(v)) return "float";
    if (holds<py::str>(v)) return "str";
    if (holds<ir::TypeP>(v)) return "Type";
    if (holds<Val::List>(v)) return "list";
    return "dict";
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

bool is_str_val(const Val& v, const char* s) { return holds<py::str>(v) && as<py::str>(v) == K(s); }

[[noreturn]] void type_error(const std::string& m) { throw py::TypeError(m); }
[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

bool has(const Dict& d, const char* key) { return d.contains(K(key)); }
Val vget(const Dict& d, const char* key) { return d.get(K(key), Val()); }                         // d.get(key)
Val vget(const Dict& d, const char* key, const Val& dflt) { return d.get(K(key), dflt); }          // d.get(key, dflt)
// d[key]: KeyError (whose text is the repr of the key, as Python's)
Val vat(const Dict& d, const char* key) {
    auto it = d.get(K(key));
    if (!it) throw py::KeyError(py::repr(K(key)));
    return *it;
}

// the object `v` used as `v.get(...)`: AttributeError for what is not a dict
Dict as_dict_get(const Val& v) {
    if (!holds<Dict>(v)) throw py::AttributeError("'" + pyname(v) + "' object has no attribute 'get'");
    return as<Dict>(v);
}
// the object `v` used as `v[...]`: TypeError for what is not a dict
Dict as_dict_sub(const Val& v) {
    if (!holds<Dict>(v)) type_error("'" + pyname(v) + "' object is not subscriptable");
    return as<Dict>(v);
}

// `v or {}`, then `.get(...)`
Dict or_empty(const Val& v) { return is_truthy(v) ? as_dict_get(v) : Dict(); }

// d[k1][k2]
Val vat2(const Dict& d, const char* k1, const char* k2) { return vat(as_dict_sub(vat(d, k1)), k2); }

// v as a str (the argument of a str method)
py::str str_in(const Val& v) {
    if (!holds<py::str>(v)) type_error("expected str, got '" + pyname(v) + "'");
    return as<py::str>(v);
}

// the text handed to the type parser: a str (the parser's regex raises TypeError for anything else)
py::str parse_text(const Val& v) {
    if (!holds<py::str>(v)) type_error("expected string or bytes-like object, got '" + pyname(v) + "'");
    return as<py::str>(v);
}

// `v.method(...)` on a JSON value: the str, AttributeError for what is not one
py::str str_for(const Val& v, const char* method) {
    if (!holds<py::str>(v)) throw py::AttributeError("'" + pyname(v) + "' object has no attribute '" + method + "'");
    return as<py::str>(v);
}

// d.get(key) as an optional str (None for absent and null)
OptStr opt_str(const Dict& d, const char* key) {
    Val v = vget(d, key);
    if (v.is_none()) return std::nullopt;
    return str_in(v);
}

// a JSON value that is a str or None
OptStr opt_of(const Val& v) {
    if (v.is_none()) return std::nullopt;
    return str_in(v);
}

// str(v)
py::str str_of(const Val& v) {
    if (holds<py::str>(v)) return as<py::str>(v);
    if (v.is_none()) return K("None");
    if (holds<bool>(v)) return py::to_str(as<bool>(v));
    if (holds<int64_t>(v)) return py::to_str(as<int64_t>(v));
    if (holds<double>(v)) return py::to_str(as<double>(v));
    return v.repr();
}

// int(v)
int64_t to_int(const Val& v) {
    if (holds<int64_t>(v)) return as<int64_t>(v);
    if (holds<bool>(v)) return as<bool>(v) ? 1 : 0;
    if (holds<py::str>(v)) return py::int_parse(as<py::str>(v), 10);
    if (holds<double>(v)) return py::int_from_float(as<double>(v));
    type_error("int() argument must be a string, a bytes-like object or a real number, not '" + pyname(v) + "'");
}

// float(v)
double to_float(const Val& v) {
    if (holds<double>(v)) return as<double>(v);
    if (holds<int64_t>(v)) return double(as<int64_t>(v));
    if (holds<bool>(v)) return as<bool>(v) ? 1.0 : 0.0;
    if (holds<py::str>(v)) return py::float_from_str(as<py::str>(v));
    type_error("float() argument must be a string or a real number, not '" + pyname(v) + "'");
}

// n.get('l', 0) / n.get('c', 0): a number
int64_t number_of(const Val& v) {
    if (v.is_none()) return 0;
    if (holds<int64_t>(v)) return as<int64_t>(v);
    if (holds<bool>(v)) return as<bool>(v) ? 1 : 0;
    type_error("not a number: " + v.repr().utf8());
}

// the key of an id in a py::dict<str, ...> (an id that is not a str, None included, is its own key)
py::str val_key(const Val& v) {
    if (holds<py::str>(v)) return as<py::str>(v);
    return K("\x01") + v.repr();
}
py::str key_of(const OptStr& s) { return s ? *s : K("\x01None"); }

template <class V> std::optional<V> dget(const py::dict<py::str, V>& d, const Val& k) { return d.get(val_key(k)); }

// d.get(key, []) iterated: the nodes of the list (a missing key is an empty list; a null is not iterable)
py::list<Dict> inner_get(const Dict& d, const char* key) {
    py::list<Dict> out;
    auto it = d.get(K(key));
    if (!it) return out;
    if (it->is_none()) type_error("'NoneType' object is not iterable");
    if (!holds<Val::List>(*it)) type_error("'" + pyname(*it) + "' object is not iterable");
    for (const Val& x : as<Val::List>(*it)) out.append(as_dict_get(x));
    return out;
}

// (d.get(key) or []) iterated
py::list<Dict> inner_or(const Dict& d, const char* key) {
    py::list<Dict> out;
    Val v = vget(d, key);
    if (!is_truthy(v)) return out;
    if (!holds<Val::List>(v)) type_error("'" + pyname(v) + "' object is not iterable");
    for (const Val& x : as<Val::List>(v)) out.append(as_dict_get(x));
    return out;
}

// d[key][i]
Dict sub_at(const Dict& d, const char* key, int64_t i) {
    Val v = vat(d, key);
    if (!holds<Val::List>(v)) type_error("'" + pyname(v) + "' object is not subscriptable");
    return as_dict_get(as<Val::List>(v)[i]);
}

// n.get('kind') (a str, or None)
OptStr kind_of(const Dict& n) { return opt_str(n, "kind"); }

// n.get('kind', '') used as a str
py::str kind_s(const Dict& n) {
    Val v = vget(n, "kind", Val(K("")));
    if (v.is_none()) none_attr("endswith");
    if (!holds<py::str>(v)) throw py::AttributeError("'" + pyname(v) + "' object has no attribute 'endswith'");
    return as<py::str>(v);
}

bool kind_is(const Dict& n, const char* k) { return is_str_val(vget(n, "kind"), k); }

// `'prefix' + s`, TypeError for a null
py::str cat(const char* prefix, const OptStr& s) {
    if (!s) type_error("can only concatenate str (not \"NoneType\") to str");
    return K(prefix) + *s;
}

// d.get(key) as a Dict, for a key that holds an object ({} when absent or null)
Dict obj_or_empty(const Dict& d, const char* key) { return or_empty(vget(d, key)); }

// (the tail of s.rsplit('.', 1))
py::str last_segment(const py::str& s) {
    int64_t i = s.rfind(K("."));
    return i < 0 ? s : s.slice(i + 1, s.size());
}
// s.split('<')[0]
py::str before_lt(const py::str& s) {
    int64_t i = s.find(K("<"));
    return i < 0 ? s : s.slice(0, i);
}
py::str join_dot(const std::vector<py::str>& ns, const py::str& last) {
    py::str out;
    for (const py::str& x : ns) out = out + x + K(".");
    return out + last;
}

// Python's `except (TypeError, KeyError, ValueError, IndexError)`
bool soft_error(const py::Exception& e) {
    return dynamic_cast<const py::TypeError*>(&e) || dynamic_cast<const py::KeyError*>(&e) ||
           dynamic_cast<const py::ValueError*>(&e) || dynamic_cast<const py::IndexError*>(&e);
}
py::str message_of(const py::Exception& e) { return py::str(std::string(e.what())); }

// ---------------------------------------------------------------------------------------------- IR helpers
TypeP mk_type(const char* kind, const OptStr& name = std::nullopt, const py::list<TypeP>& args = py::list<TypeP>(),
              const TypeP& elem = nullptr, const Dict& info = Dict()) {
    return std::make_shared<ir::Type>(K(kind), name, args, elem, std::nullopt, info);
}

TypeP ptr_to(const TypeP& elem) { return ir::pointer_to(elem, K("flat")); }

SymP mk_sym(const char* kind, const py::str& name, const OptStr& owner = std::nullopt, bool static_ = false,
            const TypeP& type = nullptr) {
    return std::make_shared<ir::Sym>(K(kind), name, owner, static_, type);
}

// a Sym's name from a JSON value (the Python keeps whatever it is; the slot is a str here)
py::str sym_name(const Val& v) {
    if (v.is_none()) return K("");   // (a symbol without a name: the Python keeps the None, a Sym's name is a str here)
    if (!holds<py::str>(v)) type_error("a symbol's name must be a str, not '" + pyname(v) + "'");
    return as<py::str>(v);
}

template <class T> py::ref<T> mk(const Pos& p = Pos()) {
    auto n = std::make_shared<T>();
    n->line = p.line;
    n->col = p.col;
    return n;
}
template <class T> py::ref<T> mke(const TypeP& t, const Pos& p = Pos()) {
    auto n = mk<T>(p);
    n->type = t;
    return n;
}

template <class T> py::ref<T> as_node(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }

// `x.type` of a node (AttributeError for None, and for a node that has no type slot)
TypeP type_of(const NodeP& n) {
    if (!n) none_attr("type");
    auto e = as_node<ir::Expr>(n);
    if (!e) throw py::AttributeError("'" + n->class_name().utf8() + "' object has no attribute 'type'");
    return e->type;
}
// `t.kind` (AttributeError for None)
const py::str& kind_attr(const TypeP& t) {
    if (!t) none_attr("kind");
    return t->kind;
}
// t.info.get(key)
Val info_get(const TypeP& t, const char* key) {
    if (!t) none_attr("info");
    return t->info.get(K(key), Val());
}
bool is_ref_type(const TypeP& t) { return is_truthy(info_get(t, "ref")); }
// a JSON value that holds a Type (or None)
TypeP type_val(const Val& v) {
    if (v.is_none()) return nullptr;
    if (!holds<TypeP>(v)) type_error("not a Type: " + v.repr().utf8());
    return as<TypeP>(v);
}

py::ref<ir::ExprStmt> mk_expr_stmt(const NodeP& e, const Pos& p = Pos()) {
    auto s = mk<ir::ExprStmt>(p);
    s->expr = e;
    return s;
}
py::ref<ir::Block> mk_block(const py::list<NodeP>& stmts, const Pos& p = Pos()) {
    auto b = mk<ir::Block>(p);
    b->stmts = stmts;
    return b;
}
py::ref<ir::Unary> mk_unary(const char* op, const NodeP& x, const TypeP& t, const Pos& p = Pos()) {
    auto u = mke<ir::Unary>(t, p);
    u->op = K(op);
    u->x = x;
    u->postfix = false;
    return u;
}
py::ref<ir::Lit> mk_lit(const Val& value, const TypeP& t, const Pos& p = Pos(), const OptStr& text = std::nullopt) {
    auto l = mke<ir::Lit>(t, p);
    l->value = value;
    l->text = text;
    return l;
}
py::ref<ir::Cast> mk_cast(const NodeP& x, const TypeP& t, const Pos& p, bool implicit) {
    auto c = mke<ir::Cast>(t, p);
    c->x = x;
    if (implicit) c->implicit = true;
    return c;
}
py::ref<ir::Select> mk_select(const NodeP& obj, const SymP& s, const TypeP& t, const Pos& p) {
    auto e = mke<ir::Select>(t, p);
    e->obj = obj;
    e->sym = s;
    return e;
}
py::ref<ir::TypeName> mk_typename(const TypeP& t, const Pos& p) { return mke<ir::TypeName>(t, p); }
py::ref<ir::Name> mk_name(const SymP& s, const TypeP& t, const Pos& p) {
    auto e = mke<ir::Name>(t, p);
    e->sym = s;
    return e;
}
py::ref<ir::New> mk_new(const SymP& s, const py::list<NodeP>& args, const char* memory, const TypeP& t, const Pos& p) {
    auto e = mke<ir::New>(t, p);
    e->sym = s;
    e->args = args;
    e->memory = K(memory);
    return e;
}
py::ref<ir::Call> mk_call(const NodeP& target, const SymP& s, const py::list<NodeP>& args, const TypeP& t, const Pos& p) {
    auto c = mke<ir::Call>(t, p);
    c->target = target;
    c->sym = s;
    c->args = args;
    c->typeargs = py::list<TypeP>();
    return c;
}

py::list<NodeP> list_of(const NodeP& n) {
    py::list<NodeP> out;
    out.append(n);
    return out;
}

bool is_polymorphic_flag(const Dict& dd, const char* key) { return is_truthy(vget(dd, key)); }

// _wrap's table of widths
int64_t bits_of(const py::str& kind, bool& known) {
    static const std::pair<const char*, int64_t> BITS[] = {{"i8", 8},  {"u8", 8},  {"char", 8}, {"i16", 16}, {"u16", 16}, {"i32", 32},
                                                           {"u32", 32}, {"i64", 64}, {"u64", 64}, {"bool", 1}};
    for (const auto& b : BITS) {
        if (kind == b.first) {
            known = true;
            return b.second;
        }
    }
    known = false;
    return 0;
}

// int arithmetic of the constants: Python's ints do not overflow; past int64 is an OverflowError here
[[noreturn]] void overflow() { throw py::OverflowError("integer constant past int64"); }
int64_t ck_add(int64_t a, int64_t b) {
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) overflow();
    return a + b;
}
int64_t ck_sub(int64_t a, int64_t b) {
    if ((b < 0 && a > INT64_MAX + b) || (b > 0 && a < INT64_MIN + b)) overflow();
    return a - b;
}
int64_t ck_mul(int64_t a, int64_t b) {
    if (a == 0 || b == 0) return 0;
    if (a == -1) {
        if (b == INT64_MIN) overflow();
        return -b;
    }
    if (b == -1) {
        if (a == INT64_MIN) overflow();
        return -a;
    }
    int64_t r = int64_t(uint64_t(a) * uint64_t(b));
    if (r / b != a) overflow();
    return r;
}
int64_t ck_shl(int64_t a, int64_t b) {   // (b >= 0)
    if (a == 0) return 0;
    if (b > 62) overflow();
    int64_t lim = INT64_MAX >> b;
    if (a > lim || a < -lim - 1) overflow();
    return int64_t(uint64_t(a) << b);
}
// int(a / b) (the quotient through a float, as the Python)
int64_t trunc_div(int64_t a, int64_t b) {
    if (b == 0) throw py::ZeroDivisionError("division by zero");
    return py::int_from_float(double(a) / double(b));
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

Dict load_tree(const py::str& text) {
    Val v = from_v(py::json_parse(text.utf8()));
    if (!holds<Dict>(v)) throw py::TypeError("not a JSON object");
    return as<Dict>(v);
}

// ---------------------------------------------------------------------------------------------- string literals
py::str esc_value_(const Val& sv) {
    if (!holds<py::str>(sv)) {
        if (holds<Val::List>(sv)) throw py::ValueError("'\"' is not in list");   // (a list has an index() too)
        throw py::AttributeError("'" + pyname(sv) + "' object has no attribute 'index'");
    }
    const py::str& s = as<py::str>(sv);
    int64_t q = s.index(K("\""));
    py::str prefix = s.slice(0, q);
    if (prefix == K("U")) throw py::TypeError("UTF-32 string literal");
    int64_t r = s.rfind(K("\""));
    py::str body = r > q + 1 ? s.slice(q + 1, r) : py::str();
    if (prefix == K("L") || prefix == K("u")) return wide_value_(body);   // a wide literal: its UTF-16 code units, one character each
    // body.encode('utf-8', 'surrogateescape')
    std::string raw;
    for (char32_t c : body.u32()) {
        if (c >= 0xDC80 && c <= 0xDCFF) {
            raw.push_back(char(c - 0xDC00));
        } else if (c < 0x80) {
            raw.push_back(char(c));
        } else if (c < 0x800) {
            raw.push_back(char(0xC0 | (c >> 6)));
            raw.push_back(char(0x80 | (c & 0x3F)));
        } else if (c < 0x10000) {
            raw.push_back(char(0xE0 | (c >> 12)));
            raw.push_back(char(0x80 | ((c >> 6) & 0x3F)));
            raw.push_back(char(0x80 | (c & 0x3F)));
        } else {
            raw.push_back(char(0xF0 | (c >> 18)));
            raw.push_back(char(0x80 | ((c >> 12) & 0x3F)));
            raw.push_back(char(0x80 | ((c >> 6) & 0x3F)));
            raw.push_back(char(0x80 | (c & 0x3F)));
        }
    }
    auto hex_digit = [](unsigned char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    auto octal = [](unsigned char c) { return c >= '0' && c <= '7'; };
    std::string out;
    size_t i = 0;
    while (i < raw.size()) {
        unsigned char c = static_cast<unsigned char>(raw[i]);
        if (c != 0x5c) {
            out.push_back(char(c));
            i += 1;
            continue;
        }
        i += 1;
        if (i >= raw.size()) throw py::IndexError("index out of range");
        unsigned char e = static_cast<unsigned char>(raw[i]);
        if (e == 'x') {
            size_t j = i + 1;
            while (j < raw.size() && hex_digit(static_cast<unsigned char>(raw[j])) >= 0) j += 1;
            if (j == i + 1) throw py::ValueError("invalid literal for int() with base 16: b''");
            // int(raw[i+1:j], 16) & 0xFF: the last two digits
            unsigned v = 0;
            size_t from = j - i - 1 > 2 ? j - 2 : i + 1;
            for (size_t k = from; k < j; ++k) v = v * 16 + unsigned(hex_digit(static_cast<unsigned char>(raw[k])));
            out.push_back(char(v & 0xFF));
            i = j;
        } else if (octal(e)) {
            size_t j = i;
            while (j < raw.size() && j < i + 3 && octal(static_cast<unsigned char>(raw[j]))) j += 1;
            unsigned v = 0;
            for (size_t k = i; k < j; ++k) v = v * 8 + unsigned(raw[k] - '0');
            out.push_back(char(v & 0xFF));
            i = j;
        } else {
            int simple = -1;
            switch (e) {
                case 'n': simple = 10; break;
                case 't': simple = 9; break;
                case 'r': simple = 13; break;
                case 'a': simple = 7; break;
                case 'b': simple = 8; break;
                case 'f': simple = 12; break;
                case 'v': simple = 11; break;
                case '\\': simple = 92; break;
                case '\'': simple = 39; break;
                case '"': simple = 34; break;
                case '?': simple = 63; break;
                case 'e': simple = 27; break;
                default: break;
            }
            if (simple >= 0) {
                out.push_back(char(simple));
                i += 1;
            } else {
                out.push_back(char(raw[i]));
                i += 1;
            }
        }
    }
    // out.decode('latin-1')
    std::u32string u;
    for (char c : out) u.push_back(char32_t(static_cast<unsigned char>(c)));
    return py::str(u);
}

py::str wide_value_(const py::str& body_s) {
    const std::u32string& body = body_s.u32();
    auto hex_digit = [](char32_t c) -> int {
        if (c >= '0' && c <= '9') return int(c - '0');
        if (c >= 'a' && c <= 'f') return int(c - 'a' + 10);
        if (c >= 'A' && c <= 'F') return int(c - 'A' + 10);
        return -1;
    };
    auto octal = [](char32_t c) { return c >= '0' && c <= '7'; };
    std::u32string out;
    size_t i = 0;
    while (i < body.size()) {
        char32_t c = body[i];
        if (c != U'\\') {
            out.push_back(c);
            i += 1;
            continue;
        }
        if (i + 1 >= body.size()) throw py::IndexError("string index out of range");
        char32_t e = body[i + 1];
        if (e == U'x' || e == U'u' || e == U'U') {
            size_t j = i + 2;
            size_t limit = i + 2 + (e == U'u' ? 4 : 8);
            while (j < body.size() && hex_digit(body[j]) >= 0 && (e == U'x' || j < limit)) j += 1;
            if (j == i + 2) throw py::ValueError("invalid literal for int() with base 16: ''");
            // int(body[i+2:j], 16) & 0xFFFF: the last four digits
            unsigned v = 0;
            size_t from = j - (i + 2) > 4 ? j - 4 : i + 2;
            for (size_t k = from; k < j; ++k) v = v * 16 + unsigned(hex_digit(body[k]));
            out.push_back(char32_t(v & 0xFFFF));
            i = j;
        } else if (octal(e)) {
            size_t j = i + 1;
            while (j < body.size() && j < i + 4 && octal(body[j])) j += 1;
            unsigned v = 0;
            for (size_t k = i + 1; k < j; ++k) v = v * 8 + unsigned(body[k] - U'0');
            out.push_back(char32_t(v));
            i = j;
        } else {
            char32_t m = e;
            switch (e) {
                case U'n': m = U'\n'; break;
                case U't': m = U'\t'; break;
                case U'r': m = U'\r'; break;
                case U'a': m = U'\a'; break;
                case U'b': m = U'\b'; break;
                case U'f': m = U'\f'; break;
                case U'v': m = U'\v'; break;
                default: break;   // '\\', "'", '"', '?' map to themselves; so does any other
            }
            out.push_back(m);
            i += 2;
        }
    }
    return py::str(out);
}

std::optional<py::str> char_text_(int64_t v) {
    if (32 <= v && v < 127 && v != '\'' && v != '\\') return K("'") + py::str(char32_t(v)) + K("'");
    switch (v) {
        case 10: return K("'\\n'");
        case 9: return K("'\\t'");
        case 13: return K("'\\r'");
        case 0: return K("'\\0'");
        case 39: return K("'\\''");
        case 92: return K("'\\\\'");
        default: return std::nullopt;
    }
}

py::str spell(const TypeP& t) {
    if (!t) return K("?");
    const py::str& k = t->kind;
    if (k == K("class")) {
        if (!t->name) type_error("can only concatenate str (not \"NoneType\") to str");
        py::str out = *t->name;
        if (py::len(t->args) != 0) {
            py::str inner;
            for (int64_t i = 0; i < py::len(t->args); ++i) inner = inner + (i ? K(", ") : K("")) + spell(t->args[i]);
            out = out + K("<") + inner + K(">");
        }
        return out;
    }
    if (k == K("ptr")) return spell(t->elem) + (is_truthy(t->info.get(K("ref"), Val())) ? K("&") : K("*"));
    if (k == K("array")) {
        Val count = t->info.get(K("count"), Val(K("")));
        return spell(t->elem) + K("[") + str_of(count) + K("]");
    }
    if (k == K("fnptr")) return spell(type_val(vat(t->info, "fn"))) + K("*");
    if (k == K("fn")) {
        Dict result = as_dict_get(vat(t->info, "result"));
        py::str out = spell(type_val(vget(result, "type"))) + K("(");
        Val ps = vat(t->info, "params");
        if (!holds<Val::List>(ps)) type_error("'" + pyname(ps) + "' object is not iterable");
        bool first = true;
        for (const Val& p : as<Val::List>(ps)) {
            out = out + (first ? K("") : K(",")) + spell(type_val(vat(as_dict_sub(p), "type")));
            first = false;
        }
        return out + K(")");
    }
    if (k == K("char")) return K("char");
    return k;
}

int64_t wrap_(int64_t v, const TypeP& t) {
    bool known = false;
    int64_t b = t ? bits_of(t->kind, known) : 0;
    if (!t || !known) return v;
    if (t->kind == K("bool")) return v ? 1 : 0;
    uint64_t mask = b == 64 ? ~uint64_t(0) : ((uint64_t(1) << b) - 1);
    uint64_t u = uint64_t(v) & mask;
    bool is_signed = t->kind.startswith(K("i")) ||
                     (t->kind == K("char") && is_truthy(t->info.get(K("signed"), Val(true))));
    if (is_signed) {
        if (b == 64) return int64_t(u);
        if (u >= (uint64_t(1) << (b - 1))) return int64_t(u) - (int64_t(1) << b);
        return int64_t(u);
    }
    if (u > uint64_t(INT64_MAX)) overflow();
    return int64_t(u);
}

// ---------------------------------------------------------------------------------------------- _Scope
Scope::Scope(const std::optional<py::str>& owner_) : owner(owner_) {}

py::str Scope::fresh(const py::str& base) {
    n += 1;
    py::str name = base + py::to_str(n);
    used.add(name);
    return name;
}

// ---------------------------------------------------------------------------------------------- Lowering
namespace {

// the scope of the function being lowered (AttributeError before the first)
Scope& scope_of(const std::shared_ptr<Scope>& s) {
    if (!s) throw py::AttributeError("'Lowering' object has no attribute 'scope'");
    return *s;
}

}  // namespace

Lowering::Lowering(const Dict& tree_, const py::str& main_path, const std::optional<py::list<py::str>>& own_,
                   const py::str& language_, const std::optional<py::str>& std__, const py::str& triple)
    : tree(tree_),
      main(py::os::path::normcase(py::os::path::abspath(main_path))),
      auto_own(!own_.has_value()),
      cpp(language_ == K("c++") || language_ == K("cpp") || language_ == K("cxx")),
      language(cpp ? K("c++") : K("c")),
      std_(std__),
      model(ctypes::Model::for_triple(triple)),
      unit_name(std::get<0>(py::os::path::splitext(py::os::path::basename(main_path)))),
      types(model, [this](const py::str& name, const py::list<TypeP>& args, const std::optional<py::str>& tag) {
          return name_(name, args, tag);
      }),
      sizes(model, [this](const TypeP& t) { return layout_of_(t); }) {
    if (own_) {
        for (const py::str& p : *own_) own.add(py::os::path::normcase(py::os::path::abspath(p)));
    }
    own.add(main);
    index_();
}

// ------------------------------------------------------------------------------------- indexing
namespace {

struct Loc {
    Val file;
    int64_t line = 0;
    int64_t col = 0;
};

}  // namespace

void Lowering::index_() {
    struct State {
        Val file;
        int64_t line = 0;
    } state;

    std::function<std::optional<Loc>(const Val&)> upd = [&](const Val& locv) -> std::optional<Loc> {
        if (!is_truthy(locv)) return std::nullopt;
        if (!holds<Dict>(locv)) type_error("argument of type '" + pyname(locv) + "' is not iterable");
        const Dict& loc = as<Dict>(locv);
        bool any = false;
        for (const char* k : LOC_KEYS) any = any || has(loc, k);
        if (any) {
            std::optional<Loc> got;
            for (const char* k : LOC_KEYS) {
                if (has(loc, k)) {
                    std::optional<Loc> r = upd(vat(loc, k));
                    if (r) got = r;
                }
            }
            return got;
        }
        if (has(loc, "file")) {
            state.file = vat(loc, "file");
            files.add(str_in(state.file));
        }
        if (has(loc, "line")) state.line = number_of(vat(loc, "line"));
        Loc out;
        out.file = state.file;
        out.line = state.line;
        out.col = number_of(vget(loc, "col", Val(int64_t(0))));
        return out;
    };

    struct Frame {
        Dict n;
        std::optional<Dict> parent;
        std::vector<py::str> ns;
    };
    std::vector<Frame> stack;
    stack.push_back(Frame{tree, std::nullopt, {}});
    std::vector<std::pair<Dict, std::vector<py::str>>> pending;
    while (!stack.empty()) {
        Frame f = std::move(stack.back());
        stack.pop_back();
        Dict n = f.n;
        const std::optional<Dict>& par = f.parent;
        const std::vector<py::str>& ns = f.ns;
        if (n.empty()) continue;   // ({}: an absent child, as in a for's parts)
        std::optional<Loc> here = upd(vget(n, "loc"));
        Dict r = obj_or_empty(n, "range");
        std::optional<Loc> rb = upd(vget(r, "begin"));
        upd(vget(r, "end"));
        if (rb) {
            Val::List l;
            l.append(rb->file);
            l.append(Val(rb->line));
            l.append(Val(rb->col));
            n.set(K("_rb"), Val(l));
        }
        if (here) {
            n.set(K("_f"), here->file);
            n.set(K("_l"), Val(here->line));
            n.set(K("_c"), Val(here->col));
        } else if (par && has(*par, "_f")) {
            n.set(K("_f"), vat(*par, "_f"));
            n.set(K("_l"), vget(*par, "_l", Val(int64_t(0))));
            n.set(K("_c"), vget(*par, "_c", Val(int64_t(0))));
        }
        py::str kind = kind_s(n);
        std::vector<py::str> child_ns = ns;
        if (kind == K("LabelStmt") && is_truthy(vget(n, "declId"))) {
            labels.set(val_key(vget(n, "declId")), opt_str(n, "name"));
        }
        Val nid = vget(n, "id");
        if ((is_truthy(nid) && kind.endswith(K("Decl"))) || kind == K("ClassTemplateSpecializationDecl")) {
            py::str id = val_key(nid);
            decls.set(id, n);
            if (par && kind_s(*par).endswith(K("Decl"))) parent.set(id, *par);
            if (kind == K("NamespaceDecl")) {
                Val name = vget(n, "name");
                child_ns.push_back(is_truthy(name) ? str_in(name) : K("(anonymous)"));
            } else if (kind == K("RecordDecl") || kind == K("CXXRecordDecl") || kind == K("ClassTemplateSpecializationDecl")) {
                if (par && in_list(kind_of(*par), {"CXXRecordDecl", "ClassTemplateSpecializationDecl"}) &&
                    same_val(vget(n, "name"), vget(*par, "name")) && !is_truthy(vget(n, "completeDefinition"))) {
                    // (the injected class name)
                } else if (kind == K("ClassTemplateSpecializationDecl")) {
                    // (its key spells its arguments, which may name records indexed later: after the walk)
                    pending.emplace_back(n, ns);
                    Val name = vget(n, "name");
                    child_ns.push_back(str_in(name));
                } else {
                    py::str key = record_key_(n, ns);
                    qnames.set(id, key);
                    if (is_truthy(vget(n, "completeDefinition")) || !records.contains(key)) records.set(key, n);
                    if (!is_truthy(vget(n, "name")) && is_truthy(vget(n, "_rb"))) {   // (clang names it by where it begins too)
                        const Val::List& rbl = as<Val::List>(vget(n, "_rb"));
                        py::str alias = join_dot(ns, K("__unnamed_") + ctypes::anon_key(number_of(rbl[1]), number_of(rbl[2])));
                        records.setdefault(alias, n);
                    }
                    if (!key.empty()) child_ns.push_back(last_segment(key));
                }
            } else if (kind == K("TypedefDecl") || kind == K("TypeAliasDecl")) {
                Dict inner;
                {
                    Val iv = vget(n, "inner");
                    if (!is_truthy(iv)) {
                        inner = Dict();
                    } else {
                        if (!holds<Val::List>(iv)) type_error("'" + pyname(iv) + "' object is not subscriptable");
                        inner = as_dict_get(as<Val::List>(iv)[0]);
                    }
                }
                while (kind_is(inner, "ElaboratedType") && is_truthy(vget(inner, "inner"))) inner = sub_at(inner, "inner", 0);
                py::str name = join_dot(ns, str_in(vat(n, "name")));
                if (in_list(kind_of(inner), {"RecordType", "EnumType"}) && is_truthy(vget(inner, "decl"))) {
                    typedefs.set(name, std::make_tuple(kind_is(inner, "RecordType") ? K("record") : K("enum"),
                                                       opt_of(vat(as_dict_sub(vat(inner, "decl")), "id"))));
                } else {
                    typedefs.set(name, std::make_tuple(K("type"), opt_str(as_dict_get(vat(n, "type")), "qualType")));
                }
            } else if (kind == K("EnumDecl")) {
                int64_t val = -1;
                for (const Dict& c : inner_get(n, "inner")) {
                    if (kind_is(c, "EnumConstantDecl")) {
                        std::optional<Dict> init;
                        {
                            Val iv = vget(c, "inner");
                            if (is_truthy(iv)) {
                                if (!holds<Val::List>(iv)) type_error("'" + pyname(iv) + "' object is not subscriptable");
                                init = as_dict_get(as<Val::List>(iv)[0]);
                            }
                        }
                        val = (init && !init->empty()) ? enum_const_(init) : val + 1;
                        enum_values.set(val_key(vat(c, "id")), val);
                    }
                }
                if (is_truthy(vget(n, "scopedEnumTag"))) scoped_enums.set(id, join_dot(ns, str_in(vat(n, "name"))));
            } else if (kind == K("LabelDecl")) {
                labels.set(id, opt_str(n, "name"));
            }
        }
        {
            py::list<Val> kids;
            Val iv = vget(n, "inner");
            if (is_truthy(iv)) {
                if (!holds<Val::List>(iv)) type_error("'" + pyname(iv) + "' object is not iterable");
                kids.extend(as<Val::List>(iv));
            }
            Val af = vget(n, "array_filler");
            if (is_truthy(af)) {
                if (!holds<Val::List>(af)) type_error("can only concatenate list (not \"" + pyname(af) + "\") to list");
                kids.extend(as<Val::List>(af));
            }
            for (int64_t i = kids.size() - 1; i >= 0; --i) {
                const Val& c = kids[i];
                if (holds<Dict>(c)) stack.push_back(Frame{as<Dict>(c), n, child_ns});
            }
        }
        // template arguments of a specialisation sit in inner too: nothing else to walk
    }
    for (int pass = 1; pass <= 2; ++pass) {   // (twice: an argument may be another instance)
        types.cache.clear();
        for (const auto& pr : pending) {
            const Dict& n = pr.first;
            py::str key = record_key_(n, pr.second);
            qnames.set(val_key(vat(n, "id")), key);
            if (is_truthy(vget(n, "completeDefinition")) || !records.contains(key)) records.set(key, n);
        }
    }
    types.cache.clear();
    if (auto_own) {   // the program's own headers: those beside its source
        py::str here = py::os::path::dirname(main);
        for (const py::str& f : files) {
            py::str p = py::os::path::normcase(py::os::path::abspath(f));
            if (py::os::path::dirname(p) == here) own.add(p);
        }
    }
    own_dirs = py::set<py::str>();
    for (const py::str& p : own) own_dirs.add(py::os::path::dirname(p));
}

py::str Lowering::record_key_(const Dict& n, const std::vector<py::str>& ns) {
    Val name = vget(n, "name");
    if (!is_truthy(name)) {
        return join_dot(ns, K("__unnamed_") + ctypes::anon_key(number_of(vget(n, "_l", Val(int64_t(0)))),
                                                            number_of(vget(n, "_c", Val(int64_t(0))))));
    }
    py::str key = join_dot(ns, str_in(name));
    if (kind_is(n, "ClassTemplateSpecializationDecl")) {
        py::list<TypeP> args;
        for (const Dict& a : inner_get(n, "inner")) {
            if (kind_is(a, "TemplateArgument")) {
                if (has(a, "type")) {
                    try {
                        args.append(types.parse(parse_text(vat2(a, "type", "qualType"))));
                    } catch (const py::Exception& e) {
                        if (!soft_error(e)) throw;
                        args.append(mk_type("class", str_in(vat2(a, "type", "qualType"))));
                    }
                } else if (has(a, "value")) {
                    args.append(mk_type("class", str_of(vat(a, "value"))));
                }
            }
        }
        py::str inner;
        for (int64_t i = 0; i < args.size(); ++i) inner = inner + (i ? K(", ") : K("")) + spell(args[i]);
        key = key + K("<") + inner + K(">");
    }
    return key;
}

int64_t Lowering::enum_const_(const std::optional<Dict>& e) {
    std::optional<int64_t> v = const_(e);
    return v ? *v : 0;
}

// ---------------------------------------------------------------------------------------- types
TypeP Lowering::name_(const py::str& name, const py::list<TypeP>& args, const std::optional<py::str>& tag) {
    struct Depth {
        int64_t& d;
        explicit Depth(int64_t& d_) : d(d_) {
            if (++d > 40) {
                --d;
                throw RecursionError();
            }
        }
        ~Depth() { --d; }
    } depth(name_depth);
    py::str dotted = name.replace(K("::"), K("."));
    if (dotted == K("__builtin_va_list")) {   // MSVC's va_list: a char pointer
        Dict info;
        info.set(K("signed"), Val(model.char_signed));
        return ptr_to(mk_type("char", std::nullopt, py::list<TypeP>(), nullptr, info));
    }
    if (dotted.startswith(K("std.")) && !records.contains(dotted)) {
        return mk_type("class", dotted, args);   // a library class: named, not laid out
    }
    py::str key = dotted;
    if (py::len(args) != 0) {
        py::str inner;
        for (int64_t i = 0; i < args.size(); ++i) inner = inner + (i ? K(", ") : K("")) + spell(args[i]);
        key = key + K("<") + inner + K(">");
    }
    std::optional<Dict> rec = find_record_(key);
    if (rec && !(tag && *tag == K("enum"))) return record_type_(rec);
    auto td = find_typedef_(dotted);
    if (td) {
        const py::str& k = std::get<0>(*td);
        const std::optional<py::str>& v = std::get<1>(*td);
        if (k == K("record")) {
            std::optional<Dict> n = decls.get(key_of(v));
            return n ? record_type_(n) : TypeP();
        }
        if (k == K("enum")) return enum_type_(key_of(v));
        if (!v) type_error("expected string or bytes-like object, got 'NoneType'");
        return types.parse(*v);
    }
    for (const auto& kv : scoped_enums.items()) {
        const py::str& qn = std::get<1>(kv);
        if (qn == dotted || qn.endswith(K(".") + dotted)) return ir::I32;   // (an enum class is its int: its constants stay named)
    }
    if ((tag && *tag == K("enum")) || find_enum_(dotted)) return ir::I32;
    return nullptr;
}

std::optional<Dict> Lowering::find_record_(const py::str& key) {
    if (records.contains(key)) return records.at(key);
    auto memo = tail_memo.get(key);
    if (memo && std::get<0>(*memo) == records.size()) return std::get<1>(*memo);
    py::str tail = K(".") + key;
    std::optional<py::str> best;
    for (const py::str& k : records) {
        if (k.endswith(tail) && (!best || k.size() < best->size())) best = k;
    }
    std::optional<Dict> got;
    if (best) got = records.at(*best);
    tail_memo.set(key, std::make_tuple(records.size(), got));
    return got;
}

std::optional<std::tuple<py::str, std::optional<py::str>>> Lowering::find_typedef_(const py::str& name) {
    if (typedefs.contains(name)) return typedefs.at(name);
    py::str tail = K(".") + name;
    for (const py::str& k : typedefs) {
        if (k.endswith(tail)) return typedefs.at(k);
    }
    return std::nullopt;
}

bool Lowering::find_enum_(const py::str& name) {
    if (!enum_names) {
        py::set<py::str> names;
        for (const auto& kv : decls.items()) {
            const Dict& n = std::get<1>(kv);
            if (kind_is(n, "EnumDecl")) {
                Val nm = vget(n, "name");
                if (holds<py::str>(nm)) names.add(as<py::str>(nm));
            }
        }
        enum_names = names;
    }
    return enum_names->contains(last_segment(name));
}

TypeP Lowering::enum_type_(const py::str& eid) {
    if (scoped_enums.contains(eid)) return ir::I32;
    return ir::I32;
}

TypeP Lowering::record_type_(const std::optional<Dict>& n) {
    if (!n) type_error("'NoneType' object is not subscriptable");
    auto key = qnames.get(val_key(vat(*n, "id")));
    if (!key) return nullptr;
    Dict full = records.get(*key, *n);
    if (kind_is(full, "ClassTemplateSpecializationDecl")) {
        int64_t lt = key->find(K("<"));
        py::str base = lt < 0 ? *key : key->slice(0, lt);
        py::list<TypeP> args;
        for (const Dict& a : inner_get(full, "inner")) {
            if (kind_is(a, "TemplateArgument")) {
                try {
                    if (has(a, "type")) args.append(types.parse(parse_text(vat2(a, "type", "qualType"))));
                    else args.append(mk_type("class", str_of(vget(a, "value"))));
                } catch (const py::Exception& e) {
                    if (!soft_error(e)) throw;
                    args.append(mk_type("class", str_in(vat2(a, "type", "qualType"))));
                }
            }
        }
        return mk_type("class", base, args);
    }
    return mk_type("class", *key);
}

TypeP Lowering::ty(const Dict& n, const char* key) {
    Dict t = obj_or_empty(n, key);
    Val text = vget(t, "qualType");
    if (text.is_none()) return ir::ANY;
    try {
        return ctypes::normalise(types.parse(parse_text(text)));
    } catch (const py::Exception& e) {
        if (!soft_error(e)) throw;
        try {
            Val dq = vget(t, "desugaredQualType");
            if (is_truthy(dq)) return ctypes::normalise(types.parse(parse_text(dq)));
        } catch (const py::Exception& e2) {
            if (!soft_error(e2)) throw;
        }
        return mk_type("class", K("<unread:") + str_of(text) + K(">"));
    }
}

TypeP Lowering::layout_of_(const TypeP& t) {
    py::str name = spell(t);
    for (const py::str& qn : scoped_enums.values()) {
        if (qn == name || qn.endswith(K(".") + name)) {
            Dict f;
            f.set(K("name"), Val("value"));
            f.set(K("offset"), Val(int64_t(0)));
            f.set(K("type"), Val(ir::I32));
            Val::List fields;
            fields.append(Val(f));
            Dict info;
            info.set(K("size"), Val(int64_t(4)));
            info.set(K("fields"), Val(fields));
            info.set(K("role"), Val("struct"));
            return mk_type("record", name, py::list<TypeP>(), nullptr, info);
        }
    }
    return record_layout_(t);
}

TypeP Lowering::record_layout_(const TypeP& t) {
    py::str key = spell(t);
    if (layout_cache.contains(key)) return layout_cache.at(key);
    std::optional<Dict> rec = find_record_(key);
    if (!rec) return nullptr;
    layout_cache.set(key, nullptr);   // (a cycle through a value field cannot be)
    py::list<std::tuple<py::str, TypeP>> fields;
    bool union_ = is_str_val(vget(*rec, "tagUsed"), "union");
    for (const Dict& c : inner_get(*rec, "inner")) {
        if (kind_is(c, "FieldDecl")) {
            if (is_truthy(vget(c, "isBitfield"))) return nullptr;
            Val nm = vget(c, "name");
            fields.append(std::make_tuple(is_truthy(nm) ? str_in(nm) : K(""), ty(c)));
        }
    }
    if (polymorphic_(*rec) || py::len(bases_(*rec)) != 0) return nullptr;
    TypeP r;
    try {
        r = std::get<0>(sizes.lay_out(key, fields, union_));
    } catch (const py::TypeError&) {
        return nullptr;   // (a field whose size is not known here: no layout)
    }
    layout_cache.set(key, r);
    return r;
}

bool Lowering::polymorphic_(const Dict& rec) {
    Dict dd = obj_or_empty(rec, "definitionData");
    return is_truthy(vget(dd, "isPolymorphic")) || is_truthy(vget(dd, "isAbstract"));
}

py::list<Dict> Lowering::bases_(const Dict& rec) { return inner_or(rec, "bases"); }

// ---------------------------------------------------------------------------------------- helpers
Pos Lowering::pos(const Dict& n) {
    Pos p;
    Val f = vget(n, "_f");
    p.line = (is_truthy(f) && is_own_(f)) ? number_of(vget(n, "_l", Val(int64_t(0)))) : 0;
    p.col = number_of(vget(n, "_c", Val(int64_t(0))));
    return p;
}

bool Lowering::is_own_(const Val& f) {
    if (f.is_none()) return false;
    py::str p = py::os::path::normcase(py::os::path::abspath(str_in(f)));
    return own.contains(p);
}

py::ref<ir::Unsupported> Lowering::unsupported(const OptStr& kind, const std::optional<Dict>& n, const OptStr& raw) {
    auto u = std::make_shared<ir::Unsupported>();
    u->kind = kind;
    u->raw = raw;
    u->type = n ? ty(*n) : ir::ANY;
    if (n) {
        Pos p = pos(*n);
        u->line = p.line;
        u->col = p.col;
    }
    return u;
}

std::optional<int64_t> Lowering::const_(const std::optional<Dict>& nn) {
    if (!nn) return std::nullopt;
    const Dict& n = *nn;
    OptStr k = kind_of(n);
    if (has(n, "value") && in_list(k, {"ConstantExpr", "IntegerLiteral"})) {
        try {
            return to_int(vat(n, "value"));
        } catch (const py::ValueError&) {
        }
    }
    if (k == K("CharacterLiteral")) return to_int(vat(n, "value"));
    if (k == K("CXXBoolLiteralExpr")) return int64_t(is_truthy(vat(n, "value")) ? 1 : 0);
    if (in_list(k, {"ConstantExpr", "ParenExpr", "ImplicitCastExpr", "CStyleCastExpr", "CXXStaticCastExpr", "CXXFunctionalCastExpr",
                    "SubstNonTypeTemplateParmExpr"})) {
        std::optional<int64_t> v;
        if (is_truthy(vget(n, "inner"))) {
            Val iv = vat(n, "inner");
            if (!holds<Val::List>(iv)) type_error("'" + pyname(iv) + "' object is not subscriptable");
            v = const_(as_dict_get(as<Val::List>(iv)[-1]));
        }
        if (v && k != K("ParenExpr") && k != K("ConstantExpr")) {
            TypeP t = ty(n);
            v = wrap_(*v, t);
        }
        return v;
    }
    if (k == K("DeclRefExpr")) {
        Dict d = obj_or_empty(n, "referencedDecl");
        if (kind_is(d, "EnumConstantDecl")) return dget(enum_values, vat(d, "id"));
        std::optional<Dict> dn = dget(decls, vget(d, "id"));
        if (dn && kind_is(*dn, "VarDecl") && is_truthy(vget(*dn, "constexpr")) && is_truthy(vget(*dn, "inner"))) {
            return const_(sub_at(*dn, "inner", 0));
        }
        return std::nullopt;
    }
    if (k == K("UnaryOperator")) {
        std::optional<int64_t> v = const_(sub_at(n, "inner", 0));
        if (!v) return std::nullopt;
        OptStr op = opt_of(vat(n, "opcode"));
        if (op == K("-")) {
            if (*v == INT64_MIN) overflow();
            return -*v;
        }
        if (op == K("+")) return *v;
        if (op == K("~")) return ~*v;
        if (op == K("!")) return int64_t(*v ? 0 : 1);
        return std::nullopt;
    }
    if (k == K("BinaryOperator")) {
        std::optional<int64_t> a = const_(sub_at(n, "inner", 0));
        std::optional<int64_t> b = const_(sub_at(n, "inner", 1));
        if (!a || !b) return std::nullopt;
        OptStr op = opt_of(vat(n, "opcode"));
        // (the Python builds a table of every operator's result before it looks `op` up: a negative
        // shift count is a ValueError for all of them, a zero divisor a ZeroDivisionError for / and %)
        if (*b < 0) return std::nullopt;
        std::optional<int64_t> v;
        if (op == K("+")) v = ck_add(*a, *b);
        else if (op == K("-")) v = ck_sub(*a, *b);
        else if (op == K("*")) v = ck_mul(*a, *b);
        else if (op == K("<<")) v = ck_shl(*a, *b);
        else if (op == K(">>")) v = *b > 63 ? (*a < 0 ? -1 : 0) : (*a >> *b);
        else if (op == K("&")) v = *a & *b;
        else if (op == K("|")) v = *a | *b;
        else if (op == K("^")) v = *a ^ *b;
        else if (op == K("==")) v = int64_t(*a == *b);
        else if (op == K("!=")) v = int64_t(*a != *b);
        else if (op == K("<")) v = int64_t(*a < *b);
        else if (op == K(">")) v = int64_t(*a > *b);
        else if (op == K("<=")) v = int64_t(*a <= *b);
        else if (op == K(">=")) v = int64_t(*a >= *b);
        else if (op == K("&&")) v = int64_t(*a && *b);
        else if (op == K("||")) v = int64_t(*a || *b);
        try {
            if (op == K("/")) {
                v = trunc_div(*a, *b);
            } else if (op == K("%")) {
                v = ck_sub(*a, ck_mul(trunc_div(*a, *b), *b));
            }
        } catch (const py::ZeroDivisionError&) {
            return std::nullopt;
        }
        return v ? std::optional<int64_t>(wrap_(*v, ty(n))) : std::nullopt;
    }
    if (k == K("UnaryExprOrTypeTraitExpr")) {
        NodeP lit = sizeof_(n);
        auto l = as_node<ir::Lit>(lit);
        if (!l) return std::nullopt;
        return to_int(l->value);
    }
    if (k == K("ConditionalOperator")) {
        std::optional<int64_t> c = const_(sub_at(n, "inner", 0));
        if (!c) return std::nullopt;
        return const_(sub_at(n, "inner", *c ? 1 : 2));
    }
    return std::nullopt;
}

// ---------------------------------------------------------------------------------------- the unit
py::ref<ir::Unit> Lowering::unit() {
    out = py::list<NodeP>();
    classes = py::dict<py::str, py::ref<ir::ClassDecl>>();
    decls_(inner_get(tree, "inner"), true);
    Dict target = model.json(language, std_ ? *std_ : py::str());
    auto u = std::make_shared<ir::Unit>();
    u->path = main;
    u->package = K("");
    u->imports = py::list<py::str>();
    u->types = out;
    u->target = target;
    return u;
}

void Lowering::decls_(const py::list<Dict>& nodes, bool top) {
    (void)top;
    for (const Dict& n : nodes) {
        OptStr k = kind_of(n);
        if (in_list(k, {"NamespaceDecl", "LinkageSpecDecl", "ExternCContextDecl"})) {
            decls_(inner_get(n, "inner"));
            continue;
        }
        if (!is_own_(vget(n, "_f")) || is_truthy(vget(n, "isImplicit"))) continue;
        try {
            decl_(n);
        } catch (const Refused&) {
            throw;
        } catch (const py::Exception& e) {
            if (!soft_error(e)) throw;
            auto m = mk<ir::MethodDecl>(pos(n));
            m->name = opt_str(n, "name");
            m->kind = K("function");
            m->params = py::list<NodeP>();
            m->ret = ir::VOID;
            py::str kind = cat("decl:", k);
            m->body = NodeP(mk_block(list_of(mk_expr_stmt(unsupported(kind, n, message_of(e))))));
            m->mods = py::list<py::str>{K("unsupported")};
            m->throws = py::list<TypeP>();
            m->typeparams = py::list<py::str>();
            out.append(m);
        }
    }
}

void Lowering::decl_(const Dict& n) {
    OptStr k = kind_of(n);
    if (in_list(k, {"RecordDecl", "CXXRecordDecl"})) {
        if (is_truthy(vget(n, "completeDefinition"))) record(n);
    } else if (k == K("ClassTemplateDecl")) {
        for (const Dict& c : inner_get(n, "inner")) {
            if (kind_is(c, "ClassTemplateSpecializationDecl") && is_truthy(vget(c, "completeDefinition"))) {
                auto q = dget(qnames, vat(c, "id"));
                std::optional<Dict> r = q ? records.get(*q) : std::nullopt;
                record(r ? *r : c);
            }
        }
    } else if (k == K("ClassTemplateSpecializationDecl")) {
        if (is_truthy(vget(n, "completeDefinition"))) record(n);
    } else if (k == K("FunctionTemplateDecl")) {
        for (const Dict& c : inner_get(n, "inner")) {
            if (in_list(kind_of(c), {"FunctionDecl", "CXXMethodDecl"}) && is_truthy(vget(c, "TemplateInstantiationPattern"))) decl_(c);
        }
    } else if (k == K("FunctionDecl")) {
        if (body_(n)) out.append(function(n));
    } else if (in_list(k, {"CXXMethodDecl", "CXXConstructorDecl", "CXXDestructorDecl", "CXXConversionDecl"})) {
        if (body_(n)) {   // a member defined outside its class
            std::optional<Dict> owner = dget(decls, vget(n, "parentDeclContextId"));
            if (!owner || owner->empty()) owner = dget(parent, vat(n, "id"));
            auto m = method(n, owner);
            std::optional<py::str> q = owner ? dget(qnames, vget(*owner, "id")) : std::nullopt;
            if (owner && owner->empty()) q = std::nullopt;
            py::ref<ir::ClassDecl> cls;
            if (q && classes.contains(*q)) cls = classes.at(*q);
            py::ref<ir::MethodDecl> prev = cls ? find_method_(cls, n) : nullptr;
            if (prev) {
                // (static, virtual, override are said in the class, not at the definition outside it)
                py::list<py::str> pm = prev->mods ? *prev->mods : py::list<py::str>();
                py::list<py::str> mm = m->mods ? *m->mods : py::list<py::str>();
                py::list<py::str> mods = pm.copy();
                for (const py::str& x : mm) {
                    if (!pm.contains(x)) mods.append(x);
                }
                m->mods = mods;
                if (mods.contains(K("static")) && m->sym) m->sym->static_ = true;
                py::list<NodeP>& members = *cls->members;
                int64_t at = -1;
                for (int64_t i = 0; i < members.size(); ++i) {
                    if (members[i].get() == prev.get()) {
                        at = i;
                        break;
                    }
                }
                if (at < 0) throw py::ValueError("value is not in list");
                members[at] = m;
            } else if (cls) {
                cls->members->append(m);
            } else {
                out.append(m);
            }
        }
    } else if (k == K("VarDecl")) {
        if (is_str_val(vget(n, "storageClass"), "extern") && !is_truthy(vget(n, "inner"))) return;
        std::optional<Dict> owner = member_owner_(n);
        if (owner) {   // `int Shape::count = 0;`: the class's static member
            auto q = dget(qnames, vat(*owner, "id"));
            py::ref<ir::ClassDecl> cls;
            if (q && classes.contains(*q)) cls = classes.at(*q);
            py::list<Dict> init;
            for (const Dict& c : inner_get(n, "inner")) {
                if (!kind_s(c).endswith(K("Attr"))) init.append(c);
            }
            if (cls) {
                for (const NodeP& m : *cls->members) {
                    auto f = as_node<ir::FieldDecl>(m);
                    if (f && same_val(Val(f->sym->name), vat(n, "name")) && !init.empty()) f->init = expr(init[-1]);
                }
            }
            return;
        }
        out.append(global_(n));
    } else if (k == K("EnumDecl")) {
        enum_(n);
    } else if (in_list(k, {"TypedefDecl", "TypeAliasDecl", "EmptyDecl", "StaticAssertDecl", "UsingDirectiveDecl", "UsingDecl",
                           "FunctionDecl", "TypeAliasTemplateDecl", "VarTemplateDecl", "UsingShadowDecl"})) {
        // nothing
    } else {
        auto m = mk<ir::MethodDecl>(pos(n));
        m->name = opt_str(n, "name");
        m->kind = K("function");
        m->params = py::list<NodeP>();
        m->ret = ir::VOID;
        m->body = NodeP(mk_block(list_of(mk_expr_stmt(unsupported(cat("decl:", k), n)))));
        m->mods = py::list<py::str>{K("unsupported")};
        m->throws = py::list<TypeP>();
        m->typeparams = py::list<py::str>();
        out.append(m);
    }
}

std::optional<py::str> Lowering::lib_owner_(const Dict& node) {
    std::vector<py::str> ns;
    std::optional<Dict> cur = dget(parent, vget(node, "id"));
    while (cur) {
        if (kind_is(*cur, "NamespaceDecl")) {
            Val nm = vget(*cur, "name");
            ns.push_back(is_truthy(nm) ? str_in(nm) : K("(anonymous)"));
        }
        cur = dget(parent, vget(*cur, "id"));
    }
    if (!ns.empty()) {
        py::str out_;
        for (size_t i = ns.size(); i-- > 0;) out_ = out_ + ns[i] + (i ? K(".") : K(""));
        return out_;
    }
    Val f = vget(node, "_f");
    if (is_truthy(f)) return py::os::path::basename(str_in(f));
    return std::nullopt;
}

std::optional<Dict> Lowering::member_owner_(const Dict& n) {
    std::optional<Dict> par = dget(parent, vget(n, "id"));
    if (par && in_list(kind_of(*par), {"CXXRecordDecl", "ClassTemplateSpecializationDecl"})) return par;
    std::optional<Dict> pd = dget(decls, vget(n, "parentDeclContextId"));
    if (pd && in_list(kind_of(*pd), {"CXXRecordDecl", "ClassTemplateSpecializationDecl"})) return pd;
    return std::nullopt;
}

py::ref<ir::MethodDecl> Lowering::find_method_(const py::ref<ir::ClassDecl>& cls, const Dict& n) {
    std::tuple<OptStr, OptStr> sig(opt_str(n, "name"), opt_str(obj_or_empty(n, "type"), "qualType"));
    for (const NodeP& m : *cls->members) {
        auto md = as_node<ir::MethodDecl>(m);
        if (!md) continue;
        auto it = sigs.find(md.get());
        if (it != sigs.end() && it->second == sig) return md;
    }
    return nullptr;
}

std::optional<Dict> Lowering::body_(const Dict& n) {
    for (const Dict& c : inner_get(n, "inner")) {
        if (in_list(kind_of(c), {"CompoundStmt", "CXXTryStmt"})) return c;
    }
    return std::nullopt;
}

// ---- records
py::ref<ir::ClassDecl> Lowering::record(const Dict& n) {
    auto key_o = dget(qnames, vat(n, "id"));
    if (!key_o || classes.contains(*key_o)) {
        if (!key_o) return nullptr;
        return classes.at(*key_o);
    }
    py::str key = *key_o;
    TypeP t = record_type_(n);
    Val tagv = vget(n, "tagUsed", Val(K("struct")));
    py::str kind = is_str_val(tagv, "union") ? K("union") : (is_str_val(tagv, "class") ? K("class") : K("struct"));
    py::list<TypeP> bases;
    for (const Dict& b : bases_(n)) bases.append(types.parse(parse_text(vat2(b, "type", "qualType"))));
    py::list<py::str> mods;
    if (polymorphic_(n)) mods.append(K("polymorphic"));
    if (is_truthy(vget(obj_or_empty(n, "definitionData"), "isAbstract"))) mods.append(K("abstract"));
    auto c = mk<ir::ClassDecl>(pos(n));
    c->name = before_lt(last_segment(key));
    c->qname = spell(t);
    c->kind = kind;
    c->mods = mods;
    c->typeparams = py::list<py::str>();
    c->extends = py::len(bases) != 0 ? bases[0] : TypeP();
    c->implements = py::len(bases) != 0 ? bases.slice(1, py::len(bases)) : py::list<TypeP>();
    c->members = py::list<NodeP>();
    c->layout = record_layout_(t);
    classes.set(key, c);
    out.append(c);
    py::str access = is_str_val(tagv, "class") ? K("private") : K("public");
    for (const Dict& m : inner_get(n, "inner")) {
        OptStr mk_ = kind_of(m);
        if (mk_ == K("AccessSpecDecl")) {
            access = str_in(vget(m, "access", Val(access)));
            continue;
        }
        if (is_truthy(vget(m, "isImplicit"))) continue;
        Pos mp = pos(m);
        if (mk_ == K("FieldDecl")) {
            TypeP ft = ty(m);
            py::list<py::str> fmods{access};
            if (is_truthy(vget(m, "isBitfield"))) fmods.append(K("bitfield"));
            NodeP init;
            for (const Dict& x : inner_get(m, "inner")) {
                if (!kind_is(x, "ConstantExpr") || !is_truthy(vget(m, "isBitfield"))) {
                    if (!kind_s(x).endswith(K("Attr"))) init = expr(x);
                }
            }
            Val nm = vget(m, "name");
            auto f = mk<ir::FieldDecl>(mp);
            f->sym = mk_sym("field", is_truthy(nm) ? str_in(nm) : K(""), c->qname, false, ft);
            f->type = ft;
            f->init = init;
            f->mods = fmods;
            c->members->append(f);
        } else if (mk_ == K("VarDecl")) {   // a static data member
            TypeP ft = ty(m);
            NodeP init;
            if (is_truthy(vget(m, "inner")) && !kind_s(sub_at(m, "inner", 0)).endswith(K("Attr"))) init = expr(sub_at(m, "inner", 0));
            py::list<py::str> fm{access, K("static")};
            if (str_in(vat2(m, "type", "qualType")).contains(K("const"))) fm.append(K("const"));
            auto f = mk<ir::FieldDecl>(mp);
            f->sym = mk_sym("field", str_in(vat(m, "name")), c->qname, true, ft);
            f->type = ft;
            f->init = init;
            f->mods = fm;
            c->members->append(f);
        } else if (in_list(mk_, {"CXXMethodDecl", "CXXConstructorDecl", "CXXDestructorDecl", "CXXConversionDecl"})) {
            if (is_truthy(vget(m, "explicitlyDefaulted")) || is_truthy(vget(m, "explicitlyDeleted"))) continue;
            auto md = method(m, n, access);
            c->members->append(md);
        } else if (in_list(mk_, {"CXXRecordDecl", "RecordDecl"}) && is_truthy(vget(m, "completeDefinition"))) {
            auto inner = record(m);
            if (inner) inner->outer = c->qname;
        } else if (mk_ == K("EnumDecl")) {
            enum_(m);
        } else if (mk_ == K("FunctionTemplateDecl")) {
            for (const Dict& x : inner_get(m, "inner")) {
                if (kind_is(x, "CXXMethodDecl") && is_truthy(vget(x, "TemplateInstantiationPattern"))) {
                    c->members->append(method(x, n, access));
                }
            }
        } else {
            if (!mk_) none_attr("endswith");
            if (mk_ == K("FriendDecl") || mk_->endswith(K("Attr")) ||
                in_list(mk_, {"TemplateArgument", "TypedefDecl", "TypeAliasDecl", "StaticAssertDecl", "UsingDecl"})) {
                continue;
            }
            auto f = mk<ir::FieldDecl>();
            f->sym = mk_sym("field", K("<") + *mk_ + K(">"), c->qname);
            f->type = ir::ANY;
            f->init = unsupported(K("member:") + *mk_, m);
            f->mods = py::list<py::str>{K("unsupported")};
            c->members->append(f);
        }
    }
    return c;
}

void Lowering::enum_(const Dict& n) {
    py::list<Dict> consts;
    for (const Dict& c : inner_get(n, "inner")) {
        if (kind_is(c, "EnumConstantDecl")) consts.append(c);
    }
    py::str nid = val_key(vat(n, "id"));
    if (scoped_enums.contains(nid)) {
        py::str qn = scoped_enums.at(nid);
        auto c = mk<ir::ClassDecl>(pos(n));
        c->name = opt_of(vat(n, "name"));
        c->qname = qn;
        c->kind = K("enum");
        c->mods = py::list<py::str>{K("scoped")};
        c->typeparams = py::list<py::str>();
        c->extends = nullptr;
        c->implements = py::list<TypeP>();
        c->members = py::list<NodeP>();
        for (const Dict& e : consts) {
            auto f = mk<ir::FieldDecl>(pos(e));
            f->sym = mk_sym("field", str_in(vat(e, "name")), qn, true, ir::I32);
            f->type = ir::I32;
            auto ev = dget(enum_values, vat(e, "id"));
            if (!ev) throw py::KeyError(py::repr(val_key(vat(e, "id"))));
            f->init = mk_lit(Val(*ev), ir::I32);
            f->mods = py::list<py::str>{K("public"), K("static"), K("const")};
            c->members->append(f);
        }
        out.append(c);
        return;
    }
    for (const Dict& e : consts) {   // a C enum's constants are ints in the global scope
        auto f = mk<ir::FieldDecl>(pos(e));
        f->sym = mk_sym("global", str_in(vat(e, "name")), unit_name, true, ir::I32);
        f->type = ir::I32;
        auto ev = dget(enum_values, vat(e, "id"));
        if (!ev) throw py::KeyError(py::repr(val_key(vat(e, "id"))));
        f->init = mk_lit(Val(*ev), ir::I32);
        Val nm = vget(n, "name");
        f->mods = py::list<py::str>{K("static"), K("const"), K("enum:") + (is_truthy(nm) ? str_in(nm) : K(""))};
        out.append(f);
    }
}

py::ref<ir::FieldDecl> Lowering::global_(const Dict& n) {
    TypeP t = ty(n);
    NodeP init;
    for (const Dict& c : inner_get(n, "inner")) {
        if (!kind_s(c).endswith(K("Attr"))) init = expr(c);
    }
    t = complete_array_(t, init);
    py::list<py::str> mods{K("static")};
    if (is_str_val(vget(n, "storageClass"), "static")) mods.append(K("internal"));
    Val qt = vget(obj_or_empty(n, "type"), "qualType", Val(K("")));
    if (str_for(qt, "startswith").startswith(K("const "))) mods.append(K("const"));
    auto f = mk<ir::FieldDecl>(pos(n));
    f->sym = mk_sym("global", str_in(vat(n, "name")), unit_name, true, t);
    f->type = t;
    f->init = init;
    f->mods = mods;
    return f;
}

TypeP Lowering::complete_array_(const TypeP& t, const NodeP& init) {
    if (kind_attr(t) == K("array") && !t->info.contains(K("count")) && init) {
        TypeP it = type_of(init);
        if (it && it->kind == K("array") && it->info.contains(K("count"))) return it;
    }
    return t;
}

// ---- functions
SymP Lowering::sym_of_function_(const Dict& d, const std::optional<py::str>& owner, bool static_) {
    (void)static_;
    std::optional<Dict> found = dget(decls, vget(d, "id"));
    Dict node = found ? *found : d;
    TypeP ft = ty(node);
    py::list<TypeP> params;
    TypeP ret = ir::VOID;
    bool var = false;
    if (ft->kind == K("fn")) {
        Val ps = vat(ft->info, "params");
        if (!holds<Val::List>(ps)) type_error("'" + pyname(ps) + "' object is not iterable");
        for (const Val& p : as<Val::List>(ps)) params.append(type_val(vat(as_dict_sub(p), "type")));
        TypeP r = type_val(vget(as_dict_get(vat(ft->info, "result")), "type"));
        ret = r ? r : ir::VOID;
        var = is_truthy(ft->info.get(K("variadic"), Val()));
    }
    OptStr k = has(node, "kind") ? kind_of(node) : kind_of(d);
    if (in_list(k, {"CXXMethodDecl", "CXXConversionDecl", "CXXDestructorDecl", "CXXConstructorDecl"})) {
        std::optional<Dict> par = dget(decls, vget(node, "parentDeclContextId"));
        if (!par || par->empty()) par = dget(parent, vget(node, "id"));
        OptStr qn = owner;
        if (par) {
            TypeP rt = record_type_(par);
            if (rt) qn = spell(rt);
        }
        const char* kind = k == K("CXXConstructorDecl") ? "ctor" : "method";
        Val nm = vget(node, "name");
        if (!is_truthy(nm)) nm = vget(d, "name");
        auto s = std::make_shared<ir::Sym>(K(kind), sym_name(nm), qn, is_str_val(vget(node, "storageClass"), "static"));
        s->params = params;
        s->ret = ret;
        s->varargs = var;
        return s;
    }
    Val f = vget(node, "_f");
    OptStr own_;
    if (is_own_(f)) own_ = unit_name;
    else own_ = lib_owner_(node);
    Val nm = vget(node, "name");
    if (!is_truthy(nm)) nm = vget(d, "name");
    auto s = std::make_shared<ir::Sym>(K("function"), sym_name(nm), own_, true);
    s->params = params;
    s->ret = ret;
    s->varargs = var;
    return s;
}

py::list<NodeP> Lowering::params_(const Dict& n, Scope& scope) {
    py::list<NodeP> out_;
    for (const Dict& p : inner_get(n, "inner")) {
        if (kind_is(p, "ParmVarDecl")) {
            TypeP t = ty(p);
            py::str name = is_truthy(vget(p, "name")) ? local_name_(p, scope) : scope.fresh(K("arg"));
            SymP s = mk_sym("param", name, scope.owner, false, t);
            scope.syms.set(val_key(vat(p, "id")), s);
            auto prm = mk<ir::Param>(pos(p));
            prm->sym = s;
            prm->type = t;
            prm->mods = py::list<py::str>();
            out_.append(prm);
        }
    }
    return out_;
}

py::str Lowering::local_name_(const Dict& d, Scope& scope) {
    Val nv = vget(d, "name");
    py::str name = is_truthy(nv) ? str_in(nv) : K("tmp");
    if (scope.used.contains(name)) {
        int64_t i = 2;
        while (scope.used.contains(name + K("_") + py::to_str(i))) i += 1;
        name = name + K("_") + py::to_str(i);
    }
    scope.used.add(name);
    return name;
}

py::ref<ir::MethodDecl> Lowering::function(const Dict& n) {
    auto sc = std::make_shared<Scope>(opt_str(n, "name"));
    SymP s = sym_of_function_(n);
    py::list<NodeP> params = params_(n, *sc);
    scope = sc;
    std::optional<Dict> b = body_(n);
    if (!b) none_attr("get");
    NodeOrList body = stmt(*b);
    py::list<py::str> mods;
    if (is_str_val(vget(n, "storageClass"), "static")) mods.append(K("internal"));
    if (is_truthy(vget(n, "inline"))) mods.append(K("inline"));
    auto m = mk<ir::MethodDecl>(pos(n));
    m->sym = s;
    m->name = opt_of(vat(n, "name"));
    m->kind = K("function");
    m->params = params;
    m->ret = s->ret;
    m->body = body;
    m->mods = mods;
    m->throws = py::list<TypeP>();
    m->typeparams = py::list<py::str>();
    return m;
}

py::ref<ir::MethodDecl> Lowering::method(const Dict& n, const std::optional<Dict>& owner, const py::str& access) {
    auto sc = std::make_shared<Scope>(opt_str(n, "name"));
    SymP s = sym_of_function_(n);
    py::list<NodeP> params = params_(n, *sc);
    scope = sc;
    py::str k = str_in(vat(n, "kind"));
    py::str kind = k == K("CXXConstructorDecl") ? K("ctor") : (k == K("CXXDestructorDecl") ? K("dtor") : K("method"));
    py::list<py::str> mods{access};
    for (const char* flag : {"virtual", "pure", "inline"}) {
        if (is_truthy(vget(n, flag))) mods.append(K(flag));
    }
    if (is_str_val(vget(n, "storageClass"), "static")) mods.append(K("static"));
    Val qtv = vget(obj_or_empty(n, "type"), "qualType", Val(K("")));
    py::str qt = str_for(qtv, "endswith");
    if (qt.endswith(K(" const")) || qt.contains(K(") const"))) mods.append(K("const"));
    for (const Dict& c : inner_get(n, "inner")) {
        if (kind_is(c, "OverrideAttr")) {
            mods.append(K("override"));
            break;
        }
    }
    NodeOrList body = NodeP();
    std::optional<Dict> b = body_(n);
    if (b) {
        py::list<NodeP> pre;
        for (const Dict& c : inner_get(n, "inner")) {
            if (kind_is(c, "CXXCtorInitializer")) pre.extend(ctor_init(c, owner));
        }
        body = stmt(*b);
        if (!pre.empty()) {
            NodeP bn = ir::node_of(body);
            auto blk = as_node<ir::Block>(bn);
            if (!blk) {
                if (bn) throw py::AttributeError("'" + bn->class_name().utf8() + "' object has no attribute 'stmts'");
                throw py::AttributeError("'list' object has no attribute 'stmts'");
            }
            blk->stmts = pre + (blk->stmts ? *blk->stmts : py::list<NodeP>());
        }
    }
    auto m = mk<ir::MethodDecl>(pos(n));
    m->sym = s;
    m->name = opt_str(n, "name");
    m->kind = kind;
    m->params = params;
    m->ret = kind == K("method") ? s->ret : ir::VOID;
    m->body = body;
    m->mods = mods;
    m->throws = py::list<TypeP>();
    m->typeparams = py::list<py::str>();
    sigs[m.get()] = std::make_tuple(opt_str(n, "name"), OptStr(qt));
    return m;
}

py::list<NodeP> Lowering::ctor_init(const Dict& c, const std::optional<Dict>& owner) {
    Pos p = pos(c);
    std::optional<Dict> val;
    if (is_truthy(vget(c, "inner"))) val = sub_at(c, "inner", 0);
    if (has(c, "baseInit")) {
        TypeP bt = types.parse(parse_text(vat2(c, "baseInit", "qualType")));
        NodeP e;
        if (val) e = expr(*val);
        py::list<NodeP> args;
        SymP sym;
        auto en = as_node<ir::New>(e);
        if (en) {
            args = en->args ? *en->args : py::list<NodeP>();
            sym = en->sym;
        } else {
            if (e) args.append(e);
            sym = mk_sym("ctor", sym_name(bt->name ? Val(*bt->name) : Val()), spell(bt));
        }
        auto cc = mk<ir::CtorCall>(p);
        cc->kind = K("super");
        cc->sym = sym;
        cc->args = args;
        cc->target = nullptr;
        return list_of(cc);
    }
    if (has(c, "delegatingInit") || has(c, "delegateInit")) {
        if (!val) none_attr("get");
        NodeP e = expr(*val);
        auto en = as_node<ir::New>(e);
        auto cc = mk<ir::CtorCall>(p);
        cc->kind = K("this");
        cc->sym = en ? en->sym : SymP();
        cc->args = en ? (en->args ? *en->args : py::list<NodeP>()) : py::list<NodeP>();
        cc->target = nullptr;
        return list_of(cc);
    }
    Dict f = obj_or_empty(c, "anyInit");
    if (f.empty() || !val) return py::list<NodeP>();
    TypeP ot = record_type_(owner);
    TypeP ft = types.parse(parse_text(vat2(f, "type", "qualType")));
    auto this_ = mke<ir::This>(ptr_to(ot), p);
    this_->qualifier = nullptr;
    auto deref = mk_unary("*", this_, ot, p);
    auto target = mk_select(deref, mk_sym("field", str_in(vat(f, "name")), spell(ot), false, ft), ft, p);
    NodeP v;
    if (kind_is(*val, "ImplicitValueInitExpr") ||
        (kind_is(*val, "CXXConstructExpr") && !is_truthy(vget(*val, "inner")) && ft->kind != K("class"))) {
        v = zero(ft, p);
    } else {
        v = expr(*val);
    }
    if (ft->kind == K("ptr") && is_truthy(ft->info.get(K("ref"), Val()))) v = mk_unary("&", v, ft, p);
    auto asg = mke<ir::Assign>(ft, p);
    asg->target = target;
    asg->value = v;
    return list_of(mk_expr_stmt(asg, p));
}

// ---------------------------------------------------------------------------------------- statements
py::list<NodeP> Lowering::flat_(const py::list<NodeOrList>& xs) {
    py::list<NodeP> out_;
    for (const NodeOrList& x : xs) {
        if (std::holds_alternative<py::list<NodeP>>(x)) out_.extend(std::get<py::list<NodeP>>(x));
        else out_.append(std::get<NodeP>(x));
    }
    return out_;
}

NodeP Lowering::as_stmt_(const NodeOrList& s) {
    if (std::holds_alternative<py::list<NodeP>>(s)) return mk_block(std::get<py::list<NodeP>>(s));
    return std::get<NodeP>(s);
}

py::list<NodeP> Lowering::commas_(const NodeP& e) {
    auto b = as_node<ir::Binary>(e);
    if (b && b->op == K(",")) return commas_(b->left) + commas_(b->right);
    return list_of(e);
}

NodeOrList Lowering::stmt(const Dict& n) {
    OptStr k = kind_of(n);
    Pos p = pos(n);
    py::list<Dict> inner = inner_or(n, "inner");
    if (k == K("CompoundStmt")) {
        py::list<NodeP> out_;
        for (const Dict& c : inner) {
            NodeOrList s = stmt(c);
            if (std::holds_alternative<py::list<NodeP>>(s)) out_.extend(std::get<py::list<NodeP>>(s));
            else out_.append(std::get<NodeP>(s));
        }
        return NodeP(mk_block(out_, p));
    }
    if (k == K("DeclStmt")) {
        py::list<NodeP> out_;
        for (const Dict& d : inner) {
            OptStr dk = kind_of(d);
            if (dk == K("VarDecl")) {
                out_.append(local(d));
            } else if (in_list(dk, {"RecordDecl", "CXXRecordDecl"})) {
                if (is_truthy(vget(d, "completeDefinition"))) record(d);
            } else if (dk == K("EnumDecl")) {
                enum_(d);
            } else if (in_list(dk, {"TypedefDecl", "TypeAliasDecl", "StaticAssertDecl", "UsingDecl", "UsingDirectiveDecl",
                                    "FunctionDecl"})) {   // (a prototype in a body)
            } else {
                out_.append(mk_expr_stmt(unsupported(cat("local:", dk ? dk : OptStr(K(""))), d), p));
            }
        }
        if (out_.size() != 1) return out_;
        return out_[0];
    }
    if (k == K("NullStmt")) return NodeP(mk<ir::Empty>(p));
    if (k == K("ReturnStmt")) {
        NodeP e;
        if (!inner.empty()) e = expr(inner[0]);
        auto r = mk<ir::Return>(p);
        r->expr = e;
        return NodeP(r);
    }
    if (k == K("IfStmt")) {
        int64_t i = 0;
        py::list<NodeOrList> pre;
        if (is_truthy(vget(n, "hasInit"))) {
            pre.append(stmt(inner[i]));
            i += 1;
        }
        if (is_truthy(vget(n, "hasVar"))) {
            pre.append(stmt(inner[i]));
            i += 1;
        }
        NodeP cond = expr(inner[i]);
        NodeOrList then = stmt(inner[i + 1]);
        std::optional<NodeOrList> orelse;
        if (is_truthy(vget(n, "hasElse")) && inner.size() > i + 2) orelse = stmt(inner[i + 2]);
        auto s = mk<ir::If>(p);
        s->cond = cond;
        s->then = as_stmt_(then);
        if (orelse) s->orelse = as_stmt_(*orelse);
        else s->orelse = NodeP();
        if (!pre.empty()) return NodeP(mk_block(flat_(pre) + list_of(s), p));
        return NodeP(s);
    }
    if (k == K("WhileStmt")) {
        int64_t i = is_truthy(vget(n, "hasVar")) ? 1 : 0;
        if (i) return NodeP(mk_expr_stmt(unsupported(K("while-declaration"), n), p));
        auto w = mk<ir::While>(p);
        w->cond = expr(inner[0]);
        w->body = as_stmt_(stmt(inner[1]));
        return NodeP(w);
    }
    if (k == K("DoStmt")) {
        auto w = mk<ir::DoWhile>(p);
        w->body = as_stmt_(stmt(inner[0]));
        w->cond = expr(inner[1]);
        return NodeP(w);
    }
    if (k == K("ForStmt")) {
        // init, var, cond, inc, body = [x if x.get('kind') else None for x in (inner + [{}] * 5)[:5]]
        std::optional<Dict> part[5];
        for (int64_t i = 0; i < 5; ++i) {
            if (i < inner.size() && is_truthy(vget(inner[i], "kind"))) part[i] = inner[i];
        }
        const std::optional<Dict>& init = part[0];
        const std::optional<Dict>& var = part[1];
        const std::optional<Dict>& cond = part[2];
        const std::optional<Dict>& inc = part[3];
        const std::optional<Dict>& body = part[4];
        if (var) return NodeP(mk_expr_stmt(unsupported(K("for-declaration"), n), p));
        py::list<NodeP> inits;
        if (init) {
            if (kind_is(*init, "DeclStmt")) {
                NodeOrList s = stmt(*init);
                if (std::holds_alternative<py::list<NodeP>>(s)) inits = std::get<py::list<NodeP>>(s);
                else inits = list_of(std::get<NodeP>(s));
            } else {
                for (const NodeP& e : commas_(expr(*init))) inits.append(mk_expr_stmt(e, p));
            }
        }
        py::list<NodeP> update;
        if (inc) update = commas_(expr(*inc));
        auto f = mk<ir::For>(p);
        f->init = inits;
        f->cond = cond ? expr(*cond) : NodeP();
        f->update = update;
        if (!body) none_attr("get");
        f->body = as_stmt_(stmt(*body));
        return NodeP(f);
    }
    if (k == K("SwitchStmt")) {
        int64_t i = int64_t(is_truthy(vget(n, "hasInit"))) + int64_t(is_truthy(vget(n, "hasVar")));
        if (i) return NodeP(mk_expr_stmt(unsupported(K("switch-declaration"), n), p));
        return switch_(n, inner[0], inner[1], p);
    }
    if (k == K("BreakStmt")) {
        auto b = mk<ir::Break>(p);
        b->label = std::nullopt;
        return NodeP(b);
    }
    if (k == K("ContinueStmt")) {
        auto b = mk<ir::Continue>(p);
        b->label = std::nullopt;
        return NodeP(b);
    }
    if (k == K("GotoStmt")) {
        auto g = mk<ir::Goto>(p);
        auto lab = dget(labels, vget(n, "targetLabelDeclId"));
        g->label = lab ? *lab : OptStr(K("L"));
        return NodeP(g);
    }
    if (k == K("LabelStmt")) {
        auto l = mk<ir::Labeled>(p);
        l->label = opt_str(n, "name");
        if (!inner.empty()) l->body = as_stmt_(stmt(inner[0]));
        else l->body = NodeP(mk<ir::Empty>(p));
        return NodeP(l);
    }
    if (in_list(k, {"CaseStmt", "DefaultStmt"})) return NodeP(mk_expr_stmt(unsupported(K("case-outside-switch-body"), n), p));
    if (k == K("AttributedStmt")) return stmt(inner[-1]);
    if (k == K("CXXTryStmt")) {
        NodeOrList body = stmt(inner[0]);
        py::list<NodeP> catches;
        for (int64_t i = 1; i < inner.size(); ++i) {
            const Dict& c = inner[i];
            py::list<Dict> ci = inner_or(c, "inner");
            std::optional<Dict> var;
            if (!ci.empty() && kind_is(ci[0], "VarDecl")) var = ci[0];
            const Dict& blk = ci[-1];
            auto cat_ = mk<ir::Catch>(pos(c));
            if (var) {
                TypeP t = ty(*var);
                Scope& sc = scope_of(scope);
                SymP s = mk_sym("local", local_name_(*var, sc), sc.owner, false, t);
                sc.syms.set(val_key(vat(*var, "id")), s);
                cat_->sym = s;
                cat_->types = py::list<TypeP>{t};
                cat_->body = ir::node_of(stmt(blk));
            } else {
                cat_->sym = nullptr;
                cat_->types = py::list<TypeP>();
                cat_->body = ir::node_of(stmt(blk));
            }
            catches.append(cat_);
        }
        auto tr = mk<ir::Try>(p);
        tr->resources = py::list<NodeP>();
        tr->body = ir::node_of(body);
        tr->catches = catches;
        tr->final = nullptr;
        return NodeP(tr);
    }
    if (k == K("CXXForRangeStmt")) return range_for(n, p);
    if (k == K("CXXThrowExpr")) {
        auto t = mk<ir::Throw>(p);
        if (!inner.empty()) t->expr = expr(inner[0]);
        return NodeP(t);
    }
    if (k == K("ExprWithCleanups") && !inner.empty() && kind_is(inner[0], "CXXThrowExpr")) return stmt(inner[0]);
    if (k && (k->endswith(K("Expr")) || k->endswith(K("Operator")) || *k == K("ExprWithCleanups"))) {
        return NodeP(mk_expr_stmt(expr(n), p));
    }
    return NodeP(mk_expr_stmt(unsupported(K("stmt:") + str_of(vget(n, "kind")), n), p));
}

NodeOrList Lowering::switch_(const Dict& n, const Dict& cond_n, const Dict& body_n, const Pos& p) {
    py::list<NodeP> cases;
    py::list<Dict> items;
    if (kind_is(body_n, "CompoundStmt")) items = inner_get(body_n, "inner");
    else items.append(body_n);
    py::list<NodeP> pre;
    for (const Dict& it : items) {
        Dict cur = it;
        py::list<NodeP> labs;
        bool default_ = false;
        while (in_list(kind_of(cur), {"CaseStmt", "DefaultStmt"})) {
            if (kind_is(cur, "CaseStmt")) {
                std::optional<int64_t> v = const_(sub_at(cur, "inner", 0));
                NodeP lab = expr(sub_at(cur, "inner", 0));
                if (v) lab = mk_lit(Val(*v), ty(sub_at(cur, "inner", 0)), pos(cur));
                labs.append(lab);
                py::list<Dict> ci = inner_get(cur, "inner");
                if (ci.size() > 1) {
                    cur = ci[-1];
                } else {
                    cur = Dict();
                    cur.set(K("kind"), Val("NullStmt"));
                }
            } else {
                default_ = true;
                if (is_truthy(vget(cur, "inner"))) {
                    cur = sub_at(cur, "inner", -1);
                } else {
                    cur = Dict();
                    cur.set(K("kind"), Val("NullStmt"));
                }
            }
        }
        if (!labs.empty() || default_ || in_list(kind_of(it), {"CaseStmt", "DefaultStmt"})) {
            NodeOrList body = stmt(cur);
            auto c = mk<ir::Case>(pos(it));
            c->labels = labs;
            c->default_ = default_;
            if (std::holds_alternative<NodeP>(body) && is_a<ir::Empty>(std::get<NodeP>(body))) {
                c->body = py::list<NodeP>();
            } else {
                py::list<NodeOrList> one;
                one.append(body);
                c->body = flat_(one);
            }
            cases.append(c);
        } else if (!cases.empty()) {
            NodeOrList s = stmt(it);
            auto last = as_node<ir::Case>(cases[-1]);
            if (std::holds_alternative<py::list<NodeP>>(s)) last->body->extend(std::get<py::list<NodeP>>(s));
            else last->body->append(std::get<NodeP>(s));
        } else {
            NodeOrList s = stmt(it);
            if (std::holds_alternative<py::list<NodeP>>(s)) pre.extend(std::get<py::list<NodeP>>(s));
            else pre.append(std::get<NodeP>(s));
        }
    }
    if (case_inside_(body_n)) return NodeP(mk_expr_stmt(unsupported(K("case-label-nested-in-a-statement"), n), p));
    auto sw = mk<ir::Switch>(p);
    sw->expr = expr(cond_n);
    sw->cases = cases;
    for (const NodeP& x : pre) {
        if (!is_a<ir::Local>(x) && !is_a<ir::Empty>(x)) {
            return NodeP(mk_expr_stmt(unsupported(K("code-before-first-case"), n), p));
        }
    }
    if (!pre.empty()) return NodeP(mk_block(pre + list_of(sw), p));
    return NodeP(sw);
}

bool Lowering::case_inside_(const Dict& body_n) {
    py::list<Dict> top;
    if (kind_is(body_n, "CompoundStmt")) top = inner_get(body_n, "inner");
    else top.append(body_n);
    std::function<bool(const Val&, int64_t)> has_ = [&](const Val& xv, int64_t depth) -> bool {
        if (!holds<Dict>(xv)) return false;
        const Dict& x = as<Dict>(xv);
        OptStr xk = kind_of(x);
        if (in_list(xk, {"CaseStmt", "DefaultStmt"}) && depth > 0) return true;
        if (xk == K("SwitchStmt")) return false;
        int64_t nxt = depth + (in_list(xk, {"CaseStmt", "DefaultStmt"}) ? 0 : 1);
        Val iv = vget(x, "inner");
        if (!is_truthy(iv)) return false;
        if (!holds<Val::List>(iv)) type_error("'" + pyname(iv) + "' object is not iterable");
        for (const Val& c : as<Val::List>(iv)) {
            if (has_(c, nxt)) return true;
        }
        return false;
    };
    for (const Dict& c : top) {
        if (has_(Val(c), 0)) return true;
    }
    return false;
}

NodeP Lowering::range_for(const Dict& n, const Pos& p) {
    py::list<Dict> inner = inner_or(n, "inner");
    std::optional<Dict> var;
    std::optional<Dict> rng;
    for (const Dict& c : inner) {
        if (kind_is(c, "DeclStmt")) {
            for (const Dict& d : inner_get(c, "inner")) {
                if (kind_is(d, "VarDecl") && is_str_val(vget(d, "name"), "__range1") && is_truthy(vget(d, "inner"))) {
                    rng = sub_at(d, "inner", 0);
                } else if (kind_is(d, "VarDecl")) {
                    Val nm = vget(d, "name");
                    py::str nms = is_truthy(nm) ? str_in(nm) : K("");
                    if (!nms.startswith(K("__"))) var = d;
                }
            }
        }
    }
    const Dict& body = inner[-1];
    if (!var || !rng) return mk_expr_stmt(unsupported(K("range-for"), n), p);
    NodeP it = expr(*rng);
    TypeP itt = type_of(it);
    if (kind_attr(itt) != K("array")) return mk_expr_stmt(unsupported(K("range-for:") + spell(itt), n), p);
    TypeP t = ty(*var);
    Scope& sc = scope_of(scope);
    SymP s = mk_sym("local", local_name_(*var, sc), sc.owner, false, t);
    sc.syms.set(val_key(vat(*var, "id")), s);
    auto loc = mk<ir::Local>(pos(*var));
    loc->sym = s;
    loc->type = t;
    loc->init = nullptr;
    loc->mods = py::list<py::str>();
    auto fe = mk<ir::ForEach>(p);
    fe->var = loc;
    fe->iterable = it;
    fe->body = as_stmt_(stmt(body));
    return fe;
}

py::ref<ir::Local> Lowering::local(const Dict& d) {
    TypeP t = ty(d);
    Scope& sc = scope_of(scope);
    py::str name = local_name_(d, sc);
    SymP s = mk_sym("local", name, sc.owner, false, t);
    sc.syms.set(val_key(vat(d, "id")), s);
    NodeP init;
    for (const Dict& c : inner_get(d, "inner")) {
        if (!kind_s(c).endswith(K("Attr"))) {
            init = expr(c);
            if (t->kind == K("ptr") && is_truthy(t->info.get(K("ref"), Val()))) init = mk_unary("&", init, t, pos(c));
        }
    }
    TypeP t2 = complete_array_(t, init);
    if (t2.get() != t.get()) {
        t = t2;
        s->type = t;
    }
    py::list<py::str> mods;
    if (is_str_val(vget(d, "storageClass"), "static")) mods.append(K("static"));
    if (is_str_val(vget(d, "storageClass"), "extern")) mods = py::list<py::str>{K("extern")};
    auto l = mk<ir::Local>(pos(d));
    l->sym = s;
    l->type = t;
    l->init = init;
    l->mods = mods;
    return l;
}

// ---------------------------------------------------------------------------------------- expressions
NodeP Lowering::zero(const TypeP& t, const Pos& p) {
    if (t->kind == K("class") || t->kind == K("record")) {
        return mk_new(mk_sym("ctor", K("<aggregate>"), spell(t)), py::list<NodeP>(), "stack", t, p);
    }
    if (t->kind == K("array")) {
        auto a = mke<ir::NewArray>(t, p);
        a->elem = t->elem;
        a->dims = py::list<NodeP>();
        a->init = py::list<NodeP>();
        return a;
    }
    if (t->kind == K("ptr") || t->kind == K("fnptr")) return mk_lit(Val(), t, p);
    if (t->kind == K("f32") || t->kind == K("f64") || t->kind == K("f80")) return mk_lit(Val(0.0), t, p);
    if (t->kind == K("bool")) return mk_lit(Val(false), t, p);
    return mk_lit(Val(int64_t(0)), t, p);
}

NodeP Lowering::expr(const Dict& n) {
    try {
        return expr_(n);
    } catch (const py::Exception& e) {
        if (!soft_error(e)) throw;
        py::str m = message_of(e);
        if (m.size() > 200) m = m.slice(0, 200);
        return unsupported(K("expr:") + str_of(vget(n, "kind")), n, m);
    }
}

NodeP Lowering::expr_(const Dict& n) {
    OptStr k = kind_of(n);
    Pos p = pos(n);
    py::list<Dict> inner = inner_or(n, "inner");
    TypeP t = ty(n);
    if (k == K("IntegerLiteral")) return mk_lit(Val(to_int(vat(n, "value"))), t, p);
    if (k == K("CharacterLiteral")) {
        int64_t v = to_int(vat(n, "value"));
        OptStr txt = char_text_(v);
        return mk_lit(Val(v), t, p, txt);
    }
    if (k == K("FloatingLiteral")) return mk_lit(Val(to_float(vat(n, "value"))), t, p);
    if (k == K("StringLiteral")) return mk_lit(Val(esc_value_(vat(n, "value"))), t, p);
    if (k == K("CXXBoolLiteralExpr")) return mk_lit(Val(is_truthy(vat(n, "value"))), t, p);
    if (in_list(k, {"CXXNullPtrLiteralExpr", "GNUNullExpr"})) return mk_lit(Val(), ir::NULL_, p);
    if (k == K("PredefinedExpr")) return !inner.empty() ? expr(inner[0]) : NodeP(unsupported(*k, n));
    if (k == K("ParenExpr")) {
        auto pe = mke<ir::Paren>(t, p);
        pe->x = expr(inner[0]);
        return pe;
    }
    if (in_wrappers(k)) {
        if (k == K("CXXDefaultInitExpr") && inner.empty()) return unsupported(*k, n);
        if (k == K("SubstNonTypeTemplateParmExpr")) return expr(inner[-1]);   // [the parameter, the value it stands for]
        return expr(inner[0]);
    }
    if (k == K("CXXDefaultArgExpr")) return default_arg(n, p);
    if (k == K("ImplicitCastExpr")) return implicit_cast(n, inner[0], t, p);
    if (in_casts(k)) {
        std::optional<Dict> x;
        if (!inner.empty()) x = inner[0];
        if (k == K("CXXFunctionalCastExpr") && x && in_list(kind_of(*x), {"CXXConstructExpr", "InitListExpr"})) return expr(*x);
        OptStr ck = opt_str(n, "castKind");
        if (ck == K("ToVoid")) {
            if (!x) none_attr("get");
            return mk_cast(expr(*x), ir::VOID, p, false);
        }
        if (in_list(ck, {"ConstructorConversion", "NoOp"}) && t->kind == K("class")) {
            if (!x) none_attr("get");
            return expr(*x);
        }
        if (!x) none_attr("get");
        return mk_cast(expr(*x), t, p, false);
    }
    if (k == K("DeclRefExpr")) return decl_ref(n, t, p);
    if (k == K("MemberExpr")) return member(n, t, p);
    if (k == K("ArraySubscriptExpr")) {
        NodeP a = expr(inner[0]);
        NodeP b = expr(inner[1]);
        TypeP at = type_of(a);
        TypeP bt = type_of(b);
        if (at && at->kind != K("ptr") && at->kind != K("array") && bt && (bt->kind == K("ptr") || bt->kind == K("array"))) std::swap(a, b);
        auto ix = mke<ir::Index>(t, p);
        ix->arr = a;
        ix->index = b;
        return ix;
    }
    if (k == K("UnaryOperator")) return unary(n, t, p);
    if (k == K("BinaryOperator")) {
        OptStr op = opt_of(vat(n, "opcode"));
        NodeP l = expr(inner[0]);
        NodeP r = expr(inner[1]);
        if (op == K("=")) {
            auto a = mke<ir::Assign>(t, p);
            a->op = std::nullopt;
            a->target = l;
            a->value = r;
            return a;
        }
        if (in_ops(op)) {
            auto b = mke<ir::Binary>(t, p);
            b->op = op;
            b->left = l;
            b->right = r;
            return b;
        }
        return unsupported(cat("binary:", op), n);
    }
    if (k == K("CompoundAssignOperator")) {
        OptStr oc = opt_of(vat(n, "opcode"));
        if (!oc) type_error("'NoneType' object is not subscriptable");
        auto a = mke<ir::Assign>(t, p);
        a->op = oc->slice(0, oc->size() - 1);
        a->target = expr(inner[0]);
        a->value = expr(inner[1]);
        return a;
    }
    if (k == K("ConditionalOperator")) {
        auto c = mke<ir::Cond>(t, p);
        c->cond = expr(inner[0]);
        c->then = expr(inner[1]);
        c->orelse = expr(inner[2]);
        return c;
    }
    if (k == K("CallExpr")) return call(n, t, p);
    if (k == K("CXXMemberCallExpr")) return member_call(n, t, p);
    if (k == K("CXXOperatorCallExpr")) return operator_call(n, t, p);
    if (k == K("UnaryExprOrTypeTraitExpr")) return sizeof_(n);
    if (k == K("InitListExpr")) return init_list(n, t, p);
    if (k == K("ImplicitValueInitExpr")) return zero(t, p);
    if (k == K("CompoundLiteralExpr")) return expr(inner[0]);
    if (in_list(k, {"CXXConstructExpr", "CXXTemporaryObjectExpr"})) return construct(n, t, p);
    if (k == K("CXXNewExpr")) return new_(n, t, p);
    if (k == K("CXXDeleteExpr")) {
        auto u = mke<ir::Unary>(ir::VOID, p);
        u->op = is_truthy(vget(n, "isArray")) ? K("delete[]") : K("delete");
        u->x = expr(inner[0]);
        u->postfix = false;
        return u;
    }
    if (k == K("CXXThisExpr")) {
        auto th = mke<ir::This>(t, p);
        th->qualifier = nullptr;
        return th;
    }
    if (k == K("CXXScalarValueInitExpr")) return zero(t, p);
    if (k == K("VAArgExpr")) {   // va_arg(ap, T): the builtin, typed T
        auto s = std::make_shared<ir::Sym>(K("function"), K("__builtin_va_arg"), std::nullopt, true);
        s->params = py::list<TypeP>{ty(inner[0])};
        s->ret = t;
        return mk_call(nullptr, s, list_of(expr(inner[0])), t, p);
    }
    if (k == K("CXXThrowExpr")) return unsupported(K("throw-as-expression"), n);
    return unsupported(k, n);
}

NodeP Lowering::implicit_cast(const Dict& n, const Dict& x_n, const TypeP& t, const Pos& p) {
    OptStr ck = opt_str(n, "castKind");
    if (in_drop_casts(ck)) return expr(x_n);
    if (in_list(ck, {"UncheckedDerivedToBase", "DerivedToBase"})) {
        NodeP x = expr(x_n);
        return mk_cast(x, t, p, true);
    }
    if (in_list(ck, {"ConstructorConversion", "UserDefinedConversion"})) return expr(x_n);
    NodeP x = expr(x_n);
    if (ck == K("BuiltinFnToFnPtr")) return x;
    return mk_cast(x, t, p, true);
}

NodeP Lowering::default_arg(const Dict& n, const Pos& p) {
    (void)p;
    std::optional<Dict> d;
    Val pv = vget(n, "param");
    if (holds<Dict>(pv)) d = dget(decls, vget(as<Dict>(pv), "id"));
    if (d && is_truthy(vget(*d, "inner"))) return expr(sub_at(*d, "inner", -1));
    return unsupported(K("default-argument"), n);
}

NodeP Lowering::decl_ref(const Dict& n, const TypeP& t, const Pos& p) {
    Dict d = obj_or_empty(n, "referencedDecl");
    OptStr dk = kind_of(d);
    Val did = vget(d, "id");
    if (in_list(dk, {"VarDecl", "ParmVarDecl", "BindingDecl"})) {
        SymP s;
        if (scope) {
            s = scope->syms.get(val_key(did));
        }
        if (!s) {
            std::optional<Dict> found = dget(decls, did);
            Dict node = found ? *found : d;
            std::optional<Dict> par = found ? member_owner_(node) : dget(parent, did);
            if (par && in_list(kind_of(*par), {"CXXRecordDecl", "ClassTemplateSpecializationDecl"})) {
                TypeP rt = record_type_(par);
                TypeP st = ty(node);
                return mk_select(mk_typename(rt, p), mk_sym("field", str_in(vat(node, "name")), spell(rt), true, st), st, p);
            }
            TypeP st = ty(node);
            Val f = vget(node, "_f");
            OptStr own_ = is_own_(f) ? OptStr(unit_name) : lib_owner_(node);
            s = mk_sym("global", sym_name(vget(d, "name")), own_, true, st);
        }
        NodeP e = mk_name(s, s->type ? s->type : t, p);
        if (s->type && s->type->kind == K("ptr") && is_truthy(s->type->info.get(K("ref"), Val()))) return mk_unary("*", e, s->type->elem, p);
        return e;
    }
    if (in_list(dk, {"FunctionDecl", "CXXMethodDecl"})) {
        SymP s = sym_of_function_(d);
        std::optional<Dict> found = dget(decls, did);
        auto a = mke<ir::AddressOf>(ctypes::decay(ty(found ? *found : d)), p);
        a->sym = s;
        return a;
    }
    if (dk == K("EnumConstantDecl")) {
        std::optional<Dict> par = dget(parent, did);
        std::optional<int64_t> v = dget(enum_values, did);
        if (par) {
            auto qn = dget(scoped_enums, vat(*par, "id"));
            if (qn) {
                return mk_select(mk_typename(mk_type("class", *qn), p), mk_sym("field", str_in(vat(d, "name")), *qn, true, ir::I32), ir::I32, p);
            }
        }
        std::optional<Dict> found = dget(decls, did);
        Dict node = found ? *found : Dict();
        if (!is_own_(vget(node, "_f"))) {
            return mk_lit(v ? Val(*v) : Val(), ir::I32, p, str_in(vat(d, "name")));
        }
        return mk_name(mk_sym("global", str_in(vat(d, "name")), unit_name, true, ir::I32), ir::I32, p);
    }
    if (dk == K("NonTypeTemplateParmDecl")) return unsupported(K("template-parameter"), n);
    return unsupported(K("ref:") + str_of(vget(d, "kind")), n);
}

NodeP Lowering::member(const Dict& n, const TypeP& t, const Pos& p) {
    Dict base_n = sub_at(n, "inner", 0);
    NodeP base = expr(base_n);
    std::optional<Dict> md = dget(decls, vget(n, "referencedMemberDecl"));
    Val name = vget(n, "name");
    if (md && kind_is(*md, "CXXMethodDecl")) return unsupported(K("bound-member-function"), n);
    if (md && kind_is(*md, "VarDecl")) {   // a static member through an object
        std::optional<Dict> par = dget(parent, vat(*md, "id"));
        TypeP rt = record_type_(par);
        return mk_select(mk_typename(rt, p), mk_sym("field", sym_name(name), spell(rt), true, t), t, p);
    }
    NodeP obj = base;
    if (is_truthy(vget(n, "isArrow"))) {
        TypeP bt = type_of(base);
        TypeP el = (bt && bt->kind == K("ptr")) ? bt->elem : ir::ANY;
        obj = mk_unary("*", base, el, p);
    }
    TypeP ot = type_of(obj);
    OptStr owner;
    if (ot) owner = spell(ot);
    if (md) {
        std::optional<Dict> par = dget(parent, vat(*md, "id"));
        if (par && record_type_(par)) owner = spell(record_type_(par));
    }
    TypeP ft = t;
    if (md) ft = ty(*md);
    auto e = mk_select(obj, mk_sym("field", sym_name(name), owner, false, ft), ft, p);
    if (ft->kind == K("ptr") && is_truthy(ft->info.get(K("ref"), Val()))) return mk_unary("*", e, ft->elem, p);
    if (!md || *ft != *t) e->type = t->kind != K("any") ? t : ft;
    return e;
}

NodeP Lowering::unary(const Dict& n, const TypeP& t, const Pos& p) {
    OptStr op = opt_of(vat(n, "opcode"));
    Dict x_n = sub_at(n, "inner", 0);
    if (op == K("&")) {
        // &function: a function's address
        Dict inner = x_n;
        while (kind_is(inner, "ParenExpr")) inner = sub_at(inner, "inner", 0);
        if (kind_is(inner, "DeclRefExpr") && kind_is(obj_or_empty(inner, "referencedDecl"), "FunctionDecl")) return expr(inner);
    }
    NodeP x = expr(x_n);
    if (op == K("__extension__")) return x;
    if (op == K("++") || op == K("--")) {
        auto u = mke<ir::Unary>(t, p);
        u->op = op;
        u->x = x;
        u->postfix = is_truthy(vget(n, "isPostfix"));
        return u;
    }
    if (op == K("-") || op == K("+") || op == K("!") || op == K("~") || op == K("*") || op == K("&")) {
        TypeP xt = type_of(x);
        if (op == K("*") && xt && xt->kind == K("fnptr")) return x;   // (*fp)(...) is fp(...)
        auto u = mke<ir::Unary>(t, p);
        u->op = op;
        u->x = x;
        u->postfix = false;
        return u;
    }
    return unsupported(cat("unary:", op), n);
}

py::list<NodeP> Lowering::bind_refs(const py::list<NodeP>& args, const py::list<TypeP>& params) {
    py::list<NodeP> out_;
    for (int64_t i = 0; i < args.size(); ++i) {
        NodeP a = args[i];
        TypeP pt = i < params.size() ? params[i] : TypeP();
        if (pt && pt->kind == K("ptr") && is_truthy(pt->info.get(K("ref"), Val()))) {
            auto au = as_node<ir::Unary>(a);
            TypeP at = type_of(a);
            bool is_amp = au && au->op == K("&");
            bool is_refd = at && at->kind == K("ptr") && is_truthy(at->info.get(K("ref"), Val()));
            if (!is_amp && !is_refd) {
                Pos ap;
                ap.line = a->line;
                ap.col = a->col;
                a = mk_unary("&", a, pt, ap);
            }
        }
        out_.append(a);
    }
    return out_;
}

NodeP Lowering::call(const Dict& n, const TypeP& t, const Pos& p) {
    py::list<Dict> inner = inner_get(n, "inner");
    if (!has(n, "inner")) throw py::KeyError(py::repr(K("inner")));
    Dict callee = inner[0];
    Dict c = callee;
    while (in_list(kind_of(c), {"ImplicitCastExpr", "ParenExpr"}) && is_truthy(vget(c, "inner"))) {
        if (kind_is(c, "ImplicitCastExpr") &&
            !in_list(opt_str(c, "castKind"), {"FunctionToPointerDecay", "BuiltinFnToFnPtr", "NoOp"})) {
            break;
        }
        c = sub_at(c, "inner", 0);
    }
    py::list<NodeP> args;
    for (int64_t i = 1; i < inner.size(); ++i) args.append(expr(inner[i]));
    if (kind_is(c, "DeclRefExpr") && in_list(kind_of(obj_or_empty(c, "referencedDecl")), {"FunctionDecl", "CXXMethodDecl"})) {
        Dict d = obj_or_empty(c, "referencedDecl");
        SymP s = sym_of_function_(d);
        args = bind_refs(args, s->params);
        std::optional<Dict> found = dget(decls, vat(d, "id"));
        Dict node = found ? *found : Dict();
        if (kind_is(node, "CXXMethodDecl")) {   // a static member function
            std::optional<Dict> par = dget(decls, vget(node, "parentDeclContextId"));
            if (!par || par->empty()) par = dget(parent, vat(node, "id"));
            return mk_call(mk_typename(record_type_(par), p), s, args, t, p);
        }
        return mk_call(nullptr, s, args, t, p);
    }
    if (kind_is(c, "MemberExpr")) {
        Dict n2 = n.copy();
        Val::List ni;
        ni.append(Val(c));
        for (int64_t i = 1; i < inner.size(); ++i) ni.append(Val(inner[i]));
        n2.set(K("inner"), Val(ni));
        return member_call(n2, t, p);
    }
    NodeP fn = expr(callee);
    return mk_call(fn, nullptr, args, t, p);
}

NodeP Lowering::member_call(const Dict& n, const TypeP& t, const Pos& p) {
    Val iv = vat(n, "inner");
    if (!holds<Val::List>(iv)) type_error("'" + pyname(iv) + "' object is not subscriptable");
    py::list<Dict> inner;
    for (const Val& x : as<Val::List>(iv)) inner.append(as_dict_get(x));
    Dict me = inner[0];
    while (kind_is(me, "ParenExpr")) me = sub_at(me, "inner", 0);
    if (!kind_is(me, "MemberExpr")) return unsupported(K("member-call:") + str_of(vget(me, "kind")), n);
    std::optional<Dict> found = dget(decls, vget(me, "referencedMemberDecl"));
    Dict md;
    if (found && !found->empty()) {
        md = *found;
    } else {
        md.set(K("id"), vget(me, "referencedMemberDecl"));
        md.set(K("name"), vget(me, "name"));
        md.set(K("kind"), Val("CXXMethodDecl"));
    }
    NodeP base = expr(sub_at(me, "inner", 0));
    NodeP obj = base;
    if (is_truthy(vget(me, "isArrow"))) {
        TypeP bt = type_of(base);
        obj = mk_unary("*", base, kind_attr(bt) == K("ptr") ? bt->elem : ir::ANY, p);
    }
    SymP s = sym_of_function_(md);
    py::list<NodeP> argl;
    for (int64_t i = 1; i < inner.size(); ++i) argl.append(expr(inner[i]));
    py::list<NodeP> args = bind_refs(argl, s->params);
    if (kind_is(md, "CXXConversionDecl")) return mk_cast(obj, t, p, false);
    return mk_call(obj, s, args, t, p);
}

NodeP Lowering::operator_call(const Dict& n, const TypeP& t, const Pos& p) {
    py::list<Dict> inner = inner_get(n, "inner");
    if (!has(n, "inner")) throw py::KeyError(py::repr(K("inner")));
    Dict c = inner[0];
    while (in_list(kind_of(c), {"ImplicitCastExpr", "ParenExpr"}) && is_truthy(vget(c, "inner"))) c = sub_at(c, "inner", 0);
    Dict d = obj_or_empty(c, "referencedDecl");
    std::optional<Dict> found = dget(decls, vget(d, "id"));
    Dict node = found ? *found : d;
    py::list<NodeP> args;
    for (int64_t i = 1; i < inner.size(); ++i) args.append(expr(inner[i]));
    Val name = vget(d, "name", Val(K("")));
    if (is_truthy(vget(node, "isImplicit")) || is_truthy(vget(node, "explicitlyDefaulted"))) {
        if (is_str_val(name, "operator=") && args.size() == 2) {
            auto a = mke<ir::Assign>(t, p);
            a->op = std::nullopt;
            a->target = args[0];
            a->value = args[1];
            return a;
        }
    }
    SymP s = sym_of_function_(d);
    if (kind_is(node, "CXXMethodDecl")) {
        return mk_call(args[0], s, bind_refs(args.slice(1, args.size()), s->params), t, p);
    }
    return mk_call(nullptr, s, bind_refs(args, s->params), t, p);
}

NodeP Lowering::construct(const Dict& n, const TypeP& t, const Pos& p) {
    py::list<NodeP> args;
    for (const Dict& a : inner_or(n, "inner")) args.append(expr(a));
    Val ctor_t = vget(obj_or_empty(n, "ctorType"), "qualType", Val(K("")));
    std::optional<Dict> cls = t->kind == K("class") ? find_record_(spell(t)) : std::nullopt;
    std::optional<Dict> ctor = cls ? find_ctor_(*cls, ctor_t) : std::nullopt;
    bool copying = false;
    if (args.size() == 1) {
        TypeP at = type_of(args[0]);
        copying = at && spell(at) == spell(t);
    }
    if (copying && (!ctor || is_truthy(vget(*ctor, "isImplicit")) || is_truthy(vget(*ctor, "explicitlyDefaulted")))) {
        return args[0];   // a copy or a move: the value itself
    }
    if (t->kind != K("class")) return !args.empty() ? args[0] : zero(t, p);
    if (!ctor || is_truthy(vget(*ctor, "isImplicit")) || is_truthy(vget(*ctor, "explicitlyDefaulted"))) {
        if (args.empty()) return mk_new(mk_sym("ctor", K("<aggregate>"), spell(t)), py::list<NodeP>(), "stack", t, p);
    }
    SymP sym = ctor ? sym_of_function_(*ctor) : mk_sym("ctor", t->name ? *t->name : K(""), spell(t));
    return mk_new(sym, bind_refs(args, sym->params), "stack", t, p);
}

std::optional<Dict> Lowering::find_ctor_(const Dict& cls, const Val& ctor_t) {
    for (const Dict& m : inner_get(cls, "inner")) {
        if (kind_is(m, "CXXConstructorDecl") && same_val(vget(obj_or_empty(m, "type"), "qualType"), ctor_t)) return m;
    }
    for (const Dict& m : inner_get(cls, "inner")) {
        if (kind_is(m, "FunctionTemplateDecl")) {
            for (const Dict& x : inner_get(m, "inner")) {
                if (kind_is(x, "CXXConstructorDecl") && same_val(vget(obj_or_empty(x, "type"), "qualType"), ctor_t)) return x;
            }
        }
    }
    return std::nullopt;
}

NodeP Lowering::new_(const Dict& n, const TypeP& t, const Pos& p) {
    py::list<Dict> inner = inner_or(n, "inner");
    TypeP el = t->kind == K("ptr") ? t->elem : ir::ANY;
    if (is_truthy(vget(n, "isArray"))) {
        NodeP size;
        std::optional<py::list<NodeP>> init;
        for (const Dict& c : inner) {
            if (kind_is(c, "InitListExpr")) {
                py::list<NodeP> items;
                for (const Dict& x : inner_get(c, "inner")) items.append(expr(x));
                init = items;
            } else if (!size && !kind_is(c, "CXXConstructExpr")) {
                size = expr(c);
            }
        }
        auto a = mke<ir::NewArray>(t, p);
        a->elem = el;
        a->dims = size ? list_of(size) : py::list<NodeP>();
        a->init = init;
        return a;
    }
    if (!inner.empty()) {
        NodeP e = expr(inner[-1]);
        auto en = as_node<ir::New>(e);
        if (en) {
            en->memory = K("manual");
            en->type = t;
            return en;
        }
        return mk_new(mk_sym("ctor", K("<value>"), spell(el)), list_of(e), "manual", t, p);
    }
    return mk_new(mk_sym("ctor", K("<value>"), spell(el)), py::list<NodeP>(), "manual", t, p);
}

NodeP Lowering::sizeof_(const Dict& n) {
    Pos p = pos(n);
    Val nv = vget(n, "name", Val(K("sizeof")));
    py::str name = str_in(nv);
    TypeP at;
    py::str text;
    if (has(n, "argType")) {
        Val qt = vat2(n, "argType", "qualType");
        at = ctypes::normalise(types.parse(parse_text(qt)));
        text = name + K("(") + str_of(qt) + K(")");
    } else {
        Dict arg = sub_at(n, "inner", 0);
        at = ty(arg);
        text = name + K(" ") + K("expr");
    }
    if (name != K("sizeof") && name != K("alignof") && name != K("_Alignof") && name != K("__alignof")) {
        return unsupported(K("trait:") + name, n);
    }
    int64_t v = name == K("sizeof") ? sizes.size(at) : sizes.align(at);
    return mk_lit(Val(v), ty(n), p, text);
}

NodeP Lowering::init_list(const Dict& n, const TypeP& t, const Pos& p) {
    py::list<Dict> inner = inner_or(n, "inner");
    std::optional<Dict> filler;
    if (has(n, "array_filler")) {   // clang writes the filler first, then the elements given
        Val af = vat(n, "array_filler");
        if (!holds<Val::List>(af)) type_error("'" + pyname(af) + "' object is not subscriptable");
        const Val::List& l = as<Val::List>(af);
        filler = as_dict_get(l[0]);
        inner = py::list<Dict>();
        for (int64_t i = 1; i < l.size(); ++i) inner.append(as_dict_get(l[i]));
    }
    if (t->kind == K("array")) {
        py::list<NodeP> items;
        for (const Dict& x : inner) items.append(expr(x));
        if (filler && !kind_is(*filler, "ImplicitValueInitExpr") && t->info.contains(K("count"))) {
            NodeP fill = expr(*filler);
            int64_t count = to_int(vat(t->info, "count"));
            for (int64_t i = items.size(); i < count; ++i) items.append(fill);
        }
        // a string literal initialising a char array
        if (items.size() == 1) {
            auto l = as_node<ir::Lit>(items[0]);
            if (l && holds<py::str>(l->value)) {
                const py::str& ek = kind_attr(t->elem);
                if (ek == K("char") || ek == K("i8") || ek == K("u8")) return items[0];
            }
        }
        auto a = mke<ir::NewArray>(t, p);
        a->elem = t->elem;
        a->dims = py::list<NodeP>();
        a->init = items;
        return a;
    }
    if (t->kind == K("class")) {
        py::list<NodeP> args;
        if (has(n, "field")) {   // a union: the member it initialises
            Dict f = as_dict_get(vat(n, "field"));
            for (const Dict& x : inner) args.append(expr(x));
            return mk_new(mk_sym("ctor", K("<aggregate:") + str_of(vget(f, "name")) + K(">"), spell(t)), args, "stack", t, p);
        }
        for (const Dict& x : inner) args.append(expr(x));
        return mk_new(mk_sym("ctor", K("<aggregate>"), spell(t)), args, "stack", t, p);
    }
    if (inner.size() == 1) return expr(inner[0]);   // int x = {5};
    if (inner.empty()) return zero(t, p);
    return unsupported(K("init-list:") + spell(t), n);
}

}  // namespace codebreaker::translate::frontends::clang::lower

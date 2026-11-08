// codebreaker/translate/frontends/clang/ctypes.cpp -- typed C++ port of
// codebreaker.translate.frontends.clang.ctypes (see ctypes.hpp).

#include "codebreaker/translate/frontends/clang/ctypes.hpp"

#include <algorithm>

#include "py_os.hpp"

namespace codebreaker::translate::frontends::clang::ctypes {

namespace {

using ir::Dict;
using ir::TypeP;
using ir::Val;
using Tok = std::optional<py::str>;

// ---- the Python's module-level tables
const py::dict<int64_t, py::str> INT_NAMES = {{1, py::str("i8")}, {2, py::str("i16")}, {4, py::str("i32")}, {8, py::str("i64")}};
const py::dict<int64_t, py::str> UINT_NAMES = {{1, py::str("u8")}, {2, py::str("u16")}, {4, py::str("u32")}, {8, py::str("u64")}};

const py::set<py::str> QUALS = {py::str("const"), py::str("volatile"), py::str("restrict"), py::str("__restrict"),
                                py::str("__unaligned"), py::str("_Atomic"), py::str("struct"), py::str("union"),
                                py::str("enum"), py::str("class"), py::str("typename"), py::str("__ptr64"),
                                py::str("__ptr32"), py::str("__w64"),
                                // calling conventions: one convention on the x64 target
                                py::str("__cdecl"), py::str("__stdcall"), py::str("__fastcall"), py::str("__thiscall"),
                                py::str("__vectorcall"), py::str("__clrcall")};
const py::set<py::str> BUILTIN = {py::str("void"), py::str("char"), py::str("short"), py::str("int"), py::str("long"),
                                  py::str("float"), py::str("double"), py::str("signed"), py::str("unsigned"),
                                  py::str("_Bool"), py::str("bool"), py::str("wchar_t"), py::str("char8_t"),
                                  py::str("char16_t"), py::str("char32_t"), py::str("__int64"), py::str("__int32"),
                                  py::str("__int16"), py::str("__int8"), py::str("__int128"), py::str("_Float16"),
                                  py::str("__fp16")};
const py::dict<py::str, int64_t> PRIM_SIZE = {
    {py::str("bool"), 1}, {py::str("i8"), 1},  {py::str("u8"), 1},  {py::str("char"), 1}, {py::str("i16"), 2},
    {py::str("u16"), 2},  {py::str("i32"), 4}, {py::str("u32"), 4}, {py::str("i64"), 8},  {py::str("u64"), 8},
    {py::str("f32"), 4},  {py::str("f64"), 8}, {py::str("f80"), 16}};

// _TOKEN (compiled on first use)
const py::re::Pattern& token_pattern() {
    static const py::re::Pattern p(py::str("\\s*(\\.\\.\\.|::|&&|[A-Za-z_~][\\w]*|\\d+|[*&()\\[\\],<>:])"), 0);
    return p;
}

// ---- small helpers
[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

// d[key] for a str key (KeyError(key) as Python's)
template <class V> V lookup(const py::dict<int64_t, V>& d, int64_t key) {
    if (!d.contains(key)) throw py::KeyError(py::to_str(key));
    return d.at(key);
}
const Val& dget(const Dict& d, const char* key) {
    if (!d.contains(py::str(key))) throw py::KeyError(py::repr(py::str(key)));
    return d.at(py::str(key));
}

TypeP mk(const char* kind, const std::optional<py::str>& name = std::nullopt, const py::list<TypeP>& args = py::list<TypeP>(),
         const TypeP& elem = nullptr, const Dict& info = Dict()) {
    return std::make_shared<ir::Type>(py::str(kind), name, args, elem, std::nullopt, info);
}

// a Type as a Val (None for a null one)
Val val_of(const TypeP& t) { return t ? Val(t) : Val(); }

// the Type a Val holds (None: null)
TypeP type_of(const Val& v) {
    if (v.is_none()) return nullptr;
    if (const TypeP* t = std::get_if<TypeP>(&v.v)) return *t;
    throw py::AttributeError("object has no attribute 'kind'");
}

const Dict& dict_of(const Val& v) {
    if (const Dict* d = std::get_if<Dict>(&v.v)) return *d;
    throw py::TypeError("'dict' expected");
}

// the truth of a value
bool truthy(const Val& v) {
    return std::visit(
        [](const auto& x) -> bool {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return false;
            else if constexpr (std::is_same_v<T, TypeP>) return true;
            else return py::truthy(x);
        },
        v.v);
}

// an integer value (a bool counts as 0 or 1)
int64_t int_of(const Val& v) {
    if (const int64_t* i = std::get_if<int64_t>(&v.v)) return *i;
    if (const bool* b = std::get_if<bool>(&v.v)) return *b ? 1 : 0;
    throw py::TypeError("an integer is required");
}

// a // b (the runtime's floordiv words the zero-division message as an older Python's)
int64_t floor_div(int64_t a, int64_t b) {
    if (b == 0) throw py::ZeroDivisionError("division by zero");
    return py::floordiv(a, b);
}

bool is(const Tok& t, const char* s) { return t && *t == py::str(s); }

py::str tok_repr(const Tok& t) { return t ? py::repr(*t) : py::str("None"); }

bool in_set(const Tok& t, std::initializer_list<const char*> xs) {
    for (const char* x : xs) {
        if (is(t, x)) return true;
    }
    return false;
}

// int(token)
int64_t int_of_token(const Tok& t) {
    if (!t) throw py::TypeError("int() argument must be a string, a bytes-like object or a real number, not 'NoneType'");
    const std::u32string& u = t->u32();
    bool ok = !u.empty();
    for (char32_t c : u) {
        if (c < U'0' || c > U'9') ok = false;
    }
    if (!ok) throw py::ValueError(py::str("invalid literal for int() with base 10: ") + py::repr(*t));
    int64_t v = 0;
    for (char32_t c : u) {
        int d = static_cast<int>(c - U'0');
        if (v > (INT64_MAX - d) / 10) throw py::OverflowError("int too large for int64");
        v = v * 10 + d;
    }
    return v;
}

}  // namespace

// ------------------------------------------------------------------------------------------ data models
Model::Model(const py::str& triple_, int64_t pointer_, int64_t long__, int64_t wchar_, bool char_signed_, int64_t long_double_)
    : triple(triple_), pointer(pointer_), long_(long__), wchar(wchar_), char_signed(char_signed_), long_double(long_double_) {}

Model Model::for_triple(const py::str& triple) {
    py::str t = triple.lower();
    bool wide = t.startswith(py::str("x86_64")) || t.startswith(py::str("aarch64"));
    if (t.contains(py::str("windows")) || t.contains(py::str("win32")) || t.contains(py::str("mingw"))) {
        return Model(triple, wide ? 8 : 4, 4, 2);
    }
    return Model(triple, wide ? 8 : 4, wide ? 8 : 4, 4, true, 16);
}

Dict Model::json(const py::str& language, const py::str& std_) const {
    Dict d;
    d.set(py::str("triple"), Val(triple));
    d.set(py::str("pointer"), Val(pointer));
    d.set(py::str("long"), Val(long_));
    d.set(py::str("wchar"), Val(wchar));
    d.set(py::str("char_signed"), Val(char_signed));
    d.set(py::str("language"), Val(language));
    d.set(py::str("std"), Val(std_));
    return d;
}

TypeP builtin(const py::list<py::str>& words, const Model& model) {
    py::list<py::str> w = words.copy();
    bool uns = w.contains(py::str("unsigned"));
    bool sig = w.contains(py::str("signed"));
    int64_t longs = w.count(py::str("long"));
    auto has = [&](const char* s) { return w.contains(py::str(s)); };
    if (has("void")) return ir::VOID;
    if (has("_Bool") || has("bool")) return ir::BOOL;
    if (has("float")) return mk("f32");
    if (has("double")) return (longs == 0 || model.long_double == 8) ? mk("f64") : mk("f80");
    if (has("__int128")) throw py::TypeError("__int128");
    if (has("_Float16") || has("__fp16")) throw py::TypeError("half float");
    if (has("wchar_t")) {
        return mk((model.wchar == 2 ? lookup(UINT_NAMES, model.wchar) : lookup(INT_NAMES, model.wchar)).utf8().c_str());
    }
    if (has("char8_t")) return mk("u8");
    if (has("char16_t")) return mk("u16");
    if (has("char32_t")) return mk("u32");
    if (has("char")) {
        if (uns) return mk("u8");
        if (sig) return mk("i8");
        Dict info;
        info.set(py::str("signed"), Val(model.char_signed));
        return mk("char", std::nullopt, py::list<TypeP>(), nullptr, info);
    }
    int64_t size;
    if (has("__int8")) size = 1;
    else if (has("short") || has("__int16")) size = 2;
    else if (longs >= 2 || has("__int64")) size = 8;
    else if (longs == 1) size = model.long_;
    else size = 4;
    return mk((uns ? lookup(UINT_NAMES, size) : lookup(INT_NAMES, size)).utf8().c_str());
}

// ------------------------------------------------------------------------------------------ the parser
py::str anon_key_text(const py::str& text) {
    auto m = py::re::search(py::str(":(\\d+):(\\d+)\\)$"), text);
    if (!m) return py::str("0_0");
    return m->group(1) + py::str("_") + m->group(2);
}

py::list<py::str> tokens(const py::str& s_) {
    py::list<py::str> out;
    int64_t i = 0;
    py::str s = s_;
    s = py::re::sub(py::str("__attribute__\\(\\((?:[^()]|\\([^()]*\\))*\\)\\)"), py::str(" "), s);   // (cdecl, aligned, ...: not a type)
    auto unnamed = [](const py::re::Match& m) { return py::str("__unnamed_") + anon_key_text(m.group(0)); };
    s = py::re::sub(py::str("\\(unnamed (struct|union|enum|class) at [^)]*\\)"), unnamed, s);
    s = py::re::sub(py::str("\\(anonymous (struct|union|enum|class) at [^)]*\\)"), unnamed, s);
    s = py::re::sub(py::str("\\((?:unnamed|anonymous) at [^)]*\\)"),
                    [](const py::re::Match& m) { return py::str("struct __unnamed_") + anon_key_text(m.group(0)); }, s);
    s = py::re::sub(py::str("\\(lambda at [^)]*\\)"), py::str("__lambda"), s);
    while (i < s.size()) {
        auto m = token_pattern().match(s, i);
        if (!m) {
            if (s.slice_from(i).strip() == py::str("")) break;
            throw py::TypeError(py::str("cannot read type ") + py::repr(s) + py::str(" at ") + py::repr(s.slice_from(i)));
        }
        out.append(m->group(1));
        i = m->end();
    }
    return out;
}

py::str anon_key(int64_t line, int64_t col) { return py::to_str(line) + py::str("_") + py::to_str(col); }

Parser::Parser(const Model& model_, const Names& names_) : model(model_), names(names_) {}

TypeP Parser::parse(const py::str& text) {
    if (cache.contains(text)) return cache.at(text);
    py::list<py::str> saved_t = t;   // (a typedef's meaning is parsed mid-parse)
    int64_t saved_i = i;
    t = tokens(text);
    i = 0;
    TypeP r;
    try {
        r = type_();
        if (i != py::len(t)) {
            throw py::TypeError(py::str("trailing ") + ir::repr_of(t.slice_from(i)) + py::str(" in type ") + py::repr(text));
        }
    } catch (...) {
        t = saved_t;
        i = saved_i;
        throw;
    }
    t = saved_t;
    i = saved_i;
    cache.set(text, r);
    return r;
}

Tok Parser::peek(int64_t k) const {
    int64_t j = i + k;
    return j < py::len(t) ? Tok(t[j]) : std::nullopt;
}

Tok Parser::take() {
    Tok tok = peek();
    i += 1;
    return tok;
}

Tok Parser::take(const char* want) {
    Tok tok = peek();
    if (!is(tok, want)) {
        throw py::TypeError(py::str("expected ") + py::repr(py::str(want)) + py::str(", got ") + tok_repr(tok));
    }
    i += 1;
    return tok;
}

TypeP Parser::type_() {
    TypeP base = specifiers();
    return declarator(base);
}

TypeP Parser::specifiers() {
    py::list<py::str> words;
    std::optional<py::str> tag;
    std::optional<std::tuple<py::str, py::list<TypeP>>> named;
    while (true) {
        Tok tok = peek();
        if (!tok) break;
        if (in_set(tok, {"struct", "union", "enum", "class"})) {
            tag = tok;
            i += 1;
            continue;
        }
        if (QUALS.contains(*tok)) {
            i += 1;
            continue;
        }
        if (BUILTIN.contains(*tok) && !named && !tag) {
            words.append(*tok);
            i += 1;
            continue;
        }
        py::str first = (*tok)[0];
        if ((first.isalpha() || first == py::str("_") || first == py::str("~") || *tok == py::str("::")) && !py::truthy(words) && !named) {
            named = qualified_name();
            continue;
        }
        break;
    }
    if (py::truthy(words)) return builtin(words, model);
    if (!named) throw py::TypeError(py::str("no type in ") + ir::repr_of(t));
    const py::str& name = std::get<0>(*named);
    const py::list<TypeP>& args = std::get<1>(*named);
    if (tag && *tag == py::str("enum")) {
        TypeP got = names(name, args, tag);
        return got ? got : ir::I32;
    }
    TypeP got = names(name, args, tag);
    if (!got) throw py::TypeError(py::str("unknown type name ") + py::repr(name));
    return got;
}

std::tuple<py::str, py::list<TypeP>> Parser::qualified_name() {
    std::vector<Tok> parts;
    py::list<TypeP> args;
    if (is(peek(), "::")) i += 1;
    while (true) {
        parts.push_back(take());
        if (is(peek(), "<")) {
            i += 1;
            args = template_args();
        }
        if (is(peek(), "::")) {
            i += 1;
            if (!parts.empty() && py::truthy(args)) {   // (an argument list in the middle: kept on its part)
                if (!parts.back()) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'str'");
                py::str spelled;
                for (int64_t k = 0; k < py::len(args); ++k) spelled = spelled + py::str(k ? ", " : "") + args[k]->repr();
                parts.back() = *parts.back() + py::str("<") + spelled + py::str(">");
                args = py::list<TypeP>();
            }
            continue;
        }
        break;
    }
    py::str joined;
    for (size_t k = 0; k < parts.size(); ++k) {
        if (!parts[k]) {
            throw py::TypeError("sequence item " + std::to_string(k) + ": expected str instance, NoneType found");
        }
        joined = joined + py::str(k ? "::" : "") + *parts[k];
    }
    return std::make_tuple(joined, args);
}

py::list<TypeP> Parser::template_args() {
    py::list<TypeP> args;
    while (!(peek() == std::nullopt || is(peek(), ">"))) {
        if (peek()->isdigit() || in_set(peek(), {"true", "false"})) {   // a non-type argument: kept as its spelling
            args.append(mk("class", take()));
        } else {
            args.append(type_());
        }
        if (is(peek(), ",")) i += 1;
    }
    take(">");
    return args;
}

TypeP Parser::declarator(TypeP ty) {
    std::vector<py::str> ops;
    while (in_set(peek(), {"*", "&", "&&"}) || (peek() && QUALS.contains(*peek()))) {
        Tok tok = take();
        if (in_set(tok, {"*", "&", "&&"})) ops.push_back(*tok);
    }
    for (const py::str& op : ops) {
        Dict info;
        info.set(py::str("space"), Val("flat"));
        if (!(op == py::str("*"))) info.set(py::str("ref"), Val(true));
        ty = mk("ptr", std::nullopt, py::list<TypeP>(), ty, info);
    }
    std::optional<py::list<py::str>> inner;
    if (is(peek(), "(") && (in_set(peek(1), {"*", "&", "&&", "("}) ||
                            ((peek(1) && QUALS.contains(*peek(1))) && in_set(peek(2), {"*", "&", "&&"})))) {
        i += 1;
        int64_t depth = 1, start = i;
        while (depth) {
            Tok tok = take();
            depth += is(tok, "(") ? 1 : 0;
            depth -= is(tok, ")") ? 1 : 0;
        }
        inner = t.slice(start, i - 1);
    }
    ty = suffixes(ty);
    if (inner) {
        py::list<py::str> saved_t = t;
        int64_t saved_i = i;
        t = *inner;
        i = 0;
        ty = declarator(ty);   // (an exception leaves t and i as they are, as in the Python)
        t = saved_t;
        i = saved_i;
    }
    return ty;
}

TypeP Parser::suffixes(TypeP ty) {
    struct Suffix {
        bool array;
        std::optional<int64_t> n;
        py::list<TypeP> params;
        bool variadic;
    };
    std::vector<Suffix> sufs;
    while (in_set(peek(), {"[", "("})) {
        if (is(take(), "[")) {
            std::optional<int64_t> n;
            if (!is(peek(), "]")) n = int_of_token(take());
            take("]");
            sufs.push_back({true, n, py::list<TypeP>(), false});
        } else {
            py::list<TypeP> params;
            bool variadic = false;
            while (!is(peek(), ")")) {
                if (is(peek(), "...")) {
                    i += 1;
                    variadic = true;
                } else {
                    params.append(type_());
                }
                if (is(peek(), ",")) i += 1;
            }
            take(")");
            while (in_set(peek(), {"const", "volatile", "noexcept", "&", "&&", "__attribute__"})) i += 1;
            if (py::len(params) == 1 && ir::same(params[0], ir::VOID)) params = py::list<TypeP>();
            sufs.push_back({false, std::nullopt, params, variadic});
        }
    }
    for (size_t k = sufs.size(); k-- > 0;) {
        const Suffix& s = sufs[k];
        if (s.array) {
            Dict info;
            if (s.n) info.set(py::str("count"), Val(*s.n));
            ty = mk("array", std::nullopt, py::list<TypeP>(), ty, info);
        } else {
            ty = fn_type(ty, s.params, s.variadic);
        }
    }
    return ty;
}

TypeP fn_type(const TypeP& ret, const py::list<TypeP>& params, bool variadic, const std::optional<py::list<std::optional<py::str>>>& names) {
    Val::List ps;
    for (int64_t k = 0; k < py::len(params); ++k) {
        Dict p;
        p.set(py::str("type"), val_of(params[k]));
        if (names && py::truthy(*names) && py::truthy((*names)[k])) p.set(py::str("name"), Val(*(*names)[k]));
        ps.append(Val(p));
    }
    Dict result;
    result.set(py::str("type"), val_of(ret));
    result.set(py::str("at"), Val("none"));
    Dict info;
    info.set(py::str("conv"), Val("cdecl"));
    info.set(py::str("params"), Val(ps));
    info.set(py::str("result"), Val(result));
    if (variadic) info.set(py::str("variadic"), Val(true));
    return mk("fn", std::nullopt, py::list<TypeP>(), nullptr, info);
}

TypeP decay(const TypeP& t) {
    if (!t) none_attr("kind");
    if (t->kind == py::str("fn")) {
        Dict info;
        info.set(py::str("space"), Val("flat"));
        info.set(py::str("fn"), Val(t));
        return mk("fnptr", std::nullopt, py::list<TypeP>(), nullptr, info);
    }
    if (t->kind == py::str("array")) {
        Dict info;
        info.set(py::str("space"), Val("flat"));
        return mk("ptr", std::nullopt, py::list<TypeP>(), t->elem, info);
    }
    return t;
}

TypeP normalise(const TypeP& t) {
    if (!t) return t;
    if (t->kind == py::str("ptr")) {
        TypeP e = normalise(t->elem);
        if (!e) none_attr("kind");
        auto ref = t->info.get(py::str("ref"));
        if (e->kind == py::str("fn") && !(ref && truthy(*ref))) {
            Dict info;
            info.set(py::str("space"), Val("flat"));
            info.set(py::str("fn"), Val(e));
            return mk("fnptr", std::nullopt, py::list<TypeP>(), nullptr, info);
        }
        return mk("ptr", std::nullopt, py::list<TypeP>(), e, t->info.copy());
    }
    if (t->kind == py::str("array")) {
        return mk("array", std::nullopt, py::list<TypeP>(), normalise(t->elem), t->info.copy());
    }
    if (t->kind == py::str("fn")) {
        Dict info = t->info.copy();
        Val::List ps;
        if (info.contains(py::str("params"))) {
            const Val* pv = &info.at(py::str("params"));
            const Val::List* list = std::get_if<Val::List>(&pv->v);
            if (!list) throw py::TypeError("params is not a list");
            for (const Val& p : *list) {
                Dict q = dict_of(p).copy();
                q.set(py::str("type"), val_of(normalise(type_of(dget(dict_of(p), "type")))));
                ps.append(Val(q));
            }
        }
        info.set(py::str("params"), Val(ps));
        Dict result = dict_of(dget(info, "result")).copy();
        auto rt = dict_of(dget(info, "result")).get(py::str("type"));
        result.set(py::str("type"), val_of(normalise(rt ? type_of(*rt) : nullptr)));
        info.set(py::str("result"), Val(result));
        return mk("fn", std::nullopt, py::list<TypeP>(), nullptr, info);
    }
    if (t->kind == py::str("class") && py::truthy(t->args)) {
        py::list<TypeP> args;
        for (const TypeP& a : t->args) args.append(normalise(a));
        return mk("class", t->name, args);
    }
    return t;
}

// ------------------------------------------------------------------------------------------ sizes
Layouts::Layouts(const Model& model_, const Record& record_) : model(model_), record(record_) {}

int64_t Layouts::size(const TypeP& t) const { return std::get<0>(size_align(t)); }

int64_t Layouts::align(const TypeP& t) const { return std::get<1>(size_align(t)); }

std::tuple<int64_t, int64_t> Layouts::size_align(const TypeP& t) const {
    if (!t) none_attr("kind");
    const py::str& k = t->kind;
    if (PRIM_SIZE.contains(k)) {
        int64_t s = PRIM_SIZE.at(k);
        return {s, s};
    }
    if (k == py::str("ptr") || k == py::str("fnptr")) return {model.pointer, model.pointer};
    if (k == py::str("array")) {
        auto sa = size_align(t->elem);
        auto count = t->info.get(py::str("count"));
        return {std::get<0>(sa) * (count ? int_of(*count) : 0), std::get<1>(sa)};
    }
    if (k == py::str("class")) {
        TypeP r = record(t);
        if (!r) throw py::TypeError(py::str("no layout for ") + t->repr());
        return {int_of(dget(r->info, "size")), record_align(r)};
    }
    if (k == py::str("record")) return {int_of(dget(t->info, "size")), record_align(t)};
    if (k == py::str("void")) return {1, 1};
    throw py::TypeError(py::str("no size for ") + t->repr());
}

int64_t Layouts::record_align(const TypeP& r) const {
    if (r->info.contains(py::str("align"))) return int_of(r->info.at(py::str("align")));
    auto fields = r->info.get(py::str("fields"));
    int64_t best = 1;
    bool any = false;
    if (fields) {
        const Val::List* list = std::get_if<Val::List>(&fields->v);
        if (!list) throw py::TypeError("fields is not a list");
        for (const Val& f : *list) {
            int64_t a = align(type_of(dget(dict_of(f), "type")));
            best = any ? std::max(best, a) : a;
            any = true;
        }
    }
    return best;
}

std::tuple<TypeP, int64_t> Layouts::lay_out(const py::str& name, const py::list<std::tuple<py::str, TypeP>>& fields, bool union_) const {
    int64_t off = 0, big = 1;
    Val::List out;
    for (const auto& f : fields) {
        const py::str& fname = std::get<0>(f);
        const TypeP& ft = std::get<1>(f);
        auto sa = size_align(ft);
        int64_t s = std::get<0>(sa), a = std::get<1>(sa);
        big = std::max(big, a);
        Dict d;
        if (union_) {
            d.set(py::str("name"), Val(fname));
            d.set(py::str("offset"), Val(int64_t(0)));
            d.set(py::str("type"), val_of(ft));
            out.append(Val(d));
            off = std::max(off, s);
        } else {
            off = floor_div(off + a - 1, a) * a;
            d.set(py::str("name"), Val(fname));
            d.set(py::str("offset"), Val(off));
            d.set(py::str("type"), val_of(ft));
            out.append(Val(d));
            off += s;
        }
    }
    int64_t size = floor_div(off + big - 1, big) * big;
    Dict info;
    info.set(py::str("size"), Val(size));
    info.set(py::str("fields"), Val(out));
    info.set(py::str("role"), Val(union_ ? "union" : "struct"));
    info.set(py::str("packed"), Val(false));
    return {mk("record", name, py::list<TypeP>(), nullptr, info), big};
}

}  // namespace codebreaker::translate::frontends::clang::ctypes

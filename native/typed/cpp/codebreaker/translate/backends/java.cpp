// codebreaker/translate/backends/java.cpp -- typed C++ port of codebreaker.translate.backends.java
// (see java.hpp).
#include "codebreaker/translate/backends/java.hpp"

#include <algorithm>
#include <cmath>

#include "codebreaker/translate/backends/java_libc.hpp"
#include "codebreaker/translate/backends/java_objects.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::backends::java {

using ir::NodeOrList;
using ir::Nodes;
using ir::Val;
using py::str;

namespace {

str g_here("codebreaker/translate/backends");

str S(const char* s) { return str(s); }

[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

// ---- string building without a temporary str per piece
void app(std::u32string& o, const str& s) { o += s.u32(); }
void app(std::u32string& o, const char* s) {
    while (*s) o.push_back(static_cast<char32_t>(static_cast<unsigned char>(*s++)));
}
void app(std::u32string& o, int64_t v) {
    std::string t = std::to_string(v);
    for (char c : t) o.push_back(static_cast<char32_t>(c));
}
void app(std::u32string& o, int v) { app(o, static_cast<int64_t>(v)); }
void app(std::u32string& o, char32_t c) { o.push_back(c); }
template <class... A> str cat(const A&... a) {
    std::u32string o;
    (app(o, a), ...);
    return str(std::move(o));
}

// str == "ascii" without building a str
bool is(const str& a, const char* b) {
    const std::u32string& u = a.u32();
    size_t i = 0;
    for (; b[i]; ++i) {
        if (i >= u.size() || u[i] != static_cast<char32_t>(static_cast<unsigned char>(b[i]))) return false;
    }
    return i == u.size();
}
bool in(const str& a, std::initializer_list<const char*> xs) {
    for (const char* x : xs) {
        if (is(a, x)) return true;
    }
    return false;
}
bool starts(const str& a, const char* b) {
    const std::u32string& u = a.u32();
    for (size_t i = 0; b[i]; ++i) {
        if (i >= u.size() || u[i] != static_cast<char32_t>(static_cast<unsigned char>(b[i]))) return false;
    }
    return true;
}
bool opt_is(const std::optional<str>& a, const char* b) { return a && is(*a, b); }
bool opt_in(const std::optional<str>& a, std::initializer_list<const char*> xs) { return a && in(*a, xs); }
str or_none(const std::optional<str>& a) { return a ? *a : S("None"); }   // '%s' % None
bool truthy(const std::optional<str>& a) { return a && !a->empty(); }

// t.kind of a missing type
const ir::Type& K(const TypeP& t) {
    if (!t) none_attr("kind");
    return *t;
}

bool has(const ir::Strs& xs, const char* x) {   // 'x' in (xs or [])
    if (!xs) return false;
    for (const str& s : *xs) {
        if (is(s, x)) return true;
    }
    return false;
}
const py::list<NodeP>& lst(const Nodes& v) {   // iterating a slot: TypeError for None
    if (!v) throw py::TypeError("'NoneType' object is not iterable");
    return *v;
}
py::list<NodeP> lst_or_empty(const Nodes& v) { return v ? *v : py::list<NodeP>(); }   // `x or []`

// ---- Val
Val val_get(const ir::Dict& d, const char* key) { return d.contains(S(key)) ? d.at(S(key)) : Val(); }
bool val_truthy(const Val& v) {
    if (std::holds_alternative<std::monostate>(v.v)) return false;
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v);
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v) != 0;
    if (std::holds_alternative<double>(v.v)) return std::get<double>(v.v) != 0.0;
    if (std::holds_alternative<str>(v.v)) return !std::get<str>(v.v).empty();
    if (std::holds_alternative<TypeP>(v.v)) return true;
    if (std::holds_alternative<Val::List>(v.v)) return !std::get<Val::List>(v.v).empty();
    return !std::get<Val::Dict>(v.v).empty();
}
// str(v)
str val_str(const Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return py::to_str(std::get<int64_t>(v.v));
    if (std::holds_alternative<bool>(v.v)) return py::to_str(std::get<bool>(v.v));
    if (std::holds_alternative<str>(v.v)) return std::get<str>(v.v);
    return v.repr();
}
// the Python int an int-or-bool value is
std::optional<int64_t> val_int(const Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v);
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v) ? 1 : 0;
    return std::nullopt;
}
// int(v)
int64_t py_int(const Val& v) {
    if (auto i = val_int(v)) return *i;
    if (std::holds_alternative<double>(v.v)) {
        double d = std::get<double>(v.v);
        if (std::isnan(d)) throw py::ValueError("cannot convert float NaN to integer");
        if (std::isinf(d)) throw py::OverflowError("cannot convert float infinity to integer");
        return static_cast<int64_t>(d);
    }
    if (std::holds_alternative<str>(v.v)) return py::int_parse(std::get<str>(v.v), 10);
    throw py::TypeError("int() argument must be a string, a bytes-like object or a real number, not 'NoneType'");
}
TypeP val_type(const Val& v) {   // a Type, or None
    if (std::holds_alternative<TypeP>(v.v)) return std::get<TypeP>(v.v);
    return nullptr;
}
// info.get(key) as an int (None: nullopt)
std::optional<int64_t> info_int(const TypeP& t, const char* key) {
    Val v = val_get(K(t).info, key);
    if (std::holds_alternative<std::monostate>(v.v)) return std::nullopt;
    return py_int(v);
}

// an expression's / declaration's `.type`
TypeP type_of(const NodeP& e) {
    if (!e) none_attr("type");
    if (auto x = as_a<ir::Expr>(e)) return x->type;
    if (auto l = as_a<ir::Local>(e)) return l->type;
    if (auto p = as_a<ir::Param>(e)) return p->type;
    if (auto f = as_a<ir::FieldDecl>(e)) return f->type;
    if (auto p = as_a<ir::Property>(e)) return p->type;
    throw py::AttributeError("'" + e->class_name().utf8() + "' object has no attribute 'type'");
}

NodeP one(const NodeOrList& v, const char* what) {   // a body slot read as a node: a list has no such attributes
    if (std::holds_alternative<py::list<NodeP>>(v))
        throw py::AttributeError(std::string("'list' object has no attribute '") + what + "'");
    return std::get<NodeP>(v);
}
bool is_none(const NodeOrList& v) { return std::holds_alternative<NodeP>(v) && !std::get<NodeP>(v); }
py::list<NodeP> walk_of(const NodeOrList& v) {   // ir.walk(x): a list has no children()
    if (std::holds_alternative<py::list<NodeP>>(v)) throw py::AttributeError("'list' object has no attribute 'children'");
    return ir::walk(std::get<NodeP>(v));
}
py::list<NodeP> walk_node(const NodeP& n) {
    if (!n) none_attr("children");
    return ir::walk(n);
}

// the statements of a function body (`f.body.stmts`)
py::list<NodeP> body_stmts(const NodeOrList& body) {
    NodeP b = one(body, "stmts");
    if (!b) none_attr("stmts");
    auto blk = as_a<ir::Block>(b);
    if (!blk) throw py::AttributeError("'" + b->class_name().utf8() + "' object has no attribute 'stmts'");
    return lst(blk->stmts);
}

str join_sep(const char* sep, const py::list<str>& parts) { return py::join(S(sep), parts); }

// the sym of a Name / Select / Call (`x.sym.name` of a missing sym)
ir::Sym& need_sym(const SymP& s) {
    if (!s) none_attr("name");
    return *s;
}

// the compiled patterns
const py::re::Pattern& pat(const char* p, int64_t flags = 0) {
    static std::map<std::pair<std::string, int64_t>, py::re::Pattern> cache;
    auto key = std::make_pair(std::string(p), flags);
    auto it = cache.find(key);
    if (it == cache.end()) it = cache.emplace(key, py::re::compile(S(p), flags)).first;
    return it->second;
}

}  // namespace

void set_here(const str& here) { g_here = here; }

str RUNTIME() { return py::os::path::join(g_here, S("java_runtime")); }

const py::dict<str, str>& JPRIM() {
    static const py::dict<str, str> d = {
        {S("bool"), S("boolean")}, {S("i8"), S("byte")},   {S("u8"), S("byte")},   {S("char"), S("byte")},
        {S("i16"), S("short")},    {S("u16"), S("char")},  {S("i32"), S("int")},   {S("u32"), S("int")},
        {S("i64"), S("long")},     {S("u64"), S("long")},  {S("f32"), S("float")}, {S("f64"), S("double")},
        {S("f80"), S("double")}};
    return d;
}
const py::dict<str, int64_t>& _BITS() {
    static const py::dict<str, int64_t> d = {{S("bool"), 1},  {S("i8"), 8},   {S("u8"), 8},   {S("char"), 8},
                                             {S("i16"), 16},  {S("u16"), 16}, {S("i32"), 32}, {S("u32"), 32},
                                             {S("i64"), 64},  {S("u64"), 64}};
    return d;
}
const py::dict<str, int64_t>& _JBITS() {
    static const py::dict<str, int64_t> d = {{S("byte"), 8}, {S("short"), 16}, {S("char"), 16}, {S("int"), 32}, {S("long"), 64}};
    return d;
}
const py::dict<str, str>& _ACCESS() {
    static const py::dict<str, str> d = {{S("byte"), S("Byte")},   {S("short"), S("Short")}, {S("char"), S("Char")},
                                         {S("int"), S("Int")},     {S("long"), S("Long")},   {S("float"), S("Float")},
                                         {S("double"), S("Double")}, {S("boolean"), S("Bool")}};
    return d;
}
const py::set<str>& _KEYWORDS() {
    static const py::set<str> s = [] {
        py::set<str> out;
        const char* words =
            "abstract assert boolean break byte case catch char class const continue default do double "
            "else enum extends final finally float for goto if implements import instanceof int interface long native new package "
            "private protected public return short static strictfp super switch synchronized this throw throws transient try void "
            "volatile while true false null var record yield sealed permits Mem LibC Fn String Object Math System Integer Long "
            "Double Float Character Byte Short Boolean frame";
        std::string w;
        for (const char* p = words;; ++p) {
            if (*p == ' ' || *p == 0) {
                if (!w.empty()) out.add(S(w.c_str()));
                w.clear();
                if (!*p) break;
            } else {
                w += *p;
            }
        }
        return out;
    }();
    return s;
}

int64_t _BINPREC(const str& op) {
    static const py::dict<str, int64_t> d = {
        {S("*"), P_MUL},  {S("/"), P_MUL},  {S("%"), P_MUL},   {S("+"), P_ADD},    {S("-"), P_ADD},   {S("<<"), P_SHIFT},
        {S(">>"), P_SHIFT}, {S(">>>"), P_SHIFT}, {S("<"), P_REL}, {S("<="), P_REL}, {S(">"), P_REL}, {S(">="), P_REL},
        {S("=="), P_EQ},  {S("!="), P_EQ},  {S("&"), P_BAND},  {S("^"), P_XOR},    {S("|"), P_BOR},   {S("&&"), P_AND},
        {S("||"), P_OR}};
    return d.at(op);
}

// ---------------------------------------------------------------------------------------------- J
str J::at(int64_t p) const { return prec >= p ? code : cat("(", code, ")"); }
str J::repr() const { return cat("J(", py::repr(code), ", ", prec, ", ", jt, ")"); }

str jname(const std::optional<str>& name_) {
    str name = truthy(name_) ? *name_ : S("x");
    // re.sub(r'\W', '_', name): a name of ASCII letters, digits and underscores is unchanged (the regex runs otherwise)
    bool plain = true;
    for (char32_t ch : name.u32()) {
        if (!((ch >= U'a' && ch <= U'z') || (ch >= U'A' && ch <= U'Z') || (ch >= U'0' && ch <= U'9') || ch == U'_')) {
            plain = false;
            break;
        }
    }
    if (!plain) name = pat("\\W").sub(S("_"), name);
    if (_KEYWORDS().contains(name) || name.slice(0, 1).isdigit()) return name + "_";
    return name;
}

str _kind(const TypeP& t) {
    if (!t) return S("void");
    if (is(t->kind, "char")) {
        Val sg = t->info.contains(S("signed")) ? t->info.at(S("signed")) : Val(true);
        return val_truthy(sg) ? S("i8") : S("u8");
    }
    return t->kind;
}

bool is_int(const TypeP& t) {
    str k = _kind(t);
    return _BITS().contains(k) && !is(k, "bool");
}
bool is_unsigned(const TypeP& t) { return in(_kind(t), {"u8", "u16", "u32", "u64"}); }
bool is_float(const TypeP& t) { return in(_kind(t), {"f32", "f64", "f80"}); }
bool is_ptr(const TypeP& t) { return t && in(t->kind, {"ptr", "fnptr", "null"}); }
bool is_record(const TypeP& t) { return t && in(t->kind, {"class", "record"}); }

str spell(const TypeP& t) {
    if (!t) return S("?");
    const str& k = t->kind;
    if (is(k, "class")) {
        if (!t->name) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'str'");
        if (!t->args.empty()) {
            py::list<str> parts;
            for (const TypeP& a : t->args) parts.append(spell(a));
            return cat(*t->name, "<", join_sep(", ", parts), ">");
        }
        return *t->name;
    }
    if (is(k, "ptr")) return spell(t->elem) + (val_truthy(val_get(t->info, "ref")) ? "&" : "*");
    if (is(k, "array")) {
        Val c = val_get(t->info, "count");
        return cat(spell(t->elem), "[", std::holds_alternative<std::monostate>(c.v) && !t->info.contains(S("count")) ? S("") : val_str(c), "]");
    }
    if (is(k, "fnptr")) {
        if (!t->info.contains(S("fn"))) throw py::KeyError("'fn'");
        return spell(val_type(t->info.at(S("fn")))) + "*";
    }
    if (is(k, "fn")) {
        py::list<str> ps;
        for (const TypeP& p : fn_params(t)) ps.append(spell(p));
        return cat(spell(fn_result(t)), "(", join_sep(",", ps), ")");
    }
    return k;
}

NodeP strip(const NodeP& e0) {
    NodeP e = e0;
    while (auto p = as_a<ir::Paren>(e)) e = p->x;
    return e;
}

str java_string(const str& s) {
    std::u32string out;
    out.push_back(U'"');
    for (char32_t ch : s.u32()) {
        int64_t o = ch;
        if (ch == U'"') out += U"\\\"";
        else if (ch == U'\\') out += U"\\\\";
        else if (ch == U'\n') out += U"\\n";
        else if (ch == U'\t') out += U"\\t";
        else if (ch == U'\r') out += U"\\r";
        else if (32 <= o && o < 127) out.push_back(ch);
        else {
            char buf[16];
            snprintf(buf, sizeof buf, "\\%03o", static_cast<unsigned>(o & 0xFF));
            for (char* p = buf; *p; ++p) out.push_back(static_cast<char32_t>(*p));
        }
    }
    out.push_back(U'"');
    return str(std::move(out));
}

str java_wstring(const str& s) {
    std::u32string out;
    out.push_back(U'"');
    for (char32_t ch : s.u32()) {
        int64_t o = ch;
        if (ch == U'"' || ch == U'\\') {
            out.push_back(U'\\');
            out.push_back(ch);
        } else if (ch == U'\n') out += U"\\n";
        else if (ch == U'\r') out += U"\\r";
        else if (ch == U'\t') out += U"\\t";
        else if (32 <= o && o < 127) out.push_back(ch);
        else if (o < 256) {
            char buf[16];
            snprintf(buf, sizeof buf, "\\%03o", static_cast<unsigned>(o));
            for (char* p = buf; *p; ++p) out.push_back(static_cast<char32_t>(*p));
        } else {
            char buf[16];
            snprintf(buf, sizeof buf, "\\u%04x", static_cast<unsigned>(o));
            for (char* p = buf; *p; ++p) out.push_back(static_cast<char32_t>(*p));
        }
    }
    out.push_back(U'"');
    return str(std::move(out));
}

namespace {
str lit_text(const py::ref<ir::Lit>& lit) {   // lit.value, which these are only called on when it is a str
    if (!std::holds_alternative<str>(lit->value.v)) throw py::TypeError("'NoneType' object is not iterable");
    return std::get<str>(lit->value.v);
}
}  // namespace

bool is_wide(const py::ref<ir::Lit>& lit) {
    const TypeP& t = lit->type;
    return t && is(t->kind, "array") && t->elem && in(_kind(t->elem), {"u16", "i16"});
}

str string_lit(const py::ref<ir::Lit>& lit) {
    if (is_wide(lit)) return java_wstring(lit_text(lit));
    return java_string(lit_text(lit));
}

J int_lit(int64_t v, const str& jt) {
    int64_t bits = _JBITS().get(jt, 32);
    uint64_t mask = bits >= 64 ? ~uint64_t(0) : ((uint64_t(1) << bits) - 1);
    uint64_t u = static_cast<uint64_t>(v) & mask;
    if (is(jt, "char")) {
        int64_t c = static_cast<int64_t>(u);
        bool odd = c > 127 || c < 32;
        bool quote = c == '\'' || c == '\\';
        if (odd || quote) return J(cat("(char) ", c), P_UNARY, S("char"));
        if (c == 127) return J(S("'\\x7f'"), P_ATOM, S("char"));
        return J(cat("'", static_cast<char32_t>(c), "'"), P_ATOM, S("char"));
    }
    int64_t sv;
    if (bits >= 64) sv = static_cast<int64_t>(u);
    else if (u >= (uint64_t(1) << (bits - 1))) sv = static_cast<int64_t>(u) - (int64_t(1) << bits);
    else sv = static_cast<int64_t>(u);
    if (is(jt, "long")) {
        if (sv < -(int64_t(1) << 40) || sv > (int64_t(1) << 40)) {
            char buf[32];
            snprintf(buf, sizeof buf, "0x%llXL", static_cast<unsigned long long>(static_cast<uint64_t>(sv)));
            return J(S(buf), P_ATOM, S("long"));
        }
        return J(cat(sv, "L"), sv >= 0 ? P_ATOM : P_UNARY, S("long"));
    }
    if (in(jt, {"byte", "short"})) return sv != 0 ? J(cat("(", jt, ") ", sv), P_UNARY, jt) : J(S("0"), P_ATOM, S("int"));
    if (sv < -(int64_t(1) << 24) && sv != -(int64_t(1) << 31)) {
        char buf[32];
        snprintf(buf, sizeof buf, "0x%X", static_cast<unsigned>(static_cast<uint64_t>(sv) & 0xFFFFFFFFu));
        return J(S(buf), P_ATOM, S("int"));
    }
    return J(cat(sv), sv >= 0 ? P_ATOM : P_UNARY, S("int"));
}

J float_lit(double v, const str& jt) {
    if (v != v) return J(is(jt, "double") ? S("Double.NaN") : S("Float.NaN"), P_ATOM, jt);
    if (std::isinf(v)) {
        const char* w = is(jt, "double") ? "Double" : "Float";
        return J(cat(w, ".", v > 0 ? "POSITIVE" : "NEGATIVE", "_INFINITY"), P_ATOM, jt);
    }
    str text = py::repr(v);
    const std::u32string& u = text.u32();
    size_t e = u.find(U'e');
    if (e != std::u32string::npos && u.substr(0, e).find(U'.') == std::u32string::npos) text = text.replace(S("e"), S(".0e"));
    return J(text + (is(jt, "float") ? "f" : ""), (v >= 0 && !starts(text, "-")) ? P_ATOM : P_UNARY, jt);
}

bool _null(const NodeP& e0) {
    NodeP e = strip(e0);
    while (auto c = as_a<ir::Cast>(e)) e = strip(c->x);
    auto l = as_a<ir::Lit>(e);
    if (!l) return false;
    const Val& v = l->value;
    if (std::holds_alternative<std::monostate>(v.v)) return true;
    if (auto i = val_int(v)) return *i == 0;   // (0, False)
    return false;                              // (a float 0.0 is excluded; a str is not 0)
}

bool _from_alloc(const NodeP& e0) {
    NodeP e = strip(e0);
    auto c = as_a<ir::Call>(e);
    return c && !c->target && c->sym && in(c->sym->name, {"malloc", "calloc", "realloc"});
}

// ============================================================================================ analysis
const py::set<str>& Analysis::RAW() {
    static const py::set<str> s = [] {
        py::set<str> out;
        for (const char* w : {"memset", "memcpy", "memmove", "memcmp", "memchr", "qsort", "bsearch", "realloc", "vprintf", "vfprintf",
                              "vsprintf", "vsnprintf", "strchr", "strrchr", "strstr", "strpbrk", "strtol", "strtoul", "strtoll",
                              "strtoull", "strtod", "modf", "frexp", "time", "strdup", "_strdup", "fgets"})
            out.add(S(w));
        return out;
    }();
    return s;
}

namespace {
// a node's `mods` (the declarations that have them)
ir::Strs mods_of(const NodeP& m) {
    if (auto f = as_a<ir::FieldDecl>(m)) return f->mods;
    if (auto d = as_a<ir::MethodDecl>(m)) return d->mods;
    if (auto c = as_a<ir::ClassDecl>(m)) return c->mods;
    if (auto l = as_a<ir::Local>(m)) return l->mods;
    if (auto p = as_a<ir::Param>(m)) return p->mods;
    if (auto p = as_a<ir::Property>(m)) return p->mods;
    throw py::AttributeError("'" + m->class_name().utf8() + "' object has no attribute 'mods'");
}
}  // namespace

Analysis& Analysis::run() {
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        if (!u) throw py::AttributeError("'" + (un ? un->class_name().utf8() : std::string("NoneType")) + "' object has no attribute 'types'");
        for (const NodeP& t : lst_or_empty(u->types)) {
            if (auto cd = as_a<ir::ClassDecl>(t)) {
                if (opt_is(cd->kind, "union")) why.append(cat("union ", or_none(cd->name)));
                bool has_method = false;
                for (const NodeP& m : lst_or_empty(cd->members)) {
                    if (is_a<ir::MethodDecl>(m)) has_method = true;
                }
                if (opt_is(cd->kind, "class") || has_method || cd->extends) cpp_classes = true;
                if (opt_in(cd->kind, {"struct", "union"})) {
                    bool bf = false;
                    for (const NodeP& m : lst_or_empty(cd->members)) {
                        if (has(mods_of(m), "bitfield")) bf = true;
                    }
                    if (bf) why.append(S("bit-fields"));
                }
            }
            if (auto md = as_a<ir::MethodDecl>(t)) {
                bool body_set = !is_none(md->body);
                if (md->sym && md->sym->varargs && body_set) why.append(cat("a variadic function (", or_none(md->name), ")"));
            }
            exempt.clear();
            for (const NodeP& n : ir::walk(t)) {
                // free's argument and C's NULL ((void *) 0) are pointers to void that name nothing
                if (auto c = as_a<ir::Call>(n)) {
                    if (!c->target && c->sym && is(c->sym->name, "free")) {
                        for (const NodeP& a : lst_or_empty(c->args)) exempt.insert(strip(a).get());
                    }
                }
                if (auto c = as_a<ir::Cast>(n)) {
                    if (_null(c->x)) {
                        exempt.insert(n.get());
                        exempt.insert(strip(c->x).get());
                    }
                }
                if (exempt.count(n.get()) && is_a<ir::Cast>(n)) {
                    exempt.insert(strip(as_a<ir::Cast>(n)->x).get());
                    continue;
                }
                visit(n);
            }
        }
    }
    return *this;
}

void Analysis::visit(const NodeP& n) {
    if (auto un = as_a<ir::Unary>(n); un && opt_is(un->op, "&")) {
        NodeP x = strip(un->x);
        auto nm = as_a<ir::Name>(x);
        if (nm && !is_record(nm->type) && (!nm->type || !is(nm->type->kind, "array"))) {
            addressed.add(need_sym(nm->sym).name);   // (a scalar whose address is taken: boxed in the idiomatic tier)
        } else if (is_a<ir::Index>(x) || (is_a<ir::Select>(x) && !is_record(type_of(x)))) {
            why.append(S("address of an element or field"));
        }
    } else if (auto c = as_a<ir::Call>(n); c && !c->target && c->sym && RAW().contains(c->sym->name)) {
        why.append(cat("raw memory (", c->sym->name, ")"));
    } else if (auto c2 = as_a<ir::Call>(n);
               c2 && c2->target && type_of(c2->target) && in(type_of(c2->target)->kind, {"fnptr", "fn"})) {
        // (the K&R test: the target's prototype has as many parameters as the call has arguments)
        TypeP tt = type_of(c2->target);
        TypeP fnt = tt->info.contains(S("fn")) && val_truthy(tt->info.at(S("fn"))) ? val_type(tt->info.at(S("fn"))) : tt;
        int64_t np = static_cast<int64_t>(fn_params(fnt).size());
        if (np != py::len(lst_or_empty(c2->args))) why.append(S("a K&R function pointer (no prototype)"));
    } else if (auto b = as_a<ir::Binary>(n); b && opt_in(b->op, {"<", "<=", ">", ">="}) && is_ptr(type_of(b->left))) {
        why.append(S("pointer comparison"));
    } else if (auto ix = as_a<ir::Index>(n); ix && ix->arr && type_of(ix->arr) && is(type_of(ix->arr)->kind, "ptr") &&
                                              type_of(ix->arr)->elem && is(type_of(ix->arr)->elem->kind, "void")) {
        why.append(S("a void pointer"));
    } else if (auto b2 = as_a<ir::Binary>(n); b2 && opt_in(b2->op, {"+", "-"}) && (is_ptr(type_of(b2->left)) || is_ptr(type_of(b2->right)))) {
        why.append(S("pointer arithmetic"));
    } else if (auto u2 = as_a<ir::Unary>(n); u2 && opt_in(u2->op, {"++", "--"}) && is_ptr(u2->type)) {
        why.append(S("pointer arithmetic"));
    } else if (auto as = as_a<ir::Assign>(n); as && opt_in(as->op, {"+", "-"}) && is_ptr(type_of(as->target))) {
        why.append(S("pointer arithmetic"));
    } else if (auto ca = as_a<ir::Cast>(n); ca && is_ptr(ca->type) && type_of(ca->x) && is_ptr(type_of(ca->x))) {
        const TypeP& a = ca->type;
        TypeP b = type_of(ca->x);
        if (is(a->kind, "ptr") && is(b->kind, "ptr") && a->elem && b->elem && _kind(a->elem) != _kind(b->elem) &&
            !(is(a->elem->kind, "void") || is(b->elem->kind, "void")) && !(ca->implicit && *ca->implicit)) {
            why.append(S("a cast between pointer types"));
        } else if (is(a->kind, "ptr") && is(b->kind, "ptr") && (is(K(a->elem).kind, "void")) != (is(K(b->elem).kind, "void"))) {
            if (!_from_alloc(ca->x)) why.append(S("a void pointer"));
        }
    } else if (auto ca2 = as_a<ir::Cast>(n); ca2 && is_ptr(ca2->type) && type_of(ca2->x) && is_int(type_of(ca2->x))) {
        why.append(S("an integer used as a pointer"));
    } else if (auto ca3 = as_a<ir::Cast>(n); ca3 && is_int(ca3->type) && is_ptr(type_of(ca3->x))) {
        why.append(S("a pointer used as an integer"));
    }
}

// ---------------------------------------------------------------------------------------------- the layout of a record Type
std::vector<LayoutField> layout_fields(const TypeP& lay) {
    std::vector<LayoutField> out;
    Val fv = val_get(K(lay).info, "fields");
    if (!std::holds_alternative<Val::List>(fv.v)) throw py::KeyError("'fields'");
    for (const Val& f : std::get<Val::List>(fv.v)) {
        const Val::Dict& d = std::get<Val::Dict>(f.v);
        LayoutField lf;
        lf.name = d.contains(S("name")) ? val_str(d.at(S("name"))) : S("None");
        lf.offset = d.contains(S("offset")) ? py_int(d.at(S("offset"))) : 0;
        lf.type = d.contains(S("type")) ? val_type(d.at(S("type"))) : nullptr;
        out.push_back(lf);
    }
    return out;
}

int64_t layout_size(const TypeP& lay) {
    Val v = val_get(K(lay).info, "size");
    if (!lay->info.contains(S("size"))) throw py::KeyError("'size'");
    return py_int(v);
}

std::vector<TypeP> fn_params(const TypeP& fn) {
    std::vector<TypeP> out;
    if (!fn) none_attr("info");
    if (!fn->info.contains(S("params"))) throw py::KeyError("'params'");
    Val pv = fn->info.at(S("params"));
    if (!std::holds_alternative<Val::List>(pv.v)) throw py::TypeError("'NoneType' object is not iterable");
    for (const Val& p : std::get<Val::List>(pv.v)) {
        const Val::Dict& d = std::get<Val::Dict>(p.v);
        if (!d.contains(S("type"))) throw py::KeyError("'type'");
        out.push_back(val_type(d.at(S("type"))));
    }
    return out;
}

TypeP fn_result(const TypeP& fn) {
    if (!fn) none_attr("info");
    if (!fn->info.contains(S("result"))) throw py::KeyError("'result'");
    Val rv = fn->info.at(S("result"));
    if (!std::holds_alternative<Val::Dict>(rv.v)) throw py::AttributeError("object has no attribute 'get'");
    const Val::Dict& d = std::get<Val::Dict>(rv.v);
    return d.contains(S("type")) ? val_type(d.at(S("type"))) : nullptr;
}

// ============================================================================================ program
namespace {
py::list<str> sorted_unique(const py::list<str>& xs) {   // sorted(set(xs))
    std::vector<str> v(xs.vec().begin(), xs.vec().end());
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return py::list<str>(v);
}
}  // namespace

Program program(const py::list<NodeP>& units, const str& name, const str& tier) {
    auto a = std::make_shared<Analysis>(units);
    a->run();
    py::list<str> why = sorted_unique(a->why);
    if (is(tier, "faithful") && a->cpp_classes) throw Refused("C++ classes have no faithful (emulated memory) form yet");
    std::optional<str> declined;
    if (is(tier, "idiomatic") || (is(tier, "auto") && (why.empty() || a->cpp_classes))) {
        if (!why.empty()) throw Refused(S("C++ code the object model cannot carry: ") + join_sep(", ", why));
        try {
            auto w = java_objects::make_objects(units, name, a);
            str src = w->write();
            if (w->gaps.empty() || a->cpp_classes || is(tier, "idiomatic")) return Program(name, src, S("idiomatic"), w->notes);
            declined = S("gaps in the idiomatic tier: ") + join_sep("; ", w->gaps);
        } catch (const Refused& e) {
            if (a->cpp_classes || is(tier, "idiomatic")) throw;
            declined = str(e.what());
        }
    }
    a = std::make_shared<Analysis>(units);       // (afresh: the idiomatic writer may have changed its facts)
    a->run();
    auto w = make_faithful(units, name, a);
    str src = w->write();
    py::list<str> notes = w->notes;
    py::list<str> sorted_why = sorted_unique(a->why);
    notes.append(S("faithful tier: ") + (declined ? S("the idiomatic tier declined (") + *declined + ")"
                                                  : (sorted_why.empty() ? S("chosen") : join_sep(", ", sorted_why))));
    return Program(name, src, S("faithful"), notes);
}

// ============================================================================================ faithful
namespace {
str first_unsupported(const NodeP& n) {
    auto u = ir::unsupported(n);
    if (u.empty()) return S("?");
    return cat(or_none(u[0]->kind), " (line ", u[0]->line, ")");
}

str ctype_text(const TypeP& t, const str& name) {
    const str& k = t->kind;
    if (is(k, "array")) {
        Val c = val_get(t->info, "count");
        return ctype_text(t->elem, cat(name, "[", t->info.contains(S("count")) ? val_str(c) : S(""), "]"));
    }
    if (is(k, "ptr")) return ctype_text(t->elem, S("*") + name);
    if (is(k, "class")) return cat("struct ", or_none(t->name), " ", name);
    if (is(k, "char")) return S("char ") + name;
    static const py::dict<str, str> m = {{S("i8"), S("signed char")},     {S("u8"), S("unsigned char")}, {S("i16"), S("short")},
                                         {S("u16"), S("unsigned short")}, {S("i32"), S("int")},          {S("u32"), S("unsigned")},
                                         {S("i64"), S("long long")},      {S("u64"), S("unsigned long long")}, {S("f32"), S("float")},
                                         {S("f64"), S("double")},         {S("bool"), S("_Bool")},       {S("fnptr"), S("function pointer")}};
    return cat(m.get(k, k), " ", name);
}
}  // namespace

str Faithful::key(const SymP& s) {
    if (in(s->kind, {"global", "function"})) return cat("g\x01", s->name);
    return cat("l\x01", s->owner ? S("s") + *s->owner : S("\x02"), "\x01", s->name);
}

namespace {
str key_g(const str& name) { return cat("g\x01", name); }
str key_l(const SymP& s) { return cat("l\x01", s->owner ? S("s") + *s->owner : S("\x02"), "\x01", s->name); }
}  // namespace

Faithful::Faithful(const py::list<NodeP>& units, const str& name, const std::shared_ptr<Analysis>& analysis)
    : units(units), name(name), a(analysis) {
    std::optional<ir::Dict> tgt;
    if (!units.empty()) {
        auto u0 = as_a<ir::Unit>(units[0]);
        if (!u0) throw py::AttributeError("object has no attribute 'target'");
        tgt = u0->target;
    }
    {
        Val p = tgt ? val_get(*tgt, "pointer") : Val();
        ptr_size = std::holds_alternative<std::monostate>(p.v) ? 8 : py_int(p);
    }
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        if (!u) throw py::AttributeError("object has no attribute 'types'");
        for (const NodeP& t : lst_or_empty(u->types)) {
            if (auto c = as_a<ir::ClassDecl>(t)) {
                records.set(or_none(c->qname), c);
            } else if (auto m = as_a<ir::MethodDecl>(t)) {
                if (m->mods && !m->mods->empty() && has(m->mods, "unsupported"))
                    throw Refused(S("a declaration the front end did not lower: ") + first_unsupported(t));
                if (!is_none(m->body) && m->sym) {
                    str mn = or_none(m->name);
                    if (functions.contains(mn)) {
                        Val lang = u->target ? val_get(*u->target, "language") : Val();
                        bool is_cpp = std::holds_alternative<str>(lang.v) && is(std::get<str>(lang.v), "c++");
                        if (!is_cpp) throw Refused(S("two definitions of ") + mn);
                        // C++ overloads (a function template's instances among them): Java overloads
                        if (!overloads.contains(mn)) overloads.set(mn, py::list<MethodP>{functions.at(mn)});
                        overloads.at(mn).append(m);
                        continue;
                    }
                    functions.set(mn, m);
                }
            } else if (auto f = as_a<ir::FieldDecl>(t)) {
                global_decls.append(f);
            }
        }
    }
}

void Faithful::init() {
    _check_records();
    _names();
    _addressed();
}

std::shared_ptr<Faithful> make_faithful(const py::list<NodeP>& units, const str& name, const std::shared_ptr<Analysis>& a) {
    auto w = std::make_shared<Faithful>(units, name, a);
    w->init();
    return w;
}

void Faithful::_check_records() {
    for (const auto& kv : records.items()) {
        const ClassP& t = std::get<1>(kv);
        if (!opt_in(t->kind, {"struct", "union"})) throw Refused(cat(or_none(t->kind), " ", or_none(t->name)));
        if (!t->layout) throw Refused(cat("struct ", or_none(t->name), " has no layout (bit-fields?)"));
    }
}

void Faithful::_names() {
    py::set<str> taken;
    for (const auto& kv : functions.items()) taken.add(jname(std::get<0>(kv)));
    for (const FieldP& f : global_decls) taken.add(jname(need_sym(f->sym).name));
    taken.add(name);
    for (const auto& kv : records.items()) {
        const ClassP& c = std::get<1>(kv);
        if (!c->name) none_attr("replace");
        str base = jname(c->name->replace(S("__unnamed_"), S("Anon")));
        base = base.slice(0, 1).upper() + base.slice_from(1);
        str n = base;
        while (taken.contains(n)) n = n + "_t";
        taken.add(n);
        layout_names.set(std::get<0>(kv), n);
    }
    used_names = taken;
}

void Faithful::_addressed() {
    // Every variable whose address is taken (by & or by decay of an array): it lives in memory.
    addr_taken = py::set<str>();
    for (const NodeP& u : units) {
        for (const NodeP& n : ir::walk(u)) {
            if (auto un = as_a<ir::Unary>(n); un && opt_is(un->op, "&")) {
                NodeP x = strip(un->x);
                if (auto nm = as_a<ir::Name>(x)) addr_taken.add(key(nm->sym));
            }
        }
    }
}

py::list<MethodP> Faithful::all_functions() {
    py::list<MethodP> out;
    for (const auto& kv : functions.items()) {
        if (overloads.contains(std::get<0>(kv))) out.extend(overloads.at(std::get<0>(kv)));
        else out.append(std::get<1>(kv));
    }
    return out;
}

MethodP Faithful::function_for(const SymP& s) {
    // The definition a call's symbol names (by its parameter types, where C++ overloads the name).
    if (!functions.contains(s->name)) return nullptr;
    if (!overloads.contains(s->name) || overloads.at(s->name).empty()) return functions.at(s->name);
    py::list<MethodP> cands = overloads.at(s->name);
    std::vector<str> want;
    for (const TypeP& p : s->params) want.push_back(spell(p));
    for (const MethodP& f : cands) {
        std::vector<str> have;
        for (const NodeP& p : lst(f->params)) have.push_back(spell(as_a<ir::Param>(p)->type));
        if (have == want) return f;
    }
    for (const MethodP& f : cands) {
        if (py::len(lst(f->params)) == static_cast<int64_t>(want.size())) return f;
    }
    return cands[0];
}

// ------------------------------------------------------------------------------------- types
str Faithful::jt(const TypeP& t) {
    str k = _kind(t);
    if (JPRIM().contains(k)) return JPRIM().at(k);
    if (is(k, "void")) return S("void");
    if (in(k, {"ptr", "fnptr", "array", "class", "record", "null"})) return S("int");
    throw Refused(S("type ") + (t ? t->repr() : S("None")));
}

TypeP Faithful::layout(const TypeP& t) {
    str rn = rec_name(t);
    if (!records.contains(rn)) throw Refused(cat("struct ", spell(t), ": a library type the C runtime for Java does not have"));
    return records.at(rn)->layout;
}

str Faithful::rec_name(const TypeP& t) {
    if (is(K(t).kind, "class")) return spell(t);
    return or_none(t->name);
}

int64_t Faithful::size(const TypeP& t) {
    str k = _kind(t);
    if (_BITS().contains(k)) return is(k, "bool") ? 1 : _BITS().at(k) / 8;
    if (is(k, "f32")) return 4;
    if (in(k, {"f64", "f80"})) return 8;
    if (in(k, {"ptr", "fnptr", "null"})) return ptr_size;
    if (is(k, "array")) {
        if (!t->info.contains(S("count"))) throw Refused("an array of unknown size");
        return size(t->elem) * py_int(t->info.at(S("count")));
    }
    if (in(k, {"class", "record"})) return layout_size(layout(t));
    if (is(k, "void")) return 1;
    throw Refused(S("size of ") + (t ? t->repr() : S("None")));
}

int64_t Faithful::align(const TypeP& t) {
    str k = _kind(t);
    if (is(k, "array")) return align(t->elem);
    if (in(k, {"class", "record"})) {
        TypeP lay = layout(t);
        int64_t best = 1;
        for (const LayoutField& f : layout_fields(lay)) best = std::max(best, align(f.type));
        return best;
    }
    return std::min<int64_t>(size(t), 8);
}

str Faithful::access(const TypeP& t) {
    str k = _kind(t);
    if (in(k, {"ptr", "fnptr", "null"})) return S("Ptr");
    return _ACCESS().at(JPRIM().at(k));
}

str Faithful::size_code(const TypeP& t) {
    if (is_record(t)) return cat(layout_names.at(rec_name(t)), ".SIZE");
    return cat(size(t));
}

LayoutField Faithful::field_off(const TypeP& rec_t, const str& fname) {
    TypeP lay = layout(rec_t);
    for (const LayoutField& f : layout_fields(lay)) {
        if (f.name == fname) return f;
    }
    throw Refused(cat("no field ", fname, " in ", rec_t ? rec_t->repr() : S("None")));
}

// ------------------------------------------------------------------------------------- the file
str Faithful::write() {
    fields_out = Lines();
    static_init = Lines();
    for (const FieldP& g : global_decls) declare_global(g);
    py::list<Lines> funcs;
    for (const MethodP& f : all_functions()) {
        funcs.append(function_or_gap(f, [&]() { return FnWriter(this, f).write(); }));
    }
    for (const FieldP& f : global_decls) init_global(f);
    Lines out;
    out.append(cat("// ", name, ".java -- translated from C by CodeBreaker (faithful tier: C memory emulated in crt.Mem)."));
    out.append(S("import crt.*;"));
    out.append(S(""));
    out.append(S("import static crt.LibC.*;"));
    out.append(S(""));
    out.append(cat("public final class ", name, " {"));
    {
        std::vector<std::pair<std::pair<int64_t, str>, str>> order;
        for (const auto& kv : records.items()) order.push_back({{std::get<1>(kv)->line, std::get<0>(kv)}, std::get<0>(kv)});
        std::sort(order.begin(), order.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& o : order) out.extend(layout_class(records.at(o.second)));
    }
    for (const str& fn : fn_consts.keys()) iface(fn_sig(functions.at(fn)->sym));   // (each registered function's interface, before they are written)
    for (size_t i = 0; i < fn_ifaces.size(); ++i) {
        Sig sig = fn_ifaces[i].first;
        str nm = fn_ifaces[i].second;
        py::list<str> ps;
        for (size_t k = 0; k < sig.params.size(); ++k) ps.append(cat(sig.params[k], " a", static_cast<int64_t>(k)));
        out.append(S(""));
        out.append(cat("    interface ", nm, " { ", sig.ret, " call(", join_sep(", ", ps), "); }"));
    }
    if (!fields_out.empty() || !static_init.empty()) out.append(S(""));
    for (const str& f : fields_out) out.append(S("    ") + f);
    for (const auto& kv : fn_consts.items()) {
        Sig sig = fn_sig(functions.at(std::get<0>(kv))->sym);
        out.append(cat("    static final int ", std::get<1>(kv), " = Fn.register((", iface(sig), ") ", name, "::", jname(std::get<0>(kv)), ");"));
    }
    if (!static_init.empty()) {
        out.append(S(""));
        out.append(S("    static {"));
        for (const str& s : static_init) out.append(S("        ") + s);
        out.append(S("    }"));
    }
    for (const Lines& f : funcs) {
        out.append(S(""));
        out.extend(f);
    }
    out.extend(entry());
    out.append(S("}"));
    for (const str& g : gaps) notes.append(S("gap: ") + g);
    return py::join(S("\n"), out) + "\n";
}

Lines Faithful::function_or_gap(const MethodP& f, const std::function<Lines()>& write, const str& indent, const std::optional<str>& owner) {
    // A function written, or -- where the back end declines it -- the explicit gap the design asks for: its
    // signature, and a body that throws with the reason (the Java lane's J2X-GAP), listed in notes.
    try {
        return write();
    } catch (const Refused& e) {
        str reason(e.what());
        if (opt_is(f->name, "main") && !owner) throw;       // (a program whose main is a gap is refused)
        std::optional<str> sig = gap_signature(f, owner);
        if (!sig) throw;
        str what = (owner ? *owner + "::" : S("")) + or_none(f->name);
        gaps.append(what + " (" + reason + ")");
        str msg = java_string(cat("CodeBreaker gap: ", what, ": ", reason));
        Lines out;
        out.append(indent + *sig + " {");
        out.append(indent + "    // " + reason.replace(S("\n"), S(" ")));
        out.append(indent + "    throw new UnsupportedOperationException(" + msg + ");");
        out.append(indent + "}");
        return out;
    }
}

std::optional<str> Faithful::gap_signature(const MethodP& f, const std::optional<str>& owner) {
    Sig sig;
    try {
        sig = fn_sig(f->sym);
    } catch (const Refused&) {
        return std::nullopt;
    }
    std::vector<str> names;
    if (sig.params.size() > static_cast<size_t>(py::len(lst(f->params)))) names.push_back(S("result"));
    for (const NodeP& p : lst(f->params)) names.push_back(jname(need_sym(as_a<ir::Param>(p)->sym).name));
    py::list<str> ps;
    for (size_t i = 0; i < std::min(sig.params.size(), names.size()); ++i) ps.append(sig.params[i] + " " + names[i]);
    str pstr = join_sep(", ", ps);
    if (f->sym->varargs) pstr = pstr + (pstr.empty() ? "" : ", ") + "Object... va";
    return cat("static ", sig.ret, " ", jname(f->name), "(", pstr, ")");
}

Lines Faithful::layout_class(const ClassP& c) {
    TypeP lay = c->layout;
    str n = layout_names.at(or_none(c->qname));
    const char* what = opt_is(c->kind, "union") ? "union" : "struct";
    py::list<str> parts;
    parts.append(cat("SIZE = ", layout_size(lay)));
    for (const LayoutField& f : layout_fields(lay)) {
        str fname = !is(f.name, "SIZE") ? jname(f.name) : S("SIZE_");
        parts.append(cat(fname, " = ", f.offset));
    }
    if (!c->name) none_attr("replace");
    Lines out;
    out.append(S(""));
    out.append(cat("    /** ", what, " ", c->name->replace(S("__unnamed_"), S("(anonymous) ")),
                   ": its size and its fields' offsets in memory. */"));
    out.append(cat("    static final class ", n, " {"));
    out.append(S("        static final int ") + join_sep(", ", parts) + ";");
    out.append(S("    }"));
    return out;
}

Lines Faithful::entry() {
    if (!functions.contains(S("main"))) throw Refused("no main function");
    MethodP m = functions.at(S("main"));
    Lines out;
    out.append(S(""));
    out.append(S("    public static void main(String[] args) {"));
    str call = S("main()");
    if (py::len(lst(m->params)) >= 2) call = cat("main(args.length + 1, Mem.argv(System.getProperty(\"c.argv0\", \"", name, "\"), args))");
    if (!m->ret || is(m->ret->kind, "void")) {
        out.append(cat("        ", call, ";"));
        out.append(S("        exit(0);"));
    } else {
        out.append(cat("        exit(", call, ");"));
    }
    out.append(S("    }"));
    return out;
}

// ------------------------------------------------------------------------------------- globals
void Faithful::declare_global(const FieldP& g) {
    SymP s = g->sym;
    str nm = jname(need_sym(s).name);
    bool has_enum = false;
    if (g->mods) {
        for (const str& m : *g->mods) {
            if (m.startswith(S("enum:"))) has_enum = true;
        }
    }
    if (has(g->mods, "const") && has_enum) {
        globals.set(s->name, Var(nm, false, ir::I32, S("enum")));
        auto lit = as_a<ir::Lit>(g->init);
        if (!lit) none_attr("value");
        fields_out.append(cat("static final int ", nm, " = ", val_str(lit->value), ";"));
        return;
    }
    TypeP t = g->type;
    bool mem = in(K(t).kind, {"array", "class", "record"}) || addr_taken.contains(key_g(s->name));
    Var v(nm, mem, t, S("global"));
    if (globals.contains(s->name) && !is(globals.at(s->name).kind, "enum")) return;   // (a tentative definition again)
    globals.set(s->name, v);
    if (mem) {
        str sc = size_code(t);
        int64_t al = align(t);
        fields_out.append(cat("static final int ", nm, " = Mem.global(", sc, ", ", al, ");  // ", ctype_text(t, s->name)));
    } else {
        str jtt = jt(t);
        NodeP init = g->init;
        std::optional<str> cv = init ? const_value(init, t) : std::nullopt;
        if (!init) fields_out.append(cat("static ", jtt, " ", nm, ";"));
        else if (cv) fields_out.append(cat("static ", jtt, " ", nm, " = ", *cv, ";"));
        else fields_out.append(cat("static ", jtt, " ", nm, ";"));
    }
}

std::optional<str> Faithful::const_value(const NodeP& e0, const TypeP& t) {
    NodeP e = strip(e0);
    if (auto c = as_a<ir::Cast>(e); c && c->implicit && *c->implicit) {
        NodeP inner = strip(c->x);
        if (auto l = as_a<ir::Lit>(inner)) {
            const Val& v = l->value;
            if (std::holds_alternative<int64_t>(v.v) || std::holds_alternative<double>(v.v)) {
                auto nl = std::make_shared<ir::Lit>();
                nl->value = v;
                nl->type = t;
                e = nl;
            }
        }
    }
    auto l = as_a<ir::Lit>(e);
    if (!l) return std::nullopt;
    const Val& v = l->value;
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v) ? S("true") : S("false");
    if (std::holds_alternative<int64_t>(v.v) && (is_int(t) || is_float(t))) {
        if (is_float(t)) return float_lit(static_cast<double>(std::get<int64_t>(v.v)), jt(t)).code;
        return int_lit(std::get<int64_t>(v.v), jt(t)).code;
    }
    if (std::holds_alternative<double>(v.v) && is_float(t)) return float_lit(std::get<double>(v.v), jt(t)).code;
    if (std::holds_alternative<std::monostate>(v.v) && is_ptr(t)) return S("0");
    return std::nullopt;
}

void Faithful::init_global(const FieldP& g) {
    if (!globals.contains(need_sym(g->sym).name)) return;
    Var v = globals.at(g->sym->name);
    if (is(v.kind, "enum") || !g->init) return;
    if (!v.mem && const_value(g->init, g->type)) return;
    FnWriter w(this, nullptr);
    if (v.mem) {
        static_init.extend(w.init_place(v.java, g->type, g->init));
    } else {
        static_init.append(cat(v.java, " = ", w.conv(w.value(g->init), type_of(g->init), g->type).code, ";"));
    }
}

// ------------------------------------------------------------------------------------- functions
Sig Faithful::fn_sig(const SymP& s) {
    // (java param types, java return type) of a function symbol (a struct result: an int address)
    Sig sig;
    for (const TypeP& p : s->params) sig.params.push_back(jt(p));
    sig.ret = s->ret ? jt(s->ret) : S("void");
    if (s->ret && is_record(s->ret)) sig.params.insert(sig.params.begin(), S("int"));
    return sig;
}

Sig Faithful::fn_sig_of_type(const TypeP& ft) {
    TypeP f = is(K(ft).kind, "fnptr") ? val_type(ft->info.at(S("fn"))) : ft;
    Sig sig;
    for (const TypeP& p : fn_params(f)) sig.params.push_back(jt(p));
    TypeP rt = fn_result(f);
    sig.ret = rt ? jt(rt) : S("void");
    if (rt && is_record(rt)) sig.params.insert(sig.params.begin(), S("int"));
    return sig;
}

str Faithful::iface(const Sig& sig) {
    for (const auto& kv : fn_ifaces) {
        if (kv.first == sig) return kv.second;
    }
    auto cap = [](const str& p) { return p.slice(0, 1).upper() + p.slice_from(1); };
    str n;
    if (!sig.params.empty()) {
        for (const str& p : sig.params) n = n + cap(p);
        n = n + "To" + cap(sig.ret);
    } else {
        n = S("To") + cap(sig.ret);
    }
    fn_ifaces.push_back({sig, n});
    return n;
}

str Faithful::fn_const(const str& nm) {
    if (!functions.contains(nm)) throw Refused(S("the address of library function ") + nm);
    if (!fn_consts.contains(nm)) fn_consts.set(nm, S("FN_") + jname(nm));
    return fn_consts.at(nm);
}

// ============================================================================================ functions
bool _is_int_text(const str& code) {
    // re.fullmatch(r'\(?-?\d+\)?', code); for ASCII text read directly (the regex runs for any other)
    const std::u32string& u = code.u32();
    for (char32_t ch : u) {
        if (ch >= 128) {
            static const py::re::Pattern p = py::re::compile(S("\\(?-?\\d+\\)?"));
            return p.fullmatch(code).has_value();
        }
    }
    size_t i = 0, n = u.size();
    if (i < n && u[i] == U'(') ++i;
    if (i < n && u[i] == U'-') ++i;
    size_t d = i;
    while (i < n && u[i] >= U'0' && u[i] <= U'9') ++i;
    if (i == d) return false;
    if (i < n && u[i] == U')') ++i;
    return i == n;
}

bool _balanced_call(const str& text) {
    int64_t depth = 0;
    const std::u32string& u = text.u32();
    for (size_t i = 0; i < u.size(); ++i) {
        char32_t ch = u[i];
        depth += ch == U'(';
        depth -= ch == U')';
        if (depth == 0 && ch == U')' && i != u.size() - 1) return false;
    }
    return true;
}

str _promoted(const str& jt) { return in(jt, {"byte", "short", "char"}) ? S("int") : jt; }

str _promoted2(const str& a0, const str& b0) {
    static const char* order[] = {"int", "long", "float", "double"};
    str a = _promoted(a0), b = _promoted(b0);
    int ia = -1, ib = -1;
    for (int i = 0; i < 4; ++i) {
        if (is(a, order[i])) ia = i;
        if (is(b, order[i])) ib = i;
    }
    if (ia >= 0 && ib >= 0) return S(order[std::max(ia, ib)]);
    return a;
}

TypeP _promote_type(const TypeP& t) {
    str k = _kind(t);
    if (in(k, {"i8", "u8", "i16", "u16", "bool"})) return ir::I32;
    return t;
}

FnWriter::FnWriter(Faithful* prog, const MethodP& fn) : p(prog), fn(fn) {
    ret_struct = fn && fn->ret && is_record(fn->ret);
}

// ------------------------------------------------------------------------------------- frame
str FnWriter::alloc(const TypeP& t, std::optional<int64_t> size_, std::optional<int64_t> align_) {
    int64_t sz = size_ ? *size_ : p->size(t);
    int64_t al = align_ ? *align_ : p->align(t);
    if (al == 0) throw py::ZeroDivisionError("integer division or modulo by zero");
    frame = (frame + al - 1) / al * al;
    int64_t off = frame;
    frame += sz;
    has_frame = true;
    return off == 0 ? S("frame") : cat("frame + ", off);
}

str FnWriter::temp(const TypeP& t) { return alloc(t); }

str FnWriter::fresh(const str& base) {
    tmp += 1;
    str n = cat(base, tmp);
    while (declared.contains(n) || p->used_names.contains(n)) {
        tmp += 1;
        n = cat(base, tmp);
    }
    declared.add(n);
    return n;
}

// ------------------------------------------------------------------------------------- the function
Lines FnWriter::write() {
    MethodP f = fn;
    SymP s = f->sym;
    Sig sp = p->fn_sig(s);
    str ret = sp.ret;
    py::list<str> params;
    std::vector<std::pair<str, str>> pre;
    if (ret_struct) {
        params.append(S("int result"));
        declared.add(S("result"));
    }
    for (const NodeP& pn : lst(f->params)) {
        auto prm = as_a<ir::Param>(pn);
        str name = jname(need_sym(prm->sym).name);
        declared.add(name);
        TypeP t = prm->type;
        str key = key_l(prm->sym);
        if (is_record(t)) {
            // a struct passed by value arrives as the address of the caller's copy
            vars.set(prm->sym->name, Var(name, true, t, S("param")));
            params.append(S("int ") + name);
        } else if (p->addr_taken.contains(key)) {
            str arg = name + "Arg";
            declared.add(arg);
            params.append(cat(p->jt(t), " ", arg));
            vars.set(prm->sym->name, Var(name, true, t, S("param")));
            str a1 = cat("int ", name, " = ", alloc(t), ";");
            str a2 = cat("Mem.set", p->access(t), "(", name, ", ", arg, ");");
            pre.push_back({a1, a2});
        } else {
            vars.set(prm->sym->name, Var(name, false, t, S("param")));
            params.append(cat(p->jt(t), " ", name));
        }
    }
    Lines body;
    if (s->varargs) {
        // the arguments past the last named one: 8-byte slots in memory, as MSVC's va_list reads them
        params.append(S("Object... va"));
        body.append(S("int vaList = Mem.varargs(va);"));
        declared.add(S("va"));
        declared.add(S("vaList"));
        has_frame = true;
    }
    for (const auto& dp : pre) {
        body.append(dp.first);
        body.append(dp.second);
    }
    block_stmts(body_stmts(f->body), body, 1);
    if (!is(ret, "void") && can_complete(f->body)) {
        if (opt_is(f->name, "main")) body.append(S("return 0;"));
        else body.append(cat("return ", !is(ret, "boolean") ? "0" : "false", ";  // (C: the function ends without a value)"));
    }
    str head = cat("    static ", ret, " ", jname(f->name), "(", join_sep(", ", params), ") {");
    Lines out;
    if (has_frame) {
        int64_t size = (frame + 15) / 16 * 16;
        out.append(head);
        out.append(cat("        int frame = Mem.enter(", size, ");"));
        out.append(S("        try {"));
        for (const str& x : body) out.append(!x.empty() ? S("            ") + x : x);
        out.append(S("        } finally {"));
        out.append(S("            Mem.leave(frame);"));
        out.append(S("        }"));
        out.append(S("    }"));
    } else {
        out.append(head);
        for (const str& x : body) out.append(!x.empty() ? S("        ") + x : x);
        out.append(S("    }"));
    }
    return out;
}

// ------------------------------------------------------------------------------------- statements
py::list<NodeP> FnWriter::structure(const py::list<NodeP>& stmts_in) {
    // C's goto, where it nests, as Java's labeled break and continue: the statements from the first
    // goto to a later label become `L: { ... }` (goto L is break L); the statements from a label to the
    // last goto back to it become `L: while (true) { ...; break; }` (goto L is continue L).
    py::list<NodeP> stmts = stmts_in.copy();
    bool any_label = false;
    for (const NodeP& s : stmts) {
        if (is_a<ir::Labeled>(s)) any_label = true;
    }
    if (!any_label) return stmts;
    while (true) {
        std::vector<int64_t> labs;
        for (int64_t i = 0; i < stmts.size(); ++i) {
            if (is_a<ir::Labeled>(stmts[i]) && !labels_done.count(stmts[i].get())) labs.push_back(i);
        }
        if (labs.empty()) return stmts;
        int64_t j = labs.back();
        auto lab = as_a<ir::Labeled>(stmts[j]);
        labels_done.insert(lab.get());
        std::optional<str> name = lab->label;
        std::vector<int64_t> where;
        for (int64_t i = 0; i < stmts.size(); ++i) {
            bool found = false;
            for (const NodeP& n : walk_node(stmts[i])) {
                if (auto g = as_a<ir::Goto>(n); g && g->label == name && !goto_map.count(n.get())) {
                    found = true;
                    break;
                }
            }
            if (found) where.push_back(i);
        }
        std::vector<int64_t> fwd, back;
        for (int64_t i : where) (i < j ? fwd : back).push_back(i);
        str jl = jname(name);
        if (!fwd.empty() && !back.empty())
            throw Refused(cat("goto ", or_none(name), " both forward and backward in ", fn ? or_none(fn->name) : S("None")));
        if (!fwd.empty()) {
            int64_t a = *std::min_element(fwd.begin(), fwd.end());
            for (int64_t i : fwd) {
                for (const NodeP& n : walk_node(stmts[i])) {
                    if (auto g = as_a<ir::Goto>(n); g && g->label == name) goto_map[n.get()] = S("break ") + jl;
                }
            }
            auto blk = std::make_shared<ir::Block>();
            blk->stmts = stmts.slice(a, j);
            auto block = std::make_shared<ir::Labeled>();
            block->label = name;
            block->body = NodeP(blk);
            labels_done.insert(block.get());
            py::list<NodeP> mid{NodeP(block), one(lab->body, "stmts")};
            stmts = stmts.slice(0, a) + mid + stmts.slice_from(j + 1);
        } else if (!back.empty()) {
            int64_t b = *std::max_element(back.begin(), back.end());
            py::list<NodeP> region{one(lab->body, "stmts")};
            region = region + stmts.slice(j + 1, b + 1);
            for (const NodeP& s : region) {
                if (loose_jump(s)) {
                    throw Refused(cat("a break or continue inside the loop goto ", or_none(name), " makes in ", fn ? or_none(fn->name) : S("None")));
                }
            }
            for (int64_t i : back) {
                for (const NodeP& n : walk_node(stmts[i])) {
                    if (auto g = as_a<ir::Goto>(n); g && g->label == name) goto_map[n.get()] = S("continue ") + jl;
                }
            }
            auto cond = std::make_shared<ir::Lit>();
            cond->value = Val(int64_t(1));
            cond->type = ir::I32;
            auto brk = std::make_shared<ir::Break>();
            auto wbody = std::make_shared<ir::Block>();
            wbody->stmts = region + py::list<NodeP>{NodeP(brk)};
            auto wh = std::make_shared<ir::While>();
            wh->cond = cond;
            wh->body = NodeP(wbody);
            auto loop = std::make_shared<ir::Labeled>();
            loop->label = name;
            loop->body = NodeP(wh);
            labels_done.insert(loop.get());
            py::list<NodeP> mid{NodeP(loop)};
            stmts = stmts.slice(0, j) + mid + stmts.slice_from(b + 1);
        } else {
            py::list<NodeP> mid{one(lab->body, "stmts")};
            stmts = stmts.slice(0, j) + mid + stmts.slice_from(j + 1);
        }
    }
}

bool FnWriter::loose_jump(const NodeP& s) {
    // An unlabeled break/continue in s that leaves s (not inside a loop or switch of s).
    if (auto b = as_a<ir::Break>(s); b && !truthy(b->label)) return true;
    if (auto c = as_a<ir::Continue>(s); c && !truthy(c->label)) return true;
    if (is_a<ir::While>(s) || is_a<ir::For>(s) || is_a<ir::DoWhile>(s) || is_a<ir::Switch>(s)) return false;
    if (!s) none_attr("children");
    for (const NodeP& c : s->children()) {
        if (is_a<ir::Stmt>(c) && loose_jump(c)) return true;
    }
    return false;
}

void FnWriter::block_stmts(const Nodes& stmts, Lines& out, int64_t ind) { block_stmts(lst(stmts), out, ind); }

void FnWriter::block_stmts(const py::list<NodeP>& stmts_in, Lines& out, int64_t) {
    py::list<NodeP> stmts = structure(stmts_in);
    for (const NodeP& s : stmts) {
        stmt(s, out);
        if (abrupt(s)) break;                                    // (Java rejects what follows a jump in a block)
    }
}

bool FnWriter::abrupt(const NodeP& s) {
    if (is_a<ir::Return>(s) || is_a<ir::Break>(s) || is_a<ir::Continue>(s) || is_a<ir::Throw>(s) || is_a<ir::Goto>(s)) return true;
    if (auto b = as_a<ir::Block>(s)) {
        for (const NodeP& x : lst(b->stmts)) {
            if (abrupt(x)) return true;
        }
        return false;
    }
    if (is_a<ir::While>(s) || is_a<ir::For>(s) || is_a<ir::DoWhile>(s)) return !can_complete(s);
    return false;
}

bool FnWriter::can_complete(const ir::NodeOrList& s) {
    if (std::holds_alternative<py::list<NodeP>>(s)) return true;
    return can_complete(std::get<NodeP>(s));
}

bool FnWriter::can_complete(const NodeP& s) {
    // Java's 'can complete normally', for the statements C programs write.
    if (!s) return true;
    if (is_a<ir::Return>(s) || is_a<ir::Break>(s) || is_a<ir::Continue>(s) || is_a<ir::Throw>(s) || is_a<ir::Goto>(s)) return false;
    if (auto b = as_a<ir::Block>(s)) {
        for (const NodeP& x : lst(b->stmts)) {
            if (!can_complete(x)) return false;
        }
        return true;
    }
    if (auto i = as_a<ir::If>(s)) {
        if (is_none(i->orelse)) return true;
        return can_complete(i->then) || can_complete(i->orelse);
    }
    if (auto w = as_a<ir::While>(s)) {
        if (!w->cond || const_true(w->cond)) return has_break(w->body);
        return true;
    }
    if (auto f = as_a<ir::For>(s)) {
        if (!f->cond || const_true(f->cond)) return has_break(f->body);
        return true;
    }
    if (auto d = as_a<ir::DoWhile>(s)) {
        if (const_true(d->cond)) return has_break(d->body);
        return true;
    }
    if (auto sw = as_a<ir::Switch>(s)) {
        // Java: a switch completes unless it has a default, no break leaves it, and its last group cannot
        const py::list<NodeP>& cases = lst(sw->cases);
        bool any_default = false;
        for (const NodeP& c : cases) {
            auto cc = as_a<ir::Case>(c);
            if (cc->default_ && *cc->default_) any_default = true;
        }
        if (!any_default || cases.empty()) return true;
        for (const NodeP& c : cases) {
            auto blk = std::make_shared<ir::Block>();
            blk->stmts = as_a<ir::Case>(c)->body;
            if (has_break(NodeP(blk))) return true;
        }
        auto blk = std::make_shared<ir::Block>();
        blk->stmts = as_a<ir::Case>(cases[-1])->body;
        return can_complete(NodeP(blk));
    }
    if (auto l = as_a<ir::Labeled>(s)) {
        str brk = S("break ") + jname(l->label);
        for (const NodeP& n : walk_of(l->body)) {
            if (is_a<ir::Goto>(n)) {
                auto it = goto_map.find(n.get());
                if (it != goto_map.end() && it->second == brk) return true;
            }
        }
        return can_complete(l->body);
    }
    return true;
}

bool FnWriter::const_true(const NodeP& c0) {
    NodeP c = strip(c0);
    if (auto k = as_a<ir::Cast>(c)) c = strip(k->x);
    auto l = as_a<ir::Lit>(c);
    if (!l) return false;
    if (auto i = val_int(l->value)) return *i != 0;
    return false;
}

bool FnWriter::has_break(const ir::NodeOrList& body) { return has_break(one(body, "children")); }

bool FnWriter::has_break(const NodeP& n) {
    // A break that leaves this loop (not one inside a nested loop or switch).
    if (is_a<ir::Break>(n)) return true;
    if (is_a<ir::While>(n) || is_a<ir::For>(n) || is_a<ir::DoWhile>(n) || is_a<ir::Switch>(n)) return false;
    if (!n) none_attr("children");
    for (const NodeP& c : n->children()) {
        if (is_a<ir::Stmt>(c) && has_break(c)) return true;
    }
    return false;
}

Lines FnWriter::sub(const ir::NodeOrList& s) {
    if (std::holds_alternative<py::list<NodeP>>(s)) {
        Lines out;
        stmt(s, out);
        return out;
    }
    return sub(std::get<NodeP>(s));
}

Lines FnWriter::sub(const NodeP& s) {
    // A statement as the body of if/while/for: its lines, indented one more.
    Lines out;
    if (auto b = as_a<ir::Block>(s)) block_stmts(b->stmts, out, 0);
    else stmt(s, out);
    Lines r;
    for (const str& x : out) r.append(S("    ") + x);
    return r;
}

void FnWriter::stmt(const ir::NodeOrList& s, Lines& out) {
    if (std::holds_alternative<py::list<NodeP>>(s)) throw Refused("statement list");
    stmt(std::get<NodeP>(s), out);
}

void FnWriter::stmt(const NodeP& s, Lines& out) {
    if (auto b = as_a<ir::Block>(s)) {
        Lines inner;
        block_stmts(b->stmts, inner, 0);
        out.append(S("{"));
        for (const str& x : inner) out.append(S("    ") + x);
        out.append(S("}"));
    } else if (auto l = as_a<ir::Local>(s)) {
        local(l, out);
    } else if (auto es = as_a<ir::ExprStmt>(s)) {
        expr_stmt(es->expr, out);
    } else if (auto r = as_a<ir::Return>(s)) {
        ret(r, out);
    } else if (auto i = as_a<ir::If>(s)) {
        out.append(cat("if (", cond(i->cond).code, ") {"));
        out.extend(sub(i->then));
        // (the else-if chain: e walks the orelse slot)
        ir::NodeOrList e = i->orelse;
        while (std::holds_alternative<NodeP>(e) && is_a<ir::If>(std::get<NodeP>(e))) {
            auto ei = as_a<ir::If>(std::get<NodeP>(e));
            out.append(cat("} else if (", cond(ei->cond).code, ") {"));
            out.extend(sub(ei->then));
            e = ei->orelse;
        }
        if (!is_none(e)) {
            out.append(S("} else {"));
            out.extend(sub(e));
        }
        out.append(S("}"));
    } else if (auto w = as_a<ir::While>(s)) {
        out.append(cat("while (", loop_cond(w->cond), ") {"));
        out.extend(sub(w->body));
        out.append(S("}"));
    } else if (auto d = as_a<ir::DoWhile>(s)) {
        out.append(S("do {"));
        out.extend(sub(d->body));
        out.append(cat("} while (", loop_cond(d->cond), ");"));
    } else if (auto f = as_a<ir::For>(s)) {
        for_(f, out);
    } else if (auto sw = as_a<ir::Switch>(s)) {
        switch_(sw, out);
    } else if (auto b2 = as_a<ir::Break>(s)) {
        out.append(truthy(b2->label) ? cat("break ", jname(b2->label), ";") : S("break;"));
    } else if (auto c = as_a<ir::Continue>(s)) {
        out.append(truthy(c->label) ? cat("continue ", jname(c->label), ";") : S("continue;"));
    } else if (is_a<ir::Empty>(s)) {
    } else if (auto g = as_a<ir::Goto>(s)) {
        auto it = goto_map.find(s.get());
        if (it != goto_map.end()) out.append(it->second + ";");
        else throw Refused(cat("goto ", or_none(g->label), " in ", fn ? or_none(fn->name) : S("an initialiser"), ": not a jump that nests"));
    } else if (auto lb = as_a<ir::Labeled>(s)) {
        if (!labels_done.count(s.get()))
            throw Refused(cat("a label (", or_none(lb->label), ") for goto in ", fn ? or_none(fn->name) : S("None"), ": not one that nests"));
        Lines inner;
        stmt(lb->body, inner);
        if (!inner.empty()) inner[0] = cat(jname(lb->label), ": ", inner[0]);
        out.extend(inner);
    } else {
        throw Refused(S("statement ") + (s ? s->class_name() : S("NoneType")));
    }
}

str FnWriter::loop_cond(const NodeP& c) {
    if (!c || const_true(c)) return S("true");
    return cond(c).code;
}

void FnWriter::for_(const py::ref<ir::For>& s, Lines& out) {
    py::list<Lines> inits;
    bool simple = true;
    for (const NodeP& i : lst_or_empty(s->init)) {
        if (auto lc = as_a<ir::Local>(i)) {
            Lines v_lines;
            local(lc, v_lines);
            if (v_lines.size() != 1 || !v_lines[0].endswith(S(";"))) simple = false;
            inits.append(v_lines);
        } else if (auto es = as_a<ir::ExprStmt>(i)) {
            Lines one_line;
            one_line.append(expr_text(es->expr) + ";");
            inits.append(one_line);
        } else {
            simple = false;
        }
    }
    py::list<str> decls;
    for (const Lines& l : inits) {
        if (l.empty()) continue;
        const std::u32string& u = l[0].u32();
        size_t sp = u.find(U' ');
        str first(u.substr(0, sp));
        if (in(first, {"int", "long", "byte", "short", "char", "float", "double", "boolean"})) decls.append(l[0]);
    }
    py::list<str> updates;
    for (const NodeP& u : lst_or_empty(s->update)) updates.append(expr_text(u));
    bool all_one = true;
    for (const Lines& l : inits) {
        if (l.size() != 1) all_one = false;
    }
    if (simple && all_one && ((decls.size() == inits.size() && inits.size() <= 1) || decls.empty())) {
        auto chop = [](const str& x) { return x.slice(0, x.size() - 1); };
        str init_text;
        if (decls.empty()) {
            py::list<str> parts;
            for (const Lines& l : inits) parts.append(chop(l[0]));
            init_text = join_sep(", ", parts);
        } else {
            init_text = chop(inits[0][0]);
        }
        str ctext = !s->cond ? S("") : (!const_true(s->cond) ? loop_cond(s->cond) : S(""));
        out.append(cat("for (", init_text, "; ", ctext, "; ", join_sep(", ", updates), ") {"));
        out.extend(sub(s->body));
        out.append(S("}"));
        return;
    }
    // an initialiser Java's for cannot hold: before the loop, in a block of its own
    Lines block;
    for (const Lines& l : inits) block.extend(l);
    block.append(cat("for (; ", !s->cond ? S("") : loop_cond(s->cond), "; ", join_sep(", ", updates), ") {"));
    block.extend(sub(s->body));
    block.append(S("}"));
    out.append(S("{"));
    for (const str& x : block) out.append(S("    ") + x);
    out.append(S("}"));
}

void FnWriter::switch_(const py::ref<ir::Switch>& s, Lines& out) {
    J subj = value(s->expr);
    TypeP st = type_of(s->expr);
    if (in(_kind(st), {"i64", "u64"})) throw Refused("a switch on a 64-bit value");
    out.append(cat("switch (", subj.code, ") {"));
    str jt = in(subj.jt, {"int", "char", "byte", "short"}) ? subj.jt : S("int");
    for (const NodeP& cn : lst(s->cases)) {
        auto c = as_a<ir::Case>(cn);
        py::list<str> heads;
        for (const NodeP& lab0 : lst(c->labels)) {
            NodeP lab = strip(lab0);
            auto lit = as_a<ir::Lit>(lab);
            if (!lit) throw Refused("a case label that is not a constant");
            heads.append(cat("case ", int_lit(py_int(lit->value), jt).code, ":"));
        }
        if (c->default_ && *c->default_) heads.append(S("default:"));
        Lines body;
        block_stmts(c->body, body, 0);
        out.append(S("    ") + join_sep(" ", heads));
        for (const str& x : body) out.append(S("        ") + x);
    }
    out.append(S("}"));
}

void FnWriter::local(const py::ref<ir::Local>& s, Lines& out) {
    str name = jname(need_sym(s->sym).name);
    if (declared.contains(name)) name = fresh(name + "_");
    declared.add(name);
    TypeP t = s->type;
    if (has(s->mods, "extern")) return;
    if (has(s->mods, "static")) {
        static_local(s, name);
        return;
    }
    str key = key_l(s->sym);
    bool mem = in(K(t).kind, {"array", "class", "record"}) || p->addr_taken.contains(key);
    Var v(name, mem, t, S("local"));
    if (mem) {
        str addr = alloc(t);
        out.append(cat("int ", name, " = ", addr, ";  // ", ctype_text(t, s->sym->name)));
        vars.set(s->sym->name, v);
        if (s->init) {
            Lines more = init_place(name, t, s->init, true);
            out.extend(more);
        }
        return;
    }
    vars.set(s->sym->name, v);
    str jtt = p->jt(t);
    if (!s->init) {
        out.append(cat(jtt, " ", name, " = ", zero(jtt), ";"));
    } else {
        J val = conv(value(s->init), type_of(s->init), t);
        out.append(cat(jtt, " ", name, " = ", val.code, ";"));
    }
}

void FnWriter::static_local(const py::ref<ir::Local>& s, const str&) {
    if (!fn) none_attr("name");
    auto g = std::make_shared<ir::FieldDecl>();
    g->sym = std::make_shared<ir::Sym>(S("global"), cat(or_none(fn->name), "_", s->sym->name), std::nullopt, true, s->type);
    g->type = s->type;
    g->init = s->init;
    g->mods = py::list<str>{S("static")};
    if (p->addr_taken.contains(key_l(s->sym))) p->addr_taken.add(key_g(g->sym->name));
    p->declare_global(g);
    p->init_global(g);
    vars.set(s->sym->name, p->globals.at(g->sym->name));
}

str FnWriter::zero(const str& jt) {
    if (is(jt, "boolean")) return S("false");
    if (is(jt, "float")) return S("0.0f");
    if (is(jt, "double")) return S("0.0");
    if (is(jt, "long")) return S("0L");
    return S("0");
}

Lines FnWriter::init_place(const str& addr, const TypeP& t, const NodeP& init0, bool) {
    // Statements storing initialiser `init` into the memory of type t at `addr`.
    NodeP init = strip(init0);
    Lines out;
    if (auto lit = as_a<ir::Lit>(init); lit && std::holds_alternative<str>(lit->value.v) && is(K(t).kind, "array")) {
        const str& sv = std::get<str>(lit->value.v);
        Val c = val_get(t->info, "count");
        int64_t n = t->info.contains(S("count")) ? py_int(c) : sv.size() + 1;
        out.append(cat("Mem.setString(", addr, ", ", java_string(sv), ", ", n, ");"));
        return out;
    }
    if (auto na = as_a<ir::NewArray>(init); na && is(K(t).kind, "array")) {
        py::list<NodeP> items = lst_or_empty(na->init);
        int64_t n = t->info.contains(S("count")) ? py_int(t->info.at(S("count"))) : items.size();
        auto leaves = flat_leaves(items, t->elem);
        if (leaves) {
            TypeP et = leaves->second;
            int64_t total = p->size(t) / std::max<int64_t>(p->size(et), 1);
            if (leaves->first.size() < total) out.append(cat("Mem.fill(", addr, ", 0, ", p->size(t), ");"));
            if (!leaves->first.empty()) {
                py::list<str> vals;
                for (const NodeP& x : leaves->first) {
                    std::optional<str> sl = small_lit(x, et);
                    if (!(sl && !sl->empty())) vals.append(conv(value(x), type_of(x), et).code);
                    else vals.append(*sl);
                }
                str acc = p->access(et);
                out.extend(wrap_call(cat("Mem.set", acc, "s(", addr, ", "), vals, S(");")));
            }
            return out;
        }
        int64_t esz = p->size(t->elem);
        if (items.size() < n) out.append(cat("Mem.fill(", addr, ", 0, ", p->size(t), ");"));
        for (int64_t i = 0; i < items.size(); ++i) out.extend(init_place(offset(addr, i * esz), t->elem, items[i]));
        return out;
    }
    if (auto nw = as_a<ir::New>(init); nw && is_record(t) && nw->sym && starts(nw->sym->name, "<aggregate")) {
        TypeP lay = p->layout(t);
        std::vector<LayoutField> fields = layout_fields(lay);
        str ln = p->layout_names.at(p->rec_name(t));
        if (starts(nw->sym->name, "<aggregate:")) {
            str fname = nw->sym->name.slice(str("<aggregate:").size(), nw->sym->name.size() - 1);
            std::vector<LayoutField> keep;
            for (const LayoutField& f : fields) {
                if (f.name == fname) keep.push_back(f);
            }
            fields = keep;
        }
        Val role = val_get(lay->info, "role");
        bool is_union = std::holds_alternative<str>(role.v) && is(std::get<str>(role.v), "union");
        py::list<NodeP> args = lst_or_empty(nw->args);
        if (args.size() < static_cast<int64_t>(fields.size()) || is_union) out.append(cat("Mem.fill(", addr, ", 0, ", ln, ".SIZE);"));
        for (size_t i = 0; i < std::min<size_t>(fields.size(), args.size()); ++i)
            out.extend(init_place(field_addr(addr, ln, fields[i]), fields[i].type, args[i]));
        return out;
    }
    if (is_record(t)) {
        out.append(cat("Mem.copy(", addr, ", ", value(init).code, ", ", p->size_code(t), ");"));
        return out;
    }
    J val = conv(value(init), type_of(init), t);
    out.append(cat("Mem.set", p->access(t), "(", addr, ", ", val.code, ");"));
    return out;
}

std::optional<str> FnWriter::small_lit(const NodeP& x0, const TypeP& et) {
    // An integer literal element of an array initialiser, written as an int for Mem.setBytes & co.
    NodeP x = strip(x0);
    if (auto c = as_a<ir::Cast>(x); c && c->implicit && *c->implicit) x = strip(c->x);
    auto l = as_a<ir::Lit>(x);
    if (l && std::holds_alternative<int64_t>(l->value.v) && is_int(et)) {
        int64_t v = std::get<int64_t>(l->value.v);
        str jtt = p->jt(et);
        if (is(jtt, "long")) return int_lit(v, S("long")).code;
        if (truthy(l->text) && starts(*l->text, "'") && 0 <= v && v < 128) return !is(*l->text, "'\\0'") ? *l->text : S("0");
        if (is_unsigned(et) || (in(jtt, {"byte", "short"}) && v >= 0)) {
            int64_t b = !is(jtt, "char") ? _JBITS().at(jtt) : 16;
            v &= (int64_t(1) << b) - 1;
            if (is(jtt, "byte") && v > 9) {
                char buf[16];
                snprintf(buf, sizeof buf, "0x%02llx", static_cast<unsigned long long>(v));
                return S(buf);
            }
            return cat(v);
        }
        return int_lit(v, S("int")).code;
    }
    return std::nullopt;
}

std::optional<std::pair<py::list<NodeP>, TypeP>> FnWriter::flat_leaves(const py::list<NodeP>& items, const TypeP& et) {
    // The scalar leaves of a (nested) array initialiser when it covers whole rows -> ([exprs], scalar type).
    using R = std::pair<py::list<NodeP>, TypeP>;
    if (is(K(et).kind, "array")) {
        py::list<NodeP> rows;
        bool has_n = et->info.contains(S("count")) && !std::holds_alternative<std::monostate>(et->info.at(S("count")).v);
        for (const NodeP& x0 : items) {
            NodeP x = strip(x0);
            auto na = as_a<ir::NewArray>(x);
            if (!na) return std::nullopt;
            auto got = flat_leaves(lst_or_empty(na->init), et->elem);
            if (!got || !has_n) return std::nullopt;
            int64_t sb = p->size(got->second);
            if (sb == 0) throw py::ZeroDivisionError("integer division or modulo by zero");
            int64_t per = p->size(et) / sb;
            if (got->first.size() != per && x.get() != items[-1].get()) return std::nullopt;
            rows.extend(got->first);
        }
        return R(rows, _scalar(et));
    }
    if (is_record(et)) return std::nullopt;
    return R(items.copy(), et);
}

TypeP FnWriter::_scalar(TypeP t) {
    while (is(K(t).kind, "array")) t = t->elem;
    return t;
}

Lines FnWriter::wrap_call(const str& head, const py::list<str>& vals, const str& tail, int64_t width) {
    str line = head + join_sep(", ", vals) + tail;
    Lines out;
    if (line.size() <= width) {
        out.append(line);
        return out;
    }
    out.append(head.rstrip());
    str cur = S("        ");
    for (int64_t i = 0; i < vals.size(); ++i) {
        str piece = vals[i] + (i < vals.size() - 1 ? ", " : "");
        if (cur.size() + piece.size() > width) {
            out.append(cur.rstrip());
            cur = S("        ");
        }
        cur = cur + piece;
    }
    out.append(cur.rstrip() + tail);
    return out;
}

str FnWriter::offset(const str& addr, int64_t off) {
    if (off == 0) return addr;
    // re.match(r'^(.*) \+ (\d+)$', addr) with no '(' or '?' in the first group: for ASCII text without a newline
    // the group is everything before the last " + " when only digits follow it (read directly; the regex runs
    // for any other text)
    {
        const std::u32string& u = addr.u32();
        bool simple = true;
        for (char32_t ch : u) {
            if (ch >= 128 || ch == U'\n') {
                simple = false;
                break;
            }
        }
        if (simple) {
            size_t at = u.rfind(U" + ");
            if (at != std::u32string::npos) {
                size_t k = at + 3;
                bool digits = k < u.size();
                for (size_t j = k; j < u.size(); ++j) {
                    if (u[j] < U'0' || u[j] > U'9') digits = false;
                }
                if (digits) {
                    str g1(u.substr(0, at));
                    if (!g1.contains(S("(")) && !g1.contains(S("?"))) return cat(g1, " + ", py::int_parse(str(u.substr(k)), 10) + off);
                }
            }
            if (is(addr, "frame")) return cat("frame + ", off);
            return cat(addr, " + ", off);
        }
    }
    static const py::re::Pattern pt = py::re::compile(S("^(.*) \\+ (\\d+)$"));
    if (auto m = pt.match(addr)) {
        str g1 = m->group(1);
        if (!g1.contains(S("(")) && !g1.contains(S("?"))) return cat(g1, " + ", py::int_parse(m->group(2), 10) + off);
    }
    if (is(addr, "frame")) return cat("frame + ", off);
    return cat(addr, " + ", off);
}

str FnWriter::field_addr(const str& addr, const str& ln, const LayoutField& f) {
    str fname = !is(f.name, "SIZE") ? jname(f.name) : S("SIZE_");
    return cat(addr, " + ", ln, ".", fname);
}

void FnWriter::ret(const py::ref<ir::Return>& s, Lines& out) {
    if (!s->expr) {
        if (fn && fn->ret && !is(fn->ret->kind, "void")) out.append(cat("return ", zero(p->jt(fn->ret)), ";  // (C: a return without a value)"));
        else out.append(S("return;"));
        return;
    }
    if (ret_struct) {
        out.append(cat("return Mem.copy(result, ", value(s->expr).code, ", ", p->size_code(fn->ret), ");"));
        return;
    }
    if (fn && fn->ret && is(fn->ret->kind, "void")) {
        expr_stmt(s->expr, out);
        out.append(S("return;"));
        return;
    }
    if (!fn) none_attr("ret");
    J v = conv(value(s->expr), type_of(s->expr), fn->ret);
    out.append(cat("return ", v.code, ";"));
}

void FnWriter::expr_stmt(const NodeP& e0, Lines& out) {
    NodeP e = strip(e0);
    if (auto c = as_a<ir::Cast>(e); c && c->type && is(c->type->kind, "void")) {
        e = strip(c->x);
        if (is_a<ir::Name>(e) || is_a<ir::Lit>(e)) return;
    }
    if (auto b = as_a<ir::Binary>(e); b && opt_is(b->op, ",")) {
        expr_stmt(b->left, out);
        expr_stmt(b->right, out);
        return;
    }
    if (is_a<ir::Lit>(e) || is_a<ir::Name>(e)) return;
    if (auto b = as_a<ir::Binary>(e); b && opt_in(b->op, {"&&", "||"}) && !pure(b->right)) {
        // `a || f();` as a statement (assert's expansion): if (!a) f();
        J c = cond(b->left);
        if (is(*b->op, "||")) {
            if (!c.code.endswith(S(" != 0"))) c = J(S("!") + c.at(P_UNARY), P_UNARY, S("boolean"));
            else c = J(c.code.slice(0, c.code.size() - 5) + " == 0", P_EQ, S("boolean"));
        }
        Lines inner;
        expr_stmt(b->right, inner);
        out.append(cat("if (", c.code, ") {"));
        for (const str& x : inner) out.append(S("    ") + x);
        out.append(S("}"));
        return;
    }
    if (auto cd = as_a<ir::Cond>(e)) {
        if (statement_expr(cd->then) && statement_expr(cd->orelse)) {
            out.append(cat("if (", cond(cd->cond).code, ") {"));
            Lines discard;
            expr_stmt(cd->then, discard);
            Lines a, b;
            expr_stmt(cd->then, a);
            expr_stmt(cd->orelse, b);
            for (const str& x : a) out.append(S("    ") + x);
            out.append(S("} else {"));
            for (const str& x : b) out.append(S("    ") + x);
            out.append(S("}"));
            return;
        }
    }
    if (auto as = as_a<ir::Assign>(e); as && as->op && !pure_place(as->target)) {
        Place place_ = place(as->target);
        if (place_.mem) {
            str t = fresh(S("at"));
            out.append(S("{"));
            out.append(cat("    int ", t, " = ", place_.java, ";"));
            out.append(cat("    ", compound(Place(t, true, place_.type), as).code, ";"));
            out.append(S("}"));
            return;
        }
    }
    str text = expr_text(e);
    if (!statement_expr(e) && !(starts(text, "Mem.") || starts(text, "printf"))) {
        if (java_statement(text)) out.append(text + ";");
        else out.append(cat("discard(", text, ");"));
        return;
    }
    out.append(text + ";");
}

bool FnWriter::java_statement(const str& text) {
    // Is this Java text a statement expression (an assignment, ++/--, a call, new)?
    if (pat("^[\\w.]+\\(.*\\)$", py::re::S).match(text) && _balanced_call(text)) return true;
    if (pat("^(\\+\\+|--)[\\w.\\[\\]]+$").match(text) || pat("^[\\w.\\[\\]]+(\\+\\+|--)$").match(text)) return true;
    if (pat("^[\\w.\\[\\]]+ (=|\\+=|-=|\\*=|/=|%=|&=|\\|=|\\^=|<<=|>>=|>>>=) ").match(text)) return true;
    return false;
}

bool FnWriter::statement_expr(const NodeP& e0) {
    NodeP e = strip(e0);
    if (is_a<ir::Assign>(e) || is_a<ir::Call>(e)) return true;
    auto u = as_a<ir::Unary>(e);
    return u && opt_in(u->op, {"++", "--"});
}

str FnWriter::expr_text(const NodeP& e0) {
    NodeP e = strip(e0);
    if (auto u = as_a<ir::Unary>(e); u && opt_in(u->op, {"++", "--"})) return incdec(u, true).code;
    return value(e).code;
}

// ------------------------------------------------------------------------------------- places
Var FnWriter::var(const SymP& s) {
    if (vars.contains(s->name) && in(s->kind, {"local", "param"})) return vars.at(s->name);
    if (p->globals.contains(s->name)) return p->globals.at(s->name);
    throw Refused(cat("unknown variable ", s->name, " (", s->kind, ", from ", or_none(s->owner), ")"));
}

Place FnWriter::place(const NodeP& e0) {
    // An lvalue -> Place (a Java variable, or memory at an address).
    NodeP e = strip(e0);
    if (auto n = as_a<ir::Name>(e)) {
        need_sym(n->sym);
        Var v = var(n->sym);
        return Place(v.java, v.mem, v.type);
    }
    if (auto u = as_a<ir::Unary>(e); u && opt_is(u->op, "*")) {
        str c = value(u->x).code;
        return Place(c, true, u->type, value(u->x).prec);
    }
    if (auto ix = as_a<ir::Index>(e)) {
        J base = value(ix->arr);                         // (an array decays to its address)
        TypeP et = ix->type;
        NodeP idxn = one(ix->index, "type");
        J idx = conv(value(idxn), type_of(idxn), ir::I32);
        std::optional<int64_t> size = !is_record(et) ? std::optional<int64_t>(p->size(et)) : std::nullopt;
        str scale = p->size_code(et);
        if (is_a<ir::Lit>(strip(idxn)) || _is_int_text(idx.code)) {
            std::optional<int64_t> n;
            if (_is_int_text(idx.code)) n = py::int_parse(idx.code.strip(S("()")), 10);
            if (n && size) return Place(offset(base.at(P_ADD), *n * *size), true, et, P_ADD);
        }
        str term = is(scale, "1") ? idx.at(P_MUL) : cat(idx.at(P_MUL), " * ", scale);
        return Place(cat(base.at(P_ADD), " + ", term), true, et, P_ADD);
    }
    if (auto sl = as_a<ir::Select>(e)) {
        NodeP obj = sl->obj;
        TypeP ot = type_of(obj);
        if (!is_record(ot)) throw Refused("a field of a non-struct");
        J base = addr(obj);
        LayoutField f = p->field_off(ot, need_sym(sl->sym).name);
        str ln = p->layout_names.at(p->rec_name(ot));
        return Place(field_addr(base.at(P_ADD), ln, f), true, !sl->type ? f.type : sl->type, P_ADD);
    }
    if (auto c = as_a<ir::Cast>(e)) return place(c->x);
    throw Refused(S("an lvalue ") + (e ? e->class_name() : S("NoneType")));
}

bool FnWriter::pure_place(const NodeP& e) { return pure(e); }

bool FnWriter::pure(const NodeP& e) {
    for (const NodeP& n : walk_node(e)) {
        if (is_a<ir::Assign>(n) || is_a<ir::Call>(n)) return false;
        if (auto u = as_a<ir::Unary>(n); u && opt_in(u->op, {"++", "--"})) return false;
    }
    return true;
}

J FnWriter::addr(const NodeP& e0) {
    // The address of an lvalue in memory (an array or struct: the value itself).
    NodeP e = strip(e0);
    if (auto u = as_a<ir::Unary>(e); u && opt_is(u->op, "*")) return value(u->x);
    if ((is_a<ir::Call>(e) || is_a<ir::New>(e) || is_a<ir::Assign>(e) || is_a<ir::Cond>(e)) && is_record(type_of(e))) return value(e);
    Place pl = place(e);
    if (!pl.mem) throw Refused(cat("the address of ", pl.java, ", which is not in memory"));
    return J(pl.java, pl.prec, S("int"));
}

J FnWriter::load(const Place& pl) {
    if (!pl.mem) return J(pl.java, P_ATOM, p->jt(pl.type));
    const TypeP& t = pl.type;
    if (in(K(t).kind, {"array", "class", "record"})) return J(pl.java, pl.prec, S("int"));
    return J(cat("Mem.get", p->access(t), "(", pl.java, ")"), P_POST, p->jt(t));
}

J FnWriter::store(const Place& pl, const J& val) {
    // Assign val (a J already of the place's type) -> J of the assignment.
    if (!pl.mem) return J(cat(pl.java, " = ", val.at(P_ASSIGN)), P_ASSIGN, p->jt(pl.type));
    const TypeP& t = pl.type;
    if (is_record(t)) return J(cat("Mem.copy(", pl.java, ", ", val.code, ", ", p->size_code(t), ")"), P_POST, S("int"));
    return J(cat("Mem.set", p->access(t), "(", pl.java, ", ", val.code, ")"), P_POST, p->jt(t));
}

// ------------------------------------------------------------------------------------- values
J FnWriter::cond(const NodeP& e0) {
    // A C condition as a Java boolean.
    NodeP e = strip(e0);
    J j = value(e);
    return truth(j, type_of(e));
}

J FnWriter::truth(const J& j, const TypeP&) {
    if (is(j.jt, "boolean")) return j;
    return J(cat(j.at(P_EQ + 1), " != 0"), P_EQ, S("boolean"));
}

J FnWriter::as_value(const J& j, const TypeP& t) {
    // A J that may be a Java boolean as the C value of type t (1/0).
    if (is(j.jt, "boolean") && !is(_kind(t), "bool")) {
        str jtt = p->jt(t);
        const char* one_ = is(jtt, "long") ? "1L" : "1";
        const char* zero_ = is(jtt, "long") ? "0L" : "0";
        return J(cat(j.at(P_COND + 1), " ? ", one_, " : ", zero_), P_COND, jtt);
    }
    return j;
}

J FnWriter::value(const NodeP& e) {
    if (!e) throw Refused("expression NoneType");
    if (auto x = as_a<ir::Lit>(e)) return x_Lit(x);
    if (auto x = as_a<ir::Name>(e)) return x_Name(x);
    if (auto x = as_a<ir::Binary>(e)) return x_Binary(x);
    if (auto x = as_a<ir::Unary>(e)) return x_Unary(x);
    if (auto x = as_a<ir::Cast>(e)) return x_Cast(x);
    if (auto x = as_a<ir::Call>(e)) return x_Call(x);
    if (auto x = as_a<ir::Select>(e)) return x_Select(x);
    if (auto x = as_a<ir::Index>(e)) return x_Index(x);
    if (auto x = as_a<ir::Assign>(e)) return x_Assign(x);
    if (auto x = as_a<ir::Cond>(e)) return x_Cond(x);
    if (auto x = as_a<ir::Paren>(e)) return x_Paren(x);
    if (auto x = as_a<CodeNode>(e)) return x__Code(x);
    if (auto x = as_a<ir::AddressOf>(e)) return x_AddressOf(x);
    if (auto x = as_a<ir::New>(e)) return x_New(x);
    if (auto x = as_a<ir::NewArray>(e)) return x_NewArray(x);
    if (auto x = as_a<ir::TypeName>(e)) return x_TypeName(x);
    if (auto x = as_a<ir::Unsupported>(e)) return x_Unsupported(x);
    if (auto x = as_a<ir::This>(e)) return x_This(x);
    if (auto x = as_a<ir::Length>(e)) return x_Length(x);
    if (auto x = as_a<ir::Concat>(e)) return x_Concat(x);
    throw Refused(S("expression ") + e->class_name());
}

J FnWriter::x_This(const py::ref<ir::This>&) { throw Refused("expression This"); }
J FnWriter::x_Length(const py::ref<ir::Length>&) { throw Refused("expression Length"); }
J FnWriter::x_Concat(const py::ref<ir::Concat>&) { throw Refused("expression Concat"); }

J FnWriter::x_Paren(const py::ref<ir::Paren>& e) {
    // the grouping the source chose is kept where it groups an operator (readability, not precedence)
    J j = value(e->x);
    if (j.prec < P_UNARY && !is(j.jt, "void")) return J(cat("(", j.code, ")"), P_ATOM, j.jt, j.fresh);
    return j;
}

J FnWriter::x_Unsupported(const py::ref<ir::Unsupported>& e) {
    throw Refused(cat(or_none(e->kind), " (line ", e->line, "): the front end did not lower it"));
}

J FnWriter::x_Lit(const py::ref<ir::Lit>& e) {
    Val v = e->value;
    TypeP t = e->type;
    if (std::holds_alternative<str>(v.v) && is_float(t)) v = Val(py::float_from_str(std::get<str>(v.v)));   // (L2 keeps inf and nan as text)
    if (std::holds_alternative<bool>(v.v)) {
        bool b = std::get<bool>(v.v);
        if (is(_kind(t), "bool")) return J(b ? S("true") : S("false"), P_ATOM, S("boolean"));
        return J(b ? S("1") : S("0"), P_ATOM, S("int"));
    }
    if (std::holds_alternative<std::monostate>(v.v)) return J(S("0"), P_ATOM, S("int"));
    if (std::holds_alternative<str>(v.v)) {
        return J(cat("Mem.", is_wide(e) ? "wstr" : "str", "(", string_lit(e), ")"), P_POST, S("int"));
    }
    str jtt = (t && !is(t->kind, "any")) ? p->jt(t) : S("int");
    if (std::holds_alternative<double>(v.v) || is_float(t)) {
        double d = std::holds_alternative<double>(v.v) ? std::get<double>(v.v) : static_cast<double>(py_int(v));
        return float_lit(d, in(jtt, {"float", "double"}) ? jtt : S("double"));
    }
    if (truthy(e->text) && starts(*e->text, "'") && is(jtt, "int")) return J(*e->text, P_ATOM, S("char"));
    return int_lit(py_int(v), jtt);
}

J FnWriter::x_Name(const py::ref<ir::Name>& e) {
    Var v = var(e->sym);
    if (v.mem && in(K(v.type).kind, {"array", "class", "record"})) return J(v.java, P_ATOM, S("int"));
    return load(Place(v.java, v.mem, v.type));
}

J FnWriter::x_Select(const py::ref<ir::Select>& e) { return load(place(e)); }
J FnWriter::x_Index(const py::ref<ir::Index>& e) { return load(place(e)); }
J FnWriter::x_TypeName(const py::ref<ir::TypeName>&) { throw Refused("a type used as a value"); }
J FnWriter::x_AddressOf(const py::ref<ir::AddressOf>& e) { return J(p->fn_const(need_sym(e->sym).name), P_ATOM, S("int")); }

J FnWriter::x_Unary(const py::ref<ir::Unary>& e) {
    str op = or_none(e->op);
    if (is(op, "*")) {
        if (e->type && is(e->type->kind, "fn")) return value(e->x);
        return load(place(e));
    }
    if (is(op, "&")) {
        NodeP x = strip(e->x);
        if (auto ao = as_a<ir::AddressOf>(x)) return value(ao);
        return addr(x);
    }
    if (is(op, "++") || is(op, "--")) return incdec(e);
    if (is(op, "!")) {
        J c = cond(e->x);
        if (c.code.endswith(S(" != 0")) && c.prec == P_EQ) return J(c.code.slice(0, c.code.size() - 5) + " == 0", P_EQ, S("boolean"));
        return J(S("!") + c.at(P_UNARY), P_UNARY, S("boolean"));
    }
    J x = as_value(value(e->x), type_of(e->x));
    str jtt = p->jt(e->type);
    if (is(op, "-")) {
        str code = S("-") + x.at(P_UNARY);
        if (starts(code, "--")) code = cat("-(", x.code, ")");
        return narrow(J(code, P_UNARY, _promoted(x.jt)), jtt);
    }
    if (is(op, "+")) return x;
    if (is(op, "~")) return narrow(J(S("~") + x.at(P_UNARY), P_UNARY, _promoted(x.jt)), jtt);
    throw Refused(S("unary ") + op);
}

J FnWriter::narrow(const J& j, const str& jt) {
    // Java widened a byte/short/char operand to int: back to the C type's Java type.
    if (j.jt == jt || (in(jt, {"int", "long", "float", "double", "boolean"}) && j.jt == jt)) return j;
    if (in(jt, {"byte", "short", "char"}) && in(j.jt, {"int", "long"})) return J(cat("(", jt, ") ", j.at(P_UNARY)), P_UNARY, jt);
    if (is(jt, "int") && is(j.jt, "long")) return J(S("(int) ") + j.at(P_UNARY), P_UNARY, S("int"));
    return (is(j.jt, "int") && is(jt, "int")) ? J(j.code, j.prec, jt) : j;
}

J FnWriter::incdec(const py::ref<ir::Unary>& e, bool as_statement) {
    TypeP t = e->type;
    Place pl = place(e->x);
    int64_t delta = 1;
    if (is_ptr(t)) delta = is(t->kind, "ptr") ? p->size(t->elem) : 1;
    const char* sign = opt_is(e->op, "++") ? "+" : "-";
    bool postfix = e->postfix && *e->postfix;
    if (!pl.mem) {
        if (delta == 1 || !is_ptr(t)) {
            if (is(_kind(t), "bool")) throw Refused("++ on a bool");
            str code = postfix ? pl.java + or_none(e->op) : or_none(e->op) + pl.java;
            return J(code, postfix ? P_POST : P_UNARY, p->jt(t));
        }
        if (as_statement || !postfix) return J(cat(pl.java, " ", sign, "= ", delta), P_ASSIGN, S("int"));
        return J(cat("(", pl.java, " ", sign, "= ", delta, ") ", is(S(sign), "+") ? "-" : "+", " ", delta), P_ADD, S("int"));
    }
    str acc = p->access(t);
    int64_t d = is(S(sign), "+") ? delta : -delta;
    str jtt = p->jt(t);
    str dl = is(jtt, "long") ? int_lit(d, S("long")).code : (in(jtt, {"float", "double"}) ? float_lit(static_cast<double>(d), jtt).code : cat(d));
    const char* fn_ = (postfix && !as_statement) ? "Mem.postAdd" : "Mem.add";
    return J(cat(fn_, acc, "(", pl.java, ", ", dl, ")"), P_POST, jtt);
}

J FnWriter::x_Binary(const py::ref<ir::Binary>& e) {
    str op = or_none(e->op);
    if (is(op, ",")) {
        J l = value(e->left);
        J r = value(e->right);
        return J(cat("seq(", l.code, ", ", r.code, ")"), P_POST, r.jt);
    }
    if (is(op, "&&") || is(op, "||")) {
        J l = cond(e->left), r = cond(e->right);
        int64_t pr = _BINPREC(op);
        return J(cat(l.at(pr), " ", op, " ", r.at(pr + 1)), pr, S("boolean"));
    }
    TypeP lt = type_of(e->left), rt = type_of(e->right);
    if (in(op, {"<", "<=", ">", ">=", "==", "!="})) return compare(e, op);
    // pointer arithmetic
    if (in(op, {"+", "-"}) && is_ptr(lt) && is_int(rt)) {
        J a = value(e->left);
        J b = value(e->right);
        return ptr_add(a, b, rt, lt, op);
    }
    if (is(op, "+") && is_int(lt) && is_ptr(rt)) {
        J a = value(e->right);
        J b = value(e->left);
        return ptr_add(a, b, lt, rt, S("+"));
    }
    if (is(op, "-") && is_ptr(lt) && is_ptr(rt)) {
        int64_t size = is(lt->kind, "ptr") ? p->size(lt->elem) : 1;
        str a = value(e->left).at(P_ADD);
        str b = value(e->right).at(P_ADD + 1);
        str diff = cat(a, " - ", b);
        str code = size == 1 ? diff : cat("(", diff, ") / ", size);
        J j(code, size == 1 ? P_ADD : P_MUL, S("int"));
        return conv(j, ir::I32, e->type);
    }
    TypeP t = e->type;
    J l = as_value(value(e->left), lt);
    J r = as_value(value(e->right), rt);
    str jtt = p->jt(t);
    if (in(op, {"<<", ">>", ">>>"})) {
        str jop = (is(op, ">>") && is_unsigned(t)) ? S(">>>") : op;
        int64_t pr = P_SHIFT;
        J rr = r;
        if (is(r.jt, "long")) rr = J(S("(int) ") + r.at(P_UNARY), P_UNARY, S("int"));
        return narrow(J(cat(l.at(pr), " ", jop, " ", rr.at(pr + 1)), pr, _promoted(l.jt)), jtt);
    }
    if (is_unsigned(t) && in(op, {"/", "%"}) && in(_kind(t), {"u32", "u64"})) {
        const char* w = is(_kind(t), "u32") ? "Integer" : "Long";
        const char* fnn = is(op, "/") ? "divideUnsigned" : "remainderUnsigned";
        return J(cat(w, ".", fnn, "(", l.code, ", ", r.code, ")"), P_POST, jtt);
    }
    int64_t pr = _BINPREC(op);
    J res(cat(l.at(pr), " ", op, " ", r.at(pr + 1)), pr, _promoted2(l.jt, r.jt));
    return in(jtt, {"byte", "short", "char"}) ? narrow(res, jtt) : res;
}

J FnWriter::ptr_add(const J& base, const J& idx, const TypeP& it, const TypeP& pt, const str& op) {
    int64_t size = is(pt->kind, "ptr") ? p->size(pt->elem) : 1;
    J i = conv(idx, it, ir::I32);
    if (_is_int_text(i.code)) {
        int64_t n = py::int_parse(i.code.strip(S("()")), 10) * size;
        if (is(op, "-")) n = -n;
        if (n == 0) return base;
        return J(cat(base.at(P_ADD), " ", n > 0 ? "+" : "-", " ", n < 0 ? -n : n), P_ADD, S("int"));
    }
    str term = size == 1 ? i.at(P_MUL + (is(op, "-") ? 1 : 0)) : cat(i.at(P_MUL), " * ", size);
    if (size != 1 && is(op, "-")) term = cat("(", term, ")");
    return J(cat(base.at(P_ADD), " ", op, " ", term), P_ADD, S("int"));
}

J FnWriter::compare(const py::ref<ir::Binary>& e, const str& op) {
    TypeP lt = type_of(e->left), rt = type_of(e->right);
    J l = as_value(value(e->left), lt);
    J r = as_value(value(e->right), rt);
    int64_t pr = _BINPREC(op);
    if (in(op, {"<", "<=", ">", ">="}) && (is_unsigned(lt) && in(_kind(lt), {"u32", "u64"}))) {
        const char* w = is(_kind(lt), "u32") ? "Integer" : "Long";
        return J(cat(w, ".compareUnsigned(", l.code, ", ", r.code, ") ", op, " 0"), P_REL, S("boolean"));
    }
    if (is(l.jt, "boolean") && is(r.jt, "boolean")) return J(cat(l.at(pr), " ", op, " ", r.at(pr + 1)), pr, S("boolean"));
    if (is(l.jt, "boolean")) l = as_value(l, ir::I32);
    if (is(r.jt, "boolean")) r = as_value(r, ir::I32);
    return J(cat(l.at(pr), " ", op, " ", r.at(pr + 1)), pr, S("boolean"));
}

J FnWriter::x_Assign(const py::ref<ir::Assign>& e) {
    if (!e->op) {
        Place pl = place(e->target);
        TypeP t = type_of(e->target);
        if (is_record(t)) {
            str a = addr(e->target).code;
            str v = value(e->value).code;
            return J(cat("Mem.copy(", a, ", ", v, ", ", p->size_code(t), ")"), P_POST, S("int"));
        }
        J val = conv(value(e->value), type_of(e->value), t);
        return store(pl, val);
    }
    Place pl = place(e->target);
    if (pl.mem && !pure(e->target)) throw Refused("a compound assignment through an address with side effects, inside an expression");
    return compound(pl, e);
}

J FnWriter::compound(const Place& pl, const py::ref<ir::Assign>& e) {
    TypeP t = type_of(e->target);
    str op = or_none(e->op);
    TypeP vt = type_of(e->value);
    if (is_ptr(t)) {
        int64_t size = is(t->kind, "ptr") ? p->size(t->elem) : 1;
        J i = conv(value(e->value), vt, ir::I32);
        str term = size == 1 ? i.code : cat(i.at(P_MUL), " * ", size);
        if (!pl.mem) return J(cat(pl.java, " ", op, "= ", term), P_ASSIGN, S("int"));
        str rhs = size == 1 ? i.at(P_MUL) : (is(op, "-") ? cat("(", term, ")") : term);
        return store(pl, J(cat(load(pl).at(P_ADD), " ", op, " ", rhs), P_ADD, S("int")));
    }
    str jtt = p->jt(t);
    J val = as_value(value(e->value), vt);
    if (!pl.mem && is_int(t) && in(op, {"+", "-", "*", "&", "|", "^", "<<"}) && is_int(vt) &&
        (_BITS().at(_kind(vt)) <= 32 || is(jtt, "long"))) {
        J v = val;
        if (is(op, "<<") && is(v.jt, "long")) v = J(S("(int) ") + v.at(P_UNARY), P_UNARY, S("int"));
        return J(cat(pl.java, " ", op, "= ", v.at(P_ASSIGN)), P_ASSIGN, jtt);
    }
    if (!pl.mem && in(_kind(t), {"i32", "i64", "f64", "f32"}) && _kind(vt) == _kind(t) && in(op, {"/", "%", ">>"}))
        return J(cat(pl.java, " ", op, "= ", val.at(P_ASSIGN)), P_ASSIGN, jtt);
    if (!pl.mem && in(_kind(t), {"u32", "u64"}) && _kind(vt) == _kind(t) && is(op, ">>"))
        return J(cat(pl.java, " >>>= ", val.at(P_ASSIGN)), P_ASSIGN, jtt);
    if (!pl.mem && is_float(t) && is_float(vt) && _kind(t) == _kind(vt))
        return J(cat(pl.java, " ", op, "= ", val.at(P_ASSIGN)), P_ASSIGN, jtt);
    // the general form: target = (T) (target op value), in the computation type
    TypeP ct = !in(op, {"<<", ">>"}) ? vt : _promote_type(t);
    if (is_float(t) && !is_float(vt)) ct = t;
    J cur = conv(load(pl), t, ct);
    auto lc = std::make_shared<CodeNode>();
    lc->j = cur;
    lc->type = ct;
    auto rc = std::make_shared<CodeNode>();
    rc->j = val;
    rc->type = vt;
    auto fake = std::make_shared<ir::Binary>();
    fake->op = op;
    fake->left = lc;
    fake->right = rc;
    fake->type = ct;
    J res = value(fake);
    return store(pl, conv(res, ct, t));
}

J FnWriter::x__Code(const py::ref<CodeNode>& e) { return e->j; }

J FnWriter::x_Cond(const py::ref<ir::Cond>& e) {
    J c = cond(e->cond);
    TypeP t = e->type;
    if (is_record(t)) {
        J a = value(e->then), b = value(e->orelse);
        return J(cat(c.at(P_COND + 1), " ? ", a.at(P_COND + 1), " : ", b.at(P_COND)), P_COND, S("int"));
    }
    J a = conv(value(e->then), type_of(e->then), t);
    J b = conv(value(e->orelse), type_of(e->orelse), t);
    str jtt = !is(K(t).kind, "void") ? p->jt(t) : a.jt;
    if (is(t->kind, "void")) throw Refused("a conditional expression of type void used as a value");
    if (in(jtt, {"byte", "short", "char"})) {
        J res(cat(c.at(P_COND + 1), " ? ", a.at(P_COND + 1), " : ", b.at(P_COND)), P_COND, S("int"));
        return J(cat("(", jtt, ") (", res.code, ")"), P_UNARY, jtt);
    }
    return J(cat(c.at(P_COND + 1), " ? ", a.at(P_COND + 1), " : ", b.at(P_COND)), P_COND, jtt);
}

J FnWriter::x_Cast(const py::ref<ir::Cast>& e) {
    NodeP x = strip(e->x);
    TypeP t = e->type;
    TypeP st = type_of(x);
    if (is(K(t).kind, "void")) return value(x);
    if (st && is(st->kind, "array") && is_ptr(t)) {
        if (auto l = as_a<ir::Lit>(x); l && std::holds_alternative<str>(l->value.v))
            return J(cat("Mem.", is_wide(l) ? "wstr" : "str", "(", string_lit(l), ")"), P_POST, S("int"));
        return addr(x);
    }
    if (auto l = as_a<ir::Lit>(x); l && std::holds_alternative<std::monostate>(l->value.v)) return J(S("0"), P_ATOM, S("int"));
    if (auto ao = as_a<ir::AddressOf>(x)) return value(ao);
    J j = value(x);
    return conv(j, st, t);
}

J FnWriter::conv(const J& j0, const TypeP& s, const TypeP& t) {
    // Convert a Java value j of C type s to C type t.
    J j = j0;
    if (!t || !s) return j;
    str sk = _kind(s), tk = _kind(t);
    if (is(tk, "void")) return j;
    if (is(tk, "bool")) {
        if (is(j.jt, "boolean")) return j;
        return J(cat(j.at(P_EQ + 1), " != 0"), P_EQ, S("boolean"));
    }
    if (is(j.jt, "boolean")) {
        j = J(cat(j.at(P_COND + 1), " ? 1 : 0"), P_COND, S("int"));
        sk = S("i32");
    }
    str tj = p->jt(t);
    if (in(sk, {"ptr", "fnptr", "array", "null", "class", "record"})) sk = S("i32");   // an address is an int
    if (in(tk, {"ptr", "fnptr", "array", "class", "record"})) tk = S("i32");
    if (sk == tk) return j.jt == tj ? j : retype(j, tj);
    if (in(tk, {"f32", "f64", "f80"})) {
        str fj = is(tk, "f32") ? S("float") : S("double");
        if (in(sk, {"f32", "f64", "f80"})) {
            if ((in(_kind(s), {"f64", "f80"}) && is(fj, "double")) || sk == tk) return j;
            return J(cat("(", fj, ") ", j.at(P_UNARY)), P_UNARY, fj);
        }
        if (is(sk, "u32")) return J(cat("(", fj, ") Integer.toUnsignedLong(", j.code, ")"), P_UNARY, fj);
        if (is(sk, "u64"))
            return J(cat(is(fj, "float") ? "(float) " : "", "unsignedToDouble(", j.code, ")"), is(fj, "float") ? P_UNARY : P_POST, fj);
        if (is(sk, "u8")) return J(cat("(", fj, ") (", j.code, " & 0xFF)"), P_UNARY, fj);
        return J(cat("(", fj, ") ", j.at(P_UNARY)), P_UNARY, fj);
    }
    // to an integer
    if (in(sk, {"f32", "f64", "f80"})) {
        if (is(tk, "u32")) return J(S("(int) (long) ") + j.at(P_UNARY), P_UNARY, S("int"));
        if (is(tk, "u64")) return J(cat("doubleToUnsigned(", j.code, ")"), P_POST, S("long"));
        if (is(tk, "i64")) return J(S("(long) ") + j.at(P_UNARY), P_UNARY, S("long"));
        if (is(tk, "i32")) return J(S("(int) ") + j.at(P_UNARY), P_UNARY, S("int"));
        return J(cat("(", tj, ") (int) ", j.at(P_UNARY)), P_UNARY, tj);
    }
    int64_t sb = _BITS().at(sk), tb = _BITS().at(tk);
    if (tb <= sb) {                                       // narrowing or the same width: the low bits
        if (tj == j.jt) return J(j.code, j.prec, tj);
        if (in(j.jt, {"byte", "short", "char"}) && is(tj, "int") && tb == sb) return widen(j, sk, tj);
        return J(cat("(", tj, ") ", j.at(P_UNARY)), P_UNARY, tj);
    }
    // widening: zero-extended when the source is unsigned
    J w = widen(j, sk, tb == 64 ? S("long") : S("int"));
    if (in(tj, {"byte", "short", "char"}) && w.jt != tj) return J(cat("(", tj, ") ", w.at(P_UNARY)), P_UNARY, tj);
    return w;
}

J FnWriter::widen(const J& j, const str& sk, const str& to) {
    // A value of C kind sk as a Java int or long of the same value.
    if (is(sk, "u8")) {
        if (_is_int_text(j.code)) {
            py::list<str> parts = py::split(j.code.strip(S("()")), S(")"));
            return J(cat(py::int_parse(parts[-1], 10) & 0xFF), P_ATOM, to);
        }
        J jj(cat(j.at(P_BAND), " & 0xFF"), P_BAND, S("int"));
        return is(to, "int") ? jj : J(cat("(long) (", jj.code, ")"), P_UNARY, S("long"));
    }
    if (is(sk, "u32") && is(to, "long")) return J(cat("Integer.toUnsignedLong(", j.code, ")"), P_POST, S("long"));
    if (is(sk, "u16")) return is(to, "int") ? J(j.code, j.prec, to) : J(S("(long) ") + j.at(P_UNARY), P_UNARY, S("long"));
    if (is(to, "long") && !is(j.jt, "long")) {
        if (_is_int_text(j.code)) return int_lit(py::int_parse(j.code.strip(S("()")), 10), S("long"));
        return J(S("(long) ") + j.at(P_UNARY), P_UNARY, S("long"));
    }
    return J(j.code, j.prec, in(j.jt, {"byte", "short", "int", "char"}) ? to : j.jt);
}

J FnWriter::retype(const J& j, const str& tj) {
    if (j.jt == tj) return j;
    if (in(j.jt, {"int", "long"}) && in(tj, {"byte", "short", "char"})) return J(cat("(", tj, ") ", j.at(P_UNARY)), P_UNARY, tj);
    if (is(j.jt, "char") && is(tj, "int")) return J(j.code, j.prec, S("int"));
    return J(j.code, j.prec, tj);
}

J FnWriter::x_Call(const py::ref<ir::Call>& e0) {
    py::ref<ir::Call> e = e0;
    py::list<NodeP> args = lst_or_empty(e->args);
    if (e->sym && !e->target) {
        str name = e->sym->name;
        if (p->functions.contains(name)) {
            MethodP f = p->function_for(e->sym);
            return own_call(f, jname(name), args, e);
        }
        return lib_call(e, name, args);
    }
    NodeP target = e->target ? strip(e->target) : NodeP();
    while (true) {
        auto u = as_a<ir::Unary>(target);
        if (!(u && opt_is(u->op, "*") && type_of(u->x) && is(type_of(u->x)->kind, "fnptr"))) break;
        target = strip(u->x);                       // (*fp)(...) calls fp
    }
    if (target && type_of(target) && is(type_of(target)->kind, "fn")) {
        auto cn = std::make_shared<CodeNode>();
        cn->j = value(target);
        ir::Dict info;
        info.set(S("space"), Val(S("flat")));
        info.set(S("fn"), Val(type_of(target)));
        cn->type = std::make_shared<ir::Type>(S("fnptr"), std::nullopt, py::list<TypeP>(), nullptr, std::nullopt, info);
        target = cn;
    }
    if (target && type_of(target) && is(type_of(target)->kind, "fnptr")) {
        auto ne = std::make_shared<ir::Call>();
        ne->target = target;
        ne->sym = nullptr;
        ne->args = args;
        ne->typeargs = py::list<TypeP>();
        ne->type = e->type;
        e = ne;
        TypeP ft = type_of(target);
        TypeP fnt = val_type(ft->info.at(S("fn")));
        std::vector<TypeP> params = fn_params(fnt);
        if (params.empty() && !args.empty()) {          // a K&R pointer `int (*f)()`: typed by the call
            params.clear();
            for (const NodeP& a : args) params.push_back(type_of(a));
            Val::List pl;
            for (const TypeP& pt : params) {
                Val::Dict d;
                d.set(S("type"), Val(pt));
                pl.append(Val(d));
            }
            ir::Dict finfo;
            finfo.set(S("conv"), Val(S("cdecl")));
            finfo.set(S("params"), Val(pl));
            finfo.set(S("result"), fnt->info.at(S("result")));
            TypeP nfn = std::make_shared<ir::Type>(S("fn"), std::nullopt, py::list<TypeP>(), nullptr, std::nullopt, finfo);
            ir::Dict pinfo;
            pinfo.set(S("space"), Val(S("flat")));
            pinfo.set(S("fn"), Val(nfn));
            ft = std::make_shared<ir::Type>(S("fnptr"), std::nullopt, py::list<TypeP>(), nullptr, std::nullopt, pinfo);
        }
        Sig sig = p->fn_sig_of_type(ft);
        str iface_ = p->iface(sig);
        J fn_ = value(e->target);
        Lines ac = this->args(args, params, fn_result(val_type(ft->info.at(S("fn")))));
        return J(cat("((", iface_, ") Fn.at(", fn_.code, ")).call(", join_sep(", ", ac), ")"), P_POST, sig.ret);
    }
    throw Refused("a call this back end cannot place");
}

Lines FnWriter::args(const py::list<NodeP>& args, const std::vector<TypeP>& params, const TypeP& ret) {
    Lines out;
    if (ret && is_record(ret)) out.append(temp(ret));
    for (int64_t i = 0; i < args.size(); ++i) {
        const NodeP& a = args[i];
        TypeP pt = i < static_cast<int64_t>(params.size()) ? params[i] : type_of(a);
        if (is_record(pt)) {
            J v = value(a);
            if (v.fresh) out.append(v.code);
            else out.append(cat("Mem.copy(", temp(pt), ", ", v.code, ", ", p->size_code(pt), ")"));
        } else {
            out.append(conv(value(a), type_of(a), pt).code);
        }
    }
    return out;
}

J FnWriter::own_call(const MethodP& f, const str& jn, const py::list<NodeP>& args_, const py::ref<ir::Call>&) {
    SymP s = f->sym;
    // the definition's own parameter types (a K&R definition's symbol has no prototype)
    std::vector<TypeP> ptypes;
    for (const NodeP& pn : lst(f->params)) ptypes.push_back(as_a<ir::Param>(pn)->type);
    Lines ac = args(args_, ptypes, s->ret);
    str jtt = s->ret ? p->jt(s->ret) : S("void");
    return J(cat(jn, "(", join_sep(", ", ac), ")"), P_POST, jtt, s->ret && is_record(s->ret));
}

J FnWriter::lib_call(const py::ref<ir::Call>& e, const str& name, const py::list<NodeP>& args0) {
    py::list<NodeP> args = args0;
    if (starts(name, "__builtin_va_") && !args.empty()) {
        NodeP a0 = strip(args[0]);            // (the builtins take their va_list by reference)
        auto u = as_a<ir::Unary>(a0);
        if (u && opt_is(u->op, "&") && u->type && val_truthy(val_get(u->type->info, "ref"))) {
            py::list<NodeP> na{u->x};
            na.extend(args.slice_from(1));
            args = na;
        }
    }
    if (is(name, "__builtin_va_start")) return store(place(args[0]), J(S("vaList"), P_ATOM, S("int")));
    if (is(name, "__builtin_va_end")) return J(S("discard(0)"), P_POST, S("void"));
    if (is(name, "__builtin_va_copy")) {
        Place pl = place(args[0]);
        return store(pl, value(args[1]));
    }
    if (is(name, "__builtin_va_arg")) {
        TypeP t = e->type;
        if (is_record(t) || is(_kind(t), "f32")) throw Refused(S("va_arg of ") + (t ? t->repr() : S("None")));
        Place pl = place(args[0]);
        str slot = !pl.mem ? cat("(", pl.java, " += 8) - 8") : cat("Mem.postAddPtr(", pl.java, ", 8)");
        return J(cat("Mem.get", p->access(t), "(", slot, ")"), P_POST, p->jt(t));
    }
    return java_libc::call(*this, e, name, args);
}

J FnWriter::x_New(const py::ref<ir::New>& e) {
    if (e->memory && is(*e->memory, "stack") && is_record(e->type)) {
        str tmp_ = temp(e->type);
        Lines lines_ = init_place(tmp_, e->type, e);
        // an aggregate value in an expression: written into a temporary in the frame first
        return J(seq_lines(lines_, tmp_), P_POST, S("int"), true);
    }
    throw Refused(cat("new (", or_none(e->memory), ")"));
}

str FnWriter::seq_lines(const Lines& lines_, const str& result) {
    py::list<str> calls;
    for (const str& l : lines_) calls.append(l.slice(0, l.size() - 1));
    if (calls.empty()) return result;
    if (calls.size() == 1) return cat("seq(", join_sep(", ", calls), ", ", result, ")");
    calls.append(result);
    return cat("seq(", join_sep(", ", calls), ")");
}

J FnWriter::x_NewArray(const py::ref<ir::NewArray>&) { throw Refused("an array value outside an initialiser"); }

}  // namespace codebreaker::translate::backends::java

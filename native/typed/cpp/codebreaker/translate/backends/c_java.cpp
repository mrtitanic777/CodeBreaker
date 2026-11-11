// codebreaker/translate/backends/c_java.cpp -- typed C++ port of codebreaker.translate.backends.c_java
// (see c_java.hpp).
#include "codebreaker/translate/backends/c_java.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "codebreaker/translate/desugar.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::translate::backends::c_java {

namespace desugar = codebreaker::translate::desugar;

using ir::NodeOrList;
using ir::NodeP;
using ir::Nodes;
using ir::SymP;
using ir::TypeP;
using py::str;

namespace {

str S(const char* s) { return str(s); }

[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

// the list a Nodes slot holds (TypeError for None, as iterating None is)
const py::list<NodeP>& lst(const Nodes& v) {
    if (!v) throw py::TypeError("'NoneType' object is not iterable");
    return *v;
}
int64_t plen(const Nodes& v) {   // len(x.params)
    if (!v) throw py::TypeError("object of type 'NoneType' has no len()");
    return py::len(*v);
}
bool nonempty(const Nodes& v) { return v && !v->empty(); }   // `x.args` / `not x.args` as a truth value

// `x.kind` of a missing object
const ir::Type& K(const TypeP& t) {
    if (!t) none_attr("kind");
    return *t;
}
ir::Sym& need_sym(const SymP& s) {
    if (!s) none_attr("name");
    return *s;
}

bool eq(const std::optional<str>& a, const char* b) { return a && *a == b; }
bool truthy(const std::optional<str>& a) { return a && !a->empty(); }
bool in2(const std::optional<str>& a, const char* x, const char* y) { return a && (*a == x || *a == y); }
bool in3(const std::optional<str>& a, const char* x, const char* y, const char* z) {
    return a && (*a == x || *a == y || *a == z);
}
str or_empty(const std::optional<str>& a) { return a ? *a : str(); }
str or_none(const std::optional<str>& a) { return a ? *a : S("None"); }   // '%s' % None
bool has(const py::list<str>& xs, const str& x) { return xs.contains(x); }

// the node a body slot holds; a bare list of statements is what block()/stmt() decline
NodeP one(const NodeOrList& v) {
    if (std::holds_alternative<py::list<NodeP>>(v)) throw Decline(S("statement list"));
    return std::get<NodeP>(v);
}
NodeP body_node(const NodeOrList& v) {   // `x.body` read as a node: a list has no such attributes
    if (std::holds_alternative<py::list<NodeP>>(v))
        throw py::AttributeError("'list' object has no attribute 'stmts'");
    return std::get<NodeP>(v);
}
bool has_body(const py::ref<ir::MethodDecl>& m) {   // `m.body is not None`
    return std::holds_alternative<py::list<NodeP>>(m->body) || std::get<NodeP>(m->body) != nullptr;
}
py::list<NodeP> walk_of(const NodeOrList& v) {   // ir.walk(x): a list has no children()
    if (std::holds_alternative<py::list<NodeP>>(v)) throw py::AttributeError("'list' object has no attribute 'children'");
    return ir::walk(std::get<NodeP>(v));
}

// `e.type` of an expression (AttributeError for a node that is not one)
TypeP type_of(const NodeP& e) {
    if (!e) none_attr("type");
    if (auto x = as_a<ir::Expr>(e)) return x->type;
    if (auto l = as_a<ir::Local>(e)) return l->type;
    throw py::AttributeError("'" + e->class_name().utf8() + "' object has no attribute 'type'");
}

int64_t id_of(const NodeP& e) { return static_cast<int64_t>(reinterpret_cast<intptr_t>(e.get())); }

// s.rsplit('.', 1)[-1]
str after_dot(const str& s) {
    const std::u32string& u = s.u32();
    size_t at = u.rfind(U'.');
    return str(at == std::u32string::npos ? u : u.substr(at + 1));
}

// s.replace(a, b, 1)
str replace_first(const str& s, const str& a, const str& b) {
    const std::u32string& u = s.u32();
    size_t at = u.find(a.u32());
    if (at == std::u32string::npos) return s;
    return str(u.substr(0, at) + b.u32() + u.substr(at + a.u32().size()));
}

str jn(const py::list<str>& parts) { return py::join(S(", "), parts); }

// an int (or a bool, which is one) held by a literal
std::optional<int64_t> int_of(const ir::Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v);
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v) ? 1 : 0;
    return std::nullopt;
}

// a Python truth value of a literal
bool val_truthy(const ir::Val& v) {
    if (std::holds_alternative<std::monostate>(v.v)) return false;
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v);
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v) != 0;
    if (std::holds_alternative<double>(v.v)) return std::get<double>(v.v) != 0.0;
    if (std::holds_alternative<str>(v.v)) return !std::get<str>(v.v).empty();
    return true;
}
// str(v) of a literal's value
str val_str(const ir::Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return py::to_str(std::get<int64_t>(v.v));
    if (std::holds_alternative<bool>(v.v)) return py::to_str(std::get<bool>(v.v));
    if (std::holds_alternative<str>(v.v)) return std::get<str>(v.v);
    return v.repr();
}

// ---- tables
const py::dict<str, str>& PRIM() {
    static const py::dict<str, str> d = {{S("bool"), S("bool")},     {S("i8"), S("int8_t")},    {S("i16"), S("int16_t")},
                                         {S("u16"), S("char16_t")},  {S("i32"), S("int32_t")},  {S("i64"), S("int64_t")},
                                         {S("f32"), S("float")},     {S("f64"), S("double")},   {S("void"), S("void")}};
    return d;
}
const py::dict<str, str>& CODE() {
    static const py::dict<str, str> d = {{S("bool"), S("Z")}, {S("i8"), S("B")},  {S("i16"), S("S")}, {S("u16"), S("C")},
                                         {S("i32"), S("I")},  {S("i64"), S("J")}, {S("f32"), S("F")}, {S("f64"), S("D")}};
    return d;
}
bool is_numeric(const str& c) {
    return c == "int8_t" || c == "int16_t" || c == "char16_t" || c == "int32_t" || c == "int64_t" || c == "float" ||
           c == "double";
}
bool is_integral(const str& c) {
    return c == "int8_t" || c == "int16_t" || c == "char16_t" || c == "int32_t" || c == "int64_t";
}
bool is_prim_c(const str& c) { return is_numeric(c) || c == "bool"; }   // _NUMERIC + ('bool',)
const py::dict<str, str>& BOX() {
    static const py::dict<str, str> d = {{S("java.lang.Integer"), S("I")},   {S("java.lang.Long"), S("J")},
                                         {S("java.lang.Short"), S("S")},     {S("java.lang.Byte"), S("B")},
                                         {S("java.lang.Character"), S("C")}, {S("java.lang.Boolean"), S("Z")},
                                         {S("java.lang.Float"), S("F")},     {S("java.lang.Double"), S("D")}};
    return d;
}
const py::dict<str, str>& CT_OF_CODE() {
    static const py::dict<str, str> d = {{S("Z"), S("bool")},     {S("B"), S("int8_t")},  {S("S"), S("int16_t")},
                                         {S("C"), S("char16_t")}, {S("I"), S("int32_t")}, {S("J"), S("int64_t")},
                                         {S("F"), S("float")},    {S("D"), S("double")}};
    return d;
}
// {C type: letter}, the reverse of CT_OF_CODE
const py::dict<str, str>& CODES() {
    static const py::dict<str, str> d = [] {
        py::dict<str, str> out;
        for (const auto& kv : CT_OF_CODE().items()) out.set(std::get<1>(kv), std::get<0>(kv));
        return out;
    }();
    return d;
}
const str OBJ = S("jobject*");
const str STR = S("jstring*");
const str ARR = S("jarray*");
const str SB = S("jrt_StringBuilder*");

// _STATIC_FIELDS: (owner, name) -> (C text, C type); keyed owner + \x01 + name
const py::dict<str, Code>& STATIC_FIELDS() {
    static const py::dict<str, Code> d = [] {
        py::dict<str, Code> m;
        auto put = [&](const char* owner, const char* name, const char* code, const char* ct) {
            m.set(S(owner) + "\x01" + name, Code(S(code), S(ct)));
        };
        put("java.lang.Integer", "MAX_VALUE", "JRT_INT_MAX", "int32_t");
        put("java.lang.Integer", "MIN_VALUE", "JRT_INT_MIN", "int32_t");
        put("java.lang.Long", "MAX_VALUE", "JRT_LONG_MAX", "int64_t");
        put("java.lang.Long", "MIN_VALUE", "JRT_LONG_MIN", "int64_t");
        put("java.lang.Math", "PI", "JRT_PI", "double");
        put("java.lang.Math", "E", "JRT_E", "double");
        put("java.lang.Double", "NaN", "NAN", "double");
        put("java.lang.Double", "POSITIVE_INFINITY", "INFINITY", "double");
        put("java.lang.Double", "NEGATIVE_INFINITY", "(-INFINITY)", "double");
        put("java.lang.System", "out", "jrt_System_out", "void*");
        put("java.lang.System", "err", "jrt_System_err", "void*");
        return m;
    }();
    return d;
}

const py::set<str>& KEYWORDS() {
    static const py::set<str> s = [] {
        py::set<str> out;
        for (const str& w : py::split(S(R"KW(auto break case char const continue default do double else enum
extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void
volatile while bool true false self self_ NULL errno stdin stdout stderr EOF jrt)KW")))
            out.add(w);
        return out;
    }();
    return s;
}

// _THROWABLES = set(q for q in jmodel.LIB_SUPERS if q.endswith(('Exception', 'Error')))
const py::set<str>& THROWABLES() {
    static const py::set<str> s = [] {
        py::set<str> out;
        for (const str& q : jmodel::LIB_SUPERS)
            if (q.endswith(S("Exception")) || q.endswith(S("Error"))) out.add(q);
        return out;
    }();
    return s;
}

// the operators whose operands are parenthesised when they mix (cpp03's _BITS), and a name or member chain
// (cpp03's _SIMPLE)
bool is_bits_op(const std::optional<str>& op) {
    return op && (*op == "<<" || *op == ">>" || *op == "&" || *op == "|" || *op == "^");
}
const py::re::Pattern& SIMPLE() {
    static const py::re::Pattern rx = py::re::compile(S("^[A-Za-z_][\\w]*(?:(?:->|\\.)[A-Za-z_]\\w*)*$"));
    return rx;
}

str _ident(const str& n) {
    return KEYWORDS().contains(n) ? n + "_" : n.replace(S("$"), S("_"));
}

// (C char16_t literal text, length) of a Java string.
std::tuple<str, int64_t> _u16(const str& v) {
    py::list<str> out;
    bool prev_hex = false;
    for (char32_t c : v.u32()) {
        int64_t o = static_cast<int64_t>(c);
        str ch(c);
        if (c == U'\\' || c == U'"') {
            out.append(S("\\") + ch);
            prev_hex = false;
        } else if (c == U'\n') {
            out.append(S("\\n"));
            prev_hex = false;
        } else if (32 <= o && o < 127) {
            bool hexdigit = (c >= U'0' && c <= U'9') || (c >= U'a' && c <= U'f') || (c >= U'A' && c <= U'F');
            out.append((prev_hex && hexdigit) ? S("\" u\"") + ch : ch);
            prev_hex = false;
        } else {
            out.append(py::percent(S("\\x%04x"), o));
            prev_hex = true;
        }
    }
    return {py::percent(S("u\"%s\""), py::join(S(""), out)), v.size()};
}

// A Java type's letter(s) in a runtime function's name: I J ... Str, O, and arrays as the element's + A.
str _code(const TypeP& t) {
    if (!t) return S("");
    if (CODE().contains(t->kind)) return CODE().at(t->kind);
    if (t->kind == "array") {
        const TypeP& e = t->elem;
        return (e ? (CODE().contains(e->kind) ? CODE().at(e->kind) : S("O")) : S("O")) + "A";
    }
    if (t->kind == "class" && eq(t->name, "java.lang.String")) return S("Str");
    return S("O");
}

py::ref<ir::Param> prm(const NodeP& n) {
    auto p = as_a<ir::Param>(n);
    if (!p) throw py::AttributeError("'" + (n ? n->class_name().utf8() : std::string("NoneType")) + "' object has no attribute 'sym'");
    return p;
}

TypeP mk_type(const char* kind, const std::optional<str>& name = std::nullopt) {
    return std::make_shared<ir::Type>(S(kind), name);
}

// m.sym is not None and m.sym.static
bool sym_static(const SymP& s) { return s && s->static_; }

// `'private' in (m.mods or [])`
bool is_private(const py::ref<ir::MethodDecl>& m) { return m->mods && m->mods->contains(S("private")); }

// m.name in ('toString', 'equals', 'hashCode') and len(m.params) == (1 if equals else 0)
bool object_method_decl(const py::ref<ir::MethodDecl>& m) {
    if (!(in3(m->name, "toString", "equals", "hashCode"))) return false;
    return plen(m->params) == (eq(m->name, "equals") ? 1 : 0);
}

str g_here = S("codebreaker/translate/backends");

}  // namespace

// ---------------------------------------------------------------------------------------------- module
void set_here(const str& here) { g_here = here; }

str RUNTIME() {
    // os.path.join(dirname(dirname(abspath(__file__))), 'runtime', 'c'); g_here is already the file's folder
    return py::os::path::join(py::os::path::join(py::os::path::dirname(g_here), S("runtime")), S("c"));
}

const str CLANG("C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Tools\\Llvm\\x64\\bin\\clang.exe");
const py::list<str> FLAGS = {S("-std=c11"), S("-fwrapv"), S("-O1"), S("-w")};

// ---------------------------------------------------------------------------------------------- Program
Program::Program(const py::list<NodeP>& units_, const std::optional<str>& main_) : units(units_), main(main_) {
    desugar::run(units);
    py::list<py::ref<ir::Unit>> us;
    for (const NodeP& n : units) us.append(std::dynamic_pointer_cast<ir::Unit>(n));
    model = std::make_shared<jmodel::Model>(us);
    model->name_anonymous();
    py::dict<str, py::set<str>> sigs;
    for (const auto& ci : model->classes.values()) {
        for (const auto& m : ci->methods()) {
            if (eq(m->kind, "method")) {
                py::list<str> codes;
                for (const NodeP& pn : lst(m->params)) codes.append(_code(jmodel::erase(prm(pn)->type)));
                sigs.setdefault(or_empty(m->name), py::set<str>()).add(jn(codes));
            }
        }
    }
    for (const auto& kv : sigs.items()) {
        if (std::get<1>(kv).size() > 1) overloaded.add(std::get<0>(kv));
    }
}

// ---- names and types
str Program::cname(const str& qname) {
    auto ci = model->info(qname);
    if (!ci) throw Decline(S("not a class of the program: ") + qname);
    return ci->cname;
}

str Program::mname(const str& name, const py::list<TypeP>& params) {
    str base = _ident(name);
    if (overloaded.contains(name)) {
        py::list<str> codes;
        for (const TypeP& p : params) codes.append(_code(jmodel::erase(p)));
        return base + "_" + py::join(S(""), codes);
    }
    return base;
}

str Program::ct(const TypeP& t) {
    if (!t) return S("void");
    if (PRIM().contains(t->kind)) return PRIM().at(t->kind);
    if (t->kind == "null") return S("nullptr_t");
    if (t->kind == "typevar" || t->kind == "wildcard" || t->kind == "any") return OBJ;
    if (t->kind == "array") return ARR;
    if (t->kind == "class") {
        if (eq(t->name, "java.lang.String")) return STR;
        if (in2(t->name, "java.lang.StringBuilder", "java.lang.StringBuffer")) return SB;
        auto ci = model->info(or_empty(t->name));
        if (ci) return eq(ci->kind, "interface") ? OBJ : ci->cname + "*";
        if (eq(t->name, "java.io.PrintStream")) return S("void*");
        return OBJ;
    }
    throw Decline(S("type ") + t->repr());
}

str Program::class_ref(const str& qname) {
    auto ci = model->info(qname);
    if (ci) return py::percent(S("&%s_class"), ci->cname);
    if (qname == "java.lang.String") return S("&jrt_String_class");
    std::optional<str> name = jmodel::LIB.contains(qname) ? std::optional<str>(jmodel::LIB.at(qname)) : std::nullopt;
    if ((!name || *name == "Object") && qname != jmodel::OBJECT) {
        if (qname == "java.lang.CharSequence") return S("&jrt_CharSequence_class");
        throw Decline(S("runtime class ") + qname);
    }
    if (qname == "java.util.ArrayList" || qname == "java.util.List" || qname == "java.util.LinkedList")
        return S("&jrt_ArrayList_class");
    if (qname == "java.util.HashMap" || qname == "java.util.Map" || qname == "java.util.LinkedHashMap" ||
        qname == "java.util.TreeMap")
        return S("&jrt_HashMap_class");
    if (qname == "java.util.Map.Entry") return S("&jrt_node_class");
    return py::percent(S("&jrt_%s_class"), or_none(name));
}

std::tuple<py::list<str>, str, py::list<TypeP>> Program::sig(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m) {
    auto root = model->overrides(ci, m);
    py::list<TypeP> params;
    if (root) {
        params = std::get<0>(*root);
    } else {
        for (const NodeP& pn : lst(m->params)) params.append(prm(pn)->type);
    }
    TypeP ret = root ? std::get<1>(*root) : m->ret;
    py::list<str> cts;
    py::list<TypeP> eps;
    for (const TypeP& p : params) cts.append(ct(jmodel::erase(p)));
    for (const TypeP& p : params) eps.append(jmodel::erase(p));
    str r = eq(m->kind, "method") ? ct(jmodel::erase(ret)) : S("void");
    return {cts, r, eps};
}

std::optional<str> Program::base_struct(const ClassInfoP& ci) {
    if (eq(ci->kind, "enum")) return S("jrt_Enum");
    if (truthy(ci->base) && *ci->base != jmodel::OBJECT) {
        auto b = model->info(*ci->base);
        if (b) return b->cname;
        if (THROWABLES().contains(*ci->base) || *ci->base == "java.lang.Throwable") return S("jrt_Throwable");
        if (*ci->base == "java.lang.Enum") return S("jrt_Enum");
        throw Decline(S("extends the runtime class ") + *ci->base);
    }
    return std::nullopt;
}

str Program::base_class_ref(const ClassInfoP& ci) {
    if (eq(ci->kind, "enum")) return S("&jrt_Enum_class");
    if (truthy(ci->base) && *ci->base != jmodel::OBJECT) return class_ref(*ci->base);
    return S("&jrt_Object_class");
}

bool Program::is_throwable(const ClassInfoP& ci) {
    std::optional<str> q = ci->base;
    while (truthy(q)) {
        if (THROWABLES().contains(*q) || *q == "java.lang.Throwable") return true;
        auto b = model->info(*q);
        q = b ? b->base : std::nullopt;
    }
    return false;
}

str Program::lit(const str& v) {
    if (!lits.contains(v)) lits.set(v, py::percent(S("lit_%d"), lits.size() + 1));
    auto [text, n] = _u16(v);
    return py::percent(S("jrt_lit(&%s, %s, %d)"), lits.at(v), text, n);
}

bool Program::needs_clinit(const ClassInfoP& ci) {
    for (const NodeP& x : lst(ci->members())) {
        if (auto md = as_a<ir::MethodDecl>(x)) {
            if (eq(md->kind, "static_init")) return true;
        }
        if (auto fd = as_a<ir::FieldDecl>(x)) {
            if (fd->sym && fd->sym->static_ && fd->init && !jmodel::constant_init(fd)) return true;
        }
        if (is_a<ir::EnumConst>(x)) return true;
    }
    return false;
}

// ---- the table of virtual slots
py::list<Slot> Program::slots(const ClassInfoP& ci) {
    auto cached = slots_cache.find(ci.get());
    if (cached != slots_cache.end()) return cached->second;
    py::list<Slot> out;
    auto b = truthy(ci->base) ? model->info(*ci->base) : ClassInfoP();
    if (b && !eq(b->kind, "interface")) {
        for (const Slot& s : slots(b)) out.append(s);   // a copy of each
    } else if (!eq(ci->kind, "interface") && is_throwable(ci)) {
        out = py::list<Slot>{Slot{S("getMessage"), py::list<str>(), STR, S("jrt_Throwable_getMessage")}};
    }
    for (const auto& m : ci->methods()) {
        if (!eq(m->kind, "method") || sym_static(m->sym) || is_private(m)) continue;
        if (in3(m->name, "toString", "equals", "hashCode") && plen(m->params) == (eq(m->name, "equals") ? 1 : 0))
            continue;   // Object's slots, in the jobject_vt part
        auto [ps, r, eps] = sig(ci, m);
        str name = mname(or_empty(m->name), eps);
        str impl = has_body(m) ? py::percent(S("%s_%s"), ci->cname, name) : S("0");
        bool found = false;
        for (int64_t i = 0; i < out.size(); ++i) {
            if (out[i].name == name) {
                if (has_body(m)) out[i].impl = impl;
                found = true;
                break;
            }
        }
        if (!found) out.append(Slot{name, ps, r, impl});
    }
    slots_cache[ci.get()] = out;
    return out;
}

py::dict<str, str> Program::object_slots(const ClassInfoP& ci) {
    py::dict<str, str> impl = {{S("toString"), S("jrt_Object_toString")},
                               {S("equals"), S("jrt_Object_equals")},
                               {S("hashCode"), S("jrt_Object_hashCode")}};
    py::list<ClassInfoP> chain;
    std::optional<str> q = ci->qname;
    while (truthy(q)) {
        auto c = model->info(*q);
        if (!c) {
            if (THROWABLES().contains(*q) || *q == "java.lang.Throwable") impl["toString"] = S("jrt_Throwable_toString");
            if (*q == "java.lang.Enum") impl["toString"] = S("jrt_Enum_toString");
            break;
        }
        chain.append(c);
        q = !eq(c->kind, "enum") ? c->base : std::optional<str>(S("java.lang.Enum"));
    }
    for (int64_t k = chain.size() - 1; k >= 0; --k) {
        const ClassInfoP& c = chain[k];
        for (const auto& m : c->methods()) {
            if (m->name && impl.contains(*m->name) && has_body(m) && eq(m->kind, "method") &&
                plen(m->params) == (eq(m->name, "equals") ? 1 : 0))
                impl[*m->name] = py::percent(S("%s_%s"), c->cname, *m->name);
        }
    }
    return impl;
}

std::vector<IMethod> Program::iface_methods(const str& qname) {
    auto ci = model->info(qname);
    if (!ci) {
        if (jmodel::SAMS.contains(qname)) {
            auto [name, ps, r] = jmodel::SAMS.at(qname);
            py::list<str> cps;
            for (const TypeP& p : ps) cps.append(ct(jmodel::erase(p)));
            return {IMethod{name, cps, ct(jmodel::erase(r)), name, ps}};
        }
        if (qname == "java.lang.Iterable") return {IMethod{S("iterator"), py::list<str>(), OBJ, S("iterator"), py::list<TypeP>()}};
        if (qname == "java.util.Iterator")
            return {IMethod{S("hasNext"), py::list<str>(), S("bool"), S("hasNext"), py::list<TypeP>()},
                    IMethod{S("next"), py::list<str>(), OBJ, S("next"), py::list<TypeP>()}};
        if (qname == "java.lang.Comparable")
            return {IMethod{S("compareTo"), py::list<str>{OBJ}, S("int32_t"), S("compareTo"),
                            py::list<TypeP>{mk_type("typevar", S("T"))}}};
        if (qname == "java.lang.CharSequence") return {};
        throw Decline(S("interface ") + qname);
    }
    std::vector<IMethod> out;
    auto known = [&](const str& n) {
        for (const IMethod& y : out)
            if (y.name == n) return true;
        return false;
    };
    for (const str& q : ci->ifaces) {
        for (const IMethod& x : iface_methods(q)) {
            if (!known(x.name)) out.push_back(x);
        }
    }
    for (const auto& m : ci->methods()) {
        if (eq(m->kind, "method") && !sym_static(m->sym)) {
            if (in3(m->name, "toString", "equals", "hashCode")) continue;
            auto [ps, r, eps] = sig(ci, m);
            str name = mname(or_empty(m->name), eps);
            if (!known(name)) out.push_back(IMethod{name, ps, r, or_empty(m->name), eps});
        }
    }
    return out;
}

py::list<str> Program::all_ifaces(const ClassInfoP& ci) {
    py::list<str> out;
    for (const str& q : model->ancestors(ci->qname).slice_from(1)) {
        if (model->is_interface(q) && !out.contains(q) &&
            (model->info(q) || jmodel::SAMS.contains(q) || q == "java.lang.Iterable" || q == "java.util.Iterator" ||
             q == "java.lang.Comparable"))
            out.append(q);
    }
    return out;
}

str Program::vt_name(const str& qname) {
    auto ci = model->info(qname);
    if (ci) return py::percent(S("struct %s_vt"), ci->cname);
    return py::percent(S("jrt_%s_vt"), after_dot(qname));
}

std::optional<str> Program::find_impl(const ClassInfoP& ci, const str& java_name, int64_t n_eps) {
    std::optional<str> q = ci->qname;
    while (truthy(q)) {
        auto c = model->info(*q);
        if (!c) break;
        for (const auto& m : c->methods()) {
            if (eq(m->kind, "method") && eq(m->name, java_name.utf8().c_str()) && has_body(m) && plen(m->params) == n_eps &&
                !sym_static(m->sym)) {
                auto [_ps, _r, meps] = sig(c, m);
                return py::percent(S("%s_%s"), c->cname, mname(or_empty(m->name), meps));
            }
        }
        q = c->base;
    }
    for (const str& qa : model->ancestors(ci->qname)) {   // a default method of an interface
        auto c = model->info(qa);
        if (!c || !eq(c->kind, "interface")) continue;
        for (const auto& m : c->methods()) {
            if (eq(m->kind, "method") && eq(m->name, java_name.utf8().c_str()) && has_body(m) && plen(m->params) == n_eps &&
                !sym_static(m->sym)) {
                auto [_ps, _r, meps] = sig(c, m);
                return py::percent(S("%s_%s"), c->cname, mname(or_empty(m->name), meps));
            }
        }
    }
    return std::nullopt;
}

// ---- the program
py::list<ClassInfoP> Program::ordered() {
    py::list<ClassInfoP> out;
    py::set<str> seen;
    std::function<void(const ClassInfoP&)> visit = [&](const ClassInfoP& ci) {
        if (seen.contains(ci->qname)) return;
        seen.add(ci->qname);
        py::list<str> sup;
        if (truthy(ci->base)) sup.append(*ci->base);
        for (const str& i : ci->ifaces) sup.append(i);
        for (const str& q : sup) {
            auto b = model->info(q);
            if (b) visit(b);
        }
        out.append(ci);
    };
    std::vector<ClassInfoP> all = model->classes.values();
    std::stable_sort(all.begin(), all.end(), [](const ClassInfoP& a, const ClassInfoP& b) {
        if (a->decl->line != b->decl->line) return a->decl->line < b->decl->line;
        return a->cname < b->cname;
    });
    for (const auto& ci : all) visit(ci);
    return out;
}

str Program::write() {
    auto order = ordered();
    py::list<str> structs, protos, tables_, defs;
    for (const auto& ci : order) structs.extend(struct_(ci));
    for (const auto& ci : order) protos.extend(prototypes(ci));
    for (const auto& ci : order) defs.extend(definitions(ci));
    for (const auto& ci : order) tables_.extend(tables(ci));
    py::list<str> head = {S("/* written by CodeBreaker's C back end for Java (codebreaker/translate/backends/c_java.py) from"),
                          S("   the program's L2; it runs on jrt.h / jrt.c, the Java library in C. */"), S("#include \"jrt.h\""),
                          S("")};
    for (const auto& ci : order)
        if (!eq(ci->kind, "interface")) head.append(py::percent(S("typedef struct %s %s;"), ci->cname, ci->cname));
    for (const auto& ci : order) head.append(py::percent(S("extern const jclass %s_class;"), ci->cname));
    head.append(S(""));
    py::list<str> lits_;
    for (const str& slot : lits.values()) lits_.append(py::percent(S("static jstring* %s;"), slot));
    py::list<ClassInfoP> mains;
    for (const auto& ci : order) {
        bool any = false;
        for (const auto& m : ci->methods()) {
            if (eq(m->name, "main") && sym_static(m->sym) && plen(m->params) == 1) {
                any = true;
                break;
            }
        }
        if (any) mains.append(ci);
    }
    py::list<str> tail = {S(""), S("int main(int argc, char** argv) {")};
    if (!mains.empty()) {
        tail.append(S("\tconst char* which = getenv(\"JRT_MAIN\");"));
        for (const auto& ci : mains) {
            tail.append(py::percent(S("\tif (which && strcmp(which, \"%s\") == 0) return jrt_run_main(%s_main, argc, argv);"),
                                    after_dot(ci->qname), ci->cname));
        }
        tail.append(py::percent(S("\treturn jrt_run_main(%s_main, argc, argv);"), truthy(main) ? *main : mains[0]->cname));
    } else {
        tail.append(S("\treturn 0;"));
    }
    tail.append(S("}"));
    py::list<str> all;
    all.extend(head);
    all.extend(structs);
    all.extend(lits_);
    all.append(S(""));
    all.extend(protos);
    all.extend(vfuncs);
    all.extend(lambdas);
    all.extend(defs);
    all.extend(tables_);
    all.extend(tail);
    return py::join(S("\n"), all) + "\n";
}

py::list<str> Program::struct_(const ClassInfoP& ci) {
    py::list<str> L;
    if (eq(ci->kind, "interface")) {
        auto ms = iface_methods(ci->qname);
        L.append(py::percent(S("struct %s_vt {"), ci->cname));
        L.append(S("\tjobject_vt o_;"));
        for (const IMethod& x : ms) {
            py::list<str> ps = py::list<str>{OBJ} + x.ps;
            L.append(py::percent(S("\t%s (*%s)(%s);"), x.r, x.name, jn(ps)));
        }
        L.append(S("};"));
        L.append(S(""));
        return L;
    }
    auto base = base_struct(ci);
    L.append(py::percent(S("struct %s {"), ci->cname));
    L.append(base ? py::percent(S("\t%s super_;"), *base) : S("\tjobject hdr_;"));
    if (ci->needs_outer) L.append(py::percent(S("\t%s outer_;"), ct(std::make_shared<ir::Type>(S("class"), ci->outer->qname))));
    for (const auto& c : ci->captures) {
        L.append(py::percent(S("\t%s cap_%s;"), ct(std::get<1>(c)), _ident(std::get<0>(c))));
    }
    for (const auto& f : ci->fields()) {
        if (f->sym && !f->sym->static_) L.append(py::percent(S("\t%s %s;"), ct(f->type), _ident(f->sym->name)));
    }
    L.append(S("};"));
    // the table type: Object's slots, then every slot of the class and its bases, in order
    L.append(py::percent(S("struct %s_vt {"), ci->cname));
    L.append(S("\tjobject_vt o_;"));
    for (const Slot& s : slots(ci)) {
        py::list<str> ps = py::list<str>{OBJ} + s.ps;
        L.append(py::percent(S("\t%s (*%s)(%s);"), s.r, s.name, jn(ps)));
    }
    L.append(S("};"));
    for (const auto& f : ci->fields()) {
        if (f->sym && f->sym->static_) {
            str init;
            if (jmodel::constant_init(f)) {
                init = Method(this, ci, nullptr).constant(f);
            } else {
                init = (f->type && f->type->is_prim()) ? S("0") : S("NULL");
            }
            L.append(py::percent(S("static %s %s_%s = %s;"), ct(f->type), ci->cname, _ident(f->sym->name), init));
        }
    }
    for (const NodeP& x : lst(ci->members())) {
        if (auto ec = as_a<ir::EnumConst>(x)) {
            L.append(py::percent(S("static %s* %s_%s;"), ci->cname, ci->cname, _ident(need_sym(ec->sym).name)));
        }
    }
    if (eq(ci->kind, "enum")) L.append(py::percent(S("static jarray* %s_values_;"), ci->cname));
    if (needs_clinit(ci)) L.append(py::percent(S("static bool %s_inited_;"), ci->cname));
    L.append(S(""));
    return L;
}

str Program::ctor_name(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m) {
    int64_t n = 0;
    for (const auto& x : ci->methods())
        if (eq(x->kind, "ctor")) ++n;
    if (n <= 1) return py::percent(S("%s_init"), ci->cname);
    if (!m) none_attr("params");
    py::list<str> codes;
    for (const NodeP& pn : lst(m->params)) codes.append(_code(jmodel::erase(prm(pn)->type)));
    str joined = py::join(S(""), codes);
    return py::percent(S("%s_init_%s"), ci->cname, joined.empty() ? S("V") : joined);
}

py::list<str> Program::ctor_params(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m) {
    py::list<str> ps = {py::percent(S("%s* self"), ci->cname)};
    if (eq(ci->kind, "enum")) {
        ps.append(S("jstring* name_in"));
        ps.append(S("int32_t ordinal_in"));
    }
    for (const NodeP& pn : lst(m->params)) {
        auto p = prm(pn);
        ps.append(py::percent(S("%s %s"), ct(p->type), _ident(need_sym(p->sym).name)));
    }
    return ps;
}

py::list<str> Program::new_params(const ClassInfoP& ci, const py::ref<ir::MethodDecl>& m) {
    py::list<str> ps;
    if (ci->needs_outer) ps.append(py::percent(S("%s outer_in"), ct(std::make_shared<ir::Type>(S("class"), ci->outer->qname))));
    for (const auto& c : ci->captures) ps.append(py::percent(S("%s cap_%s"), ct(std::get<1>(c)), _ident(std::get<0>(c))));
    if (eq(ci->kind, "enum")) {
        ps.append(S("jstring* name_in"));
        ps.append(S("int32_t ordinal_in"));
    }
    if (m) {
        for (const NodeP& pn : lst(m->params)) {
            auto p = prm(pn);
            ps.append(py::percent(S("%s %s"), ct(p->type), _ident(need_sym(p->sym).name)));
        }
    }
    return ps;
}

py::list<str> Program::prototypes(const ClassInfoP& ci) {
    py::list<str> L;
    if (eq(ci->kind, "interface")) {
        for (const auto& m : ci->methods()) {
            if (has_body(m)) {   // static and default methods
                auto [ps, r, eps] = sig(ci, m);
                bool static_ = sym_static(m->sym);
                py::list<str> all;
                if (!static_) all.append(OBJ);
                all.extend(ps);
                str args = jn(all);
                L.append(py::percent(S("static %s %s_%s(%s);"), r, ci->cname, mname(or_empty(m->name), eps),
                                     args.empty() ? S("void") : args));
            }
        }
        for (const IMethod& x : iface_methods(ci->qname)) {
            py::list<str> args = {S("o")}, params = {S("jobject* o")};
            for (int64_t i = 0; i < x.ps.size(); ++i) {
                args.append(py::percent(S("a%d"), i));
                params.append(py::percent(S("%s a%d"), x.ps[i], i));
            }
            vfuncs.append(py::percent(S("static inline %s %s_%s_i(%s) { return ((const struct %s_vt*)jrt_ivt(o, &%s_class))->%s(%s); }"),
                                      x.r, ci->cname, x.name, jn(params), ci->cname, ci->cname, x.name, jn(args)));
        }
        return L;
    }
    for (const auto& m : ci->methods()) {
        if (eq(m->kind, "ctor")) {
            L.append(py::percent(S("static void %s(%s);"), ctor_name(ci, m), jn(ctor_params(ci, m))));
            str np = jn(new_params(ci, m));
            L.append(py::percent(S("static %s* %s(%s);"), ci->cname, replace_first(ctor_name(ci, m), S("_init"), S("_new")),
                                 np.empty() ? S("void") : np));
            continue;
        }
        if (!has_body(m)) continue;
        auto [ps, r, eps] = sig(ci, m);
        str name = py::percent(S("%s_%s"), ci->cname, mname(or_empty(m->name), eps));
        if (sym_static(m->sym)) {
            str a = jn(ps);
            L.append(py::percent(S("static %s %s(%s);"), r, name, a.empty() ? S("void") : a));
        } else if (object_method_decl(m)) {
            str rr = eq(m->name, "toString") ? STR : eq(m->name, "equals") ? S("bool") : S("int32_t");
            py::list<str> all = py::list<str>{OBJ} + ps;
            L.append(py::percent(S("static %s %s(%s);"), rr, py::percent(S("%s_%s"), ci->cname, *m->name), jn(all)));
        } else {
            py::list<str> all = py::list<str>{OBJ} + ps;
            L.append(py::percent(S("static %s %s(%s);"), r, name, jn(all)));
        }
    }
    bool any_ctor = false;
    for (const auto& m : ci->methods())
        if (eq(m->kind, "ctor")) any_ctor = true;
    if (!any_ctor) {
        str np = jn(new_params(ci, nullptr));
        L.append(py::percent(S("static %s* %s_new(%s);"), ci->cname, ci->cname, np.empty() ? S("void") : np));
    }
    L.append(py::percent(S("static void %s_finit(%s* self);"), ci->cname, ci->cname));
    if (needs_clinit(ci)) L.append(py::percent(S("static void %s_clinit(void);"), ci->cname));
    if (eq(ci->kind, "enum")) L.append(py::percent(S("static jarray* %s_values(void);"), ci->cname));
    for (const Slot& s : slots(ci)) {
        auto bi = truthy(ci->base) ? model->info(*ci->base) : ClassInfoP();
        bool in_base = false;
        if (bi && !eq(bi->kind, "interface")) {
            for (const Slot& bs : slots(bi))
                if (bs.name == s.name) in_base = true;
        }
        if (in_base) continue;   // the base's helper serves
        py::list<str> args = {S("o")}, params = {S("jobject* o")};
        for (int64_t i = 0; i < s.ps.size(); ++i) {
            args.append(py::percent(S("a%d"), i));
            params.append(py::percent(S("%s a%d"), s.ps[i], i));
        }
        vfuncs.append(py::percent(S("static inline %s %s_%s_v(%s) { return ((const struct %s_vt*)jrt_vt(o))->%s(%s); }"), s.r,
                                  ci->cname, s.name, jn(params), ci->cname, s.name, jn(args)));
    }
    return L;
}

py::list<str> Program::definitions(const ClassInfoP& ci) {
    py::list<str> L;
    if (eq(ci->kind, "interface")) {
        for (const auto& m : ci->methods()) {
            if (has_body(m)) L.extend(Method(this, ci, m).define());
        }
        return L;
    }
    for (const auto& m : ci->methods()) {
        if (!has_body(m)) continue;
        L.extend(Method(this, ci, m).define());
    }
    bool any_ctor = false;
    for (const auto& m : ci->methods())
        if (eq(m->kind, "ctor")) any_ctor = true;
    if (!any_ctor) L.extend(Method(this, ci, nullptr).anon_new());
    L.extend(Method(this, ci, nullptr).finit());
    if (needs_clinit(ci)) L.extend(Method(this, ci, nullptr).clinit());
    if (eq(ci->kind, "enum")) {
        L.append(py::percent(S("static jarray* %s_values(void) {"), ci->cname));
        L.append(py::percent(S("\t%s_clinit();"), ci->cname));
        L.append(py::percent(S("\treturn jrt_array_clone(%s_values_);"), ci->cname));
        L.append(S("}"));
        L.append(S(""));
    }
    return L;
}

py::list<str> Program::tables(const ClassInfoP& ci) {
    py::list<str> L;
    if (eq(ci->kind, "interface")) {
        L.append(py::percent(S("const jclass %s_class = {\"%s\", 0, 0, 0, 0, 0, 1};"), ci->cname, ci->qname.strip(S("<>"))));
        L.append(S(""));
        return L;
    }
    auto obj = object_slots(ci);
    str objs = py::percent(S("{%s, %s, %s}"), obj.at(S("toString")), obj.at(S("equals")), obj.at(S("hashCode")));
    auto sl = slots(ci);
    str slot_text;
    for (const Slot& s : sl) {
        py::list<str> ps = py::list<str>{OBJ} + s.ps;
        slot_text += py::percent(S(", (%s (*)(%s))%s"), s.r, jn(ps), s.impl);
    }
    L.append(py::percent(S("static const struct %s_vt %s_vt_ = {%s%s};"), ci->cname, ci->cname, objs, slot_text));
    py::list<str> itabs;
    for (const str& q : all_ifaces(ci)) {
        auto ms = iface_methods(q);
        if (q == "java.lang.Comparable" && !find_impl(ci, S("compareTo"), 1))
            continue;   // an enum's: jrt_natural compares by ordinal
        py::list<str> impls;
        for (const IMethod& x : ms) {
            auto impl = find_impl(ci, x.java, x.eps.size());
            if (!impl && ci->decl->mods && ci->decl->mods->contains(S("abstract"))) impl = S("0");   // never reached: an instance is of a subclass,
            if (!impl) throw Decline(py::percent(S("%s does not implement %s.%s"), ci->cname, q, x.java));   // whose own table is found first
            py::list<str> ps = py::list<str>{OBJ} + x.ps;
            impls.append(py::percent(S("(%s (*)(%s))%s"), x.r, jn(ps), *impl));
        }
        auto qi = model->info(q);
        str vt = qi ? py::percent(S("struct %s_vt"), qi->cname) : py::percent(S("jrt_%s_vt"), after_dot(q));
        str name = py::percent(S("%s_as_%s"), ci->cname, after_dot(q).replace(S("<"), S("")).replace(S(">"), S("")));
        str tail_impls;
        for (const str& x : impls) tail_impls += S(", ") + x;
        L.append(py::percent(S("static const %s %s = {%s%s};"), vt, name, objs, tail_impls));
        itabs.append(py::percent(S("{%s, &%s}"), class_ref(q), name));
    }
    if (!itabs.empty()) L.append(py::percent(S("static const jitab %s_itabs[] = {%s};"), ci->cname, jn(itabs)));
    L.append(py::percent(S("const jclass %s_class = {\"%s\", %s, sizeof(%s), &%s_vt_, %s, 0};"), ci->cname, ci->qname.strip(S("<>")),
                         base_class_ref(ci), ci->cname, ci->cname,
                         !itabs.empty() ? py::percent(S("JRT_ITABS(%s_itabs)"), ci->cname) : S("0, 0")));
    L.append(S(""));
    return L;
}

// ---------------------------------------------------------------------------------------------- Method
Method::Method(Program* prog, const ClassInfoP& ci_, const py::ref<ir::MethodDecl>& m_,
               const std::optional<py::dict<str, Code>>& lam_)
    : p(prog), ci(ci_), m(m_), lam(lam_) {
    static_ = m && sym_static(m->sym);
    if (ci) {
        for (const auto& c : ci->captures) captures.set(std::get<0>(c), S("self->cap_") + _ident(std::get<0>(c)));
    }
}

// ---- members
py::list<str> Method::define() {
    bool has_try = false;
    for (const NodeP& n : walk_of(m->body))
        if (is_a<ir::Try>(n)) has_try = true;
    vol = has_try;
    py::list<str> L;
    if (eq(m->kind, "ctor")) {
        str name = p->ctor_name(ci, m);
        L.append(py::percent(S("static void %s(%s) {"), name, jn(p->ctor_params(ci, m))));
        for (const NodeP& pn : lst(m->params)) {
            auto pr = prm(pn);
            vars.set(_ident(need_sym(pr->sym).name), p->ct(pr->type));
        }
        auto blk = as_a<ir::Block>(body_node(m->body));
        if (!blk) none_attr("stmts");
        py::list<NodeP> stmts_ = lst(blk->stmts).copy();
        py::ref<ir::CtorCall> first = !stmts_.empty() ? as_a<ir::CtorCall>(stmts_[0]) : nullptr;
        if (first) stmts_ = stmts_.slice_from(1);
        auto first_kind = [&]() -> const str& {
            if (!first->kind) none_attr("startswith");
            return *first->kind;
        };
        bool delegated = first && first_kind().startswith(S("this"));
        if (eq(ci->kind, "enum")) {
            L.append(S("\t((jrt_Enum*)self)->name = name_in;"));
            L.append(S("\t((jrt_Enum*)self)->ordinal = ordinal_in;"));
        }
        if (first && (!first_kind().endswith(S(":implicit")) || delegated)) {
            L.extend(ctor_call(first, S("\t")));
        } else if (truthy(ci->base) && *ci->base != jmodel::OBJECT && !eq(ci->kind, "enum")) {
            auto b = p->model->info(*ci->base);
            if (b) {
                py::ref<ir::MethodDecl> bm;
                for (const auto& x : b->methods()) {
                    if (eq(x->kind, "ctor") && !nonempty(x->params)) {
                        bm = x;
                        break;
                    }
                }
                if (bm) L.append(py::percent(S("\t%s((%s*)self);"), p->ctor_name(b, bm), b->cname));
            }
        }
        if (!delegated) L.append(py::percent(S("\t%s_finit(self);"), ci->cname));
        py::list<str> out;
        stmts(stmts_, out, S("\t"));
        L.extend(out);
        L.append(S("}"));
        L.append(S(""));
        str newname = replace_first(name, S("_init"), S("_new"));
        str np = jn(p->new_params(ci, m));
        L.append(py::percent(S("static %s* %s(%s) {"), ci->cname, newname, np.empty() ? S("void") : np));
        if (p->needs_clinit(ci)) L.append(py::percent(S("\t%s_clinit();"), ci->cname));
        L.append(py::percent(S("\t%s* o = (%s*)jrt_alloc(&%s_class);"), ci->cname, ci->cname, ci->cname));
        if (ci->needs_outer) L.append(S("\to->outer_ = outer_in;"));
        for (const auto& c : ci->captures) L.append(py::percent(S("\to->cap_%s = cap_%s;"), _ident(std::get<0>(c)), _ident(std::get<0>(c))));
        py::list<str> args_;
        if (eq(ci->kind, "enum")) {
            args_.append(S("name_in"));
            args_.append(S("ordinal_in"));
        }
        for (const NodeP& pn : lst(m->params)) args_.append(_ident(need_sym(prm(pn)->sym).name));
        py::list<str> callargs = py::list<str>{S("o")} + args_;
        L.append(py::percent(S("\t%s(%s);"), name, jn(callargs)));
        L.append(S("\treturn o;"));
        L.append(S("}"));
        L.append(S(""));
        return L;
    }
    auto [ps, r, eps] = p->sig(ci, m);
    ret = r;
    bool static__ = sym_static(m->sym);
    py::list<str> params, entry;
    if (!static__) {
        params.append(S("jobject* self_"));
        entry.append(!eq(ci->kind, "interface") ? py::percent(S("\t%s* self = (%s*)self_;"), ci->cname, ci->cname)
                                                : S("\tjobject* self = self_;"));
    }
    const py::list<NodeP>& mparams = lst(m->params);
    int64_t n = std::min<int64_t>(ps.size(), mparams.size());
    for (int64_t i = 0; i < n; ++i) {
        const str& t = ps[i];
        auto pr = prm(mparams[i]);
        str own = p->ct(pr->type);
        str nm = _ident(need_sym(pr->sym).name);
        vars.set(nm, own);
        if (own != t) {
            params.append(py::percent(S("%s %s_in"), t, nm));
            entry.append(py::percent(S("\t%s %s = %s;"), own, nm, conv(nm + "_in", t, own)));
        } else {
            params.append(py::percent(S("%s %s"), t, nm));
        }
    }
    str fname;
    if (object_method_decl(m) && !static__) {
        fname = py::percent(S("%s_%s"), ci->cname, *m->name);
    } else {
        fname = py::percent(S("%s_%s"), ci->cname, p->mname(or_empty(m->name), eps));
    }
    str pj = jn(params);
    L.append(py::percent(S("static %s %s(%s) {"), r, fname, pj.empty() ? S("void") : pj));
    if (static__ && p->needs_clinit(ci)) L.append(py::percent(S("\t%s_clinit();"), ci->cname));
    L.extend(entry);
    py::list<str> out;
    auto blk = as_a<ir::Block>(body_node(m->body));
    if (!blk) none_attr("stmts");
    stmts(lst(blk->stmts), out, S("\t"));
    L.extend(out);
    L.append(S("}"));
    L.append(S(""));
    return L;
}

py::list<str> Method::ctor_call(const py::ref<ir::CtorCall>& call, const str& ind) {
    if (!call->kind) none_attr("startswith");
    if (call->kind->startswith(S("this"))) {
        auto [_c, tm] = p->model->method_decl(call->sym);
        if (!tm) throw Decline(S("this(...) target"));
        py::list<str> pcts;
        for (const NodeP& pn : lst(tm->params)) pcts.append(p->ct(prm(pn)->type));
        str a = args(call->args, pcts, call->sym);
        py::list<str> extra;
        if (eq(ci->kind, "enum")) {
            extra.append(S("name_in"));
            extra.append(S("ordinal_in"));
        }
        py::list<str> all = py::list<str>{S("self")} + extra;
        if (!a.empty()) all.append(a);
        return py::list<str>{ind + py::percent(S("%s(%s);"), p->ctor_name(ci, tm), jn(all))};
    }
    if (!truthy(ci->base) || *ci->base == jmodel::OBJECT) return py::list<str>();
    auto b = p->model->info(*ci->base);
    if (!b) {   // a runtime Throwable
        py::list<str> as;
        const py::list<NodeP>& cargs = lst(call->args);
        for (int64_t i = 0; i < cargs.size(); ++i) {
            auto [c, t] = expr(cargs[i]);
            as.append(conv(c, t, i == 0 ? STR : OBJ));
        }
        TypeP t0 = as.size() == 1 ? type_of(cargs[0]) : nullptr;
        if (as.size() == 1 && t0 && !eq(t0->name, "java.lang.String"))
            return py::list<str>{ind + py::percent(S("jrt_Throwable_init_cause((jobject*)self, NULL, %s);"), as[0])};
        if (as.size() == 2)
            return py::list<str>{ind + py::percent(S("jrt_Throwable_init_cause((jobject*)self, %s, %s);"), as[0], as[1])};
        return py::list<str>{ind + py::percent(S("jrt_Throwable_init((jobject*)self, %s);"), !as.empty() ? as[0] : S("NULL"))};
    }
    auto [_bc, bm] = p->model->method_decl(call->sym);
    if (!bm) throw Decline(S("super(...) target"));
    py::list<str> pcts;
    for (const NodeP& pn : lst(bm->params)) pcts.append(p->ct(prm(pn)->type));
    str a = args(call->args, pcts, call->sym);
    py::list<str> all = {py::percent(S("(%s*)self"), b->cname)};
    if (!a.empty()) all.append(a);
    return py::list<str>{ind + py::percent(S("%s(%s);"), p->ctor_name(b, bm), jn(all))};
}

py::list<str> Method::anon_new() {
    std::optional<str> sup;
    auto b = truthy(ci->base) ? p->model->info(*ci->base) : ClassInfoP();
    auto n = ci->new_;
    py::list<str> params = p->new_params(ci, nullptr);
    py::list<str> sargs;
    if (b && !eq(b->kind, "interface") && n && n->sym) {
        auto [_bc, bm] = p->model->method_decl(n->sym);
        if (bm) {
            int64_t i = 0;
            for (const NodeP& pn : lst(bm->params)) {
                params.append(py::percent(S("%s a%d"), p->ct(prm(pn)->type), i));
                sargs.append(py::percent(S("a%d"), i));
                ++i;
            }
            str tail;
            for (const str& a : sargs) tail += S(", ") + a;
            sup = py::percent(S("\t%s((%s*)o%s);"), p->ctor_name(b, bm), b->cname, tail);
        }
    }
    py::list<str> L;
    str pj = jn(params);
    L.append(py::percent(S("static %s* %s_new(%s) {"), ci->cname, ci->cname, pj.empty() ? S("void") : pj));
    if (p->needs_clinit(ci)) L.append(py::percent(S("\t%s_clinit();"), ci->cname));
    L.append(py::percent(S("\t%s* o = (%s*)jrt_alloc(&%s_class);"), ci->cname, ci->cname, ci->cname));
    if (ci->needs_outer) L.append(S("\to->outer_ = outer_in;"));
    for (const auto& c : ci->captures) L.append(py::percent(S("\to->cap_%s = cap_%s;"), _ident(std::get<0>(c)), _ident(std::get<0>(c))));
    if (sup && !sup->empty()) L.append(*sup);
    L.append(py::percent(S("\t%s_finit(o);"), ci->cname));
    L.append(S("\treturn o;"));
    L.append(S("}"));
    L.append(S(""));
    return L;
}

py::list<str> Method::finit() {
    py::list<str> out;
    for (const NodeP& x : lst(ci->members())) {
        if (auto fd = as_a<ir::FieldDecl>(x)) {
            if (fd->init && fd->sym && !fd->sym->static_) {
                auto thisn = std::make_shared<ir::This>();
                auto sel = std::make_shared<ir::Select>();
                sel->obj = thisn;
                sel->sym = fd->sym;
                sel->type = fd->type;
                auto as = std::make_shared<ir::Assign>();
                as->target = sel;
                as->value = fd->init;
                as->type = fd->type;
                auto es = std::make_shared<ir::ExprStmt>();
                es->expr = as;
                stmt(es, out, S("\t"));
            }
        } else if (auto md = as_a<ir::MethodDecl>(x)) {
            if (eq(md->kind, "init")) {
                auto blk = as_a<ir::Block>(body_node(md->body));
                if (!blk) none_attr("stmts");
                stmts(lst(blk->stmts), out, S("\t"));
            }
        }
    }
    py::list<str> L = {py::percent(S("static void %s_finit(%s* self) {"), ci->cname, ci->cname)};
    L.extend(out);
    L.append(S("}"));
    L.append(S(""));
    return L;
}

py::list<str> Method::clinit() {
    static_ = true;
    py::list<str> L = {py::percent(S("static void %s_clinit(void) {"), ci->cname),
                       py::percent(S("\tif (%s_inited_) return;"), ci->cname), py::percent(S("\t%s_inited_ = true;"), ci->cname)};
    auto b = truthy(ci->base) ? p->model->info(*ci->base) : ClassInfoP();
    if (b && p->needs_clinit(b)) L.append(py::percent(S("\t%s_clinit();"), b->cname));
    py::list<py::ref<ir::EnumConst>> consts;
    for (const NodeP& x : lst(ci->members()))
        if (auto ec = as_a<ir::EnumConst>(x)) consts.append(ec);
    for (int64_t k = 0; k < consts.size(); ++k) {
        const auto& x = consts[k];
        ClassInfoP sub = x->body ? (p->model->by_decl.contains(id_of(x->body)) ? p->model->by_decl.at(id_of(x->body)) : ClassInfoP())
                                 : ClassInfoP();
        ClassInfoP target = sub ? sub : ci;
        py::ref<ir::MethodDecl> cm;
        if (x->ctor) {
            auto [_cc, m2] = p->model->method_decl(x->ctor);
            cm = m2;
        }
        py::list<str> pcts;
        if (cm)
            for (const NodeP& pn : lst(cm->params)) pcts.append(p->ct(prm(pn)->type));
        str a = args(x->args ? x->args : Nodes(py::list<NodeP>()), pcts, x->ctor);
        py::list<str> lead = {p->lit(need_sym(x->sym).name), py::to_str(k)};
        if (!a.empty()) lead.append(a);
        if (sub) {
            L.append(py::percent(S("\t%s_%s = (%s*)%s_new(%s);"), ci->cname, _ident(x->sym->name), ci->cname, target->cname, jn(lead)));
        } else {
            str ctor = cm ? replace_first(p->ctor_name(ci, cm), S("_init"), S("_new")) : py::percent(S("%s_new"), ci->cname);
            L.append(py::percent(S("\t%s_%s = %s(%s);"), ci->cname, _ident(x->sym->name), ctor, jn(lead)));
        }
    }
    if (eq(ci->kind, "enum")) {
        L.append(py::percent(S("\t%s_values_ = jrt_new_array(sizeof(jobject*), %d);"), ci->cname, consts.size()));
        for (int64_t k = 0; k < consts.size(); ++k) {
            L.append(py::percent(S("\tJA(jobject*, %s_values_, %d) = (jobject*)%s_%s;"), ci->cname, k, ci->cname,
                                 _ident(consts[k]->sym->name)));
        }
    }
    for (const NodeP& x : lst(ci->members())) {
        if (auto fd = as_a<ir::FieldDecl>(x)) {
            if (fd->sym && fd->sym->static_ && fd->init && !jmodel::constant_init(fd)) {
                py::list<str> out;
                auto tn = std::make_shared<ir::TypeName>();
                tn->type = std::make_shared<ir::Type>(S("class"), ci->qname);
                auto sel = std::make_shared<ir::Select>();
                sel->obj = tn;
                sel->sym = fd->sym;
                sel->type = fd->type;
                auto as = std::make_shared<ir::Assign>();
                as->target = sel;
                as->value = fd->init;
                as->type = fd->type;
                auto es = std::make_shared<ir::ExprStmt>();
                es->expr = as;
                stmt(es, out, S("\t"));
                L.extend(out);
            }
        } else if (auto md = as_a<ir::MethodDecl>(x)) {
            if (eq(md->kind, "static_init")) {
                py::list<str> out;
                auto blk = as_a<ir::Block>(body_node(md->body));
                if (!blk) none_attr("stmts");
                stmts(lst(blk->stmts), out, S("\t"));
                L.extend(out);
            }
        }
    }
    L.append(S("}"));
    L.append(S(""));
    return L;
}

str Method::constant(const py::ref<ir::FieldDecl>& f) {
    auto [code, ct] = expr(f->init);
    return conv(code, ct, p->ct(f->type));
}

// ---- statements
void Method::stmts(const py::list<NodeP>& ss, const py::list<str>& out, const str& ind) {
    for (const NodeP& s : ss) stmt(s, out, ind);
}

void Method::block(const NodeP& s, const py::list<str>& out, const str& ind) {
    auto saved = vars.copy();
    if (auto b = as_a<ir::Block>(s)) {
        stmts(lst(b->stmts), out, ind + "\t");
    } else if (s) {
        stmt(s, out, ind + "\t");
    }
    vars = saved;
}

void Method::loop_body(const NodeP& body, const py::list<str>& out, const str& ind) {
    auto lab = cont_next;
    cont_next.reset();
    loops += 1;
    try {
        if (!lab) {
            block(body, out, ind);
        } else {
            out.append(ind + "\t{");
            block(body, out, ind + "\t");
            out.append(ind + "\t}");
            out.append(ind + "\t" + *lab + "_cont:;");
        }
    } catch (...) {
        loops -= 1;
        throw;
    }
    loops -= 1;
}

void Method::stmt(const NodeP& s, const py::list<str>& out, const str& ind) {
    auto saved = pending;
    pending = py::list<str>();
    bool saved_order = order;
    order = true;
    auto saved_subst = subst.copy();
    py::list<str> sub;
    auto finish = [&]() {
        auto decls = pending;
        pending = saved;
        order = saved_order;
        subst = saved_subst;
        return decls;
    };
    try {
        _stmt(s, sub, ind);
    } catch (...) {
        finish();
        throw;
    }
    auto decls = finish();
    for (const str& d : decls) out.append(ind + d);
    out.extend(sub);
}

str Method::cond(const NodeP& e) {
    auto [code, ct] = expr(cpp03::_strip_paren(e));
    return conv(code, ct, S("bool"));
}

str Method::decl(const str& ct, const str& name, const std::optional<str>& init) {
    str q;
    if (vol) {
        q = ct.endswith(S("*")) ? py::percent(S("%s volatile"), ct) : py::percent(S("volatile %s"), ct);
    } else {
        q = ct;
    }
    str tail = init ? S(" = ") + *init : (is_prim_c(ct) ? S(" = 0") : S(" = NULL"));
    return py::percent(S("%s %s%s;"), q, name, tail);
}

str Method::pops(int64_t depth) {
    str out;
    for (int64_t i = tries.size() - 1; i >= depth; --i) out += py::percent(S("jrt_pop(&%s); "), std::get<0>(tries[i]));
    return out;
}

void Method::_stmt(const NodeP& s, const py::list<str>& out, const str& ind) {
    if (auto b = as_a<ir::Block>(s)) {
        out.append(ind + "{");
        block(b, out, ind);
        out.append(ind + "}");
    } else if (auto l = as_a<ir::Local>(s)) {
        str name = _ident(need_sym(l->sym).name);
        str ct = p->ct(l->type);
        vars.set(name, ct);
        if (!l->init) {
            out.append(ind + decl(ct, name));
            return;
        }
        auto [code, it] = expr(l->init);
        out.append(ind + decl(ct, name, conv(code, it, ct)));
    } else if (auto es = as_a<ir::ExprStmt>(s)) {
        auto [code, _t] = expr(es->expr);
        out.append(ind + code + ";");
    } else if (auto i = as_a<ir::If>(s)) {
        out.append(ind + py::percent(S("if (%s) {"), cond(i->cond)));
        block(one(i->then), out, ind);
        NodeOrList e = i->orelse;
        while (std::holds_alternative<NodeP>(e) && is_a<ir::If>(std::get<NodeP>(e))) {
            auto ei = as_a<ir::If>(std::get<NodeP>(e));
            out.append(ind + py::percent(S("} else if (%s) {"), cond(ei->cond)));
            block(one(ei->then), out, ind);
            e = ei->orelse;
        }
        // (`e is not None`: a bare list is not None, and block() declines it)
        if (std::holds_alternative<py::list<NodeP>>(e) || std::get<NodeP>(e)) {
            out.append(ind + "} else {");
            block(one(e), out, ind);
        }
        out.append(ind + "}");
    } else if (auto w = as_a<ir::While>(s)) {
        out.append(ind + py::percent(S("while (%s) {"), cond(w->cond)));
        loop_body(one(w->body), out, ind);
        out.append(ind + "}");
    } else if (auto d = as_a<ir::DoWhile>(s)) {
        out.append(ind + "do {");
        loop_body(one(d->body), out, ind);
        out.append(ind + py::percent(S("} while (%s);"), cond(d->cond)));
    } else if (auto f = as_a<ir::For>(s)) {
        auto saved = vars.copy();
        out.append(ind + "{");
        str inner = ind + "\t";
        for (const NodeP& i0 : lst(f->init)) stmt(i0, out, inner);
        str c = f->cond ? cond(f->cond) : S("1");
        py::list<str> ups;
        for (const NodeP& u : lst(f->update)) ups.append(std::get<0>(expr(u)));
        out.append(inner + py::percent(S("for (; %s; %s) {"), c, jn(ups)));
        loop_body(one(f->body), out, inner);
        out.append(inner + "}");
        out.append(ind + "}");
        vars = saved;
    } else if (auto lb = as_a<ir::Labeled>(s)) {
        str lab = or_empty(lb->label);
        py::list<NodeP> used;
        for (const NodeP& n : walk_of(lb->body)) {
            if (auto br = as_a<ir::Break>(n)) {
                if (br->label == lb->label) used.append(n);
            } else if (auto co = as_a<ir::Continue>(n)) {
                if (co->label == lb->label) used.append(n);
            }
        }
        for (const NodeP& n : used)
            if (is_a<ir::Continue>(n)) cont_next = lab;
        stmt(one(lb->body), out, ind);
        for (const NodeP& n : used) {
            if (is_a<ir::Break>(n)) {
                out.append(ind + lab + "_brk:;");
                break;
            }
        }
    } else if (auto r = as_a<ir::Return>(s)) {
        str pp = pops(0);
        if (!r->expr) {
            out.append(ind + pp + "return;");
        } else {
            auto [code, t] = expr(r->expr);
            str v = conv(code, t, ret);
            if (!pp.empty()) {
                out.append(ind + py::percent(S("{ %s r_ = %s; %sreturn r_; }"), ret, v, pp));
            } else {
                out.append(ind + py::percent(S("return %s;"), v));
            }
        }
    } else if (is_a<ir::Break>(s) || is_a<ir::Continue>(s)) {
        bool is_break = is_a<ir::Break>(s);
        std::optional<str> label = is_break ? as_a<ir::Break>(s)->label : as_a<ir::Continue>(s)->label;
        str pp;
        if (!truthy(label)) {
            int64_t depth = 0;
            for (const auto& t : tries)
                if (std::get<1>(t) < loops) ++depth;
            pp = pops(depth);
        } else {
            pp = !tries.empty() ? pops(0) : S("");
        }
        str word = truthy(label) ? py::percent(S("goto %s_%s;"), *label, is_break ? S("brk") : S("cont"))
                                 : (is_break ? S("break;") : S("continue;"));
        out.append(ind + pp + word);
    } else if (auto th = as_a<ir::Throw>(s)) {
        auto [code, _t] = expr(th->expr);
        out.append(ind + py::percent(S("jrt_throw((jobject*)%s);"), code));
    } else if (auto sw = as_a<ir::Switch>(s)) {
        switch_(sw, out, ind);
    } else if (auto tr = as_a<ir::Try>(s)) {
        try_(tr, out, ind);
    } else if (is_a<ir::Empty>(s)) {
        out.append(ind + ";");
    } else if (auto sy = as_a<ir::Sync>(s)) {
        block(sy->body, out, ind);
    } else if (is_a<ir::Assert>(s)) {
    } else if (is_a<ir::LocalClass>(s)) {
    } else if (is_a<ir::CtorCall>(s)) {
        throw Decline(S("constructor call not first"));
    } else {
        throw Decline(S("statement ") + (s ? s->class_name() : S("NoneType")));
    }
}

str Method::case_label(const NodeP& lab0, const ClassInfoP& enum_) {
    NodeP lab = cpp03::_strip_paren(lab0);
    if (enum_) {
        std::optional<str> name;
        SymP sy;
        if (auto se = as_a<ir::Select>(lab)) sy = se->sym;
        else if (auto na = as_a<ir::Name>(lab)) sy = na->sym;
        if (sy) name = sy->name;
        int64_t idx = -1, k = 0;
        for (const NodeP& x : lst(enum_->members())) {
            if (auto ec = as_a<ir::EnumConst>(x)) {
                if (name && need_sym(ec->sym).name == *name && idx < 0) idx = k;
                ++k;
            }
        }
        if (idx < 0) throw Decline(S("enum case label"));
        return py::percent(S("%d"), idx);
    }
    if (is_a<ir::Lit>(lab)) return std::get<0>(expr(lab));
    if (auto u = as_a<ir::Unary>(lab)) {
        if (eq(u->op, "-") && is_a<ir::Lit>(u->x)) return S("-") + std::get<0>(expr(u->x));
    }
    if (auto se = as_a<ir::Select>(lab)) {
        if (se->sym) {
            auto ci2 = p->model->info(or_empty(se->sym->owner));
            py::ref<ir::FieldDecl> f;
            if (ci2) {
                for (const auto& x : ci2->fields()) {
                    if (x->sym && x->sym->name == se->sym->name) {
                        f = x;
                        break;
                    }
                }
            }
            if (f && f->init) return case_label(f->init, nullptr);
        }
    }
    throw Decline(S("case label"));
}

void Method::switch_(const py::ref<ir::Switch>& s, const py::list<str>& out, const str& ind) {
    auto [x0, xt] = expr(cpp03::_strip_paren(s->expr));
    str x = x0;
    ClassInfoP enum_ = p->model->of_type(type_of(s->expr));
    if (enum_ && eq(enum_->kind, "enum")) {
        x = py::percent(S("jrt_Enum_ordinal_(%s)"), x);
    } else if (xt == OBJ) {
        TypeP et = type_of(s->expr);
        str code = BOX().get(or_empty(K(et).name), S("I"));
        x = conv(x, xt, CT_OF_CODE().at(code));
        enum_ = nullptr;
    } else {
        enum_ = nullptr;
    }
    out.append(ind + py::percent(S("switch (%s) {"), x));
    loops += 1;
    try {
        for (const NodeP& cn : lst(s->cases)) {
            auto c = as_a<ir::Case>(cn);
            if (!c) none_attr("labels");
            for (const NodeP& lab : lst(c->labels)) out.append(ind + py::percent(S("case %s:"), case_label(lab, enum_)));
            if (c->default_ && *c->default_) out.append(ind + "default:");
            out.append(ind + "{");
            auto saved = vars.copy();
            for (const NodeP& b : lst(c->body)) stmt(b, out, ind + "\t");
            vars = saved;
            out.append(ind + "}");
        }
    } catch (...) {
        loops -= 1;
        throw;
    }
    loops -= 1;
    out.append(ind + "}");
}

void Method::try_(const py::ref<ir::Try>& s, const py::list<str>& out, const str& ind) {
    if (nonempty(s->resources)) throw Decline(S("try-with-resources"));
    if (s->final) {
        py::list<NodeP> bodies = {s->body};
        for (const NodeP& c : lst(s->catches)) {
            auto cc = as_a<ir::Catch>(c);
            if (!cc) none_attr("body");
            bodies.append(cc->body);
        }
        if (cpp03::_leaves(bodies)) throw Decline(S("a finally left by a jump"));
        temps += 1;
        str f = py::percent(S("f%d"), temps);
        out.append(ind + "{");
        out.append(ind + "\t" + py::percent(S("jrt_try %s;"), f));
        out.append(ind + "\t" + py::percent(S("jrt_push(&%s);"), f));
        out.append(ind + "\t" + py::percent(S("if (setjmp(%s.jb) == 0) {"), f));
        tries.append(std::make_tuple(f, loops));
        try {
            try_catch(s, out, ind + "\t\t");
        } catch (...) {
            tries.pop();
            throw;
        }
        tries.pop();
        out.append(ind + "\t\t" + py::percent(S("jrt_pop(&%s);"), f));
        out.append(ind + "\t} else {");
        out.append(ind + "\t\t" + py::percent(S("jrt_pop(&%s);"), f));
        block(s->final, out, ind + "\t");
        out.append(ind + "\t\t" + py::percent(S("jrt_throw(%s.ex);"), f));
        out.append(ind + "\t}");
        block(s->final, out, ind);
        out.append(ind + "}");
        return;
    }
    try_catch(s, out, ind);
}

void Method::try_catch(const py::ref<ir::Try>& s, const py::list<str>& out, const str& ind) {
    if (lst(s->catches).empty()) {
        out.append(ind + "{");
        block(s->body, out, ind);
        out.append(ind + "}");
        return;
    }
    temps += 1;
    str t = py::percent(S("t%d"), temps);
    out.append(ind + "{");
    out.append(ind + "\t" + py::percent(S("jrt_try %s;"), t));
    out.append(ind + "\t" + py::percent(S("jrt_push(&%s);"), t));
    out.append(ind + "\t" + py::percent(S("if (setjmp(%s.jb) == 0) {"), t));
    tries.append(std::make_tuple(t, loops));
    try {
        block(s->body, out, ind + "\t");
    } catch (...) {
        tries.pop();
        throw;
    }
    tries.pop();
    out.append(ind + "\t\t" + py::percent(S("jrt_pop(&%s);"), t));
    out.append(ind + "\t} else {");
    out.append(ind + "\t\t" + py::percent(S("jrt_pop(&%s);"), t));
    bool first = true;
    for (const NodeP& cn : lst(s->catches)) {
        auto c = as_a<ir::Catch>(cn);
        if (!c) none_attr("sym");
        str name = _ident(need_sym(c->sym).name);
        py::list<str> tests;
        if (!c->types) throw py::TypeError("'NoneType' object is not iterable");
        for (const TypeP& x : *c->types) tests.append(py::percent(S("jrt_instanceof(%s.ex, %s)"), t, p->class_ref(or_empty(K(x).name))));
        str test = py::join(S(" || "), tests);
        str ct = c->types->size() == 1 ? p->ct((*c->types)[0]) : S("jrt_Throwable*");
        if (ct == OBJ) ct = S("jrt_Throwable*");
        out.append(ind + "\t\t" + (first ? S("") : S("} else ")) + py::percent(S("if (%s) {"), test));
        first = false;
        auto saved = vars.copy();
        vars.set(name, ct);
        out.append(ind + "\t\t\t" + py::percent(S("%s %s = (%s)%s.ex;"), ct, name, ct, t));
        block(c->body, out, ind + "\t\t");
        vars = saved;
    }
    out.append(ind + "\t\t} else {");
    out.append(ind + "\t\t\t" + py::percent(S("jrt_throw(%s.ex);"), t));
    out.append(ind + "\t\t}");
    out.append(ind + "\t}");
    out.append(ind + "}");
}

// ---- expressions -> (code, C type)
Code Method::expr(const NodeP& e) {
    if (e && subst.contains(id_of(e))) return subst.at(id_of(e));
    enum class Kind { Paren, Lit, Name, This, Select, Length, Index, Unary, Binary, Concat, Assign, Cond, Cast, InstanceOf, Call,
                      New, NewArray, SwitchExpr, ClassLit, TypeName, Lambda, MethodRef, None };
    Kind k = Kind::None;
    if (is_a<ir::Paren>(e)) k = Kind::Paren;
    else if (is_a<ir::Lit>(e)) k = Kind::Lit;
    else if (is_a<ir::Name>(e)) k = Kind::Name;
    else if (is_a<ir::This>(e)) k = Kind::This;
    else if (is_a<ir::Select>(e)) k = Kind::Select;
    else if (is_a<ir::Length>(e)) k = Kind::Length;
    else if (is_a<ir::Index>(e)) k = Kind::Index;
    else if (is_a<ir::Unary>(e)) k = Kind::Unary;
    else if (is_a<ir::Binary>(e)) k = Kind::Binary;
    else if (is_a<ir::Concat>(e)) k = Kind::Concat;
    else if (is_a<ir::Assign>(e)) k = Kind::Assign;
    else if (is_a<ir::Cond>(e)) k = Kind::Cond;
    else if (is_a<ir::Cast>(e)) k = Kind::Cast;
    else if (is_a<ir::InstanceOf>(e)) k = Kind::InstanceOf;
    else if (is_a<ir::Call>(e)) k = Kind::Call;
    else if (is_a<ir::New>(e)) k = Kind::New;
    else if (is_a<ir::NewArray>(e)) k = Kind::NewArray;
    else if (is_a<ir::SwitchExpr>(e)) k = Kind::SwitchExpr;
    else if (is_a<ir::ClassLit>(e)) k = Kind::ClassLit;
    else if (is_a<ir::TypeName>(e)) k = Kind::TypeName;
    else if (is_a<ir::Lambda>(e)) k = Kind::Lambda;
    else if (is_a<ir::MethodRef>(e)) k = Kind::MethodRef;
    if (k == Kind::None) throw Decline(S("expression ") + (e ? e->class_name() : S("NoneType")));
    str pre = order ? sequenced(e) : S("");
    Code r;
    switch (k) {
        case Kind::Paren: r = x_Paren(as_a<ir::Paren>(e)); break;
        case Kind::Lit: r = x_Lit(as_a<ir::Lit>(e)); break;
        case Kind::Name: r = x_Name(as_a<ir::Name>(e)); break;
        case Kind::This: r = x_This(as_a<ir::This>(e)); break;
        case Kind::Select: r = x_Select(as_a<ir::Select>(e)); break;
        case Kind::Length: r = x_Length(as_a<ir::Length>(e)); break;
        case Kind::Index: r = x_Index(as_a<ir::Index>(e)); break;
        case Kind::Unary: r = x_Unary(as_a<ir::Unary>(e)); break;
        case Kind::Binary: r = x_Binary(as_a<ir::Binary>(e)); break;
        case Kind::Concat: r = x_Concat(as_a<ir::Concat>(e)); break;
        case Kind::Assign: r = x_Assign(as_a<ir::Assign>(e)); break;
        case Kind::Cond: r = x_Cond(as_a<ir::Cond>(e)); break;
        case Kind::Cast: r = x_Cast(as_a<ir::Cast>(e)); break;
        case Kind::InstanceOf: r = x_InstanceOf(as_a<ir::InstanceOf>(e)); break;
        case Kind::Call: r = x_Call(as_a<ir::Call>(e)); break;
        case Kind::New: r = x_New(as_a<ir::New>(e)); break;
        case Kind::NewArray: r = x_NewArray(as_a<ir::NewArray>(e)); break;
        case Kind::SwitchExpr: r = x_SwitchExpr(as_a<ir::SwitchExpr>(e)); break;
        case Kind::ClassLit: r = x_ClassLit(as_a<ir::ClassLit>(e)); break;
        case Kind::TypeName: r = x_TypeName(as_a<ir::TypeName>(e)); break;
        case Kind::Lambda: r = x_Lambda(as_a<ir::Lambda>(e)); break;
        case Kind::MethodRef: r = x_MethodRef(as_a<ir::MethodRef>(e)); break;
        case Kind::None: break;
    }
    if (!pre.empty()) return Code(py::percent(S("(%s, %s)"), pre, std::get<0>(r)), std::get<1>(r));
    return r;
}

str Method::sequenced(const NodeP& e) {
    if (is_a<ir::Lambda>(e) || is_a<ir::MethodRef>(e)) return S("");
    py::list<NodeP> nodes = cpp03::_operands(e);
    if (nodes.size() < 2 || !cpp03::_order_matters(nodes)) return S("");
    py::list<str> parts;
    for (const NodeP& n : nodes) {
        if (cpp03::_constant(n) || subst.contains(id_of(n)) || is_a<ir::Lambda>(n) || is_a<ir::MethodRef>(n)) continue;
        auto [code, ct] = expr(n);
        if (ct == "void" || ct == "nullptr_t") return S("");
        temps += 1;
        str name = py::percent(S("o%d"), temps);
        pending.append(decl(ct, name));
        parts.append(py::percent(S("%s = %s"), name, code));
        subst.set(id_of(n), Code(name, ct));
    }
    return jn(parts);
}

bool Method::is_sub(const str& src, const str& dst) {
    if (src == dst || dst == OBJ || dst == "void*" || src == "nullptr_t") return true;
    ClassInfoP ca, cb;
    for (const auto& c : p->model->classes.values()) {
        if (!ca && c->cname + "*" == src) ca = c;
        if (!cb && c->cname + "*" == dst) cb = c;
        if (ca && cb) break;
    }
    if (ca && cb) return p->model->is_sub(ca->qname, cb->qname);
    return false;
}

str Method::conv(const str& code, const str& src, const str& dst) {
    if (src == dst || dst == "void" || dst.empty() || src.empty()) return code;
    if (src == "nullptr_t") return S("NULL");
    if (is_prim_c(dst) && is_prim_c(src)) {
        if ((src == "double" || src == "float") && is_integral(dst)) {
            if (dst == "int64_t") return py::percent(S("jrt_d2l(%s)"), code);
            return dst == "int32_t" ? py::percent(S("jrt_d2i(%s)"), code) : py::percent(S("(%s)jrt_d2i(%s)"), dst, code);
        }
        return py::percent(S("(%s)(%s)"), dst, code);
    }
    const auto& codes = CODES();
    if (is_prim_c(dst) && src.endswith(S("*"))) return py::percent(S("jrt_unbox_%s((jobject*)%s)"), codes.at(dst), code);
    if (dst.endswith(S("*")) && is_prim_c(src))
        return py::percent(S("%sjrt_box_%s(%s)"), dst == OBJ ? S("") : py::percent(S("(%s)"), dst), codes.at(src), code);
    if (dst.endswith(S("*")) && src.endswith(S("*"))) {
        if (is_sub(src, dst)) return src == dst ? code : py::percent(S("(%s)%s"), dst, code);
        if (dst == STR) return py::percent(S("(jstring*)jrt_cast((jobject*)%s, &jrt_String_class)"), code);
        ClassInfoP c;
        for (const auto& x : p->model->classes.values()) {
            if (x->cname + "*" == dst) {
                c = x;
                break;
            }
        }
        if (c) return py::percent(S("(%s)jrt_cast((jobject*)%s, &%s_class)"), dst, code, c->cname);
        return py::percent(S("(%s)%s"), dst, code);
    }
    return code;
}

Code Method::x_Paren(const py::ref<ir::Paren>& e) {
    auto [x, t] = expr(e->x);
    return Code(py::percent(S("(%s)"), x), t);
}

Code Method::x_Lit(const py::ref<ir::Lit>& e) {
    const ir::Val& v = e->value;
    str k = e->type ? e->type->kind : S("null");
    if (v.is_none()) return Code(S("NULL"), S("nullptr_t"));
    if (k == "bool") return Code(val_truthy(v) ? S("true") : S("false"), S("bool"));
    if (k == "i32") {
        auto iv = int_of(v);
        bool is_min = iv && *iv == -2147483648LL;
        return Code(is_min ? S("JRT_INT_MIN") : val_str(v), S("int32_t"));
    }
    if (k == "i64") {
        auto iv = int_of(v);
        bool is_min = iv && *iv == INT64_MIN;
        return Code(is_min ? S("JRT_LONG_MIN") : py::percent(S("%dLL"), iv ? *iv : int64_t(0)), S("int64_t"));
    }
    if (k == "f32") return Code(cpp03::_float(v, S("F")).replace(S("F"), S("f")), S("float"));
    if (k == "f64") return Code(cpp03::_float(v, S("")), S("double"));
    if (k == "u16") {
        str c;
        if (std::holds_alternative<str>(v.v)) c = jmodel::char_literal(std::get<str>(v.v));
        else if (auto iv = int_of(v)) c = jmodel::char_literal(*iv);
        else throw py::TypeError("ord() expected string of length 1");
        return Code(c, S("char16_t"));
    }
    if (std::holds_alternative<str>(v.v)) return Code(p->lit(std::get<str>(v.v)), STR);
    throw Decline(S("literal ") + v.repr());
}

Code Method::x_Name(const py::ref<ir::Name>& e) {
    str n = _ident(need_sym(e->sym).name);
    if (vars.contains(n)) return Code(n, vars.at(n));
    if (lam && lam->contains(e->sym->name)) return lam->at(e->sym->name);
    if (captures.contains(e->sym->name)) return Code(captures.at(e->sym->name), p->ct(e->type));
    throw Decline(S("unknown local ") + n);
}

Code Method::this_ref() {
    if (lam) {
        if (!lam->contains(S("#this"))) throw Decline(S("this in a closure that did not capture it"));
        return lam->at(S("#this"));
    }
    if (static_) throw Decline(S("this in a static context"));
    return Code(S("self"), eq(ci->kind, "interface") ? OBJ : ci->cname + "*");
}

Code Method::x_This(const py::ref<ir::This>& e) {
    TypeP q = e->qualifier ? e->qualifier : e->type;
    if (q && q->kind == "class" && !(q->name && *q->name == ci->qname) && ci->outer &&
        !p->model->is_sub(ci->qname, or_empty(q->name)))
        return outer_of(or_empty(q->name));
    return this_ref();
}

Code Method::outer_of(const str& qname) {
    auto [code0, ct] = this_ref();
    str code = code0;
    ClassInfoP c = ci;
    while (c && !(c->qname == qname || p->model->is_sub(c->qname, qname))) {
        if (!c->needs_outer) throw Decline(S("no enclosing instance of ") + qname);
        code = code + "->outer_";
        c = c->outer;
    }
    if (!c) throw Decline(S("no enclosing instance of ") + qname);
    return Code(code, c->cname + "*");
}

Code Method::receiver(const NodeP& obj, const SymP& sym) {
    bool self_like = !obj;
    if (!self_like && (is_a<ir::This>(obj) || is_a<ir::Super>(obj))) {
        TypeP q = is_a<ir::This>(obj) ? as_a<ir::This>(obj)->qualifier : as_a<ir::Super>(obj)->qualifier;
        TypeP ot = type_of(obj);
        if (!q && (!ot || (ot->name && *ot->name == ci->qname) || p->model->is_sub(ci->qname, or_empty(ot ? ot->name : std::nullopt)) ||
                   is_a<ir::Super>(obj)))
            self_like = true;
    }
    if (self_like) {
        if (sym && truthy(sym->owner) && *sym->owner != ci->qname && ci->outer && !p->model->is_sub(ci->qname, *sym->owner) &&
            (!obj || is_a<ir::This>(obj)))
            return outer_of(*sym->owner);
        return this_ref();
    }
    return expr(obj);
}

Code Method::x_Select(const py::ref<ir::Select>& e) {
    auto s = e->sym;
    need_sym(s);
    if (is_a<ir::TypeName>(e->obj) || s->static_) {
        auto ci2 = p->model->info(or_empty(s->owner));
        if (!ci2) {
            str key = or_empty(s->owner) + "\x01" + s->name;
            if (!STATIC_FIELDS().contains(key)) throw Decline(S("library static ") + or_none(s->owner) + "." + s->name);
            return STATIC_FIELDS().at(key);
        }
        str code = py::percent(S("%s_%s"), ci2->cname, _ident(s->name));
        if (p->needs_clinit(ci2) && ci2 != ci) code = py::percent(S("(%s_clinit(), %s)"), ci2->cname, code);
        if (s->kind == "enumconst") return Code(code, ci2->cname + "*");
        py::ref<ir::FieldDecl> f;
        for (const auto& x : ci2->fields()) {
            if (x->sym && x->sym->name == s->name) {
                f = x;
                break;
            }
        }
        return Code(code, p->ct(f ? f->type : e->type));
    }
    auto [recv, rt] = receiver(e->obj, s);
    auto ci2 = p->model->info(or_empty(s->owner));
    if (!ci2) throw Decline(S("library field ") + or_none(s->owner) + "." + s->name);
    py::ref<ir::FieldDecl> f;
    for (const auto& x : ci2->fields()) {
        if (x->sym && x->sym->name == s->name) {
            f = x;
            break;
        }
    }
    str declared = p->ct(jmodel::erase(f ? f->type : e->type));
    str acc = py::percent(S("%s->%s"),
                          rt == ci2->cname + "*" ? recv : py::percent(S("((%s*)jrt_nn(%s))"), ci2->cname, recv), _ident(s->name));
    if (rt == ci2->cname + "*" && recv != "self" && !recv.startswith(S("self->")))
        acc = py::percent(S("((%s*)jrt_nn(%s))->%s"), ci2->cname, recv, _ident(s->name));
    str want = p->ct(e->type);
    if (declared != want && want.endswith(S("*")) && declared == OBJ) return Code(conv(acc, declared, want), want);
    return Code(acc, declared);
}

Code Method::x_Length(const py::ref<ir::Length>& e) {
    auto [a, _t] = expr(e->arr);
    return Code(py::percent(S("JLEN(%s)"), a), S("int32_t"));
}

Code Method::x_Index(const py::ref<ir::Index>& e) {
    auto [a, _t] = expr(e->arr);
    auto [i, it] = expr(ir::node_of(e->index));
    TypeP at = type_of(e->arr);
    str et = (at && at->kind == "array") ? p->ct(at->elem) : OBJ;
    str store = (et.endswith(S("*")) && et != STR && et != ARR && et != SB) ? S("jobject*") : et;
    str code = py::percent(S("JA(%s, %s, %s)"), store, a, conv(i, it, S("int32_t")));
    return Code(code, store);
}

Code Method::unboxed(const str& code, const str& ct, const TypeP& t) {
    if (ct.endswith(S("*")) && t && t->kind == "class" && t->name && BOX().contains(*t->name)) {
        str c = BOX().at(*t->name);
        return Code(py::percent(S("jrt_unbox_%s((jobject*)%s)"), c, code), CT_OF_CODE().at(c));
    }
    return Code(code, ct);
}

Code Method::x_Unary(const py::ref<ir::Unary>& e) {
    auto [x0, t0] = expr(e->x);
    str x = x0, t = t0;
    if (in2(e->op, "++", "--")) {
        if (t.endswith(S("*"))) throw Decline(S("++ on a box"));
        bool post = e->postfix && *e->postfix;
        return Code(post ? x + *e->op : *e->op + x, t);
    }
    auto ub = unboxed(x, t, type_of(e->x));
    x = std::get<0>(ub);
    t = std::get<1>(ub);
    if (eq(e->op, "!")) return Code(S("!") + x, S("bool"));
    return Code(or_none(e->op) + x, p->ct(e->type));
}

Code Method::x_Binary(const py::ref<ir::Binary>& e) {
    auto [l0, lt0] = expr(e->left);
    auto [r0, rt0] = expr(e->right);
    str l = l0, lt = lt0, r = r0, rt = rt0;
    const auto& op = e->op;
    if (in2(op, "==", "!=") && (lt.endswith(S("*")) || lt == "nullptr_t") && (rt.endswith(S("*")) || rt == "nullptr_t"))
        return Code(py::percent(S("(jobject*)%s %s (jobject*)%s"), l, *op, r), S("bool"));
    auto ul = unboxed(l, lt, type_of(e->left));
    l = std::get<0>(ul);
    lt = std::get<1>(ul);
    auto ur = unboxed(r, rt, type_of(e->right));
    r = std::get<0>(ur);
    rt = std::get<1>(ur);
    if (in2(op, "&&", "||")) return Code(py::percent(S("%s %s %s"), l, *op, r), S("bool"));
    str res = p->ct(e->type);
    if (is_bits_op(op)) {
        if (auto lb = as_a<ir::Binary>(e->left))
            if (!(lb->op == op)) l = py::percent(S("(%s)"), l);
        if (auto rb = as_a<ir::Binary>(e->right))
            if (!(rb->op == op)) r = py::percent(S("(%s)"), r);
    }
    if (in3(op, "<<", ">>", ">>>")) {
        int64_t width = lt == "int64_t" ? 64 : 32;
        NodeP lit = cpp03::_strip_paren(e->right);
        std::optional<int64_t> lv;
        if (auto li = as_a<ir::Lit>(lit)) lv = int_of(li->value);
        if (!(lv && 0 <= *lv && *lv < width)) r = py::percent(S("(%s & %d)"), r, width - 1);
        if (eq(op, ">>>")) {
            return Code(py::percent(S("jrt_ushr_%s(%s, %s)"), lt == "int64_t" ? S("J") : S("I"),
                                    (lt == "int32_t" || lt == "int64_t") ? l : py::percent(S("(int32_t)%s"), l), r),
                        res);
        }
        return Code(py::percent(S("%s %s %s"), l, *op, r), res);
    }
    if (in2(op, "/", "%") && is_integral(res)) {
        NodeP lit = cpp03::_strip_paren(e->right);
        std::optional<int64_t> lv;
        if (auto li = as_a<ir::Lit>(lit)) lv = int_of(li->value);
        if (!(lv && *lv != 0 && *lv != -1)) {
            return Code(py::percent(S("jrt_%s_%s(%s, %s)"), eq(op, "/") ? S("div") : S("rem"),
                                    res == "int64_t" ? S("J") : S("I"), l, r),
                        res);
        }
    }
    if (eq(op, "%") && (res == "float" || res == "double")) return Code(py::percent(S("(%s)fmod(%s, %s)"), res, l, r), res);
    bool cmp = in3(op, "==", "!=", "<") || in3(op, "<=", ">", ">=");
    return Code(py::percent(S("%s %s %s"), l, or_none(op), r), cmp ? S("bool") : res);
}

str Method::as_string(const NodeP& x) {
    auto [code, ct] = expr(x);
    const auto& codes = CODES();
    if (is_a<ir::Lit>(cpp03::_strip_paren(x)) && ct == STR) return code;
    if (codes.contains(ct)) {
        str c = codes.at(ct);
        if (c == "B" || c == "S") return py::percent(S("jrt_str_I((int32_t)%s)"), code);
        return py::percent(S("jrt_str_%s(%s)"), c, code);
    }
    if (ct == "nullptr_t") return S("jrt_str_O(NULL)");
    return py::percent(S("jrt_str_O((jobject*)%s)"), code);
}

Code Method::x_Concat(const py::ref<ir::Concat>& e) {
    py::list<str> parts;
    for (const NodeP& x : lst(e->parts)) parts.append(as_string(x));
    return Code(py::percent(S("jrt_concat(%d, %s)"), parts.size(), jn(parts)), STR);
}

Code Method::x_Assign(const py::ref<ir::Assign>& e) {
    auto [t, tt] = expr(e->target);
    auto [v0, vt0] = expr(e->value);
    str v = v0, vt = vt0;
    if (!e->op) return Code(py::percent(S("%s = %s"), t, conv(v, vt, tt)), tt);
    if (tt == STR && eq(e->op, "+"))
        return Code(py::percent(S("%s = jrt_concat(2, jrt_str_O((jobject*)%s), %s)"), t, t, as_string(e->value)), tt);
    NodeP tgt = cpp03::_strip_paren(e->target);
    bool plain = is_a<ir::Name>(tgt);
    if (!plain) {
        if (auto se = as_a<ir::Select>(tgt)) plain = is_a<ir::This>(se->obj) || is_a<ir::TypeName>(se->obj);
    }
    if (tt.endswith(S("*"))) throw Decline(S("compound assignment to a reference"));
    auto uv = unboxed(v, vt, type_of(e->value));
    v = std::get<0>(uv);
    vt = std::get<1>(uv);
    const str& op = *e->op;
    if ((op == "/" || op == "%") && is_integral(tt) && is_integral(vt)) {
        if (!plain) throw Decline(S("integer /= on a complex target"));
        str wide = (tt == "int64_t" || vt == "int64_t") ? S("J") : S("I");
        return Code(py::percent(S("%s = (%s)jrt_%s_%s(%s, %s)"), t, tt, op == "/" ? S("div") : S("rem"), wide, t, v), tt);
    }
    if (op == "<<" || op == ">>" || op == ">>>") {
        int64_t width = tt == "int64_t" ? 64 : 32;
        if (op == ">>>") {
            if (!plain) throw Decline(S(">>>= on a complex target"));
            return Code(py::percent(S("%s = (%s)jrt_ushr_%s(%s, (%s & %d))"), t, tt, tt == "int64_t" ? S("J") : S("I"),
                                    (tt == "int32_t" || tt == "int64_t") ? t : S("(int32_t)") + t, v, width - 1),
                        tt);
        }
        return Code(py::percent(S("%s %s= (%s & %d)"), t, op, v, width - 1), tt);
    }
    if (is_integral(tt) && (vt == "float" || vt == "double")) {
        if (!plain) throw Decline(S("integer op= double on a complex target"));
        return Code(py::percent(S("%s = (%s)%s(%s %s %s)"), t, tt, tt == "int64_t" ? S("jrt_d2l") : S("jrt_d2i"), t, op, v), tt);
    }
    if (op == "%" && (tt == "float" || tt == "double")) {
        if (!plain) throw Decline(S("%= on a complex target"));
        return Code(py::percent(S("%s = (%s)fmod(%s, %s)"), t, tt, t, v), tt);
    }
    return Code(py::percent(S("%s %s= %s"), t, op, v), tt);
}

Code Method::x_Cond(const py::ref<ir::Cond>& e) {
    str c = cond(e->cond);
    auto [a, at] = expr(e->then);
    auto [b, bt] = expr(e->orelse);
    str t = (e->type && e->type->kind != "null") ? p->ct(e->type) : (at != "nullptr_t" ? at : bt);
    return Code(py::percent(S("(%s ? %s : %s)"), c, conv(a, at, t), conv(b, bt, t)), t);
}

Code Method::x_Cast(const py::ref<ir::Cast>& e) {
    auto [x, xt] = expr(e->x);
    str ct = p->ct(e->type);
    if (ct == xt) return Code(x, ct);
    if (is_prim_c(ct) && is_prim_c(xt)) {
        if ((xt == "double" || xt == "float") && !(ct == "double" || ct == "float")) return Code(conv(x, xt, ct), ct);
        return Code(py::percent(S("(%s)%s"), ct, SIMPLE().match(x) ? x : py::percent(S("(%s)"), x)), ct);
    }
    if ((ct.endswith(S("*")) && xt.endswith(S("*"))) || xt == "nullptr_t") {
        if (is_sub(xt, ct)) return Code(xt == ct ? x : py::percent(S("(%s)%s"), ct, x), ct);
        if (K(e->type).kind == "class")
            return Code(py::percent(S("(%s)jrt_cast((jobject*)%s, %s)"), ct, x, p->class_ref(or_empty(e->type->name))), ct);
        return Code(py::percent(S("(%s)%s"), ct, x), ct);
    }
    return Code(conv(x, xt, ct), ct);
}

Code Method::x_InstanceOf(const py::ref<ir::InstanceOf>& e) {
    if (e->binding) throw Decline(S("instanceof pattern not lowered"));
    auto [x, _xt] = expr(e->x);
    if (K(e->test).kind == "array") return Code(py::percent(S("jrt_instanceof((jobject*)%s, &jrt_Array_class)"), x), S("bool"));
    return Code(py::percent(S("jrt_instanceof((jobject*)%s, %s)"), x, p->class_ref(or_empty(e->test->name))), S("bool"));
}

str Method::args(const Nodes& args0, const py::list<str>& params, const SymP& sym, bool lib) {
    py::list<NodeP> as = args0 ? args0->copy() : py::list<NodeP>();
    auto conv_expr = [&](const NodeP& a, const str& want) {
        auto [c, t] = expr(a);
        return conv(c, t, want);
    };
    bool packed_case = false;
    if (sym && sym->varargs && !params.empty()) {
        bool direct = false;
        if (as.size() == params.size()) {
            TypeP lt = type_of(as[as.size() - 1]);
            direct = lt && (lt->kind == "array" || lt->kind == "null");
        }
        packed_case = !direct;
    }
    if (packed_case) {
        if (lib) throw Decline(S("library varargs ") + sym->name);
        int64_t fixed = params.size() - 1;
        py::list<str> head;
        for (int64_t i = 0; i < std::min<int64_t>(fixed, as.size()); ++i) head.append(conv_expr(as[i], params[i]));
        const TypeP& last = sym->params[sym->params.size() - 1];
        str et = K(last).kind == "array" ? p->ct(jmodel::erase(last->elem)) : OBJ;
        str store = et.endswith(S("*")) ? S("jobject*") : et;
        py::list<str> rest;
        for (int64_t i = fixed; i < as.size(); ++i) rest.append(conv_expr(as[i], store));
        str packed;
        if (!rest.empty()) {
            packed = py::percent(S("jrt_array_of(sizeof(%s), %d, (%s[]){%s})"), store, rest.size(), store, jn(rest));
        } else {
            packed = py::percent(S("jrt_new_array(sizeof(%s), 0)"), store);
        }
        head.append(packed);
        return jn(head);
    }
    py::list<str> out;
    for (int64_t i = 0; i < as.size(); ++i) {
        str want = i < params.size() ? params[i] : S("");
        NodeP a0 = cpp03::_strip_paren(as[i]);
        Code c;
        if (is_a<ir::Lambda>(a0) || is_a<ir::MethodRef>(a0)) {
            c = closure(a0);
        } else {
            c = expr(as[i]);
        }
        out.append(conv(std::get<0>(c), std::get<1>(c), want));
    }
    return jn(out);
}

Code Method::lib_call(const py::ref<ir::Call>& e) {
    auto s = e->sym;
    str s_owner = s->owner ? *s->owner : (none_attr("rsplit"), str());
    str owner = jmodel::LIB.contains(s_owner) ? jmodel::LIB.at(s_owner) : after_dot(s_owner);
    if (s_owner == "java.util.Map.Entry") owner = S("MapEntry");
    if ((s_owner == "java.util.List" || s_owner == "java.util.Set" || s_owner == "java.util.Map") && s->name == "of") {
        py::list<str> vals;
        for (const NodeP& a : lst(e->args)) {
            auto [c, t] = expr(a);
            vals.append(conv(c, t, OBJ));
        }
        str tail;
        for (const str& v : vals) tail += S(", ") + v;
        return Code(py::percent(S("jrt_%s_of(%d%s)"), owner, vals.size(), tail), OBJ);
    }
    if (s_owner == "java.lang.String" && s->name == "format") {
        py::list<str> vals;
        const py::list<NodeP>& as = lst(e->args);
        for (int64_t i = 1; i < as.size(); ++i) {
            auto [c, t] = expr(as[i]);
            vals.append(conv(c, t, OBJ));
        }
        auto [fc, ft] = expr(as[0]);
        str f = conv(fc, ft, STR);
        str tail;
        for (const str& v : vals) tail += S(", ") + v;
        return Code(py::percent(S("jrt_String_format(%s, %d%s)"), f, vals.size(), tail), STR);
    }
    if (in3(std::optional<str>(s->name), "toString", "equals", "hashCode") &&
        py::len(s->params) == (s->name == "equals" ? 1 : 0) && !s->static_ && e->target && !is_a<ir::TypeName>(e->target) &&
        s_owner != "java.lang.String") {
        auto [recv, rt] = receiver(e->target, s);
        str fn = s->name == "toString" ? S("jrt_toString") : s->name == "equals" ? S("jrt_equals") : S("jrt_hashCode");
        py::list<str> as = {conv(recv, rt, OBJ)};
        for (const NodeP& a : lst(e->args)) {
            auto [c, t] = expr(a);
            as.append(conv(c, t, OBJ));
        }
        return Code(py::percent(S("%s(%s)"), fn, jn(as)),
                    s->name == "toString" ? STR : s->name == "equals" ? S("bool") : S("int32_t"));
    }
    if (s_owner == "java.lang.Comparable" && s->name == "compareTo") {
        auto [a, at] = expr(e->target);
        auto [b, bt] = expr(lst(e->args)[0]);
        return Code(py::percent(S("jrt_natural((jobject*)%s, %s)"), a, conv(b, bt, OBJ)), S("int32_t"));
    }
    py::list<str> params;
    for (const TypeP& t : s->params) {
        TypeP et = jmodel::erase(t);
        params.append(PRIM().contains(et->kind) ? p->ct(et)
                                                : (_code(et) == "Str" ? STR : (et->kind == "array" ? ARR : OBJ)));
    }
    // (StringBuilder's parameters: `_STR if x == _STR else x`, which changes nothing)
    py::list<str> cs;
    for (const TypeP& t : s->params) cs.append(_code(jmodel::erase(t)));
    str codes = py::join(S(""), cs);
    str name = py::percent(S("jrt_%s_%s_%s"), owner, s->name, codes);
    str as = args(e->args, params, s, true);
    str ret = p->ct(jmodel::erase(s->ret));
    if (ret.endswith(S("*")) && ret != STR && ret != ARR && ret != SB) ret = OBJ;
    if (s_owner == "java.lang.StringBuilder" && ret == OBJ) ret = SB;
    str code;
    if (s->static_ || is_a<ir::TypeName>(e->target)) {
        code = py::percent(S("%s(%s)"), name, as);
    } else {
        auto [recv, rt] = receiver(e->target, s);
        if (e->target && type_of(e->target) && type_of(e->target)->kind == "array" && s->name == "clone")
            return Code(py::percent(S("jrt_array_clone(%s)"), recv), ARR);
        str rwant = s_owner == "java.lang.String" ? STR : (owner == "StringBuilder" ? SB : (owner == "PrintStream" ? S("void*") : OBJ));
        py::list<str> all = {conv(recv, rt, rwant)};
        if (!as.empty()) all.append(as);
        code = py::percent(S("%s(%s)"), name, jn(all));
    }
    str want = p->ct(e->type);
    if (ret != want && want.endswith(S("*")) && ret == OBJ) return Code(conv(code, ret, want), want);
    if (ret != want && is_prim_c(want) && ret.endswith(S("*"))) return Code(conv(code, ret, want), want);
    return Code(code, ret);
}

Code Method::x_Call(const py::ref<ir::Call>& e) {
    auto s = e->sym;
    if (!s || s->kind != "method") throw Decline(S("call ") + (s ? s->kind : S("unresolved")));
    auto [mci, m2] = p->model->method_decl(s);
    auto eci = p->model->info(or_empty(s->owner));
    if (!m2 && eci && eq(eci->kind, "enum") && s->name == "values" && !nonempty(e->args))
        return Code(py::percent(S("%s_values()"), eci->cname), ARR);
    if (!m2) return lib_call(e);
    auto [ps, r, eps] = p->sig(mci, m2);
    str as = args(e->args, ps, s);
    str name = p->mname(or_empty(m2->name), eps);
    NodeP t = e->target;
    bool static__ = sym_static(m2->sym);
    str code;
    auto with_recv = [&](const str& recv) {
        py::list<str> all = {py::percent(S("(jobject*)%s"), recv)};
        if (!as.empty()) all.append(as);
        return jn(all);
    };
    if (static__ || is_a<ir::TypeName>(t)) {
        code = py::percent(S("%s_%s(%s)"), mci->cname, name, as);
    } else if (is_a<ir::Super>(t)) {
        auto [recv, _rt] = this_ref();
        code = py::percent(S("%s_%s(%s)"), mci->cname, name, with_recv(recv));
    } else {
        auto [recv, rt] = receiver(t, s);
        if (object_method_decl(m2)) {
            str fn = eq(m2->name, "toString") ? S("jrt_toString") : eq(m2->name, "equals") ? S("jrt_equals") : S("jrt_hashCode");
            code = py::percent(S("%s(%s)"), fn, with_recv(recv));
        } else if (eq(mci->kind, "interface")) {
            code = py::percent(S("%s_%s_i(%s)"), mci->cname, name, with_recv(recv));
        } else if (is_private(m2)) {
            code = py::percent(S("%s_%s(%s)"), mci->cname, name, with_recv(recv));
        } else {
            auto root = slot_owner(mci, name);
            code = py::percent(S("%s_%s_v(%s)"), root->cname, name, with_recv(recv));
        }
    }
    str want = p->ct(e->type);
    if (r != want && want.endswith(S("*")) && r.endswith(S("*")) && !is_sub(r, want)) return Code(conv(code, r, want), want);
    if (r != want && is_prim_c(want) && r.endswith(S("*"))) return Code(conv(code, r, want), want);
    return Code(code, r);
}

ClassInfoP Method::slot_owner(const ClassInfoP& ci2, const str& name) {
    ClassInfoP best = ci2;
    std::optional<str> q = ci2->base;
    while (truthy(q)) {
        auto b = p->model->info(*q);
        if (!b || eq(b->kind, "interface")) break;
        for (const Slot& sl : p->slots(b)) {
            if (sl.name == name) {
                best = b;
                break;
            }
        }
        q = b->base;
    }
    return best;
}

Code Method::x_New(const py::ref<ir::New>& e) {
    TypeP t = e->type;
    if (e->body) {
        if (!p->model->by_decl.contains(id_of(e->body))) throw py::KeyError("key not found");
        return new_class(p->model->by_decl.at(id_of(e->body)), e);
    }
    if (!t || t->kind != "class") throw Decline(S("new of ") + (t ? t->repr() : S("None")));
    auto ci2 = p->model->info(or_empty(t->name));
    if (!ci2) {
        str tname = or_empty(t->name);
        str owner = jmodel::LIB.contains(tname) ? jmodel::LIB.at(tname) : after_dot(tname);
        if (THROWABLES().contains(tname) || tname == "java.lang.Throwable") {
            const py::list<NodeP>& xs = lst(e->args);
            str msg = S("NULL"), cause = S("NULL");
            TypeP x0t = !xs.empty() ? type_of(xs[0]) : nullptr;
            if (xs.size() == 1 && x0t && x0t->kind == "class" && !eq(x0t->name, "java.lang.String")) {
                auto [c, ty] = expr(xs[0]);
                cause = conv(c, ty, OBJ);
            } else if (!xs.empty()) {
                auto [c, ty] = expr(xs[0]);
                msg = conv(c, ty, STR);
                if (xs.size() > 1) {
                    auto [c2, ty2] = expr(xs[1]);
                    cause = conv(c2, ty2, OBJ);
                }
            }
            return Code(py::percent(S("jrt_new_throwable(&jrt_%s_class, %s, %s)"), owner, msg, cause), OBJ);
        }
        py::list<TypeP> sps = e->sym ? e->sym->params : py::list<TypeP>();
        py::list<str> cs, params;
        for (const TypeP& x : sps) cs.append(_code(jmodel::erase(x)));
        for (const TypeP& x : sps) {
            TypeP ex = jmodel::erase(x);
            params.append(PRIM().contains(ex->kind) ? p->ct(ex) : (_code(ex) == "Str" ? STR : (ex->kind == "array" ? ARR : OBJ)));
        }
        str as = args(e->args, params, e->sym, true);
        str ret = owner == "StringBuilder" ? SB : (owner == "String" ? STR : OBJ);
        return Code(py::percent(S("jrt_%s_new_%s(%s)"), owner, py::join(S(""), cs), as), ret);
    }
    if (ci2->local || ci2->anon) return new_class(ci2, e);
    auto [_c, m2] = p->model->method_decl(e->sym);
    if (!m2) {
        for (const auto& x : ci2->methods())
            if (eq(x->kind, "ctor")) throw Decline(S("constructor of ") + ci2->cname);
    }
    py::list<str> params;
    if (m2)
        for (const NodeP& pn : lst(m2->params)) params.append(p->ct(prm(pn)->type));
    str as = args(e->args, params, e->sym);
    py::list<str> pre;
    if (ci2->needs_outer) pre.append(e->outer ? std::get<0>(expr(e->outer)) : std::get<0>(outer_of(ci2->outer->qname)));
    str name = m2 ? replace_first(p->ctor_name(ci2, m2), S("_init"), S("_new")) : py::percent(S("%s_new"), ci2->cname);
    if (!as.empty()) pre.append(as);
    return Code(py::percent(S("%s(%s)"), name, jn(pre)), ci2->cname + "*");
}

Code Method::new_class(const ClassInfoP& ci2, const py::ref<ir::New>& e) {
    py::list<str> pre;
    if (ci2->needs_outer) pre.append(std::get<0>(this_ref()));
    for (const auto& c : ci2->captures) {
        auto nm = std::make_shared<ir::Name>();
        nm->sym = std::make_shared<ir::Sym>(S("local"), std::get<0>(c));
        nm->type = std::get<1>(c);
        pre.append(std::get<0>(x_Name(nm)));
    }
    py::list<str> params;
    bool has_ctor = false;
    for (const auto& x : ci2->methods())
        if (eq(x->kind, "ctor")) has_ctor = true;
    str name;
    if (has_ctor) {
        auto [_c, m2] = p->model->method_decl(e->sym);
        if (m2)
            for (const NodeP& pn : lst(m2->params)) params.append(p->ct(prm(pn)->type));
        name = replace_first(p->ctor_name(ci2, m2), S("_init"), S("_new"));
    } else {
        auto b = truthy(ci2->base) ? p->model->info(*ci2->base) : ClassInfoP();
        if (b && !eq(b->kind, "interface") && e->sym) {
            auto [_bc, bm] = p->model->method_decl(e->sym);
            if (bm)
                for (const NodeP& pn : lst(bm->params)) params.append(p->ct(prm(pn)->type));
        }
        name = py::percent(S("%s_new"), ci2->cname);
    }
    str as = args(e->args, params, e->sym);
    if (!as.empty()) pre.append(as);
    return Code(py::percent(S("%s(%s)"), name, jn(pre)), ci2->cname + "*");
}

Code Method::x_NewArray(const py::ref<ir::NewArray>& e) {
    str et = p->ct(K(e->type).elem);
    str store = (et.endswith(S("*")) && et != STR && et != ARR && et != SB) ? S("jobject*") : et;
    if (e->init) {
        py::list<str> items;
        for (const NodeP& x : *e->init) {
            if (auto na = as_a<ir::NewArray>(x)) {
                if (!na->type) na->type = e->type->elem;
            }
            auto [code, xt] = expr(x);
            items.append(conv(code, xt, store));
        }
        if (items.empty()) return Code(py::percent(S("jrt_new_array(sizeof(%s), 0)"), store), ARR);
        return Code(py::percent(S("jrt_array_of(sizeof(%s), %d, (%s[]){%s})"), store, items.size(), store, jn(items)), ARR);
    }
    py::list<str> dims;
    for (const NodeP& d : lst(e->dims)) {
        auto [c, t] = expr(d);
        dims.append(conv(c, t, S("int32_t")));
    }
    if (dims.size() == 1) return Code(py::percent(S("jrt_new_array(sizeof(%s), %s)"), store, dims[0]), ARR);
    if (dims.size() == 2) {
        str inner = p->ct(K(e->type->elem).elem);
        inner = (inner.endswith(S("*")) && inner != STR && inner != ARR && inner != SB) ? S("jobject*") : inner;
        return Code(py::percent(S("jrt_new_array2(sizeof(%s), %s, %s)"), inner, dims[0], dims[1]), ARR);
    }
    throw Decline(py::percent(S("array of %d dimensions"), dims.size()));
}

Code Method::x_SwitchExpr(const py::ref<ir::SwitchExpr>&) { throw Decline(S("a switch expression inside an expression")); }
Code Method::x_ClassLit(const py::ref<ir::ClassLit>&) { throw Decline(S("class literal")); }
Code Method::x_TypeName(const py::ref<ir::TypeName>&) { throw Decline(S("a type as a value")); }

// ---- closures
Code Method::closure(const NodeP& e) {
    if (auto l = as_a<ir::Lambda>(e)) return x_Lambda(l);
    return x_MethodRef(as_a<ir::MethodRef>(e));
}

SamInfo Method::sam(const TypeP& t) {
    if (!t || t->kind != "class") throw Decline(S("lambda without a functional interface type"));
    auto sm = p->model->sam(or_empty(t->name));
    if (!sm) throw Decline(S("not a functional interface: ") + or_none(t->name));
    auto [name, ps, r] = *sm;
    auto ci2 = p->model->info(or_empty(t->name));
    if (ci2) {
        auto ms = p->iface_methods(*t->name);
        for (const IMethod& x : ms) {
            if (x.java == name) return SamInfo{*t->name, x.name, x.ps, x.r, ms};
        }
        throw py::StopIteration();
    }
    py::list<str> cps;
    for (const TypeP& x : ps) cps.append(p->ct(jmodel::erase(x)));
    str cr = p->ct(jmodel::erase(r));
    return SamInfo{or_empty(t->name), name, cps, cr, {IMethod{name, cps, cr, name, ps}}};
}

Code Method::x_Lambda(const py::ref<ir::Lambda>& e) {
    auto si = sam(e->type);
    const str& q = si.q;
    const py::list<str>& ps = si.ps;
    const str& r = si.r;
    if (si.ms.size() != 1) throw Decline(S("a lambda for an interface with default methods"));
    p->nlambda += 1;
    str name = py::percent(S("Lambda%d"), p->nlambda);
    // what the body reads from outside: locals and parameters of the enclosing code, and `this`
    py::set<str> inner;
    for (const NodeP& x : lst(e->params)) inner.add(_ident(need_sym(prm(x)->sym).name));
    py::list<NodeP> walked = ir::walk(e->body);
    for (const NodeP& n : walked) {
        if (auto lo = as_a<ir::Local>(n))
            if (lo->sym) inner.add(_ident(lo->sym->name));
    }
    py::list<Cap> caps;
    py::set<str> seen;
    bool uses_this = false;
    for (const NodeP& n : walked) {
        if (auto nm = as_a<ir::Name>(n)) {
            if (nm->sym) {
                str k = _ident(nm->sym->name);
                if (!inner.contains(k) && !seen.contains(k)) {
                    auto [code, ct] = x_Name(nm);
                    caps.append(Cap{k, code, ct});
                    seen.add(k);
                }
            }
        }
        bool this_use = is_a<ir::This>(n) || is_a<ir::Super>(n);
        if (auto ca = as_a<ir::Call>(n)) {
            if (!ca->target && ca->sym && !ca->sym->static_) this_use = true;
        }
        if (auto se = as_a<ir::Select>(n)) {
            if (is_a<ir::This>(se->obj)) this_use = true;
        }
        if (this_use) uses_this = true;
    }
    std::optional<str> this_code, this_ct;
    if (uses_this) {
        auto [tc, tt] = this_ref();
        this_code = tc;
        this_ct = tt;
    }
    py::list<str> fields = {S("\tjobject hdr_;")};
    for (const Cap& c : caps) fields.append(py::percent(S("\t%s cap_%s;"), c.ct, c.name));
    if (uses_this) fields.append(py::percent(S("\t%s cap_this;"), *this_ct));
    Method body(p, ci, nullptr);
    py::dict<str, Code> blam;
    for (const Cap& c : caps) blam.set(c.name, Code(S("self->cap_") + c.name, c.ct));
    if (uses_this) blam.set(S("#this"), Code(S("self->cap_this"), *this_ct));
    body.lam = blam;
    body.static_ = static_;
    body.ret = r;
    py::list<str> params = {S("jobject* self_")};
    py::list<str> entry = {py::percent(S("\tstruct %s* self = (struct %s*)self_;"), name, name), S("\t(void)self;")};
    const py::list<NodeP>& eparams = lst(e->params);
    for (int64_t i = 0; i < eparams.size(); ++i) {
        auto pr = prm(eparams[i]);
        const str& want = ps[i];
        str own = p->ct(pr->type);
        str n = _ident(need_sym(pr->sym).name);
        body.vars.set(n, own);
        if (own != want) {
            params.append(py::percent(S("%s %s_in"), want, n));
            entry.append(py::percent(S("\t%s %s = %s;"), own, n, body.conv(n + "_in", want, own)));
        } else {
            params.append(py::percent(S("%s %s"), want, n));
        }
    }
    py::list<str> out;
    if (auto blk = as_a<ir::Block>(e->body)) {
        bool has_try = false;
        for (const NodeP& n : ir::walk(e->body))
            if (is_a<ir::Try>(n)) has_try = true;
        body.vol = has_try;
        body.stmts(lst(blk->stmts), out, S("\t"));
    } else if (r != "void") {
        auto rs = std::make_shared<ir::Return>();
        rs->expr = e->body;
        body.stmt(rs, out, S("\t"));
    } else {
        auto es = std::make_shared<ir::ExprStmt>();
        es->expr = e->body;
        body.stmt(es, out, S("\t"));
    }
    auto qi = p->model->info(q);
    str vt = qi ? py::percent(S("struct %s_vt"), qi->cname) : py::percent(S("jrt_%s_vt"), after_dot(q));
    str cls_ref = p->class_ref(q);
    py::list<str> L = {py::percent(S("struct %s {"), name)};
    L.extend(fields);
    L.append(S("};"));
    L.append(py::percent(S("static %s %s_fn(%s) {"), r, name, jn(params)));
    L.extend(entry);
    L.extend(out);
    L.append(S("}"));
    L.append(py::percent(S("static const %s %s_vt = {{jrt_Object_toString, jrt_Object_equals, jrt_Object_hashCode}, %s_fn};"), vt, name, name));
    L.append(py::percent(S("static const jitab %s_itabs[] = {{%s, &%s_vt}};"), name, cls_ref, name));
    L.append(py::percent(S("static const jclass %s_class = {\"%s$$Lambda\", &jrt_Object_class, sizeof(struct %s), &jrt_Object_vt, JRT_ITABS(%s_itabs), 0};"),
                         name, ci->qname.strip(S("<>")), name, name));
    py::list<str> cparams;
    for (const Cap& c : caps) cparams.append(py::percent(S("%s c_%s"), c.ct, c.name));
    if (uses_this) cparams.append(py::percent(S("%s c_this"), *this_ct));
    str cp = jn(cparams);
    L.append(py::percent(S("static jobject* %s_new(%s) {"), name, cp.empty() ? S("void") : cp));
    L.append(py::percent(S("\tstruct %s* o = (struct %s*)jrt_alloc(&%s_class);"), name, name, name));
    for (const Cap& c : caps) L.append(py::percent(S("\to->cap_%s = c_%s;"), c.name, c.name));
    if (uses_this) L.append(S("\to->cap_this = c_this;"));
    L.append(S("\treturn (jobject*)o;"));
    L.append(S("}"));
    L.append(S(""));
    p->lambdas.extend(L);
    py::list<str> as;
    for (const Cap& c : caps) as.append(c.code);
    if (uses_this) as.append(*this_code);
    return Code(py::percent(S("%s_new(%s)"), name, jn(as)), OBJ);
}

Code Method::x_MethodRef(const py::ref<ir::MethodRef>& e) {
    auto s = e->sym;
    if (!s) throw Decline(S("unresolved method reference"));
    auto si = sam(e->type);
    auto sm = p->model->sam(si.q);
    py::list<TypeP> sam_ps = sm ? std::get<1>(*sm) : py::list<TypeP>();
    py::list<NodeP> params, names;
    for (int64_t i = 0; i < si.ps.size(); ++i) {
        TypeP t = i < sam_ps.size() ? sam_ps[i] : std::make_shared<ir::Type>(S("class"), jmodel::OBJECT);
        t = jmodel::erase(t);
        auto pr = std::make_shared<ir::Param>();
        pr->sym = std::make_shared<ir::Sym>(S("param"), py::percent(S("a%d"), i), std::nullopt, false, t);
        pr->type = t;
        pr->mods = py::list<str>();
        params.append(pr);
    }
    for (const NodeP& pn : params) {
        auto pr = as_a<ir::Param>(pn);
        auto nm = std::make_shared<ir::Name>();
        nm->sym = pr->sym;
        nm->type = pr->type;
        names.append(nm);
    }
    NodeP body;
    auto mk_lambda = [&](const NodeP& b) {
        auto lam_ = std::make_shared<ir::Lambda>();
        lam_->params = params;
        lam_->body = b;
        lam_->type = e->type;
        return lam_;
    };
    auto call_of = [&](const NodeP& target, const py::list<NodeP>& as) {
        auto c = std::make_shared<ir::Call>();
        c->target = target;
        c->sym = s;
        c->args = as;
        c->typeargs = py::list<TypeP>();
        c->type = s->ret;
        return c;
    };
    if (eq(e->mode, "new")) {
        TypeP t = e->target ? type_of(e->target) : nullptr;
        if (t && t->kind == "array") {
            auto na = std::make_shared<ir::NewArray>();
            na->elem = t->elem;
            na->dims = py::list<NodeP>{names[0]};
            na->init = std::nullopt;
            na->type = t;
            body = na;
        } else {
            auto nw = std::make_shared<ir::New>();
            nw->sym = s;
            nw->args = ref_args(names, s->params);
            nw->body = nullptr;
            nw->outer = nullptr;
            nw->type = t;
            body = nw;
        }
    } else if (s->static_) {
        auto tn = std::make_shared<ir::TypeName>();
        tn->type = std::make_shared<ir::Type>(S("class"), s->owner);
        body = call_of(tn, ref_args(names, s->params));
    } else if (is_a<ir::TypeName>(e->target)) {
        auto recv = std::make_shared<ir::Cast>();
        recv->x = names[0];
        recv->type = std::make_shared<ir::Type>(S("class"), s->owner);
        body = call_of(recv, ref_args(names.slice_from(1), s->params));
    } else {
        // the receiver is evaluated where the reference is made: held in a local the lambda captures
        Code rc = (!is_a<ir::This>(e->target) && !is_a<ir::Super>(e->target)) ? expr(e->target) : this_ref();
        str code = std::get<0>(rc), ct = std::get<1>(rc);
        temps += 1;
        str rname = py::percent(S("mr%d"), temps);
        pending.append(decl(ct, rname));
        vars.set(rname, ct);
        TypeP rt = type_of(e->target);
        auto nm = std::make_shared<ir::Name>();
        nm->sym = std::make_shared<ir::Sym>(S("local"), rname, std::nullopt, false, rt);
        nm->type = rt;
        auto lam_ = mk_lambda(call_of(nm, ref_args(names, s->params)));
        auto [got, gt] = x_Lambda(lam_);
        return Code(py::percent(S("(%s = %s, %s)"), rname, code, got), gt);
    }
    return x_Lambda(mk_lambda(body));
}

py::list<NodeP> Method::ref_args(const py::list<NodeP>& names, const py::list<TypeP>& ptypes) {
    py::list<NodeP> out;
    int64_t n = std::min<int64_t>(names.size(), ptypes.size());
    for (int64_t i = 0; i < n; ++i) {
        const NodeP& nm = names[i];
        const TypeP& t = ptypes[i];
        if (t && t->kind != "typevar" && t->kind != "wildcard" && !ir::same(type_of(nm), t)) {
            auto c = std::make_shared<ir::Cast>();
            c->x = nm;
            c->type = t;
            out.append(c);
        } else {
            out.append(nm);
        }
    }
    return out;
}

// ---------------------------------------------------------------------------------------------- entry points
str program(const py::list<NodeP>& units, const std::optional<str>& main) {
    auto prog = std::make_shared<Program>(units, main);
    return prog->write();
}

std::tuple<std::optional<str>, str> build(const py::list<NodeP>& units, const str& out) {
    str text;
    try {
        text = program(units);
    } catch (const Decline& e) {
        return {std::nullopt, S("Decline: ") + str(e.what())};
    }
    str src = py::os::path::join(out, S("program.c"));
    {
        auto f = py::open(src, S("w"), std::optional<str>(S("utf-8")));
        f.write(text);
        f.close();
    }
    str cc = py::os::path::isfile(CLANG) ? CLANG : S("gcc");
    str exe = py::os::path::join(out, S("program.exe"));
    py::list<str> argv = {cc};
    for (const str& f : FLAGS) argv.append(f);
    argv.append(S("-I"));
    argv.append(RUNTIME());
    argv.append(src);
    argv.append(py::os::path::join(RUNTIME(), S("jrt.c")));
    argv.append(S("-o"));
    argv.append(exe);
    py::subprocess::RunOptions o;
    o.text = false;
    o.stdout_ = py::subprocess::Stream::Capture;
    o.stderr_ = py::subprocess::Stream::Stdout;
    o.timeout = static_cast<int64_t>(600);   // subprocess.run(..., timeout=600): TimeoutExpired after 600 s
    auto r = py::subprocess::run(argv, o);
    if (r.returncode != 0) {
        str err = r.stdout_bytes ? r.stdout_bytes->decode("utf-8", "replace") : str();
        str first;
        bool found = false;
        for (const str& l : py::splitlines(err)) {
            if (l.contains(S("error"))) {
                first = l;
                found = true;
                break;
            }
        }
        if (!found) first = err.slice(0, 300);
        return {std::nullopt, S("does not compile: ") + first.slice(0, 400)};
    }
    return {exe, S("")};
}

}  // namespace codebreaker::translate::backends::c_java

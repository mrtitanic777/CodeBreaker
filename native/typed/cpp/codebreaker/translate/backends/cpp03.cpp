// codebreaker/translate/backends/cpp03.cpp -- typed C++ port of codebreaker.translate.backends.cpp03 (see cpp03.hpp).

#include "codebreaker/translate/backends/cpp03.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_map>

#include "codebreaker/java/emit/naming.hpp"
#include "codebreaker/java/emit/ownership.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/runtime.hpp"
#include "codebreaker/java/typemap.hpp"
#include "codebreaker/translate/desugar.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::backends::cpp03 {

namespace naming = codebreaker::java::emit::naming;
namespace ownership = codebreaker::java::emit::ownership;
namespace typemap = codebreaker::java::typemap;
namespace runtime = codebreaker::java::runtime;
namespace ledger = codebreaker::java::ledger;
namespace lower = codebreaker::translate::frontends::java::lower;

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
[[noreturn]] void not_iterable() { throw py::TypeError("'NoneType' object is not iterable"); }

template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

// the list a Nodes slot holds (TypeError for None, as iterating None is)
const py::list<NodeP>& lst(const Nodes& v) {
    if (!v) not_iterable();
    return *v;
}

// `x.kind` of a missing object
const ir::Type& K(const TypeP& t) {
    if (!t) none_attr("kind");
    return *t;
}
ir::Sym& need_sym(const SymP& s, const char* attr = "name") {
    if (!s) none_attr(attr);
    return *s;
}

// the node a body slot holds; a bare list of statements is not something the Python can read a line of
NodeP body_node(const NodeOrList& v, const char* attr) {
    if (std::holds_alternative<py::list<NodeP>>(v))
        throw py::AttributeError(std::string("'list' object has no attribute '") + attr + "'");
    return std::get<NodeP>(v);
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

str join_commas(const py::list<str>& parts) { return py::join(S(", "), parts); }

// re.sub(r'^const\s+', '', t).rstrip('&').strip()
str strip_const_ref(const str& t) {
    static const py::re::Pattern rx = py::re::compile(S("^const\\s+"));
    return rx.sub(S(""), t).rstrip(S("&")).strip();
}

bool in_set(const std::optional<str>& k, const py::set<str>& s) { return k && s.contains(*k); }

void bump(const py::dict<str, int64_t>& d, const str& k) { d[k] = d.get(k, int64_t(0)) + 1; }

// ---- tables
py::set<str> words(const char* text) {
    py::set<str> out;
    for (const str& w : py::split(S(text))) out.add(w);
    return out;
}

const py::set<str>& CPP_KEYWORDS() {
    static const py::set<str> s = words(R"KW(alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t
char32_t class compl const constexpr const_cast continue decltype default delete do double dynamic_cast else enum
explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq
nullptr operator or or_eq private protected public register reinterpret_cast return short signed sizeof static
static_assert static_cast struct switch template this thread_local throw true try typedef typeid typename union
unsigned using virtual void volatile wchar_t while xor xor_eq NULL min max)KW");
    return s;
}

// IR primitive kinds -> the Java names the transpiler's resolver keys on
const py::dict<str, str>& JAVA_PRIM() {
    static const py::dict<str, str> d = {{S("bool"), S("boolean")}, {S("i8"), S("byte")},    {S("i16"), S("short")},
                                         {S("u16"), S("char")},     {S("i32"), S("int")},    {S("i64"), S("long")},
                                         {S("f32"), S("float")},    {S("f64"), S("double")}, {S("void"), S("void")}};
    return d;
}

// java.lang.Math as C: the transpiler's table, but each function at Java's own precision (double),
// where the transpiler has the float forms (floorf, powf, ...) for the 360's float pipeline
const py::dict<str, str>& MATH() {
    static const py::dict<str, str> d = [] {
        py::dict<str, str> m = typemap::MATH_API_EMIT.copy();
        m[S("floor")] = S("floor");
        m[S("ceil")] = S("ceil");
        m[S("pow")] = S("pow");
        m[S("exp")] = S("exp");
        m[S("log")] = S("log");
        if (m.contains(S("PI"))) m.pop(S("PI"));
        return m;
    }();
    return d;
}

const py::set<str>& BITS() {
    static const py::set<str> s = {S("<<"), S(">>"), S("&"), S("|"), S("^")};
    return s;
}
const py::dict<str, str>& SATURATING() {
    static const py::dict<str, str> d = {{S("int"), S("j2xToInt")}, {S("long long"), S("j2xToLong")}};
    return d;
}
const py::set<str>& REFLECTION() {
    static const py::set<str> s = {S("Class*"), S("Constructor*")};   // left as written: the reflection pass rewrites them
    return s;
}
const py::set<str>& REFLECTIVE() {
    static const py::set<str> s = {S("newInstance"), S("forName"), S("getConstructor"), S("getDeclaredConstructor")};
    return s;
}
const py::dict<str, str>& BOXED_TEST() {
    static const py::dict<str, str> d = {{S("String"), S("isString")},
                                         {S("Integer"), S("isInteger")},
                                         {S("Character"), S("isCharacter")},
                                         {S("Boolean"), S("isBoolean")}};
    return d;
}
// X.class for a boxed or built-in type: the C++ type it is (as the transpiler's statement layer has it)
const py::dict<str, str>& CLASS_LIT_TYPE() {
    static const py::dict<str, str> d = {{S("Byte"), S("signed char")}, {S("Short"), S("short")},   {S("Integer"), S("int")},
                                         {S("Long"), S("long long")},   {S("Float"), S("float")},   {S("Double"), S("double")},
                                         {S("Character"), S("char")},   {S("Boolean"), S("bool")},  {S("String"), S("JavaString")},
                                         {S("Object"), S("JObject")}};
    return d;
}
const py::set<str>& CATCHES_NPE() {
    static const py::set<str> s = {S("java.lang.Exception"), S("java.lang.RuntimeException"), S("java.lang.Throwable"),
                                   S("java.lang.NullPointerException")};
    return s;
}
const py::dict<str, str>& UNBOX() {
    static const py::dict<str, str> d = {{S("int"), S("intValue")},          {S("short"), S("shortValue")},
                                         {S("signed char"), S("byteValue")}, {S("long long"), S("longValue")},
                                         {S("float"), S("floatValue")},      {S("double"), S("doubleValue")},
                                         {S("bool"), S("booleanValue")},     {S("unsigned char"), S("charValue")}};
    return d;
}
const py::set<str>& PURE_OWNERS() {
    static const py::set<str> s = {S("java.lang.String"),    S("java.lang.Math"),      S("java.lang.Integer"),
                                   S("java.lang.Float"),     S("java.lang.Double"),    S("java.lang.Long"),
                                   S("java.lang.Character"), S("java.lang.Boolean"),   S("java.lang.Class"),
                                   S("java.lang.reflect.Constructor")};   // (lookups: no state they change)
    return s;
}
const py::dict<str, str>& PRIM_ZERO() {
    static const py::dict<str, str> d = {{S("bool"), S("false")},      {S("int"), S("0")},           {S("long long"), S("0")},
                                         {S("short"), S("0")},         {S("float"), S("0")},         {S("double"), S("0")},
                                         {S("unsigned char"), S("0")}, {S("signed char"), S("0")},   {S("char"), S("0")},
                                         {S("unsigned int"), S("0")},  {S("long"), S("0")}};
    return d;
}

// ---- the patterns
const py::re::Pattern& COLLECTION() {
    static const py::re::Pattern rx = py::re::compile(S("(JList|JMap|JSet)<"));
    return rx;
}
const py::re::Pattern& SIMPLE() {   // a name or member chain: no grouping
    static const py::re::Pattern rx = py::re::compile(S("^[A-Za-z_][\\w]*(?:(?:->|\\.)[A-Za-z_]\\w*)*$"));
    return rx;
}
const py::re::Pattern& OWNED_CONTAINER() {
    static const py::re::Pattern rx = py::re::compile(S("^(JList|JSet)\\s*<\\s*([A-Za-z_][\\w:]*)\\s*\\*\\s*>$"));
    return rx;
}
const py::re::Pattern& FACTORY() {
    static const py::re::Pattern rx = py::re::compile(S("^(?:create|make|alloc|clone)[A-Za-z]"));
    return rx;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- types
JTypeRef jtype(const TypeP& t) {
    if (!t) return std::make_shared<jir::JType>(S("void"));
    auto [n, base] = t->dims();
    str name;
    if (JAVA_PRIM().contains(base->kind)) {
        name = JAVA_PRIM().at(base->kind);
    } else if (base->kind == S("class")) {
        name = base->simple();
    } else if (base->kind == S("typevar") || base->kind == S("wildcard")) {
        name = S("Object");
    } else if (base->kind == S("null")) {
        name = S("Object");
    } else {
        throw Decline(S("type ") + t->repr());
    }
    py::list<JTypeRef> args;
    if (base->kind == S("class")) {
        for (const TypeP& a : base->args) args.append(jtype(a));
    }
    return std::make_shared<jir::JType>(name, n, args);
}

py::list<std::tuple<str, str>> _split_params(const str& params) {
    py::list<str> out;
    int64_t depth = 0;
    std::u32string cur;
    for (char32_t ch : params.u32()) {
        if (ch == U',' && depth == 0) {
            out.append(str(cur));
            cur.clear();
            continue;
        }
        depth += ch == U'<';
        depth -= ch == U'>';
        cur.push_back(ch);
    }
    if (!str(cur).strip().empty()) out.append(str(cur));
    py::list<std::tuple<str, str>> res;
    static const py::re::Pattern rx = py::re::compile(S("(.*?)([A-Za-z_]\\w*)$"));
    for (const str& p0 : out) {
        str p = p0.strip();
        auto m = rx.match(p);
        if (!m) none_attr("group");
        res.append(std::make_tuple(m->group(1).strip(), m->group(2)));
    }
    return res;
}

bool is_pointer(const str& ctype) { return ctype.rstrip().endswith(S("*")); }

str _elem(const str& ctype) {
    static const py::re::Pattern rx = py::re::compile(S("\\s*JArray<(.*)>\\s*$"));
    auto m = rx.match(ctype);
    if (!m) throw Decline(S("index into ") + ctype);
    return m->group(1).strip();
}

// ---------------------------------------------------------------------------------------------- Context
Context::Context(const py::ref<jtypes::TypeResolver>& resolver_, const py::list<ClassRef>& classes_) : resolver(resolver_) {
    for (const ClassRef& c : classes_) classes[c->name] = c;
}

bool Context::is_game(const std::optional<str>& qualified) const {
    if (!qnames.empty()) return qualified && qnames.contains(*qualified);
    return classes.contains(after_dot(qualified ? *qualified : S("")));
}

static str param_key(const JTypeRef& jt) { return jt->name + py::repeat(S("[]"), jt->array_dims); }

MethodRef Context::old_ctor(const str& simple, const py::list<TypeP>& params) const {
    if (!classes.contains(simple)) return nullptr;
    ClassRef c = classes.at(simple);
    py::list<str> want;
    for (const TypeP& p : params) want.append(param_key(jtype(p)));
    for (const MethodRef& m : c->constructors()) {
        py::list<str> got;
        for (const auto& x : m->params) got.append(param_key(x->jtype));
        if (got == want) return m;
    }
    return nullptr;
}

ClassRef Context::old_class(const str& simple) const {
    return classes.contains(simple) ? classes.at(simple) : nullptr;
}

MethodRef Context::old_method(const str& owner_in, const str& name, const py::list<TypeP>& params) const {
    py::set<str> seen;
    std::optional<str> owner = owner_in;
    while (owner && py::truthy(*owner) && !seen.contains(*owner)) {
        seen.add(*owner);
        if (!classes.contains(*owner)) return nullptr;
        ClassRef c = classes.at(*owner);
        py::list<str> want;
        for (const TypeP& p : params) want.append(param_key(jtype(p)));
        for (const MethodRef& m : c->methods) {
            if (m->name == name && !m->is_ctor && py::len(m->params) == py::len(params)) {
                py::list<str> got;
                for (const auto& p : m->params) got.append(param_key(p->jtype));
                if (got == want) return m;
            }
        }
        for (const MethodRef& m : c->methods) {   // erased generics / raw types: by arity
            if (m->name == name && !m->is_ctor && py::len(m->params) == py::len(params)) return m;
        }
        owner = c->parent;
    }
    return nullptr;
}

std::optional<str> Context::field_ctype(const str& owner, const str& name) const {
    JTypeRef jt = resolver->member_field_type(owner, name);
    if (!jt) return std::nullopt;
    if (jt->name == S("byte") && !jt->is_array() &&
        typemap::UNSIGNED_BYTE_SCALAR_FIELDS.contains(std::make_tuple(owner, name)))
        return S("unsigned char");
    return resolver->field_type(jt);
}

// ---------------------------------------------------------------------------------------------- Writer
Writer::Writer(const py::ref<Context>& ctx_, const py::ref<ir::ClassDecl>& cls_, const py::ref<ir::MethodDecl>& m_,
               const MethodRef& old_)
    : ctx(ctx_), r(ctx_->resolver), cls(cls_), cname(cls_->name ? *cls_->name : S("None")), m(m_), old(old_) {
    if (old) {
        for (const auto& pn : _split_params(ownership::params_str(old, *r))) vars[std::get<1>(pn)] = std::get<0>(pn);
    }
    if (cls->extends) parent = cls->extends->simple();
    ret = old ? ownership::return_cpp_type(old, *r) : S("void");
    // the no-GC ownership cleanups run in methods (as the transpiler's did), not constructors
    owning = old && m && m->kind && *m->kind == S("method");
}

// ------------------------------------------------------------------ types
str Writer::ctype(const TypeP& t, const str& context) {
    JTypeRef jt = jtype(t);
    if (!jt->is_array() && r->values.contains(jt->name)) throw Decline(S("value type ") + jt->name);
    return r->cpp_type(jt, context);
}

// ------------------------------------------------------------------ statements
py::list<str> Writer::body() {
    if (!m) none_attr("body");
    if (std::holds_alternative<NodeP>(m->body) && !std::get<NodeP>(m->body)) throw Decline(S("no body"));
    NodeP b = body_node(m->body, "stmts");
    auto blk = as_a<ir::Block>(b);
    if (!blk) throw py::AttributeError("'" + b->class_name().utf8() + "' object has no attribute 'stmts'");
    py::list<str> out;
    stmts(lst(blk->stmts), out, S(""));
    return out;
}

void Writer::stmts(const py::list<NodeP>& ss, const py::list<str>& out, const str& ind) {
    for (int64_t i = 0; i < py::len(ss); ++i) {
        py::list<NodeP> saved = after;
        after = ss.slice_from(i + 1);
        try {
            stmt(ss[i], out, ind);
        } catch (...) {
            after = saved;
            throw;
        }
        after = saved;
    }
}

void Writer::block(const NodeP& s, const py::list<str>& out, const str& ind) {
    py::dict<str, str> saved = vars.copy();
    if (auto b = as_a<ir::Block>(s)) stmts(lst(b->stmts), out, ind + "\t");
    else stmt(s, out, ind + "\t");
    vars = saved;
}

void Writer::block(const NodeOrList& s, const py::list<str>& out, const str& ind) {
    py::dict<str, str> saved = vars.copy();
    if (std::holds_alternative<py::list<NodeP>>(s)) throw Decline(S("statement list"));
    NodeP n = std::get<NodeP>(s);
    if (auto b = as_a<ir::Block>(n)) stmts(lst(b->stmts), out, ind + "\t");
    else stmt(n, out, ind + "\t");
    vars = saved;
}

void Writer::loop_body(const NodeOrList& body, const py::list<str>& out, const str& ind) {
    std::optional<str> lab = cont_next;
    cont_next = std::nullopt;
    if (!lab) {
        block(body, out, ind);
        return;
    }
    out.append(ind + "\t{");
    block(body, out, ind + "\t");
    out.append(ind + "\t}");
    out.append(ind + "\t" + *lab + "_cont:;");
}

void Writer::loop_body(const NodeP& body, const py::list<str>& out, const str& ind) {
    loop_body(NodeOrList(body), out, ind);
}

void Writer::stmt(const NodeOrList& s, const py::list<str>& out, const str& ind) {
    if (std::holds_alternative<py::list<NodeP>>(s)) throw Decline(S("statement list"));
    stmt(std::get<NodeP>(s), out, ind);
}

void Writer::stmt(const NodeP& s, const py::list<str>& out, const str& ind) {
    bool loop = is_a<ir::While>(s) || is_a<ir::DoWhile>(s) || is_a<ir::For>(s) || is_a<ir::ForEach>(s) || is_a<ir::Switch>(s);
    loops += loop;
    py::list<str> saved = pending;
    pending = py::list<str>();
    bool saved_order = order;
    order = true;
    py::dict<int64_t, Code> saved_subst = subst.copy();   // temporaries belong to this rendering of the statement
    py::list<str> sub;                                      // (a finally body is written twice)
    py::list<str> decls;
    auto restore = [&]() {
        loops -= loop;
        decls = pending;
        pending = saved;
        order = saved_order;
        subst = saved_subst;
    };
    try {
        _stmt(s, sub, ind);
    } catch (...) {
        restore();
        throw;
    }
    restore();
    for (const str& d : decls) out.append(ind + d);       // the statement's ordered-operand temporaries
    out.extend(sub);
}

void Writer::_stmt(const NodeP& s, const py::list<str>& out, const str& ind) {
    if (is_a<ir::Block>(s)) {
        out.append(ind + "{");
        block(s, out, ind);
        out.append(ind + "}");
    } else if (auto lb = as_a<ir::Labeled>(s)) {
        std::optional<str> lab = lb->label;
        NodeP lbody = body_node(lb->body, "children");
        bool any_continue = false, any_break = false;
        for (const NodeP& n : ir::walk(lbody)) {
            if (auto c = as_a<ir::Continue>(n)) {
                if (c->label == lab) any_continue = true;
            } else if (auto b = as_a<ir::Break>(n)) {
                if (b->label == lab) any_break = true;
            }
        }
        if (any_continue) {
            if (!(is_a<ir::While>(lbody) || is_a<ir::DoWhile>(lbody) || is_a<ir::For>(lbody) || is_a<ir::ForEach>(lbody)))
                throw Decline(S("continue to a label that is not a loop"));
            cont_next = lab;
        }
        stmt(lb->body, out, ind);
        if (any_break) out.append(ind + py::percent(S("%s_brk:;"), lab));
    } else if (auto sy = as_a<ir::Sync>(s)) {   // as the text path: the lock is dropped, and marked
        out.append(ind + py::percent(S("/* J2X-REVIEW[synchronized-dropped]: was synchronized(%s) -- add a lock if this "
                                       "state is touched by another thread (network/pool) */ {"),
                                     _comment(std::get<0>(expr(sy->lock)))));
        block(sy->body, out, ind);
        out.append(ind + "}");
    } else if (auto lo = as_a<ir::Local>(s)) {
        str name = this->name(need_sym(lo->sym).name);
        if (vars.contains(name)) throw Decline(S("shadowed local ") + name);
        str ct = ctype(lo->type);
        vars[name] = ct;
        if (lo->init) {
            review(lo->init, out, ind);
            hoist_owned_args(lo->init, out, ind);
            auto [code, it] = expr(lo->init);
            out.append(ind + py::percent(S("%s %s = %s;"), ct, name, unbox(coerce(code, it, ct), it, ct)));
            free_owned_return(lo, ct, name, out, ind);
        } else {
            out.append(ind + py::percent(S("%s %s;"), ct, name));
        }
    } else if (auto es = as_a<ir::ExprStmt>(s)) {
        review(es->expr, out, ind);
        hoist_owned_args(es->expr, out, ind);
        free_owned_reassign(es->expr, out, ind);
        std::optional<str> kept = kept_call(es->expr);
        if (kept && py::truthy(*kept)) out.append(ind + *kept);
        else out.append(ind + std::get<0>(expr(es->expr)) + ";");
    } else if (auto iff = as_a<ir::If>(s)) {
        out.append(ind + py::percent(S("if(%s) {"), cond(iff->cond)));
        block(iff->then, out, ind);
        NodeOrList e = iff->orelse;
        while (std::holds_alternative<NodeP>(e) && is_a<ir::If>(std::get<NodeP>(e))) {
            auto ei = as_a<ir::If>(std::get<NodeP>(e));
            out.append(ind + py::percent(S("} else if(%s) {"), cond(ei->cond)));
            block(ei->then, out, ind);
            e = ei->orelse;
        }
        if (std::holds_alternative<py::list<NodeP>>(e) || std::get<NodeP>(e)) {
            out.append(ind + "} else {");
            block(e, out, ind);
        }
        out.append(ind + "}");
    } else if (auto wh = as_a<ir::While>(s)) {
        out.append(ind + py::percent(S("while(%s) {"), cond(wh->cond)));
        loop_body(wh->body, out, ind);
        out.append(ind + "}");
    } else if (auto dw = as_a<ir::DoWhile>(s)) {
        out.append(ind + "do {");
        loop_body(dw->body, out, ind);
        out.append(ind + py::percent(S("} while(%s);"), cond(dw->cond)));
    } else if (auto fo = as_a<ir::For>(s)) {
        const py::list<NodeP>& inits = lst(fo->init);
        if (py::len(inits) > 1) {
            bool all_local = true;
            for (const NodeP& i : inits) all_local = all_local && is_a<ir::Local>(i);
            if (!all_local) throw Decline(S("for init"));
        }
        str init = S("");
        if (!inits.empty()) {
            if (is_a<ir::Local>(inits[0])) {
                auto l0 = as_a<ir::Local>(inits[0]);
                str ct = ctype(l0->type);
                py::list<str> parts;
                for (const NodeP& l_ : inits) {
                    auto l = as_a<ir::Local>(l_);
                    str n = name(need_sym(l->sym).name);
                    if (vars.contains(n)) throw Decline(S("shadowed loop variable ") + n);
                    vars[n] = ct;
                    if (l->init) parts.append(py::percent(S("%s = %s"), n, std::get<0>(expr(l->init))));
                    else parts.append(n);
                }
                init = py::percent(S("%s %s"), ct, join_commas(parts));
            } else {
                py::list<str> parts;
                for (const NodeP& i : inits) {
                    auto x = as_a<ir::ExprStmt>(i);
                    if (!x) {
                        if (!i) none_attr("expr");
                        throw py::AttributeError("'" + i->class_name().utf8() + "' object has no attribute 'expr'");
                    }
                    parts.append(std::get<0>(expr(x->expr)));
                }
                init = join_commas(parts);
            }
        }
        str c = fo->cond ? cond(fo->cond) : S("");
        py::list<str> ups;
        for (const NodeP& u : lst(fo->update)) ups.append(std::get<0>(expr(u)));
        str upd = join_commas(ups);
        out.append(ind + py::percent(S("for (%s; %s; %s) {"), init, c, upd));
        loop_body(fo->body, out, ind);
        out.append(ind + "}");
        for (const NodeP& l : inits) {   // the loop variable's scope ends with the loop
            if (auto ll = as_a<ir::Local>(l)) vars.pop(name(need_sym(ll->sym).name), str(""));
        }
    } else if (is_a<ir::Return>(s) && !fin.empty()) {   // out through the finally
        auto rt = as_a<ir::Return>(s);
        int64_t n = std::get<0>(fin[-1]);
        if (rt->expr) {
            auto [code, it] = expr(rt->expr);
            out.append(ind + py::percent(S("j2xRet%d = %s;"), n, unbox(coerce(code, it, ret), it, ret)));
        }
        out.append(ind + py::percent(S("j2xFin%d = 1; goto j2xFinally%d;"), n, n));
    } else if ((is_a<ir::Break>(s) || is_a<ir::Continue>(s)) && !fin.empty() &&
               !((is_a<ir::Break>(s) ? as_a<ir::Break>(s)->label : as_a<ir::Continue>(s)->label) &&
                 py::truthy(*(is_a<ir::Break>(s) ? as_a<ir::Break>(s)->label : as_a<ir::Continue>(s)->label))) &&
               loops == std::get<1>(fin[-1])) {
        int64_t n = std::get<0>(fin[-1]);
        out.append(ind + py::percent(S("j2xFin%d = %d; goto j2xFinally%d;"), n, int64_t(is_a<ir::Break>(s) ? 2 : 3), n));
    } else if (auto rt = as_a<ir::Return>(s)) {
        if (rt->expr) {
            review(rt->expr, out, ind);
            hoist_owned_args(rt->expr, out, ind);
            auto [code, it] = expr(rt->expr);
            out.append(ind + py::percent(S("return %s;"), unbox(coerce(code, it, ret), it, ret)));
        } else {
            out.append(ind + (two_phase ? "return this;" : "return;"));
        }
    } else if (auto br = as_a<ir::Break>(s)) {
        if (br->label && py::truthy(*br->label)) out.append(ind + "goto " + *br->label + "_brk;");
        else out.append(ind + "break;");
    } else if (auto co = as_a<ir::Continue>(s)) {
        if (co->label && py::truthy(*co->label)) out.append(ind + "goto " + *co->label + "_cont;");
        else out.append(ind + "continue;");
    } else if (auto th = as_a<ir::Throw>(s)) {
        out.append(ind + py::percent(S("throw %s;"), std::get<0>(expr(th->expr))));
    } else if (auto sw = as_a<ir::Switch>(s)) {
        NodeP se = sw->expr;
        if (auto p = as_a<ir::Paren>(se)) se = p->x;
        auto [x, xt] = expr(se);
        if (xt.contains(S("JavaString"))) throw Decline(S("switch on a string"));
        out.append(ind + py::percent(S("switch(%s) {"), x));
        for (const NodeP& c_ : lst(sw->cases)) {
            auto c = as_a<ir::Case>(c_);
            if (!c) none_attr("labels");
            for (const NodeP& lab : lst(c->labels)) out.append(ind + py::percent(S("case %s:"), std::get<0>(expr(lab))));
            if (c->default_ && *c->default_) out.append(ind + "default:");
            out.append(ind + "{");
            for (const NodeP& b : lst(c->body)) stmt(b, out, ind + "\t");
            out.append(ind + "}");
        }
        out.append(ind + "}");
    } else if (auto tr = as_a<ir::Try>(s)) {
        if (tr->resources && !tr->resources->empty()) throw Decline(S("try-with-resources"));
        if (!tr->final) {
            try_catch(tr, out, ind);
        } else {
            // finally: run on the way out by an exception (catch (...), then rethrow) and on the way out
            // normally (after the try). A return, break or continue out of the try would skip the second
            // copy, so such a try is declined until phase 2's exceptions pass.
            py::list<NodeP> parts{tr->body};
            for (const NodeP& c : lst(tr->catches)) {
                auto ct = as_a<ir::Catch>(c);
                if (!ct) none_attr("body");
                parts.append(ct->body);
            }
            if (_leaves(parts)) {
                finally_with_jumps(tr, out, ind);
                return;
            }
            out.append(ind + "try {");
            try_catch(tr, out, ind + "\t");
            out.append(ind + "} catch (...) {");
            block(tr->final, out, ind);
            out.append(ind + "\tthrow;");
            out.append(ind + "}");
            out.append(ind + "{");
            block(tr->final, out, ind);
            out.append(ind + "}");
        }
    } else if (is_a<ir::Empty>(s)) {
        out.append(ind + ";");
    } else if (is_a<ir::CtorCall>(s)) {
        throw Decline(S("constructor call not first"));
    } else {
        throw Decline(S("statement ") + (s ? s->class_name() : S("NoneType")));
    }
}

void Writer::finally_with_jumps(const py::ref<ir::Try>& s, const py::list<str>& out, const str& ind) {
    if (!fin.empty()) throw Decline(S("a finally with jumps inside another"));
    py::list<NodeP> bodies{s->body};
    for (const NodeP& c : lst(s->catches)) {
        auto ct = as_a<ir::Catch>(c);
        if (!ct) none_attr("body");
        bodies.append(ct->body);
    }
    for (const NodeP& b : bodies) {
        for (const NodeP& x : ir::walk(b)) {
            if (auto bk = as_a<ir::Break>(x)) {
                if (bk->label && py::truthy(*bk->label)) throw Decline(S("a labelled jump out of a finally"));
            } else if (auto ct = as_a<ir::Continue>(x)) {
                if (ct->label && py::truthy(*ct->label)) throw Decline(S("a labelled jump out of a finally"));
            }
        }
    }
    fin_count += 1;
    int64_t n = fin_count;
    out.append(ind + "{");
    out.append(ind + py::percent(S("\tint j2xFin%d = 0;"), n));
    if (ret != S("void") && !two_phase) out.append(ind + py::percent(S("\t%s j2xRet%d;"), strip_const_ref(ret), n));
    fin.append(std::make_tuple(n, loops));
    try {
        out.append(ind + "\ttry {");
        try_catch(s, out, ind + "\t\t");
        out.append(ind + "\t} catch (...) {");
        block(s->final, out, ind + "\t");
        out.append(ind + "\t\tthrow;");
        out.append(ind + "\t}");
    } catch (...) {
        fin.pop();
        throw;
    }
    fin.pop();
    out.append(ind + py::percent(S("\tj2xFinally%d:;"), n));
    out.append(ind + "\t{");
    block(s->final, out, ind + "\t");
    out.append(ind + "\t}");
    // when the try and every catch end in a return or throw, the finally is always left by the return:
    // say so without a test, or the compiler sees a path that returns nothing (MSVC C4715)
    bool always = _abrupt(s->body);
    if (always) {
        for (const NodeP& c : lst(s->catches)) {
            auto ct = as_a<ir::Catch>(c);
            if (!_abrupt(ct->body)) {
                always = false;
                break;
            }
        }
    }
    str cnd = always ? S("") : py::percent(S("if (j2xFin%d == 1) "), n);
    if (two_phase) out.append(ind + py::percent(S("\t%sreturn this;"), cnd));
    else if (ret == S("void")) out.append(ind + py::percent(S("\t%sreturn;"), cnd));
    else out.append(ind + py::percent(S("\t%sreturn j2xRet%d;"), cnd, n));
    if (loops) {
        out.append(ind + py::percent(S("\tif (j2xFin%d == 2) break;"), n));
        out.append(ind + py::percent(S("\tif (j2xFin%d == 3) continue;"), n));
    }
    out.append(ind + "}");
}

void Writer::try_catch(const py::ref<ir::Try>& s, const py::list<str>& out, const str& ind) {
    if (!s->catches || s->catches->empty()) {
        out.append(ind + "{");
        block(s->body, out, ind);
        out.append(ind + "}");
        return;
    }
    bool npe_here = false;
    for (const NodeP& c_ : lst(s->catches)) {
        auto c = as_a<ir::Catch>(c_);
        if (!c) none_attr("types");
        if (!c->types) not_iterable();
        for (const TypeP& t : *c->types) {
            if (K(t).kind == S("class") && in_set(t->name, CATCHES_NPE())) npe_here = true;
        }
    }
    out.append(ind + "try {");
    npe += npe_here;
    try {
        block(s->body, out, ind);
    } catch (...) {
        npe -= npe_here;
        throw;
    }
    npe -= npe_here;
    for (const NodeP& c_ : lst(s->catches)) {
        auto c = as_a<ir::Catch>(c_);
        if (!c->types) not_iterable();
        if (py::len(*c->types) != 1) throw Decline(S("multi-catch"));
        str ct = ctype((*c->types)[0]);
        str nm = name(need_sym(c->sym).name);
        if (vars.contains(nm)) throw Decline(S("shadowed catch variable"));
        vars[nm] = ct;
        bool used = false;
        for (const NodeP& n : ir::walk(c->body)) {
            if (auto nn = as_a<ir::Name>(n)) {
                if (need_sym(nn->sym).name == c->sym->name) {
                    used = true;
                    break;
                }
            }
        }
        out.append(ind + (used ? py::percent(S("} catch (%s %s) {"), ct, nm) : py::percent(S("} catch (%s) {"), ct)));   // C4101
        block(c->body, out, ind);
        vars.pop(nm, str(""));
    }
    out.append(ind + "}");
}

void Writer::note(const str& category, const str& severity, const str& rule, const str& text) {
    if (ctx->ledger) {
        int64_t line = m ? m->line : 0;   // a static block or field initialiser has no method
        ctx->ledger->add(cname + ".cpp", category, text, line, severity, S("emit"), rule);
    }
}

std::optional<str> Writer::kept_call(const NodeP& e_) {
    NodeP e = e_;
    while (auto p = as_a<ir::Paren>(e)) e = p->x;
    auto call = as_a<ir::Call>(e);
    if (npe || !call || !call->target || !call->sym) return std::nullopt;
    NodeP t = call->target;
    while (auto p = as_a<ir::Paren>(t)) t = p->x;
    if (auto nw = as_a<ir::New>(t)) {
        if (!nw->body && !nw->outer && nw->type && nw->type->kind == S("class") && ctx->is_game(nw->type->name) &&
            call->sym->name != S("start") && call->sym->name != S("run")) {
            str name = jtype(nw->type)->name;
            if (r->thread_like.contains(name) || r->values.contains(name) || r->extends_value_container(name) ||
                !r->known.contains(name))
                return std::nullopt;
            str code = std::get<0>(expr(call));   // the call as written, for its method and arguments
            static const str fmt = S("\\(?new %s\\((.*?)\\)\\)?->(\\w+)\\((.*)\\)$");
            auto m2 = py::re::match(py::percent(fmt, py::re::escape(name)), code);
            if (!m2) return std::nullopt;
            str tmp = py::percent(S("_j2xleak%d"), leaks);
            leaks += 1;
            str decl = py::truthy(m2->group(1)) ? py::percent(S("%s %s(%s);"), name, tmp, m2->group(1))
                                                 : py::percent(S("%s %s;"), name, tmp);
            note(ledger::Category::RULE, ledger::Severity::MED, S("leak-temp-stackified"),
                 py::percent(S("leak fixed: throwaway `(new %s(...))->%s(...)` -> scoped stack object (Java GC temp "
                              "leaked every call)"),
                             name, m2->group(2)));
            return py::percent(S("{ %s %s.%s(%s); }"), decl, tmp, m2->group(2), m2->group(3));
        }
    }
    if (is_a<ir::Index>(t)) {
        auto [recv, rt] = expr(t);
        if (!is_pointer(rt)) return std::nullopt;
        str code = std::get<0>(expr(call));
        note(ledger::Category::RISK, ledger::Severity::MED, S("null-deref-guard"),
             S("null-deref guard inserted before list-indexed call (an unregistered/NULL slot would be "
               "a virtual call through NULL -- undefined behaviour in C++)"));
        return py::percent(S("if (%s != NULL) { %s; }"), recv, code);
    }
    return std::nullopt;
}

// ------------------------------------------------------------------ the tree walks of the ownership rules
namespace {

// (node, its ancestors nearest first) for every node under `node`, pre-order; the children of a node are
// read after the visitor has run on it (as the Python generator does)
void with_parents(const NodeP& node, const py::list<NodeP>& chain, const std::function<void(const NodeP&, const py::list<NodeP>&)>& visit) {
    visit(node, chain);
    if (!node) none_attr("children");
    py::list<NodeP> nc;
    nc.append(node);
    nc.extend(chain);
    for (const NodeP& c : node->children()) with_parents(c, nc, visit);
}

// The ancestor chains of every reference to the local `sym` in `stmts`.
py::list<py::list<NodeP>> uses(const py::list<NodeP>& stmts, const SymP& sym) {
    py::list<py::list<NodeP>> out;
    for (const NodeP& s : stmts) {
        with_parents(s, py::list<NodeP>(), [&](const NodeP& n, const py::list<NodeP>& chain) {
            auto nm = as_a<ir::Name>(n);
            if (nm && nm->sym && nm->sym->name == need_sym(sym).name) out.append(chain);   // by name: a lowered use has its own Symbol
        });
    }
    return out;
}

// A container use that lets no element outlive it: x.size() / x.isEmpty(), or x.get(i) used only as
// the receiver of a call or a field read (through casts and parentheses).
bool transient_use(const py::list<NodeP>& chain) {
    if (chain.empty()) return false;
    auto c0 = as_a<ir::Call>(chain[0]);
    if (!c0 || !c0->sym) return false;
    const str& name = c0->sym->name;
    if (name == S("size") || name == S("isEmpty")) return true;
    if (name != S("get")) return false;
    int64_t k = 1;
    while (k < py::len(chain) && (is_a<ir::Paren>(chain[k]) || is_a<ir::Cast>(chain[k]))) k++;
    if (k >= py::len(chain)) return false;
    NodeP up = chain[k], below = chain[k - 1];
    if (auto uc = as_a<ir::Call>(up)) {
        if (uc->target == below) return true;
    }
    if (auto us = as_a<ir::Select>(up)) return us->obj == below;
    return false;
}

}  // namespace

// ------------------------------------------------------------------ no-GC ownership (Java's collector, by hand)
const py::set<str>& Writer::owned_scalar() {
    if (!_owned_scalar) {
        ClassRef old_cls = ctx->classes.contains(cname) ? ctx->classes.at(cname) : ClassRef();
        py::dict<str, bool> owned = old_cls ? ownership::owned_pointers(old_cls, *r) : py::dict<str, bool>();
        py::set<str> out;
        for (const auto& kv : owned.items()) {
            if (!std::get<1>(kv)) out.add(std::get<0>(kv));
        }
        _owned_scalar = out;
    }
    return *_owned_scalar;
}

void Writer::free_owned_reassign(const NodeP& e_, const py::list<str>& out, const str& ind) {
    NodeP e = e_;
    while (auto p = as_a<ir::Paren>(e)) e = p->x;
    auto asg = as_a<ir::Assign>(e);
    if (!owning || !asg || asg->op) return;
    auto tgt = as_a<ir::Select>(asg->target);
    if (!(tgt && is_a<ir::This>(tgt->obj) && tgt->sym && owned_scalar().contains(tgt->sym->name))) return;
    NodeP v = asg->value;
    while (true) {
        if (auto p = as_a<ir::Paren>(v)) v = p->x;
        else if (auto c = as_a<ir::Cast>(v)) v = c->x;
        else break;
    }
    bool fresh = is_a<ir::New>(v);
    if (!fresh) {
        if (auto vc = as_a<ir::Call>(v)) {
            fresh = is_a<ir::TypeName>(vc->target) && vc->sym && FACTORY().match(vc->sym->name).has_value();
        }
    }
    if (fresh && !(!out.empty() && out[-1].strip() == py::percent(S("delete this->%s;"), tgt->sym->name))) {
        out.append(ind + py::percent(S("delete this->%s;   // J2X no-gc-reassign-leak: free the previous owned %s"),
                                     tgt->sym->name, tgt->sym->name));
    }
}

void Writer::free_owned_return(const py::ref<ir::Local>& s, const str& ct, const str& name, const py::list<str>& out,
                               const str& ind) {
    auto m2 = OWNED_CONTAINER().match(ct);
    NodeP init = s->init;
    while (auto p = as_a<ir::Paren>(init)) init = p->x;
    auto ic = as_a<ir::Call>(init);
    if (!owning || !m2 || !ic || !ic->sym || !r->own_catalog.return_methods.contains(ic->sym->name)) return;
    for (const auto& chain : uses(after, s->sym)) {
        if (!transient_use(chain)) return;
    }
    out.append(ind + py::percent(S("J2X_ScopedFreeElems<%s<%s*> > _j2xfree_%s(%s);   // J2X no-gc-return-leak: free the "
                                   "heap elements %s() handed back (caller-frees; Java GC'd them)"),
                                 m2->group(1), m2->group(2), name, name, ic->sym->name));
}

void Writer::hoist_owned_args(const NodeP& root, const py::list<str>& out, const str& ind) {
    if (!owning) return;
    const jtypes::OwnershipCatalog& cat = r->own_catalog;
    if (cat.scalar_methods.empty()) return;
    // (inner call, outer call, argument position) for each owned-scalar builder call passed directly as an
    // argument of a call to a method of the program that does not retain that position -- and evaluated
    // unconditionally (not under ?:, && or ||).
    with_parents(root, py::list<NodeP>(), [&](const NodeP& n, const py::list<NodeP>& chain) {
        auto inner = as_a<ir::Call>(n);
        if (!(inner && inner->sym && cat.scalar_methods.contains(inner->sym->name))) return;
        if (chain.empty() || !is_a<ir::Call>(chain[0]) || !as_a<ir::Call>(chain[0])->sym) return;
        auto outer = as_a<ir::Call>(chain[0]);
        int64_t pos = -1;
        if (outer->args) {
            for (int64_t i = 0; i < py::len(*outer->args); ++i) {
                if ((*outer->args)[i] == n) {
                    pos = i;
                    break;
                }
            }
        }
        if (pos < 0) return;
        if (!cat.corpus_methods.contains(outer->sym->name)) return;
        if (cat.retained_params.contains(outer->sym->name) && cat.retained_params.at(outer->sym->name).contains(pos)) return;
        for (const NodeP& a : chain) {
            auto b = as_a<ir::Binary>(a);
            if (is_a<ir::Cond>(a) || (b && b->op && (*b->op == S("&&") || *b->op == S("||"))) || is_a<ir::Lambda>(a)) return;
        }
        str elem = cat.scalar_methods.at(inner->sym->name);
        auto [code, ct] = expr(inner);
        arg_temps += 1;
        int64_t k = arg_temps;
        out.append(ind + py::percent(S("%s* _j2xarg_%d = %s;"), elem, k, code));
        out.append(ind + py::percent(S("J2X_ScopedDelete<%s > _j2xargd_%d(_j2xarg_%d);   // J2X no-gc-arg-leak: free the heap "
                                       "%s that %s() returned (%s() only reads it)"),
                                     elem, k, k, elem, inner->sym->name, outer->sym->name));
        subst[id_of(inner)] = std::make_tuple(py::percent(S("_j2xarg_%d"), k), ct);
    });
}

void Writer::review(const NodeP& e, const py::list<str>& out, const str& ind) {
    for (const NodeP& n : ir::walk(e)) {
        auto c = as_a<ir::Call>(n);
        if (c && c->sym && REFLECTIVE().contains(c->sym->name) && !ctx->is_game(c->sym->owner)) {
            out.append(ind + py::percent(S("// J2X-REVIEW[reflection-unsupported]: %s() has no C++ equivalent -- the "
                                           "runtime stub returns NULL. Replace with a hand-written factory (a class-keyed "
                                           "switch / registry), then null-check the result."),
                                         c->sym->name));
            note(ledger::Category::RISK, ledger::Severity::HIGH, S("reflection-unsupported"),
                 py::percent(S("reflection %s() has no C++ equivalent -- stub returns NULL; needs a hand-written "
                               "factory (J2X-REVIEW[reflection-unsupported])"),
                             c->sym->name));
            return;
        }
    }
}

str Writer::cond(const NodeP& e_) {
    NodeP e = e_;
    while (auto p = as_a<ir::Paren>(e)) e = p->x;
    return std::get<0>(expr(e));
}

// ------------------------------------------------------------------ expressions -> (code, C++ type)
str Writer::name(const str& n) const { return CPP_KEYWORDS().contains(n) ? n + "_" : n; }

bool Writer::handles(const NodeP& e) const {
    return is_a<ir::Paren>(e) || is_a<ir::Lit>(e) || is_a<ir::Name>(e) || is_a<ir::This>(e) || is_a<ir::Select>(e) ||
           is_a<ir::Length>(e) || is_a<ir::Index>(e) || is_a<ir::Unary>(e) || is_a<ir::Binary>(e) || is_a<ir::Concat>(e) ||
           is_a<ir::Assign>(e) || is_a<ir::Cond>(e) || is_a<ir::Cast>(e) || is_a<ir::InstanceOf>(e) || is_a<ir::Call>(e) ||
           is_a<ir::ClassLit>(e) || is_a<ir::New>(e) || is_a<ir::NewArray>(e);
}

Code Writer::dispatch(const NodeP& e) {
    if (auto x = as_a<ir::Paren>(e)) return x_Paren(x);
    if (auto x = as_a<ir::Lit>(e)) return x_Lit(x);
    if (auto x = as_a<ir::Name>(e)) return x_Name(x);
    if (auto x = as_a<ir::This>(e)) return x_This(x);
    if (auto x = as_a<ir::Select>(e)) return x_Select(x);
    if (auto x = as_a<ir::Length>(e)) return x_Length(x);
    if (auto x = as_a<ir::Index>(e)) return x_Index(x);
    if (auto x = as_a<ir::Unary>(e)) return x_Unary(x);
    if (auto x = as_a<ir::Binary>(e)) return x_Binary(x);
    if (auto x = as_a<ir::Concat>(e)) return x_Concat(x);
    if (auto x = as_a<ir::Assign>(e)) return x_Assign(x);
    if (auto x = as_a<ir::Cond>(e)) return x_Cond(x);
    if (auto x = as_a<ir::Cast>(e)) return x_Cast(x);
    if (auto x = as_a<ir::InstanceOf>(e)) return x_InstanceOf(x);
    if (auto x = as_a<ir::Call>(e)) return x_Call(x);
    if (auto x = as_a<ir::ClassLit>(e)) return x_ClassLit(x);
    if (auto x = as_a<ir::New>(e)) return x_New(x);
    if (auto x = as_a<ir::NewArray>(e)) return x_NewArray(x);
    throw Decline(S("expression ") + e->class_name());
}

Code Writer::expr_nl(const NodeOrList& e) {
    if (std::holds_alternative<py::list<NodeP>>(e)) throw Decline(S("expression list"));
    return expr(std::get<NodeP>(e));
}

Code Writer::expr(const NodeP& e) {
    if (e && subst.contains(id_of(e))) return subst.at(id_of(e));
    if (!e || !handles(e)) throw Decline(S("expression ") + (e ? e->class_name() : S("NoneType")));
    str pre = order ? sequenced(e) : S("");
    auto [code, ct] = dispatch(e);
    if (py::truthy(pre)) return Code(py::percent(S("(%s, %s)"), pre, code), ct);
    return Code(code, ct);
}

str Writer::sequenced(const NodeP& e) {
    py::list<NodeP> nodes = _operands(e);
    if (py::len(nodes) < 2 || !_order_matters(nodes)) return S("");
    py::list<str> parts;
    for (const NodeP& n : nodes) {
        if (_constant(n) || (n && subst.contains(id_of(n)))) continue;
        auto [code0, ct0] = expr(n);
        str code = code0, ct = ct0;
        if (ct == S("JCat")) {   // a concatenation: as the String it makes
            code = py::percent(S("JavaString((%s)._s)"), code);
            ct = S("JavaString");
        }
        ct = strip_const_ref(ct);
        if (ct == S("char*") || ct == S("char *")) ct = S("JavaString");   // text: held as the String it is
        if (ct == S("void") || ct.empty()) return S("");
        temps += 1;
        str nm = py::percent(S("j2xo%d"), temps);
        pending.append(py::percent(S("%s %s = %s;"), ct, nm, _zero(ct)));
        parts.append(py::percent(S("%s = %s"), nm, code));
        subst[id_of(n)] = std::make_tuple(nm, ct);
    }
    return join_commas(parts);
}

Code Writer::x_Paren(const py::ref<ir::Paren>& e) {
    auto [x, t] = expr(e->x);
    return Code(S("(") + x + ")", t);
}

Code Writer::x_Lit(const py::ref<ir::Lit>& e) {
    const ir::Val& v = e->value;
    const str& k = K(e->type).kind;
    if (v.is_none()) return Code(S("NULL"), S("void*"));
    auto truthy_val = [&]() {
        return std::visit(
            [](const auto& x) -> bool {
                using T = std::decay_t<decltype(x)>;
                if constexpr (std::is_same_v<T, std::monostate>) return false;
                else if constexpr (std::is_same_v<T, bool>) return x;
                else if constexpr (std::is_same_v<T, int64_t>) return x != 0;
                else if constexpr (std::is_same_v<T, double>) return x != 0.0;
                else if constexpr (std::is_same_v<T, str>) return !x.empty();
                else if constexpr (std::is_same_v<T, TypeP>) return true;
                else return !x.empty();
            },
            v.v);
    };
    // int(v) / '%d' % v of the value the front end put in a Lit
    auto int_of = [&]() -> int64_t {
        if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v);
        if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v) ? 1 : 0;
        if (std::holds_alternative<double>(v.v)) return static_cast<int64_t>(std::get<double>(v.v));
        if (std::holds_alternative<str>(v.v)) return py::int_parse(std::get<str>(v.v), 10);
        throw py::TypeError("int() argument must be a string, a bytes-like object or a real number");
    };
    auto fmt_int = [&]() -> int64_t {   // '%d' % v
        if (std::holds_alternative<str>(v.v)) throw py::TypeError("%d format: a real number is required, not str");
        return int_of();
    };
    if (k == S("bool")) return Code(truthy_val() ? S("true") : S("false"), S("bool"));
    if (k == S("i32")) {
        bool is_min = (std::holds_alternative<int64_t>(v.v) && std::get<int64_t>(v.v) == -2147483648LL) ||
                      (std::holds_alternative<double>(v.v) && std::get<double>(v.v) == -2147483648.0);
        if (is_min) return Code(S("(-2147483647 - 1)"), S("int"));
        return Code(std::holds_alternative<str>(v.v) ? std::get<str>(v.v) : v.repr(), S("int"));   // str(v)
    }
    if (k == S("i64")) {
        bool is_min = (std::holds_alternative<int64_t>(v.v) && std::get<int64_t>(v.v) == INT64_MIN) ||
                      (std::holds_alternative<double>(v.v) && std::get<double>(v.v) == -9223372036854775808.0);
        if (!is_min) return Code(py::percent(S("%dLL"), fmt_int()), S("long long"));
        return Code(S("(-9223372036854775807LL - 1)"), S("long long"));
    }
    if (k == S("f32")) return Code(_float(v, S("F")), S("float"));
    if (k == S("f64")) return Code(_float(v, S("")), S("double"));
    if (k == S("u16")) {
        int64_t c;
        if (std::holds_alternative<str>(v.v)) {
            const str& sv = std::get<str>(v.v);
            if (sv.size() != 1) throw py::TypeError("ord() expected a character");
            c = static_cast<int64_t>(sv.at_(0));
        } else {
            c = int_of();
        }
        if (ctx->char_map.contains(c)) c = ctx->char_map.at(c);   // the program's font, as in its strings
        if (32 <= c && c < 127 && c != '\\' && c != '\'') return Code(S("'") + str(static_cast<char32_t>(c)) + "'", S("unsigned char"));
        if (c > 255) return Code(py::to_str(c), S("int"));        // past the runtime's 8-bit char: its value as an
                                                                   // int (Java's char is 16 bits: '￿' is 65535)
        return Code(py::percent(S("(unsigned char)%d"), c), S("unsigned char"));
    }
    if (e->type->kind == S("class") && e->type->name && *e->type->name == S("java.lang.String"))
        return Code(_cstring(v, ctx->char_map), S("const char*"));
    throw Decline(S("literal ") + v.repr());
}

Code Writer::x_Name(const py::ref<ir::Name>& e) {
    str n = name(need_sym(e->sym).name);
    if (alias.contains(n)) {
        str got = vars.get(n, str(""));
        if (!py::truthy(got)) got = vars.get(alias.at(n), str(""));
        return Code(alias.at(n), got);
    }
    if (!vars.contains(n)) throw Decline(S("unknown local ") + n);
    return Code(n, vars.at(n));
}

Code Writer::x_This(const py::ref<ir::This>& e) {
    if (e->qualifier) throw Decline(S("qualified this"));
    return Code(S("this"), cname + "*");
}

Code Writer::x_Select(const py::ref<ir::Select>& e) {
    ir::Sym& s = need_sym(e->sym, "owner_simple");
    std::optional<str> owner_o = s.owner_simple();
    str owner = owner_o ? *owner_o : S("None");
    if (s.kind == S("enumconst")) return Code(py::percent(S("%s::%s"), owner, naming::enum_member(s.name)), ctype(e->type));
    if (is_a<ir::TypeName>(e->obj)) {
        if (!ctx->is_game(s.owner)) {
            if (s.owner && *s.owner == S("java.lang.Math") && s.name == S("PI")) return Code(typemap::MATH_API.at(S("PI")), S("double"));
            if (typemap::RUNTIME_ENUM_LIKE.contains(owner))   // Integer.MAX_VALUE, GL11.GL_BLEND, System.out
                return Code(py::percent(S("%s::%s"), owner, s.name), library_field(owner, s.name, e->type));
            auto [ct, spelled] = _runtime_field(owner, s.name);   // a runtime class's static field, spelled
            if (spelled) return Code(py::percent(S("%s::%s"), owner, *spelled), ct ? *ct : S("None"));   // as its header has it (s_name or name)
            throw Decline(py::percent(S("library static %s.%s"), owner, s.name));
        }
        std::optional<str> ct = ctx->field_ctype(owner, s.name);
        if (!ct) throw Decline(py::percent(S("static field %s.%s"), owner, s.name));
        if (r->enums.contains(owner))   // an enum's statics keep their bare names
            return Code(py::percent(S("%s::%s"), owner, s.name), *ct);
        return Code(py::percent(S("%s::%s"), owner, naming::static_field_name(s.name)), *ct);
    }
    if (s.static_) throw Decline(S("static field through an instance"));
    str recv, rt;
    if (is_a<ir::This>(e->obj) || is_a<ir::Super>(e->obj)) {   // super.field is this object's inherited field
        recv = S("this");
        rt = cname + "*";
    } else {
        std::tie(recv, rt) = expr(e->obj);
    }
    std::optional<str> base = _class_of(rt);
    std::optional<str> ct;
    if (ctx->is_game(s.owner)) {
        ct = ctx->field_ctype(base ? *base : owner, s.name);
        if (!ct || !py::truthy(*ct)) ct = ctx->field_ctype(owner, s.name);
    } else {
        std::optional<str> spelled;
        std::tie(ct, spelled) = _runtime_field(owner, s.name);   // a library object's field: as the runtime has it
        if (!spelled && (py::truthy(_runtime_header(owner)) || typemap::VALUE_STDLIB.contains(owner))) {
            ct = ctype(e->type);   // a runtime type that holds it inline
            spelled = s.name;
        }
        if (!spelled || *spelled != s.name) throw Decline(py::percent(S("library field %s.%s"), owner, s.name));
    }
    if (!ct) throw Decline(py::percent(S("field %s.%s"), owner, s.name));
    return Code(checked(recv, rt) + (is_pointer(rt) ? "->" : ".") + s.name, *ct);
}

str Writer::arg(const NodeP& a) {
    auto [code, ct] = expr(a);
    return ct == S("JCat") ? py::percent(S("JavaString((%s)._s)"), code) : code;
}

py::list<NodeP> Writer::unmarked(const Nodes& args) {
    py::list<NodeP> out;
    for (const NodeP& a : lst(args)) {
        TypeP t = type_of(a);
        bool marker = t && t->kind == S("class") && r->markers.contains(t->simple());
        if (!marker) out.append(a);
    }
    return out;
}

str Writer::checked(const str& recv, const str& rt) {
    if (npe && is_pointer(rt) && recv != S("this") && !REFLECTION().contains(rt))
        return py::percent(S("j2xNN<NullPointerException>(%s)"), recv);
    return recv;
}

Code Writer::x_Length(const py::ref<ir::Length>& e) {
    NodeP c = e->arr;
    if (auto call = as_a<ir::Call>(c)) {
        if (call->sym && call->sym->name == S("values") && (!call->args || call->args->empty()) && is_a<ir::TypeName>(call->target) &&
            in_set(call->sym->owner_simple(), r->enums) && !in_set(call->sym->owner_simple(), r->typed_enums)) {
            return Code(py::percent(S("%s::_J2X_COUNT"), call->sym->owner_simple()), S("int"));   // a constant-only enum's size
        }
    }
    auto [a, t] = expr(e->arr);
    return Code(a + ".length", S("int"));
}

Code Writer::x_Index(const py::ref<ir::Index>& e) {
    auto [a, t] = expr(e->arr);
    auto [i, it] = expr_nl(e->index);
    return Code(py::percent(S("%s[%s]"), a, i), _elem(t));
}

Code Writer::x_Unary(const py::ref<ir::Unary>& e) {
    auto [x, t] = expr(e->x);
    if (e->postfix && *e->postfix) return Code(py::percent(S("%s%s"), x, e->op), t);
    bool incdec = e->op && (*e->op == S("++") || *e->op == S("--"));
    return Code(py::percent(S("%s%s"), e->op, x), incdec ? t : ctype(e->type));
}

Code Writer::x_Binary(const py::ref<ir::Binary>& e) {
    auto [l0, lt] = expr(e->left);
    auto [r0, rt] = expr(e->right);
    str lc = l0, rc = r0;
    if (in_set(e->op, BITS())) {   // `a >> 16 & 255` is the same in Java and C++,
        auto lb = as_a<ir::Binary>(e->left);   // but MSVC warns (C4554): group it
        if (lb && lb->op != e->op) lc = py::percent(S("(%s)"), lc);
        auto rb = as_a<ir::Binary>(e->right);
        if (rb && rb->op != e->op) rc = py::percent(S("(%s)"), rc);
    }
    if (e->op && *e->op == S(">>>")) {
        const str& k = K(e->type).kind;
        // static_cast, not a C cast: the transpiler's finalize pass turns `(int)(...)` into the
        // saturating float->int helper, and a shifted bit pattern must wrap, not saturate
        if (k == S("i32"))
            return Code(py::percent(S("static_cast<int>(static_cast<unsigned int>(%s) >> (%s))"), lc, rc), S("int"));
        if (k == S("i64"))
            return Code(py::percent(S("static_cast<long long>(static_cast<unsigned long long>(%s) >> (%s))"), lc, rc),
                        S("long long"));
        throw Decline(S(">>> on ") + k);
    }
    // (a String == String compares the text, as the text path does: identity has no meaning for the
    // by-value JavaString)
    if (e->op && (*e->op == S("==") || *e->op == S("!=")) && lt != rt && (lt.contains(S("JObject")) || rt.contains(S("JObject"))) &&
        lt != S("void*") && rt != S("void*")) {
        str eq = py::percent(S("j2xIdentEq(%s, %s)"), lc, rc);   // identity across a box, as the text path
        return Code(*e->op == S("==") ? eq : S("!") + eq, S("bool"));
    }
    if (e->op && *e->op == S("%") && (K(e->type).kind == S("f32") || K(e->type).kind == S("f64"))) {   // Java's floating remainder: C's fmod
        str fmt = K(e->type).kind == S("f32") ? S("(float)fmod((double)(%s), (double)(%s))") : S("fmod(%s, %s)");
        return Code(py::percent(fmt, lc, rc), ctype(e->type));
    }
    return Code(py::percent(S("%s %s %s"), lc, e->op, rc), ctype(e->type));
}

Code Writer::x_Concat(const py::ref<ir::Concat>& e) {
    py::list<str> parts;
    for (const NodeP& p : lst(e->parts)) parts.append(std::get<0>(expr(p)));
    return Code(py::percent(S("(JCat() + %s)"), py::join(S(" + "), parts)), S("JCat"));
}

Code Writer::x_Assign(const py::ref<ir::Assign>& e) {
    auto [t, tt] = expr(e->target);
    auto [v, vt] = expr(e->value);
    if (!e->op) return Code(py::percent(S("%s = %s"), t, unbox(coerce(v, vt, tt), vt, tt)), tt);
    if (*e->op == S(">>>")) {
        const str& k = K(type_of(e->target)).kind;
        bool plain = is_a<ir::Name>(e->target);
        if (auto sel = as_a<ir::Select>(e->target)) plain = plain || is_a<ir::This>(sel->obj) || is_a<ir::TypeName>(sel->obj);
        if (!(k == S("i32") || k == S("i64")) || !plain)   // evaluated twice below: no side effects allowed
            throw Decline(S(">>>="));
        str u = k == S("i32") ? S("unsigned int") : S("unsigned long long");
        return Code(py::percent(S("%s = static_cast<%s>(static_cast<%s>(%s) >> (%s))"), t, k == S("i32") ? S("int") : S("long long"), u, t, v), tt);
    }
    if (*e->op == S("%") && (K(type_of(e->target)).kind == S("f32") || K(type_of(e->target)).kind == S("f64")))
        throw Decline(S("floating %="));
    return Code(py::percent(S("%s %s= %s"), t, e->op, v), tt);
}

Code Writer::x_Cond(const py::ref<ir::Cond>& e) {
    str c = cond(e->cond);
    auto [a0, at] = expr(e->then);
    auto [b0, bt] = expr(e->orelse);
    str a = a0, b = b0;
    if (K(e->type).kind == S("null")) return Code(py::percent(S("(%s ? %s : %s)"), c, a, b), at);
    str t = ctype(e->type);
    if (at != bt) {   // Java converts both arms to the result type
        a = arm(a, at, t);
        b = arm(b, bt, t);
    }
    return Code(py::percent(S("(%s ? %s : %s)"), c, a, b), t);
}

str Writer::arm(const str& code, const str& ctype_, const str& t) {
    if (code == S("NULL")) return null_of(t);
    if (ctype_ == t) return code;
    if (t == S("JavaString")) return ctype_ == S("JCat") ? py::percent(S("JavaString((%s)._s)"), code) : py::percent(S("JavaString(%s)"), code);
    return py::percent(S("(%s)(%s)"), t, code);
}

Code Writer::x_Cast(const py::ref<ir::Cast>& e) {
    auto [x, xt] = expr(e->x);
    if (xt.startswith(S("JObject")) && K(e->type).kind == S("class") && typemap::BOXED_VALUE_TYPES.contains(e->type->simple()))
        return Code(x, S("JObject"));   // (Integer)o stays boxed: o.intValue() is JObject's
    str ct = ctype(e->type);
    if (xt.startswith(S("JObject")) && COLLECTION().match(ct))   // (List)o: the boxed list itself, not a copy
        return Code(py::percent(S("j2xUnboxRef<%s >(%s)"), ct, x), ct);
    if (SATURATING().contains(ct))   // Java's (int)/(long) of a float saturates; C++'s is
        return Code(py::percent(S("%s(%s)"), SATURATING().at(ct), x), ct);   // undefined out of range (JavaRT.h's helpers)
    if (e->type->is_prim() && !SIMPLE().match(x))   // grouped
        return Code(py::percent(S("(%s)(%s)"), ct, x), ct);
    return Code(py::percent(S("(%s)%s"), ct, x), ct);   // as the text path: the reflection pass reads
                                                        // `(T*)recv->newInstance()`
}

Code Writer::x_InstanceOf(const py::ref<ir::InstanceOf>& e) {
    auto [x, xt] = expr(e->x);
    JTypeRef target = jtype(e->test);
    if (target->is_array() && target->array_dims == 1 && target->name == S("String") && xt.startswith(S("JObject")))
        return Code(py::percent(S("(%s).isStringArray()"), x), S("bool"));
    if (!target->is_array() && BOXED_TEST().contains(target->name)) return boxed_instanceof(x, xt, target->name);
    if (target->is_array() || !r->is_pointer(target)) throw Decline(S("instanceof ") + target->name);
    if (is_pointer(xt)) return Code(py::percent(S("(dynamic_cast<%s*>(%s) != NULL)"), target->name, x), S("bool"));
    if (xt.startswith(S("JObject"))) return Code(py::percent(S("(%s).is<%s>()"), x, target->name), S("bool"));
    throw Decline(S("instanceof on ") + xt);
}

Code Writer::boxed_instanceof(const str& x, const str& xt, const str& name) {
    if (xt.startswith(S("JObject")) && BOXED_TEST().contains(name))
        return Code(py::percent(S("(%s).%s()"), x, BOXED_TEST().at(name)), S("bool"));
    throw Decline(S("instanceof ") + name);
}

Code Writer::x_Call(const py::ref<ir::Call>& e) {
    SymP s = e->sym;
    if (!s || s->kind != S("method")) throw Decline(py::percent(S("call %s"), s ? s->kind : S("unresolved")));
    std::optional<str> owner_o = s->owner_simple();
    str owner = owner_o ? *owner_o : S("None");
    e->args = unmarked(e->args);   // the decompiler's EmptyN bridge arguments
    bool target_value = e->target && !is_a<ir::TypeName>(e->target);
    if (typemap::BOXED_VALUE_TYPES.contains(owner) && s->name.endswith(S("Value")) && s->params.empty() && target_value) {
        auto [recv, rt] = expr(e->target);   // Integer.intValue(): a boxed type is a primitive
        if (!rt.startswith(S("JObject"))) {   // here, unless it is still in its JObject box
            str ct = ctype(e->type);
            return Code(py::percent(S("((%s)(%s))"), ct, recv), ct);
        }
    }
    if (typemap::BOXED_VALUE_TYPES.contains(owner) && s->name == S("equals") && py::len(*e->args) == 1 && target_value) {
        auto [recv, rt] = expr(e->target);   // Integer.equals(Integer): the primitives' values
        if (!rt.startswith(S("JObject"))) return Code(py::percent(S("(%s == %s)"), recv, std::get<0>(expr((*e->args)[0]))), S("bool"));
    }
    if (s->name == S("ordinal") && s->params.empty() && e->target) {
        auto [recv, rt] = expr(e->target);
        if (rt == S("int")) return Code(py::percent(S("(%s)"), recv), S("int"));   // a constant-only enum is its ordinal
    }
    if (s->owner && *s->owner == S("java.lang.Object")) return object_method(e);
    if (s->name == S("values") && e->args->empty() && is_a<ir::TypeName>(e->target) && r->typed_enums.contains(owner))
        return Code(py::percent(S("%s::values()"), owner), ctype(e->type));   // the enum's own table
    str args;
    if (s->varargs) {
        args = varargs(e, owner);
    } else {
        py::list<str> parts;
        for (const NodeP& a : *e->args) parts.append(arg(a));
        args = join_commas(parts);
    }
    bool game = ctx->is_game(s->owner);
    MethodRef old_m = game ? ctx->old_method(owner, s->name, s->params) : MethodRef();
    str ret_t, name;
    if (game) {
        if (!old_m) throw Decline(py::percent(S("method %s.%s not found"), owner, s->name));
        ret_t = ownership::return_cpp_type(old_m, *r);
        name = r->renamed_method(owner, s->name);
    } else {
        ret_t = library_return(owner, s->name, s->ret);
        name = this->name(s->name);   // File.delete() -> delete_(), as the runtime has it
    }
    NodeP t = e->target;
    if (!t) return Code(py::percent(S("this->%s(%s)"), name, args), ret_t);
    if (is_a<ir::TypeName>(t)) {
        if (!game) {
            if (s->owner && *s->owner == S("java.lang.Math")) {
                if (!MATH().contains(s->name)) throw Decline(S("Math.") + s->name);
                return Code(py::percent(S("%s(%s)"), MATH().at(s->name), args), ctype(e->type));
            }
            TypeP tt = type_of(t);
            str holder = (tt && tt->kind == S("class")) ? tt->simple() : owner;
            if (!typemap::RUNTIME_HEADERS.contains(holder) && !typemap::JAVART_STATICS.contains(holder))
                throw Decline(py::percent(S("library static call %s.%s"), holder, s->name));
            return Code(py::percent(S("%s::%s(%s)"), holder, name, args), ret_t);   // as written: ARBVertexBufferObject::
        }
        return Code(py::percent(S("%s::%s(%s)"), owner, name, args), ret_t);
    }
    if (is_a<ir::Super>(t)) {
        if (!parent || !py::truthy(*parent)) throw Decline(S("super call without a parent"));
        return Code(py::percent(S("%s::%s(%s)"), *parent, name, args), ret_t);
    }
    if (is_a<ir::This>(t)) return Code(py::percent(S("this->%s(%s)"), name, args), ret_t);
    auto [recv0, rt0] = expr(t);
    str recv = recv0, rt = rt0;
    if (rt == S("const char*")) {
        recv = py::percent(S("JavaString(%s)"), recv);
        rt = S("JavaString");
    }
    if (recv.startswith(S("new ")))   // new File(p).toURI(): `new` binds looser than ->
        recv = py::percent(S("(%s)"), recv);
    return Code(py::percent(S("%s%s%s(%s)"), checked(recv, rt), is_pointer(rt) ? S("->") : S("."), name, args), ret_t);
}

Code Writer::object_method(const py::ref<ir::Call>& e) {
    SymP s = e->sym;
    NodeP t = e->target;
    str recv, rt;
    if (!t || is_a<ir::This>(t)) {
        recv = S("this");
        rt = cname + "*";
    } else {
        std::tie(recv, rt) = expr(t);
    }
    bool no_args = !e->args || e->args->empty();
    if (s->name == S("getClass") && no_args) {
        if (rt.startswith(S("JObject"))) {   // a box: the class of the object it holds
            str acc = is_pointer(rt) ? S("->") : S(".");
            return Code(py::percent(S("((%s)%sti ? j2xClassOf(*(%s)%sti) : (Class*)NULL)"), recv, acc, recv, acc), S("Class*"));
        }
        if (!is_pointer(rt)) throw Decline(S("getClass on ") + rt);
        return Code(py::percent(S("j2xClassOf(typeid(*%s))"), checked(recv, rt)), S("Class*"));
    }
    if (s->name == S("equals") && e->args && py::len(*e->args) == 1 && is_pointer(rt)) {
        auto [other, ot] = expr((*e->args)[0]);   // Object.equals is identity
        if ((is_pointer(ot) && ot == rt) || other == S("NULL")) return Code(py::percent(S("(%s == %s)"), recv, other), S("bool"));
        return Code(py::percent(S("j2xIdentEq(%s, %s)"), recv, other), S("bool"));
    }
    if (s->name == S("equals") && e->args && py::len(*e->args) == 1 && rt.startswith(S("JObject")) && !is_pointer(rt))
        return Code(py::percent(S("(%s).equals(%s)"), recv, std::get<0>(expr((*e->args)[0]))), S("bool"));   // the box's own equals
    if (s->name == S("toString") && no_args && is_pointer(rt))
        return Code(py::percent(S("%s->toString()"), checked(recv, rt)), S("JavaString"));
    throw Decline(S("Object method ") + s->name);
}

str Writer::varargs(const py::ref<ir::Call>& e, const str& owner) {
    SymP s = e->sym;
    if (!ctx->is_game(s->owner)) return library_varargs(e);
    MethodRef old_m = ctx->old_method(owner, s->name, s->params);
    if (!old_m) throw Decline(py::percent(S("method %s.%s not found"), owner, s->name));
    str last = std::get<0>(_split_params(ownership::params_str(old_m, *r))[-1]);
    int64_t fixed = py::len(s->params) - 1;
    py::list<str> args;
    for (const NodeP& a : lst(e->args)) args.append(arg(a));
    if (py::len(*e->args) == py::len(s->params)) {
        TypeP lt = type_of((*e->args)[-1]);
        if (lt && (lt->kind == S("array") || lt->kind == S("null"))) return join_commas(args);   // the array itself was passed
    }
    py::list<str> rest = args.slice_from(fixed);
    str elem = _elem(last.replace(S("const "), S("")).rstrip(S("&")).strip());
    str packed = !rest.empty() ? py::percent(S("(JArrayInit<%s >(), %s)"), elem, join_commas(rest)) : py::percent(S("JArray<%s >()"), elem);
    py::list<str> head = args.slice_to(fixed);
    head.append(packed);
    return join_commas(head);
}

str Writer::library_varargs(const py::ref<ir::Call>& e) {
    SymP s = e->sym;
    TypeP last = s->params.empty() ? TypeP() : s->params[-1];
    if (!last || last->kind != S("array")) {
        std::optional<str> o = s->owner_simple();
        throw Decline(py::percent(S("library varargs %s.%s"), o, s->name));
    }
    str elem = _elem(ctype(last));
    int64_t fixed = py::len(s->params) - 1;
    py::list<str> args;
    for (const NodeP& a : lst(e->args)) args.append(arg(a));
    if (py::len(*e->args) == py::len(s->params)) {
        TypeP lt = type_of((*e->args)[-1]);
        if (lt && (lt->kind == S("array") || lt->kind == S("null"))) return join_commas(args);
    }
    py::list<str> rest = args.slice_from(fixed);
    str packed = !rest.empty() ? py::percent(S("(JArrayInit<%s >(), %s)"), elem, join_commas(rest)) : py::percent(S("JArray<%s >()"), elem);
    py::list<str> head = args.slice_to(fixed);
    head.append(packed);
    return join_commas(head);
}

str Writer::library_field(const str& owner, const str& name, const TypeP& t) {
    if (owner == S("System") && (name == S("out") || name == S("err"))) return S("System::_PrintStream");   // a value in the runtime (JavaRT.h)
    return ctype(t);
}

str Writer::library_return(const str& owner, const str& name, const TypeP& ret_t) {
    str base = py::re::sub(S("<.*"), S(""), r->base_cpp(owner));
    std::optional<str> declared = _runtime_return(owner, name);   // what the runtime's own header declares, first
    if (declared && py::truthy(*declared)) return *declared;
    auto& stubs = runtime::STUB_MEMBERS();
    std::optional<str> rec_key;
    if (stubs.contains(base)) rec_key = base;
    else if (stubs.contains(owner)) rec_key = owner;
    py::set<str> seen;
    while (rec_key && !seen.contains(*rec_key)) {
        seen.add(*rec_key);
        const runtime::StubMembers& rec = stubs.at(*rec_key);
        if (rec.methods.contains(name)) {
            str t = rec.methods.at(name);
            if (py::truthy(t)) return t == S("std::string") ? S("JavaString") : t;
        }
        if (rec.parent && stubs.contains(*rec.parent)) rec_key = *rec.parent;
        else rec_key = std::nullopt;
    }
    if (name == S("iterator")) return S("Iterator*");
    if (ret_t && ret_t->kind == S("class") && ret_t->simple() == owner && owner.endswith(S("Buffer")))
        return owner + "&";   // NIO buffers chain by reference (put().put())
    if (!ret_t || ret_t->kind == S("void")) return S("void");
    return ctype(ret_t, S("return"));
}

Code Writer::x_ClassLit(const py::ref<ir::ClassLit>& e) {
    TypeP t = e->of;
    if (!t || t->kind != S("class")) throw Decline(S("class literal of ") + (t ? t->repr() : S("None")));
    str simple = t->simple();
    str lit = CLASS_LIT_TYPE().get(simple, simple);
    return Code(py::percent(S("j2xClassOf(typeid(%s))"), lit), S("Class*"));
}

Code Writer::x_New(const py::ref<ir::New>& e) {
    if (e->body || e->outer) throw Decline(S("anonymous or inner class"));
    TypeP t = e->type;
    JTypeRef jt = jtype(t);
    if (r->values.contains(jt->name)) throw Decline(S("value type ") + jt->name);
    e->args = unmarked(e->args);
    py::list<str> parts;
    for (const NodeP& a : *e->args) parts.append(arg(a));
    str args = join_commas(parts);
    if (K(t).kind == S("class") && ctx->is_game(t->name)) {
        if (r->extends_value_container(jt->name))   // a container subclass IS the container value
            return Code(py::percent(S("(*(new %s(%s)))"), jt->name, args), jt->name);
        return Code(py::percent(S("new %s(%s)"), jt->name, args), jt->name + "*");
    }
    str ct = r->cpp_type(jt, S("local"));
    if (typemap::BOXED_VALUE_TYPES.contains(jt->name))   // new Integer(5): a boxed type is its primitive
        return Code(py::percent(S("((%s)(%s))"), ct, args), ct);
    if (is_pointer(ct)) return Code(py::percent(S("new %s(%s)"), ct.slice(0, -1).strip(), args), ct);
    return Code(py::percent(S("%s(%s)"), ct, args), ct);
}

Code Writer::x_NewArray(const py::ref<ir::NewArray>& e) {
    str ct = ctype(e->type);
    if (e->init) {
        if (e->dims && !e->dims->empty()) throw Decline(S("sized initialised array"));
        py::list<str> parts;
        for (const NodeP& x : *e->init) parts.append(std::get<0>(expr(x)));
        return Code(py::percent(S("(JArrayInit<%s >(), %s)"), _elem(ct), join_commas(parts)), ct);
    }
    int64_t total = std::get<0>(K(e->type).dims());
    py::list<str> dims;
    for (const NodeP& d : lst(e->dims)) dims.append(std::get<0>(expr(d)));
    if (py::len(dims) == 1)   // new T[n], or jagged new T[n][]: n empty rows
        return Code(py::percent(S("%s(%s)"), ct, dims[0]), ct);
    if (py::len(dims) == total && (total == 2 || total == 3)) {   // new T[a][b]([c]): the runtime's nested builders
        str inner = ct;
        for (int64_t i = 0; i < total; ++i) inner = _elem(inner);
        return Code(py::percent(S("j2x_newarr%d<%s >(%s)"), total, inner, join_commas(dims)), ct);
    }
    throw Decline(S("multi-dimensional array"));
}

// ---------------------------------------------------------------------------------------------- module-level helpers
namespace {

// `Writer = Writer17` while a Bodies17 call runs (cpp03.py's _with_writer17 swaps the module attribute)
bool g_writer17 = false;
struct Writer17Scope {
    bool saved;
    Writer17Scope() : saved(g_writer17) { g_writer17 = true; }
    ~Writer17Scope() { g_writer17 = saved; }
};

std::unique_ptr<Writer> new_writer(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls,
                                   const py::ref<ir::MethodDecl>& m, const MethodRef& old) {
    if (g_writer17) return std::make_unique<Writer17>(ctx, cls, m, old);
    return std::make_unique<Writer>(ctx, cls, m, old);
}

// float(v)
double to_double(const ir::Val& v) {
    if (std::holds_alternative<double>(v.v)) return std::get<double>(v.v);
    if (std::holds_alternative<int64_t>(v.v)) return static_cast<double>(std::get<int64_t>(v.v));
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v) ? 1.0 : 0.0;
    if (std::holds_alternative<str>(v.v)) return py::float_from_str(std::get<str>(v.v));
    throw py::TypeError("float() argument must be a string or a real number");
}

const str& req(const std::optional<str>& s, const char* attr) {
    if (!s) none_attr(attr);
    return *s;
}

}  // namespace

ir::NodeP _strip_paren(ir::NodeP e) {
    while (auto p = as_a<ir::Paren>(e)) e = p->x;
    return e;
}

py::list<NodeP> _operands(const NodeP& e) {
    py::list<NodeP> out;
    if (auto c = as_a<ir::Call>(e)) {
        if (c->target && !(is_a<ir::This>(c->target) || is_a<ir::Super>(c->target) || is_a<ir::TypeName>(c->target)))
            out.append(c->target);
        if (c->args) out.extend(*c->args);
        return out;
    }
    if (auto n = as_a<ir::New>(e)) {
        if (n->args) out.extend(*n->args);
        return out;
    }
    if (auto b = as_a<ir::Binary>(e)) {
        if (!(b->op && (*b->op == S("&&") || *b->op == S("||")))) {
            out.append(b->left);
            out.append(b->right);
        }
        return out;
    }
    if (auto c = as_a<ir::Concat>(e)) {
        if (c->parts) out.extend(*c->parts);
        return out;
    }
    if (auto a = as_a<ir::Assign>(e)) {
        if (auto ix = as_a<ir::Index>(_strip_paren(a->target))) {
            // a[i] = v: the index before the value
            if (std::holds_alternative<py::list<NodeP>>(ix->index))
                throw py::AttributeError("'list' object has no attribute 'children'");
            out.append(std::get<NodeP>(ix->index));
            out.append(a->value);
        }
        return out;
    }
    return out;
}

// Does evaluating `n` change anything (a call into the program, an allocation, an assignment, ++/--)?
bool _effects(const NodeP& n) {
    for (const NodeP& x : ir::walk(n)) {   // (an array's allocation is not observable: its elements are walked like any operand)
        if (is_a<ir::Assign>(x) || is_a<ir::New>(x)) return true;
        if (auto u = as_a<ir::Unary>(x)) {
            if (u->op && (*u->op == S("++") || *u->op == S("--"))) return true;
        }
        if (auto c = as_a<ir::Call>(x)) {
            if (!(c->sym && in_set(c->sym->owner, PURE_OWNERS()))) return true;
        }
    }
    return false;
}

// Names (locals, fields) that evaluating `n` assigns or increments.
py::set<str> _written(const NodeP& n) {
    py::set<str> out;
    for (const NodeP& x : ir::walk(n)) {
        NodeP target;
        if (auto a = as_a<ir::Assign>(x)) target = a->target;
        else if (auto u = as_a<ir::Unary>(x)) {
            if (u->op && (*u->op == S("++") || *u->op == S("--"))) target = u->x;
        }
        if (target) target = _strip_paren(target);
        if (auto nm = as_a<ir::Name>(target)) {
            if (nm->sym) out.add(nm->sym->name);
        } else if (auto sel = as_a<ir::Select>(target)) {
            if (sel->sym) out.add(sel->sym->name);
        }
    }
    return out;
}

py::set<str> _read(const NodeP& n) {
    py::set<str> out;
    for (const NodeP& x : ir::walk(n)) {
        if (auto nm = as_a<ir::Name>(x)) {
            if (nm->sym) out.add(nm->sym->name);
        } else if (auto sel = as_a<ir::Select>(x)) {
            if (sel->sym) out.add(sel->sym->name);
        }
    }
    return out;
}

bool _order_matters(const py::list<NodeP>& nodes) {
    py::list<NodeP> eff;
    for (const NodeP& n : nodes) {
        if (_effects(n)) eff.append(n);
    }
    if (py::len(eff) >= 2) return true;
    if (py::len(eff) == 1) {
        py::set<str> w = _written(eff[0]);
        if (w.empty()) return false;
        for (const NodeP& n : nodes) {
            if (n != eff[0] && !(w & _read(n)).empty()) return true;
        }
        return false;
    }
    return false;
}

bool _constant(const NodeP& n_) {
    NodeP n = _strip_paren(n_);
    if (is_a<ir::Lit>(n) || is_a<ir::ClassLit>(n) || is_a<ir::This>(n) || is_a<ir::TypeName>(n)) return true;
    auto sel = as_a<ir::Select>(n);
    return sel && is_a<ir::TypeName>(sel->obj) && sel->sym && sel->sym->kind == S("enumconst");
}

str _zero(const str& ct) {
    if (PRIM_ZERO().contains(ct)) return PRIM_ZERO().at(ct);
    return null_of(ct);
}

str _comment(const str& text) { return text.replace(S("*/"), S("* /")); }

bool _abrupt(const NodeP& s) {
    if (is_a<ir::Return>(s) || is_a<ir::Throw>(s)) return true;
    if (auto b = as_a<ir::Block>(s)) return b->stmts && !b->stmts->empty() && _abrupt((*b->stmts)[-1]);
    if (auto i = as_a<ir::If>(s)) {
        if (std::holds_alternative<py::list<NodeP>>(i->orelse) || !std::get<NodeP>(i->orelse)) return false;
        NodeP then = std::holds_alternative<NodeP>(i->then) ? std::get<NodeP>(i->then) : NodeP();
        return _abrupt(then) && _abrupt(std::get<NodeP>(i->orelse));
    }
    if (auto t = as_a<ir::Try>(s)) {
        if (!_abrupt(t->body)) return false;
        for (const NodeP& c : lst(t->catches)) {
            auto ct = as_a<ir::Catch>(c);
            if (!ct) none_attr("body");
            if (!_abrupt(ct->body)) return false;
        }
        return true;
    }
    return false;
}

namespace {

bool leaves_walk(const NodeP& n, int64_t loops, const py::set<str>& labels) {
    if (is_a<ir::Return>(n)) return true;
    if (auto b = as_a<ir::Break>(n)) return (b->label && py::truthy(*b->label)) ? !labels.contains(*b->label) : loops == 0;
    if (auto c = as_a<ir::Continue>(n)) return (c->label && py::truthy(*c->label)) ? !labels.contains(*c->label) : loops == 0;
    if (is_a<ir::Lambda>(n) || is_a<ir::ClassDecl>(n) || is_a<ir::LocalClass>(n)) return false;
    int64_t inner = loops + (is_a<ir::While>(n) || is_a<ir::DoWhile>(n) || is_a<ir::For>(n) || is_a<ir::ForEach>(n) || is_a<ir::Switch>(n));
    py::set<str> labs = labels;
    if (auto lb = as_a<ir::Labeled>(n)) {
        labs = py::set<str>::from(labels);
        if (lb->label) labs.add(*lb->label);
    }
    if (!n) none_attr("children");
    for (const NodeP& c : n->children()) {
        if (leaves_walk(c, inner, labs)) return true;
    }
    return false;
}

}  // namespace

bool _leaves(const py::list<NodeP>& stmts) {
    for (const NodeP& s : stmts) {
        if (s && leaves_walk(s, 0, py::set<str>())) return true;
    }
    return false;
}

str _runtime_header(const str& owner) {
    str key = owner + ".h";
    auto& files = runtime::FILES();
    if (files.contains(key) && py::truthy(files.at(key))) return files.at(key);
    auto& gen = runtime::_GENERATED();
    if (gen.contains(key) && py::truthy(gen.at(key))) return gen.at(key);
    return S("");
}

namespace {
// (the header texts of the runtime do not change while the program runs, so what a scan of one finds for a name is
// found once: the Python scans again on every call)
std::optional<str> runtime_return_scan(const str& owner, const str& name) {
    py::re::Pattern rx = py::re::compile(
        py::percent(S("^\\s*(?:(?:inline|virtual|static)\\s+)*([A-Za-z_][\\w:<>,* &]*?)\\s*\\b%s\\s*\\("), py::re::escape(name)));
    for (const str& line : py::splitlines(_runtime_header(owner))) {
        auto m = rx.match(line);
        if (m) {
            str g = m->group(1).strip();
            if (g != S("return") && g != S("else") && g != S("new") && g != S("delete")) {
                str t = m->group(1).replace(S("virtual "), S("")).replace(S("inline "), S("")).replace(S("static "), S("")).strip();
                return t == S("std::string") ? S("JavaString") : t;
            }
        }
    }
    return std::nullopt;
}

}  // namespace

std::optional<str> _runtime_return(const str& owner, const str& name) {
    static std::unordered_map<std::u32string, std::optional<str>> memo;
    std::u32string key = owner.u32() + U'|' + name.u32();
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    return memo[key] = runtime_return_scan(owner, name);
}

namespace {
using FieldAnswer = std::tuple<std::optional<str>, std::optional<str>>;
FieldAnswer runtime_field_scan(const str& owner, const str& name) {
    str text = _runtime_header(owner);
    static const py::re::Pattern decl = py::re::compile(S("^\\s*(?:static\\s+)?(?:const\\s+)?([A-Za-z_][\\w:<>* &]*?)\\s+[A-Za-z_]"));
    for (const str& spelled : {S("s_") + name, name}) {
        py::re::Pattern word = py::re::compile(S("\\b") + py::re::escape(spelled) + S("\\b"));
        for (const str& line : py::splitlines(text)) {
            if (!word.search(line) || py::split(line, spelled)[0].contains(S("("))) continue;
            auto m = decl.match(line);
            if (m) return std::make_tuple(std::optional<str>(m->group(1).strip()), std::optional<str>(spelled));
        }
    }
    return std::make_tuple(std::optional<str>(), std::optional<str>());
}

}  // namespace

std::tuple<std::optional<str>, std::optional<str>> _runtime_field(const str& owner, const str& name) {
    static std::unordered_map<std::u32string, FieldAnswer> memo;
    std::u32string key = owner.u32() + U'|' + name.u32();
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    return memo[key] = runtime_field_scan(owner, name);
}

str unbox(const str& code, const str& ctype_, const str& target) {
    str t = strip_const_ref(target);
    if (ctype_.startswith(S("JObject")) && UNBOX().contains(t)) return py::percent(S("(%s).%s()"), code, UNBOX().at(t));
    return code;
}

str null_of(const str& ctype_) {
    str t = strip_const_ref(ctype_);
    if (t.empty() || t.endswith(S("*")) || t == S("void*")) return S("NULL");
    static const py::re::Pattern rx = py::re::compile(S("(JList|JMap|JSet)\\b"));
    if (rx.match(t)) return py::percent(S("%s(0LL)"), t);
    return py::percent(S("%s()"), t);
}

str coerce(const str& code, const str& ctype_, const str& target) {
    if (code == S("NULL") && py::truthy(target) && !is_pointer(target)) return null_of(target);
    return code;
}

std::optional<str> _class_of(const str& ctype_) {
    static const py::re::Pattern rx = py::re::compile(S("\\s*([A-Za-z_]\\w*)\\s*\\*\\s*$"));
    auto m = rx.match(ctype_);
    if (m) return m->group(1);
    return std::nullopt;
}

str _float(const ir::Val& v, const str& suffix) {
    double d = to_double(v);
    if (d != d || d == HUGE_VAL || d == -HUGE_VAL) throw Decline(S("non-finite literal"));
    str s = py::repr(d);
    if (s.contains(S("e")) || s.contains(S("E"))) {
        py::list<str> parts = py::split(s.lower(), S("e"));
        str mant = parts[0], exp = parts[1];
        if (!mant.contains(S("."))) mant = mant + ".0";
        s = py::percent(S("%se%s"), mant, exp);
    } else if (!s.contains(S("."))) {
        s = s + ".0";
    }
    return s + suffix;
}

str _cstring(const ir::Val& v, const py::dict<int64_t, int64_t>& char_map) {
    if (!std::holds_alternative<str>(v.v)) throw py::TypeError("object is not iterable");
    std::u32string out;
    for (char32_t c0 : std::get<str>(v.v).u32()) {
        int64_t o = static_cast<int64_t>(c0);
        o = char_map.get(o, o);   // a code the program's font draws elsewhere
        char32_t ch = static_cast<char32_t>(o);
        if (ch == U'\\') {
            out += U"\\\\";
        } else if (ch == U'"') {
            out += U"\\\"";
        } else if (ch == U'\n') {
            out += U"\\n";
        } else if (ch == U'\r') {
            out += U"\\r";
        } else if (ch == U'\t') {
            out += U"\\t";
        } else if (o < 32 || (127 <= o && o <= 255)) {   // the runtime's String is a char per byte (Latin-1),
            out.push_back(U'\\');                        // as the judge's inputs are: § is one byte, 0xA7
            out.push_back(static_cast<char32_t>(U'0' + ((o >> 6) & 7)));
            out.push_back(static_cast<char32_t>(U'0' + ((o >> 3) & 7)));
            out.push_back(static_cast<char32_t>(U'0' + (o & 7)));
        } else if (o > 255) {
            out.push_back(U'?');                         // outside the 8-bit runtime, as Latin-1 encodes it
        } else {
            out.push_back(ch);
        }
    }
    return S("\"") + str(out) + "\"";
}

// ---------------------------------------------------------------------------------------------- constructors, initialisers
namespace {

// The constructor this(...) calls: (IR constructor, the transpiler's parsed one).
std::tuple<py::ref<ir::MethodDecl>, MethodRef> delegate_target(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls,
                                                               const py::ref<ir::CtorCall>& call) {
    if (!call->sym) throw Decline(S("unresolved this(...)"));
    py::list<str> want;
    for (const TypeP& p : call->sym->params) want.append(param_key(jtype(p)));
    for (const NodeP& m_ : lst(cls->members)) {
        auto m = as_a<ir::MethodDecl>(m_);
        if (m && m->kind && *m->kind == S("ctor") && py::len(lst(m->params)) == py::len(want)) {
            py::list<str> got;
            for (const NodeP& p_ : *m->params) {
                auto p = as_a<ir::Param>(p_);
                got.append(param_key(jtype(p->type)));
            }
            if (got == want) {
                py::list<TypeP> ptypes;
                for (const NodeP& p_ : *m->params) ptypes.append(as_a<ir::Param>(p_)->type);
                MethodRef old_m = ctx->old_ctor(req(cls->name, "name"), ptypes);
                if (!old_m) throw Decline(S("delegate target not parsed"));
                return std::make_tuple(m, old_m);
            }
        }
    }
    throw Decline(S("delegate target not found"));
}

std::tuple<py::ref<ir::CtorCall>, py::list<NodeP>> split_ctor(const py::ref<ir::MethodDecl>& m) {
    py::list<NodeP> stmts;
    if (!(std::holds_alternative<NodeP>(m->body) && !std::get<NodeP>(m->body))) {
        NodeP b = body_node(m->body, "stmts");
        auto blk = as_a<ir::Block>(b);
        if (!blk) throw py::AttributeError("'" + b->class_name().utf8() + "' object has no attribute 'stmts'");
        stmts = lst(blk->stmts).copy();
    }
    py::ref<ir::CtorCall> first;
    if (!stmts.empty()) first = as_a<ir::CtorCall>(stmts[0]);
    return std::make_tuple(first, first ? stmts.slice_from(1) : stmts);
}

// (kind, arguments) of the base initialiser the delegation chain ends in, written with each target's
// parameters replaced by the (parenthesised) arguments it was called with.
std::tuple<std::optional<str>, std::optional<str>> delegated_super(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls,
                                                                   Writer& w, const py::ref<ir::CtorCall>& call, int64_t depth) {
    if (depth > 8) throw Decline(S("delegation too deep"));
    auto [target, old_m] = delegate_target(ctx, cls, call);
    auto tw = new_writer(ctx, cls, target, old_m);
    const py::list<NodeP>& tparams = lst(target->params);
    const py::list<NodeP>& cargs = lst(call->args);
    for (int64_t i = 0; i < std::min(py::len(tparams), py::len(cargs)); ++i) {
        auto p = as_a<ir::Param>(tparams[i]);
        tw->alias[tw->name(need_sym(p->sym).name)] = py::percent(S("(%s)"), std::get<0>(w.expr(cargs[i])));
    }
    auto [first, rest] = split_ctor(target);
    (void)rest;
    if (first && req(first->kind, "startswith").startswith(S("this"))) return delegated_super(ctx, cls, *tw, first, depth + 1);
    if (first && !req(first->kind, "endswith").endswith(S(":implicit"))) {
        if (first->target) throw Decline(S("qualified super call"));
        py::list<str> parts;
        for (const NodeP& a : lst(first->args)) parts.append(std::get<0>(tw->expr(a)));
        return std::make_tuple(std::optional<str>(S("super")), std::optional<str>(join_commas(parts)));
    }
    return std::make_tuple(std::optional<str>(), std::optional<str>());
}

// The delegated-to constructor's body, in a block: its parameters as locals holding the arguments.
py::list<str> delegated_body(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, Writer& w,
                             const py::ref<ir::CtorCall>& call, int64_t depth) {
    if (depth > 8) throw Decline(S("delegation too deep"));
    auto [target, old_m] = delegate_target(ctx, cls, call);
    auto tw = new_writer(ctx, cls, target, old_m);
    py::list<str> lines{S("{")};
    const py::list<NodeP>& tparams = lst(target->params);
    const py::list<NodeP>& cargs = lst(call->args);
    for (int64_t i = 0; i < std::min(py::len(tparams), py::len(cargs)); ++i) {
        auto p = as_a<ir::Param>(tparams[i]);
        str name = tw->name(need_sym(p->sym).name);
        str ct = tw->vars.at(name);
        str temp = py::percent(S("j2xD%d_%s"), depth, name);
        auto [code, at] = w.expr(cargs[i]);
        str local = strip_const_ref(ct);
        lines.append(py::percent(S("\t%s %s = %s;"), local, temp, unbox(coerce(code, at, local), at, local)));
        tw->vars[temp] = local;
        tw->alias[name] = temp;
    }
    auto [first, rest] = split_ctor(target);
    if (first && req(first->kind, "startswith").startswith(S("this"))) {
        for (const str& x : delegated_body(ctx, cls, *tw, first, depth + 1)) lines.append(S("\t") + x);
    }
    for (const NodeP& st : rest) tw->stmt(st, lines, S("\t"));
    lines.append(S("}"));
    return lines;
}

}  // namespace

jsource::CtorParts ctor_parts(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, const py::ref<ir::MethodDecl>& m,
                              const MethodRef& old_m, bool two_phase) {
    if (!ir::unsupported(m).empty()) throw Decline(S("unsupported construct"));
    auto w = new_writer(ctx, cls, m, old_m);
    py::list<NodeP> stmts;
    if (!(std::holds_alternative<NodeP>(m->body) && !std::get<NodeP>(m->body))) {
        NodeP b = body_node(m->body, "stmts");
        auto blk = as_a<ir::Block>(b);
        if (!blk) throw py::AttributeError("'" + b->class_name().utf8() + "' object has no attribute 'stmts'");
        stmts = lst(blk->stmts).copy();
    }
    py::ref<ir::CtorCall> first;
    if (!stmts.empty()) first = as_a<ir::CtorCall>(stmts[0]);
    if (first) stmts = stmts.slice_from(1);
    std::optional<str> args;
    if (first && !req(first->kind, "endswith").endswith(S(":implicit"))) {
        if (first->target) throw Decline(S("qualified super call"));
        py::list<str> parts;
        for (const NodeP& a : lst(first->args)) parts.append(std::get<0>(w->expr(a)));
        args = join_commas(parts);
    }
    py::list<str> out;
    if (two_phase) {
        w->two_phase = true;
        w->ret = req(cls->name, "name") + "*";
        if (first && req(first->kind, "startswith").startswith(S("this"))) {
            out.append(py::percent(S("%s::j2xInit(%s);"), cls->name, (args && py::truthy(*args)) ? *args : S("")));
            args = std::nullopt;
        }
    } else if (first && req(first->kind, "startswith").startswith(S("this"))) {
        // C++03 has no delegation: the other constructor's base initialiser comes here, with its
        // parameters replaced by this call's arguments, and its body runs first, in a block of its own
        auto [kind, dargs] = delegated_super(ctx, cls, *w, first, 0);
        for (const str& x : delegated_body(ctx, cls, *w, first, 0)) out.append(x);
        for (const NodeP& s : stmts) w->stmt(s, out, S(""));
        return jsource::CtorParts{kind, dargs, out};
    }
    for (const NodeP& s : stmts) w->stmt(s, out, S(""));
    std::optional<str> kind;
    if (first) kind = py::split(req(first->kind, "split"), S(":"))[0];
    return jsource::CtorParts{kind, args, out};
}

str field_init(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, const py::ref<ir::FieldDecl>& f) {
    if (!f->init || !ir::unsupported(f).empty()) throw Decline(S("field initialiser"));
    auto w = new_writer(ctx, cls, nullptr, nullptr);
    auto [code, ct] = w->expr(f->init);
    str owner = req(cls->name, "name");
    std::optional<str> target = ctx->field_ctype(owner, need_sym(f->sym).name);
    if (!target) throw Decline(py::percent(S("field %s.%s"), owner, f->sym->name));
    return unbox(coerce(code, ct, *target), ct, *target);
}

py::list<str> block_lines(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, const py::ref<ir::MethodDecl>& m) {
    if (!ir::unsupported(m).empty()) throw Decline(S("unsupported construct"));
    auto w = new_writer(ctx, cls, nullptr, nullptr);
    py::list<str> out;
    NodeP b = body_node(m->body, "stmts");
    auto blk = as_a<ir::Block>(b);
    if (!blk) {
        if (!b) none_attr("stmts");
        throw py::AttributeError("'" + b->class_name().utf8() + "' object has no attribute 'stmts'");
    }
    for (const NodeP& s : lst(blk->stmts)) w->stmt(s, out, S(""));
    return out;
}

py::list<str> method_body(const py::ref<Context>& ctx, const py::ref<ir::ClassDecl>& cls, const py::ref<ir::MethodDecl>& m,
                          const MethodRef& old_m) {
    if (!ir::unsupported(m).empty()) throw Decline(S("unsupported construct"));
    return new_writer(ctx, cls, m, old_m)->body();
}

// ---------------------------------------------------------------------------------------------- Bodies
namespace {

// the first field named `name` of `c`, or null
py::ref<ir::FieldDecl> find_field(const py::ref<ir::ClassDecl>& c, const str& name) {
    for (const NodeP& x : lst(c->members)) {
        auto f = as_a<ir::FieldDecl>(x);
        if (f && f->sym && f->sym->name == name) return f;
    }
    return nullptr;
}

str reason_text(const Decline& e) { return str(std::string(e.what())); }

}  // namespace

Bodies::Bodies(const py::ref<Context>& ctx_, const py::list<NodeP>& units) : ctx(ctx_) {
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        if (!u) {
            if (!un) none_attr("types");
            throw py::AttributeError("'" + un->class_name().utf8() + "' object has no attribute 'types'");
        }
        for (const NodeP& c_ : lst(u->types)) {
            auto c = as_a<ir::ClassDecl>(c_);
            if (!c) none_attr("name");
            decls.setdefault(c->name ? *c->name : S("None"), c);
        }
    }
    py::set<str> q;
    for (const auto& c : decls.values()) {
        if (c->qname && py::truthy(*c->qname)) q.add(*c->qname);
    }
    ctx->qnames = q;
}

std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>> Bodies::_find(const str& cls_name, const MethodRef& old_m) {
    if (!decls.contains(cls_name)) return std::make_tuple(py::ref<ir::ClassDecl>(), py::ref<ir::MethodDecl>());
    auto c = decls.at(cls_name);
    py::list<str> want;
    for (const auto& p : old_m->params) want.append(param_key(p->jtype));
    for (const NodeP& m_ : lst(c->members)) {
        auto m = as_a<ir::MethodDecl>(m_);
        if (m && m->kind && *m->kind == S("method") && m->name == old_m->name && py::len(lst(m->params)) == py::len(old_m->params)) {
            py::list<str> got;
            try {
                for (const NodeP& p_ : *m->params) got.append(param_key(jtype(as_a<ir::Param>(p_)->type)));
            } catch (const Decline&) {
                continue;
            }
            if (got == want) return std::make_tuple(c, m);
        }
    }
    return std::make_tuple(c, py::ref<ir::MethodDecl>());
}

std::tuple<py::ref<ir::ClassDecl>, py::ref<ir::MethodDecl>> Bodies::_ctor(const str& cls_name, const MethodRef& old_m) {
    if (!decls.contains(cls_name)) return std::make_tuple(py::ref<ir::ClassDecl>(), py::ref<ir::MethodDecl>());
    auto c = decls.at(cls_name);
    py::list<py::ref<ir::MethodDecl>> ctors;
    for (const NodeP& m_ : lst(c->members)) {
        auto m = as_a<ir::MethodDecl>(m_);
        if (m && m->kind && *m->kind == S("ctor")) ctors.append(m);
    }
    if (!old_m) {   // the implicit default constructor
        for (const auto& m : ctors) {
            if (!m->params || m->params->empty()) return std::make_tuple(c, m);
        }
        return std::make_tuple(c, py::ref<ir::MethodDecl>());
    }
    py::list<str> want;
    for (const auto& p : old_m->params) want.append(param_key(p->jtype));
    for (const auto& m : ctors) {
        if (py::len(lst(m->params)) != py::len(old_m->params)) continue;
        py::list<str> got;
        try {
            for (const NodeP& p_ : *m->params) got.append(param_key(jtype(as_a<ir::Param>(p_)->type)));
        } catch (const Decline&) {
            continue;
        }
        if (got == want) return std::make_tuple(c, m);
    }
    return std::make_tuple(c, py::ref<ir::MethodDecl>());
}

std::optional<jsource::CtorParts> Bodies::ctor(const str& cls_name, const MethodRef& old_m, bool two_phase) {
    auto [c, m] = _ctor(cls_name, old_m);
    if (!m) {
        bump(declined, S("ctor not found"));
        return std::nullopt;
    }
    jsource::CtorParts parts;
    try {
        parts = ctor_parts(ctx, c, m, old_m, two_phase);
    } catch (const Decline& e) {
        str why = reason_text(e);
        bump(declined, S("ctor ") + (py::truthy(why) ? py::split(why, S(" "))[0] : S("")));
        bump(reasons, S("ctor: ") + why);
        return std::nullopt;
    }
    written_ctors += 1;
    return parts;
}

std::optional<str> Bodies::field_init(const str& cls_name, const str& name) {
    if (!decls.contains(cls_name)) return std::nullopt;
    auto c = decls.at(cls_name);
    auto f = find_field(c, name);
    if (!f || !f->init) return std::nullopt;
    try {
        return cpp03::field_init(ctx, c, f);
    } catch (const Decline& e) {
        bump(reasons, S("field init: ") + reason_text(e));
        return std::nullopt;
    }
}

std::optional<py::list<str>> Bodies::enum_args(const str& cls_name, const str& const_) {
    if (!decls.contains(cls_name)) return std::nullopt;
    auto c = decls.at(cls_name);
    py::ref<ir::EnumConst> e;
    for (const NodeP& x : lst(c->members)) {
        auto ec = as_a<ir::EnumConst>(x);
        if (ec && ec->sym && ec->sym->name == const_) {
            e = ec;
            break;
        }
    }
    if (!e || e->body) return std::nullopt;
    try {
        auto w = new_writer(ctx, c, nullptr, nullptr);
        Nodes args = e->args && !e->args->empty() ? e->args : Nodes(py::list<NodeP>());
        py::list<str> out;
        for (const NodeP& a : w->unmarked(args)) out.append(w->arg(a));
        return out;
    } catch (const Decline& ex) {
        bump(reasons, S("enum constant: ") + reason_text(ex));
        return std::nullopt;
    }
}

std::optional<py::list<str>> Bodies::array_items(const str& cls_name, const str& name) {
    if (!decls.contains(cls_name)) return std::nullopt;
    auto c = decls.at(cls_name);
    auto f = find_field(c, name);
    NodeP init_n = f ? f->init : NodeP();
    auto init = as_a<ir::NewArray>(init_n);
    if (!init || !init->init || (init->dims && !init->dims->empty())) return std::nullopt;
    try {
        auto w = new_writer(ctx, c, nullptr, nullptr);
        std::optional<str> fc = ctx->field_ctype(cls_name, name);
        str elem = _elem((fc && py::truthy(*fc)) ? *fc : w->ctype(init->type));
        py::list<str> out;
        for (const NodeP& x : *init->init) {
            auto [code, ct] = w->expr(x);
            out.append(unbox(coerce(code, ct, elem), ct, elem));
        }
        return out;
    } catch (const Decline& ex) {
        bump(reasons, S("array items: ") + reason_text(ex));
        return std::nullopt;
    }
}

std::optional<py::list<str>> Bodies::instance_inits(const str& cls_name) {
    if (!decls.contains(cls_name)) return std::nullopt;
    auto c = decls.at(cls_name);
    py::list<str> out;
    try {
        for (const NodeP& x : lst(c->members)) {
            auto f = as_a<ir::FieldDecl>(x);
            if (f && f->init && f->sym && !f->sym->static_) {
                if (f->type && f->type->kind == S("array")) {
                    auto lit = as_a<ir::Lit>(f->init);
                    if (lit && lit->value.is_none()) continue;   // an array field = null: JArray starts empty
                }
                out.append(py::percent(S("this->%s = %s;"), f->sym->name, cpp03::field_init(ctx, c, f)));
            } else if (auto md = as_a<ir::MethodDecl>(x)) {
                if (md->kind && *md->kind == S("init")) out.extend(block_lines(ctx, c, md));
            }
        }
    } catch (const Decline& e) {
        bump(reasons, S("instance init: ") + reason_text(e));
        return std::nullopt;
    }
    return out;
}

std::optional<py::list<py::list<str>>> Bodies::static_blocks(const str& cls_name) {
    if (!decls.contains(cls_name)) return std::nullopt;
    auto c = decls.at(cls_name);
    try {
        py::list<py::list<str>> out;
        for (const NodeP& x : lst(c->members)) {
            auto md = as_a<ir::MethodDecl>(x);
            if (md && md->kind && *md->kind == S("static_init")) out.append(block_lines(ctx, c, md));
        }
        return out;
    } catch (const Decline& e) {
        bump(reasons, S("static block: ") + reason_text(e));
        return std::nullopt;
    }
}

std::optional<py::list<str>> Bodies::body(const str& cls_name, const MethodRef& old_m) {
    auto [c, m] = _find(cls_name, old_m);
    if (!m) {
        bump(declined, S("not found"));
        return std::nullopt;
    }
    py::list<str> lines;
    try {
        lines = method_body(ctx, c, m, old_m);
    } catch (const Decline& e) {
        str why = reason_text(e);
        bump(declined, py::truthy(why) ? py::split(why, S(" "))[0] : S("declined"));
        bump(reasons, why);
        return std::nullopt;
    }
    written += 1;
    return lines;
}

// ============================================================================================ the C++17 dialect
void Writer17::_stmt(const NodeP& s, const py::list<str>& out, const str& ind) {
    if (auto fe = as_a<ir::ForEach>(s)) return foreach(fe, out, ind);
    if (auto lo = as_a<ir::Local>(s)) {
        auto n17 = [](const NodeP& x) {
            auto l = as_a<ir::Lit>(_strip_paren(x));
            return l && l->value.is_none();
        };
        if (n17(lo->init)) {
            str ct = ctype(lo->type);
            if (is_pointer(ct)) {   // a pointer starts as nullptr
                str nm = name(need_sym(lo->sym).name);
                if (vars.contains(nm)) throw Decline(S("shadowed local ") + nm);
                vars[nm] = ct;
                out.append(ind + py::percent(S("%s %s = nullptr;"), ct, nm));
                return;
            }
        }
        if (lo->init && spelled(lo)) return auto_local(lo, out, ind);
    }
    Writer::_stmt(s, out, ind);
}

namespace {
bool null17(const NodeP& e_) {
    NodeP e = e_ ? _strip_paren(e_) : NodeP();
    auto l = as_a<ir::Lit>(e);
    return l && l->value.is_none();
}
}  // namespace

Code Writer17::x_Binary(const py::ref<ir::Binary>& e) {
    if (e->op && (*e->op == S("==") || *e->op == S("!=")) && (null17(e->left) || null17(e->right))) {
        NodeP other = null17(e->left) ? e->right : e->left;
        bool pointer;
        try {
            TypeP ot = type_of(other);
            pointer = ot && ot->is_ref() && is_pointer(ctype(ot));
        } catch (const Decline&) {
            pointer = false;
        }
        if (pointer) {
            auto [code, ct] = expr(other);
            if (is_pointer(ct)) return Code(py::percent(S("%s %s nullptr"), code, e->op), S("bool"));
        }
    }
    return Writer::x_Binary(e);
}

bool Writer17::spelled(const py::ref<ir::Local>& s) {
    NodeP init = s->init;
    while (auto p = as_a<ir::Paren>(init)) init = p->x;
    if (!(is_a<ir::New>(init) || is_a<ir::Cast>(init)) || !s->type || !type_of(init)) return false;
    return *type_of(init) == *s->type && s->type->kind == S("class") &&
           !(is_a<ir::New>(init) && as_a<ir::New>(init)->body);
}

void Writer17::auto_local(const py::ref<ir::Local>& s, const py::list<str>& out, const str& ind) {
    py::list<str> lines;
    Writer::_stmt(s, lines, ind);
    str ct = vars.get(name(need_sym(s->sym).name), str(""));
    if (py::len(lines) == 1 && py::truthy(ct) && lines[0].startswith(ind + ct + " ")) {
        str star = is_pointer(ct) ? S("*") : S("");
        if (py::truthy(star) || ct.startswith(S("JavaString")) || ct.startswith(S("JList")) || ct.startswith(S("JMap")) ||
            ct.startswith(S("JSet"))) {
            lines[0] = ind + py::percent(S("auto%s "), star) + lines[0].slice_from(py::len(ind + ct) + 1);
        }
    }
    out.extend(lines);
}

void Writer17::foreach(const py::ref<ir::ForEach>& s, const py::list<str>& out, const str& ind) {
    auto [coll, ct] = expr(s->iterable);
    auto var = as_a<ir::Local>(s->var);
    if (!var) none_attr("sym");
    str nm = name(need_sym(var->sym).name);
    str vt = ctype(var->type);
    if (vars.contains(nm)) throw Decline(S("shadowed loop variable ") + nm);
    if (ct.startswith(S("JList<")) || ct.startswith(S("const JList<"))) {
        vars[nm] = vt;
        out.append(ind + py::percent(S("for (%s %s : %s) {"), vt, nm, coll));
        loop_body(s->body, out, ind);
        out.append(ind + "}");
        vars.pop(nm, str(""));
        return;
    }
    if (ct.startswith(S("JArray<"))) {
        temps += 1;
        str arr = py::percent(S("j2x_a%d"), temps), idx = py::percent(S("j2x_i%d"), temps);
        out.append(ind + "{");
        out.append(ind + py::percent(S("\t%s %s = %s;"), ct, arr, coll));
        out.append(ind + py::percent(S("\tfor (int %s = 0; %s < %s.length; %s++) {"), idx, idx, arr, idx));
        vars[nm] = vt;
        out.append(ind + py::percent(S("\t\t%s %s = %s[%s];"), vt, nm, arr, idx));
        loop_body(s->body, out, ind + "\t");
        out.append(ind + "\t}");
        out.append(ind + "}");
        vars.pop(nm, str(""));
        return;
    }
    throw Decline(S("for-each over ") + ct);
}

Code Writer17::x_SwitchExpr(const py::ref<ir::SwitchExpr>& e) {
    str t = ctype(e->type);
    NodeP se = e->expr;
    if (auto p = as_a<ir::Paren>(se)) se = p->x;
    auto [x, xt] = expr(se);
    if (xt.contains(S("JavaString"))) throw Decline(S("switch on a string"));
    py::list<str> lines{py::percent(S("[&]() -> %s {"), t), py::percent(S("\tswitch(%s) {"), x)};
    str saved_ret = ret;
    ret = t;
    bool saved_two = two_phase;
    two_phase = false;
    auto restore = [&]() {
        ret = saved_ret;
        two_phase = saved_two;
    };
    try {
        for (const NodeP& c_ : lst(e->cases)) {
            auto c = as_a<ir::Case>(c_);
            if (!c) none_attr("labels");
            for (const NodeP& lab : lst(c->labels)) lines.append(py::percent(S("\tcase %s:"), std::get<0>(expr(lab))));
            if (c->default_ && *c->default_) lines.append(S("\tdefault:"));
            py::list<NodeP> body;
            for (const NodeP& b : lst(c->body)) {
                if (auto y = as_a<ir::Yield>(b)) {
                    auto rt = std::make_shared<ir::Return>();
                    rt->expr = y->expr;
                    rt->line = y->line;
                    rt->col = y->col;
                    body.append(rt);
                } else {
                    body.append(b);
                }
            }
            auto holder = std::make_shared<ir::Block>();
            holder->stmts = body;
            for (const NodeP& b : ir::walk(holder)) {
                if (is_a<ir::Yield>(b)) throw Decline(S("a yield inside a nested statement"));
            }
            lines.append(S("\t{"));
            for (const NodeP& b : body) stmt(b, lines, S("\t\t"));
            lines.append(S("\t}"));
        }
    } catch (...) {
        restore();
        throw;
    }
    restore();
    lines.append(S("\t}"));
    lines.append(S("\tthrow 0;"));
    lines.append(S("}()"));
    return Code(py::join(S("\n"), lines), t);
}

bool Writer17::handles(const NodeP& e) const { return Writer::handles(e) || is_a<ir::SwitchExpr>(e); }

Code Writer17::dispatch(const NodeP& e) {
    if (auto x = as_a<ir::SwitchExpr>(e)) return x_SwitchExpr(x);
    return Writer::dispatch(e);
}

str _keep_null(const str& x) {
    static const py::re::Pattern null_word = py::re::compile(S("\\bNULL\\b"));
    if (x.contains(S("NULL")))
        return codebreaker::java::emit::std_transforms::_sub_outside_literals(null_word, S("__null"), x);
    return x;
}

namespace {

py::list<str> keep_null(const py::list<str>& xs) {
    py::list<str> out;
    for (const str& x : xs) out.append(_keep_null(x));
    return out;
}

}  // namespace

Bodies17::Bodies17(const py::ref<Context>& ctx_, const py::list<NodeP>& units) : Bodies(ctx_, [&]() -> const py::list<NodeP>& {
    py::list<py::str> keep{S("foreach"), S("switchexpr")};
    desugar::run(units, keep);
    return units;
}()) {}

std::optional<jsource::CtorParts> Bodies17::ctor(const str& cls_name, const MethodRef& old_m, bool two_phase) {
    Writer17Scope scope;
    auto r = Bodies::ctor(cls_name, old_m, two_phase);
    if (!r) return r;
    if (r->kind) r->kind = _keep_null(*r->kind);
    if (r->args) r->args = _keep_null(*r->args);
    r->body = keep_null(r->body);
    return r;
}

std::optional<str> Bodies17::field_init(const str& cls_name, const str& name) {
    Writer17Scope scope;
    auto r = Bodies::field_init(cls_name, name);
    if (!r) return r;
    return _keep_null(*r);
}

std::optional<py::list<str>> Bodies17::enum_args(const str& cls_name, const str& const_) {
    Writer17Scope scope;
    auto r = Bodies::enum_args(cls_name, const_);
    if (!r) return r;
    return keep_null(*r);
}

std::optional<py::list<str>> Bodies17::array_items(const str& cls_name, const str& name) {
    Writer17Scope scope;
    auto r = Bodies::array_items(cls_name, name);
    if (!r) return r;
    return keep_null(*r);
}

std::optional<py::list<str>> Bodies17::instance_inits(const str& cls_name) {
    Writer17Scope scope;
    auto r = Bodies::instance_inits(cls_name);
    if (!r) return r;
    return keep_null(*r);
}

std::optional<py::list<py::list<str>>> Bodies17::static_blocks(const str& cls_name) {
    Writer17Scope scope;
    auto r = Bodies::static_blocks(cls_name);
    if (!r) return r;
    py::list<py::list<str>> out;
    for (const auto& blk : *r) out.append(keep_null(blk));
    return out;
}

std::optional<py::list<str>> Bodies17::body(const str& cls_name, const MethodRef& old_m) {
    Writer17Scope scope;
    auto r = Bodies::body(cls_name, old_m);
    if (!r) return r;
    return keep_null(*r);
}

}  // namespace codebreaker::translate::backends::cpp03

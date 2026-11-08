// codebreaker/translate/backends/java_objects.cpp -- typed C++ port of codebreaker.translate.backends.java_objects
// (see java_objects.hpp).
#include "codebreaker/translate/backends/java_objects.hpp"

#include <algorithm>

#include "codebreaker/translate/backends/java_libc.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::backends::java_objects {

using ir::NodeOrList;
using ir::Nodes;
using ir::Val;
using jb::P_ASSIGN;
using jb::P_ATOM;
using jb::P_COND;
using jb::P_EQ;
using jb::P_POST;
using jb::Refused;
using jb::Sig;
using jb::is_ptr;
using jb::is_record;
using jb::jname;
using jb::strip;
using py::str;

namespace {

str S(const char* s) { return str(s); }

[[noreturn]] void none_attr(const char* attr) {
    throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
}

template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

void app(std::u32string& o, const str& s) { o += s.u32(); }
void app(std::u32string& o, const char* s) {
    while (*s) o.push_back(static_cast<char32_t>(static_cast<unsigned char>(*s++)));
}
void app(std::u32string& o, int64_t v) {
    std::string t = std::to_string(v);
    for (char c : t) o.push_back(static_cast<char32_t>(c));
}
template <class... A> str cat(const A&... a) {
    std::u32string o;
    (app(o, a), ...);
    return str(std::move(o));
}

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
str or_none(const std::optional<str>& a) { return a ? *a : S("None"); }
bool truthy(const std::optional<str>& a) { return a && !a->empty(); }

const ir::Type& K(const TypeP& t) {
    if (!t) none_attr("kind");
    return *t;
}
bool has(const ir::Strs& xs, const char* x) {
    if (!xs) return false;
    for (const str& s : *xs) {
        if (is(s, x)) return true;
    }
    return false;
}
const py::list<NodeP>& lst(const Nodes& v) {
    if (!v) throw py::TypeError("'NoneType' object is not iterable");
    return *v;
}
py::list<NodeP> lst_or_empty(const Nodes& v) { return v ? *v : py::list<NodeP>(); }

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
str val_str(const Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return py::to_str(std::get<int64_t>(v.v));
    if (std::holds_alternative<bool>(v.v)) return py::to_str(std::get<bool>(v.v));
    if (std::holds_alternative<str>(v.v)) return std::get<str>(v.v);
    return v.repr();
}
TypeP val_type(const Val& v) {
    if (std::holds_alternative<TypeP>(v.v)) return std::get<TypeP>(v.v);
    return nullptr;
}
int64_t py_int(const Val& v) {
    if (std::holds_alternative<int64_t>(v.v)) return std::get<int64_t>(v.v);
    if (std::holds_alternative<bool>(v.v)) return std::get<bool>(v.v) ? 1 : 0;
    if (std::holds_alternative<double>(v.v)) return static_cast<int64_t>(std::get<double>(v.v));
    if (std::holds_alternative<str>(v.v)) return py::int_parse(std::get<str>(v.v), 10);
    throw py::TypeError("int() argument must be a string, a bytes-like object or a real number, not 'NoneType'");
}

TypeP type_of(const NodeP& e) {
    if (!e) none_attr("type");
    if (auto x = as_a<ir::Expr>(e)) return x->type;
    if (auto l = as_a<ir::Local>(e)) return l->type;
    if (auto p = as_a<ir::Param>(e)) return p->type;
    if (auto f = as_a<ir::FieldDecl>(e)) return f->type;
    if (auto p = as_a<ir::Property>(e)) return p->type;
    throw py::AttributeError("'" + e->class_name().utf8() + "' object has no attribute 'type'");
}

ir::Sym& need_sym(const SymP& s) {
    if (!s) none_attr("name");
    return *s;
}

bool is_none(const NodeOrList& v) { return std::holds_alternative<NodeP>(v) && !std::get<NodeP>(v); }

// `m.body.stmts` (the statements of a method body)
py::list<NodeP> body_stmts(const NodeOrList& body) {
    if (std::holds_alternative<py::list<NodeP>>(body)) throw py::AttributeError("'list' object has no attribute 'stmts'");
    NodeP b = std::get<NodeP>(body);
    if (!b) none_attr("stmts");
    auto blk = as_a<ir::Block>(b);
    if (!blk) throw py::AttributeError("'" + b->class_name().utf8() + "' object has no attribute 'stmts'");
    return lst(blk->stmts);
}
bool body_set(const NodeOrList& body) { return !is_none(body); }   // m.body is not None

str join_sep(const char* sep, const py::list<str>& parts) { return py::join(S(sep), parts); }

ClassP rec_get(Objects& o, const str& k) { return o.records.contains(k) ? o.records.at(k) : nullptr; }

str indent_(const str& x, const char* pre) { return !x.empty() ? S(pre) + x : x; }

// ---- _base / _dims / spell_t
TypeP _base(TypeP t) {
    while (is(K(t).kind, "array")) t = t->elem;
    return t;
}
std::vector<int64_t> _dims(TypeP t) {
    std::vector<int64_t> out;
    while (is(K(t).kind, "array")) {
        Val c = val_get(t->info, "count");
        out.push_back(t->info.contains(S("count")) ? py_int(c) : 0);
        t = t->elem;
    }
    return out;
}
str spell_t(const TypeP& t) { return jb::spell(t); }

std::vector<TypeP> vec_of(const py::list<TypeP>& xs) {
    std::vector<TypeP> out;
    for (const TypeP& t : xs) out.push_back(t);
    return out;
}

str at_(const Place& pl) { return pl.prec >= P_POST ? pl.java : cat("(", pl.java, ")"); }

}  // namespace

str method_name(const str& name) {
    static const py::dict<str, str> OPS = {
        {S("operator+"), S("plus")},       {S("operator-"), S("minus")},        {S("operator*"), S("times")},
        {S("operator/"), S("div")},        {S("operator%"), S("rem")},          {S("operator=="), S("isEqual")},
        {S("operator!="), S("notEqual")},  {S("operator<"), S("lessThan")},    {S("operator>"), S("greaterThan")},
        {S("operator<="), S("atMost")},    {S("operator>="), S("atLeast")},     {S("operator[]"), S("at")},
        {S("operator()"), S("call")},      {S("operator+="), S("plusAssign")},  {S("operator-="), S("minusAssign")},
        {S("operator*="), S("timesAssign")}, {S("operator/="), S("divAssign")}, {S("operator="), S("assign")},
        {S("operator!"), S("not")},        {S("operator<<"), S("shiftLeft")},   {S("operator>>"), S("shiftRight")},
        {S("operator&"), S("and")},        {S("operator|"), S("or")},           {S("operator^"), S("xor")},
        {S("operator~"), S("complement")}, {S("operator++"), S("increment")},   {S("operator--"), S("decrement")},
        {S("operator bool"), S("toBool")}};
    if (OPS.contains(name)) return OPS.at(name);
    if (starts(name, "operator")) throw Refused(S("operator ") + name);
    return jname(name);
}

std::shared_ptr<Objects> make_objects(const py::list<NodeP>& units, const str& name, const std::shared_ptr<jb::Analysis>& a) {
    auto w = std::make_shared<Objects>(units, name, a);
    w->init();
    return w;
}

// ============================================================================================ Objects
void Objects::_check_records() {
    for (const auto& kv : records.items()) {
        const ClassP& c = std::get<1>(kv);
        if (opt_is(c->kind, "union")) throw Refused(S("union ") + or_none(c->name));
        if (c->implements && !c->implements->empty()) throw Refused(or_none(c->name) + " has more than one base class");
        for (const NodeP& m : lst_or_empty(c->members)) {
            ir::Strs mods;
            if (auto f = as_a<ir::FieldDecl>(m)) mods = f->mods;
            else if (auto d = as_a<ir::MethodDecl>(m)) mods = d->mods;
            else if (auto p = as_a<ir::Property>(m)) mods = p->mods;
            else if (auto cc = as_a<ir::ClassDecl>(m)) mods = cc->mods;
            else throw py::AttributeError("'" + m->class_name().utf8() + "' object has no attribute 'mods'");
            if (has(mods, "bitfield")) throw Refused(S("bit-fields in ") + or_none(c->name));
            if (auto f = as_a<ir::FieldDecl>(m); f && f->sym && starts(f->sym->name, "<"))
                throw Refused(S("a member the front end did not lower in ") + or_none(c->name));
        }
    }
    ctors_used = py::set<str>();
    copied = py::set<str>();
    assigned = py::set<str>();
    arrays_filled = false;
}

void Objects::_names() {
    py::set<str> taken;
    for (const auto& kv : functions.items()) taken.add(jname(std::get<0>(kv)));
    for (const FieldP& f : global_decls) taken.add(jname(need_sym(f->sym).name));
    taken.add(name);
    cls_names = py::dict<str, str>();
    std::vector<std::pair<str, ClassP>> order;
    for (const auto& kv : records.items()) order.push_back({std::get<0>(kv), std::get<1>(kv)});
    std::stable_sort(order.begin(), order.end(), [](const auto& a, const auto& b) { return a.second->line < b.second->line; });
    for (const auto& kv : order) {
        const str& qn = kv.first;
        const ClassP& c = kv.second;
        if (!c->name) none_attr("replace");
        str base = c->name->replace(S("__unnamed_"), S("Anon"));
        if (qn.contains(S("<"))) {                     // a template instance: Stack<i32, 8> -> Stack_int_8
            int64_t lt = qn.u32().find(U'<');
            str args = qn.slice(lt + 1, qn.size() - 1);
            py::list<str> alts;
            for (const auto& pk : jb::JPRIM().items()) alts.append(std::get<0>(pk));
            str pat = S("\\b(") + join_sep("|", alts) + ")\\b";
            args = py::re::sub(pat, [](const py::re::Match& m) { return jb::JPRIM().at(m.group(1)); }, args);
            args = py::re::sub(S("\\W+"), S("_"), args).strip(S("_"));
            base = base + "_" + args;
        }
        base = jname(base);
        str n = base;
        while (taken.contains(n)) n = n + "_";
        taken.add(n);
        cls_names.set(qn, n);
    }
    layout_names = cls_names;
    used_names = taken;
}

// ------------------------------------------------------------------------------------- types
str Objects::cls(const TypeP& t) {
    str qn = rec_name(t);
    if (starts(qn, "java.") && !cls_names.contains(qn)) {
        const std::u32string& u = qn.u32();
        size_t at = u.rfind(U'.');
        return str(u.substr(at + 1));                 // Java's own library (a program read back from Java)
    }
    if (!cls_names.contains(qn)) {
        if (starts(qn, "std.")) throw Refused(S("the C++ standard library (") + qn.replace(S("."), S("::")) + ") is not carried yet");
        throw Refused(S("no class for ") + (t ? t->repr() : S("None")));
    }
    return cls_names.at(qn);
}

std::optional<int64_t> Objects::size_hint(const TypeP& t) {
    try {
        return size(t);
    } catch (const Refused&) {
        return std::nullopt;
    }
}

bool Objects::is_enum(const TypeP& t) {
    ClassP c = (t && is(t->kind, "class") && records.contains(rec_name(t))) ? records.at(rec_name(t)) : nullptr;
    return c && opt_is(c->kind, "enum");
}

str Objects::jt(const TypeP& t) {
    str k = jb::_kind(t);
    if (jb::JPRIM().contains(k)) return jb::JPRIM().at(k);
    if (is(k, "void")) return S("void");
    if (in(k, {"class", "record"})) {
        if (is_enum(t)) return S("int");
        return cls(t);
    }
    if (is(k, "array")) return jt(t->elem) + "[]";
    if (is(k, "ptr")) {
        TypeP e = t->elem;
        if (is_record(e)) return cls(e);
        if (!e || is(e->kind, "void")) throw Refused("a void pointer");
        if (is(e->kind, "fn")) return iface(fn_sig_of_type(e));
        return jt(e) + "[]";
    }
    if (is(k, "fnptr")) return iface(fn_sig_of_type(t));
    if (is(k, "null")) return S("Object");
    throw Refused(S("type ") + (t ? t->repr() : S("None")));
}

Sig Objects::fn_sig(const SymP& s) {
    Sig sig;
    for (const TypeP& p : s->params) sig.params.push_back(jt(p));
    sig.ret = s->ret ? jt(s->ret) : S("void");
    return sig;
}

Sig Objects::fn_sig_of_type(const TypeP& ft) {
    TypeP f = is(K(ft).kind, "fnptr") ? val_type(ft->info.at(S("fn"))) : ft;
    Sig sig;
    for (const TypeP& p : jb::fn_params(f)) sig.params.push_back(jt(p));
    TypeP rt = jb::fn_result(f);
    sig.ret = rt ? jt(rt) : S("void");
    return sig;
}

str Objects::iface(const Sig& sig) {
    for (const auto& kv : fn_ifaces) {
        if (kv.first == sig) return kv.second;
    }
    auto word = [](str p) {
        p = p.replace(S("[]"), S("Array"));
        return p.slice(0, 1).upper() + p.slice_from(1);
    };
    str n;
    for (const str& p : sig.params) n = n + word(p);
    n = n + "To" + word(sig.ret);
    fn_ifaces.push_back({sig, n});
    return n;
}

str Objects::default_(const TypeP& t) {
    // A fresh value of type t: what C's zero-initialised storage holds, as Java.
    str k = jb::_kind(t);
    if (in(k, {"class", "record"})) return S("new ") + cls(t) + "()";
    if (is(k, "array") && !t->info.contains(S("count"))) return S("null");   // (an array reference -- Java's own, read back -- not storage)
    if (is(k, "array")) {
        std::vector<int64_t> dims;
        TypeP base = t;
        while (is(base->kind, "array")) {
            if (!base->info.contains(S("count"))) throw Refused("an array of unknown size");
            dims.push_back(py_int(base->info.at(S("count"))));
            base = base->elem;
        }
        if (is_record(base)) {
            if (dims.size() != 1) throw Refused("a multi-dimensional array of structs");
            arrays_filled = true;
            return cat("filled(new ", cls(base), "[", dims[0], "], ", cls(base), "::new)");
        }
        str out = S("new ") + jt(base);
        for (int64_t d : dims) out = out + "[" + py::to_str(d) + "]";
        return out;
    }
    if (in(k, {"ptr", "fnptr", "null"})) return S("null");
    str j = jb::JPRIM().get(k, S(""));
    if (is(j, "boolean")) return S("false");
    if (is(j, "float")) return S("0.0f");
    if (is(j, "double")) return S("0.0");
    if (is(j, "long")) return S("0L");
    return S("0");
}

// ------------------------------------------------------------------------------------- the file
str Objects::write() {
    fields_out = Lines();
    static_init = Lines();
    boxed = py::set<str>();
    for (const FieldP& g : global_decls) declare_global(g);
    // (written until what the classes must offer -- aggregate, copy and assignment constructors -- is
    // stable: a method written later can ask it of a class written earlier)
    std::optional<std::tuple<int64_t, int64_t, int64_t, bool>> seen;
    py::list<Lines> funcs;
    Lines classes;
    for (int round = 0; round < 4; ++round) {
        gaps = Lines();
        funcs = py::list<Lines>();
        for (const MethodP& f : all_functions()) {
            funcs.append(function_or_gap(f, [&]() { return ObjWriter(this, f).write(); }));
        }
        classes = Lines();
        std::vector<std::pair<std::pair<int64_t, str>, str>> order;
        for (const auto& kv : records.items()) order.push_back({{std::get<1>(kv)->line, std::get<0>(kv)}, std::get<0>(kv)});
        std::sort(order.begin(), order.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& o : order) classes.extend(class_decl(records.at(o.second)));
        auto now = std::make_tuple(ctors_used.size(), copied.size(), assigned.size(), arrays_filled);
        if (seen && *seen == now) break;
        seen = now;
    }
    Lines out;
    out.append(cat("// ", name, ".java -- translated from ", a->cpp_classes ? "C++" : "C",
                   " by CodeBreaker (idiomatic tier: structs as classes, pointers as references)."));
    out.append(S("import crt.*;"));
    out.append(S(""));
    out.append(S("import static crt.LibC.*;"));
    out.append(S(""));
    out.append(cat("public final class ", name, " {"));
    out.extend(classes);
    for (const auto& kv : fn_ifaces) {
        const Sig& sig = kv.first;
        py::list<str> ps;
        for (size_t i = 0; i < sig.params.size(); ++i) ps.append(cat(sig.params[i], " a", static_cast<int64_t>(i)));
        out.append(S(""));
        out.append(cat("    interface ", kv.second, " {"));
        out.append(cat("        ", sig.ret, " call(", join_sep(", ", ps), ");"));
        out.append(S("    }"));
    }
    if (!fields_out.empty()) {
        out.append(S(""));
        for (const str& f : fields_out) out.append(S("    ") + f);
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

std::optional<str> Objects::gap_signature(const MethodP& f, const std::optional<str>& owner) {
    Sig sig;
    str ret;
    try {
        sig = fn_sig(f->sym);
        ret = sig.ret;
        if (f->ret && is(f->ret->kind, "ptr") && val_truthy(val_get(f->ret->info, "ref")) && !is_record(f->ret->elem)) ret = jt(f->ret->elem);
    } catch (const Refused&) {
        return std::nullopt;
    }
    py::list<str> ps;
    std::vector<NodeP> params;
    for (const NodeP& p : lst(f->params)) params.push_back(p);
    for (size_t i = 0; i < std::min(sig.params.size(), params.size()); ++i)
        ps.append(sig.params[i] + " " + jname(need_sym(as_a<ir::Param>(params[i])->sym).name));
    str pstr = join_sep(", ", ps);
    if (!owner) return cat("static ", ret, " ", jname(f->name), "(", pstr, ")");
    if (!opt_is(f->kind, "method")) return std::nullopt;
    return cat(has(f->mods, "static") ? "static " : "", ret, " ", method_name(or_none(f->name)), "(", pstr, ")");
}

Lines Objects::entry() {
    Lines out;
    if (!functions.contains(S("main"))) {
        // a program read back from Java: its class with `static void main(String[])`
        for (const auto& kv : records.items()) {
            const ClassP& c = std::get<1>(kv);
            for (const NodeP& x : lst(c->members)) {
                auto md = as_a<ir::MethodDecl>(x);
                if (md && opt_is(md->name, "main") && has(md->mods, "static") && py::len(lst(md->params)) == 1 &&
                    is(K(type_of(lst(md->params)[0])).kind, "array")) {
                    out.append(S(""));
                    out.append(S("    public static void main(String[] args) {"));
                    out.append(cat("        ", cls_names.at(std::get<0>(kv)), ".main(args);"));
                    out.append(S("    }"));
                    return out;
                }
            }
        }
        throw Refused("no main function");
    }
    MethodP m = functions.at(S("main"));
    out.append(S(""));
    out.append(S("    public static void main(String[] args) {"));
    str call = S("main()");
    if (py::len(lst(m->params)) >= 2) call = cat("main(args.length + 1, argv(System.getProperty(\"c.argv0\", \"", name, "\"), args))");
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
void Objects::declare_global(const FieldP& g) {
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
    if (globals.contains(s->name) && !is(globals.at(s->name).kind, "enum")) return;
    TypeP t = g->type;
    // ('g', s.name) in self.addr_taken and not is_record(t) and t.kind != 'array'
    bool is_boxed = addr_taken.contains(cat("g\x01", s->name)) && !is_record(t) && !is(K(t).kind, "array");
    Var v(nm, is_boxed, t, S("global"));
    globals.set(s->name, v);
    ObjWriter w(this, nullptr);
    if (is_boxed) {
        J init = g->init ? w.init_value(t, g->init) : J(default_(t), P_ATOM, jt(t));
        fields_out.append(cat("static ", jt(t), "[] ", nm, " = {", init.code, "};"));
        return;
    }
    J init = g->init ? w.init_value(t, g->init) : J(default_(t), P_ATOM, jt(t));
    const char* fin = in(t->kind, {"array", "class", "record"}) ? "final " : "";
    fields_out.append(cat("static ", fin, jt(t), " ", nm, " = ", init.code, ";"));
}

void Objects::init_global(const FieldP&) {}

// ------------------------------------------------------------------------------------- classes
Lines Objects::class_decl(const ClassP& c) {
    str n = cls_names.at(or_none(c->qname));
    if (opt_is(c->kind, "enum")) {                         // a C++ enum class: its constants
        py::list<str> consts;
        for (const NodeP& m : lst(c->members)) {
            auto f = as_a<ir::FieldDecl>(m);
            auto lit = as_a<ir::Lit>(f->init);
            if (!lit) none_attr("value");
            consts.append(cat("static final int ", jname(need_sym(f->sym).name), " = ", val_str(lit->value), ";"));
        }
        Lines out;
        out.append(S(""));
        out.append(S("    /** enum class ") + or_none(c->name) + " */");
        out.append(S("    static final class ") + n + " {");
        for (const str& x : consts) out.append(S("        ") + x);
        out.append(S("    }"));
        return out;
    }
    py::list<FieldP> fields, inst;
    py::list<MethodP> methods;
    for (const NodeP& m : lst(c->members)) {
        if (auto f = as_a<ir::FieldDecl>(m)) fields.append(f);
        if (auto d = as_a<ir::MethodDecl>(m)) methods.append(d);
    }
    for (const FieldP& f : fields) {
        if (!has(f->mods, "static")) inst.append(f);
    }
    str mods = S("static ");
    bool abstract = has(c->mods, "abstract");
    for (const MethodP& m : methods) {
        if (has(m->mods, "pure")) abstract = true;
    }
    if (abstract) mods = mods + "abstract ";
    str ext;
    ClassP base;
    if (c->extends) {
        base = rec_get(*this, rec_name(c->extends));
        ext = S(" extends ") + cls(c->extends);
    }
    str what = in(or_none(c->kind), {"struct", "class", "union"}) ? or_none(c->kind) : or_none(c->kind);
    str title = or_none(c->qname).replace(S("."), S("::")).replace(S("__unnamed_"), S("(anonymous) "));
    Lines out;
    out.append(S(""));
    out.append(cat("    /** ", what, " ", title, " */"));
    out.append(cat("    ", mods, "class ", n, ext, " {"));
    Lines body;
    for (const FieldP& f : fields) {
        TypeP ft = f->type;
        str st = has(f->mods, "static") ? S("static ") : S("");
        str nm = jname(need_sym(f->sym).name);
        str init;
        if (f->init) {
            ObjWriter w(this, nullptr, c);
            init = S(" = ") + w.init_value(ft, f->init).code;
        } else if (in(K(ft).kind, {"array", "class", "record"})) {
            init = S(" = ") + default_(ft);
        }
        if (!st.empty() && has(f->mods, "const") && f->init) st = S("static final ");
        body.append(cat(st, jt(ft), " ", nm, init, ";"));
    }
    py::list<MethodP> ctors;
    for (const MethodP& m : methods) {
        if (opt_is(m->kind, "ctor")) ctors.append(m);
    }
    bool user_copy = false;
    for (const MethodP& m : ctors) {
        const py::list<NodeP>& ps = lst(m->params);
        if (ps.size() == 1) {
            TypeP pt = type_of(ps[0]);
            if (is_ptr(pt) && is_record(pt->elem) && rec_name(pt->elem) == or_none(c->qname)) user_copy = true;
        }
    }
    str qn = or_none(c->qname);
    // a no-argument constructor where Java would not make one: beside an aggregate or a copy constructor
    bool need_noarg = ctors.empty() && ((ctors_used.contains(qn) && !inst.empty()) || copied.contains(qn) || assigned.contains(qn));
    if (need_noarg) {
        body.append(S(""));
        body.append(n + "() {");
        body.append(S("}"));
    }
    if (ctors_used.contains(qn) && !inst.empty()) {
        py::list<str> ps;
        for (const FieldP& f : inst) ps.append(jt(f->type) + " " + jname(f->sym->name));
        body.append(S(""));
        body.append(cat(n, "(", join_sep(", ", ps), ") {"));
        for (const FieldP& f : inst) body.append(cat("    this.", jname(f->sym->name), " = ", jname(f->sym->name), ";"));
        body.append(S("}"));
    }
    if ((copied.contains(qn) || assigned.contains(qn)) && !user_copy) {
        body.append(S(""));
        body.append(cat("/** C's copy of a ", what, " value. */"));
        body.append(cat(n, "(", n, " o) {"));
        if (base) body.append(S("    super(o);"));
        for (const str& x : copy_fields(inst, S("this"), S("o"), true)) body.append(S("    ") + x);
        body.append(S("}"));
    }
    bool has_assign_op = false;
    for (const MethodP& m : methods) {
        if (opt_is(m->name, "operator=")) has_assign_op = true;
    }
    if (assigned.contains(qn) && !has_assign_op) {
        body.append(S(""));
        body.append(cat("/** C's assignment of a ", what, " value: copies o into this one. */"));
        body.append(cat(n, " set(", n, " o) {"));
        if (base) body.append(S("    super.set(o);"));
        for (const str& x : copy_fields(inst, S("this"), S("o"), false)) body.append(S("    ") + x);
        body.append(S("    return this;"));
        body.append(S("}"));
    }
    for (const MethodP& m : methods) {
        if (!body_set(m->body) && qn.contains(S("<")) && !has(m->mods, "pure")) continue;   // (a template's member this instance never uses: clang made no body)
        if (opt_is(m->kind, "dtor")) {
            if (!body_set(m->body) || (body_stmts(m->body).empty() && !has(m->mods, "virtual"))) continue;
        }
        body.append(S(""));
        body.extend(function_or_gap(m, [&]() { return ObjWriter(this, m, c).write_member(); }, S(""), c->name));
    }
    for (const str& x : body) out.append(!x.empty() ? S("        ") + x : x);
    out.append(S("    }"));
    return out;
}

Lines Objects::copy_fields(const py::list<FieldP>& fields, const str& dst, const str& src, bool fresh) {
    Lines out;
    for (const FieldP& f : fields) {
        str nm = jname(need_sym(f->sym).name);
        TypeP t = f->type;
        if (is_record(t)) {
            out.append(fresh ? cat(dst, ".", nm, " = new ", cls(t), "(", src, ".", nm, ");") : cat(dst, ".", nm, ".set(", src, ".", nm, ");"));
            copied.add(rec_name(t));
            if (!fresh) assigned.add(rec_name(t));
        } else if (is(K(t).kind, "array")) {
            TypeP base = t;
            int64_t depth = 0;
            while (is(base->kind, "array")) {
                base = base->elem;
                depth += 1;
            }
            if (is_record(base)) {
                copied.add(rec_name(base));
                if (fresh) {
                    out.append(cat(dst, ".", nm, " = new ", cls(base), "[", src, ".", nm, ".length];"));
                    out.append(cat("for (int i = 0; i < ", src, ".", nm, ".length; i++) ", dst, ".", nm, "[i] = new ", cls(base), "(", src, ".", nm, "[i]);"));
                } else {
                    assigned.add(rec_name(base));
                    out.append(cat("for (int i = 0; i < ", src, ".", nm, ".length; i++) ", dst, ".", nm, "[i].set(", src, ".", nm, "[i]);"));
                }
            } else if (depth == 1) {
                out.append(fresh ? cat(dst, ".", nm, " = ", src, ".", nm, ".clone();")
                                 : cat("System.arraycopy(", src, ".", nm, ", 0, ", dst, ".", nm, ", 0, ", dst, ".", nm, ".length);"));
            } else {
                out.append(fresh ? cat(dst, ".", nm, " = deepCopy(", src, ".", nm, ");") : cat("copyInto(", dst, ".", nm, ", ", src, ".", nm, ");"));
            }
        } else {
            out.append(cat(dst, ".", nm, " = ", src, ".", nm, ";"));
        }
    }
    return out;
}

// ============================================================================================ ObjWriter
ObjWriter::ObjWriter(Objects* prog, const MethodP& fn, const ClassP& cls) : jb::FnWriter(prog, fn), po(prog), cls(cls) {
    ret_struct = false;
}

// ------------------------------------------------------------------------------------- functions
Lines ObjWriter::write() {
    MethodP f = fn;
    auto pr = params(f);
    Lines body = pr.second.copy();
    block_stmts(body_stmts(f->body), body, 1);
    str ret_ = f->ret ? po->jt(f->ret) : S("void");
    if (!is(ret_, "void") && can_complete(f->body)) {
        if (opt_is(f->name, "main")) body.append(S("return 0;"));
        else body.append(cat("return ", default_of(f->ret), ";  // (C: the function ends without a value)"));
    }
    Lines out;
    out.append(cat("    static ", ret_, " ", jname(f->name), "(", join_sep(", ", pr.first), ") {"));
    for (const str& x : body) out.append(indent_(x, "        "));
    out.append(S("    }"));
    return out;
}

str ObjWriter::default_of(const TypeP& t) { return !is_record(t) ? po->default_(t) : S("null"); }

std::pair<py::list<str>, Lines> ObjWriter::params(const MethodP& f) {
    py::list<str> params_;
    Lines pre;
    for (const NodeP& pn : lst(f->params)) {
        auto prm = as_a<ir::Param>(pn);
        str nm = jname(need_sym(prm->sym).name);
        declared.add(nm);
        TypeP t = prm->type;
        str key = cat("l\x01", prm->sym->owner ? S("s") + *prm->sym->owner : S("\x02"), "\x01", prm->sym->name);
        if (po->addr_taken.contains(key) && !is_record(t) && !is(K(t).kind, "array")) {
            str arg = nm + "Arg";
            params_.append(po->jt(t) + " " + arg);
            pre.append(cat(po->jt(t), "[] ", nm, " = {", arg, "};"));
            vars.set(prm->sym->name, Var(nm, true, t, S("param")));
        } else {
            params_.append(po->jt(t) + " " + nm);
            vars.set(prm->sym->name, Var(nm, false, t, S("param")));
        }
    }
    return {params_, pre};
}

Lines ObjWriter::write_member() {
    MethodP m = fn;
    ClassP c = cls;
    str n = po->cls_names.at(or_none(c->qname));
    auto pr = params(m);
    py::list<str> mods;
    if (has(m->mods, "static")) mods.append(S("static"));
    if (opt_is(m->kind, "ctor")) {
        Lines body;
        py::list<NodeP> stmts = body_set(m->body) ? body_stmts(m->body).copy() : py::list<NodeP>();   // list(m.body.stmts)
        if (!stmts.empty() && is_a<ir::CtorCall>(stmts[0])) {
            auto cc = as_a<ir::CtorCall>(stmts.pop(0));
            py::list<str> ca = call_args(lst_or_empty(cc->args), cc->sym);
            body.append(cat(opt_is(cc->kind, "super") ? "super" : "this", "(", join_sep(", ", ca), ");"));
        }
        // a member initialised to zero before anything else: a Java field already is
        while (!stmts.empty() && zero_init(stmts[0], c)) stmts.pop(0);
        body.extend(pr.second);
        block_stmts(stmts, body, 1);
        Lines out;
        out.append(cat(n, "(", join_sep(", ", pr.first), ") {"));
        for (const str& x : body) out.append(indent_(x, "    "));
        out.append(S("}"));
        return out;
    }
    if (opt_is(m->kind, "static_init")) {
        Lines body;
        block_stmts(body_stmts(m->body), body, 1);
        Lines out;
        if (body.empty()) return out;
        out.append(S("static {"));
        for (const str& x : body) out.append(indent_(x, "    "));
        out.append(S("}"));
        return out;
    }
    if (opt_is(m->kind, "dtor")) {
        Lines body = pr.second.copy();
        block_stmts(body_stmts(m->body), body, 1);
        ClassP base = c->extends ? (po->records.contains(po->rec_name(c->extends)) ? po->records.at(po->rec_name(c->extends)) : nullptr) : nullptr;
        if (base) {
            for (const NodeP& xn : lst(base->members)) {
                auto x = as_a<ir::MethodDecl>(xn);
                if (x && opt_is(x->kind, "dtor") && body_set(x->body) && (!body_stmts(x->body).empty() || has(x->mods, "virtual"))) {
                    body.append(S("super.destroy();"));
                    break;
                }
            }
        }
        Lines out;
        out.append(cat("/** ~", or_none(c->name), "() */"));
        out.append(S("void destroy() {"));
        for (const str& x : body) out.append(indent_(x, "    "));
        out.append(S("}"));
        return out;
    }
    str ret_ = m->ret ? po->jt(m->ret) : S("void");
    if (m->ret && is(m->ret->kind, "ptr") && val_truthy(val_get(m->ret->info, "ref")) && !is_record(m->ret->elem))
        throw Refused(cat(or_none(m->name), " returns a reference to a ", m->ret->elem ? m->ret->elem->repr() : S("None")));
    if (m->ret && is(m->ret->kind, "ptr") && val_truthy(val_get(m->ret->info, "ref"))) ret_ = po->jt(m->ret->elem);
    str name_ = method_name(or_none(m->name));
    if (!body_set(m->body)) {
        if (has(m->mods, "pure")) {
            Lines out;
            out.append(cat("abstract ", ret_, " ", name_, "(", join_sep(", ", pr.first), ");"));
            return out;
        }
        throw Refused(cat(or_none(c->name), "::", or_none(m->name), " is declared but not defined"));
    }
    Lines body = pr.second.copy();
    block_stmts(body_stmts(m->body), body, 1);
    if (!is(ret_, "void") && can_complete(m->body)) body.append(cat("return ", default_of(m->ret), ";"));
    Lines out;
    str modstr = mods.empty() ? S("") : join_sep(" ", mods) + " ";
    out.append(cat(modstr, ret_, " ", name_, "(", join_sep(", ", pr.first), ") {"));
    for (const str& x : body) out.append(indent_(x, "    "));
    out.append(S("}"));
    return out;
}

bool ObjWriter::zero_init(const NodeP& s, const ClassP& c) {
    // `this->f = 0` (or null, false) for a field with no initialiser of its own.
    auto es = as_a<ir::ExprStmt>(s);
    if (!es) return false;
    auto as = as_a<ir::Assign>(es->expr);
    if (!as || as->op) return false;
    NodeP t = strip(as->target);
    auto sel = as_a<ir::Select>(t);
    if (!sel) return false;
    auto un = as_a<ir::Unary>(strip(sel->obj));
    if (!un || !is_a<ir::This>(strip(un->x))) return false;
    NodeP v = strip(as->value);
    while (auto cv = as_a<ir::Cast>(v)) v = strip(cv->x);
    auto lit = as_a<ir::Lit>(v);
    if (!lit) return false;
    // v.value in (0, None, False) and not a str
    const Val& lv = lit->value;
    bool zero = std::holds_alternative<std::monostate>(lv.v) ||
                (std::holds_alternative<bool>(lv.v) && !std::get<bool>(lv.v)) ||
                (std::holds_alternative<int64_t>(lv.v) && std::get<int64_t>(lv.v) == 0) ||
                (std::holds_alternative<double>(lv.v) && std::get<double>(lv.v) == 0.0);
    if (!zero) return false;
    FieldP found;
    for (const NodeP& mn : lst_or_empty(c->members)) {
        auto fd = as_a<ir::FieldDecl>(mn);
        if (fd && need_sym(fd->sym).name == need_sym(sel->sym).name) {
            found = fd;
            break;
        }
    }
    // f = [...]; bool(f) and f[0].init is None and f[0].type.kind not in (...)
    for (const NodeP& mn : lst_or_empty(c->members)) {
        auto fd = as_a<ir::FieldDecl>(mn);
        if (fd && fd->sym->name == sel->sym->name) {
            return !fd->init && !in(K(fd->type).kind, {"class", "record", "array"});
        }
    }
    return false;
}

// ------------------------------------------------------------------------------------- statements
void ObjWriter::local(const py::ref<ir::Local>& s, Lines& out) {
    str nm = jname(need_sym(s->sym).name);
    if (declared.contains(nm)) nm = fresh(nm + "_");
    declared.add(nm);
    TypeP t = s->type;
    if (has(s->mods, "extern")) return;
    if (has(s->mods, "static")) {
        static_local(s, nm);
        return;
    }
    str key = cat("l\x01", s->sym->owner ? S("s") + *s->sym->owner : S("\x02"), "\x01", s->sym->name);
    if (is_record(t) && has_destroy(t))
        throw Refused(cat(po->cls(t), " ", s->sym->name,
                          " has a destructor that runs at the end of its scope (RAII: try/finally not written yet)"));
    bool boxed = po->addr_taken.contains(key) && !is_record(t) && !is(K(t).kind, "array");
    vars.set(s->sym->name, Var(nm, boxed, t, S("local")));
    str jtt = po->jt(t);
    str init = !s->init ? po->default_(t) : init_value(t, s->init).code;
    if (boxed) out.append(cat(jtt, "[] ", nm, " = {", init, "};"));
    else out.append(cat(jtt, " ", nm, " = ", init, ";"));
}

J ObjWriter::init_value(const TypeP& t, const NodeP& init0) {
    // An initialiser of type t as a Java expression (a fresh object or array where t is one).
    NodeP init = strip(init0);
    if (auto lit = as_a<ir::Lit>(init); lit && std::holds_alternative<str>(lit->value.v) && is(K(t).kind, "array")) {
        const str& sv = std::get<str>(lit->value.v);
        int64_t n = t->info.contains(S("count")) ? py_int(t->info.at(S("count"))) : sv.size() + 1;
        return J(cat("bytes(", jb::java_string(sv), ", ", n, ")"), P_POST, S("byte[]"));
    }
    if (auto na = as_a<ir::NewArray>(init); na && is(K(t).kind, "array") && na->dims && !na->dims->empty() && !(na->init && !na->init->empty())) {
        return x_NewArray(na);                 // new T[n]
    }
    if (auto na = as_a<ir::NewArray>(init); na && is(K(t).kind, "array")) {
        py::list<str> items;
        for (const NodeP& x : lst_or_empty(na->init)) items.append(init_value(t->elem, x).code);
        int64_t n = t->info.contains(S("count")) ? py_int(t->info.at(S("count"))) : items.size();
        str jtt = po->jt(t);
        if (items.empty()) return J(po->default_(t), P_POST, jtt);
        if (items.size() < n) {
            if (is_record(t->elem)) {
                for (int64_t i = items.size(); i < n; ++i) items.append(S("new ") + po->cls(t->elem) + "()");
            } else if (is(t->elem->kind, "array")) {
                for (int64_t i = items.size(); i < n; ++i) items.append(po->default_(t->elem));
            } else {
                return J(cat("java.util.Arrays.copyOf(new ", jtt, "{", join_sep(", ", items), "}, ", n, ")"), P_POST, jtt);
            }
        }
        return J(cat("new ", jtt, "{", join_sep(", ", items), "}"), P_POST, jtt);
    }
    if (auto nw = as_a<ir::New>(init); nw && is_record(t) && nw->sym && starts(nw->sym->name, "<aggregate")) {
        return aggregate(t, lst_or_empty(nw->args));
    }
    if (is_record(t)) {
        J v = value(init);
        return v.fresh ? v : copy_of(v, t);
    }
    return conv(value(init), type_of(init), t);
}

J ObjWriter::aggregate(const TypeP& t, const py::list<NodeP>& args) {
    ClassP c = po->records.contains(po->rec_name(t)) ? po->records.at(po->rec_name(t)) : nullptr;
    if (!c) none_attr("members");
    py::list<FieldP> fields;
    for (const NodeP& m : lst(c->members)) {
        if (auto f = as_a<ir::FieldDecl>(m); f && !has(f->mods, "static")) fields.append(f);
    }
    if (args.empty()) return J(S("new ") + po->cls(t) + "()", P_POST, po->cls(t), true);
    po->ctors_used.add(or_none(c->qname));
    py::list<str> vals;
    for (int64_t i = 0; i < fields.size(); ++i) {
        if (i < args.size()) vals.append(init_value(fields[i]->type, args[i]).code);
        else vals.append(po->default_(fields[i]->type));
    }
    return J(cat("new ", po->cls(t), "(", join_sep(", ", vals), ")"), P_POST, po->cls(t), true);
}

J ObjWriter::copy_of(const J& v, const TypeP& t) {
    po->copied.add(po->rec_name(t));
    return J(cat("new ", po->cls(t), "(", v.code, ")"), P_POST, po->cls(t), true);
}

void ObjWriter::static_local(const py::ref<ir::Local>& s, const str&) {
    if (!fn) none_attr("name");
    auto g = std::make_shared<ir::FieldDecl>();
    g->sym = std::make_shared<ir::Sym>(S("global"), cat(or_none(fn->name), "_", s->sym->name), std::nullopt, true, s->type);
    g->type = s->type;
    g->init = s->init;
    g->mods = py::list<str>{S("static")};
    str key = cat("l\x01", s->sym->owner ? S("s") + *s->sym->owner : S("\x02"), "\x01", s->sym->name);
    if (po->addr_taken.contains(key)) po->addr_taken.add(cat("g\x01", g->sym->name));
    po->declare_global(g);
    vars.set(s->sym->name, po->globals.at(g->sym->name));
}

void ObjWriter::ret(const py::ref<ir::Return>& s, Lines& out) {
    MethodP f = fn;
    if (!f) none_attr("ret");
    if (!s->expr) {
        if ((f->ret && !is(f->ret->kind, "void") && opt_is(f->kind, "method")) ||
            (opt_is(f->kind, "function") && f->ret && !is(f->ret->kind, "void"))) {
            out.append(cat("return ", default_of(f->ret), ";  // (C: a return without a value)"));
        } else {
            out.append(S("return;"));
        }
        return;
    }
    if (opt_in(f->kind, {"ctor", "dtor"}) || !f->ret || is(f->ret->kind, "void")) {
        expr_stmt(s->expr, out);
        out.append(S("return;"));
        return;
    }
    TypeP rt = f->ret;
    if (is(rt->kind, "ptr") && val_truthy(val_get(rt->info, "ref"))) {
        NodeP se = strip(s->expr);
        auto u = as_a<ir::Unary>(se);
        out.append(cat("return ", value(u && opt_is(u->op, "&") ? u->x : s->expr).code, ";"));
        return;
    }
    if (is_record(rt)) {
        NodeP e = strip(s->expr);
        J v = value(e);
        auto nm = as_a<ir::Name>(e);
        bool local_ = nm && opt_in(std::optional<str>(nm->sym->kind), {"local", "param"}) &&
                      !po->addr_taken.contains(cat("l\x01", nm->sym->owner ? S("s") + *nm->sym->owner : S("\x02"), "\x01", nm->sym->name));
        out.append(cat("return ", (v.fresh || local_) ? v.code : copy_of(v, rt).code, ";"));
        return;
    }
    out.append(cat("return ", conv(value(s->expr), type_of(s->expr), rt).code, ";"));
}

void ObjWriter::expr_stmt(const NodeP& e, Lines& out) {
    NodeP e0 = strip(e);
    if (auto c = as_a<ir::Call>(e0); c && !c->target && c->sym && is(c->sym->name, "free") && !po->functions.contains(c->sym->name)) {
        out.append(cat("// free(", value(lst(c->args)[0]).code, "): the collector frees it"));
        return;
    }
    if (auto u = as_a<ir::Unary>(e0); u && opt_in(u->op, {"delete", "delete[]"})) {
        J x = value(u->x);
        TypeP t = type_of(u->x);
        if (t && is(t->kind, "ptr") && is_record(t->elem) && has_destroy(t->elem) && is(*u->op, "delete")) {
            out.append(cat(x.at(P_POST), ".destroy();"));
        } else {
            out.append(cat("// ", *u->op, " ", x.code, ": the collector frees it"));
        }
        return;
    }
    jb::FnWriter::expr_stmt(e, out);
}

bool ObjWriter::has_destroy(const TypeP& t) {
    ClassP c = po->records.contains(po->rec_name(t)) ? po->records.at(po->rec_name(t)) : nullptr;
    while (c) {
        for (const NodeP& mn : lst(c->members)) {
            auto m = as_a<ir::MethodDecl>(mn);
            if (m && opt_is(m->kind, "dtor") && body_set(m->body) && (!body_stmts(m->body).empty() || has(m->mods, "virtual"))) return true;
        }
        c = c->extends ? (po->records.contains(po->rec_name(c->extends)) ? po->records.at(po->rec_name(c->extends)) : nullptr) : nullptr;
    }
    return false;
}

// ------------------------------------------------------------------------------------- places
Var ObjWriter::var(const SymP& s) {
    if (in(s->kind, {"local", "param"}) && vars.contains(s->name)) return vars.at(s->name);
    if (po->globals.contains(s->name)) return po->globals.at(s->name);
    throw Refused(S("unknown variable ") + s->name);
}

Place ObjWriter::place(const NodeP& e0) {
    // An lvalue -> Place whose `java` is a Java lvalue (mem is never True here).
    NodeP e = strip(e0);
    if (auto n = as_a<ir::Name>(e)) {
        need_sym(n->sym);
        Var v = var(n->sym);
        return Place(v.mem ? v.java + "[0]" : v.java, false, v.type, P_POST);
    }
    if (auto u = as_a<ir::Unary>(e); u && opt_is(u->op, "*")) {
        J x = value(u->x);
        if (is_record(u->type)) return Place(x.code, false, u->type, x.prec);
        return Place(x.at(P_POST) + "[0]", false, u->type, P_POST);
    }
    if (auto ix = as_a<ir::Index>(e)) {
        J a = value(ix->arr);
        if (std::holds_alternative<py::list<NodeP>>(ix->index)) throw Refused("expression list");
        NodeP idxn = std::get<NodeP>(ix->index);
        J i = conv(value(idxn), type_of(idxn), ir::I32);
        return Place(cat(a.at(P_POST), "[", i.code, "]"), false, ix->type, P_POST);
    }
    if (auto sl = as_a<ir::Select>(e)) return Place(field(sl), false, sl->type, P_POST);
    if (auto c = as_a<ir::Cast>(e)) return place(c->x);
    throw Refused(S("an lvalue ") + (e ? e->class_name() : S("NoneType")));
}

str ObjWriter::field(const py::ref<ir::Select>& e) {
    str nm = jname(need_sym(e->sym).name);
    NodeP obj = strip(e->obj);
    if (auto tn = as_a<ir::TypeName>(obj)) {
        str owner = po->cls(tn->type);
        if (cls && po->cls_names.contains(or_none(cls->qname)) && po->cls_names.at(or_none(cls->qname)) == owner) return nm;
        return cat(owner, ".", nm);
    }
    if (auto u = as_a<ir::Unary>(obj); u && opt_is(u->op, "*") && is_a<ir::This>(strip(u->x))) {
        return !declared.contains(nm) ? nm : S("this.") + nm;
    }
    J o = value(obj);
    return cat(o.at(P_POST), ".", nm);
}

J ObjWriter::load(const Place& pl) { return J(pl.java, pl.prec, po->jt(pl.type)); }

J ObjWriter::store(const Place& pl, const J& val) {
    const TypeP& t = pl.type;
    if (is_record(t)) {
        po->assigned.add(po->rec_name(t));
        return J(cat(at_(pl), ".set(", val.code, ")"), P_POST, po->jt(t));
    }
    return J(cat(pl.java, " = ", val.at(P_ASSIGN)), P_ASSIGN, po->jt(t));
}

J ObjWriter::addr(const NodeP& e0) {
    NodeP e = strip(e0);
    if (auto n = as_a<ir::Name>(e)) {
        Var v = var(n->sym);
        return J(v.java, P_ATOM, po->jt(n->type));
    }
    if (auto u = as_a<ir::Unary>(e); u && opt_is(u->op, "*")) return value(u->x);
    TypeP t = type_of(e);
    if (is_record(t) || (t && is(t->kind, "array"))) return value(e);
    throw Refused(S("the address of ") + (e ? e->class_name() : S("NoneType")));
}

// ------------------------------------------------------------------------------------- values
J ObjWriter::truth(const J& j, const TypeP& t) {
    if (is(j.jt, "boolean")) return j;
    if (t && in(t->kind, {"ptr", "fnptr", "null"})) return J(cat(j.at(P_EQ + 1), " != null"), P_EQ, S("boolean"));
    return jb::FnWriter::truth(j, t);
}

J ObjWriter::x_Concat(const py::ref<ir::Concat>& e) {
    // Java string concatenation (a program read back from Java): Java's own semantics.
    std::vector<J> parts;
    for (const NodeP& x : lst(e->parts)) parts.push_back(value(x));
    if (parts.empty()) return J(S("\"\""), P_ATOM, S("String"));
    if (!is(parts[0].jt, "String") && !(parts.size() > 1 && is(parts[1].jt, "String"))) parts.insert(parts.begin(), J(S("\"\""), P_ATOM, S("String")));
    str code = parts[0].at(jb::P_ADD);
    for (size_t i = 1; i < parts.size(); ++i) code = code + " + " + parts[i].at(jb::P_ADD + 1);
    return J(code, jb::P_ADD, S("String"));
}

J ObjWriter::x_Length(const py::ref<ir::Length>& e) { return J(value(e->arr).at(P_POST) + ".length", P_POST, S("int")); }

J ObjWriter::x_Lit(const py::ref<ir::Lit>& e) {
    if (std::holds_alternative<std::monostate>(e->value.v) && (!e->type || in(e->type->kind, {"ptr", "fnptr", "null"})))
        return J(S("null"), P_ATOM, S("Object"));
    if (std::holds_alternative<str>(e->value.v) && e->type && is(e->type->kind, "class") && opt_is(e->type->name, "java.lang.String"))
        return J(jb::java_wstring(std::get<str>(e->value.v)), P_ATOM, S("String"));
    if (std::holds_alternative<str>(e->value.v)) {
        if (jb::is_wide(e)) throw Refused("a wide string outside a library call");
        return J(cat("bytes(", jb::java_string(std::get<str>(e->value.v)), ")"), P_POST, S("byte[]"));
    }
    return jb::FnWriter::x_Lit(e);
}

J ObjWriter::x_Name(const py::ref<ir::Name>& e) {
    Var v = var(e->sym);
    if (v.mem) return J(v.java + "[0]", P_POST, po->jt(v.type));
    return J(v.java, P_ATOM, po->jt(v.type));
}

J ObjWriter::x_Select(const py::ref<ir::Select>& e) { return J(field(e), P_POST, po->jt(e->type)); }

J ObjWriter::x_This(const py::ref<ir::This>&) {
    if (!cls) none_attr("qname");
    TypeP t = std::make_shared<ir::Type>(S("class"), cls->qname);
    return J(S("this"), P_ATOM, po->cls(t));
}

J ObjWriter::x_AddressOf(const py::ref<ir::AddressOf>& e) {
    str nm = need_sym(e->sym).name;
    if (!po->functions.contains(nm)) throw Refused(S("the address of library function ") + nm);
    return J(cat(po->name, "::", jname(nm)), P_POST, po->jt(e->type));
}

J ObjWriter::x_Unary(const py::ref<ir::Unary>& e) {
    if (opt_is(e->op, "*")) {
        NodeP x = strip(e->x);
        if (is_a<ir::This>(x)) return J(S("this"), P_ATOM, po->jt(e->type));
        if (is_record(e->type)) return value(e->x);
        return load(place(e));
    }
    if (opt_is(e->op, "&")) {
        NodeP x = strip(e->x);
        if (auto ao = as_a<ir::AddressOf>(x)) return value(ao);
        return addr(x);
    }
    if (opt_in(e->op, {"++", "--"}) && is_ptr(e->type)) throw Refused("pointer arithmetic");
    return jb::FnWriter::x_Unary(e);
}

J ObjWriter::incdec(const py::ref<ir::Unary>& e, bool) {
    Place pl = place(e->x);
    if (is(jb::_kind(e->type), "bool")) throw Refused("++ on a bool");
    bool postfix = e->postfix && *e->postfix;
    str code = postfix ? pl.java + or_none(e->op) : or_none(e->op) + pl.java;
    return J(code, postfix ? P_POST : jb::P_UNARY, po->jt(e->type));
}

J ObjWriter::compare(const py::ref<ir::Binary>& e, const str& op) {
    TypeP lt = type_of(e->left), rt = type_of(e->right);
    if ((lt && in(lt->kind, {"ptr", "fnptr", "null"})) || (rt && in(rt->kind, {"ptr", "fnptr", "null"}))) {
        J l = value(e->left), r = value(e->right);
        return J(cat(l.at(P_EQ), " ", op, " ", r.at(P_EQ + 1)), P_EQ, S("boolean"));
    }
    return jb::FnWriter::compare(e, op);
}

J ObjWriter::x_Assign(const py::ref<ir::Assign>& e) {
    if (!e->op) {
        TypeP t = type_of(e->target);
        Place pl = place(e->target);
        if (is_record(t)) return store(pl, value(e->value));
        J v = value(e->value);
        return store(pl, conv(v, type_of(e->value), t));
    }
    return jb::FnWriter::x_Assign(e);
}

J ObjWriter::x_Cast(const py::ref<ir::Cast>& e) {
    NodeP x = strip(e->x);
    TypeP t = e->type;
    TypeP st = type_of(x);
    if (is(K(t).kind, "void")) return value(x);
    if (in(t->kind, {"ptr", "fnptr"}) && jb::_null(x)) return J(S("null"), P_ATOM, S("Object"));
    if (auto c = as_a<ir::Call>(x); c && !c->target && c->sym && in(c->sym->name, {"malloc", "calloc"}) && !po->functions.contains(c->sym->name))
        return allocation(c, t);
    if (st && is(st->kind, "array") && is_ptr(t)) return value(x);       // an array is its own pointer
    if (auto l = as_a<ir::Lit>(x); l && std::holds_alternative<std::monostate>(l->value.v)) return J(S("null"), P_ATOM, S("Object"));
    if (auto ao = as_a<ir::AddressOf>(x)) return value(ao);
    J j = value(x);
    return conv(j, st, t);
}

J ObjWriter::conv(const J& j, const TypeP& s, const TypeP& t) {
    if (!s || !t) return j;
    auto is_ref = [](const TypeP& x) { return in(x->kind, {"ptr", "fnptr", "null", "class", "record", "array"}); };
    if (is_ref(s) || is_ref(t)) {
        if (is(t->kind, "bool")) return J(cat(j.at(P_EQ + 1), " != null"), P_EQ, S("boolean"));
        if (is_ref(s) != is_ref(t) && !is(t->kind, "void")) throw Refused("a pointer used as an integer, or the reverse");
        return j;
    }
    return jb::FnWriter::conv(j, s, t);
}

J ObjWriter::allocation(const py::ref<ir::Call>& call, const TypeP& t) {
    // malloc/calloc(...) converted to T*: a new object or array of T.
    if (!is(t->kind, "ptr") || !t->elem || is(t->elem->kind, "void")) throw Refused("an allocation of no type");
    TypeP el = t->elem;
    std::optional<int64_t> size = po->size_hint(el);
    (void)size;
    py::list<NodeP> args = lst_or_empty(call->args);
    std::optional<str> n;
    if (is(call->sym->name, "calloc")) n = is_size(args[1], el) ? count(args[0], nullptr) : std::nullopt;
    else n = count(args[0], el);
    if (!n) throw Refused(S("an allocation whose size is not a count of ") + el->repr());
    if (is_record(el)) {
        if (is(*n, "1")) return J(S("new ") + po->cls(el) + "()", P_POST, po->cls(el), true);
        po->arrays_filled = true;
        return J(cat("filled(new ", po->cls(el), "[", *n, "], ", po->cls(el), "::new)"), P_POST, po->cls(el) + "[]");
    }
    if (!is(el->kind, "array")) return J(cat("new ", po->jt(el), "[", *n, "]"), P_POST, po->jt(t));
    str out = cat("new ", po->jt(_base(el)), "[", *n, "]");
    for (int64_t d : _dims(el)) out = out + "[" + py::to_str(d) + "]";
    return J(out, P_POST, po->jt(t));
}

bool ObjWriter::is_size(const NodeP& e0, const TypeP& el) {
    NodeP e = strip(e0);
    if (auto c = as_a<ir::Cast>(e)) e = strip(c->x);
    auto l = as_a<ir::Lit>(e);
    if (!l) return false;
    const Val& v = l->value;
    int64_t iv;
    if (std::holds_alternative<int64_t>(v.v)) iv = std::get<int64_t>(v.v);
    else if (std::holds_alternative<bool>(v.v)) iv = std::get<bool>(v.v) ? 1 : 0;
    else return false;
    auto sh = po->size_hint(el);
    return sh && iv == *sh;
}

std::optional<str> ObjWriter::count(const NodeP& e0, const TypeP& el) {
    NodeP e = strip(e0);
    while (auto c = as_a<ir::Cast>(e)) e = strip(c->x);
    if (!el) return conv(value(e), type_of(e), ir::I32).code;
    std::optional<int64_t> size = po->size_hint(el);
    if (is_size(e, el)) return S("1");
    if (auto b = as_a<ir::Binary>(e); b && opt_is(b->op, "*")) {
        for (int pass = 0; pass < 2; ++pass) {
            NodeP a = pass == 0 ? b->left : b->right;
            NodeP bb = pass == 0 ? b->right : b->left;
            if (is_size(bb, el)) {
                NodeP a0 = strip(a);
                while (auto c = as_a<ir::Cast>(a0)) a0 = strip(c->x);
                return conv(value(a0), type_of(a0), ir::I32).code;
            }
        }
    }
    if (size && *size == 1) return conv(value(e), type_of(e), ir::I32).code;
    return std::nullopt;
}

J ObjWriter::x_Call(const py::ref<ir::Call>& e) {
    py::list<NodeP> args = lst_or_empty(e->args);
    if (e->sym && in(e->sym->kind, {"method", "ctor"}) && e->target) return method_call(e);
    if (e->sym && !e->target && po->functions.contains(e->sym->name)) {
        MethodP f = po->function_for(e->sym);
        std::vector<TypeP> ptypes;
        for (const NodeP& pn : lst(f->params)) ptypes.push_back(as_a<ir::Param>(pn)->type);
        py::list<str> ac = call_args(args, f->sym, ptypes);
        TypeP rt = f->ret;
        return J(cat(jname(e->sym->name), "(", join_sep(", ", ac), ")"), P_POST, rt ? po->jt(rt) : S("void"), rt && is_record(rt));
    }
    if (e->sym && !e->target && in(e->sym->name, {"malloc", "calloc"})) throw Refused("an allocation used without a type");
    NodeP target = e->target ? strip(e->target) : NodeP();
    if (target && type_of(target) && in(type_of(target)->kind, {"fnptr", "fn"})) {
        J fn_ = value(target);
        TypeP tt = type_of(target);
        TypeP ft = tt;
        if (!is(tt->kind, "fnptr")) {
            ir::Dict info;
            info.set(S("space"), Val(S("flat")));
            info.set(S("fn"), Val(tt));
            ft = std::make_shared<ir::Type>(S("fnptr"), std::nullopt, py::list<TypeP>(), nullptr, std::nullopt, info);
        }
        std::vector<TypeP> params = jb::fn_params(val_type(ft->info.at(S("fn"))));
        py::list<str> ac;
        for (int64_t i = 0; i < args.size(); ++i) {
            ac.append(conv(value(args[i]), type_of(args[i]), i < static_cast<int64_t>(params.size()) ? params[i] : type_of(args[i])).code);
        }
        return J(cat(fn_.at(P_POST), ".call(", join_sep(", ", ac), ")"), P_POST, po->fn_sig_of_type(ft).ret);
    }
    if (e->sym && !e->target) return lib_call(e, e->sym->name, args);
    throw Refused("a call this back end cannot place");
}

py::list<str> ObjWriter::call_args(const py::list<NodeP>& args, const SymP& sym, const std::optional<std::vector<TypeP>>& params0) {
    std::vector<TypeP> params;
    if (params0) params = *params0;
    else if (sym) params = vec_of(sym->params);
    py::list<str> out;
    for (int64_t i = 0; i < args.size(); ++i) {
        const NodeP& a = args[i];
        TypeP pt = i < static_cast<int64_t>(params.size()) ? params[i] : type_of(a);
        if (is_record(pt)) {
            J v = value(a);
            out.append(v.fresh ? v.code : copy_of(v, pt).code);
        } else if (pt && is(pt->kind, "ptr") && val_truthy(val_get(pt->info, "ref"))) {
            NodeP aa = strip(a);
            if (auto u = as_a<ir::Unary>(aa); u && opt_is(u->op, "&")) out.append(addr(u->x).code);
            else out.append(value(a).code);
        } else {
            out.append(conv(value(a), type_of(a), pt).code);
        }
    }
    return out;
}

J ObjWriter::method_call(const py::ref<ir::Call>& e) {
    SymP s = e->sym;
    NodeP target = strip(e->target);
    str nm = method_name(s->name);
    MethodP m = find_method(s);
    std::vector<TypeP> params;
    if (m) {
        for (const NodeP& pn : lst(m->params)) params.push_back(as_a<ir::Param>(pn)->type);
    } else {
        params = vec_of(s->params);
    }
    py::list<str> ac = call_args(lst_or_empty(e->args), s, params);
    TypeP rt = e->type;
    str jtt = (rt && !is(rt->kind, "void")) ? po->jt(rt) : S("void");
    bool fresh = rt && is_record(rt) && !(m && m->ret && is(m->ret->kind, "ptr"));
    if (auto tn = as_a<ir::TypeName>(target)) {
        str owner = po->cls(tn->type);
        bool here = cls && po->cls_names.contains(or_none(cls->qname)) && po->cls_names.at(or_none(cls->qname)) == owner;
        return J(cat(here ? nm : cat(owner, ".", nm), "(", join_sep(", ", ac), ")"), P_POST, jtt, fresh);
    }
    if (auto u = as_a<ir::Unary>(target); u && opt_is(u->op, "*") && is_a<ir::This>(strip(u->x)))
        return J(cat(nm, "(", join_sep(", ", ac), ")"), P_POST, jtt, fresh);
    J obj = value(target);
    return J(cat(obj.at(P_POST), ".", nm, "(", join_sep(", ", ac), ")"), P_POST, jtt, fresh);
}

MethodP ObjWriter::find_method(const SymP& s) {
    ClassP c = (s->owner && po->records.contains(*s->owner)) ? po->records.at(*s->owner) : nullptr;
    while (c) {
        for (const NodeP& mn : lst(c->members)) {
            auto m = as_a<ir::MethodDecl>(mn);
            if (m && m->name && *m->name == s->name && py::len(lst(m->params)) == static_cast<int64_t>(s->params.size())) return m;
        }
        c = c->extends ? (po->records.contains(po->rec_name(c->extends)) ? po->records.at(po->rec_name(c->extends)) : nullptr) : nullptr;
    }
    return nullptr;
}

J ObjWriter::x_New(const py::ref<ir::New>& e) {
    TypeP t = e->type;
    if (e->memory && is(*e->memory, "manual") && t && is(t->kind, "ptr")) {
        TypeP el = t->elem;
        if (is_record(el)) return construct(el, e);
        py::list<NodeP> args = lst_or_empty(e->args);
        if (!args.empty())
            return J(cat("new ", po->jt(el), "[] {", conv(value(args[0]), type_of(args[0]), el).code, "}"), P_POST, po->jt(t));
        return J(cat("new ", po->jt(el), "[1]"), P_POST, po->jt(t));
    }
    if (is_record(t)) return construct(t, e);
    throw Refused(cat("new (", or_none(e->memory), ")"));
}

J ObjWriter::construct(const TypeP& t, const py::ref<ir::New>& e) {
    if (e->sym && starts(e->sym->name, "<aggregate")) return aggregate(t, lst_or_empty(e->args));
    ClassP c = po->records.contains(po->rec_name(t)) ? po->records.at(po->rec_name(t)) : nullptr;
    MethodP ctor;
    if (c) {
        for (const NodeP& mn : lst(c->members)) {
            auto m = as_a<ir::MethodDecl>(mn);
            if (m && opt_is(m->kind, "ctor") && py::len(lst(m->params)) == py::len(lst_or_empty(e->args))) {
                if (!e->sym) none_attr("params");
                std::vector<str> a, b;
                for (const TypeP& p : e->sym->params) a.push_back(spell_t(p));
                for (const NodeP& pn : lst(m->params)) b.push_back(spell_t(as_a<ir::Param>(pn)->type));
                if (a == b) ctor = m;
            }
        }
    }
    if (!e->sym) none_attr("params");
    std::vector<TypeP> params;
    if (ctor) {
        for (const NodeP& pn : lst(ctor->params)) params.push_back(as_a<ir::Param>(pn)->type);
    } else {
        params = vec_of(e->sym->params);
    }
    py::list<str> ac = call_args(lst_or_empty(e->args), e->sym, params);
    return J(cat("new ", po->cls(t), "(", join_sep(", ", ac), ")"), P_POST, po->cls(t), true);
}

J ObjWriter::x_NewArray(const py::ref<ir::NewArray>& e) {
    TypeP t = e->type;
    if (t && is(t->kind, "ptr")) {
        TypeP el = t->elem;
        str n = (e->dims && !e->dims->empty()) ? conv(value((*e->dims)[0]), type_of((*e->dims)[0]), ir::I32).code : S("0");
        if (is_record(el)) {
            po->arrays_filled = true;
            return J(cat("filled(new ", po->cls(el), "[", n, "], ", po->cls(el), "::new)"), P_POST, po->jt(t));
        }
        if (e->init && !e->init->empty()) {
            py::list<str> items;
            for (const NodeP& x : *e->init) items.append(conv(value(x), type_of(x), el).code);
            return J(cat("java.util.Arrays.copyOf(new ", po->jt(el), "[] {", join_sep(", ", items), "}, ", n, ")"), P_POST, po->jt(t));
        }
        return J(cat("new ", po->jt(el), "[", n, "]"), P_POST, po->jt(t));
    }
    if (t && is(t->kind, "array")) {
        if (e->dims && !e->dims->empty() && !t->info.contains(S("count"))) {          // new T[n] (a Java array read back)
            str n = conv(value((*e->dims)[0]), type_of((*e->dims)[0]), ir::I32).code;
            TypeP base = _base(t);
            str tail;
            for (size_t i = 1; i < _dims(t).size(); ++i) tail = tail + "[]";
            return J(cat("new ", po->jt(base), "[", n, "]", tail), P_POST, po->jt(t));
        }
        return init_value(t, e);
    }
    throw Refused("an array value");
}

J ObjWriter::x_Cond(const py::ref<ir::Cond>& e) {
    if (is_record(e->type)) {
        J c = cond(e->cond);
        J a = value(e->then), b = value(e->orelse);
        return J(cat(c.at(P_COND + 1), " ? ", a.at(P_COND + 1), " : ", b.at(P_COND)), P_COND, po->jt(e->type));
    }
    return jb::FnWriter::x_Cond(e);
}

J ObjWriter::lib_call(const py::ref<ir::Call>& e, const str& name, const py::list<NodeP>& args) {
    if (starts(name, "__builtin_va")) throw Refused("a variadic function");
    return java_libc::call(*this, e, name, args);
}

}  // namespace codebreaker::translate::backends::java_objects

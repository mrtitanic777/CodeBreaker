// codebreaker/translate/backends/jmodel.cpp -- C++17 port of codebreaker.translate.backends.jmodel
// (see jmodel.hpp).
#include "codebreaker/translate/backends/jmodel.hpp"

#include <memory>
#include <vector>

namespace codebreaker::translate::backends::jmodel {

using ir::NodeP;
using py::ref;
using std::dynamic_pointer_cast;

// ---------------------------------------------------------------------------------------------- tables
const py::str OBJECT("java.lang.Object");

// (the types are made here, not taken from ir::I32 & co: another file's constants may not exist yet when
// these are built, and Type equality is by value)
static TypeP mk(const char* kind, const char* name = nullptr) {
    if (name) return std::make_shared<ir::Type>(py::str(kind), py::str(name));
    return std::make_shared<ir::Type>(py::str(kind));
}
static const TypeP _T = mk("typevar", "T");
static const TypeP _STR = mk("class", "java.lang.String");
static const TypeP _OBJ = mk("class", "java.lang.Object");
static const TypeP _I = mk("i32");
static const TypeP _B = mk("bool");
static const TypeP _V = mk("void");
static const TypeP _L = mk("i64");
static const TypeP _D = mk("f64");

typedef py::list<TypeP> Ts;

static py::dict<py::str, py::list<py::str>> make_lib_supers() {
    py::dict<py::str, py::list<py::str>> d;
    auto put = [&](const char* k, std::initializer_list<const char*> vs) {
        py::list<py::str> l;
        for (const char* v : vs) l.append(py::str(v));
        d.set(py::str(k), l);
    };
    put("java.lang.String", {"java.lang.CharSequence", "java.lang.Comparable"});
    put("java.util.ArrayList", {"java.util.List"});
    put("java.util.LinkedList", {"java.util.List", "java.util.Deque"});
    put("java.util.List", {"java.util.Collection"});
    put("java.util.Set", {"java.util.Collection"});
    put("java.util.HashSet", {"java.util.Set"});
    put("java.util.LinkedHashSet", {"java.util.Set"});
    put("java.util.TreeSet", {"java.util.Set"});
    put("java.util.Collection", {"java.lang.Iterable"});
    put("java.util.Deque", {"java.util.Queue"});
    put("java.util.Queue", {"java.util.Collection"});
    put("java.util.ArrayDeque", {"java.util.Deque"});
    put("java.util.HashMap", {"java.util.Map"});
    put("java.util.LinkedHashMap", {"java.util.HashMap"});
    put("java.util.TreeMap", {"java.util.Map"});
    put("java.lang.Exception", {"java.lang.Throwable"});
    put("java.lang.Error", {"java.lang.Throwable"});
    put("java.lang.RuntimeException", {"java.lang.Exception"});
    put("java.lang.IllegalArgumentException", {"java.lang.RuntimeException"});
    put("java.lang.IllegalStateException", {"java.lang.RuntimeException"});
    put("java.lang.ArithmeticException", {"java.lang.RuntimeException"});
    put("java.lang.NullPointerException", {"java.lang.RuntimeException"});
    put("java.lang.ClassCastException", {"java.lang.RuntimeException"});
    put("java.lang.UnsupportedOperationException", {"java.lang.RuntimeException"});
    put("java.lang.IndexOutOfBoundsException", {"java.lang.RuntimeException"});
    put("java.lang.ArrayIndexOutOfBoundsException", {"java.lang.IndexOutOfBoundsException"});
    put("java.lang.StringIndexOutOfBoundsException", {"java.lang.IndexOutOfBoundsException"});
    put("java.lang.NumberFormatException", {"java.lang.IllegalArgumentException"});
    put("java.util.NoSuchElementException", {"java.lang.RuntimeException"});
    put("java.io.IOException", {"java.lang.Exception"});
    put("java.lang.Integer", {"java.lang.Number", "java.lang.Comparable"});
    put("java.lang.Long", {"java.lang.Number", "java.lang.Comparable"});
    put("java.lang.Double", {"java.lang.Number", "java.lang.Comparable"});
    put("java.lang.Enum", {"java.lang.Comparable"});
    put("java.lang.Record", {});
    put("java.util.function.UnaryOperator", {"java.util.function.Function"});
    put("java.util.function.BinaryOperator", {"java.util.function.BiFunction"});
    return d;
}
const py::dict<py::str, py::list<py::str>> LIB_SUPERS = make_lib_supers();

static py::dict<py::str, Sig> make_sams() {
    py::dict<py::str, Sig> d;
    auto put = [&](const char* k, const char* name, std::initializer_list<TypeP> ps, const TypeP& r) {
        d.set(py::str(k), Sig(py::str(name), Ts(ps), r));
    };
    put("java.lang.Runnable", "run", {}, _V);
    put("java.util.function.Function", "apply", {_T}, _T);
    put("java.util.function.BiFunction", "apply", {_T, _T}, _T);
    put("java.util.function.UnaryOperator", "apply", {_T}, _T);
    put("java.util.function.BinaryOperator", "apply", {_T, _T}, _T);
    put("java.util.function.Supplier", "get", {}, _T);
    put("java.util.function.Consumer", "accept", {_T}, _V);
    put("java.util.function.BiConsumer", "accept", {_T, _T}, _V);
    put("java.util.function.Predicate", "test", {_T}, _B);
    put("java.util.function.BiPredicate", "test", {_T, _T}, _B);
    put("java.util.function.IntBinaryOperator", "applyAsInt", {_I, _I}, _I);
    put("java.util.function.IntUnaryOperator", "applyAsInt", {_I}, _I);
    put("java.util.function.IntPredicate", "test", {_I}, _B);
    put("java.util.function.IntFunction", "apply", {_I}, _T);
    put("java.util.function.IntConsumer", "accept", {_I}, _V);
    put("java.util.function.IntSupplier", "getAsInt", {}, _I);
    put("java.util.function.ToIntFunction", "applyAsInt", {_T}, _I);
    put("java.util.function.ToLongFunction", "applyAsLong", {_T}, _L);
    put("java.util.function.ToDoubleFunction", "applyAsDouble", {_T}, _D);
    put("java.util.function.IntToDoubleFunction", "applyAsDouble", {_I}, _D);
    put("java.util.function.IntToLongFunction", "applyAsLong", {_I}, _L);
    put("java.util.function.DoubleUnaryOperator", "applyAsDouble", {_D}, _D);
    put("java.util.function.DoubleBinaryOperator", "applyAsDouble", {_D, _D}, _D);
    put("java.util.function.LongBinaryOperator", "applyAsLong", {_L, _L}, _L);
    put("java.util.function.LongUnaryOperator", "applyAsLong", {_L}, _L);
    put("java.util.Comparator", "compare", {_T, _T}, _I);
    return d;
}
const py::dict<py::str, Sig> SAMS = make_sams();

static py::dict<py::str, py::list<Sig>> make_lib_methods() {
    py::dict<py::str, py::list<Sig>> d;
    auto put = [&](const py::str& k, const char* name, std::initializer_list<TypeP> ps, const TypeP& r) {
        d.setdefault(k, py::list<Sig>()).append(Sig(py::str(name), Ts(ps), r));
    };
    put(OBJECT, "toString", {}, _STR);
    put(OBJECT, "equals", {_OBJ}, _B);
    put(OBJECT, "hashCode", {}, _I);
    put(py::str("java.lang.Comparable"), "compareTo", {_T}, _I);
    put(py::str("java.lang.Iterable"), "iterator", {}, mk("class", "java.util.Iterator"));
    put(py::str("java.lang.Iterable"), "forEach", {_T}, _V);
    put(py::str("java.util.Iterator"), "hasNext", {}, _B);
    put(py::str("java.util.Iterator"), "next", {}, _T);
    put(py::str("java.util.Iterator"), "remove", {}, _V);
    put(py::str("java.lang.Throwable"), "getMessage", {}, _STR);
    put(py::str("java.lang.CharSequence"), "length", {}, _I);
    put(py::str("java.lang.CharSequence"), "charAt", {_I}, mk("u16"));
    for (const auto& kv : SAMS.items()) {
        const Sig& s = std::get<1>(kv);
        d.setdefault(std::get<0>(kv), py::list<Sig>()).append(s);
    }
    return d;
}
const py::dict<py::str, py::list<Sig>> LIB_METHODS = make_lib_methods();

// ---------------------------------------------------------------------------------------------- helpers
static bool is(const std::optional<py::str>& v, const char* s) {
    return v.has_value() && *v == py::str(s);
}
static bool has(const ir::Strs& v, const char* s) {
    return v.has_value() && v->contains(py::str(s));
}
// a node slot that is a list, iterated as the Python does: None is a TypeError
static const py::list<NodeP>& listed(const ir::Nodes& v) {
    if (!v) throw py::TypeError("'NoneType' object is not iterable");
    return *v;
}
static const py::str& name_or_empty(const std::optional<py::str>& n) {
    static const py::str empty;
    return n ? *n : empty;
}
static ref<ir::ClassDecl> as_class(const NodeP& n) {
    ref<ir::ClassDecl> c = dynamic_pointer_cast<ir::ClassDecl>(n);
    if (!c) throw py::AttributeError("object has no attribute 'qname'");
    return c;
}
// `p.type` of a parameter node
static TypeP param_type(const NodeP& p) {
    if (auto x = dynamic_pointer_cast<ir::Param>(p)) return x->type;
    if (auto x = dynamic_pointer_cast<ir::Local>(p)) return x->type;
    throw py::AttributeError("object has no attribute 'type'");
}
// `m.params`, which len() and zip() need to be a list
static const py::list<NodeP>& params_of(const ir::MethodDecl& m) {
    if (!m.params) throw py::TypeError("object of type 'NoneType' has no len()");
    return *m.params;
}
static int64_t id_of(const NodeP& n) {
    return static_cast<int64_t>(reinterpret_cast<intptr_t>(n.get()));
}

// getattr(n, 'sym', None): every node class that has a sym slot
template <class T> static bool try_sym(const NodeP& n, ir::SymP& out) {
    if (const T* p = dynamic_cast<const T*>(n.get())) {
        out = p->sym;
        return true;
    }
    return false;
}
template <class... T> static ir::SymP sym_of_any(const NodeP& n) {
    ir::SymP out;
    bool found = (try_sym<T>(n, out) || ...);
    (void)found;
    return out;
}
static ir::SymP sym_of(const NodeP& n) {
    return sym_of_any<ir::FieldDecl, ir::MethodDecl, ir::Param, ir::EnumConst, ir::Property, ir::Local, ir::Catch,
                      ir::CtorCall, ir::Name, ir::Select, ir::Call, ir::New, ir::Index, ir::MethodRef,
                      ir::AddressOf>(n);
}

// ---------------------------------------------------------------------------------------------- ClassInfo
ClassInfo::ClassInfo(const ref<ir::ClassDecl>& decl_, const py::str& qname_, const py::str& cname_,
                     const ref<ClassInfo>& outer_, bool local_, bool anon_)
    : decl(decl_), qname(qname_), cname(cname_), outer(outer_), local(local_), anon(anon_), kind(decl_->kind) {
    bool static_ = has(decl_->mods, "static") ||
                   (kind.has_value() && py::in(*kind, std::make_tuple(py::str("interface"), py::str("enum"),
                                                                       py::str("record"), py::str("annotation"))));
    inner = outer != nullptr && !static_ && !local && !anon && !is(outer->kind, "interface");
    needs_outer = inner;
}

ir::Nodes ClassInfo::members() const {
    return decl->members;
}

py::list<ref<ir::MethodDecl>> ClassInfo::methods(const std::optional<py::str>& name) const {
    py::list<ref<ir::MethodDecl>> out;
    for (const NodeP& m : listed(decl->members)) {
        auto md = dynamic_pointer_cast<ir::MethodDecl>(m);
        if (md && (is(md->kind, "method") || is(md->kind, "ctor")) &&
            (!name.has_value() || (md->name.has_value() && *md->name == *name))) {
            out.append(md);
        }
    }
    return out;
}

py::list<ref<ir::FieldDecl>> ClassInfo::fields() const {
    py::list<ref<ir::FieldDecl>> out;
    for (const NodeP& m : listed(decl->members)) {
        if (auto f = dynamic_pointer_cast<ir::FieldDecl>(m)) out.append(f);
    }
    return out;
}

py::str ClassInfo::repr() const {
    return py::percent("<class %s>", cname);
}

// A type with its type variables and wildcards as Object (Java's erasure, near enough: a bounded
// variable is Object here too, and its uses are cast to what javac said).
TypeP erase(const TypeP& t) {
    if (!t) return ir::VOID;
    if (t->kind == py::str("typevar") || t->kind == py::str("wildcard") || t->kind == py::str("any")) return _OBJ;
    if (t->kind == py::str("array")) {
        return std::make_shared<ir::Type>(py::str("array"), std::nullopt, py::list<TypeP>(), erase(t->elem));
    }
    if (t->kind == py::str("class") && py::len(t->args) > 0) {
        return std::make_shared<ir::Type>(py::str("class"), t->name);
    }
    return t;
}

// ---------------------------------------------------------------------------------------------- Model
Model::Model(const py::list<ref<ir::Unit>>& units_, bool) : units(units_) {
    for (const auto& u : units) {
        if (u->package && py::truthy(*u->package)) packages.add(*u->package);
    }
    for (const auto& u : units) {
        for (const NodeP& c : listed(u->types)) _add(as_class(c), nullptr, u->package);
    }
    for (const auto& ci : classes.values()) _supers(ci);
    for (const auto& ci : classes.values()) {
        if (ci->local || ci->anon) _capture(ci);
    }
}

// ---- the classes
py::str Model::_cname(const py::str& qname, const std::optional<py::str>& package) const {
    py::str q = qname;
    if (package && py::truthy(*package) && q.startswith(*package + py::str("."))) {
        q = q.slice(py::len(*package) + 1, py::len(q));
    }
    return q.replace(py::str("."), py::str("_")).replace(py::str("$"), py::str("_"));
}

ref<ClassInfo> Model::_add(const ref<ir::ClassDecl>& c, const ref<ClassInfo>& outer, const std::optional<py::str>& package,
                           bool local, bool anon, const ref<ir::MethodDecl>& method) {
    py::str qname = c->qname ? *c->qname : py::str();   // c.qname or ''
    py::str cname, key;
    if (anon || !py::truthy(qname) || local) {
        _anon += 1;
        py::str base = outer ? outer->cname : py::str("Local");
        py::str nm = anon ? py::str("anon") : ((c->name && py::truthy(*c->name)) ? *c->name : py::str("local"));
        cname = py::percent("%s_%s%d", base, nm, _anon);
        key = (py::truthy(qname) && !anon) ? qname : py::percent("<%s>", cname);
    } else {
        cname = _cname(qname, package);
        key = qname;
    }
    ref<ClassInfo> ci = std::make_shared<ClassInfo>(c, key, cname, outer, local, anon);
    ci->package = package;
    ci->method = method;                    // the method a local/anonymous class sits in
    if ((local || anon) && method != nullptr && outer != nullptr) {
        ci->needs_outer = !(method->sym != nullptr && method->sym->static_) && !is(method->kind, "static_init");
    }
    classes.set(key, ci);
    by_decl.set(id_of(c), ci);
    for (const NodeP& m : listed(c->members)) {
        if (auto nested = dynamic_pointer_cast<ir::ClassDecl>(m)) {
            _add(nested, ci, package);
        } else if (auto ec = dynamic_pointer_cast<ir::EnumConst>(m); ec && ec->body != nullptr) {
            ref<ClassInfo> sub = _add(as_class(ec->body), ci, package, false, true);
            sub->enum_const = ec;
            sub->needs_outer = false;
            sub->base = ci->qname;
        } else if (dynamic_pointer_cast<ir::MethodDecl>(m) || dynamic_pointer_cast<ir::FieldDecl>(m)) {
            _nested_in(m, ci, package);
        }
    }
    return ci;
}

void Model::_nested_in(const NodeP& m, const ref<ClassInfo>& ci, const std::optional<py::str>& package) {
    auto md = dynamic_pointer_cast<ir::MethodDecl>(m);
    auto fd = dynamic_pointer_cast<ir::FieldDecl>(m);
    ref<ir::MethodDecl> method = md;
    NodeP root = md ? md->body : fd->init;
    if (root == nullptr) return;
    py::list<NodeP> stack{root};
    while (py::len(stack) > 0) {
        NodeP n = stack.pop();
        if (auto lc = dynamic_pointer_cast<ir::LocalClass>(n)) {
            _add(as_class(lc->decl), ci, package, true, false, method);
            continue;
        }
        if (auto nw = dynamic_pointer_cast<ir::New>(n); nw && nw->body != nullptr) {
            if (nw->args) {
                for (const NodeP& a : *nw->args) stack.append(a);
            }
            if (nw->outer != nullptr) stack.append(nw->outer);
            ref<ClassInfo> sub = _add(as_class(nw->body), ci, package, false, true, method);
            sub->new_ = nw;
            if (method == nullptr) {                         // a field initialiser: an instance one has `this`
                ir::SymP s = sym_of(m);
                sub->needs_outer = !(s != nullptr && s->static_);
            }
            continue;
        }
        stack.extend(n->children());
    }
}

void Model::_supers(const ref<ClassInfo>& ci) {
    const ref<ir::ClassDecl>& d = ci->decl;
    if (ci->anon && ci->new_ != nullptr) {
        TypeP t = d->extends != nullptr ? d->extends : ci->new_->type;
        std::optional<py::str> target = (t != nullptr && t->kind == py::str("class")) ? t->name : std::optional<py::str>(OBJECT);
        ref<ClassInfo> tci = target ? classes.get(*target) : nullptr;
        bool known = target.has_value();
        if ((tci != nullptr && tci->kind == std::optional<py::str>(py::str("interface"))) ||
            (tci == nullptr && known && SAMS.contains(*target)) ||
            (tci == nullptr && known && LIB_METHODS.contains(*target) && *target != OBJECT &&
             *target != py::str("java.lang.Throwable"))) {
            ci->base = OBJECT;
            ci->ifaces = py::list<py::str>{*target};
        } else {
            ci->base = target;
        }
        return;
    }
    if (ci->enum_const != nullptr) return;
    if (is(d->kind, "enum")) {
        ci->base = py::str("java.lang.Enum");
    } else if (is(d->kind, "record")) {
        ci->base = OBJECT;
    } else if (d->extends != nullptr && d->extends->kind == py::str("class")) {
        ci->base = d->extends->name;
    } else {
        ci->base = !is(d->kind, "interface") ? std::optional<py::str>(OBJECT) : std::nullopt;
    }
    ci->ifaces = py::list<py::str>();
    if (d->implements) {
        for (const TypeP& t : *d->implements) {
            if (t->kind == py::str("class")) ci->ifaces.append(name_or_empty(t->name));
        }
    }
    if (is(d->kind, "interface") && d->extends != nullptr) ci->ifaces.insert(0, name_or_empty(d->extends->name));
}

void Model::_capture(const ref<ClassInfo>& ci) {
    py::set<py::str> names_declared;
    py::list<std::tuple<py::str, TypeP>> used;
    py::list<NodeP> nodes = ir::walk(ci->decl);
    for (const NodeP& n : nodes) {
        ir::SymP s = sym_of_any<ir::Local, ir::Param>(n);
        if (s != nullptr) names_declared.add(s->name);
    }
    for (const NodeP& n : nodes) {
        auto nm = dynamic_pointer_cast<ir::Name>(n);
        if (nm && nm->sym != nullptr && !names_declared.contains(nm->sym->name)) {
            bool seen = false;
            for (const auto& u : used) {
                if (std::get<0>(u) == nm->sym->name) seen = true;
            }
            if (!seen) used.append(std::make_tuple(nm->sym->name, nm->type));
        }
    }
    ci->captures = used;
}

void Model::name_anonymous() {
    for (const auto& ci : classes.values()) {
        if (!(ci->anon || ci->local)) continue;
        py::list<NodeP> stack = listed(ci->decl->members).copy();
        while (py::len(stack) > 0) {
            NodeP n = stack.pop();
            if (n.get() != ci->decl.get() && dynamic_pointer_cast<ir::ClassDecl>(n)) {
                continue;                                   // its own class does its members
            }
            ir::SymP s = sym_of(n);
            if (s != nullptr && s->owner.has_value() && !py::truthy(*s->owner) &&
                (s->kind == py::str("field") || s->kind == py::str("method") || s->kind == py::str("ctor"))) {
                s->owner = ci->qname;
            }
            if (auto th = dynamic_pointer_cast<ir::This>(n)) {
                const TypeP& t = th->type;
                if (t != nullptr && t->kind == py::str("class") && !(t->name && py::truthy(*t->name))) {
                    th->type = std::make_shared<ir::Type>(py::str("class"), ci->qname);
                }
            }
            auto md = dynamic_pointer_cast<ir::MethodDecl>(n);
            auto fd = dynamic_pointer_cast<ir::FieldDecl>(n);
            auto nw = dynamic_pointer_cast<ir::New>(n);
            if (md && md->body != nullptr) {
                stack.append(md->body);
            } else if (fd && fd->init != nullptr) {
                stack.append(fd->init);
            } else if (nw && nw->body != nullptr) {
                if (nw->args) stack.extend(*nw->args);
                if (nw->outer != nullptr) stack.append(nw->outer);
            } else {
                stack.extend(n->children());
            }
        }
    }
}

// ---- lookups
ref<ClassInfo> Model::info(const py::str& qname) const {
    return py::truthy(qname) ? classes.get(qname) : ref<ClassInfo>();
}

ref<ClassInfo> Model::of_type(const TypeP& t) const {
    if (t != nullptr && t->kind == py::str("class")) return classes.get(name_or_empty(t->name));
    return nullptr;
}

py::list<py::str> Model::supers(const py::str& qname) const {
    ref<ClassInfo> ci = classes.get(qname);
    if (ci != nullptr) {
        py::list<py::str> out;
        if (ci->base && py::truthy(*ci->base)) out.append(*ci->base);
        out.extend(ci->ifaces);
        return out;
    }
    py::list<py::str> out = LIB_SUPERS.get(qname, py::list<py::str>()).copy();
    if (qname != OBJECT) out.append(OBJECT);
    return out;
}

py::list<py::str> Model::ancestors(const py::str& qname) const {
    py::list<py::str> out, todo{qname};
    while (py::len(todo) > 0) {
        py::str q = todo.pop(0);
        if (out.contains(q)) continue;
        out.append(q);
        todo.extend(supers(q));
    }
    return out;
}

bool Model::is_sub(const py::str& a, const py::str& b) const {
    return b == OBJECT || ancestors(a).contains(b);
}

bool Model::is_interface(const py::str& qname) const {
    ref<ClassInfo> ci = classes.get(qname);
    if (ci != nullptr) return is(ci->kind, "interface");
    return SAMS.contains(qname) ||
           py::in(qname, std::make_tuple(py::str("java.lang.Comparable"), py::str("java.lang.Iterable"),
                                         py::str("java.util.Iterator"), py::str("java.lang.CharSequence"),
                                         py::str("java.util.List"), py::str("java.util.Map"),
                                         py::str("java.util.Set"), py::str("java.util.Collection")));
}

bool Model::subclassed(const py::str& qname) const {
    for (const auto& ci : classes.values()) {
        if (ci->base && *ci->base == qname) return true;
    }
    return false;
}

std::tuple<ref<ClassInfo>, ref<ir::MethodDecl>> Model::method_decl(const ir::SymP& sym) const {
    if (sym == nullptr) return {nullptr, nullptr};
    for (const py::str& q : ancestors(name_or_empty(sym->owner))) {
        ref<ClassInfo> ci = classes.get(q);
        if (ci == nullptr) continue;
        for (const auto& m : ci->methods(sym->kind == py::str("method") ? sym->name : py::str("<init>"))) {
            const py::list<NodeP>& mp = params_of(*m);
            if (py::len(mp) == py::len(sym->params)) {
                bool all = true;
                for (int64_t i = 0; i < py::len(mp) && i < py::len(sym->params); ++i) {
                    if (!(*erase(param_type(mp[i])) == *erase(sym->params[i]))) {
                        all = false;
                        break;
                    }
                }
                if (all) return {ci, m};
            }
        }
        if (sym->kind == py::str("ctor")) break;
    }
    return {nullptr, nullptr};
}

std::optional<std::tuple<py::list<TypeP>, TypeP>> Model::overrides(const ref<ClassInfo>& ci, const ref<ir::MethodDecl>& m) const {
    if (!is(m->kind, "method") || (m->sym != nullptr && m->sym->static_) || has(m->mods, "private")) return std::nullopt;
    std::optional<std::tuple<py::list<TypeP>, TypeP>> found;
    py::set<py::str> seen;
    py::list<py::str> todo = supers(ci->qname);
    while (py::len(todo) > 0) {
        py::str q = todo.pop(0);
        if (seen.contains(q)) continue;
        seen.add(q);
        ref<ClassInfo> sci = classes.get(q);
        if (sci != nullptr) {
            for (const auto& sm : sci->methods(m->name)) {
                if (!is(sm->kind, "method")) continue;
                const py::list<NodeP>& sp = params_of(*sm);
                const py::list<NodeP>& mp = params_of(*m);
                if (py::len(sp) == py::len(mp)) {
                    bool all = true;
                    for (int64_t i = 0; i < py::len(sp) && i < py::len(mp); ++i) {
                        if (!_matches(param_type(sp[i]), param_type(mp[i]))) {
                            all = false;
                            break;
                        }
                    }
                    if (all && !(sm->sym != nullptr && sm->sym->static_) && !has(sm->mods, "private")) {
                        py::list<TypeP> ps;
                        for (const NodeP& p : sp) ps.append(param_type(p));
                        found = std::make_tuple(ps, sm->ret);
                    }
                }
            }
        } else {
            for (const Sig& s : LIB_METHODS.get(q, py::list<Sig>())) {
                const py::str& name = std::get<0>(s);
                const py::list<TypeP>& ps = std::get<1>(s);
                if (!(m->name && name == *m->name)) continue;
                const py::list<NodeP>& mp = params_of(*m);
                if (py::len(ps) == py::len(mp)) {
                    bool all = true;
                    for (int64_t i = 0; i < py::len(ps) && i < py::len(mp); ++i) {
                        if (!_matches(ps[i], param_type(mp[i]))) {
                            all = false;
                            break;
                        }
                    }
                    if (all) found = std::make_tuple(ps.copy(), std::get<2>(s));
                }
            }
        }
        todo.extend(supers(q));
    }
    return found;
}

std::optional<Sig> Model::sam(const py::str& qname) const {
    if (SAMS.contains(qname)) return SAMS.at(qname);
    ref<ClassInfo> ci = classes.get(qname);
    if (ci == nullptr || !is(ci->kind, "interface")) return std::nullopt;
    py::list<Sig> abstract;
    for (const py::str& q : ancestors(qname)) {
        ref<ClassInfo> sci = classes.get(q);
        if (sci == nullptr) {
            if (SAMS.contains(q)) abstract.append(SAMS.at(q));
            continue;
        }
        if (!is(sci->kind, "interface")) continue;
        for (const auto& m : sci->methods()) {
            if (m->body == nullptr && !(m->sym != nullptr && m->sym->static_) &&
                !(m->name && py::in(*m->name, std::make_tuple(py::str("toString"), py::str("equals"), py::str("hashCode"))))) {
                bool dup = false;
                for (const Sig& a : abstract) {
                    if (m->name && std::get<0>(a) == *m->name && py::len(std::get<1>(a)) == py::len(params_of(*m))) dup = true;
                }
                if (!dup) {
                    py::list<TypeP> ps;
                    for (const NodeP& p : params_of(*m)) ps.append(param_type(p));
                    abstract.append(Sig(name_or_empty(m->name), ps, m->ret));
                }
            }
        }
    }
    if (py::len(abstract) == 1) return abstract[0];
    return std::nullopt;
}

py::list<py::str> Model::cycles() const {
    py::dict<py::str, py::set<py::str>> graph;
    for (const auto& ci : classes.values()) {
        py::set<py::str> out;
        for (const auto& f : ci->fields()) {
            if (f->sym == nullptr || f->sym->static_) continue;
            for (const py::str& q : _refs(f->type)) {
                if (classes.contains(q)) {
                    out.add(q);
                } else if (py::in(q, std::make_tuple(py::str("java.lang.Object"), py::str("java.util.List"),
                                                     py::str("java.util.ArrayList"), py::str("java.util.Map"),
                                                     py::str("java.util.HashMap"), py::str("java.util.Set"),
                                                     py::str("java.util.HashSet"), py::str("java.util.function.Function"),
                                                     py::str("java.util.function.Supplier"), py::str("java.lang.Runnable")))) {
                    out.add(py::str("*"));                       // anything at all: it may hold this class again
                }
            }
        }
        if (ci->needs_outer && ci->outer != nullptr) out.add(ci->outer->qname);
        graph.set(ci->qname, out);
    }
    py::list<py::str> result;
    for (const py::str& q : graph) {
        py::set<py::str> seen;
        py::list<py::str> todo = py::list<py::str>::from(graph.at(q));
        bool hit = false;
        while (py::len(todo) > 0) {
            py::str x = todo.pop();
            if (x == q || x == py::str("*")) {
                hit = true;
                break;
            }
            if (seen.contains(x)) continue;
            seen.add(x);
            todo.extend(graph.get(x, py::set<py::str>()));
            for (const auto& kv : classes.items()) {          // a field of type X may hold a subclass of X
                const py::str& sub = std::get<0>(kv);
                if (std::get<1>(kv)->base && *std::get<1>(kv)->base == x && !seen.contains(sub)) todo.append(sub);
            }
        }
        if (hit) result.append(classes.at(q)->cname);
    }
    return py::sorted(result);
}

// A parameter of an overridden method (super_t, as declared) and of the overriding one (t).
bool _matches(const TypeP& super_t, const TypeP& t) {
    if (super_t == nullptr || t == nullptr) return false;
    if (super_t->kind == py::str("typevar") || super_t->kind == py::str("wildcard")) return t->is_ref();
    return *erase(super_t) == *erase(t);
}

// The class names a field type can hold a reference to.
py::list<py::str> _refs(const TypeP& t) {
    py::list<py::str> out;
    if (t == nullptr) return out;
    if (t->kind == py::str("array")) return _refs(t->elem);
    if (t->kind == py::str("class")) {
        out.append(name_or_empty(t->name));
        for (const TypeP& a : t->args) out.extend(_refs(a));
    }
    if (t->kind == py::str("typevar") || t->kind == py::str("wildcard")) out.append(OBJECT);
    return out;
}

// ---------------------------------------------------------------------------------------------- the runtimes' names
static py::dict<py::str, py::str> make_lib() {
    py::dict<py::str, py::str> d;
    auto put = [&](const char* k, const char* v) { d.set(py::str(k), py::str(v)); };
    put("java.lang.Object", "Object"); put("java.lang.String", "String"); put("java.lang.CharSequence", "Object");
    put("java.lang.Integer", "Integer"); put("java.lang.Long", "Long"); put("java.lang.Short", "Short");
    put("java.lang.Byte", "Byte"); put("java.lang.Character", "Character"); put("java.lang.Boolean", "Boolean");
    put("java.lang.Float", "Float"); put("java.lang.Double", "Double"); put("java.lang.Number", "Object");
    put("java.lang.StringBuilder", "StringBuilder"); put("java.lang.StringBuffer", "StringBuilder");
    put("java.lang.AbstractStringBuilder", "StringBuilder"); put("java.lang.Math", "Math");
    put("java.lang.StrictMath", "Math"); put("java.lang.System", "System"); put("java.lang.Comparable", "Comparable");
    put("java.lang.Iterable", "Iterable"); put("java.lang.Runnable", "Runnable"); put("java.lang.Enum", "Enum");
    put("java.lang.Record", "Object"); put("java.lang.Throwable", "Throwable"); put("java.io.PrintStream", "PrintStream");
    put("java.util.List", "List"); put("java.util.ArrayList", "ArrayList"); put("java.util.LinkedList", "LinkedList");
    put("java.util.Collection", "Collection"); put("java.util.Iterator", "Iterator"); put("java.util.Map", "Map");
    put("java.util.HashMap", "HashMap"); put("java.util.LinkedHashMap", "LinkedHashMap"); put("java.util.TreeMap", "TreeMap");
    put("java.util.Map.Entry", "MapEntry"); put("java.util.Set", "Set"); put("java.util.HashSet", "HashSet");
    put("java.util.LinkedHashSet", "LinkedHashSet"); put("java.util.TreeSet", "TreeSet"); put("java.util.Deque", "LinkedList");
    put("java.util.Queue", "LinkedList"); put("java.util.ArrayDeque", "ArrayDeque"); put("java.util.Arrays", "Arrays");
    put("java.util.Collections", "Collections"); put("java.util.Objects", "Objects"); put("java.util.Optional", "Optional");
    put("java.util.OptionalInt", "OptionalInt"); put("java.util.OptionalDouble", "OptionalDouble");
    put("java.util.Comparator", "Comparator"); put("java.util.stream.Stream", "Stream");
    put("java.util.stream.IntStream", "IntStream"); put("java.util.stream.Collectors", "Collectors");
    put("java.util.stream.Collector", "Collector");
    // the last dotted part of a name: _q.rsplit('.', 1)[-1]
    auto tail = [](const py::str& q) { return std::get<2>(q.rpartition(py::str("."))); };
    for (const auto& kv : SAMS.items()) {
        const py::str& q = std::get<0>(kv);
        if (!d.contains(q)) d.set(q, tail(q));
    }
    for (const auto& kv : LIB_SUPERS.items()) {
        const py::str& q = std::get<0>(kv);
        if (q.endswith(std::make_tuple(py::str("Exception"), py::str("Error"))) || q == py::str("java.lang.Throwable")) {
            if (!d.contains(q)) d.set(q, tail(q));
        }
    }
    return d;
}
const py::dict<py::str, py::str> LIB = make_lib();

py::str char_literal(const py::str& c) {
    return char_literal(py::ord(c));
}

py::str char_literal(int64_t o) {
    if (32 <= o && o < 127 && o != '\\' && o != '\'') {
        return py::percent("u'%s'", py::chr(o));
    }
    return py::percent("(char16_t)%d", o);
}

bool constant_init(const ref<ir::FieldDecl>& f) {
    NodeP init = f->init;
    while (auto p = dynamic_pointer_cast<ir::Paren>(init)) init = p->x;
    if (auto u = dynamic_pointer_cast<ir::Unary>(init)) {
        if (is(u->op, "-") && dynamic_pointer_cast<ir::Lit>(u->x)) init = u->x;
    }
    auto lit = dynamic_pointer_cast<ir::Lit>(init);
    return lit != nullptr && !lit->value.is_none() && !std::holds_alternative<py::str>(lit->value.v) &&
           f->type != nullptr && f->type->is_prim();
}

}  // namespace codebreaker::translate::backends::jmodel

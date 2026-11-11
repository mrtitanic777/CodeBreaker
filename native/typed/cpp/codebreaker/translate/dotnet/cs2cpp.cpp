// codebreaker/translate/dotnet/cs2cpp.cpp -- typed C++ port of codebreaker.translate.dotnet.cs2cpp (see cs2cpp.hpp).

#include "codebreaker/translate/dotnet/cs2cpp.hpp"

#include <initializer_list>
#include <memory>

#include "py_os.hpp"

namespace codebreaker::translate::dotnet::cs2cpp {

using py::str;
using ir::NodeP;
using ir::SymP;
using ir::TypeP;
using ir::Val;

namespace {

str S(const char* s) { return str(s); }

// ---- tiny helpers over the IR (Python's isinstance / attribute access / `x in (...)`)
template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }
template <class T> py::ref<T> as_a(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

bool one_of(const str& s, std::initializer_list<const char*> xs) {
    for (const char* x : xs)
        if (s == x) return true;
    return false;
}
bool one_of(const std::optional<str>& s, std::initializer_list<const char*> xs) { return s && one_of(*s, xs); }
bool is_str(const std::optional<str>& s, const char* x) { return s && *s == x; }

[[noreturn]] void no_attr(const NodeP& n, const char* attr) {
    if (!n) throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'");
    throw py::AttributeError("'" + n->class_name().utf8() + "' object has no attribute '" + attr + "'");
}
[[noreturn]] void none_attr_(const char* attr) { throw py::AttributeError(std::string("'NoneType' object has no attribute '") + attr + "'"); }

// `e.type` (AttributeError for None and for a node that is not an expression)
TypeP type_of(const NodeP& e) {
    if (!e) no_attr(e, "type");
    if (auto x = as_a<ir::Expr>(e)) return x->type;
    no_attr(e, "type");
}

// `t.name` of a Type that must have one (a class), as the Python reads it
const str& name_of(const TypeP& t) {
    if (!t) none_attr_("name");
    if (!t->name) none_attr_("startswith");
    return *t->name;
}

// the class node of a unit's `types` / a member list
py::ref<ir::ClassDecl> class_of(const NodeP& c) {
    auto x = as_a<ir::ClassDecl>(c);
    if (!x) no_attr(c, "kind");
    return x;
}

// c.qname where it is used as a string (None + str is a TypeError)
str qn(const py::ref<ir::ClassDecl>& c) {
    if (!c->qname) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'str'");
    return *c->qname;
}

template <class T> const py::list<T>& iter_(const std::optional<py::list<T>>& x) { return py::none_iter(x); }
template <class T> py::list<T> or_empty(const std::optional<py::list<T>>& x) { return x ? *x : py::list<T>(); }

template <class T> std::shared_ptr<T> mk() { return std::make_shared<T>(); }

TypeP type_(const char* kind) { return std::make_shared<ir::Type>(str(kind)); }
TypeP class_type(const std::optional<str>& name) { return std::make_shared<ir::Type>(S("class"), name); }
TypeP class_type(const char* name) { return class_type(std::optional<str>(str(name))); }
TypeP typevar(const char* name) { return std::make_shared<ir::Type>(S("typevar"), std::optional<str>(str(name))); }

ir::TypeArgs no_typeargs() { return ir::TypeArgs(std::variant<py::list<TypeP>, py::list<Val>>(py::list<TypeP>())); }

NodeP lit_str(const char* value, const TypeP& t) {
    auto l = mk<ir::Lit>();
    l->value = Val(str(value));
    l->type = t;
    return l;
}

// a `body` slot read as the Python reads a statement: None, a node, or a list of statements
bool body_is_none(const ir::NodeOrList& b) { return std::holds_alternative<NodeP>(b) && !std::get<NodeP>(b); }

// ---- the class of a node as the Python's type(x).__name__
str kind_name(const NodeP& n) { return n ? n->class_name() : S("NoneType"); }

// the unqualified part of a dotted name: name.rsplit('.', 1)[-1]
str last_part(const str& n) {
    int64_t k = n.rfind(S("."));
    return k < 0 ? n : n.slice_from(k + 1);
}

str join_mods(const py::list<str>& mods) { return py::join(S(" "), mods); }

}  // namespace

// ---------------------------------------------------------------------------------------------- tables
const py::dict<str, str> _PRIM_JAVA = {{S("bool"), S("boolean")}, {S("i8"), S("byte")},   {S("i16"), S("short")},
                                       {S("u16"), S("char")},     {S("i32"), S("int")},    {S("i64"), S("long")},
                                       {S("f32"), S("float")},    {S("f64"), S("double")}, {S("void"), S("void")}};
const py::dict<str, str> _BOXED = {{S("bool"), S("java.lang.Boolean")}, {S("i8"), S("java.lang.Byte")},
                                   {S("i16"), S("java.lang.Short")},    {S("u16"), S("java.lang.Character")},
                                   {S("i32"), S("java.lang.Integer")},  {S("i64"), S("java.lang.Long")},
                                   {S("f32"), S("java.lang.Float")},    {S("f64"), S("java.lang.Double")}};
const py::dict<str, str> _CLASSES = {
    {S("System.String"), S("java.lang.String")},
    {S("System.Object"), S("java.lang.Object")},
    {S("System.Collections.Generic.List"), S("java.util.ArrayList")},
    {S("System.Exception"), S("java.lang.RuntimeException")},
    {S("System.InvalidOperationException"), S("java.lang.IllegalStateException")},
    {S("System.ArgumentException"), S("java.lang.IllegalArgumentException")},
    {S("System.DivideByZeroException"), S("java.lang.ArithmeticException")},
    {S("System.ArithmeticException"), S("java.lang.ArithmeticException")},
    {S("System.NullReferenceException"), S("java.lang.NullPointerException")},
    {S("System.IndexOutOfRangeException"), S("java.lang.ArrayIndexOutOfBoundsException")},
    {S("System.Math"), S("java.lang.Math")},
};
const py::dict<str, str> _MATH = {{S("Sqrt"), S("sqrt")}, {S("Abs"), S("abs")},   {S("Max"), S("max")},     {S("Min"), S("min")},
                                  {S("Sin"), S("sin")},   {S("Cos"), S("cos")},   {S("Floor"), S("floor")}, {S("Pow"), S("pow")},
                                  {S("Atan2"), S("atan2")}, {S("Tan"), S("tan")}, {S("Exp"), S("exp")},     {S("Log"), S("log")}};
const TypeP STRING = std::make_shared<ir::Type>(str("class"), std::optional<str>(str("java.lang.String")));

SymP _m(const str& name, const std::optional<str>& owner, const py::list<TypeP>& params, const TypeP& ret, bool static_) {
    return std::make_shared<ir::Sym>(S("method"), name, owner, static_, nullptr, params, ret);
}

// ---------------------------------------------------------------------------------------------- ToJava
ToJava::ToJava(const py::list<NodeP>& units_) : units(units_) {
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        if (!u) no_attr(un, "types");
        for (const NodeP& c : iter_(u->types)) {
            if (auto cd = as_a<ir::ClassDecl>(c)) classes.set(cd->qname, cd);
        }
    }
}

// ---- types
TypeP ToJava::ty(const TypeP& t, bool boxed) {
    if (!t) return nullptr;
    const str& k = t->kind;
    if (one_of(k, {"u8", "u32", "u64"}))
        throw Refuse(S("unsigned"), py::percent(S("an unsigned integer (%s): Java and its C++ runtime have none"), k));
    if (_PRIM_JAVA.contains(k)) return boxed && k != "void" ? class_type(std::optional<str>(_BOXED.at(k))) : std::make_shared<ir::Type>(k);
    if (k == "array") {
        // t.info.get('rank', 1) != 1
        bool one = true;
        if (t->info.contains(S("rank"))) {
            const Val& r = t->info.at(S("rank"));
            if (std::holds_alternative<int64_t>(r.v)) one = std::get<int64_t>(r.v) == 1;
            else if (std::holds_alternative<bool>(r.v)) one = std::get<bool>(r.v);
            else if (std::holds_alternative<double>(r.v)) one = std::get<double>(r.v) == 1.0;
            else one = false;
        }
        if (!one) throw Refuse(S("array"), S("a rectangular array"));
        return std::make_shared<ir::Type>(S("array"), std::nullopt, py::list<TypeP>(), ty(t->elem));
    }
    if (k == "class") {
        const str& name = name_of(t);
        if (name.startswith(S("java."))) return t;   // (already Java's)
        if (name == "System.Nullable") throw Refuse(S("nullable"), S("a nullable value type"));
        if (classes.contains(std::optional<str>(name))) {
            auto c = classes.at(std::optional<str>(name));
            if (!one_of(c->kind, {"class", "interface"}))
                throw Refuse(c->kind ? *c->kind : S("None"),
                             py::percent(S("a C# %s: no faithful form on the C++03 path"), c->kind));
            return class_type(std::optional<str>(name));
        }
        if (_CLASSES.contains(name)) {
            py::list<TypeP> args;
            for (const TypeP& a : t->args) args.append(ty(a, true));
            return std::make_shared<ir::Type>(S("class"), std::optional<str>(_CLASSES.at(name)), args);
        }
        throw Refuse(S("library"), py::percent(S("the .NET type %s has no mapping onto Java's runtime"), name));
    }
    if (k == "null" || k == "any") return t;
    if (k == "typevar") throw Refuse(S("generic"), py::percent(S("a type parameter (%s)"), t->name));
    throw Refuse(S("type"), S("the type ") + t->repr());
}

SymP ToJava::sym(const SymP& s) {
    if (!s) return nullptr;
    std::optional<str> owner = s->owner;
    if (owner && _CLASSES.contains(*owner)) owner = _CLASSES.at(*owner);
    py::list<TypeP> params;
    for (const TypeP& p : s->params) params.append(ty(p));
    TypeP t = s->type ? ty(s->type) : nullptr;
    TypeP r = s->ret ? ty(s->ret) : nullptr;
    return std::make_shared<ir::Sym>(s->kind, s->name, owner, s->static_, t, params, r, s->varargs);
}

// ---- the program
py::list<Refusal> ToJava::run() {
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        py::list<NodeP> kept;
        for (const NodeP& c : iter_(u->types)) {
            try {
                check_class(c);
                kept.append(c);
            } catch (const Refuse& r) {
                refused.append(Refusal(class_of(c)->name, r.kind, r.why));
            }
        }
        u->types = kept;
    }
    // (self.classes.values(): a class refused above is still among them)
    py::list<py::ref<ir::ClassDecl>> all;
    for (const auto& kv : classes.items()) all.append(std::get<1>(kv));
    for (const auto& c : all) {
        try {
            collect_props(c);
        } catch (const Refuse& r) {
            refused.append(Refusal(c->name, r.kind, r.why));
        }
    }
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        for (const NodeP& c : iter_(u->types)) {
            try {
                klass(c);
            } catch (const Refuse& r) {
                refused.append(Refusal(class_of(c)->name, r.kind, r.why));
            }
        }
        u->lang = S("java");
    }
    return refused;
}

void ToJava::check_class(const NodeP& cn) {
    auto c = class_of(cn);
    if (!one_of(c->kind, {"class", "interface"}))
        throw Refuse(c->kind ? *c->kind : S("None"), py::percent(S("a C# %s: no faithful form on the C++03 path"), c->kind));
    if (py::truthy(c->typeparams)) throw Refuse(S("generic"), py::percent(S("a generic class (%s)"), c->name));
    for (const NodeP& m : iter_(c->members)) {
        if (auto nested = as_a<ir::ClassDecl>(m))
            throw Refuse(S("nested"), py::percent(S("a nested class (%s.%s): the transpiler finds bodies by top-level class"), c->name,
                                                  nested->name));
        if (auto un = as_a<ir::Unsupported>(m)) {
            str raw = un->raw && !un->raw->empty() ? *un->raw : (un->kind ? *un->kind : S("None"));
            throw Refuse(un->kind ? *un->kind : S("None"), raw);
        }
    }
}

void ToJava::collect_props(const NodeP& cn) {
    auto c = class_of(cn);
    for (const NodeP& mn : iter_(c->members)) {
        auto m = as_a<ir::Property>(mn);
        if (!m) continue;
        if (py::truthy(m->params)) throw Refuse(S("indexer"), S("an indexer"));
        TypeP t = ty(m->type);
        if (!m->sym) none_attr_("static");
        bool static_ = m->sym->static_;
        SymP g = _m(S("get") + m->sym->name, c->qname, py::list<TypeP>(), t, static_);
        bool writable = false;
        for (const NodeP& an : or_empty(m->accessors)) {
            auto a = as_a<ir::MethodDecl>(an);
            if (!a) no_attr(an, "kind");
            if (one_of(a->kind, {"set", "init"})) writable = true;
        }
        SymP s = writable ? _m(S("set") + m->sym->name, c->qname, py::list<TypeP>{t}, ir::VOID, static_) : nullptr;
        Key key(c->qname, m->sym->name);
        props.set(key, Accessors{g, s, static_});
        bool all_none = true;
        for (const NodeP& an : or_empty(m->accessors))
            if (!body_is_none(as_a<ir::MethodDecl>(an)->body)) all_none = false;
        bool abstract_ = false;
        for (const str& x : or_empty(m->mods))
            if (x == "abstract") abstract_ = true;
        if (all_none && !abstract_ && !is_str(c->kind, "interface"))
            auto_.set(key, std::make_shared<ir::Sym>(S("field"), S("_") + m->sym->name, c->qname, static_, t));
    }
}

std::optional<Accessors> ToJava::prop(std::optional<str> owner, const str& name) {
    py::set<str> seen;
    while (owner && !owner->empty() && !seen.contains(*owner)) {
        seen.add(*owner);
        Key k(owner, name);
        if (props.contains(k)) return props.at(k);
        if (!classes.contains(owner)) break;
        auto c = classes.at(owner);
        owner = c->extends ? c->extends->name : std::nullopt;
        for (const TypeP& i : or_empty(c->implements)) {
            if (props.contains(Key(i->name, name))) return props.at(Key(i->name, name));
        }
    }
    return std::nullopt;
}

void ToJava::klass(const NodeP& cn) {
    auto c = class_of(cn);
    {
        py::list<str> mods_;
        for (const str& m : iter_(c->mods))
            if (one_of(m, {"public", "abstract"})) mods_.append(m);
        bool sealed = false;
        for (const str& m : iter_(c->mods))
            if (m == "sealed") sealed = true;
        if (sealed) mods_.append(S("final"));
        c->mods = mods_;
    }
    if (c->extends) c->extends = ty(c->extends);
    {
        py::list<TypeP> impl;
        for (const TypeP& i : or_empty(c->implements)) impl.append(ty(i));
        c->implements = impl;
    }
    py::list<NodeP> out;
    for (const NodeP& m : iter_(c->members)) {
        try {
            out.extend(member(m, c));
        } catch (const Refuse& r) {
            // getattr(m, 'name', None) or getattr(m.sym, 'name', '?')
            std::optional<str> nm;
            if (auto md = as_a<ir::MethodDecl>(m)) nm = md->name;
            if (!nm || nm->empty()) {
                SymP sy;
                if (auto x = as_a<ir::FieldDecl>(m)) sy = x->sym;
                else if (auto x = as_a<ir::MethodDecl>(m)) sy = x->sym;
                else if (auto x = as_a<ir::Property>(m)) sy = x->sym;
                else if (auto x = as_a<ir::EnumConst>(m)) sy = x->sym;
                else if (auto x = as_a<ir::Param>(m)) sy = x->sym;
                else if (auto x = as_a<ir::Local>(m)) sy = x->sym;
                nm = sy ? sy->name : S("?");
            }
            refused.append(Refusal(py::percent(S("%s.%s"), c->name, nm), r.kind, r.why));
            if (auto md = as_a<ir::MethodDecl>(m); md && !body_is_none(md->body)) {
                auto un = mk<ir::Unsupported>();
                un->kind = r.kind;
                un->raw = r.why;
                un->type = ir::ANY;
                auto es = mk<ir::ExprStmt>();
                es->expr = un;
                auto b = mk<ir::Block>();
                b->stmts = py::list<NodeP>{es};
                md->body = NodeP(b);
                out.append(m);
            }
        }
    }
    c->members = out;
}

py::list<str> ToJava::mods(const std::optional<py::list<str>>& mods_) {
    py::list<str> out;
    for (const str& m : or_empty(mods_)) {
        if (one_of(m, {"public", "private", "protected", "static", "abstract"})) {
            out.append(m);
        } else if (m == "const") {
            out.append(S("static"));
            out.append(S("final"));
        } else if (m == "sealed") {
            out.append(S("final"));
        }
    }
    return out;
}

py::list<NodeP> ToJava::member(const NodeP& m, const NodeP& c) {
    if (auto f = as_a<ir::FieldDecl>(m)) {
        SymP s = sym(f->sym);
        f->sym = s;
        f->type = ty(f->type);
        f->mods = mods(f->mods);
        if (f->init) f->init = expr(f->init);
        return py::list<NodeP>{m};
    }
    if (is_a<ir::Property>(m)) return lower_prop(m, c);
    if (is_a<ir::MethodDecl>(m)) return py::list<NodeP>{method(m, c)};
    if (is_a<ir::EnumConst>(m)) throw Refuse(S("enum"), S("an enum"));
    throw Refuse(S("member"), S("a member ") + kind_name(m));
}

py::list<NodeP> ToJava::lower_prop(const NodeP& pn, const NodeP& cn) {
    auto p = as_a<ir::Property>(pn);
    auto c = class_of(cn);
    if (!p->sym) none_attr_("name");
    if (!props.contains(Key(c->qname, p->sym->name)))
        throw Refuse(S("property"), py::percent(S("the property %s.%s (its type has no form here)"), c->name, p->sym->name));
    Accessors acc = props.at(Key(c->qname, p->sym->name));
    SymP g = acc.getter, s = acc.setter;
    bool static_ = acc.static_;
    TypeP t = g->ret;
    py::list<NodeP> out;
    py::list<str> pm;
    for (const str& x : or_empty(p->mods))
        if (!one_of(x, {"override", "virtual"})) pm.append(x);
    py::list<str> mods_ = mods(pm);
    bool abstract_ = false;
    for (const str& x : or_empty(p->mods))
        if (x == "abstract") abstract_ = true;
    abstract_ = abstract_ || is_str(c->kind, "interface");
    bool auto_p = !abstract_;
    if (auto_p) {
        for (const NodeP& an : or_empty(p->accessors)) {
            auto a = as_a<ir::MethodDecl>(an);
            if (!a) no_attr(an, "body");
            if (!body_is_none(a->body)) {
                auto_p = false;
                break;
            }
        }
    }
    SymP field;
    if (auto_p) {
        SymP fs = std::make_shared<ir::Sym>(S("field"), S("_") + p->sym->name, c->qname, static_, t);
        field = fs;
        auto fd = mk<ir::FieldDecl>();
        fd->sym = fs;
        fd->type = t;
        NodeP init = p->init ? expr(p->init) : nullptr;
        fd->init = init;
        py::list<str> fm{S("private")};
        if (static_) fm.append(S("static"));
        fd->mods = fm;
        fd->line = p->line;
        fd->col = p->col;
        out.append(fd);
    }
    for (const NodeP& an : or_empty(p->accessors)) {
        auto a = as_a<ir::MethodDecl>(an);
        if (!a) no_attr(an, "kind");
        SymP sy = is_str(a->kind, "get") ? g : s;
        if (!sy) continue;
        py::list<str> amods;
        for (const str& x : mods_)
            if (!one_of(x, {"public", "private", "protected"})) amods.append(x);
        if (py::truthy(a->mods)) amods = mods(a->mods) + amods;
        else amods = mods_;
        NodeP body;
        if (abstract_) {
            body = nullptr;
            py::list<str> keep;
            for (const str& x : amods)
                if (x != "abstract") keep.append(x);
            if (!is_str(c->kind, "interface")) keep.append(S("abstract"));
            amods = keep;
        } else if (body_is_none(a->body)) {   // the compiler's own accessor
            auto tn = [&]() -> NodeP {
                if (static_) {
                    auto x = mk<ir::TypeName>();
                    x->type = class_type(c->qname);
                    return x;
                }
                auto x = mk<ir::This>();
                x->qualifier = nullptr;
                x->type = class_type(c->qname);
                return x;
            };
            auto fsel = mk<ir::Select>();
            fsel->obj = tn();
            fsel->sym = field;
            fsel->type = t;
            if (is_str(a->kind, "get")) {
                auto r = mk<ir::Return>();
                r->expr = fsel;
                auto b = mk<ir::Block>();
                b->stmts = py::list<NodeP>{r};
                body = b;
            } else {
                auto v = mk<ir::Name>();
                v->sym = std::make_shared<ir::Sym>(S("param"), S("value"), std::optional<str>(qn(c) + S(".set") + p->sym->name), false, t);
                v->type = t;
                auto as = mk<ir::Assign>();
                as->op = std::nullopt;
                as->target = fsel;
                as->value = v;
                as->type = t;
                auto es = mk<ir::ExprStmt>();
                es->expr = as;
                auto b = mk<ir::Block>();
                b->stmts = py::list<NodeP>{es};
                body = b;
            }
        } else {
            if (!std::holds_alternative<NodeP>(a->body)) no_attr(nullptr, "stmts");
            body = block(std::get<NodeP>(a->body));
        }
        py::list<NodeP> params;
        if (!is_str(a->kind, "get")) {
            auto pr = mk<ir::Param>();
            pr->sym = std::make_shared<ir::Sym>(S("param"), S("value"), std::optional<str>(qn(c) + S(".set") + p->sym->name), false, t);
            pr->type = t;
            pr->mods = py::list<str>();
            params.append(pr);
        }
        auto md = mk<ir::MethodDecl>();
        md->sym = sy;
        md->name = sy->name;
        md->kind = S("method");
        md->params = params;
        md->ret = sy->ret;
        md->body = body;
        md->mods = amods;
        md->throws = py::list<TypeP>();
        md->typeparams = py::list<str>();
        md->line = a->line ? a->line : p->line;
        md->col = a->col ? a->col : p->col;
        out.append(md);
    }
    return out;
}

NodeP ToJava::method(const NodeP& mn, const NodeP& cn) {
    auto m = as_a<ir::MethodDecl>(mn);
    auto c = class_of(cn);
    std::optional<str> name = m->name;
    {
        static const py::dict<str, str> rename = {{S("ToString"), S("toString")}, {S("Equals"), S("equals")}, {S("GetHashCode"), S("hashCode")}};
        if (m->name && rename.contains(*m->name)) name = rename.at(*m->name);
    }
    bool is_static = false;
    for (const str& x : or_empty(m->mods))
        if (x == "static") is_static = true;
    if (is_str(m->kind, "method") && is_str(m->name, "Main") && is_static) {
        name = S("main");
        if (!py::truthy(m->params)) {
            TypeP st = std::make_shared<ir::Type>(S("array"), std::nullopt, py::list<TypeP>(), STRING);
            auto pr = mk<ir::Param>();
            pr->sym = std::make_shared<ir::Sym>(S("param"), S("args"), std::optional<str>(qn(c) + S(".main")), false, st);
            pr->type = st;
            pr->mods = py::list<str>();
            m->params = py::list<NodeP>{pr};
        }
    }
    for (const str& x : or_empty(m->mods))
        if (x == "operator") throw Refuse(S("operator"), S("an operator"));
    m->name = name;
    m->sym = m->sym ? sym(m->sym) : nullptr;
    if (m->sym) {
        m->sym->name = name ? *name : S("None");
        if (is_str(name, "main")) m->sym->params = py::list<TypeP>{std::make_shared<ir::Type>(S("array"), std::nullopt, py::list<TypeP>(), STRING)};
    }
    for (const NodeP& pn : or_empty(m->params)) {
        auto p = as_a<ir::Param>(pn);
        if (!p) no_attr(pn, "sym");
        bool any_ref = false;
        if (p->sym)
            for (const str& r : p->sym->refs)
                if (!r.empty()) any_ref = true;
        if (any_ref) throw Refuse(S("ref"), S("a ref or out parameter"));
        p->type = ty(p->type);
        p->sym = sym(p->sym);
    }
    m->ret = ty(m->ret);
    if (!is_str(m->kind, "ctor")) {
        m->mods = mods(m->mods);
    } else {
        py::list<str> v;
        for (const str& x : mods(m->mods))
            if (x != "static") v.append(x);
        bool implicit = false;
        for (const str& x : or_empty(m->mods))
            if (x == "IMPLICIT") implicit = true;
        if (implicit) v.append(S("IMPLICIT"));
        m->mods = v;
    }
    if (is_str(c->kind, "interface")) {
        py::list<str> v;
        for (const str& x : iter_(m->mods))
            if (x != "abstract") v.append(x);
        m->mods = v;
    }
    if (!body_is_none(m->body)) {
        if (!std::holds_alternative<NodeP>(m->body)) no_attr(nullptr, "stmts");
        m->body = NodeP(block(std::get<NodeP>(m->body)));
    }
    return m;
}

// ---- statements
NodeP ToJava::block(const NodeP& bn) {
    auto b = as_a<ir::Block>(bn);
    if (!b) no_attr(bn, "stmts");
    py::list<NodeP> ss;
    for (const NodeP& s : iter_(b->stmts)) ss.append(stmt(s));
    auto nb = mk<ir::Block>();
    nb->stmts = ss;
    nb->line = b->line;
    nb->col = b->col;
    return nb;
}

NodeP ToJava::stmt(const NodeP& s) {
    str k = kind_name(s);
    auto stmt_slot = [&](const ir::NodeOrList& b) -> NodeP {
        if (std::holds_alternative<NodeP>(b)) return stmt(std::get<NodeP>(b));
        throw Refuse(S("list"), S("the statement list on the C++03 path"));
    };
    if (is_a<ir::Block>(s)) return block(s);
    if (auto l = as_a<ir::Local>(s)) {
        SymP a = sym(l->sym);
        TypeP b = ty(l->type);
        l->sym = a;
        l->type = b;
        if (l->init) l->init = expr(l->init);
        return s;
    }
    if (auto es = as_a<ir::ExprStmt>(s)) {
        NodeP e = es->expr;
        auto asg = as_a<ir::Assign>(e);
        if (asg && is_a<ir::Select>(asg->target) && as_a<ir::Select>(asg->target)->sym && as_a<ir::Select>(asg->target)->sym->kind == "property") {
            auto r = mk<ir::ExprStmt>();
            r->expr = set_prop(e);
            r->line = s->line;
            r->col = s->col;
            return r;
        }
        auto un = as_a<ir::Unary>(e);
        if (un && one_of(un->op, {"++", "--"}) && is_a<ir::Select>(un->x) && as_a<ir::Select>(un->x)->sym &&
            as_a<ir::Select>(un->x)->sym->kind == "property") {
            auto one = mk<ir::Lit>();
            one->value = Val(int64_t(1));
            one->type = ir::I32;
            auto a2 = mk<ir::Assign>();
            a2->op = un->op->slice(0, 1);   // e.op[0]
            a2->target = un->x;
            a2->value = one;
            a2->type = un->type;
            auto r = mk<ir::ExprStmt>();
            r->expr = set_prop(a2);
            r->line = s->line;
            r->col = s->col;
            return r;
        }
        if (asg && is_a<ir::Index>(asg->target) && as_a<ir::Index>(asg->target)->sym && !asg->op) {
            auto r = mk<ir::ExprStmt>();
            r->expr = set_index(e);
            r->line = s->line;
            r->col = s->col;
            return r;
        }
        es->expr = expr(e);
        return s;
    }
    if (auto i = as_a<ir::If>(s)) {
        NodeP c = expr(i->cond);
        NodeP t = stmt_slot(i->then);
        i->cond = c;
        i->then = NodeP(t);
        if (!body_is_none(i->orelse)) i->orelse = NodeP(stmt_slot(i->orelse));
        else i->orelse = NodeP();
        return s;
    }
    if (auto w = as_a<ir::While>(s)) {
        NodeP c = expr(w->cond);
        NodeP b = stmt_slot(w->body);
        w->cond = c;
        w->body = NodeP(b);
        return s;
    }
    if (auto d = as_a<ir::DoWhile>(s)) {
        NodeP b = stmt_slot(d->body);
        NodeP c = expr(d->cond);
        d->body = NodeP(b);
        d->cond = c;
        return s;
    }
    if (auto f = as_a<ir::For>(s)) {
        py::list<NodeP> init;
        for (const NodeP& x : or_empty(f->init)) init.append(stmt(x));
        f->init = init;
        f->cond = f->cond ? expr(f->cond) : nullptr;
        py::list<NodeP> upd;
        for (const NodeP& x : or_empty(f->update)) upd.append(expr(x));
        f->update = upd;
        f->body = NodeP(stmt_slot(f->body));
        return s;
    }
    if (is_a<ir::ForEach>(s)) return foreach_(s);
    if (auto sw = as_a<ir::Switch>(s)) {
        TypeP et = type_of(sw->expr);
        if (et && et->kind == "class" && is_str(et->name, "System.String")) return string_switch(s);
        sw->expr = expr(sw->expr);
        for (const NodeP& cn : or_empty(sw->cases)) {
            auto c = as_a<ir::Case>(cn);
            if (!c) no_attr(cn, "labels");
            py::list<NodeP> labels;
            for (const NodeP& x : or_empty(c->labels)) labels.append(expr(x));
            c->labels = labels;
            py::list<NodeP> body;
            for (const NodeP& x : or_empty(c->body)) body.append(stmt(x));
            c->body = body;
        }
        return s;
    }
    if (one_of(k, {"Break", "Continue", "Empty", "Comment"})) {
        if (auto b = as_a<ir::Break>(s)) {
            if (b->label && !b->label->empty()) throw Refuse(S("label"), S("a labelled jump"));
        }
        if (auto b = as_a<ir::Continue>(s)) {
            if (b->label && !b->label->empty()) throw Refuse(S("label"), S("a labelled jump"));
        }
        return s;
    }
    if (auto r = as_a<ir::Return>(s)) {
        r->expr = r->expr ? expr(r->expr) : nullptr;
        return s;
    }
    if (auto t = as_a<ir::Throw>(s)) {
        if (!t->expr) throw Refuse(S("rethrow"), S("`throw;`"));
        t->expr = expr(t->expr);
        return s;
    }
    if (auto t = as_a<ir::Try>(s)) {
        if (py::truthy(t->resources)) throw Refuse(S("using"), S("using (IDisposable): the C++03 runtime has no Dispose to call"));
        t->body = block(t->body);
        for (const NodeP& cn : or_empty(t->catches)) {
            auto c = as_a<ir::Catch>(cn);
            if (!c) no_attr(cn, "sym");
            c->sym = sym(c->sym);
            py::list<TypeP> types;
            for (const TypeP& x : or_empty(c->types)) types.append(ty(x));
            c->types = types;
            c->body = block(c->body);
        }
        t->final = t->final ? block(t->final) : nullptr;
        return s;
    }
    if (auto cc = as_a<ir::CtorCall>(s)) {
        cc->sym = sym(cc->sym);
        py::list<NodeP> args;
        for (const NodeP& a : or_empty(cc->args)) args.append(expr(a));
        cc->args = args;
        return s;
    }
    throw Refuse(k, py::percent(S("the statement %s on the C++03 path"), k));
}

NodeP ToJava::foreach_(const NodeP& sn) {
    auto s = as_a<ir::ForEach>(sn);
    NodeP varn = stmt(s->var);
    auto var = as_a<ir::Local>(varn);
    if (!var) no_attr(varn, "init");
    NodeP coll = expr(s->iterable);
    auto csel = as_a<ir::Select>(coll);
    if ((!is_a<ir::Name>(coll) && !csel && !is_a<ir::This>(coll)) ||
        (csel && !(is_a<ir::Name>(csel->obj) || is_a<ir::This>(csel->obj) || is_a<ir::TypeName>(csel->obj))))
        throw Refuse(S("foreach"), S("foreach over an expression with effects"));
    loops = loops + 1;
    if (!var->sym) none_attr_("owner");
    SymP i = std::make_shared<ir::Sym>(S("local"), py::percent(S("_i%d"), loops), var->sym->owner, false, ir::I32);
    auto iv = [&]() {
        auto x = mk<ir::Name>();
        x->sym = i;
        x->type = ir::I32;
        return x;
    };
    NodeP ivn = iv();
    TypeP ct = type_of(coll);
    NodeP n, item;
    if (ct && ct->kind == "array") {
        auto len = mk<ir::Length>();
        len->arr = coll;
        len->type = ir::I32;
        n = len;
        auto ix = mk<ir::Index>();
        ix->arr = coll;
        ix->index = ivn;
        ix->type = var->type;
        item = ix;
    } else if (ct && ct->kind == "class" && is_str(ct->name, "java.util.ArrayList")) {
        n = call(coll, _m(S("size"), std::optional<str>(S("java.util.ArrayList")), py::list<TypeP>(), ir::I32), py::list<NodeP>(), ir::I32);
        item = call(coll, _m(S("get"), std::optional<str>(S("java.util.ArrayList")), py::list<TypeP>{ir::I32}, typevar("E")),
                    py::list<NodeP>{ivn}, var->type);
    } else if (ct && ct->kind == "class" && is_str(ct->name, "java.lang.String")) {
        n = call(coll, _m(S("length"), std::optional<str>(S("java.lang.String")), py::list<TypeP>(), ir::I32), py::list<NodeP>(), ir::I32);
        item = call(coll, _m(S("charAt"), std::optional<str>(S("java.lang.String")), py::list<TypeP>{ir::I32}, type_("u16")),
                    py::list<NodeP>{ivn}, type_("u16"));
    } else {
        throw Refuse(S("foreach"), S("foreach over a ") + (ct ? ct->repr() : S("None")));
    }
    var->init = item;
    NodeP body = stmt(s->body);
    py::list<NodeP> stmts{varn};
    if (auto bb = as_a<ir::Block>(body)) stmts.extend(or_empty(bb->stmts));
    else stmts.append(body);
    auto zero = mk<ir::Lit>();
    zero->value = Val(int64_t(0));
    zero->type = ir::I32;
    auto init = mk<ir::Local>();
    init->sym = i;
    init->type = ir::I32;
    init->init = zero;
    init->mods = py::list<str>();
    auto cond = mk<ir::Binary>();
    cond->op = S("<");
    cond->left = ivn;
    cond->right = n;
    cond->type = ir::BOOL;
    auto upd = mk<ir::Unary>();
    upd->op = S("++");
    upd->x = ivn;
    upd->postfix = false;
    upd->type = ir::I32;
    auto fb = mk<ir::Block>();
    fb->stmts = stmts;
    auto f = mk<ir::For>();
    f->init = py::list<NodeP>{init};
    f->cond = cond;
    f->update = py::list<NodeP>{upd};
    f->body = NodeP(fb);
    f->line = s->line;
    f->col = s->col;
    return f;
}

NodeP ToJava::string_switch(const NodeP& sn) {
    auto s = as_a<ir::Switch>(sn);
    NodeP v = expr(s->expr);
    {
        auto vs = as_a<ir::Select>(v);
        if (!(is_a<ir::Name>(v) || is_a<ir::This>(v)) && !(vs && (is_a<ir::Name>(vs->obj) || is_a<ir::This>(vs->obj) || is_a<ir::TypeName>(vs->obj))))
            throw Refuse(S("switch"), S("a switch on a string expression with effects"));
    }
    struct Link {
        NodeP test;
        py::list<NodeP> body;
    };
    std::vector<Link> chain;
    std::optional<py::list<NodeP>> default_;
    py::list<NodeP> pend;
    for (const NodeP& cn : or_empty(s->cases)) {
        auto c = as_a<ir::Case>(cn);
        if (!c) no_attr(cn, "labels");
        pend.extend(or_empty(c->labels));
        if (!py::truthy(c->body)) {
            if (c->default_ && *c->default_) throw Refuse(S("switch"), S("a default label sharing a section"));
            continue;
        }
        py::list<NodeP> body = py::list<NodeP>::from(*c->body);
        if (!body.empty()) {
            auto lb = as_a<ir::Break>(body[-1]);
            if (lb && !(lb->label && !lb->label->empty())) body = body.slice(0, body.size() - 1);
        }
        for (const NodeP& x : body) {
            for (const NodeP& n : ir::walk(x)) {
                if (is_a<ir::Break>(n) || is_a<ir::Continue>(n))
                    throw Refuse(S("switch"), S("a break or continue inside a string switch section"));
            }
        }
        py::list<NodeP> nb;
        for (const NodeP& x : body) nb.append(stmt(x));
        body = nb;
        if (c->default_ && *c->default_) {
            default_ = body;
        } else {
            NodeP tests;
            for (const NodeP& lab : pend) {
                NodeP lx = expr(lab);
                NodeP eq = call(v, _m(S("equals"), std::optional<str>(S("java.lang.String")), py::list<TypeP>{class_type("java.lang.Object")}, ir::BOOL),
                                py::list<NodeP>{lx}, ir::BOOL);
                if (!tests) {
                    tests = eq;
                } else {
                    auto b = mk<ir::Binary>();
                    b->op = S("||");
                    b->left = tests;
                    b->right = eq;
                    b->type = ir::BOOL;
                    tests = b;
                }
            }
            chain.push_back(Link{tests, body});
        }
        pend = py::list<NodeP>();
    }
    NodeP out;
    if (default_) {
        auto b = mk<ir::Block>();
        b->stmts = *default_;
        out = b;
    }
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        auto tb = mk<ir::Block>();
        tb->stmts = it->body;
        auto n = mk<ir::If>();
        n->cond = it->test;
        n->then = NodeP(tb);
        n->orelse = NodeP(out);
        n->line = s->line;
        n->col = s->col;
        out = n;
    }
    return out ? out : NodeP(mk<ir::Empty>());
}

NodeP ToJava::set_prop(const NodeP& en) {
    auto e = as_a<ir::Assign>(en);
    auto t = as_a<ir::Select>(e->target);
    std::optional<Accessors> acc = prop(t->sym->owner, t->sym->name);
    Key key(t->sym->owner, t->sym->name);
    SymP field = auto_.contains(key) ? auto_.at(key) : nullptr;
    if (acc && !acc->setter && field && !e->op) {
        // a get-only auto-property set in its constructor: its field
        TypeP ft = field->type;
        auto sel = mk<ir::Select>();
        sel->obj = expr(t->obj);
        sel->sym = field;
        sel->type = ft;
        NodeP value = expr(e->value);
        auto asg = mk<ir::Assign>();
        asg->op = std::nullopt;
        asg->target = sel;
        asg->value = value;
        asg->type = ft;
        return asg;
    }
    if (!acc || !acc->setter)
        throw Refuse(S("property"), py::percent(S("setting %s.%s"), t->sym->owner, t->sym->name));
    SymP g = acc->getter, s = acc->setter;
    NodeP obj = expr(t->obj);
    NodeP value = expr(e->value);
    if (e->op) {
        if (!(is_a<ir::This>(obj) || is_a<ir::TypeName>(obj) || is_a<ir::Name>(obj)))
            throw Refuse(S("property"), S("a compound assignment to a property of an expression"));
        auto cur = mk<ir::Call>();
        cur->target = obj;
        cur->sym = g;
        cur->args = py::list<NodeP>();
        cur->typeargs = no_typeargs();
        cur->type = g->ret;
        auto b = mk<ir::Binary>();
        b->op = e->op;
        b->left = cur;
        b->right = value;
        b->type = g->ret;
        value = b;
        if (g->ret && one_of(g->ret->kind, {"i8", "i16", "u16"})) {
            auto cst = mk<ir::Cast>();
            cst->x = value;
            cst->type = g->ret;
            value = cst;
        }
    }
    return call(obj, s, py::list<NodeP>{value}, ir::VOID);
}

NodeP ToJava::set_index(const NodeP& en) {
    auto e = as_a<ir::Assign>(en);
    auto t = as_a<ir::Index>(e->target);
    std::optional<str> owner = t->sym->owner;
    if (is_str(owner, "System.Collections.Generic.List")) {
        SymP s = _m(S("set"), std::optional<str>(S("java.util.ArrayList")), py::list<TypeP>{ir::I32, typevar("E")}, typevar("E"));
        NodeP target = expr(t->arr);
        if (!std::holds_alternative<NodeP>(t->index)) throw Refuse(S("list"), S("the expression list on the C++03 path"));
        NodeP idx = expr(std::get<NodeP>(t->index));
        NodeP val = expr(e->value);
        return call(target, s, py::list<NodeP>{idx, val}, ty(t->type));
    }
    throw Refuse(S("indexer"), py::percent(S("an indexer of %s"), owner));
}

// ---- expressions
NodeP ToJava::expr(const NodeP& e) {
    if (is_a<ir::Lit>(e)) return x_Lit(e);
    if (is_a<ir::Paren>(e)) return x_Paren(e);
    if (is_a<ir::Name>(e)) return x_Name(e);
    if (is_a<ir::This>(e)) return x_This(e);
    if (is_a<ir::Super>(e)) return x_Super(e);
    if (is_a<ir::TypeName>(e)) return x_TypeName(e);
    if (is_a<ir::Select>(e)) return x_Select(e);
    if (is_a<ir::Length>(e)) return x_Length(e);
    if (is_a<ir::Index>(e)) return x_Index(e);
    if (is_a<ir::Unary>(e)) return x_Unary(e);
    if (is_a<ir::Binary>(e)) return x_Binary(e);
    if (is_a<ir::Assign>(e)) return x_Assign(e);
    if (is_a<ir::Cond>(e)) return x_Cond(e);
    if (is_a<ir::Cast>(e)) return x_Cast(e);
    if (is_a<ir::InstanceOf>(e)) return x_InstanceOf(e);
    if (is_a<ir::New>(e)) return x_New(e);
    if (is_a<ir::NewArray>(e)) return x_NewArray(e);
    if (is_a<ir::Concat>(e)) return x_Concat(e);
    if (is_a<ir::Call>(e)) return x_Call(e);
    if (is_a<ir::Lambda>(e)) x_Lambda(e);
    if (is_a<ir::MethodRef>(e)) x_MethodRef(e);
    if (is_a<ir::Unsupported>(e)) x_Unsupported(e);
    str k = kind_name(e);
    throw Refuse(k, py::percent(S("the expression %s on the C++03 path"), k));
}

NodeP ToJava::_typed(const NodeP& e) {
    if (auto x = as_a<ir::Expr>(e)) {
        if (x->type) x->type = ty(x->type);
    }
    return e;
}

NodeP ToJava::x_Lit(const NodeP& e) { return _typed(e); }

NodeP ToJava::x_Paren(const NodeP& e) {
    auto p = as_a<ir::Paren>(e);
    p->x = expr(p->x);
    return _typed(e);
}

NodeP ToJava::x_Name(const NodeP& e) {
    auto n = as_a<ir::Name>(e);
    n->sym = sym(n->sym);
    return _typed(e);
}

NodeP ToJava::x_This(const NodeP& e) { return _typed(e); }
NodeP ToJava::x_Super(const NodeP& e) { return _typed(e); }
NodeP ToJava::x_TypeName(const NodeP& e) { return _typed(e); }

NodeP ToJava::call(const NodeP& target, const SymP& sym_, const py::list<NodeP>& args, const TypeP& t) {
    auto c = mk<ir::Call>();
    c->target = target;
    c->sym = sym_;
    c->args = args;
    c->typeargs = no_typeargs();
    c->type = t;
    return c;
}

NodeP ToJava::x_Select(const NodeP& en) {
    auto e = as_a<ir::Select>(en);
    SymP s = e->sym;
    if (!s) none_attr_("kind");
    if (s->kind == "property") {
        if (is_str(s->owner, "System.String") && s->name == "Length")
            return call(expr(e->obj), _m(S("length"), std::optional<str>(S("java.lang.String")), py::list<TypeP>(), ir::I32), py::list<NodeP>(), ir::I32);
        if (is_str(s->owner, "System.Collections.Generic.List") && s->name == "Count")
            return call(expr(e->obj), _m(S("size"), std::optional<str>(S("java.util.ArrayList")), py::list<TypeP>(), ir::I32), py::list<NodeP>(), ir::I32);
        if (is_str(s->owner, "System.Exception") && s->name == "Message")
            throw Refuse(S("runtime"), S("an exception's Message: the C++03 runtime's exceptions keep no message (RuntimeException(...) {})"));
        std::optional<Accessors> acc = prop(s->owner, s->name);
        if (!acc) throw Refuse(S("library"), py::percent(S("the .NET property %s.%s"), s->owner, s->name));
        SymP g = acc->getter;
        NodeP obj = expr(e->obj);
        auto c = mk<ir::Call>();
        c->target = is_a<ir::This>(obj) ? NodeP() : obj;
        c->sym = g;
        c->args = py::list<NodeP>();
        c->typeargs = no_typeargs();
        c->type = g->ret;
        c->line = e->line;
        c->col = e->col;
        return c;
    }
    if (!s->owner || !classes.contains(s->owner))
        throw Refuse(S("library"), py::percent(S("the .NET field %s.%s"), s->owner, s->name));
    e->obj = expr(e->obj);
    e->sym = sym(s);
    return _typed(en);
}

NodeP ToJava::x_Length(const NodeP& en) {
    auto e = as_a<ir::Length>(en);
    e->arr = expr(e->arr);
    return _typed(en);
}

NodeP ToJava::x_Index(const NodeP& en) {
    auto e = as_a<ir::Index>(en);
    if (e->sym) {
        if (is_str(e->sym->owner, "System.String")) {
            NodeP arr = expr(e->arr);
            if (!std::holds_alternative<NodeP>(e->index)) throw Refuse(S("list"), S("the expression list on the C++03 path"));
            NodeP idx = expr(std::get<NodeP>(e->index));
            return call(arr, _m(S("charAt"), std::optional<str>(S("java.lang.String")), py::list<TypeP>{ir::I32}, type_("u16")),
                        py::list<NodeP>{idx}, type_("u16"));
        }
        if (is_str(e->sym->owner, "System.Collections.Generic.List")) {
            TypeP t = ty(e->type);
            NodeP arr = expr(e->arr);
            if (!std::holds_alternative<NodeP>(e->index)) throw Refuse(S("list"), S("the expression list on the C++03 path"));
            NodeP idx = expr(std::get<NodeP>(e->index));
            return call(arr, _m(S("get"), std::optional<str>(S("java.util.ArrayList")), py::list<TypeP>{ir::I32}, typevar("E")),
                        py::list<NodeP>{idx}, t);
        }
        throw Refuse(S("indexer"), py::percent(S("an indexer of %s"), e->sym->owner));
    }
    if (std::holds_alternative<py::list<NodeP>>(e->index)) throw Refuse(S("array"), S("a rectangular array"));
    NodeP arr = expr(e->arr);
    NodeP idx = expr(std::get<NodeP>(e->index));
    e->arr = arr;
    e->index = NodeP(idx);
    return _typed(en);
}

NodeP ToJava::x_Unary(const NodeP& en) {
    auto e = as_a<ir::Unary>(en);
    if (is_str(e->overflow, "trap")) throw Refuse(S("checked"), S("checked arithmetic"));
    if (one_of(e->op, {"++", "--"}) && is_a<ir::Select>(e->x) && as_a<ir::Select>(e->x)->sym && as_a<ir::Select>(e->x)->sym->kind == "property")
        throw Refuse(S("property"), S("++/-- on a property inside an expression"));
    e->x = expr(e->x);
    return _typed(en);
}

NodeP ToJava::x_Binary(const NodeP& en) {
    auto e = as_a<ir::Binary>(en);
    if (is_str(e->overflow, "trap")) throw Refuse(S("checked"), S("checked arithmetic"));
    if (is_str(e->op, "??")) throw Refuse(S("coalesce"), S("??"));
    NodeP l = expr(e->left);
    NodeP r = expr(e->right);
    e->left = l;
    e->right = r;
    return _typed(en);
}

NodeP ToJava::x_Assign(const NodeP& en) {
    auto e = as_a<ir::Assign>(en);
    if (is_a<ir::Select>(e->target) && as_a<ir::Select>(e->target)->sym && as_a<ir::Select>(e->target)->sym->kind == "property")
        throw Refuse(S("property"), S("a property assigned inside an expression"));
    if (is_str(e->op, "??")) throw Refuse(S("coalesce"), S("??="));
    NodeP t = expr(e->target);
    NodeP v = expr(e->value);
    e->target = t;
    e->value = v;
    return _typed(en);
}

NodeP ToJava::x_Cond(const NodeP& en) {
    auto e = as_a<ir::Cond>(en);
    NodeP c = expr(e->cond);
    NodeP t = expr(e->then);
    NodeP o = expr(e->orelse);
    e->cond = c;
    e->then = t;
    e->orelse = o;
    return _typed(en);
}

NodeP ToJava::x_Cast(const NodeP& en) {
    auto e = as_a<ir::Cast>(en);
    if (is_str(e->overflow, "trap")) throw Refuse(S("checked"), S("a checked conversion"));
    e->x = expr(e->x);
    return _typed(en);
}

NodeP ToJava::x_InstanceOf(const NodeP& en) {
    auto e = as_a<ir::InstanceOf>(en);
    if (e->binding) throw Refuse(S("pattern"), S("a type pattern that declares a name"));
    e->x = expr(e->x);
    e->test = ty(e->test);
    return _typed(en);
}

NodeP ToJava::x_New(const NodeP& en) {
    auto e = as_a<ir::New>(en);
    TypeP t = ty(e->type);
    e->sym = sym(e->sym);
    if (e->sym) {
        if (!e->type) none_attr_("name");
        if (e->type->name && _CLASSES.contains(*e->type->name)) e->sym->owner = _CLASSES.at(*e->type->name);
    }
    py::list<NodeP> args;
    for (const NodeP& a : or_empty(e->args)) args.append(expr(a));
    e->args = args;
    e->type = t;
    return en;
}

NodeP ToJava::x_NewArray(const NodeP& en) {
    auto e = as_a<ir::NewArray>(en);
    e->elem = ty(e->elem);
    py::list<NodeP> dims;
    for (const NodeP& d : or_empty(e->dims)) dims.append(expr(d));
    e->dims = dims;
    if (e->init) {
        py::list<NodeP> init;
        for (const NodeP& x : *e->init) init.append(expr(x));
        e->init = init;
    } else {
        e->init = std::nullopt;
    }
    return _typed(en);
}

NodeP ToJava::x_Concat(const NodeP& en) {
    auto e = as_a<ir::Concat>(en);
    py::list<NodeP> parts;
    for (const NodeP& p : iter_(e->parts)) parts.append(text(p));
    e->parts = parts;
    return _typed(en);
}

NodeP ToJava::text(const NodeP& p) {
    TypeP t = type_of(p);
    if (t && t->kind == "bool") {
        auto c = mk<ir::Cond>();
        c->cond = expr(p);
        c->then = lit_str("True", STRING);
        c->orelse = lit_str("False", STRING);
        c->type = STRING;
        return c;
    }
    if (t && one_of(t->kind, {"f32", "f64"}))
        throw Refuse(S("double-text"), py::percent(S("a %s turned into text: C#'s format is not Java's"), t->kind));
    if (t && t->kind == "class" && !is_str(t->name, "System.String")) {
        if (t->name && classes.contains(t->name)) {
            SymP ts = _m(S("toString"), t->name, py::list<TypeP>(), STRING);
            return call(expr(p), ts, py::list<NodeP>(), STRING);
        }
        throw Refuse(S("object-text"), py::percent(S("a %s turned into text"), t->name));
    }
    return expr(p);
}

NodeP ToJava::x_Call(const NodeP& en) {
    auto e = as_a<ir::Call>(en);
    SymP s = e->sym;
    if (!s) none_attr_("owner");
    std::optional<str> o = s->owner;
    py::list<NodeP> args = or_empty(e->args);
    auto jstr = [&](const char* n) { return std::optional<str>(str(n)); };
    if (is_str(o, "System.Console") && one_of(s->name, {"WriteLine", "Write"})) {
        auto out = mk<ir::Select>();
        auto tn = mk<ir::TypeName>();
        tn->type = class_type("java.lang.System");
        out->obj = tn;
        out->sym = std::make_shared<ir::Sym>(S("field"), S("out"), jstr("java.lang.System"), true, class_type("java.io.PrintStream"));
        out->type = class_type("java.io.PrintStream");
        str name = s->name == "WriteLine" ? S("println") : S("print");
        if (args.empty()) return call(out, _m(name, jstr("java.io.PrintStream"), py::list<TypeP>(), ir::VOID), py::list<NodeP>(), ir::VOID);
        if (args.size() != 1) throw Refuse(S("library"), py::percent(S("Console.%s with a format"), s->name));
        NodeP a = text(args[0]);
        TypeP at = type_of(a);
        TypeP pt = at && one_of(at->kind, {"i32", "i64", "u16"}) ? at : STRING;
        return call(out, _m(name, jstr("java.io.PrintStream"), py::list<TypeP>{pt}, ir::VOID), py::list<NodeP>{a}, ir::VOID);
    }
    if (is_str(o, "System.Math") && _MATH.contains(s->name)) {
        auto tn = mk<ir::TypeName>();
        tn->type = class_type("java.lang.Math");
        py::list<TypeP> params;
        for (const TypeP& p : s->params) params.append(ty(p));
        TypeP ret = ty(s->ret);
        SymP ms = _m(_MATH.at(s->name), jstr("java.lang.Math"), params, ret, true);
        py::list<NodeP> xs;
        for (const NodeP& a : args) xs.append(expr(a));
        auto c = mk<ir::Call>();
        c->target = tn;
        c->sym = ms;
        c->args = xs;
        c->typeargs = no_typeargs();
        c->type = ty(e->type);
        return c;
    }
    if (is_str(o, "System.String")) {
        bool is_op = false;
        for (const str& f : s->flags)
            if (f == "operator") is_op = true;
        if (is_op && one_of(s->name, {"op_Equality", "op_Inequality"})) {
            if (args.size() != 2) throw py::ValueError(args.size() > 2 ? "too many values to unpack (expected 2)" : "not enough values to unpack (expected 2, got " + std::to_string(args.size()) + ")");
            NodeP l = args[0], r = args[1];
            auto nil = [](const NodeP& x) {
                auto lt = as_a<ir::Lit>(x);
                return lt && lt->value.is_none();
            };
            if (nil(l) || nil(r)) {
                auto b = mk<ir::Binary>();
                b->op = s->name == "op_Equality" ? S("==") : S("!=");
                b->left = expr(l);
                b->right = expr(r);
                b->type = ir::BOOL;
                return b;
            }
            NodeP lx = expr(l);
            NodeP rx = expr(r);
            NodeP eq = call(lx, _m(S("equals"), jstr("java.lang.String"), py::list<TypeP>{class_type("java.lang.Object")}, ir::BOOL), py::list<NodeP>{rx}, ir::BOOL);
            if (s->name == "op_Equality") return eq;
            auto u = mk<ir::Unary>();
            u->op = S("!");
            u->x = eq;
            u->postfix = false;
            u->type = ir::BOOL;
            return u;
        }
        struct Row {
            const char* name;
            int64_t n;
            const char* java;
        };
        static const Row rows[] = {{"ToUpper", 0, "toUpperCase"}, {"ToLower", 0, "toLowerCase"}, {"Contains", 1, "contains"},
                                   {"Substring", 1, "substring"}, {"Equals", 1, "equals"},       {"Trim", 0, "trim"}};
        const char* name = nullptr;
        for (const Row& r : rows)
            if (s->name == r.name && args.size() == r.n) name = r.java;
        if (!name && s->name == "Substring" && args.size() == 2 && (is_a<ir::Lit>(args[0]) || is_a<ir::Name>(args[0]))) {
            auto end = mk<ir::Binary>();
            end->op = S("+");
            end->left = expr(args[0]);
            end->right = expr(args[1]);
            end->type = ir::I32;
            NodeP target = expr(e->target);
            NodeP a0 = expr(args[0]);
            return call(target, _m(S("substring"), jstr("java.lang.String"), py::list<TypeP>{ir::I32, ir::I32}, STRING), py::list<NodeP>{a0, end}, STRING);
        }
        if (!name) throw Refuse(S("library"), py::percent(S("string.%s/%d"), s->name, args.size()));
        NodeP target = expr(e->target);
        py::list<TypeP> params;
        for (const TypeP& p : s->params) params.append(ty(p));
        TypeP ret = ty(s->ret);
        SymP ms = _m(S(name), jstr("java.lang.String"), params, ret);
        py::list<NodeP> xs;
        for (const NodeP& a : args) xs.append(expr(a));
        return call(target, ms, xs, ty(e->type));
    }
    if (is_str(o, "System.Collections.Generic.List")) {
        TypeP el = typevar("E");
        TypeP obj = class_type("java.lang.Object");
        struct Row {
            const char* name;
            int64_t n;
            const char* java;
            py::list<TypeP> params;
            TypeP ret;
        };
        std::vector<Row> rows = {{"Add", 1, "add", py::list<TypeP>{el}, ir::BOOL},
                                 {"Contains", 1, "contains", py::list<TypeP>{obj}, ir::BOOL},
                                 {"Clear", 0, "clear", py::list<TypeP>(), ir::VOID},
                                 {"Insert", 2, "add", py::list<TypeP>{ir::I32, el}, ir::VOID},
                                 {"RemoveAt", 1, "remove", py::list<TypeP>{ir::I32}, el},
                                 {"IndexOf", 1, "indexOf", py::list<TypeP>{obj}, ir::I32}};
        const Row* got = nullptr;
        for (const Row& r : rows)
            if (s->name == r.name && args.size() == r.n) got = &r;
        if (!got) throw Refuse(S("library"), py::percent(S("List.%s/%d"), s->name, args.size()));
        NodeP target = expr(e->target);
        py::list<NodeP> xs;
        for (const NodeP& a : args) xs.append(expr(a));
        TypeP rt = e->type && e->type->kind != "void" ? ty(e->type) : ir::VOID;
        return call(target, _m(S(got->java), jstr("java.util.ArrayList"), got->params, got->ret), xs, rt);
    }
    if (is_str(o, "System.Object") && s->name == "ToString" && args.empty())
        return call(expr(e->target), _m(S("toString"), jstr("java.lang.Object"), py::list<TypeP>(), STRING), py::list<NodeP>(), STRING);
    if (!o || !classes.contains(o)) throw Refuse(S("library"), py::percent(S("the .NET call %s.%s"), o, s->name));
    for (const str& f : s->flags)
        if (one_of(f, {"operator", "extension", "delegate"})) throw Refuse(S("call"), S("an operator, extension or delegate call"));
    e->sym = sym(s);
    {
        static const py::dict<str, str> rename = {{S("ToString"), S("toString")}, {S("Equals"), S("equals")}, {S("GetHashCode"), S("hashCode")}};
        e->sym->name = rename.contains(s->name) ? rename.at(s->name) : s->name;
    }
    if (e->target) e->target = expr(e->target);
    py::list<NodeP> xs;
    for (const NodeP& a : args) xs.append(expr(a));
    e->args = xs;
    return _typed(en);
}

NodeP ToJava::x_Lambda(const NodeP&) { throw Refuse(S("lambda"), S("a lambda: the C++03 back end has none")); }
NodeP ToJava::x_MethodRef(const NodeP&) { throw Refuse(S("delegate"), S("a method group as a delegate")); }
NodeP ToJava::x_Unsupported(const NodeP& en) {
    auto e = as_a<ir::Unsupported>(en);
    str raw = e->raw && !e->raw->empty() ? *e->raw : (e->kind ? *e->kind : S("None"));
    throw Refuse(e->kind ? *e->kind : S("None"), raw);
}

// ---------------------------------------------------------------------------------------------- Java text
str _jtype(const TypeP& t) {
    if (!t) return S("void");
    if (_PRIM_JAVA.contains(t->kind)) return _PRIM_JAVA.at(t->kind);
    if (t->kind == "array") return _jtype(t->elem) + S("[]");
    if (t->kind == "class") {
        str n = last_part(name_of(t));
        if (!t->args.empty()) {
            py::list<str> as;
            for (const TypeP& a : t->args) as.append(_jtype(a));
            n = n + S("<") + py::join(S(", "), as) + S(">");
        }
        return n;
    }
    if (t->kind == "typevar") {
        if (!t->name) none_attr_("name");
        return *t->name;
    }
    return S("Object");
}

str _default(const TypeP& t) {
    if (!t || t->kind == "void") return S("");
    if (t->kind == "bool") return S(" false");
    if (_PRIM_JAVA.contains(t->kind)) return S(" 0");
    return S(" null");
}

str skeleton(const NodeP& cn) {
    auto c = class_of(cn);
    if (!c->qname) none_attr_("endswith");
    if (!c->name) throw py::TypeError("object of type 'NoneType' has no len()");
    str pkg;
    if (c->qname->endswith(*c->name)) {
        int64_t n = c->name->size();
        pkg = (n == 0 ? c->qname->slice(0, 0) : c->qname->slice(0, c->qname->size() - n)).rstrip(S("."));
    }
    py::list<str> lines;
    if (!pkg.empty()) lines.append(S("package ") + pkg + S(";"));
    lines.append(S("import java.util.ArrayList;"));
    lines.append(S(""));
    py::list<str> hp = py::list<str>::from(iter_(c->mods));
    hp.append(is_str(c->kind, "interface") ? S("interface") : S("class"));
    hp.append(*c->name);
    str head = join_mods(hp);
    if (c->extends) head = head + S(" extends ") + _jtype(c->extends);
    if (py::truthy(c->implements)) {
        py::list<str> is;
        for (const TypeP& i : *c->implements) is.append(_jtype(i));
        head = head + (is_str(c->kind, "interface") ? S(" extends ") : S(" implements ")) + py::join(S(", "), is);
    }
    lines.append(head + S(" {"));
    for (const NodeP& m : iter_(c->members)) {
        if (auto f = as_a<ir::FieldDecl>(m)) {
            lines.append(py::percent(S("\t%s %s %s;"), join_mods(or_empty(f->mods)), _jtype(f->type), f->sym->name));
        } else if (auto md = as_a<ir::MethodDecl>(m)) {
            py::list<str> ps;
            for (const NodeP& pn : or_empty(md->params)) {
                auto p = as_a<ir::Param>(pn);
                ps.append(py::percent(S("%s %s"), _jtype(p->type), p->sym->name));
            }
            str pss = py::join(S(", "), ps);
            py::list<str> mods_;
            for (const str& x : iter_(md->mods))
                if (x != "IMPLICIT") mods_.append(x);
            str ms = join_mods(mods_);
            if (is_str(md->kind, "ctor")) {
                bool implicit = false;
                for (const str& x : or_empty(md->mods))
                    if (x == "IMPLICIT") implicit = true;
                if (implicit) continue;
                lines.append(py::percent(S("\t%s %s(%s) {\n\t}"), ms, *c->name, pss));
            } else if (is_str(md->kind, "static_init")) {
                lines.append(S("\tstatic {\n\t}"));
            } else if (body_is_none(md->body)) {
                lines.append(py::percent(S("\t%s %s %s(%s);"), ms, _jtype(md->ret), md->name, pss));
            } else {
                str ret = _default(md->ret);
                lines.append(py::percent(S("\t%s %s %s(%s) {\n\t\treturn%s;\n\t}"), ms, _jtype(md->ret), md->name, pss, ret));
            }
        }
    }
    lines.append(S("}"));
    py::list<str> fixed;
    for (const str& l : lines) fixed.append(l.replace(S("\t "), S("\t")));
    return py::join(S("\n"), fixed) + S("\n");
}

// ---------------------------------------------------------------------------------------------- the conversion
std::tuple<str, py::list<Refusal>, java::transpile::Summary> convert(const py::list<NodeP>& units, const str& work, const Transpiler& transpiler) {
    ToJava tj(units);
    py::list<Refusal> refused = tj.run();
    str src = py::os::path::join(work, S("java-skeleton"));
    str out = py::os::path::join(work, S("cpp"));
    for (const str& d : {src, out}) {
        py::shutil::rmtree(d, true);
        py::os::makedirs(d);
    }
    for (const NodeP& un : units) {
        auto u = as_a<ir::Unit>(un);
        for (const NodeP& c : iter_(u->types)) {
            auto f = py::open(py::os::path::join(src, *class_of(c)->name + S(".java")), S("w"), std::optional<str>(S("utf-8")));
            f.write(skeleton(c));
            f.close();
        }
    }
    Transpiled got = transpiler(src, out, units);   // (the C# kit's L2, not javac's: see the header)
    if (got.has_tree) {
        for (const auto& kv : got.reasons.items())
            refused.append(Refusal(S("C++03 back end"), S("declined"), py::percent(S("%d bodies: %s"), std::get<1>(kv), std::get<0>(kv))));
        for (const auto& kv : got.declined.items()) {
            if (std::get<0>(kv) == "not found" || !py::truthy(got.reasons))
                refused.append(Refusal(S("C++03 back end"), S("declined"), py::percent(S("%d bodies: %s"), std::get<1>(kv), std::get<0>(kv))));
        }
    }
    _host_print(out);
    return std::make_tuple(out, refused, got.stats);
}

bool _host_print(const str& cpp_dir) {
    str p = py::os::path::join(cpp_dir, S("JavaRT.h"));
    str text;
    {
        auto f = py::open(p, S("r"), std::optional<str>(S("utf-8")));
        text = f.read_text();
        f.close();
    }
    // (written with explicit "\n" and "\t": the same text whatever line endings this source file is checked out with)
    str old = S("\tstruct _PrintStream {\n"
                "\t\ttemplate<class T> void println(const T&) {}\n"
                "\t\tvoid println() {}\n"
                "\t\ttemplate<class T> void print(const T&) {}\n"
                "\t};\n"
                "\tstatic _PrintStream out;\n"
                "\tstatic _PrintStream err;");
    str neu = S("\tstruct _PrintStream {                     // (the judge's host PrintStream: CodeBreaker's C# kit)\n"
                "\t\tFILE* f;\n"
                "\t\texplicit _PrintStream(FILE* to) : f(to) {}\n"
                "\t\tvoid put(const std::string& s) { fwrite(s.data(), 1, s.size(), f); }\n"
                "\t\ttemplate<class T> void print(const T& x) { std::ostringstream o; o << x; put(o.str()); }\n"
                "\t\tvoid print(bool b) { put(b ? \"true\" : \"false\"); }\n"
                "\t\tvoid print(char c) { put(std::string(1, c)); }\n"
                "\t\tvoid print(double d) { put(j2xNumberText(d, false)); }\n"
                "\t\tvoid print(float d) { put(j2xNumberText(d, true)); }\n"
                "\t\ttemplate<class T> void println(const T& x) { print(x); put(\"\\n\"); }\n"
                "\t\tvoid println() { put(\"\\n\"); }\n"
                "\t};\n"
                "\tstatic _PrintStream out(stdout);\n"
                "\tstatic _PrintStream err(stderr);");
    if (text.contains(old)) {
        text = text.replace(old, neu);
        if (!text.contains(S("#include <sstream>"))) text = S("#include <sstream>\n#include <cstdio>\n") + text;
        auto f = py::open(p, S("w"), std::optional<str>(S("utf-8")));
        f.write(text);
        f.close();
        return true;
    }
    return false;
}

}  // namespace codebreaker::translate::dotnet::cs2cpp

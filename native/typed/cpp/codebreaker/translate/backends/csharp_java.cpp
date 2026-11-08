// codebreaker/translate/backends/csharp_java.cpp -- typed C++ port of codebreaker.translate.backends.csharp_java
// (see csharp_java.hpp).
#include "codebreaker/translate/backends/csharp_java.hpp"

#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace codebreaker::translate::backends::csharp {

using ir::NodeOrList;
using ir::Nodes;
using py::str;
using ClassP = JavaLib::ClassP;
using MethodP = JavaLib::MethodP;

namespace {

str S(const char* s) { return str(s); }
template <class T> T* as(const NodeP& n) { return dynamic_cast<T*>(n.get()); }
template <class T> py::ref<T> ref_as(const NodeP& n) { return std::dynamic_pointer_cast<T>(n); }
template <class T> bool is_a(const NodeP& n) { return dynamic_cast<const T*>(n.get()) != nullptr; }

str or_empty(const std::optional<str>& a) { return a ? *a : str(); }
const py::list<NodeP>& nodes_of(const Nodes& v) {
    static const py::list<NodeP> none;
    return v ? *v : none;
}
py::list<str> strs_of(const ir::Strs& v) { return v ? *v : py::list<str>(); }
bool in_strs(const ir::Strs& v, const char* x) { return v && v->contains(S(x)); }
bool nonempty(const Nodes& v) { return v && !v->empty(); }
str jn(const str& sep, const py::list<str>& parts) { return py::join(sep, parts); }

str mods_text(const py::list<str>& mods) {
    str out;
    for (const str& m : mods) out += m + S(" ");
    return out;
}

TypeP type_of(const NodeP& e) {
    if (!e) throw py::AttributeError("'NoneType' object has no attribute 'type'");
    if (auto x = as<ir::Expr>(e)) return x->type;
    if (auto l = as<ir::Local>(e)) return l->type;
    throw py::AttributeError("'" + e->class_name().utf8() + "' object has no attribute 'type'");
}

bool has_body(const py::ref<ir::MethodDecl>& m) {   // `m.body is not None`
    return std::holds_alternative<py::list<NodeP>>(m->body) || std::get<NodeP>(m->body) != nullptr;
}

// the qualified name of a class Type, else None
std::optional<str> qual(const TypeP& t) {
    if (t && t->kind == "class") return t->name;
    return std::nullopt;
}
bool starts_java(const std::optional<str>& q) {
    return q && (q->startswith(S("java.")) || q->startswith(S("javax.")));
}

using SMap = std::unordered_map<std::string, const char*>;

const SMap& TYPES() {
    static const SMap m = {
        {"java.lang.String", "string"}, {"java.lang.Object", "object"}, {"java.lang.Integer", "int"}, {"java.lang.Long", "long"},
        {"java.lang.Short", "short"}, {"java.lang.Byte", "sbyte"}, {"java.lang.Character", "char"}, {"java.lang.Boolean", "bool"},
        {"java.lang.Double", "double"}, {"java.lang.Float", "float"}, {"java.lang.Number", "object"},
        {"java.lang.CharSequence", "string"}, {"java.lang.StringBuilder", "System.Text.StringBuilder"},
        {"java.lang.StringBuffer", "System.Text.StringBuilder"}, {"java.lang.Math", "Math"}, {"java.lang.StrictMath", "Math"},
        {"java.lang.System", "Console"}, {"java.lang.Throwable", "Exception"}, {"java.lang.Exception", "Exception"},
        {"java.lang.RuntimeException", "Exception"}, {"java.lang.Error", "Exception"},
        {"java.lang.ArithmeticException", "ArithmeticException"}, {"java.lang.NullPointerException", "NullReferenceException"},
        {"java.lang.ArrayIndexOutOfBoundsException", "IndexOutOfRangeException"},
        {"java.lang.IndexOutOfBoundsException", "ArgumentOutOfRangeException"},
        {"java.lang.StringIndexOutOfBoundsException", "ArgumentOutOfRangeException"},
        {"java.lang.IllegalArgumentException", "ArgumentException"}, {"java.lang.IllegalStateException", "InvalidOperationException"},
        {"java.lang.UnsupportedOperationException", "NotSupportedException"}, {"java.lang.NumberFormatException", "FormatException"},
        {"java.lang.ClassCastException", "InvalidCastException"}, {"java.lang.NegativeArraySizeException", "OverflowException"},
        {"java.util.NoSuchElementException", "InvalidOperationException"},
        {"java.lang.Comparable", "IComparable"}, {"java.lang.Iterable", "IEnumerable"}, {"java.lang.AutoCloseable", "IDisposable"},
        {"java.io.Closeable", "IDisposable"}, {"java.lang.Runnable", "Action"},
        {"java.util.Random", "JavaRandom"}};
    return m;
}
const SMap& GENERIC() {
    static const SMap m = {
        {"java.util.List", "List"}, {"java.util.ArrayList", "List"}, {"java.util.LinkedList", "List"}, {"java.util.Vector", "List"},
        {"java.util.Collection", "ICollection"}, {"java.util.AbstractList", "List"},
        {"java.util.Map", "IDictionary"}, {"java.util.HashMap", "Dictionary"}, {"java.util.LinkedHashMap", "Dictionary"},
        {"java.util.Hashtable", "Dictionary"}, {"java.util.TreeMap", "SortedDictionary"}, {"java.util.SortedMap", "IDictionary"},
        {"java.util.Set", "ISet"}, {"java.util.HashSet", "HashSet"}, {"java.util.LinkedHashSet", "HashSet"},
        {"java.util.TreeSet", "SortedSet"}, {"java.util.SortedSet", "ISet"},
        {"java.util.Map.Entry", "KeyValuePair"}};
    return m;
}
int64_t arity(const std::string& g) {
    static const std::unordered_map<std::string, int64_t> m = {
        {"List", 1}, {"Dictionary", 2}, {"SortedDictionary", 2}, {"HashSet", 1}, {"SortedSet", 1}, {"KeyValuePair", 2},
        {"IDictionary", 2}, {"ISet", 1}, {"ICollection", 1}};
    return m.at(g);
}
bool ordered_class(const std::optional<str>& q) {
    if (!q) return false;
    return *q == "java.util.TreeMap" || *q == "java.util.LinkedHashMap" || *q == "java.util.TreeSet" || *q == "java.util.LinkedHashSet";
}

// functional interfaces -> delegates: (C# generic, how the interface's own type arguments become it)
enum class FnMode { Same, Plus_bool, Unary, Binary, IntBin, IntUn, IntPred, IntFn, ToInt, IntSup, DblUn };
struct FnSpec {
    const char* gen;
    FnMode mode;
};
const std::unordered_map<std::string, FnSpec>& FUNCTIONS() {
    static const std::unordered_map<std::string, FnSpec> m = {
        {"java.util.function.Function", {"Func", FnMode::Same}}, {"java.util.function.BiFunction", {"Func", FnMode::Same}},
        {"java.util.function.Supplier", {"Func", FnMode::Same}}, {"java.util.function.Consumer", {"Action", FnMode::Same}},
        {"java.util.function.BiConsumer", {"Action", FnMode::Same}},
        {"java.util.function.Predicate", {"Func", FnMode::Plus_bool}}, {"java.util.function.BiPredicate", {"Func", FnMode::Plus_bool}},
        {"java.util.function.UnaryOperator", {"Func", FnMode::Unary}}, {"java.util.function.BinaryOperator", {"Func", FnMode::Binary}},
        {"java.util.function.IntBinaryOperator", {"Func", FnMode::IntBin}}, {"java.util.function.IntUnaryOperator", {"Func", FnMode::IntUn}},
        {"java.util.function.IntPredicate", {"Func", FnMode::IntPred}}, {"java.util.function.IntFunction", {"Func", FnMode::IntFn}},
        {"java.util.function.ToIntFunction", {"Func", FnMode::ToInt}}, {"java.util.function.IntSupplier", {"Func", FnMode::IntSup}},
        {"java.util.function.DoubleUnaryOperator", {"Func", FnMode::DblUn}}, {"java.util.Comparator", {"Comparison", FnMode::Same}}};
    return m;
}
py::list<str> fn_args(FnMode mode, const py::list<str>& a) {
    py::list<str> out;
    switch (mode) {
        case FnMode::Same: return a;
        case FnMode::Plus_bool: out = a.copy(); out.append(S("bool")); return out;
        case FnMode::Unary: out.append(a[0]); out.append(a[0]); return out;
        case FnMode::Binary: out.append(a[0]); out.append(a[0]); out.append(a[0]); return out;
        case FnMode::IntBin: out.append(S("int")); out.append(S("int")); out.append(S("int")); return out;
        case FnMode::IntUn: out.append(S("int")); out.append(S("int")); return out;
        case FnMode::IntPred: out.append(S("int")); out.append(S("bool")); return out;
        case FnMode::IntFn: out.append(S("int")); out.extend(a); return out;
        case FnMode::ToInt: out = a.copy(); out.append(S("int")); return out;
        case FnMode::IntSup: out.append(S("int")); return out;
        case FnMode::DblUn: out.append(S("double")); out.append(S("double")); return out;
    }
    return out;
}

// a library class's family: the classes that answer a call the same way (the map's keys use the family)
const std::unordered_map<std::string, std::string>& FAMILY() {
    static const std::unordered_map<std::string, std::string> m = [] {
        std::unordered_map<std::string, std::string> r = {
            {"java.lang.String", "String"}, {"java.lang.StringBuilder", "StringBuilder"}, {"java.lang.StringBuffer", "StringBuilder"},
            {"java.lang.AbstractStringBuilder", "StringBuilder"}, {"java.lang.Object", "Object"}, {"java.lang.Math", "Math"},
            {"java.lang.StrictMath", "Math"}, {"java.lang.Integer", "Integer"}, {"java.lang.Long", "Long"}, {"java.lang.Double", "Double"},
            {"java.lang.Float", "Float"}, {"java.lang.Boolean", "Boolean"}, {"java.lang.Character", "Character"},
            {"java.lang.System", "System"}, {"java.lang.Thread", "Thread"}, {"java.io.PrintStream", "PrintStream"},
            {"java.util.Arrays", "Arrays"}, {"java.util.Collections", "Collections"}, {"java.util.Random", "Random"},
            {"java.lang.Throwable", "Throwable"}, {"java.lang.Exception", "Throwable"}, {"java.lang.RuntimeException", "Throwable"},
            {"java.util.Map.Entry", "Entry"}, {"java.lang.Number", "Integer"}, {"java.lang.CharSequence", "String"},
            {"java.lang.Enum", "Enum"}};
        for (const char* n : {"List", "ArrayList", "LinkedList", "Vector", "Collection", "AbstractList", "AbstractCollection", "Iterable",
                              "AbstractSequentialList"})
            r[std::string("java.util.") + n] = "List";
        r["java.lang.Iterable"] = "List";
        for (const char* n : {"Map", "HashMap", "LinkedHashMap", "TreeMap", "AbstractMap", "SortedMap", "Hashtable", "NavigableMap"})
            r[std::string("java.util.") + n] = "Map";
        for (const char* n : {"Set", "HashSet", "LinkedHashSet", "TreeSet", "AbstractSet", "SortedSet", "NavigableSet"})
            r[std::string("java.util.") + n] = "Set";
        for (auto& kv : FUNCTIONS()) r[kv.first] = kv.first.substr(kv.first.rfind('.') + 1);
        r["java.lang.Runnable"] = "Runnable";
        return r;
    }();
    return m;
}

const std::map<std::pair<std::string, std::string>, const char*>& FIELDS() {
    static const std::map<std::pair<std::string, std::string>, const char*> m = {
        {{"java.lang.Integer", "MAX_VALUE"}, "int.MaxValue"}, {{"java.lang.Integer", "MIN_VALUE"}, "int.MinValue"},
        {{"java.lang.Long", "MAX_VALUE"}, "long.MaxValue"}, {{"java.lang.Long", "MIN_VALUE"}, "long.MinValue"},
        {{"java.lang.Short", "MAX_VALUE"}, "short.MaxValue"}, {{"java.lang.Short", "MIN_VALUE"}, "short.MinValue"},
        {{"java.lang.Byte", "MAX_VALUE"}, "sbyte.MaxValue"}, {{"java.lang.Byte", "MIN_VALUE"}, "sbyte.MinValue"},
        {{"java.lang.Character", "MAX_VALUE"}, "char.MaxValue"}, {{"java.lang.Character", "MIN_VALUE"}, "char.MinValue"},
        {{"java.lang.Double", "MAX_VALUE"}, "double.MaxValue"}, {{"java.lang.Double", "MIN_VALUE"}, "double.Epsilon"},
        {{"java.lang.Double", "POSITIVE_INFINITY"}, "double.PositiveInfinity"},
        {{"java.lang.Double", "NEGATIVE_INFINITY"}, "double.NegativeInfinity"}, {{"java.lang.Double", "NaN"}, "double.NaN"},
        {{"java.lang.Float", "MAX_VALUE"}, "float.MaxValue"}, {{"java.lang.Float", "MIN_VALUE"}, "float.Epsilon"},
        {{"java.lang.Float", "NaN"}, "float.NaN"}, {{"java.lang.Math", "PI"}, "Math.PI"}, {{"java.lang.Math", "E"}, "Math.E"}};
    return m;
}

// (name, argument count) -> the .NET method
const char* object_method(const str& name, int64_t n) {
    if (n == 0 && name == "toString") return "ToString";
    if (n == 1 && name == "equals") return "Equals";
    if (n == 0 && name == "hashCode") return "GetHashCode";
    return nullptr;
}

const char* param_kind(const str& k) {
    static const std::unordered_map<std::string, const char*> m = {{"i32", "int"}, {"i64", "long"}, {"i16", "int"}, {"i8", "int"},
                                                                   {"u16", "int"}, {"f32", "float"}, {"f64", "double"}, {"bool", "boolean"}};
    auto it = m.find(k.utf8());
    return it == m.end() ? nullptr : it->second;
}

std::shared_ptr<java::apimap::Registry> registry() {
    static std::shared_ptr<java::apimap::Registry> reg;
    if (!reg) reg = java::apimap::load();
    return reg;
}

// Is this string expression never null? (a literal, a concatenation, a library call that gives text)
bool non_null(const NodeP& e) {
    if (is_a<ir::Concat>(e)) return true;
    if (auto l = as<ir::Lit>(e))
        if (std::holds_alternative<str>(l->value.v)) return true;
    if (auto c = as<ir::Call>(e)) {
        if (!c->sym || !c->sym->owner) return false;
        const str& o = *c->sym->owner;
        if (o == "java.lang.String" || o == "java.lang.StringBuilder" || o == "java.lang.Integer" || o == "java.lang.Long" ||
            o == "java.lang.Double" || o == "java.lang.Float") {
            static const char* names[] = {"toString", "substring", "trim", "toUpperCase", "toLowerCase", "replace", "valueOf",
                                          "concat", "repeat", "join", "strip", "intern"};
            for (const char* n : names)
                if (c->sym->name == n) return true;
        }
    }
    return false;
}

std::string concrete_key(const std::optional<str>& owner, const str& name) {
    return (owner ? "S" + owner->utf8() : std::string("N")) + "\n" + name.utf8();
}

}  // namespace

// -------------------------------------------------------------------------------------- JavaLib
JavaLib::JavaLib(Writer& w_) : w(w_), prog(w_.prog) {}

// ---- types
std::optional<str> JavaLib::type_name(const TypeP& t) {
    str q = or_empty(t->name);
    const std::string qs = q.utf8();
    if (prog.classes.contains(q)) return std::nullopt;
    auto fit = FUNCTIONS().find(qs);
    if (fit != FUNCTIONS().end()) {
        py::list<str> cargs;
        for (const TypeP& a : t->args) cargs.append(w.ctype(a));
        py::list<str> args = fn_args(fit->second.mode, cargs);
        std::string gen = fit->second.gen;
        if (gen == "Action" && args.empty()) return S("Action");
        w.usings.add(S("System"));
        return S(gen.c_str()) + S("<") + jn(S(", "), args) + S(">");
    }
    auto git = GENERIC().find(qs);
    if (git != GENERIC().end()) {
        std::string gen = git->second;
        w.usings.add(S("System.Collections.Generic"));
        int64_t n = arity(gen);
        py::list<str> args;
        for (const TypeP& a : t->args) args.append(w.ctype(a));
        if (args.empty())
            for (int64_t i = 0; i < n; ++i) args.append(S("object"));
        if (args.size() != n) throw Decline(S("type"), S("the type ") + t->repr());
        return S(gen.c_str()) + S("<") + jn(S(", "), args) + S(">");
    }
    auto tit = TYPES().find(qs);
    if (tit != TYPES().end()) {
        std::string name = tit->second;
        if (name == "JavaRandom") prog.runtime.add(S("JavaRt"));
        if ((name == "IComparable" || name == "IEnumerable") && !t->args.empty()) {
            if (name == "IEnumerable") w.usings.add(S("System.Collections.Generic"));
            py::list<str> as_;
            for (const TypeP& a : t->args) as_.append(w.ctype(a));
            return S(name.c_str()) + S("<") + jn(S(", "), as_) + S(">");
        }
        if (name.find('.') == std::string::npos && !name.empty() && name[0] >= 'A' && name[0] <= 'Z') w.usings.add(S("System"));
        return S(name.c_str());
    }
    if (q.startswith(S("java.")) || q.startswith(S("javax."))) throw Decline(S("library"), S("the Java library class ") + q + S(" has no C# mapping"));
    return std::nullopt;
}

bool JavaLib::keep_interface(const TypeP& t) {
    auto q = qual(t);
    if (q && (*q == "java.io.Serializable" || *q == "java.lang.Cloneable" || *q == "java.util.RandomAccess")) return false;
    return true;
}

str JavaLib::exception(const TypeP& t) { return w.ctype(t); }

// ---- declarations
void JavaLib::check_class(const ClassP& c) {
    if (c->kind && *c->kind == "class" && c->outer && !c->outer->empty() && !in_strs(c->mods, "static")) {
        ClassP outer = prog.classes.get(*c->outer);
        if (outer && outer->kind && *outer->kind == "class") {
            for (const NodeP& n : ir::walk(c)) {
                if (auto th = as<ir::This>(n)) {
                    if (th->type && th->type->name && outer->qname && *th->type->name == *outer->qname)
                        throw Decline(S("inner-class"), S("an inner class that uses its outer instance (") + or_empty(c->name) + S(")"));
                }
                if (auto cl = as<ir::Call>(n)) {
                    if (!cl->target && cl->sym && cl->sym->owner && outer->qname && *cl->sym->owner == *outer->qname)
                        throw Decline(S("inner-class"), S("an inner class that calls its outer instance (") + or_empty(c->name) + S(")"));
                }
            }
        }
    }
    if (c->kind && *c->kind == "enum") {
        for (const NodeP& m : nodes_of(c->members)) {
            bool bad = is_a<ir::FieldDecl>(m);
            if (!bad)
                if (auto md = as<ir::MethodDecl>(m)) bad = !in_strs(md->mods, "IMPLICIT");
            if (!bad)
                if (auto ec = as<ir::EnumConst>(m)) bad = nonempty(ec->args) || ec->body;
            if (bad) throw Decline(S("enum"), S("a Java enum with fields, methods or constructor arguments (an enum of objects)"));
        }
    }
}

py::list<str> JavaLib::class_mods(const ClassP& c) {
    py::list<str> out;
    for (const str& m : strs_of(c->mods)) {
        if (m == "public" || m == "protected" || m == "private" || m == "abstract") out.append(m);
        else if (m == "final" && c->kind && *c->kind == "class") out.append(S("sealed"));
    }
    if (!c->outer && !out.contains(S("public"))) out.insert(0, S("internal"));
    return out;
}

py::list<str> JavaLib::access_(const py::list<str>& mods, const ClassP& c) const {
    py::list<str> acc;
    for (const str& m : mods)
        if (m == "public" || m == "protected" || m == "private") acc.append(m);
    if (acc.size() == 1 && acc[0] == "private" && c->outer && !c->outer->empty()) {
        acc = py::list<str>();
        acc.append(S("internal"));   // Java's private reaches the whole top-level class
    }
    if (acc.empty() && !(c->kind && *c->kind == "interface")) acc.append(S("internal"));
    if (acc.size() == 1 && acc[0] == "protected") {
        acc = py::list<str>();
        acc.append(S("protected"));   // Java's protected is package-visible too
        acc.append(S("internal"));
    }
    return acc;
}

bool JavaLib::access_const_(const py::ref<ir::FieldDecl>& f) const {
    NodeP init = f->init ? strip(f->init) : NodeP();
    auto l = as<ir::Lit>(init);
    return l && !l->value.is_none() && f->type && (f->type->is_prim() || is_string(f->type));
}

py::list<str> JavaLib::field_mods(const py::ref<ir::FieldDecl>& f, const ClassP& c) {
    py::list<str> mods = strs_of(f->mods);
    py::list<str> out;
    if (c->kind && *c->kind == "interface") {
        if (access_const_(f)) {
            out.append(S("public"));
            out.append(S("const"));
        } else {
            out.append(S("public"));
            out.append(S("static"));
            out.append(S("readonly"));
        }
        return out;
    }
    out = access_(mods, c);
    bool is_static = mods.contains(S("static")), is_final = mods.contains(S("final"));
    if (is_static && is_final && access_const_(f)) {
        out.append(S("const"));
        return out;
    }
    if (is_static) out.append(S("static"));
    NodeP init = f->init;
    auto it = prog.inits.find(static_cast<const ir::Node*>(f.get()));
    if (it != prog.inits.end()) init = it->second;
    if (is_final && init && !prog.java_moved.count(static_cast<const ir::Node*>(f.get()))) out.append(S("readonly"));
    if (mods.contains(S("volatile"))) out.append(S("volatile"));
    return out;
}

py::list<str> JavaLib::method_mods(const MethodP& m, const ClassP& c) {
    py::list<str> mods = strs_of(m->mods);
    py::list<str> out;
    if (c->kind && *c->kind == "interface") {
        if (mods.contains(S("static"))) out.append(S("static"));
        return out;
    }
    out = access_(mods, c);
    if (m->kind && *m->kind == "ctor") return out;
    if (mods.contains(S("static"))) {
        out.append(S("static"));
        return out;
    }
    if (mods.contains(S("abstract"))) {
        out.append(S("abstract"));
        if (overrides(c, m)) out.append(S("override"));
        return out;
    }
    if (overrides(c, m)) {
        out.append(S("override"));
        if (mods.contains(S("final"))) out.append(S("sealed"));
        return out;
    }
    if (mods.contains(S("private")) || mods.contains(S("final")) || in_strs(c->mods, "final")) return out;
    out.append(S("virtual"));
    return out;
}

bool JavaLib::overrides(const ClassP& c, const MethodP& m) {
    if (object_method(or_empty(m->name), py::len(nodes_of(m->params)))) return true;
    return prog.overrides(c, m);
}

str JavaLib::method_name(const MethodP& m, const ClassP& c) {
    int64_t n = py::len(nodes_of(m->params));
    str name = or_empty(m->name);
    if (const char* om = object_method(name, n)) return S(om);
    if (name == "main" && in_strs(m->mods, "static") && n == 1) return S("Main");
    if (name == "compareTo" && n == 1 && c->implements) {
        for (const TypeP& i : *c->implements) {
            auto q = qual(i);
            if (q && *q == "java.lang.Comparable") return S("CompareTo");
        }
    }
    if (name == "close" && n == 0 && closeable_(c)) return S("Dispose");
    if (c->name && name == *c->name) return ident(name) + S("_");   // (C# forbids a member named as its class)
    return ident(name);
}

bool JavaLib::closeable_(const ClassP& c0) const {
    std::set<std::string> seen;
    ClassP c = c0;
    while (c && !seen.count(c->qname->utf8())) {
        seen.insert(c->qname->utf8());
        if (c->implements)
            for (const TypeP& i : *c->implements) {
                auto q = qual(i);
                if (q && (*q == "java.lang.AutoCloseable" || *q == "java.io.Closeable")) return true;
            }
        c = prog.base_of(c);
    }
    return false;
}

str JavaLib::member_name(const SymP& s) {
    if (s->kind == "method") {
        int64_t n = py::len(s->params);
        if (const char* om = object_method(s->name, n)) return S(om);
        ClassP c = s->owner ? prog.classes.get(*s->owner) : nullptr;
        if (c) {
            if (s->name == "compareTo" && n == 1) return S("CompareTo");
            if (s->name == "close" && n == 0 && closeable_(c)) return S("Dispose");
            if (c->name && s->name == *c->name) return ident(s->name) + S("_");
            if (s->name == "main" && s->static_ && n == 1) return S("Main");
        }
    }
    return ident(s->name);
}

py::list<NodeP> JavaLib::arrange_members(const ClassP& c, const py::list<NodeP>& members) {
    py::list<NodeP> out;
    std::vector<std::pair<bool, NodeP>> statics;   // (is a field, the member)
    bool has_block = false;
    std::vector<std::pair<bool, NodeP>> moves;
    bool base_is_ours = c->extends && c->extends->kind == "class" && c->extends->name && prog.classes.contains(*c->extends->name);
    for (const NodeP& m : members) {
        auto md = as<ir::MethodDecl>(m);
        if (md && md->kind && *md->kind == "static_init") {
            has_block = true;
            statics.emplace_back(false, m);
            continue;
        }
        auto fd = ref_as<ir::FieldDecl>(m);
        if (fd && in_strs(fd->mods, "static") && fd->init && !access_const_(fd)) statics.emplace_back(true, m);
        if (fd && !in_strs(fd->mods, "static") && fd->init && base_is_ours) moves.emplace_back(true, m);
        if (md && md->kind && *md->kind == "init") moves.emplace_back(false, m);
        out.append(m);
    }
    auto assign_of = [&](const py::ref<ir::FieldDecl>& m, const NodeP& obj) {
        auto sel = std::make_shared<ir::Select>();
        sel->obj = obj;
        sel->sym = m->sym;
        sel->type = m->type;
        auto asg = std::make_shared<ir::Assign>();
        asg->target = sel;
        asg->value = m->init;
        asg->type = m->type;
        auto es = std::make_shared<ir::ExprStmt>();
        es->expr = asg;
        return NodeP(es);
    };
    if (has_block) {
        py::list<NodeP> body;
        for (auto& kv : statics) {
            if (kv.first) {
                auto m = ref_as<ir::FieldDecl>(kv.second);
                prog.inits[static_cast<const ir::Node*>(m.get())] = nullptr;
                prog.java_moved.insert(static_cast<const ir::Node*>(m.get()));
                auto tn = std::make_shared<ir::TypeName>();
                tn->type = std::make_shared<ir::Type>(S("class"), c->qname);
                body.append(assign_of(m, tn));
            } else {
                auto m = as<ir::MethodDecl>(kv.second);
                auto blk = std::holds_alternative<NodeP>(m->body) ? as<ir::Block>(std::get<NodeP>(m->body)) : nullptr;
                if (!blk) throw py::AttributeError("'list' object has no attribute 'stmts'");
                body.extend(nodes_of(blk->stmts));
            }
        }
        auto md = std::make_shared<ir::MethodDecl>();
        md->name = S("<clinit>");
        md->kind = S("cctor");
        md->params = py::list<NodeP>();
        md->ret = ir::VOID;
        auto blk = std::make_shared<ir::Block>();
        blk->stmts = body;
        md->body = NodeP(blk);
        py::list<str> mods;
        mods.append(S("static"));
        md->mods = mods;
        md->throws = py::list<TypeP>();
        md->typeparams = py::list<str>();
        out.append(md);
    }
    if (!moves.empty()) {
        py::list<NodeP> pro;
        for (auto& kv : moves) {
            if (kv.first) {
                auto m = ref_as<ir::FieldDecl>(kv.second);
                prog.inits[static_cast<const ir::Node*>(m.get())] = nullptr;
                prog.java_moved.insert(static_cast<const ir::Node*>(m.get()));
                auto th = std::make_shared<ir::This>();
                th->type = std::make_shared<ir::Type>(S("class"), c->qname);
                pro.append(assign_of(m, th));
            } else {
                auto m = as<ir::MethodDecl>(kv.second);
                auto blk = std::holds_alternative<NodeP>(m->body) ? as<ir::Block>(std::get<NodeP>(m->body)) : nullptr;
                if (!blk) throw py::AttributeError("'list' object has no attribute 'stmts'");
                pro.extend(nodes_of(blk->stmts));
            }
        }
        prog.java_prologue[c->qname->utf8()] = pro;
    }
    return out;
}

bool JavaLib::ctor_needed(const ClassP& c, const MethodP&) {
    auto it = prog.java_prologue.find(c->qname->utf8());
    return it != prog.java_prologue.end() && !it->second.empty();
}

py::list<NodeP> JavaLib::ctor_prologue(const ClassP& c, const MethodP&, const py::ref<ir::CtorCall>& call) {
    if (call && call->kind && *call->kind == "this") return py::list<NodeP>();
    auto it = prog.java_prologue.find(c->qname->utf8());
    if (it == prog.java_prologue.end()) return py::list<NodeP>();
    return it->second.copy();
}

// ---- expressions
std::optional<Ex> JavaLib::select(const py::ref<ir::Select>& e) {
    const SymP& s = e->sym;
    if (!s || s->kind != "field") return std::nullopt;
    str owner = or_empty(s->owner);
    auto it = FIELDS().find({owner.utf8(), s->name.utf8()});
    if (s->owner && it != FIELDS().end()) {
        w.usings.add(S("System"));
        return Ex{S(it->second), P_PRIMARY};
    }
    if (s->owner && *s->owner == "java.lang.System" && (s->name == "out" || s->name == "err")) {
        w.usings.add(S("System"));
        return Ex{S(s->name == "out" ? "Console.Out" : "Console.Error"), P_PRIMARY};
    }
    if (starts_java(s->owner))
        throw Decline(S("library"), S("the Java library field ") + owner + S(".") + s->name + S(" has no C# mapping"));
    return std::nullopt;
}

namespace {
// how a library call is looked up in the registry: the mapping found (null: none) and the row's C# entry
struct Lookup {
    std::shared_ptr<java::apimap::Mapping> m;
    java::apimap::MapEntry raw;
};
}  // namespace

static Lookup lookup_(const py::ref<ir::Call>& e) {
    const SymP& s = e->sym;
    Lookup out;
    auto fit = s->owner ? FAMILY().find(s->owner->utf8()) : FAMILY().end();
    if (fit == FAMILY().end()) return out;
    str fam = S(fit->second.c_str());
    int64_t n = py::len(nodes_of(e->args));
    py::list<str> keys;
    if (!s->params.empty()) {
        TypeP p0 = s->params[0];
        str pk;
        if (const char* k = param_kind(p0->kind)) pk = S(k);
        else if (p0->kind == "class") pk = p0->simple();
        else if (p0->kind == "array" && p0->elem->kind == "u16") pk = S("char[]");
        else pk = S("Object");
        if (p0->kind == "typevar") pk = S("Object");
        keys.append(fam + S(".") + s->name + S("(") + pk + S(")"));
    }
    keys.append(fam + S(".") + s->name + S("/") + py::to_str(n));
    keys.append(fam + S(".") + s->name);
    auto reg = registry();
    for (const str& k : keys) {
        auto m = reg->resolve(k, S("java"), S("csharp"));
        if (m && m->name == k) {
            out.m = m;
            auto rec = reg->_sym.get(std::make_tuple(S("java"), k));
            if (rec && rec->maps.contains(S("csharp"))) out.raw = rec->maps.at(S("csharp"));
            return out;
        }
    }
    return out;
}

std::optional<Ex> JavaLib::call(const py::ref<ir::Call>& e, bool statement) {
    const SymP& s = e->sym;
    if (!s) return std::nullopt;
    ClassP c = s->owner ? prog.classes.get(*s->owner) : nullptr;
    if (c && c->kind && *c->kind == "enum" && s->static_ && (s->name == "values" || s->name == "valueOf")) {
        w.usings.add(S("System"));
        str tn = w.ctype(std::make_shared<ir::Type>(S("class"), c->qname));
        if (s->name == "values") return Ex{S("Enum.GetValues<") + tn + S(">()"), P_PRIMARY};
        return Ex{S("Enum.Parse<") + tn + S(">(") + w.expr(nodes_of(e->args)[0]).first + S(")"), P_PRIMARY};
    }
    if (c || !s->owner || s->owner->empty()) return std::nullopt;
    Lookup lk = lookup_(e);
    int64_t nargs = py::len(nodes_of(e->args));
    if (!lk.m) {
        if (object_method(s->name, nargs) && e->target) {
            TypeP tt = type_of(e->target);
            if (tt && tt->kind == "class" && tt->name && prog.classes.contains(*tt->name)) return std::nullopt;   // a class of ours: its own override
        }
        if (starts_java(s->owner))
            throw Decline(S("library"), S("the Java library call ") + *s->owner + S(".") + s->name + S("/") + py::to_str(nargs) + S(" has no C# mapping"));
        return std::nullopt;
    }
    const auto& m = lk.m;
    const auto& raw = lk.raw;
    if (m->kind == "unsupported")
        throw Decline(S("library"), *s->owner + S(".") + s->name + S(": ") + (!m->note.empty() ? m->note : S("no faithful C# form")));
    if (raw.ordered && *raw.ordered && e->target && !ordered_class(concrete(e->target)))
        throw Decline(S("hash-order"), S("iterating a ") + (concrete(e->target) && !concrete(e->target)->empty() ? *concrete(e->target) : S("?")) +
                                           S(": Java visits it in hash order, .NET in another"));
    bool has_stmt = raw.stmt && !raw.stmt->empty();
    str tmpl = (statement && has_stmt) ? *raw.stmt : m->target;
    if (tmpl.empty()) {
        if (has_stmt) throw Decline(S("library"), *s->owner + S(".") + s->name + S(" used for its value"));
        return std::nullopt;
    }
    if (tmpl.contains(S("JavaRt")) || (raw.target.contains(S("JavaRt")) && tmpl.contains(S("{s")))) prog.runtime.add(S("JavaRt"));
    str text = fill(tmpl, e);
    int64_t prec = P_PRIMARY;
    if (raw.prec && *raw.prec == "eq") prec = P_EQ;
    else if (raw.prec && *raw.prec == "unary") prec = P_UNARY;
    return Ex{text, prec};
}

std::optional<str> JavaLib::stmt_call(const py::ref<ir::Call>& e) {
    auto got = call(e, true);
    if (got) return got->first;
    return std::nullopt;
}

str JavaLib::fill(const str& tmpl, const py::ref<ir::Call>& e) {
    const py::list<NodeP>& args = nodes_of(e->args);
    const std::u32string& t = tmpl.u32();
    std::u32string out;
    size_t i = 0;
    auto all_digits = [](const std::u32string& x) {
        if (x.empty()) return false;
        for (char32_t ch : x)
            if (ch < U'0' || ch > U'9') return false;
        return true;
    };
    while (i < t.size()) {
        if (t[i] == U'{') {
            size_t close = t.find(U'}', i + 1);
            if (close != std::u32string::npos) {
                std::u32string k = t.substr(i + 1, close - i - 1);
                bool match = k == U"t" || k == U"out" || k == U"*" || (k.size() > 1 && k[0] == U's' && all_digits(k.substr(1))) ||
                             all_digits(k) || (k.size() > 1 && k.back() == U'p' && all_digits(k.substr(0, k.size() - 1)));
                if (match) {
                    str r;
                    if (k == U"t") {
                        r = e->target ? w.wrap(e->target, P_PRIMARY) : S("this");
                    } else if (k == U"out") {
                        w.usings.add(S("System"));
                        auto tsel = as<ir::Select>(e->target);
                        if (tsel && tsel->sym && tsel->sym->name == "err") r = S("Console.Error");
                        else r = S("Console");
                    } else if (k == U"*") {
                        py::list<str> xs;
                        for (const NodeP& a : args) xs.append(w.expr(a).first);
                        r = jn(S(", "), xs);
                    } else if (k[0] == U's') {
                        r = string_of(args[std::stoll(str(k.substr(1)).utf8())]);
                    } else if (k.back() == U'p') {
                        r = w.wrap(args[std::stoll(str(k.substr(0, k.size() - 1)).utf8())], P_PRIMARY);
                    } else {
                        int64_t idx = std::stoll(str(k).utf8());
                        TypeP pt = idx < py::len(e->sym->params) ? e->sym->params[idx] : TypeP();
                        r = (pt && pt->kind != "typevar") ? w.coerce(args[idx], pt) : w.expr(args[idx]).first;
                    }
                    out += r.u32();
                    i = close + 1;
                    continue;
                }
            }
        }
        out.push_back(t[i]);
        ++i;
    }
    str res(out);
    if (res.contains(S("JavaRt"))) prog.runtime.add(S("JavaRt"));
    if (res.contains(S("StringComparison")) || res.contains(S("Convert.")) || res.contains(S("Math.")) || res.contains(S("Array.")) ||
        res.contains(S("Environment.")) || res.contains(S("Console")))
        w.usings.add(S("System"));
    return res;
}

str JavaLib::string_of(const NodeP& a) {
    TypeP t = type_of(a);
    NodeP e = strip(a);
    if (auto l = as<ir::Lit>(e))
        if (std::holds_alternative<str>(l->value.v)) return w.expr(a).first;
    if ((t && (t->kind == "i8" || t->kind == "i16" || t->kind == "i32" || t->kind == "i64" || t->kind == "u16")) || is_a<ir::Concat>(e))
        return w.expr(a).first;
    if (t && t->kind == "array" && t->elem->kind == "u16") return S("new string(") + w.expr(a).first + S(")");
    if (is_string(t) && non_null(e)) return w.expr(a).first;
    prog.runtime.add(S("JavaRt"));
    return S("JavaRt.Str(") + w.expr(a).first + S(")");
}

std::optional<str> JavaLib::concat_part(const NodeP& p) {
    TypeP t = type_of(p);
    NodeP e = strip(p);
    if (auto l = as<ir::Lit>(e))
        if (std::holds_alternative<str>(l->value.v)) return std::nullopt;
    if (t && (t->kind == "i8" || t->kind == "i16" || t->kind == "i32" || t->kind == "i64" || t->kind == "u16")) return std::nullopt;
    if (is_string(t) && non_null(e)) return std::nullopt;
    return string_of(p);
}

std::optional<Ex> JavaLib::new_(const py::ref<ir::New>& e) {
    TypeP t = e->type;
    auto q = qual(t);
    if (!q || prog.classes.contains(*q)) return std::nullopt;
    str tn = w.ctype(t);
    const py::list<NodeP>& args = nodes_of(e->args);
    auto join_args = [&]() {
        py::list<str> xs;
        for (const NodeP& a : args) xs.append(w.expr(a).first);
        return jn(S(", "), xs);
    };
    if (GENERIC().count(q->utf8())) {
        if (args.size() == 1 && type_of(args[0]) && type_of(args[0])->is_prim())
            return Ex{S("new ") + tn + S("(") + w.expr(args[0]).first + S(")"), P_PRIMARY};   // a capacity
        return Ex{S("new ") + tn + S("(") + join_args() + S(")"), P_PRIMARY};
    }
    if (*q == "java.lang.StringBuilder" || *q == "java.lang.StringBuffer") {
        w.usings.add(S("System.Text"));
        if (!args.empty() && is_string(type_of(args[0]))) return Ex{S("new StringBuilder(") + string_of(args[0]) + S(")"), P_PRIMARY};
        return Ex{S("new StringBuilder(") + join_args() + S(")"), P_PRIMARY};
    }
    if (*q == "java.util.Random") {
        prog.runtime.add(S("JavaRt"));
        return Ex{S("new JavaRandom(") + join_args() + S(")"), P_PRIMARY};
    }
    if (TYPES().count(q->utf8()) && tn.endswith(S("Exception"))) {
        if (args.size() > 1 || (!args.empty() && !is_string(type_of(args[0]))))
            throw Decline(S("exception"), S("an exception made with a cause"));
        return Ex{S("new ") + tn + S("(") + join_args() + S(")"), P_PRIMARY};
    }
    if (*q == "java.lang.Object") return Ex{S("new object()"), P_PRIMARY};
    throw Decline(S("library"), S("new ") + *q + S(": no C# mapping"));
}

std::optional<Ex> JavaLib::binary(const py::ref<ir::Binary>& e) {
    if (e->op && (*e->op == "==" || *e->op == "!=") && is_string(type_of(e->left)) && is_string(type_of(e->right))) {
        NodeP l = strip(e->left), r = strip(e->right);
        auto ll = as<ir::Lit>(l);
        auto rl = as<ir::Lit>(r);
        if (!(ll && ll->value.is_none()) && !(rl && rl->value.is_none())) {
            str a = w.wrap(e->left, P_UNARY);
            str b = w.wrap(e->right, P_UNARY);
            return Ex{S("(object)") + a + S(" ") + *e->op + S(" (object)") + b, P_EQ};   // Java compares the references
        }
    }
    return std::nullopt;
}

std::optional<str> JavaLib::coerce(const NodeP& e, const TypeP& t) {
    if (t && t->kind == "class" && t->name) {
        static const char* boxes[] = {"java.lang.Integer", "java.lang.Long", "java.lang.Double", "java.lang.Float",
                                      "java.lang.Boolean", "java.lang.Character", "java.lang.Short", "java.lang.Byte"};
        bool is_box = false;
        for (const char* b : boxes)
            if (*t->name == b) is_box = true;
        if (is_box) {
            NodeP x = strip(e);
            auto l = as<ir::Lit>(x);
            if (l && l->value.is_none())
                throw Decline(S("boxed-null"), S("a null ") + t->simple() + S(": Java boxes it, the translation keeps it a value"));
        }
    }
    return std::nullopt;
}

void JavaLib::record(const SymP& sym, const NodeP& init) {
    NodeP x = init ? strip(init) : NodeP();
    auto n = as<ir::New>(x);
    if (sym && n && n->type && n->type->kind == "class") prog.concrete[concrete_key(sym->owner, sym->name)] = or_empty(n->type->name);
}

std::optional<str> JavaLib::concrete(const NodeP& e) {
    NodeP x = strip(e);
    SymP sym;
    if (auto n = as<ir::Name>(x)) sym = n->sym;
    else if (auto s = as<ir::Select>(x)) sym = s->sym;
    if (sym) {
        auto it = prog.concrete.find(concrete_key(sym->owner, sym->name));
        if (it != prog.concrete.end()) return it->second;
    }
    TypeP t = type_of(x);
    if (t && t->kind == "class") return t->name;
    return std::nullopt;
}

std::optional<str> JavaLib::iterable(const NodeP& e) {
    NodeP x = strip(e);
    if (auto c = ref_as<ir::Call>(x)) {
        auto got = call(c);
        if (got) return got->first;
    }
    TypeP t = type_of(e);
    if (t && t->kind == "class") {
        auto fit = t->name ? FAMILY().find(t->name->utf8()) : FAMILY().end();
        if (fit != FAMILY().end() && (fit->second == "Set" || fit->second == "Map") && !ordered_class(concrete(e))) {
            auto cc = concrete(e);
            throw Decline(S("hash-order"), S("iterating a ") + (cc ? *cc : S("None")) + S(": Java visits it in hash order, .NET in another"));
        }
    }
    return std::nullopt;
}

void JavaLib::check_lambda(const py::ref<ir::Lambda>& e) {
    auto q = qual(e->type);
    if (q && !FUNCTIONS().count(q->utf8()) && *q != "java.lang.Runnable" && !prog.classes.contains(*q))
        throw Decline(S("lambda"), S("a lambda of the interface ") + *q + S(" (no C# delegate for it)"));
    if (q && prog.classes.contains(*q))
        throw Decline(S("lambda"), S("a lambda of an interface declared in the program (C# lambdas make delegates only)"));
}

}  // namespace codebreaker::translate::backends::csharp

namespace codebreaker::translate::backends::csharp_java {

namespace {
namespace ir = codebreaker::translate::ir;
using py::str;

template <class T> T* as(const ir::NodeP& n) { return dynamic_cast<T*>(n.get()); }

// the interfaces of c (its own and those they extend), found depth first from the end of the list
py::list<py::ref<ir::ClassDecl>> interfaces(csharp::Program& prog, const py::ref<ir::ClassDecl>& c) {
    py::list<py::ref<ir::ClassDecl>> out;
    std::vector<ir::TypeP> todo;
    if (c->implements)
        for (const ir::TypeP& t : *c->implements) todo.push_back(t);
    std::set<std::string> seen;
    while (!todo.empty()) {
        ir::TypeP t = todo.back();
        todo.pop_back();
        py::ref<ir::ClassDecl> i = (t && t->kind == "class" && t->name) ? prog.classes.get(*t->name) : nullptr;
        if (!i || seen.count(i->qname->utf8())) continue;
        seen.insert(i->qname->utf8());
        out.append(i);
        if (i->implements)
            for (const ir::TypeP& x : *i->implements) todo.push_back(x);
        if (i->extends) todo.push_back(i->extends);
    }
    return out;
}

}  // namespace

void prepare(csharp::Program& prog) {
    if (prog.java_prepared) return;
    prog.java_prepared = true;
    std::vector<py::ref<ir::ClassDecl>> all;
    for (const str& k : prog.classes) all.push_back(prog.classes.at(k));
    for (const auto& c : all) {
        if (!(c->kind && *c->kind == "class") || !(c->mods && c->mods->contains(str("abstract")))) continue;
        std::set<std::pair<std::string, int64_t>> have;
        py::ref<ir::ClassDecl> k = c;
        std::set<std::string> seen;
        while (k && !seen.count(k->qname->utf8())) {
            seen.insert(k->qname->utf8());
            if (k->members)
                for (const ir::NodeP& m : *k->members)
                    if (auto md = as<ir::MethodDecl>(m))
                        have.insert({md->name ? md->name->utf8() : std::string(), md->params ? py::len(*md->params) : 0});
            k = prog.base_of(k);
        }
        for (const auto& i : interfaces(prog, c)) {
            if (!i->members) continue;
            std::vector<ir::NodeP> ms;
            for (const ir::NodeP& n : *i->members) ms.push_back(n);
            for (const ir::NodeP& m : ms) {
                auto md = std::dynamic_pointer_cast<ir::MethodDecl>(m);
                if (!md) continue;
                bool body = std::holds_alternative<py::list<ir::NodeP>>(md->body) || std::get<ir::NodeP>(md->body) != nullptr;
                std::pair<std::string, int64_t> key{md->name ? md->name->utf8() : std::string(), md->params ? py::len(*md->params) : 0};
                if (!body && !(md->mods && md->mods->contains(str("static"))) && !have.count(key)) {
                    auto nm = std::make_shared<ir::MethodDecl>();
                    nm->sym = md->sym;
                    nm->name = md->name;
                    nm->kind = str("method");
                    nm->params = md->params;
                    nm->ret = md->ret;
                    nm->body = ir::NodeP();
                    py::list<str> mods;
                    mods.append(str("public"));
                    mods.append(str("abstract"));
                    nm->mods = mods;
                    nm->throws = py::list<ir::TypeP>();
                    nm->typeparams = md->typeparams;
                    nm->line = md->line;
                    nm->col = md->col;
                    if (!c->members) c->members = py::list<ir::NodeP>();
                    c->members->append(nm);
                    have.insert(key);
                }
            }
        }
    }
}

}  // namespace codebreaker::translate::backends::csharp_java

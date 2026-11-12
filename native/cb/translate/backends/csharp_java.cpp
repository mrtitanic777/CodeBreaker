// cb/translate/backends/csharp_java.cpp -- Java's library and semantics for the C# back end (see the header).
#include "cb/translate/backends/csharp_java.hpp"

#include <algorithm>
#include <cctype>

#include "cb/core/text.hpp"

namespace cb::translate::backends::csharp_java {

namespace {

using cs::Decline;
using cs::Ex;
using ir::Node;

// ---- the tables ---------------------------------------------------------------------------------------------------------------

// Java class -> the C# type that stands for it (lookup only).
const std::unordered_map<std::string_view, std::string_view>& cj_types() {
    static const std::unordered_map<std::string_view, std::string_view> table = {
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
        {"java.lang.UnsupportedOperationException", "NotSupportedException"},
        {"java.lang.NumberFormatException", "FormatException"}, {"java.lang.ClassCastException", "InvalidCastException"},
        {"java.lang.NegativeArraySizeException", "OverflowException"}, {"java.util.NoSuchElementException", "InvalidOperationException"},
        {"java.lang.Comparable", "IComparable"}, {"java.lang.Iterable", "IEnumerable"}, {"java.lang.AutoCloseable", "IDisposable"},
        {"java.io.Closeable", "IDisposable"}, {"java.lang.Runnable", "Action"}, {"java.util.Random", "JavaRandom"}};
    return table;
}

// Java generic collection -> the .NET generic that stands for it.
const std::unordered_map<std::string_view, std::string_view>& cj_generic() {
    static const std::unordered_map<std::string_view, std::string_view> table = {
        {"java.util.List", "List"}, {"java.util.ArrayList", "List"}, {"java.util.LinkedList", "List"}, {"java.util.Vector", "List"},
        {"java.util.Collection", "ICollection"}, {"java.util.AbstractList", "List"}, {"java.util.Map", "IDictionary"},
        {"java.util.HashMap", "Dictionary"}, {"java.util.LinkedHashMap", "Dictionary"}, {"java.util.Hashtable", "Dictionary"},
        {"java.util.TreeMap", "SortedDictionary"}, {"java.util.SortedMap", "IDictionary"}, {"java.util.Set", "ISet"},
        {"java.util.HashSet", "HashSet"}, {"java.util.LinkedHashSet", "HashSet"}, {"java.util.TreeSet", "SortedSet"},
        {"java.util.SortedSet", "ISet"}, {"java.util.Map.Entry", "KeyValuePair"}};
    return table;
}

std::size_t cj_arity(std::string_view generic) {
    static const std::unordered_map<std::string_view, std::size_t> table = {
        {"List", 1}, {"Dictionary", 2}, {"SortedDictionary", 2}, {"HashSet", 1}, {"SortedSet", 1}, {"KeyValuePair", 2},
        {"IDictionary", 2}, {"ISet", 1}, {"ICollection", 1}};
    return table.at(generic);
}

// The collections whose iteration order is the same in Java and .NET.
bool cj_ordered(std::string_view qname) {
    return qname == "java.util.TreeMap" || qname == "java.util.LinkedHashMap" || qname == "java.util.TreeSet" || qname == "java.util.LinkedHashSet";
}

// A functional interface as a .NET delegate: its generic, and how the interface's own type arguments become the delegate's.
using Args = std::vector<std::string>;
struct Delegate {
    std::string_view generic;
    Args (*arguments)(Args);
};

const std::unordered_map<std::string_view, Delegate>& cj_functions() {
    static const auto same = [](Args a) { return a; };
    static const std::unordered_map<std::string_view, Delegate> table = {
        {"java.util.function.Function", {"Func", same}},
        {"java.util.function.BiFunction", {"Func", same}},
        {"java.util.function.Supplier", {"Func", same}},
        {"java.util.function.Consumer", {"Action", same}},
        {"java.util.function.BiConsumer", {"Action", same}},
        {"java.util.function.Predicate", {"Func", [](Args a) { a.push_back("bool"); return a; }}},
        {"java.util.function.BiPredicate", {"Func", [](Args a) { a.push_back("bool"); return a; }}},
        {"java.util.function.UnaryOperator", {"Func", [](Args a) { const std::string t = a.empty() ? "object" : a[0]; return Args{t, t}; }}},
        {"java.util.function.BinaryOperator", {"Func", [](Args a) { const std::string t = a.empty() ? "object" : a[0]; return Args{t, t, t}; }}},
        {"java.util.function.IntBinaryOperator", {"Func", [](Args) { return Args{"int", "int", "int"}; }}},
        {"java.util.function.IntUnaryOperator", {"Func", [](Args) { return Args{"int", "int"}; }}},
        {"java.util.function.IntPredicate", {"Func", [](Args) { return Args{"int", "bool"}; }}},
        {"java.util.function.IntFunction", {"Func", [](Args a) { a.insert(a.begin(), "int"); return a; }}},
        {"java.util.function.ToIntFunction", {"Func", [](Args a) { a.push_back("int"); return a; }}},
        {"java.util.function.IntSupplier", {"Func", [](Args) { return Args{"int"}; }}},
        {"java.util.function.DoubleUnaryOperator", {"Func", [](Args) { return Args{"double", "double"}; }}},
        {"java.util.Comparator", {"Comparison", same}}};
    return table;
}

// A library class's family: the classes that answer a call the same way (the registry's keys use the family).
const std::unordered_map<std::string, std::string>& cj_family() {
    static const std::unordered_map<std::string, std::string> table = [] {
        std::unordered_map<std::string, std::string> t = {
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
                              "AbstractSequentialList"}) {
            t[std::string("java.util.") + n] = "List";
        }
        t["java.lang.Iterable"] = "List";
        for (const char* n : {"Map", "HashMap", "LinkedHashMap", "TreeMap", "AbstractMap", "SortedMap", "Hashtable", "NavigableMap"}) {
            t[std::string("java.util.") + n] = "Map";
        }
        for (const char* n : {"Set", "HashSet", "LinkedHashSet", "TreeSet", "AbstractSet", "SortedSet", "NavigableSet"}) {
            t[std::string("java.util.") + n] = "Set";
        }
        for (const auto& [qname, delegate] : cj_functions()) {
            const std::string_view q = qname;
            t[std::string(q)] = std::string(q.substr(q.rfind('.') + 1));
        }
        t["java.lang.Runnable"] = "Runnable";
        return t;
    }();
    return table;
}

// The library's constants (a Java static field and the C# text for it).
std::optional<std::string_view> cj_field(std::string_view owner, std::string_view name) {
    static const std::map<std::pair<std::string_view, std::string_view>, std::string_view> table = {
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
    const auto it = table.find({owner, name});
    if (it == table.end()) return std::nullopt;
    return it->second;
}

// Java's Object methods and what they are called in C#.
std::optional<std::string_view> cj_object_method(std::string_view name, std::size_t arity) {
    if (name == "toString" && arity == 0) return "ToString";
    if (name == "equals" && arity == 1) return "Equals";
    if (name == "hashCode" && arity == 0) return "GetHashCode";
    return std::nullopt;
}

// A parameter type as the registry's keys name it (`String.indexOf(char)`): by kind, else by class.
std::string cj_param_key(const ir::TypeP& p) {
    const std::string_view kind = p->kind();
    if (kind == "typevar") return "Object";
    static const std::unordered_map<std::string_view, std::string_view> by_kind = {
        {"i32", "int"}, {"i64", "long"}, {"i16", "int"}, {"i8", "int"}, {"u16", "int"}, {"f32", "float"}, {"f64", "double"}, {"bool", "boolean"}};
    if (const auto it = by_kind.find(kind); it != by_kind.end()) return std::string(it->second);
    if (kind == "class") return p->simple();
    if (const auto* a = std::get_if<ir::ArrayType>(&p->body); a != nullptr && a->elem && a->elem->kind() == "u16") return "char[]";
    return "Object";
}

// ---- small helpers ------------------------------------------------------------------------------------------------------------

bool cj_has(const std::vector<std::string>& mods, std::string_view m) { return std::find(mods.begin(), mods.end(), m) != mods.end(); }

// The class name of a type, empty for any type that is not a class.
const std::string* cj_qual(const ir::TypeP& t) {
    const auto* c = t ? std::get_if<ir::ClassType>(&t->body) : nullptr;
    return c != nullptr ? &c->name : nullptr;
}

bool cj_library_name(std::string_view q) { return q.starts_with("java.") || q.starts_with("javax."); }

// The statements of a body that is a Block (or any other statement list).
std::vector<Node*> cj_body_statements(const ir::Suite& body) {
    if (const auto* one = std::get_if<Node*>(&body)) {
        if (const ir::Block* b = ir::get<ir::Block>(*one)) return b->stmts;
    }
    return ir::statements(body);
}

bool cj_string_value(Node* e) {   // a string literal
    const ir::Lit* l = ir::get<ir::Lit>(e);
    return l != nullptr && std::holds_alternative<std::string>(l->value);
}

bool cj_null_literal(Node* e) {
    const ir::Lit* l = ir::get<ir::Lit>(e);
    return l != nullptr && std::holds_alternative<std::monostate>(l->value);
}

bool cj_integral_type(const ir::TypeP& t) {
    if (!t) return false;
    const std::string_view k = t->kind();
    return k == "i8" || k == "i16" || k == "i32" || k == "i64" || k == "u16";
}

// Is this string expression never null? (a literal, a concatenation, a library call that gives text)
bool cj_non_null(Node* e) {
    if (ir::is<ir::Concat>(e) || cj_string_value(e)) return true;
    const ir::Call* c = ir::get<ir::Call>(e);
    if (c == nullptr || c->sym == nullptr || !c->sym->owner) return false;
    static const std::unordered_set<std::string_view> owners = {"java.lang.String", "java.lang.StringBuilder", "java.lang.Integer",
                                                                 "java.lang.Long", "java.lang.Double", "java.lang.Float"};
    static const std::unordered_set<std::string_view> names = {"toString", "substring", "trim", "toUpperCase", "toLowerCase", "replace",
                                                                "valueOf", "concat", "repeat", "join", "strip", "intern"};
    return owners.contains(*c->sym->owner) && names.contains(c->sym->name);
}

}  // namespace

// ---- before writing ---------------------------------------------------------------------------------------------------------

namespace {

// The classes of the program in the order they are declared (the unit's types, each before the classes nested in it).
void cj_collect(Node* node, std::vector<Node*>& out) {
    if (ir::get<ir::ClassDecl>(node) == nullptr) return;
    out.push_back(node);
    for (Node* m : ir::get<ir::ClassDecl>(node)->members) cj_collect(m, out);
}

// The interfaces a class implements, directly or through other interfaces (those declared in these units).
std::vector<ir::ClassDecl*> cj_interfaces(cs::Program& program, const ir::ClassDecl& c) {
    std::vector<ir::ClassDecl*> out;
    std::vector<ir::TypeP> todo = c.implements;
    std::set<std::string> seen;
    while (!todo.empty()) {
        const ir::TypeP t = todo.back();
        todo.pop_back();
        const std::string* q = cj_qual(t);
        ir::ClassDecl* i = q != nullptr ? program.class_named(*q) : nullptr;
        if (i == nullptr || !i->qname || !seen.insert(*i->qname).second) continue;
        out.push_back(i);
        todo.insert(todo.end(), i->implements.begin(), i->implements.end());
        if (i->extends) todo.push_back(i->extends);
    }
    return out;
}

}  // namespace

void JavaLibrary::prepare(cs::Program& program) {
    std::vector<Node*> classes;
    for (Node* u : program.units()) {
        if (const ir::Unit* unit = ir::get<ir::Unit>(u)) {
            for (Node* t : unit->types) cj_collect(t, classes);
        }
    }
    for (Node* cn : classes) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (c.kind != "class" || !cj_has(c.mods, "abstract")) continue;
        std::set<std::pair<std::string, std::size_t>> have;   // (name, arity) of what the class and its bases declare
        std::set<std::string> seen;
        for (ir::ClassDecl* k = &c; k != nullptr && k->qname && seen.insert(*k->qname).second; k = program.base_of(*k)) {
            for (Node* m : k->members) {
                if (const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m)) have.emplace(md->name.value_or(""), md->params.size());
            }
        }
        for (ir::ClassDecl* i : cj_interfaces(program, c)) {
            for (Node* mn : std::vector<Node*>(i->members)) {
                const ir::MethodDecl* m = ir::get<ir::MethodDecl>(mn);
                if (m == nullptr || ir::present(m->body) || cj_has(m->mods, "static")) continue;
                const std::pair<std::string, std::size_t> key{m->name.value_or(""), m->params.size()};
                if (have.contains(key)) continue;
                ir::MethodDecl abstract_copy;
                abstract_copy.sym = m->sym;
                abstract_copy.name = m->name;
                abstract_copy.kind = "method";
                abstract_copy.params = m->params;
                abstract_copy.ret = m->ret;
                abstract_copy.mods = {"public", "abstract"};
                abstract_copy.typeparams = m->typeparams;
                c.members.push_back(ir::make(program.arena(), std::move(abstract_copy), ir::TypeP{}, mn->line, mn->col));
                have.insert(key);
            }
        }
    }
}

// ---- types ------------------------------------------------------------------------------------------------------------------

std::optional<std::string> JavaLibrary::type_name(cs::Context& w, const ir::TypeP& t) {
    const std::string& q = std::get<ir::ClassType>(t->body).name;
    const std::vector<ir::TypeP>& type_args = std::get<ir::ClassType>(t->body).args;
    if (w.program().class_named(q) != nullptr) return std::nullopt;
    const auto ctypes = [&] {
        std::vector<std::string> out;
        for (const ir::TypeP& a : type_args) out.push_back(w.ctype(a));
        return out;
    };
    if (const auto f = cj_functions().find(q); f != cj_functions().end()) {
        const std::vector<std::string> args = f->second.arguments(ctypes());
        if (f->second.generic == "Action" && args.empty()) return "Action";
        w.add_using("System");
        return std::string(f->second.generic) + "<" + text::join(args, ", ") + ">";
    }
    if (const auto g = cj_generic().find(q); g != cj_generic().end()) {
        const std::string generic(g->second);
        w.add_using("System.Collections.Generic");
        const std::size_t n = cj_arity(generic);
        std::vector<std::string> args = ctypes();
        if (args.empty()) args.assign(n, "object");   // a raw type: of Object
        if (args.size() != n) throw Decline("type", "the type " + ir::to_text(*t));
        return generic + "<" + text::join(args, ", ") + ">";
    }
    if (const auto ty = cj_types().find(q); ty != cj_types().end()) {
        const std::string name(ty->second);
        if (name == "JavaRandom") w.program().use_runtime("JavaRt");
        if ((name == "IComparable" || name == "IEnumerable") && !type_args.empty()) {
            if (name == "IEnumerable") w.add_using("System.Collections.Generic");
            return name + "<" + text::join(ctypes(), ", ") + ">";
        }
        if (name.find('.') == std::string::npos && std::isupper(static_cast<unsigned char>(name[0])) != 0) w.add_using("System");
        return name;
    }
    if (cj_library_name(q)) throw Decline("library", "the Java library class " + q + " has no C# mapping");
    return std::nullopt;
}

bool JavaLibrary::keep_interface(const ir::TypeP& t) {
    const std::string* q = cj_qual(t);
    return q == nullptr || (*q != "java.io.Serializable" && *q != "java.lang.Cloneable" && *q != "java.util.RandomAccess");
}

// ---- declarations -------------------------------------------------------------------------------------------------------------

void JavaLibrary::check_class(cs::Context& w, ir::ClassDecl& c) {
    if (c.kind == "class" && c.outer && !cj_has(c.mods, "static")) {
        const ir::ClassDecl* outer = w.program().class_named(*c.outer);
        if (outer != nullptr && outer->kind == "class") {
            for (Node* m : c.members) {
                for (Node* n : ir::walk(m)) {
                    if (ir::is<ir::This>(n)) {
                        const std::string* q = cj_qual(n->type);
                        if (q != nullptr && outer->qname && *q == *outer->qname) {
                            throw Decline("inner-class", "an inner class that uses its outer instance (" + c.name.value_or("") + ")");
                        }
                    }
                    if (const ir::Call* call = ir::get<ir::Call>(n);
                        call != nullptr && call->target == nullptr && call->sym != nullptr && call->sym->owner == outer->qname) {
                        throw Decline("inner-class", "an inner class that calls its outer instance (" + c.name.value_or("") + ")");
                    }
                }
            }
        }
    }
    if (c.kind == "enum") {
        for (Node* m : c.members) {
            const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m);
            const ir::EnumConst* ec = ir::get<ir::EnumConst>(m);
            if (ir::is<ir::FieldDecl>(m) || (md != nullptr && !cj_has(md->mods, "IMPLICIT")) || (ec != nullptr && (!ec->args.empty() || ec->body != nullptr))) {
                throw Decline("enum", "a Java enum with fields, methods or constructor arguments (an enum of objects)");
            }
        }
    }
}

std::vector<std::string> JavaLibrary::class_mods(cs::Context&, ir::ClassDecl& c) {
    std::vector<std::string> out;
    for (const std::string& m : c.mods) {
        if (m == "public" || m == "protected" || m == "private" || m == "abstract") out.push_back(m);
        else if (m == "final" && c.kind == "class") out.push_back("sealed");
    }
    if (!c.outer && !cj_has(out, "public")) out.insert(out.begin(), "internal");
    return out;
}

std::vector<std::string> JavaLibrary::access(const std::vector<std::string>& mods, const ir::ClassDecl& c) const {
    std::vector<std::string> acc;
    for (const std::string& m : mods) {
        if (m == "public" || m == "protected" || m == "private") acc.push_back(m);
    }
    if (acc == std::vector<std::string>{"private"} && c.outer) acc = {"internal"};   // Java's private reaches the whole top-level class
    if (acc.empty() && c.kind != "interface") acc = {"internal"};
    if (acc == std::vector<std::string>{"protected"}) acc = {"protected", "internal"};   // Java's protected is package-visible too
    return acc;
}

bool JavaLibrary::is_const(const ir::FieldDecl& f) const {
    if (f.init == nullptr) return false;
    Node* init = cs::strip(f.init);
    const ir::Lit* lit = ir::get<ir::Lit>(init);
    return lit != nullptr && !std::holds_alternative<std::monostate>(lit->value) && f.type && (f.type->is_prim() || cs::is_string(f.type));
}

std::vector<std::string> JavaLibrary::field_mods(cs::Context&, ir::FieldDecl& f, ir::ClassDecl& c) {
    const std::vector<std::string>& mods = f.mods;
    if (c.kind == "interface") return is_const(f) ? std::vector<std::string>{"public", "const"} : std::vector<std::string>{"public", "static", "readonly"};
    std::vector<std::string> out = access(mods, c);
    if (cj_has(mods, "static") && cj_has(mods, "final") && is_const(f)) {
        out.push_back("const");
        return out;
    }
    if (cj_has(mods, "static")) out.push_back("static");
    if (cj_has(mods, "final") && f.init != nullptr && !moved_.contains(&f)) out.push_back("readonly");
    if (cj_has(mods, "volatile")) out.push_back("volatile");
    return out;
}

bool JavaLibrary::overrides(cs::Context& w, const ir::ClassDecl& c, const ir::MethodDecl& m) const {
    if (cj_object_method(m.name.value_or(""), m.params.size())) return true;
    return w.program().overrides(c, m);
}

std::vector<std::string> JavaLibrary::method_mods(cs::Context& w, ir::MethodDecl& m, ir::ClassDecl& c) {
    const std::vector<std::string>& mods = m.mods;
    if (c.kind == "interface") return cj_has(mods, "static") ? std::vector<std::string>{"static"} : std::vector<std::string>{};
    std::vector<std::string> out = access(mods, c);
    if (m.kind == "ctor") return out;
    if (cj_has(mods, "static")) {
        out.push_back("static");
        return out;
    }
    if (cj_has(mods, "abstract")) {
        out.push_back("abstract");
        if (overrides(w, c, m)) out.push_back("override");
        return out;
    }
    if (overrides(w, c, m)) {
        out.push_back("override");
        if (cj_has(mods, "final")) out.push_back("sealed");
        return out;
    }
    if (cj_has(mods, "private") || cj_has(mods, "final") || cj_has(c.mods, "final")) return out;
    out.push_back("virtual");
    return out;
}

bool JavaLibrary::closeable(cs::Context& w, const ir::ClassDecl& start) const {
    std::set<std::string> seen;
    for (const ir::ClassDecl* c = &start; c != nullptr && c->qname && seen.insert(*c->qname).second; c = w.program().base_of(*c)) {
        for (const ir::TypeP& i : c->implements) {
            const std::string* q = cj_qual(i);
            if (q != nullptr && (*q == "java.lang.AutoCloseable" || *q == "java.io.Closeable")) return true;
        }
    }
    return false;
}

std::string JavaLibrary::method_name(cs::Context& w, ir::MethodDecl& m, ir::ClassDecl& c) {
    const std::string name = m.name.value_or("");
    const std::size_t n = m.params.size();
    if (const auto object_name = cj_object_method(name, n)) return std::string(*object_name);
    if (name == "main" && cj_has(m.mods, "static") && n == 1) return "Main";
    if (name == "compareTo" && n == 1 &&
        std::any_of(c.implements.begin(), c.implements.end(), [](const ir::TypeP& i) { const std::string* q = cj_qual(i); return q != nullptr && *q == "java.lang.Comparable"; })) {
        return "CompareTo";
    }
    if (name == "close" && n == 0 && closeable(w, c)) return "Dispose";
    if (m.name == c.name) return cs::ident(name) + "_";   // (C# forbids a member named as its class)
    return cs::ident(name);
}

std::string JavaLibrary::member_name(cs::Context& w, const ir::Sym& s) {
    if (s.kind == ir::SymKind::Method) {
        const std::size_t n = s.params.size();
        if (const auto object_name = cj_object_method(s.name, n)) return std::string(*object_name);
        const ir::ClassDecl* c = s.owner ? w.program().class_named(*s.owner) : nullptr;
        if (c != nullptr) {
            if (s.name == "compareTo" && n == 1) return "CompareTo";
            if (s.name == "close" && n == 0 && closeable(w, *c)) return "Dispose";
            if (c->name && s.name == *c->name) return cs::ident(s.name) + "_";
            if (s.name == "main" && s.is_static && n == 1) return "Main";
        }
    }
    return cs::ident(s.name);
}

std::vector<Node*> JavaLibrary::arrange_members(cs::Context& w, ir::ClassDecl& c, std::vector<Node*> members) {
    cs::Program& program = w.program();
    Arena& arena = program.arena();
    const std::string qname = c.qname.value_or("");
    std::vector<Node*> out;
    std::vector<Node*> statics;   // the static fields with an initialiser and the static blocks, in their textual order
    std::vector<Node*> moves;     // the instance fields with an initialiser and the instance blocks
    bool has_block = false;
    const std::string* base = cj_qual(c.extends);
    const bool base_is_ours = c.extends && base != nullptr && program.class_named(*base) != nullptr;
    for (Node* mn : members) {
        const ir::MethodDecl* md = ir::get<ir::MethodDecl>(mn);
        if (md != nullptr && md->kind == "static_init") {
            has_block = true;
            statics.push_back(mn);
            continue;   // (not written where it stands)
        }
        if (const ir::FieldDecl* f = ir::get<ir::FieldDecl>(mn)) {
            if (cj_has(f->mods, "static") && f->init != nullptr && !is_const(*f)) statics.push_back(mn);
            if (!cj_has(f->mods, "static") && f->init != nullptr && base_is_ours) moves.push_back(mn);
        }
        if (md != nullptr && md->kind == "init") moves.push_back(mn);
        out.push_back(mn);
    }
    // `Class.field = init;` or `this.field = init;` as a statement
    const auto assignment = [&](ir::FieldDecl& f, Node* object) {
        Node* target = ir::make(arena, ir::Select{.obj = object, .sym = f.sym}, f.type);
        Node* assign = ir::make(arena, ir::Assign{.op = std::nullopt, .target = target, .value = f.init}, f.type);
        return ir::make(arena, ir::ExprStmt{.expr = assign});
    };
    const auto move_out = [&](Node* node) {
        ir::FieldDecl& f = *ir::get<ir::FieldDecl>(node);
        program.set_init(node, nullptr);
        moved_.insert(&f);
        return &f;
    };
    if (has_block) {
        std::vector<Node*> body;
        for (Node* mn : statics) {
            if (ir::get<ir::FieldDecl>(mn) != nullptr) {
                Node* object = ir::make(arena, ir::TypeName{}, ir::class_type(qname));
                body.push_back(assignment(*move_out(mn), object));
            } else {
                const std::vector<Node*> stmts = cj_body_statements(ir::get<ir::MethodDecl>(mn)->body);
                body.insert(body.end(), stmts.begin(), stmts.end());
            }
        }
        ir::MethodDecl cctor;
        cctor.name = "<clinit>";
        cctor.kind = "cctor";
        cctor.ret = ir::void_type();
        cctor.body = ir::make(arena, ir::Block{.stmts = std::move(body)});
        cctor.mods = {"static"};
        out.push_back(ir::make(arena, std::move(cctor)));
    }
    if (!moves.empty()) {
        std::vector<Node*> prologue;
        for (Node* mn : moves) {
            if (ir::get<ir::FieldDecl>(mn) != nullptr) {
                Node* object = ir::make(arena, ir::This{}, ir::class_type(qname));
                prologue.push_back(assignment(*move_out(mn), object));
            } else {
                const std::vector<Node*> stmts = cj_body_statements(ir::get<ir::MethodDecl>(mn)->body);
                prologue.insert(prologue.end(), stmts.begin(), stmts.end());
            }
        }
        prologue_[qname] = std::move(prologue);
    }
    return out;
}

bool JavaLibrary::ctor_needed(cs::Context&, ir::ClassDecl& c, ir::MethodDecl&) {
    const auto it = prologue_.find(c.qname.value_or(""));
    return it != prologue_.end() && !it->second.empty();
}

std::vector<Node*> JavaLibrary::ctor_prologue(cs::Context&, ir::ClassDecl& c, ir::MethodDecl&, ir::CtorCall* call) {
    if (call != nullptr && call->kind == "this") return {};
    const auto it = prologue_.find(c.qname.value_or(""));
    return it != prologue_.end() ? it->second : std::vector<Node*>{};
}

// ---- expressions --------------------------------------------------------------------------------------------------------------

std::optional<Ex> JavaLibrary::select(cs::Context& w, Node* e) {
    const ir::Select& sel = *ir::get<ir::Select>(e);
    const ir::SymP& s = sel.sym;
    if (s == nullptr || s->kind != ir::SymKind::Field) return std::nullopt;
    const std::string owner = s->owner.value_or("");
    if (const auto text = cj_field(owner, s->name)) {
        w.add_using("System");
        return Ex{std::string(*text), cs::kPrimary};
    }
    if (owner == "java.lang.System" && (s->name == "out" || s->name == "err")) {
        w.add_using("System");
        return Ex{s->name == "out" ? "Console.Out" : "Console.Error", cs::kPrimary};
    }
    if (cj_library_name(owner)) throw Decline("library", "the Java library field " + owner + "." + s->name + " has no C# mapping");
    return std::nullopt;
}

JavaLibrary::Row JavaLibrary::lookup(const ir::Call& call) const {
    const ir::Sym& s = *call.sym;
    const auto fam = cj_family().find(s.owner.value_or(""));
    if (fam == cj_family().end()) return {};
    std::vector<std::string> keys;
    if (!s.params.empty()) keys.push_back(fam->second + "." + s.name + "(" + cj_param_key(s.params[0]) + ")");
    keys.push_back(fam->second + "." + s.name + "/" + std::to_string(call.args.size()));
    keys.push_back(fam->second + "." + s.name);
    for (const std::string& k : keys) {
        std::optional<cb::java::apimap::Mapping> m = registry_.resolve(k, "java", "csharp");
        if (!m || m->name != k) continue;
        const cb::java::apimap::SymbolRecord* rec = registry_.record("java", k);
        const cb::java::apimap::TargetMap* raw = rec != nullptr ? rec->maps.find("csharp") : nullptr;
        return {std::move(m), raw};
    }
    return {};
}

std::optional<Ex> JavaLibrary::call(cs::Context& w, Node* e, bool statement) {
    const ir::Call& call = *ir::get<ir::Call>(e);
    const ir::SymP& s = call.sym;
    if (s == nullptr) return std::nullopt;
    const std::string owner = s->owner.value_or("");
    const ir::ClassDecl* c = w.program().class_named(owner);
    if (c != nullptr && c->kind == "enum" && s->is_static && (s->name == "values" || s->name == "valueOf")) {
        w.add_using("System");
        const std::string tn = w.ctype(ir::class_type(c->qname.value_or("")));
        if (s->name == "values") return Ex{"Enum.GetValues<" + tn + ">()", cs::kPrimary};
        return Ex{"Enum.Parse<" + tn + ">(" + w.expr(call.args.at(0)).text + ")", cs::kPrimary};
    }
    if (c != nullptr || owner.empty()) return std::nullopt;
    const Row row = lookup(call);
    if (!row.mapping) {
        if (cj_object_method(s->name, call.args.size()) && call.target != nullptr) {
            const std::string* tq = cj_qual(call.target->type);
            if (tq != nullptr && w.program().class_named(*tq) != nullptr) return std::nullopt;   // a class of ours: its own override
        }
        if (cj_library_name(owner)) {
            throw Decline("library", "the Java library call " + owner + "." + s->name + "/" + std::to_string(call.args.size()) + " has no C# mapping");
        }
        return std::nullopt;
    }
    const cb::java::apimap::Mapping& m = *row.mapping;
    if (m.kind == cb::java::apimap::Kind::Unsupported) {
        throw Decline("library", owner + "." + s->name + ": " + (m.note.empty() ? std::string("no faithful C# form") : m.note));
    }
    const bool has_stmt = row.raw != nullptr && row.raw->stmt && !row.raw->stmt->empty();
    if (row.raw != nullptr && row.raw->ordered.value_or(false) && call.target != nullptr) {
        const std::optional<std::string> held = concrete(call.target);
        if (!held || !cj_ordered(*held)) {
            throw Decline("hash-order", "iterating a " + held.value_or("?") + ": Java visits it in hash order, .NET in another");
        }
    }
    const std::string tmpl = statement && has_stmt ? *row.raw->stmt : m.target;
    if (tmpl.empty()) {
        if (has_stmt) throw Decline("library", owner + "." + s->name + " used for its value");
        return std::nullopt;
    }
    const std::string raw_target = row.raw != nullptr ? row.raw->target : std::string();
    if (tmpl.find("JavaRt") != std::string::npos || (raw_target.find("JavaRt") != std::string::npos && tmpl.find("{s") != std::string::npos)) {
        w.program().use_runtime("JavaRt");
    }
    std::string text = fill(w, tmpl, call);
    int prec = cs::kPrimary;
    if (row.raw != nullptr && row.raw->prec) {
        if (*row.raw->prec == "eq") prec = cs::kEq;
        else if (*row.raw->prec == "unary") prec = cs::kUnary;
    }
    return Ex{std::move(text), prec};
}

std::optional<std::string> JavaLibrary::stmt_call(cs::Context& w, Node* e) {
    std::optional<Ex> got = call(w, e, true);
    if (!got) return std::nullopt;
    return std::move(got->text);
}

// A template's `{t}` `{out}` `{*}` `{sN}` `{N}` and `{Np}` filled in; any other brace is C# text and stays.
std::string JavaLibrary::fill(cs::Context& w, const std::string& tmpl, const ir::Call& call) {
    const std::vector<Node*>& args = call.args;
    const auto nth = [&](std::string_view digits) -> std::size_t {
        return static_cast<std::size_t>(text::parse_int<std::size_t>(digits).value_or(0));
    };
    std::string out;
    for (std::size_t i = 0; i < tmpl.size();) {
        const std::size_t close = tmpl[i] == '{' ? tmpl.find('}', i) : std::string::npos;
        if (close == std::string::npos) {
            out += tmpl[i++];
            continue;
        }
        const std::string_view key = std::string_view(tmpl).substr(i + 1, close - i - 1);
        const auto all_digits = [](std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char ch) { return ch >= '0' && ch <= '9'; }); };
        if (key == "t") {
            out += call.target != nullptr ? w.wrap(call.target, cs::kPrimary) : std::string("this");
        } else if (key == "out") {
            w.add_using("System");
            const ir::Select* sel = ir::get<ir::Select>(call.target);
            out += sel != nullptr && sel->sym != nullptr && sel->sym->name == "err" ? "Console.Error" : "Console";
        } else if (key == "*") {
            std::vector<std::string> parts;
            for (Node* a : args) parts.push_back(w.expr(a).text);
            out += text::join(parts, ", ");
        } else if (key.size() > 1 && key[0] == 's' && all_digits(key.substr(1))) {
            out += string_of(w, args.at(nth(key.substr(1))));
        } else if (key.size() > 1 && key.back() == 'p' && all_digits(key.substr(0, key.size() - 1))) {
            out += w.wrap(args.at(nth(key.substr(0, key.size() - 1))), cs::kPrimary);
        } else if (all_digits(key)) {
            const std::size_t n = nth(key);
            const ir::TypeP* pt = n < call.sym->params.size() ? &call.sym->params[n] : nullptr;
            out += pt != nullptr && (*pt)->kind() != "typevar" ? w.coerce(args.at(n), *pt) : w.expr(args.at(n)).text;
        } else {
            out += tmpl[i++];
            continue;
        }
        i = close + 1;
    }
    if (out.find("JavaRt") != std::string::npos) w.program().use_runtime("JavaRt");
    for (const char* word : {"StringComparison", "Convert.", "Math.", "Array.", "Environment.", "Console"}) {
        if (out.find(word) != std::string::npos) {
            w.add_using("System");
            break;
        }
    }
    return out;
}

std::string JavaLibrary::string_of(cs::Context& w, Node* a) {
    const ir::TypeP& t = a->type;
    Node* e = cs::strip(a);
    if (cj_string_value(e)) return w.expr(a).text;
    if (cj_integral_type(t) || ir::is<ir::Concat>(e)) return w.expr(a).text;
    if (const auto* arr = t ? std::get_if<ir::ArrayType>(&t->body) : nullptr; arr != nullptr && arr->elem && arr->elem->kind() == "u16") {
        return "new string(" + w.expr(a).text + ")";
    }
    if (cs::is_string(t) && cj_non_null(e)) return w.expr(a).text;
    w.program().use_runtime("JavaRt");
    return "JavaRt.Str(" + w.expr(a).text + ")";
}

std::optional<std::string> JavaLibrary::concat_part(cs::Context& w, Node* part) {
    const ir::TypeP& t = part->type;
    Node* e = cs::strip(part);
    if (cj_string_value(e)) return std::nullopt;
    if (cj_integral_type(t)) return std::nullopt;
    if (cs::is_string(t) && cj_non_null(e)) return std::nullopt;
    return string_of(w, part);
}

std::optional<Ex> JavaLibrary::new_object(cs::Context& w, Node* e) {
    const ir::TypeP& t = e->type;
    const std::string* q = cj_qual(t);
    if (q == nullptr || w.program().class_named(*q) != nullptr) return std::nullopt;
    const std::string tn = w.ctype(t);
    const std::vector<Node*>& args = ir::get<ir::New>(e)->args;
    const auto joined = [&] {
        std::vector<std::string> parts;
        for (Node* a : args) parts.push_back(w.expr(a).text);
        return text::join(parts, ", ");
    };
    if (cj_generic().contains(*q)) return Ex{"new " + tn + "(" + joined() + ")", cs::kPrimary};   // (a capacity, or the collection copied)
    if (*q == "java.lang.StringBuilder" || *q == "java.lang.StringBuffer") {
        w.add_using("System.Text");
        if (!args.empty() && cs::is_string(args[0]->type)) return Ex{"new StringBuilder(" + string_of(w, args[0]) + ")", cs::kPrimary};
        return Ex{"new StringBuilder(" + joined() + ")", cs::kPrimary};
    }
    if (*q == "java.util.Random") {
        w.program().use_runtime("JavaRt");
        return Ex{"new JavaRandom(" + joined() + ")", cs::kPrimary};
    }
    if (cj_types().contains(*q) && tn.ends_with("Exception")) {
        if (args.size() > 1 || (!args.empty() && !cs::is_string(args[0]->type))) throw Decline("exception", "an exception made with a cause");
        return Ex{"new " + tn + "(" + joined() + ")", cs::kPrimary};
    }
    if (*q == "java.lang.Object") return Ex{"new object()", cs::kPrimary};
    throw Decline("library", "new " + *q + ": no C# mapping");
}

std::optional<Ex> JavaLibrary::binary(cs::Context& w, Node* e) {
    const ir::Binary& b = *ir::get<ir::Binary>(e);
    if ((b.op == "==" || b.op == "!=") && cs::is_string(b.left->type) && cs::is_string(b.right->type)) {
        if (cj_null_literal(cs::strip(b.left)) || cj_null_literal(cs::strip(b.right))) return std::nullopt;
        // Java compares the references
        return Ex{"(object)" + w.wrap(b.left, cs::kUnary) + " " + b.op + " (object)" + w.wrap(b.right, cs::kUnary), cs::kEq};
    }
    return std::nullopt;
}

std::optional<std::string> JavaLibrary::coerce(cs::Context&, Node* e, const ir::TypeP& type) {
    static const std::unordered_set<std::string_view> boxed = {"java.lang.Integer", "java.lang.Long", "java.lang.Double", "java.lang.Float",
                                                                "java.lang.Boolean", "java.lang.Character", "java.lang.Short", "java.lang.Byte"};
    const std::string* q = cj_qual(type);
    if (q != nullptr && boxed.contains(*q) && cj_null_literal(cs::strip(e))) {
        throw Decline("boxed-null", "a null " + type->simple() + ": Java boxes it, the translation keeps it a value");
    }
    return std::nullopt;
}

void JavaLibrary::record(cs::Context&, const ir::SymP& sym, Node* init) {
    Node* x = init != nullptr ? cs::strip(init) : nullptr;
    const std::string* q = x != nullptr && ir::is<ir::New>(x) ? cj_qual(x->type) : nullptr;
    if (sym != nullptr && q != nullptr) concrete_[{sym->owner.value_or(""), sym->name}] = *q;
}

std::optional<std::string> JavaLibrary::concrete(Node* e) const {
    Node* x = cs::strip(e);
    const ir::SymP* sym = nullptr;
    if (const ir::Name* n = ir::get<ir::Name>(x)) sym = &n->sym;
    else if (const ir::Select* s = ir::get<ir::Select>(x)) sym = &s->sym;
    if (sym != nullptr && *sym != nullptr) {
        const auto it = concrete_.find({(*sym)->owner.value_or(""), (*sym)->name});
        if (it != concrete_.end()) return it->second;
    }
    if (const std::string* q = cj_qual(x->type)) return *q;
    return std::nullopt;
}

std::optional<std::string> JavaLibrary::iterable(cs::Context& w, Node* e) {
    Node* x = cs::strip(e);
    if (ir::is<ir::Call>(x)) {
        if (std::optional<Ex> got = call(w, x, false)) return std::move(got->text);
    }
    const std::string* q = cj_qual(e->type);
    if (q != nullptr) {
        const auto fam = cj_family().find(*q);
        if (fam != cj_family().end() && (fam->second == "Set" || fam->second == "Map")) {
            const std::optional<std::string> held = concrete(e);
            if (!held || !cj_ordered(*held)) {
                throw Decline("hash-order", "iterating a " + held.value_or("?") + ": Java visits it in hash order, .NET in another");
            }
        }
    }
    return std::nullopt;
}

void JavaLibrary::check_lambda(cs::Context& w, ir::Lambda&, const ir::TypeP& type) {
    const std::string* q = cj_qual(type);
    if (q == nullptr) return;
    if (w.program().class_named(*q) != nullptr) {
        throw Decline("lambda", "a lambda of an interface declared in the program (C# lambdas make delegates only)");
    }
    if (!cj_functions().contains(*q) && *q != "java.lang.Runnable") {
        throw Decline("lambda", "a lambda of the interface " + *q + " (no C# delegate for it)");
    }
}

cs::LibraryFactory factory(const cb::java::apimap::Registry& registry) {
    return [&registry] { return std::make_unique<JavaLibrary>(registry); };
}

}  // namespace cb::translate::backends::csharp_java

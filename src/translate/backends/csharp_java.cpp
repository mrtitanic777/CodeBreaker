// translate/backends/csharp_java.cpp -- Java's library and semantics for the C# back end (see the header).
#include "translate/backends/csharp_java.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <functional>

#include "core/text.hpp"
#include "translate/backends/jmodel.hpp"

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
        {"java.util.HashMap", "JHashMap"}, {"java.util.LinkedHashMap", "JLinkedHashMap"}, {"java.util.Hashtable", "Dictionary"},
        {"java.util.TreeMap", "SortedDictionary"}, {"java.util.SortedMap", "IDictionary"}, {"java.util.Set", "ISet"},
        {"java.util.HashSet", "JHashSet"}, {"java.util.LinkedHashSet", "JLinkedHashSet"}, {"java.util.TreeSet", "SortedSet"},
        {"java.util.SortedSet", "ISet"}, {"java.util.Map.Entry", "KeyValuePair"}, {"java.util.Iterator", "JIterator"}};
    return table;
}

std::size_t cj_arity(std::string_view generic) {
    static const std::unordered_map<std::string_view, std::size_t> table = {
        {"List", 1}, {"Dictionary", 2}, {"JHashMap", 2}, {"JLinkedHashMap", 2}, {"SortedDictionary", 2}, {"JHashSet", 1}, {"JLinkedHashSet", 1},
        {"SortedSet", 1}, {"KeyValuePair", 2}, {"IDictionary", 2}, {"ISet", 1}, {"ICollection", 1}, {"JIterator", 1}};
    return table.at(generic);
}

// The collections whose iteration order is the same in Java and in the translation: all but Hashtable, which walks its table its own way.
bool cj_ordered(std::string_view qname) { return qname != "java.util.Hashtable"; }

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
        {"java.util.function.IntConsumer", {"Action", [](Args) { return Args{"int"}; }}},
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

// The nodes of a member, leaving out the bodies of classes declared inside it.
std::vector<Node*> cj_own_nodes(Node* root) {
    std::vector<Node*> out;
    std::vector<Node*> todo{root};
    while (!todo.empty()) {
        Node* n = todo.back();
        todo.pop_back();
        if (ir::is<ir::ClassDecl>(n) || ir::is<ir::LocalClass>(n)) continue;
        out.push_back(n);
        if (const ir::New* made = ir::get<ir::New>(n); made != nullptr && made->body != nullptr) {
            todo.insert(todo.end(), made->args.begin(), made->args.end());
            if (made->outer != nullptr) todo.push_back(made->outer);
            continue;
        }
        for (Node* child : ir::children(*n)) todo.push_back(child);
    }
    return out;
}

// The locals and parameters these nodes declare.
std::set<std::string> cj_declared(const std::vector<Node*>& nodes) {
    std::set<std::string> out;
    for (Node* n : nodes) {
        if (ir::is<ir::Name>(n)) continue;
        ir::visit(*n, [&](auto& k) {
            if constexpr (requires { k.sym; }) {
                const ir::Sym* s = k.sym.get();
                if (s != nullptr && (s->kind == ir::SymKind::Local || s->kind == ir::SymKind::Param)) out.insert(s->name);
            }
        });
    }
    return out;
}

// Can constructing this class (and the program classes above it) run nothing of a subclass? True when no constructor, instance
// initialiser or field initialiser calls an instance method or lets `this` go anywhere but a field access.
bool cj_base_construction_is_closed(const cs::Program& program, const ir::ClassDecl* base) {
    std::set<std::string> seen;
    for (; base != nullptr && base->qname && seen.insert(*base->qname).second; base = program.base_of(*base)) {
        for (Node* m : base->members) {
            const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m);
            const ir::FieldDecl* f = ir::get<ir::FieldDecl>(m);
            const bool runs = (md != nullptr && (md->kind == "ctor" || md->kind == "init")) || (f != nullptr && !cj_has(f->mods, "static") && f->init != nullptr);
            if (!runs) continue;
            std::size_t thises = 0, field_reads = 0;
            for (Node* n : cj_own_nodes(m)) {
                if (ir::is<ir::This>(n)) ++thises;
                if (const ir::Select* sel = ir::get<ir::Select>(n); sel != nullptr && ir::is<ir::This>(sel->obj)) ++field_reads;
                if (const ir::Call* call = ir::get<ir::Call>(n); call != nullptr && call->target == nullptr && (call->sym == nullptr || !call->sym->is_static)) return false;
            }
            if (thises > field_reads) return false;
        }
    }
    return true;
}

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
    prepare_local_classes(program, classes);
    prepare_inner_classes(program, classes);
    for (Node* u : program.units()) {   // the interfaces of the program that lambdas are made for
        for (Node* n : ir::walk(u)) {
            if (const ir::Catch* caught = ir::get<ir::Catch>(n)) {
                for (const ir::TypeP& t : caught->types) {
                    const std::string* name = cj_qual(t);
                    catches_negative_size_ = catches_negative_size_ || (name != nullptr && *name == "java.lang.NegativeArraySizeException");
                }
            }
            if (!ir::is<ir::Lambda>(n) && !ir::is<ir::MethodRef>(n)) continue;
            const std::string* q = cj_qual(n->type);
            const ir::ClassDecl* target = q != nullptr ? program.class_named(*q) : nullptr;
            if (target == nullptr || target->kind != "interface" || lambda_interfaces_.contains(*q)) continue;
            if (const ir::MethodDecl* method = single_abstract(program, *target)) lambda_interfaces_.emplace(*q, method);
        }
    }
    for (Node* cn : classes) {   // an enum of objects is a class: a plain one stays a C# enum
        const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (c.kind != "enum" || !c.qname) continue;
        bool rich = false, bodies = false;
        std::vector<std::string> names;
        for (Node* m : c.members) {
            const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m);
            if (const ir::EnumConst* ec = ir::get<ir::EnumConst>(m)) {
                names.push_back(ec->sym->name);
                rich = rich || !ec->args.empty();
                bodies = bodies || ec->body != nullptr;
            } else if (ir::is<ir::FieldDecl>(m) || (md != nullptr && !cj_has(md->mods, "IMPLICIT"))) {
                rich = true;
            }
        }
        if (rich && !bodies) object_enums_[*c.qname] = std::move(names);
    }
    for (Node* cn : classes) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (c.kind != "class" || !cj_has(c.mods, "abstract")) continue;
        std::set<std::pair<std::string, std::size_t>> have;   // (name, arity) of what the class and its bases declare
        std::set<std::string> seen;
        for (ir::ClassDecl* k = &c; k != nullptr && k->qname && seen.insert(*k->qname).second; k = program.base_of(*k)) {
            for (Node* m : program.members(*k)) {
                if (const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m)) have.emplace(md->name.value_or(""), md->params.size());
            }
        }
        for (ir::ClassDecl* i : cj_interfaces(program, c)) {
            for (Node* mn : program.members(*i)) {
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
                program.add_member(c, ir::make(program.arena(), std::move(abstract_copy), ir::TypeP{}, mn->line, mn->col));
                have.insert(key);
            }
        }
    }
    prepare_boxed_nulls(program, classes);
}

// ---- boxes that may be null -------------------------------------------------------------------------------------------------

namespace {

bool cj_is_box(const ir::TypeP& t) {
    static const std::unordered_set<std::string_view> boxes = {"java.lang.Integer", "java.lang.Long", "java.lang.Double", "java.lang.Float",
                                                               "java.lang.Boolean", "java.lang.Character", "java.lang.Short", "java.lang.Byte"};
    const std::string* q = cj_qual(t);
    return q != nullptr && boxes.contains(*q);
}

bool cj_is_null_box(const ir::TypeP& t) {
    return cj_is_box(t) && std::get<ir::ClassType>(t->body).nullable;
}

ir::TypeP cj_null_box(const ir::TypeP& t) {
    ir::ClassType c = std::get<ir::ClassType>(t->body);
    c.nullable = true;
    return ir::make_type(std::move(c));
}

// `map.get(k)` of a map whose values are boxes: a key it does not hold gives a null.
bool cj_map_get(const Node* x) {
    const ir::Call* c = ir::get<ir::Call>(x);
    if (c == nullptr || c->sym == nullptr || c->sym->name != "get" || c->args.size() != 1 || !cj_is_box(x->type)) return false;
    const auto fam = cj_family().find(c->sym->owner.value_or(""));
    return fam != cj_family().end() && fam->second == "Map";
}

}  // namespace

void JavaLibrary::prepare_boxed_nulls(cs::Program& program, const std::vector<Node*>& classes) {
    for (Node* cn : classes) {                               // a call whose result is dropped reads no number
        for (Node* n : ir::walk(cn)) {
            if (const auto* s = ir::get<ir::ExprStmt>(n)) read_as_is_.insert(cs::strip(s->expr));
        }
    }
    std::unordered_set<std::string> may;                     // what may hold a null: L:method:name, F:class.field, R:name/n, P:name/n:i
    const auto of_sym = [](const ir::SymP& s, const std::string& scope) -> std::string {
        if (s == nullptr) return {};
        if (s->kind == ir::SymKind::Field) return "F:" + s->owner.value_or("") + "." + s->name;
        if (s->kind == ir::SymKind::Local || s->kind == ir::SymKind::Param) return "L:" + scope + ":" + s->name;
        return {};
    };
    const auto of_result = [](const ir::Sym& s) { return "R:" + s.name + "/" + std::to_string(s.params.size()); };
    const auto of_param = [](const std::string& method, std::size_t arity, std::size_t i) {
        return "P:" + method + "/" + std::to_string(arity) + ":" + std::to_string(i);
    };
    const auto own_method = [&](const ir::Sym& s) { return s.owner && program.class_named(*s.owner) != nullptr; };
    std::function<bool(Node*, const std::string&)> nullish = [&](Node* e, const std::string& scope) -> bool {
        if (e == nullptr) return false;
        Node* x = cs::strip(e);
        if (cj_null_literal(x) || cj_map_get(x)) return true;
        if (const auto* n = ir::get<ir::Name>(x)) return cj_is_box(x->type) && may.contains(of_sym(n->sym, scope));
        if (const auto* s = ir::get<ir::Select>(x)) return cj_is_box(x->type) && may.contains(of_sym(s->sym, scope));
        if (const auto* c = ir::get<ir::Call>(x)) return c->sym != nullptr && cj_is_box(x->type) && own_method(*c->sym) && may.contains(of_result(*c->sym));
        if (const auto* c = ir::get<ir::Cond>(x)) return cj_is_box(x->type) && (nullish(c->then, scope) || nullish(c->orelse, scope));
        if (const auto* a = ir::get<ir::Assign>(x); a != nullptr && !a->op) return nullish(a->value, scope);
        return false;
    };
    bool grew = false;
    const auto mark = [&](std::string key) {
        if (!key.empty() && may.insert(std::move(key)).second) grew = true;
    };
    const auto mark_args = [&](const std::string& method, const std::vector<Node*>& args, const ir::Sym& s, const std::string& scope) {
        for (std::size_t i = 0; i < args.size() && i < s.params.size(); ++i) {
            if (cj_is_box(s.params[i]) && nullish(args[i], scope)) mark(of_param(method, s.params.size(), i));
        }
    };
    // what one node says: `ret` is the method's result while its own body is walked (a lambda's return is the lambda's)
    std::function<void(Node*, const std::string&, const std::string&)> look = [&](Node* n, const std::string& scope, const std::string& ret) {
        if (n == nullptr) return;
        if (const auto* l = ir::get<ir::Local>(n); l != nullptr && cj_is_box(l->type) && nullish(l->init, scope)) mark(of_sym(l->sym, scope));
        if (const auto* a = ir::get<ir::Assign>(n); a != nullptr && !a->op && cj_is_box(a->target->type) && nullish(a->value, scope)) {
            Node* t = cs::strip(a->target);
            if (const auto* name = ir::get<ir::Name>(t)) mark(of_sym(name->sym, scope));
            if (const auto* sel = ir::get<ir::Select>(t)) mark(of_sym(sel->sym, scope));
        }
        if (const auto* r = ir::get<ir::Return>(n); r != nullptr && !ret.empty() && nullish(r->expr, scope)) mark(ret);
        if (const auto* c = ir::get<ir::Call>(n); c != nullptr && c->sym != nullptr && own_method(*c->sym)) mark_args(c->sym->name, c->args, *c->sym, scope);
        if (const auto* w = ir::get<ir::New>(n); w != nullptr && w->sym != nullptr && own_method(*w->sym)) mark_args("<init>" + w->sym->owner.value_or(""), w->args, *w->sym, scope);
        const std::string inner = ir::is<ir::Lambda>(n) ? std::string() : ret;
        for (Node* child : ir::children(*n)) look(child, scope, inner);
    };
    const auto method_id = [](const ir::ClassDecl& c, const ir::MethodDecl& m) {
        return m.kind == "ctor" ? "<init>" + c.qname.value_or("") : m.name.value_or("");
    };
    const auto scope_of = [](const ir::ClassDecl& c, const Node* m) { return c.qname.value_or("") + "#" + std::to_string(reinterpret_cast<std::uintptr_t>(m)); };
    do {
        grew = false;
        for (Node* cn : classes) {
            const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
            for (Node* x : program.members(const_cast<ir::ClassDecl&>(c))) {
                if (const auto* f = ir::get<ir::FieldDecl>(x); f != nullptr && cj_is_box(f->type) && (f->init == nullptr || nullish(f->init, ""))) mark(of_sym(f->sym, ""));
                const auto* m = ir::get<ir::MethodDecl>(x);
                if (m == nullptr) {
                    if (ir::is<ir::FieldDecl>(x)) look(x, "", "");
                    continue;
                }
                const std::string scope = scope_of(c, x);
                for (std::size_t i = 0; i < m->params.size(); ++i) {
                    const auto* p = ir::get<ir::Param>(m->params[i]);
                    if (p != nullptr && cj_is_box(p->type) && may.contains(of_param(method_id(c, *m), m->params.size(), i))) mark(of_sym(p->sym, scope));
                }
                look(x, scope, m->kind == "method" && cj_is_box(m->ret) && m->sym != nullptr ? of_result(*m->sym) : std::string());
            }
        }
    } while (grew);
    if (may.empty()) return;
    // the declarations become `T?`, and each read of one is known
    std::function<void(Node*, const std::string&)> rewrite = [&](Node* n, const std::string& scope) {
        if (n == nullptr) return;
        for (Node* child : ir::children(*n)) rewrite(child, scope);
        if (auto* l = ir::get<ir::Local>(n); l != nullptr && cj_is_box(l->type) && may.contains(of_sym(l->sym, scope))) {
            l->type = cj_null_box(l->type);
        } else if (const auto* name = ir::get<ir::Name>(n); name != nullptr && cj_is_box(n->type) && may.contains(of_sym(name->sym, scope))) {
            n->type = cj_null_box(n->type);
            null_reads_.insert(n);
        } else if (const auto* sel = ir::get<ir::Select>(n); sel != nullptr && cj_is_box(n->type) && may.contains(of_sym(sel->sym, scope))) {
            n->type = cj_null_box(n->type);
            null_reads_.insert(n);
        } else if (auto* call = ir::get<ir::Call>(n); call != nullptr && call->sym != nullptr && own_method(*call->sym)) {
            for (std::size_t i = 0; i < call->sym->params.size(); ++i) {
                if (cj_is_box(call->sym->params[i]) && may.contains(of_param(call->sym->name, call->sym->params.size(), i)))
                    call->sym->params[i] = cj_null_box(call->sym->params[i]);
            }
            if (cj_is_box(n->type) && may.contains(of_result(*call->sym))) {
                n->type = cj_null_box(n->type);
                null_reads_.insert(n);
            }
        } else if (auto* made = ir::get<ir::New>(n); made != nullptr && made->sym != nullptr && own_method(*made->sym)) {
            const std::string id = "<init>" + made->sym->owner.value_or("");
            for (std::size_t i = 0; i < made->sym->params.size(); ++i) {
                if (cj_is_box(made->sym->params[i]) && may.contains(of_param(id, made->sym->params.size(), i)))
                    made->sym->params[i] = cj_null_box(made->sym->params[i]);
            }
        } else if (ir::is<ir::Cond>(n) && cj_is_box(n->type) && nullish(n, scope)) {
            n->type = cj_null_box(n->type);
        }
        // what is assigned to, and a call whose result is dropped, is not a read of the number
        if (const auto* a = ir::get<ir::Assign>(n)) read_as_is_.insert(cs::strip(a->target));
        if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) read_as_is_.insert(cs::strip(u->x));
    };
    for (Node* cn : classes) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        for (Node* x : program.members(c)) {
            if (auto* f = ir::get<ir::FieldDecl>(x)) {
                rewrite(x, "");
                if (cj_is_box(f->type) && may.contains(of_sym(f->sym, ""))) f->type = cj_null_box(f->type);
                continue;
            }
            auto* m = ir::get<ir::MethodDecl>(x);
            if (m == nullptr) continue;
            const std::string scope = scope_of(c, x);
            rewrite(x, scope);
            for (std::size_t i = 0; i < m->params.size(); ++i) {
                auto* p = ir::get<ir::Param>(m->params[i]);
                if (p != nullptr && cj_is_box(p->type) && may.contains(of_sym(p->sym, scope))) p->type = cj_null_box(p->type);
            }
            if (m->kind == "method" && cj_is_box(m->ret) && m->sym != nullptr && may.contains(of_result(*m->sym))) m->ret = cj_null_box(m->ret);
        }
    }
}

bool JavaLibrary::may_be_null(Node* e) const {
    Node* x = cs::strip(e);
    if (null_reads_.contains(x) || cj_map_get(x)) return true;
    const ir::Cond* c = ir::get<ir::Cond>(x);
    return c != nullptr && cj_is_null_box(x->type);
}

std::string JavaLibrary::kept_null(cs::Context& w, Node* e) {
    Node* x = cs::strip(e);
    if (cj_null_literal(x)) return "null";
    if (null_reads_.contains(x)) {
        const bool already = !read_as_is_.insert(x).second;
        std::string text = w.expr(x).text;
        if (!already) read_as_is_.erase(x);
        return text;
    }
    if (cj_map_get(x)) {
        const ir::Call& call = *ir::get<ir::Call>(x);
        w.program().use_runtime("JavaCollections");
        return "JLib.GetBox(" + w.expr(call.target).text + ", " + w.expr(call.args[0]).text + ")";
    }
    if (const ir::Cond* c = ir::get<ir::Cond>(x); c != nullptr && cj_is_box(x->type)) {
        const std::string type = w.ctype(cj_null_box(x->type));
        return "(" + w.wrap(c->cond, cs::kCond + 1) + " ? (" + type + ")" + kept_null(w, c->then) + " : " + kept_null(w, c->orelse) + ")";
    }
    return w.expr(e).text;
}

std::optional<std::string> JavaLibrary::unboxed_read(cs::Context& w, Node* e) {
    if (!null_reads_.contains(e) || read_as_is_.contains(e)) return std::nullopt;
    read_as_is_.insert(e);
    const std::string text = w.expr(e).text;
    read_as_is_.erase(e);
    w.program().use_runtime("JavaCollections");
    return "JLib.Unbox(" + text + ")";
}

// `x += v`, `x++` of a box that may be null: Java unboxes, so a null throws; C#'s lifted operators would leave it null.
std::optional<Ex> JavaLibrary::update(cs::Context& w, Node* e) {
    Node* target = nullptr;
    std::string op;
    Node* value = nullptr;
    bool postfix = false;
    if (const auto* a = ir::get<ir::Assign>(e)) {
        target = cs::strip(a->target);
        op = a->op.value_or("");
        value = a->value;
    } else if (const auto* u = ir::get<ir::Unary>(e)) {
        target = cs::strip(u->x);
        op = u->op == "++" ? "+" : "-";
        postfix = u->postfix;
    }
    if (target == nullptr || !null_reads_.contains(target)) return std::nullopt;
    for (Node* part : ir::walk(target)) {   // (the place is written twice below: what finds it must not run twice)
        const ir::Unary* step = ir::get<ir::Unary>(part);
        if (ir::is<ir::Call>(part) || ir::is<ir::Assign>(part) || ir::is<ir::New>(part) || (step != nullptr && (step->op == "++" || step->op == "--"))) {
            throw Decline("boxed-null", "changing a box that may be null, at a place found by a call");
        }
    }
    w.program().use_runtime("JavaCollections");
    const std::string place = w.expr(target).text;   // (a target is written as it is)
    std::string plain = w.ctype(target->type);
    if (plain.ends_with('?')) plain.pop_back();
    const std::string right = value != nullptr ? w.wrap(value, cs::kUnary) : "1";
    const std::string next = place + " = (" + plain + ")(JLib.Unbox(" + place + ") " + op + " " + right + ")";
    if (postfix) return Ex{"JLib.Before(JLib.Unbox(" + place + "), " + next + ")", cs::kPrimary};
    return Ex{next, cs::kAssign};
}

// ---- types ------------------------------------------------------------------------------------------------------------------

std::optional<std::string> JavaLibrary::type_name(cs::Context& w, const ir::TypeP& t) {
    const std::string& q = std::get<ir::ClassType>(t->body).name;
    const std::vector<ir::TypeP>& type_args = std::get<ir::ClassType>(t->body).args;
    if (w.program().class_named(q) != nullptr) return std::nullopt;
    if (cj_is_null_box(t)) return std::string(cj_types().at(q)) + "?";
    const auto ctypes = [&] {
        std::vector<std::string> out;
        for (const ir::TypeP& a : type_args) out.push_back(w.ctype(a));
        return out;
    };
    if (q == "java.util.stream.Stream" || q == "java.util.stream.IntStream" || q == "java.util.Optional" || q == "java.util.OptionalInt" ||
        q == "java.util.OptionalDouble" || q == "java.util.stream.Collector") {
        w.program().use_runtime("JavaRt");
        w.program().use_runtime("JavaCollections");
        w.program().use_runtime("JavaStreams");
        if (q == "java.util.stream.IntStream") return "JIntStream";
        if (q == "java.util.OptionalInt") return "JOptionalInt";
        if (q == "java.util.OptionalDouble") return "JOptionalDouble";
        if (q == "java.util.stream.Stream" && type_args.size() == 1) return "JStream<" + w.ctype(type_args[0]) + ">";
        if (q == "java.util.Optional" && type_args.size() == 1) return "JOptional<" + w.ctype(type_args[0]) + ">";
        if (q == "java.util.stream.Collector" && type_args.size() == 3) return "JCollector<" + w.ctype(type_args[0]) + ", " + w.ctype(type_args[2]) + ">";
        throw Decline("type", "the raw type " + ir::to_text(*t));
    }
    if (const auto f = cj_functions().find(q); f != cj_functions().end()) {
        const std::vector<std::string> args = f->second.arguments(ctypes());
        if (f->second.generic == "Action" && args.empty()) return "Action";
        w.add_using("System");
        return std::string(f->second.generic) + "<" + text::join(args, ", ") + ">";
    }
    if (const auto g = cj_generic().find(q); g != cj_generic().end()) {
        const std::string generic(g->second);
        w.add_using("System.Collections.Generic");
        if (generic.starts_with("J")) {
            w.program().use_runtime("JavaRt");
            w.program().use_runtime("JavaCollections");
        }
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
    if (needs_outer(c) || has_captures(c)) {
        // Java sets these before the base constructor runs, C# only after it: fine unless that constructor can reach an override that reads them
        const ir::ClassDecl* base = w.program().base_of(c);
        if (base != nullptr && !cj_base_construction_is_closed(w.program(), base)) {
            throw Decline("inner-class", "a class whose base constructor may call back into it before its outer instance and captured locals are set (" + c.name.value_or("") + ")");
        }
    }
    if (c.kind == "enum" && !enum_as_class(c)) {
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
    if (c.kind == "record") out.push_back("sealed");   // (a Java record is final)
    if (!c.outer && !cj_has(out, "public")) out.insert(out.begin(), "internal");
    if (c.outer && !cj_has(out, "public") && !cj_has(out, "protected")) {   // Java's package and private nested classes are named by the methods around them
        std::erase(out, "private");
        out.insert(out.begin(), "internal");
    }
    return out;
}

std::optional<std::string> JavaLibrary::enum_case(const ir::TypeP& scrutinee, const std::string& name) const {
    const std::string* q = cj_qual(scrutinee);
    if (q == nullptr) return std::nullopt;
    const auto found = object_enums_.find(*q);
    if (found == object_enums_.end()) return std::nullopt;
    const auto at = std::find(found->second.begin(), found->second.end(), name);
    return std::to_string(at == found->second.end() ? -1 : at - found->second.begin());
}

std::optional<std::string> JavaLibrary::class_keyword(const ir::ClassDecl& c) {
    if (c.kind == "record" || enum_as_class(c)) return "class";
    return std::nullopt;
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
    if (cj_has(mods, "private") || cj_has(mods, "final") || cj_has(c.mods, "final") || c.kind == "record") return out;
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
    if (s.kind == ir::SymKind::Field && s.owner) {   // Java keeps fields and methods apart; C# forbids a field named as a method of its class
        if (const ir::ClassDecl* c = w.program().class_named(*s.owner)) {
            const bool method_too = std::any_of(c->members.begin(), c->members.end(), [&](Node* m) {
                const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m);
                return md != nullptr && md->name == s.name;
            });
            if (method_too) return cs::ident(s.name) + "_";
        }
    }
    return cs::ident(s.name);
}

std::vector<Node*> JavaLibrary::arrange_members(cs::Context& w, ir::ClassDecl& c, std::vector<Node*> members) {
    cs::Program& program = w.program();
    Arena& arena = program.arena();
    const std::string qname = c.qname.value_or("");
    if (enum_as_class(c)) {   // each constant a static field, first, made in order; javac's values and valueOf are the library's to write
        program.use_runtime("JavaRt");
        program.use_runtime("JavaCollections");
        std::vector<Node*> kept;
        int ordinal = 0;
        const ir::TypeP self = ir::class_type(qname);
        for (Node* mn : members) {
            if (const ir::EnumConst* ec = ir::get<ir::EnumConst>(mn)) {
                Node* made = ir::make(arena, ir::New{.sym = ec->ctor, .args = ec->args}, self);
                enum_made_[made] = {ec->sym->name, ordinal++};
                ir::FieldDecl field;
                field.sym = ir::make_sym(ir::SymKind::Field, ec->sym->name, qname, true, self);
                field.type = self;
                field.init = made;
                field.mods = {"public", "static", "final"};
                kept.push_back(ir::make(arena, std::move(field)));
            }
        }
        for (Node* mn : members) {
            const ir::MethodDecl* md = ir::get<ir::MethodDecl>(mn);
            if (ir::is<ir::EnumConst>(mn) || (md != nullptr && md->kind != "ctor" && cj_has(md->mods, "IMPLICIT"))) continue;
            kept.push_back(mn);
        }
        members = std::move(kept);
    }
    std::vector<Node*> out;
    std::vector<Node*> statics;   // the static fields with an initialiser and the static blocks, in their textual order
    std::vector<Node*> moves;     // the instance fields with an initialiser and the instance blocks
    bool has_block = false;
    const std::string* base = cj_qual(c.extends);
    const bool base_is_ours = c.extends && base != nullptr && program.class_named(*base) != nullptr;
    // a field initialiser that makes a class needing `this` cannot stay one: C# has no `this` there
    const bool init_needs_this = std::any_of(members.begin(), members.end(), [&](Node* mn) {
        const ir::FieldDecl* f = ir::get<ir::FieldDecl>(mn);
        if (f == nullptr || f->init == nullptr || cj_has(f->mods, "static")) return false;
        const std::vector<Node*> nodes = cj_own_nodes(f->init);
        return std::any_of(nodes.begin(), nodes.end(), [&](Node* n) {
            const ir::New* made = ir::get<ir::New>(n);
            if (made == nullptr) return false;
            const ir::ClassDecl* target = nullptr;
            if (made->body != nullptr) {
                const auto found = hoist_of_.find(made->body);
                target = found != hoist_of_.end() ? found->second : nullptr;
            } else if (const std::string* q = cj_qual(n->type)) {
                target = program.class_named(*q);
            }
            return target != nullptr && needs_outer(*target);
        });
    });
    for (Node* mn : members) {
        const ir::MethodDecl* md = ir::get<ir::MethodDecl>(mn);
        if (md != nullptr && md->kind == "static_init") {
            has_block = true;
            statics.push_back(mn);
            continue;   // (not written where it stands)
        }
        if (const ir::FieldDecl* f = ir::get<ir::FieldDecl>(mn)) {
            if (cj_has(f->mods, "static") && f->init != nullptr && !is_const(*f)) statics.push_back(mn);
            if (!cj_has(f->mods, "static") && f->init != nullptr && (base_is_ours || needs_outer(c) || has_captures(c) || init_needs_this)) moves.push_back(mn);
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
    if (c.kind == "record" || needs_outer(c) || has_captures(c)) return true;   // (a record's assigns the components, an inner class's takes the outer instance and the locals it read)
    const auto it = prologue_.find(c.qname.value_or(""));
    return it != prologue_.end() && !it->second.empty();
}

std::vector<Node*> JavaLibrary::ctor_prologue(cs::Context& w, ir::ClassDecl& c, ir::MethodDecl& m, ir::CtorCall* call) {
    if (call != nullptr && call->kind == "this") return {};
    std::vector<Node*> out;
    if (needs_outer(c) && !m.params.empty()) {   // the outer instance first: what follows may use it
        Arena& arena = w.program().arena();
        const ir::SymP& field = inner_.at(*c.qname);
        const ir::Param* given = ir::get<ir::Param>(m.params.front());
        Node* self = ir::make(arena, ir::This{}, ir::class_type(*c.qname));
        Node* target = ir::make(arena, ir::Select{.obj = self, .sym = field}, field->type);
        Node* value = ir::make(arena, ir::Name{.sym = given->sym}, field->type);
        Node* assign = ir::make(arena, ir::Assign{.op = std::nullopt, .target = target, .value = value}, field->type);
        out.push_back(ir::make(arena, ir::ExprStmt{.expr = assign}));
    }
    if (const Hoisted* h = hoisted(c)) {   // then the locals it read, which follow the outer instance among the parameters
        Arena& arena = w.program().arena();
        const std::size_t first = needs_outer(c) ? 1 : 0;
        for (std::size_t i = 0; i < h->fields.size(); ++i) {
            const ir::SymP& field = h->fields[i];
            const ir::Param* given = ir::get<ir::Param>(m.params.at(first + i));
            Node* self = ir::make(arena, ir::This{}, ir::class_type(*c.qname));
            Node* target = ir::make(arena, ir::Select{.obj = self, .sym = field}, field->type);
            Node* value = ir::make(arena, ir::Name{.sym = given->sym}, field->type);
            Node* assign = ir::make(arena, ir::Assign{.op = std::nullopt, .target = target, .value = value}, field->type);
            out.push_back(ir::make(arena, ir::ExprStmt{.expr = assign}));
        }
    }
    const auto it = prologue_.find(c.qname.value_or(""));
    if (it != prologue_.end()) out.insert(out.end(), it->second.begin(), it->second.end());
    return out;
}

// ---- inner classes -------------------------------------------------------------------------------------------------------------

namespace {

// Is `q` this class or one it extends (among the program's classes)?
bool cj_is_a(const cs::Program& program, const ir::ClassDecl* c, const std::string& q) {
    std::set<std::string> seen;
    for (; c != nullptr && c->qname && seen.insert(*c->qname).second; c = program.base_of(*c)) {
        if (*c->qname == q) return true;
    }
    return false;
}

// A class as its own members name it: a generic one with its own type variables.
ir::TypeP cj_own_type(const cs::Program& program, const std::string& qname) {
    const ir::ClassDecl* c = program.class_named(qname);
    if (c == nullptr || c->typeparams.empty()) return ir::class_type(qname);
    ir::ClassType own;
    own.name = qname;
    for (const std::string& t : c->typeparams) own.args.push_back(ir::typevar_type(t));
    return ir::make_type(std::move(own));
}

bool cj_is_inner(const ir::ClassDecl& c) { return c.kind == "class" && c.outer && !cj_has(c.mods, "static"); }

// Does anything in `c` reach an instance of a class around it?
bool cj_reaches_outer(const cs::Program& program, const ir::ClassDecl& c, const std::unordered_map<std::string, ir::SymP>& inner) {
    const auto around = [&](const std::string& q) {
        if (cj_is_a(program, &c, q)) return false;
        for (const ir::ClassDecl* k = c.outer ? program.class_named(*c.outer) : nullptr; k != nullptr; k = k->outer ? program.class_named(*k->outer) : nullptr) {
            if (cj_is_a(program, k, q)) return true;
        }
        return false;
    };
    for (Node* m : c.members) {
        if (const ir::ClassDecl* nested = ir::get<ir::ClassDecl>(m); nested != nullptr && cj_has(nested->mods, "static")) continue;
        for (Node* n : ir::walk(m)) {
            if (const ir::This* th = ir::get<ir::This>(n)) {
                const std::string* q = cj_qual(th->qualifier ? th->qualifier : n->type);
                if (q != nullptr && around(*q)) return true;
            }
            if (const ir::Call* call = ir::get<ir::Call>(n);
                call != nullptr && call->target == nullptr && call->sym != nullptr && !call->sym->is_static && call->sym->owner && around(*call->sym->owner)) {
                return true;
            }
            if (const ir::New* made = ir::get<ir::New>(n); made != nullptr && made->outer == nullptr && made->body == nullptr) {
                const std::string* q = cj_qual(n->type);
                if (q != nullptr && inner.contains(*q)) {
                    const ir::ClassDecl* other = program.class_named(*q);
                    if (other != nullptr && other->outer && around(*other->outer)) return true;
                }
            }
        }
    }
    return false;
}

}  // namespace

const JavaLibrary::Hoisted* JavaLibrary::hoisted(const ir::ClassDecl& c) const {
    if (!c.qname) return nullptr;
    const auto it = hoisted_.find(*c.qname);
    return it != hoisted_.end() && it->second.decl == &c ? &it->second : nullptr;
}

bool JavaLibrary::has_captures(const ir::ClassDecl& c) const {
    const Hoisted* h = hoisted(c);
    return h != nullptr && !h->captures.empty();
}

// A Java Iterable has `iterator()`; .NET's foreach wants GetEnumerator.
// The one method an interface leaves to implement, with what it inherits; null when there are several, or one a generic parent would retype.
const ir::MethodDecl* JavaLibrary::single_abstract(cs::Program& program, const ir::ClassDecl& start) const {
    std::vector<const ir::MethodDecl*> found;
    std::set<std::string> seen;
    bool must_close = false;
    const std::function<bool(const ir::ClassDecl&, bool)> visit = [&](const ir::ClassDecl& k, bool top) {
        if (!k.qname || !seen.insert(*k.qname).second) return true;
        if (!k.typeparams.empty() && !top) return false;
        for (Node* mn : k.members) {
            const ir::MethodDecl* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr || cj_has(m->mods, "static") || cj_has(m->mods, "default") || cj_has(m->mods, "private")) continue;
            const Node* const* one = std::get_if<Node*>(&m->body);
            if (one != nullptr && *one != nullptr) continue;
            if (const auto* many = std::get_if<std::vector<Node*>>(&m->body); many != nullptr && !many->empty()) continue;
            const std::string name = m->name.value_or("");
            if (cj_object_method(name, m->params.size())) continue;
            const bool again = std::any_of(found.begin(), found.end(), [&](const ir::MethodDecl* x) { return x->name == m->name && x->params.size() == m->params.size(); });
            if (!again) found.push_back(m);
        }
        std::vector<ir::TypeP> parents = k.implements;
        if (k.extends) parents.push_back(k.extends);
        for (const ir::TypeP& parent : parents) {
            const std::string* q = cj_qual(parent);
            if (q == nullptr) return false;
            if (*q == "java.lang.Object") continue;
            if (*q == "java.lang.AutoCloseable" || *q == "java.io.Closeable") {
                must_close = true;
            } else if (const ir::ClassDecl* up = program.class_named(*q)) {
                if (!visit(*up, false)) return false;
            } else {
                return false;
            }
        }
        return true;
    };
    if (!visit(start, true) || found.size() != 1 || !found[0]->typeparams.empty()) return nullptr;
    if (must_close && !(found[0]->name == "close" && found[0]->params.empty())) return nullptr;
    return found[0];
}

std::optional<std::string> JavaLibrary::lambda_class(cs::Context& w, const ir::TypeP& type) {
    const std::string* q = cj_qual(type);
    if (q == nullptr || !lambda_interfaces_.contains(*q)) return std::nullopt;
    return w.ctype(type) + ".JFn";
}

std::vector<std::string> JavaLibrary::extra_members(cs::Context& w, ir::ClassDecl& c) {
    if (c.kind == "interface") {
        const auto held = c.qname ? lambda_interfaces_.find(*c.qname) : lambda_interfaces_.end();
        if (held == lambda_interfaces_.end()) return {};
        const ir::MethodDecl& m = *held->second;
        w.add_using("System");
        std::vector<std::string> types, params, names;
        for (std::size_t i = 0; i < m.params.size(); ++i) {
            const std::string t = w.ctype(ir::get<ir::Param>(m.params[i])->type);
            types.push_back(t);
            names.push_back("a" + std::to_string(i));
            params.push_back(t + " " + names.back());
        }
        const std::string result = w.ctype(m.ret);
        if (result != "void") types.push_back(result);
        const std::string word = result == "void" ? "Action" : "Func";
        const std::string fn = types.empty() ? word : word + "<" + text::join(types, ", ") + ">";
        std::string self = cs::ident(c.name.value_or(""));
        if (!c.typeparams.empty()) self += "<" + text::join(c.typeparams, ", ") + ">";
        return {"public sealed class JFn : " + self,
                "{",
                "    private readonly " + fn + " body;",
                "    public JFn(" + fn + " body) { this.body = body; }",
                "    public " + result + " " + member_name(w, *m.sym) + "(" + text::join(params, ", ") + ") => body(" + text::join(names, ", ") + ");",
                "}"};
    }
    if (c.kind != "class" && c.kind != "record") return {};
    for (const ir::TypeP& i : c.implements) {
        const std::string* q = cj_qual(i);
        if (q == nullptr || *q != "java.lang.Iterable") continue;
        const auto& args = std::get<ir::ClassType>(i->body).args;
        const std::string item = args.empty() ? std::string("object") : w.ctype(args[0]);
        w.add_using("System.Collections.Generic");
        w.program().use_runtime("JavaRt");
        w.program().use_runtime("JavaCollections");
        return {"public IEnumerator<" + item + "> GetEnumerator() => JLib.Walk(iterator());",
                "System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() => GetEnumerator();"};
    }
    return {};
}

std::optional<std::string> JavaLibrary::name_text(cs::Context& w, Node* e) {
    if (auto read = unboxed_read(w, e)) return read;
    const auto it = capture_names_.find(e);
    if (it == capture_names_.end()) return std::nullopt;
    return it->second;
}

void JavaLibrary::prepare_local_classes(cs::Program& program, std::vector<Node*>& classes) {
    const jmodel::Model model(program.units());
    Arena& arena = program.arena();
    std::map<std::string, int> local_names;   // (Java may reuse a local class's name in another method; a member's must be unique)
    for (Node* u : program.units()) {
        for (Node* n : ir::walk(u)) {
            if (const ir::LocalClass* lc = ir::get<ir::LocalClass>(n)) ++local_names[ir::get<ir::ClassDecl>(lc->decl)->qname.value_or("")];
        }
    }
    int anonymous = 0;
    for (const auto& entry : model.classes()) {
        const jmodel::ClassInfo& ci = *entry.second;
        if (!(ci.local || ci.anon) || ci.enum_const != nullptr || ci.outer == nullptr) continue;
        const auto refuse = [&](std::string why) { refused_[ci.node] = std::move(why); };
        ir::ClassDecl* around = ci.outer->decl;
        if (ci.outer->local || ci.outer->anon) {
            const auto found = hoist_of_.find(ci.outer->node);
            around = found != hoist_of_.end() ? found->second : nullptr;
        }
        if (around == nullptr || !around->qname) {
            refuse("a class inside a class that could not be written");
            continue;
        }
        ir::ClassDecl copy = *ci.decl;
        std::string name, q;
        Hoisted made;
        if (ci.anon) {
            const ir::New& creation = *ir::get<ir::New>(ci.creation);
            if (creation.outer != nullptr) { refuse("an anonymous class made on an outer instance"); continue; }
            if (creation.sym != nullptr && creation.sym->varargs) { refuse("an anonymous class with a varargs base constructor"); continue; }
            if (std::any_of(copy.members.begin(), copy.members.end(), [](Node* m) { return ir::is<ir::ClassDecl>(m); })) {
                refuse("an anonymous class with a class inside it");
                continue;
            }
            do {
                name = "Anon" + std::to_string(++anonymous);
                q = *around->qname + "." + name;
            } while (program.class_named(q) != nullptr);
            const bool is_interface = !ci.ifaces.empty();
            const std::string target = is_interface ? ci.ifaces[0] : ci.base.value_or("java.lang.Object");
            const ir::TypeP written = copy.extends ? copy.extends : ci.creation->type;
            copy.extends = ir::TypeP{};
            copy.implements.clear();
            if (model.info(target) != nullptr) {
                if (is_interface) copy.implements.push_back(written);
                else copy.extends = written;
            } else if (cj_functions().contains(target) || target == "java.lang.Runnable") {
                const jmodel::Signature* single = jmodel::library_functional(target);
                if (single == nullptr) { refuse("an anonymous " + target); continue; }
                made.delegate_method = single->name;
                const std::string* typed = cj_qual(ci.creation->type);
                made.delegate_type = typed != nullptr && *typed == target ? ci.creation->type : written;
            } else if (target == "java.lang.Comparable" || target == "java.lang.AutoCloseable" || target == "java.io.Closeable" ||
                       target == "java.util.Iterator" || target == "java.lang.Iterable") {
                copy.implements.push_back(written);
            } else if (const auto mapped = cj_types().find(target); mapped != cj_types().end() && mapped->second.ends_with("Exception")) {
                copy.extends = written;
            } else if (target != "java.lang.Object") {
                refuse("an anonymous class of the library type " + target);
                continue;
            }
            std::erase_if(copy.members, [](Node* m) { const ir::MethodDecl* md = ir::get<ir::MethodDecl>(m); return md != nullptr && md->kind == "ctor"; });
            // its one constructor hands its arguments to the base's
            ir::MethodDecl ctor;
            ctor.sym = ir::make_sym(ir::SymKind::Ctor, name, q);
            ctor.name = name;
            ctor.kind = "ctor";
            ctor.ret = ir::void_type();
            std::vector<Node*> forwarded;
            for (std::size_t i = 0; i < creation.args.size(); ++i) {
                const bool typed = creation.sym != nullptr && i < creation.sym->params.size() && creation.sym->params[i] && creation.sym->params[i]->kind() != "typevar";
                const ir::TypeP type = typed ? creation.sym->params[i] : creation.args[i]->type;
                const ir::SymP given = ir::make_sym(ir::SymKind::Param, "a" + std::to_string(i), std::nullopt, false, type);
                ctor.params.push_back(ir::make(arena, ir::Param{.sym = given, .type = type}));
                forwarded.push_back(ir::make(arena, ir::Name{.sym = given}, type));
            }
            std::vector<Node*> body;
            if (!forwarded.empty()) body.push_back(ir::make(arena, ir::CtorCall{.kind = "super", .sym = creation.sym, .args = std::move(forwarded)}));
            ctor.body = ir::make(arena, ir::Block{.stmts = std::move(body)});
            copy.members.push_back(ir::make(arena, std::move(ctor)));
        } else {
            q = ci.decl->qname.value_or("");
            name = ci.decl->name.value_or("");
            if (local_names[q] > 1 || program.class_named(q) != nullptr) {
                refuse("two classes named " + name + " where C# needs one");
                continue;
            }
        }
        copy.name = name;
        copy.qname = q;
        copy.outer = *around->qname;
        if (!ci.needs_outer && copy.kind == "class" && !cj_has(copy.mods, "static")) copy.mods.push_back("static");   // (declared where there is no `this`)
        for (Node* member : copy.members) {   // javac leaves the owner of what such a class declares empty
            for (Node* n : cj_own_nodes(member)) {
                ir::visit(*n, [&](auto& k) {
                    if constexpr (requires { k.sym; }) {
                        ir::Sym* s = k.sym.get();
                        if (s != nullptr && s->owner && s->owner->empty() &&
                            (s->kind == ir::SymKind::Field || s->kind == ir::SymKind::Method || s->kind == ir::SymKind::Ctor)) {
                            s->owner = q;
                        }
                    }
                });
                if (ir::is<ir::This>(n)) {
                    const std::string* t = cj_qual(n->type);
                    if (t != nullptr && t->empty()) n->type = ir::class_type(q);
                }
            }
        }
        Node* node = ir::make(arena, std::move(copy), ir::TypeP{}, ci.node->line, ci.node->col);
        made.decl = ir::get<ir::ClassDecl>(node);
        program.add_class(*around, node);
        hoist_of_[ci.node] = made.decl;
        hoisted_[q] = std::move(made);
        cj_collect(node, classes);
    }
    find_local_captures();
    add_capture_fields(program);
}

// What each class reads from the methods around it: the names its members do not declare, then whatever a class it makes needs from there.
void JavaLibrary::find_local_captures() {
    const auto add = [](Hoisted& h, const std::string& name, const ir::TypeP& type) {
        if (std::any_of(h.captures.begin(), h.captures.end(), [&](const auto& c) { return c.first == name; })) return false;
        h.captures.emplace_back(name, type);
        return true;
    };
    struct Part {
        Hoisted* owner;
        std::vector<Node*> nodes;
        std::set<std::string> declared;
    };
    std::vector<Part> parts;
    for (auto& [q, h] : hoisted_) {
        for (Node* member : h.decl->members) {
            Part part{&h, cj_own_nodes(member), {}};
            part.declared = cj_declared(part.nodes);
            for (Node* n : part.nodes) {
                const ir::Name* read = ir::get<ir::Name>(n);
                if (read == nullptr || !read->sym || part.declared.contains(read->sym->name)) continue;
                add(h, read->sym->name, n->type);
                capture_names_[n] = "cap_" + read->sym->name;
            }
            parts.push_back(std::move(part));
        }
    }
    for (bool changed = true; changed;) {
        changed = false;
        for (Part& part : parts) {
            for (Node* n : part.nodes) {
                const ir::New* made = ir::get<ir::New>(n);
                if (made == nullptr) continue;
                const Hoisted* target = nullptr;
                if (made->body != nullptr) {
                    if (const auto found = hoist_of_.find(made->body); found != hoist_of_.end()) target = hoisted(*found->second);
                } else if (const std::string* q = cj_qual(n->type)) {
                    if (const auto found = hoisted_.find(*q); found != hoisted_.end()) target = &found->second;
                }
                if (target == nullptr) continue;
                const std::vector<std::pair<std::string, ir::TypeP>> wanted = target->captures;   // (it may be this class)
                for (const auto& [name, type] : wanted) {
                    if (part.declared.contains(name)) continue;
                    via_field_[n].insert(name);
                    if (add(*part.owner, name, type)) changed = true;
                }
            }
        }
    }
}

void JavaLibrary::add_capture_fields(cs::Program& program) {
    Arena& arena = program.arena();
    for (auto& [q, h] : hoisted_) {
        if (h.captures.empty()) continue;
        std::sort(h.captures.begin(), h.captures.end(), [](const auto& a, const auto& b) { return a.first < b.first; });   // (the map's order is no order)
        for (const auto& [name, type] : h.captures) {
            if (!type) throw Decline("local-class", "a captured local with no type (" + name + ")");
            const ir::SymP field = ir::make_sym(ir::SymKind::Field, "cap_" + name, q, false, type);
            h.fields.push_back(field);
            program.add_member(*h.decl, ir::make(arena, ir::FieldDecl{.sym = field, .type = type, .mods = {}}));
        }
        for (Node* mn : h.decl->members) {
            ir::MethodDecl* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr || m->kind != "ctor") continue;
            if (m->sym && m->sym->varargs) throw Decline("local-class", "a local class with a varargs constructor (" + h.decl->name.value_or("") + ")");
            std::vector<Node*> params;
            std::vector<Node*> passed;
            std::vector<ir::TypeP> types;
            for (const auto& [name, type] : h.captures) {
                const ir::SymP given = ir::make_sym(ir::SymKind::Param, "cap_" + name + "_arg", std::nullopt, false, type);
                params.push_back(ir::make(arena, ir::Param{.sym = given, .type = type}));
                passed.push_back(ir::make(arena, ir::Name{.sym = given}, type));
                types.push_back(type);
            }
            m->params.insert(m->params.begin(), params.begin(), params.end());
            for (Node* s : ir::statements(m->body)) {
                const ir::Block* block = ir::get<ir::Block>(s);
                for (Node* st : block != nullptr ? block->stmts : std::vector<Node*>{s}) {
                    ir::CtorCall* chained = ir::get<ir::CtorCall>(st);
                    if (chained == nullptr) continue;
                    for (Node* arg : chained->args) {   // (C# has no `this` in a constructor's initialiser: these read its parameters)
                        for (Node* n : cj_own_nodes(arg)) {
                            if (const auto held = capture_names_.find(n); held != capture_names_.end()) held->second += "_arg";
                            if (ir::is<ir::New>(n)) before_base_.insert(n);
                        }
                    }
                    if (chained->kind != "this") continue;
                    chained->args.insert(chained->args.begin(), passed.begin(), passed.end());
                    if (chained->sym) {   // (its parameters line up with its arguments)
                        auto shifted = std::make_shared<ir::Sym>(*chained->sym);
                        shifted->params.insert(shifted->params.begin(), types.begin(), types.end());
                        if (!shifted->refs.empty()) shifted->refs.insert(shifted->refs.begin(), types.size(), shifted->refs.front());
                        chained->sym = std::move(shifted);
                    }
                }
            }
        }
    }
}

void JavaLibrary::prepare_inner_classes(cs::Program& program, const std::vector<Node*>& classes) {
    // which inner classes need their instance: one that makes another that does needs its own, so until nothing changes
    for (bool changed = true; changed;) {
        changed = false;
        for (Node* cn : classes) {
            const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
            if (!cj_is_inner(c) || !c.qname || inner_.contains(*c.qname) || !cj_reaches_outer(program, c, inner_)) continue;
            inner_[*c.qname] = ir::make_sym(ir::SymKind::Field, "outer_", *c.qname, false, cj_own_type(program, *c.outer));
            changed = true;
        }
    }
    for (Node* cn : classes) {   // `outer.new Inner()` hands it one whether it reads it or not
        for (Node* n : ir::walk(cn)) {
            const ir::New* made = ir::get<ir::New>(n);
            const std::string* q = made != nullptr && made->outer != nullptr ? cj_qual(n->type) : nullptr;
            const ir::ClassDecl* c = q != nullptr ? program.class_named(*q) : nullptr;
            if (c != nullptr && c->outer && !inner_.contains(*q)) inner_[*q] = ir::make_sym(ir::SymKind::Field, "outer_", *q, false, cj_own_type(program, *c->outer));
        }
    }
    Arena& arena = program.arena();
    for (Node* cn : classes) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (!needs_outer(c)) continue;
        const ir::SymP& field = inner_.at(*c.qname);
        program.add_member(c, ir::make(arena, ir::FieldDecl{.sym = field, .type = field->type, .mods = {}}));
        for (Node* mn : c.members) {
            ir::MethodDecl* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr || m->kind != "ctor") continue;
            if (m->sym && m->sym->varargs) throw Decline("inner-class", "an inner class with a varargs constructor (" + c.name.value_or("") + ")");
            const ir::SymP given = ir::make_sym(ir::SymKind::Param, "outer_arg", std::nullopt, false, field->type);
            m->params.insert(m->params.begin(), ir::make(arena, ir::Param{.sym = given, .type = field->type}));
            for (Node* s : ir::statements(m->body)) {
                const ir::Block* block = ir::get<ir::Block>(s);
                for (Node* st : block != nullptr ? block->stmts : std::vector<Node*>{s}) {
                    ir::CtorCall* chained = ir::get<ir::CtorCall>(st);
                    if (chained == nullptr || chained->kind != "this") continue;
                    chained->args.insert(chained->args.begin(), ir::make(arena, ir::Name{.sym = given}, field->type));
                    if (chained->sym) {   // (its parameters line up with its arguments)
                        auto shifted = std::make_shared<ir::Sym>(*chained->sym);
                        shifted->params.insert(shifted->params.begin(), field->type);
                        if (!shifted->refs.empty()) shifted->refs.insert(shifted->refs.begin(), shifted->refs.front());
                        chained->sym = std::move(shifted);
                    }
                }
            }
        }
    }
}

std::string JavaLibrary::outer_path(cs::Context& w, const std::string& qname) const {
    const cs::Program& program = w.program();
    const ir::ClassDecl* k = w.current_class();
    std::string path;
    while (k != nullptr && !cj_is_a(program, k, qname)) {
        if (!needs_outer(*k)) return {};   // (not an instance it holds: as written)
        path += path.empty() ? "outer_" : ".outer_";
        k = k->outer ? program.class_named(*k->outer) : nullptr;
    }
    return k != nullptr ? path : std::string();
}

std::optional<std::string> JavaLibrary::this_text(cs::Context& w, Node* e) {
    const ir::This& th = *ir::get<ir::This>(e);
    const std::string* q = cj_qual(th.qualifier ? th.qualifier : e->type);
    if (q == nullptr) return std::nullopt;
    std::string path = outer_path(w, *q);
    if (path.empty()) {
        if (th.qualifier && w.current_class() != nullptr && cj_is_a(w.program(), w.current_class(), *q)) return "this";   // (`Self.this`)
        return std::nullopt;
    }
    return path;
}

std::optional<Ex> JavaLibrary::new_inner(cs::Context& w, Node* e, const ir::ClassDecl& c) {
    const ir::New& made = *ir::get<ir::New>(e);
    std::vector<std::string> parts;
    if (needs_outer(c)) {
        std::string outer;
        if (made.outer != nullptr) {
            outer = "JLib.NonNull(" + w.expr(made.outer).text + ")";   // (Java throws on `null.new Inner()`)
            w.program().use_runtime("JavaRt");
            w.program().use_runtime("JavaCollections");
        } else {
            outer = outer_path(w, *c.outer);
            if (outer.empty()) outer = "this";
        }
        parts.push_back(std::move(outer));
    }
    const Hoisted* local = hoisted(c);
    if (local != nullptr) {
        const auto from_field = via_field_.find(e);
        for (const auto& [name, type] : local->captures) {
            if (from_field != via_field_.end() && from_field->second.contains(name)) {
                parts.push_back("cap_" + name + (before_base_.contains(e) ? "_arg" : ""));
                continue;
            }
            const auto own = std::find_if(local->captures.begin(), local->captures.end(), [&](const auto& x) { return x.first == name; });
            Node* read = ir::make(w.program().arena(), ir::Name{.sym = ir::make_sym(ir::SymKind::Local, own->first)}, type);
            parts.push_back(w.expr(read).text);
        }
    }
    for (std::size_t i = 0; i < made.args.size(); ++i) {
        const bool typed = made.sym != nullptr && i < made.sym->params.size() && made.sym->params[i] && made.sym->params[i]->kind() != "typevar";
        parts.push_back(typed ? w.coerce(made.args[i], made.sym->params[i]) : w.expr(made.args[i]).text);
    }
    const std::string type = made.body != nullptr ? cs::ident(c.name.value_or("")) : w.ctype(e->type);   // (made inside the class around it, whose T it shares)
    const std::string object = "new " + type + "(" + text::join(parts, ", ") + ")";
    if (local != nullptr && !local->delegate_method.empty()) {   // a library functional interface is a delegate here: to this object's one method
        return Ex{"new " + w.ctype(local->delegate_type) + "(" + object + "." + cs::ident(local->delegate_method) + ")", cs::kPrimary};
    }
    return Ex{object, cs::kPrimary};
}

// ---- expressions --------------------------------------------------------------------------------------------------------------

std::optional<Ex> JavaLibrary::select(cs::Context& w, Node* e) {
    if (auto read = unboxed_read(w, e)) return Ex{std::move(*read), cs::kPrimary};
    const ir::Select& sel = *ir::get<ir::Select>(e);
    const ir::SymP& s = sel.sym;
    if (s == nullptr || s->kind != ir::SymKind::Field) return std::nullopt;
    const std::string owner = s->owner.value_or("");
    if (ir::is<ir::This>(sel.obj)) {   // a field of an enclosing instance
        if (const std::optional<std::string> path = this_text(w, sel.obj); path && *path != "this") return Ex{*path + "." + member_name(w, *s), cs::kPrimary};
    }
    if (owner == "java.lang.Character") {   // Java's Unicode categories, by Java's own numbers (.NET numbers them differently)
        static const std::unordered_map<std::string_view, int> categories = {{"UNASSIGNED", 0}, {"UPPERCASE_LETTER", 1}, {"LOWERCASE_LETTER", 2}, {"TITLECASE_LETTER", 3}, {"MODIFIER_LETTER", 4}, {"OTHER_LETTER", 5}, {"NON_SPACING_MARK", 6}, {"ENCLOSING_MARK", 7}, {"COMBINING_SPACING_MARK", 8}, {"DECIMAL_DIGIT_NUMBER", 9}, {"LETTER_NUMBER", 10}, {"OTHER_NUMBER", 11}, {"SPACE_SEPARATOR", 12}, {"LINE_SEPARATOR", 13}, {"PARAGRAPH_SEPARATOR", 14}, {"CONTROL", 15}, {"FORMAT", 16}, {"PRIVATE_USE", 18}, {"SURROGATE", 19}, {"DASH_PUNCTUATION", 20}, {"START_PUNCTUATION", 21}, {"END_PUNCTUATION", 22}, {"CONNECTOR_PUNCTUATION", 23}, {"OTHER_PUNCTUATION", 24}, {"MATH_SYMBOL", 25}, {"CURRENCY_SYMBOL", 26}, {"MODIFIER_SYMBOL", 27}, {"OTHER_SYMBOL", 28}, {"INITIAL_QUOTE_PUNCTUATION", 29}, {"FINAL_QUOTE_PUNCTUATION", 30}};
        if (const auto category = categories.find(s->name); category != categories.end()) return Ex{"(sbyte)" + std::to_string(category->second), cs::kUnary};
    }
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
    if (auto read = unboxed_read(w, e)) return Ex{std::move(*read), cs::kPrimary};
    const ir::Call& call = *ir::get<ir::Call>(e);
    const ir::SymP& s = call.sym;
    if (s == nullptr) return std::nullopt;
    const std::string owner = s->owner.value_or("");
    if (call.target != nullptr && call.target->type && w.program().class_named(owner) == nullptr) {   // a null put into a collection of numbers
        static const std::unordered_set<std::string_view> stores = {"add", "addFirst", "addLast", "offer", "push", "put", "putIfAbsent", "replace", "set"};
        if (const auto* held = std::get_if<ir::ClassType>(&call.target->type->body); held != nullptr && stores.contains(s->name)) {
            const bool of_boxes = std::any_of(held->args.begin(), held->args.end(), [](const ir::TypeP& a) { return cj_is_box(a); });
            const bool given_null = std::any_of(call.args.begin(), call.args.end(), [](Node* a) { return cj_null_literal(cs::strip(a)); });
            if (of_boxes && given_null) throw Decline("boxed-null", "a null held in a collection of " + held->args.back()->simple() + ": the translation keeps those values");
        }
    }
    if (!s->is_static && call.target != nullptr && !ir::is<ir::TypeName>(call.target) && cj_is_box(call.target->type)) {   // a method of a box
        if (s->name == "equals" && call.args.size() == 1) {   // (of another type of box, false)
            const std::string other = may_be_null(call.args[0]) ? kept_null(w, call.args[0]) : w.expr(call.args[0]).text;
            return Ex{"((object)" + w.wrap(call.target, cs::kUnary) + ").Equals((object)" + other + ")", cs::kPrimary};
        }
        if (s->name == "compareTo" && call.args.size() == 1) {
            w.program().use_runtime("JavaCollections");
            return Ex{"JLib.Natural(" + w.expr(call.target).text + ", " + w.expr(call.args[0]).text + ")", cs::kPrimary};
        }
        static const std::unordered_map<std::string_view, std::pair<std::string_view, int>> values = {
            {"byteValue", {"sbyte", 1}}, {"shortValue", {"short", 2}}, {"intValue", {"int", 3}}, {"longValue", {"long", 4}},
            {"floatValue", {"float", 5}}, {"doubleValue", {"double", 6}}};
        static const std::unordered_map<std::string_view, int> ranks = {{"java.lang.Byte", 1}, {"java.lang.Short", 2}, {"java.lang.Integer", 3},
                                                                        {"java.lang.Long", 4}, {"java.lang.Float", 5}, {"java.lang.Double", 6}};
        const auto wanted = values.find(s->name);
        const auto have = ranks.find(*cj_qual(call.target->type));
        if (wanted != values.end() && have != ranks.end() && call.args.empty() && wanted->second.second >= have->second) {   // (a widening: C#'s cast is Java's)
            return Ex{"(" + std::string(wanted->second.first) + ")" + w.wrap(call.target, cs::kUnary), cs::kUnary};
        }
    }
    const ir::ClassDecl* c = w.program().class_named(owner);
    const ir::ClassDecl* object_enum = c != nullptr && enum_as_class(*c) ? c : nullptr;
    if (object_enum == nullptr && call.target != nullptr) {
        const std::string* tq = cj_qual(call.target->type);
        const ir::ClassDecl* tc = tq != nullptr ? w.program().class_named(*tq) : nullptr;
        if (tc != nullptr && enum_as_class(*tc)) object_enum = tc;
    }
    if (object_enum == nullptr && call.target == nullptr && owner == "java.lang.Enum" && w.current_class() != nullptr && enum_as_class(*w.current_class())) {
        object_enum = w.current_class();   // (`name()` inside the enum)
    }
    if (object_enum != nullptr) {
        const std::string tn = w.ctype(ir::class_type(*object_enum->qname));
        const auto receiver = [&] { return call.target != nullptr && !ir::is<ir::TypeName>(call.target) ? w.wrap(call.target, cs::kPrimary) : std::string("this"); };
        if (s->is_static && (s->name == "values" || s->name == "valueOf")) {
            std::vector<std::string> all;
            for (const std::string& n : object_enums_.at(*object_enum->qname)) all.push_back(tn + "." + cs::ident(n));
            const std::string array = "new " + tn + "[] { " + text::join(all, ", ") + " }";
            if (s->name == "values") return Ex{array, cs::kPrimary};
            return Ex{"JLib.EnumOf(" + array + ", " + w.expr(call.args.at(0)).text + ")", cs::kPrimary};
        }
        const bool inherited = owner == "java.lang.Enum" || c == nullptr;
        if (inherited && s->name == "name" && call.args.empty()) return Ex{receiver() + ".JName", cs::kPrimary};
        if (inherited && s->name == "ordinal" && call.args.empty()) return Ex{receiver() + ".JOrdinal", cs::kPrimary};
        if (inherited && s->name == "compareTo" && call.args.size() == 1) return Ex{receiver() + ".CompareTo(" + w.expr(call.args[0]).text + ")", cs::kPrimary};
        if (inherited && s->name == "toString" && call.args.empty()) return Ex{receiver() + ".ToString()", cs::kPrimary};
        if (inherited && s->name == "equals" && call.args.size() == 1) return Ex{receiver() + ".Equals(" + w.expr(call.args[0]).text + ")", cs::kPrimary};
    }
    if (c != nullptr && c->kind == "enum" && s->is_static && (s->name == "values" || s->name == "valueOf")) {
        w.add_using("System");
        const std::string tn = w.ctype(ir::class_type(c->qname.value_or("")));
        if (s->name == "values") return Ex{"Enum.GetValues<" + tn + ">()", cs::kPrimary};
        return Ex{"Enum.Parse<" + tn + ">(" + w.expr(call.args.at(0)).text + ")", cs::kPrimary};
    }
    if (c != nullptr && call.target == nullptr && !s->is_static) {   // a method of an enclosing instance, called by its name alone
        if (!outer_path(w, owner).empty()) {
            Node* outer = ir::make(w.program().arena(), ir::This{}, ir::class_type(owner));
            ir::Call through = call;
            through.target = outer;
            return w.expr(ir::make(w.program().arena(), std::move(through), e->type), statement);
        }
    }
    if (c != nullptr || owner.empty()) return std::nullopt;
    if (owner == "java.lang.Character" && s->name == "getType" && call.args.size() == 1 && w.ctype(call.args[0]->type) == "char") {
        w.program().use_runtime("JavaCharTypes");
        return Ex{"JavaCharTypes.Of(" + w.expr(call.args[0]).text + ")", cs::kPrimary};
    }
    if (owner == "java.lang.Throwable" && call.target != nullptr && (s->name == "getSuppressed" || s->name == "addSuppressed")) {
        w.program().use_runtime("JavaRt");
        w.program().use_runtime("JavaCollections");
        if (s->name == "getSuppressed" && call.args.empty()) return Ex{"JLib.Suppressed(" + w.expr(call.target).text + ")", cs::kPrimary};
        if (s->name == "addSuppressed" && call.args.size() == 1) {
            return Ex{"JLib.AddSuppressed(" + w.expr(call.target).text + ", " + w.expr(call.args[0]).text + ")", cs::kPrimary};
        }
    }
    check_list_change(call);
    if (std::optional<Ex> got = stream_call(w, e)) return got;
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

std::string_view JavaLibrary::list_limit(Node* e) const {
    if (e == nullptr) return {};
    Node* x = cs::strip(e);
    if (const ir::Call* made = ir::get<ir::Call>(x); made != nullptr && made->sym != nullptr) {
        if (made->sym->owner == "java.util.Arrays" && made->sym->name == "asList") return "fixed";
        if (made->sym->owner == "java.util.stream.Stream" && made->sym->name == "toList") return "immutable";
        if (made->sym->owner == "java.util.List" && made->sym->name == "of") return "immutable";
    }
    const std::optional<std::string> held = concrete(x);
    if (held == "java.util.Arrays.asList") return "fixed";
    if (held == "java.util.stream.Stream.toList") return "immutable";
    return {};
}

void JavaLibrary::check_list_change(const ir::Call& call) const {
    const ir::Sym& s = *call.sym;
    const bool utility = s.owner == "java.util.Collections" && (s.name == "sort" || s.name == "reverse" || s.name == "shuffle");
    Node* list = utility ? (call.args.empty() ? nullptr : call.args[0]) : call.target;
    const std::string_view limit = list_limit(list);
    if (limit.empty()) return;
    const bool resizes = !utility && (s.name == "add" || s.name == "addAll" || s.name == "remove" || s.name == "removeAll" || s.name == "removeIf" ||
                                      s.name == "retainAll" || s.name == "clear");
    const bool writes = utility || s.name == "set" || s.name == "sort" || s.name == "replaceAll";
    if (resizes || (limit == "immutable" && writes)) {
        throw Decline("fixed-list", "changing a list Java refuses to change (" + s.name + " on " +
                                        (limit == "fixed" ? "Arrays.asList" : "an immutable list") + "): Java throws, a C# List would not");
    }
}

std::optional<Ex> JavaLibrary::stream_call(cs::Context& w, Node* e) {
    const ir::Call& call = *ir::get<ir::Call>(e);
    const ir::Sym& s = *call.sym;
    const std::string owner = s.owner.value_or("");
    const auto arguments = [&] {
        std::vector<std::string> parts;
        for (Node* a : call.args) parts.push_back(w.expr(a).text);
        return text::join(parts, ", ");
    };
    const auto uses = [&] {
        w.program().use_runtime("JavaRt");
        w.program().use_runtime("JavaCollections");
        w.program().use_runtime("JavaStreams");
        w.add_using("System");
    };
    // the type argument javac inferred for the call's result: what C# cannot infer for an untyped lambda
    const auto result_arg = [&](std::size_t i) {
        const auto* c = e->type ? std::get_if<ir::ClassType>(&e->type->body) : nullptr;
        if (c == nullptr || i >= c->args.size()) throw Decline("library", owner + "." + s.name + " with no inferred type");
        return w.ctype(c->args[i]);
    };
    const auto on_target = [&] {
        uses();
        return Ex{w.wrap(call.target, cs::kPrimary) + "." + s.name + "(" + arguments() + ")", cs::kPrimary};
    };
    const bool is_static = s.is_static || ir::is<ir::TypeName>(call.target);

    if (owner == "java.util.stream.Stream") {
        if (!is_static) return on_target();
        uses();
        return Ex{"JStream<" + result_arg(0) + ">." + s.name + "(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.stream.IntStream") {
        if (!is_static) return on_target();
        uses();
        return Ex{"JIntStream." + s.name + "(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.Optional" || owner == "java.util.OptionalInt" || owner == "java.util.OptionalDouble") {
        if (!is_static) return on_target();
        uses();
        const std::string type = owner == "java.util.Optional" ? "JOptional<" + result_arg(0) + ">" : owner == "java.util.OptionalInt" ? "JOptionalInt" : "JOptionalDouble";
        return Ex{type + "." + s.name + "(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.stream.Collectors") {
        uses();
        return Ex{"JCollectors<" + result_arg(0) + ">." + s.name + "(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.Comparator" && s.name != "compare") {
        uses();
        const std::string args = arguments();
        std::string all = is_static ? args : w.expr(call.target).text;
        if (!is_static && !args.empty()) all += ", " + args;
        return Ex{"JCompare<" + result_arg(0) + ">." + s.name + "(" + all + ")", cs::kPrimary};
    }
    if (owner == "java.util.Arrays" && s.name == "stream" && call.args.size() == 1) {
        uses();
        const auto* array = call.args[0]->type ? std::get_if<ir::ArrayType>(&call.args[0]->type->body) : nullptr;
        if (array != nullptr && array->elem && array->elem->kind() == "i32") return Ex{"JIntStream.of(" + arguments() + ")", cs::kPrimary};
        if (array == nullptr || array->elem->is_prim()) throw Decline("library", "Arrays.stream of this array");
        return Ex{"JStream<" + w.ctype(array->elem) + ">.of(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.List" && s.name == "of" && is_static) {
        uses();
        return Ex{"JLib.listOf<" + result_arg(0) + ">(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.lang.String" && ((s.name == "format" && is_static) || s.name == "formatted")) {
        if (s.name == "format" && !call.args.empty() && !cs::is_string(call.args[0]->type)) throw Decline("library", "String.format with a Locale");
        uses();
        const std::string args = arguments();
        if (s.name == "format") return Ex{"JLib.format(" + args + ")", cs::kPrimary};
        return Ex{"JLib.format(" + w.expr(call.target).text + (args.empty() ? "" : ", " + args) + ")", cs::kPrimary};
    }
    if (owner == "java.io.PrintStream" && (s.name == "printf" || s.name == "format") && call.target != nullptr) {
        if (!call.args.empty() && !cs::is_string(call.args[0]->type)) throw Decline("library", "printf with a Locale");
        uses();
        return Ex{w.wrap(call.target, cs::kPrimary) + ".Write(JLib.format(" + arguments() + "))", cs::kPrimary};
    }
    if (owner == "java.lang.Comparable" && s.name == "compareTo" && call.args.size() == 1 && call.target != nullptr) {   // (a bounded type variable: C# has no bound here)
        uses();
        return Ex{"JLib.Natural(" + w.expr(call.target).text + ", " + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.Iterator") {
        if ((s.name == "hasNext" || s.name == "next") && call.args.empty()) return on_target();
        throw Decline("library", "Iterator." + s.name);
    }
    if (const auto family = cj_family().find(owner); family != cj_family().end() && !is_static) {
        if ((family->second == "List" || family->second == "Set") && s.name == "iterator" && call.args.empty() && call.target != nullptr) {
            uses();
            const std::optional<std::string> walked = iterable(w, call.target);
            return Ex{"JLib.iteratorOf(" + (walked ? *walked : w.expr(call.target).text) + ")", cs::kPrimary};
        }
        if (family->second == "Map" && call.target != nullptr &&
            ((s.name == "merge" && call.args.size() == 3) || (s.name == "putIfAbsent" && call.args.size() == 2) ||
             ((s.name == "computeIfAbsent" || s.name == "computeIfPresent" || s.name == "compute") && call.args.size() == 2))) {
            uses();
            return Ex{"JLib." + s.name + "(" + w.expr(call.target).text + ", " + arguments() + ")", cs::kPrimary};
        }
    }
    if (owner == "java.util.Set" && s.name == "of" && is_static) {
        uses();
        return Ex{"JLib.setOf<" + result_arg(0) + ">(" + arguments() + ")", cs::kPrimary};
    }
    if (owner == "java.util.Map" && s.name == "of" && is_static) {
        uses();
        std::vector<std::string> pairs;
        for (std::size_t i = 0; i + 1 < call.args.size(); i += 2) pairs.push_back("(" + w.expr(call.args[i]).text + ", " + w.expr(call.args[i + 1]).text + ")");
        return Ex{"JLib.mapOf<" + result_arg(0) + ", " + result_arg(1) + ">(" + text::join(pairs, ", ") + ")", cs::kPrimary};
    }
    if (owner == "java.util.Arrays" && s.name == "asList") {
        if (call.args.size() == 1 && call.args[0]->type && call.args[0]->type->kind() == "array") {
            throw Decline("library", "Arrays.asList over an existing array: Java's list writes through to it");
        }
        w.add_using("System.Collections.Generic");
        const std::string type = result_arg(0);
        if (call.args.empty()) return Ex{"new List<" + type + ">()", cs::kPrimary};
        return Ex{"new List<" + type + "> { " + arguments() + " }", cs::kPrimary};
    }
    if (s.name == "stream" && call.args.empty() && call.target != nullptr) {
        const auto fam = cj_family().find(owner);
        if (fam != cj_family().end() && (fam->second == "List" || fam->second == "Set")) {
            if (const std::string* held = cj_qual(call.target->type); held != nullptr && cj_family().contains(*held) && cj_family().at(*held) == "Set") {
                const std::optional<std::string> made = concrete(call.target);
                if (!made || !cj_ordered(*made)) throw Decline("hash-order", "streaming a " + made.value_or("?") + ": Java visits it in hash order, .NET in another");
            }
            uses();
            return Ex{"JLib.stream(" + w.expr(call.target).text + ")", cs::kPrimary};
        }
    }
    // equals and hashCode of a hash collection compare what it holds
    if (call.target != nullptr && ((s.name == "equals" && call.args.size() == 1) || (s.name == "hashCode" && call.args.empty()))) {
        const auto fam = cj_family().find(owner);
        const std::optional<std::string> held = concrete(call.target);
        if (fam != cj_family().end() && (fam->second == "Map" || fam->second == "Set") && held && *held != "java.util.TreeMap" && *held != "java.util.TreeSet" &&
            *held != "java.util.Hashtable") {
            uses();
            if (s.name == "hashCode") return Ex{"JLib.Hash(" + w.expr(call.target).text + ")", cs::kPrimary};
            return Ex{"JLib.Equal(" + w.expr(call.target).text + ", " + arguments() + ")", cs::kPrimary};
        }
    }
    // the statics javac's members for a record call, and the ones a method reference names
    const bool boxed = owner == "java.lang.Integer" || owner == "java.lang.Long" || owner == "java.lang.Short" || owner == "java.lang.Byte" ||
                       owner == "java.lang.Character" || owner == "java.lang.Boolean" || owner == "java.lang.Float" || owner == "java.lang.Double";
    if (s.is_static && ((boxed && s.name == "hashCode") || (owner == "java.util.Objects" && s.name == "hashCode")) && call.args.size() == 1) {
        uses();
        return Ex{"JLib.Hash(" + arguments() + ")", cs::kPrimary};
    }
    if (s.is_static && owner == "java.util.Objects" && s.name == "equals" && call.args.size() == 2) {
        uses();
        return Ex{"JLib.Equal(" + arguments() + ")", cs::kPrimary};
    }
    if (s.is_static && owner == "java.lang.Float" && s.name == "compare" && call.args.size() == 2) {
        w.program().use_runtime("JavaRt");
        return Ex{"JavaRt.CompareDouble(" + arguments() + ")", cs::kPrimary};
    }
    if (s.is_static && (owner == "java.lang.Integer" || owner == "java.lang.Long" || owner == "java.lang.Double") && s.name == "sum" && call.args.size() == 2) {
        return Ex{w.wrap(call.args[0], cs::kAdd) + " + " + w.wrap(call.args[1], cs::kAdd, true), cs::kAdd};
    }
    return std::nullopt;
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
    if (may_be_null(e)) {   // Java writes a null box as "null"
        w.program().use_runtime("JavaRt");
        return "JavaRt.Str((object)" + kept_null(w, e) + ")";
    }
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
    if (may_be_null(e)) return string_of(w, part);
    if (cj_string_value(e)) return std::nullopt;
    if (cj_integral_type(t)) return std::nullopt;
    if (cs::is_string(t) && cj_non_null(e)) return std::nullopt;
    return string_of(w, part);
}

std::optional<Ex> JavaLibrary::new_object(cs::Context& w, Node* e) {
    const ir::TypeP& t = e->type;
    if (const auto constant = enum_made_.find(e); constant != enum_made_.end()) {
        const ir::New& made = *ir::get<ir::New>(e);
        std::vector<std::string> parts;
        for (std::size_t i = 0; i < made.args.size(); ++i) {
            const bool typed = made.sym != nullptr && i < made.sym->params.size() && made.sym->params[i] && made.sym->params[i]->kind() != "typevar";
            parts.push_back(typed ? w.coerce(made.args[i], made.sym->params[i]) : w.expr(made.args[i]).text);
        }
        return Ex{"JLib.EnumMade(new " + w.ctype(t) + "(" + text::join(parts, ", ") + "), \"" + constant->second.first + "\", " + std::to_string(constant->second.second) + ")", cs::kPrimary};
    }
    if (const Node* body = ir::get<ir::New>(e)->body; body != nullptr) {
        const auto found = hoist_of_.find(body);
        if (found != hoist_of_.end()) return new_inner(w, e, *found->second);
        const auto why = refused_.find(body);
        throw Decline("anonymous-class", why != refused_.end() ? why->second : std::string("an anonymous class"));
    }
    const std::string* q = cj_qual(t);
    if (q != nullptr) {
        if (const ir::ClassDecl* own = w.program().class_named(*q); own != nullptr && (needs_outer(*own) || has_captures(*own))) return new_inner(w, e, *own);
    }
    if (q == nullptr || w.program().class_named(*q) != nullptr) return std::nullopt;
    const std::string tn = w.ctype(t);
    const std::vector<Node*>& args = ir::get<ir::New>(e)->args;
    const auto joined = [&] {
        std::vector<std::string> parts;
        for (Node* a : args) parts.push_back(w.expr(a).text);
        return text::join(parts, ", ");
    };
    if (*q == "java.util.TreeMap" || *q == "java.util.TreeSet") {   // ordered as Java orders its keys, not by the culture's rules for text
        const auto* made = std::get_if<ir::ClassType>(&t->body);
        const std::string key = made != nullptr && !made->args.empty() ? w.ctype(made->args[0]) : std::string("object");
        w.program().use_runtime("JavaRt");
        w.program().use_runtime("JavaCollections");
        if (args.empty()) return Ex{"new " + tn + "(JLib.Order<" + key + ">())", cs::kPrimary};
        const std::string* given = args.size() == 1 ? cj_qual(args[0]->type) : nullptr;
        if (given != nullptr && *given == "java.util.Comparator") return Ex{"new " + tn + "(Comparer<" + key + ">.Create(" + w.expr(args[0]).text + "))", cs::kPrimary};
        if (args.size() == 1) return Ex{"new " + tn + "(" + joined() + ", JLib.Order<" + key + ">())", cs::kPrimary};
        throw Decline("library", "new " + *q + " with these arguments");
    }
    if (*q == "java.util.LinkedHashMap" && args.size() == 3) throw Decline("library", "a LinkedHashMap in access order");
    if (cj_generic().contains(*q)) return Ex{"new " + tn + "(" + joined() + ")", cs::kPrimary};   // (a capacity, or the collection copied)
    if (*q == "java.lang.String") {
        if (args.empty()) return Ex{"new string(new char[0])", cs::kPrimary};
        const auto* array = args[0]->type ? std::get_if<ir::ArrayType>(&args[0]->type->body) : nullptr;
        if (array != nullptr && array->elem && array->elem->kind() == "u16" && (args.size() == 1 || args.size() == 3)) return Ex{"new string(" + joined() + ")", cs::kPrimary};
        if (args.size() == 1 && cs::is_string(args[0]->type)) return Ex{"new string(" + w.wrap(args[0], cs::kPrimary) + ".ToCharArray())", cs::kPrimary};   // (a new object, as Java's is)
        throw Decline("library", "new java.lang.String with these arguments");
    }
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
    if (b.op == "==" || b.op == "!=") {
        Node* l = cs::strip(b.left);
        Node* r = cs::strip(b.right);
        if (cj_null_literal(r) && may_be_null(l)) return Ex{kept_null(w, l) + " " + b.op + " null", cs::kEq};
        if (cj_null_literal(l) && may_be_null(r)) return Ex{kept_null(w, r) + " " + b.op + " null", cs::kEq};
        if (cj_is_box(l->type) && cj_is_box(r->type)) {   // two boxes: Java compares the references
            std::string plain = w.ctype(l->type);
            if (plain.ends_with('?')) plain.pop_back();
            w.program().use_runtime("JavaCollections");
            return Ex{std::string(b.op == "!=" ? "!" : "") + "JLib.SameBox<" + plain + ">(" + kept_null(w, l) + ", " + kept_null(w, r) + ")",
                      b.op == "!=" ? cs::kUnary : cs::kPrimary};
        }
    }
    if ((b.op == "==" || b.op == "!=") && cs::is_string(b.left->type) && cs::is_string(b.right->type)) {
        if (cj_null_literal(cs::strip(b.left)) || cj_null_literal(cs::strip(b.right))) return std::nullopt;
        // Java compares the references
        return Ex{"(object)" + w.wrap(b.left, cs::kUnary) + " " + b.op + " (object)" + w.wrap(b.right, cs::kUnary), cs::kEq};
    }
    return std::nullopt;
}

std::optional<std::string> JavaLibrary::coerce(cs::Context& w, Node* e, const ir::TypeP& type) {
    if (cj_is_null_box(type)) return kept_null(w, e);        // a place that can hold the null
    if (cj_is_box(type) && cj_null_literal(cs::strip(e))) {  // (a type argument of a collection stays a value)
        throw Decline("boxed-null", "a null " + type->simple() + " held in a collection or an array: the translation keeps those values");
    }
    const std::string* q = cj_qual(type);
    if (q != nullptr && *q == "java.lang.Object" && may_be_null(e)) return "(object)" + kept_null(w, e);
    return std::nullopt;
}

void JavaLibrary::record(cs::Context&, const ir::SymP& sym, Node* init) {
    Node* x = init != nullptr ? cs::strip(init) : nullptr;
    const std::string* q = x != nullptr && ir::is<ir::New>(x) ? cj_qual(x->type) : nullptr;
    if (sym != nullptr && q != nullptr) concrete_[{sym->owner.value_or(""), sym->name}] = *q;
    if (sym != nullptr && x != nullptr) {   // a list Java will not let the program change
        const std::string_view limit = list_limit(x);
        if (limit == "fixed") concrete_[{sym->owner.value_or(""), sym->name}] = "java.util.Arrays.asList";
        if (limit == "immutable") concrete_[{sym->owner.value_or(""), sym->name}] = "java.util.stream.Stream.toList";
    }
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
        if (lambda_interfaces_.contains(*q)) return;
        throw Decline("lambda", "a lambda of an interface of the program with no single method a delegate can stand for");
    }
    if (!cj_functions().contains(*q) && *q != "java.lang.Runnable") {
        throw Decline("lambda", "a lambda of the interface " + *q + " (no C# delegate for it)");
    }
}

Node* JavaLibrary::method_ref(cs::Context& w, Node* e) {
    const ir::MethodRef& x = *ir::get<ir::MethodRef>(e);
    const ir::SymP& s = x.sym;
    if (s == nullptr || x.mode == "new" || !s->owner) return nullptr;
    const bool in_library = w.program().class_named(*s->owner) == nullptr;
    const bool unbound = ir::is<ir::TypeName>(x.target) && !s->is_static;
    if (!in_library && !unbound) return nullptr;   // (a method of the program: C#'s method group)
    const std::string* interface_name = cj_qual(e->type);
    const jmodel::Signature* single = interface_name != nullptr ? jmodel::library_functional(*interface_name) : nullptr;
    if (single == nullptr) return nullptr;
    if (!s->is_static && !unbound) {   // `out::println`: Java evaluates the receiver once, when the reference is made
        const ir::Select* held = ir::get<ir::Select>(x.target);
        if (held == nullptr || held->sym == nullptr || held->sym->owner != "java.lang.System") {
            throw Decline("method-ref", "a reference to a method of a library object");
        }
    }
    Arena& arena = w.program().arena();
    const std::size_t count = single->params.size();
    const std::size_t receivers = unbound ? 1 : 0;
    std::vector<Node*> params, names;
    for (std::size_t i = 0; i < count; ++i) {
        ir::TypeP type = ir::class_type("java.lang.Object");
        if (unbound && i == 0) type = ir::class_type(*s->owner);
        else if (i >= receivers && i - receivers < s->params.size()) type = s->params[i - receivers];
        const ir::SymP sym = ir::make_sym(ir::SymKind::Param, "a" + std::to_string(i), std::nullopt, false, type);
        params.push_back(ir::make(arena, ir::Param{.sym = sym, .type = type}));
        names.push_back(ir::make(arena, ir::Name{.sym = sym}, type));
    }
    if (count < receivers) return nullptr;
    Node* target = unbound ? names[0] : s->is_static ? ir::make(arena, ir::TypeName{}, ir::class_type(*s->owner)) : x.target;
    std::vector<Node*> args(names.begin() + static_cast<std::ptrdiff_t>(receivers), names.end());
    Node* body = ir::make(arena, ir::Call{.target = target, .sym = s, .args = std::move(args)}, s->ret);
    return ir::make(arena, ir::Lambda{.params = std::move(params), .body = body}, e->type);
}

cs::LibraryFactory factory(const cb::java::apimap::Registry& registry) {
    return [&registry] { return std::make_unique<JavaLibrary>(registry); };
}

}  // namespace cb::translate::backends::csharp_java

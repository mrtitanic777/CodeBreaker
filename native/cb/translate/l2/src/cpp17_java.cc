// cpp17_java.cc -- the C++17 back end's Java rules (Unit.lang 'java', or none): a whole Java program
// from L2, declarations and bodies, as one C++17 translation unit on the runtime
// codebreaker/translate/runtime/cpp17/jrt.hpp. Its choices, each the idiomatic form where it is provably
// the same as Java and the faithful form elsewhere:
//   - references are jrt::ref<T> (std::shared_ptr): reference counting in place of the collector; a class
//     whose fields can lead back to itself can form a cycle that is never freed (Model::cycles lists them,
//     the place a collector would be needed);
//   - a record and any class nothing extends whose constructors never hand `this` out gets real C++
//     constructors (std::make_shared<Point>(x, y)); every other class is made in two phases as the JVM
//     makes it (jrt::make: fields zero, then ctor_), so a virtual call in a constructor reaches the override;
//   - lambdas and method references are C++ lambdas held by the functional interface (jrt::fn<I>), their
//     captures copied ([=]) and `this` held by a strong reference (self_);
//   - for-each is a range-for (jrt::in over an array, jrt::each over an Iterable, a Map's entries bound
//     by a structured binding through jrt::entries);
//   - a switch expression is an immediately invoked lambda, each `yield` its return;
//   - static fields are `static inline` members; a class with a static initialiser runs it on first use;
//   - generics are erased; a value read through a type variable is cast back (jrt::cast);
//   - Java's integer semantics: wrapping arithmetic (-fwrapv), shift counts masked, `/` and `%` by zero
//     throw (jrt::div, jrt::rem), (int) of a double saturates (jrt::d2i);
//   - Java's left-to-right evaluation order where C++'s is unspecified and the order matters (the
//     operands into temporaries, in Java's order);
//   - try/finally through a scope guard (jrt::Finally) when a jump leaves the try, else written twice.
// It declines (Decline) what it cannot write; the judge (javajudge.py) reports a declined program as refused.
#include <algorithm>
#include <cstdio>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "cpp17_jmodel.hpp"
#include "json.hpp"

namespace cpp17 {
namespace {

using jm::ClassInfo;
using jm::OBJECT;
using E = std::pair<std::string, std::string>;     // (code, C++ type)

const std::unordered_set<std::string>& keywords() {
    static const std::unordered_set<std::string> k = [] {
        std::unordered_set<std::string> s;
        const char* words =
            "alignas alignof and and_eq asm auto bitand bitor bool break case catch char char16_t "
            "char32_t class compl const constexpr const_cast continue decltype default delete do double dynamic_cast else enum "
            "explicit export extern false float for friend goto if inline int long mutable namespace new noexcept not not_eq "
            "nullptr operator or or_eq private protected public register reinterpret_cast return short signed sizeof static "
            "static_assert static_cast struct switch template this thread_local throw true try typedef typeid typename union "
            "unsigned using virtual void volatile wchar_t while xor xor_eq NULL "
            "ref O jrt std self_ NULL EOF stdin stdout stderr errno assert offsetof INFINITY NAN";
        for (auto& w : py::split_ws(words)) s.insert(w);
        return s;
    }();
    return k;
}

std::string ident(const std::string& n) { return keywords().count(n) ? n + "_" : py::replace(n, "$", "_"); }

const char* prim(const std::string& k) {
    static const std::unordered_map<std::string, const char*> p = {
        {"bool", "bool"}, {"i8", "int8_t"}, {"i16", "int16_t"}, {"u16", "char16_t"}, {"i32", "int32_t"},
        {"i64", "int64_t"}, {"f32", "float"}, {"f64", "double"}, {"void", "void"}};
    auto it = p.find(k);
    return it == p.end() ? nullptr : it->second;
}

bool numeric(const std::string& c) {
    return c == "int8_t" || c == "int16_t" || c == "char16_t" || c == "int32_t" || c == "int64_t" || c == "float" ||
           c == "double";
}
bool numeric_or_bool(const std::string& c) { return numeric(c) || c == "bool"; }
bool integral(const std::string& c) {
    return c == "int8_t" || c == "int16_t" || c == "char16_t" || c == "int32_t" || c == "int64_t";
}

// java.lang's boxes and the primitive each holds
const char* box(const std::string& q) {
    static const std::unordered_map<std::string, const char*> b = {
        {"java.lang.Integer", "int32_t"}, {"java.lang.Long", "int64_t"},  {"java.lang.Short", "int16_t"},
        {"java.lang.Byte", "int8_t"},     {"java.lang.Character", "char16_t"}, {"java.lang.Boolean", "bool"},
        {"java.lang.Float", "float"},     {"java.lang.Double", "double"}};
    auto it = b.find(q);
    return it == b.end() ? nullptr : it->second;
}

// library classes -> the runtime's (jrt.hpp), in their order
const std::vector<std::pair<std::string, std::string>>& lib() {
    static const std::vector<std::pair<std::string, std::string>> v = [] {
        std::vector<std::pair<std::string, std::string>> m = {
            {"java.lang.Object", "Object"}, {"java.lang.String", "String"}, {"java.lang.CharSequence", "Object"},
            {"java.lang.Integer", "Integer"}, {"java.lang.Long", "Long"}, {"java.lang.Short", "Short"},
            {"java.lang.Byte", "Byte"}, {"java.lang.Character", "Character"}, {"java.lang.Boolean", "Boolean"},
            {"java.lang.Float", "Float"}, {"java.lang.Double", "Double"}, {"java.lang.Number", "Object"},
            {"java.lang.StringBuilder", "StringBuilder"}, {"java.lang.StringBuffer", "StringBuilder"},
            {"java.lang.AbstractStringBuilder", "StringBuilder"}, {"java.lang.Math", "Math"},
            {"java.lang.StrictMath", "Math"}, {"java.lang.System", "System"}, {"java.lang.Comparable", "Comparable"},
            {"java.lang.Iterable", "Iterable"}, {"java.lang.Runnable", "Runnable"}, {"java.lang.Enum", "Enum"},
            {"java.lang.Record", "Object"}, {"java.lang.Throwable", "Throwable"}, {"java.io.PrintStream", "PrintStream"},
            {"java.util.List", "List"}, {"java.util.ArrayList", "ArrayList"}, {"java.util.LinkedList", "LinkedList"},
            {"java.util.Collection", "Collection"}, {"java.util.Iterator", "Iterator"}, {"java.util.Map", "Map"},
            {"java.util.HashMap", "HashMap"}, {"java.util.LinkedHashMap", "LinkedHashMap"},
            {"java.util.TreeMap", "TreeMap"}, {"java.util.Map.Entry", "MapEntry"}, {"java.util.Set", "Set"},
            {"java.util.HashSet", "HashSet"}, {"java.util.LinkedHashSet", "LinkedHashSet"},
            {"java.util.TreeSet", "TreeSet"}, {"java.util.Deque", "LinkedList"}, {"java.util.Queue", "LinkedList"},
            {"java.util.ArrayDeque", "ArrayDeque"}, {"java.util.Arrays", "Arrays"},
            {"java.util.Collections", "Collections"}, {"java.util.Objects", "Objects"},
            {"java.util.Optional", "Optional"}, {"java.util.OptionalInt", "OptionalInt"},
            {"java.util.OptionalDouble", "OptionalDouble"}, {"java.util.Comparator", "Comparator"},
            {"java.util.stream.Stream", "Stream"}, {"java.util.stream.IntStream", "IntStream"},
            {"java.util.stream.Collectors", "Collectors"}, {"java.util.stream.Collector", "Collector"},
        };
        auto setdefault = [&](const std::string& q) {
            for (auto& e : m)
                if (e.first == q) return;
            m.push_back({q, py::rsplit(q, ".", 1).back()});
        };
        for (auto& s : jm::sams()) setdefault(s.first);
        for (auto& s : jm::lib_supers())
            if (ends(s.first, "Exception") || ends(s.first, "Error") || s.first == "java.lang.Throwable") setdefault(s.first);
        return m;
    }();
    return v;
}

const std::string* lib_of(const std::string& q) {
    for (auto& e : lib())
        if (e.first == q) return &e.second;
    return nullptr;
}

// where a library class's static members live in the runtime, when not in the class itself
const char* statics(const std::string& q) {
    static const std::unordered_map<std::string, const char*> s = {
        {"java.lang.Integer", "Integer_"}, {"java.lang.Long", "Long_"},   {"java.lang.Double", "Double_"},
        {"java.lang.Float", "Float_"},     {"java.lang.Short", "Short_"}, {"java.lang.Byte", "Byte_"},
        {"java.lang.Character", "Character"}};
    auto it = s.find(q);
    return it == s.end() ? nullptr : it->second;
}

const char* static_field(const std::string& owner, const std::string& name) {
    if (owner == "java.lang.System" && name == "out") return "System::out_()";
    if (owner == "java.lang.System" && name == "err") return "System::err_()";
    if (owner == "java.lang.Character" && name == "MAX_VALUE") return "Character_::MAX_VALUE";
    if (owner == "java.lang.Character" && name == "MIN_VALUE") return "Character_::MIN_VALUE";
    return nullptr;
}

const std::set<std::string>& pure_lib() {
    static const std::set<std::string> s = {"java.lang.String", "java.lang.StringBuilder", "java.lang.AbstractStringBuilder",
                                            "java.io.PrintStream", "java.lang.Math", "java.lang.Integer", "java.lang.Long",
                                            "java.lang.Double", "java.lang.Character", "java.lang.Boolean", "java.util.Objects"};
    return s;
}

// A Java string as a C++ UTF-16 literal's text.
std::string u16(const std::string& v) {
    std::string out;
    bool prev_hex = false;
    for (size_t i = 0; i < v.size();) {
        uint32_t o = py::decode_cp(v, i);
        if (o == '\\') out += "\\\\";
        else if (o == '"') out += "\\\"";
        else if (o == '\n') out += "\\n";
        else if (o == '\t') out += "\\t";
        else if (o == '\r') out += "\\r";
        else if (o >= 32 && o < 127) {
            char ch = (char)o;
            if (prev_hex && std::strchr("0123456789abcdefABCDEF", ch)) out += std::string("\" u\"") + ch;
            else out += ch;
            prev_hex = false;
            continue;
        } else {
            char b[16];
            std::snprintf(b, sizeof b, "\\x%04x", (unsigned)o);
            out += b;
            prev_hex = true;
            continue;
        }
        prev_hex = false;
    }
    return "u\"" + out + "\"";
}

std::string char_lit(const V& c) {
    long long o;
    if (c.is_str()) {
        size_t i = 0;
        o = c.str().empty() ? 0 : py::decode_cp(c.str(), i);
    } else {
        o = c.as_int();
    }
    if (o >= 32 && o < 127 && o != '\\' && o != '\'') return std::string("u'") + (char)o + "'";
    return "(char16_t)" + std::to_string(o);
}

bool is_ref(const std::string& ct) { return starts(ct, "ref<") || ct == "nullptr_t"; }

std::string cls_of(const std::string& ct) {
    if (starts(ct, "ref<") && ends(ct, ">")) return ct.substr(4, ct.size() - 5);
    return std::string();
}

NodeP strip_paren(NodeP e) {
    while (is(e, "Paren")) e = node(e, "x");
    return e;
}

bool null_lit(const NodeP& e) { return is(e, "Lit") && raw(e, "value").is_none(); }

// synthetic nodes (the field initialisers, a lambda's expression body)
NodeP mk(const char* kind) {
    auto n = std::make_shared<l2::Node>();
    n->cls = l2::node_class(kind);
    n->slots.resize(n->cls->slots.size());
    return n;
}
void put(const NodeP& n, const char* slot, const NodeP& child) {
    l2::Item& it = n->slot(slot);
    it = l2::Item();
    if (child) {
        it.kind = l2::Item::NodeK;
        it.node = child;
    }
}
void put_sym(const NodeP& n, const SymP& s) {
    l2::Item& it = n->slot("sym");
    it = l2::Item();
    if (s) {
        it.kind = l2::Item::SymK;
        it.sym = s;
    }
}
NodeP assign_to(const NodeP& obj, const NodeP& field) {
    // ExprStmt(Assign(op=None, target=Select(obj, sym=f.sym, type=f.type), value=f.init, type=f.type))
    NodeP sel = mk("Select");
    put(sel, "obj", obj);
    put_sym(sel, sym(field));
    set_type(sel, type(field));
    NodeP as = mk("Assign");
    put(as, "target", sel);
    put(as, "value", node(field, "init"));
    set_type(as, type(field));
    NodeP st = mk("ExprStmt");
    put(st, "expr", as);
    return st;
}

// cpp03's helpers the order rule and the finally rule use
std::vector<NodeP> operands(const NodeP& e) {
    std::vector<NodeP> out;
    if (is(e, "Call")) {
        NodeP t = node(e, "target");
        if (t && !is(t, "This") && !is(t, "Super") && !is(t, "TypeName")) out.push_back(t);
        for (auto& a : nodes(e, "args")) out.push_back(a);
    } else if (is(e, "New")) {
        out = nodes(e, "args");
    } else if (is(e, "Binary") && str(raw(e, "op")) != "&&" && str(raw(e, "op")) != "||") {
        out = {node(e, "left"), node(e, "right")};
    } else if (is(e, "Concat")) {
        out = nodes(e, "parts");
    } else if (is(e, "Assign") && is(strip_paren(node(e, "target")), "Index")) {
        out = {node(strip_paren(node(e, "target")), "index"), node(e, "value")};
    }
    return out;
}

bool effects(const NodeP& n) {
    static const std::set<std::string> pure = {"java.lang.String",    "java.lang.Math",   "java.lang.Integer",
                                               "java.lang.Float",     "java.lang.Double", "java.lang.Long",
                                               "java.lang.Character", "java.lang.Boolean", "java.lang.Class",
                                               "java.lang.reflect.Constructor"};
    for (auto& x : walk(n)) {
        if (is(x, "Assign") || is(x, "New")) return true;
        if (is(x, "Unary") && (str(raw(x, "op")) == "++" || str(raw(x, "op")) == "--")) return true;
        if (is(x, "Call") && !(sym(x) && pure.count(sowner(sym(x))))) return true;
    }
    return false;
}

std::set<std::string> written(const NodeP& n) {
    std::set<std::string> out;
    for (auto& x : walk(n)) {
        NodeP t;
        if (is(x, "Assign")) t = node(x, "target");
        else if (is(x, "Unary") && (str(raw(x, "op")) == "++" || str(raw(x, "op")) == "--")) t = node(x, "x");
        t = t ? strip_paren(t) : nullptr;
        if ((is(t, "Name") || is(t, "Select")) && sym(t)) out.insert(sname(sym(t)));
    }
    return out;
}

std::set<std::string> read_names(const NodeP& n) {
    std::set<std::string> out;
    for (auto& x : walk(n))
        if ((is(x, "Name") || is(x, "Select")) && sym(x)) out.insert(sname(sym(x)));
    return out;
}

bool order_matters(const std::vector<NodeP>& ns) {
    std::vector<NodeP> eff;
    for (auto& n : ns)
        if (effects(n)) eff.push_back(n);
    if (eff.size() >= 2) return true;
    if (eff.size() == 1) {
        auto w = written(eff[0]);
        if (w.empty()) return false;
        for (auto& n : ns) {
            if (n.get() == eff[0].get()) continue;
            for (auto& r : read_names(n))
                if (w.count(r)) return true;
        }
    }
    return false;
}

bool is_constant(NodeP n) {
    n = strip_paren(n);
    return is(n, "Lit") || is(n, "ClassLit") || is(n, "This") || is(n, "TypeName") ||
           (is(n, "Select") && is(node(n, "obj"), "TypeName") && sym(n) && skind(sym(n)) == "enumconst");
}

bool leaves_walk(const NodeP& n, int loops, const std::set<std::string>& labels) {
    if (is(n, "Return")) return true;
    if (is(n, "Break") || is(n, "Continue")) {
        std::string l = str(raw(n, "label"));
        return !l.empty() ? !labels.count(l) : loops == 0;
    }
    if (is(n, "Lambda") || is(n, "ClassDecl") || is(n, "LocalClass")) return false;
    int inner = loops + ((is(n, "While") || is(n, "DoWhile") || is(n, "For") || is(n, "ForEach") || is(n, "Switch")) ? 1 : 0);
    std::set<std::string> labs = labels;
    if (is(n, "Labeled")) labs.insert(str(raw(n, "label")));
    for (auto& c : children(n))
        if (leaves_walk(c, inner, labs)) return true;
    return false;
}

bool leaves(const std::vector<NodeP>& stmts) {
    for (auto& s : stmts)
        if (s && leaves_walk(s, 0, {})) return true;
    return false;
}

// A finally body that cannot jump and cannot throw: running it from a scope guard is the same.
bool simple_finally(const NodeP& block) {
    for (auto& n : walk(block)) {
        if (is(n, "Return") || is(n, "Break") || is(n, "Continue") || is(n, "Throw") || is(n, "New") ||
            is(n, "NewArray") || is(n, "Index"))
            return false;
        if (is(n, "Call")) {
            std::string o = sowner(sym(n));
            if (!sym(n) || (o != "java.io.PrintStream" && o != "java.lang.StringBuilder")) return false;
        }
        if (is(n, "Binary") && (str(raw(n, "op")) == "/" || str(raw(n, "op")) == "%")) return false;
    }
    return true;
}

// A static field whose initialiser is a literal number, char or boolean.
bool constant_init(const NodeP& f) {
    NodeP init = node(f, "init");
    while (is(init, "Paren")) init = node(init, "x");
    if (is(init, "Unary") && str(raw(init, "op")) == "-" && is(node(init, "x"), "Lit")) init = node(init, "x");
    return is(init, "Lit") && !raw(init, "value").is_none() && !raw(init, "value").is_str() && type(f) && is_prim(type(f));
}

// Whether This `node` is only used as the receiver of a field read or a call (it does not escape).
bool receiver_only(const NodeP& root, const NodeP& n) {
    for (auto& x : walk(root)) {
        if (is(x, "Select") && node(x, "obj").get() == n.get()) return true;
        if (is(x, "Call") && node(x, "target").get() == n.get()) return true;
    }
    return false;
}

bool simple_code(const std::string& code) {
    return (bool)re::rx("^[A-Za-z_][\\w]*(?:(?:->|\\.)[A-Za-z_]\\w*)*$").match(code);
}

std::string comment_text(const std::string& t) { return py::replace(t, "*/", "* /"); }

// ---------------------------------------------------------------------------------------------- the program
struct Program;

struct Method : Emitter {
    Program& p;
    ClassInfo* ci;
    NodeP m;
    std::map<std::string, std::string> vars;
    std::string ret = "void";
    Strs pending;
    int temps = 0;
    bool order = false;
    std::map<const l2::Node*, E> subst;
    std::string cont_next;   // '' = None
    int lambda_depth = 0;
    bool uses_self = false;
    std::map<std::string, std::string> captures;
    bool is_static;
    Method(Program& prog, ClassInfo* c, const NodeP& md);

    Strs define(bool two, bool clinit);
    Strs ctor_call(const NodeP& call, const std::string& ind, bool two);
    Strs field_inits(const std::string& ind);
    Strs finit();
    Strs clinit();
    std::string constant(const NodeP& f);
    void stmts(const std::vector<NodeP>& ss, Strs& out, const std::string& ind) {
        for (auto& s : ss) stmt(s, out, ind);
    }
    void block(const NodeP& s, Strs& out, const std::string& ind);
    void nested(const NodeP& body, Strs& out, const std::string& level) override { block(body, out, level); }
    void loop_nested(const NodeP& body, Strs& out, const std::string& ind) override;
    std::string cond_text(const NodeP& e) override { return cond(e); }
    void stmt(const NodeP& s, Strs& out, const std::string& ind);
    std::string cond(const NodeP& e);
    void stmt_(const NodeP& s, Strs& out, const std::string& ind);
    bool entry_binding(const NodeP& s, std::map<const l2::Node*, std::string>& uses);
    void foreach(const NodeP& s, Strs& out, const std::string& ind);
    std::string case_label(const NodeP& lab, ClassInfo* en);
    std::pair<std::string, ClassInfo*> switch_head(const NodeP& e);
    void switch_(const NodeP& s, Strs& out, const std::string& ind);
    void try_(const NodeP& s, Strs& out, const std::string& ind);
    void try_catch(const NodeP& s, Strs& out, const std::string& ind);
    E expr(const NodeP& e);
    std::string sequenced(const NodeP& e);
    std::string conv(const std::string& code, const std::string& src, const std::string& dst);
    E x(const NodeP& e);
    E x_lit(const NodeP& e);
    E x_name(const NodeP& e);
    E x_name_of(const std::string& name, const TypeP& t);
    std::string this_x();
    E x_this(const NodeP& e);
    E outer_of(const std::string& q);
    E receiver(const NodeP& obj, const SymP& s);
    E x_select(const NodeP& e);
    E unboxed(const std::string& code, const std::string& ct, const TypeP& t);
    E x_unary(const NodeP& e);
    E x_binary(const NodeP& e);
    E x_concat(const NodeP& e);
    E x_assign(const NodeP& e);
    E x_cond(const NodeP& e);
    E x_cast(const NodeP& e);
    E x_instanceof(const NodeP& e);
    std::string args(const std::vector<NodeP>& as, const Strs& params, const SymP& s);
    E x_call(const NodeP& e);
    E x_new(const NodeP& e);
    E new_local(ClassInfo* c, const NodeP& e);
    E x_newarray(const NodeP& e);
    E x_switchexpr(const NodeP& e);
    E string_switch_expr(const NodeP& e, const std::string& t);
    E functional(const NodeP& e) { return is(e, "Lambda") ? x_lambda(e) : x_methodref(e); }
    void sam_of(const TypeP& t, std::string& name, Strs& ps, std::string& r);
    std::string capture_list(bool inner_uses_self);
    E x_lambda(const NodeP& e);
    E x_methodref(const NodeP& e);
};

struct Program {
    std::vector<NodeP> units;
    std::unique_ptr<jm::Model> model;
    std::string main;
    std::set<std::string> cnames, used, method_names;

    Program(std::vector<NodeP> us, const std::string& mn) : units(std::move(us)), main(mn) {
        model.reset(new jm::Model(units));
        model->name_anonymous();
        for (auto* ci : model->all()) cnames.insert(ci->cname);
        for (auto* ci : model->all())
            for (auto& m : ci->methods()) method_names.insert(str(raw(m, "name")));
    }

    // ---------------------------------------------------------------- names and types
    std::string rt(const std::string& name) {
        if (cnames.count(name)) return "jrt::" + name;
        used.insert(name);
        return name;
    }

    std::string field_name(const std::string& name) {
        std::string n = ident(name);
        return method_names.count(name) ? n + "_" : n;
    }

    std::string cls_name(const std::string& q) {
        ClassInfo* ci = model->info(q);
        if (ci) return ci->cname;
        if (auto l = lib_of(q)) return rt(*l);
        throw Decline("library class " + q);
    }

    std::string ct(const TypeP& t) {
        if (!t) return "void";
        if (auto pr = prim(t->kind)) return pr;
        if (t->kind == "null") return "nullptr_t";
        if (t->kind == "typevar" || t->kind == "wildcard" || t->kind == "any") {
            if (t->kind == "wildcard" && t->has_bound && py::truthy(t->bound) && str(t->bound) == "extends")
                return ct(t->bound_type);
            return "ref<" + rt("Object") + ">";
        }
        if (t->kind == "array") return "ref<" + rt("Array") + "<" + ct(t->elem) + ">>";
        if (t->kind == "class") return "ref<" + cls_name(tname(t)) + ">";
        throw Decline("type " + type_repr(t));
    }

    std::string qname_of_ct(const std::string& c0) {
        std::string c = cls_of(c0);
        if (c.empty()) return std::string();
        if (starts(c, "jrt::")) c = c.substr(5);
        for (auto* ci : model->all())
            if (ci->cname == c) return ci->qname;
        for (auto& e : lib())
            if (e.second == c) return e.first;
        return std::string();
    }

    bool is_sub_ct(const std::string& a, const std::string& b) {
        if (a == b || a == "nullptr_t") return true;
        if (b == "ref<" + rt("Object") + ">") return true;
        if (a.find("Array<") != std::string::npos || b.find("Array<") != std::string::npos) return false;
        std::string qa = qname_of_ct(a), qb = qname_of_ct(b);
        if (qa.empty() || qb.empty()) return false;
        if (qa == "java.util.LinkedList" && (qb == "java.util.Deque" || qb == "java.util.Queue")) return true;
        return model->is_sub(qa, qb);
    }

    // (C++ parameter types, C++ result type) of a program method: the root declaration's, erased
    std::pair<Strs, std::string> sig(ClassInfo* c, const NodeP& m) {
        std::vector<TypeP> ps;
        TypeP r;
        if (!model->overrides(c, m, ps, r)) {
            ps.clear();
            for (auto& p : nodes(m, "params")) ps.push_back(type(p));
            r = type(m, "ret");
        }
        Strs out;
        for (auto& t : ps) out.push_back(ct(jm::erase(t)));
        return {out, str(raw(m, "kind")) == "method" ? ct(jm::erase(r)) : std::string("void")};
    }

    struct CallSig {
        Strs params;
        std::string ret;
        ClassInfo* ci = nullptr;
        NodeP m;
    };
    CallSig call_sig(const SymP& s) {
        CallSig cs;
        auto d = model->method_decl(s);
        if (d.second) {
            auto sg = sig(d.first, d.second);
            cs.params = sg.first;
            cs.ret = sg.second;
            cs.ci = d.first;
            cs.m = d.second;
            return cs;
        }
        for (auto& p : s->params) cs.params.push_back(ct(jm::erase(p)));
        cs.ret = ct(jm::erase(s->ret));
        return cs;
    }

    bool two_phase(ClassInfo* c) {
        if (c->kind == "interface" || c->kind == "enum" || c->anon || c->local || c->needs_outer) return c->kind != "interface";
        if (c->base != OBJECT || (!c->ifaces.empty() && c->kind != "record")) return true;
        if (model->subclassed(c->qname)) return true;
        for (auto& m : c->methods()) {
            NodeP body = node(m, "body");
            if (str(raw(m, "kind")) == "ctor" && body) {
                for (auto& n : walk(body)) {
                    if (is(n, "Lambda") || is(n, "MethodRef") || is(n, "New") || is(n, "LocalClass")) return true;
                    if (is(n, "CtorCall") && starts(str(raw(n, "kind")), "this")) return true;
                    if (is(n, "This") && !receiver_only(body, n)) return true;
                    if (is(n, "Call") && !node(n, "target")) return true;
                }
            }
        }
        for (auto& f : c->fields()) {
            NodeP init = node(f, "init");
            if (init && !(sym(f) && sstatic(sym(f))))
                for (auto& n : walk(init))
                    if (is(n, "This") || is(n, "Lambda") || is(n, "New") || is(n, "Call")) return true;
        }
        return false;
    }

    bool needs_clinit(ClassInfo* c) {
        for (auto& x : c->members()) {
            if (is(x, "MethodDecl") && str(raw(x, "kind")) == "static_init") return true;
            if (is(x, "FieldDecl") && sym(x) && sstatic(sym(x)) && node(x, "init") && !constant_init(x)) return true;
            if (is(x, "EnumConst")) return true;
        }
        return false;
    }

    // ---------------------------------------------------------------- the program
    std::string write() {
        auto ord = ordered();
        Strs body, defs;
        for (auto* c : ord) {
            auto s = struct_(c);
            body.insert(body.end(), s.begin(), s.end());
        }
        for (auto* c : ord) {
            auto d = definitions(c);
            defs.insert(defs.end(), d.begin(), d.end());
        }
        Strs head = {"// written by CodeBreaker's C++17 back end (codebreaker/translate/backends/cpp17.py) from the",
                     "// Java program's L2; it runs on jrt.hpp, the Java library in C++17."};
        auto cyc = model->cycles();
        if (!cyc.empty()) {
            head.push_back("// reference counting cannot free a cycle: these classes can form one, where a collector");
            head.push_back("// (Boehm) would be needed to reclaim it: " + join(cyc, ", "));
        }
        head.push_back("#include \"jrt.hpp\"");
        head.push_back("");
        head.push_back("using jrt::ref;");
        for (auto& n : used)
            if (!cnames.count(n)) head.push_back("using jrt::" + n + ";");
        head.push_back("");
        for (auto* c : ord) head.push_back("struct " + c->cname + ";");
        head.push_back("");
        std::vector<ClassInfo*> mains;
        for (auto* c : ord)
            for (auto& m : c->methods())
                if (str(raw(m, "name")) == "main" && sym(m) && sstatic(sym(m)) && nodes(m, "params").size() == 1) {
                    mains.push_back(c);
                    break;
                }
        Strs tail = {"", "int main(int argc, char** argv) {"};
        if (!mains.empty()) {
            std::string pick = !main.empty() ? main : mains[0]->cname;
            tail.push_back("\tconst char* which = std::getenv(\"JRT_MAIN\");");
            for (auto* c : mains)
                tail.push_back("\tif (which && std::strcmp(which, \"" + py::rsplit(c->qname, ".", 1).back() +
                               "\") == 0) return jrt::run_main(&" + c->cname + "::main, argc, argv);");
            tail.push_back("\treturn jrt::run_main(&" + pick + "::main, argc, argv);");
        } else {
            tail.push_back("\treturn 0;");
        }
        tail.push_back("}");
        Strs all = head;
        all.insert(all.end(), body.begin(), body.end());
        all.insert(all.end(), defs.begin(), defs.end());
        all.insert(all.end(), tail.begin(), tail.end());
        return join(all, "\n") + "\n";
    }

    std::vector<ClassInfo*> ordered() {
        std::vector<ClassInfo*> out;
        std::set<std::string> seen;
        std::function<void(ClassInfo*)> visit = [&](ClassInfo* c) {
            if (seen.count(c->qname)) return;
            seen.insert(c->qname);
            std::vector<std::string> sup;
            if (!c->base.empty()) sup.push_back(c->base);
            sup.insert(sup.end(), c->ifaces.begin(), c->ifaces.end());
            for (auto& q : sup)
                if (ClassInfo* b = model->info(q)) visit(b);
            out.push_back(c);
        };
        std::vector<ClassInfo*> all = model->all();
        std::stable_sort(all.begin(), all.end(), [](ClassInfo* a, ClassInfo* b) {
            if (py::lt(a->decl->line, b->decl->line)) return true;
            if (py::lt(b->decl->line, a->decl->line)) return false;
            return a->cname < b->cname;
        });
        for (auto* c : all) visit(c);
        return out;
    }

    // ---------------------------------------------------------------- declarations
    Strs bases(ClassInfo* c) {
        Strs out;
        if (!c->base.empty() && c->base != OBJECT) out.push_back(cls_name(c->base));
        for (auto& q : c->ifaces) {
            if (q == "java.lang.Comparable") continue;   // compareTo is reached through natural()'s dynamic test
            out.push_back("virtual " + cls_name(q));
        }
        if (c->kind == "interface" || out.empty() || c->base == OBJECT) out.insert(out.begin(), "virtual " + rt("Object"));
        for (auto& q : c->ifaces)
            if (q == "java.lang.Comparable") {
                out.push_back("virtual " + rt("Comparable"));
                break;
            }
        return out;
    }

    std::string param_name(const NodeP& prm, const std::string& t) {
        std::string name = ident(sname(sym(prm)));
        return t != ct(type(prm)) ? name + "_in" : name;
    }

    std::string ctor_params(ClassInfo* c, const NodeP& m) {
        Strs ps;
        if (c->kind == "enum") {
            ps.push_back("ref<" + rt("String") + "> name_in");
            ps.push_back("int32_t ordinal_in");
        }
        for (auto& p : nodes(m, "params")) ps.push_back(ct(type(p)) + " " + ident(sname(sym(p))));
        return join(ps, ", ");
    }

    Strs struct_(ClassInfo* c);
    Strs definitions(ClassInfo* c);
};

Strs Program::struct_(ClassInfo* c) {
    Strs L{"struct " + c->cname + " : " + join(bases(c), ", ") + " {"};
    L.push_back("\tconst char16_t* jclass_() const override { return " + u16(py::strip_chars(c->qname, "<>")) + "; }");
    if (c->needs_outer) L.push_back("\t" + ct(mk_type("class", c->outer->qname)) + " outer_;");
    for (auto& cap : c->captures) L.push_back("\t" + ct(cap.second) + " cap_" + ident(cap.first) + "{};");
    for (auto& f : c->fields()) {
        if (!sym(f)) continue;
        std::string t = ct(type(f));
        std::string name = field_name(sname(sym(f)));
        if (sstatic(sym(f))) {
            if (constant_init(f)) L.push_back("\tstatic inline " + t + " " + name + " = " + Method(*this, c, nullptr).constant(f) + ";");
            else L.push_back("\tstatic inline " + t + " " + name + "{};");
        } else {
            L.push_back("\t" + t + " " + name + "{};");
        }
    }
    std::vector<NodeP> consts;
    for (auto& x : c->members())
        if (is(x, "EnumConst")) consts.push_back(x);
    for (auto& x : consts) L.push_back("\tstatic inline ref<" + c->cname + "> " + ident(sname(sym(x))) + ";");
    if (c->kind == "enum") {
        L.push_back("\tstatic inline ref<" + rt("Array") + "<ref<" + c->cname + ">>> values_;");
        L.push_back("\tstatic ref<" + rt("Array") + "<ref<" + c->cname + ">>> values() { clinit_(); return values_->clone(); }");
        L.push_back("\tstatic ref<" + c->cname + "> valueOf(ref<" + rt("String") + "> n);");
    }
    if (needs_clinit(c)) {
        L.push_back("\tstatic inline bool inited_ = false;");
        L.push_back("\tstatic void clinit_();");
    }
    bool two = two_phase(c);
    std::set<std::string> names;
    for (auto& m : c->methods()) {
        std::string mn = str(raw(m, "name"));
        names.insert(mn);
        if (str(raw(m, "kind")) == "ctor") {
            std::string ps = ctor_params(c, m);
            L.push_back(two ? "\tvoid ctor_(" + ps + ");" : "\t" + c->cname + "(" + ps + ");");
            continue;
        }
        auto sg = sig(c, m);
        Strs params;
        auto& mps = nodes(m, "params");
        for (size_t i = 0; i < sg.first.size() && i < mps.size(); i++)
            params.push_back(sg.first[i] + " " + param_name(mps[i], sg.first[i]));
        bool stat = sym(m) && sstatic(sym(m));
        if (stat) L.push_back("\tstatic " + sg.second + " " + ident(mn) + "(" + join(params, ", ") + ");");
        else if (!node(m, "body")) L.push_back("\tvirtual " + sg.second + " " + ident(mn) + "(" + join(params, ", ") + ") = 0;");
        else
            L.push_back(std::string("\t") + (has_mod(m, "private") ? "" : "virtual ") + sg.second + " " + ident(mn) + "(" +
                        join(params, ", ") + ");");
    }
    bool any_ctor = false, nullary = false;
    for (auto& m : c->methods())
        if (str(raw(m, "kind")) == "ctor") {
            any_ctor = true;
            if (nodes(m, "params").empty()) nullary = true;
        }
    if (two && c->kind != "interface") {
        if (!any_ctor) L.push_back("\ttemplate <class... A> void ctor_(A&&... a);");
        L.push_back("\tvoid finit_();");
    } else if (c->kind != "interface" && !nullary) {
        L.push_back("\t" + c->cname + "() {}");
    }
    // a method declared here hides the base's overloads of the same name (C++ name lookup): bring them in
    for (auto& n : names) {
        auto an = model->ancestors(c->qname);
        for (size_t i = 1; i < an.size(); i++) {
            ClassInfo* b = model->info(an[i]);
            if (!b || b->kind == "interface") continue;
            bool has = false;
            for (auto& m : b->methods())
                if (str(raw(m, "name")) == n && str(raw(m, "kind")) == "method") has = true;
            if (has) {
                L.push_back("\tusing " + b->cname + "::" + ident(n) + ";");
                break;
            }
        }
    }
    if (c->kind == "interface" && model->sam(c->qname)) L.push_back("\ttemplate <class L> struct Of;");
    L.push_back("};");
    if (c->kind == "interface") {
        auto s = model->sam(c->qname);
        if (s) {
            Strs cps;
            for (auto& p : s->params) cps.push_back(ct(jm::erase(p)));
            std::string cr = ct(jm::erase(s->ret));
            Strs params, as;
            for (size_t i = 0; i < cps.size(); i++) {
                params.push_back(cps[i] + " a" + std::to_string(i));
                as.push_back("a" + std::to_string(i));
            }
            L.push_back("template <class L> struct " + c->cname + "::Of : " + c->cname + " {");
            L.push_back("\tL l;");
            L.push_back("\texplicit Of(L x) : l(std::move(x)) {}");
            L.push_back("\t" + cr + " " + ident(s->name) + "(" + join(params, ", ") + ") override { return l(" + join(as, ", ") + "); }");
            L.push_back("};");
        }
    }
    L.push_back("");
    return L;
}

Strs Program::definitions(ClassInfo* c) {
    Strs L;
    bool two = two_phase(c);
    bool clinit = needs_clinit(c);
    for (auto& m : c->methods()) {
        if (!node(m, "body")) continue;
        auto d = Method(*this, c, m).define(two, clinit);
        L.insert(L.end(), d.begin(), d.end());
    }
    if (two && c->kind != "interface") {
        bool any_ctor = false;
        for (auto& m : c->methods()) any_ctor = any_ctor || str(raw(m, "kind")) == "ctor";
        if (!any_ctor) {                // an anonymous class: the super's constructor
            std::string base = (!c->base.empty() && c->base != OBJECT) ? cls_name(c->base) : std::string();
            L.push_back("template <class... A> void " + c->cname + "::ctor_(A&&... a) {");
            if (!base.empty() && !model->is_interface(c->base)) L.push_back("\t" + base + "::ctor_(std::forward<A>(a)...);");
            L.push_back("\tfinit_();");
            L.push_back("}");
        }
        auto f = Method(*this, c, nullptr).finit();
        L.insert(L.end(), f.begin(), f.end());
    }
    if (clinit) {
        auto f = Method(*this, c, nullptr).clinit();
        L.insert(L.end(), f.begin(), f.end());
    }
    if (c->kind == "enum") {
        L.push_back("ref<" + c->cname + "> " + c->cname + "::valueOf(ref<" + rt("String") + "> n) {");
        L.push_back("\tclinit_();");
        L.push_back("\tfor (auto& v : *values_) if (v->name()->equals(n)) return v;");
        L.push_back("\tjrt::raise_msg(std::make_shared<jrt::IllegalArgumentException>(), u\"No enum constant " + c->qname +
                    ".\" + n->s);");
        L.push_back("}");
        L.push_back("");
    }
    return L;
}

// ---------------------------------------------------------------------------------------------- one method
Method::Method(Program& prog, ClassInfo* c, const NodeP& md) : Emitter("\t"), p(prog), ci(c), m(md) {
    if (c)
        for (auto& cap : c->captures) captures[cap.first] = "cap_" + ident(cap.first);
    is_static = m && sym(m) && sstatic(sym(m));
}

Strs Method::define(bool two, bool clinit) {
    Strs L;
    if (str(raw(m, "kind")) == "ctor") {
        std::string ps = p.ctor_params(ci, m);
        L.push_back(two ? "void " + ci->cname + "::ctor_(" + ps + ") {" : ci->cname + "::" + ci->cname + "(" + ps + ") {");
        for (auto& prm : nodes(m, "params")) vars[ident(sname(sym(prm)))] = p.ct(type(prm));
        if (clinit) L.push_back("\tclinit_();");
        std::vector<NodeP> ss = nodes(node(m, "body"), "stmts");
        NodeP first = !ss.empty() && is(ss[0], "CtorCall") ? ss[0] : nullptr;
        if (first) ss.erase(ss.begin());
        bool delegated = first && starts(str(raw(first, "kind")), "this");
        if (ci->kind == "enum") L.push_back("\tjrt::Enum::ctor_(name_in, ordinal_in);");
        if ((first && !ends(str(raw(first, "kind")), ":implicit")) || delegated) {
            auto c = ctor_call(first, "\t", two);
            L.insert(L.end(), c.begin(), c.end());
        } else if (!ci->base.empty() && ci->base != OBJECT && ci->kind != "enum" && two) {
            L.push_back("\t" + p.cls_name(ci->base) + "::ctor_();");
        }
        if (!delegated) {
            if (two) L.push_back("\tfinit_();");
            else {
                auto f = field_inits("\t");
                L.insert(L.end(), f.begin(), f.end());
            }
        }
        ret = "void";
        Strs out;
        stmts(ss, out, "\t");
        L.insert(L.end(), out.begin(), out.end());
        L.push_back("}");
        L.push_back("");
        return L;
    }
    auto sg = p.sig(ci, m);
    ret = sg.second;
    Strs params, entry;
    auto& mps = nodes(m, "params");
    for (size_t i = 0; i < sg.first.size() && i < mps.size(); i++) {
        const std::string& t = sg.first[i];
        std::string name = p.param_name(mps[i], t);
        params.push_back(t + " " + name);
        std::string own = p.ct(type(mps[i]));
        std::string id = ident(sname(sym(mps[i])));
        vars[id] = own;
        if (name != id) entry.push_back("\t" + own + " " + id + " = " + conv(name, t, own) + ";");
    }
    L.push_back(sg.second + " " + ci->cname + "::" + ident(str(raw(m, "name"))) + "(" + join(params, ", ") + ") {");
    if (clinit && is_static) L.push_back("\tclinit_();");
    L.insert(L.end(), entry.begin(), entry.end());
    Strs out;
    stmts(nodes(node(m, "body"), "stmts"), out, "\t");
    L.insert(L.end(), out.begin(), out.end());
    L.push_back("}");
    L.push_back("");
    return L;
}

Strs Method::ctor_call(const NodeP& call, const std::string& ind, bool two) {
    (void)two;
    SymP s = sym(call);
    if (starts(str(raw(call, "kind")), "this")) {
        Strs ps;
        for (auto& x : s->params) ps.push_back(p.ct(x));
        return {ind + "this->ctor_(" + args(nodes(call, "args"), ps, s) + ");"};
    }
    std::string base = ci->base;
    if (base == OBJECT || base.empty()) return {};
    if (node(call, "target")) throw Decline("qualified super(...)");
    auto d = p.model->method_decl(s);
    Strs ps;
    if (d.second)
        for (auto& x : nodes(d.second, "params")) ps.push_back(p.ct(type(x)));
    else
        for (auto& x : s->params) ps.push_back(p.ct(jm::erase(x)));
    std::string a = args(nodes(call, "args"), ps, s);
    return {ind + p.cls_name(base) + "::ctor_(" + a + ");"};
}

Strs Method::field_inits(const std::string& ind) {
    Strs out;
    for (auto& x : ci->members()) {
        if (is(x, "FieldDecl") && node(x, "init") && sym(x) && !sstatic(sym(x))) {
            std::string saved = ret;
            ret = "void";
            NodeP th = mk("This");
            stmt(assign_to(th, x), out, ind);
            ret = saved;
        } else if (is(x, "MethodDecl") && str(raw(x, "kind")) == "init") {
            stmts(nodes(node(x, "body"), "stmts"), out, ind);
        }
    }
    return out;
}

Strs Method::finit() {
    Strs L{"void " + ci->cname + "::finit_() {"};
    auto f = field_inits("\t");
    L.insert(L.end(), f.begin(), f.end());
    L.push_back("}");
    L.push_back("");
    return L;
}

Strs Method::clinit() {
    Strs L{"void " + ci->cname + "::clinit_() {", "\tif (inited_) return;", "\tinited_ = true;"};
    ClassInfo* b = !ci->base.empty() ? p.model->info(ci->base) : nullptr;
    if (b && p.needs_clinit(b)) L.push_back("\t" + b->cname + "::clinit_();");
    is_static = true;
    std::vector<NodeP> consts;
    for (auto& x : ci->members())
        if (is(x, "EnumConst")) consts.push_back(x);
    for (size_t k = 0; k < consts.size(); k++) {
        const NodeP& x = consts[k];
        ClassInfo* sub = node(x, "body") ? p.model->by_decl(node(x, "body").get()) : nullptr;
        std::string target = sub ? sub->cname : ci->cname;
        Strs ctor_ps;
        SymP ctor = sym(x, "ctor");
        if (ctor) {
            auto d = p.model->method_decl(ctor);
            if (d.second)
                for (auto& q : nodes(d.second, "params")) ctor_ps.push_back(p.ct(type(q)));
            else
                for (auto& q : ctor->params) ctor_ps.push_back(p.ct(q));
        }
        std::string a = args(nodes(x, "args"), ctor_ps, ctor);
        L.push_back("\t" + ident(sname(sym(x))) + " = jrt::make<" + target + ">(jrt::lit(" + u16(sname(sym(x))) + "), " +
                    std::to_string(k) + (!a.empty() ? ", " + a : "") + ");");
    }
    if (ci->kind == "enum") {
        Strs ns;
        for (auto& x : consts) ns.push_back(ident(sname(sym(x))));
        L.push_back("\tvalues_ = jrt::array_of<ref<" + ci->cname + ">>({" + join(ns, ", ") + "});");
    }
    for (auto& x : ci->members()) {
        if (is(x, "FieldDecl") && sym(x) && sstatic(sym(x)) && node(x, "init") && !constant_init(x)) {
            Strs out;
            NodeP tn = mk("TypeName");
            set_type(tn, mk_type("class", ci->qname));
            stmt(assign_to(tn, x), out, "\t");
            L.insert(L.end(), out.begin(), out.end());
        } else if (is(x, "MethodDecl") && str(raw(x, "kind")) == "static_init") {
            Strs out;
            stmts(nodes(node(x, "body"), "stmts"), out, "\t");
            L.insert(L.end(), out.begin(), out.end());
        }
    }
    L.push_back("}");
    L.push_back("");
    return L;
}

std::string Method::constant(const NodeP& f) {
    E c = expr(node(f, "init"));
    return conv(c.first, c.second, p.ct(type(f)));
}

// ---- statements
void Method::block(const NodeP& s, Strs& out, const std::string& ind) {
    auto saved = vars;
    if (is(s, "Block")) stmts(nodes(s, "stmts"), out, ind + "\t");
    else if (s) stmt(s, out, ind + "\t");
    vars = saved;
}

void Method::loop_nested(const NodeP& body, Strs& out, const std::string& ind) {
    std::string lab = cont_next;
    cont_next.clear();
    if (lab.empty()) {
        block(body, out, ind);
        return;
    }
    out.push_back(ind + "\t{");
    block(body, out, ind + "\t");
    out.push_back(ind + "\t}");
    out.push_back(ind + "\t" + lab + "_cont:;");
}

void Method::stmt(const NodeP& s, Strs& out, const std::string& ind) {
    // the temporaries Java's evaluation order needs are declared before the statement (pending)
    Strs saved;
    saved.swap(pending);
    bool saved_order = order;
    order = true;
    auto saved_subst = subst;
    Strs sub;
    try {
        stmt_(s, sub, ind);
    } catch (...) {
        pending.swap(saved);
        order = saved_order;
        subst = saved_subst;
        throw;
    }
    Strs decls;
    decls.swap(pending);
    pending.swap(saved);
    order = saved_order;
    subst = saved_subst;
    for (auto& d : decls) out.push_back(ind + d);
    out.insert(out.end(), sub.begin(), sub.end());
}

std::string Method::cond(const NodeP& e0) {
    NodeP e = strip_paren(e0);
    E c = expr(e);
    return conv(c.first, c.second, "bool");
}

void Method::stmt_(const NodeP& s, Strs& out, const std::string& ind) {
    std::string k = s->kind();
    if (k == "Block") {
        emit_block(s, out, ind);
    } else if (k == "Local") {
        std::string name = ident(sname(sym(s)));
        std::string c = p.ct(type(s));
        vars[name] = c;
        NodeP init0 = node(s, "init");
        if (!init0) {
            out.push_back(ind + c + " " + name + "{};");
            return;
        }
        E v = expr(init0);
        NodeP init = strip_paren(init0);
        if (v.second == c && (is(init, "New") || is(init, "Cast")) && is_ref(c))
            out.push_back(ind + "auto " + name + " = " + v.first + ";");   // the type is spelled where it is made
        else
            out.push_back(ind + c + " " + name + " = " + conv(v.first, v.second, c) + ";");
    } else if (k == "ExprStmt") {
        out.push_back(ind + expr(node(s, "expr")).first + ";");
    } else if (k == "If") {
        emit_if(s, out, ind);
    } else if (k == "While") {
        emit_while(s, out, ind);
    } else if (k == "DoWhile") {
        out.push_back(ind + "do {");
        loop_nested(node(s, "body"), out, ind);
        out.push_back(ind + "} while (" + cond(node(s, "cond")) + ");");
    } else if (k == "For") {
        auto saved = vars;
        std::string init;
        auto& inits = nodes(s, "init");
        if (!inits.empty() && is(inits[0], "Local")) {
            std::string c = p.ct(type(inits[0]));
            Strs parts;
            for (auto& l : inits) {
                std::string n = ident(sname(sym(l)));
                vars[n] = c;
                if (node(l, "init")) {
                    E v = expr(node(l, "init"));
                    parts.push_back(n + " = " + conv(v.first, v.second, c));
                } else {
                    parts.push_back(n);
                }
            }
            init = c + " " + join(parts, ", ");
        } else if (!inits.empty()) {
            Strs parts;
            for (auto& i : inits) parts.push_back(expr(node(i, "expr")).first);
            init = join(parts, ", ");
        }
        std::string cnd = node(s, "cond") ? cond(node(s, "cond")) : "";
        Strs upd;
        for (auto& u : nodes(s, "update")) upd.push_back(expr(u).first);
        out.push_back(ind + "for (" + init + "; " + cnd + "; " + join(upd, ", ") + ") {");
        loop_nested(node(s, "body"), out, ind);
        out.push_back(ind + "}");
        vars = saved;
    } else if (k == "ForEach") {
        foreach(s, out, ind);
    } else if (k == "Labeled") {
        std::string lab = str(raw(s, "label"));
        bool cont = false, brk = false;
        for (auto& n : walk(node(s, "body"))) {
            if ((is(n, "Break") || is(n, "Continue")) && str(raw(n, "label")) == lab && raw(n, "label").is_str()) {
                if (is(n, "Continue")) cont = true;
                else brk = true;
            }
        }
        if (cont) cont_next = lab;
        stmt(node(s, "body"), out, ind);
        if (brk) out.push_back(ind + lab + "_brk:;");
    } else if (k == "Return") {
        NodeP e = node(s, "expr");
        if (!e) out.push_back(ind + "return;");
        else {
            E v = expr(e);
            out.push_back(ind + "return " + conv(v.first, v.second, ret) + ";");
        }
    } else if (k == "Break") {
        std::string l = str(raw(s, "label"));
        out.push_back(ind + (!l.empty() ? "goto " + l + "_brk;" : "break;"));
    } else if (k == "Continue") {
        std::string l = str(raw(s, "label"));
        out.push_back(ind + (!l.empty() ? "goto " + l + "_cont;" : "continue;"));
    } else if (k == "Throw") {
        out.push_back(ind + "jrt::raise(" + expr(node(s, "expr")).first + ");");
    } else if (k == "Switch") {
        switch_(s, out, ind);
    } else if (k == "Try") {
        try_(s, out, ind);
    } else if (k == "Empty") {
        out.push_back(ind + ";");
    } else if (k == "Sync") {
        out.push_back(ind + "{   // synchronized: one thread here");
        block(node(s, "body"), out, ind);
        out.push_back(ind + "}");
    } else if (k == "Assert") {
        out.push_back(ind + "// assert " + comment_text(expr(node(s, "cond")).first) + " (Java runs without -ea: not checked)");
    } else if (k == "LocalClass") {
        // declared at the top level (Model)
    } else if (k == "Yield") {
        E v = expr(node(s, "expr"));
        out.push_back(ind + "return " + conv(v.first, v.second, ret) + ";");
    } else if (k == "CtorCall") {
        throw Decline("constructor call not first");
    } else {
        throw Decline("statement " + k);
    }
}

// for (Map.Entry<K, V> e : m.entrySet()) that only reads e.getKey() / e.getValue() and touches no map
bool Method::entry_binding(const NodeP& s, std::map<const l2::Node*, std::string>& uses) {
    NodeP it = strip_paren(node(s, "iterable"));
    NodeP var = node(s, "var");
    TypeP vt = type(var);
    if (!(is(it, "Call") && sym(it) && sname(sym(it)) == "entrySet" && nodes(it, "args").empty() && vt &&
          vt->kind == "class" && tname(vt) == "java.util.Map.Entry" && vt->args.size() == 2))
        return false;
    std::string name = sname(sym(var));
    NodeP body = node(s, "body");
    auto all = walk(body);
    std::unordered_map<const l2::Node*, NodeP> parents;
    for (auto& n : all)
        for (auto& c : children(n)) parents[c.get()] = n;
    for (auto& n : all) {
        if (is(n, "Name") && sym(n) && sname(sym(n)) == name) {
            auto pi = parents.find(n.get());
            NodeP call = pi == parents.end() ? nullptr : pi->second;
            if (!(is(call, "Call") && node(call, "target").get() == n.get() && sym(call) &&
                  (sname(sym(call)) == "getKey" || sname(sym(call)) == "getValue") && nodes(call, "args").empty()))
                return false;
            uses[call.get()] = sname(sym(call));
        }
        if (is(n, "Call") && sym(n) && sname(sym(n)) != "getKey" && sname(sym(n)) != "getValue" &&
            !pure_lib().count(sowner(sym(n))))
            return false;
        if (is(n, "Assign") && !is(strip_paren(node(n, "target")), "Name")) return false;
        if (is(n, "Lambda") || is(n, "MethodRef") || is(n, "New")) {
            if (!(is(n, "New") && !node(n, "body") && type(n) && pure_lib().count(tname(type(n))))) return false;
        }
    }
    return true;
}

void Method::foreach(const NodeP& s, Strs& out, const std::string& ind) {
    std::map<const l2::Node*, std::string> uses;
    NodeP var = node(s, "var");
    if (entry_binding(s, uses)) {
        E mm = expr(node(strip_paren(node(s, "iterable")), "target"));
        TypeP vt = type(var);
        std::string kc = p.ct(jm::erase(vt->args[0])), vc = p.ct(jm::erase(vt->args[1]));
        std::string base = ident(sname(sym(var)));
        std::string kn = base + "_key", vn = base + "_value";
        out.push_back(ind + range_for(binding({kn, vn}), "jrt::entries<" + cls_of(kc) + ", " + cls_of(vc) + ">(" + mm.first + ")"));
        vars[kn] = kc;
        vars[vn] = vc;
        auto saved = subst;
        for (auto& n : walk(node(s, "body"))) {
            auto u = uses.find(n.get());
            if (u != uses.end()) subst[n.get()] = u->second == "getKey" ? E(kn, kc) : E(vn, vc);
        }
        loop_nested(node(s, "body"), out, ind);
        subst = saved;
        out.push_back(ind + "}");
        return;
    }
    E coll = expr(node(s, "iterable"));
    std::string name = ident(sname(sym(var)));
    std::string vt = p.ct(type(var));
    vars[name] = vt;
    TypeP t = type(node(s, "iterable"));
    if (t && t->kind == "array") {
        std::string et = p.ct(t->elem);
        if (et == vt) {
            out.push_back(ind + range_for(vt + " " + name, "jrt::in(" + coll.first + ")"));
        } else {
            out.push_back(ind + range_for(et + " " + name + "_e", "jrt::in(" + coll.first + ")"));
            out.push_back(ind + "\t" + vt + " " + name + " = " + conv(name + "_e", et, vt) + ";");
        }
    } else {
        if (vt == "ref<" + p.rt("Object") + ">") {
            out.push_back(ind + range_for(vt + " " + name, "jrt::in(" + coll.first + ")"));
        } else {
            std::string elem = !is_ref(vt) ? vt : cls_of(vt);
            out.push_back(ind + range_for(vt + " " + name, "jrt::each<" + elem + ">(" + coll.first + ")"));
        }
    }
    loop_nested(node(s, "body"), out, ind);
    out.push_back(ind + "}");
}

std::string Method::case_label(const NodeP& lab0, ClassInfo* en) {
    NodeP lab = strip_paren(lab0);
    if (en) {
        std::string name;
        if (is(lab, "Select") && sym(lab) && skind(sym(lab)) == "enumconst") name = sname(sym(lab));
        else if (is(lab, "Name")) name = sname(sym(lab));
        else throw Decline("enum case label");
        int k = 0, at = -1;
        for (auto& x : en->members())
            if (is(x, "EnumConst")) {
                if (sname(sym(x)) == name && at < 0) at = k;
                k++;
            }
        if (at < 0) throw std::runtime_error("ValueError: " + name + " is not in list");
        return std::to_string(at) + " /* " + name + " */";
    }
    if (is(lab, "Lit")) return expr(lab).first;
    if (is(lab, "Select") && sym(lab) && skind(sym(lab)) == "field") {
        ClassInfo* c = p.model->info(sowner(sym(lab)));
        if (c)
            for (auto& f : c->fields())
                if (sym(f) && sname(sym(f)) == sname(sym(lab))) {
                    if (node(f, "init")) return case_label(node(f, "init"), nullptr);
                    break;
                }
    }
    if (is(lab, "Unary") && str(raw(lab, "op")) == "-" && is(node(lab, "x"), "Lit")) return "-" + expr(node(lab, "x")).first;
    throw Decline("case label");
}

std::pair<std::string, ClassInfo*> Method::switch_head(const NodeP& e) {
    E v = expr(strip_paren(e));
    ClassInfo* en = p.model->of_type(type(e));
    if (en && en->kind == "enum") return {"jrt::nn(" + v.first + ")->ordinal()", en};
    if (is_ref(v.second)) {
        if (v.second == "ref<" + p.rt("String") + ">") throw Decline("switch on a String not lowered");
        const char* b = box(tname(type(e)));
        return {"jrt::unbox<" + std::string(b ? b : "int32_t") + ">(" + v.first + ")", nullptr};
    }
    return {v.first, nullptr};
}

void Method::switch_(const NodeP& s, Strs& out, const std::string& ind) {
    auto h = switch_head(node(s, "expr"));
    out.push_back(ind + "switch (" + h.first + ") {");
    for (auto& c : nodes(s, "cases")) {
        for (auto& lab : nodes(c, "labels")) out.push_back(ind + "case " + case_label(lab, h.second) + ":");
        if (py::truthy(raw(c, "default"))) out.push_back(ind + "default:");
        out.push_back(ind + "{");
        auto saved = vars;
        for (auto& b : nodes(c, "body")) stmt(b, out, ind + "\t");
        vars = saved;
        out.push_back(ind + "}");
    }
    out.push_back(ind + "}");
}

void Method::try_(const NodeP& s, Strs& out, const std::string& ind) {
    auto& res = nodes(s, "resources");
    if (!res.empty()) {
        out.push_back(ind + "{");
        std::string inner = ind + "\t";
        auto saved = vars;
        Strs names;
        for (auto& r : res) {
            if (is(r, "Local")) {
                stmt(r, out, inner);
                names.push_back(ident(sname(sym(r))));
            } else {
                E v = expr(node(r, "expr"));
                temps++;
                std::string n = "j2x_res" + std::to_string(temps);
                out.push_back(inner + "auto " + n + " = " + v.first + ";");
                names.push_back(n);
            }
        }
        for (auto& n : names) {         // closed in reverse order of opening: the guards' order
            temps++;
            out.push_back(inner + "jrt::Finally j2x_close" + std::to_string(temps) + "([&] { if (" + n + ") " + n + "->close(); });");
        }
        if (!nodes(s, "catches").empty() || node(s, "final")) throw Decline("try-with-resources with catch or finally");
        block(node(s, "body"), out, ind);
        vars = saved;
        out.push_back(ind + "}");
        return;
    }
    NodeP fin = node(s, "final");
    if (fin) {
        std::vector<NodeP> parts{node(s, "body")};
        for (auto& c : nodes(s, "catches")) parts.push_back(node(c, "body"));
        if (!leaves(parts)) {
            // no jump out: the finally on the exceptional way out (then rethrown) and after, written twice
            out.push_back(ind + "try {");
            try_catch(s, out, ind + "\t");
            out.push_back(ind + "} catch (...) {");
            block(fin, out, ind);
            out.push_back(ind + "\tthrow;");
            out.push_back(ind + "}");
            out.push_back(ind + "{");
            block(fin, out, ind);
            out.push_back(ind + "}");
            return;
        }
        if (!simple_finally(fin)) throw Decline("a finally left by a jump that can itself throw or jump");
        out.push_back(ind + "{");
        temps++;
        Strs f;
        block(fin, f, ind + "\t");
        out.push_back(ind + "\tjrt::Finally j2x_fin" + std::to_string(temps) + "([&] {");
        for (auto& x : f) out.push_back("\t" + x);
        out.push_back(ind + "\t});");
        try_catch(s, out, ind + "\t");
        out.push_back(ind + "}");
        return;
    }
    try_catch(s, out, ind);
}

void Method::try_catch(const NodeP& s, Strs& out, const std::string& ind) {
    auto& catches = nodes(s, "catches");
    if (catches.empty()) {
        out.push_back(ind + "{");
        block(node(s, "body"), out, ind);
        out.push_back(ind + "}");
        return;
    }
    out.push_back(ind + "try {");
    block(node(s, "body"), out, ind);
    temps++;
    std::string t = "j2x_t" + std::to_string(temps);
    out.push_back(ind + "} catch (jrt::JavaThrow& " + t + ") {");
    bool first = true;
    for (auto& c : catches) {
        std::string name = ident(sname(sym(c)));
        auto ts = types(c, "types");
        std::string test, c_t;
        if (ts.size() == 1) {
            if (!(ts[0] && ts[0]->kind == "class")) throw Decline("catch type");
            std::string cls = p.cls_name(tname(ts[0]));
            test = "auto " + name + " = jrt::as<" + cls + ">(" + t + ".t)";
            c_t = "ref<" + cls + ">";
        } else {
            Strs tests;
            for (auto& x : ts) tests.push_back("jrt::isa<" + p.cls_name(tname(x)) + ">(" + t + ".t)");
            test = join(tests, " || ");
            c_t = "ref<" + p.rt("Throwable") + ">";
        }
        out.push_back(ind + "\t" + (first ? "" : "} else ") + "if (" + test + ") {");
        first = false;
        auto saved = vars;
        vars[name] = c_t;
        if (ts.size() > 1) out.push_back(ind + "\t\t" + c_t + " " + name + " = " + t + ".t;");
        block(node(c, "body"), out, ind + "\t");
        vars = saved;
    }
    out.push_back(ind + "\t} else {");
    out.push_back(ind + "\t\tthrow;");
    out.push_back(ind + "\t}");
    out.push_back(ind + "}");
}

// ---- expressions -> (code, C++ type)
E Method::expr(const NodeP& e) {
    auto it = subst.find(e.get());
    if (it != subst.end()) return it->second;
    static const std::set<std::string> known = {"Paren", "Lit",    "Name",   "This",     "Select",     "TypeName",
                                                "Length", "Index", "Unary",  "Binary",   "Concat",     "Assign",
                                                "Cond",  "Cast",   "InstanceOf", "Call", "New", "NewArray",
                                                "ClassLit", "SwitchExpr", "Lambda", "MethodRef"};
    std::string k = e ? e->kind() : std::string("NoneType");
    if (!known.count(k)) throw Decline("expression " + k);
    std::string pre = order ? sequenced(e) : std::string();
    E r = x(e);
    if (!pre.empty()) return E("(" + pre + ", " + r.first + ")", r.second);
    return r;
}

std::string Method::sequenced(const NodeP& e) {
    if (is(e, "Lambda") || is(e, "SwitchExpr")) return std::string();
    auto ns = operands(e);
    if (ns.size() < 2 || !order_matters(ns)) return std::string();
    Strs parts;
    for (auto& n : ns) {
        if (is_constant(n) || subst.count(n.get()) || is(n, "Lambda") || is(n, "MethodRef")) continue;
        E v = expr(n);
        if (v.second == "void" || v.second == "nullptr_t") return std::string();
        temps++;
        std::string name = "j2xo" + std::to_string(temps);
        pending.push_back(v.second + " " + name + "{};");
        parts.push_back(name + " = " + v.first);
        subst[n.get()] = E(name, v.second);
    }
    return join(parts, ", ");
}

std::string Method::conv(const std::string& code, const std::string& src, const std::string& dst) {
    if (src == dst || dst == "void" || dst.empty() || src.empty()) return code;
    if (src == "nullptr_t") return code;
    if (numeric_or_bool(dst) && numeric_or_bool(src)) {
        if ((src == "double" || src == "float") &&
            (dst == "int32_t" || dst == "int64_t" || dst == "int16_t" || dst == "int8_t" || dst == "char16_t")) {
            if (dst == "int64_t") return "jrt::d2l(" + code + ")";
            return dst == "int32_t" ? "jrt::d2i(" + code + ")" : "(" + dst + ")jrt::d2i(" + code + ")";
        }
        return !simple_code(code) ? "(" + dst + ")(" + code + ")" : "(" + dst + ")" + code;
    }
    if (numeric_or_bool(dst) && is_ref(src)) return "jrt::unbox<" + dst + ">(" + code + ")";
    if (is_ref(dst) && numeric_or_bool(src)) {
        std::string q = p.qname_of_ct(dst);
        const char* want = box(q);
        std::string c = code;
        if (want && src != want) c = "(" + std::string(want) + ")(" + c + ")";
        return "jrt::box(" + c + ")";
    }
    if (is_ref(dst) && is_ref(src)) {
        if (p.is_sub_ct(src, dst)) return code;
        if (src.find("Array<") != std::string::npos && dst.find("Array<") != std::string::npos) return code;
        return "jrt::cast<" + cls_of(dst) + ">(" + code + ")";
    }
    return code;
}

E Method::x(const NodeP& e) {
    std::string k = e->kind();
    if (k == "Paren") {
        E v = expr(node(e, "x"));
        return E("(" + v.first + ")", v.second);
    }
    if (k == "Lit") return x_lit(e);
    if (k == "Name") return x_name(e);
    if (k == "This") return x_this(e);
    if (k == "Select") return x_select(e);
    if (k == "TypeName") throw Decline("a type as a value");
    if (k == "Length") return E("jrt::nn(" + expr(node(e, "arr")).first + ")->length", "int32_t");
    if (k == "Index") {
        E a = expr(node(e, "arr"));
        E i = expr(node(e, "index"));
        auto mt = re::rx("^ref<(?:jrt::)?Array<(.*)>>$").match(a.second);
        if (!mt) throw Decline("index into " + a.second);
        return E("(*jrt::nn(" + a.first + "))[" + conv(i.first, i.second, "int32_t") + "]", mt.group(1));
    }
    if (k == "Unary") return x_unary(e);
    if (k == "Binary") return x_binary(e);
    if (k == "Concat") return x_concat(e);
    if (k == "Assign") return x_assign(e);
    if (k == "Cond") return x_cond(e);
    if (k == "Cast") return x_cast(e);
    if (k == "InstanceOf") return x_instanceof(e);
    if (k == "Call") return x_call(e);
    if (k == "New") return x_new(e);
    if (k == "NewArray") return x_newarray(e);
    if (k == "ClassLit") throw Decline("class literal");
    if (k == "SwitchExpr") return x_switchexpr(e);
    if (k == "Lambda") return x_lambda(e);
    return x_methodref(e);
}

E Method::x_lit(const NodeP& e) {
    const V& v = raw(e, "value");
    TypeP t = type(e);
    std::string k = t ? t->kind : std::string("null");
    if (v.is_none()) return E("nullptr", "nullptr_t");
    if (k == "bool") return E(py::truthy(v) ? "true" : "false", "bool");
    if (k == "i32") return E(v.as_int() != -2147483648LL ? std::to_string(v.as_int()) : "(-2147483647 - 1)", "int32_t");
    if (k == "i64") return E(v.as_int() != INT64_MIN ? std::to_string(v.as_int()) + "LL" : "(-9223372036854775807LL - 1)", "int64_t");
    if (k == "f32" || k == "f64") {
        double f = 0;
        if (!lit_float(e, f)) throw std::runtime_error("TypeError: float() argument must be a string or a number");
        if (k == "f32") return E(py::replace(float_text(f, "F"), "F", "f"), "float");
        return E(float_text(f, ""), "double");
    }
    if (k == "u16") return E(char_lit(v), "char16_t");
    if (v.is_str()) return E("jrt::lit(" + u16(v.str()) + ")", "ref<" + p.rt("String") + ">");
    throw Decline("literal " + py::repr(v));
}

E Method::x_name_of(const std::string& raw_name, const TypeP& t) {
    std::string n = ident(raw_name);
    auto it = vars.find(n);
    if (it != vars.end()) return E(n, it->second);
    auto c = captures.find(raw_name);
    if (c != captures.end()) return E(this_x() + "->" + c->second, p.ct(t));
    throw Decline("unknown local " + n);
}

E Method::x_name(const NodeP& e) { return x_name_of(sname(sym(e)), type(e)); }

std::string Method::this_x() {
    if (lambda_depth) {
        uses_self = true;
        return "self_";
    }
    return "this";
}

E Method::x_this(const NodeP& e) {
    TypeP q = type(e, "qualifier");
    if (!q) q = type(e);
    if (q && q->kind == "class" && tname(q) != ci->qname && ci->outer && !p.model->is_sub(ci->qname, tname(q)))
        return outer_of(tname(q));
    std::string code = this_x();
    if (code == "this") return E("jrt::self(this)", "ref<" + ci->cname + ">");
    return E(code, "ref<" + ci->cname + ">");
}

E Method::outer_of(const std::string& q) {
    std::string code = this_x();
    ClassInfo* c = ci;
    while (c && !(c->qname == q || p.model->is_sub(c->qname, q))) {
        if (!c->needs_outer) throw Decline("no enclosing instance of " + q);
        code += "->outer_";
        c = c->outer;
    }
    if (!c) throw Decline("no enclosing instance of " + q);
    return E(code, "ref<" + c->cname + ">");
}

E Method::receiver(const NodeP& obj, const SymP& s) {
    bool implicit = !obj;
    if (!implicit && (is(obj, "This") || is(obj, "Super")) && !type(obj, "qualifier")) {
        TypeP ot = type(obj);
        implicit = !ot || tname(ot) == ci->qname || p.model->is_sub(ci->qname, tname(ot)) || is(obj, "Super");
    }
    if (implicit) {
        if (s && !sowner(s).empty() && sowner(s) != ci->qname && ci->outer && !p.model->is_sub(ci->qname, sowner(s)) &&
            (!obj || is(obj, "This")))
            return outer_of(sowner(s));
        return E(this_x(), "ref<" + ci->cname + ">");
    }
    E v = expr(obj);
    TypeP ot = type(obj);
    if (ot && (ot->kind == "typevar" || ot->kind == "wildcard") && s && !sowner(s).empty()) {
        std::string want = "ref<" + p.cls_name(sowner(s)) + ">";
        if (!p.is_sub_ct(v.second, want)) return E("jrt::cast<" + cls_of(want) + ">(" + v.first + ")", want);
    }
    return v;
}

E Method::x_select(const NodeP& e) {
    SymP s = sym(e);
    NodeP obj = node(e, "obj");
    if (is(obj, "TypeName") || sstatic(s)) {
        std::string owner = sowner(s);
        ClassInfo* c = p.model->info(owner);
        if (!c) {
            if (auto sf = static_field(owner, sname(s))) return E(std::string("jrt::") + sf, p.ct(type(e)));
            const char* st = statics(owner);
            const std::string* lb = lib_of(owner);
            if (!st && !lb) throw Decline("library static " + owner + "." + sname(s));
            return E("jrt::" + (st ? std::string(st) : *lb) + "::" + sname(s), p.ct(type(e)));
        }
        std::string name = skind(s) == "enumconst" ? ident(sname(s)) : p.field_name(sname(s));
        std::string code = c->cname + "::" + name;
        if (p.needs_clinit(c) && c != ci) code = "(" + c->cname + "::clinit_(), " + code + ")";
        NodeP f;
        for (auto& x : c->fields())
            if (sym(x) && sname(sym(x)) == sname(s)) {
                f = x;
                break;
            }
        return E(code, f ? p.ct(type(f)) : p.ct(type(e)));
    }
    E r = receiver(obj, s);
    ClassInfo* c = p.model->info(sowner(s));
    if (!c) throw Decline("library field " + sowner(s) + "." + sname(s));
    NodeP f;
    for (auto& x : c->fields())
        if (sym(x) && sname(sym(x)) == sname(s)) {
            f = x;
            break;
        }
    std::string declared = f ? p.ct(jm::erase(type(f))) : p.ct(jm::erase(type(e)));
    std::string code = r.first + "->" + p.field_name(sname(s));
    std::string want = p.ct(type(e));
    if (declared != want && is_ref(want) && !p.is_sub_ct(declared, want))
        return E("jrt::cast<" + cls_of(want) + ">(" + code + ")", want);
    return E(code, declared);
}

E Method::unboxed(const std::string& code, const std::string& c, const TypeP& t) {
    if (is_ref(c) && t && t->kind == "class" && box(tname(t))) {
        std::string b = box(tname(t));
        return E("jrt::unbox<" + b + ">(" + code + ")", b);
    }
    return E(code, c);
}

E Method::x_unary(const NodeP& e) {
    E v = expr(node(e, "x"));
    std::string op = str(raw(e, "op"));
    if (op == "++" || op == "--") {
        if (is_ref(v.second)) throw Decline("++ on a box");
        return E(py::truthy(raw(e, "postfix")) ? v.first + op : op + v.first, v.second);
    }
    v = unboxed(v.first, v.second, type(node(e, "x")));
    std::string r = p.ct(type(e));
    if (op == "-" && (v.second == "int32_t" || v.second == "int64_t")) return E("-" + v.first, r);
    if (op == "!") return E("!" + v.first, "bool");
    return E(op + v.first, r);
}

E Method::x_binary(const NodeP& e) {
    NodeP le = node(e, "left"), re_ = node(e, "right");
    E l = expr(le);
    E r = expr(re_);
    std::string op = str(raw(e, "op"));
    if ((op == "==" || op == "!=") && is_ref(l.second) && is_ref(r.second)) return E(l.first + " " + op + " " + r.first, "bool");
    if ((op == "==" || op == "!=") && (l.second == "nullptr_t" || r.second == "nullptr_t"))
        return E(l.first + " " + op + " " + r.first, "bool");
    l = unboxed(l.first, l.second, type(le));
    r = unboxed(r.first, r.second, type(re_));
    if (op == "&&" || op == "||") return E(l.first + " " + op + " " + r.first, "bool");
    std::string res = p.ct(type(e));
    std::string lc = l.first, rc = r.first;
    if (op == "<<" || op == ">>" || op == "&" || op == "|" || op == "^") {
        if (is(le, "Binary") && str(raw(le, "op")) != op) lc = "(" + lc + ")";
        if (is(re_, "Binary") && str(raw(re_, "op")) != op) rc = "(" + rc + ")";
    }
    if (op == "<<" || op == ">>" || op == ">>>") {
        int width = l.second == "int64_t" ? 64 : 32;
        NodeP lit = strip_paren(re_);
        const V& lv = raw(lit, "value");
        if (!(is(lit, "Lit") && lv.is_int() && lv.as_int() >= 0 && lv.as_int() < width))
            rc = "(" + rc + " & " + std::to_string(width - 1) + ")";
        if (op == ">>>") {
            if (l.second != "int32_t" && l.second != "int64_t") lc = "(int32_t)" + lc;
            return E("jrt::ushr(" + lc + ", " + rc + ")", res);
        }
        return E(lc + " " + op + " " + rc, res);
    }
    if ((op == "/" || op == "%") && integral(res)) {
        NodeP lit = strip_paren(re_);
        const V& lv = raw(lit, "value");
        if (!(is(lit, "Lit") && lv.is_int() && lv.as_int() != 0 && lv.as_int() != -1))
            return E("jrt::" + std::string(op == "/" ? "div" : "rem") + "<" + res + ">(" + lc + ", " + rc + ")", res);
    }
    if (op == "%" && (res == "float" || res == "double")) return E("(" + res + ")std::fmod(" + lc + ", " + rc + ")", res);
    bool cmp = op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=";
    return E(lc + " " + op + " " + rc, cmp ? "bool" : res);
}

E Method::x_concat(const NodeP& e) {
    Strs parts;
    for (auto& xn : nodes(e, "parts")) {
        E v = expr(xn);
        std::string code = v.first;
        if (v.second == "nullptr_t") code = "ref<" + p.rt("Object") + ">()";
        else if (v.second == "int8_t" || v.second == "int16_t") code = "(int32_t)" + code;
        parts.push_back(code);
    }
    return E("jrt::concat(" + join(parts, ", ") + ")", "ref<" + p.rt("String") + ">");
}

E Method::x_assign(const NodeP& e) {
    NodeP tg = node(e, "target");
    E t = expr(tg);
    E v = expr(node(e, "value"));
    const std::string& tt = t.second;
    std::string op = str(raw(e, "op"));
    if (raw(e, "op").is_none()) return E(t.first + " = " + conv(v.first, v.second, tt), tt);
    if (tt == "ref<" + p.rt("String") + ">" && op == "+") return E(t.first + " = jrt::concat(" + t.first + ", " + v.first + ")", tt);
    NodeP st = strip_paren(tg);
    bool plain = is(st, "Name") || (is(st, "Select") && (is(node(st, "obj"), "This") || is(node(st, "obj"), "TypeName")));
    if (is_ref(tt)) throw Decline("compound assignment to a box");
    v = unboxed(v.first, v.second, type(node(e, "value")));
    const std::string& vt = v.second;
    if ((op == "/" || op == "%") && integral(tt) && integral(vt)) {
        if (!plain) throw Decline("integer /= on a complex target");
        std::string wide = (tt == "int64_t" || vt == "int64_t") ? "int64_t" : "int32_t";
        return E(t.first + " = (" + tt + ")jrt::" + (op == "/" ? "div" : "rem") + "<" + wide + ">(" + t.first + ", " + v.first + ")", tt);
    }
    if (op == "<<" || op == ">>" || op == ">>>") {
        int width = tt == "int64_t" ? 64 : 32;
        if (op == ">>>") {
            if (!plain) throw Decline(">>>= on a complex target");
            std::string base = (tt == "int32_t" || tt == "int64_t") ? t.first : "(int32_t)" + t.first;
            return E(t.first + " = (" + tt + ")jrt::ushr(" + base + ", (" + v.first + " & " + std::to_string(width - 1) + "))", tt);
        }
        return E(t.first + " " + op + "= (" + v.first + " & " + std::to_string(width - 1) + ")", tt);
    }
    if (integral(tt) && (vt == "float" || vt == "double")) {
        if (!plain) throw Decline("integer op= double on a complex target");
        std::string fn = tt == "int64_t" ? "jrt::d2l" : "jrt::d2i";
        return E(t.first + " = (" + tt + ")" + fn + "(" + t.first + " " + op + " " + v.first + ")", tt);
    }
    if (op == "%" && (tt == "float" || tt == "double")) {
        if (!plain) throw Decline("%= on a complex target");
        return E(t.first + " = (" + tt + ")std::fmod(" + t.first + ", " + v.first + ")", tt);
    }
    return E(t.first + " " + op + "= " + v.first, tt);
}

E Method::x_cond(const NodeP& e) {
    std::string c = cond(node(e, "cond"));
    E a = expr(node(e, "then"));
    E b = expr(node(e, "orelse"));
    TypeP et = type(e);
    std::string t = (et && et->kind != "null") ? p.ct(et) : (a.second != "nullptr_t" ? a.second : b.second);
    std::string ac = conv(a.first, a.second, t);
    return E("(" + c + " ? " + ac + " : " + conv(b.first, b.second, t) + ")", t);
}

E Method::x_cast(const NodeP& e) {
    E v = expr(node(e, "x"));
    std::string c = p.ct(type(e));
    if (c == v.second) return E(v.first, c);
    if (numeric_or_bool(c) && numeric_or_bool(v.second)) {
        if ((v.second == "double" || v.second == "float") && c != "double" && c != "float") return E(conv(v.first, v.second, c), c);
        return E("(" + c + ")" + (simple_code(v.first) ? v.first : "(" + v.first + ")"), c);
    }
    if (is_ref(c) && is_ref(v.second)) {
        if (p.is_sub_ct(v.second, c)) return E(v.first, v.second != "nullptr_t" ? v.second : c);
        return E("jrt::cast<" + cls_of(c) + ">(" + v.first + ")", c);
    }
    return E(conv(v.first, v.second, c), c);
}

E Method::x_instanceof(const NodeP& e) {
    if (node(e, "binding")) throw Decline("instanceof pattern not lowered");
    E v = expr(node(e, "x"));
    TypeP t = type(e, "test");
    if (t->kind == "array") return E("jrt::isa<" + cls_of(p.ct(t)) + ">(" + v.first + ")", "bool");
    return E("jrt::isa<" + p.cls_name(tname(t)) + ">(" + v.first + ")", "bool");
}

// The arguments of a call, each converted to its parameter's C++ type; a program's varargs packed into
// the array, a library's passed one by one (the runtime's are variadic templates).
std::string Method::args(const std::vector<NodeP>& as, const Strs& params, const SymP& s) {
    bool varargs = s && py::truthy(s->varargs) && !params.empty();
    bool lib = s && !p.model->method_decl(s).second;
    if (varargs) {
        TypeP lt = as.empty() ? nullptr : type(as.back());
        bool passes_array = as.size() == params.size() && lt && (lt->kind == "array" || lt->kind == "null");
        if (!passes_array) {
            size_t fixed = params.size() - 1;
            Strs head, rest;
            for (size_t i = 0; i < as.size() && i < fixed; i++) {
                E v = expr(as[i]);
                head.push_back(conv(v.first, v.second, params[i]));
            }
            TypeP last = s->params.back();
            TypeP elem_t = last && last->kind == "array" ? last->elem : nullptr;
            std::string ect = elem_t ? p.ct(jm::erase(elem_t)) : "ref<" + p.rt("Object") + ">";
            for (size_t i = fixed; i < as.size(); i++) {
                E v = expr(as[i]);
                rest.push_back(conv(v.first, v.second, ect));
            }
            if (lib) {
                head.insert(head.end(), rest.begin(), rest.end());
                return join(head, ", ");
            }
            head.push_back("jrt::array_of<" + ect + ">({" + join(rest, ", ") + "})");
            return join(head, ", ");
        }
    }
    Strs out;
    for (size_t i = 0; i < as.size(); i++) {
        std::string want = i < params.size() ? params[i] : std::string();
        NodeP a = strip_paren(as[i]);
        E v = (is(a, "Lambda") || is(a, "MethodRef")) ? functional(a) : expr(as[i]);
        out.push_back(conv(v.first, v.second, want));
    }
    return join(out, ", ");
}

E Method::x_call(const NodeP& e) {
    SymP s = sym(e);
    if (!s || skind(s) != "method") throw Decline("call " + (s ? skind(s) : std::string("unresolved")));
    auto cs = p.call_sig(s);
    std::string want = p.ct(type(e));
    NodeP t = node(e, "target");
    TypeP tt = t ? type(t) : nullptr;
    if (tt && tt->kind == "array" && sname(s) == "clone") {
        E a = expr(t);
        return E("jrt::nn(" + a.first + ")->clone()", a.second);
    }
    if (tt && tt->kind == "array" && sname(s) == "length") throw Decline("array method " + sname(s));
    if (sowner(s) == "java.lang.Comparable" && sname(s) == "compareTo" && t && !is(t, "This") && !is(t, "Super") &&
        !is(t, "TypeName")) {
        // Comparable is an interface the runtime's own classes do not derive from: through natural()
        E a = expr(t);
        E b = expr(nodes(e, "args")[0]);
        return E("jrt::natural(" + a.first + ", " + conv(b.first, b.second, "ref<" + p.rt("Object") + ">") + ")", "int32_t");
    }
    std::string a = args(nodes(e, "args"), cs.params, s);
    std::string name = ident(sname(s));
    std::string code;
    if (sstatic(s) || is(t, "TypeName")) {
        ClassInfo* c = p.model->info(sowner(s));
        if (c) {
            code = c->cname + "::" + name + "(" + a + ")";
        } else {
            const char* st = statics(sowner(s));
            const std::string* lb = lib_of(sowner(s));
            if (!st && !lb) throw Decline("library static " + sowner(s) + "." + sname(s));
            auto& ea = nodes(e, "args");
            if (sowner(s) == "java.lang.String" && sname(s) == "valueOf" && !ea.empty() && type(ea[0]) &&
                (type(ea[0])->kind == "i8" || type(ea[0])->kind == "i16"))
                a = "(int32_t)" + a;
            code = "jrt::" + (st ? std::string(st) : *lb) + "::" + name + "(" + a + ")";
        }
    } else if (is(t, "Super")) {
        std::string base = !(!sowner(s).empty() && p.model->is_interface(sowner(s))) ? ci->base : sowner(s);
        std::string prefix = this_x() == "this" ? std::string() : this_x() + "->";
        code = prefix + p.cls_name(base) + "::" + name + "(" + a + ")";
    } else {
        E r = receiver(t, s);
        if (sowner(s) == OBJECT && sname(s) == "getClass") throw Decline("getClass");
        code = ((r.first == "this" || r.first == "self_" || !t) ? r.first : "jrt::nn(" + r.first + ")") + "->" + name + "(" + a + ")";
        if (r.first == "this") code = "this->" + name + "(" + a + ")";
    }
    if (cs.ret != want && is_ref(want) && is_ref(cs.ret) && !p.is_sub_ct(cs.ret, want))
        return E("jrt::cast<" + cls_of(want) + ">(" + code + ")", want);
    if (cs.ret != want && numeric_or_bool(want) && is_ref(cs.ret)) return E("jrt::unbox<" + want + ">(" + code + ")", want);
    return E(code, cs.ret);
}

E Method::x_new(const NodeP& e) {
    TypeP t = type(e);
    if (node(e, "body")) {
        ClassInfo* c = p.model->by_decl(node(e, "body").get());
        if (!c) throw std::runtime_error("KeyError: an anonymous class not in the model");
        return new_local(c, e);
    }
    if (!t || t->kind != "class") throw Decline("new of " + type_repr(t));
    ClassInfo* c = p.model->info(tname(t));
    if (c && (c->local || c->anon)) return new_local(c, e);
    SymP s = sym(e);
    Strs params;
    Program::CallSig cs;
    if (s) {
        cs = p.call_sig(s);
        params = cs.params;
    }
    if (c && cs.m) {
        params.clear();
        for (auto& x : nodes(cs.m, "params")) params.push_back(p.ct(type(x)));
    }
    std::string a = args(nodes(e, "args"), params, s);
    std::string name = p.cls_name(tname(t));
    if (c && c->needs_outer) {
        std::string outer = node(e, "outer") ? expr(node(e, "outer")).first : outer_of(c->outer->qname).first;
        if (outer == "this") outer = "jrt::self(this)";
        return E("[&] { auto o_ = std::make_shared<" + name + ">(); o_->outer_ = " + outer + "; o_->ctor_(" + a + "); return o_; }()",
                 "ref<" + name + ">");
    }
    if (c && !p.two_phase(c)) return E(make_shared(name, a), "ref<" + name + ">");
    return E("jrt::make<" + name + ">(" + a + ")", "ref<" + name + ">");
}

E Method::new_local(ClassInfo* c, const NodeP& e) {
    Strs sets;
    if (c->needs_outer) sets.push_back("o_->outer_ = " + std::string(this_x() == "this" ? "jrt::self(this)" : this_x()) + ";");
    for (auto& cap : c->captures) {
        E v = x_name_of(cap.first, cap.second);
        sets.push_back("o_->cap_" + ident(cap.first) + " = " + v.first + ";");
    }
    Strs params;
    SymP s = sym(e);
    if (s) {
        auto cs = p.call_sig(s);
        if (cs.m) {
            for (auto& x : nodes(cs.m, "params")) params.push_back(p.ct(type(x)));
        } else {
            params = cs.params;
        }
    }
    std::string a = args(nodes(e, "args"), params, s);
    return E("[&] { auto o_ = std::make_shared<" + c->cname + ">(); " + join(sets, " ") + " o_->ctor_(" + a + "); return o_; }()",
             "ref<" + c->cname + ">");
}

E Method::x_newarray(const NodeP& e) {
    std::string c = p.ct(type(e));
    std::string elem = re::rx("^ref<(?:jrt::)?Array<(.*)>>$").match(c).group(1);
    auto& init = nodes(e, "init");
    if (item(e, "init") && (item(e, "init")->kind == l2::Item::Nodes || raw(e, "init").is_list())) {
        Strs items;
        for (auto& xn : init) {
            if (is(xn, "NewArray") && !type(xn)) set_type(xn, type(e)->elem);
            E v = expr(xn);
            items.push_back(conv(v.first, v.second, elem));
        }
        return E("jrt::array_of<" + elem + ">({" + join(items, ", ") + "})", c);
    }
    Strs dims;
    for (auto& d : nodes(e, "dims")) {
        E v = expr(d);
        dims.push_back(conv(v.first, v.second, "int32_t"));
    }
    if (dims.size() == 1) return E("jrt::new_array<" + elem + ">(" + dims[0] + ")", c);
    if (dims.size() == 2) {
        std::string inner = re::rx("^ref<(?:jrt::)?Array<(.*)>>$").match(elem).group(1);
        return E("jrt::new_array2<" + inner + ">(" + dims[0] + ", " + dims[1] + ")", c);
    }
    throw Decline("array of " + std::to_string(dims.size()) + " dimensions");
}

E Method::x_switchexpr(const NodeP& e) {
    std::string t = p.ct(type(e));
    NodeP se = node(e, "expr");
    TypeP st = type(se);
    if (st && st->kind == "class" && tname(st) == "java.lang.String") return string_switch_expr(e, t);
    auto h = switch_head(se);
    Strs lines{"[&]() -> " + t + " {", "\tswitch (" + h.first + ") {"};
    std::string saved_ret = ret;
    ret = t;
    struct R {
        Method& w;
        std::string s;
        ~R() { w.ret = s; }
    } restore{*this, saved_ret};
    for (auto& c : nodes(e, "cases")) {
        for (auto& lab : nodes(c, "labels")) lines.push_back("\tcase " + case_label(lab, h.second) + ":");
        if (py::truthy(raw(c, "default"))) lines.push_back("\tdefault:");
        lines.push_back("\t{");
        auto saved = vars;
        for (auto& b : nodes(c, "body")) stmt(b, lines, "\t\t");
        vars = saved;
        lines.push_back("\t}");
    }
    lines.push_back("\t}");
    lines.push_back("\tjrt::raise_new<jrt::IllegalStateException>(u\"no switch arm matched\");");
    lines.push_back("}()");
    return E(join(lines, "\n"), t);
}

E Method::string_switch_expr(const NodeP& e, const std::string& t) {
    E v = expr(strip_paren(node(e, "expr")));
    Strs lines{"[&]() -> " + t + " {", "\tauto s_ = jrt::nn(" + v.first + ");"};
    std::string saved_ret = ret;
    ret = t;
    struct R {
        Method& w;
        std::string s;
        ~R() { w.ret = s; }
    } restore{*this, saved_ret};
    NodeP dflt;
    for (auto& c : nodes(e, "cases")) {
        if (py::truthy(raw(c, "default"))) {
            dflt = c;
            if (nodes(c, "labels").empty()) continue;
        }
        Strs conds;
        for (auto& lab : nodes(c, "labels")) conds.push_back("s_->equals(" + expr(lab).first + ")");
        lines.push_back("\tif (" + join(conds, " || ") + ") {");
        auto saved = vars;
        for (auto& b : nodes(c, "body")) stmt(b, lines, "\t\t");
        vars = saved;
        lines.push_back("\t}");
    }
    if (dflt) {
        lines.push_back("\t{");
        for (auto& b : nodes(dflt, "body")) stmt(b, lines, "\t\t");
        lines.push_back("\t}");
    }
    lines.push_back("\tjrt::raise_new<jrt::IllegalStateException>(u\"no switch arm matched\");");
    lines.push_back("}()");
    return E(join(lines, "\n"), t);
}

// ---- lambdas and method references
void Method::sam_of(const TypeP& t, std::string& name, Strs& ps, std::string& r) {
    if (!t || t->kind != "class") throw Decline("lambda without a functional interface type");
    auto s = p.model->sam(tname(t));
    if (!s) throw Decline("not a functional interface: " + tname(t));
    name = s->name;
    ps.clear();
    for (auto& x : s->params) ps.push_back(p.ct(jm::erase(x)));
    r = p.ct(jm::erase(s->ret));
}

std::string Method::capture_list(bool inner_uses_self) {
    if (lambda_depth == 0 && inner_uses_self && !is_static) return "[=, self_ = jrt::self(this)]";
    return "[=]";
}

E Method::x_lambda(const NodeP& e) {
    std::string iface = p.cls_name(tname(type(e)));
    std::string nm, r;
    Strs ps;
    sam_of(type(e), nm, ps, r);
    auto s_vars = vars;
    std::string s_ret = ret;
    Strs s_pending = pending;
    bool s_uses = uses_self;
    auto s_subst = subst;
    lambda_depth++;
    uses_self = false;
    subst.clear();
    Strs params, entry;
    auto& eps = nodes(e, "params");
    for (size_t i = 0; i < eps.size(); i++) {
        std::string own = p.ct(type(eps[i]));
        std::string w = i < ps.size() ? ps[i] : own;
        std::string n = ident(sname(sym(eps[i])));
        if (own == w) {
            params.push_back(w + " " + n);
        } else {
            params.push_back(w + " " + n + "_in");
            entry.push_back(own + " " + n + " = " + conv(n + "_in", w, own) + ";");
        }
        vars[n] = own;
    }
    ret = r;
    Strs body;
    bool used = false;
    auto restore = [&]() {
        used = uses_self;
        lambda_depth--;
        vars = s_vars;
        ret = s_ret;
        pending = s_pending;
        uses_self = s_uses;
        subst = s_subst;
        uses_self = uses_self || (used && lambda_depth > 0);
    };
    try {
        NodeP b = node(e, "body");
        if (is(b, "Block")) {
            stmts(nodes(b, "stmts"), body, "\t");
        } else {
            NodeP st = mk(r != "void" ? "Return" : "ExprStmt");
            put(st, r != "void" ? "expr" : "expr", b);
            stmt(st, body, "\t");
        }
    } catch (...) {
        restore();
        throw;
    }
    restore();
    std::string cap = capture_list(used);
    Strs lines{"jrt::fn<" + iface + ">(" + cap + "(" + join(params, ", ") + ") -> " + r + " {"};
    for (auto& x : entry) lines.push_back("\t" + x);
    lines.insert(lines.end(), body.begin(), body.end());
    lines.push_back("})");
    if (lines.size() == 3 && entry.empty())   // a one-line body stays on one line
        return E("jrt::fn<" + iface + ">(" + cap + "(" + join(params, ", ") + ") -> " + r + " { " + py::strip(body[0]) + " })",
                 "ref<" + iface + ">");
    return E(join(lines, "\n"), "ref<" + iface + ">");
}

E Method::x_methodref(const NodeP& e) {
    SymP s = sym(e);
    std::string iface = p.cls_name(tname(type(e)));
    std::string nm, r;
    Strs ps;
    sam_of(type(e), nm, ps, r);
    Strs names, pd;
    for (size_t i = 0; i < ps.size(); i++) {
        names.push_back("a" + std::to_string(i));
        pd.push_back(ps[i] + " " + names.back());
    }
    std::string params = join(pd, ", ");
    std::string cap = "[=]";
    if (!s) throw Decline("unresolved method reference");
    auto argv_of = [&](size_t from, const Strs& cps) {
        Strs v;
        for (size_t i = from, j = 0; i < names.size() && j < cps.size(); i++, j++) v.push_back(conv(names[i], ps[i], cps[j]));
        return join(v, ", ");
    };
    std::string call, c;
    NodeP tg = node(e, "target");
    if (str(raw(e, "mode")) == "new") {
        TypeP t = tg ? type(tg) : nullptr;
        if (t && t->kind == "array") {
            call = "jrt::new_array<" + p.ct(t->elem) + ">(" + names[0] + ")";
            c = p.ct(t);
        } else {
            auto cs = p.call_sig(s);
            Strs cps = cs.params;
            if (cs.m) {
                cps.clear();
                for (auto& x : nodes(cs.m, "params")) cps.push_back(p.ct(type(x)));
            }
            std::string argv = argv_of(0, cps);
            ClassInfo* ci2 = t ? p.model->info(tname(t)) : nullptr;
            std::string name = p.cls_name(tname(t));
            call = (ci2 && !p.two_phase(ci2)) ? make_shared(name, argv) : "jrt::make<" + name + ">(" + argv + ")";
            c = "ref<" + name + ">";
        }
    } else {
        auto cs = p.call_sig(s);
        bool unbound = is(tg, "TypeName") && !sstatic(s);
        if (sstatic(s)) {
            ClassInfo* oc = p.model->info(sowner(s));
            std::string holder;
            if (oc) holder = oc->cname;
            else {
                const char* st = statics(sowner(s));
                const std::string* lb = lib_of(sowner(s));
                holder = "jrt::" + (st ? std::string(st) : lb ? *lb : std::string("?"));
            }
            call = holder + "::" + ident(sname(s)) + "(" + argv_of(0, cs.params) + ")";
        } else if (unbound) {
            std::string owner = "ref<" + p.cls_name(sowner(s)) + ">";
            std::string recv = conv(names[0], ps[0], owner);
            call = "jrt::nn(" + recv + ")->" + ident(sname(s)) + "(" + argv_of(1, cs.params) + ")";
        } else {
            std::string rc = (!is(tg, "This") && !is(tg, "Super")) ? expr(tg).first : this_x();
            if (rc == "this") rc = "jrt::self(this)";
            cap = "[=, r_ = " + rc + "]";
            call = "r_->" + ident(sname(s)) + "(" + argv_of(0, cs.params) + ")";
        }
        c = cs.ret;
    }
    if (r == "void") return E("jrt::fn<" + iface + ">(" + cap + "(" + params + ") -> void { " + call + "; })", "ref<" + iface + ">");
    return E("jrt::fn<" + iface + ">(" + cap + "(" + params + ") -> " + r + " { return " + conv(call, c, r) + "; })",
             "ref<" + iface + ">");
}

}  // namespace

std::string program(const V& unit_docs, const V& main) {
    std::vector<NodeP> units;
    for (auto& d : unit_docs.seq()) {
        NodeP u = l2::from_json(d);
        if (semantics(raw(u, "lang")) != Lang::Java) throw Decline("a whole program is Java's: a Python module is write_unit()");
        units.push_back(u);
    }
    Program prog(units, main.is_str() ? main.str() : std::string());
    return prog.write();
}

}  // namespace cpp17

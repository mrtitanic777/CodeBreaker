// translate/rules/java_csharp.cpp -- the rule set java of the C# back end (see the header).
//
// One Pass per call. It clones the units, then rewrites each class: its declarations (modifiers, names, the members Java's construction
// order moves), its statements and its expressions, each rewritten from the Java node with the Java facts still on it (the types are
// Java's until the last walk restates them as .NET's). A library call's C# template is parsed into L2 (TemplateReader) with the call's
// own target and arguments in its holes.
#include "translate/rules/java_csharp.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <map>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "core/text.hpp"
#include "translate/backends/jmodel.hpp"

namespace cb::translate::rules::java_csharp {

namespace jcs {

using ir::Node;
using ir::TypeP;
namespace text = cb::text;
namespace apimap = cb::java::apimap;
namespace jmodel = cb::translate::backends::jmodel;

struct Declined {
    std::string kind;
    std::string why;
};

bool has(const std::vector<std::string>& v, std::string_view x) { return std::find(v.begin(), v.end(), x) != v.end(); }

Node* strip(Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

const std::string* qual(const TypeP& t) {
    const auto* c = t ? std::get_if<ir::ClassType>(&t->body) : nullptr;
    return c != nullptr ? &c->name : nullptr;
}

bool library_name(std::string_view q) { return q.starts_with("java.") || q.starts_with("javax."); }
bool string_lit(Node* e) {
    const auto* l = ir::get<ir::Lit>(e);
    return l != nullptr && std::holds_alternative<std::string>(l->value);
}
bool null_lit(Node* e) {
    const auto* l = ir::get<ir::Lit>(e);
    return l != nullptr && std::holds_alternative<std::monostate>(l->value);
}
bool is_java_string(const TypeP& t) {
    const std::string* q = qual(t);
    return q != nullptr && *q == "java.lang.String";
}
bool integral(const TypeP& t) {
    if (!t) return false;
    const std::string_view k = t->kind();
    return k == "i8" || k == "i16" || k == "i32" || k == "i64" || k == "u16";
}

std::vector<Node*> body_statements(const ir::Suite& body) {
    if (const auto* one = std::get_if<Node*>(&body)) {
        if (const ir::Block* b = ir::get<ir::Block>(*one)) return b->stmts;
    }
    return ir::statements(body);
}

// The nodes of a member, leaving out the bodies of classes declared inside it.
std::vector<Node*> own_nodes(Node* root) {
    std::vector<Node*> out;
    std::vector<Node*> todo{root};
    while (!todo.empty()) {
        Node* n = todo.back();
        todo.pop_back();
        if (n == nullptr || ir::is<ir::ClassDecl>(n) || ir::is<ir::LocalClass>(n)) continue;
        out.push_back(n);
        if (const auto* made = ir::get<ir::New>(n); made != nullptr && made->body != nullptr) {
            todo.insert(todo.end(), made->args.begin(), made->args.end());
            if (made->outer != nullptr) todo.push_back(made->outer);
            continue;
        }
        for (Node* child : ir::children(*n)) todo.push_back(child);
    }
    return out;
}

// ---- the tables -----------------------------------------------------------------------------------------------------------------

const TypeP& char_type() {
    static const TypeP t = ir::prim_type(ir::Prim::U16, std::nullopt, ir::CharEncoding::Utf16);
    return t;
}

// a box -> the number it holds
std::optional<ir::Prim> box_prim(std::string_view q) {
    static const std::unordered_map<std::string_view, ir::Prim> m = {
        {"java.lang.Integer", ir::Prim::I32}, {"java.lang.Long", ir::Prim::I64}, {"java.lang.Short", ir::Prim::I16}, {"java.lang.Byte", ir::Prim::I8},
        {"java.lang.Character", ir::Prim::U16}, {"java.lang.Boolean", ir::Prim::Bool}, {"java.lang.Double", ir::Prim::F64}, {"java.lang.Float", ir::Prim::F32}};
    const auto it = m.find(q);
    if (it == m.end()) return std::nullopt;
    return it->second;
}
bool is_box(const TypeP& t) {
    const std::string* q = qual(t);
    return q != nullptr && box_prim(*q).has_value();
}

// Java class -> the .NET class that stands for it (a name with no namespace is the runtime's)
const std::unordered_map<std::string_view, std::string_view>& class_table() {
    static const std::unordered_map<std::string_view, std::string_view> m = {
        {"java.lang.String", "System.String"}, {"java.lang.Object", "System.Object"}, {"java.lang.Number", "System.Object"},
        {"java.lang.CharSequence", "System.String"}, {"java.lang.StringBuilder", "System.Text.StringBuilder"},
        {"java.lang.StringBuffer", "System.Text.StringBuilder"}, {"java.lang.Math", "System.Math"}, {"java.lang.StrictMath", "System.Math"},
        {"java.lang.System", "System.Console"}, {"java.lang.Throwable", "System.Exception"}, {"java.lang.Exception", "System.Exception"},
        {"java.lang.RuntimeException", "System.Exception"}, {"java.lang.Error", "System.Exception"}, {"java.lang.OutOfMemoryError", "System.OutOfMemoryException"},
        {"java.lang.ArithmeticException", "System.ArithmeticException"}, {"java.lang.NullPointerException", "System.NullReferenceException"},
        {"java.lang.ArrayIndexOutOfBoundsException", "System.IndexOutOfRangeException"},
        {"java.lang.IndexOutOfBoundsException", "System.ArgumentOutOfRangeException"},
        {"java.lang.StringIndexOutOfBoundsException", "System.ArgumentOutOfRangeException"},
        {"java.lang.IllegalArgumentException", "System.ArgumentException"}, {"java.lang.IllegalStateException", "System.InvalidOperationException"},
        {"java.lang.UnsupportedOperationException", "System.NotSupportedException"}, {"java.lang.NumberFormatException", "System.FormatException"},
        {"java.lang.ClassCastException", "System.InvalidCastException"}, {"java.lang.NegativeArraySizeException", "System.OverflowException"},
        {"java.util.NoSuchElementException", "System.InvalidOperationException"}, {"java.lang.Comparable", "System.IComparable"},
        {"java.lang.Iterable", "System.Collections.Generic.IEnumerable"}, {"java.lang.AutoCloseable", "System.IDisposable"},
        {"java.io.Closeable", "System.IDisposable"}, {"java.io.IOException", "System.IO.IOException"}, {"java.lang.Runnable", "System.Action"},
        {"java.util.Random", "JavaRandom"}};
    return m;
}

// Java generic collection -> the .NET generic, its arity
const std::unordered_map<std::string_view, std::pair<std::string_view, std::size_t>>& generic_table() {
    static const std::unordered_map<std::string_view, std::pair<std::string_view, std::size_t>> m = {
        {"java.util.List", {"System.Collections.Generic.List", 1}}, {"java.util.ArrayList", {"System.Collections.Generic.List", 1}},
        {"java.util.LinkedList", {"System.Collections.Generic.List", 1}}, {"java.util.Vector", {"System.Collections.Generic.List", 1}},
        {"java.util.Collection", {"System.Collections.Generic.ICollection", 1}}, {"java.util.AbstractList", {"System.Collections.Generic.List", 1}},
        {"java.util.Map", {"System.Collections.Generic.IDictionary", 2}}, {"java.util.HashMap", {"JHashMap", 2}},
        {"java.util.LinkedHashMap", {"JLinkedHashMap", 2}}, {"java.util.Hashtable", {"System.Collections.Generic.Dictionary", 2}},
        {"java.util.TreeMap", {"System.Collections.Generic.SortedDictionary", 2}}, {"java.util.SortedMap", {"System.Collections.Generic.IDictionary", 2}},
        {"java.util.Set", {"System.Collections.Generic.ISet", 1}}, {"java.util.HashSet", {"JHashSet", 1}}, {"java.util.LinkedHashSet", {"JLinkedHashSet", 1}},
        {"java.util.TreeSet", {"System.Collections.Generic.SortedSet", 1}}, {"java.util.SortedSet", {"System.Collections.Generic.ISet", 1}},
        {"java.util.Map.Entry", {"System.Collections.Generic.KeyValuePair", 2}}, {"java.util.Iterator", {"JIterator", 1}}};
    return m;
}

bool ordered(std::string_view qname) { return qname != "java.util.Hashtable"; }

// A functional interface as a .NET delegate: the delegate, and how its type arguments come from the interface's.
using TArgs = std::vector<TypeP>;
struct Delegate {
    std::string_view generic;
    TArgs (*arguments)(TArgs);
};
const TypeP& obj_type() {
    static const TypeP t = ir::class_type("java.lang.Object");
    return t;
}
const std::unordered_map<std::string_view, Delegate>& function_table() {
    static const auto same = [](TArgs a) { return a; };
    static const auto with_bool = [](TArgs a) { a.push_back(ir::bool_type()); return a; };
    static const std::unordered_map<std::string_view, Delegate> m = {
        {"java.util.function.Function", {"System.Func", same}}, {"java.util.function.BiFunction", {"System.Func", same}},
        {"java.util.function.Supplier", {"System.Func", same}}, {"java.util.function.Consumer", {"System.Action", same}},
        {"java.util.function.BiConsumer", {"System.Action", same}}, {"java.util.function.Predicate", {"System.Func", with_bool}},
        {"java.util.function.BiPredicate", {"System.Func", with_bool}},
        {"java.util.function.UnaryOperator", {"System.Func", [](TArgs a) { const TypeP t = a.empty() ? obj_type() : a[0]; return TArgs{t, t}; }}},
        {"java.util.function.BinaryOperator", {"System.Func", [](TArgs a) { const TypeP t = a.empty() ? obj_type() : a[0]; return TArgs{t, t, t}; }}},
        {"java.util.function.IntBinaryOperator", {"System.Func", [](TArgs) { return TArgs{ir::i32_type(), ir::i32_type(), ir::i32_type()}; }}},
        {"java.util.function.IntUnaryOperator", {"System.Func", [](TArgs) { return TArgs{ir::i32_type(), ir::i32_type()}; }}},
        {"java.util.function.IntPredicate", {"System.Func", [](TArgs) { return TArgs{ir::i32_type(), ir::bool_type()}; }}},
        {"java.util.function.IntFunction", {"System.Func", [](TArgs a) { a.insert(a.begin(), ir::i32_type()); return a; }}},
        {"java.util.function.IntConsumer", {"System.Action", [](TArgs) { return TArgs{ir::i32_type()}; }}},
        {"java.util.function.ToIntFunction", {"System.Func", [](TArgs a) { a.push_back(ir::i32_type()); return a; }}},
        {"java.util.function.IntSupplier", {"System.Func", [](TArgs) { return TArgs{ir::i32_type()}; }}},
        {"java.util.function.DoubleUnaryOperator", {"System.Func", [](TArgs) { const TypeP d = ir::prim_type(ir::Prim::F64); return TArgs{d, d}; }}},
        {"java.util.Comparator", {"System.Comparison", same}}};
    return m;
}

// The functional interfaces' one method: its name and arity.
std::optional<std::pair<std::string_view, std::size_t>> functional_method(std::string_view q) {
    static const std::unordered_map<std::string_view, std::pair<std::string_view, std::size_t>> m = {
        {"java.util.function.Function", {"apply", 1}}, {"java.util.function.BiFunction", {"apply", 2}}, {"java.util.function.Supplier", {"get", 0}},
        {"java.util.function.Consumer", {"accept", 1}}, {"java.util.function.BiConsumer", {"accept", 2}}, {"java.util.function.Predicate", {"test", 1}},
        {"java.util.function.BiPredicate", {"test", 2}}, {"java.util.function.UnaryOperator", {"apply", 1}}, {"java.util.function.BinaryOperator", {"apply", 2}},
        {"java.util.function.IntBinaryOperator", {"applyAsInt", 2}}, {"java.util.function.IntUnaryOperator", {"applyAsInt", 1}},
        {"java.util.function.IntPredicate", {"test", 1}}, {"java.util.function.IntFunction", {"apply", 1}}, {"java.util.function.IntConsumer", {"accept", 1}},
        {"java.util.function.ToIntFunction", {"applyAsInt", 1}}, {"java.util.function.IntSupplier", {"getAsInt", 0}},
        {"java.util.function.DoubleUnaryOperator", {"applyAsDouble", 1}}, {"java.util.Comparator", {"compare", 2}}, {"java.lang.Runnable", {"run", 0}}};
    const auto it = m.find(q);
    if (it == m.end()) return std::nullopt;
    return it->second;
}

// A library class's family: the classes that answer a call the same way (the registry's keys use the family).
const std::unordered_map<std::string, std::string>& family_table() {
    static const std::unordered_map<std::string, std::string> m = [] {
        std::unordered_map<std::string, std::string> t = {
            {"java.lang.String", "String"}, {"java.lang.StringBuilder", "StringBuilder"}, {"java.lang.StringBuffer", "StringBuilder"},
            {"java.lang.AbstractStringBuilder", "StringBuilder"}, {"java.lang.Object", "Object"}, {"java.lang.Math", "Math"},
            {"java.lang.StrictMath", "Math"}, {"java.lang.Integer", "Integer"}, {"java.lang.Long", "Long"}, {"java.lang.Double", "Double"},
            {"java.lang.Float", "Float"}, {"java.lang.Boolean", "Boolean"}, {"java.lang.Character", "Character"}, {"java.lang.Byte", "Byte"},
            {"java.lang.Short", "Short"}, {"java.lang.System", "System"}, {"java.lang.Thread", "Thread"}, {"java.io.PrintStream", "PrintStream"},
            {"java.util.Arrays", "Arrays"}, {"java.util.Collections", "Collections"}, {"java.util.Random", "Random"},
            {"java.lang.Throwable", "Throwable"}, {"java.lang.Exception", "Throwable"}, {"java.lang.RuntimeException", "Throwable"},
            {"java.util.Map.Entry", "Entry"}, {"java.lang.Number", "Integer"}, {"java.lang.CharSequence", "String"}, {"java.lang.Enum", "Enum"}};
        for (const char* n : {"List", "ArrayList", "LinkedList", "Vector", "Collection", "AbstractList", "AbstractCollection", "Iterable", "AbstractSequentialList"})
            t[std::string("java.util.") + n] = "List";
        t["java.lang.Iterable"] = "List";
        for (const char* n : {"Map", "HashMap", "LinkedHashMap", "TreeMap", "AbstractMap", "SortedMap", "Hashtable", "NavigableMap"}) t[std::string("java.util.") + n] = "Map";
        for (const char* n : {"Set", "HashSet", "LinkedHashSet", "TreeSet", "AbstractSet", "SortedSet", "NavigableSet"}) t[std::string("java.util.") + n] = "Set";
        for (const auto& [q, d] : function_table()) t[std::string(q)] = std::string(q.substr(q.rfind('.') + 1));
        t["java.lang.Runnable"] = "Runnable";
        return t;
    }();
    return m;
}

std::optional<std::string_view> object_method(std::string_view name, std::size_t arity) {
    if (name == "toString" && arity == 0) return "ToString";
    if (name == "equals" && arity == 1) return "Equals";
    if (name == "hashCode" && arity == 0) return "GetHashCode";
    return std::nullopt;
}

std::string param_key(const TypeP& p) {
    const std::string_view kind = p->kind();
    if (kind == "typevar") return "Object";
    static const std::unordered_map<std::string_view, std::string_view> by_kind = {
        {"i32", "int"}, {"i64", "long"}, {"i16", "int"}, {"i8", "int"}, {"u16", "int"}, {"f32", "float"}, {"f64", "double"}, {"bool", "boolean"}};
    if (const auto it = by_kind.find(kind); it != by_kind.end()) return std::string(it->second);
    if (kind == "class") return p->simple();
    if (const auto* a = std::get_if<ir::ArrayType>(&p->body); a != nullptr && a->elem && a->elem->kind() == "u16") return "char[]";
    return "Object";
}

// a .NET short name a template names -> its namespace-qualified name (the runtime's stay as they are)
std::string dotnet_class(std::string_view name) {
    static const std::unordered_set<std::string_view> system = {"Math", "Console", "Convert", "Array", "Environment", "StringComparison", "BitConverter",
                                                                 "Enum", "Func", "Action", "Comparison", "Exception"};
    if (system.contains(name)) return "System." + std::string(name);
    if (name == "StringBuilder") return "System.Text.StringBuilder";
    if (name == "Comparer" || name == "List" || name == "Dictionary") return "System.Collections.Generic." + std::string(name);
    return std::string(name);
}

// ---- making nodes -------------------------------------------------------------------------------------------------------------

class Make {
public:
    explicit Make(Arena& arena) : arena_(arena) {}
    Arena& arena() { return arena_; }

    Node* type_name(const TypeP& t) { return ir::make(arena_, ir::TypeName{}, t); }
    Node* type_name(const std::string& cls) { return type_name(ir::class_type(cls)); }
    ir::SymP method(const std::string& name, std::optional<std::string> owner = std::nullopt, bool is_static = false) {
        return ir::make_sym(ir::SymKind::Method, name, std::move(owner), is_static);
    }
    Node* call(Node* target, const std::string& name, std::vector<Node*> args, const TypeP& type, bool is_static = false) {
        return ir::make(arena_, ir::Call{.target = target, .sym = method(name, std::nullopt, is_static), .args = std::move(args)}, type);
    }
    Node* static_call(const std::string& cls, const std::string& name, std::vector<Node*> args, const TypeP& type) {
        auto sym = method(name, cls, true);
        return ir::make(arena_, ir::Call{.target = type_name(cls), .sym = sym, .args = std::move(args)}, type);
    }
    Node* field(Node* obj, const std::string& name, const TypeP& type, bool is_static = false) {
        return ir::make(arena_, ir::Select{.obj = obj, .sym = ir::make_sym(ir::SymKind::Field, name, std::nullopt, is_static, type)}, type);
    }
    Node* cast(const TypeP& to, Node* x) { return ir::make(arena_, ir::Cast{.x = x}, to); }
    Node* text(const std::string& s) { return ir::make(arena_, ir::Lit{.value = s}, ir::class_type("System.String")); }
    Node* number(std::int64_t v) { return ir::make(arena_, ir::Lit{.value = v}, ir::i32_type()); }
    Node* null() { return ir::make(arena_, ir::Lit{.value = std::monostate{}}, ir::null_type()); }
    Node* boolean(bool v) { return ir::make(arena_, ir::Lit{.value = v}, ir::bool_type()); }
    Node* name(const ir::SymP& s, const TypeP& t) { return ir::make(arena_, ir::Name{.sym = s}, t); }
    Node* expr_stmt(Node* e) { return ir::make(arena_, ir::ExprStmt{.expr = e}); }
    Node* block(std::vector<Node*> stmts) { return ir::make(arena_, ir::Block{.stmts = std::move(stmts)}); }
    Node* assign(Node* target, Node* value) { return ir::make(arena_, ir::Assign{.target = target, .value = value}, target->type); }
    Node* unsupported(const std::string& kind, const std::string& why) {
        return ir::make(arena_, ir::Unsupported{.kind = kind, .raw = why});
    }

private:
    Arena& arena_;
};

// ---- the templates: C# expression text with holes, read into L2 -----------------------------------------------------------------

// The holes of one template: `{t}`, `{out}`, `{*}`, `{N}`, `{Np}`, `{sN}`; each key gives the node (or nodes, for `{*}`) it stands for.
using HoleFn = std::function<std::vector<Node*>(const std::string& key)>;

class TemplateReader {
public:
    TemplateReader(Make& make, std::string_view text, HoleFn hole, TypeP result) : mk_(make), s_(text), hole_(std::move(hole)), result_(std::move(result)) {}

    // The whole template as one expression (its type the call's); a template that is not one is a Declined.
    Node* read() {
        Node* e = assignment();
        skip();
        if (at_ < s_.size()) fail();
        if (!e->type || std::holds_alternative<ir::AnyType>(e->type->body)) e->type = result_;
        return e;
    }

private:
    [[noreturn]] void fail() const { throw Declined{"library", "a C# template the rule set does not read: " + std::string(s_)}; }
    void skip() {
        while (at_ < s_.size() && s_[at_] == ' ') ++at_;
    }
    bool eat(std::string_view tok) {
        skip();
        if (s_.substr(at_).starts_with(tok)) {
            at_ += tok.size();
            return true;
        }
        return false;
    }
    bool peek(std::string_view tok) {
        skip();
        return s_.substr(at_).starts_with(tok);
    }
    std::string ident() {
        skip();
        const std::size_t start = at_;
        while (at_ < s_.size() && (std::isalnum(static_cast<unsigned char>(s_[at_])) != 0 || s_[at_] == '_')) ++at_;
        if (start == at_) fail();
        return std::string(s_.substr(start, at_ - start));
    }
    static std::optional<TypeP> keyword_type(std::string_view w) {
        if (w == "int") return ir::i32_type();
        if (w == "uint") return ir::prim_type(ir::Prim::U32);
        if (w == "long") return ir::prim_type(ir::Prim::I64);
        if (w == "double") return ir::prim_type(ir::Prim::F64);
        if (w == "float") return ir::prim_type(ir::Prim::F32);
        if (w == "char") return char_type();
        if (w == "bool") return ir::bool_type();
        if (w == "string") return ir::class_type("System.String");
        if (w == "object") return ir::class_type("System.Object");
        return std::nullopt;
    }

    Node* assignment() {
        Node* left = binary(0);
        if (peek("==")) return left;
        if (eat("=")) {
            Node* right = assignment();
            return mk_.assign(left, right);
        }
        return left;
    }
    Node* binary(int level) {
        static const std::vector<std::vector<std::string_view>> levels = {{"||"}, {"&&"}, {"==", "!="}, {"+", "-"}, {"*", "/", "%"}};
        if (level >= static_cast<int>(levels.size())) return unary();
        Node* left = binary(level + 1);
        for (;;) {
            bool matched = false;
            for (std::string_view op : levels[static_cast<std::size_t>(level)]) {
                if (peek(op) && !(op == "=" )) {
                    eat(op);
                    Node* right = binary(level + 1);
                    const TypeP type = level <= 2 ? ir::bool_type() : left->type;
                    left = ir::make(mk_.arena(), ir::Binary{.op = std::string(op), .left = left, .right = right}, type);
                    matched = true;
                    break;
                }
            }
            if (!matched) return left;
        }
    }
    Node* unary() {
        if (eat("!")) return ir::make(mk_.arena(), ir::Unary{.op = "!", .x = unary()}, ir::bool_type());
        if (peek("(")) {   // a cast `(T)x`, or a parenthesised expression
            const std::size_t save = at_;
            eat("(");
            skip();
            const std::size_t start = at_;
            while (at_ < s_.size() && (std::isalnum(static_cast<unsigned char>(s_[at_])) != 0 || s_[at_] == '.' || s_[at_] == '_')) ++at_;
            const std::string word(s_.substr(start, at_ - start));
            if (!word.empty() && eat(")")) {
                skip();
                const bool operand_follows = at_ < s_.size() && (s_[at_] == '{' || s_[at_] == '(' || std::isalnum(static_cast<unsigned char>(s_[at_])) != 0);
                if (operand_follows) {
                    const std::optional<TypeP> kw = keyword_type(word);
                    return mk_.cast(kw ? *kw : ir::class_type(dotnet_class(word)), unary());
                }
            }
            at_ = save;
        }
        return postfix(primary());
    }
    std::vector<Node*> arguments() {
        std::vector<Node*> out;
        if (eat(")")) return out;
        for (;;) {
            skip();
            if (s_.substr(at_).starts_with("{*}")) {
                at_ += 3;
                for (Node* n : hole_("*")) out.push_back(n);
            } else {
                out.push_back(assignment());
            }
            if (eat(")")) return out;
            if (!eat(",")) fail();
        }
    }
    Node* postfix(Node* e) {
        for (;;) {
            if (eat(".")) {
                const std::string name = ident();
                if (eat("(")) e = mk_.call(e, name, arguments(), TypeP{});
                else e = mk_.field(e, name, TypeP{});
            } else if (eat("[")) {
                Node* index = assignment();
                if (!eat("]")) fail();
                e = ir::make(mk_.arena(), ir::Index{.arr = e, .index = index}, TypeP{});
            } else if (eat("(")) {   // a delegate called
                auto* call = ir::get<ir::Call>(e = ir::make(mk_.arena(), ir::Call{.target = e, .sym = mk_.method("Invoke"), .args = arguments()}, TypeP{}));
                call->sym->flags.push_back("delegate");
            } else {
                return e;
            }
        }
    }
    Node* primary() {
        skip();
        if (at_ >= s_.size()) fail();
        const char c = s_[at_];
        if (c == '{') {
            const std::size_t close = s_.find('}', at_);
            if (close == std::string_view::npos) fail();
            const std::string key(s_.substr(at_ + 1, close - at_ - 1));
            at_ = close + 1;
            std::vector<Node*> got = hole_(key);
            if (got.size() != 1) fail();
            return got[0];
        }
        if (c == '"') {
            const std::size_t close = s_.find('"', at_ + 1);
            if (close == std::string_view::npos) fail();
            Node* lit = mk_.text(std::string(s_.substr(at_ + 1, close - at_ - 1)));
            at_ = close + 1;
            return lit;
        }
        if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
            const std::size_t start = at_;
            while (at_ < s_.size() && std::isdigit(static_cast<unsigned char>(s_[at_])) != 0) ++at_;
            return mk_.number(std::stoll(std::string(s_.substr(start, at_ - start))));
        }
        if (eat("(")) {
            Node* inner = assignment();
            if (!eat(")")) fail();
            return inner;
        }
        const std::string word = ident();
        if (word == "new") {
            const std::string type_word = ident();
            const std::optional<TypeP> kw = keyword_type(type_word);
            const TypeP made = kw ? *kw : ir::class_type(dotnet_class(type_word));
            if (!eat("(")) fail();
            return ir::make(mk_.arena(), ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, type_word), .args = arguments()}, made);
        }
        // a type, then its members: `Math.Abs(...)`, `System.Threading.Thread.Sleep(...)`, `char.IsDigit(...)`, `StringComparison.Ordinal`
        std::vector<std::string> chain{word};
        for (;;) {
            const std::size_t save = at_;
            if (!eat(".")) break;
            chain.push_back(ident());
            if (peek("(")) break;
            (void)save;
        }
        if (chain.size() < 2) fail();
        const std::string member = chain.back();
        chain.pop_back();
        const std::string cls = text::join(chain, ".");
        const std::optional<TypeP> kw = chain.size() == 1 ? keyword_type(cls) : std::nullopt;
        const TypeP owner = kw ? *kw : ir::class_type(chain.size() == 1 ? dotnet_class(cls) : cls);
        Node* type = mk_.type_name(owner);
        if (eat("(")) {
            auto sym = mk_.method(member, qual(owner) != nullptr ? std::optional<std::string>(*qual(owner)) : std::nullopt, true);
            return ir::make(mk_.arena(), ir::Call{.target = type, .sym = sym, .args = arguments()}, TypeP{});
        }
        return ir::make(mk_.arena(), ir::Select{.obj = type, .sym = ir::make_sym(ir::SymKind::Field, member, std::nullopt, true)}, TypeP{});
    }

    Make& mk_;
    std::string_view s_;
    HoleFn hole_;
    TypeP result_;
    std::size_t at_ = 0;
};

// ---- the pass ---------------------------------------------------------------------------------------------------------------------

class Pass {
public:
    Pass(Arena& arena, const apimap::Registry& registry) : mk_(arena), arena_(arena), registry_(registry) {}

    Result run(std::span<Node* const> units);

private:
    // ---- the program
    void index(Node* class_node);
    ir::ClassDecl* class_named(const std::string& q) const {
        const auto it = classes_.find(q);
        return it == classes_.end() ? nullptr : it->second;
    }
    ir::ClassDecl* base_of(const ir::ClassDecl& c) const {
        const std::string* q = qual(c.extends);
        return q != nullptr ? class_named(*q) : nullptr;
    }
    bool overrides_base(const ir::ClassDecl& c, const ir::MethodDecl& m) const;
    // A method's Java name (rewrite_class gives it its C# one: what overrides what is read by the Java names).
    std::string jname(const ir::MethodDecl& m) const {
        const auto it = java_names_.find(&m);
        return it != java_names_.end() ? it->second : m.name.value_or("");
    }
    bool closeable(const ir::ClassDecl& c) const;
    void prepare(std::vector<Node*>& classes);
    std::optional<std::string> refused_class(ir::ClassDecl& c);

    // ---- classes in methods, anonymous classes, inner classes
    struct Hoisted {
        ir::ClassDecl* decl = nullptr;
        std::vector<std::pair<std::string, TypeP>> captures;   // the locals it reads from the methods around it, by name
        std::vector<ir::SymP> fields;                          // their fields, cap_NAME
        std::string delegate_method;                           // an anonymous class of a library functional interface: its one method
        TypeP delegate_type;
    };
    void prepare_local_classes(std::span<Node* const> units, std::vector<Node*>& classes);
    void find_local_captures();
    void add_capture_fields();
    void prepare_inner_classes(const std::vector<Node*>& classes);
    bool is_a(const ir::ClassDecl* c, const std::string& q) const;
    TypeP own_type(const std::string& qname) const;
    bool reaches_outer(const ir::ClassDecl& c) const;
    const Hoisted* hoisted(const ir::ClassDecl& c) const;
    bool has_captures(const ir::ClassDecl& c) const;
    bool needs_outer(const ir::ClassDecl& c) const { return c.qname && inner_.contains(*c.qname); }
    Node* outer_path(const std::string& qname);
    Node* this_node(Node* e);
    Node* new_inner(Node* e, const ir::ClassDecl& c);
    bool base_construction_is_closed(const ir::ClassDecl* base) const;

    // ---- enums of objects: a class of JEnum, each constant a static field made in order
    const std::vector<std::string>* object_enum(const std::string& q) const {
        const auto it = object_enums_.find(q);
        return it == object_enums_.end() ? nullptr : &it->second;
    }
    void enum_members(ir::ClassDecl& c);
    Node* enum_call(Node* e);
    Node* enum_scrutinee(Node* scrutinee, std::vector<Node*>& cases);

    // ---- lambdas of the program's interfaces
    const ir::MethodDecl* single_abstract(const ir::ClassDecl& start) const;
    void prepare_lambda_classes();
    Node* as_interface(Node* fn, const TypeP& face);

    // ---- declarations
    void rewrite_class(ir::ClassDecl& c);
    std::vector<std::string> class_mods(const ir::ClassDecl& c) const;
    std::vector<std::string> access(const std::vector<std::string>& mods, const ir::ClassDecl& c) const;
    bool is_const(const ir::FieldDecl& f) const;
    std::vector<std::string> field_mods(const ir::FieldDecl& f, const ir::ClassDecl& c) const;
    std::vector<std::string> method_mods(const ir::MethodDecl& m, const ir::ClassDecl& c) const;
    std::vector<std::string> method_mods_plain(const ir::MethodDecl& m, const ir::ClassDecl& c) const;
    std::string method_name(const ir::MethodDecl& m, const ir::ClassDecl& c) const;
    std::optional<std::string> member_name(const ir::Sym& s) const;   // the C# name, where it is not the Java one
    std::vector<Node*> arrange_members(ir::ClassDecl& c);

    // ---- statements
    void suite(ir::Suite& s);
    Node* stmt_node(Node* s);          // one statement in place of one
    void stmts(std::vector<Node*>& list);
    std::vector<Node*> stmt(Node* s);  // the statements one becomes
    void loop_with_labels(Node* loop, const std::string& label, bool continues);
    std::vector<Node*> try_stmt(Node* s);
    std::vector<Node*> using_stmt(std::span<Node* const> resources, Node* body);
    bool jumps_out(Node* n, int loops, std::set<std::string> labels, int switches = 0) const;

    // ---- expressions
    Node* expr(Node* e);
    Node* call(Node* e, bool statement);
    Node* library_call(Node* e);
    Node* select(Node* e);
    Node* new_object(Node* e);
    Node* binary(Node* e);
    Node* concat(Node* e);
    Node* string_of(Node* a);
    Node* method_ref(Node* e);
    void check_lambda(Node* e);
    void raw_lambda_params(ir::Lambda& l, const TypeP& face);
    Node* fill(const std::string& tmpl, Node* call_node, const ir::Call& call);
    struct Row {
        std::optional<apimap::Mapping> mapping;
        const apimap::TargetMap* raw = nullptr;
    };
    Row lookup(const ir::Call& call) const;
    void record(const ir::SymP& sym, Node* init);
    std::optional<std::string> concrete(Node* e) const;
    std::string_view list_limit(Node* e) const;
    void check_list_change(const ir::Call& call) const;
    Node* jlib(const std::string& name, std::vector<Node*> args, const TypeP& type);

    // ---- boxes that may hold a null
    void prepare_boxed_nulls(const std::vector<Node*>& classes);
    bool may_be_null(Node* e) const;
    Node* kept(Node* e);
    Node* to(Node* e, const TypeP& t);
    Node* update(Node* e);

    // ---- types
    TypeP cs(const TypeP& t);
    void retype(Node* root);
    void note_runtime(Node* root);

    Make mk_;
    Arena& arena_;
    const apimap::Registry& registry_;
    std::unordered_map<std::string, ir::ClassDecl*> classes_;
    std::set<std::string> runtime_;
    ir::ClassDecl* cls_ = nullptr;
    std::unordered_map<const ir::FieldDecl*, bool> moved_;
    std::unordered_map<std::string, std::vector<Node*>> prologue_;
    std::unordered_set<std::string> prologue_done_;
    std::unordered_map<const ir::MethodDecl*, std::string> java_names_;   // a method renamed -> its Java name
    bool catches_negative_size_ = false;                     // whether the program catches NegativeArraySizeException
    std::map<std::pair<std::string, std::string>, std::string> concrete_;
    std::unordered_set<std::string> stored_arrays_;
    bool list_over_array_ = false;
    int try_helpers_ = 0;
    std::map<std::string, std::pair<bool, bool>> labels_;   // a label -> its uses (break, continue)
    TypeP ret_;                                              // the result of the method (or lambda) whose body is rewritten
    TypeP yield_type_;                                       // the result of the switch expression whose arm is rewritten
    std::vector<Node*> java_;                                // the Java units (copies)
    std::map<std::string, Hoisted> hoisted_;                 // the classes hoisted out of methods, by qualified name
    std::unordered_map<const Node*, ir::ClassDecl*> hoist_of_;   // a class declared in a method or anonymous (its ClassDecl) -> its hoisted copy
    std::unordered_map<const Node*, std::string> refused_;   // a class that could not be hoisted (its ClassDecl node) -> why
    std::unordered_map<const ir::ClassDecl*, std::string> refused_decl_;   // a class that could not be carried -> why
    std::unordered_map<const Node*, std::string> capture_names_;   // a read of a captured local -> the field (cap_x) or parameter (cap_x_arg)
    std::unordered_map<const Node*, std::set<std::string>> via_field_;   // a New that hands on captures the class making it holds
    std::unordered_set<const Node*> before_base_;            // a New in a constructor's base call (its captures are the parameters)
    std::unordered_map<std::string, ir::SymP> inner_;        // an inner class that holds its outer instance -> its outer_ field
    std::map<std::string, const ir::MethodDecl*> lambda_interfaces_;   // an interface of the program lambdas are made for -> its one method
    std::map<std::string, std::vector<std::string>> object_enums_;   // an enum of objects -> its constants, in order
    std::unordered_set<const Node*> null_reads_;             // reads of a box that may be null
    std::unordered_set<const Node*> read_as_is_;             // ... and those of them that are not read as a number
};

void Pass::index(Node* class_node) {
    auto* cd = ir::get<ir::ClassDecl>(class_node);
    if (cd == nullptr) return;
    if (cd->qname) classes_[*cd->qname] = cd;
    for (Node* m : cd->members) index(m);
}

bool Pass::overrides_base(const ir::ClassDecl& c, const ir::MethodDecl& m) const {
    const auto signature = [](const ir::MethodDecl& md) {
        std::vector<TypeP> sig;
        for (Node* p : md.params) {
            if (const auto* pp = ir::get<ir::Param>(p)) sig.push_back(pp->type);
        }
        return sig;
    };
    const auto mine = signature(m);
    std::set<std::string> seen;
    for (const ir::ClassDecl* b = base_of(c); b != nullptr && b->qname && seen.insert(*b->qname).second; b = base_of(*b)) {
        for (Node* xn : b->members) {
            const auto* x = ir::get<ir::MethodDecl>(xn);
            if (x != nullptr && x->kind == "method" && jname(*x) == jname(m) && signature(*x) == mine && !has(x->mods, "static") && !has(x->mods, "private")) return true;
        }
    }
    return false;
}

bool Pass::closeable(const ir::ClassDecl& start) const {
    std::set<std::string> seen;
    std::vector<const ir::ClassDecl*> todo{&start};   // (its bases, and the interfaces of the program they implement)
    while (!todo.empty()) {
        const ir::ClassDecl* c = todo.back();
        todo.pop_back();
        if (c == nullptr || !c->qname || !seen.insert(*c->qname).second) continue;
        for (const TypeP& i : c->implements) {
            const std::string* q = qual(i);
            if (q != nullptr && (*q == "java.lang.AutoCloseable" || *q == "java.io.Closeable")) return true;
            if (q != nullptr) todo.push_back(class_named(*q));
        }
        todo.push_back(base_of(*c));
    }
    return false;
}

// What of this class the rule set does not carry yet (each named in the header): the reason, or nothing.
std::optional<std::string> Pass::refused_class(ir::ClassDecl& c) {
    if (const auto why = refused_decl_.find(&c); why != refused_decl_.end()) return why->second;
    return std::nullopt;
}

// ---- before the rewrite: what the whole program says ---------------------------------------------------------------------------------

void Pass::prepare(std::vector<Node*>& classes) {
    for (Node* cn : classes) {   // CodeBreaker's Class and Constructor are of no type in C#: what Java types T is an object, cast where it is used
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (c.qname != "java.lang.Class" && c.qname != "java.lang.reflect.Constructor") continue;
        c.typeparams.clear();
        c.tparams.clear();
        const auto plain = [&](const TypeP& t) { return t && t->kind() == "typevar" ? obj_type() : t; };
        for (Node* m : c.members) {
            for (Node* n : ir::walk(m)) {
                n->type = plain(n->type);
                ir::visit(*n, [&](auto& k) {
                    using K = std::decay_t<decltype(k)>;
                    if constexpr (std::is_same_v<K, ir::Local> || std::is_same_v<K, ir::Param> || std::is_same_v<K, ir::FieldDecl>) k.type = plain(k.type);
                    if constexpr (std::is_same_v<K, ir::MethodDecl>) k.ret = plain(k.ret);
                });
            }
        }
    }
    for (Node* cn : classes) {   // an enum of objects is a class: a plain one stays a C# enum
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (c.kind != "enum" || !c.qname) continue;
        bool rich = false, bodies = false;
        std::vector<std::string> names;
        for (Node* m : c.members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            if (const auto* ec = ir::get<ir::EnumConst>(m)) {
                names.push_back(ec->sym->name);
                rich = rich || !ec->args.empty();
                bodies = bodies || ec->body != nullptr;
            } else if (ir::is<ir::FieldDecl>(m) || (md != nullptr && !has(md->mods, "IMPLICIT"))) {
                rich = true;
            }
        }
        if (rich || bodies) object_enums_[*c.qname] = std::move(names);
        if (bodies) {   // what a constant's body must give makes the enum's class abstract
            const bool open = std::any_of(c.members.begin(), c.members.end(), [](Node* m) {
                const auto* md = ir::get<ir::MethodDecl>(m);
                return md != nullptr && has(md->mods, "abstract");
            });
            if (open && !has(c.mods, "abstract")) c.mods.push_back("abstract");
            std::erase(c.mods, "final");
        }
    }
    prepare_local_classes(java_, classes);
    prepare_inner_classes(classes);
    prepare_lambda_classes();
    for (Node* u : java_) {
        for (Node* n : ir::walk(u)) {
            if (const auto* caught = ir::get<ir::Catch>(n)) {
                for (const TypeP& t : caught->types) catches_negative_size_ = catches_negative_size_ || (qual(t) != nullptr && *qual(t) == "java.lang.NegativeArraySizeException");
            }
        }
    }
    for (Node* cn : classes) {   // an abstract class that leaves an interface's method to its subclasses declares it abstract (C# requires it)
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (c.kind != "class" || !has(c.mods, "abstract")) continue;
        std::set<std::pair<std::string, std::size_t>> have;
        std::set<std::string> seen;
        for (ir::ClassDecl* k = &c; k != nullptr && k->qname && seen.insert(*k->qname).second; k = base_of(*k)) {
            for (Node* m : k->members) {
                if (const auto* md = ir::get<ir::MethodDecl>(m)) have.emplace(md->name.value_or(""), md->params.size());
            }
        }
        std::vector<TypeP> todo = c.implements;
        std::set<std::string> faces;
        std::vector<Node*> added;
        while (!todo.empty()) {
            const TypeP t = todo.back();
            todo.pop_back();
            const std::string* q = qual(t);
            ir::ClassDecl* i = q != nullptr ? class_named(*q) : nullptr;
            if (i == nullptr || !i->qname || !faces.insert(*i->qname).second) continue;
            todo.insert(todo.end(), i->implements.begin(), i->implements.end());
            for (Node* mn : i->members) {
                const auto* m = ir::get<ir::MethodDecl>(mn);
                if (m == nullptr || ir::present(m->body) || has(m->mods, "static")) continue;
                const std::pair<std::string, std::size_t> key{m->name.value_or(""), m->params.size()};
                if (have.contains(key)) continue;
                ir::MethodDecl copy;
                copy.sym = m->sym;
                copy.name = m->name;
                copy.kind = "method";
                copy.params = m->params;
                copy.ret = m->ret;
                copy.mods = {"public", "abstract"};
                copy.typeparams = m->typeparams;
                added.push_back(ir::make(arena_, std::move(copy), TypeP{}, mn->line, mn->col));
                have.insert(key);
            }
        }
        c.members.insert(c.members.end(), added.begin(), added.end());
    }
    for (Node* cn : classes) {   // the arrays something stores into: a list made over one (Arrays.asList) would have to see it
        for (Node* n : ir::walk(cn)) {
            const auto stored = [&](Node* target) {
                const auto* at = target != nullptr ? ir::get<ir::Index>(strip(target)) : nullptr;
                if (at != nullptr && at->arr != nullptr && at->arr->type) stored_arrays_.insert(ir::to_text(*at->arr->type));
            };
            if (const auto* a = ir::get<ir::Assign>(n)) stored(a->target);
            if (const auto* c = ir::get<ir::Call>(n); c != nullptr && c->sym != nullptr && c->sym->owner == "java.util.Arrays" && c->sym->name == "asList" &&
                                                      c->args.size() == 1 && c->args[0]->type && c->args[0]->type->kind() == "array")
                list_over_array_ = true;
        }
    }
    prepare_boxed_nulls(classes);
}

// ---- declarations ---------------------------------------------------------------------------------------------------------------------

std::vector<std::string> Pass::class_mods(const ir::ClassDecl& c) const {
    std::vector<std::string> out;
    for (const std::string& m : c.mods) {
        if (m == "public" || m == "protected" || m == "private" || m == "abstract") out.push_back(m);
        else if (m == "final" && c.kind == "class") out.push_back("sealed");
    }
    if (c.kind == "record") out.push_back("sealed");
    if (!c.outer && !has(out, "public")) out.insert(out.begin(), "internal");
    if (c.outer && !has(out, "public") && !has(out, "protected")) {
        std::erase(out, "private");
        out.insert(out.begin(), "internal");
    }
    return out;
}

std::vector<std::string> Pass::access(const std::vector<std::string>& mods, const ir::ClassDecl& c) const {
    std::vector<std::string> acc;
    for (const std::string& m : mods) {
        if (m == "public" || m == "protected" || m == "private") acc.push_back(m);
    }
    if (acc == std::vector<std::string>{"private"} && c.outer) acc = {"internal"};
    if (acc.empty() && c.kind != "interface") acc = {"internal"};
    if (acc == std::vector<std::string>{"protected"}) acc = {"protected", "internal"};
    return acc;
}

bool Pass::is_const(const ir::FieldDecl& f) const {
    if (f.init == nullptr) return false;
    const auto* lit = ir::get<ir::Lit>(strip(f.init));
    return lit != nullptr && !std::holds_alternative<std::monostate>(lit->value) && f.type && (f.type->is_prim() || is_java_string(f.type));
}

std::vector<std::string> Pass::field_mods(const ir::FieldDecl& f, const ir::ClassDecl& c) const {
    const std::vector<std::string>& mods = f.mods;
    if (c.kind == "interface") return is_const(f) ? std::vector<std::string>{"public", "const"} : std::vector<std::string>{"public", "static", "readonly"};
    std::vector<std::string> out = access(mods, c);
    if (has(mods, "static") && has(mods, "final") && is_const(f)) {
        out.push_back("const");
        return out;
    }
    if (has(mods, "static")) out.push_back("static");
    if (has(mods, "final") && f.init != nullptr && !moved_.contains(&f)) out.push_back("readonly");
    if (has(mods, "volatile")) out.push_back("volatile");
    return out;
}

// Whether the type names a class of the program that C# keeps internal.
bool hides(const Pass*, const std::function<ir::ClassDecl*(const std::string&)>& named, const TypeP& t) {
    if (!t) return false;
    if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) return hides(nullptr, named, a->elem);
    const auto* c = std::get_if<ir::ClassType>(&t->body);
    if (c == nullptr) return false;
    if (const ir::ClassDecl* own = named(c->name); own != nullptr && !own->outer && !has(own->mods, "public")) return true;
    return std::any_of(c->args.begin(), c->args.end(), [&](const TypeP& x) { return hides(nullptr, named, x); });
}

std::vector<std::string> Pass::method_mods(const ir::MethodDecl& m, const ir::ClassDecl& c) const {
    std::vector<std::string> out = method_mods_plain(m, c);
    const bool open = has(out, "public") || has(out, "protected");
    if (!open || c.kind == "interface" || has(out, "override")) return out;
    const auto named = [&](const std::string& q) { return class_named(q); };
    bool hidden = hides(this, named, m.ret);
    for (Node* p : m.params) hidden = hidden || hides(this, named, ir::get<ir::Param>(p)->type);
    if (!hidden) return out;
    for (const TypeP& i : c.implements) {
        const std::string* q = qual(i);
        const ir::ClassDecl* face = q != nullptr ? class_named(*q) : nullptr;
        if (face == nullptr) continue;
        for (Node* member : face->members) {
            const auto* d = ir::get<ir::MethodDecl>(member);
            if (d != nullptr && jname(*d) == jname(m) && d->params.size() == m.params.size()) return out;
        }
    }
    std::vector<std::string> lowered{"internal"};
    for (const std::string& x : out) {
        if (x != "public" && x != "protected" && x != "internal") lowered.push_back(x);
    }
    return lowered;
}

std::vector<std::string> Pass::method_mods_plain(const ir::MethodDecl& m, const ir::ClassDecl& c) const {
    const std::vector<std::string>& mods = m.mods;
    if (c.kind == "interface") return has(mods, "static") ? std::vector<std::string>{"static"} : std::vector<std::string>{};
    std::vector<std::string> out = access(mods, c);
    if (m.kind == "ctor") return out;
    if (m.name == "equals" && m.params.size() == 1 && !has(mods, "static")) {   // equals(Thing) is an overload: it overrides nothing of Object's
        const std::string* compared = qual(ir::get<ir::Param>(m.params[0])->type);
        if (compared == nullptr || *compared != "java.lang.Object") {
            if (has(mods, "abstract")) out.push_back("abstract");
            return out;
        }
    }
    if (has(mods, "static")) {
        out.push_back("static");
        return out;
    }
    const bool overrides = object_method(m.name.value_or(""), m.params.size()).has_value() || overrides_base(c, m);
    if (has(mods, "abstract")) {
        out.push_back("abstract");
        if (overrides) out.push_back("override");
        return out;
    }
    if (overrides) {
        for (const ir::ClassDecl* up = base_of(c); up != nullptr; up = base_of(*up)) {   // Java may widen what it overrides; C# wants the base's own access
            for (Node* member : up->members) {
                const auto* d = ir::get<ir::MethodDecl>(member);
                if (d != nullptr && d->kind == "method" && jname(*d) == jname(m) && d->params.size() == m.params.size() && !has(d->mods, "static")) out = access(d->mods, *up);
            }
        }
        out.push_back("override");
        if (has(mods, "final")) out.push_back("sealed");
        return out;
    }
    if (has(mods, "private") || has(mods, "final") || has(c.mods, "final") || has(c.mods, "sealed") || c.kind == "record") return out;
    out.push_back("virtual");
    return out;
}

std::string Pass::method_name(const ir::MethodDecl& m, const ir::ClassDecl& c) const {
    const std::string name = m.name.value_or("");
    const std::size_t n = m.params.size();
    const std::string* compared = name == "equals" && n == 1 ? qual(ir::get<ir::Param>(m.params[0])->type) : nullptr;
    if (name == "equals" && n == 1 && (compared == nullptr || *compared != "java.lang.Object")) return name;
    if (const auto object_name = object_method(name, n)) return std::string(*object_name);
    if (name == "main" && has(m.mods, "static") && n == 1) return "Main";
    if (name == "compareTo" && n == 1 && std::any_of(c.implements.begin(), c.implements.end(), [](const TypeP& i) {
            const std::string* q = qual(i);
            return q != nullptr && *q == "java.lang.Comparable";
        }))
        return "CompareTo";
    if (name == "close" && n == 0 && closeable(c)) return "Dispose";
    if (m.name == c.name) return name + "_";
    return name;
}

std::optional<std::string> Pass::member_name(const ir::Sym& s) const {
    if (s.kind == ir::SymKind::Method) {
        const std::size_t n = s.params.size();
        const ir::ClassDecl* c = s.owner ? class_named(*s.owner) : nullptr;
        if (c != nullptr && s.name == "equals" && n == 1) {
            const std::string* compared = qual(s.params[0]);
            if (compared == nullptr || *compared != "java.lang.Object") return std::nullopt;
        }
        if (const auto object_name = object_method(s.name, n)) return std::string(*object_name);
        if (c != nullptr) {
            if (s.name == "compareTo" && n == 1) return "CompareTo";
            if (s.name == "close" && n == 0 && closeable(*c)) return "Dispose";
            if (c->name && s.name == *c->name) return s.name + "_";
            if (s.name == "main" && s.is_static && n == 1) return "Main";
        }
    }
    if (s.kind == ir::SymKind::Field && s.owner) {   // Java keeps fields and methods apart; C# forbids a field named as a method of its class
        if (const ir::ClassDecl* c = class_named(*s.owner)) {
            const bool method_too = std::any_of(c->members.begin(), c->members.end(), [&](Node* m) {
                const auto* md = ir::get<ir::MethodDecl>(m);
                return md != nullptr && jname(*md) == s.name;
            });
            if (method_too) return s.name + "_";
        }
    }
    return std::nullopt;
}

ir::SymP renamed(const ir::SymP& s, const std::string& name) {
    auto copy = std::make_shared<ir::Sym>(*s);
    copy->name = name;
    return copy;
}

// Java's initialisation order: instance initialisers after the base constructor (moved into the constructors, where the base is one of
// the program's classes), statics in one textual order (in a static constructor, when there is a static block).
std::vector<Node*> Pass::arrange_members(ir::ClassDecl& c) {
    const std::string qname = c.qname.value_or("");
    std::vector<Node*> out, statics, moves;
    bool has_block = false;
    const std::string* base = qual(c.extends);
    const bool base_is_ours = c.extends && base != nullptr && class_named(*base) != nullptr;
    const bool init_needs_this = std::any_of(c.members.begin(), c.members.end(), [&](Node* mn) {
        const auto* f = ir::get<ir::FieldDecl>(mn);
        if (f == nullptr || f->init == nullptr || has(f->mods, "static")) return false;
        for (Node* n : own_nodes(f->init)) {
            if (ir::is<ir::This>(n)) return true;
            if (const auto* read = ir::get<ir::Name>(n); read != nullptr && read->sym && read->sym->kind == ir::SymKind::Field && !read->sym->is_static) return true;
            if (const auto* call = ir::get<ir::Call>(n); call != nullptr && call->target == nullptr && call->sym && !call->sym->is_static) return true;
            const auto* made = ir::get<ir::New>(n);
            if (made == nullptr) continue;
            const ir::ClassDecl* target = nullptr;
            if (made->body != nullptr) {
                const auto found = hoist_of_.find(made->body);
                target = found != hoist_of_.end() ? found->second : nullptr;
            } else if (const std::string* q = qual(n->type)) {
                target = class_named(*q);
            }
            if (target != nullptr && needs_outer(*target)) return true;
        }
        return false;
    });
    for (Node* mn : c.members) {
        const auto* md = ir::get<ir::MethodDecl>(mn);
        if (md != nullptr && md->kind == "static_init") {
            has_block = true;
            statics.push_back(mn);
            continue;
        }
        if (md != nullptr && md->kind == "init") {
            moves.push_back(mn);
            continue;
        }
        if (const auto* f = ir::get<ir::FieldDecl>(mn)) {
            if (has(f->mods, "static") && f->init != nullptr && !is_const(*f)) statics.push_back(mn);
            if (!has(f->mods, "static") && f->init != nullptr && (base_is_ours || needs_outer(c) || has_captures(c) || init_needs_this)) moves.push_back(mn);
        }
        out.push_back(mn);
    }
    const auto assignment = [&](ir::FieldDecl& f, Node* object) {
        Node* target = ir::make(arena_, ir::Select{.obj = object, .sym = f.sym}, f.type);
        return mk_.expr_stmt(ir::make(arena_, ir::Assign{.target = target, .value = f.init}, f.type));
    };
    const auto move_out = [&](Node* node) {
        auto& f = *ir::get<ir::FieldDecl>(node);
        moved_[&f] = true;
        return &f;
    };
    if (has_block) {
        std::vector<Node*> body;
        for (Node* mn : statics) {
            if (ir::get<ir::FieldDecl>(mn) != nullptr) {
                ir::FieldDecl* f = move_out(mn);
                body.push_back(assignment(*f, mk_.type_name(qname)));
                f->init = nullptr;
            } else {
                const std::vector<Node*> s = body_statements(ir::get<ir::MethodDecl>(mn)->body);
                body.insert(body.end(), s.begin(), s.end());
            }
        }
        ir::MethodDecl cctor;
        cctor.name = "<clinit>";
        cctor.kind = "cctor";
        cctor.ret = ir::void_type();
        cctor.body = mk_.block(std::move(body));
        cctor.mods = {"static"};
        out.push_back(ir::make(arena_, std::move(cctor)));
    }
    if (!moves.empty()) {
        std::vector<Node*> prologue;
        for (Node* mn : moves) {
            if (ir::get<ir::FieldDecl>(mn) != nullptr) {
                ir::FieldDecl* f = move_out(mn);
                prologue.push_back(assignment(*f, ir::make(arena_, ir::This{}, ir::class_type(qname))));
                f->init = nullptr;
            } else {
                const std::vector<Node*> s = body_statements(ir::get<ir::MethodDecl>(mn)->body);
                prologue.insert(prologue.end(), s.begin(), s.end());
            }
        }
        prologue_[qname] = std::move(prologue);
    }
    return out;
}

void Pass::rewrite_class(ir::ClassDecl& c) {
    struct Restore {
        ir::ClassDecl*& slot;
        ir::ClassDecl* saved;
        ~Restore() { slot = saved; }
    } restore{cls_, cls_};
    cls_ = &c;
    if (needs_outer(c) || has_captures(c)) {   // Java sets these before the base constructor runs, C# only after it
        const ir::ClassDecl* base = base_of(c);
        if (base != nullptr && !base_construction_is_closed(base))
            throw Declined{"inner-class", "a class whose base constructor may call back into it before its outer instance and captured locals are set (" +
                                              c.name.value_or("") + ")"};
    }
    TypeP comparator, iterable;   // a Comparator converts to the delegate that stands for one; an Iterable gives .NET's enumerator
    for (const TypeP& i : c.implements) {
        const std::string* q = qual(i);
        if (q != nullptr && *q == "java.util.Comparator") comparator = i;
        if (q != nullptr && *q == "java.lang.Iterable") iterable = i;
    }
    std::erase_if(c.implements, [](const TypeP& i) {
        const std::string* q = qual(i);
        return q != nullptr && (*q == "java.io.Serializable" || *q == "java.lang.Cloneable" || *q == "java.util.RandomAccess" || *q == "java.util.Comparator");
    });
    c.mods = class_mods(c);
    struct Record {
        ir::ClassDecl& c;
        ~Record() {
            if (c.kind == "record") c.kind = "class";
        }
    } record{c};
    if (c.kind == "enum" && c.qname && object_enum(*c.qname) != nullptr) enum_members(c);
    if (c.kind == "enum") {   // javac's values, valueOf and constructor of a plain enum: C#'s enum has its own
        std::erase_if(c.members, [](Node* m) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            return md != nullptr && has(md->mods, "IMPLICIT");
        });
    }
    c.members = arrange_members(c);
    const std::string qname = c.qname.value_or("");
    for (Node*& mn : c.members) {
        if (auto* nested = ir::get<ir::ClassDecl>(mn)) {
            if (const auto why = refused_class(*nested)) {
                mn = mk_.unsupported("class", *why + " (" + nested->name.value_or("") + ")");
                continue;
            }
            try {
                rewrite_class(*nested);
            } catch (const Declined& d) {
                mn = mk_.unsupported(d.kind, d.why);
            }
            continue;
        }
        try {
            if (auto* f = ir::get<ir::FieldDecl>(mn)) {
                const std::vector<std::string> mods = field_mods(*f, c);
                if (auto n = member_name(*f->sym)) f->sym = renamed(f->sym, *n);
                if (f->init != nullptr) f->init = to(f->init, f->type);
                f->mods = mods;
                continue;
            }
            auto* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr) continue;
            if (m->kind == "cctor" || m->kind == "static_init") {
                suite(m->body);
                continue;
            }
            const std::vector<std::string> mods = m->kind == "ctor" ? access(m->mods, c) : method_mods(*m, c);
            if (m->kind == "method") {
                java_names_.emplace(m, m->name.value_or(""));
                m->name = method_name(*m, c);
            }
            if (m->kind == "ctor") {
                const bool implicit = has(m->mods, "IMPLICIT");
                const auto pro = prologue_.find(qname);
                const bool needed = c.kind == "record" || (pro != prologue_.end() && !pro->second.empty()) || needs_outer(c) || has_captures(c);
                m->mods = mods;
                if (m->sym != nullptr && m->sym->varargs && !m->params.empty()) {   // Java's T... is C#'s params T[]
                    if (auto* last = ir::get<ir::Param>(m->params.back()); last != nullptr && !has(last->mods, "params")) last->mods.push_back("params");
                }
                if (implicit && !needed) m->mods.push_back("IMPLICIT");
                labels_.clear();
                ret_ = TypeP{};
                m->body = mk_.block(body_statements(m->body));
                suite(m->body);
                if (pro != prologue_.end() && prologue_done_.insert(qname).second) stmts(pro->second);   // (rewritten once, copied into each)
                // the statements Java runs after the base call: in each constructor that does not call another of its own
                std::vector<Node*> body = body_statements(m->body);
                const auto* first = body.empty() ? nullptr : ir::get<ir::CtorCall>(body[0]);
                if (!(first != nullptr && first->kind == "this")) {
                    std::vector<Node*> copies;
                    const auto set_field = [&](const ir::SymP& field, std::size_t at) {
                        const auto* given = ir::get<ir::Param>(m->params.at(at));
                        Node* self = ir::make(arena_, ir::This{}, ir::class_type(qname));
                        copies.push_back(mk_.expr_stmt(mk_.assign(ir::make(arena_, ir::Select{.obj = self, .sym = field}, field->type), mk_.name(given->sym, field->type))));
                    };
                    if (needs_outer(c) && !m->params.empty()) set_field(inner_.at(qname), 0);   // the outer instance first: what follows may use it
                    if (const Hoisted* h = hoisted(c)) {   // then the locals it read, which follow the outer instance among the parameters
                        const std::size_t at = needs_outer(c) ? 1 : 0;
                        for (std::size_t i = 0; i < h->fields.size(); ++i) set_field(h->fields[i], at + i);
                    }
                    if (pro != prologue_.end()) {
                        for (Node* s : pro->second) copies.push_back(ir::clone(arena_, s));
                    }
                    body.insert(body.begin() + (first != nullptr ? 1 : 0), copies.begin(), copies.end());
                }
                m->body = mk_.block(std::move(body));
                continue;
            }
            m->mods = mods;
            if (m->sym != nullptr && m->sym->varargs && !m->params.empty()) {   // Java's T... is C#'s params T[]
                if (auto* last = ir::get<ir::Param>(m->params.back()); last != nullptr && !has(last->mods, "params")) last->mods.push_back("params");
            }
            labels_.clear();
            ret_ = m->ret;
            suite(m->body);
        } catch (const Declined& d) {
            if (auto* m = ir::get<ir::MethodDecl>(mn)) {
                m->body = mk_.block({mk_.expr_stmt(mk_.unsupported(d.kind, d.why))});
            } else {
                mn = mk_.unsupported(d.kind, d.why);
            }
        }
    }
    const auto item_of = [](const TypeP& t) {
        const auto& args = std::get<ir::ClassType>(t->body).args;
        return args.empty() ? obj_type() : args[0];
    };
    const auto method = [&](const std::string& name, const TypeP& ret, std::vector<std::string> mods, std::vector<Node*> params, Node* result) {
        ir::MethodDecl m;
        m.sym = mk_.method(name, qname);
        m.name = name;
        m.kind = "method";
        m.ret = ret;
        m.mods = std::move(mods);
        m.params = std::move(params);
        m.body = mk_.block({ir::make(arena_, ir::Return{.expr = result})});
        c.members.push_back(ir::make(arena_, std::move(m)));
    };
    if (comparator && c.kind != "interface") {   // handed over wherever Java wants a Comparator: `public static implicit operator Comparison<T>(C by) => by.compare;`
        const TypeP self = own_type(qname);
        const TypeP to = ir::class_type("System.Comparison", {item_of(comparator)});
        const ir::SymP by = ir::make_sym(ir::SymKind::Param, "by", std::nullopt, false, self);
        Node* group = ir::make(arena_, ir::MethodRef{.target = mk_.name(by, self), .sym = mk_.method("compare", qname)}, to);
        method("op_Implicit", to, {"public", "static", "operator"}, {ir::make(arena_, ir::Param{.sym = by, .type = self})}, group);
    }
    if (iterable && c.kind != "interface") {   // `foreach` walks its iterator()
        const TypeP item = item_of(iterable);
        Node* walk = mk_.call(nullptr, "iterator", {}, ir::class_type("java.util.Iterator", {item}));
        method("GetEnumerator", ir::class_type("System.Collections.Generic.IEnumerator", {item}), {"public"}, {},
               jlib("Walk", {walk}, ir::class_type("System.Collections.Generic.IEnumerator", {item})));
        method("System.Collections.IEnumerable.GetEnumerator", ir::class_type("System.Collections.IEnumerator"), {}, {},
               mk_.call(nullptr, "GetEnumerator", {}, ir::class_type("System.Collections.Generic.IEnumerator", {item})));
    }
}

// ---- statements ---------------------------------------------------------------------------------------------------------------------

void Pass::suite(ir::Suite& s) {
    if (auto* one = std::get_if<Node*>(&s)) {
        if (*one != nullptr) *one = stmt_node(*one);
        return;
    }
    stmts(std::get<std::vector<Node*>>(s));
}

Node* Pass::stmt_node(Node* s) {
    std::vector<Node*> got = stmt(s);
    if (got.size() == 1) return got[0];
    return mk_.block(std::move(got));
}

void Pass::stmts(std::vector<Node*>& list) {
    std::vector<Node*> out;
    for (Node* s : list) {
        std::vector<Node*> got = stmt(s);
        out.insert(out.end(), got.begin(), got.end());
    }
    list = std::move(out);
}

bool Pass::jumps_out(Node* n, int loops, std::set<std::string> labels, int switches) const {
    if (n == nullptr) return false;
    if (ir::is<ir::Return>(n)) return true;
    if (const auto* b = ir::get<ir::Break>(n)) return b->label ? !labels.contains(*b->label) : loops + switches == 0;
    if (const auto* c = ir::get<ir::Continue>(n)) return c->label ? !labels.contains(*c->label) : loops == 0;
    if (ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n) || ir::is<ir::LocalClass>(n)) return false;
    if (ir::is<ir::While>(n) || ir::is<ir::DoWhile>(n) || ir::is<ir::For>(n) || ir::is<ir::ForEach>(n)) ++loops;
    if (ir::is<ir::Switch>(n)) ++switches;
    if (const auto* l = ir::get<ir::Labeled>(n); l != nullptr && l->label) labels.insert(*l->label);
    for (Node* c : ir::children(*n)) {
        if (jumps_out(c, loops, labels, switches)) return true;
    }
    return false;
}

Node* labelled_empty(Arena& arena, const std::string& label) {
    return ir::make(arena, ir::Labeled{.label = label, .body = ir::make(arena, ir::Empty{})});
}

std::vector<Node*> Pass::stmt(Node* s) {
    if (s == nullptr) return {};
    if (auto* x = ir::get<ir::ExprStmt>(s)) {
        Node* e = strip(x->expr);
        x->expr = ir::is<ir::Call>(e) ? call(e, true) : expr(x->expr);
        return {s};
    }
    if (auto* x = ir::get<ir::Local>(s)) {
        if (x->init != nullptr) {
            record(x->sym, x->init);
            x->init = to(x->init, x->type);
        }
        return {s};
    }
    if (auto* x = ir::get<ir::Return>(s)) {
        if (x->expr != nullptr) x->expr = to(x->expr, ret_);
        return {s};
    }
    if (auto* x = ir::get<ir::If>(s)) {
        x->cond = expr(x->cond);
        suite(x->then);
        suite(x->orelse);
        return {s};
    }
    if (auto* x = ir::get<ir::Block>(s)) {
        stmts(x->stmts);
        return {s};
    }
    if (auto* x = ir::get<ir::While>(s)) {
        x->cond = expr(x->cond);
        suite(x->body);
        return {s};
    }
    if (auto* x = ir::get<ir::DoWhile>(s)) {
        suite(x->body);
        x->cond = expr(x->cond);
        return {s};
    }
    if (auto* x = ir::get<ir::For>(s)) {
        stmts(x->init);
        if (x->cond != nullptr) x->cond = expr(x->cond);
        for (Node*& u : x->update) u = expr(u);
        suite(x->body);
        return {s};
    }
    if (auto* x = ir::get<ir::ForEach>(s)) {
        Node* it = strip(x->iterable);
        const std::string* q = qual(x->iterable->type);
        if (q != nullptr) {
            const auto fam = family_table().find(*q);
            if (fam != family_table().end() && (fam->second == "Set" || fam->second == "Map")) {
                const std::optional<std::string> held = concrete(x->iterable);
                if (!held || !ordered(*held)) throw Declined{"hash-order", "iterating a " + held.value_or("?") + ": Java visits it in hash order, .NET in another"};
            }
        }
        x->iterable = ir::is<ir::Call>(it) ? call(it, false) : expr(x->iterable);
        x->body = stmt_node(x->body);
        return {s};
    }
    if (auto* x = ir::get<ir::Labeled>(s)) {
        const std::string label = x->label.value_or("");
        const auto* single = std::get_if<Node*>(&x->body);
        Node* bn = single != nullptr ? *single : nullptr;
        std::pair<bool, bool> uses{false, false};
        for (Node* root : ir::statements(x->body)) {
            for (Node* n : ir::walk(root)) {
                if (const auto* b = ir::get<ir::Break>(n); b != nullptr && b->label == label) uses.first = true;
                if (const auto* c = ir::get<ir::Continue>(n); c != nullptr && c->label == label) uses.second = true;
            }
        }
        labels_[label] = uses;
        const bool loop = bn != nullptr && (ir::is<ir::While>(bn) || ir::is<ir::DoWhile>(bn) || ir::is<ir::For>(bn) || ir::is<ir::ForEach>(bn));
        std::vector<Node*> out;
        if (loop) {
            out = stmt(bn);
            if (uses.second) {   // `continue label`: to the end of the loop's body
                ir::Suite* body = nullptr;
                if (auto* w = ir::get<ir::While>(bn)) body = &w->body;
                else if (auto* d = ir::get<ir::DoWhile>(bn)) body = &d->body;
                else if (auto* f = ir::get<ir::For>(bn)) body = &f->body;
                if (body != nullptr) {
                    std::vector<Node*> list = body_statements(*body);
                    list.push_back(labelled_empty(arena_, label + "_continue"));
                    *body = mk_.block(std::move(list));
                } else if (auto* fe = ir::get<ir::ForEach>(bn)) {
                    std::vector<Node*> list = body_statements(ir::Suite(fe->body));
                    list.push_back(labelled_empty(arena_, label + "_continue"));
                    fe->body = mk_.block(std::move(list));
                }
            }
        } else {
            for (Node* inner : ir::statements(x->body)) {
                std::vector<Node*> got = stmt(inner);
                out.insert(out.end(), got.begin(), got.end());
            }
        }
        if (uses.first) out.push_back(labelled_empty(arena_, label + "_break"));
        return out;
    }
    if (const auto* x = ir::get<ir::Break>(s)) {
        if (x->label && !x->label->empty()) return {ir::make(arena_, ir::Goto{.label = *x->label + "_break"})};
        return {s};
    }
    if (const auto* x = ir::get<ir::Continue>(s)) {
        if (x->label && !x->label->empty()) return {ir::make(arena_, ir::Goto{.label = *x->label + "_continue"})};
        return {s};
    }
    if (auto* x = ir::get<ir::Throw>(s)) {
        if (x->expr != nullptr) x->expr = expr(x->expr);
        return {s};
    }
    if (auto* x = ir::get<ir::Sync>(s)) {
        x->lock = expr(x->lock);
        x->body = stmt_node(x->body);
        return {s};
    }
    if (ir::is<ir::Assert>(s)) return {ir::make(arena_, ir::Comment{.text = "assert dropped: Java runs asserts only with -ea"})};
    if (auto* x = ir::get<ir::Switch>(s)) {
        x->expr = enum_scrutinee(x->expr, x->cases);
        for (Node* cn : x->cases) {
            auto* c = ir::get<ir::Case>(cn);
            for (Node*& l : c->labels) {
                if (!ir::is<ir::Select>(strip(l))) l = expr(l);
            }
            stmts(c->body);
        }
        return {s};
    }
    if (ir::is<ir::Try>(s)) return try_stmt(s);
    if (auto* x = ir::get<ir::CtorCall>(s)) {
        for (Node*& a : x->args) a = expr(a);
        return {s};
    }
    if (const auto* lc = ir::get<ir::LocalClass>(s)) {
        if (hoist_of_.contains(lc->decl)) return {};
        const auto why = refused_.find(lc->decl);
        throw Declined{"local-class", why != refused_.end() ? why->second : std::string("a class declared in a method")};
    }
    if (auto* x = ir::get<ir::Yield>(s)) {
        if (!yield_type_) throw Declined{"switch-expression", "a yield outside a switch expression"};
        x->expr = to(x->expr, yield_type_);
        return {s};
    }
    return {s};
}

// C#'s `using` lets a throwing Dispose replace the exception on its way; Java keeps that one and adds the close's to it as suppressed.
std::vector<Node*> Pass::using_stmt(std::span<Node* const> resources, Node* body) {
    Node* r = resources[0];
    std::vector<Node*> out;
    ir::SymP held;
    TypeP held_type;
    if (auto* local = ir::get<ir::Local>(r)) {
        local->init = expr(local->init);
        held = local->sym;
        held_type = local->type;
        out.push_back(r);
    } else if (auto* given = ir::get<ir::ExprStmt>(r)) {
        held_type = given->expr->type;
        held = ir::make_sym(ir::SymKind::Local, "j_resource" + std::to_string(try_helpers_ + 1), std::nullopt, false, held_type);
        out.push_back(ir::make(arena_, ir::Local{.sym = held, .type = held_type, .init = expr(given->expr)}));
    } else {
        throw Declined{"using", "a resource " + std::string(ir::kind_name(*r))};
    }
    ++try_helpers_;
    const TypeP exception = ir::class_type("System.Exception");
    const ir::SymP thrown = ir::make_sym(ir::SymKind::Local, "j_thrown" + std::to_string(try_helpers_), std::nullopt, false, exception);
    const ir::SymP caught = ir::make_sym(ir::SymKind::Local, "j_caught" + std::to_string(try_helpers_), std::nullopt, false, exception);
    out.push_back(ir::make(arena_, ir::Local{.sym = thrown, .type = exception, .init = mk_.null()}));
    Node* inner = nullptr;
    if (resources.size() > 1) {
        inner = mk_.block(using_stmt(resources.subspan(1), body));
    } else {
        inner = stmt_node(body);
        if (!ir::is<ir::Block>(inner)) inner = mk_.block({inner});
    }
    Node* keep = mk_.expr_stmt(mk_.assign(mk_.name(thrown, exception), mk_.name(caught, exception)));
    Node* handler = ir::make(arena_, ir::Catch{.sym = caught, .types = {exception}, .body = mk_.block({keep, ir::make(arena_, ir::Throw{})})});
    Node* close = mk_.expr_stmt(jlib("CloseResource", {mk_.name(held, held_type), mk_.name(thrown, exception)}, ir::void_type()));
    out.push_back(ir::make(arena_, ir::Try{.body = inner, .catches = {handler}, .finally_body = mk_.block({close})}));
    return {mk_.block(std::move(out))};
}

std::vector<Node*> Pass::try_stmt(Node* s) {
    auto& t = *ir::get<ir::Try>(s);
    if (t.finally_body != nullptr && jumps_out(t.finally_body, 0, {})) {   // C# forbids leaving a finally: it is written after the try
        bool body_jumps = jumps_out(t.body, 0, {});
        for (Node* c : t.catches) body_jumps = body_jumps || jumps_out(c, 0, {});
        if (body_jumps) throw Declined{"finally", "a finally that returns or jumps, around a body that also does"};
        ++try_helpers_;
        const TypeP exception = ir::class_type("System.Exception");
        const ir::SymP thrown = ir::make_sym(ir::SymKind::Local, "j_thrown" + std::to_string(try_helpers_), std::nullopt, false, exception);
        const ir::SymP caught = ir::make_sym(ir::SymKind::Local, "j_caught" + std::to_string(try_helpers_), std::nullopt, false, exception);
        Node* after = t.finally_body;
        t.finally_body = nullptr;
        std::vector<Node*> out;
        out.push_back(ir::make(arena_, ir::Local{.sym = thrown, .type = exception, .init = mk_.null()}));
        Node* tried = (t.resources.empty() && t.catches.empty()) ? stmt_node(t.body) : mk_.block(try_stmt(s));
        if (!ir::is<ir::Block>(tried)) tried = mk_.block({tried});
        Node* keep = mk_.expr_stmt(mk_.assign(mk_.name(thrown, exception), mk_.name(caught, exception)));
        Node* handler = ir::make(arena_, ir::Catch{.sym = caught, .types = {exception}, .body = mk_.block({keep})});
        out.push_back(ir::make(arena_, ir::Try{.body = tried, .catches = {handler}}));
        out.push_back(stmt_node(after));
        Node* test = ir::make(arena_, ir::Binary{.op = "!=", .left = mk_.name(thrown, exception), .right = mk_.null()}, ir::bool_type());
        out.push_back(ir::make(arena_, ir::If{.cond = test, .then = ir::Suite(ir::make(arena_, ir::Throw{.expr = mk_.name(thrown, exception)}))}));
        return {mk_.block(std::move(out))};
    }
    std::vector<Node*> resources = t.resources;
    t.resources.clear();
    if (!resources.empty()) {
        std::vector<Node*> got = using_stmt(resources, t.body);
        if (t.catches.empty() && t.finally_body == nullptr) return got;
        Node* used = got.size() == 1 ? got[0] : mk_.block(std::move(got));
        t.body = used;
    } else {
        t.body = stmt_node(t.body);
    }
    for (Node* cn : t.catches) {
        auto* c = ir::get<ir::Catch>(cn);
        c->body = stmt_node(c->body);
    }
    if (t.finally_body != nullptr) t.finally_body = stmt_node(t.finally_body);
    return {s};
}

// ---- boxes that may hold a null ---------------------------------------------------------------------------------------------------
//
// A Java box is a reference: it may be null where the number it holds is read, and reading it then throws. The translation writes a box as
// the number unless a null can reach it. What can is found by a flow over the program (prepare_boxed_nulls): a local, field, parameter or
// result that a null literal, a missing map key, a read of a collection or array that holds nulls, or another such place can be given;
// such a place is `int?`, a collection or array that holds a null is of `int?`. Each read of one (null_reads_) is `JLib.Unbox(x)` where
// Java reads the number, and is kept as it is where Java keeps the reference (a comparison with null, the text of it, a place that is
// itself `int?` or an Object).

bool is_null_box(const TypeP& t) { return is_box(t) && std::get<ir::ClassType>(t->body).nullable; }

TypeP null_box(const TypeP& t) {
    ir::ClassType c = std::get<ir::ClassType>(t->body);
    c.nullable = true;
    return ir::make_type(std::move(c));
}

TypeP plain_box(const TypeP& t) {
    if (!is_box(t)) return t;
    ir::ClassType c = std::get<ir::ClassType>(t->body);
    c.nullable = false;
    return ir::make_type(std::move(c));
}

// `map.get(k)` of a map whose values are boxes: a key it does not hold gives a null.
bool map_get(const Node* x) {
    const auto* c = ir::get<ir::Call>(x);
    if (c == nullptr || c->sym == nullptr || c->sym->name != "get" || c->args.size() != 1 || !is_box(x->type)) return false;
    const auto fam = family_table().find(c->sym->owner.value_or(""));
    return fam != family_table().end() && fam->second == "Map";
}

void Pass::prepare_boxed_nulls(const std::vector<Node*>& classes) {
    for (Node* cn : classes) {   // a call whose result is dropped reads no number
        for (Node* n : ir::walk(cn)) {
            if (const auto* s = ir::get<ir::ExprStmt>(n)) read_as_is_.insert(strip(s->expr));
        }
    }
    std::unordered_set<std::string> may;    // what may hold a null: L:scope:name, F:class.field, R:name/n, P:name/n:i
    std::unordered_set<std::string> held;   // the kinds of box some collection or array holds a null of: all of that kind are `T?` there
    std::function<bool(const TypeP&)> holds = [&](const TypeP& t) -> bool {
        if (!t) return false;
        if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) return (is_box(a->elem) && held.contains(*qual(a->elem))) || holds(a->elem);
        if (const auto* c = std::get_if<ir::ClassType>(&t->body))
            return std::any_of(c->args.begin(), c->args.end(), [&](const TypeP& x) { return (is_box(x) && held.contains(*qual(x))) || holds(x); });
        return false;
    };
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
    const auto own_method = [&](const ir::Sym& s) { return s.owner && class_named(*s.owner) != nullptr; };
    std::function<bool(Node*, const std::string&)> nullish = [&](Node* e, const std::string& scope) -> bool {
        if (e == nullptr) return false;
        Node* x = strip(e);
        if (null_lit(x) || map_get(x)) return true;
        if (const auto* i = ir::get<ir::Index>(x)) return is_box(x->type) && holds(i->arr->type);
        if (const auto* cast = ir::get<ir::Cast>(x); cast != nullptr && is_box(x->type)) {   // (Integer)o: a null stays a null
            Node* inner = strip(cast->x);
            return is_box(inner->type) ? nullish(inner, scope) : inner->type && !inner->type->is_prim();
        }
        if (const auto* c = ir::get<ir::Call>(x); c != nullptr && c->target != nullptr && is_box(x->type) && holds(c->target->type)) return true;
        if (const auto* n = ir::get<ir::Name>(x)) return is_box(x->type) && may.contains(of_sym(n->sym, scope));
        if (const auto* s = ir::get<ir::Select>(x)) return is_box(x->type) && may.contains(of_sym(s->sym, scope));
        if (const auto* c = ir::get<ir::Call>(x)) return c->sym != nullptr && is_box(x->type) && own_method(*c->sym) && may.contains(of_result(*c->sym));
        if (const auto* c = ir::get<ir::Cond>(x)) return is_box(x->type) && (nullish(c->then, scope) || nullish(c->orelse, scope));
        if (const auto* a = ir::get<ir::Assign>(x); a != nullptr && !a->op) return nullish(a->value, scope);
        return false;
    };
    bool grew = false;
    const auto mark = [&](std::string key) {
        if (!key.empty() && may.insert(std::move(key)).second) grew = true;
    };
    const auto mark_args = [&](const std::string& method, const std::vector<Node*>& args, const ir::Sym& s, const std::string& scope) {
        for (std::size_t i = 0; i < args.size() && i < s.params.size(); ++i) {
            if (is_box(s.params[i]) && nullish(args[i], scope)) mark(of_param(method, s.params.size(), i));
        }
    };
    const auto hold = [&](const TypeP& box) {
        if (is_box(box) && held.insert(*qual(box)).second) grew = true;
    };
    // what one node says: `ret` is the method's result while its own body is walked (a lambda's return is the lambda's)
    std::function<void(Node*, const std::string&, const std::string&)> look = [&](Node* n, const std::string& scope, const std::string& ret) {
        if (n == nullptr) return;
        if (const auto* c = ir::get<ir::Call>(n); c != nullptr && c->sym != nullptr && c->target != nullptr && c->target->type && !own_method(*c->sym)) {   // a null stored
            static const std::unordered_set<std::string_view> stores = {"add", "addFirst", "addLast", "offer", "push", "put", "putIfAbsent", "replace", "set"};
            if (const auto* into = std::get_if<ir::ClassType>(&c->target->type->body); into != nullptr && stores.contains(c->sym->name)) {
                if (std::any_of(c->args.begin(), c->args.end(), [&](Node* a) { return nullish(a, scope); })) {
                    for (const TypeP& x : into->args) hold(x);
                }
            }
        }
        if (const auto* a = ir::get<ir::NewArray>(n)) {   // a Java array of boxes starts full of nulls
            const auto* made = n->type ? std::get_if<ir::ArrayType>(&n->type->body) : nullptr;
            if (made != nullptr) {
                TypeP inner = made->elem;
                while (inner && std::holds_alternative<ir::ArrayType>(inner->body)) inner = std::get<ir::ArrayType>(inner->body).elem;
                if (!a->dims.empty()) hold(inner);
                if (std::any_of(a->init.begin(), a->init.end(), [&](Node* item) { return nullish(item, scope); })) hold(inner);
            }
        }
        if (const auto* a = ir::get<ir::Assign>(n); a != nullptr && !a->op && ir::is<ir::Index>(strip(a->target)) && nullish(a->value, scope)) hold(a->target->type);
        if (const auto* each = ir::get<ir::ForEach>(n); each != nullptr && holds(each->iterable->type)) {
            if (const auto* v = ir::get<ir::Local>(each->var); v != nullptr && is_box(v->type)) mark(of_sym(v->sym, scope));
        }
        if (const auto* l = ir::get<ir::Local>(n); l != nullptr && is_box(l->type) && nullish(l->init, scope)) mark(of_sym(l->sym, scope));
        if (const auto* a = ir::get<ir::Assign>(n); a != nullptr && !a->op && is_box(a->target->type) && nullish(a->value, scope)) {
            Node* t = strip(a->target);
            if (const auto* name = ir::get<ir::Name>(t)) mark(of_sym(name->sym, scope));
            if (const auto* sel = ir::get<ir::Select>(t)) mark(of_sym(sel->sym, scope));
        }
        if (const auto* r = ir::get<ir::Return>(n); r != nullptr && !ret.empty() && nullish(r->expr, scope)) mark(ret);
        if (const auto* c = ir::get<ir::Call>(n); c != nullptr && c->sym != nullptr && own_method(*c->sym)) mark_args(c->sym->name, c->args, *c->sym, scope);
        if (const auto* w = ir::get<ir::New>(n); w != nullptr && w->sym != nullptr && own_method(*w->sym))
            mark_args("<init>" + w->sym->owner.value_or(""), w->args, *w->sym, scope);
        const std::string inner = ir::is<ir::Lambda>(n) ? std::string() : ret;
        for (Node* child : ir::children(*n)) look(child, scope, inner);
    };
    const auto method_id = [](const ir::ClassDecl& c, const ir::MethodDecl& m) { return m.kind == "ctor" ? "<init>" + c.qname.value_or("") : m.name.value_or(""); };
    const auto scope_of = [](const ir::ClassDecl& c, const Node* m) { return c.qname.value_or("") + "#" + std::to_string(reinterpret_cast<std::uintptr_t>(m)); };
    do {
        grew = false;
        for (Node* cn : classes) {
            const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
            for (Node* x : c.members) {
                if (const auto* f = ir::get<ir::FieldDecl>(x); f != nullptr && is_box(f->type) && (f->init == nullptr || nullish(f->init, ""))) mark(of_sym(f->sym, ""));
                const auto* m = ir::get<ir::MethodDecl>(x);
                if (m == nullptr) {
                    if (ir::is<ir::FieldDecl>(x)) look(x, "", "");
                    continue;
                }
                const std::string scope = scope_of(c, x);
                for (std::size_t i = 0; i < m->params.size(); ++i) {
                    const auto* p = ir::get<ir::Param>(m->params[i]);
                    if (p != nullptr && is_box(p->type) && may.contains(of_param(method_id(c, *m), m->params.size(), i))) mark(of_sym(p->sym, scope));
                }
                look(x, scope, m->kind == "method" && is_box(m->ret) && m->sym != nullptr ? of_result(*m->sym) : std::string());
            }
        }
    } while (grew);
    if (may.empty() && held.empty()) return;
    // the element types of what holds nulls: List<Integer> is List<int?>, Integer[] is int?[]
    std::function<TypeP(const TypeP&)> holding = [&](const TypeP& t) -> TypeP {
        if (!t || held.empty()) return t;
        if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) {
            ir::ArrayType next = *a;
            next.elem = is_box(a->elem) && held.contains(*qual(a->elem)) ? null_box(a->elem) : holding(a->elem);
            return ir::make_type(std::move(next));
        }
        if (const auto* c = std::get_if<ir::ClassType>(&t->body); c != nullptr && !c->args.empty()) {
            ir::ClassType next = *c;
            for (TypeP& x : next.args) x = is_box(x) && held.contains(*qual(x)) ? null_box(x) : holding(x);
            return ir::make_type(std::move(next));
        }
        return t;
    };
    if (!held.empty()) {
        for (Node* cn : classes) {
            for (Node* m : ir::get<ir::ClassDecl>(cn)->members) {
                for (Node* n : own_nodes(m)) {
                    ir::visit(*n, [&](auto& k) {
                        using K = std::decay_t<decltype(k)>;
                        if constexpr (std::is_same_v<K, ir::Local> || std::is_same_v<K, ir::Param> || std::is_same_v<K, ir::FieldDecl>) k.type = holding(k.type);
                        if constexpr (std::is_same_v<K, ir::MethodDecl>) k.ret = holding(k.ret);
                        if constexpr (std::is_same_v<K, ir::NewArray>) k.elem = holding(k.elem);
                    });
                    n->type = holding(n->type);
                }
            }
        }
    }
    // the declarations become `T?`, and each read of one is known
    std::function<void(Node*, const std::string&)> rewrite = [&](Node* n, const std::string& scope) {
        if (n == nullptr) return;
        for (Node* child : ir::children(*n)) rewrite(child, scope);
        if (is_box(n->type) && !is_null_box(n->type)) {   // a read out of what holds nulls
            const auto* index = ir::get<ir::Index>(n);
            const auto* from = ir::get<ir::Call>(n);
            const auto* cast = ir::get<ir::Cast>(n);
            const bool cast_may = cast != nullptr && nullish(n, scope);
            if (cast_may || (index != nullptr && holds(index->arr->type)) || (from != nullptr && from->target != nullptr && holds(from->target->type))) {
                n->type = null_box(n->type);
                null_reads_.insert(n);
            }
        }
        if (auto* l = ir::get<ir::Local>(n); l != nullptr && is_box(l->type) && may.contains(of_sym(l->sym, scope))) {
            l->type = null_box(l->type);
        } else if (const auto* name = ir::get<ir::Name>(n); name != nullptr && is_box(n->type) && may.contains(of_sym(name->sym, scope))) {
            n->type = null_box(n->type);
            null_reads_.insert(n);
        } else if (const auto* sel = ir::get<ir::Select>(n); sel != nullptr && is_box(n->type) && may.contains(of_sym(sel->sym, scope))) {
            n->type = null_box(n->type);
            null_reads_.insert(n);
        } else if (auto* call = ir::get<ir::Call>(n); call != nullptr && call->sym != nullptr && own_method(*call->sym)) {
            auto sym = std::make_shared<ir::Sym>(*call->sym);
            for (std::size_t i = 0; i < sym->params.size(); ++i) {
                if (is_box(sym->params[i]) && may.contains(of_param(sym->name, sym->params.size(), i))) sym->params[i] = null_box(sym->params[i]);
            }
            call->sym = std::move(sym);
            if (is_box(n->type) && may.contains(of_result(*call->sym))) {
                n->type = null_box(n->type);
                null_reads_.insert(n);
            }
        } else if (auto* made = ir::get<ir::New>(n); made != nullptr && made->sym != nullptr && own_method(*made->sym)) {
            const std::string id = "<init>" + made->sym->owner.value_or("");
            auto sym = std::make_shared<ir::Sym>(*made->sym);
            for (std::size_t i = 0; i < sym->params.size(); ++i) {
                if (is_box(sym->params[i]) && may.contains(of_param(id, sym->params.size(), i))) sym->params[i] = null_box(sym->params[i]);
            }
            made->sym = std::move(sym);
        } else if (ir::is<ir::Cond>(n) && is_box(n->type) && nullish(n, scope)) {
            n->type = null_box(n->type);
        }
        // what is assigned to, and a call whose result is dropped, is not a read of the number
        if (const auto* a = ir::get<ir::Assign>(n)) read_as_is_.insert(strip(a->target));
        if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && (u->op == "++" || u->op == "--")) read_as_is_.insert(strip(u->x));
    };
    for (Node* cn : classes) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        for (Node* x : c.members) {
            if (auto* f = ir::get<ir::FieldDecl>(x)) {
                rewrite(x, "");
                if (is_box(f->type) && may.contains(of_sym(f->sym, ""))) f->type = null_box(f->type);
                continue;
            }
            auto* m = ir::get<ir::MethodDecl>(x);
            if (m == nullptr) continue;
            const std::string scope = scope_of(c, x);
            rewrite(x, scope);
            for (std::size_t i = 0; i < m->params.size(); ++i) {
                auto* p = ir::get<ir::Param>(m->params[i]);
                if (p != nullptr && is_box(p->type) && may.contains(of_sym(p->sym, scope))) p->type = null_box(p->type);
            }
            if (m->kind == "method" && is_box(m->ret) && m->sym != nullptr && may.contains(of_result(*m->sym))) m->ret = null_box(m->ret);
        }
    }
}

bool Pass::may_be_null(Node* e) const {
    Node* x = strip(e);
    if (null_reads_.contains(x) || map_get(x)) return true;
    return ir::is<ir::Cond>(x) && is_null_box(x->type);
}

// `e` with the null it may hold kept (not unboxed).
Node* Pass::kept(Node* e) {
    Node* x = strip(e);
    if (null_lit(x)) return expr(e);
    if (null_reads_.contains(x)) {
        const bool already = !read_as_is_.insert(x).second;
        Node* out = expr(e);
        if (!already) read_as_is_.erase(x);
        return out;
    }
    if (map_get(x)) {
        auto& c = *ir::get<ir::Call>(x);
        runtime_.insert("JavaCollections");
        return mk_.static_call("JLib", "GetBox", {expr(c.target), expr(c.args[0])}, null_box(x->type));
    }
    if (auto* c = ir::get<ir::Cond>(x); c != nullptr && is_box(x->type)) {
        c->cond = expr(c->cond);
        c->then = mk_.cast(null_box(x->type), kept(c->then));
        c->orelse = kept(c->orelse);
        x->type = null_box(x->type);
        return x;
    }
    return expr(e);
}

// `e` where a value of type `t` is wanted (an initialiser, an argument, a return, an assignment): a place that holds the null keeps it.
Node* Pass::to(Node* e, const TypeP& t) {
    if (e == nullptr || !t) return expr(e);
    if (is_null_box(t)) return kept(e);
    if (is_box(t) && null_lit(strip(e)))   // (a type argument of a collection stays a value)
        throw Declined{"boxed-null", "a null " + t->simple() + " held in a collection or an array: the translation keeps those values"};
    const std::string* q = qual(t);
    if (q != nullptr && *q == "java.lang.Object" && may_be_null(e)) return mk_.cast(ir::class_type("java.lang.Object"), kept(e));
    return expr(e);
}

// `x += v`, `x++` of a box that may be null: Java unboxes, so a null throws; C#'s lifted operators would leave it null. Null when `e` is
// not one.
Node* Pass::update(Node* e) {
    Node* target = nullptr;
    std::string op;
    Node* value = nullptr;
    bool postfix = false;
    if (const auto* a = ir::get<ir::Assign>(e); a != nullptr && a->op) {
        target = strip(a->target);
        op = *a->op;
        if (op.ends_with('=')) op.pop_back();
        value = a->value;
    } else if (const auto* u = ir::get<ir::Unary>(e); u != nullptr && (u->op == "++" || u->op == "--")) {
        target = strip(u->x);
        op = u->op == "++" ? "+" : "-";
        postfix = u->postfix;
    }
    if (target == nullptr || !null_reads_.contains(target)) return nullptr;
    for (Node* part : ir::walk(target)) {   // (the place is written twice below: what finds it must not run twice)
        const auto* step = ir::get<ir::Unary>(part);
        if (ir::is<ir::Call>(part) || ir::is<ir::Assign>(part) || ir::is<ir::New>(part) || (step != nullptr && (step->op == "++" || step->op == "--")))
            throw Declined{"boxed-null", "changing a box that may be null, at a place found by a call"};
    }
    runtime_.insert("JavaCollections");
    const TypeP plain = plain_box(target->type);
    Node* place = expr(target);   // (a target is written as it is)
    const auto unboxed = [&] { return mk_.static_call("JLib", "Unbox", {ir::clone(arena_, place)}, plain); };
    Node* right = value != nullptr ? expr(value) : mk_.number(1);
    Node* sum = ir::make(arena_, ir::Binary{.op = op, .left = unboxed(), .right = right}, plain);
    Node* next = mk_.assign(place, mk_.cast(plain, sum));
    if (postfix) return mk_.static_call("JLib", "Before", {unboxed(), next}, plain);
    return next;
}

// ---- classes in methods, anonymous classes, inner classes ---------------------------------------------------------------------------
//
// C# has neither classes declared in a method nor anonymous ones, and its nested classes hold no outer instance. Each class declared in a
// method or anonymous is hoisted to a member of the class around it (an anonymous one named Anon1, Anon2 ..., an enum constant's body
// Const_NAME), with a field `cap_x` for each local of the methods around it that it reads, given to its constructor (`cap_x_arg`); an
// inner class that reaches its outer instance gets a field `outer_`, given first. `Outer.this` is then the path of outer_ fields from
// the class that names it, and a class that needs them is made with them.

// Is `q` this class or one it extends (among the program's classes)?
bool Pass::is_a(const ir::ClassDecl* c, const std::string& q) const {
    std::set<std::string> seen;
    for (; c != nullptr && c->qname && seen.insert(*c->qname).second; c = base_of(*c)) {
        if (*c->qname == q) return true;
    }
    return false;
}

// A class as its own members name it: a generic one with its own type variables.
TypeP Pass::own_type(const std::string& qname) const {
    const ir::ClassDecl* c = class_named(qname);
    if (c == nullptr || c->typeparams.empty()) return ir::class_type(qname);
    ir::ClassType own;
    own.name = qname;
    for (const std::string& t : c->typeparams) own.args.push_back(ir::typevar_type(t));
    return ir::make_type(std::move(own));
}

bool is_inner(const ir::ClassDecl& c) { return c.kind == "class" && c.outer && !has(c.mods, "static"); }

// Does anything in `c` reach an instance of a class around it?
bool Pass::reaches_outer(const ir::ClassDecl& c) const {
    const auto around = [&](const std::string& q) {
        if (is_a(&c, q)) return false;
        for (const ir::ClassDecl* k = c.outer ? class_named(*c.outer) : nullptr; k != nullptr; k = k->outer ? class_named(*k->outer) : nullptr) {
            if (is_a(k, q)) return true;
        }
        return false;
    };
    for (Node* m : c.members) {
        if (const auto* nested = ir::get<ir::ClassDecl>(m); nested != nullptr && has(nested->mods, "static")) continue;
        for (Node* n : ir::walk(m)) {
            if (const auto* th = ir::get<ir::This>(n)) {
                const std::string* q = qual(th->qualifier ? th->qualifier : n->type);
                if (q != nullptr && around(*q)) return true;
            }
            if (const auto* call = ir::get<ir::Call>(n);
                call != nullptr && call->target == nullptr && call->sym != nullptr && !call->sym->is_static && call->sym->owner && around(*call->sym->owner))
                return true;
            if (const auto* made = ir::get<ir::New>(n); made != nullptr && made->outer == nullptr && made->body == nullptr) {
                const std::string* q = qual(n->type);
                if (q != nullptr && inner_.contains(*q)) {
                    const ir::ClassDecl* other = class_named(*q);
                    if (other != nullptr && other->outer && around(*other->outer)) return true;
                }
            }
        }
    }
    return false;
}

const Pass::Hoisted* Pass::hoisted(const ir::ClassDecl& c) const {
    if (!c.qname) return nullptr;
    const auto it = hoisted_.find(*c.qname);
    return it != hoisted_.end() && it->second.decl == &c ? &it->second : nullptr;
}

bool Pass::has_captures(const ir::ClassDecl& c) const {
    const Hoisted* h = hoisted(c);
    return h != nullptr && !h->captures.empty();
}

// The locals and parameters these nodes declare.
std::set<std::string> declared_in(const std::vector<Node*>& nodes) {
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

void Pass::prepare_local_classes(std::span<Node* const> units, std::vector<Node*>& classes) {
    const jmodel::Model model(units);
    std::map<std::string, int> local_names;   // (Java may reuse a local class's name in another method; a member's must be unique)
    for (Node* u : units) {
        for (Node* n : ir::walk(u)) {
            if (const auto* lc = ir::get<ir::LocalClass>(n)) ++local_names[ir::get<ir::ClassDecl>(lc->decl)->qname.value_or("")];
        }
    }
    int anonymous = 0;
    for (const auto& entry : model.classes()) {
        const jmodel::ClassInfo& ci = *entry.second;
        if (!(ci.local || ci.anon) || ci.outer == nullptr) continue;
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
        const auto forwarding_ctor = [&](const ir::SymP& base, const std::vector<Node*>& given, const std::vector<TypeP>& types) {
            ir::MethodDecl ctor;
            ctor.sym = ir::make_sym(ir::SymKind::Ctor, name, q);
            ctor.name = name;
            ctor.kind = "ctor";
            ctor.ret = ir::void_type();
            std::vector<Node*> forwarded;
            for (std::size_t i = 0; i < given.size(); ++i) {
                const TypeP type = types[i];
                const ir::SymP param = ir::make_sym(ir::SymKind::Param, "a" + std::to_string(i), std::nullopt, false, type);
                ctor.params.push_back(ir::make(arena_, ir::Param{.sym = param, .type = type}));
                forwarded.push_back(mk_.name(param, type));
                ctor.sym->params.push_back(type);
            }
            std::vector<Node*> body;
            if (!forwarded.empty()) body.push_back(ir::make(arena_, ir::CtorCall{.kind = "super", .sym = base, .args = std::move(forwarded)}));
            ctor.body = mk_.block(std::move(body));
            return ir::make(arena_, std::move(ctor));
        };
        if (const auto* constant = ir::get<ir::EnumConst>(ci.enum_const)) {   // a constant with a body: a class of the enum's, made with the constant's arguments
            name = "Const_" + constant->sym->name;
            q = *around->qname + "." + name;
            copy.kind = "class";
            copy.extends = ir::class_type(*around->qname);
            copy.implements.clear();
            std::erase_if(copy.members, [](Node* m) {
                const auto* md = ir::get<ir::MethodDecl>(m);
                return md != nullptr && md->kind == "ctor";
            });
            std::vector<TypeP> types;
            for (std::size_t i = 0; i < constant->args.size(); ++i) {
                const bool typed = constant->ctor != nullptr && i < constant->ctor->params.size() && static_cast<bool>(constant->ctor->params[i]);
                types.push_back(typed ? constant->ctor->params[i] : constant->args[i]->type);
            }
            copy.members.push_back(forwarding_ctor(constant->ctor, constant->args, types));
        } else if (ci.anon) {
            const auto& creation = *ir::get<ir::New>(ci.creation);
            if (creation.outer != nullptr) {
                refuse("an anonymous class made on an outer instance");
                continue;
            }
            if (creation.sym != nullptr && creation.sym->varargs) {
                refuse("an anonymous class with a varargs base constructor");
                continue;
            }
            if (std::any_of(copy.members.begin(), copy.members.end(), [](Node* m) { return ir::is<ir::ClassDecl>(m); })) {
                refuse("an anonymous class with a class inside it");
                continue;
            }
            do {
                name = "Anon" + std::to_string(++anonymous);
                q = *around->qname + "." + name;
            } while (class_named(q) != nullptr);
            const bool is_interface = !ci.ifaces.empty();
            const std::string target = is_interface ? ci.ifaces[0] : ci.base.value_or("java.lang.Object");
            const TypeP written = copy.extends ? copy.extends : ci.creation->type;
            copy.extends = TypeP{};
            copy.implements.clear();
            if (model.info(target) != nullptr) {
                if (is_interface) copy.implements.push_back(written);
                else copy.extends = written;
            } else if (function_table().contains(target) || target == "java.lang.Runnable") {
                const jmodel::Signature* single = jmodel::library_functional(target);
                if (single == nullptr) {
                    refuse("an anonymous " + target);
                    continue;
                }
                made.delegate_method = single->name;
                const std::string* typed = qual(ci.creation->type);
                made.delegate_type = typed != nullptr && *typed == target ? ci.creation->type : written;
            } else if (target == "java.lang.Comparable" || target == "java.lang.AutoCloseable" || target == "java.io.Closeable" || target == "java.util.Iterator" ||
                       target == "java.lang.Iterable") {
                copy.implements.push_back(written);
            } else if (const auto mapped = class_table().find(target); mapped != class_table().end() && mapped->second.ends_with("Exception")) {
                copy.extends = written;
            } else if (!is_interface && generic_table().contains(target)) {   // `new ArrayList<>() {{ add(1); }}`: a collection filled as it is made
                const bool overrides = std::any_of(copy.members.begin(), copy.members.end(), [](Node* m) {
                    const auto* md = ir::get<ir::MethodDecl>(m);
                    return md != nullptr && md->kind == "method" && !has(md->mods, "IMPLICIT");
                });
                if (overrides) {
                    refuse("an anonymous class overriding methods of the library type " + target);
                    continue;
                }
                copy.extends = written;
                for (Node* member : copy.members) {   // what the block calls by name alone is the collection's own
                    for (Node* n : own_nodes(member)) {
                        auto* call = ir::get<ir::Call>(n);
                        if (call == nullptr || call->target != nullptr || call->sym == nullptr || call->sym->is_static) continue;
                        if (class_named(call->sym->owner.value_or("")) == nullptr) call->target = ir::make(arena_, ir::This{}, written);
                    }
                }
            } else if (target != "java.lang.Object") {
                refuse("an anonymous class of the library type " + target);
                continue;
            }
            std::erase_if(copy.members, [](Node* m) {
                const auto* md = ir::get<ir::MethodDecl>(m);
                return md != nullptr && md->kind == "ctor";
            });
            // its one constructor hands its arguments to the base's
            std::vector<TypeP> types;
            for (std::size_t i = 0; i < creation.args.size(); ++i) {
                const bool typed = creation.sym != nullptr && i < creation.sym->params.size() && creation.sym->params[i] && creation.sym->params[i]->kind() != "typevar";
                types.push_back(typed ? creation.sym->params[i] : creation.args[i]->type);
            }
            copy.members.push_back(forwarding_ctor(creation.sym, creation.args, types));
        } else {
            q = ci.decl->qname.value_or("");
            name = ci.decl->name.value_or("");
            if (local_names[q] > 1 || class_named(q) != nullptr) {
                refuse("two classes named " + name + " where C# needs one");
                continue;
            }
        }
        copy.name = name;
        copy.qname = q;
        copy.outer = *around->qname;
        if (!ci.needs_outer && copy.kind == "class" && !has(copy.mods, "static")) copy.mods.push_back("static");   // (declared where there is no `this`)
        for (Node* member : copy.members) {   // javac leaves the owner of what such a class declares empty
            for (Node* n : own_nodes(member)) {
                ir::visit(*n, [&](auto& k) {
                    if constexpr (requires { k.sym; }) {
                        ir::Sym* s = k.sym.get();
                        if (s != nullptr && s->owner && s->owner->empty() && (s->kind == ir::SymKind::Field || s->kind == ir::SymKind::Method || s->kind == ir::SymKind::Ctor))
                            s->owner = q;
                    }
                });
                if (ir::is<ir::This>(n)) {
                    const std::string* t = qual(n->type);
                    if (t != nullptr && t->empty()) n->type = ir::class_type(q);
                }
            }
        }
        Node* node = ir::make(arena_, std::move(copy), TypeP{}, ci.node->line, ci.node->col);
        made.decl = ir::get<ir::ClassDecl>(node);
        around->members.push_back(node);
        index(node);
        hoist_of_[ci.node] = made.decl;
        hoisted_[q] = std::move(made);
        const std::function<void(Node*)> collect = [&](Node* n) {
            if (auto* cd = ir::get<ir::ClassDecl>(n)) {
                classes.push_back(n);
                for (Node* m : cd->members) collect(m);
            }
        };
        collect(node);
    }
    find_local_captures();
    add_capture_fields();
}

// What each class reads from the methods around it: the names its members do not declare, then whatever a class it makes needs from there.
void Pass::find_local_captures() {
    const auto add = [](Hoisted& h, const std::string& name, const TypeP& type) {
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
            Part part{&h, own_nodes(member), {}};
            part.declared = declared_in(part.nodes);
            for (Node* n : part.nodes) {
                const auto* read = ir::get<ir::Name>(n);
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
                const auto* made = ir::get<ir::New>(n);
                if (made == nullptr) continue;
                const Hoisted* target = nullptr;
                if (made->body != nullptr) {
                    if (const auto found = hoist_of_.find(made->body); found != hoist_of_.end()) target = hoisted(*found->second);
                } else if (const std::string* q = qual(n->type)) {
                    if (const auto found = hoisted_.find(*q); found != hoisted_.end()) target = &found->second;
                }
                if (target == nullptr) continue;
                const std::vector<std::pair<std::string, TypeP>> wanted = target->captures;   // (it may be this class)
                for (const auto& [name, type] : wanted) {
                    if (part.declared.contains(name)) continue;
                    via_field_[n].insert(name);
                    if (add(*part.owner, name, type)) changed = true;
                }
            }
        }
    }
}

void Pass::add_capture_fields() {
    for (auto& [q, h] : hoisted_) {
        if (h.captures.empty()) continue;
        std::sort(h.captures.begin(), h.captures.end(), [](const auto& a, const auto& b) { return a.first < b.first; });   // (the map's order is no order)
        for (const auto& [name, type] : h.captures) {
            if (!type) {
                refused_decl_[h.decl] = "a captured local with no type (" + name + ")";
                continue;
            }
            const ir::SymP field = ir::make_sym(ir::SymKind::Field, "cap_" + name, q, false, type);
            h.fields.push_back(field);
            h.decl->members.push_back(ir::make(arena_, ir::FieldDecl{.sym = field, .type = type, .mods = {}}));
        }
        for (Node* mn : h.decl->members) {
            auto* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr || m->kind != "ctor") continue;
            if (m->sym && m->sym->varargs) {
                refused_decl_[h.decl] = "a local class with a varargs constructor (" + h.decl->name.value_or("") + ")";
                continue;
            }
            std::vector<Node*> params, passed;
            std::vector<TypeP> types;
            for (const auto& [name, type] : h.captures) {
                const ir::SymP given = ir::make_sym(ir::SymKind::Param, "cap_" + name + "_arg", std::nullopt, false, type);
                params.push_back(ir::make(arena_, ir::Param{.sym = given, .type = type}));
                passed.push_back(mk_.name(given, type));
                types.push_back(type);
            }
            m->params.insert(m->params.begin(), params.begin(), params.end());
            if (m->sym) {
                auto shifted = std::make_shared<ir::Sym>(*m->sym);
                shifted->params.insert(shifted->params.begin(), types.begin(), types.end());
                m->sym = std::move(shifted);
            }
            for (Node* st : body_statements(m->body)) {
                auto* chained = ir::get<ir::CtorCall>(st);
                if (chained == nullptr) continue;
                for (Node* arg : chained->args) {   // (C# has no `this` in a constructor's initialiser: these read its parameters)
                    for (Node* n : own_nodes(arg)) {
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

void Pass::prepare_inner_classes(const std::vector<Node*>& classes) {
    // which inner classes need their instance: one that makes another that does needs its own, so until nothing changes
    for (bool changed = true; changed;) {
        changed = false;
        for (Node* cn : classes) {
            const ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
            if (!is_inner(c) || !c.qname || inner_.contains(*c.qname) || !reaches_outer(c)) continue;
            inner_[*c.qname] = ir::make_sym(ir::SymKind::Field, "outer_", *c.qname, false, own_type(*c.outer));
            changed = true;
        }
    }
    for (Node* cn : classes) {   // `outer.new Inner()` hands it one whether it reads it or not
        for (Node* n : ir::walk(cn)) {
            const auto* made = ir::get<ir::New>(n);
            const std::string* q = made != nullptr && made->outer != nullptr ? qual(n->type) : nullptr;
            const ir::ClassDecl* c = q != nullptr ? class_named(*q) : nullptr;
            if (c != nullptr && c->outer && !inner_.contains(*q)) inner_[*q] = ir::make_sym(ir::SymKind::Field, "outer_", *q, false, own_type(*c->outer));
        }
    }
    for (Node* cn : classes) {
        ir::ClassDecl& c = *ir::get<ir::ClassDecl>(cn);
        if (!needs_outer(c)) continue;
        const ir::SymP& field = inner_.at(*c.qname);
        c.members.push_back(ir::make(arena_, ir::FieldDecl{.sym = field, .type = field->type, .mods = {}}));
        for (Node* mn : c.members) {
            auto* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr || m->kind != "ctor") continue;
            if (m->sym && m->sym->varargs) {
                refused_decl_[&c] = "an inner class with a varargs constructor (" + c.name.value_or("") + ")";
                continue;
            }
            const ir::SymP given = ir::make_sym(ir::SymKind::Param, "outer_arg", std::nullopt, false, field->type);
            m->params.insert(m->params.begin(), ir::make(arena_, ir::Param{.sym = given, .type = field->type}));
            if (m->sym) {
                auto shifted = std::make_shared<ir::Sym>(*m->sym);
                shifted->params.insert(shifted->params.begin(), field->type);
                m->sym = std::move(shifted);
            }
            for (Node* st : body_statements(m->body)) {
                auto* chained = ir::get<ir::CtorCall>(st);
                if (chained == nullptr || chained->kind != "this") continue;
                chained->args.insert(chained->args.begin(), mk_.name(given, field->type));
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

// The path of outer_ fields from the class being rewritten to its instance of `qname`: null when it is not one it holds.
Node* Pass::outer_path(const std::string& qname) {
    const ir::ClassDecl* k = cls_;
    Node* path = nullptr;
    while (k != nullptr && !is_a(k, qname)) {
        if (!needs_outer(*k)) return nullptr;   // (not an instance it holds: as written)
        const ir::SymP& field = inner_.at(*k->qname);
        Node* from = path != nullptr ? path : ir::make(arena_, ir::This{}, ir::class_type(*k->qname));
        path = ir::make(arena_, ir::Select{.obj = from, .sym = field}, field->type);
        k = k->outer ? class_named(*k->outer) : nullptr;
    }
    return k != nullptr ? path : nullptr;
}

// `this`, `Outer.this`: the path to the instance it names.
Node* Pass::this_node(Node* e) {
    auto& th = *ir::get<ir::This>(e);
    const std::string* q = qual(th.qualifier ? th.qualifier : e->type);
    if (q == nullptr) return e;
    if (Node* path = outer_path(*q)) return path;
    if (th.qualifier && cls_ != nullptr && is_a(cls_, *q)) {   // (`Self.this`)
        th.qualifier = TypeP{};
        e->type = ir::class_type(cls_->qname.value_or(""));
    }
    return e;
}

// A class that needs its outer instance or the locals it read, made: those first, then its own arguments.
Node* Pass::new_inner(Node* e, const ir::ClassDecl& c) {
    auto& made = *ir::get<ir::New>(e);
    std::vector<Node*> args;
    std::vector<TypeP> types;
    if (needs_outer(c)) {
        Node* outer = nullptr;
        const TypeP type = inner_.at(*c.qname)->type;
        if (made.outer != nullptr) {
            outer = jlib("NonNull", {expr(made.outer)}, type);   // (Java throws on `null.new Inner()`)
        } else {
            outer = outer_path(*c.outer);
            if (outer == nullptr) outer = ir::make(arena_, ir::This{}, cls_ != nullptr ? ir::class_type(cls_->qname.value_or("")) : type);
        }
        args.push_back(outer);
        types.push_back(type);
    }
    const Hoisted* local = hoisted(c);
    if (local != nullptr) {
        const auto from_field = via_field_.find(e);
        for (const auto& [name, type] : local->captures) {
            types.push_back(type);
            if (from_field != via_field_.end() && from_field->second.contains(name)) {
                if (before_base_.contains(e)) {
                    args.push_back(mk_.name(ir::make_sym(ir::SymKind::Param, "cap_" + name + "_arg", std::nullopt, false, type), type));
                } else {
                    const std::string owner = cls_ != nullptr ? cls_->qname.value_or("") : std::string();
                    args.push_back(mk_.field(ir::make(arena_, ir::This{}, ir::class_type(owner)), "cap_" + name, type));
                }
                continue;
            }
            args.push_back(expr(mk_.name(ir::make_sym(ir::SymKind::Local, name, std::nullopt, false, type), type)));
        }
    }
    for (std::size_t i = 0; i < made.args.size(); ++i) {
        const bool typed = made.sym != nullptr && i < made.sym->params.size() && made.sym->params[i] && made.sym->params[i]->kind() != "typevar";
        args.push_back(typed ? to(made.args[i], made.sym->params[i]) : expr(made.args[i]));
        types.push_back(typed ? made.sym->params[i] : made.args[i]->type);
    }
    auto sym = ir::make_sym(ir::SymKind::Ctor, c.name.value_or(""), c.qname);
    sym->params = std::move(types);
    const TypeP type = made.body != nullptr ? ir::class_type(c.qname.value_or("")) : e->type;   // (made inside the class around it, whose T it shares)
    Node* object = ir::make(arena_, ir::New{.sym = sym, .args = std::move(args)}, type);
    if (local != nullptr && !local->delegate_method.empty()) {   // a library functional interface is a delegate here: to this object's one method
        const auto method = ir::make_sym(ir::SymKind::Method, local->delegate_method, c.qname);
        Node* group = ir::make(arena_, ir::MethodRef{.target = object, .sym = method}, local->delegate_type);
        return ir::make(arena_, ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "delegate"), .args = {group}}, local->delegate_type);
    }
    return object;
}

// Can constructing this class (and the program classes above it) run nothing of a subclass? True when no constructor, instance
// initialiser or field initialiser calls an instance method or lets `this` go anywhere but a field access.
bool Pass::base_construction_is_closed(const ir::ClassDecl* base) const {
    std::set<std::string> seen;
    for (; base != nullptr && base->qname && seen.insert(*base->qname).second; base = base_of(*base)) {
        for (Node* m : base->members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            const auto* f = ir::get<ir::FieldDecl>(m);
            const bool runs = (md != nullptr && (md->kind == "ctor" || md->kind == "init")) || (f != nullptr && !has(f->mods, "static") && f->init != nullptr);
            if (!runs) continue;
            std::size_t thises = 0, field_reads = 0;
            for (Node* n : own_nodes(m)) {
                if (ir::is<ir::This>(n)) ++thises;
                if (const auto* sel = ir::get<ir::Select>(n); sel != nullptr && ir::is<ir::This>(sel->obj)) ++field_reads;
                if (const auto* call = ir::get<ir::Call>(n); call != nullptr && call->target == nullptr && (call->sym == nullptr || !call->sym->is_static)) return false;
            }
            if (thises > field_reads) return false;
        }
    }
    return true;
}

// ---- enums of objects ----------------------------------------------------------------------------------------------------------------
//
// A Java enum with fields, methods, constructor arguments or constants with bodies is a class of objects; C#'s enum is a named number. It is
// written as a class of the runtime's JEnum (its JName and JOrdinal), each constant a static field made in order by JLib.EnumMade, a
// constant with a body an object of its hoisted class Const_NAME; values() is the array of them, valueOf JLib.EnumOf, and a switch on
// one switches on its ordinal.

void Pass::enum_members(ir::ClassDecl& c) {
    const std::string qname = c.qname.value_or("");
    runtime_.insert("JavaRt");
    runtime_.insert("JavaCollections");
    const TypeP self = ir::class_type(qname);
    std::vector<Node*> kept;
    std::int64_t ordinal = 0;
    for (Node* mn : c.members) {
        const auto* ec = ir::get<ir::EnumConst>(mn);
        if (ec == nullptr) continue;
        TypeP made_type = self;
        if (ec->body != nullptr) {
            if (const auto own = hoist_of_.find(ec->body); own != hoist_of_.end()) made_type = ir::class_type(own->second->qname.value_or(""));
            else throw Declined{"enum", refused_.contains(ec->body) ? refused_.at(ec->body) : std::string("an enum constant with a body")};
        }
        auto sym = ec->ctor != nullptr ? std::make_shared<ir::Sym>(*ec->ctor) : ir::make_sym(ir::SymKind::Ctor, c.name.value_or(""), qname);
        if (ec->body != nullptr) {
            sym->name = hoist_of_.at(ec->body)->name.value_or(sym->name);
            sym->owner = qual(made_type) != nullptr ? std::optional<std::string>(*qual(made_type)) : std::nullopt;
        }
        Node* made = ir::make(arena_, ir::New{.sym = sym, .args = ec->args}, made_type);
        Node* named = jlib("EnumMade", {made, mk_.text(ec->sym->name), mk_.number(ordinal++)}, self);
        ir::FieldDecl field;
        field.sym = ir::make_sym(ir::SymKind::Field, ec->sym->name, qname, true, self);
        field.type = self;
        field.init = named;
        field.mods = {"public", "static", "final"};
        kept.push_back(ir::make(arena_, std::move(field)));
    }
    for (Node* mn : c.members) {
        const auto* md = ir::get<ir::MethodDecl>(mn);
        if (ir::is<ir::EnumConst>(mn) || (md != nullptr && md->kind != "ctor" && has(md->mods, "IMPLICIT"))) continue;
        kept.push_back(mn);
    }
    c.members = std::move(kept);
    c.kind = "class";
    c.extends = ir::class_type("JEnum");
}

// A call of an enum of objects' own or inherited methods (values, valueOf, name, ordinal ...); null for any other call.
Node* Pass::enum_call(Node* e) {
    auto& call = *ir::get<ir::Call>(e);
    const ir::Sym& s = *call.sym;
    const std::string owner = s.owner.value_or("");
    const ir::ClassDecl* c = class_named(owner);
    const ir::ClassDecl* of = c != nullptr && c->qname && object_enum(*c->qname) != nullptr ? c : nullptr;
    if (of == nullptr && call.target != nullptr) {
        const std::string* tq = qual(call.target->type);
        const ir::ClassDecl* tc = tq != nullptr ? class_named(*tq) : nullptr;
        if (tc != nullptr && object_enum(*tq) != nullptr) of = tc;
    }
    if (of == nullptr && call.target == nullptr && owner == "java.lang.Enum" && cls_ != nullptr && cls_->qname && object_enum(*cls_->qname) != nullptr)
        of = cls_;   // (`name()` inside the enum)
    if (of == nullptr) return nullptr;
    const std::string q = *of->qname;
    const TypeP self = ir::class_type(q);
    const auto receiver = [&]() -> Node* {
        if (call.target != nullptr && !ir::is<ir::TypeName>(call.target)) return expr(call.target);
        return ir::make(arena_, ir::This{}, self);
    };
    if (s.is_static && (s.name == "values" || s.name == "valueOf")) {
        std::vector<Node*> all;
        for (const std::string& n : *object_enum(q)) all.push_back(mk_.field(mk_.type_name(self), n, self, true));
        Node* array = ir::make(arena_, ir::NewArray{.elem = self, .init = std::move(all)}, ir::array_of(self));
        if (s.name == "values") return array;
        return jlib("EnumOf", {array, expr(call.args.at(0))}, self);
    }
    const bool inherited = owner == "java.lang.Enum" || c == nullptr;
    if (!inherited) return nullptr;
    if (s.name == "name" && call.args.empty()) return mk_.field(receiver(), "JName", ir::class_type("java.lang.String"));
    if (s.name == "ordinal" && call.args.empty()) return mk_.field(receiver(), "JOrdinal", ir::i32_type());
    if (s.name == "compareTo" && call.args.size() == 1) return mk_.call(receiver(), "CompareTo", {expr(call.args[0])}, ir::i32_type());
    if (s.name == "toString" && call.args.empty()) return mk_.call(receiver(), "ToString", {}, ir::class_type("java.lang.String"));
    if (s.name == "equals" && call.args.size() == 1) return mk_.call(receiver(), "Equals", {expr(call.args[0])}, ir::bool_type());
    return nullptr;
}

// A switch on an enum of objects: on its ordinal, each constant's label its number (C# cases need constants). The scrutinee as written.
Node* Pass::enum_scrutinee(Node* scrutinee, std::vector<Node*>& cases) {
    const std::string* q = qual(scrutinee->type);
    const std::vector<std::string>* names = q != nullptr ? object_enum(*q) : nullptr;
    if (names == nullptr) return expr(scrutinee);
    for (Node* cn : cases) {
        for (Node*& l : ir::get<ir::Case>(cn)->labels) {
            const auto* sel = ir::get<ir::Select>(strip(l));
            const auto* name = ir::get<ir::Name>(strip(l));
            const std::string label = sel != nullptr && sel->sym ? sel->sym->name : name != nullptr && name->sym ? name->sym->name : std::string();
            const auto at = std::find(names->begin(), names->end(), label);
            if (at == names->end()) throw Declined{"enum", "a case label that is not a constant of " + *q};
            l = mk_.number(at - names->begin());
        }
    }
    return mk_.field(expr(scrutinee), "JOrdinal", ir::i32_type());
}

// ---- lambdas of the program's interfaces --------------------------------------------------------------------------------------------
//
// C#'s lambda converts to a delegate, never to an interface. An interface of the program that a lambda (or a method reference) is made for,
// with one method to implement, gets a class JFn that implements it over a delegate, and the lambda is made as one: `new Op<int>.JFn(x => x
// + 1)`.

// The one method an interface leaves to implement, with what it inherits; null when there are several, or one a generic parent would retype.
const ir::MethodDecl* Pass::single_abstract(const ir::ClassDecl& start) const {
    std::vector<const ir::MethodDecl*> found;
    std::set<std::string> seen;
    bool must_close = false;
    const std::function<bool(const ir::ClassDecl&, bool)> visit = [&](const ir::ClassDecl& k, bool top) {
        if (!k.qname || !seen.insert(*k.qname).second) return true;
        if (!k.typeparams.empty() && !top) return false;
        for (Node* mn : k.members) {
            const auto* m = ir::get<ir::MethodDecl>(mn);
            if (m == nullptr || has(m->mods, "static") || has(m->mods, "default") || has(m->mods, "private") || ir::present(m->body)) continue;
            const std::string name = m->name.value_or("");
            if (object_method(name, m->params.size())) continue;
            const bool again = std::any_of(found.begin(), found.end(), [&](const ir::MethodDecl* x) { return x->name == m->name && x->params.size() == m->params.size(); });
            if (!again) found.push_back(m);
        }
        std::vector<TypeP> parents = k.implements;
        if (k.extends) parents.push_back(k.extends);
        for (const TypeP& parent : parents) {
            const std::string* q = qual(parent);
            if (q == nullptr) return false;
            if (*q == "java.lang.Object") continue;
            if (*q == "java.lang.AutoCloseable" || *q == "java.io.Closeable") {
                must_close = true;
            } else if (const ir::ClassDecl* up = class_named(*q)) {
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

void Pass::prepare_lambda_classes() {
    for (Node* u : java_) {   // the interfaces of the program that lambdas are made for
        for (Node* n : ir::walk(u)) {
            if (!ir::is<ir::Lambda>(n) && !ir::is<ir::MethodRef>(n)) continue;
            const std::string* q = qual(n->type);
            const ir::ClassDecl* target = q != nullptr ? class_named(*q) : nullptr;
            if (target == nullptr || target->kind != "interface" || lambda_interfaces_.contains(*q)) continue;
            if (const ir::MethodDecl* method = single_abstract(*target)) lambda_interfaces_.emplace(*q, method);
        }
    }
    for (const auto& [q, m] : lambda_interfaces_) {
        ir::ClassDecl& face = *class_named(q);
        const std::string fq = q + ".JFn";
        // the delegate it holds: Func<params..., result>, or Action<params...>
        std::vector<TypeP> types;
        for (Node* p : m->params) types.push_back(ir::get<ir::Param>(p)->type);
        const bool returns = m->ret && m->ret->kind() != "void";
        if (returns) types.push_back(m->ret);
        const TypeP fn = ir::class_type(returns ? "System.Func" : "System.Action", std::move(types));
        const ir::SymP field = ir::make_sym(ir::SymKind::Field, "body", fq, false, fn);
        const auto self = [&] { return ir::make(arena_, ir::This{}, ir::class_type(fq)); };
        const auto held = [&] { return ir::make(arena_, ir::Select{.obj = self(), .sym = field}, fn); };
        ir::MethodDecl ctor;
        ctor.sym = ir::make_sym(ir::SymKind::Ctor, "JFn", fq);
        ctor.sym->params = {fn};
        ctor.name = "JFn";
        ctor.kind = "ctor";
        ctor.ret = ir::void_type();
        ctor.mods = {"public"};
        const ir::SymP given = ir::make_sym(ir::SymKind::Param, "body", std::nullopt, false, fn);
        ctor.params = {ir::make(arena_, ir::Param{.sym = given, .type = fn})};
        ctor.body = mk_.block({mk_.expr_stmt(mk_.assign(held(), mk_.name(given, fn)))});
        ir::MethodDecl method;
        method.sym = std::make_shared<ir::Sym>(*m->sym);
        method.sym->owner = fq;
        method.name = m->name;
        method.kind = "method";
        method.ret = m->ret;
        method.mods = {"public"};
        std::vector<Node*> passed;
        for (std::size_t i = 0; i < m->params.size(); ++i) {
            const TypeP type = ir::get<ir::Param>(m->params[i])->type;
            const ir::SymP a = ir::make_sym(ir::SymKind::Param, "a" + std::to_string(i), std::nullopt, false, type);
            method.params.push_back(ir::make(arena_, ir::Param{.sym = a, .type = type}));
            passed.push_back(mk_.name(a, type));
        }
        auto invoke = ir::make_sym(ir::SymKind::Method, "Invoke");
        invoke->flags.push_back("delegate");
        Node* run = ir::make(arena_, ir::Call{.target = held(), .sym = invoke, .args = std::move(passed)}, m->ret);
        method.body = mk_.block({returns ? ir::make(arena_, ir::Return{.expr = run}) : mk_.expr_stmt(run)});
        ir::ClassDecl jfn;
        jfn.kind = "class";
        jfn.name = "JFn";
        jfn.qname = fq;
        jfn.outer = q;
        jfn.mods = {"public", "final"};
        jfn.implements = {own_type(q)};
        jfn.members = {ir::make(arena_, ir::FieldDecl{.sym = field, .type = fn, .mods = {"private", "final"}}), ir::make(arena_, std::move(ctor)),
                       ir::make(arena_, std::move(method))};
        Node* node = ir::make(arena_, std::move(jfn));
        face.members.push_back(node);
        index(node);
    }
}

// A lambda or a method group made for an interface of the program: an object of its JFn over it.
Node* Pass::as_interface(Node* fn, const TypeP& face) {
    const std::string* q = qual(face);
    if (q == nullptr || !lambda_interfaces_.contains(*q)) return fn;
    const auto* c = std::get_if<ir::ClassType>(&face->body);
    const TypeP made = ir::class_type(*q + ".JFn", c != nullptr ? c->args : std::vector<TypeP>{});
    return ir::make(arena_, ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "JFn", *q + ".JFn"), .args = {fn}}, made);
}

// ---- expressions --------------------------------------------------------------------------------------------------------------

Node* Pass::jlib(const std::string& name, std::vector<Node*> args, const TypeP& type) {
    runtime_.insert("JavaRt");
    runtime_.insert("JavaCollections");
    return mk_.static_call("JLib", name, std::move(args), type);
}

void Pass::record(const ir::SymP& sym, Node* init) {
    Node* x = init != nullptr ? strip(init) : nullptr;
    const std::string* q = x != nullptr && ir::is<ir::New>(x) ? qual(x->type) : nullptr;
    if (sym != nullptr && q != nullptr) concrete_[{sym->owner.value_or(""), sym->name}] = *q;
    if (sym != nullptr && x != nullptr) {
        const std::string_view limit = list_limit(x);
        if (limit == "fixed") concrete_[{sym->owner.value_or(""), sym->name}] = "java.util.Arrays.asList";
        if (limit == "immutable") concrete_[{sym->owner.value_or(""), sym->name}] = "java.util.stream.Stream.toList";
    }
}

std::optional<std::string> Pass::concrete(Node* e) const {
    Node* x = strip(e);
    const ir::SymP* sym = nullptr;
    if (const auto* n = ir::get<ir::Name>(x)) sym = &n->sym;
    else if (const auto* s = ir::get<ir::Select>(x)) sym = &s->sym;
    if (sym != nullptr && *sym != nullptr) {
        const auto it = concrete_.find({(*sym)->owner.value_or(""), (*sym)->name});
        if (it != concrete_.end()) return it->second;
    }
    if (const std::string* q = qual(x->type)) return *q;
    return std::nullopt;
}

std::string_view Pass::list_limit(Node* e) const {
    if (e == nullptr) return {};
    Node* x = strip(e);
    if (const auto* made = ir::get<ir::Call>(x); made != nullptr && made->sym != nullptr) {
        if (made->sym->owner == "java.util.Arrays" && made->sym->name == "asList") return "fixed";
        if (made->sym->owner == "java.util.List" && made->sym->name == "of") return "immutable";
    }
    const std::optional<std::string> held = concrete(x);
    if (held == "java.util.Arrays.asList") return "fixed";
    if (held == "java.util.stream.Stream.toList") return "immutable";
    return {};
}

void Pass::check_list_change(const ir::Call& call) const {
    const ir::Sym& s = *call.sym;
    const bool utility = s.owner == "java.util.Collections" && (s.name == "sort" || s.name == "reverse" || s.name == "shuffle");
    Node* list = utility ? (call.args.empty() ? nullptr : call.args[0]) : call.target;
    const std::string_view limit = list_limit(list);
    if (limit.empty()) return;
    const bool resizes = !utility && (s.name == "add" || s.name == "addAll" || s.name == "remove" || s.name == "removeAll" || s.name == "removeIf" ||
                                      s.name == "retainAll" || s.name == "clear");
    const bool writes = utility || s.name == "set" || s.name == "sort" || s.name == "replaceAll";
    if (limit == "fixed" && writes && list_over_array_)
        throw Declined{"fixed-list", "changing a list made over an array (" + s.name + " on Arrays.asList): Java writes through to the array, a C# copy would not"};
    if (resizes || (limit == "immutable" && writes))
        throw Declined{"fixed-list", "changing a list Java refuses to change (" + s.name + " on " + (limit == "fixed" ? "Arrays.asList" : "an immutable list") +
                                         "): Java throws, a C# List would not"};
}

Pass::Row Pass::lookup(const ir::Call& call) const {
    const ir::Sym& s = *call.sym;
    const auto fam = family_table().find(s.owner.value_or(""));
    if (fam == family_table().end()) return {};
    std::vector<std::string> keys;
    if (!s.params.empty()) keys.push_back(fam->second + "." + s.name + "(" + param_key(s.params[0]) + ")");
    keys.push_back(fam->second + "." + s.name + "/" + std::to_string(call.args.size()));
    keys.push_back(fam->second + "." + s.name);
    for (const std::string& k : keys) {
        std::optional<apimap::Mapping> m = registry_.resolve(k, "java", "csharp");
        if (!m || m->name != k) continue;
        const apimap::SymbolRecord* rec = registry_.record("java", k);
        const apimap::TargetMap* raw = rec != nullptr ? rec->maps.find("csharp") : nullptr;
        return {std::move(m), raw};
    }
    return {};
}

// A template filled with the call's target and arguments: `{t}` the target (`this` for none), `{out}` Console or Console.Error, `{*}`
// every argument, `{sN}` an argument as Java turns it into text, `{N}` / `{Np}` an argument.
Node* Pass::fill(const std::string& tmpl, Node* call_node, const ir::Call& call) {
    const auto all_digits = [](std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char ch) { return ch >= '0' && ch <= '9'; }); };
    const HoleFn hole = [&](const std::string& key) -> std::vector<Node*> {
        if (key == "t") return {call.target != nullptr ? expr(call.target) : ir::make(arena_, ir::This{}, cls_ != nullptr ? ir::class_type(cls_->qname.value_or("")) : TypeP{})};
        if (key == "out") {
            const auto* sel = ir::get<ir::Select>(call.target);
            const bool err = sel != nullptr && sel->sym != nullptr && sel->sym->name == "err";
            return {err ? mk_.field(mk_.type_name("System.Console"), "Error", ir::class_type("System.IO.TextWriter"), true) : mk_.type_name("System.Console")};
        }
        if (key == "*") {
            std::vector<Node*> out;
            for (Node* a : call.args) out.push_back(expr(a));
            return out;
        }
        if (key.size() > 1 && key[0] == 's' && all_digits(key.substr(1))) return {string_of(call.args.at(std::stoul(key.substr(1))))};
        if (key.size() > 1 && key.back() == 'p' && all_digits(key.substr(0, key.size() - 1))) return {expr(call.args.at(std::stoul(key.substr(0, key.size() - 1))))};
        if (all_digits(key)) return {expr(call.args.at(std::stoul(key)))};
        throw Declined{"library", "the template hole {" + key + "}"};
    };
    TemplateReader reader(mk_, tmpl, hole, call_node->type);
    Node* out = reader.read();
    if (tmpl.find("JavaRt") != std::string::npos) runtime_.insert("JavaRt");
    return out;
}

// `a` as Java turns it into text (String.valueOf).
Node* Pass::string_of(Node* a) {
    const TypeP t = a->type;
    Node* e = strip(a);
    if (may_be_null(e)) {   // Java writes a null box as "null"
        runtime_.insert("JavaRt");
        return mk_.static_call("JavaRt", "Str", {mk_.cast(ir::class_type("java.lang.Object"), kept(a))}, ir::class_type("System.String"));
    }
    if (string_lit(e)) return expr(a);
    if (integral(t) || ir::is<ir::Concat>(e)) return expr(a);
    if (const auto* arr = t ? std::get_if<ir::ArrayType>(&t->body) : nullptr; arr != nullptr && arr->elem && arr->elem->kind() == "u16") {
        return ir::make(arena_, ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "string"), .args = {expr(a)}}, ir::class_type("System.String"));
    }
    if (is_java_string(t)) {   // a string that is never null: a literal, a concatenation, a library call that gives text
        if (const auto* c = ir::get<ir::Call>(e); c != nullptr && c->sym != nullptr && c->sym->owner) {
            static const std::unordered_set<std::string_view> owners = {"java.lang.String", "java.lang.StringBuilder", "java.lang.Integer", "java.lang.Long",
                                                                         "java.lang.Double", "java.lang.Float"};
            static const std::unordered_set<std::string_view> names = {"toString", "substring", "trim", "toUpperCase", "toLowerCase", "replace",
                                                                        "valueOf", "concat", "repeat", "join", "strip", "intern"};
            if (owners.contains(*c->sym->owner) && names.contains(c->sym->name)) return expr(a);
        }
    }
    runtime_.insert("JavaRt");
    return mk_.static_call("JavaRt", "Str", {expr(a)}, ir::class_type("System.String"));
}

Node* Pass::concat(Node* e) {
    auto& x = *ir::get<ir::Concat>(e);
    for (Node*& part : x.parts) {
        const TypeP t = part->type;
        Node* p = strip(part);
        const bool as_is = !may_be_null(p) && (string_lit(p) || integral(t));
        part = as_is ? expr(part) : string_of(part);
    }
    return e;
}

Node* Pass::binary(Node* e) {
    auto& b = *ir::get<ir::Binary>(e);
    if (b.op == "==" || b.op == "!=") {
        Node* l = strip(b.left);
        Node* r = strip(b.right);
        if ((null_lit(r) && may_be_null(l)) || (null_lit(l) && may_be_null(r))) {
            b.left = kept(b.left);
            b.right = kept(b.right);
            return e;
        }
        if (is_box(l->type) && is_box(r->type)) {   // two boxes: Java compares the references
            runtime_.insert("JavaCollections");
            const TypeP plain = ir::prim_type(*box_prim(*qual(l->type)));
            auto* same = ir::get<ir::Call>(jlib("SameBox", {kept(b.left), kept(b.right)}, ir::bool_type()));
            same->typeargs.push_back(plain);
            Node* made = ir::make(arena_, std::move(*same), ir::bool_type());
            if (b.op == "!=") return ir::make(arena_, ir::Unary{.op = "!", .x = made}, ir::bool_type());
            return made;
        }
        if (is_java_string(b.left->type) && is_java_string(b.right->type) && !null_lit(l) && !null_lit(r)) {   // Java compares the references
            const TypeP object = ir::class_type("System.Object");
            b.left = mk_.cast(object, expr(b.left));
            b.right = mk_.cast(object, expr(b.right));
            return e;
        }
    }
    b.left = expr(b.left);
    b.right = expr(b.right);
    return e;
}

Node* Pass::select(Node* e) {
    auto& sel = *ir::get<ir::Select>(e);
    const ir::SymP& s = sel.sym;
    if (s == nullptr || s->kind != ir::SymKind::Field) {
        sel.obj = expr(sel.obj);
        return e;
    }
    const std::string owner = s->owner.value_or("");
    const auto keyword_member = [&](const TypeP& kw, const std::string& name) { return mk_.field(mk_.type_name(kw), name, e->type, true); };
    if (owner == "java.lang.Float" || owner == "java.lang.Double") {
        const TypeP word = ir::prim_type(owner == "java.lang.Float" ? ir::Prim::F32 : ir::Prim::F64);
        if (s->name == "POSITIVE_INFINITY") return keyword_member(word, "PositiveInfinity");
        if (s->name == "NEGATIVE_INFINITY") return keyword_member(word, "NegativeInfinity");
        if (s->name == "NaN") return keyword_member(word, "NaN");
        if (s->name == "MAX_VALUE") return keyword_member(word, "MaxValue");
        if (s->name == "MIN_VALUE") return keyword_member(word, "Epsilon");   // (Java's smallest positive value)
    }
    static const std::map<std::pair<std::string_view, std::string_view>, std::pair<ir::Prim, std::string_view>> limits = {
        {{"java.lang.Integer", "MAX_VALUE"}, {ir::Prim::I32, "MaxValue"}}, {{"java.lang.Integer", "MIN_VALUE"}, {ir::Prim::I32, "MinValue"}},
        {{"java.lang.Long", "MAX_VALUE"}, {ir::Prim::I64, "MaxValue"}}, {{"java.lang.Long", "MIN_VALUE"}, {ir::Prim::I64, "MinValue"}},
        {{"java.lang.Short", "MAX_VALUE"}, {ir::Prim::I16, "MaxValue"}}, {{"java.lang.Short", "MIN_VALUE"}, {ir::Prim::I16, "MinValue"}},
        {{"java.lang.Byte", "MAX_VALUE"}, {ir::Prim::I8, "MaxValue"}}, {{"java.lang.Byte", "MIN_VALUE"}, {ir::Prim::I8, "MinValue"}},
        {{"java.lang.Character", "MAX_VALUE"}, {ir::Prim::U16, "MaxValue"}}, {{"java.lang.Character", "MIN_VALUE"}, {ir::Prim::U16, "MinValue"}}};
    if (const auto it = limits.find({owner, s->name}); it != limits.end()) {
        const TypeP word = it->second.first == ir::Prim::U16 ? char_type() : ir::prim_type(it->second.first);
        return keyword_member(word, std::string(it->second.second));
    }
    if (owner == "java.lang.Character") {   // Java's Unicode categories, by Java's own numbers (.NET numbers them differently)
        static const std::unordered_map<std::string_view, int> categories = {
            {"UNASSIGNED", 0}, {"UPPERCASE_LETTER", 1}, {"LOWERCASE_LETTER", 2}, {"TITLECASE_LETTER", 3}, {"MODIFIER_LETTER", 4}, {"OTHER_LETTER", 5},
            {"NON_SPACING_MARK", 6}, {"ENCLOSING_MARK", 7}, {"COMBINING_SPACING_MARK", 8}, {"DECIMAL_DIGIT_NUMBER", 9}, {"LETTER_NUMBER", 10},
            {"OTHER_NUMBER", 11}, {"SPACE_SEPARATOR", 12}, {"LINE_SEPARATOR", 13}, {"PARAGRAPH_SEPARATOR", 14}, {"CONTROL", 15}, {"FORMAT", 16},
            {"PRIVATE_USE", 18}, {"SURROGATE", 19}, {"DASH_PUNCTUATION", 20}, {"START_PUNCTUATION", 21}, {"END_PUNCTUATION", 22},
            {"CONNECTOR_PUNCTUATION", 23}, {"OTHER_PUNCTUATION", 24}, {"MATH_SYMBOL", 25}, {"CURRENCY_SYMBOL", 26}, {"MODIFIER_SYMBOL", 27},
            {"OTHER_SYMBOL", 28}, {"INITIAL_QUOTE_PUNCTUATION", 29}, {"FINAL_QUOTE_PUNCTUATION", 30}};
        if (const auto category = categories.find(s->name); category != categories.end())
            return mk_.cast(ir::prim_type(ir::Prim::I8), ir::make(arena_, ir::Lit{.value = std::int64_t{category->second}}, ir::i32_type()));
    }
    if (owner == "java.lang.Math" && (s->name == "PI" || s->name == "E")) return mk_.field(mk_.type_name("System.Math"), s->name, e->type, true);
    if (owner == "java.lang.System" && (s->name == "out" || s->name == "err"))
        return mk_.field(mk_.type_name("System.Console"), s->name == "out" ? "Out" : "Error", ir::class_type("System.IO.TextWriter"), true);
    if (class_named(owner) == nullptr && library_name(owner)) throw Declined{"library", "the Java library field " + owner + "." + s->name + " has no C# mapping"};
    if (auto n = member_name(*s)) sel.sym = renamed(s, *n);
    sel.obj = expr(sel.obj);
    return e;
}

Node* Pass::new_object(Node* e) {
    auto& made = *ir::get<ir::New>(e);
    const TypeP& t = e->type;
    const std::string* q = qual(t);
    const auto args = [&] {
        std::vector<Node*> out;
        for (Node* a : made.args) out.push_back(expr(a));
        return out;
    };
    if (q == nullptr || class_named(*q) != nullptr) {
        for (std::size_t i = 0; i < made.args.size(); ++i) {
            const bool typed = made.sym != nullptr && i < made.sym->params.size() && made.sym->params[i] && made.sym->params[i]->kind() != "typevar" &&
                               !(made.sym->varargs && i + 1 >= made.sym->params.size());
            made.args[i] = typed ? to(made.args[i], made.sym->params[i]) : expr(made.args[i]);
        }
        return e;
    }
    const auto make_new = [&](const TypeP& type, std::vector<Node*> given) { return ir::make(arena_, ir::New{.sym = made.sym, .args = std::move(given)}, type); };
    if (*q == "java.util.TreeMap" || *q == "java.util.TreeSet") {   // ordered as Java orders its keys, not by the culture's rules for text
        const auto* c = std::get_if<ir::ClassType>(&t->body);
        const TypeP key = c != nullptr && !c->args.empty() ? c->args[0] : obj_type();
        const auto order = [&] {
            auto* call = ir::get<ir::Call>(jlib("Order", {}, TypeP{}));
            call->typeargs.push_back(key);
            return ir::make(arena_, std::move(*call), TypeP{});
        };
        if (made.args.empty()) return make_new(t, {order()});
        if (made.args.size() == 1) {
            const std::string* given = qual(made.args[0]->type);
            if (given != nullptr && *given == "java.util.Comparator") {
                auto* create = ir::get<ir::Call>(mk_.static_call("System.Collections.Generic.Comparer", "Create", {expr(made.args[0])}, TypeP{}));
                auto* by = ir::get<ir::TypeName>(create->target);
                (void)by;
                create->target = mk_.type_name(ir::class_type("System.Collections.Generic.Comparer", {key}));
                return make_new(t, {ir::make(arena_, std::move(*create), TypeP{})});
            }
            std::vector<Node*> given_args = args();
            given_args.push_back(order());
            return make_new(t, std::move(given_args));
        }
        throw Declined{"library", "new " + *q + " with these arguments"};
    }
    if (*q == "java.util.LinkedHashMap" && made.args.size() == 3) throw Declined{"library", "a LinkedHashMap in access order"};
    if (generic_table().contains(*q)) {
        made.args = args();
        return e;
    }
    if (*q == "java.lang.String") {
        if (made.args.empty()) return make_new(t, {ir::make(arena_, ir::NewArray{.elem = char_type(), .dims = {mk_.number(0)}}, ir::array_of(char_type()))});
        const auto* array = made.args[0]->type ? std::get_if<ir::ArrayType>(&made.args[0]->type->body) : nullptr;
        if (array != nullptr && array->elem && array->elem->kind() == "u16" && (made.args.size() == 1 || made.args.size() == 3)) {
            made.args = args();
            return e;
        }
        if (made.args.size() == 1 && is_java_string(made.args[0]->type)) return make_new(t, {mk_.call(expr(made.args[0]), "ToCharArray", {}, ir::array_of(char_type()))});
        throw Declined{"library", "new java.lang.String with these arguments"};
    }
    if (*q == "java.lang.StringBuilder" || *q == "java.lang.StringBuffer") {
        if (!made.args.empty() && is_java_string(made.args[0]->type)) return make_new(t, {string_of(made.args[0])});
        made.args = args();
        return e;
    }
    if (*q == "java.util.Random") {
        runtime_.insert("JavaRt");
        made.args = args();
        return e;
    }
    const auto mapped = class_table().find(*q);
    if (mapped != class_table().end() && mapped->second.ends_with("Exception")) {
        if (made.args.size() == 1 && !is_java_string(made.args[0]->type)) {   // made from a cause: its message is the cause's text
            Node* cause = expr(made.args[0]);
            return make_new(t, {jlib("CauseText", {cause}, ir::class_type("System.String")), ir::clone(arena_, cause)});
        }
        if (made.args.size() > 2) throw Declined{"exception", "an exception made with more than a message and a cause"};
        made.args = args();
        return e;
    }
    if (*q == "java.lang.Object") return e;
    if (is_box(t) && made.args.size() == 1 && !is_java_string(made.args[0]->type)) return mk_.cast(t, expr(made.args[0]));
    throw Declined{"library", "new " + *q + ": no C# mapping"};
}

// What the registry has no one template for: streams, Optional and Collectors (the runtime's JStream ...), String.format, a Comparator's
// combinators, the collections' iterators and their equality, Map's merge and compute, List.of, Set.of, Map.of and Arrays.asList.
Node* Pass::library_call(Node* e) {
    auto& c = *ir::get<ir::Call>(e);
    const ir::Sym& s = *c.sym;
    const std::string owner = s.owner.value_or("");
    const auto arguments = [&] {
        std::vector<Node*> out;
        for (Node* a : c.args) out.push_back(expr(a));
        return out;
    };
    const auto uses = [&] {
        runtime_.insert("JavaRt");
        runtime_.insert("JavaCollections");
        runtime_.insert("JavaStreams");
    };
    // the type argument javac inferred for the call's result: what C# cannot infer for an untyped lambda
    const auto result_arg = [&](std::size_t i) -> TypeP {
        const auto* ct = e->type ? std::get_if<ir::ClassType>(&e->type->body) : nullptr;
        if (ct == nullptr || i >= ct->args.size()) throw Declined{"library", owner + "." + s.name + " with no inferred type"};
        return ct->args[i];
    };
    const auto on_target = [&]() -> Node* {
        uses();
        c.target = expr(c.target);
        for (Node*& a : c.args) a = expr(a);
        return e;
    };
    const auto on_type = [&](const TypeP& type, const std::string& name, std::vector<Node*> args) {
        uses();
        auto sym = mk_.method(name, qual(type) != nullptr ? std::optional<std::string>(*qual(type)) : std::nullopt, true);
        return ir::make(arena_, ir::Call{.target = mk_.type_name(type), .sym = sym, .args = std::move(args)}, e->type);
    };
    const auto lib = [&](const std::string& name, std::vector<Node*> args, std::vector<TypeP> typeargs = {}) {
        uses();
        auto* made = ir::get<ir::Call>(mk_.static_call("JLib", name, std::move(args), e->type));
        for (TypeP& t : typeargs) made->typeargs.push_back(std::move(t));
        return ir::make(arena_, std::move(*made), e->type);
    };
    const bool is_static = s.is_static || ir::is<ir::TypeName>(c.target);
    const auto family = family_table().find(owner);
    const std::string fam = family != family_table().end() ? family->second : std::string();

    if (owner == "java.lang.Number" && !is_static && c.args.empty() && c.target != nullptr) {   // a Number is an object here: its box's value
        static const std::unordered_map<std::string_view, std::string_view> values = {{"intValue", "numberInt"},     {"longValue", "numberLong"},
                                                                                      {"doubleValue", "numberDouble"}, {"floatValue", "numberFloat"},
                                                                                      {"shortValue", "numberShort"}, {"byteValue", "numberByte"}};
        if (const auto v = values.find(s.name); v != values.end()) return lib(std::string(v->second), {expr(c.target)});
    }
    if (owner == "java.util.stream.Stream") return is_static ? on_type(ir::class_type("JStream", {result_arg(0)}), s.name, arguments()) : on_target();
    if (owner == "java.util.stream.IntStream") return is_static ? on_type(ir::class_type("JIntStream"), s.name, arguments()) : on_target();
    if (owner == "java.util.Optional" || owner == "java.util.OptionalInt" || owner == "java.util.OptionalDouble") {
        if (!is_static) return on_target();
        const TypeP type = owner == "java.util.Optional" ? ir::class_type("JOptional", {result_arg(0)})
                                                         : ir::class_type(owner == "java.util.OptionalInt" ? "JOptionalInt" : "JOptionalDouble");
        return on_type(type, s.name, arguments());
    }
    if (owner == "java.util.stream.Collectors") return on_type(ir::class_type("JCollectors", {result_arg(0)}), s.name, arguments());
    if (owner == "java.util.Comparator" && s.name != "compare") {
        std::vector<Node*> all;
        if (!is_static) all.push_back(expr(c.target));
        for (Node* a : arguments()) all.push_back(a);
        return on_type(ir::class_type("JCompare", {result_arg(0)}), s.name, std::move(all));
    }
    if (owner == "java.util.Arrays" && s.name == "stream" && c.args.size() == 1) {
        const auto* array = c.args[0]->type ? std::get_if<ir::ArrayType>(&c.args[0]->type->body) : nullptr;
        if (array != nullptr && array->elem && array->elem->kind() == "i32") return on_type(ir::class_type("JIntStream"), "of", arguments());
        if (array == nullptr || array->elem->is_prim()) throw Declined{"library", "Arrays.stream of this array"};
        return on_type(ir::class_type("JStream", {array->elem}), "of", arguments());
    }
    if (owner == "java.util.List" && s.name == "of" && is_static) return lib("listOf", arguments(), {result_arg(0)});
    if (owner == "java.lang.String" && ((s.name == "format" && is_static) || s.name == "formatted")) {
        if (s.name == "format" && !c.args.empty() && !is_java_string(c.args[0]->type)) throw Declined{"library", "String.format with a Locale"};
        std::vector<Node*> all;
        if (s.name == "formatted") all.push_back(expr(c.target));
        for (Node* a : arguments()) all.push_back(a);
        return lib("format", std::move(all));
    }
    if (owner == "java.io.PrintStream" && (s.name == "printf" || s.name == "format") && c.target != nullptr) {
        if (!c.args.empty() && !is_java_string(c.args[0]->type)) throw Declined{"library", "printf with a Locale"};
        Node* text = lib("format", arguments());
        text->type = ir::class_type("java.lang.String");
        return mk_.call(expr(c.target), "Write", {text}, ir::void_type());
    }
    if (owner == "java.lang.Comparable" && s.name == "compareTo" && c.args.size() == 1 && c.target != nullptr)   // (a bounded type variable: C# has no bound here)
        return lib("Natural", {expr(c.target), expr(c.args[0])});
    if (owner == "java.util.Iterator") {
        if ((s.name == "hasNext" || s.name == "next") && c.args.empty()) return on_target();
        throw Declined{"library", "Iterator." + s.name};
    }
    if (!fam.empty() && !is_static) {
        if ((fam == "List" || fam == "Set") && s.name == "iterator" && c.args.empty() && c.target != nullptr) {
            const std::string* held = qual(c.target->type);
            const auto kind = held != nullptr ? family_table().find(*held) : family_table().end();
            if (kind != family_table().end() && (kind->second == "Set" || kind->second == "Map")) {
                const std::optional<std::string> made = concrete(c.target);
                if (!made || !ordered(*made)) throw Declined{"hash-order", "iterating a " + made.value_or("?") + ": Java visits it in hash order, .NET in another"};
            }
            Node* target = strip(c.target);
            return lib("iteratorOf", {ir::is<ir::Call>(target) ? call(target, false) : expr(c.target)});
        }
        if (fam == "Map" && c.target != nullptr &&
            ((s.name == "merge" && c.args.size() == 3) || (s.name == "putIfAbsent" && c.args.size() == 2) ||
             ((s.name == "computeIfAbsent" || s.name == "computeIfPresent" || s.name == "compute") && c.args.size() == 2))) {
            std::vector<Node*> all{expr(c.target)};
            for (Node* a : arguments()) all.push_back(a);
            return lib(s.name, std::move(all));
        }
    }
    if (owner == "java.util.Set" && s.name == "of" && is_static) return lib("setOf", arguments(), {result_arg(0)});
    if (owner == "java.util.Map" && s.name == "of" && is_static) {
        std::vector<Node*> pairs;
        for (std::size_t i = 0; i + 1 < c.args.size(); i += 2)
            pairs.push_back(mk_.static_call("System.ValueTuple", "Create", {expr(c.args[i]), expr(c.args[i + 1])}, TypeP{}));
        return lib("mapOf", std::move(pairs), {result_arg(0), result_arg(1)});
    }
    if (owner == "java.util.Arrays" && s.name == "asList") {
        const TypeP list = ir::class_type("java.util.ArrayList", {result_arg(0)});
        const auto make_list = [&](std::vector<Node*> args) {
            return ir::make(arena_, ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "List"), .args = std::move(args)}, list);
        };
        if (c.args.size() == 1 && c.args[0]->type && c.args[0]->type->kind() == "array") {
            // the list reads the array as it is later: a copy is the same while nothing in the program ever stores into an array of this type
            if (stored_arrays_.contains(ir::to_text(*c.args[0]->type)))
                throw Declined{"library", "Arrays.asList over an existing array: Java's list writes through to it"};
            return make_list({expr(c.args[0])});
        }
        if (c.args.empty()) return make_list({});
        const TypeP elem = result_arg(0);
        return make_list({ir::make(arena_, ir::NewArray{.elem = elem, .init = arguments()}, ir::array_of(elem))});
    }
    if (s.name == "stream" && c.args.empty() && c.target != nullptr && (fam == "List" || fam == "Set")) {
        if (const std::string* held = qual(c.target->type); held != nullptr && family_table().contains(*held) && family_table().at(*held) == "Set") {
            const std::optional<std::string> made = concrete(c.target);
            if (!made || !ordered(*made)) throw Declined{"hash-order", "streaming a " + made.value_or("?") + ": Java visits it in hash order, .NET in another"};
        }
        return lib("stream", {expr(c.target)});
    }
    // equals and hashCode of a collection compare what it holds
    if (c.target != nullptr && ((s.name == "equals" && c.args.size() == 1) || (s.name == "hashCode" && c.args.empty()))) {
        const std::optional<std::string> held = concrete(c.target);
        const bool list = fam == "List" && owner != "java.lang.Iterable";   // (a list is equal when its elements are, in order)
        if (list || ((fam == "Map" || fam == "Set") && held && *held != "java.util.TreeMap" && *held != "java.util.TreeSet" && *held != "java.util.Hashtable")) {
            if (s.name == "hashCode") return lib("Hash", {expr(c.target)});
            return lib("Equal", {expr(c.target), expr(c.args[0])});
        }
    }
    // the statics javac's members for a record call, and the ones a method reference names
    if (s.is_static && ((box_prim(owner).has_value() && s.name == "hashCode") || (owner == "java.util.Objects" && s.name == "hashCode")) && c.args.size() == 1)
        return lib("Hash", arguments());
    if (s.is_static && owner == "java.util.Objects" && s.name == "equals" && c.args.size() == 2) return lib("Equal", arguments());
    if (s.is_static && owner == "java.lang.Float" && s.name == "compare" && c.args.size() == 2) {
        runtime_.insert("JavaRt");
        return mk_.static_call("JavaRt", "CompareDouble", arguments(), e->type);
    }
    if (s.is_static && (owner == "java.lang.Integer" || owner == "java.lang.Long" || owner == "java.lang.Double") && s.name == "sum" && c.args.size() == 2) {
        std::vector<Node*> both = arguments();
        return ir::make(arena_, ir::Binary{.op = "+", .left = both[0], .right = both[1]}, e->type);
    }
    return nullptr;
}

Node* Pass::call(Node* e, bool statement) {
    auto& c = *ir::get<ir::Call>(e);
    const ir::SymP& s = c.sym;
    const auto plain = [&]() -> Node* {
        if (c.target != nullptr) c.target = expr(c.target);
        for (Node*& a : c.args) a = expr(a);
        return e;
    };
    if (s == nullptr) return plain();
    const std::string owner = s->owner.value_or("");
    if (s->name == "clone" && c.args.empty() && c.target != nullptr && c.target->type && c.target->type->kind() == "array")   // a copy of an array: shallow, as Java's
        return mk_.cast(c.target->type, mk_.call(expr(c.target), "Clone", {}, ir::class_type("System.Object")));
    if (s->name == "containsKey" && c.args.size() == 1 && c.target != nullptr && c.target->type && c.args[0]->type) {   // a key of another type: not there
        const auto* held = std::get_if<ir::ClassType>(&c.target->type->body);
        if (held != nullptr && !held->args.empty() && ir::to_text(*held->args[0]) != ir::to_text(*c.args[0]->type) && class_named(owner) == nullptr) {
            runtime_.insert("JavaRt");
            return mk_.static_call("JavaRt", "ContainsKey", {expr(c.target), expr(c.args[0])}, ir::bool_type());
        }
    }
    if (c.target != nullptr && c.target->type && class_named(owner) == nullptr) {   // a null put into a collection of numbers
        static const std::unordered_set<std::string_view> stores = {"add", "addFirst", "addLast", "offer", "push", "put", "putIfAbsent", "replace", "set"};
        if (const auto* held = std::get_if<ir::ClassType>(&c.target->type->body); held != nullptr && stores.contains(s->name)) {
            const bool of_boxes = std::any_of(held->args.begin(), held->args.end(), [](const TypeP& a) { return is_box(a) && !is_null_box(a); });
            const bool given_null = std::any_of(c.args.begin(), c.args.end(), [](Node* a) { return null_lit(strip(a)); });
            if (of_boxes && given_null) throw Declined{"boxed-null", "a null held in a collection of " + held->args.back()->simple() + ": the translation keeps those values"};
        }
    }
    if (!s->is_static && c.target != nullptr && !ir::is<ir::TypeName>(c.target) && is_box(c.target->type)) {   // a method of a box
        if (s->name == "equals" && c.args.size() == 1) {   // (of another type of box, false)
            const TypeP object = ir::class_type("System.Object");
            return mk_.call(mk_.cast(object, expr(c.target)), "Equals", {mk_.cast(object, kept(c.args[0]))}, ir::bool_type());
        }
        if (s->name == "compareTo" && c.args.size() == 1) return jlib("Natural", {expr(c.target), expr(c.args[0])}, ir::i32_type());
        static const std::unordered_map<std::string_view, std::pair<ir::Prim, int>> values = {
            {"byteValue", {ir::Prim::I8, 1}}, {"shortValue", {ir::Prim::I16, 2}}, {"intValue", {ir::Prim::I32, 3}}, {"longValue", {ir::Prim::I64, 4}},
            {"floatValue", {ir::Prim::F32, 5}}, {"doubleValue", {ir::Prim::F64, 6}}};
        static const std::unordered_map<std::string_view, int> ranks = {{"java.lang.Byte", 1}, {"java.lang.Short", 2}, {"java.lang.Integer", 3},
                                                                        {"java.lang.Long", 4}, {"java.lang.Float", 5}, {"java.lang.Double", 6}};
        const auto wanted = values.find(s->name);
        const auto have = ranks.find(*qual(c.target->type));
        if (wanted != values.end() && have != ranks.end() && c.args.empty() && wanted->second.second >= have->second)   // (a widening: C#'s cast is Java's)
            return mk_.cast(ir::prim_type(wanted->second.first), expr(c.target));
    }
    if (Node* got = enum_call(e)) return got;
    if ((owner == "java.lang.Class" || owner == "java.lang.reflect.Constructor") && s->ret && s->ret->kind() == "typevar" && !statement) {
        const std::string* wanted = qual(e->type);   // (an object in C#: the type Java knows it to be)
        if (wanted != nullptr && *wanted != "java.lang.Object") {
            const TypeP to = e->type;
            auto copy = std::make_shared<ir::Sym>(*s);
            copy->ret = obj_type();
            c.sym = std::move(copy);
            e->type = obj_type();
            return mk_.cast(to, call(e, false));
        }
    }
    const ir::ClassDecl* own = class_named(owner);
    if (own != nullptr && c.target == nullptr && !s->is_static && outer_path(owner) != nullptr)   // a method of an enclosing instance, by its name alone
        c.target = ir::make(arena_, ir::This{}, ir::class_type(owner));
    if (own != nullptr && (own->kind == "enum" && own->qname && object_enum(*own->qname) == nullptr) && s->is_static && (s->name == "values" || s->name == "valueOf")) {
        const TypeP tn = ir::class_type(own->qname.value_or(""));
        auto* made = ir::get<ir::Call>(mk_.static_call("System.Enum", s->name == "values" ? "GetValues" : "Parse", {}, e->type));
        made->typeargs.push_back(tn);
        if (s->name == "valueOf") made->args.push_back(expr(c.args.at(0)));
        return ir::make(arena_, std::move(*made), e->type);
    }
    if (own != nullptr || owner.empty()) {   // a method of the program: its C# name
        if (auto n = member_name(*s)) c.sym = renamed(s, *n);
        if (c.target != nullptr) c.target = expr(c.target);
        for (std::size_t i = 0; i < c.args.size(); ++i) {
            const bool typed = i < s->params.size() && s->params[i] && s->params[i]->kind() != "typevar" && !(s->varargs && i + 1 >= s->params.size());
            c.args[i] = typed ? to(c.args[i], s->params[i]) : expr(c.args[i]);
        }
        return e;
    }
    if ((owner == "java.lang.Double" || owner == "java.lang.Float") && s->is_static && c.args.size() == 1) {   // a number's bits, with Java's one NaN
        if (s->name == "doubleToLongBits") return jlib("DoubleBits", {expr(c.args[0])}, e->type);
        if (s->name == "floatToIntBits") return jlib("FloatBits", {expr(c.args[0])}, e->type);
        if (s->name == "longBitsToDouble") return mk_.static_call("System.BitConverter", "Int64BitsToDouble", {expr(c.args[0])}, e->type);
        if (s->name == "intBitsToFloat") return mk_.static_call("System.BitConverter", "Int32BitsToSingle", {expr(c.args[0])}, e->type);
        if (s->name == "floatToRawIntBits") return mk_.static_call("System.BitConverter", "SingleToInt32Bits", {expr(c.args[0])}, e->type);
    }
    {   // what .NET's collections and numbers have no one call for
        const auto helper = [&](const std::string& name, const std::vector<Node*>& given) {
            std::vector<Node*> parts;
            for (Node* a : given) parts.push_back(expr(a));
            return jlib(name, std::move(parts), e->type);
        };
        const auto family = family_table().find(owner);
        const bool collection = family != family_table().end() || owner == "java.util.Collection";
        if (collection && c.target != nullptr && s->name == "removeAll" && c.args.size() == 1) return helper("RemoveAll", {c.target, c.args[0]});
        if (c.target != nullptr && s->name == "addAll" && c.args.size() == 1) {
            const std::string* on = qual(c.target->type);
            const auto kind = on != nullptr ? family_table().find(*on) : family_table().end();
            if (kind != family_table().end() && kind->second == "Set") return helper("AddAll", {c.target, c.args[0]});
        }
        if (owner == "java.util.TreeSet" && c.target != nullptr && c.args.empty() && s->name == "first") return helper("First", {c.target});
        if (owner == "java.util.TreeSet" && c.target != nullptr && c.args.empty() && s->name == "last") return helper("Last", {c.target});
        if (owner == "java.util.Arrays" && s->name == "fill" && c.args.size() == 4) return helper("FillRange", c.args);
        if (owner == "java.lang.Integer" && s->is_static && s->name == "toString" && c.args.size() == 2) return helper("ToRadix", c.args);
        if (owner == "java.lang.Float" && s->is_static && s->name == "parseFloat" && c.args.size() == 1) return helper("ParseFloat", c.args);
        if (owner == "java.lang.Throwable" && s->name == "toString" && c.args.empty() && c.target != nullptr) return helper("ThrowableText", {c.target});
        if (owner == "java.lang.Throwable" && c.target != nullptr && s->name == "getMessage" && c.args.empty()) return helper("Message", {c.target});
        if (owner == "java.lang.Throwable" && c.target != nullptr && s->name == "getSuppressed" && c.args.empty()) return helper("Suppressed", {c.target});
        if (owner == "java.lang.Throwable" && c.target != nullptr && s->name == "addSuppressed" && c.args.size() == 1) return helper("AddSuppressed", {c.target, c.args[0]});
        if (owner == "java.util.Arrays" && s->name == "sort" && c.args.size() == 2) {   // stable, as Java's is for objects
            const auto* array = c.args[0]->type ? std::get_if<ir::ArrayType>(&c.args[0]->type->body) : nullptr;
            if (array == nullptr) throw Declined{"library", "Arrays.sort of what is not an array"};
            const std::string* given = qual(c.args[1]->type);
            Node* order = expr(c.args[1]);
            if (given != nullptr && class_named(*given) != nullptr)   // (a Comparator class of the program: its compare)
                order = ir::make(arena_, ir::MethodRef{.target = order, .sym = mk_.method("compare", *given)}, ir::class_type("System.Comparison", {array->elem}));
            Node* by = ir::make(arena_, ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "Comparison"), .args = {order}}, ir::class_type("System.Comparison", {array->elem}));
            return jlib("SortWith", {expr(c.args[0]), by}, e->type);
        }
    }
    if (owner == "java.lang.Character" && s->name == "getType" && c.args.size() == 1 && c.args[0]->type && c.args[0]->type->kind() == "u16") {
        runtime_.insert("JavaCharTypes");
        return mk_.static_call("JavaCharTypes", "Of", {expr(c.args[0])}, e->type);
    }
    check_list_change(c);
    if (Node* got = library_call(e)) return got;
    const Row row = lookup(c);
    if (!row.mapping) {
        if (object_method(s->name, c.args.size()) && c.target != nullptr) {
            const std::string* tq = qual(c.target->type);
            if (tq == nullptr || class_named(*tq) != nullptr || !library_name(*tq)) {
                c.sym = renamed(s, std::string(*object_method(s->name, c.args.size())));
                return plain();
            }
        }
        if (library_name(owner)) throw Declined{"library", "the Java library call " + owner + "." + s->name + "/" + std::to_string(c.args.size()) + " has no C# mapping"};
        return plain();
    }
    const apimap::Mapping& m = *row.mapping;
    if (m.kind == apimap::Kind::Unsupported) throw Declined{"library", owner + "." + s->name + ": " + (m.note.empty() ? std::string("no faithful C# form") : m.note)};
    const bool has_stmt = row.raw != nullptr && row.raw->stmt && !row.raw->stmt->empty();
    if (row.raw != nullptr && row.raw->ordered.value_or(false) && c.target != nullptr) {
        const std::optional<std::string> held = concrete(c.target);
        if (!held || !ordered(*held)) throw Declined{"hash-order", "iterating a " + held.value_or("?") + ": Java visits it in hash order, .NET in another"};
    }
    const std::string tmpl = statement && has_stmt ? *row.raw->stmt : m.target;
    if (tmpl.empty()) {
        if (has_stmt) throw Declined{"library", owner + "." + s->name + " used for its value"};
        return plain();
    }
    const std::string raw_target = row.raw != nullptr ? row.raw->target : std::string();
    if (raw_target.find("JavaRt") != std::string::npos) runtime_.insert("JavaRt");
    return fill(tmpl, e, c);
}

// `Type::method`, and any reference into Java's library, as the lambda it stands for.
Node* Pass::method_ref(Node* e) {
    auto& x = *ir::get<ir::MethodRef>(e);
    const ir::SymP& s = x.sym;
    if (s == nullptr || x.mode == "new" || !s->owner) throw Declined{"method-ref", "a constructor reference"};
    const bool in_library = class_named(*s->owner) == nullptr;
    const bool unbound = ir::is<ir::TypeName>(x.target) && !s->is_static;
    if (!in_library && !unbound) {   // (a method of the program: C#'s method group)
        if (auto n = member_name(*s)) x.sym = renamed(s, *n);
        x.target = expr(x.target);
        return as_interface(e, e->type);
    }
    const std::string* interface_name = qual(e->type);
    const auto single = interface_name != nullptr ? functional_method(*interface_name) : std::nullopt;
    if (!single) throw Declined{"method-ref", "a method reference of a type with no C# delegate"};
    if (!s->is_static && !unbound) {   // `out::println`: Java evaluates the receiver once, when the reference is made
        const auto* held = ir::get<ir::Select>(x.target);
        if (held == nullptr || held->sym == nullptr || held->sym->owner != "java.lang.System") throw Declined{"method-ref", "a reference to a method of a library object"};
    }
    const std::size_t count = single->second;
    const std::size_t receivers = unbound ? 1 : 0;
    if (count < receivers) throw Declined{"method-ref", "a method reference with no receiver"};
    std::vector<Node*> params, names;
    for (std::size_t i = 0; i < count; ++i) {
        TypeP type = obj_type();
        if (unbound && i == 0) type = ir::class_type(*s->owner);
        else if (i >= receivers && i - receivers < s->params.size()) type = s->params[i - receivers];
        const ir::SymP sym = ir::make_sym(ir::SymKind::Param, "a" + std::to_string(i), std::nullopt, false, type);
        params.push_back(ir::make(arena_, ir::Param{.sym = sym, .type = type}));
        names.push_back(mk_.name(sym, type));
    }
    Node* target = unbound ? names[0] : s->is_static ? mk_.type_name(*s->owner) : x.target;
    std::vector<Node*> args(names.begin() + static_cast<std::ptrdiff_t>(receivers), names.end());
    Node* body = ir::make(arena_, ir::Call{.target = target, .sym = s, .args = std::move(args)}, s->ret);
    Node* lambda = ir::make(arena_, ir::Lambda{.params = std::move(params), .body = body}, e->type);
    return expr(lambda);
}

// A lambda of a JDK functional interface named raw (Predicate: the decompiler's bytecode, or a raw source) whose parameters are declared of a
// class: its delegate takes objects (Func<object, bool>), and C# converts a lambda to it only if the lambda takes objects too. Each such
// parameter is taken as an object and read as its class (`(Item)x`), which is what Java's bridge method does.
void Pass::raw_lambda_params(ir::Lambda& l, const TypeP& face) {
    const auto* c = face ? std::get_if<ir::ClassType>(&face->body) : nullptr;
    if (c == nullptr || !c->args.empty() || !function_table().contains(c->name)) return;
    std::map<std::string, TypeP> narrowed;
    for (Node* pn : l.params) {
        auto* p = ir::get<ir::Param>(pn);
        if (p == nullptr || !p->sym || !p->type || !std::holds_alternative<ir::ClassType>(p->type->body)) continue;
        if (std::get<ir::ClassType>(p->type->body).name == "java.lang.Object") continue;
        narrowed[p->sym->name] = p->type;
        p->type = ir::class_type("java.lang.Object");
        p->sym = ir::make_sym(p->sym->kind, p->sym->name, p->sym->owner, p->sym->is_static, p->type);
    }
    if (narrowed.empty()) return;
    std::function<void(Node*&, const std::map<std::string, TypeP>&)> read_as = [&](Node*& n, const std::map<std::string, TypeP>& names) {
        if (n == nullptr || names.empty() || ir::is<ir::ClassDecl>(n)) return;
        if (const auto* inner = ir::get<ir::Lambda>(n)) {   // an inner lambda's parameters hide the names they take
            std::map<std::string, TypeP> left = names;
            for (Node* pn : inner->params)
                if (const auto* p = ir::get<ir::Param>(pn); p != nullptr && p->sym) left.erase(p->sym->name);
            ir::visit(*n, [&](auto& k) { k.each_child([&](Node*& child) { read_as(child, left); }); });
            return;
        }
        if (const auto* name = ir::get<ir::Name>(n); name != nullptr && name->sym && name->sym->kind == ir::SymKind::Param) {
            if (const auto it = names.find(name->sym->name); it != names.end()) {
                Node* taken = ir::make(arena_, ir::Name{.sym = ir::make_sym(ir::SymKind::Param, name->sym->name, name->sym->owner, false, ir::class_type("java.lang.Object"))},
                                       ir::class_type("java.lang.Object"));
                n = ir::make(arena_, ir::Cast{.x = taken}, it->second);
                return;
            }
        }
        ir::visit(*n, [&](auto& k) { k.each_child([&](Node*& child) { read_as(child, names); }); });
    };
    read_as(l.body, narrowed);
}

void Pass::check_lambda(Node* e) {
    const std::string* q = qual(e->type);
    if (q == nullptr) return;
    if (class_named(*q) != nullptr) {
        if (lambda_interfaces_.contains(*q)) return;
        throw Declined{"lambda", "a lambda of an interface of the program with no single method a delegate can stand for"};
    }
    if (!function_table().contains(*q) && *q != "java.lang.Runnable") throw Declined{"lambda", "a lambda of the interface " + *q + " (no C# delegate for it)"};
}

Node* Pass::expr(Node* e) {
    if (e == nullptr) return e;
    Node* x = strip(e);
    if (null_reads_.contains(x) && !read_as_is_.contains(x)) {   // Java reads the number: a null throws
        read_as_is_.insert(x);
        Node* inner = expr(e);
        read_as_is_.erase(x);
        runtime_.insert("JavaCollections");
        return jlib("Unbox", {inner}, plain_box(x->type));
    }
    if (Node* changed = update(x)) return changed;
    if (const auto held = capture_names_.find(x); held != capture_names_.end()) {   // a local of the methods around: the class's copy
        if (held->second.ends_with("_arg")) return mk_.name(ir::make_sym(ir::SymKind::Param, held->second, std::nullopt, false, x->type), x->type);
        const std::string owner = cls_ != nullptr ? cls_->qname.value_or("") : std::string();
        return mk_.field(ir::make(arena_, ir::This{}, ir::class_type(owner)), held->second, x->type);
    }
    if (ir::is<ir::This>(x)) return this_node(x);
    if (ir::is<ir::Call>(x)) return call(x, false);
    if (ir::is<ir::Select>(x)) return select(x);
    if (ir::is<ir::New>(x)) {
        if (const Node* body = ir::get<ir::New>(x)->body; body != nullptr) {
            if (const auto found = hoist_of_.find(body); found != hoist_of_.end()) return new_inner(x, *found->second);
            const auto why = refused_.find(body);
            throw Declined{"anonymous-class", why != refused_.end() ? why->second : std::string("an anonymous class")};
        }
        if (const std::string* q = qual(x->type)) {
            if (const ir::ClassDecl* own = class_named(*q); own != nullptr && (needs_outer(*own) || has_captures(*own))) return new_inner(x, *own);
        }
        return new_object(x);
    }
    if (auto* is = ir::get<ir::InstanceOf>(x)) {
        const auto* tested = is->test ? std::get_if<ir::ClassType>(&is->test->body) : nullptr;
        const ir::ClassDecl* own = tested != nullptr && tested->args.empty() ? class_named(tested->name) : nullptr;
        if (own != nullptr && !own->typeparams.empty()) {   // `o instanceof Box` of a generic class: a Box of any arguments (C#'s `is` names one)
            if (is->binding != nullptr) throw Declined{"instanceof", "a pattern of the raw type " + tested->name};
            Node* closed = ir::make(arena_, ir::ClassLit{.of = is->test}, ir::class_type("System.Type"));   // (its definition is what is compared)
            return jlib("isRaw", {expr(is->x), closed}, x->type);
        }
    }
    if (ir::is<ir::Binary>(x)) return binary(x);
    if (ir::is<ir::Concat>(x)) return concat(x);
    if (ir::is<ir::MethodRef>(x)) return method_ref(x);
    if (auto* l = ir::get<ir::Lambda>(x)) {
        check_lambda(x);
        raw_lambda_params(*l, x->type);
        const TypeP saved_ret = std::exchange(ret_, TypeP{});
        if (ir::is<ir::Block>(l->body)) {
            auto saved = labels_;
            l->body = stmt_node(l->body);
            labels_ = std::move(saved);
        } else {
            l->body = expr(l->body);
        }
        ret_ = saved_ret;
        return as_interface(x, x->type);
    }
    if (auto* c = ir::get<ir::Cond>(x)) {
        c->cond = expr(c->cond);
        c->then = to(c->then, x->type);
        c->orelse = to(c->orelse, x->type);
        return x;
    }
    if (auto* a = ir::get<ir::NewArray>(x)) {
        for (Node*& d : a->dims) {
            d = expr(d);
            if (catches_negative_size_ && !ir::is<ir::Lit>(strip(d))) d = jlib("ArraySize", {d}, ir::i32_type());   // (the program reads the message: Java's is the size)
        }
        if (a->dims.size() > 1 && a->init.empty()) {   // Java's arrays of arrays, each its own: C# has no one expression for them
            auto* made = ir::get<ir::Call>(jlib("Jagged", a->dims, x->type));
            made->typeargs.push_back(x->type);
            return ir::make(arena_, std::move(*made), x->type);
        }
        const auto* whole = x->type ? std::get_if<ir::ArrayType>(&x->type->body) : nullptr;   // (the array's own type says what its items hold)
        for (Node*& item : a->init) item = to(item, whole != nullptr ? whole->elem : a->elem);
        return x;
    }
    if (ir::is<ir::SwitchExpr>(x)) {
        auto& sw = *ir::get<ir::SwitchExpr>(x);
        sw.expr = enum_scrutinee(sw.expr, sw.cases);
        const TypeP saved = std::exchange(yield_type_, x->type);
        for (Node* cn : sw.cases) {
            auto* c = ir::get<ir::Case>(cn);
            for (Node*& l : c->labels) {
                if (!ir::is<ir::Select>(strip(l)) && !ir::is<ir::Lit>(strip(l))) l = expr(l);
            }
            if (c->body.size() == 1 && ir::is<ir::Block>(c->body[0])) {
                // C#'s arms are expressions: a block is a function called on the spot, its `yield` that function's return:
                // `((Func<T>)(() => { ...; return v; }))()`
                const TypeP saved_ret = std::exchange(ret_, TypeP{});
                auto saved_labels = labels_;
                Node* body = stmt_node(c->body[0]);
                labels_ = std::move(saved_labels);
                ret_ = saved_ret;
                const std::function<void(Node*&)> returns = [&](Node*& n) {
                    if (n == nullptr || ir::is<ir::SwitchExpr>(n) || ir::is<ir::Lambda>(n) || ir::is<ir::ClassDecl>(n)) return;
                    if (const auto* y = ir::get<ir::Yield>(n)) {
                        n = ir::make(arena_, ir::Return{.expr = y->expr});
                        return;
                    }
                    ir::visit(*n, [&](auto& k) { k.each_child([&](Node*& child) { returns(child); }); });
                };
                returns(body);
                const TypeP fn = ir::class_type("System.Func", {x->type});
                Node* lambda = ir::make(arena_, ir::Lambda{.body = body}, fn);
                auto invoke = ir::make_sym(ir::SymKind::Method, "Invoke");
                invoke->flags.push_back("delegate");
                Node* run = ir::make(arena_, ir::Call{.target = mk_.cast(fn, lambda), .sym = invoke}, x->type);
                c->body = {ir::make(arena_, ir::Yield{.expr = run})};
                continue;
            }
            stmts(c->body);
        }
        yield_type_ = saved;
        return x;
    }
    if (auto* w = ir::get<ir::Assign>(x)) {
        const TypeP place = w->target->type;
        w->target = expr(w->target);
        w->value = w->op ? expr(w->value) : to(w->value, place);
        return x;
    }
    ir::visit(*x, [&](auto& k) { k.each_child([&](Node*& child) { child = expr(child); }); });
    return x;
}

// ---- types ----------------------------------------------------------------------------------------------------------------------

TypeP Pass::cs(const TypeP& t) {
    if (!t) return t;
    if (const auto* p = std::get_if<ir::PrimType>(&t->body)) {
        if (p->prim == ir::Prim::U16 && !p->encoding) return char_type();
        return t;
    }
    if (const auto* a = std::get_if<ir::ArrayType>(&t->body)) {
        ir::ArrayType next = *a;
        next.elem = cs(a->elem);
        return ir::make_type(std::move(next));
    }
    if (const auto* w = std::get_if<ir::WildcardType>(&t->body)) {
        if (w->bound && w->bound->mode == ir::BoundMode::Extends) return cs(w->bound->type);
        return ir::class_type("System.Object");
    }
    const auto* c = std::get_if<ir::ClassType>(&t->body);
    if (c == nullptr) return t;
    const std::string& q = c->name;
    std::vector<TypeP> args;
    for (const TypeP& a : c->args) args.push_back(cs(a));
    if (const ir::ClassDecl* own = class_named(q)) {
        if (q == "java.lang.Class" || q == "java.lang.reflect.Constructor") return ir::class_type(q);   // (of no type: see prepare)
        if (args.empty() && !own->typeparams.empty()) args.assign(own->typeparams.size(), ir::class_type("System.Object"));   // a raw use: of Object
        ir::ClassType next = *c;
        next.args = std::move(args);
        return ir::make_type(std::move(next));
    }
    if (const auto prim = box_prim(q)) {
        const TypeP number = *prim == ir::Prim::U16 ? char_type() : ir::prim_type(*prim);
        return c->nullable ? ir::class_type("System.Nullable", {number}) : number;
    }
    if (const auto f = function_table().find(q); f != function_table().end()) {
        std::vector<TypeP> given = c->args;
        if (given.empty()) {   // a raw use: the type arguments are Object
            static const std::unordered_map<std::string_view, std::size_t> raw = {
                {"java.util.function.Function", 2}, {"java.util.function.BiFunction", 3}, {"java.util.function.Supplier", 1},
                {"java.util.function.Consumer", 1}, {"java.util.function.BiConsumer", 2}, {"java.util.function.Predicate", 1},
                {"java.util.function.BiPredicate", 2}, {"java.util.function.IntFunction", 1}, {"java.util.function.ToIntFunction", 1},
                {"java.util.Comparator", 1}};
            if (const auto r = raw.find(q); r != raw.end()) given.assign(r->second, obj_type());
        }
        std::vector<TypeP> mapped;
        for (const TypeP& a : f->second.arguments(given)) mapped.push_back(cs(a));
        return ir::class_type(std::string(f->second.generic), std::move(mapped));
    }
    if (const auto g = generic_table().find(q); g != generic_table().end()) {
        if (g->second.first.starts_with("J")) {
            runtime_.insert("JavaRt");
            runtime_.insert("JavaCollections");
        }
        if (args.empty()) args.assign(g->second.second, ir::class_type("System.Object"));
        if (args.size() != g->second.second) throw Declined{"type", "the type " + ir::to_text(*t)};
        return ir::class_type(std::string(g->second.first), std::move(args));
    }
    if (const auto m = class_table().find(q); m != class_table().end()) {
        const std::string name(m->second);
        if (name == "JavaRandom") runtime_.insert("JavaRt");
        if ((name == "System.IComparable" || name == "System.Collections.Generic.IEnumerable")) {
            if (args.empty() && name.ends_with("IEnumerable")) args.push_back(ir::class_type("System.Object"));
            return ir::class_type(name, std::move(args));
        }
        return ir::class_type(name);
    }
    if (q == "java.util.stream.Stream" || q == "java.util.stream.IntStream" || q == "java.util.Optional" || q == "java.util.OptionalInt" ||
        q == "java.util.OptionalDouble" || q == "java.util.stream.Collector") {
        runtime_.insert("JavaRt");
        runtime_.insert("JavaCollections");
        runtime_.insert("JavaStreams");
        if (q == "java.util.stream.IntStream") return ir::class_type("JIntStream");
        if (q == "java.util.OptionalInt") return ir::class_type("JOptionalInt");
        if (q == "java.util.OptionalDouble") return ir::class_type("JOptionalDouble");
        if (q == "java.util.stream.Stream" && args.size() == 1) return ir::class_type("JStream", std::move(args));
        if (q == "java.util.Optional" && args.size() == 1) return ir::class_type("JOptional", std::move(args));
        if (q == "java.util.stream.Collector" && args.size() == 3) return ir::class_type("JCollector", {args[0], args[2]});
        throw Declined{"type", "the raw type " + ir::to_text(*t)};
    }
    if (library_name(q)) throw Declined{"library", "the Java library class " + q + " has no C# mapping"};
    if (args.empty()) return t;
    ir::ClassType next = *c;   // (a runtime class: its arguments restated)
    next.args = std::move(args);
    return ir::make_type(std::move(next));
}

// Every type of the tree restated as .NET's.
void Pass::retype(Node* root) {
    for (Node* n : ir::walk(root)) {
        n->type = cs(n->type);
        ir::visit(*n, [&](auto& k) {
            using K = std::decay_t<decltype(k)>;
            if constexpr (std::is_same_v<K, ir::Local> || std::is_same_v<K, ir::Param> || std::is_same_v<K, ir::FieldDecl>) k.type = cs(k.type);
            if constexpr (std::is_same_v<K, ir::MethodDecl>) k.ret = cs(k.ret);
            if constexpr (std::is_same_v<K, ir::NewArray>) k.elem = cs(k.elem);
            if constexpr (std::is_same_v<K, ir::InstanceOf>) k.test = cs(k.test);
            if constexpr (std::is_same_v<K, ir::ClassLit>) k.of = cs(k.of);
            if constexpr (std::is_same_v<K, ir::Catch>) {
                for (TypeP& t : k.types) t = cs(t);
            }
            if constexpr (std::is_same_v<K, ir::ClassDecl>) {
                k.extends = cs(k.extends);
                for (TypeP& t : k.implements) t = cs(t);
                for (Node* tp : k.tparams) {
                    if (auto* p = ir::get<ir::TypeParam>(tp)) {
                        for (TypeP& b : p->bounds) b = cs(b);
                    }
                }
            }
            if constexpr (std::is_same_v<K, ir::MethodDecl>) {
                for (Node* tp : k.tparams) {
                    if (auto* p = ir::get<ir::TypeParam>(tp)) {
                        for (TypeP& b : p->bounds) b = cs(b);
                    }
                }
            }
            if constexpr (std::is_same_v<K, ir::Call>) {
                for (ir::TypeArg& a : k.typeargs) {
                    if (auto* t = std::get_if<TypeP>(&a)) *t = cs(*t);
                }
            }
            if constexpr (requires { k.sym; }) {
                if (k.sym != nullptr) {
                    auto copy = std::make_shared<ir::Sym>(*k.sym);
                    copy->type = cs(copy->type);
                    copy->ret = cs(copy->ret);
                    for (TypeP& p : copy->params) p = cs(p);
                    k.sym = std::move(copy);
                }
            }
        });
    }
}

// The runtime files the tree names.
void Pass::note_runtime(Node* root) {
    for (Node* n : ir::walk(root)) {
        const std::string* q = qual(n->type);
        if (q == nullptr) continue;
        if (*q == "JavaRt" || *q == "JavaRandom") runtime_.insert("JavaRt");
        if (*q == "JLib" || q->starts_with("JHash") || q->starts_with("JLinked") || *q == "JIterator") {
            runtime_.insert("JavaRt");
            runtime_.insert("JavaCollections");
        }
        if (*q == "JavaCharTypes") runtime_.insert("JavaCharTypes");
        if (q->starts_with("JStream") || q->starts_with("JIntStream") || q->starts_with("JOptional") || q->starts_with("JCollect") || *q == "JCompare") {
            runtime_.insert("JavaRt");
            runtime_.insert("JavaCollections");
            runtime_.insert("JavaStreams");
        }
    }
}

Result Pass::run(std::span<Node* const> units) {
    Result result;
    std::vector<Node*> java;
    for (Node* u : units) {
        const auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr || unit->lang != "java") {
            result.units.push_back(u);
            continue;
        }
        Node* copy = ir::clone(arena_, u);
        result.units.push_back(copy);
        java.push_back(copy);
        for (Node* t : ir::get<ir::Unit>(copy)->types) index(t);
    }
    std::vector<Node*> classes;
    const std::function<void(Node*)> collect = [&](Node* n) {
        if (auto* cd = ir::get<ir::ClassDecl>(n)) {
            classes.push_back(n);
            for (Node* m : cd->members) collect(m);
        }
    };
    for (Node* u : java) {
        for (Node* t : ir::get<ir::Unit>(u)->types) collect(t);
    }
    java_ = java;
    prepare(classes);
    for (Node* u : java) {
        for (Node*& t : ir::get<ir::Unit>(u)->types) {
            auto* cd = ir::get<ir::ClassDecl>(t);
            if (cd == nullptr) continue;
            if (const auto why = refused_class(*cd)) {
                t = mk_.unsupported("class", *why + " (" + cd->name.value_or("") + ")");
                continue;
            }
            try {
                rewrite_class(*cd);
            } catch (const Declined& d) {
                t = mk_.unsupported(d.kind, d.why);
            }
        }
    }
    for (Node* u : java) {   // the types last, once every unit is rewritten: the rules above read Java's (of other units' classes too)
        for (Node*& t : ir::get<ir::Unit>(u)->types) {
            auto* cd = ir::get<ir::ClassDecl>(t);
            if (cd == nullptr) continue;
            try {
                for (Node*& m : cd->members) {
                    try {
                        retype(m);
                    } catch (const Declined& d) {
                        if (auto* md = ir::get<ir::MethodDecl>(m); md != nullptr && ir::present(md->body)) {
                            md->body = mk_.block({mk_.expr_stmt(mk_.unsupported(d.kind, d.why))});
                            retype(m);
                        } else {
                            throw;
                        }
                    }
                }
                ir::ClassDecl shell = *cd;
                shell.members.clear();
                Node* head = ir::make(arena_, std::move(shell));
                retype(head);
                const auto& h = *ir::get<ir::ClassDecl>(head);
                cd->extends = h.extends;
                cd->implements = h.implements;
                cd->tparams = h.tparams;
            } catch (const Declined& d) {
                t = mk_.unsupported(d.kind, d.why + " (" + cd->name.value_or("") + ")");
                continue;
            }
            note_runtime(t);
        }
    }
    result.runtime = runtime_;
    return result;
}

}  // namespace jcs

Result lower(Arena& arena, std::span<ir::Node* const> units, const cb::java::apimap::Registry& registry) {
    jcs::Pass pass(arena, registry);
    return pass.run(units);
}

}  // namespace cb::translate::rules::java_csharp

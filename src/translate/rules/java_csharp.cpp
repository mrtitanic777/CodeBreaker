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

namespace cb::translate::rules::java_csharp {

namespace jcs {

using ir::Node;
using ir::TypeP;
namespace text = cb::text;
namespace apimap = cb::java::apimap;

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
    bool closeable(const ir::ClassDecl& c) const;
    void prepare(std::vector<Node*>& classes);
    std::optional<std::string> refused_class(ir::ClassDecl& c);

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
    std::map<std::pair<std::string, std::string>, std::string> concrete_;
    std::unordered_set<std::string> stored_arrays_;
    bool list_over_array_ = false;
    int try_helpers_ = 0;
    std::map<std::string, std::pair<bool, bool>> labels_;   // a label -> its uses (break, continue)
    TypeP ret_;                                              // the result of the method (or lambda) whose body is rewritten
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
            if (x != nullptr && x->kind == "method" && x->name == m.name && signature(*x) == mine && !has(x->mods, "static") && !has(x->mods, "private")) return true;
        }
    }
    return false;
}

bool Pass::closeable(const ir::ClassDecl& start) const {
    std::set<std::string> seen;
    for (const ir::ClassDecl* c = &start; c != nullptr && c->qname && seen.insert(*c->qname).second; c = base_of(*c)) {
        for (const TypeP& i : c->implements) {
            const std::string* q = qual(i);
            if (q != nullptr && (*q == "java.lang.AutoCloseable" || *q == "java.io.Closeable")) return true;
        }
    }
    return false;
}

// What of this class the rule set does not carry yet (each named in the header): the reason, or nothing.
std::optional<std::string> Pass::refused_class(ir::ClassDecl& c) {
    if (c.qname == "java.lang.Class" || c.qname == "java.lang.reflect.Constructor") return "the reflection classes";
    if (c.kind == "enum") {
        for (Node* m : c.members) {
            const auto* md = ir::get<ir::MethodDecl>(m);
            const auto* ec = ir::get<ir::EnumConst>(m);
            if (ir::is<ir::FieldDecl>(m) || (md != nullptr && !has(md->mods, "IMPLICIT")) || (ec != nullptr && (!ec->args.empty() || ec->body != nullptr)))
                return "a Java enum with fields, methods or constructor arguments (an enum of objects)";
        }
    }
    for (const TypeP& i : c.implements) {
        const std::string* q = qual(i);
        if (q != nullptr && *q == "java.lang.Iterable") return "a class that is a Java Iterable (its GetEnumerator)";
        if (q != nullptr && *q == "java.util.Comparator") return "a class that is a Java Comparator (its conversion to Comparison)";
    }
    const bool inner = c.kind == "class" && c.outer && !has(c.mods, "static");
    for (Node* m : c.members) {
        if (const auto* nested = ir::get<ir::ClassDecl>(m); nested != nullptr) continue;
        for (Node* n : ir::walk(m)) {
            if (ir::is<ir::LocalClass>(n)) return "a class declared in a method";
            if (const auto* made = ir::get<ir::New>(n); made != nullptr && made->body != nullptr) return "an anonymous class";
            if (const auto* made = ir::get<ir::New>(n); made != nullptr && made->outer != nullptr) return "an inner class made on an outer instance";
            if (const auto* th = ir::get<ir::This>(n); th != nullptr && th->qualifier && c.qname && qual(th->qualifier) != nullptr && *qual(th->qualifier) != *c.qname)
                return "an inner class that reaches its outer instance";
            if (inner) {
                if (const auto* call = ir::get<ir::Call>(n); call != nullptr && call->target == nullptr && call->sym != nullptr && !call->sym->is_static &&
                                                             call->sym->owner && c.qname && *call->sym->owner != *c.qname && class_named(*call->sym->owner) != nullptr) {
                    // a method of its own base is its own
                    bool own = false;
                    for (const ir::ClassDecl* k = &c; k != nullptr; k = base_of(*k)) own = own || k->qname == call->sym->owner;
                    if (!own) return "an inner class that reaches its outer instance";
                }
                if (const auto* sel = ir::get<ir::Select>(n); sel != nullptr && ir::is<ir::This>(sel->obj) && sel->sym && sel->sym->owner && c.qname &&
                                                             *sel->sym->owner != *c.qname && class_named(*sel->sym->owner) != nullptr) {
                    bool own = false;
                    for (const ir::ClassDecl* k = &c; k != nullptr; k = base_of(*k)) own = own || k->qname == sel->sym->owner;
                    if (!own) return "an inner class that reaches its outer instance";
                }
            }
            if (ir::is<ir::Lambda>(n) || ir::is<ir::MethodRef>(n)) {
                const std::string* target = qual(n->type);
                if (target != nullptr && class_named(*target) != nullptr) return "a lambda of an interface of the program";
            }
        }
    }
    return std::nullopt;
}

// ---- before the rewrite: what the whole program says ---------------------------------------------------------------------------------

void Pass::prepare(std::vector<Node*>& classes) {
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
            if (d != nullptr && d->name == m.name && d->params.size() == m.params.size()) return out;
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
                if (d != nullptr && d->kind == "method" && d->name == m.name && d->params.size() == m.params.size() && !has(d->mods, "static")) out = access(d->mods, *up);
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
                return md != nullptr && md->name == s.name;
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
        for (Node* n : ir::walk(f->init)) {
            if (ir::is<ir::This>(n)) return true;
            if (const auto* call = ir::get<ir::Call>(n); call != nullptr && call->target == nullptr && call->sym && !call->sym->is_static) return true;
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
            if (!has(f->mods, "static") && f->init != nullptr && (base_is_ours || init_needs_this)) moves.push_back(mn);
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
    std::erase_if(c.implements, [](const TypeP& i) {
        const std::string* q = qual(i);
        return q != nullptr && (*q == "java.io.Serializable" || *q == "java.lang.Cloneable" || *q == "java.util.RandomAccess");
    });
    c.mods = class_mods(c);
    struct Record {
        ir::ClassDecl& c;
        ~Record() {
            if (c.kind == "record") c.kind = "class";
        }
    } record{c};
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
            rewrite_class(*nested);
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
            if (m->kind == "method") m->name = method_name(*m, c);
            if (m->kind == "ctor") {
                const bool implicit = has(m->mods, "IMPLICIT");
                const auto pro = prologue_.find(qname);
                const bool needed = c.kind == "record" || (pro != prologue_.end() && !pro->second.empty());
                // the statements Java runs after the base call: in each constructor that does not call another of its own
                std::vector<Node*> body = body_statements(m->body);
                const auto* first = body.empty() ? nullptr : ir::get<ir::CtorCall>(body[0]);
                if (pro != prologue_.end() && !(first != nullptr && first->kind == "this")) {
                    std::vector<Node*> copies;
                    for (Node* s : pro->second) copies.push_back(ir::clone(arena_, s));
                    body.insert(body.begin() + (first != nullptr ? 1 : 0), copies.begin(), copies.end());
                }
                m->body = mk_.block(std::move(body));
                m->mods = mods;
                if (m->sym != nullptr && m->sym->varargs && !m->params.empty()) {   // Java's T... is C#'s params T[]
                    if (auto* last = ir::get<ir::Param>(m->params.back()); last != nullptr && !has(last->mods, "params")) last->mods.push_back("params");
                }
                if (implicit && !needed) m->mods.push_back("IMPLICIT");
                labels_.clear();
                ret_ = TypeP{};
                suite(m->body);
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
        x->expr = expr(x->expr);
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
    if (ir::is<ir::LocalClass>(s)) throw Declined{"local-class", "a class declared in a method"};
    if (ir::is<ir::Yield>(s)) throw Declined{"switch-expression", "a switch-expression arm with statements"};
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
    const ir::ClassDecl* own = class_named(owner);
    if (own != nullptr && own->kind == "enum" && s->is_static && (s->name == "values" || s->name == "valueOf")) {
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
            Node* by = ir::make(arena_, ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "Comparison"), .args = {expr(c.args[1])}},
                                ir::class_type("System.Comparison", {array->elem}));
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
        return e;
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

void Pass::check_lambda(Node* e) {
    const std::string* q = qual(e->type);
    if (q == nullptr) return;
    if (class_named(*q) != nullptr) throw Declined{"lambda", "a lambda of an interface of the program"};
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
    if (ir::is<ir::Call>(x)) return call(x, false);
    if (ir::is<ir::Select>(x)) return select(x);
    if (ir::is<ir::New>(x)) {
        if (ir::get<ir::New>(x)->body != nullptr) throw Declined{"anonymous-class", "an anonymous class"};
        return new_object(x);
    }
    if (ir::is<ir::Binary>(x)) return binary(x);
    if (ir::is<ir::Concat>(x)) return concat(x);
    if (ir::is<ir::MethodRef>(x)) return method_ref(x);
    if (auto* l = ir::get<ir::Lambda>(x)) {
        check_lambda(x);
        const TypeP saved_ret = std::exchange(ret_, TypeP{});
        if (ir::is<ir::Block>(l->body)) {
            auto saved = labels_;
            l->body = stmt_node(l->body);
            labels_ = std::move(saved);
        } else {
            l->body = expr(l->body);
        }
        ret_ = saved_ret;
        return x;
    }
    if (auto* c = ir::get<ir::Cond>(x)) {
        c->cond = expr(c->cond);
        c->then = to(c->then, x->type);
        c->orelse = to(c->orelse, x->type);
        return x;
    }
    if (auto* a = ir::get<ir::NewArray>(x)) {
        for (Node*& d : a->dims) d = expr(d);
        for (Node*& item : a->init) item = to(item, a->elem);
        return x;
    }
    if (ir::is<ir::SwitchExpr>(x)) {
        auto& sw = *ir::get<ir::SwitchExpr>(x);
        sw.expr = expr(sw.expr);
        for (Node* cn : sw.cases) {
            auto* c = ir::get<ir::Case>(cn);
            for (Node*& l : c->labels) {
                if (!ir::is<ir::Select>(strip(l))) l = expr(l);
            }
            for (Node* b : c->body) {
                if (ir::is<ir::Block>(b)) throw Declined{"switch-expression", "a switch-expression arm with statements"};
            }
            stmts(c->body);
        }
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
        // the types last: the rules above read Java's
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

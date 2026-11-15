// The C# -> C++17 back end, expressions and the library's calls.
#include <algorithm>
#include <charconv>
#include <format>
#include <limits>

#include "cb/core/text.hpp"
#include "cb/translate/backends/csharp_cpp17_internal.hpp"

namespace cb::translate::backends::csharp_cpp17::detail {

namespace {

bool ce_has(const std::vector<std::string>& v, std::string_view s) { return std::ranges::find(v, s) != v.end(); }

// What an expression does besides computing a value: it writes a variable (assignment, ++, --) or calls something.
struct CeEffects {
    bool writes = false, calls = false;
};
CeEffects ce_effects_of(Node* n) {
    CeEffects e;
    for (Node* x : ir::walk(n)) {
        if (ir::is<ir::Assign>(x)) e.writes = true;
        if (const auto* u = ir::get<ir::Unary>(x); u != nullptr && (u->op == "++" || u->op == "--")) e.writes = true;
        if (ir::is<ir::Call>(x) || ir::is<ir::New>(x)) e.calls = true;
        if (ir::is<ir::Await>(x)) e.calls = e.writes = true;   // (an await runs other code before it gives its value)
    }
    return e;
}

// Whether the order two operands are evaluated in can be seen: one writes what the other may read, or both call something.
bool ce_effects(Node* a, Node* b) {
    const CeEffects x = ce_effects_of(a), y = ce_effects_of(b);
    const auto literal = [](Node* n) { return ir::is<ir::Lit>(n); };
    return (x.writes && !literal(b)) || (y.writes && !literal(a)) || (x.calls && y.calls);
}

std::string ce_simple(const std::string& qname) {
    const std::size_t dot = qname.rfind('.');
    return dot == std::string::npos ? qname : qname.substr(dot + 1);
}

std::string ce_float_text(float v) {
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, v);
    std::string s(buf, r.ptr);
    if (s.find_first_of(".eE") == std::string::npos) s += ".0";
    return s + "f";
}

std::string ce_double_text(double v) {
    if (v != v) return "std::numeric_limits<double>::quiet_NaN()";
    if (v == std::numeric_limits<double>::infinity()) return "std::numeric_limits<double>::infinity()";
    if (v == -std::numeric_limits<double>::infinity()) return "(-std::numeric_limits<double>::infinity())";
    char buf[64];
    const auto r = std::to_chars(buf, buf + sizeof buf, v);
    std::string s(buf, r.ptr);
    if (s.find_first_of(".eE") == std::string::npos) s += ".0";
    return v < 0 ? "(" + s + ")" : s;
}

}  // namespace

// ------------------------------------------------------------------------------------------------------------------- small pieces

std::string Writer::bare(const std::string& e) {
    if (e.size() < 2 || e.front() != '(' || e.back() != ')') return e;
    int depth = 0;
    for (std::size_t i = 0; i < e.size(); ++i) {
        if (e[i] == '(') ++depth;
        else if (e[i] == ')') {
            --depth;
            if (depth == 0 && i + 1 < e.size()) return e;   // the first paren closes before the end: not one pair around the whole
        }
    }
    return e.substr(1, e.size() - 2);
}

std::string Writer::local_name(const ir::SymP& s) const {
    const std::string n = ident(s->name);
    return boxed_.contains(sym_key(*s)) ? "(*" + n + ")" : n;
}

std::string Writer::null_of(const TypeP& t) const {
    if (is_nullable(t)) return "std::nullopt";
    if (is_string(t)) return "cs::String()";
    if (!t || !t->is_prim()) {
        if (is_function(t)) return "nullptr";
        if (is_user_value(t)) return "{}";
    }
    return "nullptr";
}

std::string Writer::literal(const ir::Lit& l, const TypeP& t) {
    return std::visit(
        [&](const auto& v) -> std::string {
            using V = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<V, std::monostate>) {
                return null_of(t);
            } else if constexpr (std::is_same_v<V, bool>) {
                return v ? "true" : "false";
            } else if constexpr (std::is_same_v<V, std::string>) {
                const auto* cp = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
                if (cp != nullptr && cp->prim == ir::Prim::U16) {   // a char the front end spelled as text
                    const std::string lit = u16_literal_text(v);
                    return "static_cast<char16_t>(" + lit + "[0])";
                }
                return "cs::String(" + u16_literal_text(v) + ")";
            } else if constexpr (std::is_same_v<V, double>) {
                const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
                if (p != nullptr && p->prim == ir::Prim::F32) return ce_float_text(static_cast<float>(v));
                return ce_double_text(v);
            } else if constexpr (std::is_same_v<V, std::uint64_t>) {
                return std::to_string(v) + "ull";
            } else {
                const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
                if (p == nullptr) {
                    if (is_user_value(t)) return "static_cast<" + type(t) + ">(" + std::to_string(v) + ")";
                    return std::to_string(v);
                }
                switch (p->prim) {
                    case ir::Prim::I64:
                        return v == std::numeric_limits<std::int64_t>::min() ? "(-9223372036854775807LL - 1)" : std::to_string(v) + "LL";
                    case ir::Prim::U64: return std::to_string(v) + "ull";
                    case ir::Prim::U32: return std::to_string(v) + "u";
                    case ir::Prim::F32: return ce_float_text(static_cast<float>(v));
                    case ir::Prim::F64: return ce_double_text(static_cast<double>(v));
                    case ir::Prim::I32:
                        return v == std::numeric_limits<std::int32_t>::min() ? "(-2147483647 - 1)" : std::to_string(v);
                    case ir::Prim::U16:
                        if (p->encoding && *p->encoding == ir::CharEncoding::Utf16) {
                            if (v >= 0x20 && v < 0x7F && v != '\'' && v != '\\') return std::string("u'") + static_cast<char>(v) + "'";
                            return "static_cast<char16_t>(" + std::to_string(v) + ")";
                        }
                        return "static_cast<" + prim_name(*p) + ">(" + std::to_string(v) + ")";
                    case ir::Prim::Bool: return v != 0 ? "true" : "false";
                    default: return "static_cast<" + prim_name(*p) + ">(" + std::to_string(v) + ")";
                }
            }
        },
        l.value);
}

// An expression as a value of `target`: a null typed, a value boxed where an object is wanted.
std::string Writer::conv(Node* e, const TypeP& target) {
    if (const auto* l = ir::get<ir::Lit>(e); l != nullptr && std::holds_alternative<std::monostate>(l->value)) return null_of(target ? target : e->type);
    std::string text = expr(e);
    if (!target || !e->type) return text;
    if (is_object_like(target) && is_value(e->type) && !is_function(e->type)) return "cs::box(" + bare(text) + ")";
    return text;
}

// The member access operator for an object: `x->` for a reference, `x.` for a value.
std::string Writer::receiver(Node* target) {
    if (ir::is<ir::This>(target) || ir::is<ir::Super>(target)) return "this->";
    return expr(target) + (is_value(target->type) ? "." : "->");   // (every compound expression comes back parenthesised)
}

std::string Writer::args_text(const std::vector<Node*>& args, const ir::SymP& sym) {
    std::string out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        const TypeP param = sym && i < sym->params.size() ? sym->params[i] : TypeP{};
        const bool by_ref = sym && i < sym->refs.size() && sym->refs[i] != ir::PassMode::ByValue && sym->refs[i] != ir::PassMode::In;
        out += (i ? ", " : "") + bare(by_ref ? expr(args[i]) : conv(args[i], param));
    }
    return out;
}

// ------------------------------------------------------------------------------------------------------------------- dispatch

std::string Writer::expr(Node* e) {
    if (e == nullptr) decline("expression", "a missing expression");
    return ir::visit(*e, [&](const auto& k) -> std::string {
        using K = std::decay_t<decltype(k)>;
        if constexpr (std::is_same_v<K, ir::Lit>) {
            return literal(k, e->type);
        } else if constexpr (std::is_same_v<K, ir::Name>) {
            const ir::SymP& s = k.sym;
            if (s->kind == ir::SymKind::Field || s->kind == ir::SymKind::Property) {
                const std::string owner = s->owner ? cxx_class(*s->owner) : "";
                if (s->is_static) return owner + "::" + (s->kind == ir::SymKind::Property ? "get_" + s->name + "()" : ident(s->name));
                return s->kind == ir::SymKind::Property ? "this->get_" + s->name + "()" : "this->" + ident(s->name);
            }
            return local_name(s);
        } else if constexpr (std::is_same_v<K, ir::Select>) {
            return select(k, e);
        } else if constexpr (std::is_same_v<K, ir::This>) {
            if (is_user_value(e->type)) return "(*this)";
            return "cs::self(this)";
        } else if constexpr (std::is_same_v<K, ir::Call>) {
            return call(k, e);
        } else if constexpr (std::is_same_v<K, ir::New>) {
            return new_expr(k, e);
        } else if constexpr (std::is_same_v<K, ir::NewArray>) {
            return new_array(k, e);
        } else if constexpr (std::is_same_v<K, ir::Index>) {
            return index_expr(k);
        } else if constexpr (std::is_same_v<K, ir::Length>) {
            return receiver(k.arr) + "Length()";
        } else if constexpr (std::is_same_v<K, ir::Unary>) {
            return unary(k, e);
        } else if constexpr (std::is_same_v<K, ir::Binary>) {
            return binary(k, e);
        } else if constexpr (std::is_same_v<K, ir::Concat>) {
            std::string out = "(cs::Cat()";
            for (std::size_t i = 0; i < k.parts.size(); ++i) out += " << " + expr(k.parts[i]);
            return out + ").done()";
        } else if constexpr (std::is_same_v<K, ir::Assign>) {
            return assign(k, e);
        } else if constexpr (std::is_same_v<K, ir::Cond>) {
            const bool wrap = e->type && !e->type->is_prim();
            const std::string t = wrap ? type(e->type) : "";
            const auto branch = [&](Node* n) { return wrap ? t + "(" + bare(conv(n, e->type)) + ")" : conv(n, e->type); };
            return "(" + bare(expr(k.cond)) + " ? " + branch(k.then) + " : " + branch(k.orelse) + ")";
        } else if constexpr (std::is_same_v<K, ir::Cast>) {
            return cast(k, e);
        } else if constexpr (std::is_same_v<K, ir::InstanceOf>) {
            return instance_of(k);
        } else if constexpr (std::is_same_v<K, ir::Await>) {
            return "cs::await(" + bare(expr(k.x)) + ")";
        } else if constexpr (std::is_same_v<K, ir::Lambda>) {
            return lambda(k, e);
        } else if constexpr (std::is_same_v<K, ir::MethodRef>) {
            if (k.mode.value_or("invoke") != "invoke") decline("method-group", "a constructor reference");
            std::string callee;
            if (k.sym->is_static || k.target == nullptr || ir::is<ir::TypeName>(k.target)) {
                callee = (k.sym->owner ? cxx_class(*k.sym->owner) : "") + "::" + ident(k.sym->name);
                if (k.target != nullptr && ir::is<ir::TypeName>(k.target)) {
                    if (const auto* ct = class_of(k.target->type)) callee = class_text(*ct) + "::" + ident(k.sym->name);
                }
            } else {
                callee = receiver(k.target) + ident(k.sym->name);
            }
            return "[=](auto&&... a) -> decltype(auto) { return " + callee + "(std::forward<decltype(a)>(a)...); }";
        } else if constexpr (std::is_same_v<K, ir::SwitchExpr>) {
            return switch_expr(k, e);
        } else if constexpr (std::is_same_v<K, ir::Paren>) {
            return "(" + bare(expr(k.x)) + ")";
        } else if constexpr (std::is_same_v<K, ir::Unsupported>) {
            decline(k.kind.value_or("unsupported"), k.raw.value_or("a construct the front end did not lower"));
        } else {
            decline(std::string(ir::kind_name(*e)), "an expression of this kind has no C++17 form yet");
        }
    });
}

// ------------------------------------------------------------------------------------------------------------------- members

std::string Writer::select(const ir::Select& s, Node* n) {
    (void)n;
    const ir::SymP& sym = s.sym;
    const std::string owner = sym->owner.value_or("");
    const bool system = owner.starts_with("System.") && !info(owner);
    if (ir::is<ir::TypeName>(s.obj)) {
        if (system) return "cs::sys::" + ce_simple(owner) + "::" + sym->name;
        const auto* ct = class_of(s.obj->type);
        const std::string cls = ct != nullptr ? class_text(*ct) : cxx_class(owner);
        if (sym->kind == ir::SymKind::Property) return cls + "::get_" + (sym->name == "this[]" ? "Item" : sym->name) + "()";
        return cls + "::" + ident(sym->name);
    }
    const std::string recv = receiver(s.obj);
    if (sym->kind == ir::SymKind::Property) {
        if (system) {
            if (owner == "System.Nullable") return sym->name == "HasValue" ? recv + "has_value()" : "cs::value_of(" + bare(expr(s.obj)) + ")";
            return recv + sym->name + "()";
        }
        return recv + "get_" + sym->name + "()";
    }
    if (sym->kind == ir::SymKind::Event) decline("event", "an event");
    if (system && sym->kind == ir::SymKind::Field) return recv + ident(sym->name);
    return recv + ident(sym->name);
}

std::string Writer::index_expr(const ir::Index& i) {
    const TypeP at = i.arr->type;
    std::vector<Node*> idx = ir::statements(i.index);
    const auto* arr = at ? std::get_if<ir::ArrayType>(&at->body) : nullptr;
    if (arr != nullptr && !i.sym) {
        if (arr->rank == 1) return "(*" + expr(i.arr) + ")[" + bare(expr(idx.at(0))) + "]";
        if (arr->rank == 2) return "(*" + expr(i.arr) + ")(" + bare(expr(idx.at(0))) + ", " + bare(expr(idx.at(1))) + ")";
        decline("array", "an array of more than two dimensions");
    }
    std::string args;
    for (std::size_t k = 0; k < idx.size(); ++k) args += (k ? ", " : "") + bare(expr(idx[k]));
    return receiver(i.arr) + "get_Item(" + args + ")";
}

std::string Writer::new_array(const ir::NewArray& a, Node* n) {
    const auto* at = n->type ? std::get_if<ir::ArrayType>(&n->type->body) : nullptr;
    const std::size_t rank = at != nullptr ? at->rank : 1;
    const std::string elem = type(at != nullptr ? at->elem : a.elem);   // (the node's own type says what a jagged array holds)
    if (!a.init.empty()) {
        if (rank != 1) decline("array", "an initialiser of a multi-dimensional array");
        std::string items;
        for (std::size_t i = 0; i < a.init.size(); ++i) items += (i ? ", " : "") + bare(conv(a.init[i], a.elem));
        return "cs::array_of<" + elem + ">({" + items + "})";
    }
    if (a.dims.size() == 1 && rank == 1) return "cs::make_array<" + elem + ">(" + bare(expr(a.dims[0])) + ")";
    if (a.dims.size() == 2 && rank == 2) return "cs::make_array2<" + elem + ">(" + bare(expr(a.dims[0])) + ", " + bare(expr(a.dims[1])) + ")";
    if (a.dims.empty() && a.init.empty()) return "cs::make_array<" + elem + ">(0)";
    decline("array", "an array creation of this shape");
}

std::string Writer::new_expr(const ir::New& e, Node* n) {
    if (e.body != nullptr) decline("anonymous-class", "an anonymous class");
    if (e.outer != nullptr) decline("inner-class", "an instance of an inner class");
    const auto* ct = class_of(n->type);
    if (ct == nullptr) decline("new", "an object of a type that is not a class");
    if (const ClassInfo* k = info(ct->name); k != nullptr && k->is_delegate) return bare(expr(e.args.at(0)));
    if (ct->name == "System.Object") return "std::make_shared<cs::Object>()";
    const std::string args = args_text(e.args, e.sym);
    const std::string cls = class_text(*ct);
    if (is_user_value(n->type)) return cls + "(" + args + ")";
    if (is_string(n->type)) return "cs::String(" + args + ")";
    return "std::make_shared<" + cls + ">(" + args + ")";
}

// ------------------------------------------------------------------------------------------------------------------- calls

std::string Writer::call(const ir::Call& c, Node* n) {
    const ir::SymP& sym = c.sym;
    const std::string owner = sym->owner.value_or("");
    if (owner.starts_with("System") && !info(owner)) return library_call(c, n, owner);
    std::string tail;
    if (!c.typeargs.empty()) {
        tail = "<";
        for (std::size_t i = 0; i < c.typeargs.size(); ++i) {
            const TypeP* t = std::get_if<TypeP>(&c.typeargs[i]);
            if (t == nullptr) decline("call", "a type argument that is not a type");
            tail += (i ? ", " : "") + type(*t);
        }
        tail += ">";
    }
    const std::string name = ident(sym->name) + tail;
    const bool operator_call = ce_has(sym->flags, "operator");
    std::vector<Node*> args = c.args;
    Node* target = c.target;
    if (ce_has(sym->flags, "extension") && target != nullptr && !ir::is<ir::TypeName>(target) && args.size() + 1 == sym->params.size()) {
        args.insert(args.begin(), target);   // a static method called as if on its first argument
        target = nullptr;
    }
    const std::string arg_list = args_text(args, sym);
    if (sym->is_static || operator_call) {
        std::string cls;
        if (target != nullptr && ir::is<ir::TypeName>(target)) {
            const auto* ct = class_of(target->type);
            cls = ct != nullptr ? class_text(*ct) : cxx_class(owner);
        } else {
            cls = cxx_class(owner);
        }
        return cls + "::" + name + "(" + arg_list + ")";
    }
    if (sym->name == "Invoke" && target != nullptr && is_function(target->type)) return expr(target) + "(" + arg_list + ")";
    if (target == nullptr) return "this->" + name + "(" + arg_list + ")";
    if (ir::is<ir::Super>(target)) return cxx_class(owner) + "::" + name + "(" + arg_list + ")";
    return receiver(target) + (tail.empty() ? "" : "template ") + name + "(" + arg_list + ")";
}

std::string Writer::library_call(const ir::Call& c, Node* n, const std::string& owner) {
    (void)n;
    const ir::SymP& sym = c.sym;
    const std::string cls = ce_simple(owner);
    const std::string name = sym->name;
    Node* target = c.target;
    std::vector<Node*> args = c.args;
    const bool static_call = sym->is_static;
    if (ce_has(sym->flags, "extension") && target != nullptr && !ir::is<ir::TypeName>(target) && args.size() + 1 == sym->params.size()) {
        args.insert(args.begin(), target);
        target = nullptr;
    }
    std::string list;
    for (std::size_t i = 0; i < args.size(); ++i) list += (i ? ", " : "") + bare(conv(args[i], TypeP{}));
    if (static_call) {
        std::string holder = cls;
        if (owner == "System.String") holder = "StringStatics";
        if (owner == "System.Array") holder = "ArrayStatics";
        if (name == "op_Equality" && args.size() == 2) return "(" + bare(expr(args[0])) + " == " + bare(expr(args[1])) + ")";
        if (name == "op_Inequality" && args.size() == 2) return "(" + bare(expr(args[0])) + " != " + bare(expr(args[1])) + ")";
        return "cs::sys::" + holder + "::" + name + "(" + list + ")";
    }
    if (target == nullptr) decline("library", "a call of " + owner + "." + name + " without a receiver");
    // members every type has, on a reference, a value or a type variable
    if (owner == "System.Object" || owner == "System.ValueType" || owner == "System.Enum" || owner == "System.IComparable" || is_numeric(target->type) ||
        (target->type && target->type->is_prim())) {
        if (name == "ToString") return args.empty() ? "cs::str(" + bare(expr(target)) + ")" : "cs::to_string(" + bare(expr(target)) + ", " + list + ")";
        if (name == "CompareTo") return "cs::compare_to(" + bare(expr(target)) + ", " + list + ")";
        if (name == "GetType") return "cs::box(" + bare(expr(target)) + ")->GetType()";
        if (name == "Equals") return "(" + bare(expr(target)) + " == " + list + ")";
        if (name == "GetHashCode") return "static_cast<int>(std::hash<" + type(target->type) + ">()(" + bare(expr(target)) + "))";
    }
    if (owner == "System.IComparable" || name == "CompareTo") return "cs::compare_to(" + bare(expr(target)) + ", " + list + ")";
    if (name == "Invoke" && is_function(target->type)) return expr(target) + "(" + list + ")";
    if (owner == "System.Nullable" && name == "GetValueOrDefault") {
        const std::string inner = type(std::get<ir::ClassType>(target->type->body).args.at(0));
        return receiver(target) + "value_or(" + (args.empty() ? inner + "{}" : bare(conv(args[0], std::get<ir::ClassType>(target->type->body).args.at(0)))) + ")";
    }
    if (owner == "System.String" && name == "ToString") return expr(target);
    if (name == "Equals" && args.size() == 1) return receiver(target) + "Equals(" + bare(conv(args[0], ir::class_type("System.Object"))) + ")";
    return receiver(target) + name + "(" + list + ")";
}

// ------------------------------------------------------------------------------------------------------------------- operators

// The operand as a value of the promoted type where C++'s own conversions would differ from C#'s (an unsigned mixed with a signed).
std::string Writer::operand(Node* n, const std::string& pt) {
    const std::string text = expr(n);
    if (pt != "long long") return text;
    const auto* p = n->type ? std::get_if<ir::PrimType>(&n->type->body) : nullptr;
    if (p != nullptr && p->prim == ir::Prim::U32) return "static_cast<long long>(" + bare(text) + ")";
    return text;
}

std::string Writer::binop(const std::string& op, const std::string& l, const std::string& r, const std::string& pt, bool is_float, bool checked) {
    if (op == "<<") return "cs::shl<" + pt + ">(" + bare(l) + ", " + bare(r) + ")";
    if (op == ">>") return "cs::shr<" + pt + ">(" + bare(l) + ", " + bare(r) + ")";
    if (op == ">>>") return "cs::ushr<" + pt + ">(" + bare(l) + ", " + bare(r) + ")";
    if (op == "/") return is_float ? "(" + l + " / " + r + ")" : "cs::div<" + pt + ">(" + bare(l) + ", " + bare(r) + ")";
    if (op == "%") return "cs::rem<" + pt + ">(" + bare(l) + ", " + bare(r) + ")";
    if (checked && (op == "+" || op == "-" || op == "*")) {
        return std::string("cs::checked_") + (op == "+" ? "add" : op == "-" ? "sub" : "mul") + "<" + pt + ">(" + bare(l) + ", " + bare(r) + ")";
    }
    return "(" + l + " " + op + " " + r + ")";
}

std::string Writer::binary(const ir::Binary& b, Node* n) {
    const std::string& op = b.op;
    if (op == "&&" || op == "||") {
        // (an operand the source parenthesised keeps its parentheses: `(a || b) && c` is not `a || b && c`)
        const auto side = [&](Node* x) { return ir::is<ir::Paren>(x) ? expr(x) : bare(expr(x)); };
        return "(" + side(b.left) + " " + op + " " + side(b.right) + ")";
    }
    if (op == "??") {
        return "cs::coalesce(" + bare(expr(b.left)) + ", [&] { return " + bare(conv(b.right, n->type)) + "; })";
    }
    const auto is_null_lit = [&](Node* x) {
        const auto* l = ir::get<ir::Lit>(x);
        return l != nullptr && std::holds_alternative<std::monostate>(l->value);
    };
    if (op == "==" || op == "!=") {
        if (is_null_lit(b.right)) return std::string(op == "!=" ? "(!" : "(") + "cs::is_null(" + bare(expr(b.left)) + "))";
        if (is_null_lit(b.left)) return std::string(op == "!=" ? "(!" : "(") + "cs::is_null(" + bare(expr(b.right)) + "))";
    }
    const std::string pt = promoted(b.left->type, b.right->type);
    const bool numeric = !pt.empty();
    std::string l = numeric ? operand(b.left, pt) : expr(b.left);
    std::string r = numeric ? operand(b.right, pt) : expr(b.right);
    // C# evaluates the left operand first; C++ leaves it open: two operands that both do something are evaluated in order
    if (ce_effects(b.left, b.right)) {
        const std::string lt = type(b.left->type), rt = type(b.right->type);
        const std::string lo = l, ro = r;
        l = "cs_a";
        r = "cs_b";
        Node* saved_left = b.left;
        (void)saved_left;
        const std::string inner = binary_core(b, n, pt, l, r);
        return "cs::LR<" + lt + ", " + rt + ">{" + bare(lo) + ", " + bare(ro) + "}.then([&](" + lt + " cs_a, " + rt + " cs_b) { return " + bare(inner) + "; })";
    }
    return binary_core(b, n, pt, l, r);
}

std::string Writer::binary_core(const ir::Binary& b, Node* n, const std::string& pt, const std::string& l, const std::string& r) {
    const std::string& op = b.op;
    if (op == "+" && (is_string(b.left->type) || is_string(b.right->type))) return "cs::cat(" + bare(l) + ", " + bare(r) + ")";
    const bool checked = b.overflow && *b.overflow == ir::Overflow::Trap;
    if (op == "<<" || op == ">>" || op == ">>>") {
        const std::string shift_type = promoted(b.left->type, ir::i32_type());
        return binop(op, l, r, shift_type.empty() ? type(n->type) : shift_type, false, false);
    }
    const bool is_float = pt == "double" || pt == "float";
    const std::string result = pt.empty() ? (n->type ? type(n->type) : "int") : pt;
    if (op == "/" || op == "%" || (checked && (op == "+" || op == "-" || op == "*"))) return binop(op, l, r, result, is_float, checked);
    return "(" + l + " " + op + " " + r + ")";
}

std::string Writer::unary(const ir::Unary& u, Node* n) {
    (void)n;
    if (u.op == "++" || u.op == "--") {
        Node* t = u.x;
        const bool special = [&] {
            if (const auto* s = ir::get<ir::Select>(t); s != nullptr && s->sym->kind == ir::SymKind::Property) return true;
            if (const auto* i = ir::get<ir::Index>(t); i != nullptr && i->sym) return true;
            return false;
        }();
        if (!special) return u.postfix ? "(" + expr(t) + u.op + ")" : "(" + u.op + expr(t) + ")";
        // a property or indexer: read, add, write back (the value is the old one for a postfix form)
        ir::Assign as;
        as.op = u.op.substr(0, 1);
        as.target = t;
        Node one;
        one.type = ir::i32_type();
        one.kind = ir::Lit{.value = std::int64_t{1}};
        as.value = &one;
        if (u.postfix) return "[&] { auto cs_old = " + bare(expr(t)) + "; " + bare(assign(as, &one)) + "; return cs_old; }()";
        return assign(as, &one);
    }
    const std::string x = expr(u.x);
    if (u.op == "-" ) return "(-" + x + ")";
    if (u.op == "+") return x;
    if (u.op == "!") return "(!" + x + ")";
    if (u.op == "~") return "(~" + x + ")";
    decline("unary", "the operator " + u.op);
}

std::string Writer::assign(const ir::Assign& a, Node* n) {
    (void)n;
    Node* t = a.target;
    // what the target is: a plain lvalue, or a property / indexer with a getter and a setter
    std::string lvalue, getter;
    std::function<std::string(const std::string&)> setter;
    if (const auto* s = ir::get<ir::Select>(t); s != nullptr && s->sym->kind == ir::SymKind::Property) {
        const std::string owner = s->sym->owner.value_or("");
        const bool system = owner.starts_with("System.") && !info(owner);
        const auto pi = prop_of(s->sym);
        const std::string recv = ir::is<ir::TypeName>(s->obj) ? (class_of(s->obj->type) ? class_text(*class_of(s->obj->type)) + "::" : "") : receiver(s->obj);
        if (pi && !pi->has_set && pi->is_auto) {
            lvalue = recv + "bk_" + s->sym->name;   // a get-only property is written in the constructor: its field
        } else if (system) {
            getter = recv + s->sym->name + "()";
            setter = [recv, name = s->sym->name](const std::string& v) { return recv + "set_" + name + "(" + v + ")"; };
        } else {
            getter = recv + "get_" + s->sym->name + "()";
            setter = [recv, name = s->sym->name](const std::string& v) { return recv + "set_" + name + "(" + v + ")"; };
        }
    } else if (const auto* i = ir::get<ir::Index>(t); i != nullptr && i->sym) {
        const std::string recv = receiver(i->arr);
        std::string idx;
        const std::vector<Node*> list = ir::statements(i->index);
        for (std::size_t k = 0; k < list.size(); ++k) idx += (k ? ", " : "") + bare(expr(list[k]));
        getter = recv + "get_Item(" + idx + ")";
        setter = [recv, idx](const std::string& v) { return recv + "set_Item(" + idx + ", " + v + ")"; };
    } else {
        lvalue = bare(expr(t));
    }
    const bool plain = !setter;
    const std::string current = plain ? lvalue : getter;
    const std::string target_type = t->type ? type(t->type) : "auto";
    std::string value_text;
    if (!a.op) {
        value_text = bare(conv(a.value, t->type));
    } else {
        const std::string& op = *a.op;
        const bool checked = a.overflow && *a.overflow == ir::Overflow::Trap;
        if (op == "??") {
            value_text = "cs::coalesce(" + current + ", [&] { return " + bare(conv(a.value, t->type)) + "; })";
        } else if (is_string(t->type) && op == "+") {
            value_text = "cs::cat(" + current + ", " + bare(expr(a.value)) + ")";
        } else {
            const std::string pt = promoted(t->type, a.value->type);
            const bool shift = op == "<<" || op == ">>" || op == ">>>";
            const std::string use = shift ? promoted(t->type, ir::i32_type()) : pt;
            const bool is_float = pt == "double" || pt == "float";
            const std::string rhs = operand(a.value, pt);
            const bool simple_op = (op == "+" || op == "-" || op == "*" || op == "&" || op == "|" || op == "^") && !checked;
            if (plain && simple_op && !is_float) return "(" + lvalue + " " + op + "= " + bare(rhs) + ")";
            if (plain && simple_op && is_float) return "(" + lvalue + " " + op + "= " + bare(rhs) + ")";
            const std::string computed = binop(op, current, rhs, use.empty() ? target_type : use, is_float, checked);
            value_text = is_integral(t->type) || (t->type && t->type->is_prim()) ? "static_cast<" + target_type + ">(" + bare(computed) + ")" : computed;
        }
    }
    if (plain) return "(" + lvalue + " = " + value_text + ")";
    return setter(value_text);
}

std::string Writer::cast(const ir::Cast& c, Node* n) {
    const TypeP& to = n->type;
    const TypeP& from = c.x->type;
    const bool trap = c.overflow && *c.overflow == ir::Overflow::Trap;
    const std::string x = bare(expr(c.x));
    if (is_numeric(to)) {
        if (is_numeric(from)) {
            if (c.implicit && !trap) return "static_cast<" + type(to) + ">(" + x + ")";
            if (is_floating(from) && is_integral(to)) return std::string(trap ? "cs::checked_cast<" : "cs::to_int<") + type(to) + ">(" + x + ")";
            if (trap && is_integral(from) && is_integral(to)) return "cs::checked_cast<" + type(to) + ">(" + x + ")";
            return "static_cast<" + type(to) + ">(" + x + ")";
        }
        if (is_user_value(from)) return "static_cast<" + type(to) + ">(" + x + ")";
        if (is_object_like(from)) return "cs::unbox<" + type(to) + ">(" + x + ")";
        if (is_nullable(from)) return "cs::value_of(" + x + ")";
    }
    if (is_user_value(to)) {
        if (is_numeric(from) || is_user_value(from)) return "static_cast<" + type(to) + ">(" + x + ")";
        if (is_object_like(from)) return "cs::unbox<" + type(to) + ">(" + x + ")";
    }
    if (is_nullable(to)) {
        if (is_nullable(from)) return x;
        if (is_object_like(from)) return "cs::unbox<" + type(to) + ">(" + x + ")";
        return type(to) + "(" + x + ")";
    }
    if (is_string(to)) {
        if (is_string(from)) return x;
        if (is_object_like(from)) return "cs::unbox<cs::String>(" + x + ")";
    }
    if (const auto* tc = class_of(to); tc != nullptr && !is_value(to)) {
        if (from && is_value(from)) return "cs::box(" + x + ")";
        if (c.implicit) return x;
        return "cs::cast<" + class_text(*tc) + ">(" + x + ")";
    }
    if (c.implicit) return x;
    if (to && std::holds_alternative<ir::TypeVarType>(to->body)) return "static_cast<" + type(to) + ">(" + x + ")";
    if (is_function(to)) return x;
    decline("cast", "a conversion from " + (from ? ir::to_text(*from) : "?") + " to " + (to ? ir::to_text(*to) : "?"));
}

std::string Writer::instance_of(const ir::InstanceOf& t) {
    const TypeP& tt = t.test;
    std::string test_type;
    if (const auto* cls = class_of(tt); cls != nullptr && !is_value(tt)) test_type = "cs::Ref<" + class_text(*cls) + ">";
    else test_type = type(tt);
    const std::string x = bare(expr(t.x));
    if (t.binding == nullptr) return "cs::is<" + test_type + ">(" + x + ")";
    const auto* b = ir::get<ir::Local>(t.binding);
    return "cs::is_as(" + x + ", " + local_name(b->sym) + ")";
}

std::string Writer::lambda(const ir::Lambda& l, Node* n) {
    const TypeP result = function_result(n->type);
    std::string params;
    for (std::size_t i = 0; i < l.params.size(); ++i) {
        const auto* p = ir::get<ir::Param>(l.params[i]);
        params += (i ? ", " : "") + type(p->type) + " " + ident(p->sym->name);
    }
    std::string head = "[=](" + params + ")";
    if (result) head += " -> " + type(result);
    returns_.push_back(result);
    std::string text;
    try {
        if (const auto* blk = ir::get<ir::Block>(l.body)) {
            collect_captures(l.body);
            text = capture([&] {
                line(head + " {");
                ++indent_;
                for (Node* s : blk->stmts) write_hoists(s);
                stmts(blk->stmts);
                --indent_;
                line("}");
            });
        } else {
            collect_captures(l.body);
            std::vector<Node*> hoists;
            collect_hoists(l.body, hoists);
            const bool void_result = !result || std::holds_alternative<ir::VoidType>(result->body);
            const std::string value = void_result ? bare(expr(l.body)) : bare(conv(l.body, result));
            std::string decls;
            for (Node* h : hoists) {
                const auto* hl = ir::get<ir::Local>(h);
                if (hoisted_.contains(sym_key(*hl->sym))) continue;
                hoisted_.insert(sym_key(*hl->sym));
                decls += var_decl(hl->sym, hl->type, "", false) + " ";
            }
            text = head + " { " + decls + (void_result ? value + ";" : "return " + value + ";") + " }";
        }
    } catch (...) {
        returns_.pop_back();
        throw;
    }
    returns_.pop_back();
    return text;
}

std::string Writer::switch_expr(const ir::SwitchExpr& s, Node* n) {
    returns_.push_back(n->type);
    std::string text;
    try {
        text = capture([&] {
            line("[&]() -> " + type(n->type) + " {");
            ++indent_;
            switch_cases(s.expr, s.cases);
            line("cs::raise<cs::InvalidOperationException>();");
            --indent_;
            line("}()");
        });
    } catch (...) {
        returns_.pop_back();
        throw;
    }
    returns_.pop_back();
    return text;
}

}  // namespace cb::translate::backends::csharp_cpp17::detail

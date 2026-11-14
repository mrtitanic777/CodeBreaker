#include "cb/translate/frontends/clang/j2x.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <utility>

namespace cb::translate::frontends::clang::j2x {

namespace ir = cb::translate::ir;
using ir::Node;
using ir::SymP;
using ir::TypeP;

namespace {

// ---- the Java library's names ---------------------------------------------------------------------------------------------

const TypeP& j2x_string() {
    static const TypeP t = ir::class_type("java.lang.String");
    return t;
}
const TypeP& j2x_system() {
    static const TypeP t = ir::class_type("java.lang.System");
    return t;
}
const TypeP& j2x_printstream() {
    static const TypeP t = ir::class_type("java.io.PrintStream");
    return t;
}
const TypeP& j2x_math() {
    static const TypeP t = ir::class_type("java.lang.Math");
    return t;
}

// The runtime's names for java.lang.String (and the C++ strings the lowering of the runtime's own code uses).
bool j2x_is_string_class(const std::string& name) {
    return name == "JavaString" || name == "JCat" || name == "std.string" || name == "std.basic_string" || name == "StringBuilder";
}

// The runtime's saturating casts and Math helpers by the Java method they are.
const std::map<std::string, std::string>& j2x_math_names() {
    static const std::map<std::string, std::string> names = {
        {"javaMathSqrt", "sqrt"}, {"javaMathRound", "round"}, {"javaMathSignum", "signum"}, {"javaMathRandom", "random"}};
    return names;
}

bool j2x_is_array(const TypeP& t) { return t && std::holds_alternative<ir::ArrayType>(t->body); }
bool j2x_is_ref_pointer(const TypeP& t) {
    const auto* p = t ? std::get_if<ir::PtrType>(&t->body) : nullptr;
    return p != nullptr && p->ref;
}
bool j2x_is_integer_of_32_or_64_bits(const TypeP& t) {
    const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    return p != nullptr && (p->prim == ir::Prim::I32 || p->prim == ir::Prim::I64 || p->prim == ir::Prim::U32 || p->prim == ir::Prim::U64);
}
bool j2x_is_plain_char(const TypeP& t) {
    const auto* p = t ? std::get_if<ir::PrimType>(&t->body) : nullptr;
    return p != nullptr && p->prim == ir::Prim::Char;
}

// An expression with its parentheses taken off.
Node* j2x_strip(Node* e) {
    while (const auto* p = ir::get<ir::Paren>(e)) e = p->x;
    return e;
}

const ir::Unary* j2x_unary_op(const Node* e, std::string_view op) {
    const auto* u = ir::get<ir::Unary>(e);
    return u != nullptr && u->op == op ? u : nullptr;
}

const ir::Binary* j2x_comma(const Node* e) {
    const auto* b = ir::get<ir::Binary>(e);
    return b != nullptr && b->op == "," ? b : nullptr;
}

// An argument bound to a reference parameter (a `&` typed ref): the value it binds.
Node* j2x_value(Node* arg) {
    Node* x = j2x_strip(arg);
    if (const auto* u = j2x_unary_op(x, "&"); u != nullptr && j2x_is_ref_pointer(x->type)) return u->x;
    return arg;
}

}  // namespace

ir::TypeP lift_type(const ir::TypeP& type) {
    if (!type) return type;
    if (const auto* c = std::get_if<ir::ClassType>(&type->body)) {
        if (j2x_is_string_class(c->name)) return j2x_string();
        if (c->name == "JArray" && !c->args.empty()) return ir::array_of(lift_type(c->args[0]));
        if (c->name == "System._PrintStream") return j2x_printstream();
        ir::ClassType lifted = *c;
        bool changed = false;
        for (TypeP& a : lifted.args) {
            TypeP l = lift_type(a);
            changed = changed || l.get() != a.get();
            a = std::move(l);
        }
        return changed ? ir::make_type(std::move(lifted)) : type;
    }
    if (const auto* p = std::get_if<ir::PtrType>(&type->body)) {
        TypeP to = lift_type(p->to);
        if (j2x_is_array(to) || to == j2x_string()) return to;   // (a pointer to an array or String: the reference)
        if (to.get() == p->to.get()) return type;
        ir::PtrType lifted = *p;
        lifted.to = std::move(to);
        return ir::make_type(std::move(lifted));
    }
    if (const auto* a = std::get_if<ir::ArrayType>(&type->body)) {
        if (j2x_is_plain_char(a->elem)) return j2x_string();   // a string literal's char[N]
        TypeP elem = lift_type(a->elem);
        if (elem.get() == a->elem.get()) return type;
        ir::ArrayType lifted = *a;
        lifted.elem = std::move(elem);
        return ir::make_type(std::move(lifted));
    }
    return type;
}

namespace {

// ---- the pass ---------------------------------------------------------------------------------------------------------------

class J2xLift {
public:
    explicit J2xLift(Arena& arena) : arena_(arena) {}

    // A static field s_x is x again (the C++ back end's prefix), where no field x exists.
    void names(const ir::Unit& unit) {
        for (Node* t : unit.types) {
            const auto* c = ir::get<ir::ClassDecl>(t);
            if (c == nullptr) continue;
            std::set<std::string> fields;
            for (const Node* m : c->members) {
                if (const auto* f = ir::get<ir::FieldDecl>(m); f != nullptr && f->sym) fields.insert(f->sym->name);
            }
            for (const Node* m : c->members) {
                const auto* f = ir::get<ir::FieldDecl>(m);
                if (f == nullptr || !f->sym || std::ranges::find(f->mods, "static") == f->mods.end()) continue;
                if (f->sym->name.starts_with("s_") && !fields.contains(f->sym->name.substr(2)))
                    renamed_[{c->qname.value_or(""), f->sym->name}] = f->sym->name.substr(2);
            }
        }
    }

    void unit(Node* unit_node) {
        auto* unit = ir::get<ir::Unit>(unit_node);
        for (Node*& t : unit->types) t = lift(t);
        for (Node* t : unit->types) {
            auto* c = ir::get<ir::ClassDecl>(t);
            if (c == nullptr) continue;
            std::vector<Node*> members;
            for (Node* m : c->members) {
                if (auto* method = ir::get<ir::MethodDecl>(m)) {
                    if (method->kind == "dtor" && !has_statements(method->body)) continue;   // (Java has no destructor; an empty one says nothing)
                    if (method->name == "initStatics" && std::ranges::find(method->mods, "static") != method->mods.end()) method->kind = "static_init";
                }
                members.push_back(m);
            }
            c->members = std::move(members);
        }
    }

private:
    static bool has_statements(const ir::Suite& body) {
        for (const Node* s : ir::statements(body)) {
            if (const auto* b = ir::get<ir::Block>(s)) {
                if (!b->stmts.empty()) return true;
            } else {
                return true;
            }
        }
        return false;
    }

    // ---- types and symbols
    TypeP type(const TypeP& t) const { return lift_type(t); }
    void types(std::vector<TypeP>& list) const {
        for (TypeP& t : list) t = lift_type(t);
    }

    SymP sym(const SymP& s) {
        if (!s) return s;
        if (const auto it = lifted_syms_.find(s.get()); it != lifted_syms_.end()) return it->second;
        auto out = std::make_shared<ir::Sym>(*s);
        if (const auto it = renamed_.find({s->owner.value_or(""), s->name}); it != renamed_.end()) out->name = it->second;
        out->type = type(s->type);
        types(out->params);
        out->ret = type(s->ret);
        lifted_syms_[s.get()] = out;
        return out;
    }

    // ---- a node: its slots, its children, then what it is
    Node* lift(Node* n) {
        if (n == nullptr) return n;
        if (auto* block = ir::get<ir::Block>(n)) {
            for (Node*& s : block->stmts) s = lift(s);
            fold_temps(block->stmts);
            return n;
        }
        n->type = type(n->type);
        ir::visit(*n, [this](auto& k) { slots(k); });
        return rewrite(n);
    }

    template <class K> void slots(K& k) {
        if constexpr (requires { k.sym; }) {
            if constexpr (std::is_same_v<decltype(k.sym), SymP>) k.sym = sym(k.sym);
        }
        if constexpr (requires { k.type; }) {
            if constexpr (std::is_same_v<decltype(k.type), TypeP>) k.type = type(k.type);
        }
        if constexpr (requires { k.ret; }) k.ret = type(k.ret);
        if constexpr (requires { k.extends; }) k.extends = type(k.extends);
        if constexpr (requires { k.layout; }) k.layout = type(k.layout);
        if constexpr (requires { k.test; }) k.test = type(k.test);
        if constexpr (requires { k.of; }) k.of = type(k.of);
        if constexpr (requires { k.elem; }) k.elem = type(k.elem);
        if constexpr (requires { k.ctor; }) k.ctor = sym(k.ctor);
        if constexpr (requires { k.implements; }) types(k.implements);
        if constexpr (requires { k.throws; }) types(k.throws);
        if constexpr (std::is_same_v<K, ir::Catch>) types(k.types);
        if constexpr (std::is_same_v<K, ir::Call>) {
            for (ir::TypeArg& a : k.typeargs) {
                if (auto* t = std::get_if<TypeP>(&a)) *t = type(*t);
            }
        }
        if constexpr (std::is_same_v<K, ir::InstanceOf>) k.binding = lift(k.binding);
        k.each_child([this](Node*& child) { child = lift(child); });
    }

    Node* rewrite(Node* n) {
        if (auto* k = ir::get<ir::Lit>(n)) {
            if (std::holds_alternative<std::string>(k->value)) n->type = j2x_string();
            return n;
        }
        if (ir::is<ir::Cast>(n)) return x_cast(n);
        if (ir::is<ir::Select>(n)) return x_select(n);
        if (ir::is<ir::Name>(n)) return x_name(n);
        if (ir::is<ir::Unary>(n)) return x_unary(n);
        if (ir::is<ir::New>(n)) return x_new(n);
        if (ir::is<ir::Call>(n)) return x_call(n);
        return n;
    }

    template <class K> Node* make(K kind, TypeP t, const Node* at) { return ir::make(arena_, std::move(kind), std::move(t), at->line, at->col); }

    Node* x_cast(Node* e) {
        const auto* cast = ir::get<ir::Cast>(e);
        Node* x = j2x_strip(cast->x);
        if (x == nullptr) return e;
        const auto* lit = ir::get<ir::Lit>(x);
        const bool to_pointer = e->type && std::holds_alternative<ir::PtrType>(e->type->body);
        if (e->type == j2x_string() || (lit != nullptr && std::holds_alternative<std::string>(lit->value)) || (x->type == j2x_string() && to_pointer))
            return x;   // (a string's decay or conversion: the string)
        if (j2x_is_array(e->type) && j2x_is_array(x->type)) return x;
        return e;
    }

    Node* x_select(Node* e) {
        const auto* select = ir::get<ir::Select>(e);
        Node* obj = j2x_strip(select->obj);
        if (obj == nullptr || !select->sym) return e;
        if (select->sym->name == "_s" && (obj->type == j2x_string() || ir::is<ir::Concat>(obj))) return obj;
        if (select->sym->name == "length") {
            if (j2x_is_array(obj->type)) return make(ir::Length{.arr = obj}, ir::i32_type(), e);
            if (const auto* u = j2x_unary_op(obj, "*"); u != nullptr && u->x != nullptr && j2x_is_array(u->x->type))
                return make(ir::Length{.arr = u->x}, ir::i32_type(), e);
        }
        return e;
    }

    Node* x_name(Node* e) {
        const auto* name = ir::get<ir::Name>(e);
        const SymP& s = name->sym;
        if (s && s->kind == ir::SymKind::Global && s->owner == "System" && (s->name == "out" || s->name == "err")) {
            Node* type_name = ir::make(arena_, ir::TypeName{}, j2x_system());
            return make(ir::Select{.obj = type_name, .sym = ir::make_sym(ir::SymKind::Field, s->name, "java.lang.System", true, j2x_printstream())}, j2x_printstream(), e);
        }
        return e;
    }

    Node* x_unary(Node* e) {
        const auto* u = ir::get<ir::Unary>(e);
        if (u->x == nullptr || !u->x->type) return e;
        const bool array_or_string = j2x_is_array(u->x->type) || u->x->type == j2x_string();
        if (u->op == "*" && array_or_string) return u->x;   // (*arr / *str: the reference itself)
        if (u->op == "&" && j2x_is_ref_pointer(e->type) && array_or_string) return u->x;
        return e;
    }

    Node* x_new(Node* e) {
        auto* k = ir::get<ir::New>(e);
        const ir::PtrType* ptr = e->type ? std::get_if<ir::PtrType>(&e->type->body) : nullptr;
        const TypeP base = ptr != nullptr ? ptr->to : e->type;
        const std::string owner = k->sym ? k->sym->owner.value_or("") : "";
        if (j2x_is_string_class(owner) || base == j2x_string()) {
            if (k->args.empty()) return make(ir::Lit{.value = std::string()}, j2x_string(), e);
            if (k->args.size() == 1) {
                Node* a = j2x_strip(j2x_value(k->args[0]));
                if (a != nullptr && (a->type == j2x_string() || ir::is<ir::Concat>(a) || ir::is<ir::Lit>(a))) {
                    if (owner == "JCat") return make(ir::Concat{}, j2x_string(), e);
                    return a;
                }
            }
        }
        if (owner == "JCat" && k->args.empty()) return make(ir::Concat{}, j2x_string(), e);
        if (owner.starts_with("JArray") || j2x_is_array(base)) {
            if (j2x_is_array(base) && k->args.size() == 1 && j2x_is_integer_of_32_or_64_bits(k->args[0]->type)) {
                const auto& array = std::get<ir::ArrayType>(base->body);
                return make(ir::NewArray{.elem = array.elem, .dims = {k->args[0]}}, base, e);
            }
        }
        return e;
    }

    Node* x_call(Node* e) {
        auto* call = ir::get<ir::Call>(e);
        const SymP& s = call->sym;
        const std::string name = s ? s->name : "";
        std::vector<Node*> args;
        for (Node* a : call->args) args.push_back(j2x_value(a));   // (the runtime's helpers take const T&: the value)
        Node* target = call->target != nullptr ? j2x_strip(call->target) : nullptr;

        if (s && name == "operator+" && target != nullptr &&
            (s->owner == "JCat" || ir::is<ir::Concat>(target) || (target->type == j2x_string() && (!s->owner || s->owner == "JCat")))) {
            ir::Concat concat;
            if (const auto* t = ir::get<ir::Concat>(target)) concat.parts = t->parts;
            else concat.parts.push_back(target);
            if (!args.empty()) {
                Node* a = j2x_strip(args[0]);
                if (const auto* u = j2x_unary_op(a, "&")) a = j2x_strip(u->x);
                concat.parts.push_back(a);
            }
            return make(std::move(concat), j2x_string(), e);
        }
        if (name == "operator[]" && call->target != nullptr && j2x_is_array(call->target->type) && !args.empty())
            return make(ir::Index{.arr = call->target, .index = args[0]}, e->type, e);
        if (call->target == nullptr && !args.empty() && (name == "j2xToInt" || name == "j2xToLong"))
            return make(ir::Cast{.x = args[0]}, name == "j2xToInt" ? ir::i32_type() : ir::prim_type(ir::Prim::I64), e);
        if (call->target == nullptr && name == "j2xUshr" && args.size() >= 2) return make(ir::Binary{.op = ">>>", .left = args[0], .right = args[1]}, e->type, e);
        if (call->target == nullptr && s) {
            if (const auto it = j2x_math_names().find(name); it != j2x_math_names().end()) {
                SymP method = ir::make_sym(ir::SymKind::Method, it->second, "java.lang.Math", true);
                method->params = s->params;
                method->ret = s->ret;
                Node* type_name = ir::make(arena_, ir::TypeName{}, j2x_math());
                return make(ir::Call{.target = type_name, .sym = method, .args = args}, e->type, e);
            }
        }
        if ((name == "println" || name == "print") && call->target != nullptr && call->target->type == j2x_printstream()) {
            if (!args.empty()) {
                Node* a = j2x_strip(args[0]);
                if (const auto* u = j2x_unary_op(a, "&")) a = u->x;
                args = {a};
            }
            SymP method = ir::make_sym(ir::SymKind::Method, name, "java.io.PrintStream", false);
            for (const Node* a : args) method->params.push_back(a->type);
            method->ret = ir::void_type();
            return make(ir::Call{.target = call->target, .sym = method, .args = args}, ir::void_type(), e);
        }
        return e;
    }

    // ---- the sequencing temporaries
    // `int j2xo1 = 0; ...; S((j2xo1 = a, j2xo2 = b, X))` -> `int j2xo1 = a; ...; S(X)`.
    struct Comma {
        Node* node = nullptr;
        Node** slot = nullptr;   // where it (with the parentheses round it) sits; null when it is the statement itself
    };

    // The first comma expression in a statement, in the order of a walk, with the slot to put something in its place.
    static void find_comma(Node** slot, Node* node, std::optional<Comma>& found) {
        if (found || node == nullptr) return;
        if (const auto* p = ir::get<ir::Paren>(node)) {
            find_comma(slot, p->x, found);   // (the slot that holds the parentheses)
            return;
        }
        if (j2x_comma(node) != nullptr) {
            found = Comma{node, slot};
            return;
        }
        ir::visit(*node, [&found](auto& k) {
            k.each_child([&found](Node*& child) { find_comma(&child, child, found); });
        });
    }

    static void flatten_commas(Node* e, std::vector<Node*>& out) {
        e = j2x_strip(e);
        if (const auto* b = j2x_comma(e)) {
            flatten_commas(b->left, out);
            flatten_commas(b->right, out);
        } else {
            out.push_back(e);
        }
    }

    void fold_temps(std::vector<Node*>& out) {
        std::size_t i = 0;
        while (i < out.size()) {
            std::map<std::string, std::size_t> temps;   // a temporary's name -> its place in `out`
            std::size_t j = i;
            while (j < out.size()) {
                const auto* local = ir::get<ir::Local>(out[j]);
                if (local == nullptr || !local->sym || !local->sym->name.starts_with("j2xo")) break;
                temps[local->sym->name] = j;
                ++j;
            }
            if (!temps.empty() && j < out.size()) {
                std::optional<Comma> comma;
                find_comma(nullptr, out[j], comma);
                if (comma && comma->slot != nullptr) {
                    std::vector<Node*> parts;
                    flatten_commas(comma->node, parts);
                    const std::vector<Node*> assigns(parts.begin(), parts.end() - 1);
                    Node* last = parts.back();
                    bool all = assigns.size() == temps.size();
                    for (Node* a : assigns) {
                        const auto* assign = ir::get<ir::Assign>(a);
                        const auto* target = assign != nullptr && !assign->op ? ir::get<ir::Name>(j2x_strip(assign->target)) : nullptr;
                        all = all && target != nullptr && target->sym && temps.contains(target->sym->name);
                    }
                    if (all) {
                        std::vector<Node*> order;
                        for (Node* a : assigns) {
                            const auto* assign = ir::get<ir::Assign>(a);
                            Node* local = out[temps.at(ir::get<ir::Name>(j2x_strip(assign->target))->sym->name)];
                            ir::get<ir::Local>(local)->init = assign->value;
                            order.push_back(local);
                        }
                        *comma->slot = last;
                        out.erase(out.begin() + static_cast<std::ptrdiff_t>(i), out.begin() + static_cast<std::ptrdiff_t>(j));
                        out.insert(out.begin() + static_cast<std::ptrdiff_t>(i), order.begin(), order.end());
                        i += order.size();
                        continue;
                    }
                }
            }
            ++i;
        }
    }

    Arena& arena_;
    std::map<std::pair<std::string, std::string>, std::string> renamed_;   // (owner, s_name) -> name
    std::map<const ir::Sym*, SymP> lifted_syms_;                           // a symbol is lifted once: its uses keep sharing it
};

}  // namespace

void lift(Arena& arena, std::span<ir::Node* const> units) {
    J2xLift pass(arena);
    for (const Node* u : units) {
        if (const auto* unit = ir::get<ir::Unit>(u)) pass.names(*unit);
    }
    for (Node* u : units) {
        if (ir::is<ir::Unit>(u)) pass.unit(u);
    }
}

void bind(judge_native_jvm::Host& host) {
    host.lift_java_runtime = [](Arena& arena, std::span<ir::Node* const> units) { lift(arena, units); };
}

}  // namespace cb::translate::frontends::clang::j2x

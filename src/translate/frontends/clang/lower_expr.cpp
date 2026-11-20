// translate/frontends/clang/lower_expr.cpp -- expressions: literals, casts, names, members, operators, calls, construction,
// sizeof and initialiser lists.
#include <algorithm>
#include <charconv>
#include <functional>
#include <utility>

#include "core/text.hpp"
#include "translate/frontends/clang/lower_impl.hpp"

namespace cb::translate::frontends::clang {

namespace {

bool in_set(std::string_view word, std::initializer_list<std::string_view> words) { return std::ranges::find(words, word) != words.end(); }

bool is_binary_operator_kept(std::string_view op) {
    return in_set(op, {"+", "-", "*", "/", "%", "<<", ">>", "&", "|", "^", "&&", "||", "==", "!=", "<", "<=", ">", ">=", ","});
}

const ir::PtrType* as_ptr(const ir::TypeP& t) { return t ? std::get_if<ir::PtrType>(&t->body) : nullptr; }
bool is_ref_ptr(const ir::TypeP& t) {
    const ir::PtrType* p = as_ptr(t);
    return p != nullptr && p->ref;
}
bool is_class(const ir::TypeP& t) { return t && (std::holds_alternative<ir::ClassType>(t->body) || std::holds_alternative<ir::RecordType>(t->body)); }
bool is_array(const ir::TypeP& t) { return t && std::holds_alternative<ir::ArrayType>(t->body); }
bool is_ptr_or_array(const ir::TypeP& t) { return as_ptr(t) != nullptr || is_array(t); }

ir::LitValue integer_literal(const std::string& digits) {
    if (const auto v = text::parse_int<std::int64_t>(digits)) return *v;
    if (const auto u = text::parse_int<std::uint64_t>(digits)) return *u;
    tree::bad("an integer literal that is not an integer: '" + digits + "'");
}

// The unsigned value as the integer a Lit holds.
ir::LitValue integer_value(const Json& node, std::string_view key) {
    const Json& v = tree::require(node, key);
    if (v.is_string()) return integer_literal(v.get<std::string>());
    if (v.is_number_unsigned() && v.get<std::uint64_t>() > static_cast<std::uint64_t>(INT64_MAX)) return v.get<std::uint64_t>();
    return tree::integer(node, key);
}

double floating_value(const Json& node) {
    const Json& v = tree::require(node, "value");
    if (v.is_number()) return v.get<double>();
    if (!v.is_string()) tree::bad("a floating literal that is not a number");
    const std::string& s = v.get_ref<const std::string&>();
    double d = 0;
    const auto [end, ec] = std::from_chars(s.data(), s.data() + s.size(), d);
    if (ec != std::errc() || end != s.data() + s.size()) tree::bad("a floating literal that is not a number: '" + s + "'");
    return d;
}

}  // namespace

ir::Node* Lowering::unary_node(const char* op, ir::Node* x, const ir::TypeP& type, Pos p) {
    return make(ir::Unary{.op = op, .x = x}, type, p);
}

ir::Node* Lowering::zero(const ir::TypeP& t, Pos p) {
    if (!t) tree::bad("a value of no type");
    if (is_class(t)) {
        return make(ir::New{.sym = sym(ir::SymKind::Ctor, "<aggregate>", spell(t)), .memory = ir::MemoryModel::Stack}, t, p);
    }
    if (const auto* array = std::get_if<ir::ArrayType>(&t->body)) return make(ir::NewArray{.elem = array->elem}, t, p);
    if (std::holds_alternative<ir::PtrType>(t->body) || std::holds_alternative<ir::FnPtrType>(t->body)) return make(ir::Lit{}, t, p);
    if (const auto* prim = std::get_if<ir::PrimType>(&t->body)) {
        if (prim->prim == ir::Prim::F32 || prim->prim == ir::Prim::F64 || prim->prim == ir::Prim::F80) return make(ir::Lit{.value = 0.0}, t, p);
        if (prim->prim == ir::Prim::Bool) return make(ir::Lit{.value = false}, t, p);
    }
    return make(ir::Lit{.value = std::int64_t{0}}, t, p);
}

// Lowers one expression; an expression the dump does not describe as clang writes it is an Unsupported of that node.
ir::Node* Lowering::expr(const Json& n) {
    try {
        return expr_unguarded(n);
    } catch (const ParseError& e) {
        return unsupported_node("expr:" + tree::maybe_text(n, "kind").value_or("None"), &n, std::string(e.what()).substr(0, 200));
    } catch (const FormatError& e) {
        return unsupported_node("expr:" + tree::maybe_text(n, "kind").value_or("None"), &n, std::string(e.what()).substr(0, 200));
    }
}

ir::Node* Lowering::expr_unguarded(const Json& n) {
    const std::string k = tree::kind(n);
    const Pos p = pos(n);
    const std::vector<Json>& inner = tree::inner(n);
    const ir::TypeP t = ty(n);
    if (k == "IntegerLiteral") return make(ir::Lit{.value = integer_value(n, "value")}, t, p);
    if (k == "CharacterLiteral") {
        const std::int64_t v = tree::integer(n, "value");
        return make(ir::Lit{.value = v, .text = char_literal_text(v)}, t, p);
    }
    if (k == "FloatingLiteral") return make(ir::Lit{.value = floating_value(n)}, t, p);
    if (k == "StringLiteral") return make(ir::Lit{.value = string_literal_value(tree::require(n, "value").get<std::string>())}, t, p);
    if (k == "CXXBoolLiteralExpr") return make(ir::Lit{.value = tree::flag(n, "value")}, t, p);
    if (k == "CXXNullPtrLiteralExpr" || k == "GNUNullExpr") return make(ir::Lit{}, ir::null_type(), p);
    if (k == "PredefinedExpr") return !inner.empty() ? expr(inner[0]) : unsupported_node(k, &n);
    if (k == "ParenExpr") return make(ir::Paren{.x = expr(tree::child(n, 0))}, t, p);
    if (in_set(k, {"ExprWithCleanups", "MaterializeTemporaryExpr", "CXXBindTemporaryExpr", "FullExpr", "SubstNonTypeTemplateParmExpr",
                   "CXXDefaultInitExpr", "ConstantExpr"})) {
        if (k == "CXXDefaultInitExpr" && inner.empty()) return unsupported_node(k, &n);
        if (k == "SubstNonTypeTemplateParmExpr") return expr(tree::child(n, -1));   // [the parameter, the value it stands for]
        return expr(tree::child(n, 0));
    }
    if (k == "CXXDefaultArgExpr") return default_arg(n);
    if (k == "ImplicitCastExpr") return implicit_cast(n, tree::child(n, 0), t, p);
    if (in_set(k, {"CStyleCastExpr", "CXXStaticCastExpr", "CXXReinterpretCastExpr", "CXXConstCastExpr", "CXXFunctionalCastExpr",
                   "CXXDynamicCastExpr", "BuiltinBitCastExpr"})) {
        const Json* x = inner.empty() ? nullptr : &inner[0];
        if (x == nullptr) tree::bad("a cast of nothing");
        if (k == "CXXFunctionalCastExpr" && in_set(tree::kind(*x), {"CXXConstructExpr", "InitListExpr"})) return expr(*x);
        const std::string cast_kind = tree::text(n, "castKind");
        if (cast_kind == "ToVoid") return make(ir::Cast{.x = expr(*x)}, ir::void_type(), p);
        if ((cast_kind == "ConstructorConversion" || cast_kind == "NoOp") && t && std::holds_alternative<ir::ClassType>(t->body)) return expr(*x);
        return make(ir::Cast{.x = expr(*x)}, t, p);
    }
    if (k == "DeclRefExpr") return decl_ref(n, t, p);
    if (k == "MemberExpr") return member(n, t, p);
    if (k == "ArraySubscriptExpr") {
        ir::Node* a = expr(tree::child(n, 0));
        ir::Node* b = expr(tree::child(n, 1));
        if (a->type && !is_ptr_or_array(a->type) && b->type && is_ptr_or_array(b->type)) std::swap(a, b);
        return make(ir::Index{.arr = a, .index = b}, t, p);
    }
    if (k == "UnaryOperator") return unary(n, t, p);
    if (k == "BinaryOperator") {
        const std::string op = tree::require(n, "opcode").get<std::string>();
        ir::Node* l = expr(tree::child(n, 0));
        ir::Node* r = expr(tree::child(n, 1));
        if (op == "=") return make(ir::Assign{.target = l, .value = r}, t, p);
        if (is_binary_operator_kept(op)) return make(ir::Binary{.op = op, .left = l, .right = r}, t, p);
        return unsupported_node("binary:" + op, &n);
    }
    if (k == "CompoundAssignOperator") {
        std::string op = tree::require(n, "opcode").get<std::string>();
        if (!op.empty()) op.pop_back();
        ir::Node* target = expr(tree::child(n, 0));
        ir::Node* value = expr(tree::child(n, 1));
        return make(ir::Assign{.op = op, .target = target, .value = value}, t, p);
    }
    if (k == "ConditionalOperator") {
        ir::Node* c = expr(tree::child(n, 0));
        ir::Node* a = expr(tree::child(n, 1));
        ir::Node* b = expr(tree::child(n, 2));
        return make(ir::Cond{.cond = c, .then = a, .orelse = b}, t, p);
    }
    if (k == "CallExpr") return call(n, t, p);
    if (k == "CXXMemberCallExpr") return member_call(n, tree::child(n, 0), 1, t, p);
    if (k == "CXXOperatorCallExpr") return operator_call(n, t, p);
    if (k == "UnaryExprOrTypeTraitExpr") return size_of(n);
    if (k == "InitListExpr") return init_list(n, t, p);
    if (k == "ImplicitValueInitExpr") return zero(t, p);
    if (k == "CompoundLiteralExpr") return expr(tree::child(n, 0));
    if (k == "CXXConstructExpr" || k == "CXXTemporaryObjectExpr") return construct(n, t, p);
    if (k == "CXXNewExpr") return new_expr(n, t, p);
    if (k == "CXXDeleteExpr") return make(ir::Unary{.op = tree::flag(n, "isArray") ? "delete[]" : "delete", .x = expr(tree::child(n, 0))}, ir::void_type(), p);
    if (k == "CXXThisExpr") return make(ir::This{}, t, p);
    if (k == "CXXScalarValueInitExpr") return zero(t, p);
    if (k == "VAArgExpr") {   // va_arg(ap, T): the builtin, typed T
        ir::SymP s = sym(ir::SymKind::Function, "__builtin_va_arg", std::nullopt, true);
        s->params = {ty(tree::child(n, 0))};
        s->ret = t;
        return make(ir::Call{.sym = s, .args = {expr(tree::child(n, 0))}}, t, p);
    }
    if (k == "CXXThrowExpr") return unsupported_node("throw-as-expression", &n);
    return unsupported_node(k, &n);
}

ir::Node* Lowering::implicit_cast(const Json& n, const Json& x_n, const ir::TypeP& t, Pos p) {
    const std::string ck = tree::text(n, "castKind");
    if (in_set(ck, {"LValueToRValue", "NoOp", "FunctionToPointerDecay", "LValueToRValueBitCast", "AtomicToNonAtomic", "NonAtomicToAtomic"})) {
        return expr(x_n);
    }
    if (ck == "UncheckedDerivedToBase" || ck == "DerivedToBase") return make(ir::Cast{.x = expr(x_n), .implicit = true}, t, p);
    if (ck == "ConstructorConversion" || ck == "UserDefinedConversion") return expr(x_n);
    ir::Node* x = expr(x_n);
    if (ck == "BuiltinFnToFnPtr") return x;
    return make(ir::Cast{.x = x, .implicit = true}, t, p);
}

// A default argument is the expression of its parameter's declaration.
ir::Node* Lowering::default_arg(const Json& n) {
    const Json* param = tree::get(n, "param");
    if (param != nullptr && param->is_object()) {
        if (const auto it = decls_.find(tree::text(*param, "id")); it != decls_.end() && !tree::inner(*it->second).empty()) {
            return expr(tree::child(*it->second, -1));
        }
    }
    return unsupported_node("default-argument", &n);
}

ir::Node* Lowering::decl_ref(const Json& n, const ir::TypeP& t, Pos p) {
    const Json& d = tree::object(n, "referencedDecl");
    const std::string dk = tree::kind(d);
    const std::string did = tree::text(d, "id");
    if (dk == "VarDecl" || dk == "ParmVarDecl" || dk == "BindingDecl") {
        ir::SymP s;
        if (scope_) {
            if (const auto it = scope_->syms.find(did); it != scope_->syms.end()) s = it->second;
        }
        if (!s) {
            const Json* node = &d;
            if (const auto it = decls_.find(did); it != decls_.end()) node = it->second;
            const Json* par = nullptr;
            if (node != &d) {
                par = member_owner(*node);
            } else if (const auto it = parent_.find(did); it != parent_.end()) {
                par = it->second;
            }
            if (par != nullptr && (tree::kind(*par) == "CXXRecordDecl" || tree::kind(*par) == "ClassTemplateSpecializationDecl")) {
                const ir::TypeP rt = record_type(par);
                const ir::TypeP st = ty(*node);
                ir::Node* qualifier = make(ir::TypeName{}, rt, p);
                return make(ir::Select{.obj = qualifier, .sym = sym(ir::SymKind::Field, tree::require(*node, "name").get<std::string>(), spell(rt), true, st)}, st, p);
            }
            const ir::TypeP st = ty(*node);
            const std::optional<std::string> owner = is_own(loc(*node).file) ? std::optional<std::string>(unit_name_) : lib_owner(*node);
            s = sym(ir::SymKind::Global, tree::text(d, "name"), owner, true, st);
        }
        ir::Node* e = make(ir::Name{.sym = s}, s->type ? s->type : t, p);
        if (const ir::PtrType* ptr = as_ptr(s->type); ptr != nullptr && ptr->ref) return make(ir::Unary{.op = "*", .x = e}, ptr->to, p);
        return e;
    }
    if (dk == "FunctionDecl" || dk == "CXXMethodDecl") {
        ir::SymP s = sym_of_function(d);
        const Json* node = &d;
        if (const auto it = decls_.find(did); it != decls_.end()) node = it->second;
        return make(ir::AddressOf{.sym = s}, ctypes::decay(ty(*node)), p);
    }
    if (dk == "EnumConstantDecl") {
        const auto par = parent_.find(did);
        const auto value = enum_values_.find(did);
        if (par != parent_.end()) {
            if (const std::string* qn = scoped_enums_.find(tree::text(*par->second, "id"))) {
                ir::Node* qualifier = make(ir::TypeName{}, ir::class_type(*qn), p);
                return make(ir::Select{.obj = qualifier, .sym = sym(ir::SymKind::Field, tree::require(d, "name").get<std::string>(), *qn, true, ir::i32_type())},
                            ir::i32_type(), p);
            }
        }
        const auto node = decls_.find(did);
        const std::int32_t file = node != decls_.end() ? loc(*node->second).file : -1;
        if (!is_own(file)) {
            ir::Lit lit;
            if (value != enum_values_.end()) lit.value = value->second;
            lit.text = tree::require(d, "name").get<std::string>();
            return make(std::move(lit), ir::i32_type(), p);
        }
        return make(ir::Name{.sym = sym(ir::SymKind::Global, tree::require(d, "name").get<std::string>(), unit_name_, true, ir::i32_type())}, ir::i32_type(), p);
    }
    if (dk == "NonTypeTemplateParmDecl") return unsupported_node("template-parameter", &n);
    return unsupported_node("ref:" + tree::maybe_text(d, "kind").value_or("None"), &n);
}

ir::Node* Lowering::member(const Json& n, const ir::TypeP& t, Pos p) {
    ir::Node* base = expr(tree::child(n, 0));
    const std::string member_id = tree::text(n, "referencedMemberDecl");
    const Json* md = nullptr;
    if (const auto it = decls_.find(member_id); it != decls_.end()) md = it->second;
    const std::string member_name = tree::text(n, "name");
    if (md != nullptr && tree::kind(*md) == "CXXMethodDecl") return unsupported_node("bound-member-function", &n);
    if (md != nullptr && tree::kind(*md) == "VarDecl") {   // a static member through an object
        const auto par = parent_.find(tree::require(*md, "id").get<std::string>());
        const ir::TypeP rt = par != parent_.end() ? record_type(par->second) : ir::TypeP();
        ir::Node* qualifier = make(ir::TypeName{}, rt, p);
        return make(ir::Select{.obj = qualifier, .sym = sym(ir::SymKind::Field, member_name, spell(rt), true, t)}, t, p);
    }
    ir::Node* obj = base;
    if (tree::flag(n, "isArrow")) {
        const ir::PtrType* ptr = as_ptr(base->type);
        obj = make(ir::Unary{.op = "*", .x = base}, ptr != nullptr ? ptr->to : ir::any_type(), p);
    }
    std::optional<std::string> owner = obj->type ? std::optional<std::string>(spell(obj->type)) : std::nullopt;
    if (md != nullptr) {
        if (const auto par = parent_.find(tree::require(*md, "id").get<std::string>()); par != parent_.end()) {
            if (const ir::TypeP rt = record_type(par->second)) owner = spell(rt);
        }
    }
    ir::TypeP ft = t;
    if (md != nullptr) ft = ty(*md);
    ir::Node* e = make(ir::Select{.obj = obj, .sym = sym(ir::SymKind::Field, member_name, owner, false, ft)}, ft, p);
    if (const ir::PtrType* ptr = as_ptr(ft); ptr != nullptr && ptr->ref) return make(ir::Unary{.op = "*", .x = e}, ptr->to, p);
    if (md == nullptr || !(ft == t)) e->type = (t && std::holds_alternative<ir::AnyType>(t->body)) ? ft : t;
    return e;
}

ir::Node* Lowering::unary(const Json& n, const ir::TypeP& t, Pos p) {
    const std::string op = tree::require(n, "opcode").get<std::string>();
    const Json& x_n = tree::child(n, 0);
    if (op == "&") {   // &function: a function's address
        const Json* target = &x_n;
        while (tree::kind(*target) == "ParenExpr") target = &tree::child(*target, 0);
        if (tree::kind(*target) == "DeclRefExpr" && tree::kind(tree::object(*target, "referencedDecl")) == "FunctionDecl") return expr(*target);
    }
    ir::Node* x = expr(x_n);
    if (op == "__extension__") return x;
    if (op == "++" || op == "--") return make(ir::Unary{.op = op, .x = x, .postfix = tree::flag(n, "isPostfix")}, t, p);
    if (in_set(op, {"-", "+", "!", "~", "*", "&"})) {
        if (op == "*" && x->type && std::holds_alternative<ir::FnPtrType>(x->type->body)) return x;   // (*fp)(...) is fp(...)
        return make(ir::Unary{.op = op, .x = x}, t, p);
    }
    return unsupported_node("unary:" + op, &n);
}

// An argument given to a reference parameter: its binding made explicit (Unary '&'), as for a pointer, so every back end
// sees which variables a call can change.
std::vector<ir::Node*> Lowering::bind_refs(std::vector<ir::Node*> args, const std::vector<ir::TypeP>& params) {
    for (std::size_t i = 0; i < args.size(); ++i) {
        ir::Node* a = args[i];
        if (i >= params.size() || !is_ref_ptr(params[i])) continue;
        const auto* already = ir::get<ir::Unary>(a);
        if ((already != nullptr && already->op == "&") || is_ref_ptr(a->type)) continue;
        args[i] = make(ir::Unary{.op = "&", .x = a}, params[i], Pos{a->line, a->col});
    }
    return args;
}

ir::Node* Lowering::call(const Json& n, const ir::TypeP& t, Pos p) {
    const std::vector<Json>& inner = tree::inner(n);
    const Json& callee = tree::child(n, 0);
    const Json* c = &callee;
    while ((tree::kind(*c) == "ImplicitCastExpr" || tree::kind(*c) == "ParenExpr") && !tree::inner(*c).empty()) {
        if (tree::kind(*c) == "ImplicitCastExpr" && !in_set(tree::text(*c, "castKind"), {"FunctionToPointerDecay", "BuiltinFnToFnPtr", "NoOp"})) break;
        c = &tree::child(*c, 0);
    }
    std::vector<ir::Node*> args;
    for (std::size_t i = 1; i < inner.size(); ++i) args.push_back(expr(inner[i]));
    if (tree::kind(*c) == "DeclRefExpr" && in_set(tree::kind(tree::object(*c, "referencedDecl")), {"FunctionDecl", "CXXMethodDecl"})) {
        const Json& d = tree::object(*c, "referencedDecl");
        ir::SymP s = sym_of_function(d);
        args = bind_refs(std::move(args), s->params);
        const Json* node = &d;
        if (const auto it = decls_.find(tree::require(d, "id").get<std::string>()); it != decls_.end()) node = it->second;
        if (tree::kind(*node) == "CXXMethodDecl") {   // a static member function
            const Json* par = nullptr;
            if (const auto it = decls_.find(tree::text(*node, "parentDeclContextId")); it != decls_.end()) par = it->second;
            if (par == nullptr) {
                if (const auto it = parent_.find(tree::require(*node, "id").get<std::string>()); it != parent_.end()) par = it->second;
            }
            ir::Node* qualifier = make(ir::TypeName{}, par != nullptr ? record_type(par) : ir::TypeP(), p);
            return make(ir::Call{.target = qualifier, .sym = s, .args = std::move(args)}, t, p);
        }
        return make(ir::Call{.sym = s, .args = std::move(args)}, t, p);
    }
    if (tree::kind(*c) == "MemberExpr") return member_call(n, *c, 1, t, p);
    ir::Node* fn = expr(callee);
    return make(ir::Call{.target = fn, .args = std::move(args)}, t, p);
}

ir::Node* Lowering::member_call(const Json& call_node, const Json& callee, std::size_t first_arg, const ir::TypeP& t, Pos p) {
    const Json* me = &callee;
    while (tree::kind(*me) == "ParenExpr") me = &tree::child(*me, 0);
    if (tree::kind(*me) != "MemberExpr") return unsupported_node("member-call:" + tree::maybe_text(*me, "kind").value_or("None"), &call_node);
    const std::string member_id = tree::text(*me, "referencedMemberDecl");
    Json stub;   // (a method clang did not declare in this unit: named by the call)
    const Json* md = nullptr;
    if (const auto it = decls_.find(member_id); it != decls_.end()) {
        md = it->second;
    } else {
        stub = Json::object();
        stub["id"] = member_id;
        stub["name"] = tree::text(*me, "name");
        stub["kind"] = "CXXMethodDecl";
        md = &stub;
    }
    ir::Node* base = expr(tree::child(*me, 0));
    ir::Node* obj = base;
    if (tree::flag(*me, "isArrow")) {
        const ir::PtrType* ptr = as_ptr(base->type);
        obj = make(ir::Unary{.op = "*", .x = base}, ptr != nullptr ? ptr->to : ir::any_type(), p);
    }
    ir::SymP s = sym_of_function(*md);
    std::vector<ir::Node*> args;
    const std::vector<Json>& inner = tree::inner(call_node);
    for (std::size_t i = first_arg; i < inner.size(); ++i) args.push_back(expr(inner[i]));
    args = bind_refs(std::move(args), s->params);
    if (tree::kind(*md) == "CXXConversionDecl") return make(ir::Cast{.x = obj}, t, p);
    return make(ir::Call{.target = obj, .sym = s, .args = std::move(args)}, t, p);
}

ir::Node* Lowering::operator_call(const Json& n, const ir::TypeP& t, Pos p) {
    const std::vector<Json>& inner = tree::inner(n);
    const Json* c = &tree::child(n, 0);
    while ((tree::kind(*c) == "ImplicitCastExpr" || tree::kind(*c) == "ParenExpr") && !tree::inner(*c).empty()) c = &tree::child(*c, 0);
    const Json& d = tree::object(*c, "referencedDecl");
    const Json* node = &d;
    if (const auto it = decls_.find(tree::text(d, "id")); it != decls_.end()) node = it->second;
    std::vector<ir::Node*> args;
    for (std::size_t i = 1; i < inner.size(); ++i) args.push_back(expr(inner[i]));
    const std::string name_text = tree::text(d, "name");
    if (tree::flag(*node, "isImplicit") || tree::flag(*node, "explicitlyDefaulted")) {
        if (name_text == "operator=" && args.size() == 2) return make(ir::Assign{.target = args[0], .value = args[1]}, t, p);
    }
    ir::SymP s = sym_of_function(d);
    if (tree::kind(*node) == "CXXMethodDecl") {
        if (args.empty()) tree::bad("a member operator call with no object");
        ir::Node* object = args[0];
        std::vector<ir::Node*> rest(args.begin() + 1, args.end());
        return make(ir::Call{.target = object, .sym = s, .args = bind_refs(std::move(rest), s->params)}, t, p);
    }
    return make(ir::Call{.sym = s, .args = bind_refs(std::move(args), s->params)}, t, p);
}

ir::Node* Lowering::construct(const Json& n, const ir::TypeP& t, Pos p) {
    std::vector<ir::Node*> args;
    for (const Json& a : tree::inner(n)) args.push_back(expr(a));
    const std::string ctor_type = tree::text(tree::object(n, "ctorType"), "qualType");
    const bool is_named_class = t && std::holds_alternative<ir::ClassType>(t->body);
    const Json* cls = is_named_class ? find_record(spell(t)) : nullptr;
    const Json* ctor = cls != nullptr ? find_ctor(*cls, ctor_type) : nullptr;
    const bool copying = args.size() == 1 && args[0]->type && spell(args[0]->type) == spell(t);
    const bool trivial = ctor == nullptr || tree::flag(*ctor, "isImplicit") || tree::flag(*ctor, "explicitlyDefaulted");
    if (copying && trivial) return args[0];   // a copy or a move: the value itself
    if (!is_named_class) return !args.empty() ? args[0] : zero(t, p);
    if (trivial && args.empty()) {
        return make(ir::New{.sym = sym(ir::SymKind::Ctor, "<aggregate>", spell(t)), .memory = ir::MemoryModel::Stack}, t, p);
    }
    const auto* named = std::get_if<ir::ClassType>(&t->body);
    ir::SymP s = ctor != nullptr ? sym_of_function(*ctor) : sym(ir::SymKind::Ctor, named->name, spell(t));
    return make(ir::New{.sym = s, .args = bind_refs(std::move(args), s->params), .memory = ir::MemoryModel::Stack}, t, p);
}

const Json* Lowering::find_ctor(const Json& cls, const std::string& ctor_type) {
    for (const Json& m : tree::inner(cls)) {
        if (tree::kind(m) == "CXXConstructorDecl" && tree::maybe_text(tree::object(m, "type"), "qualType") == ctor_type) return &m;
    }
    for (const Json& m : tree::inner(cls)) {
        if (tree::kind(m) != "FunctionTemplateDecl") continue;
        for (const Json& x : tree::inner(m)) {
            if (tree::kind(x) == "CXXConstructorDecl" && tree::maybe_text(tree::object(x, "type"), "qualType") == ctor_type) return &x;
        }
    }
    return nullptr;
}

ir::Node* Lowering::new_expr(const Json& n, const ir::TypeP& t, Pos p) {
    const std::vector<Json>& inner = tree::inner(n);
    const ir::PtrType* ptr = as_ptr(t);
    const ir::TypeP el = ptr != nullptr ? ptr->to : ir::any_type();
    if (tree::flag(n, "isArray")) {
        ir::Node* size = nullptr;
        std::vector<ir::Node*> init;
        for (const Json& c : inner) {
            if (tree::kind(c) == "InitListExpr") {
                init.clear();
                for (const Json& x : tree::inner(c)) init.push_back(expr(x));
            } else if (size == nullptr && tree::kind(c) != "CXXConstructExpr") {
                size = expr(c);
            }
        }
        ir::NewArray made;
        made.elem = el;
        if (size != nullptr) made.dims = {size};
        made.init = std::move(init);
        return make(std::move(made), t, p);
    }
    if (!inner.empty()) {
        ir::Node* e = expr(inner.back());
        if (auto* made = ir::get<ir::New>(e)) {
            made->memory = ir::MemoryModel::Manual;
            e->type = t;
            return e;
        }
        return make(ir::New{.sym = sym(ir::SymKind::Ctor, "<value>", spell(el)), .args = {e}, .memory = ir::MemoryModel::Manual}, t, p);
    }
    return make(ir::New{.sym = sym(ir::SymKind::Ctor, "<value>", spell(el)), .memory = ir::MemoryModel::Manual}, t, p);
}

ir::Node* Lowering::size_of(const Json& n) {
    const Pos p = pos(n);
    const std::string name_text = tree::maybe_text(n, "name").value_or("sizeof");
    ir::TypeP at;
    std::string text_of;
    if (tree::get(n, "argType") != nullptr) {
        const std::string spelling = tree::text(tree::require(n, "argType"), "qualType");
        at = ctypes::normalise(types_.parse(spelling));
        text_of = name_text + "(" + spelling + ")";
    } else {
        at = ty(tree::child(n, 0));
        text_of = name_text + " expr";
    }
    if (!in_set(name_text, {"sizeof", "alignof", "_Alignof", "__alignof"})) return unsupported_node("trait:" + name_text, &n);
    const std::uint64_t v = name_text == "sizeof" ? sizes_.size(at) : sizes_.align(at);
    return make(ir::Lit{.value = v <= static_cast<std::uint64_t>(INT64_MAX) ? ir::LitValue(static_cast<std::int64_t>(v)) : ir::LitValue(v), .text = text_of}, ty(n), p);
}

ir::Node* Lowering::init_list(const Json& n, const ir::TypeP& t, Pos p) {
    const std::vector<Json>* items_json = &tree::inner(n);
    const Json* filler = nullptr;
    const Json* filler_list = tree::get(n, "array_filler");
    if (filler_list != nullptr && filler_list->is_array()) {   // clang writes the filler first, then the elements given
        if (filler_list->empty()) tree::bad("an array filler with no filler");
        filler = &(*filler_list)[0];
        items_json = &filler_list->get_ref<const Json::array_t&>();
    }
    std::vector<const Json*> given;
    for (std::size_t i = filler != nullptr ? 1 : 0; i < items_json->size(); ++i) given.push_back(&(*items_json)[i]);
    if (const auto* array = std::get_if<ir::ArrayType>(&t->body)) {
        std::vector<ir::Node*> items;
        for (const Json* x : given) items.push_back(expr(*x));
        if (filler != nullptr && tree::kind(*filler) != "ImplicitValueInitExpr" && array->count) {
            ir::Node* fill = expr(*filler);
            while (items.size() < *array->count) items.push_back(fill);
        }
        // a string literal initialising a char array
        if (items.size() == 1) {
            const auto* lit = ir::get<ir::Lit>(items[0]);
            const auto* elem = array->elem ? std::get_if<ir::PrimType>(&array->elem->body) : nullptr;
            if (lit != nullptr && std::holds_alternative<std::string>(lit->value) && elem != nullptr &&
                (elem->prim == ir::Prim::Char || elem->prim == ir::Prim::I8 || elem->prim == ir::Prim::U8)) {
                return items[0];
            }
        }
        return make(ir::NewArray{.elem = array->elem, .init = std::move(items)}, t, p);
    }
    if (std::holds_alternative<ir::ClassType>(t->body)) {
        std::vector<ir::Node*> args;
        for (const Json* x : given) args.push_back(expr(*x));
        std::string ctor_name = "<aggregate>";
        if (const Json* f = tree::get(n, "field")) ctor_name = "<aggregate:" + tree::text(*f, "name") + ">";   // a union: the member it initialises
        return make(ir::New{.sym = sym(ir::SymKind::Ctor, ctor_name, spell(t)), .args = std::move(args), .memory = ir::MemoryModel::Stack}, t, p);
    }
    if (given.size() == 1) return expr(*given[0]);   // int x = {5};
    if (given.empty()) return zero(t, p);
    return unsupported_node("init-list:" + spell(t), &n);
}

}  // namespace cb::translate::frontends::clang

// cb-unity: alone (file-local helpers named for this module)
// One function: its signature, its body, and the small makers of nodes the other files of the lowering share.
#include <format>

#include "cb/translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

Lowerer::Lowerer(Context& context, const Fn& function, infer::Eval& evaluation)
    : ctx(context),
      p(context.program()),
      ev(evaluation),
      fn(&function),
      module(function.module),
      cls(function.cls),
      qname(function.qname),
      kind(function.kind),
      params(function.params),
      kwonly(function.kwonly),
      source(function.module->source) {}

Lowerer::Lowerer(Context& context, const Module& top_level, infer::Eval& evaluation)
    : ctx(context), p(context.program()), ev(evaluation), module(&top_level), qname(top_level.name + ".<module>"), source(top_level.source) {}

// ---- making nodes -----------------------------------------------------------------------------------------------------------------

Node* Lowerer::lit(ir::LitValue value, const TypeP& type, std::optional<std::string> text) {
    return mk(ir::Lit{.value = std::move(value), .text = std::move(text)}, type);
}

Node* Lowerer::py_call(std::string_view name, std::vector<Node*> args, const TypeP& type, Node* target, std::uint32_t line, std::vector<ir::TypeArg> typeargs) {
    const ir::SymP sym = ir::make_sym(ir::SymKind::Function, std::string(name), std::string(kPy));
    return mk(ir::Call{.target = target, .sym = sym, .args = std::move(args), .typeargs = std::move(typeargs)}, type, line);
}

ir::SymP Lowerer::local_sym(const std::string& name, const TypeP& type) const { return ir::make_sym(ir::SymKind::Local, name, qname, false, type); }

Node* Lowerer::local_decl(const std::string& name, const TypeP& type, Node* init, std::vector<std::string> mods, std::uint32_t line) {
    return mk(ir::Local{.sym = local_sym(name, type), .type = type, .init = init, .mods = std::move(mods)}, {}, line);
}

Node* Lowerer::name_expr(const std::string& name) {
    const auto at = local_types.find(name);
    if (at == local_types.end()) refuse(RefusalClass::Untyped, "local " + name);
    return mk(ir::Name{.sym = local_sym(name, at->second)}, at->second);
}

Node* Lowerer::block_of(std::vector<Node*> stmts) { return mk(ir::Block{.stmts = std::move(stmts)}); }

Node* Lowerer::if_node(Node* cond_node, Node* then, Node* orelse, std::uint32_t line) {
    ir::If node;
    node.cond = cond_node;
    node.then = then;
    if (orelse != nullptr) node.orelse = orelse;
    else node.orelse = static_cast<Node*>(nullptr);
    return mk(std::move(node), {}, line);
}

Node* Lowerer::not_of(Node* cond_node) {
    return mk(ir::Unary{.op = "!", .x = mk(ir::Paren{.x = cond_node}, bool_type()), .postfix = false}, bool_type());
}

Node* Lowerer::assign_stmt(Node* target, Node* value, const TypeP& type, std::uint32_t line) {
    return mk(ir::ExprStmt{.expr = mk(ir::Assign{.op = std::nullopt, .target = target, .value = value}, type)}, {}, line);
}

Node* Lowerer::expr_stmt(Node* expression, std::uint32_t line) { return mk(ir::ExprStmt{.expr = expression}, {}, line); }

Node* Lowerer::class_name_node(std::string_view class_name) { return mk(ir::TypeName{}, py_class(class_name)); }

void Lowerer::refuse(RefusalClass cls_value, std::string detail, std::uint32_t line) const { throw Refused(cls_value, std::move(detail), line); }

// ---- types and positions ----------------------------------------------------------------------------------------------------------

const Type& Lowerer::type_at(const void* node, std::uint32_t line) const {
    const auto found = ev.types.find(node);
    if (found == ev.types.end()) throw Refused(RefusalClass::Untyped, "no type reached the node", line);
    return found->second;
}

const infer::Res* Lowerer::res_at(const void* node) const {
    const auto found = ev.res.find(node);
    return found == ev.res.end() ? nullptr : &found->second;
}

// ---- the function ----------------------------------------------------------------------------------------------------------------

Node* Lowerer::lower() {
    const Fn& f = *fn;
    const std::uint32_t line = line_of(f.node);
    for (const infer::Issue& issue : ev.issues) throw Refused(issue);
    if (const auto crashed = p.crashes.find(f.qname); crashed != nullptr) throw Refused(RefusalClass::FrontendError, *crashed, line);
    if (f.kind == program::FnKind::ClassMethod) throw Refused(RefusalClass::Dynamic, "classmethod", line);

    std::vector<Node*> parameters;
    const std::size_t skip = f.kind == program::FnKind::Method || f.kind == program::FnKind::Property ? 1 : 0;
    std::vector<std::string> names(f.params.begin() + static_cast<std::ptrdiff_t>(std::min(skip, f.params.size())), f.params.end());
    names.insert(names.end(), f.kwonly.begin(), f.kwonly.end());
    std::vector<TypeP> parameter_types;
    for (const std::string& name : names) {
        const Type t = p.param_type(f, name);
        if (t.kind == pt::Kind::Bottom) {
            const auto poison = p.param_poison.find({f.qname, name});
            if (poison != p.param_poison.end()) {
                const std::string& why = poison->second.why;
                const std::string head = why.substr(0, why.find(':'));
                const std::string rest = why.substr(why.find(':') == std::string::npos ? 0 : why.find(':') + 1);
                const std::string& caller = poison->second.caller;
                throw Refused(RefusalClass::Untyped,
                              std::format("parameter {} (its callers pass values of no type: {}:{} in {})", name, head, rest.substr(0, 40),
                                          caller.substr(caller.rfind('.') == std::string::npos ? 0 : caller.rfind('.') + 1)),
                              line);
            }
            throw Refused(RefusalClass::Untyped, "parameter " + name + " (no annotation, no call site)", line);
        }
        const TypeP it = ty(t, f.node, "parameter " + name);
        local_types[name] = it;
        declared.insert(name);
        std::vector<std::string> mods;
        if (const auto* d = f.defaults.find(name); d != nullptr)
            if (const auto* constant = syntax::as<syntax::Constant>(*d)) mods.push_back("default=" + default_text(constant->value));
        parameters.push_back(mk(ir::Param{.sym = ir::make_sym(ir::SymKind::Param, name, f.qname, false, it), .type = it, .mods = std::move(mods)}));
        parameter_types.push_back(it);
    }
    const auto returned = p.returns.find(f.qname);
    Type ret_type = returned != p.returns.end() ? returned->second : pt::none();
    if (ret_type.kind == pt::Kind::Bottom) ret_type = pt::none();
    bool is_generator = false;   // a function with `yield` in it (not in a nested one: those are refused) returns what it yields
    for (const pyast::AnyNode& an : pyast::walk(pyast::AnyNode(static_cast<const syntax::Stmt*>(f.node)))) {
        if (an.expr() != nullptr && an.expr()->kind == syntax::ExprKind::Yield) is_generator = true;
    }
    ret = f.name == "__init__" ? none_type() : ty(ret_type, f.node, is_generator ? "generator result" : "return value");
    plan_locals();

    // a docstring is the function's comment, not a statement
    const std::vector<syntax::Stmt*>& all = f.node->body;
    const syntax::ExprStmt* first = all.empty() ? nullptr : syntax::as<syntax::ExprStmt>(all.front());
    const auto doc = first != nullptr ? pyast::string_constant(first->value) : std::nullopt;
    std::vector<syntax::Stmt*> statements(all.begin() + (doc ? 1 : 0), all.end());
    Node* body = block(statements);

    const bool is_ctor = f.cls != nullptr && f.name == "__init__";
    const std::string decl_kind = f.cls == nullptr ? "function" : (is_ctor ? "ctor" : "method");
    std::vector<std::string> mods;
    if (f.kind == program::FnKind::Static) mods.push_back("static");
    if (f.kind == program::FnKind::Property) mods.push_back("property");
    if (is_generator) mods.push_back("iterator");
    const ir::SymP sym = ir::make_sym(f.cls == nullptr ? ir::SymKind::Function : (is_ctor ? ir::SymKind::Ctor : ir::SymKind::Method), f.name,
                                      f.cls != nullptr ? f.cls->qname : f.module->name, f.kind == program::FnKind::Static);
    sym->params = parameter_types;
    sym->ret = ret;
    ir::MethodDecl decl;
    decl.sym = sym;
    decl.name = f.name;
    decl.kind = decl_kind;
    decl.params = std::move(parameters);
    decl.ret = ret;
    decl.body = body;
    decl.mods = std::move(mods);
    if (doc) decl.comments.push_back(clean_docstring(*doc));
    return mk(std::move(decl), {}, line);
}

Node* Lowerer::value_of(const syntax::Expr* expression, const TypeP& want) { return coerce(expr(expression), want); }

}  // namespace cb::translate::frontends::python::lower

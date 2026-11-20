// cb-unity: alone (file-local helpers named for this module)
// The statements of a function.
#include <algorithm>

#include "core/text.hpp"
#include "translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

namespace {

bool is_comprehension(const syntax::Expr* e) {
    return syntax::as<syntax::ListComp>(e) != nullptr || syntax::as<syntax::SetComp>(e) != nullptr || syntax::as<syntax::DictComp>(e) != nullptr;
}

bool is_none_literal(const syntax::Expr* e) {
    const auto* c = syntax::as<syntax::Constant>(e);
    return c != nullptr && c->value.kind == syntax::ConstValue::Kind::None;
}

// The names of a loop or comprehension target, which must be names (or a tuple of them).
std::vector<std::string> names_of_target(const syntax::Expr* target, const Lowerer& l, std::string_view what) {
    std::vector<std::string> names;
    const std::vector<syntax::Expr*>* elements = nullptr;
    if (const auto* n = syntax::as<syntax::Name>(target)) {
        names.push_back(n->id);
    } else {
        if (const auto* t = syntax::as<syntax::Tuple>(target)) elements = &t->elts;
        else if (const auto* li = syntax::as<syntax::List>(target)) elements = &li->elts;
        if (elements != nullptr)
            for (const syntax::Expr* e : *elements)
                if (const auto* item = syntax::as<syntax::Name>(e)) names.push_back(item->id);
        const std::size_t count = elements != nullptr ? elements->size() : 0;
        if (names.size() != count) l.refuse(RefusalClass::Syntax, std::string(what), Lowerer::line_of(target));
    }
    return names;
}

}  // namespace

// ---- blocks and statements ---------------------------------------------------------------------------------------------------

Node* Lowerer::block(const std::vector<syntax::Stmt*>& stmts) {
    std::vector<Node*> out;
    for (const syntax::Stmt* st : stmts) {
        if (const auto names = decl_at.find(st); names != decl_at.end())
            for (const std::string& name : names->second) {
                out.push_back(local_decl(name, local_types.at(name)));
                declared.insert(name);
            }
        pre.clear();
        std::vector<Node*> got = stmt(st);
        out.insert(out.end(), pre.begin(), pre.end());
        pre.clear();
        out.insert(out.end(), got.begin(), got.end());
    }
    return block_of(std::move(out));
}

std::vector<Node*> Lowerer::stmt(const syntax::Stmt* st) {
    using K = syntax::StmtKind;
    switch (st->kind) {
        case K::Pass: return {};
        case K::Expr: return s_expr(syntax::cast<syntax::ExprStmt>(st));
        case K::Assign: return s_assign(syntax::cast<syntax::Assign>(st));
        case K::AnnAssign: return s_ann_assign(syntax::cast<syntax::AnnAssign>(st));
        case K::AugAssign: return s_aug_assign(syntax::cast<syntax::AugAssign>(st));
        case K::Return: return s_return(syntax::cast<syntax::Return>(st));
        case K::Raise: return s_raise(syntax::cast<syntax::Raise>(st));
        case K::Assert: return s_assert(syntax::cast<syntax::Assert>(st));
        case K::If: return s_if(syntax::cast<syntax::If>(st));
        case K::While: return s_while(syntax::cast<syntax::While>(st));
        case K::Break: return {mk(ir::Break{}, {}, line_of(st))};
        case K::Continue: return {mk(ir::Continue{}, {}, line_of(st))};
        case K::For: return s_for(syntax::cast<syntax::For>(st));
        case K::Try: return s_try(syntax::cast<syntax::Try>(st));
        case K::With: return s_with(syntax::cast<syntax::With>(st));
        case K::Import:
        case K::ImportFrom: return {};
        default: refuse(RefusalClass::Syntax, text::lower(syntax::name_of(st->kind)), line_of(st));
    }
}

std::vector<Node*> Lowerer::s_expr(const syntax::ExprStmt* st) {
    if (const auto* c = syntax::as<syntax::Constant>(st->value)) {
        // a bare string is a comment; any other bare constant does nothing
        if (c->value.kind == syntax::ConstValue::Kind::Str) return {mk(ir::Comment{.text = c->value.text})};
        return {};
    }
    if (const auto* y = syntax::as<syntax::Yield>(st->value)) {   // `yield x`: the next item of the generator
        if (y->value == nullptr) refuse(RefusalClass::Syntax, "a bare yield", line_of(st));
        return {mk(ir::YieldReturn{.expr = expr(y->value)})};
    }
    return {expr_stmt(expr(st->value), line_of(st))};
}

std::vector<Node*> Lowerer::s_assign(const syntax::Assign* st) {
    if (st->targets.size() != 1) refuse(RefusalClass::Syntax, "chained assignment", line_of(st));
    const syntax::Expr* target = st->targets[0];
    const auto* name = syntax::as<syntax::Name>(target);
    if (is_comprehension(st->value) && name != nullptr) {
        bool mentions_target = false;
        for (const pyast::AnyNode& n : pyast::walk(st->value))
            if (const auto* id = syntax::as<syntax::Name>(n.expr()); id != nullptr && id->id == name->id) mentions_target = true;
        if (!mentions_target) return comp_into(name, st->value);
    }
    if (is_record_literal(st->value) && name != nullptr && decl_init.contains(st)) return record_builder(st, name);
    Node* value = expr(st->value);
    return store(target, value, st);
}

std::vector<Node*> Lowerer::s_ann_assign(const syntax::AnnAssign* st) {
    if (st->value == nullptr) return {};
    return store(st->target, expr(st->value), st);
}

std::vector<Node*> Lowerer::s_aug_assign(const syntax::AugAssign* st) {
    const Type& t = type_at(st, line_of(st));
    Node* target = target_expr(st->target);
    Node* value = expr(st->value);
    Node* current = expr_of_target(st->target, target);
    const auto target_type = ev.types.find(st->target);
    Node* updated = binop(st, st->op, current, value, target_type != ev.types.end() ? target_type->second : t, type_at(st->value, line_of(st)), t);
    const std::uint32_t line = line_of(st);
    if (const auto* b = ir::get<ir::Binary>(updated)) {
        static const std::set<std::string> compound = {"+", "-", "*", "&", "|", "^", "<<", ">>", "/", "%"};
        if (compound.contains(b->op) && b->left == current)
            return {expr_stmt(mk(ir::Assign{.op = b->op, .target = target, .value = value}, updated->type), line)};
    }
    if (const auto* c = ir::get<ir::Concat>(updated)) {
        if (!c->parts.empty() && c->parts.front() == current) {
            Node* rest = c->parts.size() > 2 ? mk(ir::Concat{.parts = std::vector<Node*>(c->parts.begin() + 1, c->parts.end())}, updated->type) : c->parts[1];
            return {expr_stmt(mk(ir::Assign{.op = "+", .target = target, .value = rest}, updated->type), line)};
        }
    }
    return {expr_stmt(mk(ir::Assign{.op = std::nullopt, .target = target, .value = updated}, updated->type), line)};
}

// The target read as a value (for x op= y).
Node* Lowerer::expr_of_target(const syntax::Expr* target, Node* lowered) {
    if (syntax::as<syntax::Name>(target) != nullptr) return mk(ir::Name{.sym = ir::get<ir::Name>(lowered)->sym}, lowered->type);
    return lowered;
}

std::vector<Node*> Lowerer::store(const syntax::Expr* target, Node* value, const syntax::Stmt* st) {
    const std::uint32_t line = line_of(st);
    if (const auto* name = syntax::as<syntax::Name>(target)) {
        const auto at = local_types.find(name->id);
        if (at == local_types.end()) refuse(RefusalClass::Untyped, "local " + name->id, line);
        const TypeP t = at->second;
        value = coerce(value, t);
        if (decl_init.contains(st) && !declared.contains(name->id)) {
            declared.insert(name->id);
            return {local_decl(name->id, t, value, {}, line)};
        }
        return {assign_stmt(mk(ir::Name{.sym = local_sym(name->id, t)}, t), value, t, line)};
    }
    if (syntax::as<syntax::Tuple>(target) != nullptr || syntax::as<syntax::List>(target) != nullptr) return unpack(target, value, st);
    Node* lowered = target_expr(target);
    value = coerce(value, lowered->type);
    return {assign_stmt(lowered, value, lowered->type, line)};
}

std::vector<Node*> Lowerer::unpack(const syntax::Expr* target, Node* value, const syntax::Stmt* st) {
    const std::uint32_t line = line_of(st);
    const std::vector<syntax::Expr*>& elements = syntax::as<syntax::Tuple>(target) != nullptr ? syntax::as<syntax::Tuple>(target)->elts
                                                                                              : syntax::as<syntax::List>(target)->elts;
    std::vector<std::string> names;
    for (const syntax::Expr* e : elements) {
        const auto* n = syntax::as<syntax::Name>(e);
        if (n == nullptr) refuse(RefusalClass::Syntax, "unpacking into " + text::lower(syntax::name_of(e->kind)), line);
        names.push_back(n->id);
    }
    const ir::ClassType* tuple = names_class(value->type, "py.tuple") ? class_type_of(value->type) : nullptr;
    if (tuple == nullptr || tuple->args.size() != names.size()) refuse(RefusalClass::Conflict, "unpacking a non-tuple", line);
    const std::vector<TypeP>& item_types = tuple->args;
    const auto fresh = decl_tuple.find(st);
    const auto is_fresh = [&](const std::string& n) { return fresh != decl_tuple.end() && fresh->second.contains(n) && !declared.contains(n); };
    const auto local_type = [&](const std::string& n) -> TypeP {
        const auto at = local_types.find(n);
        return at == local_types.end() ? TypeP() : at->second;
    };
    if (names.size() == 1) {   // `x, = t`: its one item
        Node* item = coerce(py_call("get", {value}, item_types[0], nullptr, 0, {std::int64_t{0}}), local_type(names[0]));
        if (is_fresh(names[0])) {
            declared.insert(names[0]);
            return {local_decl(names[0], local_types.at(names[0]), item, {}, line)};
        }
        return store(elements[0], item, st);
    }
    const auto all_fresh = [&] {
        for (std::size_t i = 0; i < names.size(); ++i)
            if (!is_fresh(names[i]) || !(local_type(names[i]) == item_types[i])) return false;
        return true;
    };
    const auto name_exprs = [&] {
        std::vector<Node*> out;
        for (const std::string& n : names) out.push_back(name_expr(n));
        return out;
    };
    if (all_fresh()) {   // each name is declared by the unpacking itself
        for (const std::string& n : names) declared.insert(n);
        return {assign_stmt(py_call("bind", name_exprs(), value->type), value, value->type, line)};
    }
    std::vector<Node*> out;
    for (const std::string& n : names)
        if (!declared.contains(n)) {
            out.push_back(local_decl(n, local_types.at(n)));
            declared.insert(n);
        }
    out.push_back(assign_stmt(py_call("tie", name_exprs(), value->type), value, value->type, line));
    return out;
}

Node* Lowerer::target_expr(const syntax::Expr* target) {
    const std::uint32_t line = line_of(target);
    if (const auto* name = syntax::as<syntax::Name>(target)) {
        const auto at = local_types.find(name->id);
        if (at == local_types.end()) refuse(RefusalClass::Untyped, "local " + name->id, line);
        if (!declared.contains(name->id)) {
            pre.push_back(local_decl(name->id, at->second));
            declared.insert(name->id);
        }
        return name_expr(name->id);
    }
    if (const auto* attribute = syntax::as<syntax::Attribute>(target)) {
        const infer::Res* r = res_at(target);
        if (r == nullptr || r->kind != infer::ResKind::Field) refuse(RefusalClass::Dynamic, "attribute store", line);
        Node* obj = expr(attribute->value);
        const auto stored = p.attr_type(r->name.value_or(""), r->member);
        if (!stored) refuse(RefusalClass::Untyped, "attribute " + r->member, line);
        const TypeP field_type = ctx.type_of(*stored, line, "attribute " + r->member);
        return mk(ir::Select{.obj = obj, .sym = ir::make_sym(ir::SymKind::Field, r->member, r->name, false, field_type)}, field_type, line);
    }
    if (const auto* subscript = syntax::as<syntax::Subscript>(target)) {
        const infer::Res* r = res_at(target);
        Node* obj = expr(subscript->value);
        if (r != nullptr && r->kind == infer::ResKind::DictStore && r->name && is_record_class(obj->type))
            return field_ref(obj, *r->name, subscript->value, line, /*store=*/true);
        if (syntax::as<syntax::Slice>(subscript->slice) != nullptr) refuse(RefusalClass::Syntax, "slice assignment", line);
        Node* index = expr(subscript->slice);
        return mk(ir::Index{.arr = obj, .index = index}, elem_type(obj->type, line), line);
    }
    refuse(RefusalClass::Syntax, "assignment target " + text::lower(syntax::name_of(target->kind)), line);
}

TypeP Lowerer::elem_type(const TypeP& t, std::uint32_t line) {
    const ir::ClassType* c = class_type_of(t);
    if (c != nullptr && (c->name == "py.list" || c->name == "py.set") && !c->args.empty()) return c->args[0];
    if (c != nullptr && c->name == "py.dict") return c->args.at(1);
    if (c != nullptr && (c->name == "py.bytes" || c->name == "py.bytearray")) return i64_type();
    if (c != nullptr && c->name == "py.str") return str_type();
    refuse(RefusalClass::Conflict, "item of " + (t ? ir::to_text(*t) : std::string("nothing")), line);
}

// ---- return, raise, assert ---------------------------------------------------------------------------------------------------

std::vector<Node*> Lowerer::s_return(const syntax::Return* st) {
    const std::uint32_t line = line_of(st);
    const bool returns_nothing = ret == none_type() || kind_of(ret) == "void";
    if (st->value == nullptr) return {mk(ir::Return{.expr = returns_nothing ? nullptr : lit_none(ret)}, {}, line)};
    if (is_comprehension(st->value)) {   // built into a temporary, then returned
        const std::string name = new_tmp("result");
        const TypeP t = ty(type_at(st->value, line), st->value);
        std::vector<Node*> out = {local_decl(name, t, empty(t))};
        local_types[name] = t;
        declared.insert(name);
        const std::vector<Node*> loops = comp_loops(st->value, name, t);
        out.insert(out.end(), loops.begin(), loops.end());
        out.push_back(mk(ir::Return{.expr = name_expr(name)}, {}, line));
        return out;
    }
    if (returns_nothing) {
        Node* e = expr(st->value);
        if (is_none_literal(st->value)) return {mk(ir::Return{}, {}, line)};
        return {expr_stmt(e), mk(ir::Return{}, {}, line)};
    }
    return {mk(ir::Return{.expr = coerce(expr(st->value), ret)}, {}, line)};
}

std::vector<Node*> Lowerer::s_raise(const syntax::Raise* st) {
    if (st->exc == nullptr) return {mk(ir::Throw{}, {}, line_of(st))};
    return {mk(ir::Throw{.expr = expr(st->exc)}, {}, line_of(st))};
}

std::vector<Node*> Lowerer::s_assert(const syntax::Assert* st) {
    Node* test = cond(st->test);
    std::vector<Node*> args;
    if (st->msg != nullptr) {
        args.push_back(expr(st->msg));
        if (!(args[0]->type == str_type())) args = {py_call("str", args, str_type())};
    }
    Node* exception = mk(ir::New{.sym = ir::make_sym(ir::SymKind::Ctor, "AssertionError", std::string(kPy)), .args = std::move(args)}, py_class("py.AssertionError"));
    return {if_node(not_of(test), block_of({mk(ir::Throw{.expr = exception})}), nullptr, line_of(st))};
}

// ---- if, while, for ----------------------------------------------------------------------------------------------------------

std::vector<Node*> Lowerer::s_if(const syntax::If* st) {
    Node* test = cond(st->test);
    std::vector<Node*> before = std::move(pre);   // what the condition needs first stays before the whole statement
    pre.clear();
    Node* then = block(st->body);
    Node* orelse = nullptr;
    if (!st->orelse.empty()) {
        const auto* elif = st->orelse.size() == 1 ? syntax::as<syntax::If>(st->orelse[0]) : nullptr;
        if (elif != nullptr && !decl_at.contains(elif)) {   // else-if: a chain, not a nested block
            std::vector<Node*> saved = std::move(pre);
            pre.clear();
            Node* inner = s_if(elif).front();
            if (!pre.empty()) {
                std::vector<Node*> stmts = pre;
                stmts.push_back(inner);
                orelse = block_of(std::move(stmts));
            } else {
                orelse = inner;
            }
            pre = std::move(saved);
        } else {
            orelse = block(st->orelse);
        }
    }
    pre = std::move(before);
    return {if_node(test, then, orelse, line_of(st))};
}

std::vector<Node*> Lowerer::s_while(const syntax::While* st) {
    std::vector<Node*> saved = std::move(pre);
    pre.clear();
    Node* test = cond(st->test);
    if (!pre.empty()) refuse(RefusalClass::Syntax, "a while condition that needs statements", line_of(st));
    pre = std::move(saved);
    Node* body = block(st->body);
    return {mk(ir::While{.cond = test, .body = body}, {}, line_of(st))};
}

std::vector<Node*> Lowerer::s_for(const syntax::For* st) {
    const std::uint32_t line = line_of(st);
    const syntax::Expr* it = st->iter;
    const infer::Res* r = res_at(it);
    if (r != nullptr && r->kind == infer::ResKind::Builtin && r->name == "range" && syntax::as<syntax::Name>(st->target) != nullptr) return range_loop(st);   // a counted loop
    Node* iterable = expr(it);
    const syntax::Expr* target = st->target;
    const std::vector<std::string> names = names_of_target(target, *this, "loop target");
    const bool inner_only = std::all_of(names.begin(), names.end(), [&](const std::string& n) { return loop_local(st, n); });
    const TypeP item_type = ty(type_at(target, line), target, "loop item");
    const auto* single = syntax::as<syntax::Name>(target);
    if (inner_only) {   // the variable lives in the loop's header
        for (const std::string& n : names) declared.insert(n);
        Node* var;
        if (single != nullptr) {
            const TypeP t = local_types.at(single->id);
            var = local_decl(single->id, t);
        } else {
            var = local_decl(text::join(names, ","), item_type, nullptr, {"bind"});
        }
        Node* body = block(st->body);
        return {mk(ir::ForEach{.var = var, .iterable = iterable, .body = body}, {}, line)};
    }
    const std::string item_name = new_tmp("item");
    local_types[item_name] = item_type;
    Node* var = local_decl(item_name, item_type);
    std::vector<Node*> out;
    for (const std::string& n : names)
        if (!declared.contains(n)) {
            out.push_back(local_decl(n, local_types.at(n)));
            declared.insert(n);
        }
    std::vector<Node*> first = single != nullptr ? store(target, name_expr(item_name), st) : unpack(target, name_expr(item_name), st);
    Node* body = block(st->body);
    auto* stmts = ir::get<ir::Block>(body);
    stmts->stmts.insert(stmts->stmts.begin(), first.begin(), first.end());
    out.push_back(mk(ir::ForEach{.var = var, .iterable = iterable, .body = body}, {}, line));
    return out;
}

// The loop variable is used only inside the loop (C++ can declare it in the loop's header).
bool Lowerer::loop_local(const syntax::For* st, const std::string& name) {
    if (loopvars.contains(name)) return true;
    std::unordered_set<const syntax::Node*> inside;
    for (const pyast::AnyNode& n : pyast::walk(st)) inside.insert(n.node);
    for (const pyast::AnyNode& n : pyast::walk(fn->node))
        if (const auto* id = syntax::as<syntax::Name>(n.expr()); id != nullptr && id->id == name && !inside.contains(id)) return false;
    const auto* tuple = syntax::as<syntax::Tuple>(st->target);
    for (const pyast::AnyNode& n : pyast::walk(st)) {   // nor stored again inside, nor in a nested loop's target
        const auto* id = syntax::as<syntax::Name>(n.expr());
        if (id == nullptr || id->id != name || id->ctx != syntax::ExprCtx::Store || id == st->target) continue;
        if (tuple != nullptr && std::find(tuple->elts.begin(), tuple->elts.end(), id) != tuple->elts.end()) continue;
        return false;
    }
    return !declared.contains(name) || loopvars.contains(name);
}

std::vector<Node*> Lowerer::range_loop(const syntax::For* st) {
    const std::uint32_t line = line_of(st);
    const auto* call = syntax::cast<syntax::Call>(st->iter);
    std::vector<Node*> args;
    for (const syntax::Expr* a : call->args) args.push_back(expr(a));
    std::int64_t step = 1;
    if (call->args.size() == 3) {
        const auto constant = infer::int_constant(call->args[2]);
        if (!constant || !constant->to_i64() || *constant->to_i64() == 0) refuse(RefusalClass::Syntax, "range with a computed step", line);
        step = *constant->to_i64();
    }
    Node* start = args.size() == 1 ? lit(std::int64_t{0}, i64_type()) : args[0];
    Node* stop = args.size() == 1 ? args[0] : args[1];
    const std::string name = syntax::cast<syntax::Name>(st->target)->id;
    const auto declared_type = local_types.find(name);
    const TypeP it_type = declared_type != local_types.end() ? declared_type->second : i64_type();
    const bool inner = loop_local(st, name);
    std::vector<Node*> out;
    if (!pure(call->args.size() < 3 ? call->args.back() : call->args[1], st)) {   // the bound is read once, before the loop
        const std::string stop_name = new_tmp("stop");
        local_types[stop_name] = i64_type();
        out.push_back(local_decl(stop_name, i64_type(), stop, {"const"}));
        stop = name_expr(stop_name);
    }
    std::string var;
    if (inner) {
        var = name;
        declared.insert(name);
    } else {
        var = new_tmp(name);
        local_types[var] = i64_type();
        if (!declared.contains(name)) {
            out.push_back(local_decl(name, it_type));
            declared.insert(name);
        }
    }
    const ir::SymP var_sym = local_sym(var, i64_type());
    const auto var_ref = [&] { return mk(ir::Name{.sym = var_sym}, i64_type()); };
    std::vector<Node*> init = {mk(ir::Local{.sym = var_sym, .type = i64_type(), .init = start})};
    Node* test = mk(ir::Binary{.op = step > 0 ? "<" : ">", .left = var_ref(), .right = stop}, bool_type());
    Node* update = step == 1    ? mk(ir::Unary{.op = "++", .x = var_ref(), .postfix = false}, i64_type())
                   : step == -1 ? mk(ir::Unary{.op = "--", .x = var_ref(), .postfix = false}, i64_type())
                                : mk(ir::Assign{.op = "+", .target = var_ref(), .value = lit(step, i64_type())}, i64_type());
    Node* body = block(st->body);
    if (!inner) {   // the loop's own counter is copied into the variable the source reads
        auto* stmts = ir::get<ir::Block>(body);
        stmts->stmts.insert(stmts->stmts.begin(), assign_stmt(name_expr(name), var_ref(), i64_type()));
    }
    out.push_back(mk(ir::For{.init = std::move(init), .cond = test, .update = {update}, .body = body}, {}, line));
    return out;
}

// A range bound C++ may evaluate on every pass: a constant, or a local the loop does not store.
bool Lowerer::pure(const syntax::Expr* node, const syntax::Stmt* loop) {
    if (syntax::as<syntax::Constant>(node) != nullptr) return true;
    const auto* name = syntax::as<syntax::Name>(node);
    if (name == nullptr || !local_types.contains(name->id)) return false;
    for (const pyast::AnyNode& n : pyast::walk(loop))
        if (const auto* id = syntax::as<syntax::Name>(n.expr()); id != nullptr && id->id == name->id && id->ctx == syntax::ExprCtx::Store) return false;
    return true;
}

// ---- try, with ------------------------------------------------------------------------------------------------------------------------

std::vector<Node*> Lowerer::s_try(const syntax::Try* st) {
    Node* body = block(st->body);
    std::vector<Node*> catches;
    for (const syntax::ExceptHandler* h : st->handlers) {
        std::vector<TypeP> types;
        if (h->type == nullptr) {
            types.push_back(py_class("py.Exception"));
        } else {
            std::vector<const syntax::Expr*> caught;
            if (const auto* tuple = syntax::as<syntax::Tuple>(h->type)) caught.assign(tuple->elts.begin(), tuple->elts.end());
            else caught.push_back(h->type);
            for (const syntax::Expr* n : caught) {
                const auto* name = syntax::as<syntax::Name>(n);
                std::optional<std::string> program_class = name != nullptr ? ev.lookup_class(n) : std::nullopt;
                if (program_class) {
                    types.push_back(ir::make_type(ir::ClassType{.name = *program_class, .ref = false}));
                } else if (name != nullptr) {
                    types.push_back(py_class("py." + name->id));
                } else if (const auto* attribute = syntax::as<syntax::Attribute>(n)) {
                    types.push_back(pyast::describe_expr(n) == "struct.error" ? py_class("py.StructError") : py_class("py." + attribute->attr));
                } else {
                    refuse(RefusalClass::Syntax, "except clause", line_of(h));
                }
            }
        }
        catches.push_back(mk(ir::Catch{.sym = nullptr, .types = std::move(types), .body = block(h->body)}, {}, line_of(h)));
    }
    return {mk(ir::Try{.resources = {}, .body = body, .catches = std::move(catches), .finally_body = nullptr}, {}, line_of(st))};
}

std::vector<Node*> Lowerer::s_with(const syntax::With* st) {
    std::vector<Node*> out;
    for (const syntax::withitem* item : st->items) {
        Node* value = expr(item->context_expr);
        if (item->optional_vars != nullptr) {
            const std::vector<Node*> stored = store(item->optional_vars, value, st);
            out.insert(out.end(), stored.begin(), stored.end());
        } else {
            out.push_back(expr_stmt(value));
        }
    }
    Node* body = block(st->body);
    const auto& inner = ir::get<ir::Block>(body)->stmts;
    out.insert(out.end(), inner.begin(), inner.end());
    return {block_of(std::move(out))};
}

}  // namespace cb::translate::frontends::python::lower

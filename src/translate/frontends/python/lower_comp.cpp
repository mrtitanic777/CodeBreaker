// cb-unity: alone (file-local helpers named for this module)
// Comprehensions (loops that fill a temporary) and records (a dict whose every use names a constant key, as a struct).
#include <algorithm>
#include <format>

#include "core/text.hpp"
#include "translate/frontends/python/lower_internal.hpp"

namespace cb::translate::frontends::python::lower {

namespace {

const std::vector<syntax::comprehension*>& generators_of(const syntax::Expr* comp) {
    if (const auto* l = syntax::as<syntax::ListComp>(comp)) return l->generators;
    if (const auto* s = syntax::as<syntax::SetComp>(comp)) return s->generators;
    return syntax::cast<syntax::DictComp>(comp)->generators;
}

}  // namespace

// ---- temporaries ------------------------------------------------------------------------------------------------------------------

// A name for a temporary that no local of the function has: the base itself the first time, then base_2, base_3 ...
std::string Lowerer::new_tmp(const std::string& base) {
    ++tmp;
    std::string name = (local_types.contains(base) || ev.locals.contains(base) || tmp > 1) ? std::format("{}_{}", base, tmp) : base;
    while (local_types.contains(name) || ev.locals.contains(name)) {
        ++tmp;
        name = std::format("{}_{}", base, tmp);
    }
    return name;
}

Node* Lowerer::empty(const TypeP& t) { return py_call("empty", {}, t); }

// ---- comprehensions -----------------------------------------------------------------------------------------------------------------

std::vector<Node*> Lowerer::comp_into(const syntax::Name* target, const syntax::Expr* comp) {
    const std::string& name = target->id;
    const TypeP t = local_types.at(name);
    std::vector<Node*> out;
    if (!declared.contains(name)) {
        out.push_back(local_decl(name, t, empty(t)));
        declared.insert(name);
    } else {
        out.push_back(assign_stmt(name_expr(name), empty(t), t));
    }
    const std::vector<Node*> loops = comp_loops(comp, name, t);
    out.insert(out.end(), loops.begin(), loops.end());
    return out;
}

// The loops of a comprehension, adding each item to the local `into`.
std::vector<Node*> Lowerer::comp_loops(const syntax::Expr* comp, const std::string& into, const TypeP& t) {
    const ir::ClassType* container = class_type_of(t);
    const auto add = [&]() -> Node* {
        if (const auto* d = syntax::as<syntax::DictComp>(comp)) {
            Node* key = expr(d->key);
            const TypeP value_type = container->args.at(1);
            Node* value = coerce(expr(d->value), value_type);
            Node* slot = mk(ir::Index{.arr = name_expr(into), .index = key}, value_type);
            return assign_stmt(slot, value, value_type);
        }
        const syntax::Expr* element = syntax::as<syntax::ListComp>(comp) != nullptr ? syntax::as<syntax::ListComp>(comp)->elt : syntax::as<syntax::SetComp>(comp)->elt;
        Node* e = coerce(expr(element), container->args.at(0));
        return expr_stmt(py_call(container->name == "py.list" ? "list.append" : "set.add", {e}, none_type(), name_expr(into)));
    };
    return gen_loops(generators_of(comp), 0, add);
}

std::vector<Node*> Lowerer::gen_loops(const std::vector<syntax::comprehension*>& gens, std::size_t k, const std::function<Node*()>& innermost) {
    const syntax::comprehension* g = gens[k];
    std::vector<Node*> saved = std::move(pre);
    pre.clear();
    const infer::Res* r = res_at(g->iter);
    const bool is_range = r != nullptr && r->kind == infer::ResKind::Builtin && r->name == "range";
    Node* iterable = is_range ? nullptr : expr(g->iter);
    const syntax::Expr* target = g->target;
    const auto* single = syntax::as<syntax::Name>(target);
    std::vector<std::string> names;
    const std::vector<syntax::Expr*>* elements = nullptr;
    if (single != nullptr) {
        names.push_back(single->id);
    } else {
        if (const auto* t = syntax::as<syntax::Tuple>(target)) elements = &t->elts;
        else if (const auto* l = syntax::as<syntax::List>(target)) elements = &l->elts;
        if (elements != nullptr)
            for (const syntax::Expr* e : *elements)
                if (const auto* n = syntax::as<syntax::Name>(e)) names.push_back(n->id);
        if (names.size() != (elements != nullptr ? elements->size() : 0)) refuse(RefusalClass::Syntax, "comprehension target", line_of(target));
    }
    const TypeP item_type = ty(type_at(target, line_of(target)), target, "comprehension item");
    for (const std::string& n : names) {
        const auto at = local_types.find(n);
        if (at != local_types.end() && declared.contains(n) && !(at->second == item_type))
            refuse(RefusalClass::Syntax, std::format("comprehension variable {} shadows a local of another type", n), line_of(target));
    }
    Node* var;
    if (single != nullptr) {
        local_types[single->id] = item_type;
        var = local_decl(single->id, item_type);
    } else {
        const ir::ClassType* tuple = class_type_of(item_type);
        for (std::size_t i = 0; elements != nullptr && tuple != nullptr && i < elements->size() && i < tuple->args.size(); ++i)
            if (const auto* n = syntax::as<syntax::Name>((*elements)[i])) local_types[n->id] = tuple->args[i];
        var = local_decl(text::join(names, ","), item_type, nullptr, {"bind"});
    }
    std::vector<Node*> body_stmts;
    if (k + 1 < gens.size()) {
        body_stmts = gen_loops(gens, k + 1, innermost);
    } else {
        pre.clear();
        Node* innermost_stmt = innermost();
        body_stmts = pre;
        body_stmts.push_back(innermost_stmt);
        pre.clear();
    }
    for (auto condition = g->ifs.rbegin(); condition != g->ifs.rend(); ++condition) {
        pre.clear();
        Node* c = cond(*condition);
        if (!pre.empty()) refuse(RefusalClass::Syntax, "comprehension condition needing statements", line_of(*condition));
        body_stmts = {if_node(c, block_of(std::move(body_stmts)), nullptr)};
    }
    Node* loop;
    if (is_range && single != nullptr) {
        const auto* range_call = syntax::as<syntax::Call>(g->iter);
        if (range_call == nullptr) refuse(RefusalClass::Syntax, "comprehension target", line_of(target));
        std::vector<Node*> args;
        for (const syntax::Expr* a : range_call->args) args.push_back(expr(a));
        Node* start = args.size() == 1 ? lit(std::int64_t{0}, i64_type()) : args[0];
        Node* stop = args.size() == 1 ? args[0] : args[1];
        std::int64_t step = 1;
        if (args.size() == 3) {
            const auto constant = infer::int_constant(range_call->args[2]);
            if (!constant || !constant->to_i64() || *constant->to_i64() == 0) refuse(RefusalClass::Syntax, "range with a computed step", line_of(target));
            step = *constant->to_i64();
        }
        const auto counter = [&] { return name_expr(single->id); };
        loop = mk(ir::For{.init = {mk(ir::Local{.sym = local_sym(single->id, i64_type()), .type = i64_type(), .init = start})},
                          .cond = mk(ir::Binary{.op = step > 0 ? "<" : ">", .left = counter(), .right = stop}, bool_type()),
                          .update = {step == 1 ? mk(ir::Unary{.op = "++", .x = counter(), .postfix = false}, i64_type())
                                               : mk(ir::Assign{.op = "+", .target = counter(), .value = lit(step, i64_type())}, i64_type())},
                          .body = block_of(std::move(body_stmts))});
    } else {
        if (iterable == nullptr) refuse(RefusalClass::Syntax, "comprehension target", line_of(target));
        loop = mk(ir::ForEach{.var = var, .iterable = iterable, .body = block_of(std::move(body_stmts))});
    }
    std::vector<Node*> out = pre;
    out.push_back(loop);
    pre = std::move(saved);
    return out;
}

// A comprehension inside an expression: built into a temporary before the statement.
Node* Lowerer::hoist_comp(const syntax::Expr* node, std::string_view what) {
    const TypeP t = ty(type_at(node, line_of(node)), node);
    const std::string name = new_tmp(what == "dict" ? "table" : "items");
    local_types[name] = t;
    declared.insert(name);
    std::vector<Node*> stmts = {local_decl(name, t, empty(t))};
    const std::vector<Node*> loops = comp_loops(node, name, t);
    stmts.insert(stmts.end(), loops.begin(), loops.end());
    pre.insert(pre.end(), stmts.begin(), stmts.end());
    return name_expr(name);
}

// A generator expression consumed whole (sum, join, sorted, list, set, min, max): its items as a list.
Node* Lowerer::gen_as_list(const syntax::GeneratorExp* gen) {
    const TypeP item_type = ty(type_at(gen->elt, line_of(gen)), gen->elt, "generator item");
    const TypeP t = py_class("py.list", {item_type});
    const std::string name = new_tmp("items");
    local_types[name] = t;
    declared.insert(name);
    std::vector<Node*> stmts = {local_decl(name, t, empty(t))};
    const auto add = [&]() -> Node* {
        return expr_stmt(py_call("list.append", {coerce(expr(gen->elt), item_type)}, none_type(), name_expr(name)));
    };
    const std::vector<Node*> loops = gen_loops(gen->generators, 0, add);
    stmts.insert(stmts.end(), loops.begin(), loops.end());
    pre.insert(pre.end(), stmts.begin(), stmts.end());
    return name_expr(name);
}

// any(gen) / all(gen): a loop that stops at the first item deciding it (Python's short-circuit).
Node* Lowerer::any_all(const syntax::Call* node, std::string_view which) {
    const syntax::Expr* argument = node->args.at(0);
    const auto* gen = syntax::as<syntax::GeneratorExp>(argument);
    if (gen == nullptr) return py_call(which, {expr(argument)}, bool_type());
    const bool is_all = which == "all";
    const std::string name = new_tmp(is_all ? "every" : "found");
    local_types[name] = bool_type();
    declared.insert(name);
    std::vector<Node*> stmts = {local_decl(name, bool_type(), lit(is_all, bool_type()))};
    const auto decide = [&]() -> Node* {
        Node* c = cond(gen->elt);
        if (is_all) c = not_of(c);
        return if_node(c, block_of({assign_stmt(name_expr(name), lit(!is_all, bool_type()), bool_type()), mk(ir::Break{})}), nullptr);
    };
    const std::vector<Node*> loops = gen_loops(gen->generators, 0, decide);
    if (gen->generators.size() > 1)
        refuse(RefusalClass::Syntax, std::format("{} over nested loops (break leaves only the inner one)", which), line_of(node));
    stmts.insert(stmts.end(), loops.begin(), loops.end());
    pre.insert(pre.end(), stmts.begin(), stmts.end());
    return name_expr(name);
}

// ---- records ----------------------------------------------------------------------------------------------------------------------------

bool Lowerer::is_record_literal(const syntax::Expr* node) const {
    const auto t = ev.types.find(node);
    if (t == ev.types.end() || t->second.kind != pt::Kind::Dict || !t->second.site || !p.is_record(*t->second.site)) return false;
    if (syntax::as<syntax::Dict>(node) != nullptr) return true;
    const auto* call = syntax::as<syntax::Call>(node);
    return call != nullptr && infer::is_name(call->func, "dict");
}

// `h = {...}` (then `h["k"] = v` ...): the record declared, then filled as Python fills it.
std::vector<Node*> Lowerer::record_builder(const syntax::Assign* st, const syntax::Name* target) {
    const std::string& name = target->id;
    const TypeP t = local_types.at(name);
    Node* value = expr(st->value);
    declared.insert(name);
    return {local_decl(name, t, value, {}, line_of(st))};
}

// A dict display (or dict(...) call) that is a record: its fields in the struct's order.
Node* Lowerer::record_value(const syntax::Expr* node) {
    const Type& t = type_at(node, line_of(node));
    const TypeP record_type = ty(t, node);
    const SiteId root = p.find(t.site.value_or(0));
    const std::vector<RecordField> fields = ctx.records().fields(root);
    std::vector<std::pair<std::string, Node*>> given;   // (key, lowered) in source order
    const auto key_text = [&](const syntax::Expr* k) {
        const auto text_value = pyast::string_constant(k);
        if (!text_value) refuse(RefusalClass::Syntax, "a record key that is not a constant", line_of(k));
        return *text_value;
    };
    const auto spread = [&](const syntax::Expr* value) {
        Node* source_record = expr(value);
        for (auto& entry : spread_fields(source_record, value)) given.push_back(std::move(entry));
    };
    if (const auto* d = syntax::as<syntax::Dict>(node)) {
        for (std::size_t i = 0; i < d->keys.size(); ++i) {
            if (d->keys[i] == nullptr) spread(d->values[i]);
            else given.emplace_back(key_text(d->keys[i]), expr(d->values[i]));
        }
    } else {
        const auto* call = syntax::cast<syntax::Call>(node);
        for (const syntax::Expr* a : call->args) spread(a);
        for (const syntax::keyword* kw : call->keywords) {
            if (!kw->has_arg) spread(kw->value);
            else given.emplace_back(kw->arg, expr(kw->value));
        }
    }
    std::map<std::string, Node*> by_key;   // (a key given twice: the later one)
    std::vector<std::string> order;
    for (const auto& [key, value] : given) {
        by_key[key] = value;
        order.push_back(key);
    }
    std::vector<std::string> in_field_order;
    for (const RecordField& f : fields)
        if (by_key.contains(f.key)) in_field_order.push_back(f.key);
    const auto impure = std::count_if(given.begin(), given.end(), [&](const auto& kv) { return effects(kv.second); });
    if (order != in_field_order && impure > 1) {
        for (const std::string& key : order) {   // keep Python's evaluation order
            Node* e = by_key.at(key);
            if (!effects(e)) continue;
            const std::string tmp_name = new_tmp(key);
            local_types[tmp_name] = e->type;
            declared.insert(tmp_name);
            pre.push_back(local_decl(tmp_name, e->type, e));
            by_key[key] = name_expr(tmp_name);
        }
    }
    std::vector<Node*> args;
    std::vector<ir::TypeArg> keys;
    for (const RecordField& f : fields) {
        TypeP want = ty(f.type, node, "field " + f.key);
        if (!f.required && !is_opt(want)) want = optional_type(want);
        const auto at = by_key.find(f.key);
        args.push_back(at != by_key.end() ? coerce(at->second, want) : lit_none(want));
        keys.push_back(f.key);
    }
    return py_call("record", std::move(args), record_type, nullptr, line_of(node), std::move(keys));
}

std::vector<std::pair<std::string, Node*>> Lowerer::spread_fields(Node* src, const syntax::Expr* node) {
    if (!is_record_class(src->type)) refuse(RefusalClass::Syntax, "** of a dict", line_of(node));
    const Type& t = type_at(node, line_of(node));
    std::vector<std::pair<std::string, Node*>> out;
    for (const RecordField& f : ctx.records().fields(t.site.value_or(0)))
        out.emplace_back(f.key, field_ref(src, f.key, node, line_of(node), /*store=*/false, /*raw=*/true));
    return out;
}

Node* Lowerer::field_ref(Node* obj, const std::string& key, const syntax::Expr* node, std::uint32_t line, bool store_value, bool raw) {
    const Type& t = type_at(node, line);
    const std::vector<RecordField> fields = ctx.records().fields(p.find(t.site.value_or(0)));
    const auto field = std::find_if(fields.begin(), fields.end(), [&](const RecordField& f) { return f.key == key; });
    if (field == fields.end()) refuse(RefusalClass::Conflict, "no field '" + key + "'", line);
    TypeP field_type = ty(field->type, node, "field " + key);
    if (!field->required && (raw || store_value) && !is_opt(field_type)) field_type = optional_type(field_type);
    const ir::ClassType* owner = class_type_of(obj->type);
    Node* selected = mk(ir::Select{.obj = obj, .sym = ir::make_sym(ir::SymKind::Field, key, owner != nullptr ? std::optional<std::string>(owner->name) : std::nullopt, false, field_type)},
                        field_type, line);
    if (!field->required && !raw && !store_value) return py_call("opt_value", {selected}, field_type);   // an absent key raises (KeyError in Python)
    return selected;
}

}  // namespace cb::translate::frontends::python::lower

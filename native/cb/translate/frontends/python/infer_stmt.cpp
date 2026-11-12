// cb/translate/frontends/python/infer_stmt.cpp -- the statements of a function run on types.
#include <algorithm>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/infer_util.hpp"
#include "cb/translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

using SK = syntax::StmtKind;

Flow Eval::block(const std::vector<syntax::Stmt*>& stmts, Flow state) {
    for (const syntax::Stmt* st : stmts) {
        if (!state) return std::nullopt;
        state = stmt(st, *state);
    }
    return state;
}

Flow Eval::stmt(const syntax::Stmt* st, const State& state) { return s_dispatch(st, state); }

Flow Eval::s_dispatch(const syntax::Stmt* st, const State& state) {
    switch (st->kind) {
        case SK::Pass: return state;
        case SK::Expr: return s_expr(syntax::cast<syntax::ExprStmt>(st), state);
        case SK::Assign: return s_assign(syntax::cast<syntax::Assign>(st), state);
        case SK::AnnAssign: return s_ann_assign(syntax::cast<syntax::AnnAssign>(st), state);
        case SK::AugAssign: return s_aug_assign(syntax::cast<syntax::AugAssign>(st), state);
        case SK::Return: return s_return(syntax::cast<syntax::Return>(st), state);
        case SK::Raise: return s_raise(syntax::cast<syntax::Raise>(st), state);
        case SK::Assert: return s_assert(syntax::cast<syntax::Assert>(st), state);
        case SK::Global: issue(st, IssueClass::Dynamic, "global"); return state;
        case SK::Nonlocal: issue(st, IssueClass::Dynamic, "nonlocal"); return state;
        case SK::Delete: issue(st, IssueClass::Dynamic, "del"); return state;
        case SK::FunctionDef:
        case SK::AsyncFunctionDef: issue(st, IssueClass::Syntax, "nested function"); return state;
        case SK::ClassDef: issue(st, IssueClass::Syntax, "nested class"); return state;
        case SK::Import: return s_import(syntax::cast<syntax::Import>(st), state);
        case SK::ImportFrom: return s_import_from(syntax::cast<syntax::ImportFrom>(st), state);
        case SK::If: return s_if(syntax::cast<syntax::If>(st), state);
        case SK::With: return s_with(syntax::cast<syntax::With>(st), state);
        case SK::Try: return s_try(syntax::cast<syntax::Try>(st), state);
        case SK::Break: return s_break(state);
        case SK::Continue: return s_continue(state);
        case SK::While: return s_while(syntax::cast<syntax::While>(st), state);
        case SK::For:
        case SK::AsyncFor: return s_for(syntax::cast<syntax::For>(st), state);
        default: {
            // any other statement (match, class patterns, type aliases ...): the front end does not lower it
            issue(st, IssueClass::Syntax, text::lower(syntax::name_of(st->kind)));
            return state;
        }
    }
}

Flow Eval::s_expr(const syntax::ExprStmt* st, const State& state) {
    if (syntax::as<syntax::Constant>(st->value) != nullptr) return state;
    ev(st->value, state);
    return after_access(st->value, state);
}

Flow Eval::s_assign(const syntax::Assign* st, const State& state) {
    const bool builder_form = st->targets.size() == 1 && syntax::as<syntax::Name>(st->targets[0]) != nullptr &&
                              (syntax::as<syntax::Dict>(st->value) != nullptr || syntax::as<syntax::Call>(st->value) != nullptr);
    if (builder_form) builder = static_cast<const syntax::Node*>(st->value);   // its creation keys are counted by builder_keys
    const Type t = ev(st->value, state);
    State s = *after_access(st->value, state);
    for (const syntax::Expr* target : st->targets) s = assign(target, t, s);
    if (builder_form) builder_keys(st, t);
    return s;
}

Flow Eval::s_ann_assign(const syntax::AnnAssign* st, const State& state) {
    if (st->value == nullptr) return state;
    Type t = ev(st->value, state);
    if (syntax::as<syntax::Name>(st->target) != nullptr) {
        const Type annotated = p.ann_to_type(*m, st->annotation);
        if (!is_one_of(annotated, {Kind::Bottom, Kind::Top, Kind::Int})) {
            // the annotation's container is joined with the value's, so that the two are one object class
            if (is_one_of(t, {Kind::List, Kind::Dict, Kind::Set}) && annotated.kind == t.kind) p.join(annotated, t);
            t = annotated;
        }
    }
    return assign(st->target, t, state);
}

Flow Eval::s_aug_assign(const syntax::AugAssign* st, const State& state) {
    const Type current = ev(st->target, state);
    const Type value = ev(st->value, state);
    const Type t = binop(st, st->op, current, value, st->target, st->value);
    note(st, t);
    return assign(st->target, t, state, true);
}

Flow Eval::s_return(const syntax::Return* st, const State& state) {
    if (st->value != nullptr) {
        const Type t = ev(st->value, state);
        returns_value = true;
        if (live()) ret = p.join(ret, t);
    } else if (live()) {
        ret = p.join(ret, P::none());
    }
    return std::nullopt;
}

Flow Eval::s_raise(const syntax::Raise* st, const State& state) {
    if (st->exc != nullptr) {
        const Type t = ev(st->exc, state);
        if (!is_one_of(t, {Kind::Inst, Kind::Cls, Kind::Bottom, Kind::Top, Kind::Extern})) {
            issue(st, IssueClass::Conflict, "raise of a " + std::string(P::kind_word(t.kind)));
        }
    }
    if (st->cause != nullptr) issue(st, IssueClass::Syntax, "raise from");
    return std::nullopt;
}

Flow Eval::s_assert(const syntax::Assert* st, const State& state) {
    ev(st->test, state);
    if (st->msg != nullptr) ev(st->msg, state);
    return refine(st->test, state, true);
}

Flow Eval::s_import(const syntax::Import* st, const State& state) {
    State s = state;
    for (const syntax::alias* a : st->names) {
        const std::string top = first_part(a->name);
        const std::string local = a->has_asname ? a->asname : top;
        local_imports.insert_or_assign(local, p.module_ref(a->has_asname ? a->name : top));
        s.pop(local);
    }
    return s;
}

Flow Eval::s_import_from(const syntax::ImportFrom* st, const State& state) {
    State s = state;
    const std::string base = p.from_base(*m, *st);
    for (const syntax::alias* a : st->names) {
        const std::string local = a->has_asname ? a->asname : a->name;
        const std::string full = !base.empty() ? base + "." + a->name : a->name;
        if (p.modules.contains(full)) {
            local_imports.insert_or_assign(local, Ref{Ref::Kind::Module, full, {}});
        } else if (p.modules.contains(base)) {
            const std::optional<Ref> r = p.resolve_global(*p.modules.at(base), a->name);
            local_imports.insert_or_assign(local, r ? *r : Ref{Ref::Kind::Missing, full, {}});
        } else {
            const bool std_lib = program::is_stdlib_module(first_part(full));
            local_imports.insert_or_assign(local, Ref{std_lib ? Ref::Kind::StdObject : Ref::Kind::Extern, full, {}});
        }
        s.pop(local);
    }
    return s;
}

Flow Eval::s_if(const syntax::If* st, const State& state) {
    ev(st->test, state);
    const Flow cur = after_access(st->test, state);
    const Flow a = block(st->body, refine(st->test, cur, true));
    const Flow b = block(st->orelse, refine(st->test, cur, false));
    return join_states(a, b);
}

Flow Eval::s_with(const syntax::With* st, const State& state) {
    State s = state;
    for (const syntax::withitem* item : st->items) {
        const Type t = ev(item->context_expr, s);
        if (!(t.kind == Kind::Extern && t.name && *t.name == "file")) issue(st, IssueClass::Syntax, "with");
        if (item->optional_vars != nullptr) s = assign(item->optional_vars, t, s);
    }
    return block(st->body, s);
}

// The exceptions of the standard library the front end maps: `except struct.error`.
bool Eval::std_exception(const syntax::Expr* n) {
    const auto* attribute = syntax::as<syntax::Attribute>(n);
    if (attribute == nullptr || syntax::as<syntax::Name>(attribute->value) == nullptr) return false;
    const std::string& module = syntax::cast<syntax::Name>(attribute->value)->id;
    const std::string& name = attribute->attr;
    return (module == "struct" && name == "error") || (module == "subprocess" && name == "TimeoutExpired") ||
           (module == "subprocess" && name == "CalledProcessError") || (module == "json" && name == "JSONDecodeError") ||
           (module == "zlib" && name == "error");
}

Flow Eval::s_try(const syntax::Try* st, const State& state) {
    if (!st->finalbody.empty()) issue(st, IssueClass::Syntax, "finally");
    if (!st->orelse.empty()) issue(st, IssueClass::Syntax, "try-else");
    const Flow body = block(st->body, state);
    // a handler can be entered from anywhere in the body: its state is what the body had at entry joined with every
    // state along the body (approximated by entry joined with the end of the body)
    const Flow entry = body ? join_states(Flow(state), body) : Flow(state);
    std::vector<Flow> outs = {body};
    for (const syntax::ExceptHandler* h : st->handlers) {
        State handler_state = *entry;
        if (h->type != nullptr) {
            std::vector<const syntax::Expr*> names;
            if (const auto* tuple = syntax::as<syntax::Tuple>(h->type)) {
                names.assign(tuple->elts.begin(), tuple->elts.end());
            } else {
                names.push_back(h->type);
            }
            for (const syntax::Expr* n : names) {
                const auto* name = syntax::as<syntax::Name>(n);
                const bool known = name != nullptr && (program::is_builtin_exception(name->id) || lookup_class(n).has_value());
                if (!known && !std_exception(n)) issue(h, IssueClass::Dynamic, "except " + pyast::describe_expr(n));
            }
            if (h->has_name) {
                issue(h, IssueClass::Syntax, "except as");
                handler_state.set(h->name, named_type(Kind::Extern, "exception"));
            }
        }
        outs.push_back(block(h->body, handler_state));
    }
    if (!st->orelse.empty()) outs[0] = outs[0] ? block(st->orelse, outs[0]) : std::nullopt;
    Flow joined;
    for (const Flow& o : outs) joined = join_states(joined, o);
    return joined;
}

Flow Eval::s_break(const State& state) {
    if (!loops.empty()) loops.back().breaks.push_back(state);
    return std::nullopt;
}

Flow Eval::s_continue(const State& state) {
    if (!loops.empty()) loops.back().continues.push_back(state);
    return std::nullopt;
}

Flow Eval::s_while(const syntax::While* st, const State& state) {
    if (!st->orelse.empty()) issue(st, IssueClass::Syntax, "while-else");
    const BodyFn body = [this, st](const State& head) -> Flow {
        ev(st->test, head);
        const Flow after = after_access(st->test, head);
        return block(st->body, refine(st->test, after, true));
    };
    const BodyFn exit_refine = [this, st](const State& head) -> Flow { return refine(st->test, Flow(head), false); };
    const BoundFn bound = [this, st](const State& head, const State& init) { return while_bound(st, head, init); };
    return loop(st, state, body, exit_refine, bound);
}

Flow Eval::s_for(const syntax::For* st, const State& state) {
    if (!st->orelse.empty()) issue(st, IssueClass::Syntax, "for-else");
    const Type iterated = ev(st->iter, state);
    const auto [element, passes] = iteration(st->iter, iterated);
    const Flow cur = after_access(st->iter, state);
    const BodyFn body = [this, st, element = element](const State& head) -> Flow {
        return block(st->body, assign(st->target, element, head));
    };
    const BodyFn exit_refine = [](const State& head) -> Flow { return head; };
    const BoundFn bound = [count = passes](const State&, const State&) { return count; };
    return loop(st, *cur, body, exit_refine, bound);
}

}  // namespace cb::translate::frontends::python::infer

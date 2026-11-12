// cb/translate/frontends/python/infer_loop.cpp -- loops: run a body to a fixpoint (widening integer bounds, then
// narrowing them back), the join of flow states, and the counter rule that bounds a variable a loop only ever steps.
#include <algorithm>
#include <unordered_set>

#include "cb/translate/frontends/python/infer_util.hpp"
#include "cb/translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

namespace {

// Raises `quiet` for a loop's fixpoint passes (nothing recorded, nothing contributed) and restores it, also on a throw.
class QuietScope {
public:
    explicit QuietScope(int& quiet) : quiet_(quiet) { ++quiet_; }
    ~QuietScope() { --quiet_; }
    QuietScope(const QuietScope&) = delete;
    QuietScope& operator=(const QuietScope&) = delete;

private:
    int& quiet_;
};

bool is_loop(const syntax::Stmt* st) {
    return st->kind == syntax::StmtKind::For || st->kind == syntax::StmtKind::AsyncFor || st->kind == syntax::StmtKind::While;
}

const std::vector<syntax::Stmt*>& loop_body(const syntax::Stmt* loop_stmt) {
    return loop_stmt->kind == syntax::StmtKind::While ? syntax::cast<syntax::While>(loop_stmt)->body
                                                       : syntax::cast<syntax::For>(loop_stmt)->body;
}

const Type* seen_type(const std::unordered_map<const void*, Type>& seen, const syntax::Node* node) {
    const auto it = seen.find(node);
    return it == seen.end() ? nullptr : &it->second;
}

// The number of passes a `for` over these types makes, as the loop helpers add them.
using Passes = Eval::Passes;
Passes finite_passes(const Wide& n) { return Passes{false, n}; }
Passes unbounded_passes() { return Passes{true, Wide(0)}; }
Passes fewer_passes(const Passes& a, const Passes& b) {
    if (a.infinite) return b;
    if (b.infinite) return a;
    return a.count <= b.count ? a : b;
}
Passes sum_passes(const Passes& a, const Passes& b) {
    if (a.infinite || b.infinite) return unbounded_passes();
    return finite_passes(a.count + b.count);
}

}  // namespace

// Run a loop body to a fixpoint; the last pass (with final heads) records and contributes. Inside an enclosing loop's
// fixpoint (quiet), a loop met again with the same state at entry gives the same state at exit: that is remembered
// rather than run again.
Flow Eval::loop(const syntax::Stmt* st, const State& init, const BodyFn& body, const BodyFn& exit_refine, const BoundFn& bound) {
    std::optional<MemoKey> memo_key;
    if (quiet > 0) {
        MemoKey key;
        key.st = st;
        for (const auto& [name, type] : init.items()) key.items.emplace_back(name, p.canon(type));
        std::ranges::sort(key.items, [](const auto& a, const auto& b) { return a.first < b.first; });
        const auto got = loop_memo.find(key);
        if (got != loop_memo.end()) return got->second;
        memo_key = std::move(key);
    }
    State head = init;
    {
        QuietScope quiet_scope(quiet);
        Program::SuspendScope suspend_scope(p);
        head = fixpoint(st, init, head, body, bound);
    }
    loops.emplace_back();
    body(head);   // the pass that counts
    LoopExits exits = std::move(loops.back());
    loops.pop_back();
    Flow out = exit_refine(head);
    for (const State& b : exits.breaks) out = join_states(out, Flow(b));
    if (memo_key) loop_memo[*memo_key] = *out;
    return out;
}

State Eval::fixpoint(const syntax::Stmt* st, const State& init, State head, const BodyFn& body, const BoundFn& bound) {
    // One pass over the body from `from`: the state back at the loop's head (continues joined in), widened or not.
    const auto pass = [&](const State& from) {
        loops.emplace_back();
        Flow back = body(from);
        LoopExits exits = std::move(loops.back());
        loops.pop_back();
        for (const State& c : exits.continues) back = join_states(back, Flow(c));
        const Flow joined = back ? join_states(Flow(init), back) : Flow(init);
        return merge_head(from, *joined);
    };
    bool widened = false;
    for (int i = 0; i < kMaxLoopPasses; ++i) {
        State incoming = pass(head);
        if (i >= 1) {
            incoming = widen_states(head, incoming);
            widened = true;
        }
        if (same_state(incoming, head)) break;
        if (i == kMaxLoopPasses - 1) {   // it does not settle: what still moves is unknown
            for (auto& [name, type] : incoming.items()) {
                const Type* before = head.find(name);
                if (before != nullptr && p.canon(*before) != p.canon(type)) {
                    type = type.kind == Kind::Int ? P::int_range() : P::top("a value a loop keeps changing");
                }
            }
        }
        head = std::move(incoming);
    }
    if (widened) {   // narrowing: a decreasing pass
        const State decreased = pass(head);
        State narrowed;
        for (const auto& [name, type] : decreased.items()) narrowed.set(name, narrow_one(head.find(name), type));
        head = counter_rule(st, narrowed, init, bound);
    }
    return head;
}

State Eval::merge_head(const State& head, State incoming) {
    for (const auto& [name, type] : head.items()) {
        if (!incoming.contains(name)) incoming.set(name, type);
    }
    return incoming;
}

Type Eval::narrow_one(const Type* old_type, const Type& incoming) {
    if (old_type == nullptr || old_type->kind != Kind::Int || incoming.kind != Kind::Int) return incoming;
    // a bound the widening gave up is taken back from the decreasing pass
    return P::int_range(old_type->lo ? old_type->lo : incoming.lo, old_type->hi ? old_type->hi : incoming.hi);
}

State Eval::widen_states(const State& old_state, const State& incoming) {
    State out = incoming;
    for (const auto& [name, type] : incoming.items()) {
        const Type* before = old_state.find(name);
        if (before != nullptr && before->kind == Kind::Int && type.kind == Kind::Int && *before != type) {
            out.set(name, P::widen_int(*before, type));
        }
    }
    return out;
}

bool Eval::same_state(const State& a, const State& b) {
    if (!a.same_keys(b)) return false;
    return std::ranges::all_of(a.items(), [&](const auto& kv) { return p.canon(kv.second) == p.canon(b.at(kv.first)); });
}

Flow Eval::join_states(const Flow& a, const Flow& b) {
    if (!a) return b;
    if (!b) return a;
    State out = *a;
    for (const auto& [name, type] : b->items()) {
        const Type* current = out.find(name);
        out.set(name, current != nullptr ? p.join(*current, type) : type);
    }
    return out;
}

// A variable a loop only ever steps by a bounded amount, in a loop of a bounded number of passes, is bounded by its
// start plus steps times passes.
State Eval::counter_rule(const syntax::Stmt* st, State head, const State& init, const BoundFn& bound) {
    std::vector<std::string> loose;
    for (const auto& [name, type] : head.items()) {
        if (type.kind == Kind::Int && (!type.hi || !type.lo)) loose.push_back(name);
    }
    if (loose.empty()) return head;
    const std::optional<Wide> passes = bound(head, init);
    if (!passes) return head;
    const std::vector<syntax::Stmt*>& body = loop_body(st);
    for (const std::string& name : loose) {
        const auto step = step_bound(body, name);
        const Type* start = init.find(name);
        if (!step || start == nullptr || start->kind != Kind::Int) continue;
        const auto& [up, down] = *step;
        const Bound lo = down == Wide(0) ? start->lo : (start->lo ? Bound(*start->lo - down * *passes) : Bound());
        const Bound hi = up == Wide(0) ? start->hi : (start->hi ? Bound(*start->hi + up * *passes) : Bound());
        const Type current = head.at(name);
        head.set(name, P::int_range(current.lo ? current.lo : lo, current.hi ? current.hi : hi));
    }
    return head;
}

std::optional<std::pair<Wide, Wide>> Eval::step_bound(const std::vector<syntax::Stmt*>& stmts, const std::string& var) {
    Wide up(0), down(0);
    for (const syntax::Stmt* st : stmts) {
        const auto step = step_one(st, var);
        if (!step) return std::nullopt;
        up = up + step->first;
        down = down + step->second;
    }
    return std::make_pair(up, down);
}

std::optional<std::pair<Wide, Wide>> Eval::step_one(const syntax::Stmt* st, const std::string& var) {
    if (const auto* aug = syntax::as<syntax::AugAssign>(st)) {
        const auto* target = syntax::as<syntax::Name>(aug->target);
        if (target != nullptr && target->id == var) {
            const Type* step = seen_type(seen, aug->value);
            if (step == nullptr || step->kind != Kind::Int || !step->lo || !step->hi) return std::nullopt;
            if (aug->op == syntax::OpKind::Add) return std::make_pair(std::max(*step->hi, Wide(0)), std::max(-*step->lo, Wide(0)));
            if (aug->op == syntax::OpKind::Sub) return std::make_pair(std::max(-*step->lo, Wide(0)), std::max(*step->hi, Wide(0)));
            return std::nullopt;
        }
    }
    if (is_loop(st)) {
        const auto inner = step_bound(loop_body(st), var);
        if (!inner) return std::nullopt;
        if (inner->first == Wide(0) && inner->second == Wide(0)) return std::make_pair(Wide(0), Wide(0));
        std::optional<Wide> passes;
        if (st->kind != syntax::StmtKind::While) passes = range_count(syntax::cast<syntax::For>(st)->iter);
        if (!passes) return std::nullopt;
        return std::make_pair(inner->first * *passes, inner->second * *passes);
    }
    bool found_aug = false;
    for (const pyast::AnyNode& an : pyast::walk(pyast::AnyNode(st))) {
        if (const syntax::Expr* e = an.expr()) {
            const auto* name = syntax::as<syntax::Name>(e);
            if (name != nullptr && name->id == var && name->ctx == syntax::ExprCtx::Store) return std::nullopt;
        } else if (const syntax::Stmt* s = an.stmt()) {
            const auto* aug = syntax::as<syntax::AugAssign>(s);
            const auto* target = aug != nullptr ? syntax::as<syntax::Name>(aug->target) : nullptr;
            if (target != nullptr && target->id == var) {
                found_aug = true;
                break;
            }
        }
    }
    if (!found_aug) return std::make_pair(Wide(0), Wide(0));
    if (const auto* branch = syntax::as<syntax::If>(st)) {
        const auto then_step = step_bound(branch->body, var);
        const auto else_step = step_bound(branch->orelse, var);
        if (!then_step || !else_step) return std::nullopt;
        return std::make_pair(std::max(then_step->first, else_step->first), std::max(then_step->second, else_step->second));
    }
    if (st->kind == syntax::StmtKind::Try && syntax::cast<syntax::Try>(st)->finalbody.empty()) {
        const auto* tr = syntax::cast<syntax::Try>(st);
        std::vector<std::optional<std::pair<Wide, Wide>>> parts = {step_bound(tr->body, var)};
        for (const syntax::ExceptHandler* h : tr->handlers) parts.push_back(step_bound(h->body, var));
        Wide up(0), down(0);
        for (const auto& part : parts) {
            if (!part) return std::nullopt;
            up = up + part->first;
            down = down + part->second;
        }
        return std::make_pair(up, down);
    }
    return std::nullopt;
}

// The passes of `for x in <it>`, when something bounds them.
std::optional<Wide> Eval::range_count(const syntax::Expr* it) {
    const Type* iterated = seen_type(seen, it);
    const auto* call = syntax::as<syntax::Call>(it);
    if (call != nullptr && is_name(call->func, "range")) {
        std::vector<const Type*> args;
        for (const syntax::Expr* a : call->args) {
            const Type* t = seen_type(seen, a);
            if (t == nullptr || t->kind != Kind::Int) return std::nullopt;
            args.push_back(t);
        }
        if (args.size() == 1) return args[0]->hi ? std::optional<Wide>(std::max(*args[0]->hi, Wide(0))) : std::nullopt;
        if (args.size() >= 2) {
            const std::optional<Wide> step = call->args.size() == 3 ? int_constant(call->args[2]) : std::optional<Wide>(Wide(1));
            if (!step || *step == Wide(0)) return std::nullopt;
            const Type& first = *args[0];
            const Type& stop = *args[1];
            if (*step > Wide(0)) {
                if (!stop.hi || !first.lo) return std::nullopt;
                return std::max(floor_div(*stop.hi - *first.lo + *step - Wide(1), *step), Wide(0));
            }
            if (!first.hi || !stop.lo) return std::nullopt;
            return std::max(floor_div(*first.hi - *stop.lo - *step - Wide(1), -*step), Wide(0));
        }
    }
    if (iterated != nullptr &&
        is_one_of(*iterated, {Kind::List, Kind::Tuple, Kind::Str, Kind::Bytes, Kind::Bytearray, Kind::Dict, Kind::Set})) {
        return P::mem();
    }
    if (call != nullptr) {
        std::optional<std::string_view> name;
        if (const auto* n = syntax::as<syntax::Name>(call->func)) name = n->id;
        else if (const auto* a = syntax::as<syntax::Attribute>(call->func)) name = a->attr;
        if (name && word_in(*name, {"enumerate", "zip", "reversed", "sorted", "items", "keys", "values"})) return P::mem();
    }
    return std::nullopt;
}

// A `while var < bound:` whose body steps var up every pass runs a bounded number of times.
std::optional<Wide> Eval::while_bound(const syntax::While* st, const State& head, const State& init) {
    (void)head;
    const syntax::Expr* test = st->test;
    if (const auto* both = syntax::as<syntax::BoolOp>(test); both != nullptr && both->op == syntax::BoolOpKind::And) {
        for (const syntax::Expr* v : both->values) {
            if (auto r = cmp_bound(v, st, init)) return r;
        }
        return std::nullopt;
    }
    return cmp_bound(test, st, init);
}

std::optional<Wide> Eval::cmp_bound(const syntax::Expr* t, const syntax::While* st, const State& init) {
    const auto* cmp = syntax::as<syntax::Compare>(t);
    if (cmp == nullptr || cmp->ops.size() != 1 || syntax::as<syntax::Name>(cmp->left) == nullptr) return std::nullopt;
    const std::string& var = syntax::cast<syntax::Name>(cmp->left)->id;
    const syntax::CmpOpKind op = cmp->ops[0];
    if (op != syntax::CmpOpKind::Lt && op != syntax::CmpOpKind::LtE) return std::nullopt;
    const Passes step = min_step(st->body, var);
    if (!step.infinite && step.count == Wide(0)) return std::nullopt;
    if (step.infinite) return Wide(1);
    const Type* limit = seen_type(seen, cmp->comparators[0]);
    const Type* start = init.find(var);
    if (limit == nullptr || limit->kind != Kind::Int || !limit->hi || start == nullptr || start->kind != Kind::Int || !start->lo) {
        return std::nullopt;
    }
    return std::max(floor_div(*limit->hi - *start->lo, step.count) + Wide(2), Wide(0));
}

Eval::Passes Eval::min_step(const std::vector<syntax::Stmt*>& stmts, const std::string& var) {
    const auto r = paths(stmts, var);
    if (!r) return finite_passes(Wide(0));
    return fewer_passes(r->first, r->second);
}

// (the least var grows by falling off the end of stmts, the least it grows by each `continue`)
std::optional<std::pair<Eval::Passes, Eval::Passes>> Eval::paths(const std::vector<syntax::Stmt*>& stmts, const std::string& var) {
    Passes acc = finite_passes(Wide(0));
    Passes cont = unbounded_passes();
    for (const syntax::Stmt* st : stmts) {
        if (st->kind == syntax::StmtKind::Break || st->kind == syntax::StmtKind::Return || st->kind == syntax::StmtKind::Raise) {
            return std::make_pair(unbounded_passes(), cont);
        }
        if (st->kind == syntax::StmtKind::Continue) return std::make_pair(unbounded_passes(), fewer_passes(cont, acc));
        const auto* aug = syntax::as<syntax::AugAssign>(st);
        const auto* target = aug != nullptr ? syntax::as<syntax::Name>(aug->target) : nullptr;
        if (target != nullptr && target->id == var) {
            const Type* step = seen_type(seen, aug->value);
            if (aug->op != syntax::OpKind::Add || step == nullptr || step->kind != Kind::Int || !step->lo || step->lo->sign() < 0) {
                return std::nullopt;
            }
            acc = sum_passes(acc, finite_passes(*step->lo));
        } else if (const auto* branch = syntax::as<syntax::If>(st)) {
            const auto then_paths = paths(branch->body, var);
            const auto else_paths = paths(branch->orelse, var);
            if (!then_paths || !else_paths) return std::nullopt;
            cont = fewer_passes(fewer_passes(cont, sum_passes(acc, then_paths->second)), sum_passes(acc, else_paths->second));
            const Passes fall = fewer_passes(then_paths->first, else_paths->first);
            if (fall.infinite) return std::make_pair(unbounded_passes(), cont);
            acc = sum_passes(acc, fall);
        } else if (!grows_only(st, var)) {
            return std::nullopt;
        }
    }
    return std::make_pair(acc, cont);
}

// Does the statement touch `var` only by adding a non-negative amount (and no `continue` outside a loop of its own)?
bool Eval::grows_only(const syntax::Stmt* st, const std::string& var) {
    const std::vector<pyast::AnyNode> nodes = pyast::walk(pyast::AnyNode(st));
    std::unordered_set<const void*> aug_targets;
    for (const pyast::AnyNode& an : nodes) {
        if (const syntax::Stmt* s = an.stmt()) {
            if (const auto* aug = syntax::as<syntax::AugAssign>(s)) aug_targets.insert(aug->target);
        }
    }
    const bool in_loop = is_loop(st);
    for (const pyast::AnyNode& an : nodes) {
        if (const syntax::Expr* e = an.expr()) {
            const auto* name = syntax::as<syntax::Name>(e);
            if (name != nullptr && name->id == var && name->ctx == syntax::ExprCtx::Store && !aug_targets.contains(e)) return false;
        } else if (const syntax::Stmt* s = an.stmt()) {
            if (const auto* aug = syntax::as<syntax::AugAssign>(s)) {
                const auto* target = syntax::as<syntax::Name>(aug->target);
                if (target != nullptr && target->id == var) {
                    const Type* step = seen_type(seen, aug->value);
                    if (aug->op != syntax::OpKind::Add || step == nullptr || step->kind != Kind::Int || !step->lo ||
                        step->lo->sign() < 0) {
                        return false;
                    }
                }
            }
            if (s->kind == syntax::StmtKind::Continue && !in_loop) return false;
        }
    }
    return true;
}

// The type of one element of a value iterated over, and how many passes that makes at most.
std::pair<Type, std::optional<Wide>> Eval::iteration(const syntax::Expr* node, const Type& t) {
    switch (t.kind) {
        case Kind::Bottom: return {P::bottom(), std::nullopt};
        case Kind::Top: return {t, std::nullopt};
        case Kind::List:
        case Kind::Set:
            if (t.kind == Kind::Set) issue(node, IssueClass::Dynamic, "set iteration order");
            return {p.root(t.site.value()).elem, P::mem()};
        case Kind::Dict: {
            const program::SiteId root = p.find(t.site.value());
            p.make_dynamic(root, "iterated");
            return {p.site(root).keys, P::mem()};
        }
        case Kind::Tuple: {
            Type element = P::bottom();
            for (const Type& a : t.args) element = p.join(element, a);
            if (element.kind == Kind::Top) return {issue(node, IssueClass::Conflict, "iterating a tuple of mixed items"), std::nullopt};
            return {element, Wide(static_cast<std::int64_t>(t.args.size()))};
        }
        case Kind::Str: return {P::str(), P::mem()};
        case Kind::Bytes:
        case Kind::Bytearray: return {P::unsigned_int(8), t.hi ? *t.hi : P::mem()};
        case Kind::Iter: return {t.args[0], t.lo};
        default: break;
    }
    return {issue(node, IssueClass::Conflict, "iterating a " + std::string(P::kind_word(t.kind))), std::nullopt};
}

}  // namespace cb::translate::frontends::python::infer

// translate/frontends/python/infer_assign.cpp -- what an assignment teaches (locals, attributes, items, unpackings),
// and what a test or a successful access tells about the variables it names.
#include <algorithm>
#include <format>
#include <unordered_set>

#include "core/error.hpp"
#include "translate/frontends/python/infer_util.hpp"
#include "translate/frontends/python/pyast.hpp"

namespace cb::translate::frontends::python::infer {

namespace {

bool is_container(const Type& t) { return is_one_of(t, {Kind::List, Kind::Dict, Kind::Set}); }

enum class Relation { Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual };

Relation negated(Relation r) {
    switch (r) {
        case Relation::Less: return Relation::GreaterEqual;
        case Relation::LessEqual: return Relation::Greater;
        case Relation::Greater: return Relation::LessEqual;
        case Relation::GreaterEqual: return Relation::Less;
        case Relation::Equal: return Relation::NotEqual;
        case Relation::NotEqual: return Relation::Equal;
    }
    return r;
}

// The relation read from the other side: a < b is b > a.
Relation mirrored(Relation r) {
    switch (r) {
        case Relation::Less: return Relation::Greater;
        case Relation::LessEqual: return Relation::GreaterEqual;
        case Relation::Greater: return Relation::Less;
        case Relation::GreaterEqual: return Relation::LessEqual;
        default: return r;
    }
}

// `v` limited by `v <relation> other`.
Type limited_by(const Type& v, Relation relation, const Type& other) {
    switch (relation) {
        case Relation::Less: return P::meet(v, std::nullopt, other.hi ? Bound(*other.hi - Wide(1)) : Bound());
        case Relation::LessEqual: return P::meet(v, std::nullopt, other.hi);
        case Relation::Greater: return P::meet(v, other.lo ? Bound(*other.lo + Wide(1)) : Bound(), std::nullopt);
        case Relation::GreaterEqual: return P::meet(v, other.lo, std::nullopt);
        case Relation::Equal: return P::meet(v, other.lo, other.hi);
        case Relation::NotEqual: break;
    }
    return v;
}

std::optional<Relation> relation_of(syntax::CmpOpKind op) {
    using C = syntax::CmpOpKind;
    switch (op) {
        case C::Lt: return Relation::Less;
        case C::LtE: return Relation::LessEqual;
        case C::Gt: return Relation::Greater;
        case C::GtE: return Relation::GreaterEqual;
        case C::Eq: return Relation::Equal;
        case C::NotEq: return Relation::NotEqual;
        default: return std::nullopt;
    }
}

const Type* local_int(const State& state, const syntax::Name* name) {
    if (name == nullptr) return nullptr;
    const Type* t = state.find(name->id);
    return t != nullptr && t->kind == Kind::Int ? t : nullptr;
}

}  // namespace

State Eval::assign(const syntax::Expr* target, const Type& value, State state, bool aug) {
    Type t = value;
    if (const auto* name = syntax::as<syntax::Name>(target)) {
        state.set(name->id, t);
        note(target, t, Res::of(ResKind::Local));
        if (is_container(t)) p.hint(t.site.value(), name->id);
        return state;
    }
    if (const auto* attribute = syntax::as<syntax::Attribute>(target)) {
        const Type owner = ev(attribute->value, state);
        const std::string& attr = attribute->attr;
        if (owner.kind == Kind::Inst) {
            t = fit(target, t, "attribute " + attr);
            contribute_attr(*owner.name, attr, t);
            note(target, t, Res::of(ResKind::Field, *owner.name, attr));
            if (is_container(t)) p.hint(t.site.value(), attr);
        } else if (owner.kind == Kind::Top) {
            note(target, owner);
        } else if (owner.kind != Kind::Bottom) {
            note(target, issue(target, IssueClass::Dynamic, "attribute store on a " + kind_text(owner)));
        }
        return state;
    }
    if (const auto* subscript = syntax::as<syntax::Subscript>(target)) {
        const Type container = ev(subscript->value, state);
        if (const auto* slice = syntax::as<syntax::Slice>(subscript->slice)) {
            ev_slice_parts(slice, state);
            if (container.kind == Kind::List && t.kind == Kind::List) {
                p.grow(program::Table::Elem, container.site.value(), p.root(t.site.value()).elem);
            } else if (container.kind == Kind::Bytearray && is_one_of(t, {Kind::Bytes, Kind::Bytearray})) {
                // a byte string stored over a range of a bytearray
            } else if (container.kind != Kind::Bottom) {
                issue(target, IssueClass::Syntax, "slice assignment to a " + kind_text(container));
            }
            note(target, t, Res::of(ResKind::SliceStore, kind_text(container)));
            return state;
        }
        const Type index = ev(subscript->slice, state);
        if (container.kind == Kind::List) {
            p.grow(program::Table::Elem, container.site.value(), t);
            need_int_index(subscript->slice, index);
            note(target, t, Res::of(ResKind::IndexStore, "list"));
        } else if (container.kind == Kind::Bytearray) {
            if (t.kind != Kind::Int && t.kind != Kind::Bottom) issue(target, IssueClass::Conflict, "bytearray item of " + kind_text(t));
            note(target, P::unsigned_int(8), Res::of(ResKind::IndexStore, "bytearray"));
        } else if (container.kind == Kind::Dict) {
            const std::optional<std::string> key = pyast::string_constant(subscript->slice);
            const program::SiteId root = p.find(container.site.value());
            t = fit(target, t, "field " + key.value_or("None"));
            if (key) {
                p.grow_field(root, *key, t);
                if ((aug || !p.builder_targets.contains(target)) && live()) p.mark_mutated(root);
            } else {
                p.make_dynamic(root, "a key that is not a constant string");
                p.grow(program::Table::Keys, root, index);
                p.grow(program::Table::Elem, root, t);
            }
            note(target, t, Res::of(ResKind::DictStore, key));
        } else if (!is_one_of(container, {Kind::Bottom, Kind::Top})) {
            issue(target, IssueClass::Conflict, "item store on a " + kind_text(container));
        }
        return state;
    }
    const std::vector<syntax::Expr*>* elts = nullptr;
    if (const auto* tuple = syntax::as<syntax::Tuple>(target)) {
        elts = &tuple->elts;
    } else if (const auto* list = syntax::as<syntax::List>(target)) {
        elts = &list->elts;
    }
    if (elts == nullptr) {
        issue(target, IssueClass::Syntax, "assignment to " + std::string(syntax::name_of(target->kind)));
        return state;
    }
    for (const syntax::Expr* e : *elts) {
        if (syntax::as<syntax::Starred>(e) != nullptr) {
            issue(target, IssueClass::Syntax, "starred unpacking");
            return state;
        }
    }
    if (t.kind == Kind::Tuple) {
        if (t.args.size() != elts->size()) {
            issue(target, IssueClass::Conflict, std::format("unpacking {} items into {}", t.args.size(), elts->size()));
            return state;
        }
        for (std::size_t i = 0; i < elts->size(); ++i) state = assign((*elts)[i], t.args[i], state);
        note(target, t, Res::of(ResKind::Unpack));
        return state;
    }
    if (is_one_of(t, {Kind::Bottom, Kind::Top})) {
        for (const syntax::Expr* e : *elts) state = assign(e, t, state);
        return state;
    }
    issue(target, IssueClass::Conflict, "unpacking a " + kind_text(t) + " (its length is not known to the type)");
    for (const syntax::Expr* e : *elts) state = assign(e, P::top("unpack"), state);
    return state;
}

// A record made empty (or partly) and filled by the statements right after it: the keys those statements set are set by
// every creation. (Called with the Assign; scans its block.)
void Eval::builder_keys(const syntax::Assign* st, const Type& t) {
    if (t.kind != Kind::Dict) return;
    const std::string& name = syntax::cast<syntax::Name>(st->targets[0])->id;
    const std::vector<syntax::Stmt*>* statements = p.block_of(st);
    if (statements == nullptr) return;
    program::StringSet keys;
    if (const auto literal = p.literal_keys.find(st->value); literal != p.literal_keys.end()) {
        keys.insert(literal->second.begin(), literal->second.end());
    }
    const auto here = std::ranges::find(*statements, static_cast<const syntax::Stmt*>(st));
    CB_ASSERT(here != statements->end(), "an assignment is in the statement list that holds it");
    for (auto it = here + 1; it != statements->end(); ++it) {
        const syntax::Stmt* next = *it;
        const auto* store = syntax::as<syntax::Assign>(next);
        const syntax::Subscript* item = nullptr;
        if (store != nullptr && store->targets.size() == 1) item = syntax::as<syntax::Subscript>(store->targets[0]);
        const auto* target_name = item != nullptr ? syntax::as<syntax::Name>(item->value) : nullptr;
        const std::optional<std::string> key = item != nullptr ? pyast::string_constant(item->slice) : std::nullopt;
        if (target_name != nullptr && target_name->id == name && key && reads_fields_only(store->value, name)) {
            keys.insert(*key);
            p.builder_targets.insert(item);
            continue;
        }
        // any other use of the record ends the run of statements that fill it
        const std::vector<pyast::AnyNode> nodes = pyast::walk(pyast::AnyNode(next));
        const bool uses = std::ranges::any_of(nodes, [&](const pyast::AnyNode& an) {
            const auto* n = syntax::as<syntax::Name>(an.expr());
            return n != nullptr && n->id == name;
        });
        if (uses) break;
    }
    if (live()) p.created(t.site.value(), keys);
}

// `value` uses the record `name` only to read its fields by constant key.
bool Eval::reads_fields_only(const syntax::Expr* value, const std::string& name) {
    const std::vector<pyast::AnyNode> nodes = pyast::walk(pyast::AnyNode(value));
    std::unordered_set<const void*> field_reads;
    for (const pyast::AnyNode& an : nodes) {
        const auto* item = syntax::as<syntax::Subscript>(an.expr());
        const auto* owner = item != nullptr ? syntax::as<syntax::Name>(item->value) : nullptr;
        if (owner != nullptr && owner->id == name && pyast::string_constant(item->slice) && item->ctx == syntax::ExprCtx::Load) {
            field_reads.insert(item->value);
        }
    }
    return std::ranges::none_of(nodes, [&](const pyast::AnyNode& an) {
        const auto* n = syntax::as<syntax::Name>(an.expr());
        return n != nullptr && n->id == name && !field_reads.contains(n);
    });
}

// ---- refinement ------------------------------------------------------------------------------------------------

Flow Eval::refine(const syntax::Expr* test, const Flow& state, bool truth) {
    if (!state) return std::nullopt;
    if (const auto* unary = syntax::as<syntax::UnaryOp>(test); unary != nullptr && unary->op == syntax::UnaryOpKind::Not) {
        return refine(unary->operand, state, !truth);
    }
    if (const auto* boolean = syntax::as<syntax::BoolOp>(test)) {
        if ((boolean->op == syntax::BoolOpKind::And) == truth) {   // and/True, or/False: every part holds
            Flow s = state;
            for (const syntax::Expr* v : boolean->values) s = refine(v, s, truth);
            return s;
        }
        Flow outs;   // one of them failed: join each case
        Flow s = state;
        for (const syntax::Expr* v : boolean->values) {
            outs = join_states(outs, refine(v, s, truth));
            s = refine(v, s, !truth);
        }
        return outs;
    }
    if (const auto* name = syntax::as<syntax::Name>(test); name != nullptr && state->contains(name->id)) {
        const Type t = state->at(name->id);
        State s = *state;
        if (t.kind == Kind::Opt && truth) {
            s.set(name->id, t.args[0]);
        } else if (t.kind == Kind::Opt && !truth && t.args[0].kind == Kind::Inst) {
            s.set(name->id, P::none());
        } else if (t.kind == Kind::Int && !truth) {
            s.set(name->id, P::meet(t, Wide(0), Wide(0)));
        }
        return s;
    }
    if (const auto* call = syntax::as<syntax::Call>(test);
        call != nullptr && is_name(call->func, "isinstance") && call->args.size() == 2 && syntax::as<syntax::Name>(call->args[0]) != nullptr) {
        return state;
    }
    if (const auto* compare = syntax::as<syntax::Compare>(test); compare != nullptr && !compare->ops.empty()) {
        const syntax::Expr* left = compare->left;
        State s = *state;
        for (std::size_t i = 0; i < compare->ops.size(); ++i) {
            const syntax::Expr* right = compare->comparators[i];
            s = refine_cmp(left, compare->ops[i], right, s, truth);
            if (!truth && compare->ops.size() > 1) return s;
            left = right;
        }
        return s;
    }
    return state;
}

State Eval::refine_cmp(const syntax::Expr* left, syntax::CmpOpKind op, const syntax::Expr* right, State state, bool truth) {
    using C = syntax::CmpOpKind;
    const auto* left_name = syntax::as<syntax::Name>(left);
    const bool right_none = pyast::is_none_constant(right);
    if ((op == C::Is || op == C::IsNot) && right_none && left_name != nullptr && state.contains(left_name->id)) {
        const bool is_none = (op == C::Is) == truth;
        state.set(left_name->id, is_none ? P::none() : P::strip_optional(state.at(left_name->id)));
        return state;
    }
    if ((op == C::Eq || op == C::NotEq) && right_none && left_name != nullptr && state.contains(left_name->id)) {
        return refine_cmp(left, op == C::Eq ? C::Is : C::IsNot, right, state, truth);
    }
    std::optional<Relation> relation = relation_of(op);
    if (!relation) return state;
    if (!truth) relation = negated(*relation);
    const auto seen_of = [this](const syntax::Expr* e) -> const Type* {
        const auto it = seen.find(e);
        return it == seen.end() ? nullptr : &it->second;
    };
    const Type* left_type = seen_of(left);
    const Type* right_type = seen_of(right);
    if (const Type* v = local_int(state, left_name); v != nullptr && right_type != nullptr && right_type->kind == Kind::Int) {
        state.set(left_name->id, limited_by(*v, *relation, *right_type));
    }
    const auto* right_name = syntax::as<syntax::Name>(right);
    if (const Type* v = local_int(state, right_name); v != nullptr && left_type != nullptr && left_type->kind == Kind::Int) {
        state.set(right_name->id, limited_by(*v, mirrored(*relation), *left_type));
    }
    return state;
}

// A successful index bounds the variable that indexed: data[p] returned, so p is inside data; so does passing it as an
// index parameter, or as the offset of an unpack_from/pack_into.
Flow Eval::after_access(const syntax::Expr* node, const Flow& incoming) {
    if (!incoming) return std::nullopt;
    State state = *incoming;
    const auto bound_to_memory = [&state](const syntax::Name* variable) {
        const Type* v = local_int(state, variable);
        if (v != nullptr) state.set(variable->id, P::meet(*v, -P::mem(), P::mem()));
    };
    for (const syntax::Expr* n : p.access_nodes(node)) {
        if (const auto* subscript = syntax::as<syntax::Subscript>(n)) {
            const auto* index = syntax::as<syntax::Name>(subscript->slice);
            if (index == nullptr || !state.contains(index->id)) continue;
            const auto indexed = seen.find(subscript->value);
            if (indexed == seen.end() || !is_one_of(indexed->second, {Kind::Bytes, Kind::Bytearray, Kind::List, Kind::Str, Kind::Tuple})) {
                continue;
            }
            const Type& sequence = indexed->second;
            const Type v = state.at(index->id);
            if (v.kind == Kind::Int) {
                const Wide limit = (!is_one_of(sequence, {Kind::Bytes, Kind::Bytearray}) || !sequence.hi) ? P::mem() : *sequence.hi;
                state.set(index->id, P::meet(v, -limit, limit - Wide(1)));
            }
        } else if (const auto* call = syntax::as<syntax::Call>(n)) {
            if (const std::optional<std::string> callee = res_fn(call)) {
                const auto indexes = p.index_params.find(*callee);
                const std::vector<std::string> names = p.fns.at(*callee)->bound();
                for (std::size_t i = 0; i < call->args.size() && i < names.size(); ++i) {
                    const auto* argument = syntax::as<syntax::Name>(call->args[i]);
                    if (indexes != p.index_params.end() && indexes->second.contains(names[i])) bound_to_memory(argument);
                }
            } else if (const auto* method = syntax::as<syntax::Attribute>(call->func);
                       method != nullptr && (method->attr == "unpack_from" || method->attr == "pack_into") && call->args.size() >= 3) {
                bound_to_memory(syntax::as<syntax::Name>(call->args[2]));
            }
        }
    }
    return state;
}

// The function a call resolved to, when it is a call of the program's.
std::optional<std::string> Eval::res_fn(const syntax::Expr* call) {
    const Res* r = nullptr;
    if (record) {
        const auto it = res.find(call);
        if (it != res.end()) r = &it->second;
    }
    if (r == nullptr) {
        const auto it = res_quiet.find(call);
        if (it != res_quiet.end()) r = &it->second;
    }
    if (r != nullptr && (r->kind == ResKind::Fn || r->kind == ResKind::Method)) return r->name;
    return std::nullopt;
}

}  // namespace cb::translate::frontends::python::infer

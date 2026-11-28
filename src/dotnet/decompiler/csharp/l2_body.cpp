// dotnet/decompiler/csharp/l2_body.cpp -- see l2_body.hpp.
#include "dotnet/decompiler/csharp/l2_body.hpp"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <variant>

#include "core/fs.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "translate/frontends/cil/cil_names.hpp"
#include "dotnet/decompiler/csharp/call_builder.hpp"
#include "dotnet/decompiler/csharp/expression_builder.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/tuple_transform.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"

namespace cb::dotnet::decompiler::csharp {

L2Lane::L2Lane() : check_(fs::get_env("CB_DN_L2_CHECK").has_value()) {}
L2Lane::~L2Lane() = default;

void L2Lane::keep(std::unique_ptr<L2Body> body) {
    if (body == nullptr) return;
    by_function_.get(body->function()).value = body.get();
    kept_.push_back(std::move(body));
}

L2Body* L2Lane::body_of(const il::ILInstruction* inst) const {
    const il::ILInstruction* root = inst;
    while (root != nullptr && root->parent() != nullptr) root = root->parent();
    const auto* f = il::as<il::ILFunction>(root);
    L2Body* const* body = f != nullptr ? by_function_.find(f) : nullptr;
    return body != nullptr ? *body : nullptr;
}

template <class T> T L2Lane::checked(T from_l2, T from_ilast, std::source_location where) {
    ++checks;
    if (!(from_l2 == from_ilast)) {
        ++check_differences;
        ++check_differences_at[where.line()];
    }
    return from_l2;
}

VariableFacts L2Lane::ilast_variable(const il::ILVariable& v) {
    return VariableFacts{v.kind(), v.type(), v.name, v.uses_initial_value(), v.initial_value_is_initialized(), v.is_ref_read_only,
                         v.load_count(), v.store_count(), v.address_count(), v.capture_scope, v.index()};
}

ContainerFacts L2Lane::ilast_container(const il::BlockContainer& c) {
    return ContainerFacts{c.entry_point()->incoming_edge_count(), il::is<il::ILFunction>(c.parent())};
}

bool L2Lane::ilast_hides_variable_with_name(const il::ILFunction& f, const std::string& name) {
    return ExpressionBuilder::hides_variable_with_name(const_cast<il::ILFunction*>(&f), name);
}

VariableFacts variable_of(L2Lane* l2, const il::ILVariable& v) { return l2 != nullptr ? l2->variable(v) : L2Lane::ilast_variable(v); }
ContainerFacts container_of(L2Lane* l2, const il::BlockContainer& c) { return l2 != nullptr ? l2->container(c) : L2Lane::ilast_container(c); }
il::ILFunctionKind function_kind_of(L2Lane* l2, const il::ILFunction& f) { return l2 != nullptr ? l2->function_kind(f) : f.kind(); }
il::BlockContainer* function_body_of(L2Lane* l2, const il::ILFunction& f) {
    return l2 != nullptr ? l2->function_body(f) : il::as<il::BlockContainer>(f.body());
}
std::vector<il::ILVariable*> captured_variables_of(L2Lane* l2, const il::ILFunction& f) {
    return l2 != nullptr ? l2->captured_variables(f) : std::vector<il::ILVariable*>(f.captured_variables.begin(), f.captured_variables.end());
}
il::BlockContainer* enclosing_container_of(L2Lane* l2, const il::ILInstruction& inst) {
    return l2 != nullptr ? l2->enclosing_container(inst) : il::BlockContainer::find_closest_container(inst.parent());
}
bool hides_variable_with_name_of(L2Lane* l2, const il::ILFunction& f, const std::string& name) {
    return l2 != nullptr ? l2->hides_variable_with_name(f, name) : L2Lane::ilast_hides_variable_with_name(f, name);
}

il::BlockContainer* L2Lane::enclosing_container(const il::ILInstruction& inst) {
    std::optional<il::BlockContainer*> l2;
    if (L2Body* body = body_of(&inst)) l2 = body->enclosing_container(inst);
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    il::BlockContainer* ilast = il::BlockContainer::find_closest_container(inst.parent());
    if (!l2) return ilast;
    return check_ ? checked(*l2, ilast) : *l2;
}

bool L2Lane::hides_variable_with_name(const il::ILFunction& f, const std::string& name) {
    std::optional<bool> l2;
    if (L2Body* body = body_of(&f)) l2 = body->hides_variable_with_name(f, name);
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast_hides_variable_with_name(f, name);
    return check_ ? checked(*l2, ilast_hides_variable_with_name(f, name)) : *l2;
}

void L2Lane::variable_registered(il::ILFunction* function, il::ILVariable* v) {
    if (L2Body* body = body_of(function)) body->variable_registered(function, v);
}

void L2Lane::variable_changed(il::ILVariable* v) {
    L2Body* body = v->function() != nullptr ? body_of(v->function()) : nullptr;
    if (body != nullptr && body->sym(v) != nullptr) body->variable_changed(v);
}

namespace {
// The kinds an instruction's node is, as lift.cpp decides them.
bool node_is_call(const ir::Node* n) {
    if (const auto* c = ir::get<ir::Call>(n)) return c->form != ir::CallForm::Indirect && !ir::has_flag(*n, ir::NodeFlag::PinnableRef);
    return ir::is<ir::New>(n);
}
bool node_is_user_defined_compound(const ir::Node* n) {
    const auto* a = ir::get<ir::Assign>(n);
    return a != nullptr && a->eval && a->sym;
}
bool node_is_dynamic_compound(const ir::Node* n) {
    const auto* i = ir::get<ir::Intrinsic>(n);
    return i != nullptr && i->name == "cs.dynamic.compound_assign";
}
}  // namespace

bool L2Lane::is_call_instruction(const il::ILInstruction& inst) {
    L2Body* body = body_of(&inst);
    const ir::Node* n = body != nullptr ? body->node(&inst) : nullptr;
    const bool ilast = dynamic_cast<const il::CallInstruction*>(&inst) != nullptr;
    ++(n != nullptr ? facts_from_l2 : facts_from_ilast);
    if (n == nullptr) return ilast;
    return check_ ? checked(node_is_call(n), ilast) : node_is_call(n);
}

bool L2Lane::is_user_defined_compound_assign(const il::ILInstruction& inst) {
    L2Body* body = body_of(&inst);
    const ir::Node* n = body != nullptr ? body->node(&inst) : nullptr;
    const bool ilast = il::is<il::UserDefinedCompoundAssign>(&inst);
    ++(n != nullptr ? facts_from_l2 : facts_from_ilast);
    if (n == nullptr) return ilast;
    return check_ ? checked(node_is_user_defined_compound(n), ilast) : node_is_user_defined_compound(n);
}

bool L2Lane::is_dynamic_compound_assign(const il::ILInstruction& inst) {
    L2Body* body = body_of(&inst);
    const ir::Node* n = body != nullptr ? body->node(&inst) : nullptr;
    const bool ilast = il::is<il::DynamicCompoundAssign>(&inst);
    ++(n != nullptr ? facts_from_l2 : facts_from_ilast);
    if (n == nullptr) return ilast;
    return check_ ? checked(node_is_dynamic_compound(n), ilast) : node_is_dynamic_compound(n);
}

const typesystem::IMethod* L2Lane::call_method(const il::CallInstruction& call) {
    L2Body* body = body_of(&call);
    const ir::Node* n = body != nullptr ? body->node(&call) : nullptr;
    const typesystem::IMethod* l2 = nullptr;
    if (const auto* c = ir::get<ir::Call>(n)) l2 = translate::frontends::cil::clr_method(c->sym);
    else if (const auto* nw = ir::get<ir::New>(n)) l2 = translate::frontends::cil::clr_method(nw->sym);
    ++(l2 != nullptr ? facts_from_l2 : facts_from_ilast);
    if (l2 == nullptr) return call.method;
    return check_ ? checked(l2, call.method) : l2;
}

const typesystem::IMethod* L2Lane::match_method(const il::MatchInstruction& match) {
    L2Body* body = body_of(&match);
    const auto* m = ir::get<ir::PatternMatch>(body != nullptr ? body->node(&match) : nullptr);
    ++(m != nullptr ? facts_from_l2 : facts_from_ilast);
    if (m == nullptr) return match.method();
    const typesystem::IMethod* l2 = translate::frontends::cil::clr_method(m->deconstruct);
    return check_ ? checked(l2, match.method()) : l2;
}

std::vector<il::ILVariable*> L2Lane::function_variables(const il::ILFunction& f) {
    std::optional<std::vector<il::ILVariable*>> l2;
    if (L2Body* body = body_of(&f)) l2 = body->function_variables(f);
    const auto ilast = [&] { return f.variables().items(); };
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast();
    return check_ ? checked(*l2, ilast()) : *l2;
}

std::vector<il::ILVariable*> L2Lane::variables_named(il::ILInstruction& inst) {
    std::optional<std::vector<il::ILVariable*>> l2;
    if (L2Body* body = body_of(&inst)) l2 = body->variables_named(inst);
    const auto ilast = [&] {
        std::vector<il::ILVariable*> out;
        for (il::ILInstruction* d : inst.descendants())
            if (auto* w = dynamic_cast<il::IInstructionWithVariableOperand*>(d)) out.push_back(w->variable());
        return out;
    };
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast();
    if (check_) {
        // (the same variables; the order of a handler's filter and body differs between the two)
        std::vector<il::ILVariable*> a = *l2, b = ilast();
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        checked(a, b);
    }
    return *l2;
}

std::optional<std::vector<il::BlockContainer*>> L2Lane::single_this_call_containers(const il::ILVariable& v) {
    std::optional<std::optional<std::vector<il::BlockContainer*>>> l2;
    if (L2Body* body = v.function() != nullptr ? body_of(v.function()) : nullptr) l2 = body->single_this_call_containers(v);
    const auto ilast = [&]() -> std::optional<std::vector<il::BlockContainer*>> {
        il::LdLoca* address = v.address_instructions()[0];
        auto* call = il::as<il::Call>(address->parent());
        if (call == nullptr || address->child_index() != 0 || call->method->is_static()) return std::nullopt;
        std::vector<il::BlockContainer*> out;
        for (il::ILInstruction* node = call->parent(); node != nullptr; node = node->parent()) {
            if (auto* c = il::as<il::BlockContainer>(node)) {
                out.push_back(c);
                break;
            }
        }
        return out;
    };
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast();
    return check_ ? checked(*l2, ilast()) : *l2;
}

VariableFacts L2Lane::variable(const il::ILVariable& v) {
    std::optional<VariableFacts> l2;
    if (L2Body* body = v.function() != nullptr ? body_of(v.function()) : nullptr) l2 = body->variable_facts(v);
    const auto ilast = [&] { return ilast_variable(v); };
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast();
    return check_ ? checked(*l2, ilast()) : *l2;
}

ContainerFacts L2Lane::container(const il::BlockContainer& c) {
    std::optional<ContainerFacts> l2;
    if (L2Body* body = body_of(&c)) l2 = body->container_facts(c);
    const auto ilast = [&] { return ilast_container(c); };
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast();
    return check_ ? checked(*l2, ilast()) : *l2;
}

il::ILFunctionKind L2Lane::function_kind(const il::ILFunction& f) {
    std::optional<il::ILFunctionKind> l2;
    if (L2Body* body = body_of(&f)) l2 = body->function_kind(f);
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return f.kind();
    return check_ ? checked(*l2, f.kind()) : *l2;
}

il::BlockContainer* L2Lane::function_body(const il::ILFunction& f) {
    L2Body* body = body_of(&f);
    il::BlockContainer* l2 = body != nullptr ? body->function_body(f) : nullptr;
    il::BlockContainer* ilast = il::as<il::BlockContainer>(f.body());
    ++(l2 != nullptr ? facts_from_l2 : facts_from_ilast);
    if (l2 == nullptr) return ilast;
    return check_ ? checked(l2, ilast) : l2;
}

std::vector<il::ILVariable*> L2Lane::captured_variables(const il::ILFunction& f) {
    std::optional<std::vector<il::ILVariable*>> l2;
    if (L2Body* body = body_of(&f)) l2 = body->captured_variables(f);
    const auto ilast = [&] { return std::vector<il::ILVariable*>(f.captured_variables.begin(), f.captured_variables.end()); };
    ++(l2 ? facts_from_l2 : facts_from_ilast);
    if (!l2) return ilast();
    return check_ ? checked(*l2, ilast()) : *l2;
}

template <class T> static T check_against(L2Lane& lane, bool check, T from_l2, T from_ilast, std::source_location where = std::source_location::current()) {
    if (check) {
        ++lane.checks;
        if (!(from_l2 == from_ilast)) {
            ++lane.check_differences;
            ++lane.check_differences_at[where.line()];
        }
    }
    return from_l2;
}

std::optional<DelegateConstructionMatch> L2Body::ilast_delegate_construction(il::NewObj& newobj) {
    const typesystem::IMethod* method = nullptr;
    il::ILInstruction* target = nullptr;
    const typesystem::IType* type = nullptr;
    if (!il::transforms::DelegateConstruction::match_delegate_construction(&newobj, &method, &target, &type)) return std::nullopt;
    return DelegateConstructionMatch{method, target, type, newobj.arguments()[1]->op_code == il::OpCode::LdVirtFtn};
}

std::optional<DelegateConstructionMatch> L2Body::match_delegate_construction(il::NewObj& newobj) {
    // a New of two arguments, the second the address of a method (AddressOf: ldftn) or a virtual method pointer (MethodRef virtual_ptr:
    // ldvirtftn); the delegate type the constructor's
    const auto* n = ir::get<ir::New>(map_.node(&newobj));
    if (n == nullptr) {
        ++lane_.facts_from_ilast;
        return ilast_delegate_construction(newobj);
    }
    ++lane_.facts_from_l2;
    std::optional<DelegateConstructionMatch> m;
    if (n->args.size() == 2) {
        const typesystem::IMethod* ctor = translate::frontends::cil::clr_method(n->sym);
        const ir::Node* func = n->args[1];
        const typesystem::IMethod* method = nullptr;
        bool is_virtual = false;
        if (const auto* a = ir::get<ir::AddressOf>(func)) {
            method = translate::frontends::cil::clr_method(a->sym);
        } else if (const auto* r = ir::get<ir::MethodRef>(func); r != nullptr && r->mode == "virtual_ptr") {
            method = translate::frontends::cil::clr_method(r->sym);
            is_virtual = true;
        }
        il::ILInstruction* target = operand(n->args[0], newobj);
        const typesystem::IType* type = ctor != nullptr ? ctor->declaring_type() : nullptr;
        if (method != nullptr && target != nullptr && type != nullptr &&
            (type->kind() == typesystem::TypeKind::Delegate || type->kind() == typesystem::TypeKind::Unknown))
            m = DelegateConstructionMatch{method, target, type, is_virtual};
    }
    return check_against(lane_, lane_.check_enabled(), m, lane_.check_enabled() ? ilast_delegate_construction(newobj) : m);
}

bool L2Body::match_tuple_construction(il::NewObj& newobj, std::vector<il::ILInstruction*>* elements) {
    // TupleTransform::match_tuple_construction on the New nodes: seven elements and a Rest that is a tuple's New, then its own
    const ir::Node* node = map_.node(&newobj);
    if (!ir::is<ir::New>(node)) {
        ++lane_.facts_from_ilast;
        return il::transforms::TupleTransform::match_tuple_construction(&newobj, elements);
    }
    ++lane_.facts_from_l2;
    const auto l2 = [&](std::vector<il::ILInstruction*>* out) {
        out->clear();
        const ir::Node* n = node;
        il::ILInstruction* holder = &newobj;
        const auto* k = ir::get<ir::New>(n);
        const typesystem::IMethod* ctor = translate::frontends::cil::clr_method(k->sym);
        int element_count = 0;
        if (ctor == nullptr || !typesystem::TupleType::is_tuple_compatible(ctor->declaring_type(), element_count)) return false;
        out->assign(static_cast<std::size_t>(element_count), nullptr);
        std::size_t at = 0;
        while (element_count >= typesystem::TupleType::kRestPosition) {
            if (static_cast<int>(k->args.size()) != typesystem::TupleType::kRestPosition) return false;
            for (int pos = 1; pos < typesystem::TupleType::kRestPosition; pos++) {
                il::ILInstruction* e = operand(k->args[static_cast<std::size_t>(pos - 1)], *holder);
                if (e == nullptr) return false;
                (*out)[at++] = e;
            }
            element_count -= typesystem::TupleType::kRestPosition - 1;
            n = k->args.back();
            il::ILInstruction* rest = operand(n, *holder);
            k = ir::get<ir::New>(n);
            if (k == nullptr || rest == nullptr) return false;
            holder = rest;
            ctor = translate::frontends::cil::clr_method(k->sym);
            int rest_element_count = 0;
            if (ctor == nullptr || !typesystem::TupleType::is_tuple_compatible(ctor->declaring_type(), rest_element_count)) return false;
            if (rest_element_count != element_count) return false;
        }
        if (static_cast<int>(k->args.size()) != element_count) return false;
        for (int i = 0; i < element_count; i++) {
            il::ILInstruction* e = operand(k->args[static_cast<std::size_t>(i)], *holder);
            if (e == nullptr) return false;
            (*out)[at++] = e;
        }
        return true;
    };
    const bool matched = l2(elements);
    if (lane_.check_enabled()) {
        std::vector<il::ILInstruction*> ilast;
        const bool ilast_matched = il::transforms::TupleTransform::match_tuple_construction(&newobj, &ilast);
        check_against(lane_, true, std::make_pair(matched, matched ? *elements : std::vector<il::ILInstruction*>{}),
                      std::make_pair(ilast_matched, ilast_matched ? ilast : std::vector<il::ILInstruction*>{}));
    }
    return matched;
}

bool L2Body::match_span_string_concat(il::CallInstruction& call, std::vector<std::pair<il::ILInstruction*, typesystem::KnownTypeCode>>* operands) {
    // CallBuilder::is_span_based_string_concat on the Call node: string.Concat over ReadOnlySpan<char>, each argument string's implicit
    // conversion (a Call) or a ReadOnlySpan<char> made over the address of a char (a New of one RValueAddr Unary &)
    const auto* k = ir::get<ir::Call>(map_.node(&call));
    if (k == nullptr) {
        ++lane_.facts_from_ilast;
        return CallBuilder::is_span_based_string_concat(&call, operands);
    }
    ++lane_.facts_from_l2;
    const auto l2 = [&](std::vector<std::pair<il::ILInstruction*, typesystem::KnownTypeCode>>* out) {
        out->clear();
        const typesystem::IMethod* method = translate::frontends::cil::clr_method(k->sym);
        if (method == nullptr || !CallBuilder::is_span_based_string_concat(method) || k->target != nullptr) return false;
        std::optional<std::size_t> first_string;
        for (std::size_t i = 0; i < k->args.size(); ++i) {
            const ir::Node* a = k->args[i];
            il::ILInstruction* arg = operand(a, call);
            if (arg == nullptr) return false;
            if (const auto* c = ir::get<ir::Call>(a); c != nullptr && c->form == ir::CallForm::Direct && !ir::has_flag(*a, ir::NodeFlag::PinnableRef) &&
                                                     CallBuilder::is_string_to_read_only_span_char_implicit_conversion(translate::frontends::cil::clr_method(c->sym))) {
                if (!first_string) first_string = i;
                const ir::Node* only = c->target != nullptr ? c->target : (c->args.size() == 1 ? c->args.front() : nullptr);
                il::ILInstruction* x = (c->target == nullptr && c->args.size() == 1) ? operand(only, *arg) : nullptr;
                if (x == nullptr) return false;
                out->push_back({x, typesystem::KnownTypeCode::String});
            } else if (const auto* nw = ir::get<ir::New>(a); nw != nullptr && nw->args.size() == 1 &&
                                                            il::transforms::ILInlining::is_read_only_span_char_ctor(translate::frontends::cil::clr_method(nw->sym))) {
                const auto* address = ir::get<ir::Unary>(nw->args.front());
                if (address == nullptr || address->op != "&" || !ir::has_flag(*nw->args.front(), ir::NodeFlag::RValueAddr)) return false;
                il::ILInstruction* addr = operand(nw->args.front(), *arg);
                il::ILInstruction* value = addr != nullptr ? operand(address->x, *addr) : nullptr;
                if (value == nullptr) return false;
                out->push_back({value, typesystem::KnownTypeCode::Char});
            } else {
                return false;
            }
        }
        return k->args.size() >= 2 && first_string.has_value() && *first_string <= 1;
    };
    const bool matched = l2(operands);
    if (lane_.check_enabled()) {
        std::vector<std::pair<il::ILInstruction*, typesystem::KnownTypeCode>> ilast;
        const bool ilast_matched = CallBuilder::is_span_based_string_concat(&call, &ilast);
        check_against(lane_, true, std::make_pair(matched, matched ? *operands : decltype(ilast){}), std::make_pair(ilast_matched, ilast_matched ? ilast : decltype(ilast){}));
    }
    return matched;
}

namespace {
bool is_i4(const ir::Node* n, std::int64_t value) {
    const auto* lit = ir::get<ir::Lit>(n);
    if (lit == nullptr || n->stack != ir::Stack::I4) return false;
    const auto* v = std::get_if<std::int64_t>(&lit->value);
    return v != nullptr && static_cast<std::int32_t>(*v) == value;
}
}  // namespace

bool L2Body::match_logic_and(il::IfInstruction& inst, il::ILInstruction** lhs, il::ILInstruction** rhs) {
    // a Cond whose else is the ldc.i4 0
    const auto* c = ir::get<ir::Cond>(map_.node(&inst));
    if (c == nullptr) {
        ++lane_.facts_from_ilast;
        return inst.match_logic_and(lhs, rhs);
    }
    ++lane_.facts_from_l2;
    *lhs = *rhs = nullptr;
    il::ILInstruction* cond = operand(c->cond, inst);
    il::ILInstruction* then = operand(c->then, inst);
    const bool matched = c->orelse != nullptr && is_i4(c->orelse, 0) && cond != nullptr && then != nullptr;
    if (matched) *lhs = cond, *rhs = then;
    if (lane_.check_enabled()) {
        il::ILInstruction* a = nullptr;
        il::ILInstruction* b = nullptr;
        const bool ilast = inst.match_logic_and(&a, &b);
        check_against(lane_, true, std::make_tuple(matched, *lhs, *rhs), std::make_tuple(ilast, a, b));
    }
    return matched;
}

bool L2Body::match_logic_or(il::IfInstruction& inst, il::ILInstruction** lhs, il::ILInstruction** rhs) {
    // a Cond whose then is the ldc.i4 1
    const auto* c = ir::get<ir::Cond>(map_.node(&inst));
    if (c == nullptr) {
        ++lane_.facts_from_ilast;
        return inst.match_logic_or(lhs, rhs);
    }
    ++lane_.facts_from_l2;
    *lhs = *rhs = nullptr;
    il::ILInstruction* cond = operand(c->cond, inst);
    il::ILInstruction* orelse = c->orelse != nullptr ? operand(c->orelse, inst) : nullptr;
    const bool matched = is_i4(c->then, 1) && cond != nullptr && orelse != nullptr;
    if (matched) *lhs = cond, *rhs = orelse;
    if (lane_.check_enabled()) {
        il::ILInstruction* a = nullptr;
        il::ILInstruction* b = nullptr;
        const bool ilast = inst.match_logic_or(&a, &b);
        check_against(lane_, true, std::make_tuple(matched, *lhs, *rhs), std::make_tuple(ilast, a, b));
    }
    return matched;
}

std::optional<std::pair<il::StLoc*, il::BlockContainer*>> L2Body::match_store_then_while(il::Block& block, int at) {
    const auto ilast = [&]() -> std::optional<std::pair<il::StLoc*, il::BlockContainer*>> {
        auto* store = il::as<il::StLoc>(block.instructions[at]);
        if (store == nullptr || !(at + 1 < block.instructions.count())) return std::nullopt;
        auto* loop = il::as<il::BlockContainer>(block.instructions[at + 1]);
        if (loop == nullptr || loop->kind() != il::ContainerKind::While) return std::nullopt;
        return std::make_pair(store, loop);
    };
    // the Block in the block's node (a container's Labeled), its statements; a plain Assign to a Name, then a Region of kind While
    const ir::Node* n = map_.node(&block);
    if (const auto* labeled = ir::get<ir::Labeled>(n)) n = std::holds_alternative<ir::Node*>(labeled->body) ? std::get<ir::Node*>(labeled->body) : nullptr;
    const std::vector<ir::Node*>* stmts = nullptr;
    if (const auto* b = ir::get<ir::Block>(n)) stmts = &b->stmts;
    else if (const auto* e = ir::get<ir::BlockExpr>(n)) stmts = &e->stmts;
    if (stmts == nullptr || stmts->size() != static_cast<std::size_t>(block.instructions.count())) {
        ++lane_.facts_from_ilast;
        return ilast();
    }
    ++lane_.facts_from_l2;
    std::optional<std::pair<il::StLoc*, il::BlockContainer*>> m;
    const auto unwrap = [](const ir::Node* s) {
        while (const auto* e = ir::get<ir::ExprStmt>(s)) s = e->expr;
        return s;
    };
    const ir::Node* first = unwrap((*stmts)[static_cast<std::size_t>(at)]);
    const auto* assign = ir::get<ir::Assign>(first);
    if (assign != nullptr && !assign->op && !assign->eval && !assign->sym && ir::is<ir::Name>(assign->target) &&
        static_cast<std::size_t>(at + 1) < stmts->size()) {
        const ir::Node* second = unwrap((*stmts)[static_cast<std::size_t>(at + 1)]);
        const auto* region = ir::get<ir::Region>(second);
        auto* store = il::as<il::StLoc>(operand(first, block));
        auto* loop = il::as<il::BlockContainer>(operand(second, block));
        if (region != nullptr && region->kind == ir::RegionKind::While && store != nullptr && loop != nullptr) m = std::make_pair(store, loop);
    }
    return check_against(lane_, lane_.check_enabled(), m, lane_.check_enabled() ? ilast() : m);
}

const typesystem::IMethod* L2Body::call_method(const il::CallInstruction& call) {
    const ir::Node* n = map_.node(&call);
    const typesystem::IMethod* l2 = nullptr;
    if (const auto* c = ir::get<ir::Call>(n)) l2 = translate::frontends::cil::clr_method(c->sym);
    else if (const auto* nw = ir::get<ir::New>(n)) l2 = translate::frontends::cil::clr_method(nw->sym);
    if (l2 == nullptr) {
        ++lane_.facts_from_ilast;
        return call.method;
    }
    ++lane_.facts_from_l2;
    return check_against(lane_, lane_.check_enabled(), l2, call.method);
}

il::ContainerKind L2Body::container_kind(const il::BlockContainer& c) {
    const auto* region = ir::get<ir::Region>(map_.node(&c));
    if (region == nullptr) {
        ++lane_.facts_from_ilast;
        return c.kind();
    }
    ++lane_.facts_from_l2;
    return check_against(lane_, lane_.check_enabled(), translate::frontends::cil::container_kind(region->kind), c.kind());
}

bool L2Body::is_ldc_i4(const il::ILInstruction& inst, std::int32_t value) {
    const ir::Node* n = map_.node(&inst);
    if (n == nullptr) {
        ++lane_.facts_from_ilast;
        return inst.match_ldc_i4(value);
    }
    ++lane_.facts_from_l2;
    const auto* lit = ir::get<ir::Lit>(n);
    const auto* v = lit != nullptr && n->stack == ir::Stack::I4 ? std::get_if<std::int64_t>(&lit->value) : nullptr;
    const bool l2 = v != nullptr && static_cast<std::int32_t>(*v) == value;
    return check_against(lane_, lane_.check_enabled(), l2, inst.match_ldc_i4(value));
}

std::optional<std::pair<il::ILInstruction*, const typesystem::IType*>> L2Body::cast_around(il::ILInstruction& inst) {
    const auto ilast = [&]() -> std::optional<std::pair<il::ILInstruction*, const typesystem::IType*>> {
        if (auto* cc = il::as<il::CastClass>(inst.parent())) return std::make_pair(static_cast<il::ILInstruction*>(cc), cc->type());
        if (auto* ua = il::as<il::UnboxAny>(inst.parent())) return std::make_pair(static_cast<il::ILInstruction*>(ua), ua->type());
        return std::nullopt;
    };
    // the node above the instruction's: a Cast of kind RefCast (castclass) or UnboxAny, its type the node's
    const ir::Node* n = map_.node(&inst);
    ir::Node* parent = n != nullptr ? index_.place(n).parent : nullptr;
    il::ILInstruction* parent_inst = parent != nullptr ? map_.instruction(parent) : nullptr;
    if (parent_inst == nullptr || parent_inst != inst.parent()) {
        ++lane_.facts_from_ilast;
        return ilast();
    }
    ++lane_.facts_from_l2;
    std::optional<std::pair<il::ILInstruction*, const typesystem::IType*>> m;
    if (const auto* cast = ir::get<ir::Cast>(parent); cast != nullptr && (cast->conv == ir::ConvKind::RefCast || cast->conv == ir::ConvKind::UnboxAny))
        m = std::make_pair(parent_inst, translate::frontends::cil::clr_type(parent->type));
    return check_against(lane_, lane_.check_enabled(), m, lane_.check_enabled() ? ilast() : m);
}

std::optional<il::BlockContainer*> L2Body::enclosing_container(const il::ILInstruction& inst) {
    // the closest Region above the node (a node that stands for no container of the ILAst, as a container's Labeled block, is passed)
    const ir::Node* n = map_.node(&inst);
    if (n == nullptr) return std::nullopt;
    for (ir::Node* p = index_.place(n).parent; p != nullptr; p = index_.place(p).parent) {
        if (!ir::is<ir::Region>(p)) continue;
        auto* c = il::as<il::BlockContainer>(map_.instruction(p));
        if (c == nullptr) return std::nullopt;
        return c;
    }
    return static_cast<il::BlockContainer*>(nullptr);
}

std::optional<bool> L2Body::hides_variable_with_name(const il::ILFunction& f, const std::string& name) {
    // the function and each function around it: a local with the name, a local function with the name
    const ir::Node* n = map_.node(&f);
    if (n == nullptr) return std::nullopt;
    for (const ir::Node* p = n; p != nullptr; p = index_.place(p).parent) {
        const std::vector<ir::SymP>* locals = nullptr;
        const std::vector<ir::Node*>* nested = nullptr;
        if (const auto* m = ir::get<ir::MethodDecl>(p)) locals = &m->locals, nested = &m->nested;
        else if (const auto* l = ir::get<ir::Lambda>(p)) locals = &l->locals, nested = &l->nested;
        else continue;
        for (const ir::SymP& s : *locals)
            if (!s->name.empty() && s->name == name) return true;
        for (const ir::Node* lf : *nested) {
            const auto* g = ir::get<ir::MethodDecl>(lf);
            if (g == nullptr) return std::nullopt;
            if (g->name.value_or("") == name) return true;
        }
    }
    return false;
}

std::optional<std::vector<il::ILVariable*>> L2Body::function_variables(const il::ILFunction& f) const {
    const ir::Node* n = map_.node(&f);
    const std::vector<ir::SymP>* locals = nullptr;
    if (const auto* m = ir::get<ir::MethodDecl>(n)) locals = &m->locals;
    else if (const auto* l = ir::get<ir::Lambda>(n)) locals = &l->locals;
    if (locals == nullptr) return std::nullopt;
    std::vector<il::ILVariable*> out;
    out.reserve(locals->size());
    for (const ir::SymP& s : *locals) {
        il::ILVariable* v = map_.variable(s.get());
        if (v == nullptr) return std::nullopt;
        out.push_back(v);
    }
    return out;
}

std::optional<std::vector<il::ILVariable*>> L2Body::variables_named(il::ILInstruction& inst) {
    // the nodes that name a variable (a load, a store, an address, a pattern's, a handler's, a using's or a fixed's Local), in the order
    // the ILAst lists its instructions (a node after its operands)
    const ir::Node* root = map_.node(&inst);
    if (root == nullptr) return std::nullopt;
    std::vector<il::ILVariable*> out;
    bool missing = false;
    const auto sym_of_named = [&](const ir::Node* n) -> const ir::Sym* {
        if (const auto* a = ir::get<ir::Assign>(n); a != nullptr && !a->op && !a->eval && !a->sym)
            if (const auto* name = ir::get<ir::Name>(a->target)) return name->sym.get();
        if (const auto* u = ir::get<ir::Unary>(n); u != nullptr && u->op == "&" && !ir::has_flag(*n, ir::NodeFlag::RValueAddr))
            if (const auto* name = ir::get<ir::Name>(u->x)) return name->sym.get();
        if (const auto* name = ir::get<ir::Name>(n); name != nullptr && map_.instruction(n) != nullptr) return name->sym.get();
        if (const auto* m = ir::get<ir::PatternMatch>(n)) return m->var.get();
        if (const auto* c = ir::get<ir::Catch>(n)) return c->sym.get();
        if (const auto* t = ir::get<ir::Try>(n); t != nullptr && t->resources.size() == 1)
            if (const auto* l = ir::get<ir::Local>(t->resources.front())) return l->sym.get();
        if (const auto* f = ir::get<ir::Fixed>(n))
            if (const auto* l = ir::get<ir::Local>(f->var)) return l->sym.get();
        return nullptr;
    };
    const std::function<void(const ir::Node*)> walk = [&](const ir::Node* n) {
        if (n == nullptr || missing) return;
        for (const ir::Node* c : ir::children(*n)) walk(c);
        if (map_.instruction(n) == nullptr) return;   // (a helper node: its instruction is the node above)
        if (const ir::Sym* s = sym_of_named(n)) {
            il::ILVariable* v = map_.variable(s);
            if (v == nullptr) missing = true;
            else out.push_back(v);
        }
    };
    walk(root);
    if (missing) return std::nullopt;
    return out;
}

std::optional<std::optional<std::vector<il::BlockContainer*>>> L2Body::single_this_call_containers(const il::ILVariable& v) {
    // the variable's one address (a Unary & of its Name), the `this` (the target) of a call that is not static; then the first Region
    // above the call
    const ir::SymP* sym = map_.sym(&v);
    if (sym == nullptr) return std::nullopt;
    const ir::TreeIndex::Uses& uses = index_.uses(sym->get());
    if (uses.addresses.size() != 1) return std::nullopt;
    ir::Node* address = uses.addresses.front();
    ir::Node* call = index_.place(address).parent;
    const auto* c = ir::get<ir::Call>(call);
    if (c == nullptr || c->form != ir::CallForm::Direct || ir::has_flag(*call, ir::NodeFlag::PinnableRef) || c->target != address)
        return std::optional<std::vector<il::BlockContainer*>>();
    const typesystem::IMethod* method = translate::frontends::cil::clr_method(c->sym);
    if (method == nullptr || method->is_static()) return std::optional<std::vector<il::BlockContainer*>>();
    std::vector<il::BlockContainer*> out;
    for (ir::Node* p = index_.place(call).parent; p != nullptr; p = index_.place(p).parent) {
        if (!ir::is<ir::Region>(p)) continue;
        auto* container = il::as<il::BlockContainer>(map_.instruction(p));
        if (container == nullptr) return std::nullopt;
        out.push_back(container);
        break;
    }
    return std::optional<std::vector<il::BlockContainer*>>(std::move(out));
}

std::optional<VariableFacts> L2Body::variable_facts(const il::ILVariable& v) {
    // the symbol (Lowering::fill_variable, restated as the builders change the variable) and its uses in the tree
    const ir::SymP* sym = map_.sym(&v);
    if (sym == nullptr || !(*sym)->role) return std::nullopt;
    const ir::Sym& s = **sym;
    const std::string* exact = s.info.get_as<std::string>("cil.kind_exact");
    VariableFacts facts{translate::frontends::cil::variable_kind(*s.role, exact), translate::frontends::cil::clr_type(s.type),
                        s.name.empty() ? std::nullopt : std::optional<std::string>(s.name), s.entry_used, s.entry_initialized, s.ref_readonly, 0, 0, 0,
                        nullptr, s.index ? std::optional<int>(static_cast<int>(*s.index)) : std::nullopt};
    const ir::TreeIndex::Uses& uses = index_.uses(&s);
    facts.load_count = static_cast<int>(uses.loads.size());
    facts.store_count = static_cast<int>(index_.store_count(&s));
    facts.address_count = static_cast<int>(uses.addresses.size());
    if (const auto* scope = s.info.get_as<std::string>("cil.capture_scope")) {
        // a region's label is its entry block's
        ir::Node* labeled = index_.block(*scope);
        ir::Node* region = labeled != nullptr ? index_.place(labeled).parent : nullptr;
        facts.capture_scope = region != nullptr ? il::as<il::BlockContainer>(map_.instruction(region)) : nullptr;
        if (facts.capture_scope == nullptr) return std::nullopt;
    }
    return facts;
}

std::optional<ContainerFacts> L2Body::container_facts(const il::BlockContainer& c) {
    const auto* region = ir::get<ir::Region>(map_.node(&c));
    const auto* entry = region != nullptr && !region->blocks.empty() ? ir::get<ir::Labeled>(region->blocks.front()) : nullptr;
    if (entry == nullptr || !entry->label) return std::nullopt;
    const ir::Node* holder = index_.place(map_.node(&c)).parent;
    return ContainerFacts{static_cast<int>(index_.incoming(*entry->label)), ir::is<ir::MethodDecl>(holder) || ir::is<ir::Lambda>(holder)};
}

std::optional<il::ILFunctionKind> L2Body::function_kind(const il::ILFunction& f) const {
    // lift.cpp: a MethodDecl is a top-level or (with "local") a local function, a Lambda a delegate or (with "expression_tree") a tree
    const ir::Node* n = map_.node(&f);
    const std::vector<std::string>* mods = nullptr;
    if (const auto* m = ir::get<ir::MethodDecl>(n)) mods = &m->mods;
    else if (const auto* l = ir::get<ir::Lambda>(n)) mods = &l->mods;
    if (mods == nullptr) return std::nullopt;
    const bool local = std::find(mods->begin(), mods->end(), "local") != mods->end();
    const bool tree = std::find(mods->begin(), mods->end(), "expression_tree") != mods->end();
    if (ir::is<ir::MethodDecl>(n)) return local ? il::ILFunctionKind::LocalFunction : il::ILFunctionKind::TopLevelFunction;
    return tree ? il::ILFunctionKind::ExpressionTree : il::ILFunctionKind::Delegate;
}

il::BlockContainer* L2Body::function_body(const il::ILFunction& f) const {
    const ir::Node* n = map_.node(&f);
    const ir::Node* body = nullptr;
    if (const auto* m = ir::get<ir::MethodDecl>(n)) body = std::holds_alternative<ir::Node*>(m->body) ? std::get<ir::Node*>(m->body) : nullptr;
    else if (const auto* l = ir::get<ir::Lambda>(n)) body = l->body;
    return body != nullptr ? il::as<il::BlockContainer>(map_.instruction(body)) : nullptr;
}

std::optional<std::vector<il::ILVariable*>> L2Body::captured_variables(const il::ILFunction& f) const {
    const ir::Node* n = map_.node(&f);
    const std::vector<ir::SymP>* captures = nullptr;
    if (const auto* m = ir::get<ir::MethodDecl>(n)) captures = &m->captures;
    else if (const auto* l = ir::get<ir::Lambda>(n)) captures = &l->captures;
    if (captures == nullptr) return std::nullopt;
    std::vector<il::ILVariable*> out;
    out.reserve(captures->size());
    for (const ir::SymP& s : *captures) {
        il::ILVariable* v = map_.variable(s.get());
        if (v == nullptr) return std::nullopt;
        out.push_back(v);
    }
    return out;
}

std::unique_ptr<L2Body> L2Body::make(il::ILFunction& function, L2Lane& lane) {
    std::unique_ptr<L2Body> body(new L2Body(lane));
    const auto start = std::chrono::steady_clock::now();
    struct Clock {
        std::chrono::steady_clock::time_point start;
        std::chrono::steady_clock::duration& total;
        ~Clock() { total += std::chrono::steady_clock::now() - start; }
    } clock{start, lane.lowering_time};
    try {
        body->root_ = lane.lowering.function(function, &body->map_);
        body->function_ = &function;
    } catch (const std::logic_error&) {
        ++lane.refused;
        return nullptr;   // a state L2 cannot hold (none on the test corpus): the builders read the ILAst
    }
    ++lane.bodies;
    body->index_ = ir::TreeIndex(body->root_);
    return body;
}

void L2Body::count(bool read_from_l2, const il::ILInstruction& inst) {
    if (read_from_l2) {
        ++lane_.from_l2;
    } else {
        ++lane_.from_ilast;
        ++lane_.from_ilast_by_op[static_cast<std::size_t>(inst.op_code)];
        if (map_.node(&inst) == nullptr) ++lane_.without_node_by_op[static_cast<std::size_t>(inst.op_code)];
    }
}

void L2Body::variable_changed(il::ILVariable* variable) {
    const ir::SymP* sym = map_.sym(variable);
    if (sym == nullptr) throw std::logic_error("L2Body: a variable with no symbol changed");
    lane_.lowering.restate(**sym, *variable);
    index_.invalidate();
}

void L2Body::variable_registered(il::ILFunction* function, il::ILVariable* variable) {
    ir::SymP sym = lane_.lowering.new_variable(*variable);
    auto* owner = const_cast<ir::Node*>(map_.node(function));   // (the body owns its tree)
    if (auto* m = ir::get<ir::MethodDecl>(owner)) m->locals.push_back(sym);
    else if (auto* l = ir::get<ir::Lambda>(owner)) l->locals.push_back(sym);
    else throw std::logic_error("L2Body: a variable registered in a function with no node");
    map_.link(sym, variable);
}

il::ILInstruction* L2Body::operand(const ir::Node* child, const il::ILInstruction& parent) const {
    while (child != nullptr) {
        if (const auto* e = ir::get<ir::ExprStmt>(child)) child = e->expr;
        else if (const auto* y = ir::get<ir::Yield>(child)) child = y->expr;
        else break;
    }
    il::ILInstruction* i = child != nullptr ? map_.instruction(child) : nullptr;
    return i != nullptr && i->parent() == &parent && parent.get_child(i->child_index()) == i ? i : nullptr;
}

std::optional<SwitchFacts> L2Body::switch_facts(const ir::Node& node, il::SwitchInstruction& inst) const {
    // lift.cpp: the value switched on, `lifted` and `of`; each Case a section (its ranges the labels, null_label, the compiler's default
    // a flag), its one statement the body
    const ir::Node* value = nullptr;
    const std::vector<ir::Node*>* cases = nullptr;
    bool lifted = false;
    const ir::TypeP* of = nullptr;
    if (const auto* s = ir::get<ir::Switch>(&node)) {
        value = s->expr, cases = &s->cases, lifted = s->lifted, of = &s->of;
    } else if (const auto* e = ir::get<ir::SwitchExpr>(&node)) {
        value = e->expr, cases = &e->cases, lifted = e->lifted, of = &e->of;
    } else {
        return std::nullopt;
    }
    SwitchFacts facts{&inst, operand(value, inst), translate::frontends::cil::clr_type(*of), lifted, static_cast<il::StackType>(node.stack), {}};
    if (facts.value_ == nullptr || static_cast<std::size_t>(inst.sections.count()) != cases->size()) return std::nullopt;
    facts.sections_.reserve(cases->size());
    for (const ir::Node* cn : *cases) {
        const auto* c = ir::get<ir::Case>(cn);
        auto* section = il::as<il::SwitchSection>(map_.instruction(cn));
        if (c == nullptr || section == nullptr || section->parent() != &inst || c->body.size() != 1) return std::nullopt;
        il::ILInstruction* body = operand(c->body.front(), *section);
        if (body == nullptr) return std::nullopt;
        std::vector<dotnet::decompiler::util::LongInterval> intervals;
        intervals.reserve(c->ranges.size());
        for (const auto& [low, high] : c->ranges) intervals.push_back(dotnet::decompiler::util::LongInterval::inclusive(low, high));
        facts.sections_.push_back({section, dotnet::decompiler::util::LongSet(intervals), c->null_label, ir::has_flag(*cn, ir::NodeFlag::CompilerGenerated), body});
    }
    return facts;
}

ir::Node* L2Body::lower_new(il::ILInstruction* inst) {
    if (const ir::Node* known = map_.node(inst)) return const_cast<ir::Node*>(known);
    const auto name_of = [&](il::ILVariable* v) {
        const ir::SymP* sym = map_.sym(v);
        if (sym == nullptr) throw std::logic_error("L2Body: an instruction names a variable with no symbol");
        return ir::make(lane_.arena, ir::Name{.sym = *sym}, (*sym)->type);
    };
    ir::Node* n;
    if (auto* ld = il::as<il::LdLoc>(inst)) {
        n = name_of(ld->variable());
    } else if (auto* lda = il::as<il::LdLoca>(inst)) {
        n = ir::make(lane_.arena, ir::Unary{.op = "&", .x = name_of(lda->variable())});
    } else if (auto* st = il::as<il::StLoc>(inst)) {
        ir::Node* target = name_of(st->variable());
        ir::TypeP type = target->type;
        n = ir::make(lane_.arena, ir::Assign{.target = target, .value = lower_new(st->value())}, std::move(type));
    } else {
        throw std::logic_error("L2Body: the builders made an instruction it does not mirror");
    }
    n->stack = static_cast<ir::Stack>(inst->result_type());
    map_.link(n, inst);
    return n;
}

void L2Body::replaced(il::ILInstruction* old, il::ILInstruction* now) {
    const ir::Node* was = map_.node(old);
    if (was == nullptr) throw std::logic_error("L2Body: an instruction with no node was replaced");
    // (a node in a statement place stands inside an ExprStmt, which stays)
    index_.replace(const_cast<ir::Node*>(was), lower_new(now));
}

const ir::Node* L2Body::node_or_lower(il::ILInstruction* inst) {
    if (const ir::Node* n = map_.node(inst)) return n;
    if (inst == nullptr || inst->parent() != nullptr) return nullptr;   // (an instruction of the function the tree does not have)
    // (a new instruction over operands the tree has, as a call the builders put together from the function's instructions, is not
    // lowered: its operands would be lowered twice)
    for (il::ILInstruction* d : inst->descendants())
        if (d != inst && map_.node(d) != nullptr) return nullptr;
    try {
        return lane_.lowering.expression(*inst, &map_);
    } catch (const std::logic_error&) {
        return nullptr;
    }
}

void L2Body::moved(il::ILInstruction* old, il::ILInstruction* now) {
    const ir::Node* was = map_.node(old);
    const ir::Node* node = map_.node(now);
    if (was == nullptr || node == nullptr) return;   // (one the tree does not have: the builders read that operand from the ILAst)
    // the node leaves the place it came from (an empty node is left there: the tree holds a node once, and the ILAst's use lists count
    // the instruction once), then takes the place of `old`'s
    if (index_.place(node).parent != nullptr) index_.replace(const_cast<ir::Node*>(node), ir::make(lane_.arena, ir::Empty{}));
    index_.replace(const_cast<ir::Node*>(was), const_cast<ir::Node*>(node));
}

void L2Body::inserted(il::Block* block, int at) {
    const ir::Node* b = map_.node(block);
    if (const auto* labeled = ir::get<ir::Labeled>(b)) b = std::get<ir::Node*>(labeled->body);   // a container's block: the Block in it
    ir::Node* stmt = lower_new(block->instructions[at]);
    if (ir::category(*stmt) == ir::Category::Expression) stmt = ir::make(lane_.arena, ir::ExprStmt{.expr = stmt});
    index_.insert_stmt(const_cast<ir::Node*>(b), static_cast<std::size_t>(at), stmt);
}

}  // namespace cb::dotnet::decompiler::csharp

// dotnet/decompiler/il/transforms/deconstruction_transform.cpp -- see deconstruction_transform.hpp.
#include "dotnet/decompiler/il/transforms/deconstruction_transform.hpp"

#include <algorithm>
#include <cassert>
#include <climits>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/tuple_transform.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::TypeKind;

namespace {

// ImmutableArray<IType>.IsDefaultOrEmpty
bool is_default_or_empty(const std::optional<std::vector<const IType*>>& types) { return !types.has_value() || types->empty(); }

// `actions += action` for a delegate that may be null.
template <class Action>
void add_action(std::vector<Action>* actions, const Action& action) {
    if (action) actions->push_back(action);
}

template <class Action>
void add_actions(std::vector<Action>* actions, const std::vector<Action>& more) {
    actions->insert(actions->end(), more.begin(), more.end());
}

ILInstruction* as_instruction(IStoreInstruction* store) { return store->as_instruction(); }

}  // namespace

DeconstructionTransform::TupleNode::TupleNode(ILVariable* variable, const TupleType* type)
    : variable(variable), type(type), nested_elements(static_cast<std::size_t>(type->cardinality()), nullptr) {}

void DeconstructionTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().deconstruction) return;

    struct Cleanup {
        DeconstructionTransform* self;
        ~Cleanup() {
            self->context_ = nullptr;
            self->reset();
        }
    } cleanup{this};
    context_ = &context;
    reset();

    if (transform_deconstruction(block, pos)) return;
    if (inline_deconstruction_initializer(block, pos)) return;
}

void DeconstructionTransform::reset() {
    deconstruction_results_lookup_.clear();
    tuple_nodes_.clear();
    tuple_root_ = nullptr;
    deconstruction_results_.clear();
    rooted_in_deconstruct_call_ = false;
    calls_.clear();
    nodes_.clear();
}

bool DeconstructionTransform::lookup_deconstruction_result(ILVariable* variable, int* index) const {
    for (const auto& entry : deconstruction_results_lookup_) {
        if (entry.first == variable) {
            *index = entry.second;
            return true;
        }
    }
    return false;
}

void DeconstructionTransform::add_deconstruction_result(ILVariable* variable, int index) {
    int existing = 0;
    if (lookup_deconstruction_result(variable, &existing)) throw std::invalid_argument("An item with the same key has already been added.");
    deconstruction_results_lookup_.push_back({variable, index});
}

DeconstructionTransform::TupleNode* DeconstructionTransform::make_tuple_node(ILVariable* variable, const TupleType* type) {
    nodes_.emplace_back(variable, type);
    return &nodes_.back();
}

DeconstructionTransform::TupleNode* DeconstructionTransform::find_tuple_node(ILVariable* variable) const {
    for (const auto& entry : tuple_nodes_)
        if (entry.first == variable) return entry.second;
    return nullptr;
}

// call Deconstruct(target, ldloca out0, ...) [+ nested Deconstruct calls]
//   | stloc temp(ldobj(ldflda ItemN(ldloca tuple))) ... [nested tuple designations]
// stloc conv0(conv(...)) ...
// assignments ...
// =>
// deconstruct { init: pattern: conversions: assignments: }   (see the header)
bool DeconstructionTransform::transform_deconstruction(Block* block, int pos) {
    int start_pos = pos;
    // Blocks are processed back to front, so the inner parts of a nested deconstruction are visited before the position its matching
    // starts at; matching them on their own would consume the pattern piecemeal. Defer to the enclosing attempt where one exists (see
    // the guard for the precision guarantees).
    if (is_consumable_by_enclosing_deconstruction(block, pos)) return false;
    DeconstructionCall* root_call = nullptr;
    ILInstruction* root_tested_operand = nullptr;
    std::vector<StLoc*> conversion_st_locs;
    DelayedActions delayed_actions;
    if (!match_deconstruction_sequence(block, start_pos, &pos, &root_call, &root_tested_operand, &conversion_st_locs, &delayed_actions)) {
        return false;
    }
    context_->step("Deconstruction", block->instructions[start_pos]);
    auto* replacement = make<DeconstructInstruction>();
    const IMethod* deconstruct_method = root_call != nullptr ? root_call->method : nullptr;
    const IType* deconstructed_type;
    if (deconstruct_method == nullptr) {
        deconstructed_type = tuple_root_->type;
        root_tested_operand = make<LdLoc>(tuple_root_->variable);
    } else {
        if (deconstruct_method->is_static()) {
            deconstructed_type = deconstruct_method->parameters()[0]->type();
        } else {
            deconstructed_type = deconstruct_method->declaring_type();
        }
    }
    ILVariable* root_temp_variable = context_->function()->register_variable(VariableKind::PatternLocal, deconstructed_type);
    if (root_call != nullptr) {
        replacement->set_pattern(build_pattern_match(root_call, root_temp_variable, root_tested_operand));
    } else {
        replacement->set_pattern(build_tuple_pattern_match(tuple_root_, root_temp_variable, root_tested_operand));
    }
    replacement->set_conversions(make<Block>(BlockKind::DeconstructionConversions));
    for (StLoc* conv_inst : conversion_st_locs) {
        replacement->conversions()->instructions.add(conv_inst);
    }
    replacement->set_assignments(make<Block>(BlockKind::DeconstructionAssignments));
    for (const DelayedAction& action : delayed_actions) action(replacement);
    block->instructions.set(start_pos, replacement);
    block->instructions.remove_range(start_pos + 1, pos - start_pos - 1);
    context_->end_step(replacement);
    return true;
}

// Matches the full statement sequence of one deconstruction, starting at start_pos:
//   [Deconstruct call + nested calls | nested tuple designation temporaries]
//   [conversions]
//   [assignments]
// On success, end_pos is the position after the last consumed statement. The block is not modified; all rewrites are accumulated in
// delayed_actions.
bool DeconstructionTransform::match_deconstruction_sequence(Block* block, int start_pos, int* end_pos, DeconstructionCall** root_call,
                                                            ILInstruction** root_tested_operand, std::vector<StLoc*>* conversion_st_locs,
                                                            DelayedActions* delayed_actions) {
    std::optional<std::unordered_set<ILVariable*>> do_not_nest;
    while (true) {
        reset();
        *end_pos = start_pos;
        int pos = start_pos;
        delayed_actions->clear();
        match_deconstruction(block, &pos, root_call, root_tested_operand);
        if (*root_call == nullptr) match_nested_tuple_designations(block, &pos, do_not_nest ? &*do_not_nest : nullptr);
        std::vector<std::pair<ILVariable*, ConversionInfo>> conversions;
        if (!match_conversions(block, &pos, &conversions, conversion_st_locs, delayed_actions)) return false;
        bool any_assignments = false;
        if (!match_assignments(block, &pos, conversions, *conversion_st_locs, delayed_actions, *root_call != nullptr, &any_assignments)) {
            return false;
        }
        // Without any assignment the statement is a plain Deconstruct call, unless a nested deconstruction was consumed: then all leaves
        // are single-use elements handled by the forwarding fixup in MatchAssignments.
        bool has_nested_call = false;
        if (*root_call != nullptr) {
            for (DeconstructionCall* c : (*root_call)->nested_calls)
                if (c != nullptr) has_nested_call = true;
        }
        if (!any_assignments && !(*root_call != nullptr && has_nested_call)) return false;
        // A nested tuple designation only holds if the pattern consumed every read of its temporary; a remaining read means the value
        // escapes the designation. Retry with the variable as a plain designator leaf, which restores the flat deconstruction the
        // escaping read needs. This has to be decided before the leaf check below: every leaf of a wrongly nested first element
        // precedes the assigned ones, so that check would report the pattern as starting mid-way and give up on a designation the retry
        // can still make work.
        std::vector<ILVariable*> escaped;
        for (const auto& entry : tuple_nodes_) {
            TupleNode* node = entry.second;
            if (node == tuple_root_) continue;
            if (node->matched_access_count != node->variable->load_count() + node->variable->address_count()) escaped.push_back(node->variable);
        }
        if (!escaped.empty()) {
            if (!do_not_nest) do_not_nest.emplace();
            do_not_nest->insert(escaped.begin(), escaped.end());
            continue;
        }
        // first tuple element may not be discarded, otherwise we would run this transform on a suffix of the actual pattern.
        if (deconstruction_results_.at(0) == nullptr) return false;
        *end_pos = pos;
        return true;
    }
}

// stloc v(value)
// expr(..., deconstruct { ... }, ...)
// =>
// expr(..., deconstruct { init: stloc v(value) ... }, ...)
bool DeconstructionTransform::inline_deconstruction_initializer(Block* block, int pos) {
    ILVariable* v = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[pos]->match_st_loc(&v, &value)) return false;
    if (!(v->is_single_definition() && v->load_instructions().size() == 1)) return false;
    LdLoc* load_inst = v->load_instructions()[0];
    if (pos + 1 >= block->instructions.count()) return false;
    ILInlining::FindResult result = ILInlining::find_load_in_next(block->instructions[pos + 1], v, value, InliningOptions::FindDeconstruction);
    if (result.type != ILInlining::FindResultType::Deconstruction) return false;
    auto* deconstruction = static_cast<DeconstructInstruction*>(result.load_inst);
    if (!load_inst->is_descendant_of(deconstruction->assignments())) return false;
    if (load_inst->slot_info() == &StObj::target_slot) {
        if (value->op_code == OpCode::LdFlda || value->op_code == OpCode::LdElema) return false;
    }
    if (deconstruction->init.count() > 0) {
        ILVariable* init_variable = deconstruction->init[0]->variable();
        if (init_variable->load_instructions().size() != 1) throw UsageError("Sequence contains no elements or more than one");
        LdLoc* a = init_variable->load_instructions()[0];
        if (!load_inst->is_before(a)) return false;
    }
    context_->step("InlineDeconstructionInitializer", block->instructions[pos]);
    deconstruction->init.insert(0, static_cast<StLoc*>(block->instructions[pos]));
    block->instructions.remove_at(pos);
    v->set_kind(VariableKind::DeconstructionInitTemporary);
    return true;
}

// Whether the statement at pos belongs to a deconstruction whose matching starts at an earlier position in the block, in either nesting
// shape:
//
//   call Deconstruct(..., ldloca inner, ...)             at enclosingPos
//   [stloc copy(ldloc inner)]                            defensive copy of a struct element
//   call Deconstruct(ldloc(a) inner|copy, ...)           at pos
//
//   stloc temp(ldobj(ldflda ItemN(ldloc(a) outer)))      earlier in the block
//   ...
//   stloc x([conv](ldobj(ldflda ItemK(ldloc(a) temp))))  at pos
//
// The chained calls are emitted back to back, so an enclosing call that is not the preceding statement has something between it and pos
// that stops it from reaching here; the deconstruction at pos is then matched on its own. Nested designation temporaries are stored
// before the enclosing run's own element reads, so the two are not adjacent and only the store has to be found.
//
// Deferring is worth it only if the enclosing attempt can succeed, so the constraint MatchDeconstructionCall places on out-parameters is
// checked here as well: without it an element used more than once would defer this position to an attempt that then rejects the call,
// and the back-to-front walk gives it no second chance.
//
// Getting this wrong costs sugar, never correctness: the statement at pos is either folded into the enclosing deconstruction or
// decompiled as the explicit calls and element reads it came from.
bool DeconstructionTransform::is_consumable_by_enclosing_deconstruction(Block* block, int pos) {
    int enclosing_pos = 0;
    if (try_find_enclosing_deconstruction_call(block, pos, &enclosing_pos)) {
        auto* previous = pos >= 1 ? as<StLoc>(block->instructions[pos - 1]) : nullptr;
        if (enclosing_pos != pos - 1 && !(enclosing_pos == pos - 2 && previous != nullptr && as<LdLoc>(previous->value()) != nullptr)) {
            return false;
        }
        auto* enclosing_call = static_cast<CallInstruction*>(block->instructions[enclosing_pos]);
        for (int i = 1; i < enclosing_call->arguments().count(); i++) {
            ILVariable* out_param = nullptr;
            if (!enclosing_call->arguments()[i]->match_ld_loca(&out_param) ||
                !(out_param->store_count() == 0 && out_param->address_count() == 1 && out_param->load_count() <= 1)) {
                return false;
            }
        }
        return true;
    }
    return has_enclosing_tuple_designation(block, pos);
}

// call Deconstruct(..., ldloca v, ...)                  at enclosingPos
// [stloc copy(ldloc v)]                                 defensive copy of a struct element
// ...
// call Deconstruct(ldloc(a) v|copy, ...)                at pos
bool DeconstructionTransform::try_find_enclosing_deconstruction_call(Block* block, int pos, int* enclosing_pos) {
    *enclosing_pos = -1;
    auto* call = dynamic_cast<CallInstruction*>(block->instructions[pos]);
    if (call == nullptr) return false;
    if (!MatchInstruction::is_deconstruct_method(call->method) || call->arguments().count() == 0) return false;
    ILInstruction* target = call->arguments()[0];
    ILVariable* v = nullptr;
    if (!match_ld_loc_or_ld_loca(target, &v)) return false;
    // look through the defensive copy of a struct element
    if (v->store_instructions().size() == 1) {
        if (auto* copy = as<StLoc>(as_instruction(v->store_instructions()[0]))) {
            ILVariable* copy_source = nullptr;
            if (copy->value()->match_ld_loc(&copy_source)) v = copy_source;
        }
    }
    // StoreCount also counts the initial value of parameters, on purpose
    if (v->store_count() != 0) return false;
    if (!(v->address_instructions().size() == 1)) return false;
    LdLoca* address_load = v->address_instructions()[0];
    auto* enclosing_call = dynamic_cast<CallInstruction*>(address_load->parent());
    if (!(enclosing_call != nullptr && address_load->child_index() > 0 && MatchInstruction::is_deconstruct_method(enclosing_call->method))) {
        return false;
    }
    if (enclosing_call->parent() != block) return false;
    *enclosing_pos = enclosing_call->child_index();
    return *enclosing_pos >= 0 && *enclosing_pos < pos;
}

// stloc temp(ldobj(ldflda ItemN(ldloc(a) outer)))       earlier in the block
// ...
// stloc x([conv](ldobj(ldflda ItemK(ldloc(a) temp))))   at pos
// The statement at pos reads an element of a tuple stored by an earlier statement that is itself an element read, i.e. a candidate
// nested designation temporary.
bool DeconstructionTransform::has_enclosing_tuple_designation(Block* block, int pos) {
    ILVariable* unused_variable = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[pos]->match_st_loc(&unused_variable, &value)) return false;
    if (auto* conv = as<Conv>(value)) value = conv->argument();
    ILVariable* container = nullptr;
    const IType* unused_type = nullptr;
    int unused_index = 0;
    if (!match_tuple_element_read(value, &container, &unused_type, &unused_index)) return false;
    if (container->store_instructions().size() != 1) return false;
    auto* store = as<StLoc>(as_instruction(container->store_instructions()[0]));
    if (store == nullptr || store->parent() != block) return false;
    ILVariable* unused_temp = nullptr;
    ILVariable* unused_container = nullptr;
    if (!match_tuple_element_store(store, &unused_temp, &unused_container, &unused_type, &unused_index)) return false;
    if (store->child_index() >= pos) return false;
    // The temporaries and element reads of one designation are stored back to back. A statement of any other kind in between stops the
    // enclosing pattern from reaching this position, and deferring to it would lose the deconstruction here as well, because the
    // back-to-front walk does not come back.
    for (int between = store->child_index() + 1; between < pos; between++) {
        if (!match_tuple_element_store(block->instructions[between], &unused_temp, &unused_container, &unused_type, &unused_index)) return false;
    }
    return true;
}

// call Deconstruct(target, ldloca x, ldloca inner)      the root call, at pos
// [nested Deconstruct calls, see MatchNestedDeconstructions]
// On success, the leaf out-variables carry flat indices in depth-first order: this is the order in which StatementBuilder/
// ExpressionBuilder pair pattern variables with assignments, so the index checks in MatchConversions/MatchAssignments work unchanged
// for nested patterns.
void DeconstructionTransform::match_deconstruction(Block* block, int* pos, DeconstructionCall** root_call, ILInstruction** tested_operand) {
    *root_call = match_deconstruction_call(block->instructions[*pos], tested_operand);
    if (*root_call == nullptr) return;
    rooted_in_deconstruct_call_ = true;
    (*pos)++;
    match_nested_deconstructions(block, pos, *root_call);
    // Assign flat indices to the leaves in depth-first order: this is the order in which StatementBuilder/ExpressionBuilder pair
    // pattern variables with assignments, so the index checks in MatchConversions/MatchAssignments work unchanged for nested patterns.
    std::vector<ILVariable*> leaves;
    std::function<void(DeconstructionCall*)> collect_leaves = [&](DeconstructionCall* call) {
        for (std::size_t i = 0; i < call->results.size(); i++) {
            if (call->nested_calls[i] != nullptr)
                collect_leaves(call->nested_calls[i]);
            else
                leaves.push_back(call->results[i]);
        }
    };
    collect_leaves(*root_call);
    deconstruction_results_ = leaves;
    for (std::size_t i = 0; i < deconstruction_results_.size(); i++) {
        add_deconstruction_result(deconstruction_results_[i], static_cast<int>(i));
    }
}

// call(virt) Deconstruct(target, ldloca out0, ldloca out1, ...)
// where every out-argument is a single-use temporary.
DeconstructionTransform::DeconstructionCall* DeconstructionTransform::match_deconstruction_call(ILInstruction* inst, ILInstruction** tested_operand) {
    *tested_operand = nullptr;
    auto* call = dynamic_cast<CallInstruction*>(inst);
    if (call == nullptr) return nullptr;
    if (!MatchInstruction::is_deconstruct_method(call->method)) return nullptr;
    std::optional<bool> is_reference_type = call->method->declaring_type()->is_reference_type();
    if (call->method->is_static() || (is_reference_type.has_value() && !*is_reference_type)) {
        if (!(call->op_code == OpCode::Call)) return nullptr;
    } else {
        if (!(call->op_code == OpCode::CallVirt)) return nullptr;
    }
    if (call->arguments().count() < 3) return nullptr;
    std::vector<ILVariable*> results(static_cast<std::size_t>(call->arguments().count() - 1), nullptr);
    for (std::size_t i = 0; i < results.size(); i++) {
        ILVariable* v = nullptr;
        if (!call->arguments()[static_cast<int>(i) + 1]->match_ld_loca(&v)) return nullptr;
        // TODO v.LoadCount may be 2 if the deconstruction is assigned to a tuple variable or 0? because of discards
        if (!(v->store_count() == 0 && v->address_count() == 1 && v->load_count() <= 1)) return nullptr;
        results[i] = v;
    }
    *tested_operand = call->arguments()[0];
    calls_.emplace_back();
    DeconstructionCall* result = &calls_.back();
    result->method = call->method;
    result->results = results;
    result->nested_calls.assign(results.size(), nullptr);
    return result;
}

// Per element of the parent call, in order:
//   [stloc copy(ldloc result)]                          defensive copy for a struct element
//   call Deconstruct(ldloc(a) result|copy, ldloca ...)  recursing into its elements
// C# evaluates nested Deconstruct calls left-to-right, directly after the parent call, before any conversions or assignments: the
// elements are visited depth-first, and the stack of pending elements takes the place of recursing into a matched nested call.
void DeconstructionTransform::match_nested_deconstructions(Block* block, int* pos, DeconstructionCall* root_call) {
    auto is_receiver_reference = [](ILInstruction* target, ILVariable* receiver) {
        ILVariable* v = nullptr;
        return match_ld_loc_or_ld_loca(target, &v) && v == receiver;
    };

    // upstream's BindsOnElementType
    auto binds_on_element_type = [](const IMethod* method, const IType* element_type) {
        int out_param_count = static_cast<int>(method->parameters().size()) - (method->is_static() ? 1 : 0);
        const IType* type = element_type;
        while (type != nullptr) {
            if (!method->is_static() && typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(type, method->declaring_type())) return true;
            auto deconstructs = type->get_methods([](const IMethod* m) { return m->name() == "Deconstruct"; }, typesystem::GetMemberOptions::IgnoreInheritedMembers);
            for (const IMethod* m : deconstructs) {
                if (!m->is_static() && static_cast<int>(m->parameters().size()) == out_param_count) {
                    // An instance Deconstruct of the same arity is declared on a type more derived than the called method's declaring
                    // type: it hides the called method (and wins over a called extension method).
                    return false;
                }
            }
            const IType* base_class = nullptr;
            for (const IType* t : type->direct_base_types()) {
                if (t->kind() == TypeKind::Class) {
                    base_class = t;
                    break;
                }
            }
            type = base_class;
        }
        // The chain ended without seeing the declaring type, so an instance method's binding cannot be verified. An extension method is
        // reached by its receiver type, and one declared on a more derived type wins over it; which extensions are in scope where the
        // output is compiled is not known here, so the binding is only certain when the element type is the receiver type itself.
        return method->is_static() &&
               typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(element_type, method->parameters()[0]->type());
    };

    std::vector<std::pair<DeconstructionCall*, int>> pending_elements;
    pending_elements.push_back({root_call, 0});
    while (!pending_elements.empty()) {
        auto [parent, i] = pending_elements.back();
        pending_elements.pop_back();
        if (static_cast<std::size_t>(i) + 1 < parent->results.size()) pending_elements.push_back({parent, i + 1});
        ILVariable* result = parent->results[static_cast<std::size_t>(i)];
        int saved_pos = *pos;
        ILVariable* receiver = result;
        ILInstruction* inst = block->instructions.element_at_or_default(*pos);
        ILVariable* copy = nullptr;
        ILInstruction* copied_value = nullptr;
        if (inst != nullptr && inst->match_st_loc(&copy, &copied_value) && copied_value->match_ld_loc(static_cast<const ILVariable*>(result)) &&
            copy->store_count() == 1 && copy->load_count() + copy->address_count() == 1) {
            receiver = copy;
            (*pos)++;
            inst = block->instructions.element_at_or_default(*pos);
        }
        ILInstruction* unused_operand = nullptr;
        DeconstructionCall* nested = inst == nullptr ? nullptr : match_deconstruction_call(inst, &unused_operand);
        if (nested == nullptr || !is_receiver_reference(static_cast<CallInstruction*>(inst)->arguments()[0], receiver)) {
            *pos = saved_pos;
            continue;
        }
        if (receiver != result && result->load_count() != 1) {
            // the copy must be the element's only use
            *pos = saved_pos;
            continue;
        }
        if (!binds_on_element_type(nested->method, result->type())) {
            // A nested designation rebinds Deconstruct on the element's static type when recompiled; if that picks a different method
            // (member hiding), the call must stay explicit, where a cast can preserve the binding.
            *pos = saved_pos;
            continue;
        }
        (*pos)++;
        nested->receiver = receiver;
        parent->nested_calls[static_cast<std::size_t>(i)] = nested;
        // its elements are evaluated before the parent's remaining ones
        pending_elements.push_back({nested, 0});
    }
}

// stloc temp(ldobj(ldflda ItemN(ldloc(a) container)))   one per nested designation
// ...
// The temporaries a nested tuple designation is lowered to: parents before children, all evaluated before any conversions or
// assignments. The consumed variables form the tuple node tree rooted at the outermost tuple.
void DeconstructionTransform::match_nested_tuple_designations(Block* block, int* pos, const std::unordered_set<ILVariable*>* do_not_nest) {
    while (true) {
        ILVariable* temp = nullptr;
        ILVariable* container = nullptr;
        const IType* container_type = nullptr;
        int index = 0;
        if (!match_tuple_element_store(block->instructions.element_at_or_default(*pos), &temp, &container, &container_type, &index)) break;
        if (do_not_nest != nullptr && do_not_nest->contains(temp)) break;
        if (!(temp->store_count() == 1 && temp->load_count() + temp->address_count() >= 1)) break;
        // Every use of the temporary must itself be a tuple element read, 'ldobj(ldflda ItemK(...temp...))', so that the pattern can
        // consume them all; reads it does not consume are rejected by the escape check afterwards.
        bool all_uses_are_tuple_element_reads = true;
        auto is_element_read_use = [](ILInstruction* use) {
            auto* element_access = as<LdFlda>(use->parent());
            return element_access != nullptr && as<LdObj>(element_access->parent()) != nullptr;
        };
        for (LdLoca* use : temp->address_instructions())
            if (!is_element_read_use(use)) all_uses_are_tuple_element_reads = false;
        for (LdLoc* use : temp->load_instructions())
            if (!is_element_read_use(use)) all_uses_are_tuple_element_reads = false;
        if (!all_uses_are_tuple_element_reads) break;
        TupleNode* container_node = resolve_tuple_container(container, container_type);
        if (container_node == nullptr) break;
        if (index >= static_cast<int>(container_node->nested_elements.size()) || container_node->nested_elements[static_cast<std::size_t>(index)] != nullptr) break;
        // The container's element type is authoritative for the temporary's tuple type: a stack slot's own type can be imprecise. A
        // temporary with a precise type must agree with the element type.
        const IType* element_type = container_node->type->element_types()[static_cast<std::size_t>(index)];
        if (is_default_or_empty(typesystem::TupleType::get_tuple_element_types(element_type))) break;
        const TupleType* temp_type = typesystem::TupleType::from_underlying_type(context_->type_system(), element_type);
        if (temp_type == nullptr || temp_type->cardinality() < 2) break;
        if (!is_default_or_empty(typesystem::TupleType::get_tuple_element_types(temp->type())) &&
            !typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(element_type, temp->type())) {
            break;
        }
        TupleNode* node = make_tuple_node(temp, temp_type);
        container_node->nested_elements[static_cast<std::size_t>(index)] = node;
        // The temporary's store reads one element of the container.
        container_node->matched_access_count++;
        if (find_tuple_node(temp) != nullptr) throw std::invalid_argument("An item with the same key has already been added.");
        tuple_nodes_.push_back({temp, node});
        initialize_flat_leaf_indices();
        (*pos)++;
    }
}

// Resolves the container of a tuple element access against the tree of tuple nodes; the first access establishes its container as the
// root. Returns null if the container is not part of the tree or its type does not fit a deconstruction.
DeconstructionTransform::TupleNode* DeconstructionTransform::resolve_tuple_container(ILVariable* container, const IType* container_type) {
    const TupleType* normalized_type = typesystem::TupleType::from_underlying_type(context_->type_system(), container_type);
    if (normalized_type == nullptr || normalized_type->cardinality() < 2) return nullptr;
    if (tuple_root_ == nullptr) {
        tuple_root_ = make_tuple_node(container, normalized_type);
        if (find_tuple_node(container) != nullptr) throw std::invalid_argument("An item with the same key has already been added.");
        tuple_nodes_.push_back({container, tuple_root_});
        initialize_flat_leaf_indices();
    }
    TupleNode* node = find_tuple_node(container);
    if (node == nullptr) return nullptr;
    return node->type->equals(normalized_type) ? node : nullptr;
}

// Assigns depth-first flat leaf indices to every element of the tuple node tree and allocates the flat results array. Depth-first order
// is the order in which the consumers pair pattern variables with conversions and assignments. Called whenever the tree grows; the
// results array is still empty then, because the tree is complete before MatchConversions/MatchAssignments start populating it.
void DeconstructionTransform::initialize_flat_leaf_indices() {
    std::function<int(TupleNode*, int)> assign_flat_indices = [&](TupleNode* node, int next_leaf_index) {
        node->element_flat_index.assign(static_cast<std::size_t>(node->type->cardinality()), 0);
        for (int i = 0; i < node->type->cardinality(); i++) {
            node->element_flat_index[static_cast<std::size_t>(i)] = next_leaf_index;
            if (node->nested_elements[static_cast<std::size_t>(i)] != nullptr)
                next_leaf_index = assign_flat_indices(node->nested_elements[static_cast<std::size_t>(i)], next_leaf_index);
            else
                next_leaf_index++;
        }
        return next_leaf_index;
    };
    int total_leaves = assign_flat_indices(tuple_root_, 0);
    deconstruction_results_.assign(static_cast<std::size_t>(total_leaves), nullptr);
}

// stloc conv0(conv(FindIndex-resolvable value))
// stloc conv1(conv(...))
// ...
// The run of single-use conversion temporaries following the deconstruction, in flat leaf index order.
bool DeconstructionTransform::match_conversions(Block* block, int* pos, std::vector<std::pair<ILVariable*, ConversionInfo>>* conversions,
                                                std::vector<StLoc*>* conversion_st_locs, DelayedActions* delayed_actions) {
    conversions->clear();
    conversion_st_locs->clear();
    int previous_index = -1;
    ILInstruction* input_instruction = nullptr;
    ILVariable* output_variable = nullptr;
    ConversionInfo info;
    while (match_conversion(block->instructions.element_at_or_default(*pos), &input_instruction, &output_variable, &info)) {
        DelayedActions tuple_access_adjustment;
        int index = find_index(input_instruction, &tuple_access_adjustment);
        if (index <= previous_index) return false;
        if (!(output_variable->is_single_definition() && output_variable->load_count() == 1)) return false;
        add_actions(delayed_actions, tuple_access_adjustment);
        add_deconstruction_result(output_variable, index);
        for (const auto& entry : *conversions)
            if (entry.first == output_variable) throw std::invalid_argument("An item with the same key has already been added.");
        conversions->push_back({output_variable, info});
        conversion_st_locs->push_back(static_cast<StLoc*>(block->instructions[*pos]));
        (*pos)++;
        previous_index = index;
    }
    return true;
}

// stloc output(conv(input))
bool DeconstructionTransform::match_conversion(ILInstruction* inst, ILInstruction** input_instruction, ILVariable** output_variable, ConversionInfo* info) {
    *info = ConversionInfo();
    *input_instruction = nullptr;
    *output_variable = nullptr;
    if (inst == nullptr) return false;
    ILInstruction* value = nullptr;
    if (!inst->match_st_loc(output_variable, &value)) return false;
    auto* conv = as<Conv>(value);
    if (conv == nullptr) return false;
    info->input_type = infer_type(conv->argument(), &context_->type_system());
    info->conv = conv;
    *input_instruction = conv->argument();
    return true;
}

// assignment(FindIndex-resolvable value)                see MatchAssignment
// ...
// The run of assignments following the conversions, in flat leaf index order. Single-use elements without an assignment are forwarded
// through a fresh variable assigned inside the deconstruction.
bool DeconstructionTransform::match_assignments(Block* block, int* pos, const std::vector<std::pair<ILVariable*, ConversionInfo>>& conversions,
                                                const std::vector<StLoc*>& conversion_st_locs, DelayedActions* delayed_actions,
                                                bool allow_unrelated_assignments, bool* any_assignments) {
    *any_assignments = false;
    int previous_index = -1;
    int conversion_st_loc_index = 0;
    int start_pos = *pos;

    auto add_missing_assignments_for_conversions = [&](int index) {
        while (conversion_st_loc_index < static_cast<int>(conversion_st_locs.size())) {
            StLoc* st_loc = conversion_st_locs[static_cast<std::size_t>(conversion_st_loc_index)];
            int conversion_result_index = 0;
            if (!lookup_deconstruction_result(st_loc->variable(), &conversion_result_index)) throw std::out_of_range("The given key was not present in the dictionary.");

            if (conversion_result_index >= index) break;
            if (conversion_result_index > previous_index) {
                delayed_actions->push_back([this, st_loc](DeconstructInstruction* deconstruct_inst) {
                    ILVariable* fresh_var = context_->function()->register_variable(VariableKind::StackSlot, st_loc->variable()->type());
                    deconstruct_inst->assignments()->instructions.add(make<StLoc>(st_loc->variable(), make<LdLoc>(fresh_var)));
                    st_loc->set_variable(fresh_var);
                });
            }
            previous_index = conversion_result_index;
            conversion_st_loc_index++;
        }
    };

    const IType* target_type = nullptr;
    ILInstruction* value_inst = nullptr;
    DelayedAction add_assignment;
    while (match_assignment(block->instructions.element_at_or_default(*pos), &target_type, &value_inst, &add_assignment)) {
        DelayedActions tuple_access_adjustment;
        int index = find_index(value_inst, &tuple_access_adjustment);
        if (index < 0 && allow_unrelated_assignments) {
            // For a Deconstruct call the element list is fixed by the call's out-arguments, so an assignment whose value is unrelated to
            // the deconstruction just ends the pattern and stays after the deconstruct instruction. (For tuples the elements are
            // discovered from the assignments, so ending early would misread a suffix as the pattern: keep rejecting there.)
            break;
        }
        if (index <= previous_index) return false;
        add_missing_assignments_for_conversions(index);
        ConversionInfo conversion_info;
        ILVariable* result_variable = nullptr;
        bool found_conversion = false;
        if (value_inst->match_ld_loc(&result_variable)) {
            for (const auto& entry : conversions) {
                if (entry.first == result_variable) {
                    conversion_info = entry.second;
                    found_conversion = true;
                    break;
                }
            }
        }
        if (!found_conversion) {
            conversion_info = ConversionInfo();
            conversion_info.input_type = infer_type(value_inst, &context_->type_system());
        }
        ILVariable* assignment_target = nullptr;
        if (block->instructions[*pos]->match_st_loc(&assignment_target) && assignment_target->kind() == VariableKind::StackSlot &&
            assignment_target->is_single_definition() && conversion_info.conv == nullptr) {
            const IType* input_type = conversion_info.input_type;
            delayed_actions->push_back([assignment_target, input_type](DeconstructInstruction*) { assignment_target->set_type(input_type); });
        } else {
            if (!is_compatible_implicit_conversion(target_type, conversion_info)) return false;
        }
        add_action(delayed_actions, add_assignment);
        add_actions(delayed_actions, tuple_access_adjustment);
        (*pos)++;
        previous_index = index;
    }
    add_missing_assignments_for_conversions(INT_MAX);

    auto get_assignment_index = [this](ILInstruction* inst) {
        ILInstruction* value = nullptr;
        const IType* unused_type = nullptr;
        ILVariable* input_variable = nullptr;
        if (DeconstructInstruction::is_assignment(inst, &context_->type_system(), &unused_type, &value) && value->match_ld_loc(&input_variable)) {
            int index = 0;
            if (lookup_deconstruction_result(input_variable, &index)) return index;
            // Forwarding assignments produced for conversions load a fresh variable; their pattern index is that of the conversion
            // output they store to.
            if (auto* st_loc = as<StLoc>(inst)) {
                if (lookup_deconstruction_result(st_loc->variable(), &index)) return index;
            }
        }
        return INT_MAX;
    };

    for (ILVariable* v : deconstruction_results_) {
        // In optimized code a deconstruction element is not stored to a temporary, if it is used directly (and only once!) after the
        // deconstruction. This happens for trailing elements, but also for leading elements, e.g., when a nested deconstruction copies
        // the inner element to a temporary before the elements preceding it are used. Forward such elements through a fresh variable
        // assigned inside the deconstruction, so that every pattern variable's load is a descendant of the deconstruct instruction. The
        // assignment is inserted in pattern order, because StatementBuilder and ExpressionBuilder pair pattern variables with
        // assignments positionally. LoadCount must be read eagerly, at match time: for a tuple deconstruction the elements are the
        // fresh "E_i" variables created in FindIndex, whose loads only materialize when the delayed ReplaceWith actions run, so
        // LoadCount is still 0 here and forwarding never fires on that path. That is load-bearing, not incidental: the fresh variables
        // are never registered in deconstructionResultsLookup, so GetAssignmentIndex could not position a forwarding assignment among
        // a tuple's assignments.
        if (v == nullptr || v->load_count() != 1) continue;
        delayed_actions->push_back([this, v, get_assignment_index](DeconstructInstruction* deconstruct_inst) {
            LdLoc* load = v->load_instructions()[0];
            if (load->is_descendant_of(deconstruct_inst)) return;
            // MatchDeconstruction registered every deconstruction result in the lookup, and the tuple path never gets here (see above);
            // a miss would leave the load outside the deconstruct instruction, i.e. a malformed pattern, because the transform is
            // already committed at this point.
            int index = 0;
            bool is_deconstruction_result = lookup_deconstruction_result(v, &index);
            assert(is_deconstruction_result);
            (void)is_deconstruction_result;
            ILVariable* fresh_var = context_->function()->register_variable(VariableKind::StackSlot, v->type());
            auto& instructions = deconstruct_inst->assignments()->instructions;
            int insert_pos = 0;
            while (insert_pos < instructions.count() && get_assignment_index(instructions[insert_pos]) < index) insert_pos++;
            instructions.insert(insert_pos, make<StLoc>(fresh_var, make<LdLoc>(v)));
            load->set_variable(fresh_var);
        });
    }

    *any_assignments = start_pos != *pos;
    return true;
}

// stloc v(value) | stobj(target, value) | call set_Property(target, value)
// or the result-used form
//   stloc s(Block CallInlineAssign { call set_Property(target, stloc tmp(value)); final: ldloc tmp })
// where the setter call is moved into the assignments block.
bool DeconstructionTransform::match_assignment(ILInstruction* inst, const IType** target_type, ILInstruction** value_inst, DelayedAction* add_assignment) {
    *target_type = nullptr;
    *value_inst = nullptr;
    *add_assignment = nullptr;
    if (inst == nullptr) return false;
    ILVariable* v = nullptr;
    ILInstruction* value = nullptr;
    CallInstruction* call = nullptr;
    ILInstruction* unused_value = nullptr;
    if (inst->match_st_loc(&v, &value) && as<Block>(value) != nullptr && static_cast<Block*>(value)->match_inline_assign_block(&call, value_inst)) {
        if (!DeconstructInstruction::is_assignment(call, &context_->type_system(), target_type, &unused_value)) return false;
        if (!(v->is_single_definition() && v->load_count() == 0)) return false;
        ILInstruction* value_inst_copy = *value_inst;
        *add_assignment = [call, value_inst_copy](DeconstructInstruction* deconstruct_inst) {
            call->arguments().set(call->arguments().count() - 1, value_inst_copy);
            deconstruct_inst->assignments()->instructions.add(call);
        };
        return true;
    } else if (DeconstructInstruction::is_assignment(inst, &context_->type_system(), target_type, value_inst)) {
        // OK - use the assignment as is
        *add_assignment = [inst](DeconstructInstruction* deconstruct_inst) { deconstruct_inst->assignments()->instructions.add(inst); };
        return true;
    } else {
        return false;
    }
}

// ldloc result                                          a registered result or conversion output
// ldobj(ldflda ItemN(ldloc(a) v))                       an element read on the tuple node tree
// Resolves the value of a conversion or assignment to its flat leaf index. Returns -1 on failure.
int DeconstructionTransform::find_index(ILInstruction* inst, DelayedActions* delayed_actions) {
    delayed_actions->clear();
    ILVariable* v = nullptr;
    if (inst->match_ld_loc(&v)) {
        int index = 0;
        if (!lookup_deconstruction_result(v, &index)) return -1;
        return index;
    }
    ILVariable* container = nullptr;
    const IType* container_type = nullptr;
    int element_index = 0;
    if (!match_tuple_element_read(inst, &container, &container_type, &element_index)) return -1;
    if (rooted_in_deconstruct_call_) {
        // A pattern rooted in a Deconstruct call must not absorb tuple element accesses: discovering the tuple here would overwrite the
        // call's result bookkeeping and destroy the rewritten tuple access on failure.
        return -1;
    }
    TupleNode* node = resolve_tuple_container(container, container_type);
    if (node == nullptr) return -1;
    if (element_index >= static_cast<int>(node->nested_elements.size()) || node->nested_elements[static_cast<std::size_t>(element_index)] != nullptr) {
        // The element is bound to a nested designation; a direct read of it would be a second consumption of the same element.
        return -1;
    }
    int flat_index = node->element_flat_index[static_cast<std::size_t>(element_index)];
    node->matched_access_count++;
    if (deconstruction_results_[static_cast<std::size_t>(flat_index)] == nullptr) {
        auto* fresh_var = make<ILVariable>(VariableKind::StackSlot, node->type->element_types()[static_cast<std::size_t>(element_index)]);
        fresh_var->name = "E_" + std::to_string(flat_index);
        delayed_actions->push_back([this, fresh_var](DeconstructInstruction*) { context_->function()->variables().add(fresh_var); });
        deconstruction_results_[static_cast<std::size_t>(flat_index)] = fresh_var;
    }
    delayed_actions->push_back([this, inst, flat_index](DeconstructInstruction*) {
        inst->replace_with(make<LdLoc>(deconstruction_results_[static_cast<std::size_t>(flat_index)]));
    });
    return flat_index;
}

// Gets whether the matched conv instruction (or its absence) is the lowering of the implicit conversion from the input type to the
// assignment's target type.
bool DeconstructionTransform::is_compatible_implicit_conversion(const IType* target_type, const ConversionInfo& conversion_info) {
    const semantics::Conversion* c =
        csharp::resolver::CSharpConversions::get(context_->type_system()).implicit_conversion(conversion_info.input_type, target_type);
    if (!c->is_valid()) return false;
    const IType* input_type = conversion_info.input_type;
    Conv* conv = conversion_info.conv;
    if (c->is_identity_conversion() || c->is_reference_conversion()) {
        return conv == nullptr || conv->kind == ConversionKind::Nop;
    }
    if (c->is_numeric_conversion() && conv != nullptr) {
        switch (conv->kind) {
            case ConversionKind::IntToFloat: return typesystem::get_sign(*input_type) == conv->input_sign;
            case ConversionKind::FloatPrecisionChange: return true;
            case ConversionKind::SignExtend: return typesystem::get_sign(*input_type) == Sign::Signed;
            case ConversionKind::ZeroExtend: return typesystem::get_sign(*input_type) == Sign::Unsigned;
            default: return false;
        }
    }
    return false;
}

// Builds, recursing into nested calls:
// match.deconstruct[Method] (matchVariable = testedOperand) {
//     match(result_i = deconstruct.result i(ldloc matchVariable)),
//     match.deconstruct[...] (receiver_j = deconstruct.result j(ldloc matchVariable)) { ... }
// }
MatchInstruction* DeconstructionTransform::build_pattern_match(DeconstructionCall* call, ILVariable* match_variable, ILInstruction* tested_operand) {
    match_variable->set_kind(VariableKind::PatternLocal);
    auto* match = make<MatchInstruction>(match_variable, call->method, tested_operand);
    match->is_deconstruct_call = true;
    for (std::size_t i = 0; i < call->results.size(); i++) {
        DeconstructionCall* nested = call->nested_calls[i];
        if (nested != nullptr) {
            ILVariable* receiver = nested->receiver;
            auto* result_inst = make<DeconstructResultInstruction>(static_cast<int>(i), receiver->stack_type, make<LdLoc>(match_variable));
            match->sub_patterns().add(build_pattern_match(nested, receiver, result_inst));
        } else {
            ILVariable* result = call->results[i];
            result->set_kind(VariableKind::PatternLocal);
            auto* result_inst = make<DeconstructResultInstruction>(static_cast<int>(i), result->stack_type, make<LdLoc>(match_variable));
            match->sub_patterns().add(make<MatchInstruction>(result, result_inst));
        }
    }
    return match;
}

// Builds, recursing into nested designations:
// match.tuple (matchVariable = testedOperand) {
//     match(result_i = deconstruct.result i(ldloc matchVariable)),
//     match.tuple (temp_j = deconstruct.result j(ldloc matchVariable)) { ... }
// }
// Unassigned leaf elements get a fresh, load-free pattern variable (a discard).
MatchInstruction* DeconstructionTransform::build_tuple_pattern_match(TupleNode* node, ILVariable* match_variable, ILInstruction* tested_operand) {
    match_variable->set_kind(VariableKind::PatternLocal);
    auto* match = make<MatchInstruction>(match_variable, nullptr, tested_operand);
    match->is_deconstruct_tuple = true;
    for (int i = 0; i < node->type->cardinality(); i++) {
        TupleNode* nested = node->nested_elements[static_cast<std::size_t>(i)];
        if (nested != nullptr) {
            // A stack-slot temporary can have an imprecise type; the match variable of a tuple pattern must have the tuple type.
            if (is_default_or_empty(typesystem::TupleType::get_tuple_element_types(nested->variable->type()))) nested->variable->set_type(nested->type);
            auto* result_inst = make<DeconstructResultInstruction>(i, nested->variable->stack_type, make<LdLoc>(match_variable));
            match->sub_patterns().add(build_tuple_pattern_match(nested, nested->variable, result_inst));
        } else {
            int flat_index = node->element_flat_index[static_cast<std::size_t>(i)];
            ILVariable* result = deconstruction_results_[static_cast<std::size_t>(flat_index)];
            if (result == nullptr) {
                auto* fresh_var = make<ILVariable>(VariableKind::PatternLocal, node->type->element_types()[static_cast<std::size_t>(i)]);
                fresh_var->name = "E_" + std::to_string(flat_index);
                context_->function()->variables().add(fresh_var);
                result = fresh_var;
            } else {
                result->set_kind(VariableKind::PatternLocal);
            }
            auto* result_inst = make<DeconstructResultInstruction>(i, result->stack_type, make<LdLoc>(match_variable));
            match->sub_patterns().add(make<MatchInstruction>(result, result_inst));
        }
    }
    return match;
}

// ldobj(ldflda ItemN(ldloc(a) container))
// The returned index is zero-based; Rest chains of long tuples are flattened. Non-escaping element reads may have been rewritten from
// ldloca to ldloc, so both load kinds are accepted.
bool DeconstructionTransform::match_tuple_element_read(ILInstruction* inst, ILVariable** container, const IType** container_type, int* index) {
    *container = nullptr;
    *container_type = nullptr;
    *index = -1;
    auto* ldobj = as<LdObj>(inst);
    if (ldobj == nullptr) return false;
    auto* ldflda = as<LdFlda>(ldobj->target());
    if (ldflda == nullptr) return false;
    if (ldobj->unaligned_prefix() != 0 || ldobj->is_volatile()) return false;
    ILInstruction* target = nullptr;
    int position = 0;
    if (!TupleTransform::match_tuple_field_access(ldflda, container_type, &target, &position)) return false;
    // Item fields are one-based, we use zero-based indexing.
    *index = position - 1;
    return match_ld_loc_or_ld_loca(target, container);
}

// stloc temp(ldobj(ldflda ItemN(ldloc(a) container)))
// The store of a nested tuple designation temporary.
bool DeconstructionTransform::match_tuple_element_store(ILInstruction* inst, ILVariable** temp, ILVariable** container, const IType** container_type, int* index) {
    if (auto* store = as<StLoc>(inst); store != nullptr && match_tuple_element_read(store->value(), container, container_type, index)) {
        *temp = store->variable();
        return true;
    }
    *temp = nullptr;
    *container = nullptr;
    *container_type = nullptr;
    *index = -1;
    return false;
}

// ldloc variable | ldloca variable
bool DeconstructionTransform::match_ld_loc_or_ld_loca(ILInstruction* inst, ILVariable** variable) {
    return inst->match_ld_loc(variable) || inst->match_ld_loca(variable);
}

}  // namespace cb::dotnet::decompiler::il::transforms

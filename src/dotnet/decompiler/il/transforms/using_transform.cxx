// dotnet/decompiler/il/transforms/using_transform.cxx -- see using_transform.hpp.
#include "dotnet/decompiler/il/transforms/using_transform.hpp"

#include <algorithm>
#include <optional>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;

void UsingTransform::run(Block* block, BlockTransformContext& context) {
    if (!context.settings().using_statement) return;
    context_ = &context;
    for (int i = context.index_of_first_already_transformed_instruction - 1; i >= 0; i--) {
        if (transform_using(block, i)) {
            context.index_of_first_already_transformed_instruction = block->instructions.count();
            continue;
        }
        if (transform_using_vb(block, i)) {
            context.index_of_first_already_transformed_instruction = block->instructions.count();
            continue;
        }
        if (transform_async_using(block, i)) {
            context.index_of_first_already_transformed_instruction = block->instructions.count();
            continue;
        }
    }
}

bool UsingTransform::transform_using(Block* block, int i) {
    if (i + 1 >= block->instructions.count()) return false;
    auto* try_finally = as<TryFinally>(block->instructions[i + 1]);
    auto* store_inst = as<StLoc>(block->instructions[i]);
    if (try_finally == nullptr || store_inst == nullptr) return false;
    if (!(store_inst->value()->match_ld_null() || check_resource_type(store_inst->variable()->type()))) return false;
    if (store_inst->variable()->kind() != VariableKind::Local) return false;
    for (LdLoc* ld : store_inst->variable()->load_instructions())
        if (!ld->is_descendant_of(try_finally)) return false;
    auto validate_address_use = [&](LdLoca* la) {
        if (!la->is_descendant_of(try_finally)) return false;
        if (la->is_descendant_of(try_finally->try_block())) {
            if (!(ILInlining::is_used_as_this_pointer_in_call(la) || ILInlining::is_passed_to_in_parameter(la))) return false;
        }
        return true;
    };
    for (LdLoca* la : store_inst->variable()->address_instructions())
        if (!validate_address_use(la)) return false;
    if (store_inst->variable()->store_instructions().size() > 1) return false;
    auto* container = as<BlockContainer>(try_finally->finally_block());
    if (container == nullptr) return false;
    if (!match_dispose_block(container, store_inst->variable(), store_inst->value()->match_ld_null(), "System.IDisposable.Dispose",
                             KnownTypeCode::IDisposable))
        return false;
    context_->step("UsingTransform", try_finally);
    store_inst->variable()->set_kind(VariableKind::UsingLocal);
    block->instructions.remove_at(i + 1);
    auto* using_inst = make<UsingInstruction>(store_inst->variable(), store_inst->value(), try_finally->try_block());
    using_inst->set_is_ref_struct(context_->settings().introduce_ref_modifiers_on_structs &&
                                  store_inst->variable()->type()->kind() == TypeKind::Struct && store_inst->variable()->type()->is_by_ref_like());
    with_il_range(using_inst, store_inst);
    block->instructions.set(i, using_inst);
    context_->end_step(using_inst);
    return true;
}

bool UsingTransform::transform_using_vb(Block* block, int i) {
    if (i >= block->instructions.count()) return false;
    auto* try_finally = as<TryFinally>(block->instructions[i]);
    if (try_finally == nullptr) return false;
    auto* try_container = as<BlockContainer>(try_finally->try_block());
    if (try_container == nullptr) return false;
    auto* store_inst = as<StLoc>(try_container->entry_point()->instructions.first_or_default());
    if (store_inst == nullptr) return false;
    if (!(store_inst->value()->match_ld_null() || check_resource_type(store_inst->variable()->type()))) return false;
    if (store_inst->variable()->kind() != VariableKind::Local) return false;
    for (LdLoc* ld : store_inst->variable()->load_instructions())
        if (!ld->is_descendant_of(try_finally)) return false;
    for (LdLoca* la : store_inst->variable()->address_instructions())
        if (!la->is_descendant_of(try_finally) || (la->is_descendant_of(try_finally->try_block()) && !ILInlining::is_used_as_this_pointer_in_call(la)))
            return false;
    if (store_inst->variable()->store_instructions().size() > 1) return false;
    auto* container = as<BlockContainer>(try_finally->finally_block());
    if (container == nullptr ||
        !match_dispose_block(container, store_inst->variable(), store_inst->value()->match_ld_null(), "System.IDisposable.Dispose",
                             KnownTypeCode::IDisposable))
        return false;
    context_->step("UsingTransformVB", try_finally);
    store_inst->variable()->set_kind(VariableKind::UsingLocal);
    try_container->entry_point()->instructions.remove_at(0);
    auto* using_inst = make<UsingInstruction>(store_inst->variable(), store_inst->value(), try_finally->try_block());
    block->instructions.set(i, using_inst);
    context_->end_step(using_inst);
    return true;
}

bool UsingTransform::check_resource_type(const IType* type) {
    // non-generic IEnumerator does not implement IDisposable.
    // This is a workaround for non-generic foreach.
    if (typesystem::is_known_type(type, KnownTypeCode::IEnumerator)) return true;
    for (const IType* b : typesystem::get_all_base_types(type))
        if (typesystem::is_known_type(b, KnownTypeCode::IEnumerator)) return true;
    for (const IType* b : typesystem::get_all_base_types(typesystem::nullable_type::get_underlying_type(type)))
        if (typesystem::is_known_type(b, KnownTypeCode::IDisposable)) return true;
    if (context_->settings().introduce_ref_modifiers_on_structs && type->kind() == TypeKind::Struct && type->is_by_ref_like()) return true;
    // General GetEnumerator-pattern?
    auto methods = type->get_methods([](const IMethod* m) {
        return m->name() == "GetEnumerator" && m->type_parameters().empty() && m->parameters().empty();
    });
    for (const IMethod* m : methods)
        if (implements_foreach_pattern(m->return_type())) return true;
    return false;
}

bool UsingTransform::implements_foreach_pattern(const IType* type) {
    auto move_next = type->get_methods([](const IMethod* m) {
        return m->name() == "MoveNext" && m->type_parameters().empty() && m->parameters().empty();
    });
    bool any_bool = false;
    for (const IMethod* m : move_next)
        if (typesystem::is_known_type(m->return_type(), KnownTypeCode::Boolean)) {
            any_bool = true;
            break;
        }
    if (!any_bool) return false;
    auto current = type->get_properties([](const typesystem::IProperty* p) { return p->name() == "Current" && p->can_get() && !p->is_indexer(); });
    if (current.empty()) return false;
    return true;
}

//	finally BlockContainer {
//		Block IL_0012(incoming: 1) {
//			if (comp(ldloc obj != ldnull)) Block IL_001a  {
//				callvirt Dispose(obj)
//			}
//			leave IL_0012(nop)
//		}
// }
bool UsingTransform::match_dispose_block(BlockContainer* container, ILVariable* obj_var, bool using_null, const std::string& dispose_method_full_name,
                                         KnownTypeCode dispose_type_code) {
    Block* entry_point = container->entry_point();
    if (entry_point->incoming_edge_count() != 1) return false;
    int pos = 0;
    // `objVar.Type.IsReferenceType != false`
    std::optional<bool> reference_type = obj_var->type()->is_reference_type();
    bool is_reference = !(reference_type.has_value() && !*reference_type);
    // optional:
    // stloc temp(isinst TDisposable(ldloc obj))
    ILVariable* temp_var = nullptr;
    ILInstruction* isinst = nullptr;
    ILInstruction* at_pos = entry_point->instructions.element_at_or_default(pos);
    if (at_pos != nullptr && at_pos->match_st_loc(&temp_var, &isinst)) {
        ILInstruction* load = nullptr;
        const IType* disposable_type = nullptr;
        if (!isinst->match_is_inst(&load, &disposable_type) || !load->match_ld_loc(obj_var) ||
            !typesystem::is_known_type(disposable_type, dispose_type_code)) {
            return false;
        }
        if (!temp_var->is_single_definition()) return false;
        is_reference = true;
        pos++;
        obj_var = temp_var;
    }
    // if (comp(ldloc obj != ldnull)) Block IL_001a  {
    //	callvirt Dispose(obj)
    // }
    ILInstruction* check_inst = entry_point->instructions.element_at_or_default(pos);
    if (check_inst == nullptr) return false;
    int num_obj_var_loads_in_check = 0;
    if (!match_dispose_check(obj_var, check_inst, is_reference, using_null, &num_obj_var_loads_in_check, dispose_method_full_name, dispose_type_code)) {
        return false;
    }
    // make sure the (optional) temporary is used only in the dispose check
    if (pos > 0 && obj_var->load_count() != num_obj_var_loads_in_check) return false;
    pos++;
    // make sure, the finally ends in a leave(nop) instruction.
    ILInstruction* leave = entry_point->instructions.element_at_or_default(pos);
    ILInstruction* return_value = nullptr;
    if (leave == nullptr || !leave->match_leave(container, &return_value)) return false;
    if (!return_value->match_nop()) return false;
    // leave is the last instruction
    return (pos + 1) == entry_point->instructions.count();
}

bool UsingTransform::match_dispose_check(ILVariable* obj_var, ILInstruction* check_inst, bool is_reference, bool using_null,
                                         int* num_obj_var_loads_in_check, std::string dispose_method_full_name, KnownTypeCode dispose_type_code) {
    *num_obj_var_loads_in_check = 2;
    ILInstruction* dispose_invocation = nullptr;
    CallInstruction* dispose_call = nullptr;
    if (typesystem::is_known_type(obj_var->type(), KnownTypeCode::NullableOfT)) {
        ILInstruction* condition = nullptr;
        ILInstruction* dispose_inst = nullptr;
        if (check_inst->match_if_instruction(&condition, &dispose_inst)) {
            if (!NullableLiftingTransform::match_has_value_call(condition, obj_var)) return false;
            auto* dispose_block = as<Block>(dispose_inst);
            if (dispose_block == nullptr || dispose_block->instructions.count() != 1) return false;
            dispose_invocation = dispose_block->instructions[0];
        } else if (check_inst->match_nullable_rewrap(&dispose_inst)) {
            dispose_invocation = dispose_inst;
        } else {
            return false;
        }
        if (dispose_type_code == KnownTypeCode::IAsyncDisposable) {
            if (!unwrap_await(&dispose_invocation)) return false;
        }
        dispose_call = as<CallVirt>(dispose_invocation);
        if (dispose_call == nullptr) return false;
        if (dispose_call->method->full_name() != dispose_method_full_name) return false;
        if (!dispose_call->method->parameters().empty()) return false;
        if (dispose_call->arguments().count() != 1) return false;
        ILInstruction* first_arg = dispose_call->arguments().first_or_default();
        ILInstruction* inner_arg1 = nullptr;
        const IType* unbox_type = nullptr;
        if (!(first_arg->match_unbox_any(&inner_arg1, &unbox_type) && typesystem::is_known_type(unbox_type, dispose_type_code))) {
            ILInstruction* inner_arg2 = nullptr;
            const IType* address_type = nullptr;
            if (!first_arg->match_address_of(&inner_arg2, &address_type)) return false;
            if (NullableLiftingTransform::match_get_value_or_default(inner_arg2, obj_var)) return true;
            auto* unwrap = as<NullableUnwrap>(inner_arg2);
            return unwrap != nullptr && unwrap->argument()->match_ld_loc(obj_var);
        } else {
            const IType* box_type = nullptr;
            if (!(inner_arg1->match_box(&first_arg, &box_type) && typesystem::is_known_type(box_type, KnownTypeCode::NullableOfT) &&
                  typesystem::type_equals(typesystem::nullable_type::get_underlying_type(box_type),
                                          typesystem::nullable_type::get_underlying_type(obj_var->type()))))
                return false;
            return first_arg->match_ld_loc(obj_var);
        }
    } else {
        ILInstruction* target = nullptr;
        bool boxed_value = false;
        if (auto* rewrap = as<NullableRewrap>(check_inst); is_reference && rewrap != nullptr) {
            // the null check of reference types might have been transformed into "objVar?.Dispose();"
            auto* cv = as<CallVirt>(rewrap->argument());
            if (cv == nullptr) return false;
            target = cv->arguments().first_or_default();
            if (auto* ld_obj_if_ref = as<LdObjIfRef>(target)) target = ld_obj_if_ref->target();
            auto* unwrap = as<NullableUnwrap>(target);
            if (unwrap == nullptr) return false;
            *num_obj_var_loads_in_check = 1;
            dispose_call = cv;
            target = unwrap->argument();
        } else if (is_reference) {
            // reference types have a null check.
            ILInstruction* condition = nullptr;
            ILInstruction* dispose_inst = nullptr;
            if (!check_inst->match_if_instruction(&condition, &dispose_inst)) return false;
            bool is_inlined_is_inst = false;
            if (!match_null_check_or_type_check(condition, &obj_var, dispose_type_code, &is_inlined_is_inst)) return false;
            auto* dispose_block = as<Block>(dispose_inst);
            if (dispose_block == nullptr || dispose_block->instructions.count() != 1) return false;
            dispose_invocation = dispose_block->instructions[0];
            if (dispose_type_code == KnownTypeCode::IAsyncDisposable) {
                if (!unwrap_await(&dispose_invocation)) return false;
            }
            auto* cv = as<CallVirt>(dispose_invocation);
            if (cv == nullptr) return false;
            target = cv->arguments().first_or_default();
            if (target == nullptr) return false;
            if (auto* ld_obj_if_ref = as<LdObjIfRef>(target)) target = ld_obj_if_ref->target();
            ILInstruction* new_target = nullptr;
            const IType* type = nullptr;
            if (target->match_box(&new_target, &type) && typesystem::type_equals(type, obj_var->type()))
                target = new_target;
            else if (is_inlined_is_inst && target->match_is_inst(&new_target, &type) && typesystem::is_known_type(type, dispose_type_code))
                target = new_target;
            dispose_call = cv;
        } else if (obj_var->type()->kind() == TypeKind::Struct && obj_var->type()->is_by_ref_like()) {
            auto* call = as<Call>(check_inst);
            if (call == nullptr || call->method->declaring_type() != obj_var->type()) return false;
            target = call->arguments().first_or_default();
            if (target == nullptr) return false;
            if (call->method->name() != "Dispose") return false;
            dispose_method_full_name = call->method->full_name();
            dispose_call = call;
        } else {
            if (dispose_type_code == KnownTypeCode::IAsyncDisposable) {
                if (!unwrap_await(&check_inst)) return false;
            }
            auto* cv = as<CallInstruction>(check_inst);
            if (cv == nullptr) return false;
            target = cv->arguments().first_or_default();
            if (target == nullptr) return false;
            ILInstruction* new_target = nullptr;
            const IType* type = nullptr;
            if (target->match_box(&new_target, &type) && typesystem::type_equals(type, obj_var->type())) {
                std::optional<bool> is_ref = type->is_reference_type();
                boxed_value = !(is_ref.has_value() && *is_ref);
                target = new_target;
            }
            dispose_call = cv;
        }
        if (dispose_call->method->is_static()) return false;
        if (dispose_type_code == KnownTypeCode::IAsyncDisposable) {
            if (dispose_call->method->name() != "DisposeAsync") return false;
        } else {
            if (dispose_call->method->full_name() != dispose_method_full_name) return false;
        }

        if (!dispose_call->method->parameters().empty()) return false;
        if (dispose_call->arguments().count() != 1) return false;
        if (target->match_ld_loc_ref(obj_var) || (boxed_value && target->match_ld_loc(obj_var)) ||
            (using_null && dispose_call->arguments()[0]->match_ld_null()))
            return true;
        ILInstruction* arg = nullptr;
        const IType* type2 = nullptr;
        return is_reference && is<NullableRewrap>(check_inst) && target->match_is_inst(&arg, &type2) && arg->match_ld_loc(obj_var) &&
               typesystem::is_known_type(type2, dispose_type_code);
    }
}

bool UsingTransform::match_null_check_or_type_check(ILInstruction* condition, ILVariable** obj_var, KnownTypeCode dispose_type,
                                                    bool* is_inlined_is_inst) {
    *is_inlined_is_inst = false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (condition->match_comp_not_equals(&left, &right)) {
        ILVariable* inline_assign_var = nullptr;
        ILInstruction* inline_assign_val = nullptr;
        ILInstruction* arg = nullptr;
        const IType* type = nullptr;
        if (left->match_st_loc(&inline_assign_var, &inline_assign_val)) {
            if (!inline_assign_val->match_is_inst(&arg, &type) || !typesystem::is_known_type(type, dispose_type)) return false;
            if (!inline_assign_var->is_single_definition() || inline_assign_var->load_count() != 1) return false;
            if (!typesystem::is_known_type(inline_assign_var->type(), dispose_type)) return false;
            *is_inlined_is_inst = true;
            left = arg;
            if (!left->match_ld_loc(*obj_var) || !right->match_ld_null()) return false;
            *obj_var = inline_assign_var;
            return true;
        } else if (left->match_is_inst(&arg, &type) && typesystem::is_known_type(type, dispose_type)) {
            *is_inlined_is_inst = true;
            left = arg;
        }
        if (!left->match_ld_loc(*obj_var) || !right->match_ld_null()) return false;
        return true;
    }
    if (auto* match = as<MatchInstruction>(condition);
        match != nullptr && match->check_not_null && match->check_type && is<LdLoc>(match->tested_operand())) {
        ILVariable* v = as<LdLoc>(match->tested_operand())->variable();
        ILVariable* new_obj_var = match->variable();
        if (v != *obj_var) return false;
        if (!typesystem::is_known_type(new_obj_var->type(), dispose_type)) return false;
        *obj_var = new_obj_var;
        return true;
    }
    return false;
}

bool UsingTransform::transform_async_using(Block* block, int i) {
    if (!context_->settings().async_using_and_for_each_statement) return false;
    if (i < 1 || i >= block->instructions.count()) return false;
    auto* try_finally = as<TryFinally>(block->instructions[i]);
    auto* store_inst = as<StLoc>(block->instructions[i - 1]);
    if (try_finally == nullptr || store_inst == nullptr) return false;
    std::string dispose_method_full_name;
    if (!check_async_resource_type(store_inst->variable()->type(), &dispose_method_full_name)) return false;
    if (store_inst->variable()->kind() != VariableKind::Local) return false;
    for (LdLoc* ld : store_inst->variable()->load_instructions())
        if (!ld->is_descendant_of(try_finally)) return false;
    for (LdLoca* la : store_inst->variable()->address_instructions())
        if (!la->is_descendant_of(try_finally) || (la->is_descendant_of(try_finally->try_block()) && !ILInlining::is_used_as_this_pointer_in_call(la)))
            return false;
    if (store_inst->variable()->store_instructions().size() > 1) return false;
    auto* container = as<BlockContainer>(try_finally->finally_block());
    if (container == nullptr || !match_dispose_block(container, store_inst->variable(), false, dispose_method_full_name, KnownTypeCode::IAsyncDisposable))
        return false;
    context_->step("AsyncUsingTransform", try_finally);
    store_inst->variable()->set_kind(VariableKind::UsingLocal);
    block->instructions.remove_at(i);
    auto* using_inst = make<UsingInstruction>(store_inst->variable(), store_inst->value(), try_finally->try_block());
    using_inst->set_is_async(true);
    with_il_range(using_inst, store_inst);
    block->instructions.set(i - 1, using_inst);
    context_->end_step(using_inst);
    return true;
}

bool UsingTransform::check_async_resource_type(const IType* type, std::string* dispose_method_full_name) {
    dispose_method_full_name->clear();
    const IType* t = typesystem::nullable_type::get_underlying_type(type);
    for (const IType* b : typesystem::get_all_base_types(t)) {
        if (typesystem::is_known_type(b, KnownTypeCode::IAsyncDisposable)) {
            *dispose_method_full_name = "System.IAsyncDisposable.DisposeAsync";
            return true;
        }
    }

    auto dispose_methods = t->get_methods([](const IMethod* m) {
        return m->parameters().empty() && m->type_parameters().empty() && m->name() == "DisposeAsync";
    });
    // SingleOrDefault: no method is null, more than one is an error
    if (dispose_methods.size() > 1) throw UsageError("Sequence contains more than one matching element");
    if (!dispose_methods.empty()) {
        *dispose_method_full_name = dispose_methods[0]->full_name();
        return true;
    }

    return false;
}

bool UsingTransform::unwrap_await(ILInstruction** await_instruction) {
    if (*await_instruction == nullptr) return false;
    ILInstruction* arg = nullptr;
    if (!(*await_instruction)->match_await(&arg)) return false;
    ILInstruction* await_instruction_in_address_of = nullptr;
    const IType* type = nullptr;
    if (arg->match_address_of(&await_instruction_in_address_of, &type)) {
        *await_instruction = await_instruction_in_address_of;
    } else {
        *await_instruction = arg;
    }
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms

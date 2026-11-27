// dotnet/decompiler/il/transforms/lock_transform.cpp -- see lock_transform.hpp.
#include "dotnet/decompiler/il/transforms/lock_transform.hpp"

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::transforms {

void LockTransform::run(Block* block, BlockTransformContext& context) {
    if (!context.settings().lock_statement) return;
    context_ = &context;
    for (int i = context.index_of_first_already_transformed_instruction - 1; i >= 0; i--) {
        bool changed = do_transform(block, i);
        if (changed) {
            context.index_of_first_already_transformed_instruction = block->instructions.count();
        }
        // This happens in some cases:
        // Use correct index after transformation.
        if (i >= block->instructions.count()) i = block->instructions.count();
    }
}

bool LockTransform::do_transform(Block* block, int i) {
    if (transform_lock_roslyn(block, i)) {
        return true;
    }
    if (transform_lock_v4(block, i)) {
        return true;
    }
    if (transform_lock_v4_yield_return(block, i)) {
        return true;
    }
    if (transform_lock_v2(block, i)) {
        return true;
    }
    return transform_lock_mcs(block, i);
}

/// stloc lockObj(lockExpression)
/// call Enter(ldloc lockObj)
/// .try BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// } finally BlockContainer {
///     Block exitBlock (incoming: 1) {
///         call Exit(ldloc lockObj)
///         leave exitBlock (nop)
///     }
/// }
/// =>
/// .lock (lockExpression) BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// }
bool LockTransform::transform_lock_mcs(Block* block, int i) {
    if (i < 2) return false;
    auto* body = as<TryFinally>(block->instructions[i]);
    auto* object_store = as<StLoc>(block->instructions[i - 2]);
    if (body == nullptr || object_store == nullptr ||
        !match_call(as<Call>(block->instructions[i - 1]), "Enter", {object_store->variable()}))
        return false;
    if (!object_store->variable()->is_single_definition()) return false;
    auto* try_container = as<BlockContainer>(body->try_block());
    if (try_container == nullptr || try_container->entry_point()->instructions.count() == 0 ||
        try_container->entry_point()->incoming_edge_count() != 1)
        return false;
    auto* finally_container = as<BlockContainer>(body->finally_block());
    if (finally_container == nullptr || !match_exit_block(finally_container->entry_point(), nullptr, object_store->variable())) return false;
    if (object_store->variable()->load_count() > 2) return false;
    context_->step("LockTransformMCS", block);
    block->instructions.remove_at(i - 1);
    block->instructions.remove_at(i - 2);
    auto* lock_instruction = with_il_range(make<LockInstruction>(object_store->value(), body->try_block()), object_store);
    body->replace_with(lock_instruction);
    context_->end_step(lock_instruction);
    return true;
}

/// stloc lockObj(ldloc tempVar)
/// call Enter(ldloc tempVar)
/// .try BlockContainer {
///     Block lockBlock(incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// } finally BlockContainer {
///     Block exitBlock(incoming: 1) {
///         call Exit(ldloc lockObj)
///         leave exitBlock (nop)
///     }
/// }
/// =>
/// .lock (lockObj) BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// }
bool LockTransform::transform_lock_v2(Block* block, int i) {
    if (i < 2) return false;
    auto* body = as<TryFinally>(block->instructions[i]);
    auto* object_store = as<StLoc>(block->instructions[i - 2]);
    ILVariable* temp_var = nullptr;
    if (body == nullptr || object_store == nullptr || !object_store->value()->match_ld_loc(&temp_var) ||
        !match_call(as<Call>(block->instructions[i - 1]), "Enter", {temp_var}))
        return false;
    if (!object_store->variable()->is_single_definition()) return false;
    auto* try_container = as<BlockContainer>(body->try_block());
    if (try_container == nullptr || try_container->entry_point()->instructions.count() == 0 ||
        try_container->entry_point()->incoming_edge_count() != 1)
        return false;
    auto* finally_container = as<BlockContainer>(body->finally_block());
    if (finally_container == nullptr || !match_exit_block(finally_container->entry_point(), nullptr, object_store->variable())) return false;
    if (object_store->variable()->load_count() > 1) return false;
    context_->step("LockTransformV2", block);
    block->instructions.remove_at(i - 1);
    block->instructions.remove_at(i - 2);
    auto* lock_instruction = with_il_range(make<LockInstruction>(object_store->value(), body->try_block()), object_store);
    body->replace_with(lock_instruction);
    context_->end_step(lock_instruction);
    return true;
}

/// stloc flag(ldc.i4 0)
/// .try BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call Enter(stloc obj(lockObj), ldloca flag)
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// } finally BlockContainer {
///     Block (incoming: 1) {
///         if (ldloc flag) Block  {
///             call Exit(ldloc obj)
///         }
///         leave lockBlock (nop)
///     }
/// }
/// =>
/// .lock (lockObj) BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// }
bool LockTransform::transform_lock_v4(Block* block, int i) {
    if (i < 1) return false;
    auto* body = as<TryFinally>(block->instructions[i]);
    auto* flag_store = as<StLoc>(block->instructions[i - 1]);
    if (body == nullptr || flag_store == nullptr) return false;
    if (!typesystem::is_known_type(flag_store->variable()->type(), typesystem::KnownTypeCode::Boolean) || !flag_store->value()->match_ldc_i4(0))
        return false;
    auto* try_container = as<BlockContainer>(body->try_block());
    StLoc* object_store = nullptr;
    if (try_container == nullptr || !match_lock_entry_point(try_container->entry_point(), flag_store->variable(), &object_store)) return false;
    auto* finally_container = as<BlockContainer>(body->finally_block());
    if (finally_container == nullptr || !match_exit_block(finally_container->entry_point(), flag_store->variable(), object_store->variable()))
        return false;
    if (object_store->variable()->load_count() > 1) return false;
    context_->step("LockTransformV4", block);
    block->instructions.remove_at(i - 1);
    try_container->entry_point()->instructions.remove_at(0);
    auto* lock_instruction = with_il_range(make<LockInstruction>(object_store->value(), body->try_block()), object_store);
    body->replace_with(lock_instruction);
    context_->end_step(lock_instruction);
    return true;
}

/// stloc flag(ldc.i4 0)
/// .try BlockContainer {
///     Block lockBlock (incoming: 1) {
///         stloc obj1(stloc obj2(lockObj))
///         call Enter(ldloc obj2, ldloca flag)
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// } finally BlockContainer {
///     Block (incoming: 1) {
///         if (ldloc flag) Block  {
///             call Exit(ldloc obj1)
///         }
///         leave lockBlock (nop)
///     }
/// }
/// =>
/// .lock (lockObj) BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// }
bool LockTransform::transform_lock_v4_yield_return(Block* block, int i) {
    if (i < 1) return false;
    auto* body = as<TryFinally>(block->instructions[i]);
    auto* flag_store = as<StLoc>(block->instructions[i - 1]);
    if (body == nullptr || flag_store == nullptr) return false;
    if (!typesystem::is_known_type(flag_store->variable()->type(), typesystem::KnownTypeCode::Boolean) || !flag_store->value()->match_ldc_i4(0))
        return false;
    auto* try_container = as<BlockContainer>(body->try_block());
    ILVariable* exit_variable = nullptr;
    StLoc* object_store = nullptr;
    if (try_container == nullptr ||
        !match_lock_entry_point(try_container->entry_point(), flag_store->variable(), &exit_variable, &object_store))
        return false;
    auto* finally_container = as<BlockContainer>(body->finally_block());
    if (finally_container == nullptr || !match_exit_block(finally_container->entry_point(), flag_store->variable(), exit_variable)) return false;
    if (object_store->variable()->load_count() > 1) return false;
    context_->step("LockTransformV4YieldReturn", block);
    block->instructions.remove_at(i - 1);
    try_container->entry_point()->instructions.remove_range(0, 2);
    auto* lock_instruction = with_il_range(make<LockInstruction>(object_store->value(), body->try_block()), object_store);
    body->replace_with(lock_instruction);
    context_->end_step(lock_instruction);
    return true;
}

/// stloc obj(lockObj)
/// stloc flag(ldc.i4 0)
/// .try BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call Enter(ldloc obj, ldloca flag)
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// } finally BlockContainer {
///     Block (incoming: 1) {
///         if (ldloc flag) Block  {
///             call Exit(ldloc obj)
///         }
///         leave lockBlock (nop)
///     }
/// }
/// =>
/// .lock (lockObj) BlockContainer {
///     Block lockBlock (incoming: 1) {
///         call WriteLine()
///         leave lockBlock (nop)
///     }
/// }
bool LockTransform::transform_lock_roslyn(Block* block, int i) {
    if (i < 2) return false;
    auto* body = as<TryFinally>(block->instructions[i]);
    auto* flag_store = as<StLoc>(block->instructions[i - 1]);
    auto* object_store = as<StLoc>(block->instructions[i - 2]);
    if (body == nullptr || flag_store == nullptr || object_store == nullptr) return false;
    if (!object_store->variable()->is_single_definition() ||
        !typesystem::is_known_type(flag_store->variable()->type(), typesystem::KnownTypeCode::Boolean) || !flag_store->value()->match_ldc_i4(0))
        return false;
    auto* try_container = as<BlockContainer>(body->try_block());
    if (try_container == nullptr || !match_lock_entry_point(try_container->entry_point(), flag_store->variable(), object_store->variable()))
        return false;
    auto* finally_container = as<BlockContainer>(body->finally_block());
    if (finally_container == nullptr || !match_exit_block(finally_container->entry_point(), flag_store->variable(), object_store->variable()))
        return false;
    if (object_store->variable()->load_count() > 2) return false;
    context_->step("LockTransformRoslyn", block);
    block->instructions.remove_at(i - 1);
    block->instructions.remove_at(i - 2);
    try_container->entry_point()->instructions.remove_at(0);
    auto* lock_instruction = with_il_range(make<LockInstruction>(object_store->value(), body->try_block()), object_store);
    body->replace_with(lock_instruction);
    context_->end_step(lock_instruction);
    return true;
}

bool LockTransform::match_exit_block(Block* entry_point, ILVariable* flag, ILVariable* obj) {
    if (entry_point->instructions.count() != 2 || entry_point->incoming_edge_count() != 1) return false;
    if (flag != nullptr) {
        ILInstruction* cond = nullptr;
        ILInstruction* true_inst = nullptr;
        if (!entry_point->instructions[0]->match_if_instruction(&cond, &true_inst) || !is<Block>(true_inst)) return false;
        auto* true_block = as<Block>(true_inst);
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        if (!(cond->match_ld_loc(flag) || (cond->match_comp_not_equals(&left, &right) && left->match_ld_loc(flag) && right->match_ldc_i4(0))) ||
            !match_exit_block(true_block, obj))
            return false;
    } else {
        if (!match_call(as<Call>(entry_point->instructions[0]), "Exit", {obj})) return false;
    }
    ILInstruction* ret_val = nullptr;
    if (!entry_point->instructions[1]->match_leave(as<BlockContainer>(entry_point->parent()), &ret_val) || !ret_val->match_nop()) return false;
    return true;
}

bool LockTransform::match_exit_block(Block* exit_block, ILVariable* obj) {
    if (exit_block->instructions.count() != 1) return false;
    if (!match_call(as<Call>(exit_block->instructions[0]), "Exit", {obj})) return false;
    return true;
}

bool LockTransform::match_lock_entry_point(Block* entry_point, ILVariable* flag, ILVariable* obj) {
    if (entry_point->instructions.count() == 0 || entry_point->incoming_edge_count() != 1) return false;
    if (!match_call(as<Call>(entry_point->instructions[0]), "Enter", {obj, flag})) return false;
    return true;
}

bool LockTransform::match_lock_entry_point(Block* entry_point, ILVariable* flag, StLoc** obj) {
    *obj = nullptr;
    if (entry_point->instructions.count() == 0 || entry_point->incoming_edge_count() != 1) return false;
    if (!match_call(as<Call>(entry_point->instructions[0]), "Enter", flag, obj)) return false;
    return true;
}

bool LockTransform::match_lock_entry_point(Block* entry_point, ILVariable* flag, ILVariable** exit_variable, StLoc** obj) {
    // This pattern is commonly seen in yield return state machines emitted by the legacy C# compiler.
    *obj = nullptr;
    *exit_variable = nullptr;
    if (entry_point->instructions.count() < 1 || entry_point->incoming_edge_count() != 1) return false;
    ILInstruction* value = nullptr;
    if (entry_point->instructions[0]->match_st_loc(exit_variable, &value)) {
        auto* nested_stloc = as<StLoc>(value);
        if (nested_stloc == nullptr) return false;
        *obj = nested_stloc;
        if (!match_call(as<Call>(entry_point->instructions.element_at_or_default(1)), "Enter", {nested_stloc->variable(), flag})) return false;
        return true;
    }
    return false;
}

bool LockTransform::match_call(Call* call, const std::string& method_name, ILVariable* flag, StLoc** obj) {
    *obj = nullptr;
    const std::string threading_monitor = "System.Threading.Monitor";
    if (call == nullptr || call->method->name() != method_name || call->method->declaring_type()->full_name() != threading_monitor ||
        !call->method->type_arguments().empty() || call->arguments().count() != 2)
        return false;
    auto* val = as<StLoc>(call->arguments()[0]);
    if (!call->arguments()[1]->match_ld_loca(flag) || val == nullptr) return false;
    *obj = val;
    return true;
}

bool LockTransform::match_call(Call* call, const std::string& method_name, const std::vector<ILVariable*>& variables) {
    const std::string threading_monitor = "System.Threading.Monitor";
    if (call == nullptr || call->method->name() != method_name || call->method->declaring_type()->full_name() != threading_monitor ||
        !call->method->type_arguments().empty() || call->arguments().count() != static_cast<int>(variables.size()))
        return false;
    if (!call->arguments()[0]->match_ld_loc(variables[0])) return false;
    if (variables.size() == 2) {
        if (!call->arguments()[1]->match_ld_loca(variables[1])) return false;
    }
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms

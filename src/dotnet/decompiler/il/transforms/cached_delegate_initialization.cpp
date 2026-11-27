// dotnet/decompiler/il/transforms/cached_delegate_initialization.cpp -- see cached_delegate_initialization.hpp.
#include "dotnet/decompiler/il/transforms/cached_delegate_initialization.hpp"

#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/delegate_construction.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace {

// parent.Children.ElementAtOrDefault(index)
ILInstruction* child_at_or_default(ILInstruction* parent, int index) {
    if (index < 0 || index >= parent->get_child_count()) return nullptr;
    return parent->get_child(index);
}

}  // namespace

void CachedDelegateInitialization::run(Block* block, BlockTransformContext& context) {
    context_ = &context;
    if (!context.settings().anonymous_methods) return;
    for (int i = context.index_of_first_already_transformed_instruction - 1; i >= 0; i--) {
        if (auto* inst = as<IfInstruction>(block->instructions[i])) {
            if (cached_delegate_initialization_with_field(inst)) {
                block->instructions.remove_at(i);
                context.index_of_first_already_transformed_instruction = block->instructions.count();
                continue;
            }
            if (cached_delegate_initialization_with_local(inst)) {
                ILInlining::inline_one_if_possible(block, i, InliningOptions::Aggressive, context);
                context.index_of_first_already_transformed_instruction = block->instructions.count();
                continue;
            }
            if (cached_delegate_initialization_roslyn_in_static_with_local(inst) || cached_delegate_initialization_roslyn_with_local(inst)) {
                block->instructions.remove_at(i);
                context.index_of_first_already_transformed_instruction = block->instructions.count();
                continue;
            }
            if (cached_delegate_initialization_vb(inst)) {
                context.index_of_first_already_transformed_instruction = block->instructions.count();
                continue;
            }
            if (cached_delegate_initialization_vb_with_return(inst)) {
                block->instructions.remove_at(i);
                context.index_of_first_already_transformed_instruction = block->instructions.count();
                continue;
            }
            if (cached_delegate_initialization_vb_with_closure(inst)) {
                context.index_of_first_already_transformed_instruction = block->instructions.count();
                continue;
            }
        }
    }
}

/// if (comp(ldsfld CachedAnonMethodDelegate == ldnull)) {
///     stsfld CachedAnonMethodDelegate(DelegateConstruction)
/// }
/// ... one usage of CachedAnonMethodDelegate ...
/// =>
/// ... one usage of DelegateConstruction ...
bool CachedDelegateInitialization::cached_delegate_initialization_with_field(IfInstruction* inst) {
    auto* true_inst = as<Block>(inst->true_inst());
    if (true_inst == nullptr || true_inst->instructions.count() != 1 || !inst->false_inst()->match_nop()) return false;
    ILInstruction* store_inst = true_inst->instructions[0];
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    const IField* field = nullptr;
    if (!inst->condition()->match_comp_equals(&left, &right) || !left->match_lds_fld(&field) || !right->match_ld_null()) return false;
    const IField* field2 = nullptr;
    ILInstruction* value = nullptr;
    if (!store_inst->match_sts_fld(&field2, &value) || !typesystem::member_equals(field, field2) ||
        !is_compiler_generated_or_is_in_compiler_generated_class(field))
        return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(as<NewObj>(value->unwrap_conv(ConversionKind::Invalid)), &target_method, &target,
                                                           &delegate_type, true))
        return false;
    ILInstruction* next_instruction = child_at_or_default(inst->parent(), inst->child_index() + 1);
    if (next_instruction == nullptr) return false;
    std::vector<ILInstruction*> usages;
    for (ILInstruction* d : next_instruction->descendants())
        if (d->match_lds_fld(field)) usages.push_back(d);
    if (usages.size() != 1) return false;
    context_->step("CachedDelegateInitializationWithField", inst);
    usages[0]->replace_with(value);
    context_->end_step(value);
    return true;
}

/// if (comp(ldloc v == ldnull)) {
///     stloc v(DelegateConstruction)
/// }
/// =>
/// stloc v(DelegateConstruction)
bool CachedDelegateInitialization::cached_delegate_initialization_with_local(IfInstruction* inst) {
    auto* true_inst = as<Block>(inst->true_inst());
    if (true_inst == nullptr || true_inst->instructions.count() != 1 || !inst->false_inst()->match_nop()) return false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    ILVariable* v = nullptr;
    if (!inst->condition()->match_comp_equals(&left, &right) || !left->match_ld_loc(&v) || !right->match_ld_null()) return false;
    ILInstruction* store_inst = true_inst->instructions.last();
    ILInstruction* value = nullptr;
    if (!store_inst->match_st_loc(v, &value)) return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(as<NewObj>(value), &target_method, &target, &delegate_type, true)) return false;
    // do not transform if there are other stores/loads of this variable
    if (v->store_count() != 2 || v->store_instructions().size() != 2 || v->load_count() != 2 || v->address_count() != 0) return false;
    // do not transform if the first assignment is not assigning null:
    StLoc* other_store = nullptr;
    for (IStoreInstruction* s : v->store_instructions()) {
        auto* store = as<StLoc>(s->as_instruction());
        if (store != nullptr && store != store_inst) {
            // SingleOrDefault
            if (other_store != nullptr) throw UsageError("Sequence contains more than one matching element");
            other_store = store;
        }
    }
    if (other_store == nullptr || !other_store->value()->match_ld_null() || !is<Block>(other_store->parent())) return false;
    // do not transform if there is no usage directly afterwards
    ILInstruction* next_instruction = child_at_or_default(inst->parent(), inst->child_index() + 1);
    if (next_instruction == nullptr) return false;
    std::vector<ILInstruction*> usages;
    for (ILInstruction* d : next_instruction->descendants())
        if (d->match_ld_loc(v)) usages.push_back(d);
    if (usages.size() != 1) return false;
    context_->step("CachedDelegateInitializationWithLocal", inst);
    as<Block>(other_store->parent())->instructions.remove(other_store);
    inst->replace_with(store_inst);
    context_->end_step(store_inst);
    return true;
}

/// stloc s(ldobj(ldsflda(CachedAnonMethodDelegate))
/// if (comp(ldloc s == null)) {
///     stloc s(stobj(ldsflda(CachedAnonMethodDelegate), DelegateConstruction))
/// }
/// =>
/// stloc s(DelegateConstruction)
bool CachedDelegateInitialization::cached_delegate_initialization_roslyn_in_static_with_local(IfInstruction* inst) {
    auto* true_inst = as<Block>(inst->true_inst());
    if (true_inst == nullptr || true_inst->instructions.count() != 1 || !inst->false_inst()->match_nop()) return false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    ILVariable* s = nullptr;
    if (!inst->condition()->match_comp_equals(&left, &right) || !left->match_ld_loc(&s) || !right->match_ld_null()) return false;
    auto* store_inst = as<StLoc>(true_inst->instructions.last());
    auto* store_before_if = as<StLoc>(child_at_or_default(inst->parent(), inst->child_index() - 1));
    if (store_before_if == nullptr || store_inst == nullptr || store_before_if->variable() != s || store_inst->variable() != s) return false;
    auto* stobj = as<StObj>(store_inst->value());
    auto* ldobj = as<LdObj>(store_before_if->value());
    if (stobj == nullptr || ldobj == nullptr) return false;
    if (!is<NewObj>(stobj->value())) return false;
    const IField* field1 = nullptr;
    const IField* field2 = nullptr;
    if (!stobj->target()->match_lds_flda(&field1) || !ldobj->target()->match_lds_flda(&field2) || !typesystem::member_equals(field1, field2))
        return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(as<NewObj>(stobj->value()), &target_method, &target, &delegate_type, true)) return false;
    context_->step("CachedDelegateInitializationRoslynInStaticWithLocal", inst);
    store_before_if->set_value(stobj->value());
    return true;
}

/// stloc s(ldobj(ldflda(CachedAnonMethodDelegate))
/// if (comp(ldloc s == null)) {
///     stloc s(stobj(ldflda(CachedAnonMethodDelegate), DelegateConstruction))
/// }
/// =>
/// stloc s(DelegateConstruction)
bool CachedDelegateInitialization::cached_delegate_initialization_roslyn_with_local(IfInstruction* inst) {
    auto* true_inst = as<Block>(inst->true_inst());
    if (true_inst == nullptr || true_inst->instructions.count() != 1 || !inst->false_inst()->match_nop()) return false;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    ILVariable* s = nullptr;
    if (!inst->condition()->match_comp_equals(&left, &right) || !left->match_ld_loc(&s) || !right->match_ld_null()) return false;
    auto* store_inst = as<StLoc>(true_inst->instructions.last());
    auto* store_before_if = as<StLoc>(child_at_or_default(inst->parent(), inst->child_index() - 1));
    if (store_before_if == nullptr || store_inst == nullptr || store_before_if->variable() != s || store_inst->variable() != s) return false;
    auto* stobj = as<StObj>(store_inst->value());
    auto* ldobj = as<LdObj>(store_before_if->value());
    if (stobj == nullptr || ldobj == nullptr) return false;
    if (!is<NewObj>(stobj->value())) return false;
    ILInstruction* target_unused1 = nullptr;
    ILInstruction* target_unused2 = nullptr;
    const IField* field1 = nullptr;
    const IField* field2 = nullptr;
    if (!stobj->target()->match_ld_flda(&target_unused1, &field1) || !ldobj->target()->match_ld_flda(&target_unused2, &field2) ||
        !typesystem::member_equals(field1, field2))
        return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(as<NewObj>(stobj->value()), &target_method, &target, &delegate_type, true)) return false;
    context_->step("CachedDelegateInitializationRoslynWithLocal", inst);
    store_before_if->set_value(stobj->value());
    return true;
}

/// if (comp.i4(comp.o(ldobj delegateType(ldsflda CachedAnonMethodDelegate) != ldnull) == ldc.i4 0)) Block {
///     stloc s(stobj(ldflda(CachedAnonMethodDelegate), DelegateConstruction))
/// } else Block {
///     stloc s(ldobj System.Action(ldsflda $I4-1))
/// }
/// =>
/// stloc s(DelegateConstruction)
bool CachedDelegateInitialization::cached_delegate_initialization_vb(IfInstruction* inst) {
    auto* true_inst = as<Block>(inst->true_inst());
    auto* false_inst = as<Block>(inst->false_inst());
    if (true_inst == nullptr || false_inst == nullptr) return false;
    if (true_inst->instructions.count() != 1 || false_inst->instructions.count() != 1) return false;
    ILVariable* s = nullptr;
    ILInstruction* true_init_value = nullptr;
    ILInstruction* false_init_value = nullptr;
    if (!(true_inst->instructions[0]->match_st_loc(&s, &true_init_value) && false_inst->instructions[0]->match_st_loc(s, &false_init_value))) {
        return false;
    }
    if (s->kind() != VariableKind::StackSlot || s->store_count() != 2) return false;
    auto* stobj = as<StObj>(true_init_value);
    auto* ldobj = as<LdObj>(false_init_value);
    if (stobj == nullptr || ldobj == nullptr) return false;
    auto* delegate_construction = as<NewObj>(stobj->value());
    if (delegate_construction == nullptr) return false;
    const IField* field1 = nullptr;
    const IField* field2 = nullptr;
    if (!stobj->target()->match_lds_flda(&field1) || !ldobj->target()->match_lds_flda(&field2) || !typesystem::member_equals(field1, field2)) {
        return false;
    }
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!inst->condition()->match_comp_equals(&left, &right) || !right->match_ld_null()) return false;
    if (!ldobj->match(left).success()) return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(delegate_construction, &target_method, &target, &delegate_type, true)) return false;
    context_->step("CachedDelegateInitializationVB", inst);
    auto* stloc = make<StLoc>(s, delegate_construction);
    inst->replace_with(stloc);
    context_->end_step(stloc);
    return true;
}

/// if (comp.o(ldsfld CachedAnonMethodDelegate != ldnull)) {
///     leave IL_0005 (ldsfld CachedAnonMethodDelegate)
/// }
/// leave IL_0005 (stsfld CachedAnonMethodDelegate(DelegateConstruction))
/// =>
/// leave IL_0005 (DelegateConstruction)
bool CachedDelegateInitialization::cached_delegate_initialization_vb_with_return(IfInstruction* inst) {
    ILInstruction* arg = nullptr;
    const IField* field = nullptr;
    if (!inst->condition()->match_comp_not_equals_null(&arg) || !arg->match_lds_fld(&field)) return false;
    if (!inst->false_inst()->match_nop()) return false;
    if (!inst->true_inst()->match_return(&arg) || !arg->match_lds_fld(field)) return false;
    auto* leave_after_if = as<Leave>(child_at_or_default(inst->parent(), inst->child_index() + 1));
    if (leave_after_if == nullptr || !leave_after_if->is_leaving_function()) return false;
    const IField* field2 = nullptr;
    ILInstruction* delegate_construction = nullptr;
    if (!leave_after_if->value()->match_sts_fld(&field2, &delegate_construction) || !typesystem::member_equals(field, field2)) return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(delegate_construction, &target_method, &target, &delegate_type, true)) return false;
    context_->step("CachedDelegateInitializationVBWithReturn", inst);
    leave_after_if->set_value(delegate_construction);
    return true;
}

/// if (comp.o(ldobj delegateType(ldflda CachedAnonMethodDelegate(ldloc closure)) != ldnull)) Block {
///     stloc s(ldobj delegateType(ldflda CachedAnonMethodDelegate(ldloc closure)))
/// } else Block {
///     stloc s(stobj delegateType(ldflda CachedAnonMethodDelegate(ldloc closure), DelegateConstruction))
/// }
/// =>
/// stloc s(DelegateConstruction)
bool CachedDelegateInitialization::cached_delegate_initialization_vb_with_closure(IfInstruction* inst) {
    auto* true_inst = as<Block>(inst->true_inst());
    auto* false_inst = as<Block>(inst->false_inst());
    if (true_inst == nullptr || false_inst == nullptr) return false;
    if (true_inst->instructions.count() != 1 || false_inst->instructions.count() != 1) return false;
    ILVariable* s = nullptr;
    ILInstruction* true_init_value = nullptr;
    ILInstruction* false_init_value = nullptr;
    if (!(true_inst->instructions[0]->match_st_loc(&s, &true_init_value) && false_inst->instructions[0]->match_st_loc(s, &false_init_value))) {
        return false;
    }
    if (s->kind() != VariableKind::StackSlot || s->store_count() != 2) return false;
    auto* stobj = as<StObj>(false_init_value);
    auto* ldobj = as<LdObj>(true_init_value);
    if (stobj == nullptr || ldobj == nullptr) return false;
    auto* delegate_construction = as<NewObj>(stobj->value());
    if (delegate_construction == nullptr) return false;
    ILInstruction* target1 = nullptr;
    ILInstruction* target2 = nullptr;
    const IField* field1 = nullptr;
    const IField* field2 = nullptr;
    if (!stobj->target()->match_ld_flda(&target1, &field1) || !ldobj->target()->match_ld_flda(&target2, &field2) ||
        !typesystem::member_equals(field1, field2) || !target1->match(target2).success()) {
        return false;
    }
    ILInstruction* left = nullptr;
    if (!inst->condition()->match_comp_not_equals_null(&left)) return false;
    if (!ldobj->match(left).success()) return false;
    const IMethod* target_method = nullptr;
    ILInstruction* target = nullptr;
    const IType* delegate_type = nullptr;
    if (!DelegateConstruction::match_delegate_construction(delegate_construction, &target_method, &target, &delegate_type, true)) return false;
    context_->step("CachedDelegateInitializationVBWithClosure", inst);
    auto* stloc = make<StLoc>(s, delegate_construction);
    inst->replace_with(stloc);
    context_->end_step(stloc);
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms

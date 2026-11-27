// dotnet/decompiler/il/transforms/dynamic_call_site_transform.cxx -- see dynamic_call_site_transform.hpp.
#include "dotnet/decompiler/il/transforms/dynamic_call_site_transform.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <typeinfo>
#include <unordered_set>
#include <utility>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/transform_array_initializers.hpp"
#include "dotnet/decompiler/il/transforms/transform_expression_trees.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

constexpr const char* kCallSiteTypeName = "System.Runtime.CompilerServices.CallSite";
constexpr const char* kCSharpBinderTypeName = "Microsoft.CSharp.RuntimeBinder.Binder";

// C#'s `(T)inst` for a node that must be a T.
template <class T>
T* checked_cast(ILInstruction* inst) {
    T* result = as<T>(inst);
    if (result == nullptr) throw std::bad_cast();
    return result;
}

// arguments[from..]
std::vector<ILInstruction*> skip_arguments(CallVirt* call, int count) {
    std::vector<ILInstruction*> result;
    for (int i = count; i < call->arguments().count(); i++) result.push_back(call->arguments()[i]);
    return result;
}

}  // namespace

std::size_t DynamicCallSiteTransform::FieldHash::operator()(const IField* field) const { return static_cast<std::size_t>(field->hash_code()); }
bool DynamicCallSiteTransform::FieldEqual::operator()(const IField* a, const IField* b) const { return typesystem::member_equals(a, b); }

void DynamicCallSiteTransform::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().dynamic) return;

    CallSiteMap callsites;

    for (ILInstruction* descendant : function->descendants()) {
        auto* block = as<Block>(descendant);
        if (block == nullptr) continue;
        find_dynamic_call_sites_in_block(block, callsites, context);
    }

    auto modified_containers = transform_call_sites(checked_cast<BlockContainer>(function->body()), callsites, context);

    for (BlockContainer* container : modified_containers) container->sort_blocks(true);
}

void DynamicCallSiteTransform::run_on_basic_block(Block* block, ILTransformContext& context) {
    if (!context.settings().dynamic) return;

    CallSiteMap callsites;
    find_dynamic_call_sites_in_block(block, callsites, context);
    // Deleting the now-unreachable callsite-init blocks is deferred to the SortBlocks call
    // at the end of AsyncAwaitDecompiler.AnalyzeStateMachine: this runs while that method is
    // iterating over the container's blocks, so removing blocks here would corrupt the loop.
    transform_call_sites(checked_cast<BlockContainer>(block->parent()), callsites, context);
}

std::vector<BlockContainer*> DynamicCallSiteTransform::transform_call_sites(BlockContainer* parent, const CallSiteMap& callsites,
                                                                            ILTransformContext& context) {
    std::vector<StLoc*> stores_to_remove;
    // a HashSet upstream, only walked for the sort: insertion order
    std::vector<BlockContainer*> modified_containers;
    std::unordered_set<BlockContainer*> modified_set;

    for (ILInstruction* descendant : parent->descendants()) {
        auto* invoke_call = as<CallVirt>(descendant);
        if (invoke_call == nullptr) continue;
        if (invoke_call->method->declaring_type()->kind() != TypeKind::Delegate || invoke_call->method->name() != "Invoke" ||
            invoke_call->arguments().count() == 0)
            continue;
        ILInstruction* first_argument = invoke_call->arguments()[0];
        ILVariable* stack_slot = nullptr;
        if (first_argument->match_ld_loc(&stack_slot) && is_single_definition_temporary(stack_slot)) {
            first_argument = checked_cast<StLoc>(stack_slot->store_instructions()[0]->as_instruction())->value();
        }
        ILInstruction* cache_field_load = nullptr;
        const IField* target_field = nullptr;
        if (!first_argument->match_ld_fld(&cache_field_load, &target_field)) continue;
        const IField* cache_field = nullptr;
        if (!cache_field_load->match_lds_fld(&cache_field)) continue;
        auto found = callsites.find(cache_field);
        if (found == callsites.end()) continue;
        const CallSiteInfo& callsite = found->second;
        context.step("Transform callsite for " + callsite.member_name, nullptr);
        std::vector<ILInstruction*> dead_arguments;
        ILInstruction* replacement = make_dynamic_instruction(callsite, invoke_call, dead_arguments);
        if (replacement == nullptr) continue;
        invoke_call->replace_with(replacement);
        assert(callsite.conditional_jump_to_init != nullptr && is<Block>(callsite.conditional_jump_to_init->parent()));
        auto* block = checked_cast<Block>(callsite.conditional_jump_to_init->parent());
        if (callsite.inverted) {
            block->instructions.remove(callsite.conditional_jump_to_init);
            callsite.branch_after_init->replace_with(callsite.conditional_jump_to_init->true_inst());
        } else {
            block->instructions.remove(callsite.conditional_jump_to_init);
        }
        for (ILInstruction* arg : dead_arguments) {
            ILVariable* temporary = nullptr;
            if (arg->match_ld_loc(&temporary) && is_single_definition_temporary(temporary) && temporary->load_count() == 0) {
                StLoc* st_loc = checked_cast<StLoc>(temporary->store_instructions()[0]->as_instruction());
                if (is<Block>(st_loc->parent())) {
                    ILInstruction* value = st_loc->value();
                    const IField* cache_field_copy = nullptr;
                    if (value->match_lds_fld(&cache_field_copy) && typesystem::member_equals(cache_field_copy, cache_field))
                        stores_to_remove.push_back(st_loc);
                    const IField* target_field_copy = nullptr;
                    if (value->match_ld_fld(&cache_field_load, &target_field_copy) && cache_field_load->match_lds_fld(&cache_field_copy) &&
                        typesystem::member_equals(cache_field, cache_field_copy) && typesystem::member_equals(target_field, target_field_copy))
                        stores_to_remove.push_back(st_loc);
                }
            }
        }
        auto* container = checked_cast<BlockContainer>(block->parent());
        if (modified_set.insert(container).second) modified_containers.push_back(container);
    }

    for (StLoc* inst : stores_to_remove) {
        auto* parent_block = checked_cast<Block>(inst->parent());
        parent_block->instructions.remove_at(inst->child_index());
    }

    return modified_containers;
}

/// A compiler-generated temporary the callsite invoke may load its target/cache through: a single-def
/// stack slot, or a single-def local spilled from a state-machine field. The latter arises when async
/// state-machine hoisting spills the callsite target/receiver across an await (the await appears in the
/// invoke's argument list). Ordinary user locals are excluded via the state-machine-field requirement.
bool DynamicCallSiteTransform::is_single_definition_temporary(ILVariable* variable) {
    if (!variable->is_single_definition()) return false;
    return variable->kind() == VariableKind::StackSlot || (variable->kind() == VariableKind::Local && variable->state_machine_field != nullptr);
}

void DynamicCallSiteTransform::find_dynamic_call_sites_in_block(Block* block, CallSiteMap& callsites, ILTransformContext& context) {
    if (block->instructions.count() < 2) return;
    // Check if, we deal with a callsite cache field null check:
    // if (comp(ldsfld <>p__3 == ldnull)) br IL_000c
    // br IL_002b
    auto* if_inst = as<IfInstruction>(block->instructions.second_to_last_or_default());
    if (if_inst == nullptr) return;
    auto* branch_after_init = as<Branch>(block->instructions.last_or_default());
    if (branch_after_init == nullptr) return;
    const IField* call_site_cache_field = nullptr;
    const IType* call_site_delegate = nullptr;
    bool invert_branches = false;
    if (!match_call_site_cache_null_check(if_inst->condition(), &call_site_cache_field, &call_site_delegate, &invert_branches)) return;
    Block* true_block = nullptr;
    if (!if_inst->true_inst()->match_branch(&true_block)) return;
    Block* call_site_init_block = nullptr;
    Block* target_block_after_init = nullptr;
    if (invert_branches) {
        call_site_init_block = branch_after_init->target_block();
        target_block_after_init = true_block;
    } else {
        call_site_init_block = true_block;
        target_block_after_init = branch_after_init->target_block();
    }
    CallSiteInfo call_site_info;
    Block* block_after_init = nullptr;
    if (!scan_call_site_init_block(call_site_init_block, call_site_cache_field, call_site_delegate, &call_site_info, &block_after_init, context)) return;
    if (target_block_after_init != block_after_init) return;
    call_site_info.delegate_type = call_site_delegate;
    call_site_info.conditional_jump_to_init = if_inst;
    call_site_info.inverted = invert_branches;
    call_site_info.branch_after_init = branch_after_init;
    // Dictionary.Add: an ArgumentException for a field that is in already
    if (!callsites.emplace(call_site_cache_field, std::move(call_site_info)).second)
        throw std::invalid_argument("An item with the same key has already been added.");
}

ILInstruction* DynamicCallSiteTransform::make_dynamic_instruction(const CallSiteInfo& callsite, CallVirt* target_invoke_call,
                                                                  std::vector<ILInstruction*>& dead_arguments) {
    auto take_two = [&] {
        for (int i = 0; i < 2 && i < target_invoke_call->arguments().count(); i++) dead_arguments.push_back(target_invoke_call->arguments()[i]);
    };
    switch (callsite.kind) {
    case BinderMethodKind::BinaryOperation: {
        take_two();
        ILInstruction* left = target_invoke_call->arguments()[2];
        ILInstruction* right = target_invoke_call->arguments()[3];
        return make<DynamicBinaryOperatorInstruction>(callsite.flags, callsite.operation, callsite.context, callsite.argument_infos.at(0), left,
                                                      callsite.argument_infos.at(1), right);
    }
    case BinderMethodKind::Convert: {
        take_two();
        ILInstruction* result = make<DynamicConvertInstruction>(callsite.flags, callsite.convert_target_type, callsite.context,
                                                                target_invoke_call->arguments()[2]);
        if (result->result_type() == StackType::Unknown) {
            // if references are missing, we need to coerce the primitive type to None.
            // Otherwise we will get loads of assertions.
            bool is_checked = static_cast<DynamicConvertInstruction*>(result)->is_checked();
            result = make<Conv>(result, PrimitiveType::None, is_checked, typesystem::Sign::None);
        }
        return result;
    }
    case BinderMethodKind::GetIndex:
        take_two();
        return make<DynamicGetIndexInstruction>(callsite.flags, callsite.context, callsite.argument_infos, skip_arguments(target_invoke_call, 2));
    case BinderMethodKind::GetMember:
        take_two();
        return make<DynamicGetMemberInstruction>(callsite.flags, callsite.member_name, callsite.context, callsite.argument_infos.at(0),
                                                 target_invoke_call->arguments()[2]);
    case BinderMethodKind::Invoke:
        take_two();
        return make<DynamicInvokeInstruction>(callsite.flags, callsite.context, callsite.argument_infos, skip_arguments(target_invoke_call, 2));
    case BinderMethodKind::InvokeConstructor: {
        auto arguments = skip_arguments(target_invoke_call, 2);
        // Extract type information from targetInvokeCall:
        // Must either be an inlined type or
        // a reference to a variable that is initialized with a type.
        const IType* type = nullptr;
        if (!TransformExpressionTrees::match_get_type_from_handle(arguments.at(0), &type)) {
            ILVariable* temp = nullptr;
            StLoc* init_store = nullptr;
            // temp.StoreInstructions.FirstOrDefault() is StLoc initStore
            if (arguments[0]->match_ld_loc(&temp) && temp->is_single_definition() && !temp->store_instructions().empty())
                init_store = as<StLoc>(temp->store_instructions()[0]->as_instruction());
            if (init_store == nullptr) return nullptr;
            if (!TransformExpressionTrees::match_get_type_from_handle(init_store->value(), &type)) return nullptr;
        }
        take_two();
        return make<DynamicInvokeConstructorInstruction>(callsite.flags, type != nullptr ? type : &typesystem::SpecialType::unknown_type(),
                                                         callsite.context, callsite.argument_infos, arguments);
    }
    case BinderMethodKind::InvokeMember:
        take_two();
        return make<DynamicInvokeMemberInstruction>(callsite.flags, callsite.member_name, callsite.type_arguments, callsite.context,
                                                    callsite.argument_infos, skip_arguments(target_invoke_call, 2));
    case BinderMethodKind::IsEvent:
        take_two();
        return make<DynamicIsEventInstruction>(callsite.flags, callsite.member_name, callsite.context, target_invoke_call->arguments()[2]);
    case BinderMethodKind::SetIndex:
        take_two();
        return make<DynamicSetIndexInstruction>(callsite.flags, callsite.context, callsite.argument_infos, skip_arguments(target_invoke_call, 2));
    case BinderMethodKind::SetMember: {
        take_two();
        ILInstruction* target = target_invoke_call->arguments()[2];
        ILInstruction* value = target_invoke_call->arguments()[3];
        return make<DynamicSetMemberInstruction>(callsite.flags, callsite.member_name, callsite.context, callsite.argument_infos.at(0), target,
                                                 callsite.argument_infos.at(1), value);
    }
    case BinderMethodKind::UnaryOperation:
        take_two();
        return make<DynamicUnaryOperatorInstruction>(callsite.flags, callsite.operation, callsite.context, callsite.argument_infos.at(0),
                                                     target_invoke_call->arguments()[2]);
    default:
        throw std::out_of_range("Value " + std::to_string(static_cast<int>(callsite.kind)) + " is not supported!");
    }
}

bool DynamicCallSiteTransform::scan_call_site_init_block(Block* call_site_init_block, const IField* call_site_cache_field,
                                                         const IType* call_site_delegate_type, CallSiteInfo* call_site_info, Block** block_after_init,
                                                         ILTransformContext& context) {
    *call_site_info = CallSiteInfo();
    *block_after_init = nullptr;
    int inst_count = call_site_init_block->instructions.count();
    if (call_site_init_block->incoming_edge_count() != 1 || inst_count < 2) return false;
    if (!call_site_init_block->instructions[inst_count - 1]->match_branch(block_after_init)) return false;
    const IField* field = nullptr;
    ILInstruction* value = nullptr;
    if (!call_site_init_block->instructions[inst_count - 2]->match_sts_fld(&field, &value) || !typesystem::member_equals(field, call_site_cache_field))
        return false;
    auto* create_binder_call = as<Call>(value);
    if (create_binder_call == nullptr || !create_binder_call->method->type_arguments().empty() || create_binder_call->arguments().count() != 1 ||
        create_binder_call->method->name() != "Create" || create_binder_call->method->declaring_type()->full_name() != kCallSiteTypeName ||
        create_binder_call->method->declaring_type()->type_arguments().size() != 1)
        return false;
    auto* binder_call = as<Call>(create_binder_call->arguments()[0]);
    if (binder_call == nullptr || binder_call->method->declaring_type()->full_name() != kCSharpBinderTypeName ||
        binder_call->method->declaring_type()->type_parameter_count() != 0)
        return false;
    call_site_info->delegate_type = call_site_delegate_type;
    call_site_info->init_block = call_site_init_block;
    // the variables of the switch sections below share one scope upstream
    int binder_flags_integer = 0;
    std::string name;
    const IType* context_type = nullptr;
    ILVariable* variable = nullptr;
    const std::string& binder_name = binder_call->method->name();
    if (binder_name == "IsEvent") {
        call_site_info->kind = BinderMethodKind::IsEvent;
        // In the case of Binder.IsEvent all arguments should already be properly inlined, as there is no array initializer:
        // Scan arguments: binder flags, member name, context type
        if (binder_call->arguments().count() != 3) return false;
        if (!binder_call->arguments()[0]->match_ldc_i4(&binder_flags_integer)) return false;
        call_site_info->flags = static_cast<CSharpBinderFlags>(binder_flags_integer);
        if (!binder_call->arguments()[1]->match_ld_str(&name)) return false;
        call_site_info->member_name = name;
        if (!TransformExpressionTrees::match_get_type_from_handle(binder_call->arguments()[2], &context_type)) return false;
        call_site_info->context = context_type;
        return true;
    } else if (binder_name == "Convert") {
        call_site_info->kind = BinderMethodKind::Convert;
        // In the case of Binder.Convert all arguments should already be properly inlined, as there is no array initializer:
        // Scan arguments: binder flags, target type, context type
        if (binder_call->arguments().count() != 3) return false;
        if (!binder_call->arguments()[0]->match_ldc_i4(&binder_flags_integer)) return false;
        call_site_info->flags = static_cast<CSharpBinderFlags>(binder_flags_integer);
        const IType* target_type = nullptr;
        if (!TransformExpressionTrees::match_get_type_from_handle(binder_call->arguments()[1], &target_type)) return false;
        call_site_info->convert_target_type = target_type;
        if (!TransformExpressionTrees::match_get_type_from_handle(binder_call->arguments()[2], &context_type)) return false;
        call_site_info->context = context_type;
        return true;
    } else if (binder_name == "InvokeMember") {
        call_site_info->kind = BinderMethodKind::InvokeMember;
        if (binder_call->arguments().count() != 5) return false;
        // First argument: binder flags
        // The value must be a single ldc.i4 instruction.
        if (!binder_call->arguments()[0]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[0]->match_st_loc(variable, &value)) return false;
        if (!value->match_ldc_i4(&binder_flags_integer)) return false;
        call_site_info->flags = static_cast<CSharpBinderFlags>(binder_flags_integer);
        // Second argument: method name
        // The value must be a single ldstr instruction.
        if (!binder_call->arguments()[1]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[1]->match_st_loc(variable, &value)) return false;
        if (!value->match_ld_str(&name)) return false;
        call_site_info->member_name = name;
        // Third argument: type arguments
        // The value must be either ldnull (no type arguments) or an array initializer pattern.
        if (!binder_call->arguments()[2]->match_ld_loc(&variable)) return false;
        ILVariable* variable_or_temporary = nullptr;
        if (!call_site_init_block->instructions[2]->match_st_loc(&variable_or_temporary, &value)) return false;
        int number_of_type_arguments = 0;
        if (!value->match_ld_null()) {
            auto* type_args_new_arr = as<NewArr>(value);
            if (type_args_new_arr != nullptr && typesystem::is_known_type(type_args_new_arr->type(), KnownTypeCode::Type) &&
                type_args_new_arr->indices().count() == 1 && type_args_new_arr->indices()[0]->match_ldc_i4(&number_of_type_arguments)) {
                TransformArrayInitializers::ArrayInitializerValues type_arguments;
                int instructions_to_remove = 0;
                if (!TransformArrayInitializers::handle_simple_array_initializer(context.function(), call_site_init_block, 3, variable_or_temporary,
                                                                                 {number_of_type_arguments}, &type_arguments, &instructions_to_remove))
                    return false;
                int i = 0;
                call_site_info->type_arguments.assign(static_cast<std::size_t>(number_of_type_arguments), nullptr);
                for (const auto& entry : type_arguments) {
                    const IType* type = nullptr;
                    if (!TransformExpressionTrees::match_get_type_from_handle(entry.second, &type)) return false;
                    call_site_info->type_arguments.at(static_cast<std::size_t>(i)) = type;
                    i++;
                }
            } else {
                return false;
            }
        }
        int type_arguments_offset = number_of_type_arguments;
        // Special case for csc array initializers:
        if (variable_or_temporary != variable) {
            // store temporary from array initializer in variable
            if (!call_site_init_block->instructions[3 + type_arguments_offset]->match_st_loc(variable, &value)) return false;
            if (!value->match_ld_loc(variable_or_temporary)) return false;
            type_arguments_offset++;
        }
        // Fourth argument: context type
        if (!binder_call->arguments()[3]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[3 + type_arguments_offset]->match_st_loc(variable, &value)) return false;
        if (!TransformExpressionTrees::match_get_type_from_handle(value, &context_type)) return false;
        call_site_info->context = context_type;
        // Fifth argument: call parameter info
        if (!binder_call->arguments()[4]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[4 + type_arguments_offset]->match_st_loc(variable, &value)) return false;
        if (!extract_argument_info(value, call_site_info, 5 + type_arguments_offset, variable, context)) return false;
        return true;
    } else if (binder_name == "GetMember" || binder_name == "SetMember") {
        call_site_info->kind = binder_name == "GetMember" ? BinderMethodKind::GetMember : BinderMethodKind::SetMember;
        if (binder_call->arguments().count() != 4) return false;
        // First argument: binder flags
        // The value must be a single ldc.i4 instruction.
        if (!binder_call->arguments()[0]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[0]->match_st_loc(variable, &value)) return false;
        if (!value->match_ldc_i4(&binder_flags_integer)) return false;
        call_site_info->flags = static_cast<CSharpBinderFlags>(binder_flags_integer);
        // Second argument: method name
        // The value must be a single ldstr instruction.
        if (!binder_call->arguments()[1]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[1]->match_st_loc(variable, &value)) return false;
        if (!value->match_ld_str(&name)) return false;
        call_site_info->member_name = name;
        // Third argument: context type
        if (!binder_call->arguments()[2]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[2]->match_st_loc(variable, &value)) return false;
        if (!TransformExpressionTrees::match_get_type_from_handle(value, &context_type)) return false;
        call_site_info->context = context_type;
        // Fourth argument: call parameter info
        if (!binder_call->arguments()[3]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[3]->match_st_loc(variable, &value)) return false;
        if (!extract_argument_info(value, call_site_info, 4, variable, context)) return false;
        return true;
    } else if (binder_name == "GetIndex" || binder_name == "SetIndex" || binder_name == "InvokeConstructor" || binder_name == "Invoke") {
        if (binder_name == "GetIndex")
            call_site_info->kind = BinderMethodKind::GetIndex;
        else if (binder_name == "SetIndex")
            call_site_info->kind = BinderMethodKind::SetIndex;
        else if (binder_name == "InvokeConstructor")
            call_site_info->kind = BinderMethodKind::InvokeConstructor;
        else
            call_site_info->kind = BinderMethodKind::Invoke;
        if (binder_call->arguments().count() != 3) return false;
        // First argument: binder flags
        // The value must be a single ldc.i4 instruction.
        if (!binder_call->arguments()[0]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[0]->match_st_loc(variable, &value)) return false;
        if (!value->match_ldc_i4(&binder_flags_integer)) return false;
        call_site_info->flags = static_cast<CSharpBinderFlags>(binder_flags_integer);
        // Second argument: context type
        if (!binder_call->arguments()[1]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[1]->match_st_loc(variable, &value)) return false;
        if (!TransformExpressionTrees::match_get_type_from_handle(value, &context_type)) return false;
        call_site_info->context = context_type;
        // Third argument: call parameter info
        if (!binder_call->arguments()[2]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[2]->match_st_loc(variable, &value)) return false;
        if (!extract_argument_info(value, call_site_info, 3, variable, context)) return false;
        return true;
    } else if (binder_name == "UnaryOperation" || binder_name == "BinaryOperation") {
        call_site_info->kind = binder_name == "BinaryOperation" ? BinderMethodKind::BinaryOperation : BinderMethodKind::UnaryOperation;
        if (binder_call->arguments().count() != 4) return false;
        // First argument: binder flags
        // The value must be a single ldc.i4 instruction.
        if (!binder_call->arguments()[0]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[0]->match_st_loc(variable, &value)) return false;
        if (!value->match_ldc_i4(&binder_flags_integer)) return false;
        call_site_info->flags = static_cast<CSharpBinderFlags>(binder_flags_integer);
        // Second argument: operation
        // The value must be a single ldc.i4 instruction.
        if (!binder_call->arguments()[1]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[1]->match_st_loc(variable, &value)) return false;
        int operation = 0;
        if (!value->match_ldc_i4(&operation)) return false;
        call_site_info->operation = static_cast<ExpressionType>(operation);
        // Third argument: context type
        if (!binder_call->arguments()[2]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[2]->match_st_loc(variable, &value)) return false;
        if (!TransformExpressionTrees::match_get_type_from_handle(value, &context_type)) return false;
        call_site_info->context = context_type;
        // Fourth argument: call parameter info
        if (!binder_call->arguments()[3]->match_ld_loc(&variable)) return false;
        if (!call_site_init_block->instructions[3]->match_st_loc(variable, &value)) return false;
        if (!extract_argument_info(value, call_site_info, 4, variable, context)) return false;
        return true;
    }
    return false;
}

bool DynamicCallSiteTransform::extract_argument_info(ILInstruction* value, CallSiteInfo* call_site_info, int instruction_offset, ILVariable* variable,
                                                     ILTransformContext& context) {
    auto* new_arr2 = as<NewArr>(value);
    int number_of_arguments = 0;
    if (!(new_arr2 != nullptr && new_arr2->type()->full_name() == "Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo" &&
          new_arr2->indices().count() == 1 && new_arr2->indices()[0]->match_ldc_i4(&number_of_arguments)))
        return false;
    TransformArrayInitializers::ArrayInitializerValues arguments;
    int instructions_to_remove = 0;
    if (!TransformArrayInitializers::handle_simple_array_initializer(context.function(), call_site_info->init_block, instruction_offset, variable,
                                                                     {number_of_arguments}, &arguments, &instructions_to_remove))
        return false;
    int i = 0;
    call_site_info->argument_infos.assign(static_cast<std::size_t>(number_of_arguments), CSharpArgumentInfo());
    const IMethod* invoke_method = typesystem::get_delegate_invoke_method(call_site_info->delegate_type);
    if (invoke_method == nullptr) return false;
    std::vector<const IType*> compile_time_types;
    for (const typesystem::IParameter* p : invoke_method->parameters()) compile_time_types.push_back(p->type());
    for (const auto& entry : arguments) {
        auto* create_call = as<Call>(entry.second);
        if (create_call == nullptr) return false;
        if (!(create_call->method->name() == "Create" &&
              create_call->method->declaring_type()->full_name() == "Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo" &&
              create_call->arguments().count() == 2))
            return false;
        int argument_info_flags = 0;
        if (!create_call->arguments()[0]->match_ldc_i4(&argument_info_flags)) return false;
        std::string argument_name;
        if (!create_call->arguments()[1]->match_ld_str(&argument_name))
            if (!create_call->arguments()[1]->match_ld_null()) return false;
        CSharpArgumentInfo& info = call_site_info->argument_infos.at(static_cast<std::size_t>(i));
        info.flags = static_cast<CSharpArgumentInfoFlags>(argument_info_flags);
        info.name = argument_name;
        info.compile_time_type = compile_time_types.at(static_cast<std::size_t>(i + 1));
        i++;
    }
    return true;
}

bool DynamicCallSiteTransform::match_call_site_cache_null_check(ILInstruction* condition, const IField** call_site_cache_field,
                                                                const IType** call_site_delegate, bool* invert_branches) {
    *call_site_cache_field = nullptr;
    *call_site_delegate = nullptr;
    *invert_branches = false;
    ILInstruction* argument = nullptr;
    if (!condition->match_comp_equals_null(&argument)) {
        if (!condition->match_comp_not_equals_null(&argument)) return false;
        *invert_branches = true;
    }
    if (!argument->match_lds_fld(call_site_cache_field) || (*call_site_cache_field)->return_type()->type_arguments().size() != 1 ||
        (*call_site_cache_field)->return_type()->full_name() != kCallSiteTypeName)
        return false;
    *call_site_delegate = (*call_site_cache_field)->return_type()->type_arguments()[0];
    if ((*call_site_delegate)->kind() != TypeKind::Delegate) return false;
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms

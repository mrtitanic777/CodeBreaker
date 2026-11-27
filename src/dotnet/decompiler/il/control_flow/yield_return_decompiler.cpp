// dotnet/decompiler/il/control_flow/yield_return_decompiler.cpp -- see yield_return_decompiler.hpp.
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/il/control_flow/yield_return_decompiler.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <stdexcept>
#include <unordered_set>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/async_await_decompiler.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_reader.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/copy_propagation.hpp"
#include "dotnet/decompiler/il/transforms/transform_display_class_usage.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILTransformContext;
using typesystem::KnownTypeCode;

namespace {

// The field behind a (possibly specialized) field: upstream's `(IField)field.MemberDefinition`.
const IField* field_definition(const IField* field) { return dynamic_cast<const IField*>(field->member_definition()); }
// The method behind a (possibly specialized) method.
const IMethod* method_definition(const IMethod* method) { return dynamic_cast<const IMethod*>(method->member_definition()); }

// A list of type parameters that lives as long as the ILAst that refers to it.
struct TypeParameterList final : ArenaObject {
    std::vector<const typesystem::ITypeParameter*> items;
};

// Adds the variables used by the instructions below `root` to the function, each once, in the order they are first used.
void register_variables_used_in(ILFunction* function, ILInstruction* root) {
    std::vector<ILVariable*> distinct;
    std::unordered_set<ILVariable*> seen;
    for (ILInstruction* inst : root->descendants()) {
        if (auto* with_variable = as<IInstructionWithVariableOperand>(inst)) {
            ILVariable* v = with_variable->variable();
            if (seen.insert(v).second) distinct.push_back(v);
        }
    }
    for (ILVariable* v : distinct) function->variables().add(v);
}

}  // namespace

void YieldReturnDecompiler::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().yield_return) return;  // abort if enumerator decompilation is disabled
    context_ = &context;
    metadata_ = &context.pe_file().metadata();
    current_type_ = metadata_->get_method_definition(srm::MethodDefinitionHandle(context.function()->method->metadata_token())).get_declaring_type();
    enumerator_type_ = {};
    enumerator_ctor_ = {};
    is_compiled_with_mono_ = false;
    is_compiled_with_visual_basic_ = false;
    is_compiled_with_legacy_visual_basic_ = false;
    state_field_ = nullptr;
    current_field_ = nullptr;
    disposing_field_ = nullptr;
    field_to_parameter_map_.clear();
    finally_method_to_state_range_.reset();
    decompiled_finally_methods_.clear();
    return_stores_.clear();
    skip_finally_bodies_ = nullptr;
    do_finally_bodies_ = nullptr;
    cached_state_vars_.clear();
    if (!match_enumerator_creation_pattern(function)) return;
    BlockContainer* new_body = nullptr;
    try {
        analyze_ctor();
        analyze_current_property();
        resolve_ienumerable_ienumerator_field_mapping();
        construct_exception_table();
        new_body = analyze_move_next(function);
    } catch (const SymbolicAnalysisFailedException& ex) {
        function->warnings.push_back(std::string("yield-return decompiler failed: ") + ex.what());
        return;
    }

    context.step("Replacing body with MoveNext() body", function);
    function->is_iterator = true;
    function->state_machine_compiled_with_mono = is_compiled_with_mono_;
    function->state_machine_compiled_with_legacy_visual_basic = is_compiled_with_legacy_visual_basic_;
    ILInstruction* old_body = function->body();
    function->set_body(new_body);
    // register any locals used in newBody
    register_variables_used_in(function, new_body);

    // Add state machine field meta-data to parameter ILVariables.
    for (const auto& [f, p] : field_to_parameter_map_) p->state_machine_field = f;

    context.step("Delete unreachable blocks", function);

    if (is_compiled_with_mono_ || is_compiled_with_visual_basic_) {
        // mono has try-finally inline (like async on MS); we also need to sort nested blocks:
        for (Block* c : new_body->blocks) {
            for (ILInstruction* d : c->descendants()) {
                if (auto* nested_container = as<BlockContainer>(d)) nested_container->sort_blocks(true);
            }
        }
        // We need to clean up nested blocks before the main block, so that edges from unreachable code
        // in nested containers into the main container are removed before we clean up the main container.
    }
    // Note: because this only deletes blocks outright, the 'stateChanges' entries remain valid
    // (though some may point to now-deleted blocks)
    new_body->sort_blocks(true);
#ifndef NDEBUG
    function->check_invariant(ILPhase::Normal);
#endif

    try {
        if (is_compiled_with_mono_) {
            clean_skip_finally_bodies(function);
        } else if (is_compiled_with_legacy_visual_basic_) {
            clean_do_finally_bodies(function);
        } else if (is_compiled_with_visual_basic_) {
            clean_finally_state_checks(function);
        } else {
            decompile_finally_blocks();
            reconstruct_try_finally_blocks(function);
        }
    } catch (const SymbolicAnalysisFailedException& ex) {
        // Revert the yield-return transformation
        context.step("Transform failed, roll it back", function);
        function->is_iterator = false;
        function->state_machine_compiled_with_mono = false;
        function->state_machine_compiled_with_legacy_visual_basic = false;
        function->set_body(old_body);
        function->variables().remove_dead();
        function->warnings.push_back(std::string("yield-return decompiler failed: ") + ex.what());
        return;
    }

    context.step("Translate fields to local accesses", function);
    translate_fields_to_local_access(function, function, field_to_parameter_map_, is_compiled_with_mono_);

    // On mono, we still need to remove traces of the state variable(s):
    if (is_compiled_with_mono_ || is_compiled_with_visual_basic_) {
        if (ILVariable** state_var = field_to_parameter_map_.find(state_field_)) {
            for (IStoreInstruction* store : (*state_var)->store_instructions())
                if (auto* stloc = as<StLoc>(store->as_instruction())) return_stores_.push_back(stloc);
        }
        for (ILVariable* cached_state_var : cached_state_vars_) {
            for (IStoreInstruction* store : cached_state_var->store_instructions())
                if (auto* stloc = as<StLoc>(store->as_instruction())) return_stores_.push_back(stloc);
        }
    }

    if (!return_stores_.empty()) {
        context.step("Remove temporaries", function);
        for (StLoc* store : return_stores_) {
            Block* block = as<Block>(store->parent());
            if (store->variable()->load_count() == 0 && store->variable()->address_count() == 0 && block != nullptr) {
                assert(SemanticHelper::is_pure(store->value()->flags()));
                block->instructions.remove(store);
            }
        }
    }

    // Re-run control flow simplification over the newly constructed set of gotos,
    // and inlining because TranslateFieldsToLocalAccess() might have opened up new inlining opportunities.
    function->run_transforms(transforms::early_il_transforms(), context);
}

// ---- Match the enumerator creation pattern ---------------------------------------------------------------------------------------

bool YieldReturnDecompiler::match_enumerator_creation_pattern(ILFunction* function) {
    Block* body = single_block(function->body());
    if (body == nullptr || body->instructions.count() == 0) return false;

    ILInstruction* new_obj = nullptr;
    if (body->instructions.count() == 1) {
        // No parameters passed to enumerator (not even 'this'):
        // ret(newobj(...))
        if (!body->instructions[0]->match_return(&new_obj)) return false;
        if (match_enumerator_creation_new_obj(new_obj)) {
            return true;
        } else if (match_mono_enumerator_creation_new_obj(new_obj)) {
            is_compiled_with_mono_ = true;
            return true;
        } else {
            return false;
        }
    }

    // If there's parameters passed to the helper class, the class instance is first
    // stored in a variable, then the parameters are copied over, then the instance is returned.

    int pos = 0;

    // stloc(var_1, newobj(..))
    ILVariable* var1 = nullptr;
    if (!body->instructions[pos]->match_st_loc(&var1, &new_obj)) return false;
    if (match_enumerator_creation_new_obj(new_obj)) {
        pos++;  // OK
    } else if (match_mono_enumerator_creation_new_obj(new_obj)) {
        pos++;
        if (transforms::TransformDisplayClassUsage::validate_constructor(*context_, static_cast<NewObj*>(new_obj)->method)) {
            is_compiled_with_mono_ = true;
        } else {
            is_compiled_with_visual_basic_ = true;
            is_compiled_with_legacy_visual_basic_ = true;
        }
    } else {
        return false;
    }

    bool state_field_initialized = false;
    for (; pos < body->instructions.count(); pos++) {
        // stfld(..., ldloc(var_1), ldloc(parameter))
        // or (in structs): stfld(..., ldloc(var_1), ldobj(ldloc(this)))
        ILInstruction* ldloc = nullptr;
        const IField* stored_field = nullptr;
        ILInstruction* value = nullptr;
        if (!body->instructions[pos]->match_st_fld(&ldloc, &stored_field, &value)) break;
        if (!ldloc->match_ld_loc(var1)) return false;
        ILVariable* parameter = nullptr;
        if (value->match_ld_loc(&parameter) && parameter->kind() == VariableKind::Parameter) {
            field_to_parameter_map_.set(field_definition(stored_field), parameter);
        } else if (auto* ldobj = as<LdObj>(value); ldobj != nullptr && ldobj->target()->match_ld_this()) {
            // copy of 'this' in struct
            field_to_parameter_map_.set(field_definition(stored_field), static_cast<LdLoc*>(ldobj->target())->variable());
        } else if ((is_compiled_with_mono_ || is_compiled_with_legacy_visual_basic_) &&
                   (value->match_ldc_i4(-2) || value->match_ldc_i4(-1) || value->match_ldc_i4(0))) {
            state_field_ = field_definition(stored_field);
            state_field_initialized = true;
        } else {
            return false;
        }
    }

    // In debug builds, the compiler may copy the var1 into another variable (var2) before returning it.
    ILVariable* var2 = nullptr;
    ILInstruction* ldloc_for_stloc2 = nullptr;
    if (body->instructions[pos]->match_st_loc(&var2, &ldloc_for_stloc2) && ldloc_for_stloc2->match_ld_loc(var1)) {
        // stloc(var_2, ldloc(var_1))
        pos++;
    }
    ILVariable* returned_variable = var2 != nullptr ? var2 : var1;
    if (is_compiled_with_mono_ && !state_field_initialized) {
        // Mono initializes the state field separately:
        // (but not if it's left at the default value 0)
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        if (body->instructions[pos]->match_st_fld(&target, &field, &value) && target->match_ld_loc(returned_variable) &&
            (value->match_ldc_i4(-2) || value->match_ldc_i4(0))) {
            state_field_ = field_definition(field);
            pos++;
        }
    }
    ILInstruction* ret_val = nullptr;
    if (body->instructions[pos]->match_return(&ret_val) && ret_val->match_ld_loc(returned_variable)) {
        // ret(ldloc(var_2))
        return true;
    } else {
        return false;
    }
}

Block* YieldReturnDecompiler::single_block(ILInstruction* body) {
    Block* block = as<Block>(body);
    if (auto* block_container = as<BlockContainer>(body); block_container != nullptr && block_container->blocks.count() == 1) {
        block = block_container->blocks[0];
    }
    return block;
}

bool YieldReturnDecompiler::match_enumerator_creation_new_obj(ILInstruction* inst) {
    return match_enumerator_creation_new_obj(inst, *metadata_, current_type_, &enumerator_ctor_, &enumerator_type_);
}

bool YieldReturnDecompiler::match_enumerator_creation_new_obj(ILInstruction* inst, const srm::MetadataReader& metadata,
                                                              srm::TypeDefinitionHandle current_type, srm::MethodDefinitionHandle* enumerator_ctor,
                                                              srm::TypeDefinitionHandle* enumerator_type) {
    *enumerator_ctor = {};
    *enumerator_type = {};
    // newobj(CurrentType/...::.ctor, ldc.i4(-2))
    auto* new_obj = as<NewObj>(inst);
    if (new_obj == nullptr) return false;
    if (new_obj->arguments().count() != 1) return false;
    std::int32_t initial_state = 0;
    if (!new_obj->arguments()[0]->match_ldc_i4(&initial_state)) return false;
    if (!(initial_state == -2 || initial_state == 0)) return false;
    srm::EntityHandle handle = new_obj->method->metadata_token();
    *enumerator_ctor = handle.is_nil() || handle.kind() != srm::HandleKind::MethodDefinition ? srm::MethodDefinitionHandle() : srm::MethodDefinitionHandle(handle);
    *enumerator_type = enumerator_ctor->is_nil() ? srm::TypeDefinitionHandle() : metadata.get_method_definition(*enumerator_ctor).get_declaring_type();
    srm::TypeDefinitionHandle declaring_of_enumerator =
        enumerator_type->is_nil() ? srm::TypeDefinitionHandle() : metadata.get_type_definition(*enumerator_type).get_declaring_type();
    return declaring_of_enumerator == current_type && is_compiler_generator_enumerator(*enumerator_type, metadata);
}

bool YieldReturnDecompiler::match_mono_enumerator_creation_new_obj(ILInstruction* inst) {
    // mcs generates iterators that take no parameters in the ctor
    auto* new_obj = as<NewObj>(inst);
    if (new_obj == nullptr) return false;
    if (new_obj->arguments().count() != 0) return false;
    srm::EntityHandle handle = new_obj->method->metadata_token();
    enumerator_ctor_ = handle.is_nil() || handle.kind() != srm::HandleKind::MethodDefinition ? srm::MethodDefinitionHandle() : srm::MethodDefinitionHandle(handle);
    enumerator_type_ = enumerator_ctor_.is_nil() ? srm::TypeDefinitionHandle() : metadata_->get_method_definition(enumerator_ctor_).get_declaring_type();
    srm::TypeDefinitionHandle declaring_of_enumerator =
        enumerator_type_.is_nil() ? srm::TypeDefinitionHandle() : metadata_->get_type_definition(enumerator_type_).get_declaring_type();
    return declaring_of_enumerator == current_type_ && is_compiler_generator_enumerator(enumerator_type_, *metadata_);
}

bool YieldReturnDecompiler::is_compiler_generator_enumerator(srm::TypeDefinitionHandle type, const srm::MetadataReader& metadata) {
    if (type.is_nil() || !typesystem::is_compiler_generated_or_is_in_compiler_generated_class(type, metadata)) return false;
    srm::TypeDefinition td = metadata.get_type_definition(type);
    if (td.get_declaring_type().is_nil()) return false;
    for (auto i : td.get_interface_implementations()) {
        typesystem::FullTypeName tr = get_full_type_name(metadata.get_interface_implementation(i).interface_(), metadata);
        if (!tr.is_nested() && tr.top_level_type_name().namespace_() == "System.Collections" && tr.top_level_type_name().name() == "IEnumerator")
            return true;
    }
    return false;
}

// ---- Figure out what the 'state' field is (analysis of .ctor()) -------------------------------------------------------------------

void YieldReturnDecompiler::analyze_ctor() {
    Block* body = single_block(create_il_ast(enumerator_ctor_, *context_)->body());
    if (body == nullptr) throw SymbolicAnalysisFailedException("Missing enumeratorCtor.Body");
    for (ILInstruction* inst : body->instructions) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        ILVariable* arg = nullptr;
        if (inst->match_st_fld(&target, &field, &value) && target->match_ld_this() && value->match_ld_loc(&arg) &&
            arg->kind() == VariableKind::Parameter && arg->index() == 0) {
            state_field_ = field_definition(field);
        }
    }
    if (state_field_ == nullptr && !is_compiled_with_mono_) throw SymbolicAnalysisFailedException("Could not find stateField");
}

ILFunction* YieldReturnDecompiler::create_il_ast(srm::MethodDefinitionHandle method, ILTransformContext& context) {
    const srm::MetadataReader& metadata = context.pe_file().metadata();
    if (method.is_nil()) throw SymbolicAnalysisFailedException("Method not found");

    srm::MethodDefinition method_def = metadata.get_method_definition(method);
    if (!has_body(method_def)) throw SymbolicAnalysisFailedException("Method " + std::string(metadata.get_string(method_def.name())) + " has no body");

    typesystem::GenericContext generic_context = context.function()->generic_context;
    auto* type_parameters = ILArena::current().make<TypeParameterList>();
    if (generic_context.class_type_parameters != nullptr)
        type_parameters->items.insert(type_parameters->items.end(), generic_context.class_type_parameters->begin(), generic_context.class_type_parameters->end());
    if (generic_context.method_type_parameters != nullptr)
        type_parameters->items.insert(type_parameters->items.end(), generic_context.method_type_parameters->begin(), generic_context.method_type_parameters->end());
    generic_context = typesystem::GenericContext(&type_parameters->items, nullptr);
    srm::MethodBodyBlock body = context.pe_file().get_method_body(method_def.relative_virtual_address());
    std::unique_ptr<ILReader> reader = context.create_il_reader();
    ILFunction* il = reader->read_il(method, body, generic_context, ILFunctionKind::TopLevelFunction, context.cancellation_token);
    transforms::ILTransformContext il_context(il, context.type_system(), context.settings());
    il_context.cancellation_token = context.cancellation_token;
    il_context.decompile_run = context.decompile_run;
    il->run_transforms(transforms::early_il_transforms(true), il_context);
    return il;
}

// ---- Figure out what the 'current' field is (analysis of get_Current()) -----------------------------------------------------------

void YieldReturnDecompiler::analyze_current_property() {
    srm::MethodDefinitionHandle get_current_method = find_method(enumerator_type_, "get_Current");
    Block* body = single_block(create_il_ast(get_current_method, *context_)->body());
    if (body == nullptr) throw SymbolicAnalysisFailedException("get_Current has no body");
    if (body->instructions.count() == 1) {
        // release builds directly return the current field
        // ret(ldfld F(ldloc(this)))
        ILInstruction* ret_val = nullptr;
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (body->instructions[0]->match_return(&ret_val) && ret_val->match_ld_fld(&target, &field) && target->match_ld_this()) {
            current_field_ = field_definition(field);
        }
    } else if (body->instructions.count() == 2) {
        // debug builds store the return value in a temporary
        // stloc V = ldfld F(ldloc(this))
        // ret(ldloc V)
        ILVariable* v = nullptr;
        ILInstruction* ldfld = nullptr;
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* ret_val = nullptr;
        if (body->instructions[0]->match_st_loc(&v, &ldfld) && ldfld->match_ld_fld(&target, &field) && target->match_ld_this() &&
            body->instructions[1]->match_return(&ret_val) && ret_val->match_ld_loc(v)) {
            current_field_ = field_definition(field);
        }
    }
    if (current_field_ == nullptr) throw SymbolicAnalysisFailedException("Could not find currentField");
}

// ---- Figure out the mapping of IEnumerable fields to IEnumerator fields (analysis of GetEnumerator()) -----------------------------

void YieldReturnDecompiler::resolve_ienumerable_ienumerator_field_mapping() {
    srm::MethodDefinitionHandle get_enumerator_method = find_method(enumerator_type_, "GetEnumerator");
    resolve_ienumerable_ienumerator_field_mapping(get_enumerator_method, *context_, field_to_parameter_map_);
}

void YieldReturnDecompiler::resolve_ienumerable_ienumerator_field_mapping(srm::MethodDefinitionHandle get_enumerator_method, ILTransformContext& context,
                                                                          FieldToVariableMap& field_to_parameter_map) {
    if (get_enumerator_method.is_nil()) return;  // no mappings (maybe it's just an IEnumerator implementation?)
    ILFunction* function = create_il_ast(get_enumerator_method, context);
    for (ILInstruction* d : function->descendants()) {
        auto* block = as<Block>(d);
        if (block == nullptr) continue;
        for (ILInstruction* inst : block->instructions) {
            // storeTarget.storeField = this.loadField;
            ILInstruction* store_target = nullptr;
            const IField* store_field = nullptr;
            ILInstruction* store_value = nullptr;
            ILInstruction* load_target = nullptr;
            const IField* load_field = nullptr;
            if (inst->match_st_fld(&store_target, &store_field, &store_value) && store_value->match_ld_fld(&load_target, &load_field) &&
                load_target->match_ld_this()) {
                store_field = field_definition(store_field);
                load_field = field_definition(load_field);
                if (ILVariable** mapped_parameter = field_to_parameter_map.find(load_field)) field_to_parameter_map.set(store_field, *mapped_parameter);
            }
        }
    }
}

// ---- Construction of the exception table (analysis of Dispose()) -------------------------------------------------------------------
// We construct the exception table by analyzing the enumerator's Dispose() method.

void YieldReturnDecompiler::construct_exception_table() {
    dispose_method_ = find_method(enumerator_type_, "Dispose");
    ILFunction* function = create_il_ast(dispose_method_, *context_);

    if (!is_compiled_with_visual_basic_ && !is_compiled_with_mono_) {
        auto* body = static_cast<BlockContainer*>(function->body());
        bool found = false;
        for (Block* block : body->blocks) {
            for (ILInstruction* instr : block->instructions) {
                auto* call = as<CallInstruction>(instr);
                if (call != nullptr && call->arguments().count() == 1 && call->arguments()[0]->match_ld_this() &&
                    is_method(srm::MethodDefinitionHandle(call->method->metadata_token()), "MoveNext")) {
                    is_compiled_with_visual_basic_ = true;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
    }

    if (is_compiled_with_mono_ || is_compiled_with_visual_basic_) {
        auto* body = static_cast<BlockContainer*>(function->body());
        for (int i = 0; (i < body->entry_point()->instructions.count()) && !is<Branch>(body->entry_point()->instructions[i]); i++) {
            ILInstruction* target = nullptr;
            const IField* field = nullptr;
            ILInstruction* value = nullptr;
            auto* stobj = as<StObj>(body->entry_point()->instructions[i]);
            if (stobj != nullptr && stobj->match_st_fld(&target, &field, &value) && target->match_ld_this() &&
                typesystem::is_known_type(field->type(), KnownTypeCode::Boolean) && value->match_ldc_i4(1)) {
                disposing_field_ = field_definition(field);
                break;
            }
        }

        // On mono and VB, we don't need to analyse Dispose() to reconstruct the try-finally structure.
        finally_method_to_state_range_.reset();
    } else {
        // Non-Mono/Non-VB: analyze try-finally structure in Dispose()
        StateRangeAnalysis range_analysis(StateRangeAnalysisMode::IteratorDispose, state_field_);
        range_analysis.assign_state_ranges(function->body(), util::LongSet::universe());
        finally_method_to_state_range_ = range_analysis.finally_method_to_state_range;
    }
}

// ---- Analyze MoveNext() and generate new body ---------------------------------------------------------------------------------------

BlockContainer* YieldReturnDecompiler::analyze_move_next(ILFunction* function) {
    context_->step_start_group("AnalyzeMoveNext");
    srm::MethodDefinitionHandle move_next_method = find_method(enumerator_type_, "MoveNext");
    ILFunction* move_next_function = create_il_ast(move_next_method, *context_);

    function->move_next_method = move_next_function->method;
    function->sequence_point_candidates = move_next_function->sequence_point_candidates;
    function->code_size = move_next_function->code_size;
    function->local_variable_signature_length = move_next_function->local_variable_signature_length;

    // Copy-propagate temporaries holding a copy of 'this'.
    // This is necessary because the old (pre-Roslyn) C# compiler likes to store 'this' in temporary variables.
    {
        std::vector<StLoc*> stlocs;
        for (ILInstruction* d : move_next_function->descendants()) {
            auto* s = as<StLoc>(d);
            if (s != nullptr && s->variable()->is_single_definition() && s->value()->match_ld_this()) stlocs.push_back(s);
        }
        for (StLoc* stloc : stlocs) transforms::CopyPropagation::propagate(stloc, *context_);
    }

    // Copy propagate stack slots holding a 32 bit integer.
    {
        std::vector<StLoc*> stlocs;
        for (ILInstruction* d : move_next_function->descendants()) {
            auto* s = as<StLoc>(d);
            if (s != nullptr && s->variable()->kind() == VariableKind::StackSlot && s->variable()->is_single_definition() &&
                s->value()->op_code == OpCode::LdcI4)
                stlocs.push_back(s);
        }
        for (StLoc* stloc : stlocs) transforms::CopyPropagation::propagate(stloc, *context_);
    }

    for (ILInstruction* d : move_next_function->descendants()) {
        if (auto* block = as<Block>(d)) block->instructions.remove_all([](ILInstruction* inst) { return inst->op_code == OpCode::LdcI4; });
    }

    auto* body = static_cast<BlockContainer*>(move_next_function->body());
    if (auto* try_fault = body->blocks.count() == 1 && body->blocks[0]->instructions.count() == 1 ? as<TryFault>(body->blocks[0]->instructions[0]) : nullptr) {
        body = static_cast<BlockContainer*>(try_fault->try_block());
        auto* fault_block_container = as<BlockContainer>(try_fault->fault_block());
        if (fault_block_container == nullptr || fault_block_container->blocks.count() != 1)
            throw SymbolicAnalysisFailedException("Unexpected number of blocks in MoveNext() fault block");
        Block* fault_block = fault_block_container->blocks[0];
        auto* call = fault_block->instructions.count() == 2 ? as<Call>(fault_block->instructions[0]) : nullptr;
        if (!(call != nullptr && call->method->metadata_token() == srm::EntityHandle(dispose_method_) && call->arguments().count() == 1 &&
              call->arguments()[0]->match_ld_this() && fault_block->instructions[1]->match_leave(fault_block_container))) {
            throw SymbolicAnalysisFailedException("Unexpected fault block contents in MoveNext()");
        }
    }

    ILVariable* first_var = nullptr;
    ILInstruction* ldc = nullptr;
    if (is_compiled_with_legacy_visual_basic_ && (body->blocks.count() == 2 || body->blocks.count() == 1) && body->blocks[0]->instructions.count() == 2 &&
        body->blocks[0]->instructions[0]->match_st_loc(&first_var, &ldc) && ldc->match_ldc_i4(1)) {
        do_finally_bodies_ = first_var;
        if (auto* try_catch = as<TryCatch>(body->blocks[0]->instructions[1]); try_catch != nullptr && try_catch->handlers.count() == 1) {
            TryCatchHandler* catch_handler = try_catch->handlers[0];
            auto* catch_block_container = as<BlockContainer>(catch_handler->body());
            if (catch_block_container == nullptr || catch_block_container->blocks.count() != 1)
                throw SymbolicAnalysisFailedException("Unexpected number of blocks in MoveNext() catch block");
            Block* catch_block = catch_block_container->blocks[0];
            bool ok = false;
            if (catch_block->instructions.count() == 4) {
                auto* call = as<Call>(catch_block->instructions[0]);
                ILVariable* unused_variable = nullptr;
                ILInstruction* ldloc = nullptr;
                ILInstruction* ld_this = nullptr;
                const IField* fld = nullptr;
                ILInstruction* value = nullptr;
                ok = call != nullptr && call->method->name() == "SetProjectError" && call->arguments().count() == 1 &&
                     call->arguments()[0]->match_ld_loc(catch_handler->variable()) &&
                     catch_block->instructions[1]->match_st_loc(&unused_variable, &ldloc) && ldloc->match_ld_loc(catch_handler->variable()) &&
                     catch_block->instructions[2]->match_st_fld(&ld_this, &fld, &value) && ld_this->match_ld_this() &&
                     member_equals(fld->member_definition(), state_field_) && is<LdcI4>(value) && is<Rethrow>(catch_block->instructions[3]);
            }
            if (!ok) throw SymbolicAnalysisFailedException("Unexpected catch block contents in MoveNext()");
            auto* try_catch_body = static_cast<BlockContainer*>(try_catch->try_block());
            // Move return block
            if (body->blocks.count() == 2) try_catch_body->blocks.add(body->blocks[1]);
            body = try_catch_body;
        }
    }

    if (state_field_ == nullptr) {
        // With mono-compiled state machines, it's possible that we haven't discovered the state field
        // yet because the compiler let it be implicitly initialized to 0.
        // In this case, we must discover it from the first instruction in MoveNext():
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        auto* stloc = as<StLoc>(body->entry_point()->instructions[0]);
        if (stloc != nullptr && stloc->value()->match_ld_fld(&target, &field) && target->match_ld_this() &&
            typesystem::is_known_type(field->type(), KnownTypeCode::Int32)) {
            state_field_ = field_definition(field);
        } else {
            throw SymbolicAnalysisFailedException("Could not find state field.");
        }
    }

    skip_finally_bodies_ = nullptr;
    if (is_compiled_with_mono_) {
        // Mono uses skipFinallyBodies; find out which variable that is:
        for (ILInstruction* d : body->descendants()) {
            auto* try_finally = as<TryFinally>(d);
            if (try_finally == nullptr) continue;
            auto* finally_container = as<BlockContainer>(try_finally->finally_block());
            auto* if_inst = finally_container != nullptr ? as<IfInstruction>(finally_container->entry_point()->instructions[0]) : nullptr;
            if (if_inst != nullptr) {
                ILInstruction* arg = nullptr;
                ILVariable* v = nullptr;
                if (if_inst->condition()->match_logic_not(&arg) && arg->match_ld_loc(&v) && typesystem::is_known_type(v->type(), KnownTypeCode::Boolean)) {
                    bool is_initialized_in_entry_block = false;
                    for (int i = 0; i < 3; i++) {
                        auto* stloc = as<StLoc>(body->entry_point()->instructions.element_at_or_default(i));
                        if (stloc != nullptr && stloc->variable() == v && stloc->value()->match_ldc_i4(0)) {
                            is_initialized_in_entry_block = true;
                            break;
                        }
                    }
                    if (is_initialized_in_entry_block) {
                        skip_finally_bodies_ = v;
                        break;
                    }
                }
            }
        }
    }

    propagate_copies_of_fields(body);

    // Note: body may contain try-catch or try-finally statements that have nested block containers,
    // but those cannot contain any yield statements.
    // So for reconstructing the control flow, we only consider the blocks directly within body.

    StateRangeAnalysis range_analysis(StateRangeAnalysisMode::IteratorMoveNext, state_field_, nullptr, is_compiled_with_legacy_visual_basic_);
    range_analysis.skip_finally_bodies = skip_finally_bodies_;
    range_analysis.do_finally_bodies = do_finally_bodies_;
    range_analysis.cancellation_token = context_->cancellation_token;
    range_analysis.assign_state_ranges(body, util::LongSet::universe());
    cached_state_vars_.clear();
    for (ILVariable* v : range_analysis.cached_state_vars())
        if (std::find(cached_state_vars_.begin(), cached_state_vars_.end(), v) == cached_state_vars_.end()) cached_state_vars_.push_back(v);

    BlockContainer* new_body = convert_body(body, range_analysis);
    move_next_function->variables().clear();
    // release references from old moveNextFunction to instructions that were moved over to newBody
    move_next_function->release_ref();
    context_->step_end_group();
    return new_body;
}

void YieldReturnDecompiler::propagate_copies_of_fields(BlockContainer* body) {
    // Roslyn may optimize MoveNext() by copying fields from the iterator class into local variables
    // at the beginning of MoveNext(). Undo this optimization.
    context_->step_start_group("PropagateCopiesOfFields");
    std::vector<const IField*> mutable_fields;
    for (ILInstruction* d : body->descendants()) {
        auto* ldflda = as<LdFlda>(d);
        if (ldflda == nullptr || ldflda->parent()->op_code == OpCode::LdObj) continue;
        const IField* field = ldflda->field();
        bool known = false;
        for (const IField* f : mutable_fields)
            if (member_equals(f, field)) {
                known = true;
                break;
            }
        if (!known) mutable_fields.push_back(field);
    }
    for (int i = 0; i < body->entry_point()->instructions.count(); i++) {
        auto* store = as<StLoc>(body->entry_point()->instructions[i]);
        auto* ldobj = store != nullptr ? as<LdObj>(store->value()) : nullptr;
        auto* ldflda = ldobj != nullptr ? as<LdFlda>(ldobj->target()) : nullptr;
        if (store != nullptr && store->variable()->is_single_definition() && ldobj != nullptr && ldflda != nullptr && ldflda->target()->match_ld_this()) {
            bool is_mutable = false;
            for (const IField* f : mutable_fields)
                if (member_equals(f, ldflda->field())) {
                    is_mutable = true;
                    break;
                }
            if (!is_mutable) {
                // perform copy propagation: (unlike CopyPropagation.Propagate(), copy the ldobj arguments as well)
                std::vector<LdLoc*> loads = store->variable()->load_instructions();
                for (LdLoc* expr : loads) expr->replace_with(store->value()->clone());
                body->entry_point()->instructions.remove_at(i--);
            } else if (ldflda->field()->member_definition() == state_field_->member_definition()) {
                continue;
            } else {
                break;  // unsupported: load of mutable field (other than state field)
            }
        } else {
            break;  // unknown instruction
        }
    }
    context_->step_end_group();
}

// Convert the old body (of MoveNext function) to the new body (of decompiled iterator method).
//
// * Replace the sequence
//       this.currentField = expr;
//       this.state = N;
//       return true;
//     with:
//       yield return expr;
//       goto blockForState(N);
//  * Replace the sequence:
//       this._finally2();
//       this._finally1();
//       return false;
//     with:
//       yield break;
//  * Reconstruct try-finally blocks from
//      (on enter) this.state = N;
//      (on exit)  this._finallyX();
BlockContainer* YieldReturnDecompiler::convert_body(BlockContainer* old_body, StateRangeAnalysis& range_analysis) {
    ConvertBodyContext ctx;
    ctx.old_body = old_body;
    ctx.block_state_map = range_analysis.get_block_state_set_mapping(old_body);
    BlockContainer* new_body = with_il_range(make<BlockContainer>(), old_body);
    ctx.new_body = new_body;
    // create all new blocks so that they can be referenced by gotos
    for (int block_index = 0; block_index < old_body->blocks.count(); block_index++) {
        new_body->blocks.add(with_il_range(make<Block>(), old_body->blocks[block_index]));
    }
    // convert contents of blocks

    for (int i = 0; i < old_body->blocks.count(); i++) {
        Block* old_block = old_body->blocks[i];
        Block* new_block = new_body->blocks[i];
        for (ILInstruction* old_inst : old_block->instructions) {
            context_->cancellation_token.throw_if_cancellation_requested();
            ILInstruction* target = nullptr;
            const IField* field = nullptr;
            ILInstruction* value = nullptr;
            if (old_inst->match_st_fld(&target, &field, &value) && target->match_ld_this()) {
                if (member_equals(field->member_definition(), state_field_)) {
                    std::int32_t new_state = 0;
                    if (value->match_ldc_i4(&new_state)) {
                        // On state change, break up the block:
                        // (this allows us to consider each block individually for try-finally reconstruction)
                        new_block = split_block(ctx, new_block, old_inst);
                        // We keep the state-changing instruction around (as first instruction of the new block)
                        // for reconstructing the try-finallys.
                    } else {
                        new_block->instructions.add(with_il_range(make<InvalidExpression>("Assigned non-constant to iterator.state field"), old_inst));
                        report_error(new_block->instructions.last());
                        continue;  // don't copy over this instruction, but continue with the basic block
                    }
                } else if (member_equals(field->member_definition(), current_field_)) {
                    // create yield return
                    new_block->instructions.add(with_il_range(make<YieldReturn>(value), old_inst));
                    convert_branch_after_yield_return(ctx, new_block, old_block, old_inst->child_index() + 1);
                    break;  // we're done with this basic block
                }
            } else if (auto* call = as<Call>(old_inst);
                       call != nullptr && call->arguments().count() == 1 && call->arguments()[0]->match_ld_this() &&
                       [&] {
                           if (!finally_method_to_state_range_) throw std::runtime_error("Object reference not set to an instance of an object.");
                           return finally_method_to_state_range_->contains_key(method_definition(call->method));
                       }()) {
                // Break up the basic block on a call to a finally method
                // (this allows us to consider each block individually for try-finally reconstruction)
                new_block = split_block(ctx, new_block, old_inst);
            } else if (auto* try_finally = as<TryFinally>(old_inst); try_finally != nullptr && (is_compiled_with_mono_ || is_compiled_with_visual_basic_)) {
                // with mono, we have to recurse into try-finally blocks
                auto* old_try_block = static_cast<BlockContainer*>(try_finally->try_block());
                StateRangeAnalysis sra = range_analysis.create_nested_analysis();
                sra.assign_state_ranges(old_try_block, util::LongSet::universe());
                try_finally->set_try_block(convert_body(old_try_block, sra));
            } else if (auto* if_instruction = as<IfInstruction>(old_inst); is_compiled_with_legacy_visual_basic_ && if_instruction != nullptr &&
                       if_instruction->false_inst()->match_nop() && [&] {
                           ILInstruction* left = nullptr;
                           ILInstruction* right = nullptr;
                           ILInstruction* ld_this = nullptr;
                           const IField* fld = nullptr;
                           return if_instruction->condition()->match_comp_equals(&left, &right) && left->match_ld_fld(&ld_this, &fld) &&
                                  ld_this->match_ld_this() && member_equals(fld->member_definition(), disposing_field_) && right->match_ldc_i4(0);
                       }()) {
                new_block->instructions.add(if_instruction->true_inst());
                new_block->add_il_range(*if_instruction->true_inst());
                update_branch_targets(ctx, if_instruction->true_inst());
                break;
            }

            // copy over the instruction to the new block
            new_block->instructions.add(old_inst);
            new_block->add_il_range(*old_inst);
            update_branch_targets(ctx, old_inst);
        }
    }

    // Insert new artificial block as entry point, and jump to the initial state.
    // This causes the method to start directly at the first user code,
    // and the whole compiler-generated state-dispatching logic becomes unreachable code
    // and gets deleted.
    int initial_state = is_compiled_with_legacy_visual_basic_ ? -1 : 0;
    Block* entry = make<Block>();
    entry->instructions.add(make_go_to(ctx, initial_state));
    new_body->blocks.insert(0, entry);
    return new_body;
}

void YieldReturnDecompiler::convert_branch_after_yield_return(ConvertBodyContext& ctx, Block* new_block, Block* old_block, int pos) {
    Block* target_block = nullptr;
    ILInstruction* value = nullptr;
    if (is_compiled_with_mono_ && disposing_field_ != nullptr) {
        // Mono skips over the state assignment if 'this.disposing' is set:
        //      ...
        //      stfld $current(ldloc this, yield-expr)
        //  	if (ldfld $disposing(ldloc this)) br IL_007c
        //  	br targetBlock
        //  }
        //
        //  Block targetBlock (incoming: 1) {
        //  	stfld $PC(ldloc this, ldc.i4 1)
        //  	br setSkipFinallyBodies
        //  }
        //
        //  Block setSkipFinallyBodies (incoming: 2) {
        //  	stloc skipFinallyBodies(ldc.i4 1)
        //  	br returnBlock
        //  }
        ILInstruction* condition = nullptr;
        ILInstruction* unused_true_inst = nullptr;
        ILInstruction* cond_target = nullptr;
        const IField* cond_field = nullptr;
        if (old_block->instructions[pos]->match_if_instruction(&condition, &unused_true_inst) && condition->match_ld_fld(&cond_target, &cond_field) &&
            cond_target->match_ld_this() && member_equals(cond_field->member_definition(), disposing_field_) &&
            old_block->instructions[pos + 1]->match_branch(&target_block) && target_block->parent() == old_block->parent()) {
            // Keep looking at the target block:
            old_block = target_block;
            pos = 0;
        }
    }

    // Visual Basic Compiler emits additional stores to variables.
    std::optional<int> local_new_state;
    ILVariable* unused_variable = nullptr;
    if (ILInstruction* stored = nullptr; old_block->instructions[pos]->match_st_loc(&unused_variable, &stored) && is<LdcI4>(stored)) {
        local_new_state = static_cast<LdcI4*>(stored)->value;
        pos++;
    }

    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    std::int32_t new_state = 0;
    if (old_block->instructions[pos]->match_st_fld(&target, &field, &value) && target->match_ld_this() && field->member_definition() == state_field_ &&
        value->match_ldc_i4(&new_state) && (!local_new_state.has_value() || *local_new_state == new_state)) {
        pos++;
    } else {
        new_block->instructions.add(make<InvalidBranch>("Unable to find new state assignment for yield return"));
        report_error(new_block->instructions.last());
    }
    // Mono may have 'br setSkipFinallyBodies' here, so follow the branch
    if (old_block->instructions[pos]->match_branch(&target_block) && target_block->parent() == old_block->parent()) {
        old_block = target_block;
        pos = 0;
    }
    if (old_block->instructions[pos]->match_st_loc(skip_finally_bodies_, &value)) {
        if (!value->match_ldc_i4(1)) {
            auto* invalid = make<InvalidExpression>();
            invalid->expected_result_type = StackType::Void;
            invalid->message = "Unexpected assignment to skipFinallyBodies";
            new_block->instructions.add(invalid);
            report_error(new_block->instructions.last());
        }
        pos++;
    }
    // We can't use MatchStLoc like above since the doFinallyBodies variable is split by SplitVariables.
    // This occurs for the Legacy VBC compiler.
    ILVariable* var = nullptr;
    if (old_block->instructions[pos]->match_st_loc(&var, &value) && var->kind() == VariableKind::Local &&
        var->index() == (do_finally_bodies_ != nullptr ? do_finally_bodies_->index() : std::nullopt)) {
        if (!value->match_ldc_i4(0)) {
            auto* invalid = make<InvalidExpression>();
            invalid->expected_result_type = StackType::Void;
            invalid->message = "Unexpected assignment to doFinallyBodies";
            new_block->instructions.add(invalid);
            report_error(new_block->instructions.last());
        }
        pos++;
    }

    ILInstruction* ret_val = nullptr;
    if (old_block->instructions[pos]->match_return(&ret_val) && ret_val->match_ldc_i4(1)) {
        // OK, found return directly after state assignment
    } else if (old_block->instructions[pos]->match_branch(&target_block) && target_block->instructions[0]->match_return(&ret_val) &&
               ret_val->match_ldc_i4(1)) {
        // OK, jump to common return block (e.g. on Mono)
    } else {
        new_block->instructions.add(make<InvalidBranch>("Unable to find 'return true' for yield return"));
        report_error(new_block->instructions.last());
    }
    new_block->instructions.add(make_go_to(ctx, new_state));
}

Block* YieldReturnDecompiler::split_block(ConvertBodyContext& ctx, Block* new_block, ILInstruction* old_inst) {
    if (new_block->instructions.count() > 0) {
        Block* new_block2 = make<Block>();
        new_block2->add_il_range(Interval(old_inst->start_il_offset(), old_inst->start_il_offset()));
        ctx.new_body->blocks.add(new_block2);
        new_block->instructions.add(make<Branch>(new_block2));
        new_block = new_block2;
    }
    return new_block;
}

ILInstruction* YieldReturnDecompiler::make_go_to(ConvertBodyContext& ctx, int v) {
    Block* target_block = ctx.block_state_map.get_or_default(v);
    if (target_block != nullptr) {
        if (target_block->parent() == ctx.old_body)
            return make<Branch>(ctx.new_body->blocks[target_block->child_index()]);
        else
            return make<Branch>(target_block);
    } else {
        auto* err = make<InvalidBranch>("Could not find block for state " + std::to_string(v));
        report_error(err);
    }
}

void YieldReturnDecompiler::update_branch_targets(ConvertBodyContext& ctx, ILInstruction* inst) {
    if (auto* branch = as<Branch>(inst)) {
        if (branch->target_container() == ctx.old_body) {
            branch->set_target_block(ctx.new_body->blocks[branch->target_block()->child_index()]);
        }
    } else if (auto* leave = as<Leave>(inst)) {
        ILInstruction* value = nullptr;
        if (leave->match_return(&value)) {
            bool valid_yield_break = value->match_ldc_i4(0);
            ILVariable* v = nullptr;
            if (value->match_ld_loc(&v) && (v->kind() == VariableKind::Local || v->kind() == VariableKind::StackSlot)) {
                bool all_zero = true;
                for (IStoreInstruction* store : v->store_instructions()) {
                    auto* stloc = as<StLoc>(store->as_instruction());
                    if (!(stloc != nullptr && stloc->value()->match_ldc_i4(0))) {
                        all_zero = false;
                        break;
                    }
                }
                if (all_zero) {
                    valid_yield_break = true;
                    for (IStoreInstruction* store : v->store_instructions()) return_stores_.push_back(static_cast<StLoc*>(store->as_instruction()));
                }
            }
            if (valid_yield_break) {
                // yield break
                leave->replace_with(with_il_range(make<Leave>(ctx.new_body), leave));
            } else {
                // don't treat this as an error, it might just be unreachable code that will be removed soon
                // (occurs with mcs yield return)
                leave->replace_with(with_il_range(make<InvalidBranch>("Unexpected return in MoveNext()"), leave));
            }
        } else {
            if (leave->target_container() == ctx.old_body) {
                leave->set_target_container(ctx.new_body);
            }
        }
    }
    for (ILInstruction* child : inst->children()) update_branch_targets(ctx, child);
}

void YieldReturnDecompiler::report_error(ILInstruction* inst) {
    // ConvertBody is still called within the try-catch, so we can throw SymbolicAnalysisFailedException
    // to suppress conversion of the state machine altogether.
    // We still initially create an instruction before converting a to an exception,
    // so that the body of this function can be commented out for testing purposes.
    // (this allows seeing where exactly the error occurs in the converted body output)
    std::string message = "ConvertBody error";
    if (auto* invalid_branch = as<InvalidBranch>(inst))
        message = invalid_branch->message;
    else if (auto* invalid_expr = as<InvalidExpression>(inst))
        message = invalid_expr->message;
    throw SymbolicAnalysisFailedException(message);
}

// ---- TranslateFieldsToLocalAccess ----------------------------------------------------------------------------------------------------

void YieldReturnDecompiler::translate_fields_to_local_access(ILFunction* function, ILInstruction* inst, FieldToVariableMap& field_to_variable_map,
                                                             bool is_compiled_with_mono) {
    ILInstruction* ldflda_target = nullptr;
    const IField* ldflda_field = nullptr;
    if (inst->match_ld_flda(&ldflda_target, &ldflda_field) && ldflda_target->match_ld_this()) {
        const IField* field_def = field_definition(ldflda_field);
        ILVariable* v = nullptr;
        if (ILVariable** found = field_to_variable_map.find(field_def)) {
            v = *found;
        } else {
            std::string name;
            std::string field_name = field_def->name();
            if (!field_name.empty() && field_name[0] == '<') {
                std::size_t pos = field_name.find('>');
                if (pos != std::string::npos && pos > 1) name = field_name.substr(1, pos - 1);
            }
            v = function->register_variable(VariableKind::Local, ldflda_field->return_type(), name);
            v->set_initial_value_is_initialized(true);  // the field was default-initialized, so keep those semantics for the variable
            v->set_uses_initial_value(true);
            v->state_machine_field = ldflda_field;
            field_to_variable_map.try_add(field_def, v);
        }
        if (v->stack_type == StackType::Ref) {
            assert(v->kind() == VariableKind::Parameter && v->index() && *v->index() < 0);  // this pointer
            inst->replace_with(with_il_range(make<LdLoc>(v), inst));
        } else {
            inst->replace_with(with_il_range(make<LdLoca>(v), inst));
        }
    } else if (!is_compiled_with_mono && inst->match_ld_this()) {
        auto* invalid = make<InvalidExpression>("stateMachine");
        invalid->expected_result_type = inst->result_type();
        inst->replace_with(with_il_range(invalid, inst));
    } else {
        for (ILInstruction* child : inst->children()) translate_fields_to_local_access(function, child, field_to_variable_map, is_compiled_with_mono);
        auto* ldobj = as<LdObj>(inst);
        auto* ldloca = ldobj != nullptr ? as<LdLoca>(ldobj->target()) : nullptr;
        auto* stobj = as<StObj>(inst);
        auto* ldloca2 = stobj != nullptr ? as<LdLoca>(stobj->target()) : nullptr;
        if (ldobj != nullptr && ldloca != nullptr && ldloca->variable()->state_machine_field != nullptr) {
            LdLoc* ldloc = make<LdLoc>(ldloca->variable());
            ldloc->add_il_range(*ldobj);
            ldloc->add_il_range(*ldloca);
            inst->replace_with(ldloc);
        } else if (stobj != nullptr && ldloca2 != nullptr && ldloca2->variable()->state_machine_field != nullptr) {
            StLoc* stloc = make<StLoc>(ldloca2->variable(), stobj->value());
            stloc->add_il_range(*stobj);
            stloc->add_il_range(*ldloca2);
            inst->replace_with(stloc);
        }
    }
}

// ---- DecompileFinallyBlocks ------------------------------------------------------------------------------------------------------------

void YieldReturnDecompiler::decompile_finally_blocks() {
    std::vector<const IMethod*> methods;
    for (const auto& entry : *finally_method_to_state_range_) methods.push_back(entry.first);
    for (const IMethod* method : methods) {
        ILFunction* function = create_il_ast(srm::MethodDefinitionHandle(method->metadata_token()), *context_);
        auto* body = static_cast<BlockContainer*>(function->body());
        std::optional<int> new_state = get_new_state(body->entry_point());
        if (new_state.has_value()) {
            body->entry_point()->instructions.remove_at(0);
        }
        // Avoid yield-return decompilation if there are unrecognized state assignments in a finally method.
        for (ILInstruction* inst : body->descendants()) {
            if (is_state_assignment(inst))
                throw SymbolicAnalysisFailedException(std::format("Unknown state transition in {} at IL_{:04x}",
                                                                  function->name.empty() ? std::string("finally") : function->name,
                                                                  inst->start_il_offset()));
        }
        function->release_ref();  // make body reusable outside of function
        decompiled_finally_methods_[method] = DecompiledFinallyMethod{new_state, function};
    }
}

// ---- Reconstruct try-finally blocks -------------------------------------------------------------------------------------------------------

// Reconstruct try-finally blocks.
// * The stateChanges (iterator._state = N;) tell us when to open a try-finally block
// * The calls to the finally method tell us when to leave the try block.
//
// There might be multiple stateChanges for a given try-finally block, e.g.
// both the original entry point, and the target when leaving a nested block.
// In proper C# code, the entry point of the try-finally will dominate all other code
// in the try-block, so we can use dominance to find the proper entry point.
//
// Precondition: the blocks in newBody are topologically sorted.
void YieldReturnDecompiler::reconstruct_try_finally_blocks(ILFunction* iterator_function) {
    auto* new_body = static_cast<BlockContainer*>(iterator_function->body());
    context_->step("Reconstuct try-finally blocks", new_body);
    std::vector<int> block_state(static_cast<std::size_t>(new_body->blocks.count()), 0);
    block_state[0] = -1;
    std::unordered_map<int, BlockContainer*> state_to_container;
    state_to_container.emplace(-1, new_body);

    auto find_finally_method = [&](int state) -> const IMethod* {
        const IMethod* found_method = nullptr;
        for (const auto& [method, state_range] : *finally_method_to_state_range_) {
            if (state_range.contains(state)) {
                if (found_method == nullptr)
                    found_method = method;
                else
                    assert(false && "Ambiguous finally method for state");
            }
        }
        return found_method;
    };

    auto create_try_block = [&](Block* block, int state) {
        const IMethod* finally_method = find_finally_method(state);
        if (finally_method != nullptr) {
            // remove the method so that it doesn't cause ambiguity when processing nested try-finally blocks
            finally_method_to_state_range_->remove(finally_method);
        }

        Block* try_block = make<Block>();
        try_block->add_il_range(*block);
        try_block->instructions.add_range(block->instructions);
        BlockContainer* try_block_container = make<BlockContainer>();
        try_block_container->blocks.add(try_block);
        try_block_container->add_il_range(*try_block);
        state_to_container.emplace(state, try_block_container);

        ILInstruction* finally_block = nullptr;
        if (finally_method == nullptr) {
            finally_block = make<InvalidBranch>(
                "Could not find finallyMethod for state=" + std::to_string(state) + ".\n" +
                "Possibly this method is affected by a C# compiler bug that causes the finally body\n" +
                "not to run in case of an exception or early 'break;' out of a loop consuming this iterable.");
        } else if (auto it = decompiled_finally_methods_.find(finally_method); it != decompiled_finally_methods_.end()) {
            const DecompiledFinallyMethod& decompiled_method = it->second;
            finally_block = decompiled_method.function->body();
            std::vector<ILVariable*> vars(decompiled_method.function->variables().begin(), decompiled_method.function->variables().end());
            decompiled_method.function->variables().clear();
            for (ILVariable* v : vars) iterator_function->variables().add(v);
        } else {
            finally_block = make<InvalidBranch>("Missing decompiledFinallyMethod");
        }

        block->instructions.clear();
        block->instructions.add(with_il_range(make<TryFinally>(try_block_container, finally_block), try_block_container));
    };

    // First, analyse the newBody: for each block, determine the active state number.
    for (int block_index = 0; block_index < new_body->blocks.count(); block_index++) {
        Block* block = new_body->blocks[block_index];
        context_->cancellation_token.throw_if_cancellation_requested();
        int old_state = block_state[static_cast<std::size_t>(block->child_index())];
        BlockContainer* container = nullptr;  // new container for the block
        int new_state = 0;
        if (std::optional<int> state_change = get_new_state(block)) {
            new_state = *state_change;
            // OK, state change
            // Remove the state-changing instruction
            block->instructions.remove_at(0);

            if (auto found = state_to_container.find(new_state); found != state_to_container.end()) {
                container = found->second;
            } else {
                // First time we see this state.
                // This means we just found the entry point of a try block.
                create_try_block(block, new_state);
                // CreateTryBlock() wraps the contents of 'block' with a TryFinally.
                // We thus need to put the block (which now contains the whole TryFinally)
                // into the parent container.
                // Assuming a state transition never enters more than one state at once,
                // we can use stateToContainer[oldState] as parent.
                container = state_to_container.at(old_state);
            }
        } else {
            // Because newBody is topologically sorted we because we just removed unreachable code,
            // we can assume that blockState[] was already set for this block.
            new_state = old_state;
            container = state_to_container.at(old_state);
        }
        if (container != new_body) {
            // Move the block into the container.
            container->blocks.add(block);
            // Keep the stale reference in newBody.Blocks for now, to avoid
            // changing the ChildIndex of the other blocks while we use it
            // to index the blockState array.
        }
        // Propagate newState to successor blocks
        for (ILInstruction* d : block->descendants()) {
            auto* branch = as<Branch>(d);
            if (branch == nullptr) continue;
            if (branch->target_block()->parent() == new_body) {
                int state_after_branch = new_state;
                auto* call = as<Call>(Block::get_predecessor(branch));
                if (call != nullptr && call->arguments().count() == 1 && call->arguments()[0]->match_ld_this() &&
                    call->method->metadata_token() == srm::EntityHandle(dispose_method_)) {
                    // pre-roslyn compiles "yield break;" into "Dispose(); goto return_false;",
                    // so convert the dispose call into a state transition to the final state
                    state_after_branch = -1;
                    auto* nop = make<Nop>();
                    nop->comment = "Dispose call";
                    call->replace_with(nop);
                }
                assert(block_state[static_cast<std::size_t>(branch->target_block()->child_index())] == state_after_branch ||
                       block_state[static_cast<std::size_t>(branch->target_block()->child_index())] == 0);
                block_state[static_cast<std::size_t>(branch->target_block()->child_index())] = state_after_branch;
            }
        }
    }
    new_body->blocks.remove_all([new_body](Block* b) { return b->parent() != new_body; });
}

bool YieldReturnDecompiler::is_state_assignment(ILInstruction* inst) const {
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    ILInstruction* value = nullptr;
    return inst->match_st_fld(&target, &field, &value) && target->match_ld_this() && member_equals(field->member_definition(), state_field_);
}

// Gets the state that is transitioned to at the start of the block
std::optional<int> YieldReturnDecompiler::get_new_state(Block* block) const {
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    ILInstruction* value = nullptr;
    std::int32_t new_state = 0;
    if (block->instructions[0]->match_st_fld(&target, &field, &value) && target->match_ld_this() && member_equals(field->member_definition(), state_field_) &&
        value->match_ldc_i4(&new_state)) {
        return new_state;
    } else if (auto* call = as<Call>(block->instructions[0]); call != nullptr && call->arguments().count() == 1 && call->arguments()[0]->match_ld_this()) {
        auto it = decompiled_finally_methods_.find(method_definition(call->method));
        if (it != decompiled_finally_methods_.end()) return it->second.outer_state;
    }
    return std::nullopt;
}

// ---- Cleanup finally blocks -------------------------------------------------------------------------------------------------------------

// Eliminates usage of doFinallyBodies
void YieldReturnDecompiler::clean_skip_finally_bodies(ILFunction* function) {
    if (skip_finally_bodies_ == nullptr) return;  // only mono-compiled code uses skipFinallyBodies
    context_->step_start_group("CleanFinallyBlocks", function);
    if (skip_finally_bodies_->store_instructions().size() != 0 || skip_finally_bodies_->address_count() != 0) {
        // misdetected another variable as skipFinallyBodies?
        // Fortunately removing the initial store of 0 is harmless, as we
        // default-initialize the variable on uninit uses
        return;
    }
    for (ILInstruction* d : function->descendants()) {
        auto* try_finally = as<TryFinally>(d);
        if (try_finally == nullptr) continue;
        auto* container = as<BlockContainer>(try_finally->finally_block());
        if (container == nullptr) continue;
        Block* entry_point = AsyncAwaitDecompiler::get_body_entry_point(container);
        if (auto* if_inst = entry_point != nullptr ? as<IfInstruction>(entry_point->instructions[0]) : nullptr) {
            ILInstruction* logic_not_arg = nullptr;
            if (if_inst->condition()->match_logic_not(&logic_not_arg) && logic_not_arg->match_ld_loc(skip_finally_bodies_)) {
                context_->step("Remove if (skipFinallyBodies) from try-finally", try_finally);
                // condition will always be true now that we're using 'yield' instructions
                entry_point->instructions.set(0, if_inst->true_inst());
                entry_point->instructions.remove_range(1, entry_point->instructions.count() - 1);
                // find new entryPoint after the old one was modified
                entry_point = AsyncAwaitDecompiler::get_body_entry_point(container);
            }
        }
        const IMethod* finally_method = nullptr;
        // Whether `call` is a call of a finally method of the enumerator: a mono-generated "<>__Finally" method.
        auto is_call_to_mono_finally_method = [&](Call* call) {
            if (call == nullptr) return false;
            if (!(call->arguments().count() == 1 && call->arguments()[0]->match_ld_this())) return false;
            if (!call->method->name().starts_with("<>__Finally")) return false;
            const typesystem::ITypeDefinition* declaring_type_definition = call->method->declaring_type_definition();
            if (declaring_type_definition == nullptr) return false;
            if (declaring_type_definition->metadata_token() != srm::EntityHandle(enumerator_type_)) return false;
            if (&declaring_type_definition->parent_module()->metadata_file()->metadata() != metadata_) return false;
            finally_method = call->method;
            return !call->method->metadata_token().is_nil();
        };
        if (entry_point != nullptr && entry_point->instructions.count() == 2 && is_call_to_mono_finally_method(as<Call>(entry_point->instructions[0])) &&
            entry_point->instructions[1]->match_leave(static_cast<BlockContainer*>(try_finally->finally_block()))) {
            context_->step("Inline " + finally_method->full_name() + " into finally", try_finally);
            ILFunction* finally_function = create_il_ast(srm::MethodDefinitionHandle(finally_method->metadata_token()), *context_);
            try_finally->set_finally_block(finally_function->body());
            std::vector<ILVariable*> variables(finally_function->variables().begin(), finally_function->variables().end());
            finally_function->variables().clear();
            for (ILVariable* v : variables) function->variables().add(v);
        }
    }
    context_->step_end_group(true);
}

void YieldReturnDecompiler::clean_do_finally_bodies(ILFunction* function) {
    if (do_finally_bodies_ == nullptr) return;  // only VB code uses doFinallyBodies
    context_->step_start_group("CleanDoFinallyBodies", function);
    if (do_finally_bodies_->store_instructions().size() != 0 || do_finally_bodies_->address_count() != 0) {
        // misdetected another variable as skipFinallyBodies?
        // Fortunately removing the initial store of 0 is harmless, as we
        // default-initialize the variable on uninit uses
        return;
    }
    for (ILInstruction* d : function->descendants()) {
        auto* try_finally = as<TryFinally>(d);
        if (try_finally == nullptr) continue;
        auto* container = as<BlockContainer>(try_finally->finally_block());
        if (container == nullptr) continue;
        Block* entry_point = AsyncAwaitDecompiler::get_body_entry_point(container);
        if (auto* if_inst = entry_point != nullptr ? as<IfInstruction>(entry_point->instructions[0]) : nullptr) {
            ILInstruction* left = nullptr;
            ILInstruction* right = nullptr;
            if (if_inst->condition()->match_comp_equals(&left, &right) && left->match_ld_loc(do_finally_bodies_) && right->match_ldc_i4(0)) {
                context_->step("Remove if (doFinallyBodies) from try-finally", try_finally);
                // condition will always be false now that we're using 'yield' instructions
                entry_point->instructions.remove_at(0);
            }
        }
    }
    std::vector<LdLoc*> loads = do_finally_bodies_->load_instructions();
    for (LdLoc* load : loads) load->replace_with(with_il_range(make<LdcI4>(1), load));
    context_->step_end_group(true);
}

void YieldReturnDecompiler::clean_finally_state_checks(ILFunction* function) {
    context_->step_start_group("CleanFinallyStateChecks", function);
    for (ILInstruction* d : function->descendants()) {
        auto* try_finally = as<TryFinally>(d);
        if (try_finally == nullptr) continue;
        auto* container = as<BlockContainer>(try_finally->finally_block());
        if (container == nullptr) continue;
        Block* entry_point = AsyncAwaitDecompiler::get_body_entry_point(container);
        if (auto* if_inst = entry_point != nullptr ? as<IfInstruction>(entry_point->instructions[0]) : nullptr) {
            auto* comp = as<Comp>(if_inst->condition());
            ILVariable* variable = nullptr;
            if (comp != nullptr && comp->kind() == ComparisonKind::GreaterThanOrEqual && comp->input_type == StackType::I4 &&
                comp->sign == typesystem::Sign::Signed && comp->left()->match_ld_loc(&variable) &&
                std::find(cached_state_vars_.begin(), cached_state_vars_.end(), variable) != cached_state_vars_.end() && comp->right()->match_ldc_i4(0)) {
                context_->step("Remove if (stateVar >= 0) from try-finally", try_finally);
                // condition will always be false now that we're using 'yield' instructions
                entry_point->instructions.remove_at(0);
            }
        }
    }
    context_->step_end_group(true);
}

// The first method of the type with that name (or that implements an interface method of that name); a nil handle for none.
srm::MethodDefinitionHandle YieldReturnDecompiler::find_method(srm::TypeDefinitionHandle type, const std::string& name) const {
    for (srm::MethodDefinitionHandle m : metadata_->get_type_definition(type).get_methods()) {
        if (is_method(m, name)) return m;
    }
    return {};
}

bool YieldReturnDecompiler::is_method(srm::MethodDefinitionHandle method, const std::string& name) const {
    srm::MethodDefinition method_definition = metadata_->get_method_definition(method);
    if (metadata_->get_string(method_definition.name()) == name) return true;
    for (srm::MethodImplementationHandle impl_handle : get_method_implementations(method, *metadata_)) {
        srm::MethodImplementation impl = metadata_->get_method_implementation(impl_handle);
        switch (impl.method_declaration().kind()) {
            case srm::HandleKind::MethodDefinition: {
                srm::MethodDefinition md = metadata_->get_method_definition(srm::MethodDefinitionHandle(impl.method_declaration()));
                if (metadata_->get_string(md.name()) != name) continue;
                return true;
            }
            case srm::HandleKind::MemberReference: {
                srm::MemberReference mr = metadata_->get_member_reference(srm::MemberReferenceHandle(impl.method_declaration()));
                if (mr.get_kind() != srm::MemberReferenceKind::Method) continue;
                if (metadata_->get_string(mr.name()) != name) continue;
                return true;
            }
            default:
                continue;
        }
    }
    return false;
}

}  // namespace cb::dotnet::decompiler::il::control_flow

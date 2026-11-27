// dotnet/decompiler/il/control_flow/async_await_decompiler.cxx -- see async_await_decompiler.hpp.
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/il/control_flow/async_await_decompiler.hpp"

#include <algorithm>
#include <cassert>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/control_flow/await_in_catch_transform.hpp"
#include "dotnet/decompiler/il/control_flow/await_in_finally_transform.hpp"
#include "dotnet/decompiler/il/control_flow/control_flow_simplification.hpp"
#include "dotnet/decompiler/il/control_flow/runtime_async_exception_rewrite_transform.hpp"
#include "dotnet/decompiler/il/control_flow/runtime_async_manual_await_transform.hpp"
#include "dotnet/decompiler/il/control_flow/yield_return_decompiler.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/copy_propagation.hpp"
#include "dotnet/decompiler/il/transforms/dynamic_call_site_transform.hpp"
#include "dotnet/decompiler/il/transforms/early_expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/il/transforms/remove_dead_variable_init.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/fake_member.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/task_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

using transforms::ILTransformContext;
using typesystem::KnownTypeCode;

namespace {

// The field behind a (possibly specialized) field: upstream's `(IField)field.MemberDefinition`.
const IField* field_definition(const IField* field) { return dynamic_cast<const IField*>(field->member_definition()); }

// A method made up for a dynamic await, kept as long as the ILAst that refers to it.
struct DynamicAwaiterMethod final : ArenaObject {
    explicit DynamicAwaiterMethod(const typesystem::ICompilation& compilation) : method(compilation, typesystem::SymbolKind::Method) {}
    typesystem::FakeMethod method;
};

// The index of a variable (a null variable is upstream's NullReferenceException).
std::optional<int> index_of(const ILVariable* v) {
    if (v == nullptr) throw std::runtime_error("Object reference not set to an instance of an object.");
    return v->index();
}

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

bool AsyncAwaitDecompiler::is_compiler_generated_state_machine(srm::TypeDefinitionHandle type, const srm::MetadataReader& metadata) {
    if (type.is_nil()) return false;
    srm::TypeDefinition td = metadata.get_type_definition(type);
    if (td.get_declaring_type().is_nil()) return false;
    if (!(typesystem::is_compiler_generated(type, metadata) || typesystem::is_compiler_generated(td.get_declaring_type(), metadata))) return false;
    for (auto i : td.get_interface_implementations()) {
        typesystem::FullTypeName tr = get_full_type_name(metadata.get_interface_implementation(i).interface_(), metadata);
        if (!tr.is_nested() && tr.top_level_type_name().namespace_() == "System.Runtime.CompilerServices" &&
            tr.top_level_type_name().name() == "IAsyncStateMachine")
            return true;
    }
    return false;
}

bool AsyncAwaitDecompiler::is_compiler_generated_main_method(const metadata::MetadataFile& module, srm::MethodDefinitionHandle method) {
    const srm::MetadataReader& metadata = module.metadata();
    srm::MethodDefinition definition = metadata.get_method_definition(method);
    // The entry point's token, as the CLI header holds it (0 when there is no header).
    const srm::CorHeader* cor_header = module.cor_header();
    std::uint32_t entry_point_token = cor_header != nullptr ? cor_header->entry_point_token_or_relative_virtual_address : 0;
    bool is_entry_point = (entry_point_token >> 24) == static_cast<std::uint32_t>(srm::HandleKind::MethodDefinition) && (entry_point_token & 0xFFFFFF) == method.row();
    return is_entry_point && metadata.get_string(definition.name()) == "<Main>";
}

void AsyncAwaitDecompiler::run(ILFunction* function, ILTransformContext& context) {
    if (!context.settings().async_await) return;  // abort if async/await decompilation is disabled
    context_ = &context;
    field_to_parameter_map_.clear();
    cached_field_to_parameter_map_.clear();
    await_blocks_.clear();
    await_debug_infos_.clear();
    move_next_leaves_.clear();
    if (!match_task_creation_pattern(function) && !match_async_enumerator_creation_pattern(function)) {
        if (function->is_async()) {
            translate_this_copy_access(function);
            RuntimeAsyncManualAwaitTransform::run(function, context);
            RuntimeAsyncExceptionRewriteTransform::run(function, context);
        }
        return;
    }
    try {
        analyze_move_next();
        validate_catch_block();
        analyze_dispose_async();
    } catch (const SymbolicAnalysisFailedException&) {
        return;
    }

    inline_body_of_move_next(function);
#ifndef NDEBUG
    function->check_invariant(ILPhase::InAsyncAwait);
#endif
    clean_up_body_of_move_next(function);
#ifndef NDEBUG
    function->check_invariant(ILPhase::InAsyncAwait);
#endif

    analyze_state_machine(function);
    detect_await_pattern(function);
    clean_do_finally_bodies(function);

    context.step("Translate fields to local accesses", function);
    YieldReturnDecompiler::translate_fields_to_local_access(function, function, field_to_parameter_map_);
    translate_cached_fields_to_locals();

    finalize_inline_move_next(function);
    if (method_type_ == AsyncMethodType::AsyncEnumerable || method_type_ == AsyncMethodType::AsyncEnumerator) {
        static_cast<BlockContainer*>(function->body())->set_expected_result_type(StackType::Void);
    } else {
        static_cast<BlockContainer*>(function->body())->set_expected_result_type(typesystem::get_stack_type(*underlying_return_type_));
    }

    // Re-run control flow simplification over the newly constructed set of gotos,
    // and inlining because TranslateFieldsToLocalAccess() might have opened up new inlining opportunities.
    function->run_transforms(transforms::early_il_transforms(), context);

    if (!is_visual_basic_state_machine_) {
        context.step_start_group("AwaitInCatchTransform");
        AwaitInCatchTransform::run(function, context);
        context.step_end_group();
        context.step_start_group("AwaitInFinallyTransform");
        AwaitInFinallyTransform::run(function, context);
        context.step_end_group();
    }

    std::stable_sort(await_debug_infos_.begin(), await_debug_infos_.end(),
                     [](const AwaitDebugInfo& a, const AwaitDebugInfo& b) { return a.yield_offset < b.yield_offset; });
    // WAITS: AsyncDebugInfo.Await (the function's async_debug_info models only the catch handler offset)
    function->async_debug_info.catch_handler_offset = catch_handler_offset_;
    function->async_debug_info.awaits_is_default = false;
}

// Runtime-async analog of fieldToParameterMap's `<>4__this` capture: in a struct method, Roslyn lowers `this` references to a single
// ldobj at function entry stored into a local. Match the entry instruction `stloc V_X(ldobj T(ldloc this))` and rewrite every later
// `ldloc V_X` / `ldloca V_X` to go through the `this` parameter again, so AST emission renders accesses as plain `this.field` /
// `field` rather than `<copy>.field`.
bool AsyncAwaitDecompiler::translate_this_copy_access(ILFunction* function) {
    // Only applies to struct methods — that's the only shape where Roslyn copies *this at entry to keep the managed ref off the async
    // resume path.
    if (!(function->method != nullptr && function->method->declaring_type_definition() != nullptr &&
          function->method->declaring_type_definition()->kind() == typesystem::TypeKind::Struct))
        return false;
    ILVariable* this_param = nullptr;
    for (ILVariable* v : function->variables()) {
        if (v->kind() == VariableKind::Parameter && v->index() == -1) {
            this_param = v;
            break;
        }
    }
    if (this_param == nullptr) return false;
    auto* this_ref = dynamic_cast<const typesystem::ByReferenceType*>(this_param->type());
    if (this_ref == nullptr) return false;
    auto* body = as<BlockContainer>(function->body());
    if (body == nullptr || body->blocks.count() == 0) return false;
    Block* entry = body->entry_point();
    if (entry->instructions.count() == 0) return false;
    ILVariable* copy_var = nullptr;
    ILInstruction* copy_value = nullptr;
    if (!entry->instructions[0]->match_st_loc(&copy_var, &copy_value)) return false;
    if (copy_var->kind() != VariableKind::Local) return false;
    if (copy_var->store_instructions().size() != 1) return false;
    auto* ldobj = as<LdObj>(copy_value);
    if (ldobj == nullptr) return false;
    if (!ldobj->target()->match_ld_loc(this_param)) return false;
    if (!typesystem::type_equals(copy_var->type(), ldobj->type()) || !typesystem::type_equals(this_ref->element_type(), ldobj->type())) return false;

    std::vector<LdLoc*> ldlocs;
    for (ILInstruction* d : function->descendants())
        if (auto* ldloc = as<LdLoc>(d)) ldlocs.push_back(ldloc);
    for (LdLoc* ldloc : ldlocs) {
        if (ldloc->variable() != copy_var) continue;
        ldloc->replace_with(with_il_range(make<LdObj>(make<LdLoc>(this_param), ldobj->type()), ldloc));
    }
    std::vector<LdLoca*> ldlocas;
    for (ILInstruction* d : function->descendants())
        if (auto* ldloca = as<LdLoca>(d)) ldlocas.push_back(ldloca);
    for (LdLoca* ldloca : ldlocas) {
        if (ldloca->variable() != copy_var) continue;
        ldloca->replace_with(with_il_range(make<LdLoc>(this_param), ldloca));
    }

    entry->instructions.remove_at(0);
    return true;
}

void AsyncAwaitDecompiler::clean_up_body_of_move_next(ILFunction* function) {
    context_->step_start_group("CleanUpBodyOfMoveNext", function);
    // Copy-propagate stack slots holding an 'ldloca':
    {
        std::vector<StLoc*> stlocs;
        for (ILInstruction* d : function->descendants()) {
            auto* s = as<StLoc>(d);
            if (s != nullptr && s->variable()->kind() == VariableKind::StackSlot && s->variable()->is_single_definition() && is<LdLoca>(s->value()))
                stlocs.push_back(s);
        }
        for (StLoc* stloc : stlocs) transforms::CopyPropagation::propagate(stloc, *context_);
    }

    // Simplify stobj(ldloca) -> stloc
    for (ILInstruction* d : function->descendants()) {
        if (auto* stobj = as<StObj>(d)) transforms::EarlyExpressionTransforms::st_obj_to_st_loc(stobj, *context_);
    }

    if (is_visual_basic_state_machine_) {
        // Visual Basic state machines often contain stack slots for ldflda instruction for the builder field.
        // This messes up the matching code in AnalyzeAwaitBlock() and causes it to fail.
        // Propagating these stack stores makes the matching code work without the need for a lot of
        // additional matching code to handle these stack slots.
        {
            std::vector<StLoc*> stlocs;
            for (ILInstruction* d : function->descendants()) {
                auto* s = as<StLoc>(d);
                if (s == nullptr || s->variable()->kind() != VariableKind::StackSlot || !s->variable()->is_single_definition()) continue;
                ILInstruction* target = nullptr;
                const IField* field = nullptr;
                if (s->value()->match_ld_flda(&target, &field) && typesystem::member_equals(field, builder_field_) && target->match_ld_this()) stlocs.push_back(s);
            }
            for (StLoc* stloc : stlocs) transforms::CopyPropagation::propagate(stloc, *context_);
        }

        // Remove lone 'ldc.i4', present in older Roslyn VB compiler output
        for (ILInstruction* d : function->descendants()) {
            if (auto* block = as<Block>(d)) block->instructions.remove_all([](ILInstruction* inst) { return inst->op_code == OpCode::LdcI4; });
        }
    }

    // Copy-propagate temporaries holding a copy of 'this'.
    {
        std::vector<StLoc*> stlocs;
        for (ILInstruction* d : function->descendants()) {
            auto* s = as<StLoc>(d);
            if (s != nullptr && s->variable()->is_single_definition() && s->value()->match_ld_this()) stlocs.push_back(s);
        }
        for (StLoc* stloc : stlocs) transforms::CopyPropagation::propagate(stloc, *context_);
    }
    transforms::RemoveDeadVariableInit().run(function, *context_);
    for (ILInstruction* d : function->descendants()) {
        auto* block = as<Block>(d);
        if (block == nullptr) continue;
        // Run inlining, but don't remove dead variables (they might get revived by TranslateFieldsToLocalAccess)
        transforms::ILInlining::inline_all_in_block(function, block, transforms::InliningOptions::None, *context_);
        if (is_async_enumerator()) {
            // Remove lone 'ldc.i4', those are sometimes left over after C# compiler
            // optimizes out stores to the state variable.
            block->instructions.remove_all([](ILInstruction* inst) { return inst->op_code == OpCode::LdcI4; });
        }
    }
    context_->step_end_group();
}

// ---- MatchTaskCreationPattern -------------------------------------------------------------------------------------------------------

bool AsyncAwaitDecompiler::match_task_creation_pattern(ILFunction* function) {
    auto* block_container = as<BlockContainer>(function->body());
    if (block_container == nullptr) return false;
    if (block_container->blocks.count() != 1) return false;
    InstructionCollection<ILInstruction>& body = block_container->entry_point()->instructions;
    if (body.count() < 4) return false;
    /* Example:
        V_0 is an instance of the compiler-generated struct/class,
        V_1 is an instance of the builder struct/class
        Block IL_0000 (incoming: 1)  {
            ...
            stobj AsyncVoidMethodBuilder(ldflda [Field Async+<AwaitYield>d__3.<>t__builder](ldloca V_0), call Create())
            stobj System.Int32(ldflda [Field Async+<AwaitYield>d__3.<>1__state](ldloca V_0), ldc.i4 -1)
            stloc V_1(ldobj System.Runtime.CompilerServices.AsyncVoidMethodBuilder(ldflda [Field Async+<AwaitYield>d__3.<>t__builder](ldloc V_0)))
            call Start(ldloca V_1, ldloca V_0)
            leave IL_0000 (or ret for non-void async methods)
        }
        With custom task types, it's possible that the builder is a reference type.
        In that case, the variable V_1 may be inlined.
    */

    // Check the second-to-last instruction (the start call) first, as we can get the most information from that
    int pos = body.count() - 2;
    auto* start_call = as<CallInstruction>(body[pos]);
    if (start_call == nullptr) return false;
    if (start_call->method->name() != "Start") return false;
    task_type_ = function->method->return_type();
    builder_type_ = start_call->method->declaring_type();
    typesystem::FullTypeName builder_type_name;
    if (const typesystem::ITypeDefinitionOrUnknown* builder_type_def = builder_type_ != nullptr ? builder_type_->get_definition_or_unknown() : nullptr) {
        builder_type_name = builder_type_def->full_type_name();
    } else {
        return false;
    }
    typesystem::FullTypeName builder_type_name_from_task;
    if (typesystem::is_known_type(task_type_, KnownTypeCode::Void)) {
        method_type_ = AsyncMethodType::Void;
        underlying_return_type_ = task_type_;
        if (builder_type_name != typesystem::FullTypeName(typesystem::TopLevelTypeName("System.Runtime.CompilerServices", "AsyncVoidMethodBuilder"))) return false;
    } else if (typesystem::task_type::is_non_generic_task_type(task_type_, &builder_type_name_from_task)) {
        method_type_ = AsyncMethodType::Task;
        underlying_return_type_ = context_->type_system().find_type(KnownTypeCode::Void);
        if (builder_type_name_from_task != builder_type_name) return false;
    } else if (typesystem::task_type::is_generic_task_type(task_type_, &builder_type_name_from_task)) {
        method_type_ = AsyncMethodType::TaskOfT;
        if (typesystem::is_known_type(task_type_, KnownTypeCode::TaskOfT))
            underlying_return_type_ = typesystem::task_type::unpack_task(context_->type_system(), task_type_);
        else
            underlying_return_type_ = start_call->method->declaring_type()->type_arguments()[0];
        if (builder_type_name_from_task != builder_type_name) return false;
    } else {
        return false;
    }
    if (start_call->arguments().count() != 2) return false;
    ILInstruction* load_builder_expr = start_call->arguments()[0];
    ILVariable* state_machine_var = nullptr;
    if (!start_call->arguments()[1]->match_ld_loca(&state_machine_var)) return false;
    state_machine_type_ = state_machine_var->type()->get_definition();
    if (state_machine_type_ == nullptr) return false;
    pos--;

    ILVariable* builder_var = nullptr;
    if (load_builder_expr->match_ld_loc_ref(&builder_var)) {
        // Check third-to-last instruction (copy of builder)
        // stloc builder(ldfld StateMachine::<>t__builder(ldloc stateMachine))
        if (!body[pos]->match_st_loc(builder_var, &load_builder_expr)) return false;
        pos--;
    }
    ILInstruction* load_state_machine_for_builder_expr = nullptr;
    if (load_builder_expr->match_ld_fld(&load_state_machine_for_builder_expr, &builder_field_)) {
        // OK, calling Start on copy of stateMachine.<>t__builder
    } else if (load_builder_expr->match_ld_flda(&load_state_machine_for_builder_expr, &builder_field_)) {
        // OK, Roslyn 3.6 started directly calling Start without making a copy
    } else {
        return false;
    }
    builder_field_ = field_definition(builder_field_);
    if (!(load_state_machine_for_builder_expr->match_ld_loc_ref(state_machine_var) || load_state_machine_for_builder_expr->match_ld_loc(state_machine_var)))
        return false;

    // Check the last instruction (ret)
    if (method_type_ == AsyncMethodType::Void) {
        if (!body.last()->match_leave(block_container)) return false;
    } else {
        // ret(call(AsyncTaskMethodBuilder::get_Task, ldflda(StateMachine::<>t__builder, ldloca(stateMachine))))
        ILInstruction* return_value = nullptr;
        if (!body.last()->match_return(&return_value)) return false;
        InstructionCollection<ILInstruction>* get_task_args = match_call(return_value, "get_Task");
        if (get_task_args == nullptr || get_task_args->count() != 1) return false;
        ILInstruction* target = nullptr;
        const IField* builder_field2 = nullptr;
        if (builder_type_->is_reference_type() == true) {
            if (!(*get_task_args)[0]->match_ld_fld(&target, &builder_field2)) return false;
        } else {
            if (!(*get_task_args)[0]->match_ld_flda(&target, &builder_field2)) return false;
        }
        if (builder_field2->member_definition() != builder_field_) return false;
        if (!(target->match_ld_loc(state_machine_var) || target->match_ld_loca(state_machine_var))) return false;
    }

    if (is_potential_visual_basic_state_machine_initialiation(body[0], state_machine_var)) {
        // Visual Basic compiler uses a different order of field assignements
        if (match_visual_basic_state_machine_field_assignements(body, state_machine_var)) {
            is_visual_basic_state_machine_ = true;
            return true;
        }
        // If the Visual Basic matching code failed, reset the map in case it
        // was partially populated and try matching the C# state machine initialization.
        field_to_parameter_map_.clear();
    }

    // Check the last field assignment - this should be the state field
    // stfld <>1__state(ldloca stateField, ldc.i4 -1)
    ILInstruction* initial_state_expr = nullptr;
    if (!match_state_machine_st_fld(body[pos], state_machine_var, &state_field_, &initial_state_expr)) return false;
    if (!initial_state_expr->match_ldc_i4(&initial_state_)) return false;
    if (initial_state_ != -1) return false;

    int stop_pos = pos;
    pos = 0;
    if (state_machine_type_->kind() == typesystem::TypeKind::Class) {
        // If state machine is a class, the first instruction creates an instance:
        // stloc stateMachine(newobj StateMachine.ctor())
        ILInstruction* init = nullptr;
        if (!body[pos]->match_st_loc(state_machine_var, &init)) return false;
        auto* newobj = as<NewObj>(init);
        if (!(newobj != nullptr && newobj->arguments().count() == 0 && newobj->method->declaring_type_definition() == state_machine_type_)) return false;
        pos++;
    }
    bool builder_field_is_initialized = false;
    for (; pos < stop_pos; pos++) {
        // stfld StateMachine.field(ldloca stateMachine, ldvar(param))
        const IField* field = nullptr;
        ILInstruction* field_init = nullptr;
        if (!match_state_machine_st_fld(body[pos], state_machine_var, &field, &field_init)) return false;
        ILVariable* v = nullptr;
        if (field == builder_field_) {
            // stfld StateMachine.builder(ldloca stateMachine, call Create())
            auto* create = as<Call>(field_init);
            if (!(create != nullptr && create->method->name() == "Create" && create->arguments().count() == 0)) return false;
            builder_field_is_initialized = true;
        } else if (field_init->match_ld_loc(&v) && v->kind() == VariableKind::Parameter) {
            // OK, copies parameter into state machine
            field_to_parameter_map_.set(field, v);
        } else if (auto* ldobj = as<LdObj>(field_init); ldobj != nullptr && ldobj->target()->match_ld_this()) {
            // stfld <>4__this(ldloc stateMachine, ldobj AsyncInStruct(ldloc this))
            field_to_parameter_map_.set(field, static_cast<LdLoc*>(ldobj->target())->variable());
        } else {
            return false;
        }
    }

    return builder_field_is_initialized;
}

bool AsyncAwaitDecompiler::is_potential_visual_basic_state_machine_initialiation(ILInstruction* inst, ILVariable* state_machine_var) {
    // stobj VB$StateMachine(ldloca stateMachine, default.value VB$StateMachine)
    // Used by VBC (Debug and Release) and Roslyn VB (Release) builds
    ILInstruction* target = nullptr;
    ILInstruction* val = nullptr;
    const IType* type1 = nullptr;
    const IType* type2 = nullptr;
    if (inst->match_st_obj(&target, &val, &type1) && target->match_ld_loca(state_machine_var) && val->match_default_value(&type2) &&
        typesystem::type_equals(type1, type2))
        return true;
    // stloc stateMachine(newobj VB$StateMachine..ctor())
    // Used by Roslyn VB (Debug) builds
    ILInstruction* init = nullptr;
    if (inst->match_st_loc(state_machine_var, &init)) {
        auto* new_obj = as<NewObj>(init);
        if (new_obj != nullptr && new_obj->arguments().count() == 0 &&
            typesystem::type_equals(state_machine_type_, new_obj->method->declaring_type_definition()))
            return true;
    }
    return false;
}

bool AsyncAwaitDecompiler::match_visual_basic_state_machine_field_assignements(InstructionCollection<ILInstruction>& body, ILVariable* state_machine_var) {
    // stfld $State(ldloca stateField, ldc.i4 -1)
    ILInstruction* initial_state_expr = nullptr;
    if (!match_state_machine_st_fld(body[body.count() - 4], state_machine_var, &state_field_, &initial_state_expr)) return false;
    if (!initial_state_expr->match_ldc_i4(&initial_state_)) return false;
    if (initial_state_ != -1) return false;

    // stfld $Builder(ldloca stateMachine, call Create())
    const IField* builder_field2 = nullptr;
    ILInstruction* field_init = nullptr;
    if (!match_state_machine_st_fld(body[body.count() - 3], state_machine_var, &builder_field2, &field_init)) return false;
    if (!typesystem::member_equals(builder_field_, builder_field2)) return false;
    auto* create = as<Call>(field_init);
    if (!(create != nullptr && create->method->name() == "Create" && create->arguments().count() == 0)) return false;

    for (int i = 1; i < body.count() - 4; i++) {
        const IField* field = nullptr;
        if (!match_state_machine_st_fld(body[i], state_machine_var, &field, &field_init)) return false;
        // Legacy Visual Basic compiler can emit a call to RuntimeHelpers.GetObjectValue here
        if (auto* call = as<Call>(field_init); call != nullptr && call->arguments().count() == 1 && call->method->is_static() && call->method->name() == "GetObjectValue")
            field_init = call->arguments()[0];
        ILVariable* v = nullptr;
        if (field_init->match_ld_loc(&v) && v->kind() == VariableKind::Parameter) {
            // OK, copies parameter into state machine
            field_to_parameter_map_.set(field, v);
        } else if (auto* ldobj = as<LdObj>(field_init); ldobj != nullptr && ldobj->target()->match_ld_this()) {
            // stfld <>4__this(ldloc stateMachine, ldobj AsyncInStruct(ldloc this))
            field_to_parameter_map_.set(field, static_cast<LdLoc*>(ldobj->target())->variable());
        } else
            return false;
    }
    return true;
}

InstructionCollection<ILInstruction>* AsyncAwaitDecompiler::match_call(ILInstruction* inst, const std::string& name) {
    auto* call = as<CallInstruction>(inst);
    if (call != nullptr && (call->op_code == OpCode::Call || call->op_code == OpCode::CallVirt) && call->method->name() == name && !call->method->is_static()) {
        InstructionCollection<ILInstruction>* args = &call->arguments();
        return args->count() > 0 ? args : nullptr;
    }
    return nullptr;
}

bool AsyncAwaitDecompiler::match_state_machine_st_fld(ILInstruction* stfld, ILVariable* state_machine_var, const IField** field, ILInstruction** value) {
    ILInstruction* target = nullptr;
    if (!stfld->match_st_fld(&target, field, value)) return false;
    *field = *field != nullptr ? field_definition(*field) : nullptr;
    return *field != nullptr && target->match_ld_loc_ref(state_machine_var);
}

// ---- MatchAsyncEnumeratorCreationPattern ---------------------------------------------------------------------------------------------

bool AsyncAwaitDecompiler::match_async_enumerator_creation_pattern(ILFunction* function) {
    if (!context_->settings().async_enumerator) return false;
    task_type_ = function->return_type;
    if (typesystem::is_known_type(task_type_, KnownTypeCode::IAsyncEnumeratorOfT)) {
        method_type_ = AsyncMethodType::AsyncEnumerator;
    } else if (typesystem::is_known_type(task_type_, KnownTypeCode::IAsyncEnumerableOfT)) {
        method_type_ = AsyncMethodType::AsyncEnumerable;
    } else {
        return false;
    }
    if (task_type_->type_arguments().size() != 1) throw std::logic_error("Sequence contains more than one element or no element");
    underlying_return_type_ = task_type_->type_arguments()[0];
    auto* block_container = as<BlockContainer>(function->body());
    if (block_container == nullptr) return false;
    if (block_container->blocks.count() != 1) return false;
    Block* body = block_container->entry_point();
    if (body->instructions.count() == 1) {
        // No parameters passed to enumerator (not even 'this'):
        // ret(newobj(...))
        ILInstruction* new_obj = nullptr;
        if (!body->instructions[0]->match_return(&new_obj)) return false;
        if (match_enumerator_creation_new_obj(new_obj, *context_, &initial_state_, &state_machine_type_)) {
            // HACK: the normal async/await logic expects 'initialState' to be the 'in progress' state
            initial_state_ = -1;
            try {
                analyze_enumerator_ctor(static_cast<NewObj*>(new_obj)->method, *context_, &builder_field_, &builder_type_, &state_field_);
            } catch (const SymbolicAnalysisFailedException&) {
                return false;
            }
            return true;
        } else {
            return false;
        }
    } else {
        // stloc v(newobj<CountUpSlowly> d__0..ctor(ldc.i4 - 2))
        // stfld <>4__this(ldloc v, ldloc this)
        // stfld <>3__otherParam(ldloc v, ldloc otherParam)
        // leave IL_0000(ldloc v)
        int pos = 0;
        ILVariable* v = nullptr;
        ILInstruction* new_obj = nullptr;
        if (!body->instructions[pos]->match_st_loc(&v, &new_obj)) return false;
        if (!match_enumerator_creation_new_obj(new_obj, *context_, &initial_state_, &state_machine_type_)) return false;
        pos++;

        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        while (match_state_machine_st_fld(body->instructions[pos], v, &field, &value)) {
            ILVariable* p = nullptr;
            if (value->match_ld_loc(&p) && p->kind() == VariableKind::Parameter) {
                field_to_parameter_map_.set(field, p);
            } else if (auto* ldobj = as<LdObj>(value); ldobj != nullptr && ldobj->target()->match_ld_this()) {
                field_to_parameter_map_.set(field, static_cast<LdLoc*>(ldobj->target())->variable());
            } else {
                return false;
            }
            pos++;
        }
        ILInstruction* return_value = nullptr;
        if (!body->instructions[pos]->match_return(&return_value)) return false;
        if (!return_value->match_ld_loc(v)) return false;

        // HACK: the normal async/await logic expects 'initialState' to be the 'in progress' state
        initial_state_ = -1;
        try {
            analyze_enumerator_ctor(static_cast<NewObj*>(new_obj)->method, *context_, &builder_field_, &builder_type_, &state_field_);
            if (method_type_ == AsyncMethodType::AsyncEnumerable) {
                resolve_ienumerable_ienumerator_field_mapping();
            }
        } catch (const SymbolicAnalysisFailedException&) {
            return false;
        }
        return true;
    }
}

bool AsyncAwaitDecompiler::match_enumerator_creation_new_obj(ILInstruction* inst, ILTransformContext& context, int* initial_state,
                                                             const typesystem::ITypeDefinition** state_machine_type) {
    *initial_state = 0;
    *state_machine_type = nullptr;
    // newobj(CurrentType/...::.ctor, ldc.i4(-2))
    auto* new_obj = as<NewObj>(inst);
    if (new_obj == nullptr) return false;
    if (new_obj->arguments().count() != 1) return false;
    if (!new_obj->arguments()[0]->match_ldc_i4(initial_state)) return false;
    *state_machine_type = new_obj->method->declaring_type_definition();
    if (*state_machine_type == nullptr) return false;
    if ((*state_machine_type)->declaring_type_definition() != context.function()->method->declaring_type_definition()) return false;
    return is_compiler_generator_async_enumerator(srm::TypeDefinitionHandle((*state_machine_type)->metadata_token()),
                                                  context.type_system().main_module().metadata_file()->metadata());
}

bool AsyncAwaitDecompiler::is_compiler_generator_async_enumerator(srm::TypeDefinitionHandle type, const srm::MetadataReader& metadata) {
    if (type.is_nil() || !typesystem::is_compiler_generated_or_is_in_compiler_generated_class(type, metadata)) return false;
    srm::TypeDefinition td = metadata.get_type_definition(type);
    if (td.get_declaring_type().is_nil()) return false;
    for (auto i : td.get_interface_implementations()) {
        typesystem::FullTypeName tr = get_full_type_name(metadata.get_interface_implementation(i).interface_(), metadata);
        if (!tr.is_nested() && tr.top_level_type_name().namespace_() == "System.Collections.Generic" && tr.top_level_type_name().name() == "IAsyncEnumerator" &&
            tr.top_level_type_name().type_parameter_count() == 1)
            return true;
    }
    return false;
}

void AsyncAwaitDecompiler::analyze_enumerator_ctor(const IMethod* ctor, ILTransformContext& context, const IField** builder_field, const IType** builder_type,
                                                   const IField** state_field) {
    *builder_field = nullptr;
    *state_field = nullptr;

    srm::MethodDefinitionHandle ctor_handle(ctor->metadata_token());
    Block* body = YieldReturnDecompiler::single_block(YieldReturnDecompiler::create_il_ast(ctor_handle, context)->body());
    if (body == nullptr) throw SymbolicAnalysisFailedException("Missing enumeratorCtor.Body");
    // Block IL_0000 (incoming: 1) {
    //   call Object..ctor(ldloc this)
    // 	 stfld <>1__state(ldloc this, ldloc <>1__state)
    // 	 stfld <>t__builder(ldloc this, call Create())
    // 	 leave IL_0000 (nop)
    // }
    for (ILInstruction* inst : body->instructions) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        ILVariable* arg = nullptr;
        if (inst->match_st_fld(&target, &field, &value) && target->match_ld_this() && value->match_ld_loc(&arg) && arg->kind() == VariableKind::Parameter &&
            arg->index() == 0) {
            *state_field = field_definition(field);
        }
        if (inst->match_st_fld(&target, &field, &value) && target->match_ld_this()) {
            auto* call = as<Call>(value);
            if (call != nullptr && call->method->name() == "Create") *builder_field = field_definition(field);
        }
    }
    if (*state_field == nullptr || *builder_field == nullptr) throw SymbolicAnalysisFailedException();

    *builder_type = (*builder_field)->type();
    if (*builder_type == nullptr) throw SymbolicAnalysisFailedException();
}

void AsyncAwaitDecompiler::resolve_ienumerable_ienumerator_field_mapping() {
    const IMethod* get_async_enumerator = nullptr;
    for (const IMethod* m : state_machine_type_->methods()) {
        if (m->name().ends_with(".GetAsyncEnumerator")) {
            get_async_enumerator = m;
            break;
        }
    }
    if (get_async_enumerator == nullptr) throw SymbolicAnalysisFailedException();
    YieldReturnDecompiler::resolve_ienumerable_ienumerator_field_mapping(srm::MethodDefinitionHandle(get_async_enumerator->metadata_token()), *context_,
                                                                         field_to_parameter_map_);
}

// ---- AnalyzeMoveNext -----------------------------------------------------------------------------------------------------------------

// First peek into MoveNext(); analyzes everything outside the big try-catch.
void AsyncAwaitDecompiler::analyze_move_next() {
    if (state_machine_type_->metadata_token().is_nil()) throw SymbolicAnalysisFailedException();
    const srm::MetadataReader& metadata = context_->pe_file().metadata();
    srm::MethodDefinitionHandle move_next_method;
    for (srm::MethodDefinitionHandle f : metadata.get_type_definition(srm::TypeDefinitionHandle(state_machine_type_->metadata_token())).get_methods()) {
        if (metadata.get_string(metadata.get_method_definition(f).name()) == "MoveNext") {
            move_next_method = f;
            break;
        }
    }
    move_next_function_ = YieldReturnDecompiler::create_il_ast(move_next_method, *context_);
    auto* block_container = as<BlockContainer>(move_next_function_->body());
    if (block_container == nullptr) throw SymbolicAnalysisFailedException();
    if (block_container->entry_point()->incoming_edge_count() != 1) throw SymbolicAnalysisFailedException();
    std::vector<char> blocks_analyzed(static_cast<std::size_t>(block_container->blocks.count()), 0);
    cached_state_var_ = nullptr;
    int pos = 0;
    // Visual Basic state machines initialize doFinallyBodies at the start of MoveNext()
    // stloc doFinallyBodies(ldc.i4 1)
    ILVariable* v = nullptr;
    ILInstruction* ldci4 = nullptr;
    if (is_visual_basic_state_machine_ && block_container->entry_point()->instructions[pos]->match_st_loc(&v, &ldci4) && ldci4->match_ldc_i4(1)) {
        do_finally_bodies_ = v;
        pos++;
    }
    while (auto* stloc = as<StLoc>(block_container->entry_point()->instructions[pos])) {
        // stloc V_1(ldfld <>4__this(ldloc this))
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (!stloc->value()->match_ld_fld(&target, &field)) throw SymbolicAnalysisFailedException();
        if (!target->match_ld_this()) throw SymbolicAnalysisFailedException();
        if (field->member_definition() == state_field_ && cached_state_var_ == nullptr) {
            // stloc(cachedState, ldfld(valuetype StateMachineStruct::<>1__state, ldloc(this)))
            cached_state_var_ = stloc->variable();
        } else if (ILVariable** param = field_to_parameter_map_.find(field_definition(field))) {
            if (!stloc->variable()->is_single_definition()) throw SymbolicAnalysisFailedException();
            cached_field_to_parameter_map_.set(stloc->variable(), *param);
        } else {
            throw SymbolicAnalysisFailedException();
        }
        pos++;
    }
    main_try_catch_ = as<TryCatch>(block_container->entry_point()->instructions[pos]);
    if (main_try_catch_ == nullptr) throw SymbolicAnalysisFailedException();
    // CatchHandler will be validated in ValidateCatchBlock()

    // stloc doFinallyBodies(ldc.i4 1)
    if (auto* init_do_finally_bodies = as<StLoc>(static_cast<BlockContainer*>(main_try_catch_->try_block())->entry_point()->instructions[0]);
        init_do_finally_bodies != nullptr && init_do_finally_bodies->variable()->kind() == VariableKind::Local &&
        typesystem::is_known_type(init_do_finally_bodies->variable()->type(), KnownTypeCode::Boolean) && init_do_finally_bodies->value()->match_ldc_i4(1)) {
        do_finally_bodies_ = init_do_finally_bodies->variable();
    }

    assert(block_container->blocks[0] == block_container->entry_point());  // already checked this block
    blocks_analyzed[0] = 1;
    pos = 1;
    if (match_yield_block(block_container, pos)) {
        set_result_yield_block_ = block_container->blocks[pos];
        blocks_analyzed[static_cast<std::size_t>(pos)] = 1;
        pos++;
    } else {
        set_result_yield_block_ = nullptr;
    }

    set_result_return_block_ = check_set_result_return_block(block_container, pos, blocks_analyzed);

    if (!std::all_of(blocks_analyzed.begin(), blocks_analyzed.end(), [](char b) { return b != 0; })) throw SymbolicAnalysisFailedException("too many blocks");
}

bool AsyncAwaitDecompiler::match_yield_block(BlockContainer* block_container, int pos) {
    if (!is_async_enumerator()) return false;
    Block* block = block_container->blocks.element_at_or_default(pos);
    if (block == nullptr) return false;
    // call SetResult(ldflda <>v__promiseOfValueOrEnd(ldloc this), ldc.i4 1)
    // leave IL_0000(nop)
    if (block->instructions.count() != 2) return false;
    InstructionCollection<ILInstruction>* args = match_call(block->instructions[0], "SetResult");
    if (args == nullptr) return false;
    if (args->count() != 2) return false;
    if (!is_builder_or_promise_field_on_this((*args)[0])) return false;
    if (!(*args)[1]->match_ldc_i4(1)) return false;
    return block->instructions[1]->match_leave(block_container);
}

Block* AsyncAwaitDecompiler::check_set_result_return_block(BlockContainer* block_container, int set_result_return_block_index, std::vector<char>& blocks_analyzed) {
    if (set_result_return_block_index >= block_container->blocks.count()) {
        // This block can be absent if the function never exits normally,
        // but always throws an exception/loops infinitely.
        result_var_ = nullptr;
        final_state_known_ = false;  // final state will be detected in ValidateCatchBlock() instead
        return nullptr;
    }

    Block* block = block_container->blocks[set_result_return_block_index];

    int pos = 0;
    // [vb-only] stloc S_10(ldloc this)
    if (auto* stloc_this_cache = as<StLoc>(block->instructions[pos]);
        stloc_this_cache != nullptr && stloc_this_cache->value()->match_ld_this() && stloc_this_cache->variable()->kind() == VariableKind::StackSlot) {
        pos++;
    }
    // [vb-only] stloc S_11(ldc.i4 -2)
    ILVariable* final_state_slot = nullptr;
    std::optional<int> final_state_slot_value;
    if (auto* stloc_final_state = as<StLoc>(block->instructions[pos]);
        stloc_final_state != nullptr && is<LdcI4>(stloc_final_state->value()) && stloc_final_state->variable()->kind() == VariableKind::StackSlot) {
        final_state_slot = stloc_final_state->variable();
        final_state_slot_value = static_cast<LdcI4*>(stloc_final_state->value())->value;
        pos++;
    }
    // [vb-only] stloc cachedStateVar(ldloc S_11)
    if (auto* stloc_cached_state = as<StLoc>(block->instructions[pos]);
        stloc_cached_state != nullptr && stloc_cached_state->variable()->kind() == VariableKind::Local &&
        stloc_cached_state->variable()->index() == (cached_state_var_ != nullptr ? cached_state_var_->index() : std::nullopt) &&
        stloc_cached_state->value()->match_ld_loc(final_state_slot)) {
        pos++;
    }

    // stfld <>1__state(ldloc this, ldc.i4 -2)
    if (!match_state_assignment(block->instructions[pos], &final_state_)) throw SymbolicAnalysisFailedException();
    if (final_state_slot_value.has_value() && final_state_ != *final_state_slot_value) throw SymbolicAnalysisFailedException();
    final_state_known_ = true;
    pos++;

    // [optional] stfld <>u__N(ldloc this, ldnull)
    // The hoisted-local cleanup may appear either before or after the combined-tokens disposal
    // (the latter is present for async iterators with an [EnumeratorCancellation] token).
    match_hoisted_local_cleanup(block, pos);

    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (pos + 2 == block->instructions.count() && block->match_if_at_end_of_block(&condition, &true_inst, &false_inst)) {
        Block* set_result_and_exit_block = nullptr;
        if (match_dispose_combined_tokens(block_container, condition, true_inst, false_inst, blocks_analyzed, &set_result_and_exit_block)) {
            blocks_analyzed[static_cast<std::size_t>(block->child_index())] = 1;
            block = set_result_and_exit_block;
            pos = 0;
        } else {
            throw SymbolicAnalysisFailedException();
        }
    }

    // [optional] stfld <>u__N(ldloc this, ldnull)
    match_hoisted_local_cleanup(block, pos);
    check_set_result_and_exit(block_container, block, pos);
    blocks_analyzed[static_cast<std::size_t>(block->child_index())] = 1;
    return block_container->blocks[set_result_return_block_index];
}

bool AsyncAwaitDecompiler::match_dispose_combined_tokens(BlockContainer* block_container, ILInstruction* condition, ILInstruction* true_inst,
                                                         ILInstruction* false_inst, std::vector<char>& blocks_analyzed, Block** set_result_and_exit_block) {
    *set_result_and_exit_block = nullptr;
    // 	...
    // 	if (comp.o(ldfld <>x__combinedTokens(ldloc this) == ldnull)) br setResultAndExit
    // 	br disposeCombinedTokens
    // }
    //
    // Block disposeCombinedTokens (incoming: 1) {
    // 	callvirt Dispose(ldfld <>x__combinedTokens(ldloc this))
    // 	stfld <>x__combinedTokens(ldloc this, ldnull)
    // 	br setResultAndExit
    // }
    ILInstruction* tested_inst = nullptr;
    if (!condition->match_comp_equals_null(&tested_inst)) return false;
    ILInstruction* target = nullptr;
    const IField* cts_field = nullptr;
    if (!tested_inst->match_ld_fld(&target, &cts_field)) return false;
    if (!target->match_ld_this()) return false;
    {
        auto* cts_type = dynamic_cast<const typesystem::ITypeDefinition*>(cts_field->type());
        if (cts_type == nullptr) return false;
        typesystem::FullTypeName cts_name = cts_type->full_type_name();
        if (!(!cts_name.is_nested() && cts_name.top_level_type_name().name() == "CancellationTokenSource" &&
              cts_name.top_level_type_name().namespace_() == "System.Threading"))
            return false;
    }
    Block* disposed_combined_tokens_block = nullptr;
    if (!(true_inst->match_branch(set_result_and_exit_block) && false_inst->match_branch(&disposed_combined_tokens_block))) return false;
    if (!((*set_result_and_exit_block)->parent() == block_container && disposed_combined_tokens_block->parent() == block_container)) return false;

    Block* block = disposed_combined_tokens_block;
    int pos = 0;
    // callvirt Dispose(ldfld <>x__combinedTokens(ldloc this))
    auto* dispose_call = as<CallVirt>(block->instructions[pos]);
    if (!(dispose_call != nullptr && dispose_call->method->name() == "Dispose")) return false;
    if (dispose_call->arguments().count() != 1) return false;
    ILInstruction* target2 = nullptr;
    const IField* cts_field2 = nullptr;
    if (!dispose_call->arguments()[0]->match_ld_fld(&target2, &cts_field2)) return false;
    if (!(target2->match_ld_this() && typesystem::member_equals(cts_field2, cts_field))) return false;
    pos++;
    // stfld <>x__combinedTokens(ldloc this, ldnull)
    ILInstruction* target3 = nullptr;
    const IField* cts_field3 = nullptr;
    ILInstruction* stored_value = nullptr;
    if (!block->instructions[pos]->match_st_fld(&target3, &cts_field3, &stored_value)) return false;
    if (!(target3->match_ld_this() && typesystem::member_equals(cts_field3, cts_field))) return false;
    if (!stored_value->match_ld_null()) return false;
    pos++;
    // br setResultAndExit
    if (!block->instructions[pos]->match_branch(*set_result_and_exit_block)) return false;
    blocks_analyzed[static_cast<std::size_t>(block->child_index())] = 1;

    return true;
}

void AsyncAwaitDecompiler::match_hoisted_local_cleanup(Block* block, int& pos) {
    ILInstruction* target = nullptr;
    const IField* unused_field = nullptr;
    ILInstruction* value = nullptr;
    while (block->instructions[pos]->match_st_fld(&target, &unused_field, &value)) {
        // https://github.com/dotnet/roslyn/pull/39735 hoisted local cleanup
        if (!target->match_ld_this()) throw SymbolicAnalysisFailedException();
        if (!value->match_default_or_null_or_zero()) throw SymbolicAnalysisFailedException();
        pos++;
    }
}

void AsyncAwaitDecompiler::check_set_result_and_exit(BlockContainer* block_container, Block* block, int& pos) {
    // [optional] call Complete(ldflda <>t__builder(ldloc this)) (Roslyn >=3.9)
    // call SetResult(ldflda <>t__builder(ldloc this), ldloc result)
    // [optional] call Complete(ldflda <>t__builder(ldloc this))
    // leave IL_0000
    match_complete_call(block, pos);
    InstructionCollection<ILInstruction>* args = match_call(block->instructions[pos], "SetResult");
    if (args == nullptr) throw SymbolicAnalysisFailedException();
    if (!is_builder_or_promise_field_on_this((*args)[0])) throw SymbolicAnalysisFailedException();
    switch (method_type_) {
        case AsyncMethodType::TaskOfT:
            if (args->count() != 2) throw SymbolicAnalysisFailedException();
            if (!(*args)[1]->match_ld_loc(&result_var_)) throw SymbolicAnalysisFailedException();
            break;
        case AsyncMethodType::Task:
        case AsyncMethodType::Void:
            result_var_ = nullptr;
            if (args->count() != 1) throw SymbolicAnalysisFailedException();
            break;
        case AsyncMethodType::AsyncEnumerable:
        case AsyncMethodType::AsyncEnumerator:
            result_var_ = nullptr;
            if (args->count() != 2) throw SymbolicAnalysisFailedException();
            if (!(*args)[1]->match_ldc_i4(0)) throw SymbolicAnalysisFailedException();
            break;
    }
    pos++;
    match_complete_call(block, pos);
    if (!block->instructions[pos]->match_leave(block_container)) throw SymbolicAnalysisFailedException();
}

void AsyncAwaitDecompiler::validate_catch_block() {
    // catch E_143 : System.Exception if (ldc.i4 1) BlockContainer {
    // 	Block IL_008f (incoming: 1)  {
    // 		stloc exception(ldloc E_143)
    // 		stfld <>1__state(ldloc this, ldc.i4 -2)
    //      [optional] stfld <>u__N(ldloc this, ldnull)
    // 		call SetException(ldfld <>t__builder(ldloc this), ldloc exception)
    //      [optional] call Complete(ldfld <>t__builder(ldloc this))
    // 		leave IL_0000
    // 	}
    // }
    if (main_try_catch_ == nullptr || main_try_catch_->handlers.count() != 1) throw SymbolicAnalysisFailedException();
    TryCatchHandler* handler = main_try_catch_->handlers[0];
    if (!typesystem::is_known_type(handler->variable()->type(), KnownTypeCode::Exception)) throw SymbolicAnalysisFailedException();
    if (!handler->filter()->match_ldc_i4(1)) throw SymbolicAnalysisFailedException();
    auto* handler_container = as<BlockContainer>(handler->body());
    if (handler_container == nullptr) throw SymbolicAnalysisFailedException();
    std::vector<char> blocks_analyzed(static_cast<std::size_t>(handler_container->blocks.count()), 0);
    Block* catch_block = handler_container->entry_point();
    catch_handler_offset_ = catch_block->start_il_offset();
    int pos = 0;
    // [vb-only] call SetProjectError(ldloc E_143)
    if (is_visual_basic_state_machine_) {
        auto* call = as<Call>(catch_block->instructions[pos]);
        if (call != nullptr && call->method->name() == "SetProjectError" && call->arguments().count() == 1 &&
            call->arguments()[0]->match_ld_loc(handler->variable())) {
            pos++;
        }
    }
    // stloc exception(ldloc E_143)
    auto* stloc = as<StLoc>(catch_block->instructions[pos++]);
    if (stloc == nullptr) throw SymbolicAnalysisFailedException();
    if (!stloc->value()->match_ld_loc(handler->variable())) throw SymbolicAnalysisFailedException();
    // stfld <>1__state(ldloc this, ldc.i4 -2)
    int new_state = 0;
    if (!match_state_assignment(catch_block->instructions[pos++], &new_state)) throw SymbolicAnalysisFailedException();
    if (final_state_known_) {
        if (new_state != final_state_) throw SymbolicAnalysisFailedException();
    } else {
        final_state_ = new_state;
        final_state_known_ = true;
    }

    // [optional] stfld <>u__N(ldloc this, ldnull)
    // The hoisted-local cleanup may appear either before or after the combined-tokens disposal
    // (the latter is present for async iterators with an [EnumeratorCancellation] token).
    match_hoisted_local_cleanup(catch_block, pos);

    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (pos + 2 == catch_block->instructions.count() && catch_block->match_if_at_end_of_block(&condition, &true_inst, &false_inst)) {
        Block* set_result_and_exit_block = nullptr;
        if (match_dispose_combined_tokens(handler_container, condition, true_inst, false_inst, blocks_analyzed, &set_result_and_exit_block)) {
            blocks_analyzed[static_cast<std::size_t>(catch_block->child_index())] = 1;
            catch_block = set_result_and_exit_block;
            pos = 0;
        } else {
            throw SymbolicAnalysisFailedException();
        }
    }
    match_hoisted_local_cleanup(catch_block, pos);
    // [optional] call Complete(ldfld <>t__builder(ldloc this))
    match_complete_call(catch_block, pos);

    // call SetException(ldfld <>t__builder(ldloc this), ldloc exception)
    InstructionCollection<ILInstruction>* args = match_call(catch_block->instructions[pos], "SetException");
    if (args == nullptr) throw SymbolicAnalysisFailedException();
    if (args->count() != 2) throw SymbolicAnalysisFailedException();
    if (!is_builder_or_promise_field_on_this((*args)[0])) throw SymbolicAnalysisFailedException();
    if (!(*args)[1]->match_ld_loc(stloc->variable())) throw SymbolicAnalysisFailedException();

    pos++;
    // [optional] call Complete(ldfld <>t__builder(ldloc this))
    match_complete_call(catch_block, pos);

    // [vb-only] call ClearProjectError()
    if (is_visual_basic_state_machine_) {
        auto* call2 = as<Call>(catch_block->instructions[pos]);
        if (call2 != nullptr && call2->method->name() == "ClearProjectError" && call2->arguments().count() == 0) pos++;
    }

    // leave IL_0000
    if (!catch_block->instructions[pos]->match_leave(static_cast<BlockContainer*>(move_next_function_->body()))) throw SymbolicAnalysisFailedException();
    blocks_analyzed[static_cast<std::size_t>(catch_block->child_index())] = 1;
    if (!std::all_of(blocks_analyzed.begin(), blocks_analyzed.end(), [](char b) { return b != 0; })) throw SymbolicAnalysisFailedException();
}

void AsyncAwaitDecompiler::match_complete_call(Block* block, int& pos) {
    if (InstructionCollection<ILInstruction>* args = match_call(block->instructions[pos], "Complete")) {
        if (!(args->count() == 1 && is_builder_field_on_this((*args)[0]))) throw SymbolicAnalysisFailedException();
        pos++;
    }
}

bool AsyncAwaitDecompiler::is_builder_field_on_this(ILInstruction* inst) {
    const IField* field = nullptr;
    ILInstruction* target = nullptr;
    if (builder_type_->is_reference_type() == true) {
        // ldfld(StateMachine::<>t__builder, ldloc(this))
        if (!inst->match_ld_fld(&target, &field)) return false;
    } else {
        // ldflda(StateMachine::<>t__builder, ldloc(this))
        if (!inst->match_ld_flda(&target, &field)) return false;
    }
    return target->match_ld_this() && field->member_definition() == builder_field_;
}

bool AsyncAwaitDecompiler::is_builder_or_promise_field_on_this(ILInstruction* inst) {
    if (method_type_ == AsyncMethodType::AsyncEnumerable || method_type_ == AsyncMethodType::AsyncEnumerator) {
        return true;  // TODO: check against uses of promise fields in other methods?
    } else {
        return is_builder_field_on_this(inst);
    }
}

bool AsyncAwaitDecompiler::match_state_assignment(ILInstruction* inst, int* new_state) {
    // stfld(StateMachine::<>1__state, ldloc(this), ldc.i4(stateId))
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    ILInstruction* value = nullptr;
    if (inst->match_st_fld(&target, &field, &value) && stack_slot_value(target)->match_ld_this() && field->member_definition() == state_field_ &&
        stack_slot_value(value)->match_ldc_i4(new_state)) {
        return true;
    }
    *new_state = 0;
    return false;
}

// Analyse the DisposeAsync() method in order to find the disposeModeField.
void AsyncAwaitDecompiler::analyze_dispose_async() {
    dispose_mode_field_ = nullptr;
    if (!is_async_enumerator()) return;
    const IMethod* dispose_async = nullptr;
    for (const IMethod* m : state_machine_type_->methods()) {
        if (m->name().ends_with(".DisposeAsync")) {
            dispose_async = m;
            break;
        }
    }
    if (dispose_async == nullptr) throw SymbolicAnalysisFailedException("Could not find DisposeAsync()");
    srm::MethodDefinitionHandle dispose_async_handle(dispose_async->metadata_token());
    ILFunction* function = YieldReturnDecompiler::create_il_ast(dispose_async_handle, *context_);
    for (ILInstruction* store : function->descendants()) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        if (!store->match_st_fld(&target, &field, &value)) continue;
        if (!target->match_ld_this()) continue;
        if (!value->match_ldc_i4(1)) throw SymbolicAnalysisFailedException();
        if (dispose_mode_field_ != nullptr) throw SymbolicAnalysisFailedException("Multiple stores to disposeMode in DisposeAsync()");
        dispose_mode_field_ = field_definition(field);
    }
}

// ---- InlineBodyOfMoveNext ------------------------------------------------------------------------------------------------------------

void AsyncAwaitDecompiler::inline_body_of_move_next(ILFunction* function) {
    context_->step("Inline body of MoveNext()", function);
    function->set_body(main_try_catch_->try_block());
    function->async_return_type = underlying_return_type_;
    function->move_next_method = move_next_function_->method;
    function->sequence_point_candidates = move_next_function_->sequence_point_candidates;
    function->code_size = move_next_function_->code_size;
    function->local_variable_signature_length = move_next_function_->local_variable_signature_length;
    function->is_iterator = is_async_enumerator();
    move_next_function_->variables().clear();
    move_next_function_->release_ref();
    for (ILInstruction* d : function->descendants()) {
        auto* branch = as<Branch>(d);
        if (branch != nullptr && branch->target_block() == set_result_return_block_) {
            ILInstruction* result = result_var_ == nullptr ? nullptr : make<LdLoc>(result_var_);
            branch->replace_with(with_il_range(make<Leave>(static_cast<BlockContainer*>(function->body()), result), branch));
        }
    }
    if (set_result_yield_block_ != nullptr) {
        // We still might have branches to this block; and we can't quite yet get rid of it.
        static_cast<BlockContainer*>(function->body())->blocks.add(set_result_yield_block_);
    }
    for (ILInstruction* d : function->descendants()) {
        auto* leave = as<Leave>(d);
        if (leave != nullptr && leave->target_container() == move_next_function_->body()) {
            leave->set_target_container(static_cast<BlockContainer*>(function->body()));
            move_next_leaves_.insert(leave);
        }
    }
    register_variables_used_in(function, function);
    function->variables().remove_dead();
    for (const auto& entry : field_to_parameter_map_) function->variables().add(entry.second);
}

void AsyncAwaitDecompiler::finalize_inline_move_next(ILFunction* function) {
    context_->step("FinalizeInlineMoveNext()", function);
    for (ILInstruction* d : function->descendants()) {
        auto* leave = as<Leave>(d);
        if (leave != nullptr && move_next_leaves_.contains(leave)) {
            auto* invalid = make<InvalidBranch>();
            invalid->message = "leave MoveNext - await not detected correctly";
            leave->replace_with(with_il_range(invalid, leave));
        }
    }
    // Delete dead loads of the state cache variable:
    for (ILInstruction* d : function->descendants()) {
        auto* block = as<Block>(d);
        if (block == nullptr) continue;
        for (int i = block->instructions.count() - 1; i >= 0; i--) {
            ILVariable* v = nullptr;
            ILInstruction* value = nullptr;
            if (block->instructions[i]->match_st_loc(&v, &value) && v->is_single_definition() && v->load_count() == 0 && value->match_ld_loc(cached_state_var_)) {
                block->instructions.remove_at(i);
            }
        }
    }
}

// ---- AnalyzeStateMachine -------------------------------------------------------------------------------------------------------------

// Analyze the the state machine; and replace 'leave IL_0000' with await+jump to block that gets entered on the next MoveNext() call.
void AsyncAwaitDecompiler::analyze_state_machine(ILFunction* function) {
    context_->step_start_group("AnalyzeStateMachine()", function);
    smallest_awaiter_var_index_ = std::numeric_limits<int>::max();
    for (ILInstruction* d : function->descendants()) {
        auto* container = as<BlockContainer>(d);
        if (container == nullptr) continue;
        // Fold the runtime ICriticalNotifyCompletion type-check that the compiler emits when the
        // awaiter's static type is not known to implement it (dynamic awaiters, some generic awaiters)
        // into the canonical single-call form that AnalyzeAwaitBlock recognizes.
        for (int i = 0; i < container->blocks.count(); i++) normalize_await_on_completed_dual_branch(container->blocks[i]);

        // Use a separate state range analysis per container.
        StateRangeAnalysis sra(StateRangeAnalysisMode::AsyncMoveNext, state_field_, cached_state_var_);
        sra.cancellation_token = context_->cancellation_token;
        sra.do_finally_bodies = do_finally_bodies_;
        sra.assign_state_ranges(container, util::LongSet::universe());
        util::LongDict<Block*> state_to_block_map = sra.get_block_state_set_mapping(container);

        for (int i = 0; i < container->blocks.count(); i++) {
            Block* block = container->blocks[i];
            context_->cancellation_token.throw_if_cancellation_requested();
            transforms::DynamicCallSiteTransform::run_on_basic_block(block, *context_);
            auto* leave = as<Leave>(block->instructions.last());
            if (leave != nullptr && move_next_leaves_.contains(leave)) {
                // This is likely an 'await' block
                context_->step("AnalyzeAwaitBlock(" + std::to_string(block->start_il_offset()) + ")", block);
                ILVariable* awaiter_var = nullptr;
                const IField* awaiter_field = nullptr;
                int state = 0;
                int yield_offset = 0;
                if (analyze_await_block(block, &awaiter_var, &awaiter_field, &state, &yield_offset)) {
                    block->instructions.add(make<Await>(make<LdLoca>(awaiter_var)));
                    Block* target_block = state_to_block_map.get_or_default(state);
                    if (target_block != nullptr) {
                        await_debug_infos_.push_back(AwaitDebugInfo{yield_offset, target_block->start_il_offset()});
                        block->instructions.add(make<Branch>(target_block));
                    } else {
                        block->instructions.add(make<InvalidBranch>("Could not find block for state " + std::to_string(state)));
                    }
                    await_blocks_.emplace(block, AwaitBlockData{awaiter_var, awaiter_field});
                    if (awaiter_var->index() && *awaiter_var->index() < smallest_awaiter_var_index_) {
                        smallest_awaiter_var_index_ = *awaiter_var->index();
                    }
                }
            } else if (block->instructions.last()->match_branch(set_result_yield_block_)) {
                // This is a 'yield return' in an async enumerator.
                context_->step("AnalyzeYieldReturn(" + std::to_string(block->start_il_offset()) + ")", block);
                ILInstruction* yield_value = nullptr;
                int state = 0;
                if (analyze_yield_return(block, &yield_value, &state)) {
                    block->instructions.add(make<YieldReturn>(yield_value));
                    Block* target_block = state_to_block_map.get_or_default(state);
                    if (target_block != nullptr) {
                        block->instructions.add(make<Branch>(target_block));
                    } else {
                        block->instructions.add(make<InvalidBranch>("Could not find block for state " + std::to_string(state)));
                    }
                } else {
                    block->instructions.add(make<InvalidBranch>("Could not detect 'yield return'"));
                }
            }
            transform_yield_break(block);
        }
        for (int i = 0; i < container->blocks.count(); i++) simplify_if_dispose_mode(container->blocks[i]);
        // Skip the state dispatcher and directly jump to the initial state
        Block* entry_point = state_to_block_map.get_or_default(initial_state_);
        if (entry_point != nullptr) {
            Block* new_entry = make<Block>();
            new_entry->instructions.add(make<Branch>(entry_point));
            container->blocks.insert(0, new_entry);
        }
        container->sort_blocks(true);
        // Collapsing the dynamic awaiter callsites left each await's GetAwaiter/IsCompleted/GetResult spread
        // across branch-joined blocks; the SortBlocks above removed the unreachable init blocks, so re-join
        // the chains into single blocks for DetectAwaitPattern.
        coalesce_dynamic_awaiter_blocks(container, *context_);
        container->sort_blocks(true);
    }
    context_->step_end_group();
}

bool AsyncAwaitDecompiler::transform_yield_break(Block* block) {
    // stfld disposeMode(ldloc this, ldc.i4 1)
    // br nextBlock
    if (block->instructions.count() < 2) return false;
    auto* branch = as<Branch>(block->instructions.last());
    if (branch == nullptr) return false;
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[block->instructions.count() - 2]->match_st_fld(&target, &field, &value)) return false;
    if (!target->match_ld_this()) return false;
    if (field->member_definition() != dispose_mode_field_) return false;
    if (!value->match_ldc_i4(1)) return false;

    // Detected a 'yield break;'
    context_->step("TransformYieldBreak(" + std::to_string(block->start_il_offset()) + ")", block);
    ILInstruction* break_target = find_yield_break_target(branch->target_block());
    if (auto* target_block = as<Block>(break_target)) {
        branch->set_target_block(target_block);
    } else {
        assert(is<BlockContainer>(break_target));
        branch->replace_with(with_il_range(make<Leave>(static_cast<BlockContainer*>(break_target)), branch));
    }
    return true;
}

ILInstruction* AsyncAwaitDecompiler::find_yield_break_target(Block* block) {
    // We'll follow the branch and evaluate the following instructions
    // under the assumption that disposeModeField==1, which lets us follow a series of jumps
    // to determine the final target.
    std::unordered_set<Block*> visited;
    SymbolicEvaluationContext eval_context(dispose_mode_field_);
    while (true) {
        for (int i = 0; i < block->instructions.count(); i++) {
            ILInstruction* inst = block->instructions[i];
            ILInstruction* condition = nullptr;
            ILInstruction* true_inst = nullptr;
            ILInstruction* false_inst = nullptr;
            while (inst->match_if_instruction(&condition, &true_inst, &false_inst)) {
                SymbolicValue cond_val = eval_context.eval(condition).as_bool();
                if (cond_val.type == SymbolicValueType::IntegerConstant) {
                    inst = cond_val.constant != 0 ? true_inst : false_inst;
                } else if (cond_val.type == SymbolicValueType::StateInSet) {
                    inst = cond_val.value_set.contains(1) ? true_inst : false_inst;
                } else {
                    return block;
                }
            }
            Block* target_block = nullptr;
            if (inst->match_branch(&target_block)) {
                if (visited.insert(block).second) {
                    block = target_block;
                    break;  // continue with next block
                } else {
                    return block;  // infinite loop detected
                }
            } else if (auto* leave = as<Leave>(inst); leave != nullptr && leave->value()->op_code == OpCode::Nop) {
                return leave->target_container();
            } else if (inst->op_code == OpCode::Nop) {
                continue;  // continue with next instruction in this block
            } else {
                return block;
            }
        }
    }
}

bool AsyncAwaitDecompiler::simplify_if_dispose_mode(Block* block) {
    auto match_ld_dispose_mode = [this](ILInstruction* inst) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (!inst->match_ld_fld(&target, &field)) return false;
        return target->match_ld_this() && field->member_definition() == dispose_mode_field_;
    };
    // Occasionally Roslyn optimizes out an "if (disposeMode)", but keeps the
    // disposeMode field access. Get rid of those field accesses:
    block->instructions.remove_all(match_ld_dispose_mode);

    // if (logic.not(ldfld disposeMode(ldloc this))) br falseInst
    // br trueInst
    ILInstruction* condition = nullptr;
    ILInstruction* unused_true_inst = nullptr;
    ILInstruction* false_inst = nullptr;
    if (!block->match_if_at_end_of_block(&condition, &unused_true_inst, &false_inst)) return false;
    if (!match_ld_dispose_mode(condition)) return false;
    context_->step("SimplifyIfDisposeMode(" + std::to_string(block->start_il_offset()) + ")", block);
    block->instructions.set(block->instructions.count() - 2, false_inst);
    block->instructions.remove_at(block->instructions.count() - 1);
    return true;
}

// When the awaiter's static type is not known to implement ICriticalNotifyCompletion (dynamic awaiters, and some generic awaiters), the
// C# compiler picks between AwaitOnCompleted and AwaitUnsafeOnCompleted at runtime:
//   stloc criticalVar(isinst ICriticalNotifyCompletion(ldloc awaiterVar))
//   [stloc smVar(ldloc this)]                                      // class state machine only
//   if (comp.o(ldloc criticalVar != ldnull)) br unsafeOnCompletedBlock
//   br onCompletedBlock
// unsafeOnCompletedBlock:
//   call AwaitUnsafeOnCompleted(ldflda builder(this), ldloca criticalVar, ldloca smVar); br mergeBlock
// onCompletedBlock:
//   stloc notifyVar(castclass INotifyCompletion(ldloc awaiterVar))
//   call AwaitOnCompleted(ldflda builder(this), ldloca notifyVar, ldloca smVar); br mergeBlock
// mergeBlock:
//   leave
// Both branches await the same awaiter, so this folds the whole diamond into the single-call form AnalyzeAwaitBlock recognizes, awaiting
// the awaiter variable directly.
bool AsyncAwaitDecompiler::normalize_await_on_completed_dual_branch(Block* block) {
    int count = block->instructions.count();
    if (count < 3) return false;
    Block* on_completed_block = nullptr;
    if (!block->instructions[count - 1]->match_branch(&on_completed_block)) return false;
    ILInstruction* condition = nullptr;
    ILInstruction* true_branch = nullptr;
    if (!block->instructions[count - 2]->match_if_instruction(&condition, &true_branch)) return false;
    Block* unsafe_on_completed_block = nullptr;
    if (!true_branch->match_branch(&unsafe_on_completed_block)) return false;
    ILInstruction* critical_test = nullptr;
    ILVariable* critical_var = nullptr;
    if (!condition->match_comp_not_equals_null(&critical_test) || !critical_test->match_ld_loc(&critical_var)) return false;

    int is_inst_pos = count - 3;
    ILVariable* sm_var = nullptr;
    ILVariable* sm_candidate = nullptr;
    ILInstruction* sm_value = nullptr;
    if (is_inst_pos >= 0 && block->instructions[is_inst_pos]->match_st_loc(&sm_candidate, &sm_value) && sm_value->match_ld_this()) {
        sm_var = sm_candidate;
        is_inst_pos--;
    }
    ILInstruction* critical_value = nullptr;
    if (is_inst_pos < 0 || !block->instructions[is_inst_pos]->match_st_loc(critical_var, &critical_value)) return false;
    auto* is_inst = as<IsInst>(critical_value);
    ILVariable* awaiter_var = nullptr;
    if (!(is_inst != nullptr && is_inst->type()->full_name() == "System.Runtime.CompilerServices.ICriticalNotifyCompletion" &&
          is_inst->argument()->match_ld_loc(&awaiter_var))) {
        return false;
    }

    auto match_state_machine_arg = [sm_var](ILInstruction* arg) {
        ILVariable* v = nullptr;
        return sm_var != nullptr ? (arg->match_ld_loca(&v) && v == sm_var) : arg->match_ld_this();
    };

    // unsafeOnCompletedBlock: call AwaitUnsafeOnCompleted(builder, ldloca criticalVar, smArg); br mergeBlock
    if (unsafe_on_completed_block->instructions.count() != 2) return false;
    {
        InstructionCollection<ILInstruction>* unsafe_args = match_call(unsafe_on_completed_block->instructions[0], "AwaitUnsafeOnCompleted");
        ILVariable* unsafe_awaiter = nullptr;
        if (unsafe_args == nullptr || unsafe_args->count() != 3 || !is_builder_field_on_this((*unsafe_args)[0]) ||
            !(*unsafe_args)[1]->match_ld_loca(&unsafe_awaiter) || unsafe_awaiter != critical_var || !match_state_machine_arg((*unsafe_args)[2])) {
            return false;
        }
    }
    Block* merge_block = nullptr;
    if (!unsafe_on_completed_block->instructions[1]->match_branch(&merge_block)) return false;

    // onCompletedBlock: stloc notifyVar(castclass INotifyCompletion(ldloc awaiterVar)); call AwaitOnCompleted(builder, ldloca notifyVar, smArg); br mergeBlock
    if (on_completed_block->instructions.count() != 3) return false;
    ILVariable* notify_var = nullptr;
    ILInstruction* notify_value = nullptr;
    ILVariable* cast_awaiter = nullptr;
    if (!on_completed_block->instructions[0]->match_st_loc(&notify_var, &notify_value)) return false;
    auto* cast_class = as<CastClass>(notify_value);
    if (!(cast_class != nullptr && cast_class->type()->full_name() == "System.Runtime.CompilerServices.INotifyCompletion" &&
          cast_class->argument()->match_ld_loc(&cast_awaiter) && cast_awaiter == awaiter_var)) {
        return false;
    }
    {
        InstructionCollection<ILInstruction>* safe_args = match_call(on_completed_block->instructions[1], "AwaitOnCompleted");
        ILVariable* safe_awaiter = nullptr;
        if (safe_args == nullptr || safe_args->count() != 3 || !is_builder_field_on_this((*safe_args)[0]) || !(*safe_args)[1]->match_ld_loca(&safe_awaiter) ||
            safe_awaiter != notify_var || !match_state_machine_arg((*safe_args)[2])) {
            return false;
        }
    }
    Block* merge_block2 = nullptr;
    if (!on_completed_block->instructions[2]->match_branch(&merge_block2) || merge_block2 != merge_block) return false;

    auto* merge_leave = merge_block->instructions.count() == 1 ? as<Leave>(merge_block->instructions[0]) : nullptr;
    if (merge_leave == nullptr) return false;

    // Rewrite `block` into the canonical single-call form.
    context_->step("Normalize dynamic AwaitOnCompleted", block);
    auto* await_call = static_cast<CallInstruction*>(unsafe_on_completed_block->instructions[0]);
    unsafe_on_completed_block->instructions.remove_at(0);
    await_call->arguments()[1]->replace_with(make<LdLoca>(awaiter_var));
    // Remove the isinst test, the if, and the branch; keep the state/awaiter-field stores (and smVar store).
    block->instructions.remove_range(count - 2, 2);  // if + br
    block->instructions.remove_at(is_inst_pos);      // isinst store
    block->instructions.add(await_call);
    auto* new_leave = static_cast<Leave*>(merge_leave->clone());
    move_next_leaves_.insert(new_leave);
    block->instructions.add(new_leave);
    return true;
}

bool AsyncAwaitDecompiler::analyze_await_block(Block* block, ILVariable** awaiter, const IField** awaiter_field, int* state, int* yield_offset) {
    *awaiter = nullptr;
    *awaiter_field = nullptr;
    *state = 0;
    *yield_offset = -1;
    int pos = block->instructions.count() - 2;
    if (pos >= 0 && do_finally_bodies_ != nullptr) {
        if (auto* store_do_finally_bodies = as<StLoc>(block->instructions[pos])) {
            if (!(store_do_finally_bodies->variable()->kind() == VariableKind::Local &&
                  typesystem::is_known_type(store_do_finally_bodies->variable()->type(), KnownTypeCode::Boolean) &&
                  store_do_finally_bodies->variable()->index() == do_finally_bodies_->index())) {
                return false;
            }
            if (!store_do_finally_bodies->value()->match_ldc_i4(0)) return false;
            pos--;
        }
    }

    InstructionCollection<ILInstruction>* call_args = nullptr;
    if (pos >= 0 && (call_args = match_call(block->instructions[pos], "AwaitUnsafeOnCompleted")) != nullptr) {
        // call AwaitUnsafeOnCompleted(ldflda <>t__builder(ldloc this), ldloca awaiter, ldloc this)
    } else if (pos >= 0 && (call_args = match_call(block->instructions[pos], "AwaitOnCompleted")) != nullptr) {
        // call AwaitOnCompleted(ldflda <>t__builder(ldloc this), ldloca awaiter, ldloc this)
        // The C# compiler emits the non-unsafe call when the awaiter does not implement
        // ICriticalNotifyCompletion.
    } else {
        return false;
    }
    if (call_args->count() != 3) return false;
    if (!is_builder_field_on_this((*call_args)[0])) return false;
    if (!(*call_args)[1]->match_ld_loca(awaiter)) return false;
    ILVariable* temp_var = nullptr;
    if ((*call_args)[2]->match_ld_this()) {
        // OK (if state machine is a struct)
        pos--;
    } else if ((*call_args)[2]->match_ld_loca(&temp_var)) {
        // Roslyn, non-optimized uses a class for the state machine.
        // stloc tempVar(ldloc this)
        // call AwaitUnsafeOnCompleted(ldflda <>t__builder](ldloc this), ldloca awaiter, ldloca tempVar)
        ILInstruction* temp_val = nullptr;
        if (!(pos > 0 && block->instructions[pos - 1]->match_st_loc(temp_var, &temp_val))) return false;
        if (!temp_val->match_ld_this()) return false;
        pos -= 2;
    } else {
        return false;
    }
    // stfld StateMachine.<>awaiter(ldloc this, ldloc awaiter)
    ILInstruction* target = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[pos]->match_st_fld(&target, awaiter_field, &value)) return false;
    if (!target->match_ld_this()) return false;
    if (!value->match_ld_loc(*awaiter)) return false;
    pos--;
    // Store IL offset for debug info:
    *yield_offset = block->instructions[pos]->end_il_offset();

    // stloc S_10(ldloc this)
    // stloc S_11(ldc.i4 0)
    // stloc cachedStateVar(ldloc S_11)
    // stfld <>1__state(ldloc S_10, ldloc S_11)
    const IField* field = nullptr;
    if (!block->instructions[pos]->match_st_fld(&target, &field, &value)) return false;
    if (!stack_slot_value(target)->match_ld_this()) return false;
    if (field->member_definition() != state_field_) return false;
    if (!stack_slot_value(value)->match_ldc_i4(state)) return false;
    if (pos > 0) {
        auto* stloc = as<StLoc>(block->instructions[pos - 1]);
        if (stloc != nullptr && stloc->variable()->kind() == VariableKind::Local && stloc->variable()->index() == index_of(cached_state_var_) &&
            stack_slot_value(stloc->value())->match_ldc_i4(*state)) {
            // also delete the assignment to cachedStateVar
            pos--;
        }
    }
    block->instructions.remove_range(pos, block->instructions.count() - pos);
    // delete preceding dead stores:
    while (pos > 0) {
        auto* stloc2 = as<StLoc>(block->instructions[pos - 1]);
        if (!(stloc2 != nullptr && stloc2->variable()->is_single_definition() && stloc2->variable()->load_count() == 0 &&
              stloc2->variable()->kind() == VariableKind::StackSlot && SemanticHelper::is_pure(stloc2->value()->flags())))
            break;
        pos--;
    }
    block->instructions.remove_range(pos, block->instructions.count() - pos);
    return true;
}

ILInstruction* AsyncAwaitDecompiler::stack_slot_value(ILInstruction* inst) {
    ILVariable* v = nullptr;
    if (inst->match_ld_loc(&v) && v->kind() == VariableKind::StackSlot && v->is_single_definition()) {
        if (auto* stloc = as<StLoc>(v->store_instructions()[0]->as_instruction())) {
            return stloc->value();
        }
    }
    return inst;
}

bool AsyncAwaitDecompiler::analyze_yield_return(Block* block, ILInstruction** yield_value, int* new_state) {
    *yield_value = nullptr;
    *new_state = 0;
    assert(block->instructions.last()->match_branch(set_result_yield_block_));
    // stfld current(ldloc this, ldstr "yieldValue")
    // stloc S_45(ldloc this)
    // stloc S_46(ldc.i4 -5)
    // stloc V_0(ldloc S_46)
    // stfld stateField(ldloc S_45, ldloc S_46)
    // br setResultYieldBlock

    int pos = block->instructions.count() - 2;
    // Immediately before the 'yield return', there should be a state assignment:
    if (pos < 0 || !match_state_assignment(block->instructions[pos], new_state)) return false;
    pos--;

    ILInstruction* cached_state_new_value = nullptr;
    if (pos >= 0 && block->instructions[pos]->match_st_loc(cached_state_var_, &cached_state_new_value)) {
        if (stack_slot_value(cached_state_new_value)->match_ldc_i4(*new_state)) {
            pos--;  // OK, ignore V_0 store
        } else {
            return false;
        }
    }

    while (pos >= 0) {
        auto* stloc = as<StLoc>(block->instructions[pos]);
        if (stloc == nullptr) break;
        if (stloc->variable()->kind() != VariableKind::StackSlot) return false;
        if (!SemanticHelper::is_pure(stloc->value()->flags())) return false;
        pos--;
    }

    if (pos < 0 || !match_current_assignment(block->instructions[pos], yield_value)) return false;

    block->instructions.remove_range(pos, block->instructions.count() - pos);
    return true;
}

bool AsyncAwaitDecompiler::match_current_assignment(ILInstruction* inst, ILInstruction** value) {
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    if (!inst->match_st_fld(&target, &field, value)) return false;
    if (!stack_slot_value(target)->match_ld_this()) return false;
    // TODO: check that we are accessing the current field (compare with get_Current)
    return true;
}

// ---- DetectAwaitPattern --------------------------------------------------------------------------------------------------------------

void AsyncAwaitDecompiler::detect_await_pattern(ILFunction* function) {
    context_->step_start_group("DetectAwaitPattern", function);
    for (ILInstruction* d : function->descendants()) {
        auto* container = as<BlockContainer>(d);
        if (container == nullptr) continue;
        for (int i = 0; i < container->blocks.count(); i++) {
            context_->cancellation_token.throw_if_cancellation_requested();
            detect_await_pattern(container->blocks[i]);
        }
        container->sort_blocks(true);
    }
    context_->step_end_group(true);
}

void AsyncAwaitDecompiler::detect_await_pattern(Block* block) {
    // block:
    //   stloc awaiterVar(callvirt GetAwaiter(...))
    //   if (call get_IsCompleted(ldloca awaiterVar)) br completedBlock
    //   br awaitBlock
    // awaitBlock:
    //   ..
    //   br resumeBlock
    // resumeBlock:
    //   ..
    //   br completedBlock
    if (block->instructions.count() < 3) return;
    // stloc awaiterVar(callvirt GetAwaiter(...))
    auto* st_loc_awaiter = as<StLoc>(block->instructions[block->instructions.count() - 3]);
    if (st_loc_awaiter == nullptr) return;
    ILVariable* awaiter_var = st_loc_awaiter->variable();
    ILInstruction* awaited_value = nullptr;
    const IMethod* get_awaiter_method = nullptr;
    bool is_dynamic_await = false;
    auto* get_awaiter_call = as<CallInstruction>(st_loc_awaiter->value());
    auto* dyn_get_awaiter = as<DynamicInvokeMemberInstruction>(st_loc_awaiter->value());
    if (get_awaiter_call != nullptr && get_awaiter_call->method->name() == "GetAwaiter" &&
        (!get_awaiter_call->method->is_static() || get_awaiter_call->method->is_extension_method()) && get_awaiter_call->arguments().count() == 1) {
        awaited_value = get_awaiter_call->arguments()[0];
        get_awaiter_method = get_awaiter_call->method;
    } else if (dyn_get_awaiter != nullptr && dyn_get_awaiter->name() == "GetAwaiter" && dyn_get_awaiter->arguments().count() == 1) {
        // awaiting a dynamic value: GetAwaiter/IsCompleted/GetResult are dynamic callsites
        awaited_value = dyn_get_awaiter->arguments()[0];
        get_awaiter_method = create_dynamic_awaiter_method(*context_, "GetAwaiter");
        is_dynamic_await = true;
    } else {
        return;
    }
    // if (call get_IsCompleted(ldloca awaiterVar)) br completedBlock
    ILInstruction* condition = nullptr;
    ILInstruction* true_inst = nullptr;
    if (!block->instructions[block->instructions.count() - 2]->match_if_instruction(&condition, &true_inst)) return;
    Block* completed_block = nullptr;
    if (!true_inst->match_branch(&completed_block)) return;
    // br awaitBlock
    Block* await_block = nullptr;
    if (!block->instructions.last()->match_branch(&await_block)) return;
    // condition might be inverted, swap branches:
    ILInstruction* negated_condition = nullptr;
    if (condition->match_logic_not(&negated_condition)) {
        condition = negated_condition;
        std::swap(completed_block, await_block);
    }
    if (is_dynamic_await) {
        // if (dynamic.convert bool(dynamic.getmember "IsCompleted"(ldloc awaiterVar)))
        ILInstruction* is_completed_test = condition;
        if (auto* dyn_conv = as<DynamicConvertInstruction>(is_completed_test); dyn_conv != nullptr && typesystem::is_known_type(dyn_conv->type(), KnownTypeCode::Boolean))
            is_completed_test = dyn_conv->argument();
        auto* dyn_is_completed = as<DynamicGetMemberInstruction>(is_completed_test);
        if (!(dyn_is_completed != nullptr && dyn_is_completed->name() == "IsCompleted")) return;
        if (!dyn_is_completed->target()->match_ld_loc(awaiter_var)) return;
    } else {
        // continue matching call get_IsCompleted(ldloca awaiterVar)
        InstructionCollection<ILInstruction>* is_completed_args = match_call(condition, "get_IsCompleted");
        if (is_completed_args == nullptr || is_completed_args->count() != 1) return;
        if (!unwrap_conv_unknown((*is_completed_args)[0])->match_ld_loc_ref(awaiter_var)) return;
    }
    // Check awaitBlock and resumeBlock:
    auto await_block_data = await_blocks_.find(await_block);
    if (await_block_data == await_blocks_.end()) return;
    if (await_block_data->second.awaiter_var != awaiter_var) return;
    Block* resume_block = nullptr;
    const IField* stack_field = nullptr;
    if (!check_await_block(await_block, &resume_block, &stack_field)) return;
    if (!check_resume_block(resume_block, awaiter_var, await_block_data->second.awaiter_field, completed_block, stack_field)) return;
    // Check completedBlock. The first instruction involves the GetResult call, but it might have
    // been inlined into another instruction.
    ILInstruction* get_result_inst = nullptr;
    const IMethod* get_result_method = nullptr;
    if (is_dynamic_await) {
        // dynamic.invokemember "GetResult"(ldloc awaiterVar), possibly inlined into another instruction
        ILInstruction* first = completed_block->instructions[0];
        auto matches_get_result = [&](ILInstruction* inst) {
            auto* d = as<DynamicInvokeMemberInstruction>(inst);
            return d != nullptr && d->name() == "GetResult" && d->arguments().count() == 1 && d->arguments()[0]->match_ld_loc(awaiter_var);
        };
        if (matches_get_result(first)) {
            get_result_inst = first;
        } else {
            for (ILInstruction* inst : first->descendants()) {
                if (matches_get_result(inst)) {
                    get_result_inst = inst;
                    break;
                }
            }
        }
        if (get_result_inst == nullptr) return;
        get_result_method = create_dynamic_awaiter_method(*context_, "GetResult");
    } else {
        CallInstruction* get_result_call = transforms::ILInlining::find_first_inlined_call(completed_block->instructions[0]);
        if (get_result_call == nullptr) return;
        InstructionCollection<ILInstruction>* get_result_args = match_call(get_result_call, "GetResult");
        if (get_result_args == nullptr || get_result_args->count() != 1) return;
        if (!unwrap_conv_unknown((*get_result_args)[0])->match_ld_loc_ref(awaiter_var)) return;
        get_result_inst = get_result_call;
        get_result_method = get_result_call->method;
    }
    // All checks successful, let's transform.
    context_->step("Transform await pattern", block);
    block->instructions.remove_at(block->instructions.count() - 3);  // remove getAwaiter call
    block->instructions.remove_at(block->instructions.count() - 2);  // remove if (isCompleted)
    static_cast<Branch*>(block->instructions.last())->set_target_block(completed_block);  // instead, directly jump to completed block
    Await* await_inst = make<Await>(unwrap_conv_unknown(awaited_value));
    await_inst->get_result_method = get_result_method;
    await_inst->get_awaiter_method = get_awaiter_method;
    get_result_inst->replace_with(await_inst);

    // Remove useless reset of awaiterVar.
    if (auto* stobj = as<StObj>(completed_block->instructions.element_at_or_default(1))) {
        if (stobj->target()->match_ld_loca(awaiter_var) && stobj->value()->op_code == OpCode::DefaultValue) {
            completed_block->instructions.remove_at(1);
        }
    }
}

ILInstruction* AsyncAwaitDecompiler::unwrap_conv_unknown(ILInstruction* inst) {
    if (auto* conv = as<Conv>(inst); conv != nullptr && conv->target_type == PrimitiveType::Unknown) {
        return conv->argument();
    }
    return inst;
}

// Synthesizes a GetAwaiter/GetResult method on the dynamic type, so a dynamic await carries the type 'dynamic': the Await result type
// derives from GetResultMethod.ReturnType, and the dynamic declaring type also marks the await as dynamic for IntroduceDynamicTypeOnLocals.
const IMethod* AsyncAwaitDecompiler::create_dynamic_awaiter_method(ILTransformContext& context, const std::string& name) {
    auto* holder = ILArena::current().make<DynamicAwaiterMethod>(context.type_system());
    holder->method.name_value = name;
    holder->method.declaring_type_value = &typesystem::SpecialType::dynamic();
    holder->method.return_type_value = &typesystem::SpecialType::dynamic();
    return &holder->method;
}

// Collapsing the dynamic GetAwaiter/IsCompleted/GetResult callsites leaves each dynamic await spread across unconditional-branch-joined
// blocks. Re-join those chains so each dynamic await ends up in a single block for DetectAwaitPattern. Await blocks are skipped so their
// branch to the resume block survives; the unreachable callsite-init blocks must already be gone, otherwise their dangling branch would
// keep the "after init" block from having the single predecessor CombineBlockWithNextBlock needs.
void AsyncAwaitDecompiler::coalesce_dynamic_awaiter_blocks(BlockContainer* container, ILTransformContext& context) {
    for (int i = 0; i < container->blocks.count(); i++) {
        Block* block = container->blocks[i];
        if (block->instructions.count() == 0 || await_blocks_.contains(block)) continue;
        while (ControlFlowSimplification::combine_block_with_next_block(container, block, context)) {
            // keep pulling in trivial branch targets until this block can grow no further
        }
    }
}

bool AsyncAwaitDecompiler::check_await_block(Block* block, Block** resume_block, const IField** stack_field) {
    // awaitBlock:
    //   (pre-roslyn: save stack)
    //   await(ldloca V_2)
    //   br resumeBlock
    *resume_block = nullptr;
    *stack_field = nullptr;
    if (block->instructions.count() < 2) return false;
    int pos = 0;
    if (auto* stloc = as<StLoc>(block->instructions[pos]); stloc != nullptr && stloc->variable()->is_single_definition()) {
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        if (!block->instructions[pos + 1]->match_st_fld(&target, stack_field, &value)) return false;
        if (!target->match_ld_this()) return false;
        pos += 2;
    }
    // await(ldloca awaiterVar)
    if (block->instructions[pos]->op_code != OpCode::Await) return false;
    // br resumeBlock
    return block->instructions[pos + 1]->match_branch(resume_block);
}

bool AsyncAwaitDecompiler::check_resume_block(Block* block, ILVariable* awaiter_var, const IField* awaiter_field, Block* completed_block, const IField* stack_field) {
    int pos = 0;
    if (!restore_stack(block, pos, stack_field)) return false;

    auto match_store_to_awaiter_variable = [&](ILInstruction* instr) {
        // stloc awaiterVar(ldfld awaiterField(ldloc this))
        ILInstruction* value = nullptr;
        if (!instr->match_st_loc(awaiter_var, &value)) return false;
        if (auto* cast = as<CastClass>(value); cast != nullptr && typesystem::type_equals(cast->type(), awaiter_var->type())) {
            // If the awaiter is a reference type, it might get stored in a field of type `object`
            // and cast back to the awaiter type in the resume block
            value = cast->argument();
        }
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (!value->match_ld_fld(&target, &field)) return false;
        if (!target->match_ld_this()) return false;
        if (!typesystem::member_equals(field, awaiter_field)) return false;
        return true;
    };

    auto match_reset_awaiter_field = [&](Block* b, int& p) {
        // stfld awaiterField(ldloc this, default.value)
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        if (b->instructions[p]->match_st_fld(&target, &field, &value) && target->match_ld_this() && typesystem::member_equals(field, awaiter_field) &&
            (value->op_code == OpCode::DefaultValue || value->op_code == OpCode::LdNull)) {
            p++;
        } else {
            // {stloc V_6(default.value System.Runtime.CompilerServices.TaskAwaiter)}
            // {stobj System.Runtime.CompilerServices.TaskAwaiter`1[[System.Int32]](ldflda <>u__$awaiter4(ldloc this), ldloc V_6) at IL_0163}
            ILVariable* variable = nullptr;
            if (b->instructions[p]->match_st_loc(&variable, &value) && value->op_code == OpCode::DefaultValue &&
                b->instructions[p + 1]->match_st_fld(&target, &field, &value) && typesystem::member_equals(field, awaiter_field) && value->match_ld_loc(variable)) {
                p += 2;
            }
        }
    };

    auto match_state_field_assignement = [&](Block* b, int& p) {
        // stloc S_28(ldc.i4 -1)
        // stloc cachedStateVar(ldloc S_28)
        // stfld <>1__state(ldloc this, ldloc S_28)
        ILVariable* m1_var = nullptr;
        if (auto* stloc_m1 = as<StLoc>(b->instructions[p]);
            stloc_m1 != nullptr && stloc_m1->value()->match_ldc_i4(initial_state_) && stloc_m1->variable()->kind() == VariableKind::StackSlot) {
            m1_var = stloc_m1->variable();
            p++;
        }
        if (auto* stloc_cached_state = as<StLoc>(b->instructions[p])) {
            if (stloc_cached_state->variable()->kind() == VariableKind::Local &&
                stloc_cached_state->variable()->index() == (cached_state_var_ != nullptr ? cached_state_var_->index() : std::nullopt)) {
                if (stloc_cached_state->value()->match_ld_loc(m1_var) || stloc_cached_state->value()->match_ldc_i4(initial_state_)) p++;
            }
        }
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        ILInstruction* value = nullptr;
        if (!b->instructions[p]->match_st_fld(&target, &field, &value)) return false;
        if (!target->match_ld_this()) return false;
        if (field->member_definition() != state_field_->member_definition()) return false;
        if (!(value->match_ldc_i4(initial_state_) || value->match_ld_loc(m1_var))) return false;
        return true;
    };

    // Visual Basic state machines use a different order of field assignements.
    if (is_visual_basic_state_machine_) {
        // stloc S_28(ldc.i4 -1)
        // stloc cachedStateVar(ldloc S_28)
        // stfld <>1__state(ldloc this, ldloc S_28)
        if (!match_state_field_assignement(block, pos)) return false;
        pos++;

        // stloc awaiterVar(ldfld awaiterField(ldloc this))
        if (!match_store_to_awaiter_variable(block->instructions[pos])) return false;
        pos++;

        // [optional] stfld awaiterField(ldloc this, default.value)
        match_reset_awaiter_field(block, pos);
    } else {
        // stloc awaiterVar(ldfld awaiterField(ldloc this))
        if (!match_store_to_awaiter_variable(block->instructions[pos])) return false;
        pos++;

        // [optional] stfld awaiterField(ldloc this, default.value)
        match_reset_awaiter_field(block, pos);

        // stloc S_28(ldc.i4 -1)
        // stloc cachedStateVar(ldloc S_28)
        // stfld <>1__state(ldloc this, ldloc S_28)
        if (!match_state_field_assignement(block, pos)) return false;
        pos++;
    }
    return block->instructions[pos]->match_branch(completed_block);
}

bool AsyncAwaitDecompiler::restore_stack(Block* block, int& pos, const IField* stack_field) {
    if (stack_field == nullptr) {
        return true;  // nothing to restore
    }
    // stloc temp(unbox.any T(ldfld <>t__stack(ldloc this)))
    auto* stloc = as<StLoc>(block->instructions[pos]);
    if (stloc == nullptr) return false;
    if (!stloc->variable()->is_single_definition()) return false;
    auto* unbox = as<UnboxAny>(stloc->value());
    if (unbox == nullptr) return false;
    ILInstruction* target = nullptr;
    const IField* field = nullptr;
    if (!unbox->argument()->match_ld_fld(&target, &field)) return false;
    if (!target->match_ld_this()) return false;
    if (!typesystem::member_equals(field, stack_field)) return false;
    pos++;
    // restoring stack slots
    ILVariable* v = nullptr;
    while (block->instructions[pos]->match_st_loc(&v) && v->kind() == VariableKind::StackSlot) {
        pos++;
    }
    // stfld <>t__stack(ldloc this, ldnull)
    ILInstruction* value = nullptr;
    if (block->instructions[pos]->match_st_fld(&target, &field, &value)) {
        if (target->match_ld_this() && typesystem::member_equals(field, stack_field) && value->match_ld_null()) {
            pos++;
        }
    }
    return true;
}

// Eliminates usage of doFinallyBodies
void AsyncAwaitDecompiler::clean_do_finally_bodies(ILFunction* function) {
    if (do_finally_bodies_ == nullptr) return;  // roslyn-compiled code doesn't use doFinallyBodies
    context_->step_start_group("CleanDoFinallyBodies", function);
    Block* entry_point = get_body_entry_point(as<BlockContainer>(function->body()));
    ILInstruction* value = nullptr;
    if (entry_point != nullptr && entry_point->instructions[0]->match_st_loc(do_finally_bodies_, &value) && value->match_ldc_i4(1)) {
        // Remove initial doFinallyBodies assignment, if it wasn't already removed when
        // we rearranged the control flow.
        entry_point->instructions.remove_at(0);
    }
    if (do_finally_bodies_->store_instructions().size() != 0 || do_finally_bodies_->address_count() != 0) {
        // misdetected another variable as doFinallyBodies?
        // reintroduce the initial store of ldc.i4(1)
        context_->step("Re-introduce misdetected doFinallyBodies", function);
        static_cast<BlockContainer*>(function->body())->entry_point()->instructions.insert(0, make<StLoc>(do_finally_bodies_, make<LdcI4>(1)));
        return;
    }
    for (ILInstruction* d : function->descendants()) {
        auto* try_finally = as<TryFinally>(d);
        if (try_finally == nullptr) continue;
        entry_point = get_body_entry_point(as<BlockContainer>(try_finally->finally_block()));
        auto* if_inst = entry_point != nullptr ? as<IfInstruction>(entry_point->instructions[0]) : nullptr;
        if (if_inst != nullptr) {
            ILInstruction* logic_not_arg = nullptr;
            if (if_inst->condition()->match_logic_not(&logic_not_arg) && logic_not_arg->match_ld_loc(do_finally_bodies_)) {
                context_->step("Remove if(doFinallyBodies) from try-finally", try_finally);
                // condition will always be false now that we're using 'await' instructions
                entry_point->instructions.remove_at(0);
            }
        }
    }
    // if there's any remaining loads (there shouldn't be), replace them with the constant 1
    std::vector<LdLoc*> loads = do_finally_bodies_->load_instructions();
    for (LdLoc* load : loads) load->replace_with(with_il_range(make<LdcI4>(1), load));
    context_->step_end_group(true);
}

Block* AsyncAwaitDecompiler::get_body_entry_point(BlockContainer* body) {
    if (body == nullptr) return nullptr;
    Block* entry_point = body->entry_point();
    Block* target_block = nullptr;
    while (entry_point->instructions[0]->match_branch(&target_block) && target_block->incoming_edge_count() == 1 && target_block->parent() == body) {
        entry_point = target_block;
    }
    return entry_point;
}

void AsyncAwaitDecompiler::translate_cached_fields_to_locals() {
    for (const auto& [cached_var, param] : cached_field_to_parameter_map_) {
        assert(cached_var->store_count() <= 1);
        std::vector<LdLoc*> loads = cached_var->load_instructions();
        for (LdLoc* inst : loads) inst->set_variable(param);
        std::vector<LdLoca*> addresses = cached_var->address_instructions();
        for (LdLoca* inst : addresses) inst->set_variable(param);
    }
}

}  // namespace cb::dotnet::decompiler::il::control_flow

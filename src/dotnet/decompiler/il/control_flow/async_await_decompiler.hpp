// dotnet/decompiler/il/control_flow/async_await_decompiler.hpp -- turns the compiler's async state machine back into an async method with
// `await`: finds the state machine a method creates, reads its MoveNext, inlines MoveNext's body into the method, replaces each
// "store the awaiter, ask for a callback, leave" with an await, and the state dispatch with direct jumps. Also async iterators
// (IAsyncEnumerable / IAsyncEnumerator), and async methods the runtime lowers itself (runtime-async).
//
//     AsyncAwaitDecompiler transform;
//     transform.run(function, context);       // function->is_async() is set when the body was an async method's creation
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: AsyncAwaitDecompiler, from ControlFlow/AsyncAwaitDecompiler.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: CSharpDecompiler's early IL transforms (run
// after the translation), and the await entries of the debug information (AsyncDebugInfo.Await is not modeled yet; they are collected
// in await_debug_infos_ and the function gets the catch handler offset only).
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dotnet/decompiler/il/control_flow/state_range_analysis.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/typesystem/full_type_name.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::metadata {
class MetadataFile;
}

namespace cb::dotnet::decompiler::il::control_flow {

namespace srm = cb::dotnet::metadata;

// Decompiler step for C# 5 async/await.
class AsyncAwaitDecompiler final : public transforms::IILTransform {
public:
    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "AsyncAwaitDecompiler"; }

    static bool is_compiler_generated_state_machine(srm::TypeDefinitionHandle type, const srm::MetadataReader& metadata);
    static bool is_compiler_generated_main_method(const metadata::MetadataFile& module, srm::MethodDefinitionHandle method);
    static bool is_compiler_generator_async_enumerator(srm::TypeDefinitionHandle type, const srm::MetadataReader& metadata);
    // The block where a body's code starts: its entry point, following the unconditional branches to blocks only that branch enters.
    static Block* get_body_entry_point(BlockContainer* body);

private:
    enum class AsyncMethodType : std::uint8_t { Void, Task, TaskOfT, AsyncEnumerator, AsyncEnumerable };

    struct AwaitBlockData {
        ILVariable* awaiter_var = nullptr;
        const IField* awaiter_field = nullptr;
    };
    // AsyncDebugInfo.Await: where the await yields, and where the code resumes.
    struct AwaitDebugInfo {
        int yield_offset = 0;
        int resume_offset = 0;
    };

    static bool translate_this_copy_access(ILFunction* function);
    void clean_up_body_of_move_next(ILFunction* function);

    bool match_task_creation_pattern(ILFunction* function);
    bool is_potential_visual_basic_state_machine_initialiation(ILInstruction* inst, ILVariable* state_machine_var);
    bool match_visual_basic_state_machine_field_assignements(InstructionCollection<ILInstruction>& body, ILVariable* state_machine_var);
    // Matches a (potentially virtual) instance method call; its arguments, or nullptr.
    static InstructionCollection<ILInstruction>* match_call(ILInstruction* inst, const std::string& name);
    // Matches a store to the state machine.
    static bool match_state_machine_st_fld(ILInstruction* stfld, ILVariable* state_machine_var, const IField** field, ILInstruction** value);

    bool match_async_enumerator_creation_pattern(ILFunction* function);
    static bool match_enumerator_creation_new_obj(ILInstruction* inst, transforms::ILTransformContext& context, int* initial_state,
                                                  const typesystem::ITypeDefinition** state_machine_type);
    static void analyze_enumerator_ctor(const IMethod* ctor, transforms::ILTransformContext& context, const IField** builder_field,
                                        const IType** builder_type, const IField** state_field);
    void resolve_ienumerable_ienumerator_field_mapping();

    void analyze_move_next();
    bool is_async_enumerator() const { return method_type_ == AsyncMethodType::AsyncEnumerable || method_type_ == AsyncMethodType::AsyncEnumerator; }
    bool match_yield_block(BlockContainer* block_container, int pos);
    Block* check_set_result_return_block(BlockContainer* block_container, int set_result_return_block_index, std::vector<char>& blocks_analyzed);
    bool match_dispose_combined_tokens(BlockContainer* block_container, ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst,
                                       std::vector<char>& blocks_analyzed, Block** set_result_and_exit_block);
    static void match_hoisted_local_cleanup(Block* block, int& pos);
    void check_set_result_and_exit(BlockContainer* block_container, Block* block, int& pos);
    void validate_catch_block();
    void match_complete_call(Block* block, int& pos);
    bool is_builder_field_on_this(ILInstruction* inst);
    bool is_builder_or_promise_field_on_this(ILInstruction* inst);
    bool match_state_assignment(ILInstruction* inst, int* new_state);
    void analyze_dispose_async();

    void inline_body_of_move_next(ILFunction* function);
    void finalize_inline_move_next(ILFunction* function);

    void analyze_state_machine(ILFunction* function);
    bool transform_yield_break(Block* block);
    ILInstruction* find_yield_break_target(Block* block);
    bool simplify_if_dispose_mode(Block* block);
    bool normalize_await_on_completed_dual_branch(Block* block);
    bool analyze_await_block(Block* block, ILVariable** awaiter, const IField** awaiter_field, int* state, int* yield_offset);
    static ILInstruction* stack_slot_value(ILInstruction* inst);
    bool analyze_yield_return(Block* block, ILInstruction** yield_value, int* new_state);
    bool match_current_assignment(ILInstruction* inst, ILInstruction** value);

    void detect_await_pattern(ILFunction* function);
    void detect_await_pattern(Block* block);
    static ILInstruction* unwrap_conv_unknown(ILInstruction* inst);
    static const IMethod* create_dynamic_awaiter_method(transforms::ILTransformContext& context, const std::string& name);
    void coalesce_dynamic_awaiter_blocks(BlockContainer* container, transforms::ILTransformContext& context);
    bool check_await_block(Block* block, Block** resume_block, const IField** stack_field);
    bool check_resume_block(Block* block, ILVariable* awaiter_var, const IField* awaiter_field, Block* completed_block, const IField* stack_field);
    bool restore_stack(Block* block, int& pos, const IField* stack_field);

    void clean_do_finally_bodies(ILFunction* function);
    void translate_cached_fields_to_locals();

    transforms::ILTransformContext* context_ = nullptr;

    // These fields are set by match_task_creation_pattern() or match_enumerator_creation_new_obj()
    // return type of the async method; or IAsyncEnumerable{T}/IAsyncEnumerator{T}
    const IType* task_type_ = nullptr;
    // return type of the method (only the "T" for Task{T}), for async enumerators this is the type being yielded
    const IType* underlying_return_type_ = nullptr;
    AsyncMethodType method_type_ = AsyncMethodType::Void;
    const typesystem::ITypeDefinition* state_machine_type_ = nullptr;
    const IType* builder_type_ = nullptr;
    const IField* builder_field_ = nullptr;
    const IField* state_field_ = nullptr;
    int initial_state_ = 0;
    InsertionOrderedMap<const IField*, ILVariable*> field_to_parameter_map_;
    InsertionOrderedMap<ILVariable*, ILVariable*> cached_field_to_parameter_map_;
    // 'disposeMode' field (IAsyncEnumerable/IAsyncEnumerator only)
    const IField* dispose_mode_field_ = nullptr;

    // These fields are set by analyze_move_next():
    ILFunction* move_next_function_ = nullptr;
    // variable in MoveNext that caches the stateField.
    ILVariable* cached_state_var_ = nullptr;
    TryCatch* main_try_catch_ = nullptr;
    // block that is jumped to for return statements
    // Note: for async enumerators, a jump to setResultReturnBlock is a 'yield break;'
    Block* set_result_return_block_ = nullptr;
    // final state after the setResultAndExitBlock
    int final_state_ = 0;
    bool final_state_known_ = false;
    // the variable that gets returned by the setResultAndExitBlock
    ILVariable* result_var_ = nullptr;
    // block that is jumped to for 'yield return' statements
    Block* set_result_yield_block_ = nullptr;
    ILVariable* do_finally_bodies_ = nullptr;

    // These fields are set by analyze_state_machine():
    int smallest_awaiter_var_index_ = 0;
    std::unordered_set<Leave*> move_next_leaves_;

    // For each block containing an 'await', stores the awaiter variable, and the field storing the awaiter across the yield point.
    std::unordered_map<Block*, AwaitBlockData> await_blocks_;

    bool is_visual_basic_state_machine_ = false;

    int catch_handler_offset_ = 0;
    std::vector<AwaitDebugInfo> await_debug_infos_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow

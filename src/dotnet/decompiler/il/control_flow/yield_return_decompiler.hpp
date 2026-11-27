// dotnet/decompiler/il/control_flow/yield_return_decompiler.hpp -- turns the compiler's iterator state machine back into a method with
// `yield return`: finds the enumerator class a method creates, reads its constructor, get_Current, GetEnumerator, Dispose and MoveNext,
// and replaces the method's body with MoveNext's, its state changes turned into yield return and its try-finally regions rebuilt.
//
//     YieldReturnDecompiler transform;
//     transform.run(function, context);       // function->is_iterator is set when the body was an iterator's creation
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.ControlFlow: YieldReturnDecompiler, from ControlFlow/YieldReturnDecompiler.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler)..
#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/il/control_flow/state_range_analysis.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::il::control_flow {

namespace srm = cb::dotnet::metadata;

class YieldReturnDecompiler final : public transforms::IILTransform {
public:
    // The state machine's fields and the variables (parameters, `this`) they were copied from, in the order they were found.
    using FieldToVariableMap = InsertionOrderedMap<const IField*, ILVariable*>;

    void run(ILFunction* function, transforms::ILTransformContext& context) override;
    std::string name() const override { return "YieldReturnDecompiler"; }

    // Whether the type is a compiler-generated class that implements System.Collections.IEnumerator.
    static bool is_compiler_generator_enumerator(srm::TypeDefinitionHandle type, const srm::MetadataReader& metadata);

    // Matches the body of a method as a single basic block.
    static Block* single_block(ILInstruction* body);
    // Matches the newobj instruction that creates an instance of the compiler-generated enumerator helper class.
    static bool match_enumerator_creation_new_obj(ILInstruction* inst, const srm::MetadataReader& metadata, srm::TypeDefinitionHandle current_type,
                                                  srm::MethodDefinitionHandle* enumerator_ctor, srm::TypeDefinitionHandle* enumerator_type);
    // Creates ILAst for the specified method, optimized up to before the 'YieldReturn' step.
    static ILFunction* create_il_ast(srm::MethodDefinitionHandle method, transforms::ILTransformContext& context);
    static void resolve_ienumerable_ienumerator_field_mapping(srm::MethodDefinitionHandle get_enumerator_method, transforms::ILTransformContext& context,
                                                              FieldToVariableMap& field_to_parameter_map);
    // Translates all field accesses in `inst` to local variable accesses.
    static void translate_fields_to_local_access(ILFunction* function, ILInstruction* inst, FieldToVariableMap& field_to_variable_map,
                                                 bool is_compiled_with_mono = false);

private:
    struct DecompiledFinallyMethod {
        std::optional<int> outer_state;
        ILFunction* function = nullptr;
    };
    // The state of ConvertBody's local functions.
    struct ConvertBodyContext {
        BlockContainer* old_body = nullptr;
        BlockContainer* new_body = nullptr;
        util::LongDict<Block*> block_state_map;
    };

    bool match_enumerator_creation_pattern(ILFunction* function);
    bool match_enumerator_creation_new_obj(ILInstruction* inst);
    bool match_mono_enumerator_creation_new_obj(ILInstruction* inst);
    void analyze_ctor();
    void analyze_current_property();
    void resolve_ienumerable_ienumerator_field_mapping();
    void construct_exception_table();
    BlockContainer* analyze_move_next(ILFunction* function);
    void propagate_copies_of_fields(BlockContainer* body);
    BlockContainer* convert_body(BlockContainer* old_body, StateRangeAnalysis& range_analysis);
    void convert_branch_after_yield_return(ConvertBodyContext& ctx, Block* new_block, Block* old_block, int pos);
    Block* split_block(ConvertBodyContext& ctx, Block* new_block, ILInstruction* old_inst);
    ILInstruction* make_go_to(ConvertBodyContext& ctx, int v);
    void update_branch_targets(ConvertBodyContext& ctx, ILInstruction* inst);
    [[noreturn]] static void report_error(ILInstruction* inst);
    void decompile_finally_blocks();
    void reconstruct_try_finally_blocks(ILFunction* iterator_function);
    bool is_state_assignment(ILInstruction* inst) const;
    std::optional<int> get_new_state(Block* block) const;
    void clean_skip_finally_bodies(ILFunction* function);
    void clean_do_finally_bodies(ILFunction* function);
    void clean_finally_state_checks(ILFunction* function);
    bool is_method(srm::MethodDefinitionHandle method, const std::string& name) const;
    srm::MethodDefinitionHandle find_method(srm::TypeDefinitionHandle type, const std::string& name) const;

    transforms::ILTransformContext* context_ = nullptr;
    const srm::MetadataReader* metadata_ = nullptr;

    // The type that contains the function being decompiled.
    srm::TypeDefinitionHandle current_type_;
    // The compiler-generated enumerator class. Set in match_enumerator_creation_pattern().
    srm::TypeDefinitionHandle enumerator_type_;
    // The constructor of the compiler-generated enumerator class. Set in match_enumerator_creation_pattern().
    srm::MethodDefinitionHandle enumerator_ctor_;
    // Set in match_enumerator_creation_pattern().
    bool is_compiled_with_mono_ = false;
    // Set in match_enumerator_creation_pattern() or construct_exception_table().
    bool is_compiled_with_visual_basic_ = false;
    // If this is true, then is_compiled_with_visual_basic_ is also true. Set in match_enumerator_creation_pattern().
    bool is_compiled_with_legacy_visual_basic_ = false;
    // The dispose method of the compiler-generated enumerator class. Set in construct_exception_table().
    srm::MethodDefinitionHandle dispose_method_;
    // The field in the compiler-generated class holding the current state of the state machine. Set in analyze_ctor() for MS,
    // match_enumerator_creation_pattern() or analyze_move_next() for Mono.
    const IField* state_field_ = nullptr;
    // The backing field of the 'Current' property in the compiler-generated class. Set in analyze_current_property().
    const IField* current_field_ = nullptr;
    // The disposing field of the compiler-generated enumerator class. Set in construct_exception_table() for assembly compiled with Mono.
    const IField* disposing_field_ = nullptr;
    // Maps the fields of the compiler-generated class to the original parameters. Set in match_enumerator_creation_pattern() and
    // resolve_ienumerable_ienumerator_field_mapping().
    FieldToVariableMap field_to_parameter_map_;
    // The information extracted from the Dispose() method: for each "Finally Method", the set of states for which the method is being
    // called (empty for Mono and Visual Basic). Set in construct_exception_table().
    std::optional<InsertionOrderedMap<const IMethod*, util::LongSet>> finally_method_to_state_range_;
    // For each finally method, the target state when entering the finally block, and the decompiled code of the finally method body.
    std::unordered_map<const IMethod*, DecompiledFinallyMethod> decompiled_finally_methods_;
    // Temporary stores for 'yield break'.
    std::vector<StLoc*> return_stores_;
    // Local bool variable in MoveNext() that signifies whether to skip finally bodies.
    ILVariable* skip_finally_bodies_ = nullptr;
    // Local bool variable in MoveNext() that signifies whether to execute finally bodies.
    ILVariable* do_finally_bodies_ = nullptr;
    // Variables that might hold copies of the generated state field.
    std::vector<ILVariable*> cached_state_vars_;
};

}  // namespace cb::dotnet::decompiler::il::control_flow

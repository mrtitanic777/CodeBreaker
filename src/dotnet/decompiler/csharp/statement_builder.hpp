// dotnet/decompiler/csharp/statement_builder.hpp -- the C# StatementBuilder: turns the ILAst of one function into C# statements. Blocks,
// block containers (loops, switches, plain scopes), branches and leaves (break, continue, return, goto and labels), try/catch/finally,
// using, lock, foreach, fixed, yield and the statements that wrap an expression (translated by the ExpressionBuilder).
//
//     StatementBuilder builder(type_system, decompilation_context, function, settings, decompile_run, cancellation_token);
//     BlockStatement* body = builder.convert_as_block(function->body());
//
// The builder is an ILVisitorT<TranslatedStatement>: each visit_* function turns one instruction into a statement carrying the instruction
// as an annotation. The loops, the switch and the foreach/using/fixed forms are rebuilt from the shapes the ILAst transforms left.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: StatementBuilder, from StatementBuilder.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Differences: C#'s iterator methods return vectors (create_typed_case_label); a string key's
// constant is the UTF-16 string of the .NET value; local functions are ordered by name, ignoring case (upstream's culture-sensitive order).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/csharp/instruction_facts.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/translated_expression.hpp"
#include "dotnet/decompiler/csharp/translated_statement.hpp"
#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::dotnet::decompiler {
class DecompilerSettings;
class DecompileRun;
}  // namespace cb::dotnet::decompiler
namespace cb::dotnet::decompiler::typesystem {
class DecompilerTypeSystem;
}

namespace cb::translate::ir {
struct Node;
}

namespace cb::dotnet::decompiler::csharp {

class ExpressionBuilder;
class L2Body;

class StatementBuilder final : public il::ILVisitorT<TranslatedStatement> {
public:
    StatementBuilder(const typesystem::DecompilerTypeSystem& type_system, const typesystem::ITypeResolveContext& decompilation_context,
                     il::ILFunction* current_function, const DecompilerSettings& settings, DecompileRun& decompile_run,
                     const util::CancellationToken& cancellation_token, L2Body* l2 = nullptr);
    ~StatementBuilder() override;
    StatementBuilder(const StatementBuilder&) = delete;
    StatementBuilder& operator=(const StatementBuilder&) = delete;

    // The expression builder of this function (upstream's exprBuilder).
    ExpressionBuilder& expr_builder() const { return *expr_builder_; }
    DecompileRun& decompile_run;

    il::BlockContainer* current_return_container = nullptr;
    const typesystem::IType* current_result_type = nullptr;
    bool current_is_iterator = false;

    bool emit_as_ref_read_only = false;

    // The statement of the instruction (throws OperationCanceledError when canceled).
    Statement* convert(il::ILInstruction* inst);
    // The statement of the instruction as a block (a single statement is put in a new block).
    syntax::BlockStatement* convert_as_block(il::ILInstruction* inst);

    // The constants of a case label of value i in a switch on a value of the type (one per key of the string map for a switch on string).
    std::vector<semantics::ConstantResolveResult*> create_typed_case_label(
        std::int64_t i, const typesystem::IType* type, const std::vector<std::pair<std::optional<std::string>, int>>* map = nullptr);

    // The designation of a deconstruction's pattern: `(a, (b, _))`; the variables of a foreach's get the kind ForeachLocal.
    // (`l2`: the body as L2, told of a foreach variable's new kind)
    static syntax::VariableDesignation* translate_deconstruction_designation(il::DeconstructInstruction* inst, bool is_foreach, L2Body* l2 = nullptr);

    // ---- the visitor ----

    TranslatedStatement default_visit(il::ILInstruction& inst) override;
    TranslatedStatement visit_is_inst(il::IsInst& inst) override;
    TranslatedStatement visit_st_loc(il::StLoc& inst) override;
    TranslatedStatement visit_st_obj(il::StObj& inst) override;
    TranslatedStatement visit_nop(il::Nop& inst) override;
    TranslatedStatement visit_if_instruction(il::IfInstruction& inst) override;
    TranslatedStatement visit_switch_instruction(il::SwitchInstruction& inst) override;
    TranslatedStatement visit_branch(il::Branch& inst) override;
    TranslatedStatement visit_leave(il::Leave& inst) override;
    TranslatedStatement visit_throw(il::Throw& inst) override;
    TranslatedStatement visit_rethrow(il::Rethrow& inst) override;
    TranslatedStatement visit_yield_return(il::YieldReturn& inst) override;
    TranslatedStatement visit_try_catch(il::TryCatch& inst) override;
    TranslatedStatement visit_try_finally(il::TryFinally& inst) override;
    TranslatedStatement visit_try_fault(il::TryFault& inst) override;
    TranslatedStatement visit_lock_instruction(il::LockInstruction& inst) override;
    TranslatedStatement visit_using_instruction(il::UsingInstruction& inst) override;
    TranslatedStatement visit_pinned_region(il::PinnedRegion& inst) override;
    TranslatedStatement visit_block(il::Block& block) override;
    TranslatedStatement visit_block_container(il::BlockContainer& container) override;
    TranslatedStatement visit_initblk(il::Initblk& inst) override;
    TranslatedStatement visit_cpblk(il::Cpblk& inst) override;
    TranslatedStatement visit_ckfinite(il::Ckfinite& inst) override;

private:
    using Match = syntax::pattern_matching::Match;

    // What the first use of the enumerator's Current in a foreach body needs (upstream's RequiredGetCurrentTransformation).
    enum class RequiredGetCurrentTransformation {
        // Foreach transformation not possible.
        NoForeach,
        // Uninline the `stloc foreachVar(call get_Current())` and insert it as first statement in the loop body.
        UseExistingVariable,
        // No store was found, so a new variable becomes the foreach variable.
        IntroduceNewVariable,
        // As above, and the value is copied to another local to allow safe modification of its value.
        IntroduceNewVariableAndLocalCopy,
        // `call get_Current()` is the tested operand of the deconstruct instruction that is the first statement of the loop body.
        Deconstruction,
    };

    syntax::SwitchStatement* translate_switch(il::BlockContainer* switch_container, const SwitchFacts* inst);
    void convert_switch_section_body(syntax::SwitchSection* ast_section, il::ILInstruction* body_inst);

    syntax::TryCatchStatement* make_try_catch(il::ILInstruction* try_block);

    bool match_get_enumerator_pattern(Expression* resource, Match* m, bool* is_async);
    Statement* transform_to_foreach(il::UsingInstruction* inst, Expression* resource);
    Statement* transform_to_foreach_without_dispose(il::Block* block, int* i);
    Statement* transform_to_foreach_without_dispose(il::StLoc* store_inst, il::BlockContainer* loop_container);
    bool enumerator_type_can_never_be_disposable(const typesystem::IType* type);
    Statement* transform_to_foreach(il::BlockContainer* container, il::BlockContainer* loop_container, il::Leave* optional_leave_after_loop,
                                    il::ILVariable* enumerator_var, bool is_async, const Match& m, il::ILInstruction* resource_expression);
    bool is_dynamic_cast_to_ienumerable(Expression* expr, Expression** dynamic_expr);
    il::BlockContainer* unwrap_nested_container_if_possible(il::BlockContainer* container, il::Leave** optional_leave_inst);
    RequiredGetCurrentTransformation detect_get_current_transformation(il::BlockContainer* using_container, il::Block* loop_body,
                                                                       il::BlockContainer* loop_container, il::ILVariable* enumerator,
                                                                       il::ILInstruction* move_next_usage, il::CallInstruction** single_getter,
                                                                       il::ILVariable** foreach_variable);
    bool can_be_deconstructed_in_foreach(il::DeconstructInstruction* deconstruction, il::ILInstruction* single_getter,
                                         il::BlockContainer* using_container, il::BlockContainer* loop_container);
    bool variable_is_only_used_in_block(il::StLoc* store_inst, il::BlockContainer* using_container, il::BlockContainer* loop_container);
    bool current_is_struct_setter_target(il::ILInstruction* inst, il::CallInstruction* single_getter);
    bool is_target_of_setter_call(il::ILInstruction* inst, const typesystem::IType* target_type);
    bool parent_is_current_getter(il::ILInstruction* inst);

    Statement* convert_loop(il::BlockContainer* container);
    syntax::BlockStatement* convert_block_container(il::BlockContainer* container, bool is_loop);
    syntax::BlockStatement* convert_block_container(syntax::BlockStatement* block_statement, il::BlockContainer* container,
                                                    const std::vector<il::Block*>& blocks, bool is_loop);
    void declare_local_functions(il::ILFunction* current_function, il::BlockContainer* container, syntax::BlockStatement* block_statement);
    syntax::LocalFunctionDeclarationStatement* translate_function(il::ILFunction* function);

    std::string ensure_unique_label(il::Block* block);
    // Stage 14: the statement kinds read from the instruction's L2 node (statement_builder.cpp lists them); null for another kind, which
    // the ILAst visitor converts.
    Statement* convert_from_l2(const translate::ir::Node& node, il::ILInstruction& inst);
    // Each kind's translation from its facts (instruction_facts.hpp), which its visitor reads from the instruction and convert_from_l2 from
    // the node.
    TranslatedStatement translate_branch(const BranchFacts& inst);
    TranslatedStatement translate_leave(const LeaveFacts& inst);
    TranslatedStatement translate_yield_return(const YieldReturnFacts& inst);
    TranslatedStatement translate_try_catch(const TryCatchFacts& inst);
    TranslatedStatement translate_try_finally(const TryHandlerFacts& inst);
    TranslatedStatement translate_try_fault(const TryHandlerFacts& inst);
    TranslatedStatement translate_lock(const LockFacts& inst);
    TranslatedStatement translate_block_container(const BlockContainerFacts& facts);
    TranslatedStatement translate_block(const BlockFacts& facts);
    TranslatedStatement translate_using(const UsingFacts& inst);
    TranslatedStatement translate_pinned_region(const PinnedRegionFacts& inst);
    TranslatedStatement translate_switch_statement(const SwitchFacts& inst);
    TranslatedStatement translate_initblk(const MemoryBlockFacts& inst);
    TranslatedStatement translate_cpblk(const MemoryBlockFacts& inst);
    TranslatedStatement translate_ckfinite(const MemoryBlockFacts& inst);
    TranslatedStatement translate_is_statement(const TypedUnaryFacts& inst);
    // An expression in a statement place (ExpressionStatement; stloc and stobj without the top-level ref, isinst as `is`).
    TranslatedStatement translate_expression_statement(il::ILInstruction& inst, bool strip_ref);
    static bool is_final_leave(il::Leave* leave);

    std::unique_ptr<ExpressionBuilder> expr_builder_;
    il::ILFunction* current_function_;
    const typesystem::DecompilerTypeSystem* type_system_;
    const typesystem::ITypeResolveContext* decompilation_context_;
    const DecompilerSettings* settings_;
    util::CancellationToken cancellation_token_;

    // Target block that a 'continue;' statement would jump to.
    il::Block* continue_target_ = nullptr;
    // Number of ContinueStatements that were created for the current continue_target_.
    int continue_count_ = 0;
    // Maps blocks to cases. A null value marks the default case; no map (nullopt) outside a switch.
    std::optional<std::unordered_map<il::Block*, semantics::ConstantResolveResult*>> case_label_mapping_;
    // Target container that a 'break;' statement would break out of.
    il::BlockContainer* break_target_ = nullptr;
    // From BlockContainer to the label name for 'goto end_of_container;'.
    std::unordered_map<il::BlockContainer*, std::string> end_container_labels_;
    std::unordered_map<il::Block*, std::string> labels_;
    std::unordered_map<std::string, int> duplicate_labels_;
};

}  // namespace cb::dotnet::decompiler::csharp

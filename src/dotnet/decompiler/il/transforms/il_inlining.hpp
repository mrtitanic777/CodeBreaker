// dotnet/decompiler/il/transforms/il_inlining.hpp -- inlining in the ILAst: a `stloc v(expr)` whose variable is loaded exactly once, in
// the next statement and with nothing in between that cannot be reordered with it, is replaced by `expr` at the load. Also the helpers
// other transforms use to ask whether an expression can be moved to a place (can_move_into, can_inline_into, ...).
//
//     ILInlining inlining;
//     inlining.run(function, context);                                  // every block of the function
//     ILInlining::inline_one_if_possible(block, pos, InliningOptions::Aggressive, context);
//     bool ok = ILInlining::can_move_into(expression, statement, target_load);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: InliningOptions, ILInlining, ExpressionClassification, from
// Transforms/ILInlining.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: the
// uses of NamedArgumentTransform (waits for its header; without it named arguments are never introduced), of
// NullableLiftingTransform::match_nullable_ctor (a copy of its few lines is used), of SyntheticRangeIndexAccessor and of
// CSharpDecompiler::is_transparent_identifier (a copy is used).
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

enum class InliningOptions : std::uint8_t {
    None = 0,
    Aggressive = 1,
    IntroduceNamedArguments = 2,
    FindDeconstruction = 4,
    AllowChangingOrderOfEvaluationForExceptions = 8,
    AllowInliningOfLdloca = 0x10,
};
constexpr InliningOptions operator|(InliningOptions a, InliningOptions b) {
    return static_cast<InliningOptions>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}
constexpr InliningOptions operator&(InliningOptions a, InliningOptions b) {
    return static_cast<InliningOptions>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
}
constexpr InliningOptions& operator|=(InliningOptions& a, InliningOptions b) { return a = a | b; }
constexpr bool has_flag(InliningOptions options, InliningOptions flag) { return (options & flag) == flag; }

// Whether an expression turns into a C# l-value that can be used to mutate a value type (the C# compiler copies a value type to a
// temporary before calling a method on it when this is not the case).
enum class ExpressionClassification : std::uint8_t { RValue, MutableLValue, ReadonlyLValue };

// Performs inlining transformations.
class ILInlining final : public IILTransform, public IBlockTransform, public IStatementTransform {
public:
    InliningOptions options = InliningOptions::None;

    void run(ILFunction* function, ILTransformContext& context) override;
    void run(Block* block, BlockTransformContext& context) override;
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "ILInlining"; }

    static InliningOptions options_for_block(Block* block, int pos, ILTransformContext& context);
    static bool inline_all_in_block(ILFunction* function, Block* block, InliningOptions options, ILTransformContext& context);
    static bool is_in_constructor_initializer(ILFunction* function, ILInstruction* inst);
    static bool is_catch_when_block(Block* block);

    // Inlines instructions before pos into block->instructions[pos]; returns the number of instructions that were inlined.
    static int inline_into(Block* block, int pos, InliningOptions options, ILTransformContext& context);
    // Aggressively inlines the stloc at block->instructions[pos] into the next instruction, if possible.
    static bool inline_if_possible(Block* block, int pos, ILTransformContext& context);
    // Inlines the stloc at block->instructions[pos] into the next instruction, if possible.
    static bool inline_one_if_possible(Block* block, int pos, InliningOptions options, ILTransformContext& context);
    static bool variable_can_be_used_for_inlining(ILVariable* v);
    // Inlines the stloc into the next instruction. Does not check whether the variable has only one use; the caller is expected to
    // validate whether inlining it has any effects on other uses.
    static bool inline_one(StLoc* stloc, InliningOptions options, ILTransformContext& context);

    // Whether the instruction, when converted into C#, turns into an l-value that can be used to mutate a value type.
    static ExpressionClassification classify_expression(ILInstruction* inst);
    // Whether the ILInstruction will turn into a C# expression that the C# compiler considers readonly.
    static bool is_readonly_reference(ILInstruction* addr);
    static bool method_requires_copy_for_readonly_lvalue(const IMethod* method, const IType* constrained_to = nullptr);
    static bool is_used_as_this_pointer_in_call(LdLoca* ldloca);
    static bool is_passed_to_in_parameter(LdLoca* ldloca);
    static bool is_read_only_span_char_ctor(const IMethod* method);

    // Whether 'expression_being_moved' can be inlined into 'expr'.
    static bool can_inline_into(ILInstruction* expr, ILVariable* v, ILInstruction* expression_being_moved);

    enum class FindResultType : std::uint8_t {
        // Found a load; inlining is possible.
        Found,
        // Load not found and re-ordering not possible. Stop the search.
        Stop,
        // Load not found, but the expression being moved can be re-ordered with regards to the tested expression, so the search for
        // the matching load may continue.
        Continue,
        // Found a load in a call, but re-ordering is not possible with regards to the other call arguments: inlining is not possible,
        // but the call might be converted to named arguments. Only used with InliningOptions::IntroduceNamedArguments.
        NamedArgument,
        // Found a deconstruction. Only used with InliningOptions::FindDeconstruction.
        Deconstruction,
    };

    struct FindResult {
        FindResultType type = FindResultType::Stop;
        ILInstruction* load_inst = nullptr;     // the ldloc or ldloca that loads the variable to be inlined
        ILInstruction* call_argument = nullptr;  // the argument of a call that needs to be promoted to a named argument

        static FindResult stop() { return {FindResultType::Stop, nullptr, nullptr}; }
        static FindResult continue_() { return {FindResultType::Continue, nullptr, nullptr}; }
        static FindResult found(ILInstruction* load_inst);
        static FindResult named_argument(ILInstruction* load_inst, ILInstruction* call_arg);
        static FindResult deconstruction(DeconstructInstruction* deconstruction);
    };

    // Finds the position to inline to.
    static FindResult find_load_in_next(ILInstruction* expr, ILVariable* v, ILInstruction* expression_being_moved, InliningOptions options);
    // Finds the first call instruction within the instructions that were inlined into inst.
    static CallInstruction* find_first_inlined_call(ILInstruction* inst);
    // Whether 'expression_being_moved' can be moved from somewhere before 'stmt' to become the replacement of 'target_load'.
    static bool can_move_into(ILInstruction* expression_being_moved, ILInstruction* stmt, ILInstruction* target_load);
    static bool can_move_into_call_virt(ILInstruction* expression_being_moved, ILInstruction* stmt, CallVirt* nested_call_virt,
                                        ILInstruction* target_load);
    // Whether arg can be un-inlined out of stmt.
    static bool can_uninline(ILInstruction* arg, ILInstruction* stmt);

private:
    static bool prefer_expressions_over_statements(ILFunction* function);
    static bool do_inline(ILVariable* v, ILInstruction* inlined_expression, ILInstruction* next, InliningOptions options,
                          ILTransformContext& context);
    static bool is_stack_alloc_span_constructor_argument(ILInstruction* load_inst);
    static bool is_generated_temporary_for_address_of(LdLoca* load_inst, ILVariable* v, ILInstruction* inlined_expression,
                                                      InliningOptions options);
    static bool is_passed_to_inline_array_as_span(LdLoca* load_inst);
    static bool is_used_as_this_pointer_in_call(LdLoca* ldloca, const IMethod** method, const IType** constrained_type);
    static bool is_used_as_this_pointer_in_field_read(LdLoca* ldloca);
    static bool is_passed_to_read_only_span_ctor(LdLoca* ldloca);
    static bool non_aggressive_inline_into(ILInstruction* next, const FindResult& find_result, ILInstruction* inlined_expression,
                                           ILVariable* v);
    static bool is_safe_for_inline_over(ILInstruction* expr, ILInstruction* expression_being_moved);
};

}  // namespace cb::dotnet::decompiler::il::transforms

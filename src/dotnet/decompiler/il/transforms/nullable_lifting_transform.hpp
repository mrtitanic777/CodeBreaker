// dotnet/decompiler/il/transforms/nullable_lifting_transform.hpp -- nullable lifting: turns the code the C# compiler emits for lifted
// operators on Nullable<T> (a HasValue test over GetValueOrDefault calls, newobj Nullable ctor) back into lifted ILAst instructions
// (lifted conversions, unary and binary operators, comparisons, `??`, three-valued bool logic) and, through NullPropagationTransform,
// into `?.`.
//
//     if (a.HasValue && b.HasValue) newobj Nullable(add(a.GetValueOrDefault(), b.GetValueOrDefault())) else default(int?)
//         =>  binary.add.lifted(ldloc a, ldloc b)
//
//     NullableLiftingTransform lifting(context);
//     lifting.run(if_instruction);                                     // as the expression transforms do
//     NullableLiftingStatementTransform statement_transform;           // for `if (!c) leave(default) ; leave(newobj Nullable(x))`
//     bool is_has_value = NullableLiftingTransform::match_has_value_call(inst, &variable);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: NullableLiftingTransform, NullableLiftingStatementTransform, from
// Transforms/NullableLiftingTransform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet
// ported: the C# resolver's CSharpOperators (LiftUserDefinedOperator, IsComparisonOperator: no user-defined operator is lifted, and none
// is taken for a comparison operator).
#pragma once

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"

namespace cb::dotnet::decompiler::il::transforms {

// Nullable lifting gets run in two places: the usual form looks at an if-else, and runs within the expression transforms; the
// statement transform handles the cases where Roslyn generates two 'ret' statements for the null/non-null cases of a lifted operator.
// (A value type upstream: made fresh for each use, it keeps the variables found by analyzing the condition.)
class NullableLiftingTransform {
public:
    explicit NullableLiftingTransform(ILTransformContext& context) : context_(&context) {}

    // Main entry point into the normal code path of this transform. Called by the expression transforms.
    bool run(IfInstruction* if_inst);
    // VS2017.8 / Roslyn 2.9 started optimizing some cases of "a.GetValueOrDefault() == b.GetValueOrDefault() && (a.HasValue & b.HasValue)"
    // to "(a.GetValueOrDefault() == b.GetValueOrDefault()) & (a.HasValue & b.HasValue)", so this secondary entry point analyses logic.and
    // as-if it was a short-circuiting &&.
    bool run(BinaryNumericInstruction* bni);
    // VS2022.10 / Roslyn 4.10.0 adds an optimization that turns a == 42 into a.GetValueOrDefault() == 42 without any HasValue check.
    void run(Comp* comp);
    bool run_statements(Block* block, int pos);

    // Matches 'call get_HasValue(arg)'.
    static bool match_has_value_call(ILInstruction* inst, ILInstruction** arg);
    // Matches 'call get_HasValue(ldloca v)'.
    static bool match_has_value_call(ILInstruction* inst, ILVariable** v);
    // Matches 'call get_HasValue(ldloca v)' for that variable.
    static bool match_has_value_call(ILInstruction* inst, const ILVariable* v);
    // Matches 'logic.not(call get_HasValue(ldloca v))'.
    static bool match_negated_has_value_call(ILInstruction* inst, const ILVariable* v);
    // Matches 'newobj Nullable{underlyingType}.ctor(arg)'.
    static bool match_nullable_ctor(ILInstruction* inst, const IType** underlying_type, ILInstruction** arg);
    // Matches 'call Nullable{T}.GetValueOrDefault(arg)'.
    static bool match_get_value_or_default(ILInstruction* inst, ILInstruction** arg);
    // Matches 'call nullableValue.GetValueOrDefault(fallback)'.
    static bool match_get_value_or_default(ILInstruction* inst, ILInstruction** nullable_value, ILInstruction** fallback);
    // Matches 'call Nullable{T}.GetValueOrDefault(ldloca v)'.
    static bool match_get_value_or_default(ILInstruction* inst, ILVariable** v);
    // Matches 'call Nullable{T}.GetValueOrDefault(ldloca v)' for that variable.
    static bool match_get_value_or_default(ILInstruction* inst, const ILVariable* v);

private:
    // The result of lifting an instruction: the new instruction and the nullable variables it considered (none when lifting failed).
    struct LiftResult {
        ILInstruction* inst = nullptr;
        std::optional<util::BitSet> bits;
    };
    struct LiftBinaryResult {
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        std::optional<util::BitSet> bits;
    };

    // Either a non-lifted IL `Comp` or a call to one of the (non-lifted) 6 comparison operators on `System.Decimal`.
    struct CompOrDecimal {
        ILInstruction* instruction = nullptr;
        ComparisonKind kind = ComparisonKind::Equality;
        ILInstruction* left = nullptr;
        ILInstruction* right = nullptr;
        bool is_lifted = false;

        const IType* left_expected_type() const;
        const IType* right_expected_type() const;
        ILInstruction* make_lifted(ComparisonKind new_comparison_kind, ILInstruction* left, ILInstruction* right) const;
    };

    bool analyze_condition(ILInstruction* condition);
    bool analyze_negated_condition(ILInstruction* condition);

    // Main entry point for lifting; called by both the expression-transform and the block transform. `if_inst` is usually the
    // IfInstruction to which `condition` belongs, but can also be a BinaryNumericInstruction.
    ILInstruction* lift(ILInstruction* if_inst, ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst);
    static bool is_generic_new_pattern(ILInstruction* comp_left, ILInstruction* comp_right, ILInstruction* true_inst, ILInstruction* false_inst);
    bool match_three_valued_logic_condition_pattern(ILInstruction* condition, ILVariable** nullable1, ILVariable** nullable2);
    static bool match_comp_or_decimal(ILInstruction* inst, CompOrDecimal* result);

    ILInstruction* lift_csharp_equality_comparison(const CompOrDecimal& value_comp, ComparisonKind new_comparison_kind, ILInstruction* has_value_test);
    // Lifts a C# comparison. This cannot be used for (in)equality comparisons where both sides are nullable (those are handled in
    // lift_csharp_equality_comparison). The output evaluates to false when any of the nullable variables is null (except for
    // Inequality, where it evaluates to true), else to what the input did, with the same side effects.
    ILInstruction* lift_csharp_comparison(const CompOrDecimal& comp, ComparisonKind new_comparison_kind);
    ILInstruction* lift_csharp_user_equality_comparison(const CompOrDecimal& has_value_comp, ComparisonKind new_comparison_kind,
                                                        ILInstruction* nested_if_inst);
    ILInstruction* lift_csharp_user_comparison(ILInstruction* true_inst, ILInstruction* false_inst);

    // Produces a lifted instruction equivalent to `(v1 != null && ... && vn != null) ? trueInst : falseInst` where v1..vn are the
    // nullable variables; null when lifting fails.
    ILInstruction* lift_normal(ILInstruction* true_inst, ILInstruction* false_inst);
    // Lifts the instruction (a subexpression of the true instruction, in which all nullable variables are non-null) into a new
    // instruction without changing the input.
    LiftResult do_lift(ILInstruction* inst);
    LiftBinaryResult do_lift_binary(ILInstruction* lhs, ILInstruction* rhs, const IType* left_expected_type, const IType* right_expected_type);
    ILInstruction* new_nullable(ILInstruction* inst, const IType* underlying_type);

    static bool match_null(ILInstruction* inst, const IType** underlying_type);
    static bool match_null(ILInstruction* inst, const IType* underlying_type);

    ILTransformContext* context_;
    std::vector<ILVariable*> nullable_vars_;
};

class NullableLiftingStatementTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "NullableLiftingStatementTransform"; }
};

}  // namespace cb::dotnet::decompiler::il::transforms

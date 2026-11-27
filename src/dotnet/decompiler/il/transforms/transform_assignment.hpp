// dotnet/decompiler/il/transforms/transform_assignment.hpp -- builds compound assignments (`x += 1`, `p.Prop *= y`, `i++`, `++i`) and
// inline assignments (`a = b = c`, `Use(field = value)`) out of the loads and stores the C# compiler emits for them. A statement
// transform; the part that turns a store whose value reads the same place into a compound assignment (handle_compound_assign) also
// runs as an expression transform from ExpressionTransforms.
//
//     TransformAssignment transform;
//     transform.run(block, pos, statement_context);
//     TransformAssignment::handle_compound_assign(store_instruction, statement_context);
//     bool truncates = TransformAssignment::is_implicit_truncation(value, type, &compilation);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: TransformAssignment, from Transforms/TransformAssignment.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported: the C# expression builder's
// GetAssignmentOperatorTypeFromMetadataName and the call builder's IsStringToReadOnlySpanCharImplicitConversion (a copy of each is
// used).
#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ICompilation;
}

namespace cb::dotnet::decompiler::il::transforms {

class TransformAssignment final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "TransformAssignment"; }

    // Transforms compound assignments where the return value is not being used, or where there's an inlined assignment within the
    // setter call.
    //
    // Patterns handled:
    // 1.
    //   callvirt set_Property(ldloc S_1, binary.op(callvirt get_Property(ldloc S_1), value))
    //   ==> compound.op.new(callvirt get_Property(ldloc S_1), value)
    // 2.
    //   callvirt set_Property(ldloc S_1, stloc v(binary.op(callvirt get_Property(ldloc S_1), value)))
    //   ==> stloc v(compound.op.new(callvirt get_Property(ldloc S_1), value))
    // 3.
    //   stobj(target, binary.op(ldobj(target), ...))
    //     where target is pure
    //   => compound.op(target, ...)
    //
    // Called by ExpressionTransforms, or after the inline-assignment transform for setters.
    static bool handle_compound_assign(ILInstruction* compound_store, StatementTransformContext& context);

    static bool is_implicit_truncation(ILInstruction* value, const IType* type, const typesystem::ICompilation* compilation,
                                       bool allow_nullable_value = false);

    enum class ImplicitTruncationResult : std::uint8_t {
        // The value is not implicitly truncated.
        ValuePreserved,
        // The value is implicitly truncated.
        ValueChanged,
        // The value is implicitly truncated, but the sign of the target type can be changed to remove the truncation.
        ValueChangedDueToSignMismatch,
    };

    // Whether 'stobj type(..., value)' would evaluate to a different value than 'value' due to implicit truncation.
    static ImplicitTruncationResult check_implicit_truncation(ILInstruction* value, const IType* type, const typesystem::ICompilation* compilation,
                                                              bool allow_nullable_value = false);

private:
    using FinalizeMatch = std::function<void(ILTransformContext&)>;

    bool transform_inline_assignment_st_obj_or_call(Block* block, int pos);
    bool transform_inline_assignment_local(Block* block, int pos);
    bool transform_pre_inc_dec_operator_with_inline_store(Block* block, int pos);
    bool transform_post_inc_dec_operator_with_inline_store(Block* block, int pos);
    bool transform_post_inc_dec_operator(Block* block, int i);

    static ImplicitTruncationResult common_implicit_truncation(ImplicitTruncationResult left, ImplicitTruncationResult right);
    static const IType* swap_sign(const IType* type, const typesystem::ICompilation& compilation);
    static ILInstruction* unwrap_small_integer_conv(ILInstruction* inst, Conv** conv);
    static bool validate_compound_assign(BinaryNumericInstruction* binary, Conv* conv, const IType* target_type, const DecompilerSettings& settings);
    static bool matching_getter_and_setter_calls(CallInstruction* getter_call, CallInstruction* setter_call, FinalizeMatch* finalize_match);
    // Whether 'inst' is a possible store for use as a compound store. Every call should be followed by an is_matching_compound_load call.
    // store_type: the type of the value being stored. value: the value being stored (analyzed further to detect compound assignments).
    static bool is_compound_store(ILInstruction* inst, const IType** store_type, ILInstruction** value, const typesystem::ICompilation& compilation);
    // Whether 'load' and 'store' both access the same store, and can be combined to a compound assignment. 'store' must have been
    // tested via is_compound_store. 'finalize_match', if set to a non-empty function, fixes up minor mismatches between getter and
    // setter. 'forbidden_variable' (if not null) makes this return false when it is used in the load/store instructions: some
    // transforms move a store around, which is only valid if the variable stored to does not occur in the compound load/store.
    // 'previous_instruction' is the instruction preceding the load.
    static bool is_matching_compound_load(ILInstruction* load, ILInstruction* store, ILInstruction** target, CompoundTargetKind* target_kind,
                                          FinalizeMatch* finalize_match, ILVariable* forbidden_variable = nullptr,
                                          ILInstruction* previous_instruction = nullptr);
    static bool is_same_member(const IMember* a, const IMember* b);

    StatementTransformContext* context_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::il::transforms

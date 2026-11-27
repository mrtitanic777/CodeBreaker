// dotnet/decompiler/il/transforms/user_defined_logic_transform.hpp -- recognizes the C# compiler's expansion of a user-defined
// short-circuiting operator (`a && b` calling `op_False`/`op_BitwiseAnd`, `a || b` calling `op_True`/`op_BitwiseOr`) in the legacy
// compiler's and Roslyn's shapes, and the same for `dynamic`, and builds the `user.logic` / `dynamic.logic.operator` instruction.
//
//     UserDefinedLogicTransform transform;
//     transform.run(block, pos, statement_context);
//     ILInstruction* logic = UserDefinedLogicTransform::transform(condition, true_inst, false_inst);   // nullptr if no match
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: UserDefinedLogicTransform, from Transforms/UserDefinedLogicTransform.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class UserDefinedLogicTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "UserDefinedLogicTransform"; }

    //     if (call op_False(ldloc lhsVar)) ldloc lhsVar else call op_BitwiseAnd(ldloc lhsVar, rhsInst)
    //  -> user.logic op_BitwiseAnd(ldloc lhsVar, rhsInst)
    // or
    //     if (call op_True(ldloc lhsVar)) ldloc lhsVar else call op_BitwiseOr(ldloc lhsVar, rhsInst)
    //  -> user.logic op_BitwiseOr(ldloc lhsVar, rhsInst)
    static ILInstruction* transform(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst);
    static ILInstruction* transform_dynamic(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst);

private:
    bool roslyn_optimized(Block* block, int pos, StatementTransformContext& context);
    bool legacy_pattern(Block* block, int pos, StatementTransformContext& context);
    static bool match_condition(ILInstruction* condition, ILVariable** v, std::string* name);
    static bool match_bitwise_call(Call* call, ILVariable* v, const std::string& condition_method_name);
};

}  // namespace cb::dotnet::decompiler::il::transforms

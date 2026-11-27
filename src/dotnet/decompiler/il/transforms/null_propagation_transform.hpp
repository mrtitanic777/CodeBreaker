// dotnet/decompiler/il/transforms/null_propagation_transform.hpp -- recognizes the null-conditional operator: code like
// `v != null ? v.M() : null` becomes `nullable.rewrap(call M(nullable.unwrap(ldloc v)))`, and `if (x != null) x.M();` the same as a
// statement. Works for reference types, for Nullable<T> (by value and by reference) and for unconstrained generic types.
//
//     v != null ? call get_Length(ldloc v) : null           =>   nullable.rewrap(call get_Length(nullable.unwrap(ldloc v)))
//
//     NullPropagationTransform transform(context);
//     ILInstruction* result = transform.run(condition, true_inst, false_inst);      // as NullableLiftingTransform does
//     NullPropagationStatementTransform statement_transform;                        // `if (x != null) x.M();`
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: NullPropagationTransform, NullPropagationStatementTransform, from
// Transforms/NullPropagationTransform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet
// ported: the C# resolver's CanTransformToExtensionMethodCall (an extension method call never takes part in an access chain).
#pragma once

#include <cstdint>
#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class NullPropagationTransform {
public:
    explicit NullPropagationTransform(ILTransformContext& context) : context_(&context) {}

    // Whether the if instruction is a logic.and / logic.or in a condition: those are excluded to avoid turning
    // "logic.and(comp(interfaces != ldnull), call get_Count(interfaces))" into "if ((interfaces?.Count ?? 0) != 0)".
    static bool is_protected_if_inst(IfInstruction* if_inst);

    // Checks whether "condition ? trueInst : falseInst" can be simplified using the null-conditional operator; returns the replacement
    // instruction, or null if no replacement is possible. The condition must be positive (not a logic.not).
    ILInstruction* run(ILInstruction* condition, ILInstruction* true_inst, ILInstruction* false_inst);

    // if (x != null) x.AccessChain(); => x?.AccessChain();
    void run_statements(Block* block, int pos);

private:
    enum class Mode : std::uint8_t {
        // reference type or generic type (comparison is 'comp(ldloc(testedVar) == null)')
        ReferenceType,
        // nullable type, used by value (comparison is 'call get_HasValue(ldloca(testedVar))')
        NullableByValue,
        // nullable type, used by reference (comparison is 'call get_HasValue(ldloc(testedVar))')
        NullableByReference,
        // unconstrained generic type (see the pattern described at transform_null_propagation_on_unconstrained_generic_expression)
        UnconstrainedType,
    };
    static const char* mode_name(Mode mode);

    // testedVar != null ? nonNullInst : nullInst
    ILInstruction* try_null_propagation(ILVariable* tested_var, ILInstruction* non_null_inst, ILInstruction* null_inst, Mode mode);
    void try_null_prop_for_void_call(ILVariable* tested_var, Mode mode, Block* body, IfInstruction* if_inst);
    bool is_valid_access_chain(ILVariable* tested_var, Mode mode, ILInstruction* inst, ILInstruction** final_load);
    static bool is_getter(const IMethod* method);
    void introduce_unwrap(ILVariable* tested_var, ILInstruction* var_load, Mode mode);

    bool transform_null_propagation_on_unconstrained_generic_expression(Block* block, int pos, ILVariable** target, ILInstruction** non_null_inst,
                                                                        ILInstruction** null_inst, Block** end_block);
    bool match_st_loc_result_temporary(Block* block, int pos, const IType* type, ILVariable* target, ILVariable* default_temporary,
                                       ILInstruction* fallback_block, ILInstruction** non_null_inst, ILInstruction** null_inst, Block** end_block);
    bool match_leave_result(Block* block, int pos, const IType* type, ILVariable* target, ILVariable* default_temporary, ILInstruction* fallback_block,
                            ILInstruction** non_null_inst, ILInstruction** null_inst);
    bool is_fallback_block(Block* block, const IType* type, ILVariable* target, ILVariable* default_temporary, ILVariable* result_temporary,
                           ILInstruction* end_block_or_leave_container, ILInstruction** null_inst);

    ILTransformContext* context_;
};

class NullPropagationStatementTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "NullPropagationStatementTransform"; }
};

}  // namespace cb::dotnet::decompiler::il::transforms

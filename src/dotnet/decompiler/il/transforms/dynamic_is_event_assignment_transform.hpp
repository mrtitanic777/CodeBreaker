// dotnet/decompiler/il/transforms/dynamic_is_event_assignment_transform.hpp -- a dynamic `d.Event += b` is lowered by the C# compiler to a
// runtime is-event check that dispatches between the event accessor and a compound assignment. This transform collapses that diamond
// back to one `dynamic.compound.op`, when it appears as an if/else (cached or not) or, where the optimizer returns the result, as two
// leaves.
//
//     DynamicIsEventAssignmentTransform transform;
//     transform.run(block, pos, statement_context);
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: DynamicIsEventAssignmentTransform, from
// Transforms/DynamicIsEventAssignmentTransform.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class DynamicIsEventAssignmentTransform final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "DynamicIsEventAssignmentTransform"; }

private:
    // stloc V_1(dynamic.isevent (target))
    // if (logic.not(ldloc V_1)) Block IL_004a {
    //     stloc V_2(dynamic.getmember B(target))
    // }
    // [stloc copyOfValue(value)]
    // if (logic.not(ldloc V_1)) Block IL_0149 {
    //     dynamic.setmember.compound B(target, dynamic.binary.operator AddAssign(ldloc V_2, value))
    // } else Block IL_0151 {
    //     dynamic.invokemember.invokespecial.discard add_B(target, value)
    // }
    // =>
    // dynamic.compound.op (dynamic.getmember B(target), value)
    //
    // Only the cached lowering reaches here; ExpressionTransforms::transform_dynamic_add_assign_or_remove_assign collapses the uncached
    // if/else directly, where dynamic.isevent is already the condition. When the result is used the same diamond is nested as a
    // value-if inside the consuming expression, which the descendants walk still finds.
    static bool try_collapse_if_else_diamond(Block* block, int pos, StatementTransformContext& context);
    // The two-leaves form the optimizer emits instead of an if/else when the result is returned.
    // Cached (getmember still behind the flag cache):
    // stloc V_1(dynamic.isevent (target))
    // if (logic.not(ldloc V_1)) Block { stloc V_2(dynamic.getmember B(target)) }
    // [stloc copyOfValue(value)]
    // if (ldloc V_1) Block { leave X(dynamic.invokemember add_B(target, value)) }
    // leave X(dynamic.setmember.compound B(target, dynamic.binary.operator AddAssign(ldloc V_2, value)))
    // =>
    // leave X(dynamic.compound.op (dynamic.getmember B(target), value))
    //
    // Uncached (dynamic.isevent inlined into the condition, compound leave already collapsed, so only the redundant is-event branch
    // remains to drop):
    // if (dynamic.isevent (target)) Block { leave X(dynamic.invokemember add_B(target, value)) }
    // leave X(dynamic.compound.op AddAssign(dynamic.getmember B(target), value))
    // =>
    // leave X(dynamic.compound.op AddAssign(dynamic.getmember B(target), value))
    static void try_collapse_return_form(Block* block, int pos, StatementTransformContext& context);
    // The optional flag/getmember cache prefix of the cached lowering:
    // stloc V_1(dynamic.isevent (target))
    // if (logic.not(ldloc V_1)) Block { stloc V_2(dynamic.getmember B(target)) }
    // [stloc copyOfValue(value)]
    // Reports diamond_pos = the index just past the prefix where the diamond starts. Returns false with diamond_pos = pos for the
    // uncached lowering, which has no prefix.
    static bool match_cache_prefix(Block* block, int pos, DynamicIsEventInstruction** is_event, ILVariable** flag_var, ILVariable** get_member_var,
                                   DynamicGetMemberInstruction** get_member_inst, int* diamond_pos, ILVariable** value_variable, ILInstruction** value);
    // After the collapse the copy-of-value temporary is loaded exactly once (its use in the dropped add branch is gone), so inline it
    // and remove its store, which sits at `pos`.
    static void inline_copy_of_value(Block* block, int pos, ILVariable* value_variable, ILInstruction* value);
    // dynamic.invokemember add_B/remove_B(target, value): the event accessor paired with a compound, its name being 'add_'/'remove_'
    // + member for the operation and its two arguments the same target and value as the compound.
    static bool match_event_accessor(DynamicInvokeMemberInstruction* add_invoke, ILInstruction* target, ILInstruction* value, ExpressionType operation,
                                     const std::string& member_name);
    // The two-leaves diamond shared by the cached and uncached return lowerings:
    // if (condition) Block { leave X(dynamic.invokemember add_B(target, value)) }
    // leave X(compound)
    static bool match_is_event_return_diamond(ILInstruction* if_inst, ILInstruction* leave_inst, ILInstruction** condition,
                                              DynamicInvokeMemberInstruction** add_invoke, ILInstruction** compound_value);
    // if (dynamic.isevent (target)) Block { leave X(dynamic.invokemember add_B(target, value)) }
    // leave X(dynamic.compound.op AddAssign(dynamic.getmember B(target), value))
    static bool match_uncached_is_event_return_assignment(ILInstruction* if_inst, ILInstruction* leave_inst);
    // if (ldloc V_1) Block { leave X(dynamic.invokemember add_B(target, value)) }
    // leave X(dynamic.setmember.compound B(target, dynamic.binary.operator AddAssign(ldloc V_2, value)))
    //
    // get_member_var_use is the 'ldloc V_2' cache load the caller rewrites to the real dynamic.getmember before the setmember collapses
    // to a compound assignment.
    static bool match_is_event_return_assignment(ILInstruction* if_inst, ILInstruction* leave_inst, DynamicIsEventInstruction* is_event, ILVariable* flag_var,
                                                 ILVariable* get_member_var, DynamicSetMemberInstruction** set_member_inst,
                                                 ILInstruction** get_member_var_use);
    // dynamic.setmember.compound B(target, dynamic.binary.operator op(ldloc V_2, value)) paired with dynamic.invokemember
    // add_B/remove_B(target, value). V_2 is the getmember cache load, returned as get_member_var_use.
    static bool match_cached_event_compound(ILInstruction* compound, DynamicInvokeMemberInstruction* add_invoke, DynamicIsEventInstruction* is_event,
                                            ILVariable* get_member_var, DynamicSetMemberInstruction** set_member_inst, ILInstruction** get_member_var_use);
    // Matches both the expression and statement form of:
    // if (logic.not(ldloc V_1)) Block IL_0149 {
    //     dynamic.setmember.compound B(target, dynamic.binary.operator AddAssign(ldloc V_2,  value))
    // } else Block IL_0151 {
    //     dynamic.invokemember.invokespecial.discard add_B(target, value)
    // }
    static bool match_is_event_assignment_if_instruction(ILInstruction* if_inst, DynamicIsEventInstruction* is_event, ILVariable* flag_var,
                                                         ILVariable* get_member_var, DynamicSetMemberInstruction** set_member_inst,
                                                         ILInstruction** get_member_var_use, ILInstruction** is_event_condition_use);
    // if (logic.not(ldloc V_1)) Block IL_004a {
    //     stloc V_2(dynamic.getmember B(target))
    // }
    static bool match_lhs_cache_if_instruction(ILInstruction* if_inst, ILVariable* flag_var, StLoc** cache_store);
    static bool match_flag_equals_zero(ILInstruction* condition, ILVariable* flag_var);
};

}  // namespace cb::dotnet::decompiler::il::transforms

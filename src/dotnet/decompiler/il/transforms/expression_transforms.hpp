// dotnet/decompiler/il/transforms/expression_transforms.hpp -- the collection of expression clean-ups that run after inlining: they
// detect simple patterns (`cgt.un(x, ldnull)`, `comp(x != 0)` under an `if`, `conv(ldlen)`, a stloc in both branches of an `if`, a
// `switch` whose sections only leave or store, catch variables, dynamic compound assignments ...) and replace each with the
// instruction that says it better. Also the entry point other transforms use to clean up one statement they just made.
//
//     ExpressionTransforms::run_on_single_statement(if_inst, context);   // an `if` built by a control flow transform
//     ExpressionTransforms transform;
//     transform.run(block, pos, statement_context);                      // as a statement transform
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: ExpressionTransforms, from Transforms/ExpressionTransforms.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler)..
#pragma once

#include <string>

#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class ExpressionTransforms final : public ILVisitor, public IStatementTransform {
public:
    // Runs the transforms on `statement`, a direct child of a block.
    static void run_on_single_statement(ILInstruction* statement, ILTransformContext& context);

    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "ExpressionTransforms"; }

    // op is either add or remove/subtract:
    //   if (dynamic.isevent (target)) { dynamic.invokemember.invokespecial.discard op_Name(target, value) }
    //   else { dynamic.compound.op (dynamic.getmember Name(target), value) }
    // =>
    //   dynamic.compound.op (dynamic.getmember Name(target), value)
    static bool transform_dynamic_add_assign_or_remove_assign(IfInstruction* inst, ILTransformContext& context);
    // dynamic.setmember.compound Name(target, dynamic.binary.operator op(dynamic.getmember Name(target), value))
    // =>
    // dynamic.compound.op (dynamic.getmember Name(target), value)
    static void transform_dynamic_set_member_instruction(DynamicSetMemberInstruction* inst, StatementTransformContext& context);

    void default_visit(ILInstruction& inst) override;
    void visit_block_container(BlockContainer& container) override;
    void visit_block(Block& block) override;
    void visit_comp(Comp& inst) override;
    void visit_conv(Conv& inst) override;
    void visit_box(Box& inst) override;
    void visit_ld_elema(LdElema& inst) override;
    void visit_new_arr(NewArr& inst) override;
    void visit_call(Call& inst) override;
    void visit_call_virt(CallVirt& inst) override;
    void visit_new_obj(NewObj& inst) override;
    void visit_ld_obj(LdObj& inst) override;
    void visit_ld_obj_if_ref(LdObjIfRef& inst) override;
    void visit_st_obj(StObj& inst) override;
    void visit_st_loc(StLoc& inst) override;
    void visit_if_instruction(IfInstruction& inst) override;
    void visit_dynamic_set_member_instruction(DynamicSetMemberInstruction& inst) override;
    void visit_dynamic_set_index_instruction(DynamicSetIndexInstruction& inst) override;
    void visit_binary_numeric_instruction(BinaryNumericInstruction& inst) override;
    void visit_try_catch_handler(TryCatchHandler& inst) override;

private:
    // The context of the statement being transformed.
    StatementTransformContext* context_ = nullptr;

    void clean_up_array_indices(InstructionCollection<ILInstruction>& indices);
    void visit_logic_not(Comp* inst, ILInstruction* arg);
    bool transform_delegate_ctor_ld_virt_ftn_to_ld_virt_delegate(NewObj* inst, LdVirtDelegate** ld_virt_delegate);
    bool transform_span_t_ctor_containing_stack_alloc(NewObj* new_obj, ILInstruction** localloc_span);
    bool matches_element_count(ILInstruction* size_in_bytes_instr, const IType* element_type, ILInstruction* element_count_instr2);
    bool transform_decimal_field_to_constant(LdObj* inst, LdcDecimal** result);
    IfInstruction* handle_conditional_operator(IfInstruction* inst);
    void handle_switch_expression(BlockContainer* container, SwitchInstruction* switch_inst);
    void transform_catch_variable(TryCatchHandler* handler, Block* entry_point, bool is_catch_block);
    void transform_catch_when(TryCatchHandler* handler, Block* entry_point);
};

}  // namespace cb::dotnet::decompiler::il::transforms

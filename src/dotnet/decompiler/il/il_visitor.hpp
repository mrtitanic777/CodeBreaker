// dotnet/decompiler/il/il_visitor.hpp -- visitors of the ILAst: one visit_* method per instruction class, each falling back on
// default_visit. ILVisitor returns nothing; ILVisitorT<T> returns a T; ILVisitorCT<C, T> also takes a context.
//
//     struct CountCalls : ILVisitor {
//         int calls = 0;
//         void default_visit(ILInstruction& inst) override { for (auto* c : inst.children()) c->accept_visitor(*this); }
//         void visit_call(Call& inst) override { ++calls; default_visit(inst); }
//     };
//     int n = accept_visitor(*inst, my_typed_visitor);   // ILVisitorT<T>
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ILVisitor, ILVisitor<T>, ILVisitor<C, T> and the instructions' AcceptVisitor, from
// Instructions.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). C# virtual generic methods have
// no C++ counterpart, so the dispatch is one switch over the opcode (CB_IL_INSTRUCTIONS lists the instruction classes in opcode order).
#pragma once

#include "dotnet/decompiler/il/instructions.hpp"

namespace cb::dotnet::decompiler::il {

// X(OpCode and class name, visit method suffix), in opcode order.
#define CB_IL_INSTRUCTIONS(X)                                                                                                                 \
    X(InvalidBranch, invalid_branch) X(InvalidExpression, invalid_expression) X(Nop, nop) X(ILFunction, il_function)                     \
    X(BlockContainer, block_container) X(Block, block) X(PinnedRegion, pinned_region)                                                    \
    X(BinaryNumericInstruction, binary_numeric_instruction) X(NumericCompoundAssign, numeric_compound_assign)                           \
    X(UserDefinedCompoundAssign, user_defined_compound_assign) X(DynamicCompoundAssign, dynamic_compound_assign) X(BitNot, bit_not)     \
    X(Arglist, arglist) X(Branch, branch) X(Leave, leave) X(IfInstruction, if_instruction)                                               \
    X(NullCoalescingInstruction, null_coalescing_instruction) X(SwitchInstruction, switch_instruction) X(SwitchSection, switch_section) \
    X(TryCatch, try_catch) X(TryCatchHandler, try_catch_handler) X(TryFinally, try_finally) X(TryFault, try_fault)                       \
    X(LockInstruction, lock_instruction) X(UsingInstruction, using_instruction) X(DebugBreak, debug_break) X(Comp, comp) X(Call, call)  \
    X(CallVirt, call_virt) X(CallIndirect, call_indirect) X(Ckfinite, ckfinite) X(Conv, conv) X(LdLoc, ld_loc) X(LdLoca, ld_loca)        \
    X(StLoc, st_loc) X(AddressOf, address_of) X(ThreeValuedBoolAnd, three_valued_bool_and) X(ThreeValuedBoolOr, three_valued_bool_or)   \
    X(NullableUnwrap, nullable_unwrap) X(NullableRewrap, nullable_rewrap) X(LdStr, ld_str) X(LdStrUtf8, ld_str_utf8) X(LdcI4, ldc_i4)   \
    X(LdcI8, ldc_i8) X(LdcF4, ldc_f4) X(LdcF8, ldc_f8) X(LdcDecimal, ldc_decimal) X(LdNull, ld_null) X(LdFtn, ld_ftn)                    \
    X(LdVirtFtn, ld_virt_ftn) X(LdVirtDelegate, ld_virt_delegate) X(LdTypeToken, ld_type_token) X(LdMemberToken, ld_member_token)        \
    X(LocAlloc, loc_alloc) X(LocAllocSpan, loc_alloc_span) X(Cpblk, cpblk) X(Initblk, initblk) X(LdFlda, ld_flda) X(LdsFlda, lds_flda)   \
    X(CastClass, cast_class) X(IsInst, is_inst) X(LdObj, ld_obj) X(LdObjIfRef, ld_obj_if_ref) X(StObj, st_obj) X(Box, box)               \
    X(Unbox, unbox) X(UnboxAny, unbox_any) X(NewObj, new_obj) X(NewArr, new_arr) X(DefaultValue, default_value) X(Throw, throw)          \
    X(Rethrow, rethrow) X(SizeOf, size_of) X(LdLen, ld_len) X(LdElema, ld_elema) X(LdElemaInlineArray, ld_elema_inline_array)            \
    X(GetPinnableReference, get_pinnable_reference) X(StringToInt, string_to_int) X(ExpressionTreeCast, expression_tree_cast)          \
    X(UserDefinedLogicOperator, user_defined_logic_operator) X(DynamicLogicOperatorInstruction, dynamic_logic_operator_instruction)    \
    X(DynamicBinaryOperatorInstruction, dynamic_binary_operator_instruction)                                                          \
    X(DynamicUnaryOperatorInstruction, dynamic_unary_operator_instruction) X(DynamicConvertInstruction, dynamic_convert_instruction)   \
    X(DynamicGetMemberInstruction, dynamic_get_member_instruction) X(DynamicSetMemberInstruction, dynamic_set_member_instruction)      \
    X(DynamicGetIndexInstruction, dynamic_get_index_instruction) X(DynamicSetIndexInstruction, dynamic_set_index_instruction)          \
    X(DynamicInvokeMemberInstruction, dynamic_invoke_member_instruction)                                                              \
    X(DynamicInvokeConstructorInstruction, dynamic_invoke_constructor_instruction) X(DynamicInvokeInstruction, dynamic_invoke_instruction) \
    X(DynamicIsEventInstruction, dynamic_is_event_instruction) X(MatchInstruction, match_instruction) X(MakeRefAny, make_ref_any)       \
    X(RefAnyType, ref_any_type) X(RefAnyValue, ref_any_value) X(YieldReturn, yield_return) X(Await, await)                               \
    X(DeconstructInstruction, deconstruct_instruction) X(DeconstructResultInstruction, deconstruct_result_instruction) X(AnyNode, any_node)

class ILVisitor {
public:
    virtual ~ILVisitor() = default;
    virtual void default_visit(ILInstruction& inst) = 0;
#define CB_IL_VISIT(Class, name) \
    virtual void visit_##name(Class& inst) { default_visit(inst); }
    CB_IL_INSTRUCTIONS(CB_IL_VISIT)
#undef CB_IL_VISIT
};

template <class T>
class ILVisitorT {
public:
    virtual ~ILVisitorT() = default;
    virtual T default_visit(ILInstruction& inst) = 0;
#define CB_IL_VISIT(Class, name) \
    virtual T visit_##name(Class& inst) { return default_visit(inst); }
    CB_IL_INSTRUCTIONS(CB_IL_VISIT)
#undef CB_IL_VISIT
};

template <class C, class T>
class ILVisitorCT {
public:
    virtual ~ILVisitorCT() = default;
    virtual T default_visit(ILInstruction& inst, C context) = 0;
#define CB_IL_VISIT(Class, name) \
    virtual T visit_##name(Class& inst, C context) { return default_visit(inst, context); }
    CB_IL_INSTRUCTIONS(CB_IL_VISIT)
#undef CB_IL_VISIT
};

// upstream's inst.AcceptVisitor(visitor) for ILVisitor<T>
template <class T>
T accept_visitor(ILInstruction& inst, ILVisitorT<T>& visitor) {
    switch (inst.op_code) {
#define CB_IL_VISIT(Class, name) \
    case OpCode::Class: return visitor.visit_##name(static_cast<Class&>(inst));
        CB_IL_INSTRUCTIONS(CB_IL_VISIT)
#undef CB_IL_VISIT
    }
    return visitor.default_visit(inst);
}

// upstream's inst.AcceptVisitor(visitor, context) for ILVisitor<C, T>
template <class C, class T>
T accept_visitor(ILInstruction& inst, ILVisitorCT<C, T>& visitor, C context) {
    switch (inst.op_code) {
#define CB_IL_VISIT(Class, name) \
    case OpCode::Class: return visitor.visit_##name(static_cast<Class&>(inst), context);
        CB_IL_INSTRUCTIONS(CB_IL_VISIT)
#undef CB_IL_VISIT
    }
    return visitor.default_visit(inst, context);
}

}  // namespace cb::dotnet::decompiler::il

// dotnet/decompiler/csharp/expression_builder.hpp -- the translation of ILAst instructions into C# expressions: the visitor that turns
// every expression instruction (calls, arithmetic, conversions, field and array access, initializers, patterns, lambdas, dynamic call
// sites ...) into a syntax node that carries both the ILInstruction it came from and the ResolveResult that says what it means in C#.
//
//     ExpressionBuilder builder(&statement_builder, type_system, decompilation_context, function, settings, decompile_run, cancellation);
//     TranslatedExpression e = builder.translate(inst);              // e.expression: the C# node; e.type(): its C# type
//     TranslatedExpression b = builder.translate_condition(inst);    // a boolean expression, for if and loop conditions
//
// Every translated expression has an ILInstruction annotation and a ResolveResult annotation. After translate():
//   * the type of the ResolveResult matches the StackType of the instruction, except that the width of integer types does not matter
//     (I4, I and I8 count as the same stack type here);
//   * evaluating the C# expression has the same side effects as the instruction;
//   * an instruction with result type Void may become an expression of any type and value;
//   * otherwise the C# value is a similar value: an integer instruction gives an integer (or enum, pointer, char or bool) expression,
//     of the same value when the C# type has the width of the stack type, truncated when it is wider, and extended (sign or zero as the
//     C# type's sign says, IType sign) when it is narrower; a lifted nullable operation gives a Nullable<T> of such a T; a managed
//     reference made by starting to track an unmanaged one may give an integer or pointer type that converts to the same reference;
//     anything else gives the C# type of the stack type with the same value.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: ExpressionBuilder, from ExpressionBuilder.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). Where the original is C# only: virtual-generic dispatch is the switch of
// il_visitor.hpp; `out` parameters are pointers; nullable structs are std::optional.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/instruction_facts.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/resolver/type_inference.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"
#include "dotnet/decompiler/csharp/translated_expression.hpp"
#include "dotnet/decompiler/csharp/translation_context.hpp"
#include "dotnet/decompiler/il/il_visitor.hpp"
#include "dotnet/decompiler/il/transforms/transform_collection_and_object_initializers.hpp"
#include "dotnet/decompiler/semantics/initialized_object_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"

namespace cb::translate::ir {
struct Node;
}
namespace cb::dotnet::decompiler {
class DecompilerSettings;
class DecompileRun;
}  // namespace cb::dotnet::decompiler

namespace cb::dotnet::decompiler::csharp {

class StatementBuilder;
class L2Body;

using typesystem::IEvent;
using typesystem::IField;
using typesystem::IMember;
using typesystem::IMethod;
using typesystem::IProperty;
using typesystem::ITypeDefinition;
using typesystem::ITypeResolveContext;

class ExpressionBuilder final : public il::ILVisitorCT<TranslationContext, TranslatedExpression> {
public:
    // The translation of a switch's value: the expression, the type of the case labels, and the StringToInt behind a switch on strings.
    struct SwitchValue {
        TranslatedExpression value;
        const IType* case_type = nullptr;
        il::StringToInt* string_to_int = nullptr;
    };

    StatementBuilder* statement_builder;
    const typesystem::ITypeResolveContext* decompilation_context;
    il::ILFunction* current_function;
    const typesystem::ICompilation* compilation;
    resolver::CSharpResolver* resolver;
    syntax::TypeSystemAstBuilder ast_builder;
    resolver::TypeInference type_inference;
    const DecompilerSettings* settings;

    ExpressionBuilder(StatementBuilder* statement_builder, const typesystem::DecompilerTypeSystem& type_system,
                      const typesystem::ITypeResolveContext& decompilation_context, il::ILFunction* current_function, const DecompilerSettings& settings,
                      DecompileRun& decompile_run, const util::CancellationToken& cancellation_token, L2Body* l2 = nullptr);
    ExpressionBuilder(const ExpressionBuilder&) = delete;
    ExpressionBuilder& operator=(const ExpressionBuilder&) = delete;

    syntax::AstType* convert_type(const IType* type);
    ExpressionWithResolveResult convert_constant_value(ResolveResult* rr, bool allow_implicit_conversion = false);
    ExpressionWithResolveResult convert_constant_value(ResolveResult* rr, bool allow_implicit_conversion, bool display_as_hex);

    // `type_hint` null: no specific type is expected.
    TranslatedExpression translate(il::ILInstruction* inst, const IType* type_hint = nullptr);
    TranslatedExpression translate_condition(il::ILInstruction* condition, bool negate = false);

    ExpressionWithResolveResult convert_variable(il::ILVariable* variable);
    bool hides_variable_with_name(const std::string& name);
    static bool hides_variable_with_name(il::ILFunction* current_function, const std::string& name);
    il::ILFunction* resolve_local_function(const IMethod* method);

    ExpressionWithResolveResult get_default_value_expression(const IType* type);
    ExpressionWithResolveResult logic_not(TranslatedExpression expr);

    TranslatedExpression call_unsafe_intrinsic(const std::string& name, const std::vector<Expression*>& arguments, const IType* return_type,
                                               il::ILInstruction* inst = nullptr, const std::vector<const IType*>& type_arguments = {});

    bool is_current_or_containing_type(const ITypeDefinition* type);
    bool is_base_type_of_current_type(const ITypeDefinition* type);
    ExpressionWithResolveResult translate_function(const IType* delegate_type, il::ILFunction* function);

    TranslatedExpression translate_target(il::ILInstruction* target, bool non_virtual_invocation, bool member_static, const IType* member_declaring_type,
                                          const IType* constrained_to = nullptr);

    SwitchValue translate_switch_value(const SwitchFacts& inst, bool is_expression_context);

    static TranslatedExpression unwrap_boxing_conversion(TranslatedExpression arg);
    static bool is_unbox_any_with_is_inst(const TypedUnaryFacts& unbox_any, const IType* is_inst_type);
    static std::optional<syntax::AssignmentOperatorType> get_assignment_operator_type_from_metadata_name(const std::string& name,
                                                                                                         const DecompilerSettings& settings);
    static std::optional<syntax::UnaryOperatorType> get_unary_operator_type_from_metadata_name(const std::string& name, bool is_postfix);
    static TranslatedExpression change_direction_expression_to(TranslatedExpression input, typesystem::ReferenceKind kind, bool is_address_of);

    // ---- the visitor (one method per instruction class this builder translates) ----
    TranslatedExpression default_visit(il::ILInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_is_inst(il::IsInst& inst, TranslationContext context) override;
    TranslatedExpression visit_new_obj(il::NewObj& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_virt_delegate(il::LdVirtDelegate& inst, TranslationContext context) override;
    TranslatedExpression visit_new_arr(il::NewArr& inst, TranslationContext context) override;
    TranslatedExpression visit_loc_alloc(il::LocAlloc& inst, TranslationContext context) override;
    TranslatedExpression visit_loc_alloc_span(il::LocAllocSpan& inst, TranslationContext context) override;
    TranslatedExpression visit_ldc_i4(il::LdcI4& inst, TranslationContext context) override;
    TranslatedExpression visit_ldc_i8(il::LdcI8& inst, TranslationContext context) override;
    TranslatedExpression visit_ldc_f4(il::LdcF4& inst, TranslationContext context) override;
    TranslatedExpression visit_ldc_f8(il::LdcF8& inst, TranslationContext context) override;
    TranslatedExpression visit_ldc_decimal(il::LdcDecimal& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_str(il::LdStr& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_str_utf8(il::LdStrUtf8& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_null(il::LdNull& inst, TranslationContext context) override;
    TranslatedExpression visit_default_value(il::DefaultValue& inst, TranslationContext context) override;
    TranslatedExpression visit_size_of(il::SizeOf& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_type_token(il::LdTypeToken& inst, TranslationContext context) override;
    TranslatedExpression visit_bit_not(il::BitNot& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_loc(il::LdLoc& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_loca(il::LdLoca& inst, TranslationContext context) override;
    TranslatedExpression visit_st_loc(il::StLoc& inst, TranslationContext context) override;
    TranslatedExpression visit_comp(il::Comp& inst, TranslationContext context) override;
    TranslatedExpression visit_three_valued_bool_and(il::ThreeValuedBoolAnd& inst, TranslationContext context) override;
    TranslatedExpression visit_three_valued_bool_or(il::ThreeValuedBoolOr& inst, TranslationContext context) override;
    TranslatedExpression visit_throw(il::Throw& inst, TranslationContext context) override;
    TranslatedExpression visit_user_defined_logic_operator(il::UserDefinedLogicOperator& inst, TranslationContext context) override;
    TranslatedExpression visit_binary_numeric_instruction(il::BinaryNumericInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_user_defined_compound_assign(il::UserDefinedCompoundAssign& inst, TranslationContext context) override;
    TranslatedExpression visit_numeric_compound_assign(il::NumericCompoundAssign& inst, TranslationContext context) override;
    TranslatedExpression visit_conv(il::Conv& inst, TranslationContext context) override;
    TranslatedExpression visit_call(il::Call& inst, TranslationContext context) override;
    TranslatedExpression visit_call_virt(il::CallVirt& inst, TranslationContext context) override;
    TranslatedExpression visit_il_function(il::ILFunction& function, TranslationContext context) override;
    TranslatedExpression visit_block_container(il::BlockContainer& container, TranslationContext context) override;
    TranslatedExpression visit_ld_obj(il::LdObj& inst, TranslationContext context) override;
    TranslatedExpression visit_st_obj(il::StObj& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_len(il::LdLen& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_flda(il::LdFlda& inst, TranslationContext context) override;
    TranslatedExpression visit_lds_flda(il::LdsFlda& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_elema(il::LdElema& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_elema_inline_array(il::LdElemaInlineArray& inst, TranslationContext context) override;
    TranslatedExpression visit_unbox_any(il::UnboxAny& inst, TranslationContext context) override;
    TranslatedExpression visit_unbox(il::Unbox& inst, TranslationContext context) override;
    TranslatedExpression visit_box(il::Box& inst, TranslationContext context) override;
    TranslatedExpression visit_cast_class(il::CastClass& inst, TranslationContext context) override;
    TranslatedExpression visit_expression_tree_cast(il::ExpressionTreeCast& inst, TranslationContext context) override;
    TranslatedExpression visit_arglist(il::Arglist& inst, TranslationContext context) override;
    TranslatedExpression visit_make_ref_any(il::MakeRefAny& inst, TranslationContext context) override;
    TranslatedExpression visit_ref_any_type(il::RefAnyType& inst, TranslationContext context) override;
    TranslatedExpression visit_ref_any_value(il::RefAnyValue& inst, TranslationContext context) override;
    TranslatedExpression visit_block(il::Block& block, TranslationContext context) override;
    TranslatedExpression visit_null_coalescing_instruction(il::NullCoalescingInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_if_instruction(il::IfInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_switch_instruction(il::SwitchInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_address_of(il::AddressOf& inst, TranslationContext context) override;
    TranslatedExpression visit_await(il::Await& inst, TranslationContext context) override;
    TranslatedExpression visit_nullable_rewrap(il::NullableRewrap& inst, TranslationContext context) override;
    TranslatedExpression visit_nullable_unwrap(il::NullableUnwrap& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_convert_instruction(il::DynamicConvertInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_get_index_instruction(il::DynamicGetIndexInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_get_member_instruction(il::DynamicGetMemberInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_invoke_constructor_instruction(il::DynamicInvokeConstructorInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_invoke_member_instruction(il::DynamicInvokeMemberInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_invoke_instruction(il::DynamicInvokeInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_set_index_instruction(il::DynamicSetIndexInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_set_member_instruction(il::DynamicSetMemberInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_binary_operator_instruction(il::DynamicBinaryOperatorInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_logic_operator_instruction(il::DynamicLogicOperatorInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_unary_operator_instruction(il::DynamicUnaryOperatorInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_dynamic_compound_assign(il::DynamicCompoundAssign& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_ftn(il::LdFtn& inst, TranslationContext context) override;
    TranslatedExpression visit_ld_virt_ftn(il::LdVirtFtn& inst, TranslationContext context) override;
    TranslatedExpression visit_call_indirect(il::CallIndirect& inst, TranslationContext context) override;
    TranslatedExpression visit_deconstruct_instruction(il::DeconstructInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_match_instruction(il::MatchInstruction& inst, TranslationContext context) override;
    TranslatedExpression visit_invalid_branch(il::InvalidBranch& inst, TranslationContext context) override;
    TranslatedExpression visit_invalid_expression(il::InvalidExpression& inst, TranslationContext context) override;

private:
    const IType* known(typesystem::KnownTypeCode code) const { return compilation->find_type(code); }
    bool requires_qualifier(const IMember* member, const TranslatedExpression& target);
    ExpressionWithResolveResult convert_field(const IField* field, il::ILInstruction* target_instruction = nullptr);
    bool is_backing_field_of_automatic_event(const IField* field, const IEvent** ev);
    TranslatedExpression is_type(il::IsInst& inst);
    syntax::StackAllocExpression* translate_loc_alloc_span(const LocAllocFacts& inst, const IType* type_hint, const IType** element_type);
    syntax::StackAllocExpression* translate_loc_alloc(const LocAllocFacts& inst, const IType* type_hint, const IType** element_type);
    static bool should_display_as_hex(std::int64_t value, const IType* type);

    TranslatedExpression translate_ceq(const CompFacts& inst, bool* negate_output);
    TranslatedExpression try_unite_equality_operand_type(TranslatedExpression left, TranslatedExpression right);
    bool is_special_cased_reference_comparison_with_null(TranslatedExpression lhs, TranslatedExpression rhs);
    ExpressionWithResolveResult create_builtin_binary_operator(TranslatedExpression left, syntax::BinaryOperatorType type, TranslatedExpression right,
                                                               bool check_for_overflow = false);
    TranslatedExpression translate_comp(const CompFacts& inst);
    TranslatedExpression handle_three_valued_logic(const LogicOperatorFacts& inst, syntax::BinaryOperatorType op, il::ExpressionType eop);
    ExpressionWithResolveResult assignment(TranslatedExpression left, TranslatedExpression right);

    std::optional<TranslatedExpression> handle_pointer_arithmetic(const BinaryNumericFacts& inst, TranslatedExpression left, TranslatedExpression right,
                                                                  TranslationContext context);
    std::optional<TranslatedExpression> handle_managed_pointer_arithmetic(const BinaryNumericFacts& inst, TranslatedExpression left,
                                                                          TranslatedExpression right);
    TranslatedExpression ensure_integer_type(TranslatedExpression expr);
    std::optional<TranslatedExpression> get_pointer_arithmetic_offset(il::ILInstruction* byte_offset_inst, TranslatedExpression byte_offset_expr,
                                                                      const IType* pointer_element_type, bool check_for_overflow,
                                                                      bool unwrap_zero_extension = false);
    std::optional<TranslatedExpression> handle_pointer_subtraction(const BinaryNumericFacts& inst);
    TranslatedExpression handle_binary_numeric(const BinaryNumericFacts& inst, syntax::BinaryOperatorType op, TranslationContext context);
    const IType* find_type(il::StackType stack_type, typesystem::Sign sign);
    const IType* find_arithmetic_type(il::StackType stack_type, typesystem::Sign sign);
    TranslatedExpression prepare_arithmetic_argument(TranslatedExpression arg, il::StackType arg_stack_type, typesystem::Sign sign, bool is_lifted);
    static bool is_compatible_with_sign(const IType* type, typesystem::Sign sign);
    static bool binary_operator_might_check_for_overflow(syntax::BinaryOperatorType op);
    bool constant_binary_operator_overflows(syntax::BinaryOperatorType op, ResolveResult* left, ResolveResult* right);
    TranslatedExpression handle_shift(const BinaryNumericFacts& inst, syntax::BinaryOperatorType op, TranslationContext context);
    TranslatedExpression handle_compound_assignment(const NumericCompoundAssignFacts& inst, syntax::AssignmentOperatorType op);
    TranslatedExpression handle_compound_shift(const NumericCompoundAssignFacts& inst, syntax::AssignmentOperatorType op);
    static bool assignment_operator_might_check_for_overflow(syntax::AssignmentOperatorType op);
    bool value_might_be_oversized(ResolveResult* rr, il::StackType stack_type);
    TranslatedExpression wrap_in_ref(TranslatedExpression expr, const IType* type);

    const IType* infer_return_type(syntax::BlockStatement* body);
    const IType* get_task_type(const IType* result_type);
    std::vector<syntax::ParameterDeclaration*> make_parameters(const std::vector<const typesystem::IParameter*>& parameters, il::ILFunction* function);
    TranslatedExpression ensure_target_not_nullable(TranslatedExpression expr, il::ILInstruction* inst);

    ExpressionWithResolveResult ld_obj(il::ILInstruction* address, const IType* load_type);
    TranslatedExpression st_obj_via_helper_call(const StObjFacts& inst);
    TranslatedExpression translate_array_index(il::ILInstruction* i);
    TranslatedExpression convert_array_index(TranslatedExpression input, il::StackType stack_type, bool allow_int_ptr);

    TranslatedExpression translate_interpolated_string(il::Block* block);
    TranslatedExpression translate_call_with_named_args(il::Block* block);
    TranslatedExpression translate_setter_call_assignment(il::Block* block);
    TranslatedExpression translate_object_and_collection_initializer(il::Block* block);
    syntax::ArrayInitializerExpression* build_array_initializer_expression(il::Block* block, semantics::InitializedObjectResolveResult* init_obj_rr);
    static std::vector<il::ILInstruction*> get_indices(const std::vector<il::ILInstruction*>& indices,
                                                       const std::map<il::ILVariable*, il::ILInstruction*>& index_variables);
    TranslatedExpression make_initializer_assignment(semantics::InitializedObjectResolveResult* rr, const il::transforms::AccessPathElement& member_path,
                                                     const il::transforms::AccessPathElement& value_path, const std::vector<TranslatedExpression>& values,
                                                     const std::map<il::ILVariable*, il::ILInstruction*>& index_variables);
    TranslatedExpression translate_array_initializer(il::Block* block);
    TranslatedExpression translate_stack_alloc_initializer(il::Block* block, const IType* type_hint);
    TranslatedExpression translate_with_initializer(il::Block* block);
    TranslatedExpression adjust_constant_expression_to_type(TranslatedExpression expr, const IType* type_hint);
    ResolveResult* adjust_constant_to_type(ResolveResult* rr, const IType* type_hint);

    static const IType* get_csharp_switch_governing_type(const IType* type);

    TranslatedExpression translate_dynamic_target(il::ILInstruction* inst, const il::CSharpArgumentInfo& argument_info);
    static const IType* dynamic_argument_type(const il::CSharpArgumentInfo& info);
    const IMember* create_dynamic_member_symbol(const std::string& name, const il::CSharpArgumentInfo& target_info);
    const IMember* create_dynamic_invoke_member_symbol(const std::string& name, const il::CSharpArgumentInfo& target_info,
                                                       const std::vector<il::CSharpArgumentInfo>& argument_info,
                                                       const std::vector<const IType*>& type_arguments);
    const IMethod* create_dynamic_constructor_symbol(const IType* declaring_type, const std::vector<il::CSharpArgumentInfo>& argument_info);
    const IMember* create_dynamic_indexer_symbol(const IType* declaring_type, const std::vector<il::CSharpArgumentInfo>& argument_info);
    std::vector<TranslatedExpression> translate_dynamic_arguments(const std::vector<il::ILInstruction*>& arguments,
                                                                  const std::vector<il::CSharpArgumentInfo>& argument_info);
    TranslatedExpression translate_dynamic_argument(il::ILInstruction* argument, const il::CSharpArgumentInfo& info);

    ExpressionWithILInstruction translate_pattern(il::ILInstruction* pattern, const IType* left_hand_type);
    static TranslatedExpression error_expression(const std::string& message);

    // Stage 14: the instruction's translation from its L2 node: the node's facts decoded as the lifting decodes them (lift.cpp), then
    // the kind's translation below; nothing for a kind not read from L2 (the ILAst visitor translates it) or a node whose operands are
    // no longer the instruction's.
    std::optional<TranslatedExpression> translate_from_l2(const translate::ir::Node& node, ILInstruction& inst, TranslationContext context);
    // Each kind's translation from its facts (instruction_facts.hpp), which its visitor reads from the instruction and translate_from_l2 from
    // the node.
    TranslatedExpression translate_st_loc(const StLocFacts& inst);
    TranslatedExpression translate_comp_facts(const CompFacts& inst);
    TranslatedExpression translate_binary_numeric(const BinaryNumericFacts& inst, TranslationContext context);
    TranslatedExpression translate_conv(const ConvFacts& inst, TranslationContext context);
    TranslatedExpression translate_box(const TypedUnaryFacts& inst);
    TranslatedExpression translate_unbox(const TypedUnaryFacts& inst);
    TranslatedExpression translate_unbox_any(const TypedUnaryFacts& inst);
    TranslatedExpression translate_cast_class(const TypedUnaryFacts& inst);
    TranslatedExpression translate_is_inst(const TypedUnaryFacts& inst);
    TranslatedExpression translate_ld_obj(const LdObjFacts& inst, TranslationContext context);
    TranslatedExpression translate_st_obj(const StObjFacts& inst);
    TranslatedExpression translate_ld_flda(const LdFldaFacts& inst);
    TranslatedExpression translate_lds_flda(const LdsFldaFacts& inst);
    TranslatedExpression translate_ld_elema(const LdElemaFacts& inst);
    TranslatedExpression translate_new_arr(const NewArrFacts& inst);
    TranslatedExpression translate_address_of(const AddressOfFacts& inst);
    TranslatedExpression translate_call(const CallFacts& inst, TranslationContext context);   // call, callvirt (and newobj)
    TranslatedExpression translate_new_obj(const CallFacts& inst, TranslationContext context);
    TranslatedExpression translate_null_coalescing(const NullCoalescingFacts& inst);
    TranslatedExpression translate_ld_ftn(const MethodPointerFacts& inst);
    TranslatedExpression translate_ld_virt_ftn(const MethodPointerFacts& inst);
    TranslatedExpression translate_await(const AwaitFacts& inst);
    TranslatedExpression translate_nullable_unwrap(const NullableUnwrapFacts& inst);
    TranslatedExpression translate_nullable_rewrap(const OperandFacts& inst);
    TranslatedExpression translate_throw(const OperandFacts& inst);
    TranslatedExpression translate_ld_type_token(const TypeTokenFacts& inst);
    TranslatedExpression translate_if(const IfFacts& inst, TranslationContext context);
    TranslatedExpression translate_numeric_compound_assign(const NumericCompoundAssignFacts& inst);
    TranslatedExpression translate_user_defined_compound_assign(const UserDefinedCompoundAssignFacts& inst);
    TranslatedExpression translate_call_indirect(const CallIndirectFacts& inst);
    TranslatedExpression translate_match(const MatchFacts& inst);
    TranslatedExpression translate_deconstruct(const DeconstructFacts& inst);
    TranslatedExpression translate_loc_alloc_span_expression(const LocAllocFacts& inst, TranslationContext context);
    TranslatedExpression translate_loc_alloc_expression(const LocAllocFacts& inst, TranslationContext context);
    TranslatedExpression translate_user_defined_logic_operator(const LogicOperatorFacts& inst);
    TranslatedExpression translate_expression_tree_cast(const ExpressionTreeCastFacts& inst);
    TranslatedExpression translate_ld_elema_inline_array(const LdElemaInlineArrayFacts& inst);
    TranslatedExpression translate_dynamic_compound_assign(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_unary_operator(const DynamicFacts& inst, TranslationContext context);
    TranslatedExpression translate_dynamic_logic_operator(const DynamicFacts& inst, TranslationContext context);
    TranslatedExpression translate_dynamic_binary_operator(const DynamicFacts& inst, TranslationContext context);
    TranslatedExpression translate_dynamic_set_member(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_set_index(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_invoke(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_invoke_member(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_invoke_constructor(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_get_member(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_get_index(const DynamicFacts& inst);
    TranslatedExpression translate_dynamic_convert(const DynamicFacts& inst);
    TranslatedExpression translate_switch_expression(const SwitchFacts& inst, TranslationContext context);
    TranslatedExpression translate_block(il::Block& block, il::BlockKind kind, TranslationContext context);

    const typesystem::DecompilerTypeSystem* type_system_;
    util::CancellationToken cancellation_token_;
    std::set<il::ILVariable*> loaded_variables_set_;

public:
    // The body as L2 (stage 14), or null where the builders read the ILAst alone.
    L2Body* const l2;
};

}  // namespace cb::dotnet::decompiler::csharp

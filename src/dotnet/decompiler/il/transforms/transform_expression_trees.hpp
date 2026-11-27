// dotnet/decompiler/il/transforms/transform_expression_trees.hpp -- turns the calls that build a LINQ expression tree (Expression.Lambda
// over Expression.Add, Expression.Call, Expression.Constant ... and the parameter expressions they use) back into the ILFunction and
// ILAst instructions the lambda was written as.
//
//     stloc p(call Expression.Parameter(...)); call Expression.Lambda(call Add(ldloc p, ...), array {ldloc p})
//         =>   ILFunction (expression tree) { return p + ... }
//
//     TransformExpressionTrees transform;
//     transform.run(block, pos, context);
//
// The conversion builds a tree of std::function<ILInstruction*()>, which are only run when the whole conversion succeeds, so a lambda
// that cannot be converted leaves the ILAst as it was.
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL.Transforms: TransformExpressionTrees, from TransformExpressionTrees.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler). Not yet ported (marked WAITS in the .cxx): the C# resolver's
// CSharpConversions, CSharpResolver.ResolveBinaryOperator and CSharpOperators.LiftUserDefinedOperator.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "dotnet/decompiler/il/transforms/il_transform.hpp"

namespace cb::dotnet::decompiler::il::transforms {

class TransformExpressionTrees final : public IStatementTransform {
public:
    void run(Block* block, int pos, StatementTransformContext& context) override;
    std::string name() const override { return "TransformExpressionTrees"; }

    // `call Type.GetTypeFromHandle(ldtypetoken T)`.
    static bool match_get_type_from_handle(ILInstruction* inst, const IType** type);

private:
    // Builds the converted instruction (called only when everything converted) and its type.
    using Converter = std::function<ILInstruction*()>;
    // Builds the converted binding of a member initializer, given the variable of the initialized object.
    using BindConverter = std::function<ILInstruction*(ILVariable*)>;
    struct Converted {
        Converter build;     // empty when the conversion failed
        const IType* type;   // the unknown type then
    };
    struct ConvertedBind {
        BindConverter build;
        const IType* type;
    };

    static Converted fail();
    static ConvertedBind fail_bind();

    static bool might_be_expression_tree(ILInstruction* inst, ILInstruction* stmt);
    static bool is_empty_parameter_list(ILInstruction* inst);
    bool match_parameter_variable_assignment(ILInstruction* expr, ILVariable** parameter_reference_var, const IType** type, std::string* name);
    bool try_convert_expression_tree(ILInstruction* instruction, ILInstruction* statement);
    // Converts an Expression.Lambda call into an ILFunction; a failed conversion has no builder.
    Converted convert_lambda(CallInstruction* instruction);
    Converted convert_quote(CallInstruction* invocation);
    void set_expression_tree_flag(ILFunction* lambda, CallInstruction* call);
    bool read_parameters(ILInstruction* initializer, std::vector<const typesystem::IParameter*>* parameters, std::vector<ILVariable*>* parameter_variables);
    Converted convert_instruction(ILInstruction* instruction, const IType* type_hint = nullptr);
    Converted convert_instruction_core(ILInstruction* instruction, const IType* type_hint);
    static bool is_expression_tree(const IType* delegate_type);
    static const IType* unwrap_expression_tree(const IType* delegate_type);

    Converted convert_array_index(CallInstruction* invocation);
    Converted convert_array_length(CallInstruction* invocation);
    Converted convert_binary_numeric_operator(CallInstruction* invocation, BinaryNumericOperator op, std::optional<bool> is_checked = std::nullopt);
    ConvertedBind convert_bind(CallInstruction* invocation);
    Converted convert_call(CallInstruction* invocation);
    ILInstruction* prepare_call_target(const IType* expected_type, ILInstruction* target, const IType* target_type);
    static ILInstruction* unpack_constant(ILInstruction* inst);
    // The converted arguments for the method's parameters; none when one fails.
    std::optional<std::vector<Converter>> convert_call_arguments(const std::vector<ILInstruction*>& arguments, const IMethod* method);
    Converted convert_cast(CallInstruction* invocation, bool is_checked);
    Converted convert_coalesce(CallInstruction* invocation);
    Converted convert_comparison(CallInstruction* invocation, ComparisonKind kind);
    Converted convert_condition(CallInstruction* invocation);
    Converted convert_constant(CallInstruction* invocation);
    Converted convert_element_init(CallInstruction* invocation);
    Converted convert_field(CallInstruction* invocation, const IType* type_hint);
    Converted convert_invoke(CallInstruction* invocation);
    Converted convert_list_init(CallInstruction* invocation);
    Converted convert_logic_operator(CallInstruction* invocation, bool and_);
    Converted convert_member_init(CallInstruction* invocation);
    Converted convert_new_array_bounds(CallInstruction* invocation);
    Converted convert_new_array_init(CallInstruction* invocation);
    static bool match_new(CallInstruction* invocation, const IMethod** ctor);
    Converted convert_new_object(CallInstruction* invocation);
    Converted convert_not_operator(CallInstruction* invocation);
    Converted convert_property(CallInstruction* invocation);
    Converted convert_type_as(CallInstruction* invocation);
    Converted convert_type_is(CallInstruction* invocation);
    Converted convert_unary_numeric_operator(CallInstruction* invocation, BinaryNumericOperator op, std::optional<bool> is_checked = std::nullopt);
    ILInstruction* convert_value(ILInstruction* value, ILInstruction* context);

    bool is_closure_reference(ILVariable* variable);
    static bool is_expression_tree_parameter(ILVariable* variable);
    bool match_constant_call(ILInstruction* inst, ILInstruction** value, const IType** type);
    static bool match_get_method_from_handle(ILInstruction* inst, const IMember** member);
    static bool match_get_constructor_from_handle(ILInstruction* inst, const IMember** member);
    static bool match_get_field_from_handle(ILInstruction* inst, const IMember** member);
    static bool match_from_handle_parameter_list(CallInstruction* call, const IMember** member);
    static bool match_argument_list(ILInstruction* inst, std::vector<ILInstruction*>* arguments);

    // The parameter expressions the statements before the lambda made: variable -> (type, name), in order.
    const std::pair<const IType*, std::string>* find_parameter(ILVariable* variable) const;
    // The ILVariable made for an expression tree parameter, or nullptr.
    ILVariable* find_parameter_mapping(ILVariable* variable) const;

    StatementTransformContext* context_ = nullptr;
    std::vector<std::pair<ILVariable*, std::pair<const IType*, std::string>>> parameters_;
    std::vector<std::pair<ILVariable*, ILVariable*>> parameter_mapping_;
    std::vector<ILInstruction*> instructions_to_remove_;
    std::vector<ILFunction*> lambda_stack_;
};

}  // namespace cb::dotnet::decompiler::il::transforms

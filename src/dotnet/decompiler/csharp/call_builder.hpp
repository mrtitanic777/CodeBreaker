// dotnet/decompiler/csharp/call_builder.hpp -- CallBuilder: turns the ILAst's calls (call, callvirt, newobj, ldvirtdelegate ...) into C#
// call syntax. It decides how much has to be written for the call to mean the same thing again: whether the target, the type arguments,
// casts of the arguments or of the target are needed (it asks the C# overload resolution whether the call as written picks the called
// method), which arguments are optional, named, `params` elements or `out var`, and it recognizes the calls that are really other syntax:
// property and indexer accessors, delegate construction and comparison, implicit conversions, string interpolation, string concatenation
// of spans, ranges and indices, anonymous types, collection and dictionary initializers.
//
//     CallBuilder builder(expression_builder, type_system, settings);
//     TranslatedExpression call = builder.build(call_instruction, type_hint);
//     ExpressionWithResolveResult init = builder.build_collection_initializer_expression(OpCode::Call, add_method, target, arguments);
//
// ExpressionBuilder makes one CallBuilder and calls build() for every call instruction. The members of ExpressionBuilder that are read
// here (resolver, compilation, settings, type_inference, statement_builder, translate, translate_target, convert_type,
// resolve_local_function, hides_variable_with_name, is_current_or_containing_type, is_base_type_of_current_type,
// get_default_value_expression, change_direction_expression_to) are named as upstream's, in snake_case.
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp: CallBuilder, from CallBuilder.cs), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "dotnet/decompiler/csharp/annotations.hpp"
#include "dotnet/decompiler/csharp/instruction_facts.hpp"
#include "dotnet/decompiler/csharp/l2_body.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/overload_resolution_errors.hpp"
#include "dotnet/decompiler/csharp/resolver/type_inference.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/csharp/translated_expression.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/semantics/initialized_object_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"

namespace cb::dotnet::decompiler::csharp {

class ExpressionBuilder;

class CallBuilder {
public:
    // The names of the arguments that are written with a name (nullopt for a positional one), one entry per argument.
    using ArgumentNames = std::vector<std::optional<std::string>>;

    // `type_system` is the decompiler's type system (the compilation); `settings` the decompiler settings.
    CallBuilder(ExpressionBuilder& expression_builder, const typesystem::ICompilation& type_system, const DecompilerSettings& settings);

    // The C# of a call, newobj or delegate construction instruction (`type_hint` is the type the surroundings expect, for tuples).
    TranslatedExpression build(il::CallInstruction* inst, const IType* type_hint = nullptr);
    // ... from its facts (read from the instruction or from its L2 node, stage 14).
    TranslatedExpression build(const CallFacts& facts, const IType* type_hint = nullptr);

    // Whether the method is string.Concat(ReadOnlySpan<char>, ...): the C# compiler's translation of `a + b` on strings with spans.
    static bool is_span_based_string_concat(const typesystem::IMethod* method);
    // Whether the method is string's implicit conversion to ReadOnlySpan<char>.
    static bool is_string_to_read_only_span_char_implicit_conversion(const typesystem::IMethod* method);

    // The C# of a call of `method` with these arguments (`argument_to_parameter_map`: the argument at index i is the value of parameter
    // map[i], for a call with named arguments; `constrained_to`: the type of the `constrained.` prefix).
    ExpressionWithResolveResult build(il::OpCode call_op_code, const typesystem::IMethod* method, const std::vector<il::ILInstruction*>& call_arguments,
                                      const std::optional<std::vector<int>>& argument_to_parameter_map = std::nullopt, const IType* constrained_to = nullptr);

    // Converts a call to an Add method to a collection initializer expression.
    ExpressionWithResolveResult build_collection_initializer_expression(il::OpCode call_op_code, const typesystem::IMethod* method,
                                                                         semantics::InitializedObjectResolveResult* target,
                                                                         const std::vector<il::ILInstruction*>& call_arguments);

    // Converts a call of an indexer accessor to a dictionary initializer element `[indices] = value` (just `[indices]` without a value).
    ExpressionWithResolveResult build_dictionary_initializer_expression(il::OpCode call_op_code, const typesystem::IMethod* method,
                                                                         semantics::InitializedObjectResolveResult* target,
                                                                         const std::vector<il::ILInstruction*>& indices, il::ILInstruction* value = nullptr);

    TranslatedExpression build(il::LdVirtDelegate* inst);

    TranslatedExpression build(const LdVirtDelegateFacts& inst);

    // The method as a method group reference (`Type.Method`, `Method`), for ldftn and similar.
    ExpressionWithResolveResult build_method_reference(const typesystem::IMethod* method, bool is_virtual);

    // A call whose arguments are given in a different order than the parameters' (a block of stores to named-argument variables and a
    // call): C# with named arguments.
    TranslatedExpression call_with_named_args(il::Block* block);

private:
    struct ExpectedTargetDetails {
        il::OpCode call_op_code = il::OpCode::Call;
        bool needs_boxing_conversion = false;
    };

    // The arguments of a call as translated, in argument order, with what is needed to write them.
    struct ArgumentList {
        std::vector<TranslatedExpression> arguments;
        std::vector<const typesystem::IParameter*> expected_parameters;
        std::vector<std::string> parameter_names;
        std::optional<ArgumentNames> argument_names;
        // -2: there are no optional arguments; -1: optional arguments are forbidden; >= 0: the index of the first argument that can be
        // left out, because it and all after it are the default value of their parameter.
        int first_optional_argument_index = -2;
        util::BitSet is_primitive_value{0};
        std::optional<std::vector<int>> argument_to_parameter_map;

        bool add_names_to_primitive_values = false;
        bool use_implicitly_typed_out = false;
        bool is_expanded_form = false;
        int length() const { return static_cast<int>(arguments.size()); }

        std::optional<ArgumentNames> get_argument_names(int skip_count = 0);
        std::vector<ResolveResult*> get_argument_resolve_results(int skip_count = 0) const;
        std::vector<ResolveResult*> get_argument_resolve_results_direct(int skip_count = 0) const;
        std::vector<Expression*> get_argument_expressions(int skip_count = 0);
        bool can_infer_anonymous_type_property_names_from_arguments() const;
        // (upstream's [Conditional("DEBUG")] CheckNoNamedOrOptionalArguments)
        void check_no_named_or_optional_arguments() const;

    private:
        int get_actual_argument_count() const { return first_optional_argument_index < 0 ? length() : first_optional_argument_index; }
    };

    enum class CallTransformation : std::uint8_t {
        None = 0,
        RequireTarget = 1,
        RequireTypeArguments = 2,
        NoOptionalArgumentAllowed = 4,
        // Add calls to AsRefReadOnly for in parameters that did not have an explicit DirectionExpression yet.
        EnforceExplicitIn = 8,
        NoNamedArgsForPrettiness = 0x10,
        All = 0x1f,
    };
    friend constexpr CallTransformation operator|(CallTransformation a, CallTransformation b) {
        return static_cast<CallTransformation>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
    }
    friend constexpr CallTransformation operator&(CallTransformation a, CallTransformation b) {
        return static_cast<CallTransformation>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
    }
    friend constexpr CallTransformation operator~(CallTransformation a) {
        return static_cast<CallTransformation>(~static_cast<std::uint8_t>(a) & 0x1f);
    }
    friend constexpr CallTransformation& operator|=(CallTransformation& a, CallTransformation b) { return a = a | b; }
    friend constexpr CallTransformation& operator&=(CallTransformation& a, CallTransformation b) { return a = a & b; }

    enum class TokenKind : std::uint8_t {
        Error,
        String,
        Argument,
        ArgumentWithFormat,
        ArgumentWithAlignment,
        ArgumentWithAlignmentAndFormat,
    };
    struct StringInterpolationToken {
        TokenKind kind;
        int index;
        int alignment;
        std::optional<std::string> format;
    };

    ExpressionWithResolveResult build_string_concat(const typesystem::IMethod* method,
                                                    const std::vector<std::pair<il::ILInstruction*, typesystem::KnownTypeCode>>& operands);
    friend class L2Body;   // (its matcher of the same on the tree is checked against this one)
    static bool is_span_based_string_concat(il::CallInstruction* call, std::vector<std::pair<il::ILInstruction*, typesystem::KnownTypeCode>>* operands);

    // The interpolated string of String.Format / FormattableStringFactory.Create; an ExpressionWithResolveResult without an expression
    // when the format string is not one the interpolation can take.
    ExpressionWithResolveResult handle_string_interpolation(const typesystem::IMethod* method, ArgumentList& argument_list);
    static bool is_interpolated_string_creation(const typesystem::IMethod* method, const ArgumentList& argument_list);
    bool try_get_string_interpolation_tokens(const ArgumentList& argument_list, std::string* format, std::vector<StringInterpolationToken>* tokens);
    std::vector<std::pair<TokenKind, std::optional<std::string>>> tokenize_format_string(const std::string& value);

    ArgumentList build_argument_list(const ExpectedTargetDetails& expected_target_details, ResolveResult* target, const typesystem::IMethod* method,
                                     int first_param_index, const std::vector<il::ILInstruction*>& call_arguments,
                                     const std::optional<std::vector<int>>& argument_to_parameter_map);
    bool is_primitive_value_that_should_be_named_argument(const TranslatedExpression& arg, const typesystem::IMethod* method,
                                                          const typesystem::IParameter* p);
    bool transform_params_argument(const ExpectedTargetDetails& expected_target_details, ResolveResult* target_resolve_result,
                                   const typesystem::IMethod* method, const typesystem::IParameter* parameter, const TranslatedExpression& params_argument,
                                   std::vector<const typesystem::IParameter*>& expected_parameters, std::vector<TranslatedExpression>& arguments);
    bool is_optional_argument(const typesystem::IParameter* parameter, const TranslatedExpression& arg);

    CallTransformation get_required_transformations_for_call(const ExpectedTargetDetails& expected_target_details, const typesystem::IMethod* method,
                                                             TranslatedExpression& target, ArgumentList& argument_list,
                                                             CallTransformation allowed_transforms,
                                                             const typesystem::IParameterizedMember** found_method);
    void enforce_explicit_in(std::vector<TranslatedExpression>& arguments, const std::vector<const typesystem::IParameter*>& expected_parameters);
    TranslatedExpression wrap_in_as_ref_read_only(const TranslatedExpression& arg);
    static bool is_possible_extension_method_call_on_null(const typesystem::IMethod* method, const std::vector<TranslatedExpression>& arguments);
    static bool can_infer_type_arguments_from_arguments(const typesystem::IMethod* method, const ArgumentList& argument_list,
                                                        resolver::TypeInference& type_inference);
    bool pin_types_of_null_arguments(ArgumentList& argument_list);
    il::NewObj* new_anonymous_type_instance(const IType* type);
    void cast_arguments(std::vector<TranslatedExpression>& arguments, const std::vector<const typesystem::IParameter*>& expected_parameters);
    static bool is_null_conditional(Expression* expr);
    void modify_return_type_of_lambda(syntax::LambdaExpression* lambda);
    void modify_return_statement_inside_lambda(const IType* return_type, syntax::AstNode* parent);

    bool is_delegate_equality_comparison(const typesystem::IMethod* method, const std::vector<TranslatedExpression>& arguments);
    Expression* handle_delegate_equality_comparison(const typesystem::IMethod* method, const std::vector<TranslatedExpression>& arguments);
    ExpressionWithResolveResult handle_implicit_conversion(const typesystem::IMethod* method, TranslatedExpression argument);

    resolver::OverloadResolutionErrors is_unambiguous_call(const ExpectedTargetDetails& expected_target_details, const typesystem::IMethod* method,
                                                           ResolveResult* target, const std::vector<const IType*>& type_arguments,
                                                           const std::vector<ResolveResult*>& arguments, std::optional<ArgumentNames> argument_names,
                                                           int first_optional_argument_index, const typesystem::IParameterizedMember** found_member,
                                                           bool* best_candidate_is_expanded_form);
    bool is_unambiguous_access(const ExpectedTargetDetails& expected_target_details, ResolveResult* target, const typesystem::IMethod* method,
                               const std::vector<TranslatedExpression>& arguments, const std::optional<ArgumentNames>& argument_names,
                               const typesystem::IMember** found_member);
    ExpressionWithResolveResult handle_accessor_call(const ExpectedTargetDetails& expected_target_details, const typesystem::IMethod* method,
                                                     TranslatedExpression target, std::vector<TranslatedExpression> arguments,
                                                     const std::optional<ArgumentNames>& argument_names);
    bool is_appropriate_call_target(const ExpectedTargetDetails& expected_target_details, const typesystem::IMember* expected_target,
                                    const typesystem::IMember* actual_target);
    ExpressionWithResolveResult handle_constructor_call(const ExpectedTargetDetails& expected_target_details, ResolveResult* target,
                                                        const typesystem::IMethod* method, ArgumentList& argument_list);

    TranslatedExpression handle_delegate_construction(il::CallInstruction* inst, const DelegateConstructionMatch& match);
    bool can_use_delegate_construction(const typesystem::IMethod* target_method, il::ILInstruction* this_arg, const typesystem::IMethod* invoke_method);
    ExpressionWithResolveResult build_delegate_reference(const typesystem::IMethod* method, const typesystem::IMethod* invoke_method,
                                                         const ExpectedTargetDetails& expected_target_details, il::ILInstruction* this_arg);
    struct DelegateReference {
        TranslatedExpression target;
        bool add_type_arguments;
        std::string method_name;
        ResolveResult* result;
    };
    DelegateReference disambiguate_delegate_reference(const typesystem::IMethod* method, const typesystem::IMethod* invoke_method,
                                                      const ExpectedTargetDetails& expected_target_details, il::ILInstruction* this_arg);
    TranslatedExpression handle_delegate_construction(const IType* delegate_type, const typesystem::IMethod* method,
                                                      const ExpectedTargetDetails& expected_target_details, il::ILInstruction* this_arg,
                                                      il::ILInstruction* inst);
    bool is_unambiguous_method_reference(const ExpectedTargetDetails& expected_target_details, const typesystem::IMethod* method, ResolveResult* target,
                                         const std::vector<const IType*>& type_arguments, bool is_extension_method_reference, ResolveResult** result);
    static resolver::MethodGroupResolveResult* to_method_group(const typesystem::IMethod* method, il::ILFunction* local_function);

    bool handle_range_construction(ExpressionWithResolveResult* result, il::OpCode call_op_code, const typesystem::IMethod* method,
                                   const TranslatedExpression& target, const ArgumentList& argument_list);

    const DecompilerSettings& settings_;
    ExpressionBuilder& expression_builder_;
    resolver::CSharpResolver* resolver_;
    const typesystem::ICompilation& type_system_;
    // The unnamed parameters made for the elements of an expanded params argument; they live as long as the builder.
    std::vector<std::unique_ptr<typesystem::DefaultParameter>> owned_parameters_;
    const typesystem::IParameter* make_default_parameter(const IType* type);
};

}  // namespace cb::dotnet::decompiler::csharp

// dotnet/decompiler/csharp/resolver/overload_resolution.hpp -- C# overload resolution (spec 12.6.4): which of several candidate methods
// (or indexers, constructors, operators) a call with given arguments means -- the arguments are mapped to parameters, type arguments
// inferred, applicability checked and the best candidate chosen -- plus the conversions applied to the arguments and the invocation result.
//
//     OverloadResolution overload_resolution(compilation, arguments);
//     for (const IParameterizedMember* m : candidates) overload_resolution.add_candidate(m);
//     if (overload_resolution.found_applicable_candidate())
//         ResolveResult* call = overload_resolution.create_resolve_result(target);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: OverloadResolution, from OverloadResolution.cs; and
// MethodGroupResolveResult.PerformOverloadResolution, from MethodGroupResolveResult.cs, as perform_overload_resolution below: the
// method group's header has no room for it yet and it only combines the method group with this class), Copyright AlphaSierraPapa for
// the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Upstream's arrays that may be null (argument names, type arguments) are vectors, an empty vector standing for null. A candidate is a
// private structure kept by shared pointer: the best candidate stays alive after later ones are considered. The debugging trace of
// candidate results (LogCandidateAddingResult) is not ported.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_invocation_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/overload_resolution_errors.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using typesystem::IParameterizedMember;
using typesystem::ITypeParameter;

class OverloadResolution {
public:
    // `argument_names` is empty (no argument has a name) or has one entry per argument (nullopt for a positional one); `type_arguments`
    // the explicitly given type arguments; `conversions` the compilation's when null.
    OverloadResolution(const ICompilation& compilation, std::vector<ResolveResult*> arguments, std::vector<std::optional<std::string>> argument_names = {},
                       std::vector<const IType*> type_arguments = {}, CSharpConversions* conversions = nullptr);

    // ---- input properties ------------------------------------------------------------------------------------------------

    // Whether the methods are extension methods that are being called using extension method syntax. Setting this restricts the possible
    // conversions on the first argument to implicit identity, reference, or boxing conversions.
    bool is_extension_method_invocation() const { return is_extension_method_invocation_; }
    void set_is_extension_method_invocation(bool value) { is_extension_method_invocation_ = value; }

    // Whether expanding 'params' into individual elements is allowed. The default value is true.
    bool allow_expanding_params() const { return allow_expanding_params_; }
    void set_allow_expanding_params(bool value) { allow_expanding_params_ = value; }

    // Whether optional parameters may be left at their default value. The default value is true. If this is false, optional parameters
    // are treated like regular parameters.
    bool allow_optional_parameters() const { return allow_optional_parameters_; }
    void set_allow_optional_parameters(bool value) { allow_optional_parameters_ = value; }

    // Whether a value argument can be passed to an `in` reference parameter.
    bool allow_implicit_in() const { return allow_implicit_in_; }
    void set_allow_implicit_in(bool value) { allow_implicit_in_ = value; }

    // Whether the ConversionResolveResults this creates apply overflow checking. The default value is false.
    bool check_for_overflow() const { return check_for_overflow_; }
    void set_check_for_overflow(bool value) { check_for_overflow_ = value; }

    // The arguments for which this instance was created.
    const std::vector<ResolveResult*>& arguments() const { return arguments_; }

    // ---- adding candidates -----------------------------------------------------------------------------------------------

    // Adds a candidate. Returns the errors that prevent the member from being applicable, if any (not the errors that do not affect
    // applicability).
    OverloadResolutionErrors add_candidate(const IParameterizedMember* member);
    // `additional_errors` apply to the candidate besides: this is how errors of member lookup (Inaccessible) enter overload resolution.
    OverloadResolutionErrors add_candidate(const IParameterizedMember* member, OverloadResolutionErrors additional_errors);

    // Adds all candidates from the method lists, with the logic that applicable methods in derived types hide all methods in base types.
    // The methods are grouped by declaring type; base types must come first in the list.
    void add_method_lists(const std::vector<MethodListWithDeclaringType>& method_lists);

    // Whether a candidate with the given errors is still considered to be applicable.
    static bool is_applicable(OverloadResolutionErrors errors);

    // ---- constraints -----------------------------------------------------------------------------------------------------

    // Whether the type argument satisfies the constraints of the type parameter. `substitution` defines how type parameters are
    // replaced with type arguments, for constraints that depend on other type parameters (null: none).
    static bool validate_constraints(const ITypeParameter* type_parameter, const IType* type_argument, typesystem::TypeVisitor* substitution = nullptr);
    static bool validate_constraints(const ITypeParameter* type_parameter, const IType* type_argument, typesystem::TypeVisitor* substitution,
                                     CSharpConversions& conversions);

    // ---- output properties -----------------------------------------------------------------------------------------------

    const IParameterizedMember* best_candidate() const;
    // The errors that apply to the best candidate, including those that do not affect applicability (AmbiguousMatch,
    // MethodConstraintsNotSatisfied).
    OverloadResolutionErrors best_candidate_errors();
    bool found_applicable_candidate() const;
    const IParameterizedMember* best_candidate_ambiguous_with() const;
    bool best_candidate_is_expanded_form() const;
    bool is_ambiguous() const { return best_candidate_ambiguous_with_ != nullptr; }
    // The type arguments inferred for the best candidate (none when there is no best candidate or it is not generic).
    const std::vector<const IType*>& inferred_type_arguments() const;
    // The implicit conversions being applied to the arguments.
    std::vector<const Conversion*> argument_conversions() const;
    // Maps argument indices to parameter indices (-1 for arguments that could not be mapped); nullopt when there is no best candidate.
    std::optional<std::vector<int>> get_argument_to_parameter_map() const;

    // The arguments for the method call in the order they were provided (not in the order of the parameters). Arguments are wrapped in a
    // ConversionResolveResult if an implicit conversion is being applied to them when calling the method.
    std::vector<ResolveResult*> get_arguments_with_conversions();
    // The same, and for arguments where an explicit argument name was provided, wrapped in a NamedArgumentResolveResult.
    std::vector<ResolveResult*> get_arguments_with_conversions_and_names();

    // The best candidate with its method type arguments substituted (the method itself when it is not generic).
    const IParameterizedMember* get_best_candidate_with_substituted_type_arguments() const;

    // A ResolveResult representing the result of overload resolution. `target_resolve_result` is the target expression of the call (null
    // for static methods and constructors); `initializer_statements` those of an object or collection initializer; `return_type_override`,
    // when not null, is used as the type of the result instead of the member's return type.
    CSharpInvocationResolveResult* create_resolve_result(ResolveResult* target_resolve_result, std::vector<ResolveResult*> initializer_statements = {},
                                                         const IType* return_type_override = nullptr);

private:
    struct Candidate;
    class ConstraintValidatingSubstitution;

    bool calculate_candidate(const std::shared_ptr<Candidate>& candidate);
    bool resolve_parameter_types(Candidate& candidate, bool use_specialized_parameters);
    void map_corresponding_parameters(Candidate& candidate);
    void run_type_inference(Candidate& candidate);
    OverloadResolutionErrors validate_method_constraints(Candidate& candidate);
    void check_applicability(Candidate& candidate);
    int better_function_member(const Candidate& c1, const Candidate& c2);
    int better_params_collection_type(const IType* params_collection_type1, const IType* params_collection_type2);
    static bool is_array_or_array_interface_type(const IType* type, const IType** element_type);
    int better_parameter_passing_choice(const Candidate& c1, const Candidate& c2);
    int more_specific_formal_parameters(const Candidate& c1, const Candidate& c2);
    static int more_specific_formal_parameters(const std::vector<const IType*>& t1, const std::vector<const IType*>& t2);
    static int more_specific_formal_parameter(const IType* t1, const IType* t2);
    void consider_if_new_candidate_is_best(const std::shared_ptr<Candidate>& candidate);
    std::vector<ResolveResult*> get_arguments_with_conversions(ResolveResult* target_resolve_result, const IParameterizedMember* best_candidate_for_named_arguments);
    typesystem::TypeParameterSubstitution get_substitution(const Candidate& candidate) const;

    const ICompilation* compilation_;
    std::vector<ResolveResult*> arguments_;
    std::vector<std::optional<std::string>> argument_names_;
    CSharpConversions* conversions_;
    std::shared_ptr<Candidate> best_candidate_;
    std::shared_ptr<Candidate> best_candidate_ambiguous_with_;
    // empty when no type arguments were specified
    std::vector<const IType*> explicitly_given_type_arguments_;
    bool best_candidate_was_validated_ = false;
    OverloadResolutionErrors best_candidate_validation_result_ = OverloadResolutionErrors::None;

    bool is_extension_method_invocation_ = false;
    bool allow_expanding_params_ = true;
    bool allow_optional_parameters_ = true;
    bool allow_implicit_in_ = true;
    bool check_for_overflow_ = false;
};

// Performs overload resolution of a method group with the given arguments (upstream: MethodGroupResolveResult.PerformOverloadResolution).
// The methods of the group are tried first; if none is applicable and extension methods are allowed, the extension methods of the group
// (found through the resolver the group holds, on the first ask) are tried with the group's target as the first argument.
OverloadResolution perform_overload_resolution(MethodGroupResolveResult& method_group, const ICompilation& compilation, std::vector<ResolveResult*> arguments,
                                               std::vector<std::optional<std::string>> argument_names = {}, bool allow_extension_methods = true,
                                               bool allow_expanding_params = true, bool allow_optional_parameters = true, bool allow_implicit_in = true,
                                               bool check_for_overflow = false, CSharpConversions* conversions = nullptr);

}  // namespace cb::dotnet::decompiler::csharp::resolver

#include "dotnet/decompiler/csharp/resolver/overload_resolution.hpp"

#include <algorithm>
#include <stdexcept>

#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"
#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/resolver/type_inference.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/named_argument_resolve_result.hpp"
#include "dotnet/decompiler/semantics/out_var_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using semantics::ByReferenceResolveResult;
using semantics::ConversionResolveResult;
using semantics::NamedArgumentResolveResult;
using semantics::OutVarResolveResult;
using semantics::TypeResolveResult;
using typesystem::Accessibility;
using typesystem::ArrayType;
using typesystem::ByReferenceType;
using typesystem::GetMemberOptions;
using typesystem::IParameter;
using typesystem::ITypeDefinition;
using typesystem::ParameterizedType;
using typesystem::ReferenceKind;
using typesystem::TypeKind;
using typesystem::TypeParameterSubstitution;
using typesystem::TypeWithElementType;

namespace {

template <class T, class U>
const T* as_type(const U* p) {
    return dynamic_cast<const T*>(p);
}

}  // namespace

// ---- Candidate ----------------------------------------------------------------------------------------------------------------

struct OverloadResolution::Candidate {
    const IParameterizedMember* member;

    // Whether this is the expanded form (params passed as individual elements) of the member.
    bool is_expanded_form;

    // The parameter types. In the first step, these are the types without any substitution. After type inference, substitutions will be
    // performed.
    std::vector<const IType*> parameter_types;

    // argument index -> parameter index; -1 for arguments that could not be mapped
    std::vector<int> argument_to_parameter_map;

    OverloadResolutionErrors errors = OverloadResolutionErrors::None;
    int error_count = 0;

    bool has_unmapped_optional_parameters = false;

    // (upstream's null array: nullopt)
    std::optional<std::vector<const IType*>> inferred_types;

    // The original member parameters (before any substitution!)
    const std::vector<const IParameter*>* parameters;

    // The original method type parameters (before any substitution!); null for a method without type parameters.
    const std::vector<const ITypeParameter*>* type_parameters = nullptr;

    // Conversions applied to the arguments. This is set by the CheckApplicability step (upstream's null array: nullopt).
    std::optional<std::vector<const Conversion*>> argument_conversions;

    // The type of the collection that is used for the 'params' parameter (before any substitution!). Otherwise unknown.
    const IType* params_collection_type() const {
        if (is_expanded_form && !parameters->empty()) {
            const IParameter* last_parameter = parameters->back();
            if (last_parameter->is_params()) return last_parameter->type();
        }
        return &typesystem::SpecialType::unknown_type();
    }

    bool is_generic_method() const {
        const auto* method = dynamic_cast<const IMethod*>(member);
        return method != nullptr && !method->type_parameters().empty();
    }

    int arguments_passed_to_params() const {
        int count = 0;
        if (is_expanded_form) {
            const int params_parameter_index = static_cast<int>(parameters->size()) - 1;
            for (const int parameter_index : argument_to_parameter_map) {
                if (parameter_index == params_parameter_index) count++;
            }
        }
        return count;
    }

    Candidate(const IParameterizedMember* member_, bool is_expanded) : member(member_), is_expanded_form(is_expanded) {
        const auto* member_definition = dynamic_cast<const IParameterizedMember*>(member_->member_definition());
        // For specialized methods, go back to the original parameters:
        // (without any type parameter substitution, not even class type parameters)
        // We'll re-substitute them as part of RunTypeInference().
        parameters = &member_definition->parameters();
        const auto* method_definition = dynamic_cast<const IMethod*>(member_definition);
        if (method_definition != nullptr && !method_definition->type_parameters().empty()) type_parameters = &method_definition->type_parameters();
        parameter_types.assign(parameters->size(), nullptr);
    }

    void add_error(OverloadResolutionErrors new_error) {
        errors |= new_error;
        if (!OverloadResolution::is_applicable(new_error)) error_count++;
    }
};

// ---- Constructor --------------------------------------------------------------------------------------------------------------

OverloadResolution::OverloadResolution(const ICompilation& compilation, std::vector<ResolveResult*> arguments,
                                       std::vector<std::optional<std::string>> argument_names, std::vector<const IType*> type_arguments,
                                       CSharpConversions* conversions)
    : compilation_(&compilation), arguments_(std::move(arguments)), argument_names_(std::move(argument_names)) {
    if (argument_names_.empty())
        argument_names_.assign(arguments_.size(), std::nullopt);
    else if (argument_names_.size() != arguments_.size())
        throw std::invalid_argument("argumentsNames.Length must be equal to arguments.Length");

    // keep explicitly_given_type_arguments_ empty when no type arguments were specified
    if (!type_arguments.empty()) explicitly_given_type_arguments_ = std::move(type_arguments);

    conversions_ = conversions != nullptr ? conversions : &CSharpConversions::get(compilation);
}

// ---- AddCandidate -------------------------------------------------------------------------------------------------------------

OverloadResolutionErrors OverloadResolution::add_candidate(const IParameterizedMember* member) {
    return add_candidate(member, OverloadResolutionErrors::None);
}

OverloadResolutionErrors OverloadResolution::add_candidate(const IParameterizedMember* member, OverloadResolutionErrors additional_errors) {
    if (member == nullptr) throw std::invalid_argument("member");

    auto c = std::make_shared<Candidate>(member, false);
    c->add_error(additional_errors);
    calculate_candidate(c);

    if (allow_expanding_params_ && !member->parameters().empty() && member->parameters().back()->is_params()) {
        auto expanded_candidate = std::make_shared<Candidate>(member, true);
        expanded_candidate->add_error(additional_errors);
        // consider expanded form only if it isn't obviously wrong
        if (calculate_candidate(expanded_candidate)) {
            if (expanded_candidate->error_count < c->error_count) return expanded_candidate->errors;
        }
    }
    return c->errors;
}

// Calculates applicability etc. for the candidate. Returns true if the calculation was successful, false if the candidate should be
// removed without reporting an error.
bool OverloadResolution::calculate_candidate(const std::shared_ptr<Candidate>& candidate) {
    if (!resolve_parameter_types(*candidate, false)) return false;
    map_corresponding_parameters(*candidate);
    run_type_inference(*candidate);
    check_applicability(*candidate);
    consider_if_new_candidate_is_best(candidate);
    return true;
}

bool OverloadResolution::resolve_parameter_types(Candidate& candidate, bool use_specialized_parameters) {
    for (std::size_t i = 0; i < candidate.parameters->size(); i++) {
        const IType* type;
        if (use_specialized_parameters) {
            // Use the parameter type of the specialized non-generic method or indexer
            type = candidate.member->parameters()[i]->type();
        } else {
            // Use the type of the original formal parameter
            type = (*candidate.parameters)[i]->type();
        }
        if (candidate.is_expanded_form && i == candidate.parameters->size() - 1) {
            const auto* array_type = as_type<ArrayType>(type);
            if (array_type != nullptr && array_type->dimensions() == 1)
                type = array_type->element_type();
            else if (typesystem::is_known_type(type, typesystem::KnownTypeCode::ReadOnlySpanOfT) || typesystem::is_known_type(type, typesystem::KnownTypeCode::SpanOfT))
                type = type->type_arguments()[0];
            else if (typesystem::is_array_interface_type(type))
                type = type->type_arguments()[0];
            else
                return false;   // error: cannot unpack params-array. abort considering the expanded form for this candidate
        }
        candidate.parameter_types[i] = type;
    }
    return true;
}

// ---- AddMethodLists -----------------------------------------------------------------------------------------------------------

void OverloadResolution::add_method_lists(const std::vector<MethodListWithDeclaringType>& method_lists) {
    // Base types come first, so go through the list backwards (derived types first)
    std::vector<char> is_hidden_by_derived_type(method_lists.size(), 0);
    for (int i = static_cast<int>(method_lists.size()) - 1; i >= 0; i--) {
        if (is_hidden_by_derived_type[static_cast<std::size_t>(i)]) continue;

        const MethodListWithDeclaringType& method_list = method_lists[static_cast<std::size_t>(i)];
        bool found_applicable_candidate_in_current_list = false;

        for (std::size_t j = 0; j < method_list.size(); j++) {
            const IParameterizedMember* method = method_list[j];
            const OverloadResolutionErrors errors = add_candidate(method);
            found_applicable_candidate_in_current_list |= is_applicable(errors);
        }

        if (found_applicable_candidate_in_current_list && i > 0) {
            for (const IType* base_type : typesystem::get_all_base_types(method_list.declaring_type())) {
                for (int j = 0; j < i; j++) {
                    if (!is_hidden_by_derived_type[static_cast<std::size_t>(j)] && typesystem::type_equals(base_type, method_lists[static_cast<std::size_t>(j)].declaring_type()))
                        is_hidden_by_derived_type[static_cast<std::size_t>(j)] = 1;
                }
            }
        }
    }
}

// ---- MapCorrespondingParameters -----------------------------------------------------------------------------------------------

void OverloadResolution::map_corresponding_parameters(Candidate& candidate) {
    // C# spec (draft-v11): 12.6.2.2 Corresponding parameters
    // (includes the non-trailing named arguments rule from C# 7.2)
    candidate.argument_to_parameter_map.assign(arguments_.size(), 0);
    bool has_positional_argument = false;
    // go backwards, so that has_positional_argument tells us whether there
    // are non-trailing named arguments
    for (int i = static_cast<int>(arguments_.size()) - 1; i >= 0; i--) {
        const std::size_t ui = static_cast<std::size_t>(i);
        candidate.argument_to_parameter_map[ui] = -1;
        if (!argument_names_[ui].has_value() || has_positional_argument) {
            has_positional_argument = true;
            // positional argument or non-trailing named argument
            if (ui < candidate.parameter_types.size()) {
                candidate.argument_to_parameter_map[ui] = i;
                if (argument_names_[ui].has_value() && *argument_names_[ui] != (*candidate.parameters)[ui]->name()) {
                    // non-trailing named argument must match name
                    candidate.add_error(OverloadResolutionErrors::NoParameterFoundForNamedArgument);
                }
            } else if (candidate.is_expanded_form) {
                candidate.argument_to_parameter_map[ui] = static_cast<int>(candidate.parameter_types.size()) - 1;
                if (argument_names_[ui].has_value()) {
                    // can't use non-trailing named argument here
                    candidate.add_error(OverloadResolutionErrors::NoParameterFoundForNamedArgument);
                }
            } else {
                candidate.add_error(OverloadResolutionErrors::TooManyPositionalArguments);
            }
        } else {
            // (trailing) named argument
            for (std::size_t j = 0; j < candidate.parameters->size(); j++) {
                if (*argument_names_[ui] == (*candidate.parameters)[j]->name()) candidate.argument_to_parameter_map[ui] = static_cast<int>(j);
            }
            if (candidate.argument_to_parameter_map[ui] < 0) candidate.add_error(OverloadResolutionErrors::NoParameterFoundForNamedArgument);
        }
    }
}

class OverloadResolution::ConstraintValidatingSubstitution final : public TypeParameterSubstitution {
public:
    ConstraintValidatingSubstitution(std::optional<std::vector<const IType*>> class_type_arguments, std::optional<std::vector<const IType*>> method_type_arguments,
                                     OverloadResolution& overload_resolution)
        : TypeParameterSubstitution(std::move(class_type_arguments), std::move(method_type_arguments)), conversions_(overload_resolution.conversions_) {}

    bool constraints_valid = true;

    const IType* visit_parameterized_type(const ParameterizedType& type) override {
        const IType* new_type = TypeParameterSubstitution::visit_parameterized_type(type);
        if (new_type != static_cast<const IType*>(&type) && constraints_valid) {
            // something was changed, so we need to validate the constraints
            const auto* new_parameterized_type = as_type<ParameterizedType>(new_type);
            if (new_parameterized_type != nullptr) {
                // C# 4.0 spec: 4.4.4 Satisfying constraints
                const std::vector<const ITypeParameter*>& type_parameters = new_parameterized_type->type_parameters();
                TypeParameterSubstitution substitution = new_parameterized_type->get_substitution();
                for (std::size_t i = 0; i < type_parameters.size(); i++) {
                    if (!validate_constraints(type_parameters[i], new_parameterized_type->get_type_argument(static_cast<int>(i)), &substitution, *conversions_)) {
                        constraints_valid = false;
                        break;
                    }
                }
            }
        }
        return new_type;
    }

private:
    CSharpConversions* conversions_;
};

// ---- RunTypeInference ---------------------------------------------------------------------------------------------------------

void OverloadResolution::run_type_inference(Candidate& candidate) {
    if (candidate.type_parameters == nullptr) {
        if (!explicitly_given_type_arguments_.empty()) {
            // method does not expect type arguments, but was given some
            candidate.add_error(OverloadResolutionErrors::WrongNumberOfTypeArguments);
        }
        // Grab new parameter types:
        resolve_parameter_types(candidate, true);
        return;
    }
    const auto* parameterized_declaring_type = as_type<ParameterizedType>(candidate.member->declaring_type());
    std::optional<std::vector<const IType*>> class_type_arguments;
    if (parameterized_declaring_type != nullptr) class_type_arguments = parameterized_declaring_type->type_arguments();
    // The method is generic:
    if (!explicitly_given_type_arguments_.empty()) {
        if (explicitly_given_type_arguments_.size() == candidate.type_parameters->size()) {
            candidate.inferred_types = explicitly_given_type_arguments_;
        } else {
            candidate.add_error(OverloadResolutionErrors::WrongNumberOfTypeArguments);
            // wrong number of type arguments given, so truncate the list or pad with UnknownType
            std::vector<const IType*> inferred(candidate.type_parameters->size());
            for (std::size_t i = 0; i < inferred.size(); i++) {
                if (i < explicitly_given_type_arguments_.size())
                    inferred[i] = explicitly_given_type_arguments_[i];
                else
                    inferred[i] = &typesystem::SpecialType::unknown_type();
            }
            candidate.inferred_types = std::move(inferred);
        }
    } else {
        TypeInference ti(*compilation_, *conversions_);
        std::vector<const IType*> parameter_types;
        for (const int parameter_index : candidate.argument_to_parameter_map) {
            parameter_types.push_back(parameter_index >= 0 ? candidate.parameter_types[static_cast<std::size_t>(parameter_index)]
                                                           : &typesystem::SpecialType::unknown_type());
        }
        bool success;
        candidate.inferred_types = ti.infer_type_arguments(*candidate.type_parameters, arguments_, parameter_types, &success, class_type_arguments);
        if (!success) candidate.add_error(OverloadResolutionErrors::TypeInferenceFailed);
    }
    // Now substitute in the formal parameters:
    ConstraintValidatingSubstitution substitution(class_type_arguments, candidate.inferred_types, *this);
    for (std::size_t i = 0; i < candidate.parameter_types.size(); i++) {
        candidate.parameter_types[i] = candidate.parameter_types[i]->accept_visitor(substitution);
    }
    if (!substitution.constraints_valid) candidate.add_error(OverloadResolutionErrors::ConstructedTypeDoesNotSatisfyConstraint);
}

// ---- Validate Constraints -----------------------------------------------------------------------------------------------------

OverloadResolutionErrors OverloadResolution::validate_method_constraints(Candidate& candidate) {
    // If type inference already failed, we won't check the constraints:
    if ((candidate.errors & OverloadResolutionErrors::TypeInferenceFailed) != OverloadResolutionErrors::None) return OverloadResolutionErrors::None;

    if (candidate.type_parameters == nullptr || candidate.type_parameters->empty()) return OverloadResolutionErrors::None;   // the method isn't generic
    TypeParameterSubstitution substitution = get_substitution(candidate);
    for (std::size_t i = 0; i < candidate.type_parameters->size(); i++) {
        if (!validate_constraints((*candidate.type_parameters)[i], (*substitution.method_type_arguments())[i], &substitution))
            return OverloadResolutionErrors::MethodConstraintsNotSatisfied;
    }
    return OverloadResolutionErrors::None;
}

bool OverloadResolution::validate_constraints(const ITypeParameter* type_parameter, const IType* type_argument, typesystem::TypeVisitor* substitution) {
    if (type_parameter == nullptr) throw std::invalid_argument("type_parameter");
    if (type_argument == nullptr) throw std::invalid_argument("type_argument");
    if (type_parameter->owner() == nullptr) throw std::invalid_argument("type_parameter has no owner");
    return validate_constraints(type_parameter, type_argument, substitution, CSharpConversions::get(type_parameter->owner()->compilation()));
}

bool OverloadResolution::validate_constraints(const ITypeParameter* type_parameter, const IType* type_argument, typesystem::TypeVisitor* substitution,
                                              CSharpConversions& conversions) {
    switch (type_argument->kind()) {   // void, null, and pointers cannot be used as type arguments
    case TypeKind::Void:
    case TypeKind::Null:
    case TypeKind::Pointer: return false;
    default: break;
    }
    if (type_parameter->has_reference_type_constraint()) {
        if (type_argument->is_reference_type() != std::optional<bool>(true)) return false;
    }
    if (type_parameter->has_value_type_constraint()) {
        if (!typesystem::nullable_type::is_non_nullable_value_type(type_argument)) return false;
    }
    if (type_parameter->has_default_constructor_constraint()) {
        const ITypeDefinition* def = type_argument->get_definition();
        if (def != nullptr && def->is_abstract()) return false;
        const auto ctors = type_argument->get_constructors(
            [](const IMethod* m) { return m->parameters().empty() && m->accessibility() == Accessibility::Public; },
            GetMemberOptions::IgnoreInheritedMembers | GetMemberOptions::ReturnMemberDefinitions);
        if (ctors.empty()) return false;
    }
    for (const IType* constraint_type : type_parameter->direct_base_types()) {
        const IType* c = constraint_type;
        if (substitution != nullptr) c = c->accept_visitor(*substitution);
        if (!conversions.is_constraint_convertible(type_argument, c)) return false;
    }
    return true;
}

// ---- CheckApplicability -------------------------------------------------------------------------------------------------------

bool OverloadResolution::is_applicable(OverloadResolutionErrors errors) {
    constexpr OverloadResolutionErrors errors_that_do_not_matter_for_applicability =
        OverloadResolutionErrors::AmbiguousMatch | OverloadResolutionErrors::MethodConstraintsNotSatisfied;
    return (errors & ~errors_that_do_not_matter_for_applicability) == OverloadResolutionErrors::None;
}

void OverloadResolution::check_applicability(Candidate& candidate) {
    // C# 4.0 spec: 7.5.3.1 Applicable function member

    // Test whether parameters were mapped the correct number of arguments:
    std::vector<int> argument_count_per_parameter(candidate.parameter_types.size(), 0);
    for (const int parameter_index : candidate.argument_to_parameter_map) {
        if (parameter_index >= 0) argument_count_per_parameter[static_cast<std::size_t>(parameter_index)]++;
    }
    for (std::size_t i = 0; i < argument_count_per_parameter.size(); i++) {
        if (candidate.is_expanded_form && i == argument_count_per_parameter.size() - 1) continue;   // any number of arguments is fine for the params-array
        if (argument_count_per_parameter[i] == 0) {
            if (allow_optional_parameters_ && (*candidate.parameters)[i]->is_optional())
                candidate.has_unmapped_optional_parameters = true;
            else
                candidate.add_error(OverloadResolutionErrors::MissingArgumentForRequiredParameter);
        } else if (argument_count_per_parameter[i] > 1) {
            candidate.add_error(OverloadResolutionErrors::MultipleArgumentsForSingleParameter);
        }
    }

    candidate.argument_conversions = std::vector<const Conversion*>(arguments_.size(), nullptr);
    // Test whether argument passing mode matches the parameter passing mode
    for (std::size_t i = 0; i < arguments_.size(); i++) {
        const int parameter_index = candidate.argument_to_parameter_map[i];
        if (parameter_index < 0) {
            (*candidate.argument_conversions)[i] = Conversion::none();
            continue;
        }
        const std::size_t up = static_cast<std::size_t>(parameter_index);

        const ReferenceKind param_ref_kind = (*candidate.parameters)[up]->reference_kind();
        if (auto* brrr = dynamic_cast<ByReferenceResolveResult*>(arguments_[i]); brrr != nullptr) {
            if (brrr->reference_kind() != param_ref_kind) candidate.add_error(OverloadResolutionErrors::ParameterPassingModeMismatch);
        } else if (dynamic_cast<OutVarResolveResult*>(arguments_[i]) != nullptr) {
            if (param_ref_kind != ReferenceKind::Out) candidate.add_error(OverloadResolutionErrors::ParameterPassingModeMismatch);
            // 'out var decl' arguments are compatible with any out parameter
            continue;
        } else {
            // AllowImplicitIn: `in` parameters can be filled implicitly without `in` DirectionExpression
            // IsExtensionMethodInvocation: `this ref` and `this in` parameters can be filled implicitly
            const auto* brt = as_type<ByReferenceType>(typesystem::skip_modifiers(candidate.parameter_types[up]));
            if ((((param_ref_kind == ReferenceKind::In || param_ref_kind == ReferenceKind::RefReadOnly) && allow_implicit_in_) ||
                 (is_extension_method_invocation_ && parameter_index == 0 &&
                  (param_ref_kind == ReferenceKind::In || param_ref_kind == ReferenceKind::Ref || param_ref_kind == ReferenceKind::RefReadOnly))) &&
                brt != nullptr) {
                // Treat the parameter as if it was not declared "in" for the following steps
                // (applicability + better function member)
                candidate.parameter_types[up] = brt->element_type();
            } else if (param_ref_kind != ReferenceKind::None) {
                candidate.add_error(OverloadResolutionErrors::ParameterPassingModeMismatch);
            }
        }
        const IType* parameter_type = candidate.parameter_types[up];
        const Conversion* c = conversions_->implicit_conversion(arguments_[i], parameter_type);
        (*candidate.argument_conversions)[i] = c;
        if (is_extension_method_invocation_ && parameter_index == 0) {
            // First parameter to extension method must be an identity, reference, boxing or span conversion
            if (!(c == Conversion::identity_conversion() || c == Conversion::implicit_reference_conversion() || c == Conversion::boxing_conversion() ||
                  c == Conversion::implicit_span_conversion()))
                candidate.add_error(OverloadResolutionErrors::ArgumentTypeMismatch);
        } else {
            if ((!c->is_valid() && !c->is_user_defined() && !c->is_method_group_conversion()) && parameter_type->kind() != TypeKind::Unknown)
                candidate.add_error(OverloadResolutionErrors::ArgumentTypeMismatch);
        }
    }
}

// ---- BetterFunctionMember -----------------------------------------------------------------------------------------------------

// Returns 1 if c1 is better than c2; 2 if c2 is better than c1; or 0 if neither is better.
int OverloadResolution::better_function_member(const Candidate& c1, const Candidate& c2) {
    // prefer applicable members (part of heuristic that produces a best candidate even if none is applicable)
    if (c1.error_count == 0 && c2.error_count > 0) return 1;
    if (c1.error_count > 0 && c2.error_count == 0) return 2;

    // C# 4.0 spec: 7.5.3.2 Better function member
    bool c1_is_better = false;
    bool c2_is_better = false;
    bool parameter_types_equal = true;
    for (std::size_t i = 0; i < arguments_.size(); i++) {
        const int p1 = c1.argument_to_parameter_map[i];
        const int p2 = c2.argument_to_parameter_map[i];
        if (p1 >= 0 && p2 < 0) {
            c1_is_better = true;
        } else if (p1 < 0 && p2 >= 0) {
            c2_is_better = true;
        } else if (p1 >= 0 && p2 >= 0) {
            const IType* t1 = c1.parameter_types[static_cast<std::size_t>(p1)];
            const IType* t2 = c2.parameter_types[static_cast<std::size_t>(p2)];
            if (!conversions_->identity_conversion(t1, t2)) parameter_types_equal = false;
            switch (conversions_->better_conversion(arguments_[i], t1, t2)) {
            case 1: c1_is_better = true; break;
            case 2: c2_is_better = true; break;
            default: break;
            }
        }
    }
    if (c1_is_better && !c2_is_better) return 1;
    if (!c1_is_better && c2_is_better) return 2;

    // prefer members with less errors (part of heuristic that produces a best candidate even if none is applicable)
    if (c1.error_count < c2.error_count) return 1;
    if (c1.error_count > c2.error_count) return 2;

    if (!c1_is_better && !c2_is_better && parameter_types_equal) {
        // we need the tie-breaking rules

        // non-generic methods are better
        if (!c1.is_generic_method() && c2.is_generic_method())
            return 1;
        else if (c1.is_generic_method() && !c2.is_generic_method())
            return 2;

        // non-expanded members are better
        if (!c1.is_expanded_form && c2.is_expanded_form)
            return 1;
        else if (c1.is_expanded_form && !c2.is_expanded_form)
            return 2;

        // prefer the member with less arguments mapped to the params-collection
        int r = c1.arguments_passed_to_params() < c2.arguments_passed_to_params() ? -1 : (c1.arguments_passed_to_params() > c2.arguments_passed_to_params() ? 1 : 0);
        if (r < 0)
            return 1;
        else if (r > 0)
            return 2;

        // prefer the member where no default values need to be substituted
        if (!c1.has_unmapped_optional_parameters && c2.has_unmapped_optional_parameters)
            return 1;
        else if (c1.has_unmapped_optional_parameters && !c2.has_unmapped_optional_parameters)
            return 2;

        // compare the formal parameters
        r = more_specific_formal_parameters(c1, c2);
        if (r != 0) return r;

        // prefer non-lifted operators
        const auto* lift1 = dynamic_cast<const ILiftedOperator*>(c1.member);
        const auto* lift2 = dynamic_cast<const ILiftedOperator*>(c2.member);
        if (lift1 == nullptr && lift2 != nullptr) return 1;
        if (lift1 != nullptr && lift2 == nullptr) return 2;

        // prefer by-value parameters over in-parameters
        r = better_parameter_passing_choice(c1, c2);
        if (r != 0) return r;

        if (c1.is_expanded_form) {
            r = better_params_collection_type(c1.params_collection_type(), c2.params_collection_type());
            if (r != 0) return r;
        }
    }
    return 0;
}

int OverloadResolution::better_params_collection_type(const IType* params_collection_type1, const IType* params_collection_type2) {
    // see https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/proposals/csharp-13.0/params-collections#better-function-member
    using typesystem::KnownTypeCode;
    const bool is_span1 = typesystem::is_known_type(params_collection_type1, KnownTypeCode::SpanOfT) || typesystem::is_known_type(params_collection_type1, KnownTypeCode::ReadOnlySpanOfT);
    const bool is_span2 = typesystem::is_known_type(params_collection_type2, KnownTypeCode::SpanOfT) || typesystem::is_known_type(params_collection_type2, KnownTypeCode::ReadOnlySpanOfT);
    const IType* element_type = nullptr;
    if (!is_span1 && !is_span2) {
        const bool implicit_conversion1to2 = conversions_->implicit_conversion(params_collection_type1, params_collection_type2)->is_valid();
        const bool implicit_conversion2to1 = conversions_->implicit_conversion(params_collection_type2, params_collection_type1)->is_valid();
        if (implicit_conversion1to2 && !implicit_conversion2to1) return 1;
        if (!implicit_conversion1to2 && implicit_conversion2to1) return 2;
    } else if (typesystem::is_known_type(params_collection_type1, KnownTypeCode::ReadOnlySpanOfT) &&
               typesystem::is_known_type(params_collection_type2, KnownTypeCode::SpanOfT)) {
        if (conversions_->identity_conversion(params_collection_type1->type_arguments()[0], params_collection_type2->type_arguments()[0]))
            return 1;   // ReadOnlySpan<T> is better than Span<T> if there exists an identity conversion between the element types
    } else if (typesystem::is_known_type(params_collection_type2, KnownTypeCode::ReadOnlySpanOfT) &&
               typesystem::is_known_type(params_collection_type1, KnownTypeCode::SpanOfT)) {
        if (conversions_->identity_conversion(params_collection_type2->type_arguments()[0], params_collection_type1->type_arguments()[0]))
            return 2;   // ReadOnlySpan<T> is better than Span<T> if there exists an identity conversion between the element types
    } else if (is_span1 && is_array_or_array_interface_type(params_collection_type2, &element_type)) {
        if (conversions_->identity_conversion(params_collection_type1->type_arguments()[0], element_type))
            return 1;   // Span<T> is better than an array type if there exists an identity conversion between the element types
    } else if (is_span2 && is_array_or_array_interface_type(params_collection_type1, &element_type)) {
        if (conversions_->identity_conversion(params_collection_type2->type_arguments()[0], element_type))
            return 2;   // Span<T> is better than an array type if there exists an identity conversion between the element types
    }
    return 0;
}

bool OverloadResolution::is_array_or_array_interface_type(const IType* type, const IType** element_type) {
    *element_type = nullptr;
    if (const auto* array_type = as_type<ArrayType>(type); array_type != nullptr) {
        *element_type = array_type->element_type();
        return true;
    }
    if (typesystem::is_array_interface_type(type)) {
        *element_type = type->type_arguments()[0];
        return true;
    }
    return false;
}

int OverloadResolution::better_parameter_passing_choice(const Candidate& c1, const Candidate& c2) {
    bool c1_is_better = false;
    bool c2_is_better = false;

    for (std::size_t i = 0; i < c1.parameters->size(); i++) {
        const ReferenceKind ref_kind1 = (*c1.parameters)[i]->reference_kind();
        const ReferenceKind ref_kind2 = (*c2.parameters)[i]->reference_kind();

        if (ref_kind1 == ReferenceKind::None && ref_kind2 == ReferenceKind::In) c1_is_better = true;   // by-value is better than in
        if (ref_kind1 == ReferenceKind::In && ref_kind2 == ReferenceKind::None) c2_is_better = true;
    }

    if (c1_is_better && !c2_is_better) return 1;
    if (!c1_is_better && c2_is_better) return 2;

    return 0;
}

int OverloadResolution::more_specific_formal_parameters(const Candidate& c1, const Candidate& c2) {
    // prefer the member with more formal parameters (in case both have different number of optional parameters)
    if (c1.parameters->size() > c2.parameters->size())
        return 1;
    else if (c1.parameters->size() < c2.parameters->size())
        return 2;

    std::vector<const IType*> t1, t2;
    for (const IParameter* p : *c1.parameters) t1.push_back(p->type());
    for (const IParameter* p : *c2.parameters) t2.push_back(p->type());
    return more_specific_formal_parameters(t1, t2);
}

int OverloadResolution::more_specific_formal_parameters(const std::vector<const IType*>& t1, const std::vector<const IType*>& t2) {
    bool c1_is_better = false;
    bool c2_is_better = false;
    const std::size_t count = std::min(t1.size(), t2.size());
    for (std::size_t i = 0; i < count; i++) {
        switch (more_specific_formal_parameter(t1[i], t2[i])) {
        case 1: c1_is_better = true; break;
        case 2: c2_is_better = true; break;
        default: break;
        }
    }
    if (c1_is_better && !c2_is_better) return 1;
    if (!c1_is_better && c2_is_better) return 2;
    return 0;
}

int OverloadResolution::more_specific_formal_parameter(const IType* t1, const IType* t2) {
    if ((as_type<ITypeParameter>(t1) != nullptr) && !(as_type<ITypeParameter>(t2) != nullptr)) return 2;
    if ((as_type<ITypeParameter>(t2) != nullptr) && !(as_type<ITypeParameter>(t1) != nullptr)) return 1;

    const auto* p1 = as_type<ParameterizedType>(t1);
    const auto* p2 = as_type<ParameterizedType>(t2);
    if (p1 != nullptr && p2 != nullptr && p1->type_parameter_count() == p2->type_parameter_count()) {
        const int r = more_specific_formal_parameters(p1->type_arguments(), p2->type_arguments());
        if (r > 0) return r;
    }
    const auto* tew1 = as_type<TypeWithElementType>(t1);
    const auto* tew2 = as_type<TypeWithElementType>(t2);
    if (tew1 != nullptr && tew2 != nullptr) return more_specific_formal_parameter(tew1->element_type(), tew2->element_type());
    return 0;
}

// ---- ConsiderIfNewCandidateIsBest ---------------------------------------------------------------------------------------------

void OverloadResolution::consider_if_new_candidate_is_best(const std::shared_ptr<Candidate>& candidate) {
    if (best_candidate_ == nullptr) {
        best_candidate_ = candidate;
        best_candidate_was_validated_ = false;
    } else {
        switch (better_function_member(*candidate, *best_candidate_)) {
        case 0:
            // Overwrite 'best_candidate_ambiguous_with_' so that API users can
            // detect the set of all ambiguous methods if they look at
            // best_candidate_ambiguous_with after each step.
            best_candidate_ambiguous_with_ = candidate;
            break;
        case 1:
            best_candidate_ = candidate;
            best_candidate_was_validated_ = false;
            best_candidate_ambiguous_with_ = nullptr;
            break;
        default:
            // case 2: best candidate stays best
            break;
        }
    }
}

// ---- Output Properties --------------------------------------------------------------------------------------------------------

const IParameterizedMember* OverloadResolution::best_candidate() const { return best_candidate_ != nullptr ? best_candidate_->member : nullptr; }

// Returns the errors that apply to the best candidate. This includes additional errors that do not affect applicability (e.g.
// AmbiguousMatch, MethodConstraintsNotSatisfied)
OverloadResolutionErrors OverloadResolution::best_candidate_errors() {
    if (best_candidate_ == nullptr) return OverloadResolutionErrors::None;
    if (!best_candidate_was_validated_) {
        best_candidate_validation_result_ = validate_method_constraints(*best_candidate_);
        best_candidate_was_validated_ = true;
    }
    OverloadResolutionErrors err = best_candidate_->errors | best_candidate_validation_result_;
    if (best_candidate_ambiguous_with_ != nullptr) err |= OverloadResolutionErrors::AmbiguousMatch;
    return err;
}

bool OverloadResolution::found_applicable_candidate() const { return best_candidate_ != nullptr && is_applicable(best_candidate_->errors); }

const IParameterizedMember* OverloadResolution::best_candidate_ambiguous_with() const {
    return best_candidate_ambiguous_with_ != nullptr ? best_candidate_ambiguous_with_->member : nullptr;
}

bool OverloadResolution::best_candidate_is_expanded_form() const { return best_candidate_ != nullptr ? best_candidate_->is_expanded_form : false; }

const std::vector<const IType*>& OverloadResolution::inferred_type_arguments() const {
    static const std::vector<const IType*> empty;
    if (best_candidate_ != nullptr && best_candidate_->inferred_types.has_value()) return *best_candidate_->inferred_types;
    return empty;
}

std::vector<const Conversion*> OverloadResolution::argument_conversions() const {
    if (best_candidate_ != nullptr && best_candidate_->argument_conversions.has_value()) return *best_candidate_->argument_conversions;
    return std::vector<const Conversion*>(arguments_.size(), Conversion::none());
}

std::optional<std::vector<int>> OverloadResolution::get_argument_to_parameter_map() const {
    if (best_candidate_ != nullptr) return best_candidate_->argument_to_parameter_map;
    return std::nullopt;
}

std::vector<ResolveResult*> OverloadResolution::get_arguments_with_conversions() {
    if (best_candidate_ == nullptr) return arguments_;
    return get_arguments_with_conversions(nullptr, nullptr);
}

std::vector<ResolveResult*> OverloadResolution::get_arguments_with_conversions_and_names() {
    if (best_candidate_ == nullptr) return arguments_;
    return get_arguments_with_conversions(nullptr, get_best_candidate_with_substituted_type_arguments());
}

std::vector<ResolveResult*> OverloadResolution::get_arguments_with_conversions(ResolveResult* target_resolve_result,
                                                                               const IParameterizedMember* best_candidate_for_named_arguments) {
    const std::vector<const Conversion*> conversions = argument_conversions();
    std::vector<ResolveResult*> args(arguments_.size());
    for (std::size_t i = 0; i < args.size(); i++) {
        ResolveResult* argument = arguments_[i];
        if (is_extension_method_invocation_ && i == 0 && target_resolve_result != nullptr) argument = target_resolve_result;
        const int parameter_index = best_candidate_->argument_to_parameter_map[i];
        if (parameter_index >= 0 && conversions[i] != Conversion::identity_conversion()) {
            // Wrap argument in ConversionResolveResult
            const IType* parameter_type = best_candidate_->parameter_types[static_cast<std::size_t>(parameter_index)];
            if (parameter_type->kind() != TypeKind::Unknown) {
                if (arguments_[i]->is_compile_time_constant() && conversions[i]->is_valid() && !conversions[i]->is_user_defined()) {
                    CSharpResolver resolver = CSharpResolver(*compilation_).with_check_for_overflow(check_for_overflow_);
                    argument = resolver.resolve_cast(parameter_type, argument);
                } else {
                    argument = make<ConversionResolveResult>(parameter_type, argument, conversions[i], check_for_overflow_);
                }
            }
        }
        if (best_candidate_for_named_arguments != nullptr && argument_names_[i].has_value()) {
            // Wrap argument in NamedArgumentResolveResult
            if (parameter_index >= 0) {
                argument = make<NamedArgumentResolveResult>(best_candidate_for_named_arguments->parameters()[static_cast<std::size_t>(parameter_index)], argument,
                                                            best_candidate_for_named_arguments);
            } else {
                argument = make<NamedArgumentResolveResult>(*argument_names_[i], argument);
            }
        }
        args[i] = argument;
    }
    return args;
}

const IParameterizedMember* OverloadResolution::get_best_candidate_with_substituted_type_arguments() const {
    if (best_candidate_ == nullptr) return nullptr;
    const auto* method = dynamic_cast<const IMethod*>(best_candidate_->member);
    if (method != nullptr && !method->type_parameters().empty()) {
        return dynamic_cast<const IMethod*>(method->member_definition())->specialize_method(get_substitution(*best_candidate_));
    }
    return best_candidate_->member;
}

TypeParameterSubstitution OverloadResolution::get_substitution(const Candidate& candidate) const {
    // Do not compose the substitutions, but merge them.
    // This is required for InvocationTests.SubstituteClassAndMethodTypeParametersAtOnce
    return TypeParameterSubstitution(candidate.member->substitution().class_type_arguments(), candidate.inferred_types);
}

CSharpInvocationResolveResult* OverloadResolution::create_resolve_result(ResolveResult* target_resolve_result, std::vector<ResolveResult*> initializer_statements,
                                                                         const IType* return_type_override) {
    const IParameterizedMember* member = get_best_candidate_with_substituted_type_arguments();
    if (member == nullptr) throw std::logic_error("overload resolution has no best candidate");

    ResolveResult* target = target_resolve_result;
    if (is_extension_method_invocation_) {
        const IType* declaring_type = member->declaring_type();
        target = make<TypeResolveResult>(declaring_type != nullptr ? declaring_type : &typesystem::SpecialType::unknown_type());
    }
    std::vector<ResolveResult*> arguments = get_arguments_with_conversions(target_resolve_result, member);
    const OverloadResolutionErrors errors = best_candidate_errors();
    const bool expanded = best_candidate_is_expanded_form();
    std::optional<std::vector<int>> map = get_argument_to_parameter_map();
    return make<CSharpInvocationResolveResult>(target, member, std::move(arguments), errors, is_extension_method_invocation_, expanded, false, std::move(map),
                                               std::move(initializer_statements), return_type_override);
}

// ---- PerformOverloadResolution (MethodGroupResolveResult) ---------------------------------------------------------------------

OverloadResolution perform_overload_resolution(MethodGroupResolveResult& method_group, const ICompilation& compilation, std::vector<ResolveResult*> arguments,
                                               std::vector<std::optional<std::string>> argument_names, bool allow_extension_methods, bool allow_expanding_params,
                                               bool allow_optional_parameters, bool allow_implicit_in, bool check_for_overflow, CSharpConversions* conversions) {
    const std::vector<const IType*> type_argument_array = method_group.type_arguments();
    OverloadResolution overload_resolution(compilation, arguments, argument_names, type_argument_array, conversions);
    overload_resolution.set_allow_expanding_params(allow_expanding_params);
    overload_resolution.set_allow_optional_parameters(allow_optional_parameters);
    overload_resolution.set_check_for_overflow(check_for_overflow);
    overload_resolution.set_allow_implicit_in(allow_implicit_in);

    overload_resolution.add_method_lists(method_group.methods_grouped_by_declaring_type());

    if (allow_extension_methods && !overload_resolution.found_applicable_candidate()) {
        // No applicable match found, so let's try extension methods.

        // MethodGroupResolveResult.GetExtensionMethods: found on the first ask through the resolver the group holds
        if (method_group.resolver != nullptr) {
            struct ClearResolver {
                MethodGroupResolveResult& group;
                ~ClearResolver() { group.resolver = nullptr; }
            } clear_resolver{method_group};
            method_group.extension_methods = method_group.resolver->get_extension_methods(method_group.method_name(), method_group.type_arguments());
        }
        const std::vector<std::vector<const IMethod*>>& extension_methods = method_group.extension_methods;

        if (!extension_methods.empty()) {
            std::vector<ResolveResult*> ext_arguments(arguments.size() + 1);
            ext_arguments[0] = make<ResolveResult>(method_group.target_type());
            std::copy(arguments.begin(), arguments.end(), ext_arguments.begin() + 1);
            std::vector<std::optional<std::string>> ext_argument_names;
            if (!argument_names.empty()) {
                ext_argument_names.assign(argument_names.size() + 1, std::nullopt);
                std::copy(argument_names.begin(), argument_names.end(), ext_argument_names.begin() + 1);
            }
            OverloadResolution ext_or(compilation, std::move(ext_arguments), std::move(ext_argument_names), type_argument_array, conversions);
            ext_or.set_allow_expanding_params(allow_expanding_params);
            ext_or.set_allow_optional_parameters(allow_optional_parameters);
            ext_or.set_is_extension_method_invocation(true);
            ext_or.set_check_for_overflow(check_for_overflow);
            ext_or.set_allow_implicit_in(allow_implicit_in);

            for (const std::vector<const IMethod*>& g : extension_methods) {
                for (const IMethod* method : g) ext_or.add_candidate(method);
                if (ext_or.found_applicable_candidate()) break;
            }
            // For the lack of a better comparison function (the one within OverloadResolution
            // cannot be used as it depends on the argument set):
            if (ext_or.found_applicable_candidate() || overload_resolution.best_candidate() == nullptr) {
                // Consider an extension method result better than the normal result only
                // if it's applicable; or if there is no normal result.
                overload_resolution = std::move(ext_or);
            }
        }
    }
    return overload_resolution;
}

}  // namespace cb::dotnet::decompiler::csharp::resolver

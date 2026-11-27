// dotnet/decompiler/csharp/resolver/type_inference.hpp -- C# type inference (spec 12.6.3): the type arguments of a generic method call
// worked out from the arguments, by collecting exact, lower and upper bounds for each type parameter and fixing them; also the best common
// type of a set of expressions and a type that satisfies given bounds.
//
//     TypeInference inference(compilation);
//     bool success;
//     std::vector<const IType*> type_arguments =
//         inference.infer_type_arguments(method->type_parameters(), arguments, parameter_types, &success);
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: TypeInferenceAlgorithm, TypeInference, from TypeInference.cs), Copyright
// AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Upstream's HashSet<IType> of bounds is TypeSet (distinct by the types' equality, in the order added: .NET enumerates such a set in
// insertion order as long as nothing is removed, and the bounds are never removed). Upstream's Log (a debugging trace) is not ported.
#pragma once

#include <optional>
#include <vector>

#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using typesystem::ITypeParameter;

class LambdaResolveResult;

enum class TypeInferenceAlgorithm {
    // C# 4.0 type inference.
    CSharp4,
    // Improved algorithm (not part of any specification) using FindTypeInBounds for fixing.
    Improved,
    // Improved algorithm (not part of any specification) using FindTypeInBounds for fixing; uses IntersectionType to report all results
    // (in case of ambiguities).
    ImprovedReturnAllResults,
};

class TypeInference {
public:
    explicit TypeInference(const ICompilation& compilation);
    // With the compilation's conversions already at hand (upstream's internal constructor).
    TypeInference(const ICompilation& compilation, CSharpConversions& conversions);

    TypeInferenceAlgorithm algorithm() const { return algorithm_; }
    void set_algorithm(TypeInferenceAlgorithm algorithm) { algorithm_ = algorithm; }

    // Performs type inference. `type_parameters` are the method type parameters to infer, `arguments` the arguments passed to the method,
    // `parameter_types` its parameter types; *success tells whether inference worked. `class_type_arguments` are substituted for class type
    // parameters in the formal parameter types when inferring a method group or lambda. Returns the inferred type arguments (Unknown
    // for a parameter that could not be fixed).
    std::vector<const IType*> infer_type_arguments(const std::vector<const ITypeParameter*>& type_parameters, const std::vector<ResolveResult*>& arguments,
                                                   const std::vector<const IType*>& parameter_types, bool* success,
                                                   const std::optional<std::vector<const IType*>>& class_type_arguments = std::nullopt);

    // Infers type arguments for the `type_parameters` occurring in the `target_type` so that the resulting type (after substitution)
    // satisfies the given bounds.
    std::vector<const IType*> infer_type_arguments_from_bounds(const std::vector<const ITypeParameter*>& type_parameters, const IType* target_type,
                                                               const std::vector<const IType*>& lower_bounds, const std::vector<const IType*>& upper_bounds,
                                                               bool* success);

    // The best common type (spec 12.6.3.17) of a set of expressions.
    const IType* get_best_common_type(const std::vector<ResolveResult*>& expressions, bool* success);

    // Finds a type that satisfies the given lower and upper bounds.
    const IType* find_type_in_bounds(const std::vector<const IType*>& lower_bounds, const std::vector<const IType*>& upper_bounds);

private:
    // Types, distinct by their equality, in the order they were added.
    struct TypeSet {
        std::vector<const IType*> items;
        void add(const IType* type);
        std::size_t count() const { return items.size(); }
    };

    // A type parameter being inferred, with its bounds.
    struct TP {
        TypeSet lower_bounds;
        TypeSet upper_bounds;
        const IType* exact_bound = nullptr;
        bool multiple_different_exact_bounds = false;
        const ITypeParameter* type_parameter;
        const IType* fixed_to = nullptr;

        explicit TP(const ITypeParameter* type_parameter_);
        bool is_fixed() const { return fixed_to != nullptr; }
        bool has_bounds() const { return lower_bounds.count() > 0 || upper_bounds.count() > 0 || exact_bound != nullptr; }
        void add_exact_bound(const IType* type);
    };

    class OccursInVisitor;

    TypeInference create_nested_instance() const;
    void reset();

    void phase_one();
    static bool is_valid_type(const IType* type);
    bool phase_two();

    std::vector<const IType*> input_types(ResolveResult* e, const IType* t);
    std::vector<const IType*> output_types(ResolveResult* e, const IType* t);
    static const typesystem::IMethod* get_delegate_or_expression_tree_signature(const IType* t);
    bool input_types_contains_unfixed(ResolveResult* argument, const IType* parameter_type);
    bool output_type_contains_unfixed(ResolveResult* argument, const IType* parameter_type);
    bool any_type_contains_unfixed_parameter(const std::vector<const IType*>& types);

    void calculate_dependency_matrix();
    bool depends_on(const TP& x, const TP& y);

    void make_output_type_inference(ResolveResult* e, const IType* t);
    typesystem::TypeParameterSubstitution get_substitution_for_fixed_tps();
    void make_explicit_parameter_type_inference(LambdaResolveResult* e, const IType* t);
    void make_exact_inference(const IType* u, const IType* v);
    TP* get_tp_for_type(const IType* v);
    void make_lower_bound_inference(const IType* u, const IType* v);
    void make_upper_bound_inference(const IType* u, const IType* v);

    bool fix(TP& tp);

    static const IType* get_first_type_prefer_non_interfaces(const std::vector<const IType*>& result);
    std::vector<const IType*> find_types_in_bounds(const std::vector<const IType*>& lower_bounds, const std::vector<const IType*>& upper_bounds);

    const ICompilation& compilation_;
    CSharpConversions& conversions_;
    TypeInferenceAlgorithm algorithm_ = TypeInferenceAlgorithm::CSharp4;

    // determines the maximum generic nesting level; necessary to avoid infinite recursion in 'Improved' mode.
    static constexpr int max_nesting_level = 5;
    int nesting_level_ = 0;

    std::vector<TP> type_parameters_;
    std::vector<const IType*> parameter_types_;
    std::vector<ResolveResult*> arguments_;
    std::vector<char> dependency_matrix_;   // n * n, row = the dependent parameter
    bool has_dependency_matrix_ = false;
    std::optional<std::vector<const IType*>> class_type_arguments_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver

#include "dotnet/decompiler/csharp/resolver/type_inference.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/overload_resolution.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using semantics::ByReferenceResolveResult;
using typesystem::ArrayType;
using typesystem::ByReferenceType;
using typesystem::FunctionPointerType;
using typesystem::IMethod;
using typesystem::IParameter;
using typesystem::ITypeDefinition;
using typesystem::IntersectionType;
using typesystem::NullabilityAnnotatedTypeParameter;
using typesystem::ParameterizedType;
using typesystem::PointerType;
using typesystem::ReferenceKind;
using typesystem::SymbolKind;
using typesystem::TypeKind;
using typesystem::TypeParameterSubstitution;
using typesystem::TypeSystemOptions;
using typesystem::VarianceModifier;
namespace nullable_type = typesystem::nullable_type;

namespace {

template <class T, class U>
const T* as_type(const U* p) {
    return dynamic_cast<const T*>(p);
}

}  // namespace

// ---- TypeSet and TP -----------------------------------------------------------------------------------------------------------

void TypeInference::TypeSet::add(const IType* type) {
    for (const IType* existing : items) {
        if (typesystem::type_equals(existing, type)) return;
    }
    items.push_back(type);
}

TypeInference::TP::TP(const ITypeParameter* type_parameter_) : type_parameter(type_parameter_) {
    if (type_parameter_ == nullptr) throw std::invalid_argument("type_parameter");
}

void TypeInference::TP::add_exact_bound(const IType* type) {
    // Exact bounds need to stored separately, not just as Lower+Upper bounds,
    // due to TypeInferenceTests.GenericArgumentImplicitlyConvertibleToAndFromAnotherTypeList (see #281)
    if (exact_bound == nullptr)
        exact_bound = type;
    else if (!exact_bound->equals(type))
        multiple_different_exact_bounds = true;
}

// Which of the parameters being inferred occur in the types it visits.
class TypeInference::OccursInVisitor final : public typesystem::TypeVisitor {
public:
    explicit OccursInVisitor(TypeInference& type_inference) : tp_(&type_inference.type_parameters_), occurs(type_inference.type_parameters_.size(), 0) {}

    const IType* visit_type_parameter(const ITypeParameter& type) override {
        const int index = type.index();
        if (index < static_cast<int>(tp_->size()) && (*tp_)[static_cast<std::size_t>(index)].type_parameter == &type) occurs[static_cast<std::size_t>(index)] = 1;
        return TypeVisitor::visit_type_parameter(type);
    }

    std::vector<char> occurs;

private:
    const std::vector<TP>* tp_;
};

// ---- Constructor and properties -----------------------------------------------------------------------------------------------

TypeInference::TypeInference(const ICompilation& compilation) : compilation_(compilation), conversions_(CSharpConversions::get(compilation)) {}

TypeInference::TypeInference(const ICompilation& compilation, CSharpConversions& conversions) : compilation_(compilation), conversions_(conversions) {}

TypeInference TypeInference::create_nested_instance() const {
    TypeInference c(compilation_, conversions_);
    c.algorithm_ = algorithm_;
    c.nesting_level_ = nesting_level_ + 1;
    return c;
}

void TypeInference::reset() {
    // clean up so that memory used by the operation can be freed as soon as possible
    type_parameters_.clear();
    parameter_types_.clear();
    arguments_.clear();
    dependency_matrix_.clear();
    has_dependency_matrix_ = false;
    class_type_arguments_.reset();
}

// ---- InferTypeArguments (main function) ---------------------------------------------------------------------------------------

std::vector<const IType*> TypeInference::infer_type_arguments(const std::vector<const ITypeParameter*>& type_parameters,
                                                              const std::vector<ResolveResult*>& arguments,
                                                              const std::vector<const IType*>& parameter_types, bool* success,
                                                              const std::optional<std::vector<const IType*>>& class_type_arguments) {
    struct ResetOnExit {
        TypeInference& self;
        ~ResetOnExit() { self.reset(); }
    } reset_on_exit{*this};

    type_parameters_.clear();
    for (std::size_t i = 0; i < type_parameters.size(); i++) {
        if (static_cast<int>(i) != type_parameters[i]->index()) throw std::invalid_argument("Type parameter has wrong index");
        if (type_parameters[i]->owner_type() != SymbolKind::Method) throw std::invalid_argument("Type parameter must be owned by a method");
        type_parameters_.emplace_back(type_parameters[i]);
    }
    const std::size_t count = std::min(arguments.size(), parameter_types.size());
    parameter_types_.assign(count, nullptr);
    arguments_.assign(count, nullptr);
    for (std::size_t i = 0; i < count; i++) {
        if (arguments[i] == nullptr || parameter_types[i] == nullptr) throw std::invalid_argument("argument");
        arguments_[i] = arguments[i];
        parameter_types_[i] = parameter_types[i];
    }
    class_type_arguments_ = class_type_arguments;

    phase_one();
    *success = phase_two();

    std::vector<const IType*> result;
    for (const TP& tp : type_parameters_) result.push_back(tp.fixed_to != nullptr ? tp.fixed_to : &typesystem::SpecialType::unknown_type());
    return result;
}

// Infers type arguments for the type parameters occurring in the target type so that the resulting type (after substitution)
// satisfies the given bounds.
std::vector<const IType*> TypeInference::infer_type_arguments_from_bounds(const std::vector<const ITypeParameter*>& type_parameters, const IType* target_type,
                                                                          const std::vector<const IType*>& lower_bounds,
                                                                          const std::vector<const IType*>& upper_bounds, bool* success) {
    if (target_type == nullptr) throw std::invalid_argument("target_type");
    type_parameters_.clear();
    for (std::size_t i = 0; i < type_parameters.size(); i++) {
        if (static_cast<int>(i) != type_parameters[i]->index()) throw std::invalid_argument("Type parameter has wrong index");
        type_parameters_.emplace_back(type_parameters[i]);
    }
    for (const IType* b : lower_bounds) make_lower_bound_inference(b, target_type);
    for (const IType* b : upper_bounds) make_upper_bound_inference(b, target_type);
    std::vector<const IType*> result(type_parameters_.size());
    *success = true;
    for (std::size_t i = 0; i < result.size(); i++) {
        *success &= fix(type_parameters_[i]);
        result[i] = type_parameters_[i].fixed_to != nullptr ? type_parameters_[i].fixed_to : &typesystem::SpecialType::unknown_type();
    }
    reset();
    return result;
}

// ---- Inference Phases ---------------------------------------------------------------------------------------------------------

void TypeInference::phase_one() {
    // C# 4.0 spec: 7.5.2.1 The first phase
    for (std::size_t i = 0; i < arguments_.size(); i++) {
        ResolveResult* ei = arguments_[i];
        const IType* ti = parameter_types_[i];

        auto* lrr = dynamic_cast<LambdaResolveResult*>(ei);
        if (lrr != nullptr) make_explicit_parameter_type_inference(lrr, ti);
        if (lrr != nullptr || dynamic_cast<MethodGroupResolveResult*>(ei) != nullptr) {
            // this is not in the spec???
            if (output_type_contains_unfixed(ei, ti) && !input_types_contains_unfixed(ei, ti)) make_output_type_inference(ei, ti);
        }

        if (is_valid_type(ei->type())) {
            if (as_type<ByReferenceType>(ti) != nullptr) {
                make_exact_inference(ei->type(), ti);
            } else {
                make_lower_bound_inference(ei->type(), ti);
            }
        }
    }
}

bool TypeInference::is_valid_type(const IType* type) {
    return type->kind() != TypeKind::Unknown && type->kind() != TypeKind::Null && type->kind() != TypeKind::None;
}

bool TypeInference::phase_two() {
    // C# spec (draft-v11): 12.6.3.3 The second phase

    // All unfixed type variables Xi which do not depend on any Xj are fixed.
    std::vector<TP*> type_parameters_to_fix;
    for (TP& xi : type_parameters_) {
        if (!xi.is_fixed()) {
            bool depends_on_unfixed = false;
            for (TP& xj : type_parameters_) {
                if (!xj.is_fixed() && depends_on(xi, xj)) {
                    depends_on_unfixed = true;
                    break;
                }
            }
            if (!depends_on_unfixed) type_parameters_to_fix.push_back(&xi);
        }
    }
    // If no such type variables exist, all unfixed type variables Xi are fixed for which all of the following hold:
    if (type_parameters_to_fix.empty()) {
        // Trying to break the cycle by fixing any TPs that have non-empty bounds...
        for (TP& xi : type_parameters_) {
            // Xi has a non-empty set of bounds
            if (!xi.is_fixed() && xi.has_bounds()) {
                // There is at least one type variable Xj that depends on Xi
                bool any_depends = false;
                for (TP& xj : type_parameters_) {
                    if (depends_on(xj, xi)) {
                        any_depends = true;
                        break;
                    }
                }
                if (any_depends) type_parameters_to_fix.push_back(&xi);
            }
        }
    }
    // now fix 'em
    bool error_during_fix = false;
    for (TP* tp : type_parameters_to_fix) {
        if (!fix(*tp)) error_during_fix = true;
    }
    if (error_during_fix) return false;
    bool unfixed_type_variables_exist = false;
    for (const TP& x : type_parameters_) {
        if (!x.is_fixed()) {
            unfixed_type_variables_exist = true;
            break;
        }
    }
    if (type_parameters_to_fix.empty() && unfixed_type_variables_exist) {
        // If no such type variables exist and there are still unfixed type variables, type inference fails.
        return false;
    } else if (!unfixed_type_variables_exist) {
        // Otherwise, if no further unfixed type variables exist, type inference succeeds.
        return true;
    }
    // Otherwise, for all arguments ei with corresponding parameter type Ti
    for (std::size_t i = 0; i < arguments_.size(); i++) {
        ResolveResult* ei = arguments_[i];
        const IType* ti = parameter_types_[i];
        // where the output types (12.6.3.5) contain unfixed type variables Xj
        // but the input types (12.6.3.4) do not
        if (output_type_contains_unfixed(ei, ti) && !input_types_contains_unfixed(ei, ti)) {
            // an output type inference (12.6.3.8) is made for ei with type Ti.
            make_output_type_inference(ei, ti);
        }
    }
    // Then the second phase is repeated.
    return phase_two();
}

// ---- Input Types / Output Types (12.6.3.4 + 12.6.3.5) -------------------------------------------------------------------------

std::vector<const IType*> TypeInference::input_types(ResolveResult* e, const IType* t) {
    // C# spec (draft-v11): 12.6.3.4 Input types
    auto* lrr = dynamic_cast<LambdaResolveResult*>(e);
    if ((lrr != nullptr && lrr->is_implicitly_typed()) || dynamic_cast<MethodGroupResolveResult*>(e) != nullptr) {
        const IMethod* m = get_delegate_or_expression_tree_signature(t);
        if (m != nullptr) {
            std::vector<const IType*> input_types(m->parameters().size());
            for (std::size_t i = 0; i < input_types.size(); i++) input_types[i] = m->parameters()[i]->type();
            return input_types;
        }
    }
    return {};
}

std::vector<const IType*> TypeInference::output_types(ResolveResult* e, const IType* t) {
    // C# spec (draft-v11): 12.6.3.5 Output types
    auto* lrr = dynamic_cast<LambdaResolveResult*>(e);
    if (lrr != nullptr || dynamic_cast<MethodGroupResolveResult*>(e) != nullptr) {
        const IMethod* m = get_delegate_or_expression_tree_signature(t);
        if (m != nullptr) return {m->return_type()};
    }
    return {};
}

const IMethod* TypeInference::get_delegate_or_expression_tree_signature(const IType* t) {
    if (t->type_parameter_count() == 1 && t->name() == "Expression" && t->namespace_() == "System.Linq.Expressions") {
        t = t->type_arguments()[0];
    }
    return typesystem::get_delegate_invoke_method(t);
}

bool TypeInference::input_types_contains_unfixed(ResolveResult* argument, const IType* parameter_type) {
    return any_type_contains_unfixed_parameter(input_types(argument, parameter_type));
}

bool TypeInference::output_type_contains_unfixed(ResolveResult* argument, const IType* parameter_type) {
    return any_type_contains_unfixed_parameter(output_types(argument, parameter_type));
}

bool TypeInference::any_type_contains_unfixed_parameter(const std::vector<const IType*>& types) {
    OccursInVisitor o(*this);
    for (const IType* type : types) type->accept_visitor(o);
    for (std::size_t i = 0; i < type_parameters_.size(); i++) {
        if (!type_parameters_[i].is_fixed() && o.occurs[i]) return true;
    }
    return false;
}

// ---- DependsOn (12.6.3.6) -----------------------------------------------------------------------------------------------------
// C# spec (draft-v11): 12.6.3.6 Dependence

void TypeInference::calculate_dependency_matrix() {
    const std::size_t n = type_parameters_.size();
    dependency_matrix_.assign(n * n, 0);
    for (std::size_t k = 0; k < arguments_.size(); k++) {
        OccursInVisitor input(*this);
        OccursInVisitor output(*this);
        for (const IType* type : input_types(arguments_[k], parameter_types_[k])) type->accept_visitor(input);
        for (const IType* type : output_types(arguments_[k], parameter_types_[k])) type->accept_visitor(output);
        for (std::size_t i = 0; i < n; i++) {
            for (std::size_t j = 0; j < n; j++) {
                if (input.occurs[j] && output.occurs[i]) dependency_matrix_[i * n + j] = 1;
            }
        }
    }
    // calculate transitive closure using Warshall's algorithm:
    for (std::size_t i = 0; i < n; i++) {
        for (std::size_t j = 0; j < n; j++) {
            if (dependency_matrix_[i * n + j]) {
                for (std::size_t k = 0; k < n; k++) {
                    if (dependency_matrix_[j * n + k]) dependency_matrix_[i * n + k] = 1;
                }
            }
        }
    }
    has_dependency_matrix_ = true;
}

bool TypeInference::depends_on(const TP& x, const TP& y) {
    if (!has_dependency_matrix_) calculate_dependency_matrix();
    // x depends on y
    const std::size_t n = type_parameters_.size();
    return dependency_matrix_[static_cast<std::size_t>(x.type_parameter->index()) * n + static_cast<std::size_t>(y.type_parameter->index())] != 0;
}

// ---- MakeOutputTypeInference (7.5.2.6) ----------------------------------------------------------------------------------------

void TypeInference::make_output_type_inference(ResolveResult* e, const IType* t) {
    // If E is an anonymous function with inferred return type  U (12.6.3.14) and T is a delegate type or expression
    // tree type with return type Tb, then a lower-bound inference (12.6.3.11) is made from U to Tb.
    auto* lrr = dynamic_cast<LambdaResolveResult*>(e);
    if (lrr != nullptr) {
        const IMethod* m = get_delegate_or_expression_tree_signature(t);
        if (m != nullptr) {
            const IType* inferred_return_type;
            if (lrr->is_implicitly_typed()) {
                if (m->parameters().size() != lrr->parameters().size()) return;   // cannot infer due to mismatched parameter lists
                TypeParameterSubstitution substitution = get_substitution_for_fixed_tps();
                std::vector<const IType*> inferred_parameter_types(m->parameters().size());
                for (std::size_t i = 0; i < inferred_parameter_types.size(); i++) {
                    const IType* parameter_type = m->parameters()[i]->type();
                    inferred_parameter_types[i] = parameter_type->accept_visitor(substitution);
                }
                inferred_return_type = lrr->get_inferred_return_type(inferred_parameter_types);
            } else {
                inferred_return_type = lrr->get_inferred_return_type({});
            }
            make_lower_bound_inference(inferred_return_type, m->return_type());
            return;
        }
    }
    // Otherwise, if E is a method group and T is a delegate type or expression tree type
    // with parameter types T1...Tk and return type Tb, and overload resolution
    // of E with the types T1...Tk yields a single method with return type U, then a lower-bound
    // inference is made from U to Tb.
    auto* mgrr = dynamic_cast<MethodGroupResolveResult*>(e);
    if (mgrr != nullptr) {
        const IMethod* m = get_delegate_or_expression_tree_signature(t);
        if (m != nullptr) {
            std::vector<ResolveResult*> args(m->parameters().size());
            TypeParameterSubstitution substitution = get_substitution_for_fixed_tps();
            for (std::size_t i = 0; i < args.size(); i++) {
                const IParameter* param = m->parameters()[i];
                const IType* parameter_type = param->type()->accept_visitor(substitution);
                if (param->reference_kind() != ReferenceKind::None && parameter_type->kind() == TypeKind::ByReference) {
                    parameter_type = as_type<ByReferenceType>(parameter_type)->element_type();
                    args[i] = make<ByReferenceResolveResult>(parameter_type, param->reference_kind());
                } else {
                    args[i] = make<ResolveResult>(parameter_type);
                }
            }
            OverloadResolution overload_resolution = perform_overload_resolution(*mgrr, compilation_, std::move(args), {}, true, false, false, false, false, nullptr);
            if (overload_resolution.found_applicable_candidate() && overload_resolution.best_candidate_ambiguous_with() == nullptr) {
                const IType* return_type = overload_resolution.get_best_candidate_with_substituted_type_arguments()->return_type();
                make_lower_bound_inference(return_type, m->return_type());
            }
        }
        return;
    }
    // Otherwise, if E is an expression with type U, then a lower-bound inference is made from U to T.
    if (is_valid_type(e->type())) make_lower_bound_inference(e->type(), t);
}

TypeParameterSubstitution TypeInference::get_substitution_for_fixed_tps() {
    std::vector<const IType*> fixed_types(type_parameters_.size());
    for (std::size_t i = 0; i < fixed_types.size(); i++) {
        fixed_types[i] = type_parameters_[i].fixed_to != nullptr ? type_parameters_[i].fixed_to : &typesystem::SpecialType::unknown_type();
    }
    return TypeParameterSubstitution(class_type_arguments_, std::optional<std::vector<const IType*>>(std::move(fixed_types)));
}

// ---- MakeExplicitParameterTypeInference (12.6.3.9) ----------------------------------------------------------------------------

void TypeInference::make_explicit_parameter_type_inference(LambdaResolveResult* e, const IType* t) {
    // C# spec (draft-v11): 12.6.3.9 Explicit parameter type inferences
    if (e->is_implicitly_typed() || !e->has_parameter_list()) return;
    const IMethod* m = get_delegate_or_expression_tree_signature(t);
    if (m == nullptr) return;
    for (std::size_t i = 0; i < e->parameters().size() && i < m->parameters().size(); i++) {
        make_exact_inference(e->parameters()[i]->type(), m->parameters()[i]->type());
    }
}

// ---- MakeExactInference (7.5.2.8) ---------------------------------------------------------------------------------------------

// Make exact inference from U to V.
// C# 4.0 spec: 7.5.2.8 Exact inferences
void TypeInference::make_exact_inference(const IType* u, const IType* v) {
    if (u->nullability() == v->nullability()) {
        u = typesystem::without_nullability(u);
        v = typesystem::without_nullability(v);
    }

    // If V is one of the unfixed Xi then U is added to the set of bounds for Xi.
    TP* tp = get_tp_for_type(v);
    if (tp != nullptr && !tp->is_fixed()) {
        tp->add_exact_bound(u);
        return;
    }
    // Handle by reference types:
    const auto* br_u = as_type<ByReferenceType>(u);
    const auto* br_v = as_type<ByReferenceType>(v);
    if (br_u != nullptr && br_v != nullptr) {
        make_exact_inference(br_u->element_type(), br_v->element_type());
        return;
    }
    // Handle array types:
    u = typesystem::tuple_underlying_type_or_self(u);
    v = typesystem::tuple_underlying_type_or_self(v);
    {
        const auto* arr_u = as_type<ArrayType>(u);
        const auto* arr_v = as_type<ArrayType>(v);
        const auto* p_u = as_type<ParameterizedType>(u);
        const auto* p_v = as_type<ParameterizedType>(v);
        const bool first_class_span = typesystem::any(compilation_.type_system_options(), TypeSystemOptions::FirstClassSpanTypes);
        if (arr_u != nullptr && arr_v != nullptr && arr_u->dimensions() == arr_v->dimensions()) {
            make_exact_inference(arr_u->element_type(), arr_v->element_type());
            return;
        }
        if (arr_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_v, typesystem::KnownTypeCode::SpanOfT)) {
            make_exact_inference(arr_u->element_type(), p_v->type_arguments()[0]);
            return;
        }
        if (p_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_u, typesystem::KnownTypeCode::SpanOfT) &&
            typesystem::is_known_type(p_v, typesystem::KnownTypeCode::SpanOfT)) {
            make_exact_inference(p_u->type_arguments()[0], p_v->type_arguments()[0]);
            return;
        }
        if (arr_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_v, typesystem::KnownTypeCode::ReadOnlySpanOfT)) {
            make_exact_inference(arr_u->element_type(), p_v->type_arguments()[0]);
            return;
        }
        if (p_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_u, typesystem::KnownTypeCode::SpanOfT) &&
            typesystem::is_known_type(p_v, typesystem::KnownTypeCode::ReadOnlySpanOfT)) {
            make_exact_inference(p_u->type_arguments()[0], p_v->type_arguments()[0]);
            return;
        }
        if (p_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_u, typesystem::KnownTypeCode::ReadOnlySpanOfT) &&
            typesystem::is_known_type(p_v, typesystem::KnownTypeCode::ReadOnlySpanOfT)) {
            make_exact_inference(p_u->type_arguments()[0], p_v->type_arguments()[0]);
            return;
        }
    }
    // Handle parameterized type:
    {
        const auto* p_u = as_type<ParameterizedType>(u);
        const auto* p_v = as_type<ParameterizedType>(v);
        if (p_u != nullptr && p_v != nullptr && typesystem::type_equals(p_u->generic_type(), p_v->generic_type()) &&
            p_u->type_parameter_count() == p_v->type_parameter_count()) {
            for (int i = 0; i < p_u->type_parameter_count(); i++) make_exact_inference(p_u->get_type_argument(i), p_v->get_type_argument(i));
            return;
        }
    }
    // Handle pointer types:
    {
        const auto* ptr_u = as_type<PointerType>(u);
        const auto* ptr_v = as_type<PointerType>(v);
        if (ptr_u != nullptr && ptr_v != nullptr) {
            make_exact_inference(ptr_u->element_type(), ptr_v->element_type());
            return;
        }
    }
    {
        const auto* fn_ptr_u = as_type<FunctionPointerType>(u);
        const auto* fn_ptr_v = as_type<FunctionPointerType>(v);
        if (fn_ptr_u != nullptr && fn_ptr_v != nullptr) {
            make_exact_inference(fn_ptr_u->return_type, fn_ptr_v->return_type);
            const std::size_t count = std::min(fn_ptr_u->parameter_types.size(), fn_ptr_v->parameter_types.size());
            for (std::size_t i = 0; i < count; i++) make_exact_inference(fn_ptr_u->parameter_types[i], fn_ptr_v->parameter_types[i]);
            return;
        }
    }
}

TypeInference::TP* TypeInference::get_tp_for_type(const IType* v) {
    if (const auto* natp = as_type<NullabilityAnnotatedTypeParameter>(v); natp != nullptr) v = natp->original_type_parameter();
    if (const auto* p = as_type<ITypeParameter>(v); p != nullptr) {
        const int index = p->index();
        if (index < static_cast<int>(type_parameters_.size()) && type_parameters_[static_cast<std::size_t>(index)].type_parameter == p)
            return &type_parameters_[static_cast<std::size_t>(index)];
    }
    return nullptr;
}

// ---- MakeLowerBoundInference (12.6.3.11) --------------------------------------------------------------------------------------

// Make lower bound inference from U to V.
// C# spec (draft-v11): 12.6.3.11 Lower-bound inferences
void TypeInference::make_lower_bound_inference(const IType* u, const IType* v) {
    if (u->nullability() == v->nullability()) {
        u = typesystem::without_nullability(u);
        v = typesystem::without_nullability(v);
    }

    // If V is one of the unfixed Xi then U is added to the set of bounds for Xi.
    TP* tp = get_tp_for_type(v);
    if (tp != nullptr && !tp->is_fixed()) {
        tp->lower_bounds.add(u);
        return;
    }
    // Handle nullable covariance:
    if (nullable_type::is_nullable(u) && nullable_type::is_nullable(v)) {
        make_lower_bound_inference(nullable_type::get_underlying_type(u), nullable_type::get_underlying_type(v));
        return;
    }
    // Handle by reference types:
    const auto* br_u = as_type<ByReferenceType>(u);
    const auto* br_v = as_type<ByReferenceType>(v);
    if (br_u != nullptr && br_v != nullptr) {
        make_exact_inference(br_u->element_type(), br_v->element_type());
        return;
    }
    // Handle array types:
    v = typesystem::tuple_underlying_type_or_self(v);
    {
        const auto* arr_u = as_type<ArrayType>(u);
        const auto* arr_v = as_type<ArrayType>(v);
        const auto* p_u = as_type<ParameterizedType>(u);
        const auto* p_v = as_type<ParameterizedType>(v);
        const bool first_class_span = typesystem::any(compilation_.type_system_options(), TypeSystemOptions::FirstClassSpanTypes);
        if (arr_u != nullptr && arr_v != nullptr && arr_u->dimensions() == arr_v->dimensions()) {
            make_lower_bound_inference(arr_u->element_type(), arr_v->element_type());
            return;
        }
        if (arr_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_v, typesystem::KnownTypeCode::SpanOfT)) {
            make_lower_bound_inference(arr_u->element_type(), p_v->type_arguments()[0]);
            return;
        }
        if (p_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_u, typesystem::KnownTypeCode::SpanOfT) &&
            typesystem::is_known_type(p_v, typesystem::KnownTypeCode::SpanOfT)) {
            make_lower_bound_inference(p_u->type_arguments()[0], p_v->type_arguments()[0]);
            return;
        }
        if (arr_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_v, typesystem::KnownTypeCode::ReadOnlySpanOfT)) {
            make_lower_bound_inference(arr_u->element_type(), p_v->type_arguments()[0]);
            return;
        }
        if (p_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_u, typesystem::KnownTypeCode::SpanOfT) &&
            typesystem::is_known_type(p_v, typesystem::KnownTypeCode::ReadOnlySpanOfT)) {
            make_lower_bound_inference(p_u->type_arguments()[0], p_v->type_arguments()[0]);
            return;
        }
        if (p_u != nullptr && p_v != nullptr && first_class_span && typesystem::is_known_type(p_u, typesystem::KnownTypeCode::ReadOnlySpanOfT) &&
            typesystem::is_known_type(p_v, typesystem::KnownTypeCode::ReadOnlySpanOfT)) {
            make_lower_bound_inference(p_u->type_arguments()[0], p_v->type_arguments()[0]);
            return;
        }
        if (arr_u != nullptr && p_v != nullptr && typesystem::is_array_interface_type(p_v) && arr_u->dimensions() == 1) {
            make_lower_bound_inference(arr_u->element_type(), p_v->type_arguments()[0]);
            return;
        }
    }
    // Handle parameterized types:
    if (const auto* p_v = as_type<ParameterizedType>(v); p_v != nullptr) {
        const ParameterizedType* unique_base_type = nullptr;
        for (const IType* base_u : typesystem::get_all_base_types(u)) {
            const auto* p_u = as_type<ParameterizedType>(typesystem::tuple_underlying_type_or_self(base_u));
            if (p_u != nullptr && typesystem::type_equals(p_u->generic_type(), p_v->generic_type()) && p_u->type_parameter_count() == p_v->type_parameter_count()) {
                if (unique_base_type == nullptr)
                    unique_base_type = p_u;
                else
                    return;   // cannot make an inference because it's not unique
            }
        }
        if (unique_base_type != nullptr) {
            for (int i = 0; i < unique_base_type->type_parameter_count(); i++) {
                const IType* ui = unique_base_type->get_type_argument(i);
                const IType* vi = p_v->get_type_argument(i);
                if (ui->is_reference_type() == std::optional<bool>(true)) {
                    // look for variance
                    const ITypeParameter* xi = p_v->type_parameters()[static_cast<std::size_t>(i)];
                    switch (xi->variance()) {
                    case VarianceModifier::Covariant: make_lower_bound_inference(ui, vi); break;
                    case VarianceModifier::Contravariant: make_upper_bound_inference(ui, vi); break;
                    default:   // invariant
                        make_exact_inference(ui, vi);
                        break;
                    }
                } else {
                    // not known to be a reference type
                    make_exact_inference(ui, vi);
                }
            }
        }
        return;
    }
    // Handle pointer types:
    {
        const auto* ptr_u = as_type<PointerType>(u);
        const auto* ptr_v = as_type<PointerType>(v);
        if (ptr_u != nullptr && ptr_v != nullptr) {
            make_exact_inference(ptr_u->element_type(), ptr_v->element_type());
            return;
        }
    }
    {
        const auto* fn_ptr_u = as_type<FunctionPointerType>(u);
        const auto* fn_ptr_v = as_type<FunctionPointerType>(v);
        if (fn_ptr_u != nullptr && fn_ptr_v != nullptr) {
            make_lower_bound_inference(fn_ptr_u->return_type, fn_ptr_v->return_type);
            const std::size_t count = std::min(fn_ptr_u->parameter_types.size(), fn_ptr_v->parameter_types.size());
            for (std::size_t i = 0; i < count; i++) make_upper_bound_inference(fn_ptr_u->parameter_types[i], fn_ptr_v->parameter_types[i]);
            return;
        }
    }
}

// ---- MakeUpperBoundInference (7.5.2.10) ---------------------------------------------------------------------------------------

// Make upper bound inference from U to V.
// C# 4.0 spec: 7.5.2.10 Upper-bound inferences
void TypeInference::make_upper_bound_inference(const IType* u, const IType* v) {
    if (u->nullability() == v->nullability()) {
        u = typesystem::without_nullability(u);
        v = typesystem::without_nullability(v);
    }

    // If V is one of the unfixed Xi then U is added to the set of bounds for Xi.
    TP* tp = get_tp_for_type(v);
    if (tp != nullptr && !tp->is_fixed()) {
        tp->upper_bounds.add(u);
        return;
    }

    // Handle array types:
    const auto* arr_u = as_type<ArrayType>(u);
    const auto* arr_v = as_type<ArrayType>(v);
    const auto* p_u = as_type<ParameterizedType>(typesystem::tuple_underlying_type_or_self(u));
    if (arr_v != nullptr && arr_u != nullptr && arr_u->dimensions() == arr_v->dimensions()) {
        make_upper_bound_inference(arr_u->element_type(), arr_v->element_type());
        return;
    } else if (arr_v != nullptr && typesystem::is_array_interface_type(p_u) && arr_v->dimensions() == 1) {
        make_upper_bound_inference(p_u->get_type_argument(0), arr_v->element_type());
        return;
    }
    // Handle parameterized types:
    if (p_u != nullptr) {
        const ParameterizedType* unique_base_type = nullptr;
        for (const IType* base_v : typesystem::get_all_base_types(v)) {
            const auto* p_v = as_type<ParameterizedType>(typesystem::tuple_underlying_type_or_self(base_v));
            if (p_v != nullptr && typesystem::type_equals(p_u->generic_type(), p_v->generic_type()) && p_u->type_parameter_count() == p_v->type_parameter_count()) {
                if (unique_base_type == nullptr)
                    unique_base_type = p_v;
                else
                    return;   // cannot make an inference because it's not unique
            }
        }
        if (unique_base_type != nullptr) {
            for (int i = 0; i < unique_base_type->type_parameter_count(); i++) {
                const IType* ui = p_u->get_type_argument(i);
                const IType* vi = unique_base_type->get_type_argument(i);
                if (ui->is_reference_type() == std::optional<bool>(true)) {
                    // look for variance
                    const ITypeParameter* xi = p_u->type_parameters()[static_cast<std::size_t>(i)];
                    switch (xi->variance()) {
                    case VarianceModifier::Covariant: make_upper_bound_inference(ui, vi); break;
                    case VarianceModifier::Contravariant: make_lower_bound_inference(ui, vi); break;
                    default:   // invariant
                        make_exact_inference(ui, vi);
                        break;
                    }
                } else {
                    // not known to be a reference type
                    make_exact_inference(ui, vi);
                }
            }
        }
        return;
    }
    // Handle pointer types:
    {
        const auto* ptr_u = as_type<PointerType>(u);
        const auto* ptr_v = as_type<PointerType>(v);
        if (ptr_u != nullptr && ptr_v != nullptr) {
            make_exact_inference(ptr_u->element_type(), ptr_v->element_type());
            return;
        }
    }
    {
        const auto* fn_ptr_u = as_type<FunctionPointerType>(u);
        const auto* fn_ptr_v = as_type<FunctionPointerType>(v);
        if (fn_ptr_u != nullptr && fn_ptr_v != nullptr) {
            make_upper_bound_inference(fn_ptr_u->return_type, fn_ptr_v->return_type);
            const std::size_t count = std::min(fn_ptr_u->parameter_types.size(), fn_ptr_v->parameter_types.size());
            for (std::size_t i = 0; i < count; i++) make_lower_bound_inference(fn_ptr_u->parameter_types[i], fn_ptr_v->parameter_types[i]);
            return;
        }
    }
}

// ---- Fixing (12.6.3.13) -------------------------------------------------------------------------------------------------------

bool TypeInference::fix(TP& tp) {
    if (tp.exact_bound != nullptr) {
        // the exact bound will always be the result
        tp.fixed_to = tp.exact_bound;
        // check validity
        if (tp.multiple_different_exact_bounds) return false;
        for (const IType* b : tp.lower_bounds.items) {
            if (!conversions_.implicit_conversion(b, tp.fixed_to)->is_valid()) return false;
        }
        for (const IType* b : tp.upper_bounds.items) {
            if (!conversions_.implicit_conversion(tp.fixed_to, b)->is_valid()) return false;
        }
        return true;
    }
    TypeInference nested = create_nested_instance();
    const std::vector<const IType*> types = nested.find_types_in_bounds(tp.lower_bounds.items, tp.upper_bounds.items);
    if (algorithm_ == TypeInferenceAlgorithm::ImprovedReturnAllResults) {
        tp.fixed_to = IntersectionType::create(types);
        return types.size() >= 1;
    }
    tp.fixed_to = get_first_type_prefer_non_interfaces(types);
    return types.size() == 1;
}

// ---- Finding the best common type of a set of expressions ---------------------------------------------------------------------

// The best common type (C# spec draft-v11: 12.6.3.17) of a set of expressions.
const IType* TypeInference::get_best_common_type(const std::vector<ResolveResult*>& expressions, bool* success) {
    if (expressions.size() == 1) {
        *success = is_valid_type(expressions[0]->type());
        return expressions[0]->type();
    }
    struct ResetOnExit {
        TypeInference& self;
        ~ResetOnExit() { self.reset(); }
    } reset_on_exit{*this};

    const ITypeParameter* tp = typesystem::DummyTypeParameter::get_method_type_parameter(0);
    type_parameters_.clear();
    type_parameters_.emplace_back(tp);
    for (ResolveResult* r : expressions) make_output_type_inference(r, tp);
    *success = fix(type_parameters_[0]);
    return type_parameters_[0].fixed_to != nullptr ? type_parameters_[0].fixed_to : &typesystem::SpecialType::unknown_type();
}

// ---- FindTypeInBounds ---------------------------------------------------------------------------------------------------------

// Finds a type that satisfies the given lower and upper bounds.
const IType* TypeInference::find_type_in_bounds(const std::vector<const IType*>& lower_bounds, const std::vector<const IType*>& upper_bounds) {
    const std::vector<const IType*> result = find_types_in_bounds(lower_bounds, upper_bounds);

    if (algorithm_ == TypeInferenceAlgorithm::ImprovedReturnAllResults) return IntersectionType::create(result);
    // return any of the candidates (prefer non-interfaces)
    return get_first_type_prefer_non_interfaces(result);
}

const IType* TypeInference::get_first_type_prefer_non_interfaces(const std::vector<const IType*>& result) {
    for (const IType* c : result) {
        if (c->kind() != TypeKind::Interface) return c;
    }
    if (!result.empty()) return result.front();
    return &typesystem::SpecialType::unknown_type();
}

std::vector<const IType*> TypeInference::find_types_in_bounds(const std::vector<const IType*>& lower_bounds, const std::vector<const IType*>& upper_bounds) {
    // If there's only a single type; return that single type.
    // If both inputs are empty, return the empty list.
    if (lower_bounds.empty() && upper_bounds.size() <= 1) return upper_bounds;
    if (upper_bounds.empty() && lower_bounds.size() <= 1) return lower_bounds;
    if (nesting_level_ > max_nesting_level) return {};

    // Finds a type X so that "LB <: X <: UB"

    // First try the Fixing algorithm from the C# spec (12.6.3.13)
    std::vector<const IType*> candidate_types;
    {
        TypeSet union_of_bounds;
        for (const IType* t : lower_bounds) union_of_bounds.add(t);
        for (const IType* t : upper_bounds) union_of_bounds.add(t);
        for (const IType* c : union_of_bounds.items) {
            bool ok = true;
            for (const IType* b : lower_bounds) {
                if (!conversions_.implicit_conversion(b, c)->is_valid()) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            for (const IType* b : upper_bounds) {
                if (!conversions_.implicit_conversion(c, b)->is_valid()) {
                    ok = false;
                    break;
                }
            }
            if (ok) candidate_types.push_back(c);
        }
    }

    // C# spec (draft-v11) 12.6.3.13: the result is the unique candidate type
    // to which there is an implicit conversion from all the other candidate types.
    {
        std::vector<const IType*> narrowed;
        for (const IType* c : candidate_types) {
            bool all = true;
            for (const IType* o : candidate_types) {
                if (!conversions_.implicit_conversion(o, c)->is_valid()) {
                    all = false;
                    break;
                }
            }
            if (all) narrowed.push_back(c);
        }
        candidate_types = std::move(narrowed);
    }

    // If the specified algorithm produces a single candidate, we return
    // that candidate.
    // We also return the whole candidate list if we're not using the improved
    // algorithm.
    if (candidate_types.size() == 1 || !(algorithm_ == TypeInferenceAlgorithm::Improved || algorithm_ == TypeInferenceAlgorithm::ImprovedReturnAllResults)) {
        return candidate_types;
    }
    candidate_types.clear();

    // Now try the improved algorithm
    std::vector<const ITypeDefinition*> candidate_type_definitions;
    if (!lower_bounds.empty()) {
        // Find candidates by using the lower bounds:
        for (const ITypeDefinition* d : typesystem::get_all_base_type_definitions(lower_bounds[0])) {
            bool seen = false;
            for (const ITypeDefinition* e : candidate_type_definitions) {
                if (typesystem::type_equals(e, d)) {
                    seen = true;
                    break;
                }
            }
            if (!seen) candidate_type_definitions.push_back(d);
        }
        for (std::size_t i = 1; i < lower_bounds.size(); i++) {
            const std::vector<const ITypeDefinition*> others = typesystem::get_all_base_type_definitions(lower_bounds[i]);
            std::vector<const ITypeDefinition*> kept;
            for (const ITypeDefinition* d : candidate_type_definitions) {
                for (const ITypeDefinition* o : others) {
                    if (typesystem::type_equals(d, o)) {
                        kept.push_back(d);
                        break;
                    }
                }
            }
            candidate_type_definitions = std::move(kept);
        }
    } else {
        // Find candidates by looking at all classes in the project:
        candidate_type_definitions = typesystem::get_all_type_definitions(compilation_);
    }

    // Now filter out candidates that violate the upper bounds:
    for (const IType* ub : upper_bounds) {
        const ITypeDefinition* ub_def = ub->get_definition();
        if (ub_def != nullptr) {
            std::erase_if(candidate_type_definitions, [&](const ITypeDefinition* c) { return !typesystem::is_derived_from(c, ub_def); });
        }
    }

    for (const ITypeDefinition* candidate_def : candidate_type_definitions) {
        // determine the type parameters for the candidate:
        const IType* candidate;
        if (candidate_def->type_parameter_count() == 0) {
            candidate = candidate_def;
        } else {
            bool success;
            const std::vector<const ITypeParameter*>& def_type_parameters = candidate_def->type_parameters();
            std::vector<const IType*> own_type_arguments(def_type_parameters.begin(), def_type_parameters.end());
            const std::vector<const IType*> result = infer_type_arguments_from_bounds(
                def_type_parameters, typesystem::make_parameterized_type(candidate_def, own_type_arguments), lower_bounds, upper_bounds, &success);
            if (success) {
                candidate = typesystem::make_parameterized_type(candidate_def, result);
            } else {
                continue;
            }
        }

        if (upper_bounds.empty()) {
            // if there were only lower bounds, we aim for the most specific candidate:

            // if this candidate isn't made redundant by an existing, more specific candidate:
            bool redundant = false;
            for (const IType* c : candidate_types) {
                if (typesystem::is_derived_from(c->get_definition(), candidate_def)) {
                    redundant = true;
                    break;
                }
            }
            if (!redundant) {
                // remove all existing candidates made redundant by this candidate:
                std::erase_if(candidate_types, [&](const IType* c) { return typesystem::is_derived_from(candidate_def, c->get_definition()); });
                // add new candidate
                candidate_types.push_back(candidate);
            }
        } else {
            // if there were upper bounds, we aim for the least specific candidate:

            // if this candidate isn't made redundant by an existing, less specific candidate:
            bool redundant = false;
            for (const IType* c : candidate_types) {
                if (typesystem::is_derived_from(candidate_def, c->get_definition())) {
                    redundant = true;
                    break;
                }
            }
            if (!redundant) {
                // remove all existing candidates made redundant by this candidate:
                std::erase_if(candidate_types, [&](const IType* c) { return typesystem::is_derived_from(c->get_definition(), candidate_def); });
                // add new candidate
                candidate_types.push_back(candidate);
            }
        }
    }
    return candidate_types;
}

}  // namespace cb::dotnet::decompiler::csharp::resolver

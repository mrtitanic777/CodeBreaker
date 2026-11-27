// dotnet/decompiler/csharp/resolver/csharp_conversions.hpp -- which conversions C# allows between two types, or from an expression to a
// type: identity, numeric, enumeration, nullable, reference, boxing, user-defined, tuple, method group and lambda conversions; whether a
// type satisfies a generic constraint; and which of two conversion targets is the better one (overload resolution's tie-breaker).
//
//     CSharpConversions& conversions = CSharpConversions::get(compilation);
//     const Conversion* c = conversions.implicit_conversion(int_type, long_type);    // Conversion::implicit_numeric_conversion()
//     c->is_valid();                                                                   // true
//
// Ported from ILSpy (ICSharpCode.Decompiler.CSharp.Resolver: CSharpConversions, from CSharpConversions.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// One instance serves a compilation (upstream keeps it in the compilation's cache manager, which is not ported; here the compilation's
// arena keeps it) and is thread-safe: the only state is the cache of implicit conversions between two types, behind a mutex. The cache
// keeps only conversions that are shared singletons: a user-defined, method group or tuple conversion is an object of the thread's
// current ILArena (semantics/conversion.hpp), which can be gone before the compilation is, so those are worked out again on each ask.
// ReflectionHelper.GetTypeCode is get_type_code below (the TypeCode of System, which KnownTypeCode numbers the same).
#pragma once

#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "dotnet/decompiler/semantics/conversion.hpp"
#include "dotnet/decompiler/semantics/resolve_result.hpp"
#include "dotnet/decompiler/semantics/tuple_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using semantics::Conversion;
using semantics::ResolveResult;
using typesystem::ICompilation;
using typesystem::IMethod;
using typesystem::IType;
using typesystem::KnownTypeCode;

// The System.TypeCode of a type: its known type code when that is one of TypeCode's (Object ... String, not Void), else None (TypeCode.Empty).
KnownTypeCode get_type_code(const IType* type);

class CSharpConversions {
public:
    explicit CSharpConversions(const ICompilation& compilation);

    // The instance of a compilation (made on the first ask).
    static CSharpConversions& get(const ICompilation& compilation);

    // ---- implicit conversions ---------------------------------------------------------------------------------------------

    // The implicit conversion from an expression to a type (Conversion::none() when there is none).
    const Conversion* implicit_conversion(ResolveResult* resolve_result, const IType* to_type);
    // The implicit conversion from one type to another.
    const Conversion* implicit_conversion(const IType* from_type, const IType* to_type);
    // The same without user-defined conversions (C# spec: standard implicit conversions).
    const Conversion* standard_implicit_conversion(const IType* from_type, const IType* to_type);
    // Whether `from_type` is convertible to `to_type` by one of the conversions allowed when satisfying constraints (spec 8.4.5).
    bool is_constraint_convertible(const IType* from_type, const IType* to_type);

    // ---- explicit conversions ---------------------------------------------------------------------------------------------

    const Conversion* explicit_conversion(ResolveResult* resolve_result, const IType* to_type);
    const Conversion* explicit_conversion(const IType* from_type, const IType* to_type);

    // ---- single kinds -----------------------------------------------------------------------------------------------------

    // Whether the types are the same after erasing what the CLR does not tell apart (dynamic and object, nint and IntPtr, tuple names ...).
    bool identity_conversion(const IType* from_type, const IType* to_type);
    bool is_implicit_reference_conversion(const IType* from_type, const IType* to_type);
    // Whether the conversion is a boxing conversion, or an implicit one involving a type parameter that boxes when it is a value type.
    bool is_boxing_conversion_or_involving_type_parameter(const IType* from_type, const IType* to_type);
    // Whether a method is compatible with a delegate type (spec 21.4).
    bool is_delegate_compatible(const IMethod* method, const IType* delegate_type);

    // ---- better conversion ------------------------------------------------------------------------------------------------

    // The better conversion from an expression (spec 12.6.4.5): 0 = neither is better; 1 = t1 is better; 2 = t2 is better.
    int better_conversion(ResolveResult* resolve_result, const IType* t1, const IType* t2);
    // The better conversion from a type: 0 = neither is better; 1 = t1 is better; 2 = t2 is better.
    int better_conversion(const IType* s, const IType* t1, const IType* t2);

private:
    struct OperatorInfo {
        const IMethod* method;
        const IType* source_type;
        const IType* target_type;
        bool is_lifted;
    };

    struct TypePair {
        const IType* from_type;
        const IType* to_type;
    };
    struct TypePairHash {
        std::size_t operator()(const TypePair& pair) const;
    };
    struct TypePairEqual {
        bool operator()(const TypePair& a, const TypePair& b) const;
    };

    const Conversion* implicit_conversion(ResolveResult* resolve_result, const IType* to_type, bool allow_user_defined, bool allow_tuple);
    const Conversion* implicit_conversion(const IType* from_type, const IType* to_type, bool allow_user_defined, bool allow_tuple);
    const Conversion* standard_implicit_conversion(const IType* from_type, const IType* to_type, bool allow_tuple_conversion);

    const Conversion* explicit_conversion_not_user_defined(const IType* from_type, const IType* to_type);
    const Conversion* explicit_conversion_impl(const IType* from_type, const IType* to_type);

    bool implicit_numeric_conversion(const IType* from_type, const IType* to_type);
    bool is_numeric_type(const IType* type);
    bool any_numeric_conversion(const IType* from_type, const IType* to_type);

    const Conversion* implicit_enumeration_conversion(ResolveResult* rr, const IType* to_type);
    bool explicit_enumeration_conversion(const IType* from_type, const IType* to_type);

    const Conversion* implicit_nullable_conversion(const IType* from_type, const IType* to_type);
    const Conversion* explicit_nullable_conversion(const IType* from_type, const IType* to_type);

    bool null_literal_conversion(const IType* from_type, const IType* to_type);

    bool implicit_reference_conversion(const IType* from_type, const IType* to_type, int subtype_check_nesting_depth);
    const IType* unpack_generic_array_interface(const IType* interface_type);
    bool is_subtype_of(const IType* s, const IType* t, int subtype_check_nesting_depth);
    bool identity_or_variance_conversion(const IType* s, const IType* t, int subtype_check_nesting_depth);
    bool explicit_reference_conversion(const IType* from_type, const IType* to_type);
    bool is_sealed_reference_type(const IType* type);

    bool is_boxing_conversion(const IType* from_type, const IType* to_type);
    bool unboxing_conversion(const IType* from_type, const IType* to_type);

    bool implicit_constant_expression_conversion(ResolveResult* rr, const IType* to_type);

    bool implicit_type_parameter_conversion(const IType* from_type, const IType* to_type);
    const Conversion* explicit_type_parameter_conversion(const IType* from_type, const IType* to_type);

    bool implicit_pointer_conversion(const IType* from_type, const IType* to_type);
    bool explicit_pointer_conversion(const IType* from_type, const IType* to_type);
    bool is_integer_type(const IType* type);

    bool is_encompassed_by(const IType* a, const IType* b);
    bool is_encompassing_or_encompassed_by(const IType* a, const IType* b);
    const IType* find_most_encompassed_type(const std::vector<const IType*>& candidates);
    const IType* find_most_encompassing_type(const std::vector<const IType*>& candidates);
    const Conversion* select_operator(const IType* most_specific_source, const IType* most_specific_target, const std::vector<OperatorInfo>& operators,
                                      bool is_implicit, const IType* source, const IType* target);
    const Conversion* user_defined_implicit_conversion(ResolveResult* from_result, const IType* from_type, const IType* to_type);
    const Conversion* user_defined_explicit_conversion(ResolveResult* from_result, const IType* from_type, const IType* to_type);
    std::vector<OperatorInfo> get_applicable_conversion_operators(ResolveResult* from_result, const IType* from_type, const IType* to_type, bool is_explicit);

    bool is_implicit_span_conversion(const IType* from_type, const IType* to_type);

    const Conversion* anonymous_function_conversion(ResolveResult* resolve_result, const IType* to_type);
    const Conversion* method_group_conversion(ResolveResult* resolve_result, const IType* to_type);
    bool is_delegate_compatible(const IMethod* m, const IMethod* d, bool is_extension_method_invocation);

    const Conversion* tuple_conversion(semantics::TupleResolveResult* from_rr, const IType* to_type, bool is_explicit);
    const Conversion* tuple_conversion(const IType* from_type, const IType* to_type, bool is_explicit);

    bool is_exactly_matching(ResolveResult* e, const IType* t);
    int better_conversion_target(const IType* t1, const IType* t2);
    bool is_better_integral_type(KnownTypeCode t1, KnownTypeCode t2);

    const ICompilation& compilation_;
    std::mutex cache_mutex_;
    std::unordered_map<TypePair, const Conversion*, TypePairHash, TypePairEqual> implicit_conversion_cache_;
};

}  // namespace cb::dotnet::decompiler::csharp::resolver

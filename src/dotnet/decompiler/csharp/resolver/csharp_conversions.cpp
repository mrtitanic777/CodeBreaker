#include "dotnet/decompiler/csharp/resolver/csharp_conversions.hpp"

#include <cmath>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <variant>

#include "dotnet/decompiler/csharp/resolver/lambda_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/method_group_resolve_result.hpp"
#include "dotnet/decompiler/csharp/resolver/overload_resolution.hpp"
#include "dotnet/decompiler/semantics/by_reference_resolve_result.hpp"
#include "dotnet/decompiler/semantics/interpolated_string_resolve_result.hpp"
#include "dotnet/decompiler/semantics/this_resolve_result.hpp"
#include "dotnet/decompiler/semantics/throw_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/task_type.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::csharp::resolver {

using semantics::ByReferenceResolveResult;
using semantics::InterpolatedStringResolveResult;
using semantics::ThisResolveResult;
using semantics::ThrowResolveResult;
using semantics::TupleResolveResult;
using typesystem::ArrayType;
using typesystem::ByReferenceType;
using typesystem::ConstantValue;
using typesystem::FunctionPointerType;
using typesystem::IParameter;
using typesystem::ITypeDefinition;
using typesystem::ITypeParameter;
using typesystem::NormalizeTypeVisitor;
using typesystem::ParameterizedType;
using typesystem::PointerType;
using typesystem::ReferenceKind;
using typesystem::TupleType;
using typesystem::TypeKind;
using typesystem::TypeSystemOptions;
using typesystem::VarianceModifier;
namespace nullable_type = typesystem::nullable_type;

namespace {

// A System.TypeCode as a number, for the range tests upstream does with the enum's order.
int code(KnownTypeCode c) { return static_cast<int>(c); }

template <class T, class U>
const T* as_type(const U* p) {
    return dynamic_cast<const T*>(p);
}

// The type argument of an Expression<TDelegate>; the type itself for anything else.
const IType* unpack_expression_tree_type(const IType* type) {
    const auto* pt = as_type<ParameterizedType>(type);
    if (pt != nullptr && pt->type_parameter_count() == 1 && pt->name() == "Expression" && pt->namespace_() == "System.Linq.Expressions") {
        return pt->get_type_argument(0);
    }
    return type;
}

// Unpacks the generic Task<T> (or a custom task type): T. nullptr if the input is not one.
const IType* unpack_task(const IType* type) {
    const IType* builder = nullptr;
    return (typesystem::task_type::is_task(type) || typesystem::task_type::is_custom_task(type, &builder)) && type->type_parameter_count() == 1
               ? type->type_arguments()[0]
               : nullptr;
}

// Convert.ToDouble(value) == 0 for the numeric constants (a null constant converts to 0).
bool constant_is_zero(const ConstantValue& value) {
    return std::visit(
        [](const auto& v) -> bool {
            using V = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<V, std::monostate>) {
                return true;
            } else if constexpr (std::is_same_v<V, util::Decimal>) {
                return v.is_zero();
            } else if constexpr (std::is_same_v<V, std::u16string>) {
                return false;   // (Convert.ToDouble throws for strings; no string has an integral type code)
            } else {
                return static_cast<double>(v) == 0.0;
            }
        },
        value);
}

}  // namespace

KnownTypeCode get_type_code(const IType* type) {
    const auto* def = as_type<ITypeDefinition>(type);
    if (def != nullptr) {
        const KnownTypeCode type_code = def->known_type_code();
        if (code(type_code) <= code(KnownTypeCode::String) && type_code != KnownTypeCode::Void) return type_code;
        return KnownTypeCode::None;
    }
    return KnownTypeCode::None;
}

// ---- construction -------------------------------------------------------------------------------------------------------------

std::size_t CSharpConversions::TypePairHash::operator()(const TypePair& pair) const {
    const std::uint32_t h = 1000000007u * static_cast<std::uint32_t>(pair.from_type->hash_code()) +
                            1000000009u * static_cast<std::uint32_t>(pair.to_type->hash_code());
    return h;
}

bool CSharpConversions::TypePairEqual::operator()(const TypePair& a, const TypePair& b) const {
    return typesystem::type_equals(a.from_type, b.from_type) && typesystem::type_equals(a.to_type, b.to_type);
}

CSharpConversions::CSharpConversions(const ICompilation& compilation) : compilation_(compilation) {}

CSharpConversions& CSharpConversions::get(const ICompilation& compilation) {
    // 200 is this class's key among the objects a compilation's arena keeps (types.cpp and its users use the lower numbers).
    typesystem::TypeKey key{200, {&compilation}, {}, {}};
    const CSharpConversions* conversions =
        compilation.arena().intern<CSharpConversions>(std::move(key), [&] { return std::make_unique<CSharpConversions>(compilation); });
    return const_cast<CSharpConversions&>(*conversions);
}

// ---- ImplicitConversion -------------------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::implicit_conversion(ResolveResult* resolve_result, const IType* to_type, bool allow_user_defined, bool allow_tuple) {
    const Conversion* c;
    if (resolve_result->is_compile_time_constant()) {
        c = implicit_enumeration_conversion(resolve_result, to_type);
        if (c->is_valid()) return c;
        if (implicit_constant_expression_conversion(resolve_result, to_type)) return Conversion::implicit_constant_expression_conversion();
    }
    // C# 9.0 spec: 10.2.5
    if (dynamic_cast<InterpolatedStringResolveResult*>(resolve_result) != nullptr) {
        if (typesystem::is_known_type(to_type, KnownTypeCode::IFormattable) || typesystem::is_known_type(to_type, KnownTypeCode::FormattableString))
            return Conversion::implicit_interpolated_string_conversion();
    }
    if (resolve_result->type()->kind() == TypeKind::Dynamic) return Conversion::implicit_dynamic_conversion();
    c = anonymous_function_conversion(resolve_result, to_type);
    if (c != Conversion::none()) return c;
    c = method_group_conversion(resolve_result, to_type);
    if (c != Conversion::none()) return c;
    // C# 9.0 spec: 10.2.16 default literal conversions
    // TODO
    if (resolve_result->is_compile_time_constant()) {
        c = standard_implicit_conversion(resolve_result->type(), to_type, allow_tuple);
        if (c != Conversion::none()) return c;
        if (allow_user_defined) {
            c = user_defined_implicit_conversion(resolve_result, resolve_result->type(), to_type);
            if (c != Conversion::none()) return c;
        }
    } else {
        if (auto* tuple_rr = dynamic_cast<TupleResolveResult*>(resolve_result); allow_tuple && tuple_rr != nullptr) {
            c = tuple_conversion(tuple_rr, to_type, false);
            if (c != Conversion::none()) return c;
        }
        // C# 9.0 spec: 10.2.17
        if (dynamic_cast<ThrowResolveResult*>(resolve_result) != nullptr) return Conversion::throw_expression_conversion();
        if (allow_user_defined && allow_tuple) {
            // if allow_user_defined and allow_tuple are true, we might as well use the cache
            c = implicit_conversion(resolve_result->type(), to_type);
        } else {
            c = implicit_conversion(resolve_result->type(), to_type, allow_user_defined, allow_tuple);
        }
    }
    return c;
}

const Conversion* CSharpConversions::implicit_conversion(const IType* from_type, const IType* to_type, bool allow_user_defined, bool allow_tuple) {
    // C# spec (draft-v11): 10.2 Implicit conversions
    const Conversion* c = standard_implicit_conversion(from_type, to_type, allow_tuple);
    if (c == Conversion::none() && allow_user_defined) c = user_defined_implicit_conversion(nullptr, from_type, to_type);
    return c;
}

const Conversion* CSharpConversions::implicit_conversion(ResolveResult* resolve_result, const IType* to_type) {
    if (resolve_result == nullptr) throw std::invalid_argument("resolve_result");
    return implicit_conversion(resolve_result, to_type, true, true);
}

const Conversion* CSharpConversions::implicit_conversion(const IType* from_type, const IType* to_type) {
    if (from_type == nullptr) throw std::invalid_argument("from_type");
    if (to_type == nullptr) throw std::invalid_argument("to_type");

    const TypePair pair{from_type, to_type};
    {
        const std::lock_guard lock(cache_mutex_);
        if (const auto it = implicit_conversion_cache_.find(pair); it != implicit_conversion_cache_.end()) return it->second;
    }

    const Conversion* c = implicit_conversion(from_type, to_type, true, true);

    // Only the shared singletons are kept (see the header: the others live in an arena that may not outlive the compilation).
    if (!c->is_user_defined() && !c->is_method_group_conversion() && !c->is_tuple_conversion()) {
        const std::lock_guard lock(cache_mutex_);
        implicit_conversion_cache_[pair] = c;
    }
    return c;
}

const Conversion* CSharpConversions::standard_implicit_conversion(const IType* from_type, const IType* to_type) {
    if (from_type == nullptr) throw std::invalid_argument("from_type");
    if (to_type == nullptr) throw std::invalid_argument("to_type");
    return standard_implicit_conversion(from_type, to_type, true);
}

const Conversion* CSharpConversions::standard_implicit_conversion(const IType* from_type, const IType* to_type, bool allow_tuple_conversion) {
    // C# 9.0 spec: 10.4.2
    if (identity_conversion(from_type, to_type)) return Conversion::identity_conversion();
    if (implicit_numeric_conversion(from_type, to_type)) return Conversion::implicit_numeric_conversion();
    const Conversion* c = implicit_nullable_conversion(from_type, to_type);
    if (c != Conversion::none()) return c;
    if (null_literal_conversion(from_type, to_type)) return Conversion::null_literal_conversion();
    if (implicit_reference_conversion(from_type, to_type, 0)) return Conversion::implicit_reference_conversion();
    if (is_boxing_conversion(from_type, to_type)) return Conversion::boxing_conversion();
    if (implicit_type_parameter_conversion(from_type, to_type)) {
        // Implicit type parameter conversions that aren't also
        // reference conversions are considered to be boxing conversions
        return Conversion::boxing_conversion();
    }
    if (implicit_pointer_conversion(from_type, to_type)) return Conversion::implicit_pointer_conversion();
    if (allow_tuple_conversion) {
        // TODO are tuple conversions really standard implicit conversions?
        // The C# spec (draft-v11, 10.4.2) doesn't list them as standard implicit conversions.
        c = tuple_conversion(from_type, to_type, false);
        if (c != Conversion::none()) return c;
    }
    if ((typesystem::is_known_type(to_type, KnownTypeCode::SpanOfT) || typesystem::is_known_type(to_type, KnownTypeCode::ReadOnlySpanOfT)) &&
        is_inline_array_type(from_type)) {
        const IType* element_type = get_inline_array_element_type(from_type);
        const IType* span_element_type = to_type->type_arguments()[0];
        if (identity_conversion(element_type, span_element_type)) return Conversion::inline_array_conversion();
    }
    if (is_implicit_span_conversion(from_type, to_type)) return Conversion::implicit_span_conversion();
    return Conversion::none();
}

bool CSharpConversions::is_constraint_convertible(const IType* from_type, const IType* to_type) {
    if (from_type == nullptr) throw std::invalid_argument("from_type");
    if (to_type == nullptr) throw std::invalid_argument("to_type");

    if (identity_conversion(from_type, to_type)) return true;
    if (implicit_reference_conversion(from_type, to_type, 0)) return true;
    if (nullable_type::is_nullable(from_type)) {
        // An 'object' constraint still allows nullable value types
        // (object constraints don't exist in C#, but are inserted by DefaultResolvedTypeParameter.DirectBaseTypes)
        if (typesystem::is_known_type(to_type, KnownTypeCode::Object)) return true;
    } else {
        if (is_boxing_conversion(from_type, to_type)) return true;
    }
    if (implicit_type_parameter_conversion(from_type, to_type)) return true;
    return false;
}

// ---- ExplicitConversion -------------------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::explicit_conversion(ResolveResult* resolve_result, const IType* to_type) {
    if (resolve_result == nullptr) throw std::invalid_argument("resolve_result");
    if (to_type == nullptr) throw std::invalid_argument("to_type");

    if (resolve_result->type()->kind() == TypeKind::Dynamic) return Conversion::explicit_dynamic_conversion();
    const Conversion* c = implicit_conversion(resolve_result, to_type, false, false);
    if (c != Conversion::none()) return c;
    if (auto* tuple_rr = dynamic_cast<TupleResolveResult*>(resolve_result); tuple_rr != nullptr) {
        c = tuple_conversion(tuple_rr, to_type, true);
        if (c != Conversion::none()) return c;
    }
    c = explicit_conversion_impl(resolve_result->type(), to_type);
    if (c != Conversion::none()) return c;
    return user_defined_explicit_conversion(resolve_result, resolve_result->type(), to_type);
}

const Conversion* CSharpConversions::explicit_conversion(const IType* from_type, const IType* to_type) {
    if (from_type == nullptr) throw std::invalid_argument("from_type");
    if (to_type == nullptr) throw std::invalid_argument("to_type");

    const Conversion* c = implicit_conversion(from_type, to_type, false, false);
    if (c != Conversion::none()) return c;
    c = explicit_conversion_impl(from_type, to_type);
    if (c != Conversion::none()) return c;
    return user_defined_explicit_conversion(nullptr, from_type, to_type);
}

const Conversion* CSharpConversions::explicit_conversion_not_user_defined(const IType* from_type, const IType* to_type) {
    const Conversion* c = implicit_conversion(from_type, to_type, false, false);
    if (c != Conversion::none()) return c;
    return explicit_conversion_impl(from_type, to_type);
}

const Conversion* CSharpConversions::explicit_conversion_impl(const IType* from_type, const IType* to_type) {
    // This method is called after we already checked for implicit conversions,
    // so any remaining conversions must be explicit.
    if (any_numeric_conversion(from_type, to_type)) return Conversion::explicit_numeric_conversion();
    if (explicit_enumeration_conversion(from_type, to_type)) return Conversion::enumeration_conversion(false, false);
    const Conversion* c = explicit_nullable_conversion(from_type, to_type);
    if (c != Conversion::none()) return c;
    if (explicit_reference_conversion(from_type, to_type)) return Conversion::explicit_reference_conversion();
    if (unboxing_conversion(from_type, to_type)) return Conversion::unboxing_conversion();
    c = explicit_type_parameter_conversion(from_type, to_type);
    if (c != Conversion::none()) return c;
    if (explicit_pointer_conversion(from_type, to_type)) return Conversion::explicit_pointer_conversion();
    return tuple_conversion(from_type, to_type, true);
}

// ---- Identity Conversion ------------------------------------------------------------------------------------------------------

bool CSharpConversions::identity_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.2.2 Identity conversion
    from_type = from_type->accept_visitor(NormalizeTypeVisitor::type_erasure());
    to_type = to_type->accept_visitor(NormalizeTypeVisitor::type_erasure());
    return from_type->equals(to_type);
}

// ---- Numeric Conversions ------------------------------------------------------------------------------------------------------

namespace {

// to:   short  ushort  int   uint   long   ulong
// from:
constexpr bool implicit_numeric_conversion_lookup[9][6] = {
    /* char   */ {false, true, true, true, true, true},
    /* sbyte  */ {true, false, true, false, true, false},
    /* byte   */ {true, true, true, true, true, true},
    /* short  */ {true, false, true, false, true, false},
    /* ushort */ {false, true, true, true, true, true},
    /* int    */ {false, false, true, false, true, false},
    /* uint   */ {false, false, false, true, true, true},
    /* long   */ {false, false, false, false, true, false},
    /* ulong  */ {false, false, false, false, false, true},
};

}  // namespace

bool CSharpConversions::implicit_numeric_conversion(const IType* from_type, const IType* to_type) {
    // C# 9.0 spec: 10.2.3

    KnownTypeCode from = get_type_code(from_type);
    if (from == KnownTypeCode::None) {
        // When converting from a native-sized integer, treat it as 64-bits
        switch (from_type->kind()) {
        case TypeKind::NInt: from = KnownTypeCode::Int64; break;
        case TypeKind::NUInt: from = KnownTypeCode::UInt64; break;
        default: break;
        }
    }
    KnownTypeCode to = get_type_code(to_type);
    if (to == KnownTypeCode::None) {
        // When converting to a native-sized integer, only 32-bits can be stored safely
        switch (to_type->kind()) {
        case TypeKind::NInt: to = KnownTypeCode::Int32; break;
        case TypeKind::NUInt: to = KnownTypeCode::UInt32; break;
        default: break;
        }
    }
    if (code(to) >= code(KnownTypeCode::Single) && code(to) <= code(KnownTypeCode::Decimal)) {
        // Conversions to float/double/decimal exist from all integral types,
        // and there's a conversion from float to double.
        return (code(from) >= code(KnownTypeCode::Char) && code(from) <= code(KnownTypeCode::UInt64)) ||
               (from == KnownTypeCode::Single && to == KnownTypeCode::Double);
    }
    // Conversions to integral types: look at the table
    return code(from) >= code(KnownTypeCode::Char) && code(from) <= code(KnownTypeCode::UInt64) && code(to) >= code(KnownTypeCode::Int16) &&
           code(to) <= code(KnownTypeCode::UInt64) &&
           implicit_numeric_conversion_lookup[code(from) - code(KnownTypeCode::Char)][code(to) - code(KnownTypeCode::Int16)];
}

bool CSharpConversions::is_numeric_type(const IType* type) {
    switch (type->kind()) {
    case TypeKind::NInt:
    case TypeKind::NUInt: return true;
    default: break;
    }
    const KnownTypeCode c = get_type_code(type);
    return code(c) >= code(KnownTypeCode::Char) && code(c) <= code(KnownTypeCode::Decimal);
}

bool CSharpConversions::any_numeric_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.2.3 + 10.3.2 (numeric conversions)
    return is_numeric_type(from_type) && is_numeric_type(to_type);
}

// ---- Enumeration Conversions --------------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::implicit_enumeration_conversion(ResolveResult* rr, const IType* to_type) {
    // C# 9.0 spec: 10.2.4 + enum part of 10.2.6 (Nullable conversions)
    const KnownTypeCode constant_type = get_type_code(rr->type());
    if (code(constant_type) >= code(KnownTypeCode::SByte) && code(constant_type) <= code(KnownTypeCode::Decimal) && constant_is_zero(rr->constant_value())) {
        if (nullable_type::get_underlying_type(to_type)->kind() == TypeKind::Enum) {
            return Conversion::enumeration_conversion(true, nullable_type::is_nullable(to_type));
        }
    }
    return Conversion::none();
}

bool CSharpConversions::explicit_enumeration_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.3.3 Explicit enumeration conversions
    if (from_type->kind() == TypeKind::Enum) {
        return to_type->kind() == TypeKind::Enum || is_numeric_type(to_type);
    } else if (is_numeric_type(from_type)) {
        return to_type->kind() == TypeKind::Enum;
    }
    return false;
}

// ---- Nullable Conversions -----------------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::implicit_nullable_conversion(const IType* from_type, const IType* to_type) {
    // C# 9.0 spec: 10.2.6
    if (nullable_type::is_nullable(to_type)) {
        const IType* t = nullable_type::get_underlying_type(to_type);
        const IType* s = nullable_type::get_underlying_type(from_type);   // might or might not be nullable
        if (identity_conversion(s, t)) return Conversion::implicit_nullable_conversion();
        if (implicit_numeric_conversion(s, t)) return Conversion::implicit_lifted_numeric_conversion();
    }
    return Conversion::none();
}

const Conversion* CSharpConversions::explicit_nullable_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.3.4 Explicit nullable conversions
    if (nullable_type::is_nullable(to_type) || nullable_type::is_nullable(from_type)) {
        const IType* t = nullable_type::get_underlying_type(to_type);
        const IType* s = nullable_type::get_underlying_type(from_type);
        if (identity_conversion(s, t)) return Conversion::explicit_nullable_conversion();
        if (any_numeric_conversion(s, t)) return Conversion::explicit_lifted_numeric_conversion();
        if (explicit_enumeration_conversion(s, t)) return Conversion::enumeration_conversion(false, true);
    }
    return Conversion::none();
}

// ---- Null Literal Conversion --------------------------------------------------------------------------------------------------

bool CSharpConversions::null_literal_conversion(const IType* from_type, const IType* to_type) {
    // C# 9.0 spec: 10.2.7
    if (from_type->kind() == TypeKind::Null) {
        return nullable_type::is_nullable(to_type) || to_type->is_reference_type() == std::optional<bool>(true);
    }
    return false;
}

// ---- Implicit Reference Conversion --------------------------------------------------------------------------------------------

bool CSharpConversions::is_implicit_reference_conversion(const IType* from_type, const IType* to_type) {
    return implicit_reference_conversion(from_type, to_type, 0);
}

bool CSharpConversions::implicit_reference_conversion(const IType* from_type, const IType* to_type, int subtype_check_nesting_depth) {
    // C# 9.0 spec: 10.2.8

    // reference conversions are possible:
    // - if both types are known to be reference types
    // - if both types are type parameters and fromType has a class constraint
    //     (ImplicitTypeParameterConversionWithClassConstraintOnlyOnT)
    if (!(from_type->is_reference_type() == std::optional<bool>(true) && to_type->is_reference_type() != std::optional<bool>(false))) return false;

    const auto* from_array = as_type<ArrayType>(from_type);
    if (from_array != nullptr) {
        const auto* to_array = as_type<ArrayType>(to_type);
        if (to_array != nullptr) {
            // array covariance (the broken kind)
            return from_array->dimensions() == to_array->dimensions() &&
                   implicit_reference_conversion(from_array->element_type(), to_array->element_type(), subtype_check_nesting_depth);
        }
        // conversion from single-dimensional array S[] to IList<T>/IReadOnlyList<T> + base interfaces:
        const IType* to_type_argument = unpack_generic_array_interface(to_type);
        if (from_array->dimensions() == 1 && to_type_argument != nullptr) {
            // array covariance plays a part here as well (string[] is IList<object>)
            return identity_conversion(from_array->element_type(), to_type_argument) ||
                   implicit_reference_conversion(from_array->element_type(), to_type_argument, subtype_check_nesting_depth);
        }
        // conversion from any array to System.Array and the interfaces it implements:
        const IType* system_array = compilation_.find_type(KnownTypeCode::Array);
        return implicit_reference_conversion(system_array, to_type, subtype_check_nesting_depth);
    }

    // now comes the hard part: traverse the inheritance chain and figure out generics+variance
    return is_subtype_of(from_type, to_type, subtype_check_nesting_depth);
}

// For IList<T>, ICollection<T>, IEnumerable<T> and IReadOnlyList<T>, returns T. Otherwise, returns nullptr.
const IType* CSharpConversions::unpack_generic_array_interface(const IType* interface_type) {
    if (const auto* pt = as_type<ParameterizedType>(interface_type); pt != nullptr) {
        const ITypeDefinition* def = pt->get_definition();
        if (def != nullptr) {
            switch (def->known_type_code()) {
            case KnownTypeCode::IListOfT:
            case KnownTypeCode::ICollectionOfT:
            case KnownTypeCode::IEnumerableOfT:
            case KnownTypeCode::IReadOnlyListOfT:
            case KnownTypeCode::IReadOnlyCollectionOfT: return pt->get_type_argument(0);
            default: break;
            }
        }
    }
    return nullptr;
}

// Determines whether s is a subtype of t.
// Helper method used for ImplicitReferenceConversion, BoxingConversion and ImplicitTypeParameterConversion
bool CSharpConversions::is_subtype_of(const IType* s, const IType* t, int subtype_check_nesting_depth) {
    // conversion to dynamic + object are always possible
    if (t->kind() == TypeKind::Dynamic || typesystem::is_known_type(t, KnownTypeCode::Object)) return true;
    if (subtype_check_nesting_depth > 10) {
        // Subtyping in C# is undecidable
        // (see "On Decidability of Nominal Subtyping with Variance" by Andrew J. Kennedy and Benjamin C. Pierce),
        // so we'll prevent infinite recursions by putting a limit on the nesting depth of variance conversions.

        // No real C# code should use generics nested more than 10 levels deep, and even if they do, most of
        // those nestings should not involve variance.
        return false;
    }
    // let GetAllBaseTypes do the work for us
    for (const IType* base_type : typesystem::get_all_base_types(s)) {
        if (identity_or_variance_conversion(base_type, t, subtype_check_nesting_depth + 1)) return true;
    }
    return false;
}

bool CSharpConversions::identity_or_variance_conversion(const IType* s, const IType* t, int subtype_check_nesting_depth) {
    const ITypeDefinition* def = s->get_definition();
    if (def != nullptr) {
        const ITypeDefinition* t_def = t->get_definition();
        if (t_def == nullptr || !def->equals(t_def)) return false;
        const auto* ps = as_type<ParameterizedType>(s);
        const auto* pt = as_type<ParameterizedType>(t);
        if (ps != nullptr && pt != nullptr) {
            // C# spec (draft-v11): 19.2.3.3 Variance conversion
            for (int i = 0; i < static_cast<int>(def->type_parameters().size()); i++) {
                const IType* si = ps->get_type_argument(i);
                const IType* ti = pt->get_type_argument(i);
                if (identity_conversion(si, ti)) continue;
                const ITypeParameter* xi = def->type_parameters()[static_cast<std::size_t>(i)];
                switch (xi->variance()) {
                case VarianceModifier::Covariant:
                    if (!implicit_reference_conversion(si, ti, subtype_check_nesting_depth)) return false;
                    break;
                case VarianceModifier::Contravariant:
                    if (!implicit_reference_conversion(ti, si, subtype_check_nesting_depth)) return false;
                    break;
                default: return false;
                }
            }
        } else if (ps != nullptr || pt != nullptr) {
            return false;   // only of of them is parameterized, or counts don't match? -> not valid conversion
        }
        return true;
    }
    // not type definitions? we still need to check for equal types (e.g. s and t might be type parameters)
    return s->equals(t);
}

// ---- Explicit Reference Conversion --------------------------------------------------------------------------------------------

bool CSharpConversions::explicit_reference_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.3.5 Explicit reference conversions

    // test that the types are reference types:
    if (to_type->is_reference_type() != std::optional<bool>(true)) return false;
    if (from_type->is_reference_type() != std::optional<bool>(true)) {
        // special case:
        // converting from F to T is a reference conversion where T : class, F
        // (because F actually must be a reference type as well, even though C# doesn't treat it as one)
        if (from_type->kind() == TypeKind::TypeParameter) return is_subtype_of(to_type, from_type, 0);
        return false;
    }

    if (to_type->kind() == TypeKind::Array) {
        const auto* to_array = as_type<ArrayType>(to_type);
        if (from_type->kind() == TypeKind::Array) {
            // Array covariance
            const auto* from_array = as_type<ArrayType>(from_type);
            if (from_array->dimensions() != to_array->dimensions()) return false;
            return explicit_reference_conversion(from_array->element_type(), to_array->element_type());
        }
        const IType* from_type_argument = unpack_generic_array_interface(from_type);
        if (from_type_argument != nullptr && to_array->dimensions() == 1) {
            return explicit_reference_conversion(from_type_argument, to_array->element_type()) ||
                   identity_conversion(from_type_argument, to_array->element_type());
        }
        // Otherwise treat the array like a sealed class - require implicit conversion in the opposite direction
        return is_implicit_reference_conversion(to_type, from_type);
    } else if (from_type->kind() == TypeKind::Array) {
        const auto* from_array = as_type<ArrayType>(from_type);
        const IType* to_type_argument = unpack_generic_array_interface(to_type);
        if (to_type_argument != nullptr && from_array->dimensions() == 1) {
            return explicit_reference_conversion(from_array->element_type(), to_type_argument);
        }
        // Otherwise treat the array like a sealed class
        return is_implicit_reference_conversion(from_type, to_type);
    } else if (from_type->kind() == TypeKind::Delegate && to_type->kind() == TypeKind::Delegate) {
        const ITypeDefinition* def = from_type->get_definition();
        if (def == nullptr || !typesystem::type_equals(def, to_type->get_definition())) return false;
        const auto* ps = as_type<ParameterizedType>(from_type);
        const auto* pt = as_type<ParameterizedType>(to_type);
        if (ps == nullptr || pt == nullptr) {
            // non-generic delegate - return true for the identity conversion
            return ps == nullptr && pt == nullptr;
        }
        for (int i = 0; i < static_cast<int>(def->type_parameters().size()); i++) {
            const IType* si = ps->get_type_argument(i);
            const IType* ti = pt->get_type_argument(i);
            if (identity_conversion(si, ti)) continue;
            const ITypeParameter* xi = def->type_parameters()[static_cast<std::size_t>(i)];
            switch (xi->variance()) {
            case VarianceModifier::Covariant:
                if (!explicit_reference_conversion(si, ti)) return false;
                break;
            case VarianceModifier::Contravariant:
                if (!(si->is_reference_type() == std::optional<bool>(true) && ti->is_reference_type() == std::optional<bool>(true))) return false;
                break;
            default: return false;
            }
        }
        return true;
    } else if (is_sealed_reference_type(from_type)) {
        // If the source type is sealed, explicit conversions can't do anything more than implicit ones
        return is_implicit_reference_conversion(from_type, to_type);
    } else if (is_sealed_reference_type(to_type)) {
        // The the target type is sealed, there must be an implicit conversion in the opposite direction
        return is_implicit_reference_conversion(to_type, from_type);
    }
    if (from_type->kind() == TypeKind::Interface || to_type->kind() == TypeKind::Interface) return true;
    return is_implicit_reference_conversion(to_type, from_type) || is_implicit_reference_conversion(from_type, to_type);
}

bool CSharpConversions::is_sealed_reference_type(const IType* type) {
    const TypeKind kind = type->kind();
    const ITypeDefinition* def = type->get_definition();
    return (kind == TypeKind::Class && def != nullptr && def->is_sealed()) || kind == TypeKind::Delegate;
}

// ---- Boxing Conversions -------------------------------------------------------------------------------------------------------

bool CSharpConversions::is_boxing_conversion(const IType* from_type, const IType* to_type) {
    // C# 9.0 spec: 10.2.9
    from_type = nullable_type::get_underlying_type(from_type);
    if (from_type->is_reference_type() == std::optional<bool>(false) && !from_type->is_by_ref_like() &&
        to_type->is_reference_type() == std::optional<bool>(true))
        return is_subtype_of(from_type, to_type, 0);
    return false;
}

bool CSharpConversions::is_boxing_conversion_or_involving_type_parameter(const IType* from_type, const IType* to_type) {
    return is_boxing_conversion(from_type, to_type) || implicit_type_parameter_conversion(from_type, to_type);
}

bool CSharpConversions::unboxing_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.3.7 Unboxing conversions
    to_type = nullable_type::get_underlying_type(to_type);
    if (from_type->is_reference_type() == std::optional<bool>(true) && to_type->is_reference_type() == std::optional<bool>(false))
        return is_subtype_of(to_type, from_type, 0);
    return false;
}

// ---- Implicit Constant-Expression Conversion ----------------------------------------------------------------------------------

bool CSharpConversions::implicit_constant_expression_conversion(ResolveResult* rr, const IType* to_type) {
    if (rr == nullptr || !rr->is_compile_time_constant()) return false;
    // C# 9.0 spec: 10.2.11 + part of 10.2.6 (Nullable conversions)
    const KnownTypeCode from_type_code = get_type_code(rr->type());
    to_type = nullable_type::get_underlying_type(to_type);
    KnownTypeCode to_type_code = get_type_code(to_type);
    if (to_type->kind() == TypeKind::NUInt) to_type_code = KnownTypeCode::UInt32;
    const ConstantValue constant_value = rr->constant_value();
    if (from_type_code == KnownTypeCode::Int64) {
        const auto* val = std::get_if<std::int64_t>(&constant_value);
        return val != nullptr && *val >= 0 && to_type_code == KnownTypeCode::UInt64;
    } else if (from_type_code == KnownTypeCode::Int32) {
        if (std::holds_alternative<std::monostate>(constant_value)) return false;
        const auto* int_value = std::get_if<std::int32_t>(&constant_value);
        if (int_value == nullptr) return false;
        const std::int32_t val = *int_value;
        switch (to_type_code) {
        case KnownTypeCode::SByte: return val >= INT8_MIN && val <= INT8_MAX;
        case KnownTypeCode::Byte: return val >= 0 && val <= UINT8_MAX;
        case KnownTypeCode::Int16: return val >= INT16_MIN && val <= INT16_MAX;
        case KnownTypeCode::UInt16: return val >= 0 && val <= UINT16_MAX;
        case KnownTypeCode::UInt32:
        case KnownTypeCode::UInt64: return val >= 0;
        default: break;
        }
    }
    return false;
}

// ---- Conversions involving type parameters ------------------------------------------------------------------------------------

bool CSharpConversions::implicit_type_parameter_conversion(const IType* from_type, const IType* to_type) {
    // C# 9.0 spec: 10.2.12
    if (from_type->kind() != TypeKind::TypeParameter) return false;   // not a type parameter
    if (from_type->is_reference_type().has_value()) return false;     // already handled by ImplicitReferenceConversion/BoxingConversion
    return is_subtype_of(from_type, to_type, 0);
}

const Conversion* CSharpConversions::explicit_type_parameter_conversion(const IType* from_type, const IType* to_type) {
    if (to_type->kind() == TypeKind::TypeParameter) {
        // Explicit type parameter conversions that aren't also
        // reference conversions are considered to be unboxing conversions
        if (from_type->kind() == TypeKind::Interface || is_subtype_of(to_type, from_type, 0)) return Conversion::unboxing_conversion();
    } else {
        if (from_type->kind() == TypeKind::TypeParameter && to_type->kind() == TypeKind::Interface) return Conversion::boxing_conversion();
    }
    return Conversion::none();
}

// ---- Pointer Conversions ------------------------------------------------------------------------------------------------------

bool CSharpConversions::implicit_pointer_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 24.5 Pointer conversions
    if (typesystem::is_any_pointer(from_type->kind()) && as_type<PointerType>(to_type) != nullptr && to_type->reflection_name() == "System.Void*") return true;
    if (from_type->kind() == TypeKind::Null && typesystem::is_any_pointer(to_type->kind())) return true;
    const auto* from_fn_ptr = as_type<FunctionPointerType>(from_type);
    const auto* to_fn_ptr = as_type<FunctionPointerType>(to_type);
    if (from_fn_ptr != nullptr && to_fn_ptr != nullptr && from_fn_ptr->calling_convention == to_fn_ptr->calling_convention &&
        from_fn_ptr->parameter_types.size() == to_fn_ptr->parameter_types.size()) {
        // Variance applies to function pointer types
        constexpr int nesting_depth = 0;
        if (!(identity_conversion(from_fn_ptr->return_type, to_fn_ptr->return_type) ||
              implicit_reference_conversion(from_fn_ptr->return_type, to_fn_ptr->return_type, nesting_depth))) {
            return false;
        }
        for (std::size_t i = 0; i < from_fn_ptr->parameter_types.size(); i++) {
            const IType* from_pt = from_fn_ptr->parameter_types[i];
            const IType* to_pt = to_fn_ptr->parameter_types[i];
            if (!(identity_conversion(to_pt, from_pt) || implicit_reference_conversion(to_pt, from_pt, nesting_depth))) return false;
        }
        return true;
    }
    return false;
}

bool CSharpConversions::explicit_pointer_conversion(const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 24.5 Pointer conversions
    if (typesystem::is_any_pointer(from_type->kind())) return typesystem::is_any_pointer(to_type->kind()) || is_integer_type(to_type);
    return typesystem::is_any_pointer(to_type->kind()) && is_integer_type(from_type);
}

bool CSharpConversions::is_integer_type(const IType* type) {
    switch (type->kind()) {
    case TypeKind::NInt:
    case TypeKind::NUInt: return true;
    default: break;
    }
    const KnownTypeCode c = get_type_code(type);
    return code(c) >= code(KnownTypeCode::SByte) && code(c) <= code(KnownTypeCode::UInt64);
}

// ---- User-Defined Conversions -------------------------------------------------------------------------------------------------

// Whether type A is encompassed by type B.
bool CSharpConversions::is_encompassed_by(const IType* a, const IType* b) { return standard_implicit_conversion(a, b)->is_valid(); }

bool CSharpConversions::is_encompassing_or_encompassed_by(const IType* a, const IType* b) {
    return standard_implicit_conversion(a, b)->is_valid() || standard_implicit_conversion(b, a)->is_valid();
}

const IType* CSharpConversions::find_most_encompassed_type(const std::vector<const IType*>& candidates) {
    const IType* best = nullptr;
    for (const IType* current : candidates) {
        if (best == nullptr || is_encompassed_by(current, best))
            best = current;
        else if (!is_encompassed_by(best, current))
            return nullptr;   // Ambiguous
    }
    return best;
}

const IType* CSharpConversions::find_most_encompassing_type(const std::vector<const IType*>& candidates) {
    const IType* best = nullptr;
    for (const IType* current : candidates) {
        if (best == nullptr || is_encompassed_by(best, current))
            best = current;
        else if (!is_encompassed_by(current, best))
            return nullptr;   // Ambiguous
    }
    return best;
}

const Conversion* CSharpConversions::select_operator(const IType* most_specific_source, const IType* most_specific_target,
                                                     const std::vector<OperatorInfo>& operators, bool is_implicit, const IType* source,
                                                     const IType* target) {
    std::vector<const OperatorInfo*> selected;
    for (const OperatorInfo& op : operators) {
        if (typesystem::type_equals(op.source_type, most_specific_source) && typesystem::type_equals(op.target_type, most_specific_target))
            selected.push_back(&op);
    }
    if (selected.empty()) return Conversion::none();

    if (selected.size() == 1) {
        const Conversion* before = explicit_conversion_not_user_defined(source, most_specific_source);
        const Conversion* after = explicit_conversion_not_user_defined(most_specific_target, target);
        return Conversion::user_defined_conversion(selected[0]->method, is_implicit, before, after, selected[0]->is_lifted);
    }

    int n_non_lifted = 0;
    for (const OperatorInfo* s : selected) {
        if (!s->is_lifted) n_non_lifted++;
    }
    if (n_non_lifted == 1) {
        const OperatorInfo* op = nullptr;
        for (const OperatorInfo* s : selected) {
            if (!s->is_lifted) {
                op = s;
                break;
            }
        }
        const Conversion* before = explicit_conversion_not_user_defined(source, most_specific_source);
        const Conversion* after = explicit_conversion_not_user_defined(most_specific_target, target);
        return Conversion::user_defined_conversion(op->method, is_implicit, before, after, op->is_lifted);
    }

    const Conversion* before = explicit_conversion_not_user_defined(source, most_specific_source);
    const Conversion* after = explicit_conversion_not_user_defined(most_specific_target, target);
    return Conversion::user_defined_conversion(selected[0]->method, is_implicit, before, after, selected[0]->is_lifted, true);
}

const Conversion* CSharpConversions::user_defined_implicit_conversion(ResolveResult* from_result, const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.5.4 User-defined implicit conversions

    // user-defined conversions are not supported with interfaces
    if (from_type->kind() == TypeKind::Interface || to_type->kind() == TypeKind::Interface) return Conversion::none();

    const std::vector<OperatorInfo> operators = get_applicable_conversion_operators(from_result, from_type, to_type, false);

    if (!operators.empty()) {
        const IType* most_specific_source = nullptr;
        bool any_source_equals = false;
        for (const OperatorInfo& op : operators) {
            if (typesystem::type_equals(op.source_type, from_type)) {
                any_source_equals = true;
                break;
            }
        }
        if (any_source_equals) {
            most_specific_source = from_type;
        } else {
            std::vector<const IType*> source_types;
            for (const OperatorInfo& op : operators) source_types.push_back(op.source_type);
            most_specific_source = find_most_encompassed_type(source_types);
        }
        if (most_specific_source == nullptr)
            return Conversion::user_defined_conversion(operators[0].method, true, Conversion::none(), Conversion::none(), operators[0].is_lifted, true);
        const IType* most_specific_target = nullptr;
        bool any_target_equals = false;
        for (const OperatorInfo& op : operators) {
            if (typesystem::type_equals(op.target_type, to_type)) {
                any_target_equals = true;
                break;
            }
        }
        if (any_target_equals) {
            most_specific_target = to_type;
        } else {
            std::vector<const IType*> target_types;
            for (const OperatorInfo& op : operators) target_types.push_back(op.target_type);
            most_specific_target = find_most_encompassing_type(target_types);
        }
        if (most_specific_target == nullptr) {
            if (nullable_type::is_nullable(to_type))
                return user_defined_implicit_conversion(from_result, from_type, nullable_type::get_underlying_type(to_type));
            return Conversion::user_defined_conversion(operators[0].method, true, Conversion::none(), Conversion::none(), operators[0].is_lifted, true);
        }

        const Conversion* selected = select_operator(most_specific_source, most_specific_target, operators, true, from_type, to_type);
        if (selected != Conversion::none()) {
            if (selected->is_lifted() && nullable_type::is_nullable(to_type)) {
                // Prefer A -> B -> B? over A -> A? -> B?
                const Conversion* other = user_defined_implicit_conversion(from_result, from_type, nullable_type::get_underlying_type(to_type));
                if (other != Conversion::none()) return other;
            }
            return selected;
        } else if (nullable_type::is_nullable(to_type)) {
            return user_defined_implicit_conversion(from_result, from_type, nullable_type::get_underlying_type(to_type));
        }
        return Conversion::none();
    }
    return Conversion::none();
}

const Conversion* CSharpConversions::user_defined_explicit_conversion(ResolveResult* from_result, const IType* from_type, const IType* to_type) {
    // C# spec (draft-v11): 10.5.5 User-defined explicit conversions

    // user-defined conversions are not supported with interfaces
    if (from_type->kind() == TypeKind::Interface || to_type->kind() == TypeKind::Interface) return Conversion::none();

    const std::vector<OperatorInfo> operators = get_applicable_conversion_operators(from_result, from_type, to_type, true);
    if (!operators.empty()) {
        const IType* most_specific_source;
        bool any_source_equals = false;
        for (const OperatorInfo& op : operators) {
            if (typesystem::type_equals(op.source_type, from_type)) {
                any_source_equals = true;
                break;
            }
        }
        if (any_source_equals) {
            most_specific_source = from_type;
        } else {
            std::vector<const IType*> with_source_encompassing_from_type;
            for (const OperatorInfo& op : operators) {
                if (is_encompassed_by(from_type, op.source_type) ||
                    implicit_constant_expression_conversion(from_result, nullable_type::get_underlying_type(op.source_type)))
                    with_source_encompassing_from_type.push_back(op.source_type);
            }
            if (!with_source_encompassing_from_type.empty()) {
                most_specific_source = find_most_encompassed_type(with_source_encompassing_from_type);
            } else {
                std::vector<const IType*> source_types;
                for (const OperatorInfo& op : operators) source_types.push_back(op.source_type);
                most_specific_source = find_most_encompassing_type(source_types);
            }
        }
        if (most_specific_source == nullptr)
            return Conversion::user_defined_conversion(operators[0].method, false, Conversion::none(), Conversion::none(), operators[0].is_lifted, true);

        const IType* most_specific_target;
        bool any_target_equals = false;
        bool any_target_encompassed_by_to_type = false;
        for (const OperatorInfo& op : operators) {
            if (typesystem::type_equals(op.target_type, to_type)) any_target_equals = true;
        }
        if (any_target_equals) {
            most_specific_target = to_type;
        } else {
            std::vector<const IType*> encompassed_targets;
            for (const OperatorInfo& op : operators) {
                if (is_encompassed_by(op.target_type, to_type)) {
                    any_target_encompassed_by_to_type = true;
                    encompassed_targets.push_back(op.target_type);
                }
            }
            if (any_target_encompassed_by_to_type) {
                most_specific_target = find_most_encompassing_type(encompassed_targets);
            } else {
                std::vector<const IType*> target_types;
                for (const OperatorInfo& op : operators) target_types.push_back(op.target_type);
                most_specific_target = find_most_encompassed_type(target_types);
            }
        }
        if (most_specific_target == nullptr) {
            if (nullable_type::is_nullable(to_type))
                return user_defined_explicit_conversion(from_result, from_type, nullable_type::get_underlying_type(to_type));
            return Conversion::user_defined_conversion(operators[0].method, false, Conversion::none(), Conversion::none(), operators[0].is_lifted, true);
        }

        const Conversion* selected = select_operator(most_specific_source, most_specific_target, operators, false, from_type, to_type);
        if (selected != Conversion::none()) {
            if (selected->is_lifted() && nullable_type::is_nullable(to_type)) {
                // Prefer A -> B -> B? over A -> A? -> B?
                const Conversion* other = user_defined_implicit_conversion(from_result, from_type, nullable_type::get_underlying_type(to_type));
                if (other != Conversion::none()) return other;
            }
            return selected;
        } else if (nullable_type::is_nullable(to_type)) {
            return user_defined_explicit_conversion(from_result, from_type, nullable_type::get_underlying_type(to_type));
        } else if (nullable_type::is_nullable(from_type)) {
            return user_defined_explicit_conversion(nullptr, nullable_type::get_underlying_type(from_type), to_type);   // A? -> A -> B
        }
        return Conversion::none();
    }
    return Conversion::none();
}

namespace {

const IType* underlying_type_for_conversion(const IType* type) {
    if (type->kind() == TypeKind::ByReference) type = as_type<ByReferenceType>(type)->element_type();
    return nullable_type::get_underlying_type(type);
}

}  // namespace

std::vector<CSharpConversions::OperatorInfo> CSharpConversions::get_applicable_conversion_operators(ResolveResult* from_result, const IType* from_type,
                                                                                                    const IType* to_type, bool is_explicit) {
    // Find the candidate operators:
    typesystem::Filter<IMethod> op_filter;
    if (is_explicit) {
        op_filter = [](const IMethod* m) {
            return m->is_static() && m->is_operator() && (m->name() == "op_Explicit" || m->name() == "op_Implicit") && m->parameters().size() == 1;
        };
    } else {
        op_filter = [](const IMethod* m) { return m->is_static() && m->is_operator() && m->name() == "op_Implicit" && m->parameters().size() == 1; };
    }

    std::vector<const IMethod*> operators;
    for (const IMethod* m : underlying_type_for_conversion(from_type)->get_methods(op_filter)) operators.push_back(m);
    for (const IMethod* m : underlying_type_for_conversion(to_type)->get_methods(op_filter)) operators.push_back(m);
    // Distinct
    std::vector<const IMethod*> distinct_operators;
    for (const IMethod* m : operators) {
        bool seen = false;
        for (const IMethod* d : distinct_operators) {
            if (typesystem::member_equals(d, m)) {
                seen = true;
                break;
            }
        }
        if (!seen) distinct_operators.push_back(m);
    }
    // Determine whether one of them is applicable:
    std::vector<OperatorInfo> result;
    for (const IMethod* op : distinct_operators) {
        const IType* source_type = op->parameters()[0]->type();
        if (source_type->kind() == TypeKind::ByReference && op->parameters()[0]->reference_kind() == ReferenceKind::In && from_type->kind() != TypeKind::ByReference) {
            source_type = as_type<ByReferenceType>(source_type)->element_type();
        }
        const IType* target_type = op->return_type();
        // Try if the operator is applicable:
        bool is_applicable;
        if (is_explicit) {
            is_applicable = (is_encompassing_or_encompassed_by(from_type, source_type) || implicit_constant_expression_conversion(from_result, source_type)) &&
                            is_encompassing_or_encompassed_by(target_type, to_type);
        } else {
            is_applicable = (is_encompassed_by(from_type, source_type) || implicit_constant_expression_conversion(from_result, source_type)) &&
                            is_encompassed_by(target_type, to_type);
        }
        // Try if the operator is applicable in lifted form:
        if (is_applicable) result.push_back(OperatorInfo{op, source_type, target_type, false});
        if (nullable_type::is_non_nullable_value_type(source_type)) {
            // An operator can be applicable in both lifted and non-lifted form in case of explicit conversions
            const IType* lifted_source_type = nullable_type::create(compilation_, source_type);
            const IType* lifted_target_type = nullable_type::is_non_nullable_value_type(target_type) ? nullable_type::create(compilation_, target_type) : target_type;
            if (is_explicit) {
                is_applicable = is_encompassing_or_encompassed_by(from_type, lifted_source_type) && is_encompassing_or_encompassed_by(lifted_target_type, to_type);
            } else {
                is_applicable = is_encompassed_by(from_type, lifted_source_type) && is_encompassed_by(lifted_target_type, to_type);
            }

            if (is_applicable) result.push_back(OperatorInfo{op, lifted_source_type, lifted_target_type, true});
        }
    }
    return result;
}

// ---- Implicit Span Conversion -------------------------------------------------------------------------------------------------

bool CSharpConversions::is_implicit_span_conversion(const IType* from_type, const IType* to_type) {
    if (!typesystem::any(compilation_.type_system_options(), TypeSystemOptions::FirstClassSpanTypes)) return false;

    // An implicit span conversion permits array_types, System.Span<T>, System.ReadOnlySpan<T>,
    // and string to be converted between each other
    // see https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/proposals/csharp-14.0/first-class-span-types#span-conversions

    const auto* from_array = as_type<ArrayType>(from_type);
    const auto* from_parameterized = as_type<ParameterizedType>(from_type);
    if (from_array != nullptr && from_array->dimensions() == 1) {
        const IType* element_type = from_array->element_type();
        if (typesystem::is_known_type(to_type, KnownTypeCode::SpanOfT)) return identity_conversion(element_type, to_type->type_arguments()[0]);
        if (typesystem::is_known_type(to_type, KnownTypeCode::ReadOnlySpanOfT)) {
            return identity_conversion(element_type, to_type->type_arguments()[0]) || is_implicit_reference_conversion(element_type, to_type->type_arguments()[0]);
        }
    } else if (from_parameterized != nullptr && (typesystem::is_known_type(from_parameterized, KnownTypeCode::SpanOfT) ||
                                                 typesystem::is_known_type(from_parameterized, KnownTypeCode::ReadOnlySpanOfT))) {
        if (typesystem::is_known_type(to_type, KnownTypeCode::ReadOnlySpanOfT)) {
            return identity_conversion(from_parameterized->type_arguments()[0], to_type->type_arguments()[0]) ||
                   is_implicit_reference_conversion(from_parameterized->type_arguments()[0], to_type->type_arguments()[0]);
        }
    } else if (typesystem::is_known_type(from_type, KnownTypeCode::String)) {
        return typesystem::is_known_type(to_type, KnownTypeCode::ReadOnlySpanOfT) &&
               typesystem::is_known_type(to_type->type_arguments()[0], KnownTypeCode::Char);
    }

    return false;
}

// ---- AnonymousFunctionConversion ----------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::anonymous_function_conversion(ResolveResult* resolve_result, const IType* to_type) {
    // C# 9.0 spec 10.7 Anonymous function conversions
    auto* f = dynamic_cast<LambdaResolveResult*>(resolve_result);
    if (f == nullptr) return Conversion::none();
    if (!f->is_anonymous_method()) {
        // It's a lambda, so conversions to expression trees exist
        // (even if the conversion leads to a compile-time error, e.g. for statement lambdas)
        to_type = unpack_expression_tree_type(to_type);
    }
    const IMethod* d = typesystem::get_delegate_invoke_method(to_type);
    if (d == nullptr) return Conversion::none();

    std::vector<const IType*> d_param_types(d->parameters().size());
    for (std::size_t i = 0; i < d_param_types.size(); i++) d_param_types[i] = d->parameters()[i]->type();
    const IType* d_return_type = d->return_type();

    if (f->has_parameter_list()) {
        // If F contains an anonymous-function-signature, then D and F have the same number of parameters.
        if (d->parameters().size() != f->parameters().size()) return Conversion::none();

        if (f->is_implicitly_typed()) {
            // If F has an implicitly typed parameter list, D has no ref or out parameters.
            // TODO: what about in parameters?
            for (const IParameter* p : d->parameters()) {
                if (p->reference_kind() != ReferenceKind::None) return Conversion::none();
            }
        } else {
            // If F has an explicitly typed parameter list, each parameter in D has the same type
            // and modifiers as the corresponding parameter in F.
            for (std::size_t i = 0; i < f->parameters().size(); i++) {
                const IParameter* p_d = d->parameters()[i];
                const IParameter* p_f = f->parameters()[i];
                if (p_d->reference_kind() != p_f->reference_kind()) return Conversion::none();
                if (!identity_conversion(d_param_types[i], p_f->type())) return Conversion::none();
            }
        }
    } else {
        // If F does not contain an anonymous-function-signature, then D may have zero or more parameters of any
        // type, as long as no parameter of D has the out parameter modifier.
        for (const IParameter* p : d->parameters()) {
            if (p->reference_kind() == ReferenceKind::Out) return Conversion::none();
        }
    }

    return f->is_valid(d_param_types, d_return_type, this);
}

// ---- MethodGroupConversion ----------------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::method_group_conversion(ResolveResult* resolve_result, const IType* to_type) {
    // C# 9.0 spec 10.8 Method group conversions
    auto* rr = dynamic_cast<MethodGroupResolveResult*>(resolve_result);
    if (rr == nullptr) return Conversion::none();
    const IMethod* invoke = typesystem::get_delegate_invoke_method(to_type);
    if (invoke == nullptr) return Conversion::none();

    std::vector<ResolveResult*> args(invoke->parameters().size());
    for (std::size_t i = 0; i < args.size(); i++) {
        const IParameter* param = invoke->parameters()[i];
        const IType* parameter_type = param->type();
        if (param->reference_kind() != ReferenceKind::None && parameter_type->kind() == TypeKind::ByReference) {
            parameter_type = as_type<ByReferenceType>(parameter_type)->element_type();
            args[i] = make<ByReferenceResolveResult>(parameter_type, param->reference_kind());
        } else if (param->type()->kind() == TypeKind::Dynamic) {
            args[i] = make<ResolveResult>(compilation_.find_type(KnownTypeCode::Object));
        } else {
            args[i] = make<ResolveResult>(parameter_type);
        }
    }
    OverloadResolution overload_resolution = perform_overload_resolution(*rr, compilation_, std::move(args), {}, true, false, false, false, false, this);
    if (overload_resolution.found_applicable_candidate()) {
        const auto* method = dynamic_cast<const IMethod*>(overload_resolution.get_best_candidate_with_substituted_type_arguments());
        const auto* this_rr = dynamic_cast<ThisResolveResult*>(rr->target_result());
        const bool is_virtual = method->is_overridable() && !(this_rr != nullptr && this_rr->causes_non_virtual_invocation());
        const bool is_valid = !overload_resolution.is_ambiguous() && is_delegate_compatible(method, invoke, overload_resolution.is_extension_method_invocation());
        const bool delegate_captures_first_argument = overload_resolution.is_extension_method_invocation() || !method->is_static();
        if (is_valid) return Conversion::method_group_conversion(method, is_virtual, delegate_captures_first_argument);
        return Conversion::invalid_method_group_conversion(method, is_virtual, delegate_captures_first_argument);
    }
    return Conversion::none();
}

// Gets whether a method is compatible with a delegate type.
// C# spec (draft-v11): 21.4 Delegate compatibility
bool CSharpConversions::is_delegate_compatible(const IMethod* method, const IType* delegate_type) {
    if (method == nullptr) throw std::invalid_argument("method");
    if (delegate_type == nullptr) throw std::invalid_argument("delegate_type");
    const IMethod* invoke = typesystem::get_delegate_invoke_method(delegate_type);
    if (invoke == nullptr) return false;
    return is_delegate_compatible(method, invoke, false);
}

// Gets whether a method m is compatible with a delegate type (d is the invoke method of the delegate).
// If is_extension_method_invocation is true, the first parameter of m is ignored.
bool CSharpConversions::is_delegate_compatible(const IMethod* m, const IMethod* d, bool is_extension_method_invocation) {
    // C# spec (draft-v11): 21.4 Delegate compatibility
    if (m == nullptr) throw std::invalid_argument("m");
    if (d == nullptr) throw std::invalid_argument("d");
    const std::size_t first_parameter_in_m = is_extension_method_invocation ? 1 : 0;
    if (m->parameters().size() - first_parameter_in_m != d->parameters().size()) return false;
    for (std::size_t i = 0; i < d->parameters().size(); i++) {
        const IParameter* pm = m->parameters()[first_parameter_in_m + i];
        const IParameter* pd = d->parameters()[i];
        // ret/out/in must match
        if (pm->reference_kind() != pd->reference_kind()) return false;
        if (pm->reference_kind() != ReferenceKind::None) {
            // ref/out/in parameters must have same types
            // according to the spec, but Roslyn seems to allow identity conversions
            if (!identity_conversion(pd->type(), pm->type())) return false;
        } else {
            // non-ref/out parameters must have an identity or reference conversion from pd to pm
            if (!identity_conversion(pd->type(), pm->type()) && !is_implicit_reference_conversion(pd->type(), pm->type())) return false;
        }
    }
    if (m->return_type_is_ref_read_only() != d->return_type_is_ref_read_only()) return false;
    // check return type compatibility
    return identity_conversion(m->return_type(), d->return_type()) || is_implicit_reference_conversion(m->return_type(), d->return_type());
}

// ---- Tuple Conversion ---------------------------------------------------------------------------------------------------------

const Conversion* CSharpConversions::tuple_conversion(TupleResolveResult* from_rr, const IType* to_type, bool is_explicit) {
    // C# 9.0 spec: 10.2.13 (implicit tuple conversions) + 10.3.6 (explicit tuple conversions)
    const std::vector<ResolveResult*>& from_elements = from_rr->elements();
    const auto to_elements = TupleType::get_tuple_element_types(to_type);
    if (!to_elements.has_value() || from_elements.size() != to_elements->size()) return Conversion::none();
    std::vector<const Conversion*> element_conversions(from_elements.size());
    for (std::size_t i = 0; i < element_conversions.size(); i++) {
        const Conversion* c;
        if (is_explicit) {
            c = explicit_conversion(from_elements[i], (*to_elements)[i]);
        } else {
            c = implicit_conversion(from_elements[i], (*to_elements)[i]);
        }
        if (!c->is_valid()) return Conversion::none();
        element_conversions[i] = c;
    }
    return Conversion::tuple_conversion(std::move(element_conversions));
}

const Conversion* CSharpConversions::tuple_conversion(const IType* from_type, const IType* to_type, bool is_explicit) {
    // C# 9.0 spec: 10.2.13 (implicit tuple conversions) + 10.3.6 (explicit tuple conversions)
    const auto from_elements = TupleType::get_tuple_element_types(from_type);
    if (!from_elements.has_value() || from_elements->empty()) return Conversion::none();
    const auto to_elements = TupleType::get_tuple_element_types(to_type);
    if (!to_elements.has_value() || from_elements->size() != to_elements->size()) return Conversion::none();
    std::vector<const Conversion*> element_conversions(from_elements->size());
    for (std::size_t i = 0; i < element_conversions.size(); i++) {
        const Conversion* c;
        if (is_explicit) {
            c = explicit_conversion((*from_elements)[i], (*to_elements)[i]);
        } else {
            c = implicit_conversion((*from_elements)[i], (*to_elements)[i]);
        }
        if (!c->is_valid()) return Conversion::none();
        element_conversions[i] = c;
    }
    return Conversion::tuple_conversion(std::move(element_conversions));
}

// ---- BetterConversion ---------------------------------------------------------------------------------------------------------

int CSharpConversions::better_conversion(ResolveResult* resolve_result, const IType* t1, const IType* t2) {
    const bool t1_exact = is_exactly_matching(resolve_result, t1);
    const bool t2_exact = is_exactly_matching(resolve_result, t2);
    if (t1_exact && !t2_exact) return 1;
    if (t2_exact && !t1_exact) return 2;
    if (!t1_exact && !t2_exact) {
        const bool c1_implicit_span_conversion = is_implicit_span_conversion(resolve_result->type(), t1);
        const bool c2_implicit_span_conversion = is_implicit_span_conversion(resolve_result->type(), t2);
        if (c1_implicit_span_conversion && !c2_implicit_span_conversion) return 1;
        if (c2_implicit_span_conversion && !c1_implicit_span_conversion) return 2;
    }
    if (t1_exact == t2_exact) {
        const int r = better_conversion_target(t1, t2);
        if (r != 0) return r;
    }
    auto* lambda = dynamic_cast<LambdaResolveResult*>(resolve_result);
    if (lambda != nullptr) {
        if (!lambda->is_anonymous_method()) {
            t1 = unpack_expression_tree_type(t1);
            t2 = unpack_expression_tree_type(t2);
        }
        const IMethod* m1 = typesystem::get_delegate_invoke_method(t1);
        const IMethod* m2 = typesystem::get_delegate_invoke_method(t2);
        if (m1 == nullptr || m2 == nullptr) return 0;
        if (m1->parameters().size() != m2->parameters().size()) return 0;
        std::vector<const IType*> parameter_types(m1->parameters().size());
        for (std::size_t i = 0; i < parameter_types.size(); i++) {
            parameter_types[i] = m1->parameters()[i]->type();
            if (!typesystem::type_equals(parameter_types[i], m2->parameters()[i]->type())) return 0;
        }
        if (lambda->has_parameter_list() && parameter_types.size() != lambda->parameters().size()) return 0;

        const IType* ret1 = m1->return_type();
        const IType* ret2 = m2->return_type();
        if (ret1->kind() == TypeKind::Void && ret2->kind() != TypeKind::Void) return 2;
        if (ret1->kind() != TypeKind::Void && ret2->kind() == TypeKind::Void) return 1;

        const IType* inferred_ret = lambda->get_inferred_return_type(parameter_types);
        int r = better_conversion(inferred_ret, ret1, ret2);
        if (r == 0 && lambda->is_async()) {
            ret1 = unpack_task(ret1);
            ret2 = unpack_task(ret2);
            inferred_ret = unpack_task(inferred_ret);
            if (ret1 != nullptr && ret2 != nullptr && inferred_ret != nullptr) r = better_conversion(inferred_ret, ret1, ret2);
        }
        return r;
    }
    return better_conversion(resolve_result->type(), t1, t2);
}

// Gets whether an expression E exactly matches a type T (C# 8.0 spec, 12.6.4.6)
bool CSharpConversions::is_exactly_matching(ResolveResult* e, const IType* t) {
    const IType* s = e->type();
    if (identity_conversion(s, t)) return true;
    if (auto* lambda = dynamic_cast<LambdaResolveResult*>(e); lambda != nullptr) {
        if (!lambda->is_anonymous_method()) t = unpack_expression_tree_type(t);
        const IMethod* m = typesystem::get_delegate_invoke_method(t);
        if (m == nullptr) return false;
        std::vector<const IType*> parameter_types(m->parameters().size());
        for (std::size_t i = 0; i < parameter_types.size(); i++) parameter_types[i] = m->parameters()[i]->type();
        const IType* x = lambda->get_inferred_return_type(parameter_types);
        const IType* y = m->return_type();
        if (identity_conversion(x, y)) return true;
        if (lambda->is_async()) {
            x = unpack_task(x);
            y = unpack_task(y);
        }
        if (x != nullptr && y != nullptr) return is_exactly_matching(make<ResolveResult>(x), y);
        return false;
    }
    return false;
}

// The better conversion from a type (C# 4.0 spec, 7.5.3.4; the current standard no longer has this subclause, it was folded into
// 12.6.4.5-12.6.4.7)
int CSharpConversions::better_conversion(const IType* s, const IType* t1, const IType* t2) {
    const bool ident1 = identity_conversion(s, t1);
    const bool ident2 = identity_conversion(s, t2);
    if (ident1 && !ident2) return 1;
    if (ident2 && !ident1) return 2;
    return better_conversion_target(t1, t2);
}

// The better conversion target (C# 9.0 spec, 12.6.4.7)
int CSharpConversions::better_conversion_target(const IType* t1, const IType* t2) {
    if (typesystem::is_known_type(t1, KnownTypeCode::ReadOnlySpanOfT)) {
        if (typesystem::is_known_type(t2, KnownTypeCode::SpanOfT)) {
            if (identity_conversion(t1->type_arguments()[0], t2->type_arguments()[0])) return 1;
        }
        if (typesystem::is_known_type(t2, KnownTypeCode::ReadOnlySpanOfT)) {
            const bool t1_to_2 = implicit_conversion(t1->type_arguments()[0], t2->type_arguments()[0])->is_valid();
            const bool t2_to_1 = implicit_conversion(t2->type_arguments()[0], t1->type_arguments()[0])->is_valid();
            if (t1_to_2 && !t2_to_1) return 1;
        }
    }

    if (typesystem::is_known_type(t2, KnownTypeCode::ReadOnlySpanOfT)) {
        if (typesystem::is_known_type(t1, KnownTypeCode::SpanOfT)) {
            if (identity_conversion(t2->type_arguments()[0], t1->type_arguments()[0])) return 2;
        }
        if (typesystem::is_known_type(t1, KnownTypeCode::ReadOnlySpanOfT)) {
            const bool t1_to_2 = implicit_conversion(t1->type_arguments()[0], t2->type_arguments()[0])->is_valid();
            const bool t2_to_1 = implicit_conversion(t2->type_arguments()[0], t1->type_arguments()[0])->is_valid();
            if (t2_to_1 && !t1_to_2) return 2;
        }
    }

    {
        const bool t1_to_2 = implicit_conversion(t1, t2)->is_valid();
        const bool t2_to_1 = implicit_conversion(t2, t1)->is_valid();
        if (t1_to_2 && !t2_to_1) return 1;
        if (t2_to_1 && !t1_to_2) return 2;
    }

    const IType* s1 = unpack_task(t1);
    const IType* s2 = unpack_task(t2);
    if (s1 != nullptr && s2 != nullptr) return better_conversion_target(s1, s2);

    const KnownTypeCode t1_code = get_type_code(t1);
    const KnownTypeCode t2_code = get_type_code(t2);
    if (is_better_integral_type(t1_code, t2_code)) return 1;
    if (is_better_integral_type(t2_code, t1_code)) return 2;
    return 0;
}

bool CSharpConversions::is_better_integral_type(KnownTypeCode t1, KnownTypeCode t2) {
    // signed types are better than unsigned types
    switch (t1) {
    case KnownTypeCode::SByte: return t2 == KnownTypeCode::Byte || t2 == KnownTypeCode::UInt16 || t2 == KnownTypeCode::UInt32 || t2 == KnownTypeCode::UInt64;
    case KnownTypeCode::Int16: return t2 == KnownTypeCode::UInt16 || t2 == KnownTypeCode::UInt32 || t2 == KnownTypeCode::UInt64;
    case KnownTypeCode::Int32: return t2 == KnownTypeCode::UInt32 || t2 == KnownTypeCode::UInt64;
    case KnownTypeCode::Int64: return t2 == KnownTypeCode::UInt64;
    default: return false;
    }
}

}  // namespace cb::dotnet::decompiler::csharp::resolver

// dotnet/decompiler/csharp/syntax/type_system_ast_builder.cpp -- see type_system_ast_builder.hpp.
#include "dotnet/decompiler/csharp/syntax/type_system_ast_builder.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <variant>

#include "dotnet/decompiler/csharp/resolver/csharp_resolver.hpp"
#include "dotnet/decompiler/csharp/resolver/member_lookup.hpp"
#include "dotnet/decompiler/csharp/syntax/syntax.hpp"
#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/semantics/array_create_resolve_result.hpp"
#include "dotnet/decompiler/semantics/constant_resolve_result.hpp"
#include "dotnet/decompiler/semantics/conversion_resolve_result.hpp"
#include "dotnet/decompiler/semantics/error_resolve_result.hpp"
#include "dotnet/decompiler/semantics/initialized_object_resolve_result.hpp"
#include "dotnet/decompiler/semantics/member_resolve_result.hpp"
#include "dotnet/decompiler/semantics/namespace_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_of_resolve_result.hpp"
#include "dotnet/decompiler/semantics/type_resolve_result.hpp"
#include "dotnet/decompiler/semantics/unknown_member_resolve_result.hpp"
#include "dotnet/decompiler/typesystem/attributes.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/local_function_method.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/csharp_primitive_cast.hpp"
#include "dotnet/decompiler/util/culture_compare.hpp"
#include "dotnet/decompiler/util/decimal.hpp"
#include "dotnet/decompiler/util/number_format.hpp"
#include "dotnet/decompiler/il/transforms/local_function_decompiler.hpp"
#include "dotnet/decompiler/nr_extensions.hpp"
#include "dotnet/metadata/blob_reader.hpp"

namespace cb::dotnet::decompiler::csharp::syntax {

using namespace typesystem;
using resolver::CSharpResolver;
using resolver::MemberLookup;
using resolver::NameLookupMode;
using semantics::ArrayCreateResolveResult;
using semantics::ConstantResolveResult;
using semantics::ConversionResolveResult;
using semantics::ErrorResolveResult;
using semantics::InitializedObjectResolveResult;
using semantics::MemberResolveResult;
using semantics::NamespaceResolveResult;
using semantics::ResolveResult;
using semantics::TypeOfResolveResult;
using semantics::TypeResolveResult;
using semantics::UnknownIdentifierResolveResult;

namespace {

// ---- small helpers ---------------------------------------------------------------------------------------------------------

bool ends_with(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// AstNode.GetResolveResult: the node's resolve result, or the unknown error.
ResolveResult* get_resolve_result(AstNode* node) {
    ResolveResult* rr = node->annotation<ResolveResult>();
    return rr != nullptr ? rr : ErrorResolveResult::unknown_error();
}

// A type's TypeCode (ReflectionHelper.GetTypeCode): the known type code of a definition for the primitive types and string, else None.
KnownTypeCode get_type_code(const IType* type) {
    const auto* def = dynamic_cast<const ITypeDefinition*>(type);
    if (def == nullptr) return KnownTypeCode::None;
    KnownTypeCode code = def->known_type_code();
    if (code <= KnownTypeCode::String && code != KnownTypeCode::Void) return code;
    return KnownTypeCode::None;
}

SimpleType* make_simple_type(const std::string& name) {
    if (name == "_") return make<SimpleType>("@_");
    return make<SimpleType>(name);
}

MemberType* make_member_type(AstType* target, const std::string& name) {
    if (name == "_") return make<MemberType>(target, "@_");
    return make<MemberType>(target, name);
}

bool is_object_or_value_type(const IType* type) {
    const ITypeDefinition* d = type->get_definition();
    return d != nullptr && (d->known_type_code() == KnownTypeCode::Object || d->known_type_code() == KnownTypeCode::ValueType);
}

int count_descendants(AstNode* node) {
    int n = 0;
    for (AstNode* d : node->descendants()) {
        (void)d;
        ++n;
    }
    return n;
}

// String.CompareTo: .NET's culture-sensitive comparison (en-US, ICU), exact (util/culture_compare.hpp).
using util::culture_compare;

template <class T>
int compare_values(const T& a, const T& b) {
    return a < b ? -1 : (b < a ? 1 : 0);
}

// IComparable.CompareTo of two attribute argument values of the same kind; 0 when they are not comparable.
int compare_argument_values(const AttributeArgumentValue& a, const AttributeArgumentValue& b) {
    if (a.index() != b.index()) return 0;
    return std::visit(
        [&](const auto& x) -> int {
            using T = std::decay_t<decltype(x)>;
            const T& y = std::get<T>(b);
            if constexpr (std::is_same_v<T, std::monostate>) {
                return 0;
            } else if constexpr (std::is_same_v<T, bool>) {
                return compare_values(static_cast<int>(x), static_cast<int>(y));
            } else if constexpr (std::is_same_v<T, std::string>) {
                return culture_compare(x, y);
            } else if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, char16_t>) {
                return compare_values(x, y);
            } else {
                return 0;   // a type, an array: not IComparable
            }
        },
        a);
}

// ---- special constants (int.MaxValue and its like) -------------------------------------------------------------------------

struct SpecialConstantInfo {
    KnownTypeCode type;
    const char* member;
};

util::Decimal decimal_max_value() { return util::Decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, false, 0); }
util::Decimal decimal_min_value() { return util::Decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, true, 0); }

std::optional<SpecialConstantInfo> find_special_constant(const ConstantValue& constant) {
    return std::visit(
        [](const auto& v) -> std::optional<SpecialConstantInfo> {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::uint8_t>) {
                if (v == std::numeric_limits<std::uint8_t>::max()) return SpecialConstantInfo{KnownTypeCode::Byte, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::int8_t>) {
                if (v == std::numeric_limits<std::int8_t>::min()) return SpecialConstantInfo{KnownTypeCode::SByte, "MinValue"};
                if (v == std::numeric_limits<std::int8_t>::max()) return SpecialConstantInfo{KnownTypeCode::SByte, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::int16_t>) {
                if (v == std::numeric_limits<std::int16_t>::min()) return SpecialConstantInfo{KnownTypeCode::Int16, "MinValue"};
                if (v == std::numeric_limits<std::int16_t>::max()) return SpecialConstantInfo{KnownTypeCode::Int16, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::uint16_t>) {
                if (v == std::numeric_limits<std::uint16_t>::max()) return SpecialConstantInfo{KnownTypeCode::UInt16, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::int32_t>) {
                if (v == std::numeric_limits<std::int32_t>::min()) return SpecialConstantInfo{KnownTypeCode::Int32, "MinValue"};
                if (v == std::numeric_limits<std::int32_t>::max()) return SpecialConstantInfo{KnownTypeCode::Int32, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::uint32_t>) {
                if (v == std::numeric_limits<std::uint32_t>::max()) return SpecialConstantInfo{KnownTypeCode::UInt32, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::int64_t>) {
                if (v == std::numeric_limits<std::int64_t>::min()) return SpecialConstantInfo{KnownTypeCode::Int64, "MinValue"};
                if (v == std::numeric_limits<std::int64_t>::max()) return SpecialConstantInfo{KnownTypeCode::Int64, "MaxValue"};
            } else if constexpr (std::is_same_v<T, std::uint64_t>) {
                if (v == std::numeric_limits<std::uint64_t>::max()) return SpecialConstantInfo{KnownTypeCode::UInt64, "MaxValue"};
            } else if constexpr (std::is_same_v<T, float>) {
                if (std::isnan(v)) return SpecialConstantInfo{KnownTypeCode::Single, "NaN"};
                if (v == -std::numeric_limits<float>::infinity()) return SpecialConstantInfo{KnownTypeCode::Single, "NegativeInfinity"};
                if (v == std::numeric_limits<float>::infinity()) return SpecialConstantInfo{KnownTypeCode::Single, "PositiveInfinity"};
                if (v == std::numeric_limits<float>::lowest()) return SpecialConstantInfo{KnownTypeCode::Single, "MinValue"};
                if (v == std::numeric_limits<float>::max()) return SpecialConstantInfo{KnownTypeCode::Single, "MaxValue"};
                if (v == std::numeric_limits<float>::denorm_min()) return SpecialConstantInfo{KnownTypeCode::Single, "Epsilon"};
            } else if constexpr (std::is_same_v<T, double>) {
                if (std::isnan(v)) return SpecialConstantInfo{KnownTypeCode::Double, "NaN"};
                if (v == -std::numeric_limits<double>::infinity()) return SpecialConstantInfo{KnownTypeCode::Double, "NegativeInfinity"};
                if (v == std::numeric_limits<double>::infinity()) return SpecialConstantInfo{KnownTypeCode::Double, "PositiveInfinity"};
                if (v == std::numeric_limits<double>::lowest()) return SpecialConstantInfo{KnownTypeCode::Double, "MinValue"};
                if (v == std::numeric_limits<double>::max()) return SpecialConstantInfo{KnownTypeCode::Double, "MaxValue"};
                if (v == std::numeric_limits<double>::denorm_min()) return SpecialConstantInfo{KnownTypeCode::Double, "Epsilon"};
            } else if constexpr (std::is_same_v<T, util::Decimal>) {
                if (util::Decimal::compare(v, decimal_min_value()) == 0) return SpecialConstantInfo{KnownTypeCode::Decimal, "MinValue"};
                if (util::Decimal::compare(v, decimal_max_value()) == 0) return SpecialConstantInfo{KnownTypeCode::Decimal, "MaxValue"};
            }
            return std::nullopt;
        },
        constant);
}

// ---- floating point literals -----------------------------------------------------------------------------------------------

constexpr int kMaxDenominatorDouble = 1000;
constexpr int kMaxDenominatorFloat = 360;
constexpr float kMathF_PI = 3.14159274f;
constexpr float kMathF_E = 2.71828175f;

bool is_valid_fraction(std::int64_t num, std::int64_t den) {
    if (!(den > 0 && num != 0)) return false;
    if (den == 1 || std::llabs(num) == 1) return true;
    if (!(std::llabs(num) < den)) return false;
    return den % 2 == 0 || den % 3 == 0 || den % 5 == 0;
}

// (the values go through a volatile so that no more precision than the type's is kept, as upstream's `in` parameters do)
bool equal_doubles(double val1, double val2) {
    volatile double a = val1, b = val2;
    return a == b;
}
bool equal_floats(float val1, float val2) {
    volatile float a = val1, b = val2;
    return a == b;
}

bool is_equal(std::int64_t num, std::int64_t den, const ConstantValue& constant_value, bool is_double) {
    if (is_double) return equal_doubles(std::get<double>(constant_value), static_cast<double>(num) / static_cast<double>(den));
    return equal_floats(std::get<float>(constant_value), static_cast<float>(num) / static_cast<float>(den));
}

// Arithmetic of the 64-bit continued fraction terms as C# does it unchecked: wrapping.
std::int64_t wrap_mul(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b)); }
std::int64_t wrap_add(std::int64_t a, std::int64_t b) { return static_cast<std::int64_t>(static_cast<std::uint64_t>(a) + static_cast<std::uint64_t>(b)); }

// A rational approximation of a real number with a bounded denominator (David Eppstein's, from the theory of continued fractions): if
// x = a1 + 1/(a2 + 1/(a3 + ...)) the best approximation is found by truncating the series, with some adjustment in the last term. The
// fraction is kept as the first column of the product of the matrices (a_i 1; 1 0).
std::pair<std::int64_t, std::int64_t> fraction_approx(double value, int max_denominator) {
    // The range check is on the magnitude: the sign is stripped below, so a large negative value would otherwise reach the
    // continued-fraction loop and overflow the terms it accumulates.
    if (std::fabs(value) > 0x7FFFFFFF) return {0, 0};

    const double start_value = value;
    if (value < 0) value = -value;

    std::int64_t ai;
    std::int64_t m[2][2];
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    double v = value;
    for (;;) {
        ai = static_cast<std::int64_t>(v);
        if (!(wrap_add(wrap_mul(m[1][0], ai), m[1][1]) <= max_denominator)) break;
        std::int64_t t = wrap_add(wrap_mul(m[0][0], ai), m[0][1]);
        m[0][1] = m[0][0];
        m[0][0] = t;
        t = wrap_add(wrap_mul(m[1][0], ai), m[1][1]);
        m[1][1] = m[1][0];
        m[1][0] = t;
        if (v - static_cast<double>(ai) == 0) break;
        v = 1 / (v - static_cast<double>(ai));
        // Values greater than long.MaxValue cannot be stored in a fraction without overflow. The conversion of long.MaxValue to double
        // loses precision, so a value strictly greater than it can compare equal: ">=" compensates.
        if (std::fabs(v) >= static_cast<double>(std::numeric_limits<std::int64_t>::max())) break;
    }

    if (m[1][0] == 0) return {0, 0};

    const std::int64_t first_n = m[0][0];
    const std::int64_t first_d = m[1][0];

    ai = (max_denominator - m[1][1]) / m[1][0];
    const std::int64_t second_n = wrap_add(wrap_mul(m[0][0], ai), m[0][1]);
    const std::int64_t second_d = wrap_add(wrap_mul(m[1][0], ai), m[1][1]);

    const double first_delta = std::fabs(value - static_cast<double>(first_n) / static_cast<double>(first_d));
    const double second_delta = std::fabs(value - static_cast<double>(second_n) / static_cast<double>(second_d));

    if (first_delta < second_delta) return {start_value < 0 ? -first_n : first_n, first_d};
    return {start_value < 0 ? -second_n : second_n, second_d};
}

// ---- enum values -----------------------------------------------------------------------------------------------------------

// see https://en.wikipedia.org/wiki/Hamming_weight
int calculate_hamming_weight(std::uint64_t value) {
    constexpr std::uint64_t m1 = 0x5555555555555555;    // binary: 0101...
    constexpr std::uint64_t m2 = 0x3333333333333333;    // binary: 00110011..
    constexpr std::uint64_t m4 = 0x0f0f0f0f0f0f0f0f;    // binary: 4 zeros, 4 ones ...
    constexpr std::uint64_t h01 = 0x0101010101010101;   // the sum of 256 to the power of 0,1,2,3...
    std::uint64_t x = value - ((value >> 1) & m1);       // put count of each 2 bits into those 2 bits
    x = (x & m2) + ((x >> 2) & m2);                      // put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) & m4;                             // put count of each 8 bits into those 8 bits
    return static_cast<int>((x * h01) >> 56);            // the left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
}

struct EnumFieldInfo {
    std::int64_t value;
    const IField* field;
    int weight;
};

// ---- IsDefaultValueAssignmentAllowed and IsDirectImportOf (TypeSystemExtensions, not ported there) -------------------------------

bool default_value_assignment_allowed_individual(const IParameter* parameter) {
    const ReferenceKind kind = parameter->reference_kind();
    return parameter->is_optional() && parameter->has_constant_value_in_signature() &&
           (kind == ReferenceKind::None || kind == ReferenceKind::In || kind == ReferenceKind::RefReadOnly);
}

// Whether the parameter may be given a default value: it is optional, has a constant value in its signature, is passed by value or
// `in`, and so are all the following parameters (those that are optional, `params` or a closure parameter of a local function).
bool is_default_value_assignment_allowed(const IParameter* parameter) {
    if (!default_value_assignment_allowed_individual(parameter)) return false;
    const IParameterizedMember* owner = parameter->owner();
    if (owner == nullptr) return true;   // Shouldn't happen, but we need to check for it.
    const auto& parameters = owner->parameters();
    for (int i = static_cast<int>(parameters.size()) - 1; i >= 0; i--) {
        const IParameter* other = parameters[static_cast<std::size_t>(i)];
        if (other == parameter) break;
        if (il::transforms::LocalFunctionDecompiler::is_closure_parameter(other, other->owner()->declaring_type_definition())) continue;
        if (default_value_assignment_allowed_individual(other) || other->is_params()) continue;
        return false;
    }
    return true;
}

// Whether the module that declares `type` is directly referenced by `module`.
bool is_direct_import_of(const ITypeDefinition* type, const IModule* module) {
    const IModule* module_reference = type->parent_module();
    const metadata::MetadataFile* file = module->metadata_file();
    const srm::MetadataReader& md = file->metadata();
    for (const srm::AssemblyReferenceHandle handle : md.assembly_references()) {
        const metadata::AssemblyReference asm_ref(md, handle);
        if (asm_ref.full_name() == module_reference->full_assembly_name()) return true;
        if (asm_ref.name() == "netstandard" && asm_ref.public_key_token().has_value()) {
            const IModule* referenced_module = find_module_by_reference(module->compilation(), asm_ref);
            if (referenced_module != nullptr && !referenced_module->metadata_file()->get_type_forwarder(type->full_type_name()).is_nil()) return true;
        }
    }
    return false;
}

// ---- type visitors ---------------------------------------------------------------------------------------------------------

class BaseListNameabilityVisitor final : public TypeVisitor {
public:
    BaseListNameabilityVisitor(const ITypeDefinition* current_type, const MemberLookup& lookup) : current_type_(current_type), lookup_(lookup) {}
    bool all_nameable = true;

    const IType* visit_type_definition(const ITypeDefinition& type) override {
        all_nameable = all_nameable && type_definition_nameable_in_base_list(&type);
        return TypeVisitor::visit_type_definition(type);
    }

private:
    bool type_definition_nameable_in_base_list(const ITypeDefinition* td) const {
        if (td == nullptr) return true;
        // A type may name its own nested types (and those of its enclosing types) in its base list regardless of accessibility, e.g.
        // 'class F : F.IFoo'.
        for (const ITypeDefinition* t = current_type_; t != nullptr; t = t->declaring_type_definition()) {
            const ITypeDefinition* outer = td->declaring_type_definition();
            if (outer != nullptr && type_equals(outer, t)) return true;
        }
        // Everything else resolves in the enclosing scope: 'class SubF : F, F.IFoo' is CS0122 even though F.IFoo is accessible inside
        // SubF's body. Protected access through the enclosing types' inheritance chains remains available, which MemberLookup grants
        // for type definitions regardless of allowProtectedAccess.
        if (!lookup_.is_accessible(td, false)) return false;
        // Naming 'A.I' also requires 'A' to be nameable.
        return type_definition_nameable_in_base_list(td->declaring_type_definition());
    }

    const ITypeDefinition* current_type_;
    const MemberLookup& lookup_;
};

// Collects the type parameters of one method that appear with a nullable annotation ('T?') anywhere in a visited type, including nested
// positions such as List<T?> or T?[]. Type parameters of any other owner are ignored: a specialized signature can substitute a foreign
// type parameter that happens to share an index with one of this method's own.
class NullableTypeParameterCollector final : public TypeVisitor {
public:
    explicit NullableTypeParameterCollector(const std::vector<const ITypeParameter*>& type_parameters) : type_parameters_(type_parameters) {}
    std::vector<const ITypeParameter*> nullable_type_parameters;

    const IType* visit_nullability_annotated_type(const NullabilityAnnotatedType& type) override {
        const auto* natp = dynamic_cast<const NullabilityAnnotatedTypeParameter*>(&type);
        if (natp != nullptr && natp->nullability() == Nullability::Nullable) {
            const ITypeParameter* original = natp->original_type_parameter();
            const bool belongs = std::any_of(type_parameters_.begin(), type_parameters_.end(),
                                             [&](const ITypeParameter* tp) { return type_equals(tp, original); });
            const bool known = std::any_of(nullable_type_parameters.begin(), nullable_type_parameters.end(),
                                           [&](const ITypeParameter* tp) { return type_equals(tp, original); });
            if (belongs && !known) nullable_type_parameters.push_back(original);
        }
        return TypeVisitor::visit_nullability_annotated_type(type);
    }

    bool contains(const ITypeParameter* tp) const {
        return std::any_of(nullable_type_parameters.begin(), nullable_type_parameters.end(),
                           [&](const ITypeParameter* other) { return type_equals(other, tp); });
    }

private:
    const std::vector<const ITypeParameter*>& type_parameters_;
};

// Returns the constraint that keeps 'T?' meaning a nullable annotation on an override or explicit interface implementation, or nullopt
// where the type parameter neither needs nor permits one.
std::optional<std::string> get_nullability_disambiguator(const ITypeParameter* tp) {
    const std::optional<bool> is_reference = tp->is_reference_type();
    if (!is_reference.has_value()) return std::string("default");   // constrained to neither a reference type nor a value type
    if (*is_reference) return std::string("class");                 // C# accepts only plain 'class' here, never 'class?'
    return std::nullopt;                                           // a value type uses Nullable<T> rather than a nullable annotation
}

// MergeReadOnlyModifiers: when the accessors are readonly the declaration is.
void merge_read_only_modifiers(EntityDeclaration* decl, Accessor* accessor1, Accessor* accessor2) {
    if (accessor1 == nullptr) return;
    if (accessor1->has_modifier(Modifiers::Readonly) && accessor2 == nullptr) {
        accessor1->set_modifiers(accessor1->modifiers() & ~Modifiers::Readonly);
        decl->set_modifiers(decl->modifiers() | Modifiers::Readonly);
    } else if (accessor1->has_modifier(Modifiers::Readonly) && accessor2->has_modifier(Modifiers::Readonly)) {
        accessor1->set_modifiers(accessor1->modifiers() & ~Modifiers::Readonly);
        accessor2->set_modifiers(accessor2->modifiers() & ~Modifiers::Readonly);
        decl->set_modifiers(decl->modifiers() | Modifiers::Readonly);
    }
}

std::string calling_convention_name(srm::SignatureCallingConvention convention) {
    switch (convention) {
    case srm::SignatureCallingConvention::CDecl: return "Cdecl";
    case srm::SignatureCallingConvention::StdCall: return "Stdcall";
    case srm::SignatureCallingConvention::ThisCall: return "Thiscall";
    case srm::SignatureCallingConvention::FastCall: return "Fastcall";
    case srm::SignatureCallingConvention::VarArgs: return "Varargs";
    default: return std::to_string(static_cast<int>(convention));   // (an enum value without a name is written as its number)
    }
}

}  // namespace

// =============================================================================================================================
// Constructor
// =============================================================================================================================

TypeSystemAstBuilder::TypeSystemAstBuilder(const CSharpResolver* resolver) : resolver_(resolver) {
    if (resolver == nullptr) throw std::invalid_argument("resolver");
}

TypeSystemAstBuilder::TypeSystemAstBuilder() = default;

// =============================================================================================================================
// Convert Type
// =============================================================================================================================

AstType* TypeSystemAstBuilder::convert_type(const IType* type) {
    if (type == nullptr) throw std::invalid_argument("type");
    AstType* ast_type = convert_type_helper(type);
    add_type_annotation(ast_type, type);
    return ast_type;
}

void TypeSystemAstBuilder::add_type_annotation(AstType* ast_type, const IType* type) {
    if (add_resolve_result_annotations) ast_type->add_annotation(make<TypeResolveResult>(type));
}

AstType* TypeSystemAstBuilder::convert_type(const FullTypeName& full_type_name) {
    if (resolver_ != nullptr) {
        for (const IModule* asm_ : resolver_->compilation().modules()) {
            const ITypeDefinition* def = get_type_definition(*asm_, full_type_name);
            if (def != nullptr) return convert_type(def);
        }
    }
    const TopLevelTypeName& top = full_type_name.top_level_type_name();
    AstType* type;
    if (top.namespace_().empty()) {
        type = make_simple_type(top.name());
    } else {
        type = make_member_type(make_simple_type(top.namespace_()), top.name());
    }
    for (int i = 0; i < full_type_name.nesting_level(); i++) {
        type = make_member_type(type, full_type_name.get_nested_type_name(i));
    }
    return type;
}

AstType* TypeSystemAstBuilder::convert_type_helper(const IType* type) {
    if (const auto* with_element = dynamic_cast<const TypeWithElementType*>(type)) {
        if (dynamic_cast<const PointerType*>(with_element) != nullptr) {
            return convert_type(with_element->element_type())->make_pointer_type();
        } else if (const auto* array = dynamic_cast<const ArrayType*>(with_element)) {
            AstType* ast_type = convert_type(with_element->element_type())->make_array_type(array->dimensions());
            if (type->nullability() == Nullability::Nullable) return ast_type->make_nullable_type();
            return ast_type;
        } else if (dynamic_cast<const ByReferenceType*>(with_element) != nullptr) {
            return convert_type(with_element->element_type())->make_ref_type();
        } else {
            // not supported as type in C#
            return convert_type(with_element->element_type());
        }
    } else if (const auto* nat = dynamic_cast<const NullabilityAnnotatedType*>(type)) {
        AstType* ast_type = convert_type(nat->type_without_annotation());
        if (nat->nullability() == Nullability::Nullable) ast_type = ast_type->make_nullable_type();
        return ast_type;
    } else if (const auto* tuple = dynamic_cast<const TupleType*>(type)) {
        auto* ast_type = make<TupleAstType>();
        const std::size_t count = std::min(tuple->element_types().size(), tuple->element_names().size());
        for (std::size_t i = 0; i < count; i++) {
            const std::optional<std::string>& ename = tuple->element_names()[i];
            auto* element = make<TupleTypeElement>();
            element->set_type(convert_type(tuple->element_types()[i]));
            element->set_name(ename.value_or(std::string()));
            ast_type->elements().add(element);
        }
        return ast_type;
    } else if (const auto* fpt = dynamic_cast<const FunctionPointerType*>(type)) {
        auto* ast_type = make<FunctionPointerAstType>();
        if (fpt->calling_convention == srm::SignatureCallingConvention::Unmanaged) {
            ast_type->set_has_unmanaged_calling_convention(true);
        } else if (fpt->calling_convention != srm::SignatureCallingConvention::Default) {
            const std::string callconv_name = calling_convention_name(fpt->calling_convention);
            ast_type->set_has_unmanaged_calling_convention(true);
            ast_type->calling_conventions().add(make<PrimitiveType>(callconv_name));
        }
        for (const IType* custom_call_conv : fpt->custom_calling_conventions) {
            AstType* call_conv_syntax;
            const std::string name = custom_call_conv->name();
            if (custom_call_conv->namespace_() == "System.Runtime.CompilerServices" && name.rfind("CallConv", 0) == 0 && name.size() > 8) {
                call_conv_syntax = make<PrimitiveType>(name.substr(8));
                if (add_resolve_result_annotations) call_conv_syntax->add_annotation(make<TypeResolveResult>(custom_call_conv));
            } else {
                call_conv_syntax = convert_type(custom_call_conv);
            }
            ast_type->calling_conventions().add(call_conv_syntax);
        }
        for (std::size_t i = 0; i < fpt->parameter_types.size(); i++) {
            auto* param_decl = make<ParameterDeclaration>();
            param_decl->set_parameter_modifier(fpt->parameter_reference_kinds[i]);
            const IType* parameter_type = fpt->parameter_types[i];
            const auto* brt = dynamic_cast<const ByReferenceType*>(parameter_type);
            if (param_decl->parameter_modifier() != ReferenceKind::None && brt != nullptr) {
                param_decl->set_type(convert_type(brt->element_type()));
            } else {
                param_decl->set_type(convert_type(parameter_type));
            }
            ast_type->parameters().add(param_decl);
        }
        ast_type->set_return_type(convert_type(fpt->return_type));
        if (fpt->return_is_ref_read_only) {
            auto* ct = as<ComposedType>(ast_type->return_type());
            if (ct != nullptr && ct->has_ref_specifier()) ct->set_has_read_only_specifier(true);
        }
        const ITypeDefinition* treated_as = fpt->get_definition();
        if (treated_as != nullptr) {
            AstType* result = convert_type_helper(treated_as);
            result->add_trailing_trivia(make<Comment>(ast_type->to_string(), CommentType::MultiLine));
            return result;
        }
        return ast_type;
    } else {
        AstType* ast_type;
        if (dynamic_cast<const ITypeDefinition*>(type) != nullptr || dynamic_cast<const UnknownType*>(type) != nullptr) {
            if (is_unbound(type)) {
                if (show_type_parameters_for_unbound_types) {
                    ast_type = convert_type_helper(type, type->type_arguments());
                } else {
                    std::vector<const IType*> type_arguments(static_cast<std::size_t>(type->type_parameter_count()), &SpecialType::unbound_type_argument());
                    ast_type = convert_type_helper(type, type_arguments);
                }
            } else {
                ast_type = convert_type_helper(type, {});
            }
        } else if (const auto* pt = dynamic_cast<const ParameterizedType*>(type)) {
            if (use_nullable_specifier_for_value_types && is_known_type(pt, KnownTypeCode::NullableOfT)) {
                return convert_type(pt->type_arguments()[0])->make_nullable_type();
            }
            ast_type = convert_type_helper(pt->generic_type(), pt->type_arguments());
        } else {
            switch (type->kind()) {
            case TypeKind::Dynamic:
            case TypeKind::NInt:
            case TypeKind::NUInt: ast_type = make<PrimitiveType>(type->name()); break;
            default: ast_type = make_simple_type(type->name()); break;
            }
        }
        if (type->nullability() == Nullability::Nullable) {
            add_type_annotation(ast_type, type->change_nullability(Nullability::Oblivious));
            ast_type = ast_type->make_nullable_type();
        }
        return ast_type;
    }
}

AstType* TypeSystemAstBuilder::convert_type_helper(const IType* generic_type, const std::vector<const IType*>& type_arguments) {
    const ITypeDefinition* type_def = generic_type->get_definition();
    assert(type_def != nullptr || generic_type->kind() == TypeKind::Unknown);
    assert(static_cast<int>(type_arguments.size()) >= generic_type->type_parameter_count());

    if (use_keywords_for_builtin_types && type_def != nullptr) {
        const std::optional<std::string> keyword = KnownTypeReference::get_csharp_name_by_type_code(type_def->known_type_code());
        if (keyword.has_value()) return make<PrimitiveType>(*keyword);
    }

    // The number of type parameters belonging to outer classes
    const IType* declaring = generic_type->declaring_type();
    const int outer_type_parameter_count = declaring != nullptr ? declaring->type_parameter_count() : 0;

    if (resolver_ != nullptr && type_def != nullptr) {
        // Look if there's an alias to the target type
        if (use_aliases) {
            for (const resolver::UsingScope* using_scope = resolver_->current_using_scope(); using_scope != nullptr; using_scope = using_scope->parent()) {
                for (const auto& pair : using_scope->using_aliases()) {
                    if (dynamic_cast<TypeResolveResult*>(pair.second) != nullptr) {
                        if (type_matches(pair.second->type(), type_def, type_arguments)) return make_simple_type(pair.first);
                    }
                }
            }
        }

        std::vector<const IType*> local_type_arguments;
        if (type_def->type_parameter_count() > outer_type_parameter_count) {
            local_type_arguments.resize(static_cast<std::size_t>(type_def->type_parameter_count() - outer_type_parameter_count));
            for (std::size_t i = 0; i < local_type_arguments.size(); i++) {
                local_type_arguments[i] = type_arguments[static_cast<std::size_t>(outer_type_parameter_count) + i];
            }
        }
        ResolveResult* rr = resolver_->lookup_simple_name_or_type_name(type_def->name(), local_type_arguments, name_lookup_mode);
        TypeResolveResult* trr = dynamic_cast<TypeResolveResult*>(rr);
        if (trr != nullptr || (local_type_arguments.empty() && resolver_->is_variable_reference_with_same_type(rr, type_def->name(), &trr))) {
            if (!trr->is_error() && type_matches(trr->type(), type_def, type_arguments)) {
                // We can use the short type name
                SimpleType* short_result = make_simple_type(type_def->name());
                add_type_arguments(short_result, type_def->type_parameters(), type_arguments, outer_type_parameter_count, type_def->type_parameter_count());
                return short_result;
            }
        }
    }

    if (always_use_short_type_names || (type_def == nullptr && generic_type->declaring_type() == nullptr)) {
        SimpleType* short_result = make_simple_type(generic_type->name());
        add_type_arguments(short_result, generic_type->type_parameters(), type_arguments, outer_type_parameter_count, generic_type->type_parameter_count());
        return short_result;
    }
    auto* result = make<MemberType>();
    if (generic_type->declaring_type() != nullptr) {
        // Handle nested types
        result->set_target(convert_type_helper(generic_type->declaring_type(), type_arguments));
        // Use correct number of type arguments on the declaring type
        const IType* declaring_type = generic_type->declaring_type();
        if (outer_type_parameter_count > 0) {
            declaring_type = make_parameterized_type(
                generic_type->declaring_type(),
                std::vector<const IType*>(type_arguments.begin(), type_arguments.begin() + outer_type_parameter_count));
        }
        add_type_annotation(result->target(), declaring_type);
    } else {
        // Handle top-level types
        if (generic_type->namespace_().empty()) {
            result->set_target(make_global());
            result->set_is_double_colon(true);
        } else {
            NamespaceResolveResult* ignored = nullptr;
            result->set_target(convert_namespace(generic_type->namespace_(), &ignored, always_use_global || generic_type->namespace_() == generic_type->name()));
        }
    }
    result->set_member_name(generic_type->name());
    add_type_arguments(result, generic_type->type_parameters(), type_arguments, outer_type_parameter_count, generic_type->type_parameter_count());
    return result;
}

// Whether 'type' is the same as 'type_def' parameterized with the given type arguments.
bool TypeSystemAstBuilder::type_matches(const IType* type, const ITypeDefinition* type_def, const std::vector<const IType*>& type_arguments) {
    if (type_def->type_parameter_count() == 0) {
        return type_def_matches(type_def, type);
    } else {
        if (!type_def_matches(type_def, type->get_definition())) return false;
        const auto* pt = dynamic_cast<const ParameterizedType*>(type);
        if (pt == nullptr) {
            return std::all_of(type_arguments.begin(), type_arguments.end(), [](const IType* t) { return t->kind() == TypeKind::UnboundTypeArgument; });
        }
        const auto& ta = pt->type_arguments();
        for (std::size_t i = 0; i < ta.size(); i++) {
            if (!type_equals(ta[i], type_arguments[i])) return false;
        }
        return true;
    }
}

bool TypeSystemAstBuilder::type_def_matches(const ITypeDefinition* type_def, const IType* type) {
    if (type == nullptr || type->name() != type_def->name() || type->namespace_() != type_def->namespace_() ||
        type->type_parameter_count() != type_def->type_parameter_count())
        return false;
    const bool def_is_nested = type_def->declaring_type_definition() != nullptr;
    const bool type_is_nested = type->declaring_type() != nullptr;
    if (def_is_nested && type_is_nested) return type_def_matches(type_def->declaring_type_definition(), type->declaring_type());
    return def_is_nested == type_is_nested;
}

// Adds type arguments start_index .. end_index-1 to the result type (a SimpleType or MemberType).
void TypeSystemAstBuilder::add_type_arguments(AstType* result, const std::vector<const ITypeParameter*>& type_parameters,
                                              const std::vector<const IType*>& type_arguments, int start_index, int end_index) {
    assert(end_index <= static_cast<int>(type_parameters.size()));
    for (int i = start_index; i < end_index; i++) {
        const auto index = static_cast<std::size_t>(i);
        if (convert_unbound_type_arguments && type_arguments[index]->kind() == TypeKind::UnboundTypeArgument) {
            result->add_child(make_simple_type(type_parameters[index]->name()), Slots::type_argument);
        } else {
            result->add_child(convert_type(type_arguments[index]), Slots::type_argument);
        }
    }
}

AstType* TypeSystemAstBuilder::convert_namespace(const std::string& namespace_name, NamespaceResolveResult** nrr) {
    return convert_namespace(namespace_name, nrr, false);
}

AstType* TypeSystemAstBuilder::convert_namespace(const std::string& namespace_name, NamespaceResolveResult** nrr, bool requires_global_prefix) {
    if (nrr != nullptr) *nrr = nullptr;
    if (resolver_ != nullptr) {
        // Look if there's an alias to the target namespace
        if (use_aliases) {
            for (const resolver::UsingScope* using_scope = resolver_->current_using_scope(); using_scope != nullptr; using_scope = using_scope->parent()) {
                for (const auto& pair : using_scope->using_aliases()) {
                    auto* alias_nrr = dynamic_cast<NamespaceResolveResult*>(pair.second);
                    if (nrr != nullptr) *nrr = alias_nrr;
                    if (alias_nrr != nullptr && alias_nrr->namespace_name() == namespace_name) {
                        SimpleType* ns = make_simple_type(pair.first);
                        if (add_resolve_result_annotations) ns->add_annotation(alias_nrr);
                        return ns;
                    }
                }
            }
        }
    }

    const std::size_t pos = namespace_name.rfind('.');
    if (pos == std::string::npos) {
        if (is_valid_namespace(namespace_name, nrr)) {
            AstType* ns;
            if (requires_global_prefix) {
                auto* member = make<MemberType>();
                member->set_target(make_global());
                member->set_is_double_colon(true);
                member->set_member_name(namespace_name);
                ns = member;
            } else {
                ns = make_simple_type(namespace_name);
            }
            if (add_resolve_result_annotations && nrr != nullptr && *nrr != nullptr) ns->add_annotation(*nrr);
            return ns;
        } else {
            auto* ns = make<MemberType>();
            ns->set_target(make_global());
            ns->set_is_double_colon(true);
            ns->set_member_name(namespace_name);
            if (add_resolve_result_annotations) {
                const INamespace* ns_entity = resolver_->compilation().root_namespace().get_child_namespace(namespace_name);
                if (ns_entity != nullptr) {
                    auto* result = make<NamespaceResolveResult>(ns_entity);
                    if (nrr != nullptr) *nrr = result;
                    ns->add_annotation(result);
                }
            }
            return ns;
        }
    } else {
        const std::string parent_namespace = namespace_name.substr(0, pos);
        const std::string local_namespace = namespace_name.substr(pos + 1);
        NamespaceResolveResult* parent_nrr = nullptr;
        AstType* parent_ns = convert_namespace(parent_namespace, &parent_nrr, requires_global_prefix);
        auto* ns = make<MemberType>();
        ns->set_target(parent_ns);
        ns->set_member_name(local_namespace);
        if (nrr != nullptr) *nrr = nullptr;
        if (add_resolve_result_annotations && parent_nrr != nullptr) {
            const INamespace* new_namespace = parent_nrr->namespace_()->get_child_namespace(local_namespace);
            if (new_namespace != nullptr) {
                auto* result = make<NamespaceResolveResult>(new_namespace);
                if (nrr != nullptr) *nrr = result;
                ns->add_annotation(result);
            }
        }
        return ns;
    }
}

bool TypeSystemAstBuilder::is_valid_namespace(const std::string& first_namespace_part, NamespaceResolveResult** nrr) {
    if (nrr != nullptr) *nrr = nullptr;
    if (resolver_ == nullptr) return true;   // just assume namespaces are valid if we don't have a resolver
    auto* result = dynamic_cast<NamespaceResolveResult*>(resolver_->resolve_simple_name(first_namespace_part, {}));
    if (nrr != nullptr) *nrr = result;
    return result != nullptr && !result->is_error() && result->namespace_name() == first_namespace_part;
}

SimpleType* TypeSystemAstBuilder::make_global() {
    auto* global = make<SimpleType>("global");
    if (add_resolve_result_annotations && resolver_ != nullptr) global->add_annotation(make<NamespaceResolveResult>(&resolver_->compilation().root_namespace()));
    return global;
}

// =============================================================================================================================
// Convert Attribute
// =============================================================================================================================

Attribute* TypeSystemAstBuilder::convert_attribute(const IAttribute* attribute) {
    auto* attr = make<Attribute>();
    attr->set_type(convert_attribute_type(attribute->attribute_type()));
    if (auto* st = as<SimpleType>(attr->type())) {
        const std::string id = st->identifier();
        if (!id.empty() && ends_with(id, "Attribute")) st->set_identifier(id.substr(0, id.size() - 9));
    } else if (auto* mt = as<MemberType>(attr->type())) {
        if (ends_with(mt->member_name(), "Attribute")) mt->set_member_name(mt->member_name().substr(0, mt->member_name().size() - 9));
    }
    if (add_resolve_result_annotations && attribute->constructor() != nullptr) {
        attr->add_annotation(make<MemberResolveResult>(nullptr, attribute->constructor()));
    }
    static const std::vector<const IParameter*> no_parameters;
    const std::vector<const IParameter*>& parameters = attribute->constructor() != nullptr ? attribute->constructor()->parameters() : no_parameters;
    const auto& fixed_arguments = attribute->fixed_arguments();
    for (std::size_t i = 0; i < fixed_arguments.size(); i++) {
        const AttributeTypedArgument& arg = fixed_arguments[i];
        const IParameter* p = i < parameters.size() ? parameters[i] : nullptr;
        attr->arguments().add(convert_attribute_argument_value(p != nullptr ? p->type() : arg.type, arg.type, arg.value));
    }
    if (!attribute->named_arguments().empty()) {
        auto* target_result = make<InitializedObjectResolveResult>(attribute->attribute_type());
        for (const AttributeNamedArgument& named_arg : attribute->named_arguments()) {
            const std::string name = named_arg.name.value_or(std::string());
            auto* named_argument = make<NamedExpression>(name, convert_attribute_argument_value(named_arg.type, named_arg.type, named_arg.value));
            if (add_resolve_result_annotations) {
                const IMember* member = CustomAttribute::member_for_named_argument(attribute->attribute_type(), named_arg);
                if (member != nullptr) named_argument->add_annotation(make<MemberResolveResult>(target_result, member));
            }
            attr->arguments().add(named_argument);
        }
    }
    if (attribute->has_decode_errors()) {
        attr->set_has_argument_list(true);
        // An ErrorExpression renders purely as its comment, so it appears inside the parentheses without an explicit closing-paren
        // token to anchor a comment child.
        attr->arguments().add(make<ErrorExpression>("Could not decode attribute arguments."));
    }
    return attr;
}

std::vector<AttributeSection*> TypeSystemAstBuilder::convert_attributes(const std::vector<const IAttribute*>& attributes_in, const std::string& target) {
    std::vector<const IAttribute*> attributes = attributes_in;
    if (sort_attributes) {
        auto compare_type = [](const IType* a, const IType* b) { return culture_compare(a->full_name(), b->full_name()); };
        auto compare_attribute = [&](const IAttribute* a, const IAttribute* b) -> int {
            int result = compare_type(a->attribute_type(), b->attribute_type());
            if (result != 0) return result;
            if (a->has_decode_errors() && b->has_decode_errors()) return 0;
            if (a->has_decode_errors()) return -1;
            if (b->has_decode_errors()) return 1;
            result = static_cast<int>(a->fixed_arguments().size()) - static_cast<int>(b->fixed_arguments().size());
            if (result != 0) return result;
            for (std::size_t i = 0; i < a->fixed_arguments().size(); i++) {
                const auto& arg_a = a->fixed_arguments()[i];
                const auto& arg_b = b->fixed_arguments()[i];
                result = compare_type(arg_a.type, arg_b.type);
                if (result != 0) return result;
                result = compare_argument_values(arg_a.value, arg_b.value);
                if (result != 0) return result;
            }
            result = static_cast<int>(a->named_arguments().size()) - static_cast<int>(b->named_arguments().size());
            if (result != 0) return result;
            for (std::size_t i = 0; i < a->named_arguments().size(); i++) {
                const auto& arg_a = a->named_arguments()[i];
                const auto& arg_b = b->named_arguments()[i];
                result = culture_compare(arg_a.name.value_or(std::string()), arg_b.name.value_or(std::string()));
                if (result != 0) return result;
                result = compare_type(arg_a.type, arg_b.type);
                if (result != 0) return result;
                result = compare_argument_values(arg_a.value, arg_b.value);
                if (result != 0) return result;
            }
            return 0;
        };
        std::stable_sort(attributes.begin(), attributes.end(), [&](const IAttribute* a, const IAttribute* b) { return compare_attribute(a, b) < 0; });
    }
    std::vector<AttributeSection*> sections;
    sections.reserve(attributes.size());
    for (const IAttribute* a : attributes) {
        auto* section = make<AttributeSection>(convert_attribute(a));
        if (!target.empty()) section->set_attribute_target(target);
        sections.push_back(section);
    }
    return sections;
}

// =============================================================================================================================
// Convert Attribute Type
// =============================================================================================================================

AstType* TypeSystemAstBuilder::convert_attribute_type(const IType* type) {
    if (type == nullptr) throw std::invalid_argument("type");
    AstType* ast_type = convert_type_helper(type);

    std::optional<std::string> short_name;
    if (type->name().size() > 9 && ends_with(type->name(), "Attribute")) short_name = type->name().substr(0, type->name().size() - 9);
    if (always_use_short_type_names) {
        if (auto* st = as<SimpleType>(ast_type)) {
            st->set_identifier(short_name.value_or(std::string()));
        } else if (auto* mt = as<MemberType>(ast_type)) {
            mt->set_member_name(short_name.value_or(std::string()));
        }
    } else if (resolver_ != nullptr) {
        apply_short_attribute_name_if_possible(type, ast_type, short_name);
    }
    add_type_annotation(ast_type, type);

    return ast_type;
}

void TypeSystemAstBuilder::apply_short_attribute_name_if_possible(const IType* type, AstType* ast_type, const std::optional<std::string>& short_name) {
    if (auto* st = as<SimpleType>(ast_type)) {
        ResolveResult* short_rr = nullptr;
        ResolveResult* with_extra_attr_suffix = resolver_->lookup_simple_name_or_type_name(type->name() + "Attribute", {}, NameLookupMode::Type);
        if (short_name.has_value()) short_rr = resolver_->lookup_simple_name_or_type_name(*short_name, {}, NameLookupMode::Type);
        // short type is either unknown or not an attribute type -> we can use the short name.
        if (short_rr != nullptr && (dynamic_cast<UnknownIdentifierResolveResult*>(short_rr) != nullptr || !is_attribute_type(short_rr))) {
            st->set_identifier(*short_name);
        } else if (is_attribute_type(with_extra_attr_suffix)) {
            // typeName + "Attribute" is an attribute type -> we cannot use long type name, add '@' to disable implicit "Attribute" suffix.
            st->set_identifier("@" + st->identifier());
        }
    } else if (auto* mt = as<MemberType>(ast_type)) {
        if (type->declaring_type() != nullptr) {
            const ITypeDefinition* declaring_type_def = type->declaring_type()->get_definition();
            if (declaring_type_def != nullptr) {
                auto has_attribute_nested_type = [&](const std::string& name) {
                    const auto nested = declaring_type_def->get_nested_types(
                        [&](const ITypeDefinition* t) { return t->type_parameter_count() == 0 && t->name() == name; });
                    return std::any_of(nested.begin(), nested.end(), [](const IType* t) { return is_attribute_type(t); });
                };
                if (short_name.has_value() && !has_attribute_nested_type(*short_name)) {
                    mt->set_member_name(*short_name);
                } else if (has_attribute_nested_type(type->name() + "Attribute")) {
                    mt->set_member_name("@" + mt->member_name());
                }
            }
        } else if (auto* nrr = dynamic_cast<NamespaceResolveResult*>(get_resolve_result(mt->target()))) {
            if (short_name.has_value() && !is_attribute_type(nrr->namespace_()->get_type_definition(*short_name, 0))) {
                mt->set_member_name(*short_name);
            } else if (is_attribute_type(nrr->namespace_()->get_type_definition(type->name() + "Attribute", 0))) {
                mt->set_member_name("@" + mt->member_name());
            }
        }
    }
}

bool TypeSystemAstBuilder::is_attribute_type(const IType* type) {
    if (type == nullptr) return false;
    for (const IType* t : get_non_interface_base_types(type)) {
        if (is_known_type(t, KnownTypeCode::Attribute)) return true;
    }
    return false;
}

bool TypeSystemAstBuilder::is_attribute_type(ResolveResult* rr) {
    auto* trr = dynamic_cast<TypeResolveResult*>(rr);
    return trr != nullptr && is_attribute_type(trr->type());
}

// =============================================================================================================================
// Convert Constant Value
// =============================================================================================================================

Expression* TypeSystemAstBuilder::convert_constant_value(ResolveResult* rr) {
    if (rr == nullptr) throw std::invalid_argument("rr");
    bool is_boxing = false;
    if (auto* crr = dynamic_cast<ConversionResolveResult*>(rr)) {
        // unpack ConversionResolveResult if necessary (e.g. a boxing conversion or string->object reference conversion)
        rr = crr->input;
        is_boxing = crr->conversion->is_boxing_conversion();
    }

    if (auto* type_of = dynamic_cast<TypeOfResolveResult*>(rr)) {
        auto* expr = make<TypeOfExpression>(convert_type(type_of->referenced_type()));
        if (add_resolve_result_annotations) expr->add_annotation(rr);
        return expr;
    } else if (auto* acrr = dynamic_cast<ArrayCreateResolveResult*>(rr)) {
        auto* ace = make<ArrayCreateExpression>();
        ace->set_type(convert_type(acrr->type()));
        if (auto* composed_type = as<ComposedType>(ace->type())) {
            composed_type->array_specifiers().move_to(ace->additional_array_specifiers());
            if (!composed_type->has_nullable_specifier() && composed_type->pointer_rank() == 0) ace->set_type(composed_type->base_type());
        }

        if (!acrr->size_arguments.empty() && !acrr->initializer_elements.has_value()) {
            if (ArraySpecifier* first = ace->additional_array_specifiers().first_or_null()) first->remove();
            for (ResolveResult* size_argument : acrr->size_arguments) ace->arguments().add(convert_constant_value(size_argument));
        }
        if (acrr->initializer_elements.has_value()) {
            auto* initializer = make<ArrayInitializerExpression>();
            for (ResolveResult* element : *acrr->initializer_elements) initializer->elements().add(convert_constant_value(element));
            ace->set_initializer(initializer);
        }
        if (add_resolve_result_annotations) ace->add_annotation(rr);
        return ace;
    } else if (rr->is_compile_time_constant()) {
        Expression* expr = convert_constant_value(rr->type(), rr->constant_value());
        if (is_boxing && (is_csharp_small_integer_type(*rr->type()) || is_csharp_native_integer_type(*rr->type()))) {
            // C# does not have small integer literal types. We need to add a cast so that the integer literal gets boxed as the
            // correct type.
            expr = make<CastExpression>(convert_type(rr->type()), expr);
            if (add_resolve_result_annotations) expr->add_annotation(rr);
        }
        return expr;
    } else {
        return make<ErrorExpression>();
    }
}

Expression* TypeSystemAstBuilder::convert_constant_value(const IType* type, const ConstantValue& constant_value) {
    return convert_constant_value(type, type, constant_value);
}

Expression* TypeSystemAstBuilder::convert_attribute_argument_value(const IType* expected_type, const IType* type, const AttributeArgumentValue& value) {
    if (type == nullptr) throw std::invalid_argument("type");
    // (a null value, a type and an array are the cases a constant cannot be)
    if (std::holds_alternative<std::monostate>(value)) return convert_constant_value(expected_type, type, ConstantValue());
    if (const auto* typeof_type = std::get_if<const IType*>(&value)) {
        if (*typeof_type == nullptr) return convert_constant_value(expected_type, type, ConstantValue());
        auto* expr = make<TypeOfExpression>(convert_type(*typeof_type));
        if (add_resolve_result_annotations) expr->add_annotation(make<TypeOfResolveResult>(type, *typeof_type));
        return expr;
    }
    if (const auto* arr = std::get_if<std::vector<AttributeTypedArgument>>(&value)) {
        const auto* array_type = dynamic_cast<const ArrayType*>(type);
        const IType* element_type = array_type != nullptr ? array_type->element_type() : static_cast<const IType*>(&SpecialType::unknown_type());
        auto* expr = make<ArrayCreateExpression>();
        expr->set_type(convert_type(type));
        if (auto* composed_type = as<ComposedType>(expr->type())) {
            composed_type->array_specifiers().move_to(expr->additional_array_specifiers());
            if (!composed_type->has_nullable_specifier() && composed_type->pointer_rank() == 0) expr->set_type(composed_type->base_type());
        }
        auto* initializer = make<ArrayInitializerExpression>();
        for (const AttributeTypedArgument& e : *arr) initializer->elements().add(convert_attribute_argument_value(element_type, e.type, e.value));
        expr->set_initializer(initializer);
        return expr;
    }
    if (const auto* boxed = std::get_if<std::shared_ptr<const srm::CustomAttributeTypedArgument<const IType*>>>(&value)) {
        // (a boxed argument: the value with the type it carries)
        if (*boxed == nullptr) return convert_constant_value(expected_type, type, ConstantValue());
        return convert_attribute_argument_value(expected_type, (*boxed)->type, (*boxed)->value);
    }
    ConstantValue constant;
    std::visit(
        [&](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, char16_t> || std::is_same_v<T, std::int8_t> || std::is_same_v<T, std::uint8_t> ||
                          std::is_same_v<T, std::int16_t> || std::is_same_v<T, std::uint16_t> || std::is_same_v<T, std::int32_t> ||
                          std::is_same_v<T, std::uint32_t> || std::is_same_v<T, std::int64_t> || std::is_same_v<T, std::uint64_t> ||
                          std::is_same_v<T, float> || std::is_same_v<T, double>) {
                constant = v;
            } else if constexpr (std::is_same_v<T, std::string>) {
                constant = util::utf16(v);
            }
        },
        value);
    return convert_constant_value(expected_type, type, constant);
}

Expression* TypeSystemAstBuilder::convert_constant_value(const IType* expected_type, const IType* type, const ConstantValue& constant_value_in) {
    if (type == nullptr) throw std::invalid_argument("type");
    ConstantValue constant_value = constant_value_in;
    if (std::holds_alternative<std::monostate>(constant_value)) {
        const std::optional<bool> is_reference = type->is_reference_type();
        if ((is_reference.has_value() && *is_reference) || is_known_type(type, KnownTypeCode::NullableOfT) || is_any_pointer(type->kind())) {
            auto* expr = make<NullReferenceExpression>();
            if (add_resolve_result_annotations) expr->add_annotation(make<ConstantResolveResult>(&SpecialType::null_type(), ConstantValue()));
            return expr;
        } else {
            auto* expr = make<DefaultValueExpression>(convert_type(type));
            if (add_resolve_result_annotations) expr->add_annotation(make<ConstantResolveResult>(type, ConstantValue()));
            return expr;
        }
    } else {
        const IType* underlying_type = nullable_type::get_underlying_type(type);
        if (underlying_type->kind() == TypeKind::Enum) {
            return convert_enum_value(underlying_type, std::get<std::int64_t>(util::csharp_primitive_cast(KnownTypeCode::Int64, constant_value, false)));
        } else {
            Expression* expr = nullptr;
            if (!(print_integral_values_as_hex && is_csharp_primitive_integer_type(*underlying_type)) &&
                is_special_constant(underlying_type, constant_value, &expr)) {
                return expr;
            }
            if (is_known_type(underlying_type, KnownTypeCode::Double) || is_known_type(underlying_type, KnownTypeCode::Single))
                return convert_floating_point_literal(underlying_type, constant_value);
            const IType* literal_type = underlying_type;
            const bool integer_type_mismatch = is_csharp_small_integer_type(*underlying_type) || is_csharp_native_integer_type(*underlying_type);
            if (integer_type_mismatch) {
                // C# does not have integer literals of small integer types, use `int` literal instead. It also doesn't have native
                // integer literals, those also use `int` (or `uint` for `nuint`).
                const bool is_unsigned = underlying_type->kind() == TypeKind::NUInt;
                constant_value = util::csharp_primitive_cast(is_unsigned ? KnownTypeCode::UInt32 : KnownTypeCode::Int32, constant_value, false);
                const ICompilation* compilation = nullptr;
                if (resolver_ != nullptr) {
                    compilation = &resolver_->compilation();
                } else if (const ITypeDefinition* def = expected_type->get_definition()) {
                    compilation = &def->compilation();
                }
                literal_type = compilation != nullptr ? compilation->find_type(is_unsigned ? KnownTypeCode::UInt32 : KnownTypeCode::Int32) : nullptr;
            }
            LiteralFormat format = LiteralFormat::None;
            if (print_integral_values_as_hex) format = LiteralFormat::HexadecimalNumber;
            expr = make<PrimitiveExpression>(constant_value, format);
            if (add_resolve_result_annotations && literal_type != nullptr) expr->add_annotation(make<ConstantResolveResult>(literal_type, constant_value));
            if ((integer_type_mismatch && !type_equals(type, expected_type)) || underlying_type->kind() == TypeKind::Unknown) {
                expr = make<CastExpression>(convert_type(type), expr);
            }
            return expr;
        }
    }
}

bool TypeSystemAstBuilder::is_special_constant(const IType* expected_type, const ConstantValue& constant, Expression** expression) {
    *expression = nullptr;
    const std::optional<SpecialConstantInfo> info = find_special_constant(constant);
    if (!info.has_value()) return false;
    // find IType of constant in compilation.
    const IType* constant_type = expected_type;
    if (!is_known_type(expected_type, info->type)) {
        const ICompilation* compilation = nullptr;
        if (resolver_ != nullptr) {
            compilation = &resolver_->compilation();
        } else if (const ITypeDefinition* def = expected_type->get_definition()) {
            compilation = &def->compilation();
        }
        if (compilation == nullptr) return false;
        constant_type = compilation->find_type(info->type);
    }
    // if the field definition cannot be found, do not generate a reference to the field.
    const std::string member_name = info->member;
    const std::vector<const IField*> fields = constant_type->get_fields([&](const IField* p) { return p->name() == member_name; });
    if (fields.size() > 1) throw std::logic_error("Sequence contains more than one element");
    const IField* field = fields.empty() ? nullptr : fields.front();
    if (!use_special_constants || field == nullptr) {
        // +Infty, -Infty and NaN, cannot be represented in their encoded form. Use an equivalent arithmetic expression instead.
        auto divide = [&](ConstantValue left_value, ConstantValue right_value, ConstantValue result_value) {
            auto* left = make<PrimitiveExpression>(left_value);
            left->add_annotation(make<ConstantResolveResult>(constant_type, left_value));
            auto* right = make<PrimitiveExpression>(right_value);
            right->add_annotation(make<ConstantResolveResult>(constant_type, right_value));
            auto* result = make<BinaryOperatorExpression>(left, BinaryOperatorType::Divide, right);
            result->add_annotation(make<ConstantResolveResult>(constant_type, result_value));
            return result;
        };
        if (info->type == KnownTypeCode::Double) {
            const double d = std::get<double>(constant);
            if (d == -std::numeric_limits<double>::infinity()) {   // (-1.0 / 0.0)
                *expression = divide(-1.0, 0.0, d);
                return true;
            } else if (d == std::numeric_limits<double>::infinity()) {   // (1.0 / 0.0)
                *expression = divide(1.0, 0.0, d);
                return true;
            } else if (std::isnan(d)) {   // (0.0 / 0.0)
                *expression = divide(0.0, 0.0, d);
                return true;
            }
        }
        if (info->type == KnownTypeCode::Single) {
            const float f = std::get<float>(constant);
            if (f == -std::numeric_limits<float>::infinity()) {   // (-1.0f / 0.0f)
                *expression = divide(-1.0f, 0.0f, f);
                return true;
            } else if (f == std::numeric_limits<float>::infinity()) {   // (1.0f / 0.0f)
                *expression = divide(1.0f, 0.0f, f);
                return true;
            } else if (std::isnan(f)) {   // (0.0f / 0.0f)
                *expression = divide(0.0f, 0.0f, f);
                return true;
            }
        }
        return false;
    }

    Expression* type_reference = make<TypeReferenceExpression>(convert_type(constant_type));

    if (add_resolve_result_annotations) type_reference->add_annotation(make<TypeResolveResult>(constant_type));

    Expression* member_reference = make<MemberReferenceExpression>(type_reference, member_name);

    if (add_resolve_result_annotations) member_reference->add_annotation(make<MemberResolveResult>(make<TypeResolveResult>(constant_type), field));

    *expression = member_reference;
    return true;
}

bool TypeSystemAstBuilder::is_flags_enum(const ITypeDefinition* type) { return type->has_attribute(KnownAttribute::Flags); }

Expression* TypeSystemAstBuilder::convert_enum_value(const IType* type, std::int64_t val, const IField* declaring_enum_member) {
    const ITypeDefinition* enum_definition = type->get_definition();
    const KnownTypeCode enum_base_type_code = get_type_code(enum_definition->enum_underlying_type());
    const bool is_flags = is_flags_enum(enum_definition);

    std::vector<EnumFieldInfo> fields;
    for (const IField* field : enum_definition->fields()) {   // PrepareConstant
        if (!field->is_const()) continue;
        const ConstantValue constant_value = field->get_constant_value();
        if (std::holds_alternative<std::monostate>(constant_value)) continue;
        const auto value = std::get<std::int64_t>(util::csharp_primitive_cast(KnownTypeCode::Int64, constant_value, false));
        fields.push_back({value, field, calculate_hamming_weight(static_cast<std::uint64_t>(value))});
    }
    const std::int64_t declaring_token_row_number =
        declaring_enum_member == nullptr ? std::numeric_limits<std::int32_t>::max() : static_cast<std::int64_t>(srm::get_row_number(declaring_enum_member->metadata_token()));
    auto row_of = [](const IField* field) { return static_cast<std::int64_t>(srm::get_row_number(field->metadata_token())); };

    auto make_enum_member_reference = [&](const IField* field) -> Expression* {
        if (declaring_enum_member == nullptr) {
            auto* target = make<TypeReferenceExpression>(convert_type(type));
            auto* mre = make<MemberReferenceExpression>(target, field->name());
            if (add_resolve_result_annotations) mre->add_annotation(make<MemberResolveResult>(get_resolve_result(mre->target()), field));
            return mre;
        } else {
            auto* ie = make<IdentifierExpression>(field->name());
            if (add_resolve_result_annotations) ie->add_annotation(make<MemberResolveResult>(nullptr, field));
            return ie;
        }
    };

    for (const EnumFieldInfo& info : fields) {
        // In a [Flags] enum declaration, only reference single-bit members directly: combined values are built from their flag
        // components below (so that e.g. All = Item1 | Item2 | Item3), and zero members stay numeric, because mask-style enums
        // routinely contain several unrelated zero members (e.g. MethodAttributes.PrivateScope/ReuseSlot).
        if (info.value == val && (declaring_enum_member == nullptr || !is_flags || info.weight == 1)) {
            if (info.field == declaring_enum_member || (declaring_token_row_number < row_of(info.field))) {
                return convert_constant_value(enum_definition->enum_underlying_type(), util::csharp_primitive_cast(enum_base_type_code, ConstantValue(val), false));
            }
            return make_enum_member_reference(info.field);
        }
    }
    if (is_flags) {
        // The complement of a byte- or ushort-based enum member is computed in int and therefore negative, which an enum member
        // initializer cannot implicitly convert back to the underlying type -- the ~X form would not compile there.
        const bool complement_compiles =
            declaring_enum_member == nullptr || !(enum_base_type_code == KnownTypeCode::Byte || enum_base_type_code == KnownTypeCode::UInt16);
        std::int64_t enum_value = val;
        Expression* expr = nullptr;
        std::int64_t negated_enum_value = ~val;
        // limit negatedEnumValue to the appropriate range
        switch (enum_base_type_code) {
        case KnownTypeCode::Byte:
        case KnownTypeCode::SByte: negated_enum_value &= 0xFF; break;
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16: negated_enum_value &= 0xFFFF; break;
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32: negated_enum_value &= 0xFFFFFFFFLL; break;
        default: break;
        }
        Expression* negated_expr = nullptr;
        std::vector<EnumFieldInfo> by_weight = fields;
        std::stable_sort(by_weight.begin(), by_weight.end(), [](const EnumFieldInfo& a, const EnumFieldInfo& b) { return a.weight > b.weight; });
        for (const EnumFieldInfo& info : by_weight) {
            if (info.value == 0 || info.field == declaring_enum_member) continue;   // skip None enum value

            if (declaring_token_row_number < row_of(info.field)) continue;

            if ((info.value & enum_value) == info.value) {
                Expression* field_expression = make_enum_member_reference(info.field);
                if (expr == nullptr)
                    expr = field_expression;
                else
                    expr = make<BinaryOperatorExpression>(expr, BinaryOperatorType::BitwiseOr, field_expression);

                enum_value &= ~info.value;
            }
            if (complement_compiles && (info.value & negated_enum_value) == info.value) {
                Expression* field_expression = make_enum_member_reference(info.field);
                if (negated_expr == nullptr)
                    negated_expr = field_expression;
                else
                    negated_expr = make<BinaryOperatorExpression>(negated_expr, BinaryOperatorType::BitwiseOr, field_expression);

                negated_enum_value &= ~info.value;
            }
        }
        // A multi-bit value that lies entirely within a larger, previously declared member is usually a field encoding inside that
        // mask (e.g. TypeAttributes.NestedPrivate within VisibilityMask), not a union of independent flags; keep it numeric.
        const bool is_encoded_in_earlier_mask =
            declaring_enum_member != nullptr &&
            std::any_of(fields.begin(), fields.end(), [&](const EnumFieldInfo& f) {
                return f.field != declaring_enum_member && row_of(f.field) < declaring_token_row_number && (f.value & val) == val && f.value != val;
            });
        if (enum_value == 0 && expr != nullptr && !is_encoded_in_earlier_mask) {
            if (!(negated_enum_value == 0 && negated_expr != nullptr && count_descendants(negated_expr) < count_descendants(expr))) {
                return expr;
            }
        }
        if (complement_compiles && negated_enum_value == 0 && negated_expr != nullptr) {
            return make<UnaryOperatorExpression>(UnaryOperatorType::BitNot, negated_expr);
        }
    }

    Expression* numeric_expression =
        convert_constant_value(enum_definition->enum_underlying_type(), util::csharp_primitive_cast(enum_base_type_code, ConstantValue(val), false));
    if (declaring_enum_member != nullptr) return numeric_expression;
    return make<CastExpression>(convert_type(type), numeric_expression);
}

Expression* TypeSystemAstBuilder::convert_floating_point_literal(const IType* type, const ConstantValue& constant_value_in) {
    // Coerce constantValue to either float or double: there are compilers that embed 0 (and possibly other values) as int into
    // constant value signatures, even if the expected type is float or double.
    const ConstantValue constant_value = util::csharp_primitive_cast(get_type_code(type), constant_value_in, false);
    const bool is_double = is_known_type(type, KnownTypeCode::Double);
    const ICompilation& compilation = type->get_definition()->compilation();
    Expression* expr = nullptr;

    std::string str;
    if (is_double) {
        const double d = std::get<double>(constant_value);
        if (std::floor(d) == d) expr = make<PrimitiveExpression>(constant_value);
        str = util::format_r(d);
    } else {
        const float f = std::get<float>(constant_value);
        if (std::floor(f) == f) expr = make<PrimitiveExpression>(constant_value);
        str = util::format_r(f);
    }

    const bool use_fraction = (static_cast<int>(str.size()) - (!str.empty() && str[0] == '-' ? 2 : 1) > 5);

    if (use_fraction && expr == nullptr) {
        static_assert(200 < kMaxDenominatorFloat);
        // For fractions not involving PI, use a smaller MAX_DENOMINATOR to avoid coincidences such as (1f/MathF.PI) == (113f/355f)
        const auto [num, den] = is_double ? fraction_approx(std::get<double>(constant_value), kMaxDenominatorDouble)
                                          : fraction_approx(static_cast<double>(std::get<float>(constant_value)), 200);

        if (is_valid_fraction(num, den) && is_equal(num, den, constant_value, is_double) && std::llabs(den) != 1) {
            Expression* left = make_constant(type, num);
            Expression* right = make_constant(type, den);
            expr = make<BinaryOperatorExpression>(left, BinaryOperatorType::Divide, right);
        }
    }

    if (use_fraction && expr == nullptr && use_special_constants) {
        const IType* math_type;
        if (is_double) {
            math_type = find_type(compilation, FullTypeName("System.Math"));
        } else {
            math_type = find_type(compilation, FullTypeName(TopLevelTypeName("System", "MathF")));
            const ITypeDefinition* type_def = math_type->get_definition();
            if (type_def == nullptr || !is_direct_import_of(type_def, &compilation.main_module()) ||
                type_def->get_fields([](const IField* f) { return f->name() == "PI" && f->is_const(); }).empty() ||
                type_def->get_fields([](const IField* f) { return f->name() == "E" && f->is_const(); }).empty()) {
                math_type = find_type(compilation, FullTypeName("System.Math"));
            }
        }

        expr = try_extract_expression(math_type, type, constant_value, "PI", is_double);
        if (expr == nullptr) expr = try_extract_expression(math_type, type, constant_value, "E", is_double);
    }

    if (expr == nullptr) expr = make<PrimitiveExpression>(constant_value);

    if (add_resolve_result_annotations) expr->add_annotation(make<ConstantResolveResult>(type, constant_value));

    return expr;
}

Expression* TypeSystemAstBuilder::make_constant(const IType* type, std::int64_t c) {
    return make<PrimitiveExpression>(util::csharp_primitive_cast(get_type_code(type), ConstantValue(c), true));
}

Expression* TypeSystemAstBuilder::try_extract_expression(const IType* math_type, const IType* type, const ConstantValue& literal_value,
                                                         const std::string& member_name, bool is_double) {
    auto make_field_reference = [&]() -> Expression* {
        AstType* math_ast_type = convert_type(math_type);
        auto* field_ref = make<MemberReferenceExpression>(make<TypeReferenceExpression>(math_ast_type), member_name);
        if (add_resolve_result_annotations) {
            const auto fields = math_type->get_fields([&](const IField* f) { return f->name() == member_name; });
            if (!fields.empty()) field_ref->add_annotation(make<MemberResolveResult>(get_resolve_result(math_ast_type), fields.front()));
        }
        if (is_known_type(type, KnownTypeCode::Double)) return field_ref;
        if (math_type->name() == "MathF") return field_ref;
        return make<CastExpression>(convert_type(type), field_ref);
    };

    const bool is_pi = member_name == "PI";

    auto extract_expression = [&](std::int64_t n, std::int64_t d) -> Expression* {
        Expression* field_reference = make_field_reference();

        // Math.PI or Math.E or (float)Math.PI or (float)Math.E or MathF.PI or MathF.E
        Expression* expr = field_reference;

        if (n != 1) {
            if (n == -1) {
                // -field
                expr = make<UnaryOperatorExpression>(UnaryOperatorType::Minus, expr);
            } else {
                // field * n
                expr = make<BinaryOperatorExpression>(expr, BinaryOperatorType::Multiply, make_constant(type, n));
            }
        }

        if (d != 1) {
            // field * n / d or -field / d or field / d
            expr = make<BinaryOperatorExpression>(expr, BinaryOperatorType::Divide, make_constant(type, d));
        }

        if (is_double) {
            const double field = is_pi ? 3.14159265358979323846 : 2.71828182845904523536;
            const double approx_value = field * static_cast<double>(n) / static_cast<double>(d);
            if (equal_doubles(approx_value, std::get<double>(literal_value))) return expr;
        } else {
            const float field = is_pi ? kMathF_PI : kMathF_E;
            const float approx_value = field * static_cast<float>(n) / static_cast<float>(d);
            if (equal_floats(approx_value, std::get<float>(literal_value))) return expr;
        }

        // Math.PI or Math.E or (float)Math.PI or (float)Math.E or MathF.PI or MathF.E
        if (field_reference->parent() != nullptr) field_reference->remove();
        expr = field_reference;

        if (d == 1) {
            // n / field
            expr = make<BinaryOperatorExpression>(make_constant(type, n), BinaryOperatorType::Divide, expr);
        } else {
            // n / (d * field)
            expr = make<BinaryOperatorExpression>(make_constant(type, d), BinaryOperatorType::Multiply, expr);
            expr = make<BinaryOperatorExpression>(make_constant(type, n), BinaryOperatorType::Divide, expr);
        }

        if (is_double) {
            const double field = is_pi ? 3.14159265358979323846 : 2.71828182845904523536;
            const double approx_value = static_cast<double>(n) / (static_cast<double>(d) * field);
            if (equal_doubles(approx_value, std::get<double>(literal_value))) return expr;
        } else {
            const float field = is_pi ? kMathF_PI : kMathF_E;
            const float approx_value = static_cast<float>(n) / (static_cast<float>(d) * field);
            if (equal_floats(approx_value, std::get<float>(literal_value))) return expr;
        }

        return nullptr;
    };

    constexpr double kPi = 3.14159265358979323846;
    constexpr double kE = 2.71828182845904523536;

    std::pair<std::int64_t, std::int64_t> approx =
        is_double ? fraction_approx(std::get<double>(literal_value) / (is_pi ? kPi : kE), kMaxDenominatorDouble)
                  : fraction_approx(static_cast<double>(std::get<float>(literal_value) / (is_pi ? kMathF_PI : kMathF_E)), kMaxDenominatorFloat);

    if (is_valid_fraction(approx.first, approx.second)) {
        Expression* expr = extract_expression(approx.first, approx.second);
        if (expr != nullptr) return expr;
    }

    approx = is_double ? fraction_approx(std::get<double>(literal_value) * (is_pi ? kPi : kE), kMaxDenominatorDouble)
                       : fraction_approx(static_cast<double>(std::get<float>(literal_value) * (is_pi ? kMathF_PI : kMathF_E)), kMaxDenominatorFloat);

    if (is_valid_fraction(approx.first, approx.second)) return extract_expression(approx.first, approx.second);

    return nullptr;
}

// =============================================================================================================================
// Convert Parameter
// =============================================================================================================================

ParameterDeclaration* TypeSystemAstBuilder::convert_parameter(const IParameter* parameter) {
    if (parameter == nullptr) throw std::invalid_argument("parameter");
    auto* decl = make<ParameterDeclaration>();
    decl->set_parameter_modifier(parameter->reference_kind());
    decl->set_is_params(parameter->is_params());
    decl->set_is_scoped_ref(parameter->lifetime().scoped_ref);
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(parameter->get_attributes())) decl->attributes().add(section);
    }
    const IType* parameter_type;
    if (parameter->type()->kind() == TypeKind::ByReference) {
        // avoid 'out ref'
        parameter_type = dynamic_cast<const ByReferenceType*>(parameter->type())->element_type();
    } else {
        parameter_type = parameter->type();
    }
    decl->set_type(convert_type(parameter_type));
    if (show_parameter_names) decl->set_name(parameter->name());
    if (is_default_value_assignment_allowed(parameter) && show_constant_values) {
        try {
            decl->set_default_expression(convert_constant_value(parameter_type, parameter->get_constant_value(true)));
        } catch (const srm::BadImageFormatError& ex) {
            decl->set_default_expression(make<ErrorExpression>(ex.message()));
        }
    }
    return decl;
}

// =============================================================================================================================
// Convert Entity
// =============================================================================================================================

AstNode* TypeSystemAstBuilder::convert_symbol(const ISymbol* symbol) {
    if (symbol == nullptr) throw std::invalid_argument("symbol");
    switch (symbol->symbol_kind()) {
    case SymbolKind::Namespace: return convert_namespace_declaration(dynamic_cast<const INamespace*>(symbol));
    case SymbolKind::Variable: return convert_variable(dynamic_cast<const IVariable*>(symbol));
    case SymbolKind::Parameter: return convert_parameter(dynamic_cast<const IParameter*>(symbol));
    case SymbolKind::TypeParameter: return convert_type_parameter(dynamic_cast<const ITypeParameter*>(symbol));
    default: {
        const auto* entity = dynamic_cast<const IEntity*>(symbol);
        if (entity != nullptr) return convert_entity(entity);
        throw std::invalid_argument("Invalid value for SymbolKind: " + std::to_string(static_cast<int>(symbol->symbol_kind())));
    }
    }
}

EntityDeclaration* TypeSystemAstBuilder::convert_entity(const IEntity* entity) {
    if (entity == nullptr) throw std::invalid_argument("entity");
    switch (entity->symbol_kind()) {
    case SymbolKind::TypeDefinition: return convert_type_definition(dynamic_cast<const ITypeDefinition*>(entity));
    case SymbolKind::Field: return convert_field(dynamic_cast<const IField*>(entity));
    case SymbolKind::Property: return convert_property(dynamic_cast<const IProperty*>(entity));
    case SymbolKind::Indexer: return convert_indexer(dynamic_cast<const IProperty*>(entity));
    case SymbolKind::Event: return convert_event(dynamic_cast<const IEvent*>(entity));
    case SymbolKind::Method: return convert_method(dynamic_cast<const IMethod*>(entity));
    case SymbolKind::Operator: return convert_operator(dynamic_cast<const IMethod*>(entity));
    case SymbolKind::Constructor: return convert_constructor(dynamic_cast<const IMethod*>(entity));
    case SymbolKind::Destructor: return convert_destructor(dynamic_cast<const IMethod*>(entity));
    case SymbolKind::Accessor: {
        const auto* accessor = dynamic_cast<const IMethod*>(entity);
        const auto* owner = dynamic_cast<const IProperty*>(accessor->accessor_owner());
        if (owner != nullptr && is_parameterized_property(owner)) {
            // C# cannot represent the parameterized property itself; its accessors are declared as ordinary methods.
            return convert_method(accessor);
        }
        const Accessibility owner_accessibility = accessor->accessor_owner() != nullptr ? accessor->accessor_owner()->accessibility() : Accessibility::None;
        return convert_accessor(accessor, accessor->accessor_kind(), owner_accessibility, false);
    }
    default: throw std::invalid_argument("Invalid value for SymbolKind: " + std::to_string(static_cast<int>(entity->symbol_kind())));
    }
}

EntityDeclaration* TypeSystemAstBuilder::convert_extension(const ExtensionGroup& group) {
    auto* ext = make<ExtensionDeclaration>();
    std::vector<const IType*> class_type_arguments(group.type_parameters.begin(), group.type_parameters.end());
    const TypeParameterSubstitution subst(std::move(class_type_arguments), std::vector<const IType*>());
    for (const ITypeParameter* tp : group.type_parameters) ext->type_parameters().add(convert_type_parameter(tp));
    const IMethod* specialized = group.marker_method->specialize_method(subst);
    if (specialized->parameters().size() != 1) throw std::logic_error("Sequence does not contain exactly one element");
    ext->receiver_parameters().add(convert_parameter(specialized->parameters().front()));
    for (const ITypeParameter* tp : group.type_parameters) {
        if (Constraint* constraint = convert_type_parameter_constraint(tp)) ext->constraints().add(constraint);
    }
    return ext;
}

EntityDeclaration* TypeSystemAstBuilder::convert_type_definition(const ITypeDefinition* type_definition) {
    Modifiers modifiers = Modifiers::None;
    if (show_accessibility) modifiers |= modifier_from_accessibility(type_definition->accessibility(), use_private_protected_accessibility);
    if (show_modifiers) {
        if (type_definition->is_static()) {
            modifiers |= Modifiers::Static;
        } else if (type_definition->is_abstract()) {
            modifiers |= Modifiers::Abstract;
        } else if (type_definition->is_sealed()) {
            modifiers |= Modifiers::Sealed;
        }
    }

    ClassType class_type = ClassType::Class;
    bool is_class_like = false;   // the `default:` of upstream's switch
    switch (type_definition->kind()) {
    case TypeKind::Struct:
    case TypeKind::Void:
        class_type = ClassType::Struct;
        modifiers &= ~Modifiers::Sealed;
        if (show_modifiers) {
            if (type_definition->is_read_only()) modifiers |= Modifiers::Readonly;
            if (type_definition->is_by_ref_like()) modifiers |= Modifiers::Ref;
        }
        if (support_record_structs && type_definition->is_record()) class_type = ClassType::RecordStruct;
        break;
    case TypeKind::Enum:
        class_type = ClassType::Enum;
        modifiers &= ~Modifiers::Sealed;
        break;
    case TypeKind::Interface:
        class_type = ClassType::Interface;
        modifiers &= ~Modifiers::Abstract;
        break;
    case TypeKind::Delegate: {
        const IMethod* invoke = get_delegate_invoke_method(type_definition);
        if (invoke != nullptr) return convert_delegate(invoke, modifiers);
        is_class_like = true;
        break;
    }
    default: is_class_like = true; break;
    }
    if (is_class_like) {
        class_type = ClassType::Class;
        if (support_record_classes && type_definition->is_record()) class_type = ClassType::RecordClass;
    }

    auto* decl = make<TypeDeclaration>();
    decl->set_class_type(class_type);
    decl->set_modifiers(modifiers);
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(type_definition->get_attributes())) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<TypeResolveResult>(type_definition));
    decl->set_name(type_definition->name() == "_" ? "@_" : type_definition->name());

    const int outer_type_parameter_count =
        type_definition->declaring_type_definition() == nullptr ? 0 : type_definition->declaring_type_definition()->type_parameter_count();

    const auto& type_parameters = type_definition->type_parameters();
    if (show_type_parameters) {
        for (std::size_t i = static_cast<std::size_t>(outer_type_parameter_count); i < type_parameters.size(); i++) {
            decl->type_parameters().add(convert_type_parameter(type_parameters[i]));
        }
    }

    if (show_base_types) {
        MemberLookup base_list_lookup(type_definition->declaring_type_definition(), type_definition->parent_module());
        for (const IType* base_type : type_definition->direct_base_types()) {
            // Interfaces enter the interface-impl metadata transitively, so entries the base list cannot name can be dropped; a base
            // class was always written explicitly and stays even if C# could not name it.
            if (base_type->kind() == TypeKind::Interface && !base_type_accessible_from(base_type, type_definition, base_list_lookup)) continue;
            if (type_definition->kind() == TypeKind::Enum && is_known_type(base_type, KnownTypeCode::Enum)) {
                // if the declared type is an enum, replace all references to System.Enum with the enum-underlying type
                if (!is_known_type(type_definition->enum_underlying_type(), KnownTypeCode::Int32)) {
                    decl->base_types().add(convert_type(type_definition->enum_underlying_type()));
                }
            } else if ((type_definition->kind() == TypeKind::Struct || type_definition->kind() == TypeKind::Void) &&
                       is_known_type(base_type, KnownTypeCode::ValueType)) {
                // if the declared type is a struct, ignore System.ValueType
                continue;
            } else if (is_known_type(base_type, KnownTypeCode::Object)) {
                // always ignore System.Object
                continue;
            } else if (support_record_classes && type_definition->is_record() && base_type->name() == "IEquatable" && base_type->namespace_() == "System" &&
                       base_type->type_arguments().size() == 1 && type_equals(base_type->type_arguments()[0], as_parameterized_type(type_definition))) {
                // omit "IEquatable<R>" in records
                continue;
            } else {
                decl->base_types().add(convert_type(base_type));
            }
        }
    }

    if (show_type_parameters && show_type_parameter_constraints) {
        for (std::size_t i = static_cast<std::size_t>(outer_type_parameter_count); i < type_parameters.size(); i++) {
            if (Constraint* constraint = convert_type_parameter_constraint(type_parameters[i])) decl->constraints().add(constraint);
        }
    }
    return decl;
}

bool TypeSystemAstBuilder::base_type_accessible_from(const IType* base_type, const ITypeDefinition* current_type, const MemberLookup& lookup) {
    // Every type the base-list reference names must be nameable there, including type arguments ('class SubF : F, IWrap<F.IFoo>' fails
    // like F.IFoo itself would).
    BaseListNameabilityVisitor visitor(current_type, lookup);
    base_type->accept_visitor(visitor);
    return visitor.all_nameable;
}

DelegateDeclaration* TypeSystemAstBuilder::convert_delegate(const IMethod* invoke_method, Modifiers modifiers) {
    const ITypeDefinition* d = invoke_method->declaring_type_definition();

    auto* decl = make<DelegateDeclaration>();
    decl->set_modifiers(modifiers & ~Modifiers::Sealed);
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(d->get_attributes())) decl->attributes().add(section);
        for (AttributeSection* section : convert_attributes(invoke_method->get_return_type_attributes(), "return")) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<TypeResolveResult>(d));
    decl->set_return_type(convert_type(invoke_method->return_type()));
    if (invoke_method->return_type_is_ref_read_only()) {
        auto* ct = as<ComposedType>(decl->return_type());
        if (ct != nullptr && ct->has_ref_specifier()) ct->set_has_read_only_specifier(true);
    }
    decl->set_name(d->name());

    const int outer_type_parameter_count = d->declaring_type_definition() == nullptr ? 0 : d->declaring_type_definition()->type_parameter_count();
    const auto& type_parameters = d->type_parameters();

    if (show_type_parameters) {
        for (std::size_t i = static_cast<std::size_t>(outer_type_parameter_count); i < type_parameters.size(); i++) {
            decl->type_parameters().add(convert_type_parameter(type_parameters[i]));
        }
    }

    for (const IParameter* p : invoke_method->parameters()) decl->parameters().add(convert_parameter(p));

    if (show_type_parameters && show_type_parameter_constraints) {
        for (std::size_t i = static_cast<std::size_t>(outer_type_parameter_count); i < type_parameters.size(); i++) {
            if (Constraint* constraint = convert_type_parameter_constraint(type_parameters[i])) decl->constraints().add(constraint);
        }
    }
    return decl;
}

FieldDeclaration* TypeSystemAstBuilder::convert_field(const IField* field) {
    auto* decl = make<FieldDeclaration>();
    if (show_modifiers) {
        Modifiers m = get_member_modifiers(field);
        if (field->is_const()) {
            m &= ~Modifiers::Static;
            m |= Modifiers::Const;
        } else if (field->is_read_only()) {
            m |= Modifiers::Readonly;
        } else if (field->is_volatile()) {
            m |= Modifiers::Volatile;
        }
        decl->set_modifiers(m);
    }
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(field->get_attributes())) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, field));
    decl->set_return_type(convert_type(field->return_type()));
    if (auto* ct = as<ComposedType>(decl->return_type())) {
        if (ct->has_ref_specifier() && field->return_type_is_ref_read_only()) ct->set_has_read_only_specifier(true);
    }
    Expression* initializer = nullptr;
    if (field->is_const() && show_constant_values) {
        try {
            initializer = convert_constant_value(field->type(), field->get_constant_value(true));
        } catch (const srm::BadImageFormatError& ex) {
            initializer = make<ErrorExpression>(ex.message());
        }
    }
    decl->variables().add(make<VariableInitializer>(field->name(), initializer));
    return decl;
}

BlockStatement* TypeSystemAstBuilder::generate_body_block() {
    if (generate_body) {
        auto* block = make<BlockStatement>();
        block->add(make<ThrowStatement>(make<ObjectCreateExpression>(convert_type(FullTypeName(TopLevelTypeName("System", "NotImplementedException", 0))))));
        return block;
    }
    return nullptr;
}

Accessor* TypeSystemAstBuilder::convert_accessor(const IMethod* accessor, MethodSemanticsAttributes kind, Accessibility owner_accessibility,
                                                 bool add_parameter_attribute) {
    if (accessor == nullptr) return nullptr;
    auto* decl = make<Accessor>();
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(accessor->get_attributes())) decl->attributes().add(section);
        for (AttributeSection* section : convert_attributes(accessor->get_return_type_attributes(), "return")) decl->attributes().add(section);
        if (add_parameter_attribute && !accessor->parameters().empty()) {
            for (AttributeSection* section : convert_attributes(accessor->parameters().back()->get_attributes(), "param")) decl->attributes().add(section);
        }
    }
    if (show_accessibility && accessor->accessibility() != owner_accessibility)
        decl->set_modifiers(modifier_from_accessibility(accessor->accessibility(), use_private_protected_accessibility));
    if (show_modifiers && has_readonly_modifier(accessor)) decl->set_modifiers(decl->modifiers() | Modifiers::Readonly);
    AccessorKind accessor_kind;
    switch (kind) {
    case MethodSemanticsAttributes::Getter: accessor_kind = AccessorKind::Getter; break;
    case MethodSemanticsAttributes::Setter: accessor_kind = AccessorKind::Setter; break;
    case MethodSemanticsAttributes::Adder: accessor_kind = AccessorKind::Adder; break;
    case MethodSemanticsAttributes::Remover: accessor_kind = AccessorKind::Remover; break;
    default: accessor_kind = AccessorKind::Any; break;
    }
    if (kind == MethodSemanticsAttributes::Setter && support_init_accessors && accessor->is_init_only()) accessor_kind = AccessorKind::Init;
    decl->set_accessor_kind(accessor_kind);
    if (accessor->is_init_only() && accessor_kind != AccessorKind::Init) decl->add_trailing_trivia(make<Comment>("init", CommentType::MultiLine));
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, accessor));
    if (generate_body) decl->set_body(generate_body_block());
    return decl;
}

PropertyDeclaration* TypeSystemAstBuilder::convert_property(const IProperty* property) {
    auto* decl = make<PropertyDeclaration>();
    decl->set_modifiers(get_member_modifiers(property));
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(property->get_attributes())) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, property));
    decl->set_return_type(convert_type(property->return_type()));
    if (property->return_type_is_ref_read_only()) {
        auto* ct = as<ComposedType>(decl->return_type());
        if (ct != nullptr && ct->has_ref_specifier()) ct->set_has_read_only_specifier(true);
    }
    decl->set_name(property->name());
    decl->set_getter(convert_accessor(property->getter(), MethodSemanticsAttributes::Getter, property->accessibility(), false));
    decl->set_setter(convert_accessor(property->setter(), MethodSemanticsAttributes::Setter, property->accessibility(), true));
    decl->set_private_implementation_type(get_explicit_interface_type(property));
    merge_read_only_modifiers(decl, decl->getter(), decl->setter());
    return decl;
}

IndexerDeclaration* TypeSystemAstBuilder::convert_indexer(const IProperty* indexer) {
    auto* decl = make<IndexerDeclaration>();
    decl->set_modifiers(get_member_modifiers(indexer));
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(indexer->get_attributes())) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, indexer));
    decl->set_return_type(convert_type(indexer->return_type()));
    if (indexer->return_type_is_ref_read_only()) {
        auto* ct = as<ComposedType>(decl->return_type());
        if (ct != nullptr && ct->has_ref_specifier()) ct->set_has_read_only_specifier(true);
    }
    for (const IParameter* p : indexer->parameters()) decl->parameters().add(convert_parameter(p));
    decl->set_getter(convert_accessor(indexer->getter(), MethodSemanticsAttributes::Getter, indexer->accessibility(), false));
    decl->set_setter(convert_accessor(indexer->setter(), MethodSemanticsAttributes::Setter, indexer->accessibility(), true));
    decl->set_private_implementation_type(get_explicit_interface_type(indexer));
    merge_read_only_modifiers(decl, decl->getter(), decl->setter());
    return decl;
}

EntityDeclaration* TypeSystemAstBuilder::convert_event(const IEvent* ev) {
    if (use_custom_events) {
        auto* decl = make<CustomEventDeclaration>();
        decl->set_modifiers(get_member_modifiers(ev));
        if (show_attributes) {
            for (AttributeSection* section : convert_attributes(ev->get_attributes())) decl->attributes().add(section);
        }
        if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, ev));
        decl->set_return_type(convert_type(ev->return_type()));
        decl->set_name(ev->name());
        decl->set_add_accessor(convert_accessor(ev->add_accessor(), MethodSemanticsAttributes::Adder, ev->accessibility(), true));
        decl->set_remove_accessor(convert_accessor(ev->remove_accessor(), MethodSemanticsAttributes::Remover, ev->accessibility(), true));
        decl->set_private_implementation_type(get_explicit_interface_type(ev));
        merge_read_only_modifiers(decl, decl->add_accessor(), decl->remove_accessor());
        return decl;
    } else {
        auto* decl = make<EventDeclaration>();
        decl->set_modifiers(get_member_modifiers(ev));
        if (show_attributes) {
            for (AttributeSection* section : convert_attributes(ev->get_attributes())) decl->attributes().add(section);
        }
        if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, ev));
        decl->set_return_type(convert_type(ev->return_type()));
        decl->variables().add(make<VariableInitializer>(ev->name()));
        return decl;
    }
}

MethodDeclaration* TypeSystemAstBuilder::convert_method(const IMethod* method) {
    auto* decl = make<MethodDeclaration>();
    decl->set_modifiers(get_member_modifiers(method));
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(method->get_attributes())) decl->attributes().add(section);
        for (AttributeSection* section : convert_attributes(method->get_return_type_attributes(), "return")) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, method));
    decl->set_return_type(convert_type(method->return_type()));
    if (method->return_type_is_ref_read_only()) {
        auto* ct = as<ComposedType>(decl->return_type());
        if (ct != nullptr && ct->has_ref_specifier()) ct->set_has_read_only_specifier(true);
    }
    decl->set_name(method->name());

    if (show_type_parameters) {
        for (const ITypeParameter* tp : method->type_parameters()) decl->type_parameters().add(convert_type_parameter(tp));
    }

    for (const IParameter* p : method->parameters()) decl->parameters().add(convert_parameter(p));
    if (method->is_extension_method() && method->reduced_from() == nullptr && !decl->parameters().empty())
        decl->parameters().first_or_null()->set_has_this_modifier(true);

    if (show_type_parameters && show_type_parameter_constraints) {
        if (method->is_override() || method->is_explicit_interface_implementation()) {
            // C# inherits the constraints of an override or explicit interface implementation from the base member and forbids
            // restating them, with a single exception: a 'class', 'struct', or 'default' constraint may be given to disambiguate
            // whether 'T?' denotes a nullable annotation or Nullable<T>.
            add_nullability_disambiguating_constraints(decl, method);
        } else {
            for (const ITypeParameter* tp : method->type_parameters()) {
                if (Constraint* constraint = convert_type_parameter_constraint(tp)) decl->constraints().add(constraint);
            }
        }
    }
    decl->set_body(generate_body_block());
    decl->set_private_implementation_type(get_explicit_interface_type(method));
    return decl;
}

EntityDeclaration* TypeSystemAstBuilder::convert_operator(const IMethod* op) {
    const std::string op_name = op->name();
    const std::size_t dot = op_name.rfind('.');
    const std::string name = dot == std::string::npos ? op_name : op_name.substr(dot + 1);
    const std::optional<OperatorType> op_type = OperatorDeclaration::get_operator_type(name);
    if (!op_type.has_value()) return convert_method(op);
    if (*op_type == OperatorType::UnsignedRightShift && !support_unsigned_right_shift) return convert_method(op);
    if (!support_operator_checked && OperatorDeclaration::is_checked(*op_type)) return convert_method(op);

    auto* decl = make<OperatorDeclaration>();
    decl->set_modifiers(get_member_modifiers(op));
    decl->set_operator_type(*op_type);
    decl->set_return_type(convert_type(op->return_type()));
    if (op->return_type_is_ref_read_only()) {
        auto* ct = as<ComposedType>(decl->return_type());
        if (ct != nullptr && ct->has_ref_specifier()) ct->set_has_read_only_specifier(true);
    }
    for (const IParameter* p : op->parameters()) decl->parameters().add(convert_parameter(p));
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(op->get_attributes())) decl->attributes().add(section);
        for (AttributeSection* section : convert_attributes(op->get_return_type_attributes(), "return")) decl->attributes().add(section);
    }
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, op));
    decl->set_body(generate_body_block());
    decl->set_private_implementation_type(get_explicit_interface_type(op));
    return decl;
}

ConstructorDeclaration* TypeSystemAstBuilder::convert_constructor(const IMethod* ctor) {
    auto* decl = make<ConstructorDeclaration>();
    decl->set_modifiers(get_member_modifiers(ctor));
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(ctor->get_attributes())) decl->attributes().add(section);
    }
    if (ctor->declaring_type_definition() != nullptr) decl->set_name(ctor->declaring_type_definition()->name());
    for (const IParameter* p : ctor->parameters()) decl->parameters().add(convert_parameter(p));
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, ctor));
    decl->set_body(generate_body_block());
    return decl;
}

DestructorDeclaration* TypeSystemAstBuilder::convert_destructor(const IMethod* dtor) {
    auto* decl = make<DestructorDeclaration>();
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(dtor->get_attributes())) decl->attributes().add(section);
    }
    if (dtor->declaring_type_definition() != nullptr) decl->set_name(dtor->declaring_type_definition()->name());
    if (add_resolve_result_annotations) decl->add_annotation(make<MemberResolveResult>(nullptr, dtor));
    decl->set_body(generate_body_block());
    return decl;
}

// =============================================================================================================================
// Convert Modifiers
// =============================================================================================================================

Modifiers TypeSystemAstBuilder::modifier_from_accessibility(Accessibility accessibility, bool use_private_protected) {
    switch (accessibility) {
    case Accessibility::Private: return Modifiers::Private;
    case Accessibility::Public: return Modifiers::Public;
    case Accessibility::Protected: return Modifiers::Protected;
    case Accessibility::Internal: return Modifiers::Internal;
    case Accessibility::ProtectedOrInternal: return Modifiers::Protected | Modifiers::Internal;
    case Accessibility::ProtectedAndInternal: return use_private_protected ? Modifiers::Private | Modifiers::Protected : Modifiers::Protected;
    default: return Modifiers::None;
    }
}

bool TypeSystemAstBuilder::needs_accessibility(const IMember* member) {
    const IType* declaring_type = member->declaring_type();
    if (member->is_explicit_interface_implementation()) return false;
    switch (member->symbol_kind()) {
    case SymbolKind::Constructor: return !member->is_static();
    case SymbolKind::Destructor: return false;
    default: {
        if (declaring_type != nullptr && declaring_type->kind() == TypeKind::Interface) return member->accessibility() != Accessibility::Public;
        const auto* method = dynamic_cast<const IMethod*>(member);
        return method == nullptr || !method->is_local_function();
    }
    }
}

Modifiers TypeSystemAstBuilder::get_member_modifiers(const IMember* member) {
    Modifiers m = Modifiers::None;
    if (show_accessibility && needs_accessibility(member)) m |= modifier_from_accessibility(member->accessibility(), use_private_protected_accessibility);
    if (show_modifiers) {
        if (const auto* local_function = dynamic_cast<const LocalFunctionMethod*>(member)) {
            if (local_function->is_static_local_function()) m |= Modifiers::Static;
        } else {
            if (member->is_static()) m |= Modifiers::Static;
            const auto* method = dynamic_cast<const IMethod*>(member);
            if (method != nullptr && method->this_is_ref_read_only() && method->declaring_type_definition() != nullptr &&
                !method->declaring_type_definition()->is_read_only()) {
                m |= Modifiers::Readonly;
            }

            const IType* declaring_type = member->declaring_type();
            if (declaring_type != nullptr && declaring_type->kind() == TypeKind::Interface) {
                if (!member->is_static() && !member->is_virtual() && !member->is_abstract() && !member->is_override() &&
                    member->accessibility() != Accessibility::Private && method != nullptr && method->has_body()) {
                    m |= Modifiers::Sealed;
                }
                if (member->is_static()) {
                    // modifiers of static members in interfaces:
                    if (member->is_abstract())
                        m |= Modifiers::Abstract;
                    else if (member->is_virtual() && !member->is_override())
                        m |= Modifiers::Virtual;
                }
            } else {
                if (member->is_abstract())
                    m |= Modifiers::Abstract;
                else if (member->is_virtual() && !member->is_override())
                    m |= Modifiers::Virtual;
                if (member->is_override() && !member->is_explicit_interface_implementation()) m |= Modifiers::Override;
                if (member->is_sealed() && !member->is_explicit_interface_implementation()) m |= Modifiers::Sealed;
            }
        }
    }
    return m;
}

// =============================================================================================================================
// Convert Type Parameter
// =============================================================================================================================

TypeParameterDeclaration* TypeSystemAstBuilder::convert_type_parameter(const ITypeParameter* tp) {
    auto* decl = make<TypeParameterDeclaration>();
    decl->set_variance(tp->variance());
    decl->set_name(tp->name());
    if (show_attributes) {
        for (AttributeSection* section : convert_attributes(tp->get_attributes())) decl->attributes().add(section);
    }
    return decl;
}

Constraint* TypeSystemAstBuilder::convert_type_parameter_constraint(const ITypeParameter* tp) {
    const auto base_types = tp->direct_base_types();
    if (!tp->has_default_constructor_constraint() && !tp->has_reference_type_constraint() && !tp->has_value_type_constraint() && !tp->allows_ref_like_type() &&
        tp->nullability_constraint() != Nullability::NotNullable && std::all_of(base_types.begin(), base_types.end(), is_object_or_value_type)) {
        return nullptr;
    }
    auto* c = make<Constraint>();
    c->set_type_parameter(make_simple_type(tp->name()));
    if (tp->has_reference_type_constraint()) {
        if (tp->nullability_constraint() == Nullability::Nullable) {
            c->base_types().add(make<PrimitiveType>("class")->make_nullable_type());
        } else {
            c->base_types().add(make<PrimitiveType>("class"));
        }
    } else if (tp->has_value_type_constraint()) {
        if (tp->has_unmanaged_constraint()) {
            c->base_types().add(make<PrimitiveType>("unmanaged"));
        } else {
            c->base_types().add(make<PrimitiveType>("struct"));
        }
    } else if (tp->nullability_constraint() == Nullability::NotNullable) {
        c->base_types().add(make<PrimitiveType>("notnull"));
    }
    for (const TypeConstraint& t : tp->type_constraints()) {
        if (!is_object_or_value_type(t.type) || !t.attributes.empty()) {
            AstType* ast_type = convert_type(t.type);
            if (!t.attributes.empty()) {
                auto* attr_section = make<AttributeSection>();
                for (const IAttribute* a : t.attributes) attr_section->attributes().add(convert_attribute(a));
                auto* composed = make<ComposedType>();
                composed->attributes().add(attr_section);
                composed->set_base_type(ast_type);
                ast_type = composed;
            }
            c->base_types().add(ast_type);
        }
    }
    if (tp->has_default_constructor_constraint() && !tp->has_value_type_constraint()) c->base_types().add(make<PrimitiveType>("new"));
    if (tp->allows_ref_like_type()) c->base_types().add(make<PrimitiveType>("allows ref struct"));
    return c;
}

// A disambiguator is required only where the type parameter itself carries a nullable annotation ('T?') in the signature: without it
// the compiler reads 'T?' as Nullable<T>. The inherited constraints are re-emitted on the override's own type parameters in metadata,
// so which disambiguator is legal follows from them without resolving the base member. The restated disambiguator leaves no metadata
// trace of its own, hence it must be derived rather than read back. The clause is built here rather than through
// ConvertTypeParameterConstraint, which also prints 'allows ref struct' from the byreflike flag. That flag is re-emitted on the
// override's own type parameter as well, and restating it is CS0460.
void TypeSystemAstBuilder::add_nullability_disambiguating_constraints(MethodDeclaration* decl, const IMethod* method) {
    if (method->type_parameters().empty()) return;
    NullableTypeParameterCollector collector(method->type_parameters());
    method->return_type()->accept_visitor(collector);
    for (const IParameter* p : method->parameters()) p->type()->accept_visitor(collector);
    if (collector.nullable_type_parameters.empty()) return;
    for (const ITypeParameter* tp : method->type_parameters()) {
        if (!collector.contains(tp)) continue;
        const std::optional<std::string> keyword = get_nullability_disambiguator(tp);
        if (!keyword.has_value()) continue;
        auto* c = make<Constraint>();
        c->set_type_parameter(make_simple_type(tp->name()));
        c->base_types().add(make<PrimitiveType>(*keyword));
        decl->constraints().add(c);
    }
}

// =============================================================================================================================
// Convert Variable, namespace, explicit interface types
// =============================================================================================================================

VariableDeclarationStatement* TypeSystemAstBuilder::convert_variable(const IVariable* v) {
    auto* decl = make<VariableDeclarationStatement>();
    decl->set_modifiers(v->is_const() ? Modifiers::Const : Modifiers::None);
    decl->set_type(convert_type(v->type()));
    Expression* initializer = nullptr;
    if (v->is_const()) {
        try {
            initializer = convert_constant_value(v->type(), v->get_constant_value(true));
        } catch (const srm::BadImageFormatError& ex) {
            initializer = make<ErrorExpression>(ex.message());
        }
    }
    decl->variables().add(make<VariableInitializer>(v->name(), initializer));
    return decl;
}

NamespaceDeclaration* TypeSystemAstBuilder::convert_namespace_declaration(const INamespace* ns) { return make<NamespaceDeclaration>(ns->full_name()); }

AstType* TypeSystemAstBuilder::get_explicit_interface_type(const IMember* member) {
    if (member->is_explicit_interface_implementation()) {
        const auto implemented = member->explicitly_implemented_interface_members();
        if (!implemented.empty()) return convert_type(get_interface_as_implemented_by(implemented.front()->declaring_type(), member->declaring_type()));
    }
    return nullptr;
}

}  // namespace cb::dotnet::decompiler::csharp::syntax

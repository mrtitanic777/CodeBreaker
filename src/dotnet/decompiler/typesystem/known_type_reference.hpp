// dotnet/decompiler/typesystem/known_type_reference.hpp -- the types the decompiler knows by name (System.Object, System.Int32,
// IEnumerable<T>, Task<T> ...): each one's code, kind, namespace, name, type parameter count and base type.
//
//     const KnownTypeReference* k = KnownTypeReference::get(KnownTypeCode::Int32);
//     k->type_name() == TopLevelTypeName("System", "Int32");
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: KnownTypeCode, KnownTypeReference; TypeKind), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler). Resolving a known type in a compilation comes with the type system.
#pragma once

#include <array>
#include <optional>
#include <string>

#include "dotnet/decompiler/typesystem/full_type_name.hpp"

namespace cb::dotnet::decompiler::typesystem {

// The first codes are System.TypeCode's.
enum class KnownTypeCode : std::uint8_t {
    None, Object, DBNull, Boolean, Char, SByte, Byte, Int16, UInt16, Int32, UInt32, Int64, UInt64, Single, Double, Decimal, DateTime,
    String = 18, Void, Type, Array, Attribute, ValueType, Enum, Delegate, MulticastDelegate, Exception, IntPtr, UIntPtr, IEnumerable,
    IEnumerator, IEnumerableOfT, IEnumeratorOfT, ICollection, ICollectionOfT, IList, IListOfT, IReadOnlyCollectionOfT, IReadOnlyListOfT,
    Task, TaskOfT, ValueTask, ValueTaskOfT, NullableOfT, IDisposable, IAsyncDisposable, INotifyCompletion, ICriticalNotifyCompletion,
    TypedReference, IFormattable, FormattableString, DefaultInterpolatedStringHandler, SpanOfT, ReadOnlySpanOfT, MemoryOfT, Unsafe,
    IAsyncEnumerableOfT, IAsyncEnumeratorOfT, Index, Range,
};
inline constexpr int kKnownTypeCodeCount = static_cast<int>(KnownTypeCode::Range) + 1;

// TypeSystem.TypeKind.
enum class TypeKind : std::uint8_t {
    Other, Class, Interface, Struct, Delegate, Enum, Void, Unknown, Null, None, Dynamic, UnboundTypeArgument, TypeParameter, Array,
    Pointer, ByReference, Intersection, ArgList, Tuple, ModOpt, ModReq, NInt, NUInt, FunctionPointer,
};

class KnownTypeReference {
public:
    // The reference of a code; nullptr for None and the unused code 17.
    static const KnownTypeReference* get(KnownTypeCode code);
    static const std::array<const KnownTypeReference*, kKnownTypeCodeCount>& all();

    KnownTypeCode known_type_code() const { return code_; }
    const std::string& namespace_() const { return namespace_name_; }
    const std::string& name() const { return name_; }
    int type_parameter_count() const { return type_parameter_count_; }
    KnownTypeCode base_type() const { return base_type_; }
    TypeKind type_kind() const { return type_kind_; }
    TopLevelTypeName type_name() const { return TopLevelTypeName(namespace_name_, name_, type_parameter_count_); }
    // The C# keyword if the type has one ("int"), else "Namespace.Name".
    std::string to_string() const;
    // "object", "bool", "int" ... for the types C# has keywords for, else nullopt.
    static std::optional<std::string> get_csharp_name_by_type_code(KnownTypeCode code);

    KnownTypeReference(KnownTypeCode code, TypeKind kind, std::string ns, std::string name, int type_parameter_count = 0,
                       KnownTypeCode base_type = KnownTypeCode::Object);

private:
    KnownTypeCode code_;
    std::string namespace_name_, name_;
    int type_parameter_count_;
    KnownTypeCode base_type_;
    TypeKind type_kind_;
};

// TypeSystemExtensions.IsKnownType for names.
bool is_known_type(const TopLevelTypeName& name, KnownTypeCode code);
bool is_known_type(const FullTypeName& name, KnownTypeCode code);

}  // namespace cb::dotnet::decompiler::typesystem

// dotnet/decompiler/typesystem/known_type_reference.cpp -- the known types' table.
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"

#include <memory>

namespace cb::dotnet::decompiler::typesystem {

KnownTypeReference::KnownTypeReference(KnownTypeCode code, TypeKind kind, std::string ns, std::string name, int type_parameter_count,
                                       KnownTypeCode base_type)
    : code_(code), namespace_name_(std::move(ns)), name_(std::move(name)), type_parameter_count_(type_parameter_count), base_type_(base_type),
      type_kind_(kind) {
    if (kind == TypeKind::Struct && base_type == KnownTypeCode::Object) base_type_ = KnownTypeCode::ValueType;
}

const std::array<const KnownTypeReference*, kKnownTypeCodeCount>& KnownTypeReference::all() {
    using K = KnownTypeCode;
    using T = TypeKind;
    static const std::array<const KnownTypeReference*, kKnownTypeCodeCount> table = [] {
        static const KnownTypeReference refs[] = {
            {K::Object, T::Class, "System", "Object", 0, K::None},
            {K::DBNull, T::Class, "System", "DBNull"},
            {K::Boolean, T::Struct, "System", "Boolean"},
            {K::Char, T::Struct, "System", "Char"},
            {K::SByte, T::Struct, "System", "SByte"},
            {K::Byte, T::Struct, "System", "Byte"},
            {K::Int16, T::Struct, "System", "Int16"},
            {K::UInt16, T::Struct, "System", "UInt16"},
            {K::Int32, T::Struct, "System", "Int32"},
            {K::UInt32, T::Struct, "System", "UInt32"},
            {K::Int64, T::Struct, "System", "Int64"},
            {K::UInt64, T::Struct, "System", "UInt64"},
            {K::Single, T::Struct, "System", "Single"},
            {K::Double, T::Struct, "System", "Double"},
            {K::Decimal, T::Struct, "System", "Decimal"},
            {K::DateTime, T::Struct, "System", "DateTime"},
            {K::String, T::Class, "System", "String"},
            {K::Void, T::Void, "System", "Void", 0, K::ValueType},
            {K::Type, T::Class, "System", "Type"},
            {K::Array, T::Class, "System", "Array"},
            {K::Attribute, T::Class, "System", "Attribute"},
            {K::ValueType, T::Class, "System", "ValueType"},
            {K::Enum, T::Class, "System", "Enum", 0, K::ValueType},
            {K::Delegate, T::Class, "System", "Delegate"},
            {K::MulticastDelegate, T::Class, "System", "MulticastDelegate", 0, K::Delegate},
            {K::Exception, T::Class, "System", "Exception"},
            {K::IntPtr, T::Struct, "System", "IntPtr"},
            {K::UIntPtr, T::Struct, "System", "UIntPtr"},
            {K::IEnumerable, T::Interface, "System.Collections", "IEnumerable"},
            {K::IEnumerator, T::Interface, "System.Collections", "IEnumerator"},
            {K::IEnumerableOfT, T::Interface, "System.Collections.Generic", "IEnumerable", 1},
            {K::IEnumeratorOfT, T::Interface, "System.Collections.Generic", "IEnumerator", 1},
            {K::ICollection, T::Interface, "System.Collections", "ICollection"},
            {K::ICollectionOfT, T::Interface, "System.Collections.Generic", "ICollection", 1},
            {K::IList, T::Interface, "System.Collections", "IList"},
            {K::IListOfT, T::Interface, "System.Collections.Generic", "IList", 1},
            {K::IReadOnlyCollectionOfT, T::Interface, "System.Collections.Generic", "IReadOnlyCollection", 1},
            {K::IReadOnlyListOfT, T::Interface, "System.Collections.Generic", "IReadOnlyList", 1},
            {K::Task, T::Class, "System.Threading.Tasks", "Task"},
            {K::TaskOfT, T::Class, "System.Threading.Tasks", "Task", 1, K::Task},
            {K::ValueTask, T::Struct, "System.Threading.Tasks", "ValueTask"},
            {K::ValueTaskOfT, T::Struct, "System.Threading.Tasks", "ValueTask", 1},
            {K::NullableOfT, T::Struct, "System", "Nullable", 1},
            {K::IDisposable, T::Interface, "System", "IDisposable"},
            {K::IAsyncDisposable, T::Interface, "System", "IAsyncDisposable"},
            {K::INotifyCompletion, T::Interface, "System.Runtime.CompilerServices", "INotifyCompletion"},
            {K::ICriticalNotifyCompletion, T::Interface, "System.Runtime.CompilerServices", "ICriticalNotifyCompletion"},
            {K::TypedReference, T::Struct, "System", "TypedReference"},
            {K::IFormattable, T::Interface, "System", "IFormattable"},
            {K::FormattableString, T::Class, "System", "FormattableString", 0, K::IFormattable},
            {K::DefaultInterpolatedStringHandler, T::Struct, "System.Runtime.CompilerServices", "DefaultInterpolatedStringHandler"},
            {K::SpanOfT, T::Struct, "System", "Span", 1},
            {K::ReadOnlySpanOfT, T::Struct, "System", "ReadOnlySpan", 1},
            {K::MemoryOfT, T::Struct, "System", "Memory", 1},
            {K::Unsafe, T::Class, "System.Runtime.CompilerServices", "Unsafe", 0},
            {K::IAsyncEnumerableOfT, T::Interface, "System.Collections.Generic", "IAsyncEnumerable", 1},
            {K::IAsyncEnumeratorOfT, T::Interface, "System.Collections.Generic", "IAsyncEnumerator", 1},
            {K::Index, T::Struct, "System", "Index", 0},
            {K::Range, T::Struct, "System", "Range", 0},
        };
        std::array<const KnownTypeReference*, kKnownTypeCodeCount> t{};
        for (const KnownTypeReference& r : refs) t[static_cast<std::size_t>(r.known_type_code())] = &r;
        return t;
    }();
    return table;
}

const KnownTypeReference* KnownTypeReference::get(KnownTypeCode code) { return all().at(static_cast<std::size_t>(code)); }

std::optional<std::string> KnownTypeReference::get_csharp_name_by_type_code(KnownTypeCode code) {
    switch (code) {
    case KnownTypeCode::Object: return "object";
    case KnownTypeCode::Boolean: return "bool";
    case KnownTypeCode::Char: return "char";
    case KnownTypeCode::SByte: return "sbyte";
    case KnownTypeCode::Byte: return "byte";
    case KnownTypeCode::Int16: return "short";
    case KnownTypeCode::UInt16: return "ushort";
    case KnownTypeCode::Int32: return "int";
    case KnownTypeCode::UInt32: return "uint";
    case KnownTypeCode::Int64: return "long";
    case KnownTypeCode::UInt64: return "ulong";
    case KnownTypeCode::Single: return "float";
    case KnownTypeCode::Double: return "double";
    case KnownTypeCode::Decimal: return "decimal";
    case KnownTypeCode::String: return "string";
    case KnownTypeCode::Void: return "void";
    default: return std::nullopt;
    }
}

std::string KnownTypeReference::to_string() const { return get_csharp_name_by_type_code(code_).value_or(namespace_name_ + "." + name_); }

bool is_known_type(const TopLevelTypeName& name, KnownTypeCode code) {
    const KnownTypeReference* k = KnownTypeReference::get(code);
    return k != nullptr && name == k->type_name();
}

bool is_known_type(const FullTypeName& name, KnownTypeCode code) {
    const KnownTypeReference* k = KnownTypeReference::get(code);
    return k != nullptr && name == FullTypeName(k->type_name());
}

}  // namespace cb::dotnet::decompiler::typesystem

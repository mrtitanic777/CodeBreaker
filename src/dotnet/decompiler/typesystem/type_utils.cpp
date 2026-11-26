// dotnet/decompiler/typesystem/type_utils.cpp -- see type_utils.hpp.
#include "dotnet/decompiler/typesystem/type_utils.hpp"

#include <optional>

#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::typesystem {

int get_size(const IType& type_in) {
    const IType* type = &type_in;
    switch (type->kind()) {
        case TypeKind::Pointer:
        case TypeKind::ByReference:
        case TypeKind::Class:
        case TypeKind::NInt:
        case TypeKind::NUInt:
            return native_int_size;
        case TypeKind::Enum:
            type = get_enum_underlying_type(*type);
            break;
        case TypeKind::ModOpt:
        case TypeKind::ModReq:
            return get_size(*skip_modifiers(type));
        default:
            break;
    }
    const ITypeDefinition* type_def = type->get_definition();
    if (type_def == nullptr) return 0;
    switch (type_def->known_type_code()) {
        case KnownTypeCode::Boolean:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Byte:
            return 1;
        case KnownTypeCode::Char:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
            return 2;
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32:
        case KnownTypeCode::Single:
            return 4;
        case KnownTypeCode::IntPtr:
        case KnownTypeCode::UIntPtr:
            return native_int_size;
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64:
        case KnownTypeCode::Double:
            return 8;
        default:
            return 0;
    }
}

int get_size(StackType type) {
    switch (type) {
        case StackType::I4: return 4;
        case StackType::I8: return 8;
        case StackType::I:
        case StackType::Ref: return native_int_size;
        default: return 0;
    }
}

const IType* get_larger_type(const IType* type1, const IType* type2) { return get_size(*type1) >= get_size(*type2) ? type1 : type2; }

bool is_small_integer_type(const IType& type) {
    int size = get_size(type);
    return size > 0 && size < 4;
}

bool is_csharp_small_integer_type(const IType& type) {
    const ITypeDefinition* def = type.get_definition();
    if (def == nullptr) return false;
    switch (def->known_type_code()) {
        case KnownTypeCode::Byte:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
            return true;
        default:
            return false;
    }
}

bool is_csharp_native_integer_type(const IType& type) { return type.kind() == TypeKind::NInt || type.kind() == TypeKind::NUInt; }

bool is_csharp_primitive_integer_type(const IType& type) {
    const ITypeDefinition* def = type.get_definition();
    if (def == nullptr) return false;
    switch (def->known_type_code()) {
        case KnownTypeCode::Byte:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32:
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64:
            return true;
        default:
            return false;
    }
}

bool is_integer_type(StackType type) { return type == StackType::I4 || type == StackType::I || type == StackType::I8; }

bool is_float_type(StackType type) { return type == StackType::F4 || type == StackType::F8; }

bool is_compatible_pointer_type_for_memory_access(const IType& pointer_type, const IType& access_type) {
    const IType* memory_type;
    const auto* with_element = dynamic_cast<const TypeWithElementType*>(&pointer_type);
    if (with_element != nullptr && (pointer_type.kind() == TypeKind::Pointer || pointer_type.kind() == TypeKind::ByReference))
        memory_type = with_element->element_type();
    else
        return false;
    return is_compatible_type_for_memory_access(*memory_type, access_type);
}

bool is_compatible_type_for_memory_access(const IType& memory_type_in, const IType& access_type_in) {
    const IType* memory_type = memory_type_in.accept_visitor(NormalizeTypeVisitor::type_erasure());
    const IType* access_type = access_type_in.accept_visitor(NormalizeTypeVisitor::type_erasure());
    if (memory_type->equals(access_type)) return true;
    // If the types are not equal, the access still might produce equal results in some cases:
    // 1) Both types are reference types
    if (memory_type->is_reference_type() == std::optional<bool>(true) && access_type->is_reference_type() == std::optional<bool>(true))
        return true;
    // 2) Both types are integer types of equal size
    StackType memory_stack_type = get_stack_type(*memory_type);
    StackType access_stack_type = get_stack_type(*access_type);
    if (memory_stack_type == access_stack_type && is_integer_type(memory_stack_type) && get_size(*memory_type) == get_size(*access_type))
        return true;
    // 3) Any of the types is unknown: we assume they are compatible.
    return memory_type->kind() == TypeKind::Unknown || access_type->kind() == TypeKind::Unknown;
}

StackType get_stack_type(const IType& type) {
    switch (type.kind()) {
        case TypeKind::Unknown:
            if (type.is_reference_type() == std::optional<bool>(true)) return StackType::O;
            return StackType::Unknown;
        case TypeKind::ByReference:
            return StackType::Ref;
        case TypeKind::Pointer:
        case TypeKind::NInt:
        case TypeKind::NUInt:
        case TypeKind::FunctionPointer:
            return StackType::I;
        case TypeKind::TypeParameter:
            // Type parameters are always considered StackType.O, even though they might be instantiated with primitive types.
            return StackType::O;
        case TypeKind::ModOpt:
        case TypeKind::ModReq:
            return get_stack_type(*skip_modifiers(&type));
        default:
            break;
    }
    const ITypeDefinition* type_def = get_enum_underlying_type(type)->get_definition();
    if (type_def == nullptr) return StackType::O;
    switch (type_def->known_type_code()) {
        case KnownTypeCode::Boolean:
        case KnownTypeCode::Char:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Byte:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32:
            return StackType::I4;
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64:
            return StackType::I8;
        case KnownTypeCode::Single: return StackType::F4;
        case KnownTypeCode::Double: return StackType::F8;
        case KnownTypeCode::Void: return StackType::Void;
        case KnownTypeCode::IntPtr:
        case KnownTypeCode::UIntPtr:
            return StackType::I;
        default:
            return StackType::O;
    }
}

const IType* get_enum_underlying_type(const IType& type_in) {
    const IType* type = skip_modifiers(&type_in);
    return type->kind() == TypeKind::Enum ? type->get_definition()->enum_underlying_type() : type;
}

Sign get_sign(const IType& type_in) {
    const IType* type = skip_modifiers(&type_in);
    switch (type->kind()) {
        case TypeKind::Pointer:
        case TypeKind::NUInt:
        case TypeKind::FunctionPointer:
            return Sign::Unsigned;
        case TypeKind::NInt:
            return Sign::Signed;
        default:
            break;
    }
    const ITypeDefinition* type_def = get_enum_underlying_type(*type)->get_definition();
    if (type_def == nullptr) return Sign::None;
    switch (type_def->known_type_code()) {
        case KnownTypeCode::SByte:
        case KnownTypeCode::Int16:
        case KnownTypeCode::Int32:
        case KnownTypeCode::Int64:
        case KnownTypeCode::IntPtr:
        case KnownTypeCode::Single:
        case KnownTypeCode::Double:
        case KnownTypeCode::Decimal:
            return Sign::Signed;
        case KnownTypeCode::UIntPtr:
        case KnownTypeCode::Char:
        case KnownTypeCode::Boolean:
        case KnownTypeCode::Byte:
        case KnownTypeCode::UInt16:
        case KnownTypeCode::UInt32:
        case KnownTypeCode::UInt64:
            return Sign::Unsigned;
        default:
            return Sign::None;
    }
}

PrimitiveType to_primitive_type(KnownTypeCode known_type_code) {
    switch (known_type_code) {
        case KnownTypeCode::SByte: return PrimitiveType::I1;
        case KnownTypeCode::Int16: return PrimitiveType::I2;
        case KnownTypeCode::Int32: return PrimitiveType::I4;
        case KnownTypeCode::Int64: return PrimitiveType::I8;
        case KnownTypeCode::Single: return PrimitiveType::R4;
        case KnownTypeCode::Double: return PrimitiveType::R8;
        case KnownTypeCode::Byte: return PrimitiveType::U1;
        case KnownTypeCode::UInt16:
        case KnownTypeCode::Char:
            return PrimitiveType::U2;
        case KnownTypeCode::UInt32: return PrimitiveType::U4;
        case KnownTypeCode::UInt64: return PrimitiveType::U8;
        case KnownTypeCode::IntPtr: return PrimitiveType::I;
        case KnownTypeCode::UIntPtr: return PrimitiveType::U;
        default: return PrimitiveType::None;
    }
}

PrimitiveType to_primitive_type(const IType& type_in) {
    const IType* type = skip_modifiers(&type_in);
    switch (type->kind()) {
        case TypeKind::Unknown: return PrimitiveType::Unknown;
        case TypeKind::ByReference: return PrimitiveType::Ref;
        case TypeKind::NInt:
        case TypeKind::FunctionPointer:
            return PrimitiveType::I;
        case TypeKind::NUInt: return PrimitiveType::U;
        default: break;
    }
    const ITypeDefinition* def = get_enum_underlying_type(*type)->get_definition();
    return def != nullptr ? to_primitive_type(def->known_type_code()) : PrimitiveType::None;
}

KnownTypeCode to_known_type_code(PrimitiveType primitive_type) {
    switch (primitive_type) {
        case PrimitiveType::I1: return KnownTypeCode::SByte;
        case PrimitiveType::I2: return KnownTypeCode::Int16;
        case PrimitiveType::I4: return KnownTypeCode::Int32;
        case PrimitiveType::I8: return KnownTypeCode::Int64;
        case PrimitiveType::R4: return KnownTypeCode::Single;
        case PrimitiveType::R8:
        case PrimitiveType::R:
            return KnownTypeCode::Double;
        case PrimitiveType::U1: return KnownTypeCode::Byte;
        case PrimitiveType::U2: return KnownTypeCode::UInt16;
        case PrimitiveType::U4: return KnownTypeCode::UInt32;
        case PrimitiveType::U8: return KnownTypeCode::UInt64;
        case PrimitiveType::I: return KnownTypeCode::IntPtr;
        case PrimitiveType::U: return KnownTypeCode::UIntPtr;
        default: return KnownTypeCode::None;
    }
}

KnownTypeCode to_known_type_code(StackType stack_type, Sign sign) {
    switch (stack_type) {
        case StackType::I4: return sign == Sign::Unsigned ? KnownTypeCode::UInt32 : KnownTypeCode::Int32;
        case StackType::I8: return sign == Sign::Unsigned ? KnownTypeCode::UInt64 : KnownTypeCode::Int64;
        case StackType::I: return sign == Sign::Unsigned ? KnownTypeCode::UIntPtr : KnownTypeCode::IntPtr;
        case StackType::F4: return KnownTypeCode::Single;
        case StackType::F8: return KnownTypeCode::Double;
        case StackType::O: return KnownTypeCode::Object;
        case StackType::Void: return KnownTypeCode::Void;
        default: return KnownTypeCode::None;
    }
}

PrimitiveType to_primitive_type(StackType stack_type, Sign sign) {
    switch (stack_type) {
        case StackType::I4: return sign == Sign::Unsigned ? PrimitiveType::U4 : PrimitiveType::I4;
        case StackType::I8: return sign == Sign::Unsigned ? PrimitiveType::U8 : PrimitiveType::I8;
        case StackType::I: return sign == Sign::Unsigned ? PrimitiveType::U : PrimitiveType::I;
        case StackType::F4: return PrimitiveType::R4;
        case StackType::F8: return PrimitiveType::R8;
        case StackType::Ref: return PrimitiveType::Ref;
        case StackType::Unknown: return PrimitiveType::Unknown;
        default: return PrimitiveType::None;
    }
}

}  // namespace cb::dotnet::decompiler::typesystem

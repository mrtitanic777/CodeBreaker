// dotnet/decompiler/typesystem/type_utils.hpp -- what the type system and the ILAst ask about a type's representation: its size, its stack
// type, its sign, the primitive type and known type code that stand for it, and whether memory of one type can be read as another.
//
//     StackType st = get_stack_type(*type);                 // I4 for int, bool, char and small-integer enums; O for classes ...
//     PrimitiveType p = to_primitive_type(*type);           // I4, U2 ... None for what is not primitive
//     bool compatible = is_compatible_type_for_memory_access(*memory_type, *access_type);
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TypeUtils, Sign, from TypeSystem/TypeUtils.cs), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <cstdint>

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

using il::PrimitiveType;
using il::StackType;

enum class Sign : std::uint8_t { None, Signed, Unsigned };

// between 4 (Int32) and 8 (Int64)
inline constexpr int native_int_size = 6;

// The size in bytes; native_int_size for pointer-sized types, 0 for structs and other types of unknown size.
int get_size(const IType& type);
// 4 for I4, 8 for I8, native_int_size for I and Ref, 0 otherwise (O, floats, Void, Unknown).
int get_size(StackType type);
// The type of the larger size (the first when equal).
const IType* get_larger_type(const IType* type1, const IType* type2);
// bool, sbyte, byte, char, short, ushort and the enums over them.
bool is_small_integer_type(const IType& type);
// byte, sbyte, short or ushort (not bool, char or enums, as the ILAst has it)
bool is_csharp_small_integer_type(const IType& type);
// nint or nuint (not IntPtr)
bool is_csharp_native_integer_type(const IType& type);
// byte, sbyte, short, ushort, int, uint, long or ulong
bool is_csharp_primitive_integer_type(const IType& type);
// I4, I or I8
bool is_integer_type(StackType type);
// F4 or F8
bool is_float_type(StackType type);
// Whether reading or writing an element of access_type through a pointer to pointer_type's element is the same as through its own
// element type (small integers of different signs read slightly differently).
bool is_compatible_pointer_type_for_memory_access(const IType& pointer_type, const IType& access_type);
bool is_compatible_type_for_memory_access(const IType& memory_type, const IType& access_type);
StackType get_stack_type(const IType& type);
// The underlying type of an enum, the type itself for others (modifiers skipped).
const IType* get_enum_underlying_type(const IType& type);
// Integers by their sign; floats and decimal signed; char, bool and pointers unsigned; enums by their underlying type; others None.
Sign get_sign(const IType& type);
PrimitiveType to_primitive_type(KnownTypeCode known_type_code);
PrimitiveType to_primitive_type(const IType& type);
KnownTypeCode to_known_type_code(PrimitiveType primitive_type);
KnownTypeCode to_known_type_code(StackType stack_type, Sign sign = Sign::None);
PrimitiveType to_primitive_type(StackType stack_type, Sign sign = Sign::None);

}  // namespace cb::dotnet::decompiler::typesystem

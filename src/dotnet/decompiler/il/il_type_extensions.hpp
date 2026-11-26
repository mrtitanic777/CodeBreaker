// dotnet/decompiler/il/il_type_extensions.hpp -- the questions the ILAst asks about primitive types (stack type, sign, size, integer or
// float), and the C# type an instruction is known to produce. The same questions about IType and StackType (typesystem/type_utils.hpp)
// are brought into this namespace, so `get_stack_type(*type)` and `get_stack_type(PrimitiveType::I4)` are both written here.
//
//     StackType st = get_stack_type(PrimitiveType::U2);      // I4
//     int size = get_size(PrimitiveType::I8);                 // 8
//     const IType* t = infer_type(inst, &compilation);        // the type of a call's result, a local, a field address ...
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ILTypeExtensions, from ILTypeExtensions.cs), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/il/il_instruction.hpp"
#include "dotnet/decompiler/typesystem/type_utils.hpp"

namespace cb::dotnet::decompiler::typesystem {
class ICompilation;
}

namespace cb::dotnet::decompiler::il {

using typesystem::ICompilation;
using typesystem::KnownTypeCode;
using typesystem::Sign;

using typesystem::get_enum_underlying_type;
using typesystem::get_larger_type;
using typesystem::get_sign;
using typesystem::get_size;
using typesystem::get_stack_type;
using typesystem::is_compatible_pointer_type_for_memory_access;
using typesystem::is_compatible_type_for_memory_access;
using typesystem::is_csharp_native_integer_type;
using typesystem::is_csharp_primitive_integer_type;
using typesystem::is_csharp_small_integer_type;
using typesystem::is_float_type;
using typesystem::is_integer_type;
using typesystem::is_small_integer_type;
using typesystem::native_int_size;
using typesystem::to_known_type_code;
using typesystem::to_primitive_type;

StackType get_stack_type(PrimitiveType primitive_type);
Sign get_sign(PrimitiveType primitive_type);
bool has_opposite_sign(PrimitiveType primitive_type);
// The size in bytes; 0 for non-primitive types, native_int_size for native int and references.
int get_size(PrimitiveType type);
// bool, sbyte, byte, char, short, ushort and the enums over them.
bool is_small_integer_type(PrimitiveType type);
bool is_integer_type(PrimitiveType primitive_type);
bool is_float_type(PrimitiveType type);

// The C# type an instruction is known to produce; SpecialType::unknown_type() for the ones this cannot tell. Without a compilation,
// the types that need one (arrays, comparisons) are unknown.
const IType* infer_type(ILInstruction* inst, const ICompilation* compilation);

}  // namespace cb::dotnet::decompiler::il

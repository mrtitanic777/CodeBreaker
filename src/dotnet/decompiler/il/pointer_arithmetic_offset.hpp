// dotnet/decompiler/il/pointer_arithmetic_offset.hpp -- the analysis of the right-hand side of a `ptr + int` or `ptr - int`
// operation: the offset in bytes turned back into a number of elements, the size of the primitive types and whether an address is
// of a fixed variable.
//
//     ILInstruction* count = PointerArithmeticOffset::detect(byte_offset, element_type, false);   // nullptr when it is no multiple
//     std::optional<int> size = PointerArithmeticOffset::compute_size_of(*int32_type);             // 4
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: PointerArithmeticOffset, from PointerArithmeticOffset.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <optional>

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::il {

struct PointerArithmeticOffset {
    // Given an instruction that computes a pointer arithmetic offset in bytes, returns an instruction that computes the same offset in
    // number of elements; nullptr if no such instruction can be found. `unwrap_zero_extension` allows zero extensions in the mul
    // argument.
    static ILInstruction* detect(ILInstruction* byte_offset_inst, const IType* pointer_element_type, bool check_for_overflow,
                                 bool unwrap_zero_extension = false);

    // The size in bytes of the primitive types (and enums over them) and decimal; nothing for the others.
    static std::optional<int> compute_size_of(const IType& type);

    // Whether `inst` computes the address of a fixed variable; false if it computes the address of a moveable variable (see "Fixed
    // and moveable variables" in the C# specification).
    static bool is_fixed_variable(ILInstruction* inst);
};

}  // namespace cb::dotnet::decompiler::il

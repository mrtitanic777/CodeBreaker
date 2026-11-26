// dotnet/decompiler/il/il_instruction_extensions.hpp -- small helpers on ILInstruction: add an IL range to a node and give the node
// back, and find a node's next sibling.
//
//     auto* one = with_il_range(make<LdcI4>(1), byte_offset_inst);   // an ldc.i4 covering the IL of byte_offset_inst
//     ILInstruction* next = get_next_sibling(inst);                  // nullptr for the last child or a root
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ILInstructionExtensions, from ILInstructionExtensions.cs), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include "dotnet/decompiler/il/il_instruction.hpp"

namespace cb::dotnet::decompiler::il {

// `target` after its IL range was extended by `source_instruction`'s.
template <class T>
T* with_il_range(T* target, const ILInstruction& source_instruction) {
    target->add_il_range(source_instruction);
    return target;
}
template <class T>
T* with_il_range(T* target, const ILInstruction* source_instruction) {
    target->add_il_range(*source_instruction);
    return target;
}

// `target` after its IL range was extended by `range`.
template <class T>
T* with_il_range(T* target, Interval range) {
    target->add_il_range(range);
    return target;
}

// The instruction after `instruction` in its parent, or nullptr (also for a null instruction or one without a parent).
ILInstruction* get_next_sibling(ILInstruction* instruction);

}  // namespace cb::dotnet::decompiler::il

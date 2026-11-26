// dotnet/decompiler/il/instruction_output_il.hpp -- the ILAst half of InstructionOutputExtensions: an opcode, stack type or primitive
// type written as the ILAst names it (`ldc.i4`, `i4`, `u2`), a type by its reflection name, a member by its name, an IL range.
//
//     write(output, OpCode::LdcI4);       // "ldc.i4"
//     write_to(*type, output);            // "System.Collections.Generic.List`1[[System.Int32]]"
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: InstructionOutputExtensions, the half for ILAst opcodes, stack types, IType, IMember
// and Interval, with Instructions.cs's originalOpCodeNames), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <string_view>

#include "dotnet/decompiler/il/instructions_base.hpp"

namespace cb::dotnet::decompiler::il {

std::string_view opcode_name(OpCode op_code);
// C#'s ToString() of the enum values (`I4`, `Ref` ...).
std::string_view to_string(StackType stack_type);
std::string_view to_string(PrimitiveType primitive_type);

void write(ITextOutput& output, OpCode op_code);
// lower-case, as upstream's ToString().ToLowerInvariant()
void write(ITextOutput& output, StackType stack_type);
void write(ITextOutput& output, PrimitiveType primitive_type);
void write_to(const IType& type, ITextOutput& output);
void write_to(const IMember& member, ITextOutput& output);
void write_to(Interval interval, ITextOutput& output, const ILAstWritingOptions& options);

}  // namespace cb::dotnet::decompiler::il

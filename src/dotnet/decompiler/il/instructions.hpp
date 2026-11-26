// dotnet/decompiler/il/instructions.hpp -- every ILAst instruction class (include this, not the family headers).
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: Instructions.cs and Instructions/*.cs), Copyright AlphaSierraPapa for the ILSpy team,
// MIT (see NOTICE in dotnet/decompiler). The families: instructions_base (writing options, Simple/Unary/BinaryInstruction),
// control_flow_instructions (functions, containers, blocks, branches, if, switch, try, lock, using, yield, await, nop, invalid),
// expression_instructions (arithmetic, comparison, conversion, calls, locals, constants, delegates, nullable), memory_instructions
// (fields, objects, arrays, pointers, boxing, typed references, throw), advanced_instructions (compound assignments, dynamic, user
// logic operators, pattern matching, deconstruction), patterns (AnyNode, Match, ListMatch).
#pragma once

#include "dotnet/decompiler/il/instructions_base.hpp"
#include "dotnet/decompiler/il/il_variable.hpp"
#if __has_include("dotnet/decompiler/il/control_flow_instructions.hpp")
#include "dotnet/decompiler/il/control_flow_instructions.hpp"
#endif
#if __has_include("dotnet/decompiler/il/expression_instructions.hpp")
#include "dotnet/decompiler/il/expression_instructions.hpp"
#endif
#if __has_include("dotnet/decompiler/il/memory_instructions.hpp")
#include "dotnet/decompiler/il/memory_instructions.hpp"
#endif
#if __has_include("dotnet/decompiler/il/advanced_instructions.hpp")
#include "dotnet/decompiler/il/advanced_instructions.hpp"
#endif
#if __has_include("dotnet/decompiler/il/patterns.hpp")
#include "dotnet/decompiler/il/patterns.hpp"
#endif

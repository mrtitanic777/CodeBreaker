// dotnet/decompiler/metadata/il_opcodes.hpp -- what ILSpy knows about each CIL opcode: its operand's type, its display name, and the words
// ILAsm treats as keywords (a name that is one is quoted when written).
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: OperandType, ILOpCodeExtensions in OperandType.cs and the generated ILOpCodes.cs),
// Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <string>
#include <string_view>
#include <unordered_set>

#include "dotnet/metadata/il_opcode.hpp"

namespace cb::dotnet::decompiler::metadata {

using dotnet::metadata::ILOpCode;

enum class OperandType : std::uint8_t {
    BrTarget, Field, I, I8, Method, None, R = 7, Sig = 9, String, Switch, Tok, Type, Variable, ShortBrTarget, ShortI, ShortR, ShortVariable,
};
// What get_operand_type gives for an opcode that does not exist (upstream's (OperandType)255).
inline constexpr OperandType kNoOperand = static_cast<OperandType>(255);

OperandType get_operand_type(ILOpCode op);
// "ldarg.0", "br.s" ...; "" for an opcode that does not exist.
std::string_view get_display_name(ILOpCode op);
bool is_defined(ILOpCode op);
// The ILAsm keywords and every instruction name.
const std::unordered_set<std::string>& il_keywords();

}  // namespace cb::dotnet::decompiler::metadata

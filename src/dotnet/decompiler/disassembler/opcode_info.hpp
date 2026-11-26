// dotnet/decompiler/disassembler/opcode_info.hpp -- an opcode and its name, and the name of its page in the .NET documentation.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: OpCodeInfo), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler).
#pragma once

#include <string>

#include "dotnet/metadata/il_opcode.hpp"

namespace cb::dotnet::decompiler::disassembler {

struct OpCodeInfo {
    dotnet::metadata::ILOpCode code{};
    std::string name;

    OpCodeInfo(dotnet::metadata::ILOpCode code_, std::string name_) : code(code_), name(std::move(name_)) {}
    // The System.Reflection.Emit.OpCodes field it is ("Ldarg_0"; the prefixes by their own names).
    std::string encoded_name() const;
    // https://docs.microsoft.com/dotnet/api/system.reflection.emit.opcodes.<field>
    std::string link() const;
    friend bool operator==(const OpCodeInfo&, const OpCodeInfo&) = default;
};

}  // namespace cb::dotnet::decompiler::disassembler

// dotnet/decompiler/disassembler/opcode_info.cpp -- OpCodeInfo's documentation names.
#include "dotnet/decompiler/disassembler/opcode_info.hpp"

#include <cctype>

namespace cb::dotnet::decompiler::disassembler {

std::string OpCodeInfo::encoded_name() const {
    if (name == "constrained.") return "Constrained";
    if (name == "no.") return "No";
    if (name == "readonly.") return "Reaonly";   // (upstream's spelling)
    if (name == "tail.") return "Tailcall";
    if (name == "unaligned.") return "Unaligned";
    if (name == "volatile.") return "Volatile";
    std::string text;
    bool upper = true;
    for (const char ch : name) {
        if (ch == '.') {
            text += '_';
            upper = true;
        } else if (upper) {
            text += static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            upper = false;
        } else {
            text += ch;
        }
    }
    return text;
}

std::string OpCodeInfo::link() const {
    std::string lower = encoded_name();
    for (char& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return "https://docs.microsoft.com/dotnet/api/system.reflection.emit.opcodes." + lower;
}

}  // namespace cb::dotnet::decompiler::disassembler

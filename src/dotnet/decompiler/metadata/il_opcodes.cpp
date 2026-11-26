// dotnet/decompiler/metadata/il_opcodes.cpp -- the opcode tables (generated upstream from ILOpCodes.tt; copied here as data).
#include "dotnet/decompiler/metadata/il_opcodes.hpp"

#include <array>

namespace cb::dotnet::decompiler::metadata {
namespace {

using T = OperandType;

// By index: a one-byte opcode is its byte, a two-byte one (0xFExx) is 0x100 + xx.
constexpr OperandType kOperandTypes[] = {
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None,
    T::ShortVariable, T::ShortVariable, T::ShortVariable, T::ShortVariable, T::ShortVariable, T::ShortVariable, T::None, T::None,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::ShortI, T::I, T::I8, T::ShortR, T::R,
    kNoOperand, T::None, T::None, T::Method, T::Method, T::Sig, T::None, T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget,
    T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget,
    T::ShortBrTarget, T::ShortBrTarget, T::ShortBrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget,
    T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::BrTarget, T::Switch, T::None,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::Method,
    T::Type, T::Type, T::String, T::Method, T::Type, T::Type, T::None, kNoOperand, kNoOperand, T::Type, T::None, T::Field,
    T::Field, T::Field, T::Field, T::Field, T::Field, T::Type, T::None, T::None, T::None, T::None, T::None, T::None, T::None,
    T::None, T::None, T::None, T::Type, T::Type, T::None, T::Type, T::None, T::None, T::None, T::None, T::None, T::None, T::None,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::Type, T::Type,
    T::Type, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand,
    kNoOperand, kNoOperand, kNoOperand, kNoOperand, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None,
    kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, T::Type, T::None, kNoOperand, kNoOperand,
    T::Type, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, T::Tok,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::BrTarget,
    T::ShortBrTarget, T::None, T::None, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand,
    kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand,
    kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, kNoOperand, T::None, T::None, T::None, T::None, T::None, T::None,
    T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::None, T::Method, T::Method, kNoOperand, T::Variable,
    T::Variable, T::Variable, T::Variable, T::Variable, T::Variable, T::None, kNoOperand, T::None, T::ShortI, T::None, T::None,
    T::Type, T::Type, T::None, T::None, kNoOperand, T::None, kNoOperand, T::Type, T::None, T::None,
};

constexpr std::string_view kOperandNames[] = {
    "nop", "break", "ldarg.0", "ldarg.1", "ldarg.2", "ldarg.3", "ldloc.0", "ldloc.1", "ldloc.2", "ldloc.3", "stloc.0", "stloc.1",
    "stloc.2", "stloc.3", "ldarg.s", "ldarga.s", "starg.s", "ldloc.s", "ldloca.s", "stloc.s", "ldnull", "ldc.i4.m1", "ldc.i4.0",
    "ldc.i4.1", "ldc.i4.2", "ldc.i4.3", "ldc.i4.4", "ldc.i4.5", "ldc.i4.6", "ldc.i4.7", "ldc.i4.8", "ldc.i4.s", "ldc.i4",
    "ldc.i8", "ldc.r4", "ldc.r8", "", "dup", "pop", "jmp", "call", "calli", "ret", "br.s", "brfalse.s", "brtrue.s", "beq.s",
    "bge.s", "bgt.s", "ble.s", "blt.s", "bne.un.s", "bge.un.s", "bgt.un.s", "ble.un.s", "blt.un.s", "br", "brfalse", "brtrue",
    "beq", "bge", "bgt", "ble", "blt", "bne.un", "bge.un", "bgt.un", "ble.un", "blt.un", "switch", "ldind.i1", "ldind.u1",
    "ldind.i2", "ldind.u2", "ldind.i4", "ldind.u4", "ldind.i8", "ldind.i", "ldind.r4", "ldind.r8", "ldind.ref", "stind.ref",
    "stind.i1", "stind.i2", "stind.i4", "stind.i8", "stind.r4", "stind.r8", "add", "sub", "mul", "div", "div.un", "rem",
    "rem.un", "and", "or", "xor", "shl", "shr", "shr.un", "neg", "not", "conv.i1", "conv.i2", "conv.i4", "conv.i8", "conv.r4",
    "conv.r8", "conv.u4", "conv.u8", "callvirt", "cpobj", "ldobj", "ldstr", "newobj", "castclass", "isinst", "conv.r.un", "", "",
    "unbox", "throw", "ldfld", "ldflda", "stfld", "ldsfld", "ldsflda", "stsfld", "stobj", "conv.ovf.i1.un", "conv.ovf.i2.un",
    "conv.ovf.i4.un", "conv.ovf.i8.un", "conv.ovf.u1.un", "conv.ovf.u2.un", "conv.ovf.u4.un", "conv.ovf.u8.un", "conv.ovf.i.un",
    "conv.ovf.u.un", "box", "newarr", "ldlen", "ldelema", "ldelem.i1", "ldelem.u1", "ldelem.i2", "ldelem.u2", "ldelem.i4",
    "ldelem.u4", "ldelem.i8", "ldelem.i", "ldelem.r4", "ldelem.r8", "ldelem.ref", "stelem.i", "stelem.i1", "stelem.i2",
    "stelem.i4", "stelem.i8", "stelem.r4", "stelem.r8", "stelem.ref", "ldelem", "stelem", "unbox.any", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "conv.ovf.i1", "conv.ovf.u1", "conv.ovf.i2", "conv.ovf.u2", "conv.ovf.i4", "conv.ovf.u4",
    "conv.ovf.i8", "conv.ovf.u8", "", "", "", "", "", "", "", "refanyval", "ckfinite", "", "", "mkrefany", "", "", "", "", "",
    "", "", "", "", "ldtoken", "conv.u2", "conv.u1", "conv.i", "conv.ovf.i", "conv.ovf.u", "add.ovf", "add.ovf.un", "mul.ovf",
    "mul.ovf.un", "sub.ovf", "sub.ovf.un", "endfinally", "leave", "leave.s", "stind.i", "conv.u", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "prefix7", "prefix6", "prefix5", "prefix4", "prefix3", "prefix2",
    "prefix1", "prefixref", "arglist", "ceq", "cgt", "cgt.un", "clt", "clt.un", "ldftn", "ldvirtftn", "", "ldarg", "ldarga",
    "starg", "ldloc", "ldloca", "stloc", "localloc", "", "endfilter", "unaligned.", "volatile.", "tail.", "initobj",
    "constrained.", "cpblk", "initblk", "", "rethrow", "", "sizeof", "refanytype", "readonly.",
};

int index_of(ILOpCode op) {
    const int v = static_cast<int>(op);
    return ((v & 0x200) >> 1) | (v & 0xFF);
}

}  // namespace

OperandType get_operand_type(ILOpCode op) {
    const int i = index_of(op);
    return i < static_cast<int>(std::size(kOperandTypes)) ? kOperandTypes[i] : kNoOperand;
}

std::string_view get_display_name(ILOpCode op) {
    const int i = index_of(op);
    return i < static_cast<int>(std::size(kOperandNames)) ? kOperandNames[i] : std::string_view();
}

bool is_defined(ILOpCode op) { return !get_display_name(op).empty(); }

const std::unordered_set<std::string>& il_keywords() {
    static const std::unordered_set<std::string> keywords = [] {
        std::unordered_set<std::string> s = {
            "abstract", "algorithm", "alignment", "ansi", "any", "arglist", "array", "as", "assembly", "assert", "at", "auto", "autochar",
            "beforefieldinit", "blob", "blob_object", "bool", "brnull", "brnull.s", "brzero", "brzero.s", "bstr", "bytearray", "byvalstr",
            "callmostderived", "carray", "catch", "cdecl", "cf", "char", "cil", "class", "clsid", "const", "currency", "custom", "date",
            "decimal", "default", "demand", "deny", "endmac", "enum", "error", "explicit", "extends", "extern", "false", "famandassem",
            "family", "famorassem", "fastcall", "fault", "field", "filetime", "filter", "final", "finally", "fixed", "float", "float32",
            "float64", "forwardref", "fromunmanaged", "handler", "hidebysig", "hresult", "idispatch", "il", "illegal", "implements",
            "implicitcom", "implicitres", "import", "in", "inheritcheck", "init", "initonly", "instance", "int", "int16", "int32", "int64",
            "int8", "interface", "internalcall", "iunknown", "lasterr", "lcid", "linkcheck", "literal", "localloc", "lpstr", "lpstruct",
            "lptstr", "lpvoid", "lpwstr", "managed", "marshal", "method", "modopt", "modreq", "native", "nested", "newslot", "noappdomain",
            "noinlining", "nomachine", "nomangle", "nometadata", "noncasdemand", "noncasinheritance", "noncaslinkdemand", "noprocess", "not",
            "not_in_gc_heap", "notremotable", "notserialized", "null", "nullref", "object", "objectref", "opt", "optil", "out", "permitonly",
            "pinned", "pinvokeimpl", "prefix1", "prefix2", "prefix3", "prefix4", "prefix5", "prefix6", "prefix7", "prefixref", "prejitdeny",
            "prejitgrant", "preservesig", "private", "privatescope", "protected", "public", "record", "refany", "reqmin", "reqopt",
            "reqrefuse", "reqsecobj", "request", "retval", "rtspecialname", "runtime", "safearray", "sealed", "sequential", "serializable",
            "special", "specialname", "static", "stdcall", "storage", "stored_object", "stream", "streamed_object", "string", "struct",
            "synchronized", "syschar", "sysstring", "tbstr", "thiscall", "tls", "to", "true", "typedref", "unicode", "unmanaged",
            "unmanagedexp", "unsigned", "unused", "userdefined", "value", "valuetype", "vararg", "variant", "vector", "virtual", "void",
            "wchar", "winapi", "with", "wrapper",
            // not listed as keywords in the standard, but ILAsm treats them as such
            "property", "type", "flags", "codelabel", "callconv", "strict",
            // ILDasm's unsigned integers
            "uint8", "uint16", "uint32", "uint64",
        };
        for (const std::string_view n : kOperandNames)
            if (!n.empty()) s.emplace(n);
        return s;
    }();
    return keywords;
}

}  // namespace cb::dotnet::decompiler::metadata

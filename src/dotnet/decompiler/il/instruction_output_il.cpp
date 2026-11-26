// dotnet/decompiler/il/instruction_output_il.cpp -- see instruction_output_il.hpp.
#include "dotnet/decompiler/il/instruction_output_il.hpp"

#include <array>
#include <cctype>
#include <format>
#include <string>

#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il {

namespace {
constexpr std::array<std::string_view, opcode_count> original_opcode_names = {
    "invalid.branch", "invalid.expr", "nop", "ILFunction", "BlockContainer", "Block", "PinnedRegion", "binary", "numeric.compound",
    "user.compound", "dynamic.compound", "bit.not", "arglist", "br", "leave", "if", "if.notnull", "switch", "switch.section", "try.catch",
    "try.catch.handler", "try.finally", "try.fault", "lock", "using", "debug.break", "comp", "call", "callvirt", "calli", "ckfinite", "conv",
    "ldloc", "ldloca", "stloc", "addressof", "3vl.bool.and", "3vl.bool.or", "nullable.unwrap", "nullable.rewrap", "ldstr", "ldstr.utf8",
    "ldc.i4", "ldc.i8", "ldc.f4", "ldc.f8", "ldc.decimal", "ldnull", "ldftn", "ldvirtftn", "ldvirtdelegate", "ldtypetoken",
    "ldmembertoken", "localloc", "localloc.span", "cpblk", "initblk", "ldflda", "ldsflda", "castclass", "isinst", "ldobj", "ldobj.if.ref",
    "stobj", "box", "unbox", "unbox.any", "newobj", "newarr", "default.value", "throw", "rethrow", "sizeof", "ldlen", "ldelema",
    "ldelema.inlinearray", "get.pinnable.reference", "string.to.int", "expression.tree.cast", "user.logic.operator",
    "dynamic.logic.operator", "dynamic.binary.operator", "dynamic.unary.operator", "dynamic.convert", "dynamic.getmember",
    "dynamic.setmember", "dynamic.getindex", "dynamic.setindex", "dynamic.invokemember", "dynamic.invokeconstructor", "dynamic.invoke",
    "dynamic.isevent", "match", "mkrefany", "refanytype", "refanyval", "yield.return", "await", "deconstruct", "deconstruct.result",
    "AnyNode",
};

void write_lower(ITextOutput& output, std::string_view text) {
    std::string lower(text);
    for (char& c : lower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    output.write(lower);
}
}  // namespace

std::string_view opcode_name(OpCode op_code) { return original_opcode_names[static_cast<std::size_t>(op_code)]; }

std::string_view to_string(StackType stack_type) {
    switch (stack_type) {
        case StackType::Unknown: return "Unknown";
        case StackType::I4: return "I4";
        case StackType::I: return "I";
        case StackType::I8: return "I8";
        case StackType::F4: return "F4";
        case StackType::F8: return "F8";
        case StackType::O: return "O";
        case StackType::Ref: return "Ref";
        case StackType::Void: return "Void";
    }
    return "?";
}

std::string_view to_string(PrimitiveType primitive_type) {
    switch (primitive_type) {
        case PrimitiveType::None: return "None";
        case PrimitiveType::I1: return "I1";
        case PrimitiveType::I2: return "I2";
        case PrimitiveType::I4: return "I4";
        case PrimitiveType::I8: return "I8";
        case PrimitiveType::R4: return "R4";
        case PrimitiveType::R8: return "R8";
        case PrimitiveType::U1: return "U1";
        case PrimitiveType::U2: return "U2";
        case PrimitiveType::U4: return "U4";
        case PrimitiveType::U8: return "U8";
        case PrimitiveType::I: return "I";
        case PrimitiveType::U: return "U";
        case PrimitiveType::Ref: return "Ref";
        case PrimitiveType::R: return "R";
        case PrimitiveType::Unknown: return "Unknown";
    }
    return "?";
}

void write(ITextOutput& output, OpCode op_code) { output.write(opcode_name(op_code)); }
void write(ITextOutput& output, StackType stack_type) { write_lower(output, to_string(stack_type)); }
void write(ITextOutput& output, PrimitiveType primitive_type) { write_lower(output, to_string(primitive_type)); }

void write_to(const IType& type, ITextOutput& output) { output.write_reference(type, type.reflection_name()); }

void write_to(const IMember& member, ITextOutput& output) {
    auto* method = dynamic_cast<const typesystem::IMethod*>(&member);
    if (method != nullptr && method->is_constructor()) {
        const IType* declaring = method->declaring_type();
        output.write_reference(member, (declaring != nullptr ? declaring->name() : std::string()) + "." + method->name());
    } else {
        output.write_reference(member, member.name());
    }
}

void write_to(Interval interval, ITextOutput& output, const ILAstWritingOptions& options) {
    if (!options.show_il_ranges) return;
    if (interval.is_empty())
        output.write("[empty] ");
    else
        output.write(std::format("[{:04x}..{:04x}] ", interval.start, interval.inclusive_end()));
}

void ILInstruction::write_il_range(ITextOutput& output, const ILAstWritingOptions& options) const { il::write_to(il_range_, output, options); }

}  // namespace cb::dotnet::decompiler::il

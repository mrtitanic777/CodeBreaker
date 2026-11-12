#include "cb/recompile/builders/reading.hpp"

#include <format>

#include "cb/core/error.hpp"

namespace cb::recompile::builders {
namespace {

[[noreturn]] void refuse_operand(const Instruction& insn, std::size_t index, std::string_view wanted) {
    const std::string what = index < insn.operands.size() ? "operand '" + insn.operands[index] + "' is not " + std::string(wanted)
                                                          : "operand " + std::to_string(index + 1) + " is missing";
    throw ParseError(insn.mnemonic, 0, static_cast<std::uint32_t>(index + 1), what);
}

template <typename Read>
auto read_operand(const Instruction& insn, std::size_t index, std::string_view wanted, Read read) {
    if (index >= insn.operands.size()) refuse_operand(insn, index, wanted);
    const auto value = read(insn.operands[index]);
    if (!value) refuse_operand(insn, index, wanted);
    return *value;
}

}  // namespace

int gpr_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    return read_operand(insn, index, "a general register", [&](const std::string& t) { return ops.reg(t); });
}
int fpr_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    return read_operand(insn, index, "a floating-point register", [&](const std::string& t) { return ops.freg(t); });
}
int vpr_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    return read_operand(insn, index, "a vector register", [&](const std::string& t) { return ops.vreg(t); });
}
int crf_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    return read_operand(insn, index, "a condition register field", [&](const std::string& t) { return ops.cr(t); });
}
std::int64_t imm_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    return read_operand(insn, index, "an immediate", [&](const std::string& t) { return ops.imm(t); });
}
MemoryOperand mem_at(const Instruction& insn, const Operands& ops, std::size_t index) {
    return read_operand(insn, index, "a memory operand", [&](const std::string& t) { return ops.mem(t); });
}

std::vector<std::string> fill_lines(std::string_view lines, const Slots& slots) {
    std::vector<std::string> out;
    std::string line;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const char ch = lines[k];
        if (ch == '\n') {
            out.push_back(std::move(line));
            line.clear();
        } else if (ch == '{' && k + 2 < lines.size() && lines[k + 2] == '}') {
            switch (lines[k + 1]) {
                case 'd': line += std::to_string(slots.d); break;
                case 'a': line += std::to_string(slots.a); break;
                case 'b': line += std::to_string(slots.b); break;
                case 'c': line += std::to_string(slots.c); break;
                case 'i': line += slots.i; break;
                case 'j': line += slots.j; break;
                case 'k': line += slots.k; break;
                case 'p': line += slots.p; break;
                case 'q': line += slots.q; break;
                case 'r': line += slots.r; break;
                default: line += slots.z; break;   // 'z'
            }
            k += 2;
        } else {
            line += ch;
        }
    }
    out.push_back(std::move(line));
    return out;
}

std::string hex(std::uint64_t value) { return std::format("0x{:X}", value); }

}  // namespace cb::recompile::builders

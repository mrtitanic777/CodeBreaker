// codebreaker/symbols/x86.hpp -- platforms.decoder(bits, "x86", detail), as the symbols modules use it (api_arguments,
// runtime_library, names): its instructions as plain records.
//
// The decoder is platforms::decoder (platforms/decoder.cpp), over the capstone.dll found as `import capstone` finds it;
// set_capstone_dll() names another one.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "py.hpp"

namespace codebreaker::symbols::x86 {

void set_capstone_dll(const py::str& path);

// One decoded instruction (capstone's CsInsn; disp/imm fields only for a decoder made with detail=true).
struct Insn {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
    int64_t disp = 0, disp_size = 0, disp_offset = 0;
    int64_t imm_size = 0, imm_offset = 0;
    bool first_is_imm = false;      // operands[0].type == X86_OP_IMM
    int64_t first_imm = 0;          // operands[0].imm
};

// md.disasm(code, address[, count]) (count 0: to the end); `bits` 16 or 32
std::vector<Insn> disasm(int bits, bool detail, const std::string& code, int64_t address, size_t count = 0);

// md.disasm_lite(code, address): (address, size, mnemonic, op_str) of the instructions
struct Lite {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
};
std::vector<Lite> disasm_lite(int bits, const std::string& code, int64_t address);
// the first instruction only: `next(md.disasm_lite(code, address), None)`
std::optional<Lite> first_lite(int bits, const std::string& code, int64_t address);

}  // namespace codebreaker::symbols::x86

// codebreaker/symbols/x86.cpp -- see x86.hpp.

#include "codebreaker/symbols/x86.hpp"

#include <capstone/capstone.h>

#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/platforms/package.hpp"

namespace codebreaker::symbols::x86 {

namespace {

Insn make_(const cs_insn& c, bool detail) {
    Insn out;
    out.address = static_cast<int64_t>(c.address);
    out.size = c.size;
    out.mnemonic = c.mnemonic;
    out.op_str = c.op_str;
    if (detail && c.detail) {
        const cs_x86& x = c.detail->x86;
        out.disp = x.disp;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        if (x.op_count > 0 && x.operands[0].type == X86_OP_IMM) {
            out.first_is_imm = true;
            out.first_imm = x.operands[0].imm;
        }
    }
    return out;
}

}  // namespace

void set_capstone_dll(const py::str& path) { emulate::capstone_x86::load_capstone(path); }

std::vector<Insn> disasm(int bits, bool detail, const std::string& code, int64_t address, size_t count) {
    std::vector<Insn> out;
    platforms::decoder(bits, py::str("x86"), detail)->each(code, address, count, [&](const cs_insn& c) {
        out.push_back(make_(c, detail));
        return true;
    });
    return out;
}

std::vector<Lite> disasm_lite(int bits, const std::string& code, int64_t address) {
    std::vector<Lite> out;
    for (const Insn& i : disasm(bits, false, code, address)) out.push_back({i.address, i.size, i.mnemonic, i.op_str});
    return out;
}

std::optional<Lite> first_lite(int bits, const std::string& code, int64_t address) {
    std::vector<Insn> v = disasm(bits, false, code, address, 1);
    if (v.empty()) return std::nullopt;
    return Lite{v[0].address, v[0].size, v[0].mnemonic, v[0].op_str};
}

}  // namespace codebreaker::symbols::x86

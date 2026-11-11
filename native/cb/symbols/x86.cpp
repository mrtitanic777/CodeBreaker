// cb/symbols/x86.cpp -- x86 instructions as plain records (see x86.hpp).
#include "cb/symbols/x86.hpp"

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"

namespace cb::symbols::x86 {
namespace {

Insn to_insn(const cs_insn& c, bool detail) {
    Insn out;
    out.address = c.address;
    out.size = static_cast<std::uint8_t>(c.size);
    out.mnemonic = c.mnemonic;
    out.op_str = c.op_str;
    if (detail && c.detail != nullptr) {
        const cs_x86& x = c.detail->x86;
        out.disp = x.disp;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        if (x.op_count > 0 && x.operands[0].type == X86_OP_IMM) out.first_immediate = x.operands[0].imm;
    }
    return out;
}

void require_x86(const platforms::Decoder& decoder) {
    if (decoder.arch() != platforms::Arch::X86) throw UsageError("symbols::x86 needs an x86 decoder");
}

}  // namespace

std::vector<Insn> decode(const platforms::Decoder& decoder, ByteView code, std::uint64_t address, std::size_t count) {
    require_x86(decoder);
    std::vector<Insn> out;
    decoder.each(code, address, count, [&](const cs_insn& c) {
        out.push_back(to_insn(c, decoder.detail()));
        return true;
    });
    return out;
}

std::optional<Insn> decode_first(const platforms::Decoder& decoder, ByteView code, std::uint64_t address) {
    std::vector<Insn> first = decode(decoder, code, address, 1);
    if (first.empty()) return std::nullopt;
    return std::move(first.front());
}

}  // namespace cb::symbols::x86

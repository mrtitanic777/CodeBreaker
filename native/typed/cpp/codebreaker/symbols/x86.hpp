// codebreaker/symbols/x86.hpp -- compat shim for cb/symbols/x86.hpp (docs/CPP_DESIGN.md 6.3): platforms.decoder(bits, "x86",
// detail), as the symbols modules use it (api_arguments, runtime_library, names). Header-only; the decoders are the calling
// thread's (cb::platforms::DecoderSet). Deleted with its last old-layer consumer.
//
// The capstone.dll is the one beside the executable (docs/CPP_DESIGN.md section 9, decision 1): set_capstone_dll() is
// accepted and ignored, as the old loader's override is.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/symbols/x86.hpp"
#include "py.hpp"

namespace codebreaker::symbols::x86 {

inline void set_capstone_dll(const py::str&) {}

// One decoded instruction (capstone's CsInsn; disp/imm fields only for a decoder made with detail=true).
struct Insn {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
    int64_t disp = 0, disp_size = 0, disp_offset = 0;
    int64_t imm_size = 0, imm_offset = 0;
    bool first_is_imm = false;      // operands[0].type == X86_OP_IMM
    int64_t first_imm = 0;          // operands[0].imm
};

// md.disasm_lite(code, address): (address, size, mnemonic, op_str) of the instructions
struct Lite {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
};

namespace detail {
inline const cb::platforms::Decoder& decoder(int bits, bool detail) {
    thread_local cb::platforms::DecoderSet decoders;
    return decoders.get(cb::platforms::Arch::X86, static_cast<unsigned>(bits), detail);
}
inline cb::ByteView view(const std::string& code) { return cb::as_bytes(code); }
inline Insn to_old(const cb::symbols::x86::Insn& i) {
    Insn out;
    out.address = static_cast<int64_t>(i.address);
    out.size = i.size;
    out.mnemonic = i.mnemonic;
    out.op_str = i.op_str;
    out.disp = i.disp;
    out.disp_size = i.disp_size;
    out.disp_offset = i.disp_offset;
    out.imm_size = i.imm_size;
    out.imm_offset = i.imm_offset;
    out.first_is_imm = i.first_immediate.has_value();
    out.first_imm = i.first_immediate.value_or(0);
    return out;
}
}  // namespace detail

// md.disasm(code, address[, count]) (count 0: to the end); `bits` 16 or 32
inline std::vector<Insn> disasm(int bits, bool detail, const std::string& code, int64_t address, size_t count = 0) {
    std::vector<Insn> out;
    for (const auto& i : cb::symbols::x86::decode(detail::decoder(bits, detail), detail::view(code), static_cast<std::uint64_t>(address), count)) {
        out.push_back(detail::to_old(i));
    }
    return out;
}

inline std::vector<Lite> disasm_lite(int bits, const std::string& code, int64_t address) {
    std::vector<Lite> out;
    for (const Insn& i : disasm(bits, false, code, address)) out.push_back({i.address, i.size, i.mnemonic, i.op_str});
    return out;
}

// the first instruction only: `next(md.disasm_lite(code, address), None)`
inline std::optional<Lite> first_lite(int bits, const std::string& code, int64_t address) {
    const auto first = cb::symbols::x86::decode_first(detail::decoder(bits, false), detail::view(code), static_cast<std::uint64_t>(address));
    if (!first) return std::nullopt;
    return Lite{static_cast<int64_t>(first->address), first->size, first->mnemonic, first->op_str};
}

}  // namespace codebreaker::symbols::x86

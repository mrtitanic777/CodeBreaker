// cb/symbols/x86.hpp -- x86 instructions as plain records, for the symbols modules that read code (api_arguments,
// runtime_library, names): the mnemonic and operands as text, and where a displacement or an immediate sits inside the
// instruction's bytes.
//
//     platforms::DecoderSet decoders;                                     // one per thread
//     const platforms::Decoder& cs = decoders.get(platforms::Arch::X86, 32, /*detail=*/true);
//     for (const x86::Insn& i : x86::decode(cs, code, 0x1000)) { ... i.mnemonic, i.disp_offset, i.imm_size ... }
//     std::optional<x86::Insn> first = x86::decode_first(cs, code, 0x1000);
//
// Decoding is cb::platforms::Decoder (Capstone through the prebuilt capstone.dll); this module only turns what its
// records say into a value that does not hold Capstone's types. The displacement and immediate fields are filled only by
// a decoder opened with detail; a decoder without it gives them as zero. Decoding stops at the first byte that does
// not decode, as the decoder does.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::symbols::x86 {

struct Insn {
    std::uint64_t address = 0;
    std::uint8_t size = 0;
    std::string mnemonic;
    std::string op_str;
    // Where the operands' constants are encoded (detail only): the displacement of a memory operand, an immediate.
    std::int64_t disp = 0;
    std::uint8_t disp_size = 0;       // bytes of displacement in the encoding (0: none)
    std::uint8_t disp_offset = 0;     // where in the instruction's bytes
    std::uint8_t imm_size = 0;
    std::uint8_t imm_offset = 0;
    // The first operand is an immediate (a call or jump target, a push), and its value (detail only).
    std::optional<std::int64_t> first_immediate;
};

// The instructions from the start of `code` up to the first byte that does not decode; `count` 0: all of them. The
// decoder must be an x86 one (UsageError otherwise).
std::vector<Insn> decode(const platforms::Decoder& decoder, ByteView code, std::uint64_t address, std::size_t count = 0);

// The first instruction only; nullopt when the first byte does not decode.
std::optional<Insn> decode_first(const platforms::Decoder& decoder, ByteView code, std::uint64_t address);

}  // namespace cb::symbols::x86

// cb-unity: alone
// decompile/arm32_neon.cpp -- the Advanced SIMD and VFP instructions the interpreter runs beyond the rows of arm_ext (arm_ext.hpp: t2::decode_neon). Read from the ARM ARM (A7.7
// the structure loads and stores, A7.4.6 one register and a modified immediate, A8.8.340 VMOV immediate): an instruction is `Op::Ext` with `ext` = t2::kNeon and the ARM word it
// means (a Thumb one has its first halfword's 111x 1111 / 1111 1001 turned to the ARM 1111 001x / 1111 0100) in `Insn::imm`. A word of the family that is not listed here is not
// decoded: the interpreter stops at it as before.

#include <bit>

#include "decompile/arm32_neon_dp.hpp"
#include "decompile/arm_ext.hpp"

namespace cb::decompile::arm::t2 {

namespace {

constexpr std::uint32_t bits(std::uint32_t w, unsigned lsb, unsigned width) { return (w >> lsb) & ((1u << width) - 1u); }

}  // namespace

std::uint32_t neon_arm_word(std::uint16_t a, std::uint16_t b) {
    const std::uint32_t low = (static_cast<std::uint32_t>(a) & 0xFFu) << 16 | b;
    switch (a >> 8) {
        case 0xEF: return 0xF2000000u | low;   // Advanced SIMD data processing, U = 0
        case 0xFF: return 0xF3000000u | low;   // ... U = 1
        case 0xF9: return 0xF4000000u | low;   // element and structure loads and stores
        default:   // the coprocessor space: the ARM word with its condition (1110 or 1111); any other first halfword is no Advanced SIMD or VFP instruction
            return (a >> 12) >= 0xE && ((a >> 8) & 0xF) >= 0xC ? static_cast<std::uint32_t>(a) << 16 | b : 0u;
    }
}

std::optional<NeonTransfer> neon_transfer(std::uint32_t w) {
    if ((w >> 24) != 0xF4 || bits(w, 20, 1) != 0) return std::nullopt;   // (bit 20 set is a preload hint, and in Thumb a load of one register)
    NeonTransfer t;
    t.load = bits(w, 21, 1) != 0;
    t.rn = bits(w, 16, 4);
    t.rm = bits(w, 0, 4);
    t.d = bits(w, 22, 1) << 4 | bits(w, 12, 4);
    if (t.rn == 15) return std::nullopt;   // (the pc as the base: UNPREDICTABLE)
    if (bits(w, 23, 1) == 0) {   // multiple structures
        const unsigned type = bits(w, 8, 4), size = bits(w, 6, 2);
        t.kind = NeonTransfer::Multiple;
        t.ebytes = 1u << size;
        switch (type) {
            case 7: t.regs = 1; t.selem = 1; break;
            case 10: t.regs = 2; t.selem = 1; break;
            case 6: t.regs = 3; t.selem = 1; break;
            case 2: t.regs = 4; t.selem = 1; break;
            case 8: t.regs = 1; t.selem = 2; t.inc = 1; break;
            case 9: t.regs = 1; t.selem = 2; t.inc = 2; break;
            case 3: t.regs = 2; t.selem = 2; t.inc = 2; break;
            case 4: t.regs = 1; t.selem = 3; t.inc = 1; break;
            case 5: t.regs = 1; t.selem = 3; t.inc = 2; break;
            case 0: t.regs = 1; t.selem = 4; t.inc = 1; break;
            case 1: t.regs = 1; t.selem = 4; t.inc = 2; break;
            default: return std::nullopt;
        }
        if (t.selem > 1 && size == 3) return std::nullopt;
        if (t.d + (t.regs - 1) + (t.selem - 1) * t.inc > 31) return std::nullopt;
        t.bytes = 8 * t.regs * t.selem;
        return t;
    }
    const unsigned size = bits(w, 10, 2), n = bits(w, 8, 2);
    t.selem = n + 1;
    if (size == 3) {   // all lanes (a load only)
        if (!t.load) return std::nullopt;
        t.kind = NeonTransfer::AllLanes;
        if (bits(w, 6, 2) == 3 && n != 3) return std::nullopt;
        t.ebytes = 1u << bits(w, 6, 2);
        const unsigned spaced = bits(w, 5, 1);
        if (n == 0) {   // VLD1: one register, or two with T
            t.regs = spaced ? 2 : 1;
            t.inc = 1;
        } else {
            t.regs = 1;
            t.inc = spaced ? 2 : 1;
        }
        if (n == 3 && bits(w, 6, 2) == 3) t.ebytes = 4;   // (VLD4.32 with the alignment bit: 16-byte alignment, 4-byte elements)
        const unsigned count = n == 0 ? t.regs : t.selem;
        if (t.d + (count - 1) * t.inc > 31) return std::nullopt;
        t.bytes = t.ebytes * (n == 0 ? 1 : t.selem);
        if (n == 0) t.selem = 1;
        return t;
    }
    t.kind = NeonTransfer::Lane;
    t.ebytes = 1u << size;
    t.regs = 1;
    t.inc = 1;
    switch (size) {
        case 0: t.lane = bits(w, 5, 3); break;
        case 1: t.lane = bits(w, 6, 2); t.inc = bits(w, 5, 1) ? 2 : 1; break;
        default: t.lane = bits(w, 7, 1); t.inc = bits(w, 6, 1) ? 2 : 1; break;
    }
    if (n == 0 && t.inc == 2) return std::nullopt;   // (VLD1 and VST1 have one register)
    if (n == 0) t.inc = 1;
    if (t.d + (t.selem - 1) * t.inc > 31) return std::nullopt;
    t.bytes = t.selem * t.ebytes;
    return t;
}

// VFPExpandImm and AdvSIMDExpandImm are the exec's: here only whether the encoding is one of ours.
bool neon_modified_immediate(std::uint32_t w) {
    if ((w >> 24) != 0xF2 && (w >> 24) != 0xF3) return false;
    if (bits(w, 23, 1) != 1 || bits(w, 19, 3) != 0 || bits(w, 7, 1) != 0 || bits(w, 4, 1) != 1) return false;
    const unsigned cmode = bits(w, 8, 4), op = bits(w, 5, 1), q = bits(w, 6, 1);
    if (q && (bits(w, 12, 1) != 0)) return false;   // (a quadword register has an even number)
    if (cmode == 15 && op == 1) return false;
    return true;
}

bool vfp_move_immediate(std::uint32_t w) {
    if ((w & 0x0FB00EF0) != 0x0EB00A00) return false;
    return true;
}

std::optional<Insn> decode_neon(std::uint32_t w, bool thumb, std::uint32_t pc, std::uint32_t word_as_held, Arch arch) {
    if (arch < Arch::V7A) return std::nullopt;
    const bool transfer = neon_transfer(w).has_value();
    const bool immediate = neon_modified_immediate(w);
    const bool vfp = vfp_move_immediate(w) && (thumb ? (w >> 28) == 14 : (w >> 28) != 15);
    const std::optional<NeonDp> dp = neon_dp(w, thumb);
    if (!transfer && !immediate && !vfp && !dp) return std::nullopt;
    Insn i;
    i.word = word_as_held;
    i.pc = pc & ~1u;
    i.isa = thumb ? Isa::Thumb : Isa::Arm;
    i.length = 4;
    i.cond = (vfp || (dp && dp->cond != 14)) && !thumb ? static_cast<std::uint8_t>(w >> 28) : static_cast<std::uint8_t>(14);
    i.op = Op::Ext;
    i.ext = kNeon;
    i.imm = w;
    i.v6 = true;
    for (unsigned k = 0; k < 4; ++k) i.raw[k] = static_cast<std::uint8_t>(word_as_held >> (8 * k));
    if (vfp && thumb) i.cond = 14;
    return i;
}

}  // namespace cb::decompile::arm::t2

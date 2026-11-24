// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_cpu_a64_stops.cpp -- see pcode_cpu_a64_stops.hpp. Each rule is one place where the lane's first interpreter said "not run"; the key of a rule is the instruction's
// own fields (the opcode, U, the size and the like), written as the manual's encoding tables give them.
#include "engine/pcode_cpu_a64_stops.hpp"

#include <algorithm>
#include <bit>
#include <format>
#include <initializer_list>

namespace cb::engine::pcode_cpu::a64stops {

namespace da = cb::decompile::a64;

namespace {

using u32 = std::uint32_t;

bool in(unsigned key, std::initializer_list<unsigned> keys) { return std::find(keys.begin(), keys.end(), key) != keys.end(); }

struct Word {
    u32 w;
    unsigned bits(unsigned pos, unsigned n) const { return (w >> pos) & ((1u << n) - 1); }
};

// "simd: <text> is not run (<why>)"
std::string simd(const da::Insn& insn, const char* why) { return std::format("simd: {} is not run ({})", da::text(insn), why); }
// "<what>: <mnemonic> (<text>)"
std::string plain(const da::Insn& insn, const char* what) { return std::format("{}: {} ({})", what, std::string(insn.mnemonic), da::text(insn)); }

constexpr const char* kHalf = "half-precision arithmetic is not run";

std::optional<std::string> simd_stop(const da::Insn& insn, const Word f) {
    const unsigned q = f.bits(30, 1), u = f.bits(29, 1), size = f.bits(22, 2), fp_size = f.bits(22, 1), a = f.bits(23, 1);
    switch (insn.op) {
        case da::Op::SimdCopy: case da::Op::SimdScalarCopy: {
            const unsigned imm5 = f.bits(16, 5);
            if (imm5 == 0 || static_cast<unsigned>(std::countr_zero(imm5)) > 3) return simd(insn, "reserved");
            if (insn.op == da::Op::SimdCopy && u == 0 && !in(f.bits(11, 4), {0, 1, 3, 5, 7})) return simd(insn, "reserved");
            return std::nullopt;
        }
        case da::Op::SimdThree: {
            const unsigned opcode = f.bits(11, 5);
            if (opcode == 3) return std::nullopt;
            if (opcode >= 24) {
                if (fp_size && !q) return simd(insn, "reserved");
                const unsigned key = (u << 6) | (a << 5) | opcode;
                if (in(key, {24, 32 + 24, 25, 32 + 25, 26, 32 + 26, 27, 64 + 27, 64 + 26, 64 + 31, 64 + 32 + 26, 30, 32 + 30, 64 + 30, 64 + 32 + 30, 64 + 24, 64 + 32 + 24, 28, 64 + 28,
                            64 + 32 + 28, 64 + 29, 64 + 32 + 29}))
                    return std::nullopt;
                return simd(insn, "floating-point operation");
            }
            const unsigned key = (u << 5) | opcode;
            if ((key == 22 || key == 54) && size == 3) return simd(insn, "64-bit multiply");
            if (in(key, {16, 48, 19, 18, 50, 49, 17, 6, 7, 38, 39, 12, 13, 44, 45, 14, 15, 46, 47, 0, 32, 2, 34, 4, 36, 1, 5, 33, 37, 8, 40, 22, 54, 51, 23, 20, 21, 52, 53}))
                return std::nullopt;
            return simd(insn, "integer operation");
        }
        case da::Op::SimdThreeDiff: {
            const unsigned opcode = f.bits(12, 4);
            if (size > 2) return simd(insn, "reserved");
            if (opcode == 14) return u || size != 0 ? std::optional<std::string>(simd(insn, "polynomial multiply")) : std::nullopt;
            if (in(opcode, {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12})) return std::nullopt;
            return simd(insn, "long operation");
        }
        case da::Op::SimdTwoMisc: {
            const unsigned opcode = f.bits(12, 5);
            if (opcode >= 24 || (opcode >= 12 && opcode <= 15)) {
                if (fp_size && !q) return simd(insn, "reserved");
                const unsigned key = (u << 6) | (a << 5) | opcode;
                if (in(key, {24, 25, 32 + 24, 32 + 25, 64 + 24, 64 + 25, 64 + 32 + 25, 26, 27, 28, 32 + 26, 32 + 27, 64 + 26, 64 + 27, 64 + 28, 64 + 32 + 26, 64 + 32 + 27, 29, 64 + 29,
                            32 + 15, 64 + 32 + 15, 64 + 32 + 31, 32 + 12, 32 + 13, 32 + 14, 64 + 32 + 12, 64 + 32 + 13}))
                    return std::nullopt;
                return simd(insn, "floating-point operation");
            }
            switch (opcode) {
                case 0: case 1: {
                    const unsigned container = opcode == 1 ? 2 : (u ? 4 : 8);
                    return container <= (1u << size) ? std::optional<std::string>(simd(insn, "reserved")) : std::nullopt;
                }
                case 2: case 6: case 18: case 20: case 19: return size > 2 ? std::optional<std::string>(simd(insn, "reserved")) : std::nullopt;
                case 4: case 5: case 8: case 9: case 10: case 11: return std::nullopt;
                case 22: case 23: return u || !fp_size ? std::optional<std::string>(simd(insn, "half-precision conversion")) : std::nullopt;
                default: return simd(insn, "integer operation");
            }
        }
        case da::Op::SimdAcross: {
            const unsigned opcode = f.bits(12, 5);
            return in(opcode, {12, 15, 3, 27, 10, 26}) ? std::nullopt : std::optional<std::string>(simd(insn, "reduction"));
        }
        case da::Op::SimdPermute: return in(f.bits(12, 3), {1, 5, 2, 6, 3, 7}) ? std::nullopt : std::optional<std::string>(simd(insn, "reserved"));
        case da::Op::SimdShift: case da::Op::SimdScalarShift: {
            const unsigned immh = f.bits(19, 4), opcode = f.bits(11, 5);
            if (immh == 0) return simd(insn, "reserved");
            const unsigned sz = static_cast<unsigned>(std::bit_width(immh)) - 1, ebits = 8u << sz;
            const bool scalar = insn.op == da::Op::SimdScalarShift;
            if (in(opcode, {0, 2, 4, 6, 8, 10})) return std::nullopt;
            if (!scalar && (opcode == 12 || opcode == 14)) return ebits >= 64 ? std::optional<std::string>(simd(insn, "64-bit saturating shift")) : std::nullopt;
            if (!scalar && in(opcode, {16, 17, 18, 19, 20})) return sz > 2 ? std::optional<std::string>(simd(insn, "reserved")) : std::nullopt;
            if (opcode == 28 || opcode == 31) return sz < 2 ? std::optional<std::string>(simd(insn, "reserved")) : std::nullopt;
            return simd(insn, scalar ? "scalar shift" : "shift");
        }
        case da::Op::SimdByElem: {
            const unsigned opcode = f.bits(12, 4);
            if (in(opcode, {1, 5, 9})) return std::nullopt;
            if (in(opcode, {2, 6, 10})) return std::nullopt;
            if (in(opcode, {3, 7, 11})) return simd(insn, "saturating long multiply");
            if (in(opcode, {8, 0, 4, 12, 13})) return std::nullopt;
            return simd(insn, "by-element operation");
        }
        case da::Op::SimdScalarThree: {
            const unsigned opcode = f.bits(11, 5);
            if (opcode >= 24) {
                const unsigned key = (u << 6) | (a << 5) | opcode;
                if (in(key, {27, 64 + 32 + 26, 28, 64 + 28, 64 + 32 + 28, 64 + 29, 64 + 32 + 29})) return std::nullopt;
                return simd(insn, "floating-point operation");
            }
            return in((u << 5) | opcode, {16, 48, 49, 17, 6, 7, 38, 39, 8, 40}) ? std::nullopt : std::optional<std::string>(simd(insn, "scalar operation"));
        }
        case da::Op::SimdScalarTwoMisc: {
            const unsigned opcode = f.bits(12, 5);
            if (opcode >= 26 && opcode <= 29) {
                const unsigned key = (u << 6) | (a << 5) | opcode;
                if (in(key, {29, 64 + 29, 32 + 27, 64 + 32 + 27, 26, 64 + 26, 27, 64 + 27, 32 + 26, 64 + 32 + 26, 28, 64 + 28})) return std::nullopt;
                return simd(insn, "scalar conversion");
            }
            return in((u << 5) | opcode, {8, 9, 10, 40, 41, 11, 43}) ? std::nullopt : std::optional<std::string>(simd(insn, "scalar operation"));
        }
        case da::Op::SimdScalarPair: {
            if ((a == 0 && f.bits(12, 5) == 13) || f.bits(12, 5) == 27) return std::nullopt;
            return simd(insn, "scalar pairwise operation");
        }
        default: return std::nullopt;   // (the structure loads and stores, the modified immediates, EXT and TBL run whatever they hold)
    }
}

}  // namespace

std::optional<std::string> stop_reason(const da::Insn& insn) {
    const Word f{insn.word};
    const unsigned ftype = f.bits(22, 2);
    switch (insn.op) {
        case da::Op::LogicImm:
            if (!da::decode_bitmask(f.bits(22, 1), f.bits(10, 6), f.bits(16, 6), f.bits(31, 1) ? 64 : 32)) return plain(insn, "bad bitmask");
            return std::nullopt;
        case da::Op::Dp2: return in(f.bits(10, 6), {2, 3, 8, 9, 10, 11}) ? std::nullopt : std::optional<std::string>(plain(insn, "crc32 is not run"));
        case da::Op::MsrImm: return plain(insn, "msr pstate is not run");
        case da::Op::Mrs: {
            const std::string_view name = da::system_register_name(f.bits(5, 15));
            return name == "nzcv" || name == "fpcr" || name == "fpsr" || name == "tpidr_el0" || name == "tpidrro_el0" || name == "cntvct_el0" || name == "cntpct_el0" || name == "cntfrq_el0" ||
                           name == "dczid_el0" || name == "ctr_el0"
                       ? std::nullopt
                       : std::optional<std::string>(plain(insn, "system register not modelled"));
        }
        case da::Op::Msr: {
            const std::string_view name = da::system_register_name(f.bits(5, 15));
            return name == "nzcv" || name == "fpcr" || name == "fpsr" || name == "tpidr_el0" ? std::nullopt : std::optional<std::string>(plain(insn, "system register not modelled"));
        }
        case da::Op::LdStAtomic:   // (CASP of an odd register)
            if (f.bits(23, 1) == 0 && (((f.w >> 16) & 1) != 0 || (f.w & 1) != 0)) return std::format("undefined instruction {:08x}", f.w);
            return std::nullopt;
        case da::Op::Fp1Src: {
            const unsigned opcode = f.bits(15, 6);
            if (ftype == 3) return in(opcode, {0, 1, 2, 4, 5}) ? std::nullopt : std::optional<std::string>(plain(insn, kHalf));
            return in(opcode, {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14, 15}) ? std::nullopt : std::optional<std::string>(plain(insn, "floating-point operation not run"));
        }
        case da::Op::Fp2Src: case da::Op::Fp3Src: case da::Op::FpCmp: case da::Op::FpCCmp: case da::Op::FpFixed:
            return ftype == 3 ? std::optional<std::string>(plain(insn, kHalf)) : std::nullopt;
        case da::Op::FpInt: {
            const unsigned opcode = f.bits(16, 3);
            return ftype == 3 && opcode != 6 && opcode != 7 ? std::optional<std::string>(plain(insn, kHalf)) : std::nullopt;
        }
        case da::Op::Sysl: case da::Op::FlagOp: case da::Op::SimdScalarByElem: return plain(insn, "not run");
        default: return simd_stop(insn, f);
    }
}

}  // namespace cb::engine::pcode_cpu::a64stops

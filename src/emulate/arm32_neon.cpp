// cb-unity: alone
// emulate/arm32_neon.cpp -- the Advanced SIMD structure loads and stores, one register and a modified immediate, and the VFP's VMOV (immediate) (decompile/arm32_neon.cpp), run as
// the ARM ARM says (A7.7.1 VLDn / VSTn, A7.4.6 AdvSIMDExpandImm, A8.8.340 VFPExpandImm). A D register d is the pair of words vfp_[2d] (low), vfp_[2d+1].
#include <bit>

#include "decompile/arm_ext.hpp"
#include "emulate/arm.hpp"

namespace cb::emulate::arm {

namespace da = decompile::arm;
namespace t2 = da::t2;

namespace {

constexpr std::uint32_t bits(std::uint32_t w, unsigned lsb, unsigned width) { return (w >> lsb) & ((1u << width) - 1u); }

}  // namespace

std::optional<Fault> Cpu::execute_neon(const da::Insn& insn, std::uint32_t at) {
    const std::uint32_t w = insn.imm;
    if (const std::optional<t2::NeonDp> dp = t2::neon_dp(w, insn.thumb())) return execute_neon_dp(*dp, at);
    const auto byte_of = [&](unsigned d, unsigned k) -> std::uint8_t { return static_cast<std::uint8_t>(vfp_[2 * (d & 31) + (k >> 2)] >> (8 * (k & 3))); };
    const auto set_byte = [&](unsigned d, unsigned k, std::uint8_t v) {
        std::uint32_t& word = vfp_[2 * (d & 31) + (k >> 2)];
        word = (word & ~(0xFFu << (8 * (k & 3)))) | static_cast<std::uint32_t>(v) << (8 * (k & 3));
    };
    if (const std::optional<t2::NeonTransfer> t = t2::neon_transfer(w)) {
        const std::uint32_t address = r[t->rn];
        for (unsigned k = 0; k < t->bytes; ++k)   // (nothing is moved unless every byte is there)
            if (!bus_->mapped(address + k)) return Fault{Fault::Kind::BusError, at, address + k, {}, 0};
        std::uint32_t at_byte = address;
        const auto move = [&](unsigned d, unsigned element) {   // one element between memory and register d
            for (unsigned k = 0; k < t->ebytes; ++k, ++at_byte) {
                if (t->load) set_byte(d, element * t->ebytes + k, static_cast<std::uint8_t>(bus_->read(at_byte, 1)));
                else bus_->write(at_byte, 1, byte_of(d, element * t->ebytes + k));
            }
        };
        switch (t->kind) {
            case t2::NeonTransfer::Multiple: {
                const unsigned elements = 8 / t->ebytes;
                for (unsigned reg = 0; reg < t->regs; ++reg)
                    for (unsigned e = 0; e < elements; ++e)
                        for (unsigned i = 0; i < t->selem; ++i) move(t->d + reg + i * t->inc, e);
                break;
            }
            case t2::NeonTransfer::Lane:
                for (unsigned i = 0; i < t->selem; ++i) move(t->d + i * t->inc, t->lane);
                break;
            case t2::NeonTransfer::AllLanes: {
                const unsigned elements = 8 / t->ebytes;
                const unsigned count = t->selem == 1 ? t->regs : t->selem;
                for (unsigned i = 0; i < count; ++i) {
                    const unsigned reg = t->d + i * t->inc;
                    std::uint8_t value[8];
                    for (unsigned k = 0; k < t->ebytes; ++k) value[k] = static_cast<std::uint8_t>(bus_->read(at_byte + (t->selem == 1 ? 0 : i * t->ebytes) + k, 1));
                    if (t->selem == 1) {}   // (VLD1 loads one element and puts it in each register it names)
                    for (unsigned e = 0; e < elements; ++e)
                        for (unsigned k = 0; k < t->ebytes; ++k) set_byte(reg, e * t->ebytes + k, value[k]);
                }
                break;
            }
        }
        if (t->rm != 15) r[t->rn] = address + (t->rm == 13 ? t->bytes : r[t->rm]);
        return std::nullopt;
    }
    const bool vfp = (w & 0x0FB00EF0) == 0x0EB00A00;
    if (vfp) {   // VMOV (immediate): the 8 bits expand to a single or a double
        const std::uint32_t imm8 = bits(w, 16, 4) << 4 | bits(w, 0, 4);
        const std::uint32_t sign = imm8 >> 7, b6 = (imm8 >> 6) & 1, low = imm8 & 0x3F;
        if (bits(w, 8, 1) == 0) {
            vfp_[bits(w, 12, 4) << 1 | bits(w, 22, 1)] = sign << 31 | (b6 ^ 1) << 30 | (b6 ? 0x1Fu : 0u) << 25 | low << 19;
        } else {
            const std::uint64_t value = static_cast<std::uint64_t>(sign) << 63 | static_cast<std::uint64_t>(b6 ^ 1) << 62 | (b6 ? 0xFFull : 0ull) << 54 | static_cast<std::uint64_t>(low) << 48;
            const unsigned d = bits(w, 22, 1) << 4 | bits(w, 12, 4);
            vfp_[2 * d] = static_cast<std::uint32_t>(value);
            vfp_[2 * d + 1] = static_cast<std::uint32_t>(value >> 32);
        }
        return std::nullopt;
    }
    if (t2::neon_modified_immediate(w)) {
        const std::uint32_t imm8 = bits(w, 24, 1) << 7 | bits(w, 16, 3) << 4 | bits(w, 0, 4);
        const unsigned cmode = bits(w, 8, 4), op = bits(w, 5, 1);
        std::uint64_t imm = 0;
        const auto rep32 = [](std::uint32_t v) { return static_cast<std::uint64_t>(v) << 32 | v; };
        switch (cmode >> 1) {
            case 0: imm = rep32(imm8); break;
            case 1: imm = rep32(imm8 << 8); break;
            case 2: imm = rep32(imm8 << 16); break;
            case 3: imm = rep32(imm8 << 24); break;
            case 4: imm = rep32(imm8 | imm8 << 16); break;
            case 5: imm = rep32(imm8 << 8 | imm8 << 24); break;
            case 6: imm = (cmode & 1) ? rep32(imm8 << 16 | 0xFFFFu) : rep32(imm8 << 8 | 0xFFu); break;
            default:
                if (!(cmode & 1)) {
                    if (op == 0) {
                        imm = 0x0101010101010101ull * imm8;
                    } else {
                        for (unsigned k = 0; k < 8; ++k)
                            if ((imm8 >> k) & 1) imm |= 0xFFull << (8 * k);
                    }
                } else {
                    const std::uint32_t b6 = (imm8 >> 6) & 1;
                    imm = rep32((imm8 >> 7) << 31 | (b6 ^ 1) << 30 | (b6 ? 0x1Fu : 0u) << 25 | (imm8 & 0x3F) << 19);
                }
                break;
        }
        const bool is_orr_bic = (cmode >> 3) == 0 ? (cmode & 1) : ((cmode >> 2) == 2 ? (cmode & 1) : false);   // 0xx1 and 10x1: the logical forms
        const unsigned count = bits(w, 6, 1) ? 2 : 1, d = bits(w, 22, 1) << 4 | bits(w, 12, 4);
        for (unsigned k = 0; k < count; ++k) {
            std::uint64_t old = static_cast<std::uint64_t>(vfp_[2 * (d + k)]) | static_cast<std::uint64_t>(vfp_[2 * (d + k) + 1]) << 32;
            std::uint64_t result;
            if (is_orr_bic) result = op ? old & ~imm : old | imm;   // VBIC, VORR
            else result = op && cmode != 14 ? ~imm : imm;           // VMVN, VMOV
            vfp_[2 * (d + k)] = static_cast<std::uint32_t>(result);
            vfp_[2 * (d + k) + 1] = static_cast<std::uint32_t>(result >> 32);
        }
        return std::nullopt;
    }
    return Fault{Fault::Kind::NotRun, at, 0, "neon", 0};
}

}  // namespace cb::emulate::arm
